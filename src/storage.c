#include "db.h"


row *newRow(const char path[MAXPATH]){
	row *nr = malloc(sizeof(row));
	memcpy(nr->path, path, len(path));
	nr->numTags = 0;
	nr->lenTags = 0;

	return nr;
}

// Splits src into words based on a separator character (sep) and stores them in arr,
// and the length in len. Inspired by https://github.com/joshdk/tag/blob/master/src/dsv.c's split
static void split(const char *src, char sep, char ***arr, uint16_t *len){
	int slen = 0, ai = 0, wnum = 0, wlen = 0;

	while(src[slen] != '\0'){
		if(src[slen] == sep){
			++wnum;
		}
		++slen;
	}
	if(slen != 0 && src[slen-1] != sep){
		++wnum;
	}
	++slen;

	*arr = calloc((wnum+1), sizeof(char*));
	for(int i = 0; i < slen; ++i){
		if(src[i] == sep || src[i] == '\0'){
			(*arr)[ai] = calloc(wlen+1, sizeof(char));
			for(int j = i-wlen, k = 0; j < i; ++j, ++k){
				(*arr)[ai][k] = src[j];
			}
			++ai;
			wlen = 0;
		}else{
			++wlen;
		}
	}
	*len = wnum;
}

static void swapWords(char ***arr, int a, int b){
	char *tmp = (*arr)[a];
	(*arr)[a] = (*arr)[b];
	(*arr)[b] = tmp;
}

static char *normalizeTag(char *tag, uint16_t *ln){
	uint16_t l = len(tag);
	char *ntag = calloc(l+1, sizeof(char));
	for(int i = 0; i < l; ++i){
		ntag[i] = tolower(tag[i]);
		if(i == l-1 && tag[i] == ' '){
			ntag[i] = '\0';
			--l;
		}
	}
	*ln = l;
	return ntag;
}

// Adds a tag in the tags array in the row r, sorted by natural string
// comparison with strnatcmp. We assume that when adding a tag all other
// tags are already sorted. Nothing is done if the tag is already in the tags
void insertTag(row *r, char *tag){
	uint16_t l, ltag;
	tag = normalizeTag(tag, &ltag);

	if(ltag == 0){
		return;
	}
	if((r->lenTags+ltag) > MAXTAGS-1){
		fprintf(stderr, "Can't insert tag '%s': too big (%d + %d > %d - 1) (insertTag)",
			tag, r->lenTags, ltag, MAXTAGS);
		exit(EXIT_FAILURE);
	}

	// Dump tags into array of strings and add tag
	char **arr, **tmp;
	split(r->tags, ';', &arr, &l);
	
	if((tmp = realloc(arr, (l+1)*sizeof(char*))) != NULL){
		arr = tmp;
		tmp = NULL;
	}else{
		fprintf(stderr, "Error reallocating array (insertTag)");
		exit(EXIT_FAILURE);
	}
	arr[l] = malloc((len(tag)+1)*sizeof(char));
	strcpy(arr[l], tag);
	
	// Sort tag by natural string comparison, starting from the last element (the new tag)
	for(int i = l; i > 0; --i){
		switch(strnatcmp(arr[i-1], arr[i])){
			case 1:
				// arr[i-1] is higher than arr[i]; swap them
				swapWords(&arr, i, i-1);
				break;
			case -1:
				// arr[i-1] is lower than arr[i]; exit loop
				i = 0;
				break;
			case 0:
				// The tag already exists, no need to alter anything
				free(arr);
				return;
		}
	}
	++l; // Succesfully added new tag

	// Insert tags back into tags member of row structure with the ';' separator in between them
	int tagnum = 0;
	for(int i = 0; i < l; ++i){
		int j = 0;
		while(arr[i][j] != '\0'){
			r->tags[tagnum] = arr[i][j];
			++j;
			++tagnum;
		}
		r->tags[tagnum++] = ';';
	}
	r->tags[tagnum] = '\0';
	r->numTags = l;
	r->lenTags = tagnum;
}

// Remove a tag from the tags array in the row r
// Nothing is done if the tag isnt in the tags
void removeTag(row *r, char *tag){
	uint16_t l, ltag;
	tag = normalizeTag(tag, &ltag);

	if(ltag == 0){
		return;
	}

	// Dump tags into array of strings
	char **arr;
	split(r->tags, ';', &arr, &l);

	// Search for tag and remove it
	for(int i = 0; i <= l; ++i){
		if(sameStr(arr[i], tag)){
			for(int j = i; j < l; ++j){
				arr[j] = arr[j+1];
			}
			--l;
			break;
		}
	}

	// Insert tags back into tags member of row structure with the ';' separator in between them
	int tagnum = 0;
	for(int i = 0; i < l; ++i){
		int j = 0;
		while(arr[i][j] != '\0'){
			r->tags[tagnum] = arr[i][j];
			++j;
			++tagnum;
		}
		r->tags[tagnum++] = ';';
	}
	r->tags[tagnum] = '\0';
	r->numTags = l;
	r->lenTags = tagnum;
}
