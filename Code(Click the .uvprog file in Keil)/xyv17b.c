#include<reg52.h>
#include<intrins.h>
#include "XYV17B.h"
#include "buzzer.h"
#define uchar unsigned char 
#define uint unsigned int
#define XTAL 11059200 // CUP ����Ƶ��
#define baudrate 9600 // ͨ�Ų�����
#define uchar unsigned char 
#define uint unsigned int
unsigned char rtemp,sflag;
unsigned char TestBuff[3];
unsigned char mode=0;
unsigned char ArrayIndex=0;
unsigned int g=1;  
///*1s�������delay(65534)*/

/*uart_init ���ܣ���ʼ������*/

void uart_init()
{
  delay(300);//�ǵö�����ʱ��ʹ����ģ����빤��״̬ 
	TMOD = 0x20; // ��ʱ��1������8λ�Զ�����ģʽ, ���ڲ���������
	TH1=(unsigned char)(256 - (XTAL / (32L * 12L * baudrate)));
	TL1=(unsigned char)(256 - (XTAL / (32L * 12L * baudrate))); // ��ʱ��0����ֵ
	SCON = 0x50;
	PCON = 0x00;
	TR1 = 1;
	ES = 1; //�򿪽����ж�
	EA =1;  //�����ж� 
} 
/*uart_tx_byte ���ܣ����ڷ���һ���ֽ�*/
void uart_tx_byte(uchar str)
{
  ES=0;//�����ڼ�رմ����ж�
	SBUF=str;
	while(TI==0);
	TI = 0;
  ES=1;//������ɴ򿪴����ж�
}
/*uart_tx_string ���ܣ����ڷ���һ���ַ���*/
void music_1(void)//��һ�� good_job
{
   uart_tx_byte(0xAA);
   uart_tx_byte(0x07);
   uart_tx_byte(0x02); 
   uart_tx_byte(0x00);
   uart_tx_byte(0x01); 
   uart_tx_byte(0xB4); 
   for(;g<=3;g++)
  delay(65534);
}
void music_2(void)//�ڶ��� game_lose
{ 
   uart_tx_byte(0xAA);
   uart_tx_byte(0x07);
   uart_tx_byte(0x02);
   uart_tx_byte(0x00);
   uart_tx_byte(0x02);
   uart_tx_byte(0xB5); 
   /*for(;g<=36;g++)
   {delayKey(65534);}*/
}
void music_3(void)  //������ speed_down
{ 
   uart_tx_byte(0xAA);
   uart_tx_byte(0x07);
   uart_tx_byte(0x02);
   uart_tx_byte(0x00);
   uart_tx_byte(0x03);
   uart_tx_byte(0xB6); 
   /*for(;g<=3;g++)
   {delayKey(65534);}*/
}
/*void soundMax(void)    //ʹ�������
{
   uart_tx_byte(0xAA);
   uart_tx_byte(0x13);
   uart_tx_byte(0x01);
   uart_tx_byte(0x1E);
   uart_tx_byte(0xDC);
}*/
void stop(void)
	//0xAA, 0x03, 0x00, 0xAD, 
{
   uart_tx_byte(0xAA);
   uart_tx_byte(0x03);
   uart_tx_byte(0x00);
   uart_tx_byte(0xAD);
}
void nextt(void)
{
//0xAA, 0x02, 0x00, 0xAC, 
   uart_tx_byte(0xAA);
   uart_tx_byte(0x02);
   uart_tx_byte(0x00);
   uart_tx_byte(0xAC);
}	
void SerialService() interrupt 4 //�жϺ���
{ 
 if(RI) 
 { 
  RI=0; 
  if(mode==0)
    {//�������ͨ�Ĳ���ģʽ
     TestBuff[ArrayIndex]=SBUF; //��������
    ArrayIndex++; 
    } 
  if(ArrayIndex==3)
  {
  sflag=1;
  }
 } 
}
//���͵����ݻ�����,�� 8 λ������ǰ���� 8 λ�ں�
/*
SBUF��51��Ƭ���е�����Ĵ������������ݻ�������һ������һ�����ͣ���
������ʵ�ǹ��õ�һ����ַ99H���������������������Ƿֿ��ġ�
������ʹ��ʱ���Ͳ���SBUF=XXX;  ��XXXΪ��Ҫ���͵����ݣ�
������ʹ��ʱ������XXX=SBUF
*/
