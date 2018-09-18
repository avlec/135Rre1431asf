#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header/CommandParser.h"
#include "header/ProcessStructure.h"

#define DEBUG_OFF

#define PMAN_LINE  "PMan > "
#define ERROR_LINE "ERR: %s\n"

//TODO AAAAAAAAAAAAAAAAAAAAAAAAAAAAA
void exec_new_process(char * output_buffer, process_node * process) {
	// Do shit

	// Write to buffer
}

void free_process(process_node * process) {
	if(process == NULL)
		return;
	free(process);
}

void exec_kill_process(char * output_buffer, process_node * process) {
	// Do shit
	
	free_process(process);
	// Write to buffer
}

void exec_stop_process(char * output_buffer, process_node * process) {
	// Do shit

	// Write to buffer
}

void exec_start_process(char * output_buffer, process_node * process) {
	// Do shit

	// Write to buffer
}

void exec_process_status(char * output_buffer, process_node * process) {
	// Do shit

	// Write to buffer
}

int main(int argc, char ** argv) {
	
	// Initialize.
	
	int buffer_size = 256;
	char * buffer = (char *) malloc(buffer_size * sizeof(char));
	int output_buffer_size = 256;
	char * output_buffer = (char *) malloc(output_buffer_size * sizeof(char));

	int running = 1;

	#ifdef DEBUG
	test_command_code();
	test_split_command();
	#endif	
	
	process_node * root_node = NULL;
	process_node * end_node = NULL;

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
		fprintf(stderr, "Command { %s } yields\n", buffer);	
		#endif
		
		// Process Input		
		command_struct cmd_struct;
		process_input(buffer, strlen(buffer), &cmd_struct);
		#ifdef DEBUG
		fprintf(stderr, "\t> command[%s(%d)|%d] with param[%s(%d)]\n", cmd_struct.command, cmd_struct.command_length, cmd_struct.command_code, cmd_struct.command_param, cmd_struct.param_length); 
		#endif
		
		process_node * process = NULL;	
		int pid = 0;
		// Do things with command parameters
		switch(cmd_struct.command_code) {
			case NEW_PROCESS:
				process = create_process(&cmd_struct);
				add_process(end_node, process);
				exec_new_process(output_buffer, process);
				break;
			case LIST_PROCESSES:
				print_processes(output_buffer, root_node);
				break;
			case KILL_PROCESS:
				process = remove_process(&root_node, pid);
				if(process == NULL)
					print_notfound(output_buffer, pid);
				exec_kill_process(output_buffer, process);
				break;
			case STOP_PROCESS:
				process = find_process(root_node, pid);
				if(process == NULL)
					print_notfound(output_buffer, pid);
				exec_stop_process(output_buffer, process);
				break;
			case START_PROCESS:
				process = find_process(root_node, pid);
				if(process == NULL)
					print_notfound(output_buffer, pid);
				exec_start_process(output_buffer, process);
				break;
			case PROCESS_STATUS:
				process = find_process(root_node, pid);
				if(process == NULL)
					print_notfound(output_buffer, pid);
				exec_process_status(output_buffer, process);
				break;
	
			case INVALID_COMMAND:	
			default:
				printf(ERROR_LINE, "Invalid input.");	
		}
		
		// Print Response from output buffer.
	}

	// Cleanup
	free(buffer);
}
