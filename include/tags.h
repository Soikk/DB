#ifndef TAGS_H
#define TAGS_H

/*
	tags are stored in a big string, separated by semicolons (;)
	tags can have namespaces, which should come before the tag
	and be followed by a colon (:)
	should a namespace store more than one tag, the following
	tags will be separated by a comma (,)
	spaces are only allowed inside tags or namespaces, as part
	of themselves
	semicolons, colons, and commas are not allowed inside tags
	or namespaces
	example: "person:ted kaczynsky;mood:serious;meta:jpg"
*/

#define MAXTAGS 4094

void insertTag(char **tags, char *tag){
	if()
}

#endif