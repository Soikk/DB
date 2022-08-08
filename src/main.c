#include "db.h"
#include <time.h>


int main(){


	inputBuffer *in = newInputBuffer();
	database *db = newDatabase("miDB");


	addFileTag(db, "vaca.png", "naturalezas");	
	addFileTags(db, "terry-davis.jpg", 3, "holyC", "programmer", "very cool");
	addFileTag(db, "vaca.png", "lovely");
	addFileTags(db, "vaca.png", 3, "nature", "animal", "very cool");
	addFileTag(db, "terry-davis.jpg", "based");

	storeDatabase(db, "db.db");

	printDatabase(db);

	debugDatabase(db);
	
	uint64_t *l, i;
	searchFile(db, "terry-davis.jpg", 0, &l, &i);
	
	printf("Tags with file 'terry-davis.jpg':\n");
	for(uint64_t j = 0; j < i; ++j){
		printf("\t%s\n", db->ltags->table[l[j]]);
	
	}
	
	addTagFiles(db, "elemento", 2, "vaca.png", "terry-davis.jpg");
	
	printDatabase(db);
	
	debugDatabase(db);
	
	
	

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
