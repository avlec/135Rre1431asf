#ifndef PROCESS_LINKED_LIST_H
#define PROCESS_LINKED_LIST_H

#include "CommandTypes.h"

typedef struct ProcessNode ProcessNode;
struct ProcessNode {
	int pid;
	CommandStruct command_struct;
	ProcessNode * next;
	ProcessNode * prev;
};

typedef struct LinkedList {
	ProcessNode * head;
	ProcessNode * tail;
	int size;
} LinkedList;


void list_init(LinkedList * l_list);
int list_add(LinkedList * l_list, ProcessNode * new_node);
ProcessNode * list_find(LinkedList l_list, int pid);
ProcessNode * list_remove(LinkedList * l_list, int pid);
void list_print(LinkedList * l_list);
void list_free(LinkedList l_list);
#endif
