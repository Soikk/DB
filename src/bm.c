#include "db.h"

/*
	Implementation of the tuned Boyer-Moore string search algorithm,
	as defined here:
		http://www-igm.univ-mlv.fr/~lecroq/string/tunedbm.html#SECTION00195
	and here:
		http://www-igm.univ-mlv.fr/~lecroq/string/node14.html#SECTION00140
*/

#define ASIZE 128 // alphabet size, need to include most ascii characters

static void preBmBc(char *x, int m, int bmBc[]){
	for(int i = 0; i < ASIZE; ++i)
		bmBc[i] = m;
	for(int i = 0; i < m - 1; ++i)
		bmBc[x[i]] = m - i - 1;
}

// x is the needle, y is the haystack
// Should be called TUNEDBM, called BM for simplicity
ssize_t BM(char *x, int m, char *y, int n){
	int j, k, shift, bmBc[ASIZE];

	/* Preprocessing */
	preBmBc(x, m, bmBc);
	shift = bmBc[x[m-1]];
	bmBc[x[m-1]] = 0;

	/* Searching */
	j = 0;
	while(j < n){
		k = bmBc[y[j + m -1]];
		while(k != 0){
			j += k; k = bmBc[y[j + m -1]];
			//j += k; k = bmBc[y[j + m -1]];
			//j += k; k = bmBc[y[j + m -1]];
		}
		if(memcmp(x, y + j, m) == 0 && j < n)
			return j;
		j += shift;                          /* shift */
	}
	return -1;
}