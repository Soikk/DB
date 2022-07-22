#include "db.h"


database *newDatabase(char *name){
	database *db = malloc(sizeof(database));
	memcpy(db->name, name, len(name)+1);
	db->lfiles = newLtable(0);
	db->ltags = newLtable(0);
	db->hfiles = newHtable(0);
	db->htags = newHtable(0);
	db->map = newMtable(0);
	return db;
}

database *loadDatabase(const char* path){
	FILE *fp = fopen(path, "rb");
	char *header = calloc(2, sizeof(char));
	fread(header, sizeof(char), 2, fp);
	if(!sameStr(header, "DB")){
		printf("header is %c%c and not DB\n", header[0], header[1]);
	}
	char name[32];
	fread(&name, sizeof(char), 32, fp);
	database *db = newDatabase(name);
	db->lfiles = loadLtable(fp);
	db->ltags = loadLtable(fp);
	db->hfiles = loadHtable(fp);
	db->htags = loadHtable(fp);
	db->map = loadMtable(fp);
	char end[4];
	fread(&end, sizeof(char), 3, fp);
	if(!sameStr(end, "END")){
		printf("end is %s and not END\n", end);
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
	storeMtable(db->map, fp);
	
	char end[3] = "END";
	fwrite(end, sizeof(char), 3, fp);
	
	fclose(fp);
}

static int addRelation(database *db, relation r){
	if(mtableSearch(db->map, r) != -1){
		return -1;
	}
	mtableAdd(db->map, r);

	return 0;
}

int addFileTag(database *db, char *file, char *tag){
	uint32_t lf, lt;
	file = normalizeStrLimit(file, &lf, MAXPATH-1);
	tag = normalizeStrLimit(tag, &lt, MAXPATH-1);
	uint64_t hf = crc64(0, file, lf), ht = crc64(0, tag, lt);
	uint64_t fi = htableSearch(db->hfiles, hf), ti = htableSearch(db->htags, ht);
	
	if(fi == -1){
		ltableAdd(db->lfiles, file);
		htableAdd(db->hfiles, hf);
		fi = db->hfiles->size-1;
	}
	if(ti == -1){
		ltableAdd(db->ltags, tag);
		htableAdd(db->htags, ht);
		ti = db->htags->size-1;
	}
	
	addRelation(db, (relation){.file = fi, .tag = ti});
	return 0;
}

int addFileTags(database *db, char *file, int ntags, ...){
	uint32_t lf;
	file = normalizeStrLimit(file, &lf, MAXPATH-1);
	uint64_t hf = crc64(0, file, lf);
	uint64_t fi = htableSearch(db->hfiles, hf);
	
	if(fi == -1){
		ltableAdd(db->lfiles, file);
		htableAdd(db->hfiles, hf);
		fi = db->hfiles->size-1;
	}
	
	va_list tags;
	va_start(tags, ntags);
	for(uint64_t i = 0; i < ntags; ++i){
		char *tag = va_arg(tags, char*);
		uint32_t lt;
		tag = normalizeStrLimit(tag, &lt, MAXPATH-1);
		uint64_t ht = crc64(0, tag, lt);
		uint64_t ti = htableSearch(db->htags, ht);
		
		if(ti == -1){
			ltableAdd(db->ltags, tag);
			htableAdd(db->htags, ht);
			ti = db->htags->size-1;
		}
		
		addRelation(db, (relation){.file = fi, .tag = ti});
	}
	va_end(tags);

	return 0;
}

// Should return a list with the indexes of the files that have this tag
int searchTag(database *db, char *tag, uint64_t *rl){
	uint32_t l;
	tag = normalizeStrLimit(tag, &l, MAXPATH-1);
	uint64_t h = crc64(0, tag, l);
	uint64_t ti = htableSearch(db->htags, h);
	// TODO: error checking
	
	uint64_t c = 0;
	for(uint64_t i = 0; i < db->map->size; ++i){
		if(db->map->table[i].tag == ti){
			++c;
		}
	}
	uint64_t *r = malloc(c*sizeof(uint64_t));
	c = 0;
	for(uint64_t i = 0; i < db->map->size; ++i){
		if(db->map->table[i].tag == ti){
			r[c++] = db->map->table[i].file;
		}
	}
	rl = r;
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
		printf("\t\t+%s\n", db->lfiles->table[i]);
	}
	printf("\t-ltags: %d\n", db->ltags->size);
	for(uint64_t i = 0; i < db->ltags->size; ++i){
		printf("\t\t+%s\n", db->ltags->table[i]);
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
