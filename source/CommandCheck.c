#include <string.h>

#include "header/CommandCheck.h"

// End '\0' is if printf needs to be called
#define NUM_CMDS 6
const char * CMD_LIST[NUM_CMDS] = { COMMANDS };

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