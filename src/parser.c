#include "db.h"


char *keywords[NKEYWORDS] = {"EXIT", "DEBUG",
	"CREATE", "DELETE", "OPEN", "SAVE", "CLOSE",
	"ADDF", "ADDT", "DELETEF", "DELETET",
	"TAG", "REMOVE",
	"SHOW", "SHOWT", "SHOWF"};

argList *newArgList(void){
	argList *args = malloc(sizeof(argList));
	args->argc = 0;
	args->argv = NULL;
	return args;
}

argList *splitInput(inputBuffer *in){
	if(in->inputSize == 0){
		return NULL;
	}
	argList *args = newArgList();
	uint64_t len = args->argc = 0;

	// Remove leading spaces
	while(isspace(in->buffer[len])){
		++len;
	}
	// Count words (alone or quoted)
	while(in->buffer[len] != '\0'){
		while(isspace(in->buffer[len])){
			++len;
		}
		if(in->buffer[len] == '"'){
			do{
				++len;
			}while(in->buffer[len] != '"');
			++args->argc;
		}else if(in->buffer[len] == '\''){
			do{
				++len;
			}while(in->buffer[len] != '\'');
			++args->argc;
		}else{
			while(in->buffer[len] != '\0' && !isspace(in->buffer[len])){
				++len;
			}
			++args->argc;
		}
		++len;
	}
	uint64_t *wstarts = malloc(args->argc*sizeof(uint64_t)), *wlens = malloc(args->argc*sizeof(uint64_t));
	len = args->argc = 0;
	while(in->buffer[len] != '\0'){
		while(isspace(in->buffer[len])){
			++len;
		}
		if(in->buffer[len] == '"'){
			wstarts[args->argc] = len+1;
			do{
				++len;
			}while(in->buffer[len] != '"');
			wlens[args->argc] = len - wstarts[args->argc];
			++args->argc;
		}else if(in->buffer[len] == '\''){
			wstarts[args->argc] = len+1;
			do{
				++len;
			}while(in->buffer[len] != '\'');
			wlens[args->argc] = len -wstarts[args->argc];
			++args->argc;
		}else{
			wstarts[args->argc] = len;
			while(in->buffer[len] != '\0' && !isspace(in->buffer[len])){
				++len;
			}
			wlens[args->argc] = len - wstarts[args->argc];
			++args->argc;
		}
		++len;
	}
	
	args->argv = malloc(args->argc*sizeof(char*));
	for(uint64_t i = 0; i < args->argc; ++i){
		args->argv[i] = calloc(wlens[i]+1, sizeof(char));
		memcpy(args->argv[i], &in->buffer[wstarts[i]], wlens[i]);
	}
	return args;
}

static int commandExit(argList *args, database **db){
	// TODO: cleanup functions
	return EXIT_CODE;
}

static int commandDebug(argList *args, database **db){
	if(*db == NULL){
		fprintf(stderr, "Error: no database to debug\n");
		return -1;
	}
	debugDatabase(*db);
	return 0;
}

static int commandCreate(argList *args, database **db){
	if(args->argc < 2){
		fprintf(stderr, "Error, not enough arguments for command '%s'\n", args->argv[0]);
		return -1;
	}
	for(int i = 1; i < args->argc; ++i){
		int status = storeDatabase(newDatabase(args->argv[i]), args->argv[i]);
		if(status != 0){
			fprintf(stderr, "Error creating and storing database '%s'\n", args->argv[i]);
			return -1;
		}
	}
	return 0;
}

static int commandDelete(argList *args, database **db){
	if(args->argc < 2){
		fprintf(stderr, "Error, not enough arguments for command '%s'\n", args->argv[0]);
		return -1;
	}
	for(int i = 1; i < args->argc; ++i){
		database *t = loadDatabase(args->argv[i]);
		int status = deleteDatabase(&t);
		if(status != 0){
			fprintf(stderr, "Error deleting database '%s'\n", args->argv[i]);
			return -1;
		}
	}
	return 0;
}

static int commandOpen(argList *args, database **db){
	if(args->argc < 2){
		fprintf(stderr, "Error: not enough arguments for command '%s'\n", args->argv[0]);
		return -1;
	}else if(args->argc > 2){
		fprintf(stderr, "Error: too many arguments for command 'OPEN'. Only opening first one ('%s')\n", args->argv[1]);
	}
	if(*db != NULL){
		fprintf(stderr, "Error: database '%s' already opened. Close it before proceeding with \"CLOSE '%s'\"\n", (*db)->name, (*db)->name);
		return -1;
	}
	*db = loadDatabase(args->argv[1]);
	if(*db == NULL){
		fprintf(stderr, "Error loading database '%s'\n", args->argv[1]);
		return -1;
	}
	return 0;
}

