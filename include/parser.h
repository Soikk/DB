#ifndef PARSER_H
#define PARSER_H

#include "db.h"


#define NKEYWORDS 16
#define EXIT_CODE 1

typedef enum{
	COMMAND_EXIT, COMMAND_DEBUG,
	COMMAND_CREATE, COMMAND_DELETE, COMMAND_OPEN, COMMAND_SAVE, COMMAND_CLOSE,
	COMMAND_ADDF, COMMAND_ADDT, COMMAND_DELETEF, COMMAND_DELETET,
	COMMAND_TAG, COMMAND_REMOVE,
	COMMAND_SHOW, COMMAND_SHOWT, COMMAND_SHOWF,
} COMMANDS;

typedef enum{
	META_COMMAND_SUCCESS,
	META_COMMAND_FAIL,
} metaCommandStatus;

typedef struct{
	uint64_t argc;
	char **argv;
} argList;


argList *newArgList(void);

argList *splitInput(inputBuffer *in);

int handleMetaCommand(inputBuffer *in);

int handleInput(argList *args, database **db);

#endif
