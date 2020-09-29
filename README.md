[TOC]

## A Multifunctional student computer based on 51 single chip microcomputer

> Hardware: Stc89c52 and some electronic components
>
> Software: Altium Designer 17, Keil5(Using C Language)



### Ⅰ. Product introduction

1、2019.7.20——2019.10.20

2、You can refer to the demo video and GIF files.

2、Product function：

- Calender, temperature：（time（can be corrected）、date、week、temperature）

  <img src="images\time.jpg" style="zoom:50%;" />

- Calculator：（addition, subtraction, multiplication and division, and decimal point operation, which can be used for continuous numerical calculation）

  

  <img src="images\calculator.jpg" style="zoom: 33%;" />

- Music Playing：（contains multiple music, support play and pause music, with the function of manually switching songs）

- The edge is marked with scale, which can be used as a ruler for drawing and measuring

### Ⅱ. Brief description of operation steps

- First step

<img src="images\plan1.png" style="zoom: 67%;" />

- Second step

  <img src="images\plan2.png"  />

### Ⅲ. Project details

##### 1、Schematic diagram and PCB diagram

> Software: Altium Designer 17
>
> 

<img src="images\sch.png" style="zoom: 50%;" />

<img src="images\pcb.png" style="zoom:50%;" />

##### 2、List of electronic components

![](images\components.png)

##### 3、Key description

- Interface of calculator

<img src="images\page1.png" style="zoom: 80%;" />

- Interface of time

<img src="images\page2.png" style="zoom:80%;" />

### Ⅳ. Description of the code

> Software: Keil 5

##### 1 、Voice Play

> Control the component(xyv17b) by UART communication
>
> Store songs in a SD Card, remember to rename the song as the rule required.

- Initialize the Uart

  ```c
  void uart_init()
  {
    delay(300);// delay 
  	TMOD = 0x20; // Timer 1 works in 8-bit automatic overload mode, generate baud rates
  	TH1=(unsigned char)(256 - (XTAL / (32L * 12L * baudrate)));
  	TL1=(unsigned char)(256 - (XTAL / (32L * 12L * baudrate))); // 
  	SCON = 0x50;
  	PCON = 0x00;
  	TR1 = 1;
  	ES = 1; // Open receive interrupt
  	EA =1;  // 
  } 
  ```

- Play a song

```c
void uart_tx_byte(uchar str)
{
  ES=0; // Close the serial port interrupt
	SBUF=str;
	while(TI==0);
	TI = 0;
  ES=1; // Open...
}
/*uart_tx_string, Send a string*/
void music_1(void)// Play the first song
{
   uart_tx_byte(0xAA); // Follow the Specified command in the instruction of the xyv17b 
   uart_tx_byte(0x07);
   uart_tx_byte(0x02); 
   uart_tx_byte(0x00);
   uart_tx_byte(0x01); 
   uart_tx_byte(0xB4); 
   for(;g<=3;g++)
  delay(65534);
}
```

- Play the next song

```c
void nextt(void)
{
//0xAA, 0x02, 0x00, 0xAC, 
   uart_tx_byte(0xAA);
   uart_tx_byte(0x02);
   uart_tx_byte(0x00);
   uart_tx_byte(0xAC);
}
```

- The interrupt function

```c
void SerialService() interrupt 4 
{ 
 if(RI) 
 { 
  RI=0; 
  if(mode==0)
    {//
     TestBuff[ArrayIndex]=SBUF; // read the SBUF
    ArrayIndex++; 
    } 
  if(ArrayIndex==3)
  {
  sflag=1;
  }
 } 
}
```

##### 2、You can see other functions codes in .c file
