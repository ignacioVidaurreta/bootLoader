#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "process.h"
#include "include/RoundRobin.h"
#include <naiveConsole.h>
#include "time.h"
#include "buddy.h"
#include "wait.h"
#include <sysCalls.h>

int max_pid;
struct process process_table[NUM_PROCESS];
tHeader * ready_queue;
proc current_proc;

int time(uint64_t timeType);

void init_process() {
    ready_queue = mymalloc(sizeof(tHeader));
    init_wait_queue();
    process_table[0].name = "init";
    process_table[0].pid = 0;
    process_table[0].state = RUN;
    process_table[0].parent = NULL;
    process_table[0].rsp = 0;
    process_table[0].fds = mymalloc(2*sizeof(int));
    process_table[0].fds[0] = DEFAULT_STDIN;
    process_table[0].fds[1] = DEFAULT_STDOUT;
    current_proc = &process_table[0];
    process_table[0].occupied = 1;
}

uint64_t start_proc(char *proc_name, void (*function)(int argc, char *argv[]), int argc, char* argv[]) {
    int index_proc = get_new_index();

    proc process = &process_table[index_proc];
    //Exit address
    process->stack[STACK_SIZE - 1] = (uint64_t) myexit;
    //SS Stack Segment
    process->stack[STACK_SIZE - 2] = 0x0;
    //RSP
    process->stack[STACK_SIZE - 3] = (uint64_t) &process->stack[STACK_SIZE - 1];
    //RFLAGS
    //bit 1 : always 1 (EFLAGS)
    //bit 2 : Parity bit
    //bit 9 : Interrupt enable
    process->stack[STACK_SIZE - 4] = 0x206;
    //CS (code segment)
    //I'm not sure of this one but 8 is the value before interrupt was called
    //CS hasn't a weird signification in 64 bit mode let's say it'll work for now
    process->stack[STACK_SIZE - 5] = 0x8;
    //RIP
    process->stack[STACK_SIZE - 6] = (uint64_t) function;
    //RBP
    process->stack[STACK_SIZE - 17] = (uint64_t) &process->stack[STACK_SIZE - 1];
    //16 registers previously push
    //5 registers that will be poped by IRETQ
    //Return address of exit
    process->rsp = (uint64_t) &process->stack[STACK_SIZE - 21];
    /*
     *  Por ahora sólo paso por registros
     */
    int i;
    for( i= 0 ; i< argc && i<6; i++){
      switch(i){
        case 0:
          process->stack[STACK_SIZE - 16] = (uint64_t)argv[i]; //rdi
          break;
        case 1:
          process->stack[STACK_SIZE - 15]= (uint64_t)argv[i]; //rsi
          break;
        case 2:
          process->stack[STACK_SIZE - 18]= (uint64_t)argv[i]; //rdx
          break;
        case 3:
          process->stack[STACK_SIZE - 19]= (uint64_t)argv[i]; //rcx
          break;
        case 4:
          process->stack[STACK_SIZE - 14]= (uint64_t)argv[i]; //r8
          break;
        case 5:
          process->stack[STACK_SIZE - 13]= (uint64_t)argv[i]; //r9
          break;
      }
    }


    process->pid = get_new_pid();
    process->state = READY;
    process->parent = get_current_proc();
    process->name = proc_name;
    process->fds = mymalloc(2*sizeof(int));
    process->fds[0] = process->parent->fds[0];
    process->fds[1] = process->parent->fds[1];

    tNode *node = mymalloc(sizeof(tNode));
    node-> p = process;
    add_to_queue(ready_queue, node);

    process->occupied = 1;

    return process->pid;
}

void myexit(uint64_t retval) {
    end_process();

    while(1) {}
}

void end_process() {
    if (current_proc->parent->waitpid == current_proc->pid) {
        current_proc->parent->state = READY;
        delete_from_wait(current_proc->parent);
        add_proc_to_queue(ready_queue, current_proc->parent);
    }
    current_proc->state = DEAD;
    current_proc->occupied = 0;
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
    timerHandler();
    current_proc->rsp = rsp;

    current_proc = round_robin(ready_queue);

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
            ncPrintBase(sysTime(4, 0, 0, 0, 0), 10);
            ncPrint(":");
            ncPrintBase(sysTime(3, 0, 0, 0, 0), 10);
            ncPrint(":");
            ncPrintBase(sysTime(2, 0, 0, 0, 0), 10);
            ncScroll();


        }
    }
    if(num_printed == 0){
        ncPrint("No hay procesos!! [PLACEHOLDER ]");
        ncScroll();
    }

}

proc get_process(int pid){
    for(int i = 0; i < NUM_PROCESS; i++)
        if(process_table[i].pid == pid)
            return &(process_table[i]);
    return (proc) 0;
}

void kill(int pid){
    proc process = get_process(pid);
    if(process == 0)
        return;
    else{
        process->state = DEAD;
        process->occupied = 0;
    }
}

int getFd(proc p, int desiredFd){
    return p->fds[desiredFd];
}

void switchFd(proc p, int fdType, int newFd){
    p->fds[fdType] = newFd;
}
