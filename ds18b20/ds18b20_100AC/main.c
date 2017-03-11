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
//    LED2_PIN = 0;
//    LED2 = 0;

    MainInit();
	LedInit();
	LedSend(1000);
	init_18b20(); 

	int r = 0;
	
//	search_18b20();
	get_18b20();

    for(;;)
    {

        if (flag_1hz)
        {
//			LED2 ^= 1;
			flag_1hz = 0;
			init_18b20();
			get_18b20();
//			LedSend(9876);
//			LedSend(r);		
//			r++;
//			if (r > 0x43U)
//			{
//				r = 0;
//			}
        }
		asm("halt");
    }
}
