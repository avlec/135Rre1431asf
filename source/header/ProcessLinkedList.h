#ifndef PROCESS_LINKED_LIST_H
#define PROCESS_LINKED_LIST_H

#include "CommandTypes.h"

typedef struct ProcessNode ProcessNode;
struct ProcessNode {
	int pid;
	ProcessNode * next;
	ProcessNode * prev;
};

typedef struct LinkedList {
	ProcessNode * head;
	ProcessNode * tail;
	int size;
} LinkedList;


void list_init(LinkedList * l_list);
int list_add(LinkedList * l_list, int pid);
ProcessNode * list_find(LinkedList l_list, int pid);
ProcessNode * list_remove(LinkedList * l_list, int pid);
void list_print(LinkedList l_list);

#endif
