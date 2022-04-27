#ifndef REPL_H
#define REPL_H

#include "db.h"

typedef struct {
	char *buffer;
	ssize_t inputSize;
} inputBuffer;


inputBuffer *newInputBuffer(void);

void freeInputBuffer(inputBuffer *in);

void getInput(inputBuffer *in);

void prompt(void);

#endif