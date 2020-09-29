#ifndef __BUZZER_H
#define _BUZZER_H
 
#include<reg52.h>

#define uint unsigned int
#define uchar unsigned char
 
sbit BuzzerPort=P1^3;

void delay(uint z); 
void buzzer_on(void);
void buzzer_off(void);
void buzzer_delay(void);
 
#endif
