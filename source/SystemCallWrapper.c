#include "header/SystemCallWrapper.h"

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
		fprintf(stderr, "\n");
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
	pid_t ret = kill(process->pid, SIGTERM);
	if(ret != 0) {
		fprintf(stderr, "ERR: Killing process %d:%s failed.\n", process->pid, process->command_struct.command_params[1]);
		return;
	}
	free_process(process);
}

void exec_stop_process(ProcessNode * process) {	
	pid_t pid = kill(process->pid, SIGSTOP);
	if(pid != 0) {
		fprintf(stderr, "ERR: Stopping process %d:%s failed", process->pid, process->command_struct.command_params[1]);
		return;
	}
}

void exec_start_process(ProcessNode * process) {
	pid_t pid = kill(process->pid, SIGCONT);
	if(pid == 0)
		return;
	else
		fprintf(stderr, "ERR: Starting process %d:%s failed.", process->pid, process->command_struct.command_params[1]);
}

// Temporary Structure for holding Process info.
typedef struct ProcInfo {
	char comm[256];
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
	ProcInfo p_info = { .state=' ', .utime=0, .stime=0, .rss=-1, .voluntary_context_switches=-1, .nonvoluntary_context_switches=-1};
    char str[64];
	sprintf(str, "/proc/%d/stat", process->pid);
	FILE * proc_pid_stat = fopen(str, "r");
	if(proc_pid_stat == NULL) {
	    fprintf(stderr, "ERR: %s\n", "Opening /proc/.../stat.");
	    return;
	}
    
    int matches = 0;
    matches += fscanf(proc_pid_stat, "%*d %s %c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %*d %*d %*d %*d %*d %*d %*u %*u %ld",
			p_info.comm, &p_info.state, &p_info.utime, &p_info.stime, &p_info.rss);
	
    sprintf(str, "/proc/%d/status", process->pid);
	FILE * proc_pid_status = fopen(str, "r");
    if(proc_pid_status == NULL) {
	    fprintf(stderr, "ERR: %s\n", "Opening /proc/.../status.");
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

	if(matches != 7) { // Make sure that it read the files properly
		fprintf(stderr, "ERR: Reading /proc/%d/stat and or /proc/%d/status.", process->pid, process->pid);
		return;
	}

	fprintf(stdout, "Comm: %s\nState: %c (%s)\nUTime: %lf\nSTime: %lf\nRSS: %ld\nVoluntary Context Switches: %d\nNonVoluntary Context Switches: %d\n",
			p_info.comm, p_info.state, find_state(p_info.state),
			((double)p_info.utime)/((double)sysconf(_SC_CLK_TCK)),
			((double)p_info.stime)/((double)sysconf(_SC_CLK_TCK)),
			p_info.rss, p_info.voluntary_context_switches, p_info.nonvoluntary_context_switches);
	// TODO CONVERSION
    // ((double)p_info.utime)/((double)sysconf(_SC_CLK_TCK)), ((double)p_info.stime)/((double)sysconf(_SC_CLK_TCK))

	// Close files
	fclose(proc_pid_stat);
	fclose(proc_pid_status);
}

// Returns true for exists, false for doesn't
bool exec_check_process(int pid) {
	int temporary_status = 0;
	return (waitpid(pid, &temporary_status, WNOHANG) != -1);
}