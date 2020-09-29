#include<reg52.h>
#include "xyv17b.h"
#include "lcd1602.h"
//#include<stdlib.h>
#define uchar unsigned char  //�޷����ַ��� �궨��	������Χ0~255
#define uint  unsigned int	 //�޷������� �궨��	������Χ0~65535
#include "ds1302.h"
#include "buzzer.h"

extern void *memset  (void *s, char val, int n);
extern float atof (char *s1);
extern int sprintf  (char *, const char *, ...);
extern int strlen (char *);

float num1;		           //  �����һ����
float num2;		           //  ����ڶ�����
uchar jjcc;		           //  �Ӽ��˳���־����
uchar Lcd_Str[17];   //  ��ʾ������
uchar Num_str[17];
uchar value,menu_1;
uchar Lcd_num2_add = 0;	   //  �ڶ������ݿ�ʼλ������
uchar idata Lcd_dis1_i = 0;	   //  ��¼��ʾ��һ���ַ�
uchar idata Lcd_num1_i = 0;	   //  ��¼num1 �Ǽ�λ��
uchar idata Lcd_num2_i = 0;	   //  ��¼num2 �Ǽ�λ��
uchar idata num_key1;

uchar idata Dian_i = 0;	       //  С����λ��

sbit dq   = P1^4;	  //18b20 IO�ڵĶ���
uint temperature ;    //�¶ȱ���


/********************** ********************
�������ƣ�delayms;
�������ܣ���ʱz���룻
��    �Σ�z (��ʱʱ�����)
		  
*******************************************/
void delayms(uchar z)
{
   uint x,y;
    for(x=z;x>0;x--)
	  for(y=110;y>0;y--);
}
/******************************************/
sbit key_delete = P1^5;    //ɾ����
sbit key_qiehuan = P1^6; //�л���
//sbit key_stop = P1^7;
//sbit key_nextt = P3^7;

uchar Key_io;		  //  �ݴ�IOֵ
uchar Key_jl;		  //  ���ּ����������
uchar Key_value;	  //  ���ؼ�ֵ
uchar Key_D_bit;
bit Key_qh_bit;
uchar menu_2;

uchar saom(void)
{
   Key_value=20;
   P2=0xf0;		      //  ���͵���λ
   Key_io=P2;
   if(Key_io!=0xf0)   //  �鿴����λ�Ƿ��е͵�ƽ
    {
	  Key_io|=0x0f;   //  ���ָ���λ���ֵ͵�ƽ�Ĺܽţ����ߵ���λ
	  P2=Key_io;      //  ���¸���IO��
	  Key_io=P2;      //  ��ȡIO��Ϣ��ȷ����ֵ
	  if(Key_jl==0)   //  �������������ּ��
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
  if(key_delete==0)			//ɾ����
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


   key_qiehuan = 1;          //�л���
  if(key_qiehuan==0)			//ɾ����
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
  memset(Lcd_Str,0,16)    ;		  //�ڴ�ռ��ʼ��  
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
	     if(Lcd_Str[len-1]=='0') //  ȥ�������ж����0��С����
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
	     if(Lcd_Str[p-1]=='0') //  ȥ�������ж����0��С����
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
	      num1=atof(Num_str);	   //  ��������ȡ�ڶ�������
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
	   memset(Num_str,0,16);		//�ڴ�ռ��ʼ��  
	   Num_str[0] = 'A';
	   Num_str[1] = 'n';
	   Num_str[2] = 's';
	   Num_str[3] = s;
	   Dis_str(0x80,"                ");
	   Dis_str(0x80,Num_str);
	}
}

/***********************18b20��ʼ������*****************************/
void init_18b20()
{
	bit q;
	dq = 1;				//�������ø�
	delay_lcd(1);	    //15us
	dq = 0;				//����λ����
	delay_lcd(80);		//750us
	dq = 1;				//�������ø� �ȴ�
	delay_lcd(10);		//110us
	q = dq;				//��ȡ18b20��ʼ���ź�
	delay_lcd(20);		//200us
	dq = 1;				//�������ø� �ͷ�����
}

/*************д18b20�ڵ�����***************/
void write_18b20(uchar dat)
{
	uchar i;
	for(i=0;i<8;i++)
	{					 //д�����ǵ�λ��ʼ
		dq = 0;			 //�������õ�дʱ��϶��ʼ 
		dq = dat & 0x01; //��18b20����д������
		delay_lcd(5);	 // 60us
		dq = 1;			 //�ͷ�����
		dat >>= 1;
	}	
}

