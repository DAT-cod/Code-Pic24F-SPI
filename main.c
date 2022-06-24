/*
 CODE SPI MASTER
 
 */

#pragma config POSCMOD = NONE           // Primary Oscillator Select (Primary oscillator disabled)
#pragma config OSCIOFNC = OFF           // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = FRCPLL           // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))
#pragma config IESO = ON                // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) enabled)
#include <xc.h>
#include <p24FJ128GA006.h>
#include <stdio.h>
#include <stdlib.h>
#include "SPI_soft.h"
#include "lcd.h"
#include "my_delay.h"
#include "dht11.h"
#include <stdbool.h>

#define REG_NHIET_DO 0x1F
#define REG_DO_AM 0x3F

#define up PORTEbits.RE0
#define down PORTEbits.RE2
#define ok PORTEbits.RE1

#define on 0
#define off 1
#define lamp PORTEbits.RE4
#define fan PORTEbits.RE5

#define btn1 PORTGbits.RG2   //LATGbits.LATG2
#define btn2 PORTGbits.RG3   //LATGbits.LATG3
#define btn3 PORTGbits.RG6   //LATGbits.LATG6
#define btn4 PORTGbits.RG7   //LATGbits.LATG7

char stt_btn1=false;
char stt_btn2=false;
char stt_btn3=false;
char stt_btn4=false;

unsigned char nhiet_do=0;
unsigned char do_am=0;
unsigned int so_lan_bat_quat=0;
unsigned int so_lan_bat_den=0;

void set_humidity();
void set_temperature();
void check_info();
void refresh_menu(char __dem);
char menu();
void delay_user(unsigned int t);
unsigned char temp=0;
unsigned char btn_value=0;

int main(int argc,char ** argv)
{
    spi_soft_init();
    //nut nhan
    TRISEbits.TRISE0=1;
    TRISEbits.TRISE1=1;
    TRISEbits.TRISE2=1;
    
    TRISGbits.TRISG2=1;
    TRISGbits.TRISG3=1;
    TRISGbits.TRISG6=1;
    TRISGbits.TRISG7=1;
    
    //lamp & fan
    TRISEbits.TRISE4=0;
    TRISEbits.TRISE5=0; 
    lamp=off;
    fan=off;
    
    lcd_init();
    delay_ms(50);
    lcd_clear();
   
   while(1)
   {
        //hien thi nhiet do
        lcd_string(1,1,"NHIET DO:");
        lcd_char(1,11,nhiet_do/10+'0');
        lcd_char(1,12,nhiet_do%10+'0');
        lcd_char(1,13,0xDF);
        lcd_char(1,14,'c');

        //hien thi do am
        lcd_string(2,1,"DO AM:");
        lcd_char(2,8,do_am/10+'0');
        lcd_char(2,9,do_am%10+'0');
        lcd_char(2,10,'%');
       
        if(ok) menu();
        if(up) 
        {
            lamp=!lamp;
            while(up);
        }
        
        if(down) 
        {
            fan=!fan;
            while(down);
        }
        
        temp=spi_soft_write(REG_NHIET_DO);
        if(temp!=0) nhiet_do=temp;
        delay_ms(10);
        temp = spi_soft_write(REG_DO_AM);
        delay_ms(10);
        temp = spi_soft_write(REG_DO_AM);
        if(temp!=0) do_am=temp;
        //delay_user(250);
        delay_ms(300);
   }
    
   return (EXIT_SUCCESS); 
}


void set_humidity()
{
	char dem,dem_last;
	 while(ok);
	 //hmax=eep_read(add_hmax);
	 dem=dem_last=10;
	 lcd_clear();
	 lcd_string(1,1,"Setting Hmax="); lcd_char(1,16,'%');
	 lcd_char(1,14,dem/10+0x30);
	 lcd_char(1,15,dem%10+0x30);
	 while(1)
	 {
		 if(dem>99) dem=99;
		 if(dem!=dem_last)
		 {
			 lcd_char(1,14,dem/10+0x30);
			 lcd_char(1,15,dem%10+0x30);
			 dem_last=dem;
		 }
			if(up) {if(dem<99) dem++; else dem=1; delay_ms(100);}
			else if(down) {if(dem>1) dem--; else dem=1; delay_ms(100);}
			else if(ok) {break;}
	 }//while(1)
	dem=5;
	dem_last=0;
	while(ok);
}

void set_temperature()
{
	char dem,dem_last;
	 while(ok);
	 dem=dem_last=40;
	 lcd_clear();
	 lcd_string(1,1,"Setting Tmax="); lcd_char(1,16,'c');
	 lcd_char(1,14,dem/10+0x30);
	 lcd_char(1,15,dem%10+0x30);
	 while(1)
	 {
		 if(dem>99) dem=99;
		 if(dem!=dem_last)
		 {
			 lcd_char(1,14,dem/10+0x30);
			 lcd_char(1,15,dem%10+0x30);
			 dem_last=dem;
		 }
			if(up) {if(dem<99) dem++; else dem=1; delay_ms(100);}
			else if(down) {if(dem>1) dem--; else dem=1; delay_ms(100);}
			else if(ok) {break;}
	 }//while(1)
	dem=5;
	dem_last=0;
	while(ok);
}

