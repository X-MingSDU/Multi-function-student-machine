#include<reg52.h>
#include "xyv17b.h"
#include "lcd1602.h"
//#include<stdlib.h>
#define uchar unsigned char  //无符号字符型 宏定义	变量范围0~255
#define uint  unsigned int	 //无符号整型 宏定义	变量范围0~65535
#include "ds1302.h"
#include "buzzer.h"

extern void *memset  (void *s, char val, int n);
extern float atof (char *s1);
extern int sprintf  (char *, const char *, ...);
extern int strlen (char *);

float num1;		           //  保存第一个数
float num2;		           //  保存第二个数
uchar jjcc;		           //  加减乘除标志变量
uchar Lcd_Str[17];   //  显示区缓存
uchar Num_str[17];
uchar value,menu_1;
uchar Lcd_num2_add = 0;	   //  第二个数据开始位的增量
uchar idata Lcd_dis1_i = 0;	   //  记录显示第一行字符
uchar idata Lcd_num1_i = 0;	   //  记录num1 是几位数
uchar idata Lcd_num2_i = 0;	   //  记录num2 是几位数
uchar idata num_key1;

uchar idata Dian_i = 0;	       //  小数点位数

sbit dq   = P1^4;	  //18b20 IO口的定义
uint temperature ;    //温度变量


/********************** ********************
函数名称：delayms;
函数功能：延时z毫秒；
形    参：z (延时时间参数)
		  
*******************************************/
void delayms(uchar z)
{
   uint x,y;
    for(x=z;x>0;x--)
	  for(y=110;y>0;y--);
}
/******************************************/
sbit key_delete = P1^5;    //删除键
sbit key_qiehuan = P1^6; //切换键
//sbit key_stop = P1^7;
//sbit key_nextt = P3^7;

uchar Key_io;		  //  暂存IO值
uchar Key_jl;		  //  松手检测自锁变量
uchar Key_value;	  //  返回键值
uchar Key_D_bit;
bit Key_qh_bit;
uchar menu_2;

uchar saom(void)
{
   Key_value=20;
   P2=0xf0;		      //  拉低第四位
   Key_io=P2;
   if(Key_io!=0xf0)   //  查看搞四位是否有低电平
    {
	  Key_io|=0x0f;   //  保持高四位出现低电平的管脚，拉高第四位
	  P2=Key_io;      //  重新复制IO口
	  Key_io=P2;      //  读取IO信息，确定键值
	  if(Key_jl==0)   //  自锁变量，松手检测
	   {
	     Key_jl=1;
	     switch(Key_io)
		   {
		     case 0x7e: Key_value=10;
			            break ;

			 case 0x7d: Key_value=0;
			            break ;

			 case 0x7b: Key_value=11;
			            break ;

			 case 0x77: Key_value=15;
			            break ;


			 case 0xbe: Key_value=7;
			            break ;

			 case 0xbd: Key_value=8;
			            break ;

			 case 0xbb: Key_value=9;
			            break ;

			 case 0xb7: Key_value=14;
			            break ;


			 case 0xde: Key_value=4;
			            break ;

			 case 0xdd: Key_value=5;
			            break ;

			 case 0xdb: Key_value=6;
			            break ;

			 case 0xd7: Key_value=13;
			            break ;


			 case 0xee: Key_value=1;
			            break ;

			 case 0xed: Key_value=2;
			            break ;

			 case 0xeb: Key_value=3;
			            break ;

			 case 0xe7: Key_value=12;
			            break ;
		   }
//		   write_sfm2(2,0,Key_value);
//		   write_sfm16(2,5,Key_io);
	   }
	}
   else
    {
	  Key_jl=0;
	}

	
	
	key_delete = 1;
  if(key_delete==0)			//删除键
   {
     if(Key_D_bit==0)
	  {
	  	   Key_D_bit = 1;
		   Key_value = 17;
	  }
   }
  else
   {
     Key_D_bit = 0;
   }


   key_qiehuan = 1;          //切换键
  if(key_qiehuan==0)			//删除键
   {
     if(Key_qh_bit==0)
	  {
	  	   Key_qh_bit = 1;
		   Key_value = 18;
	  }
   }
  else
   {
     Key_qh_bit = 0;
   }

  return Key_value;
}


