#include "db.h"


// LTABLE

ltable *newLtable(uint64_t size){
	ltable *lt = malloc(sizeof(ltable));
	lt->size = size;
	lt->table = malloc(size*sizeof(char*));
	return lt;
}

int deleteLtable(ltable **lt){
	free((*lt)->table);
	free(*lt);
	*lt = NULL;
	return 0;
}

int insertLtable(ltable *lt, char *str){
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

int removeLtable(ltable *lt, char *str){
	uint64_t i = searchLtable(lt, str);
	if(i == UINTMAX_MAX){
		return -1;
	}
	lt->size--;
	for(uint64_t j = i; j < lt->size; ++j){
		lt->table[j] = lt->table[j+1];
	}
	return 0;
}

uint64_t searchLtable(ltable *lt, char *str){
	uint32_t l;
	str = normalizeStrLimit(str, &l, MAXPATH-1);
	for(uint64_t i = 0; i < lt->size; ++i){
		if(sameStr(str, lt->table[i])){
			return i;
		}
	}
	return UINTMAX_MAX;
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

ltable *loadLtable(FILE *fp){
	char header;
	fread(&header, sizeof(char), 1, fp);
	if(header != 'L'){
		fprintf(stderr, "Header is '%c' not 'L'\n", header);
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
		fprintf(stderr, "End is '%c' not 'E'\n", end);
	}
	return lt;
}

// CTABLE

ctable *newCtable(uint64_t size){
	ctable *ct = malloc(sizeof(ctable));
	ct->size = size;
	ct->table = calloc(size, sizeof(uint64_t));
	return ct;
}

int deleteCtable(ctable **ct){
	free((*ct)->table);
	free(*ct);
	*ct = NULL;
	return 0;
}

int insertCtable(ctable *ct, uint64_t n){
	uint64_t *nct = malloc((ct->size+1)*sizeof(uint64_t));
	for(uint64_t i = 0; i < ct->size; ++i){
		if(n == ct->table[i]){
			return -1;
		}
		nct[i] = ct->table[i];
	}
	nct[ct->size] = n;
	
	ct->size++;
	ct->table = malloc(ct->size*sizeof(uint64_t));
	for(uint64_t i = 0; i < ct->size; ++i){
		ct->table[i] = nct[i];
	}
	return 0;
}

int removeCtable(ctable *ct, uint64_t i){
	if(i >= ct->size || i < 0){
		return -1;
	}
	ct->size--;
	for(uint64_t j = i; j < ct->size; ++j){
		ct->table[j] = ct->table[j+1];
	}
	return 0;
}

uint64_t searchCtable(ctable *ct, uint64_t n){
	for(uint64_t i = 0; i < ct->size; ++i){
		if(n == ct->table[i]){
			return i;
		}
	}
	return UINTMAX_MAX;
}

int storeCtable(const ctable *ct, FILE *fp){
	char header = 'C';
	fwrite(&header, sizeof(char), 1, fp);
	fwrite(&ct->size, sizeof(uint64_t), 1, fp);
	for(uint64_t i = 0; i < ct->size; ++i){
		fwrite(&ct->table[i], sizeof(uint64_t), 1, fp);
	}
	char end = 'E';
	fwrite(&end, sizeof(char), 1, fp);
	return 0;
}

ctable *loadCtable(FILE *fp){
	char header;
	fread(&header, sizeof(char), 1, fp);
	if(header != 'C'){
		fprintf(stderr, "Header is '%c' not 'C'\n", header);
	}
	uint64_t size;
	fread(&size, sizeof(uint64_t), 1, fp);
	ctable *ct = newCtable(size);
	for(uint64_t i = 0; i < ct->size; ++i){
		fread(&ct->table[i], sizeof(uint64_t), 1, fp);
	}
	char end;
	fread(&end, sizeof(char), 1, fp);
	if(end != 'E'){
		fprintf(stderr, "End is '%c' not 'E'\n", end);
	}
	return ct;
}

// MTABLE

mtable *newMtable(uint64_t size){
	mtable *mt = malloc(sizeof(mtable));
	mt->size = size;
	mt->table = malloc(size*sizeof(relation));
	return mt;
}

int deleteMtable(mtable **mt){
	free((*mt)->table);
	free(*mt);
	*mt = NULL;
	return 0;
}

int insertMtable(mtable *mt, relation r){
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

int removeMtable(mtable *mt, relation r){
	uint64_t i = searchMtable(mt, r);
	if(i == UINTMAX_MAX){
		return -1;
	}
	mt->size--;
	for(uint64_t j = i; j < mt->size; ++j){
		mt->table[j] = mt->table[j+1];
	}
	return 0;
}

int removeFileMtable(mtable *mt, uint64_t file){
	relation *nmt = malloc(mt->size*sizeof(relation));
	uint64_t ni = 0;
	for(uint64_t i = 0; i < mt->size; ++i){
		if(file != mt->table[i].file){
			nmt[ni] = mt->table[i];
			++ni;
		}
	}
	mt->size = ni;
	mt->table = malloc(mt->size*sizeof(relation));
	for(uint64_t i = 0; i < mt->size; ++i){
		mt->table[i] = nmt[i];
	}
	return 0;
}

int removeTagMtable(mtable *mt, uint64_t tag){
	relation *nmt = malloc(mt->size*sizeof(relation));
	uint64_t ni = 0;
	for(uint64_t i = 0; i < mt->size; ++i){
		if(tag != mt->table[i].tag){
			nmt[ni] = mt->table[i];
			++ni;
		}
	}
	mt->size = ni;
	mt->table = malloc(mt->size*sizeof(relation));
	for(uint64_t i = 0; i < mt->size; ++i){
		mt->table[i] = nmt[i];
	}
	return 0;
}

uint64_t searchMtable(mtable *mt, relation r){
	for(uint64_t i = 0; i < mt->size; ++i){
		if(r.file == mt->table[i].file && r.tag == mt->table[i].tag){
			return i;
		}
	}
	return UINTMAX_MAX;
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

mtable *loadMtable(FILE *fp){
	char header;
	fread(&header, sizeof(char), 1, fp);
	if(header != 'M'){
		fprintf(stderr, "Header is '%c' not 'M'\n", header);
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
		fprintf(stderr, "End is '%c' not 'E'\n", end);
	}
	return mt;
}

// AVL TREE

node *newNode(uint64_t h, uint64_t i){
	node *n = malloc(sizeof(node));
	n->h = h;
	n->i = i;
	n->left = NULL;
	n->right = NULL;
	return n;
}

int deleteTree(tree *root){
	if(*root == NULL){
		return -1;
	}
	deleteTree(&(*root)->left);
	deleteTree(&(*root)->right);
	free(*root);
	*root = NULL;
	return 0;
}

static node *rotateNodeRight(node *r){
	node *nr = r->left;
	node *nc = nr->right;

	r->left = nc;
	nr->right = r;
	return nr;
}

static node *rotateNodeLeft(node *r){
	node *nr = r->right;
	node *nc = nr->left;

	r->right = nc;
	nr->left = r;
	return nr;
}

static inline uint64_t max(uint64_t a, uint64_t b){
	return ((a > b) ? a : b);
}

static uint64_t height(node *n){
	if(n != NULL){
		return 1 + max(height(n->left), height(n->right));
	}
	return 0;
}

static int64_t balance(node *n){
	if(n != NULL){
		return height(n->left) - height(n->right);
	}
	return 0;
}

node *insertNode(node *r, uint64_t h, uint64_t i){
	if(r == NULL){
		return newNode(h, i);
	}else if(r->h > h){
		r->left = insertNode(r->left, h, i);
	}else if(r->h < h){
		r->right = insertNode(r->right, h, i);
	}else{
		return r;
	}

	int64_t b = balance(r);
	if(b > 1 && h < r->left->h){	// Left left
		return rotateNodeRight(r);
	}
	if(b < -1 && h > r->right->h){	// Right right
		return rotateNodeLeft(r);
	}
	if(b > 1 && h > r->left->h){	// Left right
		r->left = rotateNodeLeft(r->left);
		return rotateNodeRight(r);
	}
	if(b < -1 && h < r->right->h){	// Right left
		r->right = rotateNodeRight(r->right);
		return rotateNodeLeft(r);
	}
	return r;
}

static node *lowestNode(node *n){
	node *t = n;
	while(t->left != NULL){
		t = t->left;
	}
	return t;
}

node *removeNode(node *r, uint64_t h){
	if(r == NULL){
		return r;
	}else if(r->h > h){
		r->left = removeNode(r->left, h);
	}else if(r->h < h){
		r->right = removeNode(r->right, h);
	}else{
		if(r->left == NULL || r->right == NULL){
			node *t = (r->left) ? r->left : r->right;
			if(t == NULL){
				t = r;
				r = NULL;
			}else{
				*r = *t;
			}
			free(t);
		}else{
			node *t = lowestNode(r->right);
			r->h = t->h;
			r->i = t->i;
			r->right = removeNode(r->right, t->h);
		}
	}
	if(r == NULL){
		return r;
	}
	
	int64_t b = balance(r), bl = balance(r->left), br = balance(r->right);
	if(b > 1 && bl >= 0){	// Left left
		return rotateNodeRight(r);
	}
	if(b < -1 && br <= 0){	// Right right
		return rotateNodeLeft(r);
	}
	if(b > 1 && bl < 0){	// Left right
		r->left = rotateNodeLeft(r->left);
		return rotateNodeRight(r);
	}
	if(b < -1 && br > 0){	// Right left
		r->right = rotateNodeRight(r->right);
		return rotateNodeLeft(r);
	}
	return r;
}

uint64_t searchNode(node *n, uint64_t h){
	if(n == NULL){
		return UINTMAX_MAX;
	}else if(h == n->h){
		return n->i;
	}else if(h < n->h){
		return searchNode(n->left, h);
	}else if(h > n->h){
		return searchNode(n->right, h);
	}
}

static void nodesToArray(node *n, uint64_t *array, uint64_t i){
	if(n == NULL){
		return;
	}
	array[i+0] = n->h;
	array[i+1] = n->i;
	nodesToArray(n->left, array, (2*i + 1)*2);
	nodesToArray(n->right, array, (2*i + 2)*2);
}

static uint64_t *treeToArray(tree root, uint64_t *maxNodes){
	uint64_t treeHeight = height(root);
	*maxNodes = (1<<treeHeight) - 1;
	uint64_t *treeArray = malloc((2*(*maxNodes))*sizeof(uint64_t)); // One space for h, another for i
	for(uint64_t i = 0; i < *maxNodes; ++i){
		uint64_t ai = (i<<1);
		// Lets hope we dont have an entry in the database where both the hash and the index are 18446744073709551615
		treeArray[ai+0] = UINTMAX_MAX;
		treeArray[ai+1] = UINTMAX_MAX;
	}
	nodesToArray(root, treeArray, 0);
	return treeArray;
}

int storeAVLTree(tree root, FILE *fp){
	char header = 'T';
	fwrite(&header, sizeof(char), 1, fp);
	uint64_t maxNodes;
	uint64_t *array = treeToArray(root, &maxNodes);
	fwrite(&maxNodes, sizeof(uint64_t), 1, fp);
	for(uint64_t i = 0; i < maxNodes; ++i){
		uint64_t ai = (i<<1);
		fwrite(&array[ai+0], sizeof(uint64_t), 1, fp);	// Writing h
		fwrite(&array[ai+1], sizeof(uint64_t), 1, fp);	// Writing i
	}
	char end = 'E';
	fwrite(&end, sizeof(char), 1, fp);
	return 0;
}

static node *arrayToNodes(uint64_t *array, uint64_t i, uint64_t maxNodes){
	if(i >= maxNodes*2){
		return NULL;
	}
	node *n = newNode(array[i+0], array[i+1]);
	n->left = arrayToNodes(array, (2*i + 1)*2, maxNodes);
	n->right = arrayToNodes(array, (2*i + 2)*2, maxNodes);
	return n;
}

tree loadAVLTree(FILE *fp){
	char header;
	fread(&header, sizeof(char), 1, fp);
	if(header != 'T'){
		fprintf(stderr, "Header is '%c' not 'T'\n", header);
	}
	uint64_t maxNodes;
	fread(&maxNodes, sizeof(uint64_t), 1, fp);
	uint64_t *array = malloc((2*maxNodes)*sizeof(uint64_t));
	for(uint64_t i = 0; i < maxNodes; ++i){
		uint64_t ai = (i<<1);
		fread(&array[ai+0], sizeof(uint64_t), 1, fp);	// Reading h
		fread(&array[ai+1], sizeof(uint64_t), 1, fp);	// Reading i
	}
	tree root = arrayToNodes(array, 0, maxNodes);
	char end;
	fread(&end, sizeof(char), 1, fp);
	if(end != 'E'){
		fprintf(stderr, "End is '%c' not 'E'\n", end);
	}
	return root;
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
