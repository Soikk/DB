#ifndef STORAGE_H
#define STORAGE_H

#include "db.h"

#define MAXPATH 4094
#define MAXTAGS 4094


// When intializing the struct, it is recommended
// to also initialize numTags and lenTags
typedef struct{
	char path[MAXPATH];
	char tags[MAXTAGS];
	uint16_t lenTags;
	uint16_t numTags;
} row;


row *newRow(const char *path);

void insertTag(row *r, char *tag);

void removeTag(row *r, char *tag);

#endif
