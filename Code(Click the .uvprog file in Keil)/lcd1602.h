#ifndef _LCD1602_H_
#define _LCD1602_H_

#define uint unsigned int
#define uchar unsigned char
sbit rs=P3^5;
sbit rw=P3^6;
sbit en=P3^4;
sbit busy=P3^7;


uchar code table_num[]="0123456789abcdefg";

void delay_lcd(uint z)
{
   while(z--);
}
void jiance()
{
   P0=0xff;
   rw=1;rs=0;en=1;
   while(busy);
   en=0;
}
void write_1602com(uchar com)
{
   //jiance();
   P0=com;
   rs=0;rw=0;en=1;
   delay_lcd(100);
   en=0;
}
void write_1602dat(uchar dat)
{
  //jiance();
  P0=dat;
  rs=1;rw=0;en=1;
  delay_lcd(100);
  en=0;
}
void Dis_str(uchar add, uchar *p)
{
  write_1602com(add);
  while(*p>0)
   {
     write_1602dat(*p);
	 p++;
   }
}

/***********************lcd1602上显示两位十进制数************************/
void write_sfm2_ds1302(uchar hang,uchar add,uchar date)
{
	uchar shi,ge;
	if(hang==1)   
		write_1602com(0x80+add);
	else
		write_1602com(0x80+0x40+add);
	shi=date/16;
	ge=date%16;		  
	write_1602dat(table_num[shi]);
	write_1602dat(table_num[ge]);	
}

/***********************lcd1602上显示这字符函数************************/
void write_string(uchar hang,uchar add,uchar *p)
{
	if(hang==1)   
		write_1602com(0x80+add);
	else
		write_1602com(0x80+0x40+add);
	while(1)														 
	{
		if(*p == '\0')  break;
		write_1602dat(*p);
		p++;
	}	
}

/***********************lcd1602上显示两位十进制数************************/
void write_sfm1(uchar hang,uchar add,uchar date)
{
	if(hang==1)   
		write_1602com(0x80+add);
	else
		write_1602com(0x80+0x40+add);
	write_1602dat(0x30+date % 10);	
}

/*****************控制光标函数********************/
void write_guanbiao(uchar hang,uchar add,uchar date)
{		
	if(hang==1)   
		write_1602com(0x80+add);
	else
		write_1602com(0x80+0x40+add);
	if(date == 1)
		write_1602com(0x0f);     //显示光标并且闪烁	
	else 
		write_1602com(0x0c);   //关闭光标
}

/***********************lcd1602上显示两位十进制数************************/
void write_sfm3_18B20(uchar hang,uchar add,uint date)
{
	if(hang==1)   
		write_1602com(0x80+add);
	else
		write_1602com(0x80+0x40+add);
	write_1602dat(0x30+date/100%10);
	write_1602dat(0x30+date/10%10);
	write_1602dat('.');
	write_1602dat(0x30+date%10);	
}
	 

/***********************lcd1602上显示特定的字符************************/
void write_zifu(uchar hang,uchar add,uchar date)
{
	if(hang==1)   
		write_1602com(0x80+add);
	else
		write_1602com(0x80+0x40+add);
	write_1602dat(date);	
}
void init_1602()
{
   write_1602com(0x38);
   write_1602com(0x0c);
   write_1602com(0x06);
   write_1602com(0x01);
   //delay_lcd(1000);
   write_1602com(0x80);
}



/****************开机液晶显示函数 初始化液晶的内容********************************/
void init_1602_dis_csf()  //初始化液晶
{
	write_string(1,0,"   :  :    00.0");		
	write_string(2,0,"20  -  -       ");	
	write_zifu(1,15,0xdf);  //显示度	

}
#endif