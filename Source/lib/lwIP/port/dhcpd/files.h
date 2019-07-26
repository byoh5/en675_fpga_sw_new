/* files.h */
#ifndef _FILES_H
#define _FILES_H

struct config_keyword {
	char keyword[16];
	int (*handler)(char *line, void *var);
	void *var;
	char def[32];
};


int read_config(void);

#endif
