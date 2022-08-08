#include "db.h"


database *newDatabase(char *name){
	database *db = malloc(sizeof(database));
	memcpy(db->name, name, len(name)+1);
	db->lfiles = newLtable(0);
	db->ltags = newLtable(0);
	db->cfiles = newCtable(0);
	db->ctags = newCtable(0);
	db->hfiles = NULL;
	db->htags = NULL;
	db->map = newMtable(0);
	return db;
}

uint64_t addFile(database *db, char *file){
	uint32_t l;
	file = normalizeStrLimit(file, &l, MAXPATH-1);
	uint64_t h = crc64(0, file, l);
	uint64_t i = searchNode(db->hfiles, h);
	if(i == -1){
		insertLtable(db->lfiles, file);
		insertCtable(db->cfiles, 0);
		i = db->lfiles->size-1;
		db->hfiles = insertNode(db->hfiles, h, i);
	}
	return i;
}

uint64_t addTag(database *db, char *tag){
	uint32_t l;
	tag = normalizeStrLimit(tag, &l, MAXPATH-1);
	uint64_t h = crc64(0, tag, l);
	uint64_t i = searchNode(db->htags, h);
	if(i == -1){
		insertLtable(db->ltags, tag);
		insertCtable(db->ctags, 0);
		i = db->ltags->size-1;
		db->htags = insertNode(db->htags, h, i);
		
	}
	return i;
}

static void increaseCount(ctable *ct, uint64_t i){
	ct->table[i]++;
}

static void decreaseCount(ctable *ct, uint64_t i){
	ct->table[i]--;
}

static int addRelation(database *db, relation r){
	if(searchMtable(db->map, r) == UINTMAX_MAX){
		insertMtable(db->map, r);
		return 0;
	}
	return -1;
}

int addFileTag(database *db, char *file, char *tag){
	uint64_t fi = addFile(db, file), ti = addTag(db, tag);
	int r = addRelation(db, (relation){.file = fi, .tag = ti});
	if(r == 0){
		increaseCount(db->cfiles, fi);
		increaseCount(db->ctags, ti);
		return 0;
	}
	return -1;
}

int addFileTags(database *db, char *file, int ntags, ...){
	va_list tags;
	va_start(tags, ntags);
	for(uint64_t i = 0; i < ntags; ++i){
		char *tag = va_arg(tags, char*);
		addFileTag(db, file, tag);
	}
	va_end(tags);
	return 0;
}

int addTagFiles(database *db, char *tag, int nfiles, ...){
	va_list files;
	va_start(files, nfiles);
	for(uint64_t i = 0; i < nfiles; ++i){
		char *file = va_arg(files, char*);
		addFileTag(db, file, tag);
	}
	va_end(files);
	return 0;
}

// When removing the file from the ltable and ctable we change the indexes of the tags in front
// of it. Thus, we must change their indexes on the avl tree and mapping table. To do this we
// simply get all tags with an index higher than the tag we removed and substract one from it,
// since when removing a tag from the tables all we did was shift down all the tags in front of
// it one position
static void decreaseHigherIndexNode(node *n, uint64_t i){
	if(n == NULL){
		return;
	}
	if(n->i > i){
		n->i--;
	}
	decreaseHigherIndexNode(n->left, i);
	decreaseHigherIndexNode(n->right, i);
}

static void decreaseHigherFileIndexMap(mtable *mt, uint64_t i){
	for(uint64_t j = 0; j < mt->size; ++j){
		if(mt->table[j].file > i){
			mt->table[j].file--;
		}
	}
}

static void decreaseHigherTagIndexMap(mtable *mt, uint64_t i){
	for(uint64_t j = 0; j < mt->size; ++j){
		if(mt->table[j].tag > i){
			mt->table[j].tag--;
		}
	}
}

int removeFile(database *db, char *file){
	uint32_t l;
	file = normalizeStrLimit(file, &l, MAXPATH-1);
	uint64_t i = searchLtable(db->lfiles, file);
	if(i == -1){
		return -1;
	}
	uint64_t *r, rl;
	searchFile(db, file, 0, &r, &rl);
	for(uint64_t j = 0; j < rl; ++j){
		decreaseCount(db->ctags, r[j]);
	}
	uint64_t h = crc64(0, file, l);
	removeLtable(db->lfiles, file);
	removeCtable(db->cfiles, i);
	removeNode(db->hfiles, h);
	removeFileMtable(db->map, i);
	
	decreaseHigherIndexNode(db->hfiles, i);
	decreaseHigherFileIndexMap(db->map, i);
	return 0;
}

int removeTag(database *db, char *tag){
	uint32_t l;
	tag = normalizeStrLimit(tag, &l, MAXPATH-1);
	uint64_t i = searchLtable(db->ltags, tag);
	if(i == -1){
		return -1;
	}
	uint64_t *r, rl;
	searchTag(db, tag, 0, &r, &rl);
	for(uint64_t j = 0; j < rl; ++j){
		decreaseCount(db->cfiles, r[j]);
	}
	uint64_t h = crc64(0, tag, l);
	removeLtable(db->ltags, tag);
	removeCtable(db->ctags, i);
	removeNode(db->htags, h);
	removeTagMtable(db->map, i);

	decreaseHigherIndexNode(db->htags, i);
	decreaseHigherTagIndexMap(db->map, i);
	return 0;
}

