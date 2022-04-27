#include "db.h"


uint16_t len(const char *s){
	uint16_t l = -1;
	while(s[++l]);
	return l;
}

bool sameStr(const char* s1, const char *s2){
	uint16_t i1 = 0, i2 = 0;
	while(s1[i1] && s2[i2] && s1[i1] == s2[i2])
		++i1, ++i2;
	return !s1[i1] && !s2[i2];
}

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