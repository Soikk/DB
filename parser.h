#ifndef PARSER_H
#define PARSER_H

#include "db.h"


typedef enum {
	META_COMMAND_SUCCESS,
	META_COMMAND_FAIL,
} metaCommandStatus;


uint16_t len(const char *s);

bool sameStr(const char* s1, const char *s2);

int handleMetaCommand(inputBuffer *in);

int handleInput(inputBuffer *in);

#endif