#ifndef STRINGS_H
#define STRINGS_H

#include "db.h"


uint16_t len(const char *s);

bool sameStr(const char *s1, const char *s2);

ssize_t strInTags(const char *tags, int n, const char *ndl, int m, char sep); 


#endif