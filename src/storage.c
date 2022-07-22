#include "db.h"


ltable *newLtable(uint64_t size){
	ltable *lt = malloc(sizeof(ltable));
	size = (((uint64_t)size) < 0) ? 0 : size;
	lt->size = size;
	lt->table = malloc(size*sizeof(char*));
	return lt;
}

ltable *loadLtable(FILE *fp){
	char header;
	fread(&header, sizeof(char), 1, fp);
	if(header != 'L'){
		printf("header is %c not L\n", header);
	}
	uint64_t size;
	fread(&size, sizeof(uint64_t), 1, fp);
	ltable *lt = newLtable(size);
	for(uint64_t i = 0; i < lt->size; ++i){
		uint32_t sl;
		fread(&sl, sizeof(uint32_t), 1, fp);
		lt->table[i] = malloc(sl*sizeof(char));
		fread(lt->table[i], sizeof(char), sl, fp);
	}
	char end;
	fread(&end, sizeof(char), 1, fp);
	if(end != 'E'){
		printf("end is %c not E\n", end);
	}
	return lt;
}

int storeLtable(const ltable *lt, FILE *fp){
	char header = 'L';
	fwrite(&header, sizeof(char), 1, fp);
	fwrite(&lt->size, sizeof(uint64_t), 1, fp);
	for(uint64_t i = 0; i < lt->size; ++i){
		uint32_t l = len(lt->table[i]) + 1;
		fwrite(&l, sizeof(uint32_t), 1, fp);
		fwrite(lt->table[i], sizeof(char), l, fp);
	}
	char end = 'E';
	fwrite(&end, sizeof(char), 1, fp);
	return 0;
}

int ltableAdd(ltable *lt, char *str){
	uint32_t ls;
	str = normalizeStrLimit(str, &ls, MAXPATH-1);
	
	char **nlt = malloc((lt->size+1)*sizeof(char*));
	for(uint64_t i = 0; i < lt->size; ++i){
		if(sameStr(str, lt->table[i])){
			return -1;
		}
		uint32_t l = len(lt->table[i]);
		nlt[i] = malloc((l+1)*sizeof(char));
		memcpy(nlt[i], lt->table[i], l+1);
	}
	nlt[lt->size] = malloc((ls+1)*sizeof(char));
	memcpy(nlt[lt->size], str, ls+1);
	
	lt->size++;
	lt->table = malloc(lt->size*sizeof(char*));
	for(uint64_t i = 0; i < lt->size; ++i){
		uint32_t l = len(nlt[i]);
		lt->table[i] = malloc((l+1)*sizeof(char));
		memcpy(lt->table[i], nlt[i], l+1);
	}
	return 0;
}

uint64_t ltableSearch(ltable *lt, char *str){
	uint32_t l;
	str = normalizeStrLimit(str, &l, MAXPATH-1);
	
	for(uint64_t i = 0; i < lt->size; ++i){
		if(sameStr(str, lt->table[i])){
			return i;
		}
	}
	return -1;
}

htable *newHtable(uint64_t size){
	htable *ht = malloc(sizeof(htable));
	size = (((uint64_t)size) < 0) ? 0 : size;
	ht->size = size;
	ht->table = malloc(size*sizeof(uint64_t));
	return ht;
}

htable *loadHtable(FILE *fp){
	char header;
	fread(&header, sizeof(char), 1, fp);
	if(header != 'H'){
		printf("header is %c not H\n", header);
	}
	uint64_t size;
	fread(&size, sizeof(uint64_t), 1, fp);
	htable *ht = newHtable(size);
	for(uint64_t i = 0; i < ht->size; ++i){
		fread(&ht->table[i], sizeof(uint64_t), 1, fp);
	}
	char end;
	fread(&end, sizeof(char), 1, fp);
	if(end != 'E'){
		printf("end is %c not E\n", end);
	}
	return ht;
}