uchar XS_JG(uchar add,float n)
{
  uchar len,p=0;
  memset(Lcd_Str,0,16)    ;		  //内存空间初始化  
  sprintf(Lcd_Str,"%e",n) ;
  while(Lcd_Str[p]!='e')
   {
     p++;
   }
  if(Lcd_Str[p+2]==0x30&&Lcd_Str[p+3]<0x37)
   {
   	  sprintf(Lcd_Str,"%f",n);
	  len = strlen(Lcd_Str)  ;
	  while(len)
	   {
	     if(Lcd_Str[len-1]=='.')
		  { 
		    Lcd_Str[len-1] = 0;  
			break;
		  }
	     if(Lcd_Str[len-1]=='0') //  去除数据中多余的0与小数点
		  {  
		    Lcd_Str[len-1] = 0;  	
		  }
		 else
		  {
		    break;
		  }
	     len--;
	   }
   }
  else
   {
      
   	  while(p)
	   {
	     if(Lcd_Str[p-1]=='.')
		  { 
		    Lcd_Str[p-1] =' ';  
			break;
		  }
	     if(Lcd_Str[p-1]=='0') //  去除数据中多余的0与小数点
		  {  
		    Lcd_Str[p-1] = ' ';  	
		  }
		 else
		  {
		    break;
		  }
	     p--;
	   }
   }
  
 

  Dis_str(add,"                "); 
  Dis_str(add,Lcd_Str);  
  return len;
}
void fuhao(uchar s)
{
  if(jjcc<5)
    {   
	   if(Num_str[0]!='A')
	    {    
		 if(Num_str[0]<=0x39&&Num_str[0]>=0x30&&jjcc != 2)
		 {
	      num1=atof(Num_str);	   //  从数组中取第二个数字
	//	  XS_JG(0x8a,num1);
		 }	
		}	   
	   if(jjcc==0)
	   {
	    Num_str[Lcd_dis1_i] =   s     ;
        Lcd_dis1_i++;
		Lcd_num2_add =  Lcd_dis1_i   ;
	   }
	   else
	   {
	    if(Num_str[Lcd_num2_add]==0)
	    Num_str[Lcd_dis1_i-1] = s   ;
	   }
	  
	   
       Dis_str(0x80,Num_str); 
	   
	}
  if(jjcc==5)
    {
	   Lcd_dis1_i = 4 ;
	   Lcd_num2_i = 0 ;
	   Lcd_num2_add = 4;
	   memset(Num_str,0,16);		//内存空间初始化  
	   Num_str[0] = 'A';
	   Num_str[1] = 'n';
	   Num_str[2] = 's';
	   Num_str[3] = s;
	   Dis_str(0x80,"                ");
	   Dis_str(0x80,Num_str);
	}
}

/***********************18b20初始化函数*****************************/
void init_18b20()
{
	bit q;
	dq = 1;				//把总线拿高
	delay_lcd(1);	    //15us
	dq = 0;				//给复位脉冲
	delay_lcd(80);		//750us
	dq = 1;				//把总线拿高 等待
	delay_lcd(10);		//110us
	q = dq;				//读取18b20初始化信号
	delay_lcd(20);		//200us
	dq = 1;				//把总线拿高 释放总线
}

/*************写18b20内的数据***************/
void write_18b20(uchar dat)
{
	uchar i;
	for(i=0;i<8;i++)
	{					 //写数据是低位开始
		dq = 0;			 //把总线拿低写时间隙开始 
		dq = dat & 0x01; //向18b20总线写数据了
		delay_lcd(5);	 // 60us
		dq = 1;			 //释放总线
		dat >>= 1;
	}	
}

/*************读取18b20内的数据***************/
uchar read_18b20()
{
	uchar i,value;
	for(i=0;i<8;i++)
	{
		dq = 0;			 //把总线拿低读时间隙开始 
		value >>= 1;	 //读数据是低位开始
		dq = 1;			 //释放总线
		if(dq == 1)		 //开始读写数据 
			value |= 0x80;
		delay_lcd(5);	 //60us	读一个时间隙最少要保持60us的时间
	}
	return value;		 //返回数据
}

/*************读取温度的值 读出来的是小数***************/
uint read_temp()
{
	uint value;
	uchar low;			   //在读取温度的时候如果中断的太频繁了，就应该把中断给关了，否则会影响到18b20的时序
	init_18b20();		   //初始化18b20
	write_18b20(0xcc);	   //跳过64位ROM
	write_18b20(0x44);	   //启动一次温度转换命令
	delay_lcd(50);		   //500us

	init_18b20();		   //初始化18b20
	
	write_18b20(0xcc);	   //跳过64位ROM
	write_18b20(0xbe);	   //发出读取暂存器命令
	
	EA = 0;
	low = read_18b20();	   //读温度低字节
	value = read_18b20();  //读温度高字节
	EA = 1;
	value <<= 8;		   //把温度的高位左移8位
	value |= low;		   //把读出的温度低位放到value的低八位中
	value *= 0.625;	       //转换到温度值 小数
	return value;		   //返回读出的温度 带小数
}


