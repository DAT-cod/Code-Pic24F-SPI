#ifndef DHT11_H
#define	DHT11_H
#include <xc.h> // include processor files - each processor file is guarded.  
#include "my_delay.h"

void Request();
void Response();
int Receive_data();
void read_dht11(unsigned char *t,unsigned char *h);

#endif	/* XC_HEADER_TEMPLATE_H */

