#include "db.h"




// Splits src into words based on a separator character (sep) and stores them in arr, and the length in len. Inspired by https://github.com/joshdk/tag/blob/master/src/dsv.c's split
void split(const char *src, char sep, char ***arr, int *len){
	int i = 0, asize = 1, ai = 0, wsize = 1, wi = 0;
	char c = 0;

	(*arr) = malloc(asize*sizeof(char*));
	(*arr)[ai] = malloc(wsize*sizeof(char));

	while((c = src[i]) != '\0'){
		// If there's a new word (new array index) reallocate the array and allocate space for it
		if(wi == 0){
			char **tmp;
			if((tmp = realloc((*arr), (asize+ai)*sizeof(char*))) != NULL){
				*arr = tmp;
				tmp = NULL;
			}else{
				fprintf(stderr, "Error reallocating array (split)");
				exit(EXIT_FAILURE);
			}
			(*arr)[ai] = malloc(wsize*sizeof(char));
		}
		// Allocate space for a new character in a word
		char *tmp;
		if((tmp = realloc((*arr)[ai], (wsize+wi)*sizeof(char))) != NULL){
			strcpy(tmp, (*arr)[ai]);
			(*arr)[ai] = tmp;
			tmp = NULL;
		}else{
			fprintf(stderr, "Error reallocating word (split)");
			exit(EXIT_FAILURE);
		}
		// If the character is a separator, terminate the string and increment the array index; if not, assign the character and increment the word index
		if(c == sep){
			(*arr)[ai][wi] = '\0';
			wi = 0;
			++ai;
		}else{
			(*arr)[ai][wi] = c;
			++wi;
		}
		++i;
	}
	if(src[i-1] != sep){
		(*arr)[ai][wi] = '\0';
		++ai;
	}
	(*arr)[ai] = NULL;
	*len = ai;
}

void swapWords(char ***arr, int a, int b){
	char *tmp = (*arr)[a];
	(*arr)[a] = (*arr)[b];
	(*arr)[b] = tmp;
}

char *normalizeTag(char *tag){
	uint16_t l = len(tag);
	char *ntag = calloc(l+1, sizeof(char));
	for(int i = 0; i < l; ++i){
		ntag[i] = tolower(tag[i]);
	}
	return ntag;
}

// Adds a tag in the tags array in the row r, sorted by natural string comparison with strnatcmp
// We assume that when adding a tag all other tags are already sorted
void insertTag(row *r, char *tag){

	tag = normalizeTag(tag);

	// Dump tags into array of strings and add tag
	char **arr, **tmp;
	int l;
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
	int g = 0;
	for(int i = 0; i < l; ++i){
		int j = 0;
		while(arr[i][j] != '\0'){
			r->tags[g] = arr[i][j];
			++j;
			++g;
		}
		r->tags[g++] = ';';
	}
	r->tags[g] = '\0';
}