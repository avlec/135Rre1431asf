#include <stdlib.h>

#include "header/ProcessStructure.h"

// THIS ALLOCATES MEMORY FOR A NEW PROCESS_NODE
process_node * create_process(command_struct * cmd_struct) {
	process_node * process = (process_node *) calloc(1, sizeof(process_node));
	// Fill Out Data
	process->pid = 0;
	process->cmd_struct = (* cmd_struct);
	process->next = NULL;
	
	return process;
}

void add_process(process_node * end_node, process_node * new_node) {
	end_node->next = new_node;
}

void print_notfound(char * buffer, int pid) {
	return;
}

// TODO
void print_process(process_node * process) {
	return;	
}

void print_processes(char * buffer, process_node * root_node) {
	if(root_node == NULL)
		return;
	print_process(root_node);
	print_processes(buffer, root_node->next);
}

process_node * find_process(process_node * root_node, int pid) {
	if(root_node == NULL)
		return NULL;
	if(root_node->pid == pid)
		return root_node;
	return find_process(root_node->next, pid);
}

// THIS DOES NOT FREE THE MEMORY ALLOCATED FOR THE PROCESS_NODE STRUCUTRE
// FREE_PROCESS MUST BE CALLED, AFTER DOING WHAT YOU WANT
process_node * remove_process_r(process_node * root_node, int pid) {
	if(root_node->next == NULL)
		return NULL;

	if(root_node->next->pid == pid) {
		process_node * process = root_node->next;
		root_node->next = NULL;
		return process;
	}
	if(root_node->next == NULL)
		return NULL;
	return remove_process_r(root_node->next, pid);
}
process_node * remove_process(process_node ** root_node_ref, int pid) {
	// This checks first node then moves to recursively checking next node.
	if((*root_node_ref) == NULL)
		return NULL;
	if((*root_node_ref)->pid == pid) {
		process_node * process = (*root_node_ref);
		*root_node_ref = NULL;
		return process;
	}
	if((*root_node_ref)->next == NULL)
		return NULL;
	return remove_process_r((*root_node_ref), pid);	
}
