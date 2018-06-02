#define ZERO_DIV 0
#define INV_OPCODE 1

extern void invOpcodeException();
extern void zeroDivException();

void throwException(int exceptionID){
	switch(exceptionID){
		case ZERO_DIV:
			zeroDivException();
		case INV_OPCODE:
			invOpcodeException();
	}
}