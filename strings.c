#include "db.h"


uint16_t len(const char *s){
	uint16_t l = -1;
	while(s[++l]);
	return l;
}

bool sameStr(const char *s1, const char *s2){
	uint16_t i1 = 0, i2 = 0;
	while(s1[i1] && s2[i2] && s1[i1] == s2[i2])
		++i1, ++i2;
	return !s1[i1] && !s2[i2];
}


static int *table(char *y, int n, char sep){
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
	
