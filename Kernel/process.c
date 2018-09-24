#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "process.h"
#include "include/RoundRobin.h"
#include <naiveConsole.h>
#include "time.h"
#include "buddy.h"
#include <sysCalls.h>

proc current_proc;
int max_pid;
struct process process_table[NUM_PROCESS];
tHeader * ready_queue;

int time(uint64_t timeType);

void init_process() {
    ready_queue = mymalloc(sizeof(tHeader));
    process_table[0].name = "init";
    process_table[0].pid = 0;
    process_table[0].state = RUN;
    process_table[0].parent = NULL;
    process_table[0].rsp = 0;
    current_proc = &process_table[0];
    process_table[0].occupied = 1;
}

void start_proc(char *proc_name, void (*function)(int argc, char *argv[])) {
    int index_proc = get_new_index();

    proc process = &process_table[index_proc];
    //SS Stack Segment
    process->stack[STACK_SIZE - 1] = 0x0;
    //RSP
    process->stack[STACK_SIZE - 2] = (uint64_t) &process->stack[STACK_SIZE - 1];
    //RFLAGS
    //bit 1 : always 1 (EFLAGS)
    //bit 2 : Parity bit
    //bit 9 : Interrupt enable
    process->stack[STACK_SIZE - 3] = 0x206;
    //CS (code segment)
    //I'm not sure of this one but 8 is the value before interrupt was called
    //CS hasn't a weird signification in 64 bit mode let's say it'll work for now
    process->stack[STACK_SIZE - 4] = 0x8;
    //RIP
    process->stack[STACK_SIZE - 5] = (uint64_t) function;
    //RBP
    process->stack[STACK_SIZE - 16] = (uint64_t) &process->stack[STACK_SIZE - 1];
    //16 registers previously push
    //5 registers that will be poped by IRETQ
    process->rsp = (uint64_t) &process->stack[STACK_SIZE - 20];
    process->pid = get_new_pid();
    process->state = READY;
    process->parent = get_current_proc();
    process->name= proc_name;

    tNode *node = mymalloc(sizeof(tNode));
    node-> p = process;
    add_to_queue(ready_queue, node);

    process->occupied = 1;
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

int test = 0;
uint64_t contextSwitch(uint64_t rsp) {
    timerHandler();
    current_proc->rsp = rsp;
    if (test && process_table[1].occupied) {
        current_proc = &process_table[1];
    } else if (!test && process_table[2].occupied) {
        current_proc = &process_table[2];
    }
    test = !test;
    return current_proc->rsp;
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
