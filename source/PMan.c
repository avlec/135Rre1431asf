// System Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// System Call Libraries
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// User Libraries
#include "header/Tokenizer.h"
#include "header/CommandTypes.h"
#include "header/ProcessLinkedList.h"

#define DEBUG
#define TESTS_off

#define PMAN_LINE  	"PMan > "
#define ERROR_LINE	"ERR: %s\n"
#define MSG_LINE	"MSG: %s\n"

void exec_new_process(ProcessNode * process) {
	if(process->command_struct.command_params[0] == NULL)
		return;

	int child_pid = fork();
	if(child_pid == 0) {
		#ifdef DEBUG
		fprintf(stderr, "Execing %s, starting with %s\n", process->command_struct.command_params[0], process->command_struct.command_params[1]);
		#endif
		execv(process->command_struct.command_params[0],
				process->command_struct.command_params);
		perror("execv");
		_exit(0);
	}
	process->pid = child_pid;
}

void free_process(ProcessNode * process) {
	if(process == NULL)
		return;
	free(process);
}

void exec_kill_process(ProcessNode * process) {
	// Do shit
	pid_t ret = kill(process->pid, SIGTERM);
	if(ret == 0)
		free_process(process);
	else
		fprintf(stderr, "ERR: Killing process %d:%s failed.\n", process->pid, process->command_struct.command_params[1]);
	// Write to buffer
}

void exec_stop_process(ProcessNode * process) {
	// Do shit
	pid_t pid = kill(process->pid, SIGSTOP);
	if(pid == 0)
		return;
	else
		fprintf(stderr, "ERR: Killing process %d:%s failed", process->pid, process->command_struct.command_params[1]);
	// Write to buffer
}

void exec_start_process(ProcessNode * process) {
	// Do shit
	pid_t pid = kill(process->pid, SIGCONT);
	if(pid == 0)
		return;
	else
		fprintf(stderr, "ERR: Starting process %d:%s failed.", process->pid, process->command_struct.command_params[1]);
	// Write to buffer
}

// Temporary Structure for holding Process info.
typedef struct ProcInfo {
	char * comm;
	char state;
	long unsigned int utime;
	long unsigned int stime;
	long int rss;
	int voluntary_context_switches;
    int nonvoluntary_context_switches;
} ProcInfo;

const char StateNames [12][38] = {
	"Running.", // R
	"Sleeping in an interruptable wait.", // S
	"Waiting in uninterruptible disk sleep", // D
	"Zombie.", // Z
	"Stopped", // T
	"Tracing stop.", // t
	"Paging.", // W
	"Dead.", // X | x
	"Wakekill.", // K
	"Waking.", // W
	"Parked.", // P
	"DNE"
};

const int StateCharToNameMap [12][2] = {
	{ 'R', 0 },
	{ 'S', 1 },
	{ 'D', 2 },
	{ 'Z', 3 },
	{ 'T', 4 },
	{ 't', 5 },
	{ 'W', 6 },
	{ 'X', 7 },
	{ 'x', 7 },
	{ 'K', 8 },
	{ 'W', 9 },
	{ 'P', 10 },
};

const char * find_state(char StateChar) {
	for(int i = 0; i < 11; ++i) {
		if(StateCharToNameMap[i][0] == StateChar)
			return StateNames[StateCharToNameMap[i][1]];
	}
	return StateNames[11];
}

void exec_process_status(ProcessNode * process) {
	ProcInfo p_info;
    char str[64];
	sprintf(str, "/proc/%d/stat", process->pid);
	FILE * proc_pid_stat = fopen(str, "r");
	if(proc_pid_stat == NULL) {
	    printf(ERROR_LINE, "Opening /proc/.../stat.");
	    return;
	}
    
	int matches = fscanf(proc_pid_stat, " %*d %s %c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %*d %*d %*d %*d %*d %*d %*u %*u %ld %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %*d %*d %*u %*u %*u %*u %*d",
			p_info.comm, &p_info.state, &p_info.utime, &p_info.stime, &p_info.rss);
    

	sprintf(str, "/proc/%d/status", process->pid);
	FILE * proc_pid_status = fopen(str, "r");

    if(proc_pid_status == NULL) {
	    printf(ERROR_LINE, "Opening /proc/.../status.");
	    return;
	}
	
    char buffer[64];
    while(fgets(buffer, sizeof buffer, proc_pid_status) != NULL) {
        if(strncmp(buffer, "voluntary_ctxt_switches", 23) != 0)
            continue;
        else {
            matches += sscanf(buffer, "%*s %d", &p_info.voluntary_context_switches);
            fgets(buffer, sizeof buffer, proc_pid_status);
            matches += sscanf(buffer, "%*s %d", &p_info.nonvoluntary_context_switches);
        }
	}

	fprintf(stdout, "Comm: %s\nState: %c (%s)\nUTime: %ld\nSTime: %ld\nRSS: %ld\nVoluntary Context Switches: %d\nNonVoluntary Context Switches: %d\n",
			p_info.comm, p_info.state, find_state(p_info.state), p_info.utime, p_info.stime, p_info.rss, p_info.voluntary_context_switches, p_info.nonvoluntary_context_switches);
	// ((double)p_info.utime)/((double)sysconf(_SC_CLK_TCK)), ((double)p_info.stime)/((double)sysconf(_SC_CLK_TCK))
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
	int running = 1;

	LinkedList l_list;
	list_init(&l_list);

	while(running) {
		// Print CLI line heading
		printf(PMAN_LINE); 
	
		// Clear buffers
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
		char ** tokens = stot(buffer, strlen(buffer), ' ');
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
				} else
					fprintf(stderr, ERROR_LINE, "Executing new process.");
				break;

			case LIST_PROCESSES:
				fprintf(stdout, MSG_LINE, "Listing processes.");
				list_print(l_list);
				break;

			case KILL_PROCESS:
				fprintf(stdout, MSG_LINE, "Killing process.");

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
				exec_kill_process(process);
				break;

			case STOP_PROCESS:
				fprintf(stdout, MSG_LINE, "Stopping process.");

				if(sscanf(cmd_struct.command_params[0], "%d", &pid) == 0) {
					fprintf(stderr, ERROR_LINE, "Expected pid of process to stop.");
					break;
				}

				process = list_find(l_list, pid);
				if(process == NULL) // Error check
					fprintf(stderr, ERROR_LINE, "Process not found");
				
				// Perform System Calls
				exec_stop_process(process);
				break;

			case START_PROCESS:
				fprintf(stdout, MSG_LINE, "Starting process.");
				
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
				exec_start_process(process);
				break;

			case PROCESS_STATUS:
				fprintf(stdout, MSG_LINE, "Get process status.");
				
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
				exec_process_status(process);
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
