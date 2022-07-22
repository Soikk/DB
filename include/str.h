#ifndef STRINGS_H
#define STRINGS_H

#include "db.h"


uint32_t len(const char *s);

bool sameStr(const char *s1, const char *s2);

char *normalizeStr(const char *str, uint32_t *ln);

char *normalizeStrLimit(const char *str, uint32_t *l, uint32_t limit);

ssize_t strInTags(const char *tags, int n, const char *ndl, int m, char sep); 


#endif
