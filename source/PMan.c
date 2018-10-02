// System Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

// User Libraries
#include "header/Tokenizer.h"
#include "header/CommandCheck.h"
#include "header/ProcessLinkedList.h"
#include "header/SystemCallWrapper.h"

#define DEBUG_off
#define TESTS_off

#define PMAN_LINE  	"PMan > "
#define ERROR_LINE	"ERR: %s\n"
#define MSG_LINE	"MSG: %s\n"

int buffer_size = 256;
char * buffer = NULL;
LinkedList l_list;



void signal_callback_handler(int signum) {
	list_free(l_list);
	fprintf(stdout, "\n");
	exit(0);
}

int main(int argc, char ** argv) {

	// Register Signal Callback Handler
	signal(SIGINT, signal_callback_handler);
	int running = 1;
	list_init(&l_list);

	while(running) {
		// Print CLI line heading
		printf(PMAN_LINE); 
	
		// Alloc & Clear buffers
		buffer = (char *) malloc(buffer_size * sizeof(char));
		if(buffer == NULL) {
			fprintf(stderr, ERROR_LINE, "Memory allocation.");
			exit(1);
		}
		memset(buffer, '\0', buffer_size);
		
		// Read User Input
		if(fgets(buffer, buffer_size, stdin) == NULL)
			break;
		for(int i = 0; i < buffer_size; ++i)
			if(buffer[i] == '\n')
				buffer[i] = '\0';
		#ifdef DEBUG
		fprintf(stderr, "Input {%lu|%s} yields\n", strlen(buffer), buffer);	
		#endif
		
		//Tokenize input new
		CommandStruct cmd_struct;		
		char ** tokens = stot(buffer, strlen(buffer), ' '); // bts malloc?
		if(tokens == NULL) { // ERROR
			cmd_struct.command = NULL;
			cmd_struct.command_length = 0;
			cmd_struct.command_code = INVALID_COMMAND; 
			cmd_struct.command_params = NULL;
		} else {			 // NO ERROR!
			cmd_struct.command = tokens[0];
			cmd_struct.command_length = strlen(tokens[0]);
			cmd_struct.command_code = command_code(tokens[0], cmd_struct.command_length);
			cmd_struct.command_params = tokens+1;		
		}
		
		#ifdef DEBUG
		// Print CMD structure.
		fprintf(stderr, "├Command[%d]:%s\n├Command Code:%d\n└Params\n",
				cmd_struct.command_length, cmd_struct.command,
				cmd_struct.command_code);
		int i = 0;
		if(cmd_struct.command_params != NULL) { // Don't index a NULL array...
			while(cmd_struct.command_params[i] != NULL) {
				fprintf(stderr, "  %s%s\n", (cmd_struct.command_params[i+1] == NULL ? "└" : "├"), cmd_struct.command_params[i]);
				++i;
			}
		}
		#endif
	
	
		ProcessNode * process = NULL;	
		int pid = 0;
		
		// Do things with command parameters
		switch(cmd_struct.command_code) {
			case NEW_PROCESS:
				fprintf(stdout, MSG_LINE, "New process.");
				// Initialize the process node
				process = (ProcessNode *) malloc(sizeof(ProcessNode));
				process->pid = -1;
				process->command_struct = cmd_struct;
				process->next = NULL;
				process->prev = NULL;
				
				// Perform System Calls (this assigns it a PID)
				exec_new_process(process);
				
				// Add it to the linked list (this assigns next/prev
				if(process->pid != -1) {
					list_add(&l_list, process);
					fprintf(stdout, "[%d]\n", process->pid);
				} else {
					fprintf(stderr, ERROR_LINE, "Executing new process.");
					free(process);
				}
				break;

			case LIST_PROCESSES:
				fprintf(stdout, MSG_LINE, "Listing processes.");
				list_print(&l_list);
				break;

			case KILL_PROCESS:
				fprintf(stdout, MSG_LINE, "Killing process.");

				if(cmd_struct.command_params[0] == NULL) {
					fprintf(stderr, ERROR_LINE, "Expected pid of the process of which to display status.");
					break;
				}
				if(sscanf(cmd_struct.command_params[0], "%d", &pid) == 0) {
					fprintf(stderr, ERROR_LINE, "Expected pid of the process to kill.");
					break;
				}

				process = list_remove(&l_list, pid);
				if(process == NULL) { // Error check
					fprintf(stderr, ERROR_LINE, "Process not found.");
					break;
				}
				// Perform System Calls
				if(exec_kill_process(process) == false) {
					list_remove(&l_list, process->pid);
					fprintf(stdout, "Notice: Process has exited prior to this action.\n");
				}
				
				break;

			case STOP_PROCESS:
				fprintf(stdout, MSG_LINE, "Stopping process.");

				if(cmd_struct.command_params[0] == NULL) {
					fprintf(stderr, ERROR_LINE, "Expected pid of the process of which to display status.");
					break;
				}
				if(sscanf(cmd_struct.command_params[0], "%d", &pid) == 0) {
					fprintf(stderr, ERROR_LINE, "Expected pid of process to stop.");
					break;
				}

				process = list_find(l_list, pid);
				if(process == NULL) // Error check
					fprintf(stderr, ERROR_LINE, "Process not found");
				
				// Perform System Calls
				if(exec_stop_process(process) == false) {
					list_remove(&l_list, process->pid);
					fprintf(stdout, "Notice: Process has exited prior to this action.\n");
				}
				break;

			case START_PROCESS:
				fprintf(stdout, MSG_LINE, "Starting process.");
				
				if(cmd_struct.command_params[0] == NULL) {
					fprintf(stderr, ERROR_LINE, "Expected pid of the process of which to display status.");
					break;
				}
				if(sscanf(cmd_struct.command_params[0], "%d", &pid) == 0) {
					fprintf(stderr, ERROR_LINE, "Expected pid of process to start.");
					break;
				}
				
				process = list_find(l_list, pid);
				if(process == NULL) { // Error Check
					fprintf(stderr, ERROR_LINE, "Process not found.");
					break;
				}
				// Perform System Calls
				if(exec_start_process(process) == false) {
					list_remove(&l_list, process->pid);
					fprintf(stdout, "Notice: Process has exited prior to this action.\n");
				}
				break;

			case PROCESS_STATUS:
				fprintf(stdout, MSG_LINE, "Get process status.");
				
				if(cmd_struct.command_params[0] == NULL) {
					fprintf(stderr, ERROR_LINE, "Expected pid of the process of which to display status.");
					break;
				}
				if(sscanf(cmd_struct.command_params[0], "%d", &pid) == 0) {
					fprintf(stderr, ERROR_LINE, "Expected pid of the process of which to display status.");
					break;
				}
				
				process = list_find(l_list, pid);
				if(process == NULL) { // Error check
					fprintf(stderr, ERROR_LINE, "Getting process status, process not found");
					break;
				}

				// Perform System Calls
				if(exec_process_status(process) == false) {
					list_remove(&l_list, process->pid);
					fprintf(stdout, "Notice: Process has exited prior to this action.\n");
				}
				break;
	
			case INVALID_COMMAND:
			default: // Input error handling
				printf(ERROR_LINE, "Invalid input.");
		}
		
		// Print Response from output buffer.
	}
}