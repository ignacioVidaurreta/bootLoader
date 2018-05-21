#include <stdint.h>

int BCDToDec(int num){

	int ret = 0;
	int mul = 1;
	do{
		ret += (num % 16) * mul;
		mul *= 10;
		num /= 16;
	}
	while(num > 0);
	return ret;
}