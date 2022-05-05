#include "db.h"


int main(){

	inputBuffer *in = newInputBuffer();


	row r = {"C:/xd", "perro", 1, 5};
	printf("%s %d %d\n", r.tags, r.numTags, r.lenTags);
	insertTag(&r, "caca");
	insertTag(&r, "mierda");
	insertTag(&r, "perra");
	insertTag(&r, "tu");
	printf("%s %d %d\n", r.tags, r.numTags, r.lenTags);

	printf("%d\n", strInTags(r.tags, r.lenTags, ";perro", 6, ';'));

	row *r2 = newRow("test1test2test3testtesttesttesttesttettesttestestest");

	while(0){

		prompt();
		getInput(in);
		
		insertTag(r2, in->buffer);
		printf("Tags of row '%s': %s\n", r2->path, r2->tags);

		/*switch(handleInput(in)){
			case META_COMMAND_SUCCESS:
				printf("we done it nigger\n");
				break;
			case META_COMMAND_FAIL:
				printf("uh-oh nigga i dunno what '%s' is!\n", in->buffer);
				break;
		}*/
	}
}