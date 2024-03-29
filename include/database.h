#ifndef DATABASE_H
#define DATABASE_H

#include "db.h"


typedef struct database{
	char name[32];
	ltable *lfiles, *ltags;
	ctable *cfiles, *ctags;
	tree hfiles, htags;
	mtable *map;
} database;


database *newDatabase(char *name);

int freeDatabase(database **db);

int deleteDatabase(database **db);

uint64_t addFile(database *db, char *file);

uint64_t addTag(database *db, char *tag);

int removeFile(database *db, char *file);

int removeTag(database *db, char *tag);

int addFileTag(database *db, char *file, char *tag);

int addFileTags(database *db, char *file, int ntags, ...);

int addTagFiles(database *db, char *tag, int nfiles, ...);

int removeFileTag(database *db, char *file, char *tag);

int searchFile(database *db, char *file, uint64_t n, uint64_t **r, uint64_t *rl);

int searchTag(database *db, char *tag, uint64_t n, uint64_t **r, uint64_t *rl);

int storeDatabase(database *db, const char *path);

database *loadDatabase(const char* path);

void printDatabase(database *db);

void debugDatabase(database *db);

#endif
