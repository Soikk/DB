#ifndef STORAGE_H
#define STORAGE_H

#include "db.h"

#define MAXPATH 4096
#define MAXTAGS 4096


typedef struct{
	char path[MAXPATH];
	char tags[MAXTAGS];
	uint16_t numTags;
	uint16_t lentags;
	
} row;


row *newRow(const char *path);

void split(const char *src, char sep, char ***arr, int *len);

void swapWords(char ***arr, int a, int b);

char *normalizeTag(char *tag);

void insertTag(row *r, char *tag);

void removeTag(row *r, char *tag);

#endif
