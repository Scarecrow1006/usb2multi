/*----------------------------------------------------------------------------
 *      Name:    DEMO.C
 *      Purpose: USB HID Demo
 *      Version: V1.10
 *----------------------------------------------------------------------------
 *      This software is supplied "AS IS" without any warranties, express,
 *      implied or statutory, including but not limited to the implied
 *      warranties of fitness for purpose, satisfactory quality and
 *      noninfringement. Keil extends you a royalty-free right to reproduce
 *      and distribute executable files created using this software for use
 *      on Philips LPC2xxx microcontroller devices only. Nothing else gives
 *      you the right to use this software.
 *
 *      Copyright (c) 2005-2006 Keil Software.
 *---------------------------------------------------------------------------*/

#include <LPC214X.H>                        /* LPC214x definitions */

#include "type.h"

#include "usb.h"
#include "usbcfg.h"
#include "usbhw.h"
#include "usbcore.h"

#include "demo.h"
#include "PWM.h"
#include "SPI.h"
#include "I2C.h"
#include "ADC.h"

BYTE InReport;                              /* HID Input Report */
                                            /*   Bit0,1,2:    Button */

BYTE OutReport;                             /* HID Out Report */
                                            /*   Bit0,1,2: LEDs */
unsigned char key = 0;
unsigned char rst = 0;
unsigned char cmd = 0;
unsigned char slvaddr;

/*
 *  Get HID Input Report -> InReport
 *  SW1, SW2, SW3 are pressed in a mutually exclusive manner.
 */

void GetInReport (void) { //This is for Device(LPC2148) to Host PC reporting
	switch (key){
		case 3: InReport=I2C_funcr(slvaddr<<1); break;
		case 5: InReport=ADC_func(); break;
	}
}


/*
 *  Set HID Output Report <- OutReport
 *  LED1,2 and 3 will turn ON in a mutually exclusive manner.
 */

void SetOutReport (void) { //This is for Host PC to Device(LPC2148) reporting
	unsigned char tmp;
	unsigned char datasnd;
	unsigned int cnt, tmp1, tmp2;
	if(key==0)
		key=OutReport;
	else{
		if(rst==0){
			if(OutReport==0){
				key=0;
				return;
			}
			else{
				rst=1;
				cmd=OutReport;
				return;
			}
		}

		if(rst==1){
			rst=0;
			switch(key){
				case 1:
					tmp2 = OutReport;
					for(cnt = 0; cnt < 4; cnt++)
					{
				  		tmp1 = 0x01&tmp2;
				  		if(tmp1 == 0x01)
			  			{
					  		IOCLR0 = LED1<<cnt;	
				  		}
				  		else
					  	{
		  					IOSET0 = LED1<<cnt;
				  		}
				  		tmp2 >>= 1;
					}
					rst = 0;
					break;
				case 2:
					datasnd = OutReport;
					SPI_func(datasnd);
					rst = 0;
					break;
				case 3:
					switch(cmd){
						case 1: 
							I2C_func(slvaddr<<1,OutReport);
							break;
						case 2:
							slvaddr=OutReport;
							break;
					}
					break;
				case 4:
					tmp = OutReport;
					PWM_func(tmp);
					rst = 0;
					break;
			}
			return;
		}	
	}
}


/* Main Program */

int main (void) {

  IODIR0 = SW1|SW2|SW3|LED1|LED2|LED3|(LED3<<1);                          /* LEDs defined as Outputs */

  USB_Init();                               /* USB Initialization */
  USB_Connect(TRUE);                        /* USB Connect */

  while (1);                                /* Loop forever */
}
