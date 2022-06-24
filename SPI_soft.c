#include "SPI_soft.h"
#include "my_delay.h"

void spi_soft_init()
{
    sMOSI_dir=0;
    sMISO_dir=1;
    sSCK_dir=0;
    sCS_dir=0;
    
    sCS=1;
    sSCK=0;
    sMOSI=0;
}


unsigned char spi_soft_write(unsigned char value)
{
    char count;
    unsigned char read_value=0;
    char _bit=0;
    sCS=0;
    delay_ms(1);
    //write 
    for(count=0;count<8;count++)
    {
        sSCK = 1;
        delay_ms(1);
        sMOSI=(value&(0x80>>count))?1:0;
        delay_ms(1);
        sSCK = 0;
        delay_ms(1);
    }
  
    delay_ms(5);
    
    //read 
    for(count=0;count<8;count++)
    {
        sSCK = 1;
        delay_ms(1);
        _bit=(sMISO)?1:0;
        read_value<<=1;
        read_value|=_bit;
        delay_ms(1);
        sSCK = 0;
        delay_ms(1);
    }
    sCS=1;
    sSCK=0;
    sMOSI=0;
    return read_value;    
}















