#include <iodefine.h>
#include <iodefine_ext.h>
#include "ssled.h"

unsigned long int ticks = 0;
volatile unsigned char flag_1hz = 0;

__attribute__((interrupt))
void wdti_handler(void)
{
}

__attribute__((interrupt))
void it_handler(void)
{
    ++ticks;
    if (0 == (0x07U & ticks))
    {
        flag_1hz = 1;
    }
}

void main(void)
{
    MainInit();
	LedInit();
    for(;;)
    {
        if (flag_1hz)
        {
			flag_1hz = 0;
			init_18b20();
			get_18b20();
        }
		asm("halt");
    }
}
