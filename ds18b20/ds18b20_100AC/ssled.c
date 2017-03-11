#include <iodefine.h>
#include <iodefine_ext.h>
#include "ssled.h"

volatile unsigned char led_buffer[13] = {
252,		// 0
96,			// 1
218,		// 2
242,		// 3
102,		// 4
182,		// 5
190,		// 6
224,		// 7
254,		// 8
246,		// 9
1,				// .
2,				// -
0				// clear segment
};

void LedInit(void)
{
	SSEG_LA_PIN = 0;
	SSEG_LA = 0;
	SSEG_CL_PIN = 0;
	SSEG_CL = 0;
	SSEG_SR_PIN = 0;
	SSEG_SR = 0;
	LedClear();
}

void LedClear(void)
{
	int i;	
	for (i = 4; i; --i)
        LedWrite(led_buffer[12]);
}

void LedSend(const int s)
{
		char i[4] = {
			12,
			12,
			12,
			12,
		};
		i[0] = s  % 10 / 1;
		if (s>9) { i[1] = s  % 100 / 10; }
		if (s>99) { i[2] = s  % 1000 / 100; }
		if (s>999) { i[3] = s / 1000; }
		LedWrite(led_buffer[i[0]]);
		LedWrite(led_buffer[i[1]]);
		LedWrite(led_buffer[i[2]]);
		LedWrite(led_buffer[i[3]]);
}

void LedSendChar(const int s)
{
		LedWrite(led_buffer[s]);
}

void LedSendCharWDot(const int s)
{
		LedWrite(led_buffer[s] ^ 0x01);
}
void LedWrite(char data)
{
	char counts = 0x8; 
 	while(counts--)
	{
		SSEG_CL = 0;	
		SSEG_SR = 1 & data;
		data = data>>1;
		SSEG_CL = 1;
	}
	SSEG_LA = 1;
	SSEG_LA = 0;
}