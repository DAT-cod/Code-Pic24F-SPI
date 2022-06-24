#include "lcd.h"
#include "SPI_soft.h"

//tdinh nghia chan ket noi voi LCD 
#define LCD_RS LATDbits.LATD4
#define LCD_EN LATDbits.LATD5
#define LCD_D4 LATDbits.LATD6
#define LCD_D5 LATDbits.LATD7
#define LCD_D6 LATDbits.LATD8
#define LCD_D7 LATDbits.LATD9


void lcd_enable(void)
{
	LCD_EN= 1;
	delay_us(20);
	LCD_EN = 0;
	delay_us(20);
}

void LCD_Send4Bit(unsigned char Data)
{
	LCD_D4 = Data & 0x01;
	LCD_D5 = (Data>>1)&1;
	LCD_D6 = (Data>>2)&1;
	LCD_D7 = (Data>>3)&1;
}

void LCD_SendCommand(unsigned char command)
{
	LCD_Send4Bit(command >>4);/* Gui 4 bit cao */
	lcd_enable();
	LCD_Send4Bit(command); /* Gui 4 bit thap*/
	lcd_enable();
}

void lcd_clear()
{                                       // Ham Xoa Man Hinh LCD
	LCD_SendCommand(0x01); 
	delay_us(20);
}

void lcd_init()
{
    TRISDbits.TRISD4=0;
    TRISDbits.TRISD5=0;
    TRISDbits.TRISD6=0;
    TRISDbits.TRISD7=0;
    TRISDbits.TRISD8=0;
    TRISDbits.TRISD9=0;
    
	LCD_Send4Bit(0x00);
	delay_ms(20);
	LCD_RS = 0;
	//LCD_RW = 0;
	LCD_Send4Bit(0x03);
	lcd_enable();
	delay_ms(10);
	lcd_enable();
	delay_us(100);
	lcd_enable();
	LCD_Send4Bit(0x02);
	lcd_enable();
	LCD_SendCommand( 0x28 ); // giao thuc 4 bit, hien thi 2 hang, ki tu 5x8
	LCD_SendCommand( 0x0c); // cho phep hien thi man hinh
	LCD_SendCommand( 0x06 ); // tang ID, khong dich khung hinh
	LCD_SendCommand(0x01); // xoa toan bo khung hinh
}

void lcd_xy(unsigned char y, unsigned char x)
{
	unsigned char address;
	x=x-1;
	y=y-1;
	if(!y)address = (0x80+x);
	else address = (0xc0+x);
	delay_ms(2);
	LCD_SendCommand(address);
	delay_us(30);
}

void lcd_default_char(unsigned char Data)
{//Ham Gui 1 Ki Tu
	LCD_RS = 1;
	LCD_SendCommand(Data);
	LCD_RS = 0 ;
}

void lcd_default_string (char *s)
{//Ham gui 1 chuoi ky tu
	while (*s)
	{
		lcd_default_char(*s);
		s++;
	}
}

//x chay tu 0, y chay tu 0
void lcd_char(unsigned char y, unsigned char x,unsigned char Data)
{
	lcd_xy(y,x);
	lcd_default_char(Data);
}

//x chay tu 0, y chay tu 0
void lcd_string(unsigned char y, unsigned char x,char *s)
{
	lcd_xy(y,x);
	lcd_default_string(s);
}



//---------------------------Ham bo sung----------------------------------



void lcd_dec(unsigned char y,unsigned char x,unsigned long gt)
{
	unsigned char tam8[16];
	unsigned char vt=0;
	unsigned char dem8=0;
	unsigned long temp;
	temp=gt;
	while(1)
	{
		if(gt==0) break;
		tam8[dem8]=gt%10;
		gt=gt/10;
		dem8++;
	}

	if(temp!=0)
	{
		dem8--;
		while(1)
		{
			lcd_char(y,x+vt,tam8[dem8]+48);
			vt++;
			if(dem8==0) break;
			dem8--;
		}
	}
	else
		lcd_char(y,x+vt,48);
}
















