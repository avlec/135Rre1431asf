#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header/Tokenizer.h"
#include "header/CommandTypes.h"
#include "header/ProcessLinkedList.h"

#define DEBUG_off
#define TESTS_off

#define PMAN_LINE  	"PMan > "
#define ERROR_LINE 	"ERR: %s\n"
#define MSG_LINE	"MSG: %s\n"

//TODO AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
void exec_new_process(char * output_buffer, ProcessNode * process) {
	// Do shit

	// Write to buffer
}

void free_process(ProcessNode * process) {
	if(process == NULL)
		return;
	free(process);
}

void exec_kill_process(char * output_buffer, ProcessNode * process) {
	// Do shit
	
	free_process(process);
	// Write to buffer
}

void exec_stop_process(char * output_buffer, ProcessNode * process) {
	// Do shit

	// Write to buffer
}

void exec_start_process(char * output_buffer, ProcessNode * process) {
	// Do shit

	// Write to buffer
}

void exec_process_status(char * output_buffer, ProcessNode * process) {
	// Do shit

	// Write to buffer
}


#ifndef min
	#define min(a,b) ((a) > (b) ? (a) : (b))
#endif

int command_code(char * command, int length);
int check_command(const char * reference, int r_length, char * input, int length);

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



int main(int argc, char ** argv) {
	
	// Initialize.
	
	int buffer_size = 256;
	char * buffer = (char *) malloc(buffer_size * sizeof(char));
	int output_buffer_size = 256;
	char * output_buffer = (char *) malloc(output_buffer_size * sizeof(char));
	int running = 1;

	#ifdef TESTS
	test_command_code();
	test_split_command();
	#endif	
	
	LinkedList l_list;
	list_init(&l_list);

	while(running) {
		// Print CLI line heading
		printf(PMAN_LINE); 
	
		// Clear buffers
		memset(buffer, '\0', buffer_size);
		memset(output_buffer, '\0', output_buffer_size);
		
		// Read User Input
		if(fgets(buffer, buffer_size, stdin) == NULL)
			break;
		for(int i = 0; i < buffer_size; ++i)
			if(buffer[i] == '\n')
				buffer[i] = '\0';
		#ifdef DEBUG
		fprintf(stderr, "Command {%s} yields\n", buffer);	
		#endif
		
		//Tokenize input new
		command_struct cmd_struct;		
		char ** tokens = stot(buffer, strlen(buffer), ' ');
		if(tokens == NULL) { // TODO ERROR
			cmd_struct.command = NULL;
			cmd_struct.command_length = 0;
			cmd_struct.command_code = INVALID_COMMAND; 
			cmd_struct.command_params = NULL;
		} else {
			cmd_struct.command = tokens[0];
			cmd_struct.command_length = strlen(tokens[0]);
			cmd_struct.command_code = command_code(tokens[0], cmd_struct.command_length);
			cmd_struct.command_params = tokens+1;		
		}
		
		#ifdef DEBUG
		fprintf(stderr, "\t> command[%s(%d)|%d]\n",
				cmd_struct.command,
				cmd_struct.command_length,
				cmd_struct.command_code);
		#endif
	
	
		ProcessNode * process = NULL;	
		int pid = 0;
		
		// Do things with command parameters
		switch(cmd_struct.command_code) {
			case NEW_PROCESS:
				#ifdef DEBUG
				fprintf(stderr, MSG_LINE, "New process.");
				#endif
				// Create the process object
				//HERE
				
				// Add it to the linked list
				list_add(&l_list, pid);
				
				// Perform System Calls
				exec_new_process(output_buffer, process);
				break;

			case LIST_PROCESSES:
				#ifdef DEBUG
				fprintf(stderr, MSG_LINE, "Listing processes.");
				#endif
				list_print(l_list);
				break;

			case KILL_PROCESS:
				#ifdef DEBUG
				fprintf(stderr, MSG_LINE, "Killing process.");
				#endif
				// Get the process object
				process = list_remove(&l_list, pid);
				if(process == NULL) // Error check
					fprintf(stderr, ERROR_LINE, "Killing process, process not found");
				
				// Perform System Calls
				exec_kill_process(output_buffer, process);
				break;

			case STOP_PROCESS:
				#ifdef DEBUG
				fprintf(stderr, MSG_LINE, "Stopping process.");
				#endif
				process = list_find(l_list, pid);
				if(process == NULL) // Error check
					fprintf(stderr, ERROR_LINE, "Stopping process, process not found");
				
				// Perform System Calls
				exec_stop_process(output_buffer, process);
				break;

			case START_PROCESS:
				#ifdef DEBUG
				fprintf(stderr, MSG_LINE, "Starting process.");
				#endif
				process = list_find(l_list, pid);
				if(process == NULL) // Error Check
					fprintf(stderr, ERROR_LINE, "Starting process, process not found");
				
				// Perform System Calls
				exec_start_process(output_buffer, process);
				break;

			case PROCESS_STATUS:
				#ifdef DEBUG
				fprintf(stderr, MSG_LINE, "Get process status.");
				#endif
				process = list_find(l_list, pid);
				if(process == NULL) // Error check
					fprintf(stderr, ERROR_LINE, "Getting process status, process not found");
				
				// Perform System Calls
				exec_process_status(output_buffer, process);
				break;
	
			case INVALID_COMMAND:	
			default: // Input error handling
				printf(ERROR_LINE, "Invalid input.");	
		}
		
		// Print Response from output buffer.
		if(tokens != NULL)
			free(tokens);
	}

	// Cleanup
	free(buffer);
}
