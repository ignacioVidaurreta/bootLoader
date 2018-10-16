#include <pipes.h>
#include <sysCalls.h>
#include <lib.h>
#include <naiveConsole.h>
#include <stdint.h>

void pipeCreationTest(int *pipes);
void pipeWriteAndReadTest(int *pipes);
void pipeWriteAllTest(int *pipes);	//writes the entirety of the length of the pipe
void pipeInterruptedWriteTest(int *pipes);
void pipeInterruptedReadTest(int *pipes);
void pipeDestructionTest(int *pipes);

void pipeTestSuite(){

	intptr_t aux = (intptr_t) int80(0,0,0,0,0,SYS_CREATE_PIPE);
	int *pipes = (void*) aux;
	pipeCreationTest(pipes);
	pipeWriteAndReadTest(pipes);
	pipeWriteAllTest(pipes);
	pipeInterruptedWriteTest(pipes);
	pipeInterruptedReadTest(pipes);
	pipeDestructionTest(pipes);
}

void pipeCreationTest(int *pipes){

	ncPrint("pipeCreationTest: ");
	if(isPipe(pipes[0]) && isPipe(pipes[1]))
		ncPrintTestPassed("PASSED!");
	else
		ncPrintTestFailed("FAILED!");
}

void pipeWriteAndReadTest(int *pipes){

	ncPrint("pipeWriteAndReadTest: ");
	char writer[14] = {0};
	char reader[14] = {0};
	int size = 14;
	for(int i = 0; i < size - 1; i++)
		writer[i] = 'a';
	int wrote = int80(pipes[1],(uint64_t)writer,size,0,0,SYS_WRITE);
	int read = int80(pipes[0],(uint64_t)reader,size,0,0,SYS_READ);
	if(read != -1 && wrote != -1 && strcmp(writer,reader) == 0)
		ncPrintTestPassed("PASSED!");
	else	
		ncPrintTestFailed("FAILED!");
}

void pipeWriteAllTest(int *pipes){
	ncPrint("pipeWriteAllTest: ");
	char writer[PIPE_SIZE] = {0};
	char reader[PIPE_SIZE] = {0};
	for(int i = 0; i < PIPE_SIZE - 1; i++)
		writer[i] = 'a';
	int wrote = int80(pipes[1],(uint64_t)writer,PIPE_SIZE,0,0,SYS_WRITE);
	int read = int80(pipes[0],(uint64_t)reader,PIPE_SIZE,0,0,SYS_READ);
	if(read != -1 && wrote != -1 && strcmp(writer,reader) == 0)
		ncPrintTestPassed("PASSED!");
	else	
		ncPrintTestFailed("FAILED!");
}

void pipeInterruptedWriteTest(int *pipes){

	ncPrint("pipeInterruptedWriteTest");
	char *writer1 = "writer1";
	char *writer2 = "writer2";
	char reader[15] = {0};
	int size = 8;
	int wrote1 = int80(pipes[1],(uint64_t)writer1,size - 1,0,0,SYS_WRITE);
	int wrote2 = int80(pipes[1],(uint64_t)writer2,size,0,0,SYS_WRITE);
	int read = int80(pipes[0],(uint64_t)reader,15,0,0,SYS_READ);
	if(read != -1 && wrote1 != -1 && wrote2 != -1 && strcmp(concat(writer1,writer2),reader) == 0)
		ncPrintTestPassed("PASSED!");
	else	
		ncPrintTestFailed("FAILED!");
}

void pipeInterruptedReadTest(int *pipes){

	ncPrint("pipeInterruptedReadTest: ");
	char *writer = "writerwriter";
	char reader1[7] = {0};
	char reader2[7] = {0};
	int size = 13;
	int wrote = int80(pipes[1],(uint64_t)writer,size,0,0,SYS_WRITE);
	int read1 = int80(pipes[0],(uint64_t)reader1,6,0,0,SYS_READ);
	int read2 = int80(pipes[0],(uint64_t)reader2,6,0,0,SYS_READ);
	if(read1 != -1 && read2 != -1 && wrote != -1 && strcmp(concat(reader1,reader2),writer) == 0)
		ncPrintTestPassed("PASSED!");
	else	
		ncPrintTestFailed("FAILED!");
}

void pipeDestructionTest(int *pipes){

	ncPrint("pipeDestructionTest: ");
	int one = pipes[0];
	int two = pipes[1];
	int80(pipes[0],0,0,0,0,SYS_DESTROY_PIPE);
	if(!isPipe(one) && !isPipe(two))
		ncPrintTestPassed("PASSED!");
	else
		ncPrintTestFailed("FAILED!");
}