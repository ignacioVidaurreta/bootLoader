#include <stdint.h>
#include <math.h>
#include <peripherals.h>
#include <naiveConsole.h>


void getTime1(char* buffer)
{
	int i = 0;
	uint8_t hours = BCDToDec(getTimeFromRTC(4));
	uint8_t minutes = BCDToDec(getTimeFromRTC(2));
	uint8_t seconds = BCDToDec(getTimeFromRTC(0));

	buffer[i++] = (hours / 10) + 48;
	buffer[i++] = (hours % 10) + 48;
	buffer[i++] = ':';
	buffer[i++] = (minutes / 10) + 48;
	buffer[i++] = (minutes % 10) + 48;
	buffer[i++] = ':';
	buffer[i++] = (seconds / 10) + 48;
	buffer[i++] = (seconds % 10) + 48;
	buffer[i] = 0;
}