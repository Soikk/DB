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

uint64_t addFile(database *db, char *file);

uint64_t addTag(database *db, char *tag);

int addFileTag(database *db, char *file, char *tag);

int addFileTags(database *db, char *file, int ntags, ...);

int removeFile(database *db, char *file);

int removeTag(database *db, char *tag);

int searchFile(database *db, char *file, uint64_t n, uint64_t **r, uint64_t *rl);

int searchTag(database *db, char *tag, uint64_t n, uint64_t **r, uint64_t *rl);

int storeDatabase(database *db, const char *path);

database *loadDatabase(const char* path);

void printDatabase(database *db);

void debugAVLtree(node *n);

void debugDatabase(database *db);

#endif
