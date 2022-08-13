#include "db.h"


inputBuffer *newInputBuffer(void){
	inputBuffer *in = malloc(sizeof(inputBuffer));
	in->buffer = NULL;
	in->inputSize = 0;
	return in;
}

void freeInputBuffer(inputBuffer *in){
	free(in->buffer);
	free(in);
}

void getInput(inputBuffer *in){
	size_t n;
	ssize_t r = getline(&(in->buffer), &n, stdin);
	if(r <= 0){
		fprintf(stderr, "Error\n");
		exit(r);
	}
	in->inputSize = r-1;
	in->buffer[in->inputSize] = 0;
}

void prompt(database *db){
	if(db == NULL){
		printf("db > ");
	}else{
		printf("%s > ", db->name);
	}
}
