#ifndef LCD_H
#define	LCD_H

#include <xc.h> // include processor files - each processor file is guarded.  
#include "my_delay.h"

void lcd_enable();
void LCD_Send4Bit(unsigned char Data);
void LCD_SendCommand(unsigned char command);
void lcd_xy(unsigned char y, unsigned char x);
void lcd_default_char(unsigned char Data);
void lcd_default_string (char *s);

//________________ham hay su dung_________________
void lcd_init();
void lcd_clear();
void lcd_char(unsigned char y, unsigned char x,unsigned char Data); //x,y chay tu 1 den 16
void lcd_string(unsigned char y, unsigned char x,char *s);//x,y chay tu 1 den 16
void lcd_dec(unsigned char y,unsigned char x,unsigned long gt);//x,y chay tu 1 den 16


#endif	/* XC_HEADER_TEMPLATE_H */

