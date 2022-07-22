#ifndef DATABASE_H
#define DATABASE_H

#include "db.h"


typedef struct database{
	char name[32];
	ltable *lfiles, *ltags;
	htable *hfiles, *htags;
	mtable *map;
} database;


database *newDatabase(char *name);

database *loadDatabase(const char* path);

int storeDatabase(database *db, const char *path);

int addFileTag(database *db, char *file, char *tag);

int addFileTags(database *db, char *file, int ntags, ...);

int searchTag(database *db, char *tag, uint64_t *rl);

void printDatabase(database *db);

void debugDatabase(database *db);

#endif
