#include<reg52.h>
#include<intrins.h>
#include "XYV17B.h"
#include "buzzer.h"
#define uchar unsigned char 
#define uint unsigned int
#define XTAL 11059200 // CUP 晶振频率
#define baudrate 9600 // 通信波特率
#define uchar unsigned char 
#define uint unsigned int
unsigned char rtemp,sflag;
unsigned char TestBuff[3];
unsigned char mode=0;
unsigned char ArrayIndex=0;
unsigned int g=1;  
///*1s大概三个delay(65534)*/

/*uart_init 功能：初始化串口*/

void uart_init()
{
  delay(300);//记得短暂延时，使语音模块进入工作状态 
	TMOD = 0x20; // 定时器1工作于8位自动重载模式, 用于产生波特率
	TH1=(unsigned char)(256 - (XTAL / (32L * 12L * baudrate)));
	TL1=(unsigned char)(256 - (XTAL / (32L * 12L * baudrate))); // 定时器0赋初值
	SCON = 0x50;
	PCON = 0x00;
	TR1 = 1;
	ES = 1; //打开接收中断
	EA =1;  //打开总中断 
} 
/*uart_tx_byte 功能：串口发送一个字节*/
void uart_tx_byte(uchar str)
{
  ES=0;//发送期间关闭串口中断
	SBUF=str;
	while(TI==0);
	TI = 0;
  ES=1;//发送完成打开串口中断
}
/*uart_tx_string 功能：串口发送一个字符串*/
void music_1(void)//第一首 good_job
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
void music_2(void)//第二首 game_lose
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
void music_3(void)  //第三首 speed_down
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
/*void soundMax(void)    //使音量最大
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
void SerialService() interrupt 4 //中断函数
{ 
 if(RI) 
 { 
  RI=0; 
  if(mode==0)
    {//如果处于通的测试模式
     TestBuff[ArrayIndex]=SBUF; //读缓冲区
    ArrayIndex++; 
    } 
  if(ArrayIndex==3)
  {
  sflag=1;
  }
 } 
}
//发送的数据或命令,高 8 位数据在前，低 8 位在后。
/*
SBUF：51单片机中的特殊寄存器，串行数据缓冲器（一个接收一个发送），
两个其实是共用的一个地址99H，但是两个在物理上面是分开的。
当发送使用时，就采用SBUF=XXX;  （XXX为需要传送的数据）
当接收使用时，采用XXX=SBUF
*/
