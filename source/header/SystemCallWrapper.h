#ifndef SYSTEM_CALL_WRAPPER_H
#define SYSTEM_CALL_WRAPPER_H

#include <stdbool.h>

#include "ProcessLinkedList.h"

void exec_new_process(ProcessNode * process);
void exec_kill_process(ProcessNode * process);
void exec_stop_process(ProcessNode * process);
void exec_start_process(ProcessNode * process);
void exec_process_status(ProcessNode * process);
bool exec_check_process(int pid);

#endif