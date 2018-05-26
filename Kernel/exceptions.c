#define ZERO_DIV 0
#define INV_OPCODE 1
#define EXCEPTIONS 2
#define REGS 15
#include <naiveConsole.h>

char* exceptions[EXCEPTIONS] = {"Zero division exception", "Invalid opcode exception"};
char* registers[REGS] = {"rax", "rbx", "rcx", "rdx", "rbp", "rdi","rsi","r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};

static void errorMessage(char* exception, uint64_t* sp);
static void printRegisters(uint64_t* sp);

//recordar que siempre que se vuelva de una excepción se volverá a la función main
//de esta función NO SE DEBE PODER RETORNAR a ningún lado, puesto que se ha corrompido
//el flujo del stack y se perdió la dirección de retorno de esta función.
void exceptionDispatcher(int exception, uint64_t* sp) {
	switch (exception){
		case ZERO_DIV:
			errorMessage(exceptions[ZERO_DIV], sp);
			break;
		case INV_OPCODE:
			errorMessage(exceptions[INV_OPCODE], sp);
			break;
	}
}

static void errorMessage(char* exception, uint64_t* sp) {
	ncPrint(exception);
	ncNewline();
	printRegisters(sp);
}

static void printRegisters(uint64_t* sp){
	for(int i = 0; i < REGS; i++){
		ncPrint(registers[i]); ncPrint(": "); ncPrintHex(sp[i]); ncPrint(" -- ");		//prints: "register: value --"
	}
}