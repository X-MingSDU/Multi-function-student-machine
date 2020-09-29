#ifndef _DS1302_H_
#define _DS1302_H_


/******************
	ds1302 内部RAM   RAM0  1100 000R/W   1读 0写
					 RAM1  1100 001R/W  
					       ....... 
					 RAM30 1111 110R/W   
********************/

sbit clk = P1^2;	  //ds1302时钟线定义
sbit io =  P1^1;	  //数据线
sbit rst = P1^0;	  //复位线
						//秒  分   时   日   月  年   星期 	
uchar code write_add[]={0x80,0x82,0x84,0x86,0x88,0x8c,0x8a};   //写地址
uchar code read_add[] ={0x81,0x83,0x85,0x87,0x89,0x8d,0x8b};   //读地址
uchar code init_ds[]  ={0x58,0x00,0x00,0x01,0x10,0x14,0x1};   
uchar idata miao,fen,shi,ri,yue,week,nian;
uchar i;


/*************写一个数据到对应的地址里***************/
void write_ds1302(uchar add,uchar dat)
{		
	rst = 1;			 //把复位线拿高
	for(i=0;i<8;i++)
	{				     //低位在前
		clk = 0;		 //时钟线拿低开始写数据
		io = add & 0x01;    	
		add >>= 1;		 //把地址右移一位
		clk = 1;		 //时钟线拿高
	}	
	for(i=0;i<8;i++)
	{
		clk = 0;		 //时钟线拿低开始写数据
		io = dat & 0x01;
		dat >>= 1;		 //把数据右移一位
		clk = 1;		 //时钟线拿高
	}
	rst = 0;			 //复位线合低
	clk = 0;
	io = 0;
}

/*************从对应的地址读一个数据出来***************/
uchar read_ds1302(uchar add)
{
	uchar value,i;
	rst = 1;			 //把复位线拿高
	for(i=0;i<8;i++)
	{				     //低位在前
		clk = 0;		 //时钟线拿低开始写数据
		io = add & 0x01;    	
		add >>= 1;		 //把地址右移一位
		clk = 1;		 //时钟线拿高
	}		
	for(i=0;i<8;i++)
	{
		clk = 0;		 //时钟线拿低开始读数据
		value >>= 1;
		if(io == 1)
			value |= 0x80;
		clk = 1;		 //时钟线拿高
	}
	rst = 0;			 //复位线合低
	clk = 0;
	io = 0;
	return value;		 //返回读出来的数据
}



/*************把要的时间 年月日 都读出来***************/
void read_time()
{
	miao = read_ds1302(read_add[0]);	//读秒
	fen  = read_ds1302(read_add[1]);	//读分
	shi  = read_ds1302(read_add[2]);	//读时
	ri   = read_ds1302(read_add[3]);	//读日
	yue  = read_ds1302(read_add[4]);	//读月
	nian = read_ds1302(read_add[5]);	//读年
	week = read_ds1302(read_add[6]);	//读星期
}

/*************把要写的时间 年月日 都写入ds1302里***************/
void write_time()
{
	write_ds1302(0x8e,0x00);			//打开写保护
	write_ds1302(write_add[0],miao);	//写秒
	write_ds1302(write_add[1],fen);		//写分
	write_ds1302(write_add[2],shi);		//写时
	write_ds1302(write_add[3],ri);		//写日
	write_ds1302(write_add[4],yue);		//写月
	write_ds1302(write_add[5],nian);	//写星期
	write_ds1302(write_add[6],week);	//写年
	write_ds1302(0x8e,0x80);			//关闭写保护
}

/*************把数据保存到ds1302 RAM中**0-31*************/
void write_ds1302ram(uchar add,uchar dat)
{
	add <<= 1;     //地址是从第二位开始的
	add &= 0xfe;   //把最低位清零  是写的命令
	add |= 0xc0;   //地址最高两位为 1  
	write_ds1302(0x8e,0x00);
	write_ds1302(add,dat);	
	write_ds1302(0x8e,0x80);
}

/*************把数据从ds1302 RAM读出来**0-31*************/
uchar read_ds1302ram(uchar add)
{
	add <<= 1;     //地址是从第二位开始的
	add |= 0x01;   //把最高位置1  是读命令
	add |= 0xc0;   //地址最高两位为 1  
	return(read_ds1302(add));	
}


/*************初始化ds1302时间***************/
void init_ds1302()
{
	uchar i;
	rst = 0;	//第一次读写数据时要把IO品拿低
	clk = 0;
	io = 0;
	i = read_ds1302ram(30);   
	if(i != 3)
	{	
		i = 3;
		write_ds1302ram(30,i);			 //4050   4100		 3080

		write_ds1302(0x8e,0x00);	            //打开写保护
		for(i=0;i<7;i++)
			write_ds1302(write_add[i],init_ds[i]);	//把最高位值0 允许ds1302工作
		write_ds1302(0x8e,0x80);	//关写保护
	}

	read_time();		  //读时间
	if(miao >= 0x60)
	{
		write_ds1302(0x8e,0x00);	            //打开写保护
			write_ds1302(write_add[0],0x01);	//把最高位值0 允许ds1302工作
		write_ds1302(0x8e,0x80);	//关写保护
		
	}

}



#endif
