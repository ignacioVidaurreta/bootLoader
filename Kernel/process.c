#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "process.h"
#include "include/RoundRobin.h"
#include <naiveConsole.h>

proc current_proc;
int max_pid;
struct process process_table[NUM_PROCESS];
tHeader * ready_queue;

void start_proc(char* proc_name, void (*function)(int argc, char *argv[])) {
    int index_proc = get_new_index();

    proc process = &process_table[index_proc];
    process->occupied = 1;
    process->stack[2] = function;
    process->rsp = process->stack[17];
    process->pid = get_new_pid();
    process->state = READY;
    process->parent = get_current_proc();
    process->name= proc_name;
}

int get_new_index() {
    for (int i = 0; i < NUM_PROCESS; i++) {
        if (!process_table[i].occupied) {
            return i;
        }
    }
    return -1;
}

int get_new_pid() {
    max_pid++;
    return max_pid;
}

proc get_current_proc() {
    return current_proc;
}

uint64_t contextSwitch(uint64_t rsp) {
    current_proc->rsp = rsp;

    scheduler_test();

    return current_proc->rsp;
}

int test = 0;
void scheduler_test() {
    if (test) {
        current_proc = &process_table[1];
    } else {
        current_proc = &process_table[0];
    }
    test = !test;
}

void print_proc(){
    int num_printed=0;

    ncPrint("PID        NAME        TIME");
    ncScroll();
    for(int i = 0; i<NUM_PROCESS; i++){
        if (process_table[i].occupied){
            num_printed++;
            ncPrintBase(process_table[i].pid, 10);
            ncPrint("          ");
            ncPrint(process_table[i].name);
            ncPrint("          ");
            ncPrintBase(time(4), 10);
            ncPrint(":");
            ncPrintBase(time(3), 10);
            ncPrint(":");
            ncPrintBase(time(2), 10);
            ncScroll();


        }
    }
    if(num_printed == 0){
        ncPrint("No hay procesos!! [PLACEHOLDER ]");
        ncScroll();
    }

}
