#ifndef COMMAND_TYPES_H
#define COMMAND_TYPES_H
/*	This struct holds two references to a single string, and the segment lengths.
eg. 
	bgkill 	1932
	^		^
	command command_param
*/
#define NUM_COMMANDS 6
#define COMMANDS "bg","bglist","bgkill","bgstop","bgstart","pstat"
typedef enum {
	INVALID_COMMAND,
	NEW_PROCESS,
	LIST_PROCESSES,
	KILL_PROCESS,
	STOP_PROCESS,
	START_PROCESS,
	PROCESS_STATUS	
} CommandCodeType;

typedef struct command_struct {
	char * command;
	int command_length;
	CommandCodeType command_code;
	char ** command_params;
} command_struct;
#endif
