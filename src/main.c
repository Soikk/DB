#include "db.h"


int main(){

	inputBuffer *in = newInputBuffer();

	row *r = newRow("~/test/img.png");

	while(1){

		prompt();
		getInput(in);
		
		insertTag(r, in->buffer);
		printf("Tags of row '%s': %s\n", r->path, r->tags);
		printf("Number of tags: %u. Length of tags: %u\n", r->numTags, r->lenTags);

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
