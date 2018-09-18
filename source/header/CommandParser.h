#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#ifndef min
	#define min(a,b) ((a) > (b) ? (a) : (b))
#endif

#include "CommandTypes.h"

int process_input(char * command, int length, command_struct * c_struct);
int command_code(char * command, int length);
int check_command(const char * reference, int r_length, char * input, int length);

void test_CommandParser();
void test_command_code();
void test_split_command();


#endif
