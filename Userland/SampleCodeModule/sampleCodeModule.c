//This is the program that will be called from the kernel to begin the execution
//of any and all user programs. All communication from this user space to kernel
//space should occur through systemCalls which are clearly defined at
//Kernel/include/sysCalls.h






int main() {
	//the following code is here merely as a test to see if this function is indeed called
	//anything may be added or removed from here.
	char * str;
	char * aux= "hola";
	strcpy(str, aux);
	scanf("%s", str);
	return 0xDEADC0DE;
}

