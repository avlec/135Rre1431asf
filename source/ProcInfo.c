#include <stdio.h>
#include <string.h>

typedef struct ProcInfo {
	char comm[1024];
	char state;
	long unsigned int utime;
	long unsigned int stime;
	long int rss;
	int voluntary_context_switches;
    int nonvoluntary_context_switches;
} ProcInfo;

int main()
{
    ProcInfo p_info;
    char str[64];
	sprintf(str, "/proc/%d/stat", 555);
	FILE * proc_pid_stat = fopen(str, "r");
	if(proc_pid_stat == NULL) {
	    printf("\n");
	    return;
	}
    
	int matches = fscanf(proc_pid_stat, " %*d %s %c %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %lu %lu %*ld %*ld %*ld %*ld %*ld %*ld %*llu %*lu %ld %*lu %*lu %*lu %*lu %*lu %*lu %*lu %*lu %*lu %*lu %*lu %*lu %*lu %*d %*d %*u %*u %*llu %*lu %*ld",
			p_info.comm, &p_info.state, &p_info.utime, &p_info.stime, &p_info.rss);
    

	sprintf(str, "/proc/%d/status", 555);
	FILE * proc_pid_status = fopen(str, "r");

    if(proc_pid_status == NULL) {
	    printf("AHHH\n");
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

    return 0;
}
