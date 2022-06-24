#include "dht11.h"

#define out 0
#define in 1
#define DHT11_in PORTBbits.RB1
#define DHT11_out LATBbits.LATB1
#define DHT11_dir_out TRISBbits.TRISB1=0
#define DHT11_dir_in TRISBbits.TRISB1=1

void Request()	
{
    DHT11_dir_out;
	DHT11_out = 0;		/* set to low pin */
	delay_ms(20);	/* wait for 20ms */
	DHT11_out = 1;		/* set to high pin */
}

void Response()	
{
    DHT11_dir_in;
	while(DHT11_in==1);
	while(DHT11_in==0);
	while(DHT11_in==1);
}

int Receive_data()
{
	int q,c=0;	
    DHT11_dir_in;
	for (q=0; q<8; q++)
	{
		while(DHT11_in==0);/* check received bit 0 or 1 */
		delay_us(15);
		if(DHT11_in == 1)	/* If high pulse is greater than 30ms */
		c = (c<<1)|(0x01);/* Then its logic HIGH */
		else		/* otherwise its logic LOW */
		c = (c<<1);
		while(DHT11_in==1);
	}
	return c;
}

void read_dht11(unsigned char *t,unsigned char *h)
{
    Request();	/* send start pulse */
    Response();	/* receive response */
    *h=Receive_data();	/* store first eight bit in I_RH */
    Receive_data();	/* store next eight bit in D_RH */
    *t=Receive_data();	/* store next eight bit in I_Temp */
    Receive_data();	/* store next eight bit in D_Temp */
    Receive_data();/* store next eight bit in CheckSum */
}



















