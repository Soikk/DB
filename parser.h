#ifndef PARSER_H
#define PARSER_H

#include "db.h"


typedef enum {
	META_COMMAND_SUCCESS,
	META_COMMAND_FAIL,
} metaCommandStatus;


int handleMetaCommand(inputBuffer *in);

int handleInput(inputBuffer *in);

#endif
