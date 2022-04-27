#include "db.h"





int main(){

	inputBuffer *in = newInputBuffer();

	row r = {"C:/xd", "pepe;soyjak;"};
	printf("%s\n", r.tags);
	insertTag(&r, "WOJAK");
	printf("%s\n", r.tags);

	while(0){

		prompt();
		getInput(in);
		
		char **arr;
		int len;
		split(in->buffer, ';', &arr, &len);
		for(int i = 0; i < len; ++i)
			printf("%s\t", arr[i]);
		printf("\n");
		
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