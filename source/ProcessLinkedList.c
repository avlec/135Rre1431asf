#include <stdio.h>
#include <stdlib.h>

#include "header/ProcessLinkedList.h"

void list_init(LinkedList * l_list) {
	l_list->head = NULL;
	l_list->tail = NULL;
	l_list->size = 0;
}

/*	Adds a node with pid to the list
	if the node is in the list returns -1, and does nothing.
	otherwise node is added to the list and returns 0
*/
int list_add(LinkedList * l_list, int pid) {
	// Check if duplicate
	if(list_find(*(l_list), pid) == NULL)
		return -1;

	ProcessNode * new_node = (ProcessNode *) malloc(sizeof(ProcessNode));
	new_node->pid = pid;
	new_node->next = NULL;
	new_node->prev = NULL;

	ProcessNode * t_node = l_list->tail;
	if(t_node == NULL) {
		l_list->head = new_node;
		l_list->tail = new_node;
	} else { 
		t_node->next = new_node;
		new_node->prev = t_node;
		l_list->tail = new_node;
	}
	++l_list->size;
	return 0;
}

/* 	Searches the linkedlist for node with PID = PID
	Returns the PID of the node if success, otherwise -1
*/
ProcessNode * list_find(LinkedList l_list, int pid) {
	ProcessNode * node = l_list.head;
	while(node != NULL) {
		if((node->pid == pid))
			return node;
		node = node->next;
	}
	return NULL;
}

/* 	Removes the node with PID = PID
	if the node isn't in the list, returns -1
	if the list has no nodes in it, returns -1
	otherwise, node is removed, linkedlist is updated, and returns PID
*/
ProcessNode * list_remove(LinkedList * l_list, int pid) {
	ProcessNode * node = l_list->head;
	
	// Case where there is one node
	if(l_list->size == 1) {
		if(l_list->head->pid == pid) {
			// Basically just empty the linkedlist & free memory
			l_list->head = NULL;
			l_list->tail = NULL;
			--l_list->size;
			return node;
		}
	}
	
	// Cases wher there are more than one node
	if(l_list->size > 1) {

		// Look through the list and check if it's not been found at all.
		while(node->pid != pid)
			if((node = node->next) == NULL)
				return NULL;
		
		// Account for special cases
		if(node == l_list->head) {
			l_list->head = node->next;
			l_list->head->prev = NULL;
		} else if(node == l_list->tail) {
			l_list->tail = node->prev;
			l_list->tail->next = NULL;
		} else {
			node->prev->next = node->next;
			node->next->prev = node->prev;
		}

		// Update the list counter
		--l_list->size;

		// Return the appropriate PID
		return node;
	}

	return NULL;
}


/* Prints the contents of the given linkedlist to stderr
*/
void list_print(LinkedList l_list) {
	ProcessNode * p_node = l_list.head;

	fprintf(stderr, "\nPrinting List of size %d\n", l_list.size);
	for(int i = 0; i < l_list.size; ++i) {
		fprintf(stderr, "@[%d] { pid:%d }\n", i, p_node->pid);
		p_node = p_node->next;
	}	
}
