#ifndef STORAGE_H
#define STORAGE_H

#include "db.h"

/*	(From tags.h)
	tags are stored in a big table (or their hashes are) ordered
	by alphabetical order
	tags can have namespaces which are a special tag that starts
	with a ':'
	all tags in a namespace are located between two occurrences
	of the namespace within the list,
		e.g [":people", "sam hyde", "hitler", ":people"]
	maybe namespaces use another hashing function to prevent
	collisions because of the lack of space because of the ':'
*/

/*
	tags are stored in a big table (or their hashes are) ordered
	by alphabetical order
	tags can have namespaces which are a special tag that starts
	with a ':'
	all tags in a namespace are located between two occurrences
	of the namespace within the list,
		e.g [":people", "sam hyde", "hitler", ":people"]
	maybe namespaces use another hashing function to prevent
	collisions because of the lack of space because of the ':'

	files (their paths) are stored in a big table (or their hashes
	are) by alphabetical/numerical order

	tags and files' indexes on their respective tables correspond
	to the index of the actual tag or file on alookup table.
	e.g
		iT[23] = 816820769819429900 = hash(lT[23]) = hash("cacadevaca")

	there is another table that ties the two together by their
	indexes
	if an file has more than one tag, it is stored as multiple
	relations inside the table, e.g ["1:2", "1:3", "1:4"]

	searching for an file that has a tag is as simple as
		- finding the tag on its list
		- if the tag is on the list, getting its index number
			if not, return 'tag not found'
		- search for all the relations that have the index as
			the second argument
		- if no files are found, return 'files not found'
		- store the indexes of all the files (the first argument)
		- return the list of all the files
		- (OPTIONAL) show all the other tags the files shown have
*/

#define MAXPATH 4096


// Stores the actual filepaths/tags
typedef struct lookupTable{
	uint64_t size;
	char **table; // They cant be longer than MAXPATH
} ltable;

// Stores the hashes of the filepaths/tags (for easier lookup)
typedef struct hashTable{
	uint64_t size;
	uint64_t *table;
} htable;

typedef struct relation{
	uint64_t file;
	uint64_t tag;
} relation;

// Maps the relations between the filepaths and the tags
typedef struct mappingTable{
	uint64_t size;
	relation *table;
} mtable;


// LTABLE
ltable *newLtable(uint64_t size);

ltable *loadLtable(FILE *fp);

int storeLtable(const ltable *lt, FILE *fp);

int ltableAdd(ltable *lt, char *str);

uint64_t ltableSearch(ltable *lt, char *str);

// HTABLE

htable *newHtable(uint64_t size);

htable *loadHtable(FILE *fp);

int storeHtable(const htable *ht, FILE *fp);

int htableAdd(htable *ht, uint64_t h);

uint64_t htableSearch(htable *ht, uint64_t h);

// MTABLE

mtable *newMtable(uint64_t size);

mtable *loadMtable(FILE *fp);

int storeMtable(const mtable *mt, FILE *fp);

int mtableAdd(mtable *mt, relation r);

uint64_t mtableSearch(mtable *mt, relation r);

#endif