/******************写星期函数*******************/
void write_week(uchar hang,uchar add,uchar week)//写星期函数
{
	if(hang==1)   
		write_1602com(0x80+add);
	else
		write_1602com(0x80+0x40+add);	  
	switch(week)
	{
		case 1:write_1602dat('M');//星期数为1时，显示
			   write_1602dat('O');
			   write_1602dat('N');
			   break;
	   
		case 2:write_1602dat('T');//星期数据为2时显示
			   write_1602dat('U');
			   write_1602dat('E');
			   break;
		
		case 3:write_1602dat('W');//星期数据为3时显示
			   write_1602dat('E');
			   write_1602dat('D');
			   break;
		
		case 4:write_1602dat('T');//星期数据为4是显示
			   write_1602dat('H');
			   write_1602dat('U');
			   break;
		
		case 5:write_1602dat('F');//星期数据为5时显示
			   write_1602dat('R');
			   write_1602dat('I');
			   break;
		
		case 6:write_1602dat('S');//星期数据为6时显示
			   write_1602dat('T');
			   write_1602dat('A');
			   break;
		
		case 7:write_1602dat('S');//星期数据为7时显示
			   write_1602dat('U');
			   write_1602dat('N');
			   break;
	}
}

/*************定时器0初始化程序***************/
void init_1602_ds1302()
{
	write_sfm2_ds1302(1,1,shi);		   //显示时
	write_sfm2_ds1302(1,4,fen);		   //显示分
	write_sfm2_ds1302(1,7,miao);	   //显示秒
	write_week(2,12,week);
	write_sfm3_18B20(1,11,temperature);	   //显示温度
	write_sfm2_ds1302(2,2,nian);   //显示年
	write_sfm2_ds1302(2,5,yue);	   //显示月	
	write_sfm2_ds1302(2,8,ri);	   //显示日 	
}

/**********************设置函数************************/
void jisuanqi()
{
  switch(num_key1)
  {
	case 11:  if(Num_str[Lcd_num2_add]<=0x39&&Num_str[Lcd_num2_add]>=0x30)
	          num2=atof(Num_str+Lcd_num2_add);	   //  从数组中取第二个数字
			  else
			  break;
			  Lcd_num2_i=0 ;
		//	  XS_JG(0x8a,num2);
		  	  switch(jjcc)
			   {
	
				 case 1: num1+=num2;				   // 加
				 						         
				         break;
				 case 2: num1-=num2;				   // 减
				        
				         break;
				 case 3: num1*=num2;				   // 乘
				         
				         break;
				 case 4: num1/=num2;				   // 除
				         
				         break;		   
			  }
			  XS_JG(0xc0,num1);
			  Dis_str(0x80+Lcd_dis1_i,"=");
			  jjcc=5;                                 //  等号标志
		      break;	
	case 12:  if(Num_str[0]==0||Lcd_num2_i>0)
	            break;
	          fuhao('+');
	          jjcc=1;Dian_i=0 ;
	          break;			   //  按下加号键
	case 13:  if(Num_str[0]==0||Lcd_num2_i>0)
	            break;
	          fuhao('-');
	          jjcc=2;Dian_i=0 ;   			          
	          break;			   //  按下减号键
	case 14:  if(Num_str[0]==0||Lcd_num2_i>0)
	            break;
	          fuhao('*');
			  jjcc=3;Dian_i=0 ;
	          break;			   //  按下乘号键
	case 15:  if(Num_str[0]==0||Lcd_num2_i>0)
	            break;
	          fuhao('/');		 
	          jjcc=4;Dian_i=0 ;         
	          break;			   //  按下除号键
	case 10:  if(Dian_i==0&&(Lcd_num1_i != 0||Lcd_num2_i != 0))// 输入小数点时前面必须有数字
	          {
	          Dian_i++;			    //  小数点
			  Num_str[Lcd_dis1_i] = '.'	;
			  Lcd_dis1_i++;
			  Dis_str(0x80,Num_str); 
			  }
	          break;
/*********************   删除一个数字*******************/
	case 17:  if(Lcd_dis1_i==0)			
	            break;
			   
/*********************                  删除一个数字                          删除小数点              按等号后不再撤销           *******************/
			  if(((Num_str[Lcd_dis1_i-1]<=0x39&&Num_str[Lcd_dis1_i-1]>=0x30)||Num_str[Lcd_dis1_i-1]=='.')&&jjcc!=5)
			   {
			     if(Num_str[Lcd_dis1_i-1]=='.')
				  { Dian_i = 0;   }
				 else
				  {
				    if(jjcc==0)
					 {
					   Lcd_num1_i--;
					 }
					else
					 {
					   Lcd_num2_i--;
					 }
				  }
	             Lcd_dis1_i--;	
				 Lcd_num2_add =  Lcd_dis1_i;
				 Num_str[Lcd_dis1_i] = 0 ;	 
				 Dis_str(0x80,"                ");
			     Dis_str(0x80,Num_str);
	  			 
			   }
			 
			  if(Lcd_dis1_i==0)			
	            write_1602dat('0');
	          break; 		  			  				  
	
	default:  if(num_key1<10&&Dian_i<7)			// 显示对应的数字键,输入的小数点后位数小于7位
				  {
				     if(jjcc ==5)
					 {
					   jjcc = 0;
					   Lcd_dis1_i = 0 ;
					   Lcd_num1_i = 0 ;
				       Lcd_num2_i = 0 ;
				       Lcd_num2_add = 0;
					   Dian_i = 0;
				       memset(Num_str,0,16);
					   Dis_str(0x80,"                ");   //  清除显示
					   Dis_str(0xc0,"                ");
					 }
				     if(jjcc==0)
					 {
					  if(Lcd_num1_i<7)
						  {
						  Num_str[Lcd_dis1_i] = num_key1+0x30;
					      Dis_str(0x80,Num_str); 
					      Lcd_dis1_i++;
						  Lcd_num2_add =  Lcd_dis1_i;
					      Lcd_num1_i++;	
						  }
					    
					 }
					 else if(jjcc<5)
					 {
					  if(Lcd_num2_i<7&&Lcd_dis1_i<17)
						  {
						  Num_str[Lcd_dis1_i] = num_key1+0x30;
					      Dis_str(0x80,Num_str); 
					      Lcd_dis1_i++;	  
					      Lcd_num2_i++;
						  } 
					 }							  
				  }						 
	 break; 		  		   			   		
  }						
}	

