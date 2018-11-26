/******************************************************************************************************
                                         HEADER FILES
*******************************************************************************************************/
#include<lpc214x.h>
#include "ADC.h"

/******************************************************************************************************
* Function    : Delay                                                                                 *
*                                                                                                     *
* Description : Function for delay                                                                    *
*                                                                                                     *
* Parameter   : val - the value for delay                                                             *
*                                                                                                     *
*******************************************************************************************************/
#define start 1<<24
#define AD0DR6 (*((volatile unsigned long *) 0xE0034028))

unsigned char ADC_func(void)
{
	unsigned int temp;
	unsigned char result;
	IO0DIR = 0x000E0000;
	PINSEL1 = 0x04000000;
	AD0CR = 0x00200404;
	AD0CR |= start;
	while(!(AD0DR2 & 0x80000000));
	temp = AD0DR2 & 0x0000FFC0;
	result = (temp>>8);
	return result;
}