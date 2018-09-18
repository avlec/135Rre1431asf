#include "header/CommandParser.h"
#include "header/CommandTypes.h"
#include <stdio.h>
#include <string.h>

// End '\0' is if printf needs to be called
#define NUM_CMDS 6
const char * CMD_LIST[NUM_CMDS] = { COMMANDS };


// Converts the input string into a command_struct
int process_input(char * command, int length, command_struct * c_struct) {
	char * curr = command;
	int pos = 0;
	while(curr[pos] != ' ') { // Find the spaces after the first command 
		if(pos >= length)
			return -1;
		++pos;
	}
	while(curr[pos] == ' ') { // Replaces spaces after first command with '\0'
		if(pos >= length)
			return -1;
		curr[pos] = '\0';
		++pos;
	}
	// Populate the command structure
	c_struct->command = command;
	c_struct->command_length = strlen(command);
	c_struct->command_code = command_code(command, c_struct->command_length);
	c_struct->command_param = command+pos;
	c_struct->param_length = strlen(curr);
	return 0;
}

// This converts user commands into a command code.
int command_code(char * command, int length) {
	for(int i = 0; i < NUM_CMDS; ++i) {
		if(check_command(CMD_LIST[i], strlen(CMD_LIST[i]), command, length))
			return i+1;
	}
	return 0;
}
// ret 1 for match
int check_command(const char * reference, int r_length, char * input, int length) {
	if(length < r_length) return 0;
	int checked = 0;
	for(int i = 0; i < min(length, r_length); ++i) {
		checked += reference[i] ^ input[i];
	}
	if(checked == 0)
		return 1;
	return 0;
}


// Below contains testing code, just ignore it.

void test_CommandParser() {
	test_command_code();
	test_split_command();
}

void test_command_code() {
	printf("Command: [%s][%d] result %d\n", "bgkill", 6, command_code("bgkill", 6));
	printf("Command: [%s][%d] result %d\n", "bgstop", 6, command_code("bgstop", 6));
	printf("Command: [%s][%d] result %d\n", "bgstart", 7, command_code("bgstart", 7));
	printf("Command: [%s][%d] result %d\n", "bgkillz", 7, command_code("bgkillz", 7));
	printf("Command: [%s][%d] result %d\n", "bgstopz", 7, command_code("bgstopz", 7));
	printf("Command: [%s][%d] result %d\n", "bgguns", 6, command_code("bgguns", 6));
	printf("Command: [%s][%d] result %d\n", "orgazs", 6, command_code("orgazs", 6));
	printf("Command: [%s][%d] result %d\n", "remedepe", 8, command_code("remedepe", 8));
	printf("Command: [%s][%d] result %d\n", "bgxstart", 8, command_code("bgxstart", 8));
	printf("Command: [%s][%d] result %d\n", "bgstartx", 8, command_code("bgstartx", 8));
	printf("Command: [%s][%d] result %d\n", "bg ", 6, command_code("bg ", 6));
}

void test_split_command() {
	command_struct cstruct1;
	char str1[] = "bgkill               12345";
	fprintf(stderr, "Split: [%s] results in ", str1);	
	process_input(str1, strlen(str1), &cstruct1);
	fprintf(stderr, "[%s] and [%s]\n", cstruct1.command, cstruct1.command_param); 
}
