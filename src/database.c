#include "db.h"


database *newDatabase(char *name){
	database *db = malloc(sizeof(database));
	memcpy(db->name, name, len(name)+1);
	db->lfiles = newLtable(0);
	db->ltags = newLtable(0);
	db->hfiles = newHtable(0);
	db->htags = newHtable(0);
	db->fcount = newHtable(0);
	db->tcount = newHtable(0);
	db->map = newMtable(0);
	return db;
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
	db->hfiles = loadHtable(fp);
	db->htags = loadHtable(fp);
	db->fcount = loadHtable(fp);
	db->tcount = loadHtable(fp);
	db->map = loadMtable(fp);
	
	char *end = calloc(3, sizeof(char));
	fread(end, sizeof(char), 3, fp);
	if(!sameStr(end, "END")){
		fprintf(stderr, "End is '%s' not 'END'\n", end);
	}
	fclose(fp);
	return db;
}

int storeDatabase(database *db, const char *path){
	FILE *fp = fopen(path, "wb");

	char header[2] = "DB";
	fwrite(header, sizeof(char), 2, fp);
	fwrite(db->name, sizeof(char), 32, fp);
	
	storeLtable(db->lfiles, fp);
	storeLtable(db->ltags, fp);
	storeHtable(db->hfiles, fp);
	storeHtable(db->htags, fp);
	storeHtable(db->fcount, fp);
	storeHtable(db->tcount, fp);
	storeMtable(db->map, fp);
	
	char end[3] = "END";
	fwrite(end, sizeof(char), 3, fp);
	
	fclose(fp);
	return 0;
}

static void increaseCount(htable *ht, uint64_t i){
	ht->table[i]++;
}

uint64_t addFile(database *db, char *file){
	uint32_t l;
	file = normalizeStrLimit(file, &l, MAXPATH-1);
	uint64_t h = crc64(0, file, l);
	uint64_t i = htableSearch(db->hfiles, h);
	
	if(i == -1){
		ltableAdd(db->lfiles, file);
		htableAdd(db->hfiles, h);
		htableAdd(db->fcount, 0);
		i = db->hfiles->size-1;
	}
	increaseCount(db->fcount, i);
	
	return i;
}

uint64_t addTag(database *db, char *tag){
	uint32_t l;
	tag = normalizeStrLimit(tag, &l, MAXPATH-1);
	uint64_t h = crc64(0, tag, l);
	uint64_t i = htableSearch(db->htags, h);
	
	if(i == -1){
		ltableAdd(db->ltags, tag);
		htableAdd(db->htags, h);
		htableAdd(db->tcount, 0);
		i = db->htags->size-1;
	}
	increaseCount(db->tcount, i);
	
	return i;
}

static int addRelation(database *db, relation r){
	if(mtableSearch(db->map, r) != -1){
		return -1;
	}
	mtableAdd(db->map, r);

	return 0;
}

int addFileTag(database *db, char *file, char *tag){
	uint64_t fi = addFile(db, file), ti = addTag(db, tag);
	addRelation(db, (relation){.file = fi, .tag = ti});
	
	return 0;
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

// Stores in r a list with the indexes of the first n files that have this tag
// If n is 0 or lower, it returns all of them. Stores in rl the length of r
int searchTag(database *db, char *tag, uint64_t n, uint64_t **r, uint64_t *rl){
	uint32_t l;
	tag = normalizeStrLimit(tag, &l, MAXPATH-1);
	uint64_t h = crc64(0, tag, l);
	uint64_t ti = htableSearch(db->htags, h);
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

// Stores in r a list with the indexes of the first n tags that this file has
// If n is 0 or lower, it returns all of them. Stores in rl the length of r
int searchFile(database *db, char *file, uint64_t n, uint64_t **r, uint64_t *rl){
	uint32_t l;
	file = normalizeStrLimit(file, &l, MAXPATH-1);
	uint64_t h = crc64(0, file, l);
	uint64_t fi = htableSearch(db->hfiles, h);
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

void printDatabase(database *db){
	for(uint64_t i = 0; i < db->map->size; ++i){
		printf("%s -> %s\n", db->lfiles->table[db->map->table[i].file], db->ltags->table[db->map->table[i].tag]);
	}
	printf("\n");
}

void debugDatabase(database *db){
	printf("\n");
	printf("Name: %s\n", db->name);
	printf("\t-lfiles: %d\n", db->lfiles->size);
	for(uint64_t i = 0; i < db->lfiles->size; ++i){
		printf("\t\t+%s (%" PRIu64 ")\n", db->lfiles->table[i], db->fcount->table[i]);
	}
	printf("\t-ltags: %d\n", db->ltags->size);
	for(uint64_t i = 0; i < db->ltags->size; ++i){
		printf("\t\t+%s (%" PRIu64 ")\n", db->ltags->table[i], db->tcount->table[i]);
	}
	printf("\t-hfiles: %d\n", db->hfiles->size);
	for(uint64_t i = 0; i < db->hfiles->size; ++i){
		printf("\t\t+%" PRIu64 "\n", db->hfiles->table[i]);
	}
	printf("\t-htags: %d\n", db->htags->size);
	for(uint64_t i = 0; i < db->htags->size; ++i){
		printf("\t\t+%" PRIu64 "\n", db->htags->table[i]);
	}
	printf("\t-map: %d\n", db->map->size);
	for(uint64_t i = 0; i < db->map->size; ++i){
		printf("\t\t+%" PRIu64 ":%" PRIu64 "\n", db->map->table[i].file, db->map->table[i].tag);
	}
	printf("\n");
}
