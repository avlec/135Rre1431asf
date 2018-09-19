#include <stdio.h>
#include <stdlib.h>

#include "header/ProcessLinkedList.h"


int main(int argc, char ** argv) {
	
	LinkedList l_list = { .head = NULL, .tail = NULL, .size=0 };

	list_add(&l_list, 100);
	list_print(l_list);
	fprintf(stderr, "100:%d, 200:%d, 300:%d, 400:%d\n", list_find(l_list, 100), list_find(l_list, 200), list_find(l_list, 300), list_find(l_list, 400));
	list_add(&l_list, 200);
	list_print(l_list);
	fprintf(stderr, "100:%d, 200:%d, 300:%d, 400:%d\n", list_find(l_list, 100), list_find(l_list, 200), list_find(l_list, 300), list_find(l_list, 400));
	list_add(&l_list, 300);
	list_print(l_list);
	fprintf(stderr, "100:%d, 200:%d, 300:%d, 400:%d\n", list_find(l_list, 100), list_find(l_list, 200), list_find(l_list, 300), list_find(l_list, 400));
	list_remove(&l_list, 200);
	list_print(l_list);
	fprintf(stderr, "100:%d, 200:%d, 300:%d, 400:%d\n", list_find(l_list, 100), list_find(l_list, 200), list_find(l_list, 300), list_find(l_list, 400));
	list_add(&l_list, 400);
	list_print(l_list);
	fprintf(stderr, "100:%d, 200:%d, 300:%d, 400:%d\n", list_find(l_list, 100), list_find(l_list, 200), list_find(l_list, 300), list_find(l_list, 400));
	list_remove(&l_list, 300);
	list_print(l_list);
	fprintf(stderr, "100:%d, 200:%d, 300:%d, 400:%d\n", list_find(l_list, 100), list_find(l_list, 200), list_find(l_list, 300), list_find(l_list, 400));
	list_remove(&l_list, 200);
	list_print(l_list);
	fprintf(stderr, "100:%d, 200:%d, 300:%d, 400:%d\n", list_find(l_list, 100), list_find(l_list, 200), list_find(l_list, 300), list_find(l_list, 400));
	list_remove(&l_list, 100);
	list_print(l_list);
	fprintf(stderr, "100:%d, 200:%d, 300:%d, 400:%d\n", list_find(l_list, 100), list_find(l_list, 200), list_find(l_list, 300), list_find(l_list, 400));
	list_remove(&l_list, 400);
	list_print(l_list);
	fprintf(stderr, "100:%d, 200:%d, 300:%d, 400:%d\n", list_find(l_list, 100), list_find(l_list, 200), list_find(l_list, 300), list_find(l_list, 400));
}
