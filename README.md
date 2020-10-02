

## A Multifunctional student computer based on 51 single chip microcomputer(See pictures in README.pdf)

> Hardware: Stc89c52 and some electronic components
>
> Software: Altium Designer 17, Keil5(Using C Language)



### Ⅰ. Product introduction

1、2019.7.20——2019.10.20

2、You can refer to the demo video and GIF files.

2、Product function：

- Calender, temperature：（time（can be corrected）、date、week、temperature）

- Calculator：（addition, subtraction, multiplication and division, and decimal point operation, which can be used for continuous numerical calculation）

- Music Playing：（contains multiple music, support play and pause music, with the function of manually switching songs）

- The edge is marked with scale, which can be used as a ruler for drawing and measuring.

### Ⅱ. Brief description of operation steps

- First step

> Write a business plan to select a topic, draw the schematic and PCB diagram, let a factory to process PCB board

- Second step

> Buy electronic components, C language programing, and weld components on PCB board,etc 

### Ⅲ. Project details

##### 1、Schematic diagram and PCB diagram

> Software: Altium Designer 17
>

See pictures and files in "README.PDF" or "images"

##### 2、List of electronic components

See pictures and files in "README.PDF" or "images"

##### 3、Key description

> See pictures and files in "README.PDF" or "images"

- Interface of calculator



- Interface of time



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
void music_time()
{
   delay(3000);
   uart_init();
   nextt();
		 //while(1);
} 
```

- Stop playing ( by using the independent button)

```c
void keystop() // Stop playing the music
{
	if(k1==0)		  // Check whether the key K1 is pressed
	{	
		delay(1000);   // debouncing 一般大约 about 10ms
		if(k1==0)	 // Determine if the key is pressed again
		{
			delay(3000);
      uart_init();
      stop();   			
		}
		while(!k1);	 // Check whether the button is released
	}		
}
```

- Play the next song ( by using the independent button)

```c
void keynextt() // Play the next music
{
	if(k3==0)		  // Check whether the key K1 is pressed
	{	
		delay(1000);   
		if(k3==0)	 
		{
			delay(3000);
      uart_init();
      nextt1();   			
		}
		while(!k3);	 // Check whether the key K1 is pressed
	}		
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
