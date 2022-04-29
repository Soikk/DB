#include "db.h"


int main(){

	inputBuffer *in = newInputBuffer();


	row r = {"C:/xd", "perro", 1};
	printf("%s\n", r.tags);
	insertTag(&r, "caca");
	insertTag(&r, "mierda");
	insertTag(&r, "tu");
	insertTag(&r, "tu");
	printf("%s %d\n", r.tags, r.numTags);
	removeTag(&r, "");
	printf("%s %d\n", r.tags, r.numTags);

	row *r2 = newRow("test1test2test3testtesttesttesttesttettesttestestest");

	while(1){

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