void check_info()
{
   while(ok);
   lcd_clear();
   lcd_string(1,1,"FAN ON:");
   lcd_string(2,1,"LAMP ON:");
   while(1)
   {
       lcd_char(1,10,so_lan_bat_quat/1000%10+'0');
       lcd_char(1,11,so_lan_bat_quat/100%10+'0');
       lcd_char(1,12,so_lan_bat_quat/10%10+'0');
       lcd_char(1,13,so_lan_bat_quat%10+'0');
       
       lcd_char(2,10,so_lan_bat_den/1000%10+'0');
       lcd_char(2,11,so_lan_bat_den/100%10+'0');
       lcd_char(2,12,so_lan_bat_den/10%10+'0');
       lcd_char(2,13,so_lan_bat_den%10+'0');
       if(ok) break;
   }
   while(ok);
   lcd_clear();
}


char menu()
{
  char _Exit=0;
  char dem,dem_last;
  //---------------------giao dien menu--------------------
  lcd_clear();
  dem=4;
  dem_last=!dem;
  lcd_char(1,1,0x7e);  
  lcd_string(1,2,"Setting Tmax");
  lcd_string(2,2,"Setting Hmax");
  while(ok);
  while(1)
  {
    //kiem tra su kien nhan nut up
    if(up)
    {
      if(dem<4) dem++; 
      else dem=1; 
      while(up);
    }

    //kiem tra su kien nhan nut down
    if(down)
    {
      if(dem>1) dem--; 
      else dem=4; 
      while(down);
    }    

    //neu co su kien nhan up hoac down thi hien thi cho phu hop
    if(dem!=dem_last)
    {
      lcd_clear();
      dem_last=dem;
      switch(dem)
      {
         
         case 4:
				lcd_char(1,1,0x7e);
                lcd_char(2,1,' ');
				lcd_string(1,2,"Setting Tmax");
				lcd_string(2,2,"Setting Hmax");
         break;       
				 
         case 3:
                lcd_char(1,1,' ');
				lcd_char(2,1,0x7e);
				lcd_string(1,2,"Setting Tmax");
				lcd_string(2,2,"Setting Hmax");
         break;      				 
				 
         case 2:
                 lcd_char(1,1,0x7e);
                 lcd_char(2,1,' ');
				 lcd_string(1,2,"Check info");
				 lcd_string(2,2,"Exit");
         break;   	

         case 1:
                 lcd_char(1,1,' ');
                 lcd_char(2,1,0x7e);
				 lcd_string(1,2,"Check info");
				 lcd_string(2,2,"Exit");
         break;   					 
         
      }//switch(dem)
    }//if(dem!=dem_last)
//-------------------------------------------------------------------------------------
  if(ok)
  {
      switch(dem)
      {
        case 4:  set_temperature(); dem=4; refresh_menu(dem); break;
        case 3:  set_humidity(); dem=3; refresh_menu(dem); break;
		case 2:  check_info(); dem=2; refresh_menu(dem); break;
        case 1: _Exit=1; break;
      }//switch(dem)
  }//if(ok)

  if(_Exit) break;
//-------------------------------------------------------------------------------------
  }//while(1)

   while(ok);
   lcd_clear();
   return 0;
}//char menu()

void refresh_menu(char __dem)
{
    lcd_clear();
    if(__dem==4 || __dem==3)
    {
        lcd_string(1,2,"Setting Tmax");
        lcd_string(2,2,"Setting Hmax");
    }
    else if(__dem==2 || __dem==1) 
    {
        lcd_string(1,2,"Check info");
        lcd_string(2,2,"Exit");
    }
	 
   switch(__dem)
   {
      case 4: lcd_char(1,1,0x7e); break;
      case 3: lcd_char(2,1,0x7e); break;
      case 2: lcd_char(1,1,0x7e); break;
      case 1: lcd_char(2,1,0x7e); break;
      default: lcd_char(1,1,0x7e); break;
   }
}

void delay_user(unsigned int t)
{
    while(t>0)
    {
        t--;
        if(btn1) 
        {
            stt_btn1=(stt_btn1)?false:true;
            if(stt_btn1) btn_value|=0x01; else btn_value&=~0x01;
            spi_soft_write(btn_value);
            break;
        }
        
        if(btn2) 
        {
            stt_btn2=(stt_btn2)?false:true;
            if(stt_btn2) btn_value|=0x02; else btn_value&=~0x02;
            spi_soft_write(btn_value);
            break;
        }
        
        if(btn3) 
        {
            stt_btn3=(stt_btn3)?false:true;
            if(stt_btn3) btn_value|=0x04; else btn_value&=~0x04;
            spi_soft_write(btn_value);
            break;
        }

        if(btn4) 
        {
            stt_btn4=(stt_btn4)?false:true;
            if(stt_btn4) btn_value|=0x08; else btn_value&=~0x08;
            spi_soft_write(btn_value);
            break;
        }
        delay_ms(1);
    }//
    while(btn1 || btn2 || btn3 || btn4);
}




























































