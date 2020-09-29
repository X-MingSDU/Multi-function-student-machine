#ifndef _XYV17B_H_
#define _XYV17B_H_
#define uint unsigned int
#define uchar unsigned char
void uart_init();
//void uart_tx_command(unsigned int);
void uart_tx_byte(uchar str);
void music_1(void);
void music_2(void);
void music_3(void);
void stop(void);
void nextt(void);
#endif