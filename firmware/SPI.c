#include <lpc214x.h>
#include "SPI.h"

void MASTERINIT()
{
	PINSEL0 |= 0x00001500;
	S0SPCCR = 32;
	S0SPCR |= 0x0838;

/*	 unsigned char Status;
  	 unsigned char Data;
     PINSEL0 = (PINSEL0 & ~(3 << 12)) | (1 << 12); //  MOSI0 SPI0
     PINSEL0 = (PINSEL0 & ~(3 << 10)) | (1 << 10); //  MISO0 SPI0
     PINSEL0 = (PINSEL0 & ~(3 << 8))  | (1 << 8);  //  SCK0  SPI0

     S0SPCCR = 64; // Should be an even number greater than 8

     S0SPCR  = 0|__BV(MSTR)|__BV(CPOL)|__BV(CPHA);

     Status = S0SPSR;
     Data   = S0SPDR;	*/
}
void SPItrx(unsigned int data)
{
	unsigned int dummy;
	MASTERINIT();
	data = data<<1;
	IO0CLR = 0x00000080;
	S0SPDR = data;
	while(!(S0SPSR&(0x80)));
	IO0SET = 0x00000080;
	dummy=S0SPSR;
	dummy=S0SPDR;
}

void SPI_func(unsigned char data1)
{
	unsigned int i=7;
	IO0DIR |= 0x000000D0;
	//Timerinit();
	//VICVectAddr0 = (unsigned)Timer_ISR;
	//VICVectCntl0 |= 0x20 | 4;
	IO0SET |= 0x00000080;
	//SPItrx(i);
	//while(1)
	//{
	 //SPItrx(7);
	 /*while(i)
	 {
	 i--;
	 }*/
	 SPItrx(data1);

	 //delay_sec(1);
	 //SPItrx(0);

	 //delay_sec(1);
	 //SPItrx(6);

	 /*delay_sec(1);
	 SPItrx(2);

	 delay_sec(1);
	 SPItrx(1);

	 delay_sec(1);
	 SPItrx(0);
	 delay_sec(1);

	}*/
}