static int commandSave(argList *args, database **db){
	if(args->argc > 1){
		fprintf(stderr, "Error: too many arguments for function 'SAVE'. Only saving loaded database ('%s')\n", (*db)->name);
	}
	if(*db == NULL){
		fprintf(stderr, "Error: no database to save\n");
		return -1;
	}
	int status = storeDatabase(*db, (*db)->name);
	if(status != 0){
		fprintf(stderr, "Error storing database '%s'\n", (*db)->name);
		return -1;
	}
	return 0;
}

static int commandClose(argList *args, database **db){
	if(args->argc > 1){
		fprintf(stderr, "Error: too many arguments for function 'CLOSE'. Only closing loaded database ('%s')\n", (*db)->name);
	}
	if(*db == NULL){
		fprintf(stderr, "Error: no database to close\n");
		return -1;
	}
	int status = storeDatabase(*db, (*db)->name);
	if(status != 0){
		fprintf(stderr, "Error storing database '%s' before closing it\n", (*db)->name);
		return -1;
	}
	freeDatabase(db);
	return 0;
}

static int commandAddf(argList *args, database **db){
	if(*db == NULL){
		fprintf(stderr, "Error: no database to add files to\n");
		return -1;
	}
	char **files = malloc((args->argc-1)*sizeof(char*));
	uint64_t fi = 0;
	for(uint64_t i = 1; i < args->argc; ++i){
		files[fi++] = args->argv[i];
	}
	if(fi == 0){
		fprintf(stderr, "Error: no files given to add to database\n");
		return -1;
	}
	for(uint64_t i = 0; i < fi; ++i){
		addFile(*db, files[i]);
	}
	return 0;
}

static int commandAddt(argList *args, database **db){
	if(*db == NULL){
		fprintf(stderr, "Error: no database to add tags to\n");
		return -1;
	}
	char **tags = malloc((args->argc-1)*sizeof(char*));
	uint64_t ti = 0;
	for(uint64_t i = 1; i < args->argc; ++i){
		tags[ti++] = args->argv[i];
	}
	if(ti == 0){
		fprintf(stderr, "Error: no tags given to add to database\n");
		return -1;
	}
	for(uint64_t i = 0; i < ti; ++i){
		addTag(*db, tags[i]);
	}
	return 0;
}

static int commandDeletef(argList *args, database **db){
	if(*db == NULL){
		fprintf(stderr, "Error: no database to delete files from\n");
		return -1;
	}
	char **files = malloc((args->argc-1)*sizeof(char*));
	uint64_t fi = 0;
	for(uint64_t i = 1; i < args->argc; ++i){
		files[fi++] = args->argv[i];
	}
	if(fi == 0){
		fprintf(stderr, "Error: no files given to remove from database\n");
		return -1;
	}
	for(uint64_t i = 0; i < fi; ++i){
		removeFile(*db, files[i]);
	}
	return 0;
}

static int commandDeletet(argList *args, database **db){
	if(*db == NULL){
		fprintf(stderr, "Error: no database to remove tags from\n");
		return -1;
	}
	char **tags = malloc((args->argc-1)*sizeof(char*));
	uint64_t ti = 0;
	for(uint64_t i = 1; i < args->argc; ++i){
		tags[ti++] = args->argv[i];
	}
	if(ti == 0){
		fprintf(stderr, "Error: no tags given to remove from database\n");
		return -1;
	}
	for(uint64_t i = 0; i < ti; ++i){
		removeTag(*db, tags[i]);
	}
	return 0;
}

static int commandTag(argList *args, database **db){
	if(*db == NULL){
		fprintf(stderr, "Error: no database to add files and tags to\n");
		return -1;
	}
	char **files = malloc((args->argc-1)*sizeof(char*));
	uint64_t i, fi = 0, ti = 0;
	bool with = false;
	for(i = 1; i < args->argc; ++i){
		uint32_t l1, l2;
		if(sameStr(normalizeStr(args->argv[i], &l1), normalizeStr("WITH", &l2))){
			with = true;
			++i;
			break;
		}
		files[fi++] = args->argv[i];
	}
	if(fi == 0){
		fprintf(stderr, "Error: no files given to tag\n");
		return -1;
	}
	if(with == false){
		fprintf(stderr, "Error: missing 'WITH' keyword on 'TAG' command\n");
		return -1;
	}
	char **tags = malloc((args->argc-i)*sizeof(char*));
	for(; i < args->argc; ++i){
		tags[ti++] = args->argv[i];
	}
	if(ti == 0){
		fprintf(stderr, "Error: no tags given to tag files\n");
		return -1;
	}

	for(uint64_t f = 0; f < fi; ++f){
		for(uint64_t t = 0; t < ti; ++t){
			int status = addFileTag(*db, files[f], tags[t]);
			if(status != 0){
				fprintf(stderr, "Error: couldnt add tag '%s' to file '%s'\n", tags[t], files[f]);
				return -1;
			}
		}
	}
	return 0;
}

