#include "db.h"


int handleMetaCommand(inputBuffer *in){
	if(sameStr(in->buffer, ".exit")){
		freeInputBuffer(in);
		printf("EXIT M'NIGGA\n");
		exit(EXIT_SUCCESS);
	}else{
		return META_COMMAND_FAIL;
	}
}

int handleInput(inputBuffer *in){
	if(in->buffer[0] == '.'){
		return handleMetaCommand(in);
	}else{
		printf("normal command\n");
	}
}
