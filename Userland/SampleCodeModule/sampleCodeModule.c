#include <shell.h>
#include <stdlib.h>
//This is the program that will be called from the kernel to begin the execution
//of any and all user programs. All communication from this user space to kernel
//space should occur through systemCalls which are clearly defined at
//Kernel/include/sysCalls.h






int main() {
	//the following code is here merely as a test to see if this function is indeed called
	//anything may be added or removed from here.
	start_proc_user("shell", (void*) shell, 0, 0, 1);
	// shell();
	return 0xDEADC0DE;
}