int searchFile(database *db, char *file, uint64_t n, uint64_t **r, uint64_t *rl){
	uint32_t l;
	file = normalizeStrLimit(file, &l, MAXPATH-1);
	uint64_t h = crc64(0, file, l);
	uint64_t fi = searchNode(db->hfiles, h);
	if(fi == -1){
		return -1;
	}
	
	*rl = 0;
	for(uint64_t i = 0; i < db->map->size; ++i){
		if(n < 1 || *rl < n){
			if(db->map->table[i].file == fi){
				++(*rl);
			}
		}
	}
	*r = malloc((*rl)*sizeof(uint64_t));
	uint64_t c = 0;
	for(uint64_t i = 0; i < db->map->size && c < *rl; ++i){
		if(db->map->table[i].file == fi){
			(*r)[c++] = db->map->table[i].tag;
		}
	}
	return 0;
}

int searchTag(database *db, char *tag, uint64_t n, uint64_t **r, uint64_t *rl){
	uint32_t l;
	tag = normalizeStrLimit(tag, &l, MAXPATH-1);
	uint64_t h = crc64(0, tag, l);
	uint64_t ti = searchNode(db->htags, h);
	if(ti == -1){
		return -1;
	}
	
	*rl = 0;
	for(uint64_t i = 0; i < db->map->size; ++i){
		if(n < 1 || *rl < n){
			if(db->map->table[i].tag == ti){
				++(*rl);
			}
		}
	}
	*r = malloc((*rl)*sizeof(uint64_t));
	uint64_t c = 0;
	for(uint64_t i = 0; i < db->map->size && c < *rl; ++i){
		if(db->map->table[i].tag == ti){
			(*r)[c++] = db->map->table[i].file;
		}
	}
	return 0;
}

int storeDatabase(database *db, const char *path){
	FILE *fp = fopen(path, "wb");

	char header[2] = "DB";
	fwrite(header, sizeof(char), 2, fp);
	fwrite(db->name, sizeof(char), 32, fp);
	
	storeLtable(db->lfiles, fp);
	storeLtable(db->ltags, fp);
	storeCtable(db->cfiles, fp);
	storeCtable(db->ctags, fp);
	storeAVLTree(db->hfiles, fp);
	storeAVLTree(db->htags, fp);
	storeMtable(db->map, fp);
	
	char end[3] = "END";
	fwrite(end, sizeof(char), 3, fp);
	fclose(fp);
	return 0;
}

database *loadDatabase(const char* path){
	FILE *fp = fopen(path, "rb");
	char *header = calloc(2, sizeof(char));
	fread(header, sizeof(char), 2, fp);
	if(!sameStr(header, "DB")){
		fprintf(stderr, "Header is '%s' not 'DB'\n", header);
	}
	
	char name[32];
	fread(&name, sizeof(char), 32, fp);
	database *db = newDatabase(name);
	db->lfiles = loadLtable(fp);
	db->ltags = loadLtable(fp);
	db->cfiles = loadCtable(fp);
	db->ctags = loadCtable(fp);
	db->hfiles = loadAVLTree(fp);
	db->htags = loadAVLTree(fp);
	db->map = loadMtable(fp);
	
	char *end = calloc(3, sizeof(char));
	fread(end, sizeof(char), 3, fp);
	if(!sameStr(end, "END")){
		fprintf(stderr, "End is '%s' not 'END'\n", end);
	}
	fclose(fp);
	return db;
}

void printDatabase(database *db){
	for(uint64_t i = 0; i < db->map->size; ++i){
		printf("%s -> %s\n", db->lfiles->table[db->map->table[i].file], db->ltags->table[db->map->table[i].tag]);
	}
	printf("\n");
}

void debugAVLtree(node *n){
	if(n != NULL){
		printf("\t\t+%" PRIu64 " -> %" PRIu64 "\n", n->h, n->i);
		debugAVLtree(n->left);
		debugAVLtree(n->right);
	}
}

void debugDatabase(database *db){
	printf("\n");
	printf("Name: %s\n", db->name);
	printf("\t-lfiles: %d\n", db->lfiles->size);
	for(uint64_t i = 0; i < db->lfiles->size; ++i){
		printf("\t\t+[%" PRIu64 "] %s (%" PRIu64 ")\n", i, db->lfiles->table[i], db->cfiles->table[i]);
	}
	printf("\t-ltags: %d\n", db->ltags->size);
	for(uint64_t i = 0; i < db->ltags->size; ++i){
		printf("\t\t+[%" PRIu64 "] %s (%" PRIu64 ")\n", i, db->ltags->table[i], db->ctags->table[i]);
	}
	printf("\t-hfiles: %d\n", db->lfiles->size);
	debugAVLtree(db->hfiles);
	printf("\t-htags: %d\n", db->ltags->size);
	debugAVLtree(db->htags);
	printf("\t-map: %d\n", db->map->size);
	for(uint64_t i = 0; i < db->map->size; ++i){
		printf("\t\t+[%" PRIu64 "] %" PRIu64 ":%" PRIu64 "\n", i, db->map->table[i].file, db->map->table[i].tag);
	}
	printf("\n");
}
