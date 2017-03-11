#ifndef SSLED_H__
#define SSLED_h__

/* Serial pin */
#define SSEG_SR_PIN PM1.BIT.bit3
#define SSEG_SR     P1.BIT.bit3
/* Clock pin */
#define SSEG_CL_PIN PM1.BIT.bit4
#define SSEG_CL     P1.BIT.bit4
/* Latch pin */
#define SSEG_LA_PIN PM1.BIT.bit7
#define SSEG_LA     P1.BIT.bit7

void LedInit(void);
void LedClear(void);
void LedSend(const int s);
void LedWrite(char data);

#endif // SSLED_H__
