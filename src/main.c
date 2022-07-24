#include "db.h"
#include <time.h>


int main(){

	database *db = newDatabase("miDB");
	inputBuffer *in = newInputBuffer();
	
	
	addFileTag(db, "vaca.png", "naturaleza");	
	addFileTags(db, "donald-tromp.jpg", 3, "based", "hitler", "very cool");
	addFileTag(db, "vaca.png", "lovely");
	addFileTags(db, "vaca.png", 3, "nature", "kami", "very cool");
	
	storeDatabase(db, "db.db");

	printDatabase(db);
	
	debugDatabase(db);
	
	uint64_t *l, i;
	searchFile(db, "donald-tromp.jpg", 0, &l, &i);
	
	printf("Tags with tag 'donald-tromp.jpg':\n");
	for(uint64_t j = 0; j < i; ++j){
		printf("\t%s\n", db->ltags->table[l[j]]);
	
	}
	

	while(0){

		prompt();
		getInput(in);
		
		/*
		insertTag(r, in->buffer);
		printf("Tags of row '%s': %s\n", r->path, r->tags);
		printf("Number of tags: %u. Length of tags: %u\n", r->numTags, r->lenTags);
		*/

		/*switch(handleInput(in)){
			case META_COMMAND_SUCCESS:
				printf("we done it nigger\n");
				break;
			case META_COMMAND_FAIL:
				printf("uh-oh nigga i dunno what '%s' is!\n", in->buffer);
				break;
		}*/
	}

	return 0;
}
