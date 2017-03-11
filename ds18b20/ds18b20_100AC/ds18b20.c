#include <iodefine.h>
#include <iodefine_ext.h>

// 0x04U - 1 microsec
unsigned long int PrevCounter = 0;
#define DELAY_TIMING 0x04U

// Write
//	PM1.pm1 &= ~(1U << 6); 	//As output
//	P1.p1 |=  (1U << 6); 	//Enable
//	P1.p1 &=  ~(1U << 6); 	//Disable

// Read
//	PM1.pm1 |= (1U << 6);	//As input
//  var = P1.p1 & (1 << 6); //read from port

void DsDelayWoDelay(unsigned int units)
{
	unsigned long int counter = units * DELAY_TIMING;
	
	while(counter--)
	{
		/* Delay Loop	*/
		asm("nop");
	}
}

void DsDelay(unsigned int units)
{
	unsigned long int counter = units * DELAY_TIMING;
	
	while(counter--)
	{
		/* Delay Loop	*/
		asm("nop");
	}
}

unsigned char Temp[12];
unsigned char Temp_H = 0;
unsigned char Temp_L = 0;
unsigned char OK_Flag = 0;
unsigned char temp_flag;

void P16Output(void){
	PM1.pm1 &= ~(1U << 6); 	//As output
}
void P16Input(void){
	PM1.pm1 |= (1U << 6);	//As input
}
char DsReturn = 0;
char P16Read(void){
	P16Input();
	DsReturn = P1.p1 & (1 << 6); 
	return DsReturn;
}
void P16Up(void){
	P16Output();
	P1.p1 |=  (1U << 6); 	//Enable
}
void P16Down(void){
	P16Output();
	P1.p1 &=  ~(1U << 6); 	//Disable
}

/*** Инициализация DS18B20 ***/
unsigned char init_18b20(void) {
	P16Down();
	DsDelay(480);
	P16Up();
	P16Output();
	DsDelay(60);
	OK_Flag = P16Read(); 
	DsDelay(422);
	return OK_Flag;
}

/*** Функция чтения байта из DS18B20 ***/
unsigned char read_18b20(void)
{
	unsigned char i;   
	unsigned char dat = 0;
	for(i = 0;i < 8;i++) 
	{
		P16Down();
		DsDelay(15);        
		P16Up();
		P16Output();
		DsDelay(1);        
		dat = dat >> 1;           
		char x = 0;
		x = P16Read();  
		if(x)
		{           
			dat |= 0x80;
		}
		DsDelay(50);
	}   
	return dat;
}

/*** функция записи байта в DS18B20 ***/
void write_18b20(unsigned char dat)
{
	unsigned char i;   
	for(i = 0;i < 8;i++)
	{
		P16Down();
		DsDelay(10);           
		if(dat & 0x01)
		{
			P16Up(); 
		}
		else
		{
			P16Down();
		}
		dat = dat >> 1; 
		DsDelay(45); 
		P16Up();
		DsDelay(6); 
	}   
}

unsigned int old_tempint = 0;
unsigned int tempint = 0,tempint1,tempint2,tempint3; // переменные для целого значения температуры
unsigned int temppoint = 0,temppoint1; // переменные для дробного значения температуры


void get_18b20()
{	
/*** http://radioparty.ru/prog-avr/program-c/378-lesson2-ds18b20 ***/

	write_18b20(0xCCU);     // проверка кода датчика
	write_18b20(0x44U);     // запуск температурного преобразования
	DsDelay(750); 
	init_18b20();
	write_18b20(0xCCU);     // проверка кода датчика
	write_18b20(0xBEU);     // считываем содержимое ОЗУ

	Temp_L = read_18b20(); // читаем первые 2 байта блокнота
	Temp_H = read_18b20(); 
	temp_flag = 1;    
	if(Temp_H &(1 << 3))   // проверяем бит знака температуры на равенство единице 
	{           
		signed int tmp;
		temp_flag = 0;      // флаг знака равен 0(минус)
		tmp = (Temp_H << 8) | Temp_L;
		tmp = -tmp;
		Temp_L = tmp;
		Temp_H = tmp >> 8; 
	} 
	
	tempint = ((Temp_H << 4) & 0x70)|(Temp_L >> 4); // вычисляем целое значение температуры
	tempint1 = tempint % 1000 / 100;  
	tempint2 = tempint % 100 / 10;  
	tempint3 = tempint % 10;        
	temppoint = Temp_L & 0x0F; // вычисляем дробное значение температуры
	temppoint = temppoint * 625;       // точность температуры 
	temppoint1 = temppoint / 1000;        
	
//	if(tempint != old_tempint) LedSend(tempint);	
//	if(tempint != old_tempint) {
		LedSendChar(temppoint1);
		LedSendCharWDot(tempint3);
		if(tempint2 != 0) 
		{
			LedSendChar(tempint2);
			if(temp_flag == 0) 
			{ 
				LedSendChar(11);
			} else {
				LedSendChar(12);
			}
		} else {
			if(temp_flag == 0) 
			{ 
				LedSendChar(11);
				LedSendChar(12);
			} else {
				LedSendChar(12);
				LedSendChar(12);
			}
		}
		
//	}
	
	old_tempint = tempint;
	
}