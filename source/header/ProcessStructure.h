#ifndef PROCESS_STRUCTURE_H
#define PROCESS_STRUCTURE_H

#include "CommandTypes.h"

typedef struct process_node process_node;
struct process_node {
	int pid;
	command_struct cmd_struct;
	process_node * next;
};

process_node * create_process(command_struct * cmd_struct);
void add_process(process_node * end_node, process_node * new_node);
void print_notfound(char * buffer, int pid);
void print_process(process_node * process);
void print_processes(char * buffer, process_node * root_node);
process_node * find_process(process_node * root_node, int pid);
process_node * remove_process_r(process_node * root_node, int pid);
process_node * remove_process(process_node ** root_node_ref, int pid);

#endif