/**********************设置函数************************/
void key_with()
{
	if(num_key1 == 18)   // 切换键
	{
		menu_1++;
		menu_2 = 0;
		if(menu_1 == 1)
		{
			init_1602_dis_csf();  //初始化液晶
		}

		if(menu_1>=2)
		{  
		    menu_1 = 0;	
		    init_1602();
		    write_1602dat('0');
		    memset(Num_str,0,16);
		
		}	
	}
	if(num_key1 == 17)	//选择键
	{
		if(menu_1 == 1)		  //设置时间
		{
			menu_2 ++;
			if(menu_2 == 1)	  //设置时间
			{
				write_string(1,0,"    :  :    W:  ");			
				write_string(2,0," 20  -  -       ");	
			}
			if(menu_2 > 7)
			{
				menu_2 = 0;
				init_1602_dis_csf() ; //初始化液晶
				write_guanbiao(1,2,0);//关闭光标
			  buzzer_on();
				buzzer_delay();
        buzzer_off();
			}
		}
	}
		
			
	if((menu_1 == 1) && (menu_2 != 0))//初始
	{
		
		if(menu_1 == 1)
		{
			if(menu_2 == 1)		  //设置时
			{
				if(num_key1 == 12)	//加
				{
					shi+=0x01;
					if((shi & 0x0f) >= 0x0a)
						shi = (shi & 0xf0) + 0x10;
					if(shi >= 0x24)
						shi = 0;
				}		
				if(num_key1 == 13)	//减
				{
					if(shi == 0x00)
						shi = 0x24;
					if((shi & 0x0f) == 0x00)
						shi = (shi | 0x0a) - 0x10;
					shi -- ; 
				}	  				
			}
			if(menu_2 == 2)		  //设置分
			{
				if(num_key1 == 12)	//加
				{
					fen+=0x01;
					if((fen & 0x0f) >= 0x0a)
						fen = (fen & 0xf0) + 0x10;
					if(fen >= 0x60)
						fen = 0;
				}		
				if(num_key1 == 13)	//减	  
				{
					if(fen == 0x00)
						fen = 0x5a;
					if((fen & 0x0f) == 0x00)
						fen = (fen | 0x0a) - 0x10;
					fen -- ;
				}	
			}
			if(menu_2 == 3)		  //设置秒
			{
				if(num_key1 == 12)	//加
				{
					miao+=0x01;
					if((miao & 0x0f) >= 0x0a)
						miao = (miao & 0xf0) + 0x10;
					if(miao >= 0x60)
						miao = 0;
				}	
				if(num_key1 == 13)	//减	  
				{
					if(miao == 0x00)
						miao = 0x5a;
					if((miao & 0x0f) == 0x00)
						miao = (miao | 0x0a) - 0x10;
					miao -- ;			
				}
			}
			if(menu_2 == 4)		  //设置星期
			{
				if(num_key1 == 12)	//加
				{
		    		week+=0x01;
					if((week & 0x0f) >= 0x0a)
						week = (week & 0xf0) + 0x10;
					if(week >= 0x08)
						week = 1;
				}		
				if(num_key1 == 13)	//减	  
				{
					if(week == 0x01)
						week = 0x08;
					if((week & 0x0f) == 0x00)
						week = (week | 0x0a) - 0x10;
					week -- ;
				}	
			}
			if(menu_2 == 5)		  //设置年
			{
				if(num_key1 == 12)	//加
				{
			    	nian+=0x01;
					if((nian & 0x0f) >= 0x0a)
						nian = (nian & 0xf0) + 0x10;
					if(nian >= 0x9a)
						nian = 1;
				}		
				if(num_key1 == 13)	//减	  
				{
					if(nian == 0x01)
						nian = 0x9a;
					if((nian & 0x0f) == 0x00)
						nian = (nian | 0x0a) - 0x10;
					nian -- ;		
				}	
			}
			if(menu_2 == 6)		  //设置月
			{
				if(num_key1 == 12)	//加
				{
			    	yue+=0x01;
					if((yue & 0x0f) >= 0x0a)
						yue = (yue & 0xf0) + 0x10;
					if(yue >= 0x12)
						yue = 1;
				}		
				if(num_key1 == 13)	//减	  
				{
					if(yue == 0x01)
						yue = 0x13;
					if((yue & 0x0f) == 0x00)
						yue = (yue | 0x0a) - 0x10;
					yue -- ;					
				}	
			}
			if(menu_2 == 7)		  //设置日
			{
				if(num_key1 == 12)	//加
				{
		    	ri+=0x01;
				if((ri & 0x0f) >= 0x0a)
					ri = (ri & 0xf0) + 0x10;
				if(ri >= 0x32)
					ri = 0;			
				}		
				if(num_key1 == 13)	//减	  
				{
					if(ri == 0x01)
						ri = 0x32;
					if((ri & 0x0f) == 0x00)
						ri = (ri | 0x0a) - 0x10;
					ri -- ;			
				}	
			}
			write_sfm2_ds1302(1,2,shi);	   //显示时
			write_sfm2_ds1302(1,5,fen);	   //显示分
			write_sfm2_ds1302(1,8,miao);	   //显示秒
			write_sfm1(1,14,week);	   //显示星期					
			write_sfm2_ds1302(2,3,nian);	   //显示年
			write_sfm2_ds1302(2,6,yue);	   //显示月
			write_sfm2_ds1302(2,9,ri);	   //显示日
			switch(menu_2)	   // 光标显示
			{
				case 1:  write_guanbiao(1,2,1);  break;
				case 2:  write_guanbiao(1,5,1);  break;
				case 3:  write_guanbiao(1,8,1);  break;
				case 4:  write_guanbiao(1,14,1);  break;
				case 5:  write_guanbiao(2,3,1);  break;
				case 6:  write_guanbiao(2,6,1);  break;
				case 7:  write_guanbiao(2,9,1);  break;
			}
			write_time();	   //把时间写进去
		}
	}	
}	
/*蜂鸣器函数*/

/*******************语音播放歌曲***********************/
void music_time(void)
{
	while(1)
	{
	 delay(300);
   uart_init();
   music_1();
   stop();
	 delay(65534);
	 nextt();	
  }  
}	
/***************主函数*****************/
void main(void)
{
  P0 = P1 = P2 = P3 = 0xff;		//单片机IO口初始化为1	
  init_1602();
	init_ds1302();
  temperature = read_temp();	//先读出温度的值
  write_1602dat('0');
  memset(Num_str,0,16);
	//music_time();
  while(1)
    {
	   value ++;
	   if(value >= 20)
	   {
	  		value = 0;	
			if((menu_1 == 1) && (menu_2 == 0))
			{	 	
				temperature = read_temp();	//先读出温度的值
				read_time();		  //读时间
				init_1602_ds1302();   //显示时钟	   
			}
	   }

	   num_key1=saom();				//读按键值
	   delayms(10);
	   if(num_key1<20)				//是否有按键按下，小于20时有按键按下
	     {
				if(menu_1 == 0)
		 		jisuanqi();   //计算器程序
				key_with();
				

		 } 		                   	   
	}
}
	