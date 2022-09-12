/*
 * s05_e04_atraso_laco_for.c
 *
 * Created: 03/08/2022 15:02:44
 * Author : Matheus
 */ 

#include <avr/io.h>


void atraso_for()
{
    unsigned int i, j;
    for(i=256; i!=0; i--)
    { 
		for(j=65535; j!=0; j--); // nc_bloco = 6 * 65.535
    }                            // nc_total = (6 + nc_bloco) * 256    
}

int main(void)
{	
	DDRD = 0xFF;
	for(;;)
	{
	    PORTD = 0xFF;
	    atraso_for();
		PORTD = 0x00;
		atraso_for();
	}
}