/*************��ȡ18b20�ڵ�����***************/
uchar read_18b20()
{
	uchar i,value;
	for(i=0;i<8;i++)
	{
		dq = 0;			 //�������õͶ�ʱ��϶��ʼ 
		value >>= 1;	 //�������ǵ�λ��ʼ
		dq = 1;			 //�ͷ�����
		if(dq == 1)		 //��ʼ��д���� 
			value |= 0x80;
		delay_lcd(5);	 //60us	��һ��ʱ��϶����Ҫ����60us��ʱ��
	}
	return value;		 //��������
}

/*************��ȡ�¶ȵ�ֵ ����������С��***************/
uint read_temp()
{
	uint value;
	uchar low;			   //�ڶ�ȡ�¶ȵ�ʱ������жϵ�̫Ƶ���ˣ���Ӧ�ð��жϸ����ˣ������Ӱ�쵽18b20��ʱ��
	init_18b20();		   //��ʼ��18b20
	write_18b20(0xcc);	   //����64λROM
	write_18b20(0x44);	   //����һ���¶�ת������
	delay_lcd(50);		   //500us

	init_18b20();		   //��ʼ��18b20
	
	write_18b20(0xcc);	   //����64λROM
	write_18b20(0xbe);	   //������ȡ�ݴ�������
	
	EA = 0;
	low = read_18b20();	   //���¶ȵ��ֽ�
	value = read_18b20();  //���¶ȸ��ֽ�
	EA = 1;
	value <<= 8;		   //���¶ȵĸ�λ����8λ
	value |= low;		   //�Ѷ������¶ȵ�λ�ŵ�value�ĵͰ�λ��
	value *= 0.625;	       //ת�����¶�ֵ С��
	return value;		   //���ض������¶� ��С��
}


/******************д���ں���*******************/
void write_week(uchar hang,uchar add,uchar week)//д���ں���
{
	if(hang==1)   
		write_1602com(0x80+add);
	else
		write_1602com(0x80+0x40+add);	  
	switch(week)
	{
		case 1:write_1602dat('M');//������Ϊ1ʱ����ʾ
			   write_1602dat('O');
			   write_1602dat('N');
			   break;
	   
		case 2:write_1602dat('T');//��������Ϊ2ʱ��ʾ
			   write_1602dat('U');
			   write_1602dat('E');
			   break;
		
		case 3:write_1602dat('W');//��������Ϊ3ʱ��ʾ
			   write_1602dat('E');
			   write_1602dat('D');
			   break;
		
		case 4:write_1602dat('T');//��������Ϊ4����ʾ
			   write_1602dat('H');
			   write_1602dat('U');
			   break;
		
		case 5:write_1602dat('F');//��������Ϊ5ʱ��ʾ
			   write_1602dat('R');
			   write_1602dat('I');
			   break;
		
		case 6:write_1602dat('S');//��������Ϊ6ʱ��ʾ
			   write_1602dat('T');
			   write_1602dat('A');
			   break;
		
		case 7:write_1602dat('S');//��������Ϊ7ʱ��ʾ
			   write_1602dat('U');
			   write_1602dat('N');
			   break;
	}
}

/*************��ʱ��0��ʼ������***************/
void init_1602_ds1302()
{
	write_sfm2_ds1302(1,1,shi);		   //��ʾʱ
	write_sfm2_ds1302(1,4,fen);		   //��ʾ��
	write_sfm2_ds1302(1,7,miao);	   //��ʾ��
	write_week(2,12,week);
	write_sfm3_18B20(1,11,temperature);	   //��ʾ�¶�
	write_sfm2_ds1302(2,2,nian);   //��ʾ��
	write_sfm2_ds1302(2,5,yue);	   //��ʾ��	
	write_sfm2_ds1302(2,8,ri);	   //��ʾ�� 	
}

