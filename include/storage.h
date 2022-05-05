#ifndef STORAGE_H
#define STORAGE_H

#include "db.h"

#define MAXPATH 4096
#define MAXTAGS 4096


// When intializing the struct, it is recommended
// to also initialize numTags and lenTags
typedef struct{
	char path[MAXPATH];
	char tags[MAXTAGS];
	uint16_t numTags;
	uint16_t lenTags;
} row;


row *newRow(const char *path);

void insertTag(row *r, char *tag);

void removeTag(row *r, char *tag);

#endif