int storeHtable(const htable *ht, FILE *fp){
	char header = 'H';
	fwrite(&header, sizeof(char), 1, fp);
	fwrite(&ht->size, sizeof(uint64_t), 1, fp);
	for(uint64_t i = 0; i < ht->size; ++i){
		fwrite(&ht->table[i], sizeof(uint64_t), 1, fp);
	}
	char end = 'E';
	fwrite(&end, sizeof(char), 1, fp);
	return 0;
}

int htableAdd(htable *ht, uint64_t h){
	uint64_t *nht = malloc((ht->size+1)*sizeof(uint64_t));
	for(uint64_t i = 0; i < ht->size; ++i){
		if(h == ht->table[i]){
			return -1;
		}
		nht[i] = ht->table[i];
	}
	nht[ht->size] = h;
	
	ht->size++;
	ht->table = malloc(ht->size*sizeof(uint64_t));
	for(uint64_t i = 0; i < ht->size; ++i){
		ht->table[i] = nht[i];
	}
	return 0;
}

uint64_t htableSearch(htable *ht, uint64_t h){
	for(uint64_t i = 0; i < ht->size; ++i){
		if(h == ht->table[i]){
			return i;
		}
	}
	return -1;
}

mtable *newMtable(uint64_t size){
	mtable *mt = malloc(sizeof(mtable));
	size = (((uint64_t)size) < 0) ? 0 : size;
	mt->size = size;
	mt->table = malloc(size*sizeof(relation));
	return mt;
}

mtable *loadMtable(FILE *fp){
	char header;
	fread(&header, sizeof(char), 1, fp);
	if(header != 'M'){
		printf("header is %c not M\n", header);
	}
	uint64_t size;
	fread(&size, sizeof(uint64_t), 1, fp);
	mtable *mt = newMtable(size);
	for(uint64_t i = 0; i < mt->size; ++i){
		fread(&mt->table[i].file, sizeof(uint64_t), 1, fp);
		fread(&mt->table[i].tag, sizeof(uint64_t), 1, fp);
	}
	char end;
	fread(&end, sizeof(char), 1, fp);
	if(end != 'E'){
		printf("end is %c not E\n", end);
	}
	return mt;
}

int storeMtable(const mtable *mt, FILE *fp){
	char header = 'M';
	fwrite(&header, sizeof(char), 1, fp);
	fwrite(&mt->size, sizeof(uint64_t), 1, fp);
	for(uint64_t i = 0; i < mt->size; ++i){
		fwrite(&mt->table[i].file, sizeof(uint64_t), 1, fp);
		fwrite(&mt->table[i].tag, sizeof(uint64_t), 1, fp);
	}
	char end = 'E';
	fwrite(&end, sizeof(char), 1, fp);
	return 0;
}

int mtableAdd(mtable *mt, relation r){
	relation *nmt = malloc((mt->size+1)*sizeof(relation));
	for(uint64_t i = 0; i < mt->size; ++i){
		if(r.file == mt->table[i].file && r.tag == mt->table[i].tag){
			return -1;
		}
		nmt[i] = mt->table[i];
	}
	nmt[mt->size] = r;
	
	mt->size++;
	mt->table = malloc(mt->size*sizeof(relation));
	for(uint64_t i = 0; i < mt->size; ++i){
		mt->table[i] = nmt[i];
	}
	return 0;
}

uint64_t mtableSearch(mtable *mt, relation r){
	for(uint64_t i = 0; i < mt->size; ++i){
		if(r.file == mt->table[i].file && r.tag == mt->table[i].tag){
			return i;
		}
	}
	return -1;
}




/*
// TODO: remove old impl

// Splits src into words based on a separator character (sep) and stores them in arr,
// and the length in len. Inspired by https://github.com/joshdk/tag/blob/master/src/dsv.c's split
static void split(const char *src, char sep, char ***arr, uint32_t *len){
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

// Adds a tag in the tags array in the row r, sorted by natural string
// comparison with strnatcmp. We assume that when adding a tag all other
// tags are already sorted. Nothing is done if the tag is already in the tags
void insertTag(row *r, char *tag){
	uint32_t l, ltag;
	tag = normalizeStr(tag, &ltag);

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
	uint32_t l, ltag;
	tag = normalizeStr(tag, &ltag);

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
*/