/**********************���ú���************************/
void jisuanqi()
{
  switch(num_key1)
  {
	case 11:  if(Num_str[Lcd_num2_add]<=0x39&&Num_str[Lcd_num2_add]>=0x30)
	          num2=atof(Num_str+Lcd_num2_add);	   //  ��������ȡ�ڶ�������
			  else
			  break;
			  Lcd_num2_i=0 ;
		//	  XS_JG(0x8a,num2);
		  	  switch(jjcc)
			   {
	
				 case 1: num1+=num2;				   // ��
				 						         
				         break;
				 case 2: num1-=num2;				   // ��
				        
				         break;
				 case 3: num1*=num2;				   // ��
				         
				         break;
				 case 4: num1/=num2;				   // ��
				         
				         break;		   
			  }
			  XS_JG(0xc0,num1);
			  Dis_str(0x80+Lcd_dis1_i,"=");
			  jjcc=5;                                 //  �Ⱥű�־
		      break;	
	case 12:  if(Num_str[0]==0||Lcd_num2_i>0)
	            break;
	          fuhao('+');
	          jjcc=1;Dian_i=0 ;
	          break;			   //  ���¼Ӻż�
	case 13:  if(Num_str[0]==0||Lcd_num2_i>0)
	            break;
	          fuhao('-');
	          jjcc=2;Dian_i=0 ;   			          
	          break;			   //  ���¼��ż�
	case 14:  if(Num_str[0]==0||Lcd_num2_i>0)
	            break;
	          fuhao('*');
			  jjcc=3;Dian_i=0 ;
	          break;			   //  ���³˺ż�
	case 15:  if(Num_str[0]==0||Lcd_num2_i>0)
	            break;
	          fuhao('/');		 
	          jjcc=4;Dian_i=0 ;         
	          break;			   //  ���³��ż�
	case 10:  if(Dian_i==0&&(Lcd_num1_i != 0||Lcd_num2_i != 0))// ����С����ʱǰ�����������
	          {
	          Dian_i++;			    //  С����
			  Num_str[Lcd_dis1_i] = '.'	;
			  Lcd_dis1_i++;
			  Dis_str(0x80,Num_str); 
			  }
	          break;
/*********************   ɾ��һ������*******************/
	case 17:  if(Lcd_dis1_i==0)			
	            break;
			   
/*********************                  ɾ��һ������                          ɾ��С����              ���Ⱥź��ٳ���           *******************/
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
	
	default:  if(num_key1<10&&Dian_i<7)			// ��ʾ��Ӧ�����ּ�,�����С�����λ��С��7λ
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
					   Dis_str(0x80,"                ");   //  �����ʾ
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

/**********************���ú���************************/
void key_with()
{
	if(num_key1 == 18)   // �л���
	{
		menu_1++;
		menu_2 = 0;
		if(menu_1 == 1)
		{
			init_1602_dis_csf();  //��ʼ��Һ��
		}

		if(menu_1>=2)
		{  
		    menu_1 = 0;	
		    init_1602();
		    write_1602dat('0');
		    memset(Num_str,0,16);
		
		}	
	}
	if(num_key1 == 17)	//ѡ���
	{
		if(menu_1 == 1)		  //����ʱ��
		{
			menu_2 ++;
			if(menu_2 == 1)	  //����ʱ��
			{
				write_string(1,0,"    :  :    W:  ");			
				write_string(2,0," 20  -  -       ");	
			}
			if(menu_2 > 7)
			{
				menu_2 = 0;
				init_1602_dis_csf() ; //��ʼ��Һ��
				write_guanbiao(1,2,0);//�رչ��
			  buzzer_on();
				buzzer_delay();
        buzzer_off();
			}
		}
	}
		
			
	if((menu_1 == 1) && (menu_2 != 0))//��ʼ
	{
		
		if(menu_1 == 1)
		{
			if(menu_2 == 1)		  //����ʱ
			{
				if(num_key1 == 12)	//��
				{
					shi+=0x01;
					if((shi & 0x0f) >= 0x0a)
						shi = (shi & 0xf0) + 0x10;
					if(shi >= 0x24)
						shi = 0;
				}		
				if(num_key1 == 13)	//��
				{
					if(shi == 0x00)
						shi = 0x24;
					if((shi & 0x0f) == 0x00)
						shi = (shi | 0x0a) - 0x10;
					shi -- ; 
				}	  				
			}
			if(menu_2 == 2)		  //���÷�
			{
				if(num_key1 == 12)	//��
				{
					fen+=0x01;
					if((fen & 0x0f) >= 0x0a)
						fen = (fen & 0xf0) + 0x10;
					if(fen >= 0x60)
						fen = 0;
				}		
				if(num_key1 == 13)	//��	  
				{
					if(fen == 0x00)
						fen = 0x5a;
					if((fen & 0x0f) == 0x00)
						fen = (fen | 0x0a) - 0x10;
					fen -- ;
				}	
			}
			if(menu_2 == 3)		  //������
			{
				if(num_key1 == 12)	//��
				{
					miao+=0x01;
					if((miao & 0x0f) >= 0x0a)
						miao = (miao & 0xf0) + 0x10;
					if(miao >= 0x60)
						miao = 0;
				}	
				if(num_key1 == 13)	//��	  
				{
					if(miao == 0x00)
						miao = 0x5a;
					if((miao & 0x0f) == 0x00)
						miao = (miao | 0x0a) - 0x10;
					miao -- ;			
				}
			}
			if(menu_2 == 4)		  //��������
			{
				if(num_key1 == 12)	//��
				{
		    		week+=0x01;
					if((week & 0x0f) >= 0x0a)
						week = (week & 0xf0) + 0x10;
					if(week >= 0x08)
						week = 1;
				}		
				if(num_key1 == 13)	//��	  
				{
					if(week == 0x01)
						week = 0x08;
					if((week & 0x0f) == 0x00)
						week = (week | 0x0a) - 0x10;
					week -- ;
				}	
			}
			if(menu_2 == 5)		  //������
			{
				if(num_key1 == 12)	//��
				{
			    	nian+=0x01;
					if((nian & 0x0f) >= 0x0a)
						nian = (nian & 0xf0) + 0x10;
					if(nian >= 0x9a)
						nian = 1;
				}		
				if(num_key1 == 13)	//��	  
				{
					if(nian == 0x01)
						nian = 0x9a;
					if((nian & 0x0f) == 0x00)
						nian = (nian | 0x0a) - 0x10;
					nian -- ;		
				}	
			}
			if(menu_2 == 6)		  //������
			{
				if(num_key1 == 12)	//��
				{
			    	yue+=0x01;
					if((yue & 0x0f) >= 0x0a)
						yue = (yue & 0xf0) + 0x10;
					if(yue >= 0x12)
						yue = 1;
				}		
				if(num_key1 == 13)	//��	  
				{
					if(yue == 0x01)
						yue = 0x13;
					if((yue & 0x0f) == 0x00)
						yue = (yue | 0x0a) - 0x10;
					yue -- ;					
				}	
			}
			if(menu_2 == 7)		  //������
			{
				if(num_key1 == 12)	//��
				{
		    	ri+=0x01;
				if((ri & 0x0f) >= 0x0a)
					ri = (ri & 0xf0) + 0x10;
				if(ri >= 0x32)
					ri = 0;			
				}		
				if(num_key1 == 13)	//��	  
				{
					if(ri == 0x01)
						ri = 0x32;
					if((ri & 0x0f) == 0x00)
						ri = (ri | 0x0a) - 0x10;
					ri -- ;			
				}	
			}
			write_sfm2_ds1302(1,2,shi);	   //��ʾʱ
			write_sfm2_ds1302(1,5,fen);	   //��ʾ��
			write_sfm2_ds1302(1,8,miao);	   //��ʾ��
			write_sfm1(1,14,week);	   //��ʾ����					
			write_sfm2_ds1302(2,3,nian);	   //��ʾ��
			write_sfm2_ds1302(2,6,yue);	   //��ʾ��
			write_sfm2_ds1302(2,9,ri);	   //��ʾ��
			switch(menu_2)	   // �����ʾ
			{
				case 1:  write_guanbiao(1,2,1);  break;
				case 2:  write_guanbiao(1,5,1);  break;
				case 3:  write_guanbiao(1,8,1);  break;
				case 4:  write_guanbiao(1,14,1);  break;
				case 5:  write_guanbiao(2,3,1);  break;
				case 6:  write_guanbiao(2,6,1);  break;
				case 7:  write_guanbiao(2,9,1);  break;
			}
			write_time();	   //��ʱ��д��ȥ
		}
	}	
}	
/*����������*/

/*******************�������Ÿ���***********************/
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
/***************������*****************/
void main(void)
{
  P0 = P1 = P2 = P3 = 0xff;		//��Ƭ��IO�ڳ�ʼ��Ϊ1	
  init_1602();
	init_ds1302();
  temperature = read_temp();	//�ȶ����¶ȵ�ֵ
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
				temperature = read_temp();	//�ȶ����¶ȵ�ֵ
				read_time();		  //��ʱ��
				init_1602_ds1302();   //��ʾʱ��	   
			}
	   }

	   num_key1=saom();				//������ֵ
	   delayms(10);
	   if(num_key1<20)				//�Ƿ��а������£�С��20ʱ�а�������
	     {
				if(menu_1 == 0)
		 		jisuanqi();   //����������
				key_with();
				

		 } 		                   	   
	}
}
	