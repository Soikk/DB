#include "db.h"
#include <time.h>


int main(){


	inputBuffer *in = newInputBuffer();
	
	/*database *test = newDatabase("miDB");
	addFileTag(test, "vaca.png", "naturalezas");	
	addFileTags(test, "terry-davis.jpg", 3, "holyC", "programmer", "very cool");
	addFileTag(test, "vaca.png", "lovely");
	addFileTags(test, "vaca.png", 3, "nature", "animal", "very cool");
	addFileTag(test, "terry-davis.jpg", "based");
	
	printDatabase(test);

	debugDatabase(test);

	storeDatabase(test, "db.db");
	
	test = loadDatabase("db.db");

	printDatabase(test);

	debugDatabase(test);
	
	uint64_t *l, i;
	searchFile(test, "terry-davis.jpg", 0, &l, &i);
	
	printf("Tags with file 'terry-davis.jpg':\n");
	for(uint64_t j = 0; j < i; ++j){
		printf("\t%s\n", test->ltags->table[l[j]]);
	
	}*/

	database *db = NULL;
	while(1){
		prompt(db);
		getInput(in);
		argList *args = splitInput(in);
		int status = handleInput(args, &db);
		if(status == EXIT_CODE){
			printf("Exiting db\n");
			break;
		}
	}

	return 0;
}
