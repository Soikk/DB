#ifndef TAGS_H
#define TAGS_H

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
*/

#define MAXTAGS 4094

void insertTag(char **tags, char *tag){
	if()
}

#endif