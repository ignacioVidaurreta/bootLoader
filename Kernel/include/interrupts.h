#ifndef INTERRUPS_H_
#define INTERRUPS_H_

void irq00Handler(void);
void irq01Handler(void);
void irq02Handler(void);
void irq03Handler(void);
void irq04Handler(void);
void irq05Handler(void);

void exception0Handler(void);

void clearI(void);

void setI(void);

void halt(void);

//Termina la ejecución de la cpu.
void haltCPU(void);

INT_DESCR * getIDTBaseAddress(void);
void int80Handler();

#endif