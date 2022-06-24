#ifndef SPI_SOFT_H
#define	SPI_SOFT_H

#include <xc.h> // include processor files - each processor file is guarded.  


#define sMOSI LATBbits.LATB0
#define sMISO PORTBbits.RB1
#define sSCK LATBbits.LATB2
#define sCS LATBbits.LATB3

#define sMOSI_dir TRISBbits.TRISB0
#define sMISO_dir TRISBbits.TRISB1
#define sSCK_dir TRISBbits.TRISB2
#define sCS_dir TRISBbits.TRISB3

void spi_soft_init();
unsigned char spi_soft_write(unsigned char value);

 
#endif	/* XC_HEADER_TEMPLATE_H */

