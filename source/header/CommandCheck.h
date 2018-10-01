#ifndef COMMAND_CHECK_H
#define COMMAND_CHECK_H

#include "CommandTypes.h"

#ifndef min
	#define min(a,b) ((a) > (b) ? (a) : (b))
#endif

int command_code(char * command, int length);
int check_command(const char * reference, int r_length, char * input, int length);

#endif