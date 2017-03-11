#include <iodefine.h>
#include <iodefine_ext.h>

void MainInit(void)
{
    asm("di");
    /* Setup clocks */
    CMC.cmc = 0x00U;                                        /* Disable XT1, disable X1 */
    OSTS.osts = 0x07U; /* Page 237. Oscillation stabilization time. We use max 26.2 ms */
    CSC.csc = 0xC0U;         /* stop X1 and XT1 oscillators. Port in input mode */  
    OSTC.ostc = 0x77U; /* Page 235. Wait for oscillation of the X1 to stabilize. We use max */
    CKC.ckc = 0x00U; 
	
    /* Delay 1 second */
    register unsigned long int i;
    for (i = 0x000FFFFFUL; i; --i)
        asm("nop");
//    OSMC.osmc = 0x00U;                                      /* Supply Fil to Interval Timer. Fsub not in 20-36 pin MCUs */
	OSMC.osmc = 0x90U;                                      /* Supply Fsub to Interval Timer. For 20-36 pin MCUs */

    /* Setup 12-bit interval timer */
    RTCEN = 1;                                              /* Enable 12-bit interval timer and RTC */
    ITMK = 1;                                               /* Disable IT interrupt */
    ITPR0 = 0;                                              /* Set interrupt priority - highest */
    ITPR1 = 0;

/* internal oscillator is 15000Hz!!! */
/* 34642-32768 = 1874 */
//	ITMC.itmc = 0x800EU;                                    /* Set maximum period 14/15000Hz = 1/1024 s, and start timer */ 
	ITMC.itmc = 0x8752U;                                    /* Set maximum period 1875/15000Hz = 1/8 s, and start timer */ 
//	ITMC.itmc = 0x8EA5U;                                    /* Set maximum period 3750/15000Hz = 1/4 s, and start timer */ 

    ITIF = 1;                                               /* Set interrupt request flag */
    ITMK = 0;                                               /* Enable IT interrupt */
    asm ("ei");                                             /* Enable interrupts, immediate IT interrupt must happen */
}