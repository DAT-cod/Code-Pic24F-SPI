#include "my_delay.h"

//thuc ra delay 2.2us
 void delay_us( unsigned int u2_Timer)
 {
     unsigned long mnop;
 	register int abcd asm("w0");
 	mnop = (((u2_Timer - 1) * 16));
 	abcd = mnop;
 	asm ("REPEAT  W0");
 	asm ("nop");
 }
 
 void delay_ms(unsigned int u2_Timer)
 {
 	unsigned int i;
 	do {
 		i = 4;
 		do {
 			delay_us(250);
 		} while(--i);
 	} while(--u2_Timer);
 }






















