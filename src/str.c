#include "db.h"


uint16_t len(const char *s){
	uint16_t l = -1;
	while(s[++l]);
	return l;
}

bool sameStr(const char *s1, const char *s2){
	uint16_t i1 = 0, i2 = 0;
	while(s1[i1] && s1[i1] == s2[i2])
		++i1, ++i2;
	return !s1[i1] && !s2[i2];
}

// Auxiliary function for creating a lookup table of the haystack
// table[i] will be the number of shifts right until the next
// separator when checking position i
// Only really useful for this implementation of tags
static int *table(const char *y, int n, char sep){
	int *tb = calloc(n, sizeof(int));
	if(tb == NULL){
		fprintf(stderr, "Error callocating array (table)");
		exit(EXIT_FAILURE);
	}

	int lSep = n-1;
	for(int i = n-1; i >= 0; --i){
		if(y[i] == sep){
			tb[i] = 1;
			lSep = i;
		}else if(y[i] != '\0'){
			tb[i] = lSep-i;
		}
	}
	return tb;
}

// Returns the position of ndl in tags, -1 if its not found
// A return of 0 means ndl occurs in tags starting in position 0
// Use 'if(strInTags(...) != -1)' when using this function
ssize_t strInTags(const char *tags, int n, const char *ndl, int m, char sep){
	int *tb = table(tags, n, sep);

	for(int i = 0; i < n; ){
		int j = 0;
		while(j < m && tags[i+j] == ndl[j]){
			++j;
		}
		if(j == m){
			return i;
		}
		if(tags[i+j] != ndl[j]){
			i += tb[i];
		}
	}
	return -1;
}