static int commandRemove(argList *args, database **db){
	if(*db == NULL){
		fprintf(stderr, "Error: no database to remove files and tags from\n");
		return -1;
	}
	char **tags = malloc((args->argc-1)*sizeof(char*));
	uint64_t i, ti = 0, fi = 0;
	bool from = false;
	for(i = 1; i < args->argc; ++i){
		uint32_t l1, l2;
		if(sameStr(normalizeStr(args->argv[i], &l1), normalizeStr("FROM", &l2))){
			from = true;
			++i;
			break;
		}
		tags[ti++] = args->argv[i];
	}
	if(ti == 0){
		fprintf(stderr, "Error: no tags given to remove\n");
		return -1;
	}
	if(from == false){
		fprintf(stderr, "Error: missing 'FROM' keyword on 'REMOVE' command\n");
		return -1;
	}
	char **files = malloc((args->argc-i)*sizeof(char*));
	for(; i < args->argc; ++i){
		files[fi++] = args->argv[i];
	}
	if(fi == 0){
		fprintf(stderr, "Error: no files given to remove\n");
		return -1;
	}

	for(uint64_t f = 0; f < fi; ++f){
		for(uint64_t t = 0; t < ti; ++t){
			int status = removeFileTag(*db, files[f], tags[t]);
			if(status != 0){
				fprintf(stderr, "Error: couldnt remove tag '%s' from file '%s'\n", tags[t], files[f]);
				return -1;
			}
		}
	}
	return 0;
}

static int commandShow(argList *args, database **db){
	if(*db == NULL){
		fprintf(stderr, "Error: no database to show\n");
		return -1;
	}
	printDatabase(*db);
	return 0;
}

static int commandShowt(argList *args, database **db){
	// We only show the files of one tag
	if(*db == NULL){
		fprintf(stderr, "Error: no database to show tags of file\n");
		return -1;
	}
	if(args->argc > 2){
		fprintf(stderr, "Error: too many arguments for 'SHOWT', current support for only one file. Only showing tags of first file ('%s')\n", args->argv[1]);
	}
	
	uint64_t rl, *r;
	int status = searchFile(*db, args->argv[1], 0, &r, &rl);
	if(status == -1){
		fprintf(stderr, "Error searching for file '%s'\n", args->argv[1]);
		return -1;
	}
	printf("Tags of file '%s' (%"PRIu64"):\n", args->argv[1], rl);
	for(uint64_t i = 0; i < rl; ++i){
		printf("\t- %s\n", (*db)->ltags->table[r[i]]);
	}
	printf("\n");
}

static int commandShowf(argList *args, database **db){
	// We only show the tags of one file
	if(*db == NULL){
		fprintf(stderr, "Error: no database to show files of tag\n");
		return -1;
	}
	if(args->argc > 2){
		fprintf(stderr, "Error: too many arguments for 'SHOWF', current support for only one tag. Only showing files of first tag ('%s')\n", args->argv[1]);
	}
	
	uint64_t rl, *r;
	int status = searchTag(*db, args->argv[1], 0, &r, &rl);
	if(status == -1){
		fprintf(stderr, "Error searching for tag '%s'\n", args->argv[1]);
		return -1;
	}
	printf("Files of tag '%s' (%"PRIu64"):\n", args->argv[1], rl);
	for(uint64_t i = 0; i < rl; ++i){
		printf("\t- %s\n", (*db)->lfiles->table[r[i]]);
	}
	printf("\n");
}

 int (*commands[])(argList*, database**) = {
	commandExit, commandDebug,
	commandCreate, commandDelete, commandOpen, commandSave, commandClose,
	commandAddf, commandAddt, commandDeletef, commandDeletet,
	commandTag, commandRemove,
	commandShow, commandShowt, commandShowf
	};

int handleInput(argList *args, database **db){
	if(args == NULL){
		return 0;
	}
	int command = -1;
	for(int i = 0; i < NKEYWORDS; ++i){
		uint32_t l1, l2;
		if(sameStr(normalizeStr(args->argv[0], &l1), normalizeStr(keywords[i], &l2))){
			command = i;
			break;
		}
	}
	if(command == -1){
		fprintf(stderr, "Error: command '%s' not found\n", args->argv[0]);
		return -1;
	}
	return commands[command](args, db);
}
