
#include"buzzer.h"
#define uint unsigned int
#define uchar unsigned char
 
void delay(uint z)
{
   while(z--);
}
void buzzer_on(void)
{
		BuzzerPort=0;
}
 
void buzzer_off(void)
{
		BuzzerPort=1;
}
 
void buzzer_delay(void)
{
		BuzzerPort=0;
		delay(600);
		BuzzerPort=1;
		delay(600);
}


