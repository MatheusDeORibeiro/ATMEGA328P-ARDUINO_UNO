/*
 * utilizando_seis_leds_em_tres_pinos.c
 *
 * Created: 16/01/2023 19:16:49
 * Author : Matheus
 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>


#define atraso() _delay_ms(100);

int main(void)
{
    while (1) 
    {
		//desliga todos os leds
		PORTC = 0x00;
		DDRC = 0xFF;
	    atraso();
		//liga o led 1 e desliga os demais
		PORTC = 0x00;
		DDRC &= ~(1 << PC2);
		PORTC |= 1 << PC1;
		atraso();
		//liga o led 2 e desliga os demais
		PORTC = 0x00;
		PORTC |= 1 << PC0;
		atraso();
		//liga o led 3 e desliga os demais
		PORTC = 0x00;
		DDRC = 0xFF;
		DDRC &= ~(1 << PC0);
		PORTC |= 1 << PC1;
		atraso();
		//liga o led 4 e desliga os demais
		PORTC = 0x00;
		PORTC |= 1 << PC2;
		atraso();
		//liga o led 5 e desliga os demais
		PORTC = 0x00;
		DDRC = 0xFF;
		DDRC &= ~(1 << PC1);
		PORTC |= 1 << PC2;
		atraso();
		//liga o led 6 e desliga os demais
		PORTC = 0x00;
		PORTC |= 1 << PC0;
		atraso();
		//liga os leds 4 e 5 e desliga os demais
		PORTC = 0x00;
		DDRC = 0xFF;
		PORTC |= 1 << PC2;
		atraso();
		//liga os leds 1 e 3 e desliga os demais
		PORTC = 0x00;
		PORTC |= 1 << PC1;
		atraso();
		//liga os leds 1 e 5 e desliga os demais
		PORTC = 0x00;
		PORTC |= (1 << PC1) | (1 << PC2);
		atraso();
		//liga os leds 2 e 6 e desliga os demais
		PORTC = 0x00;
		PORTC |= 1 << PC0;
		atraso();
		//liga os leds 2 e 4 e desliga os demais
		PORTC = 0x00;
		PORTC |= (1 << PC0) | (1 << PC2);
		atraso();
		//liga os leds 3 e 6 e desiga os demais
		PORTC = 0x00;
		PORTC |= (1 << PC0) | (1 << PC1);
		atraso();
		//desliga todos os leds
		PORTC = 0x00;
		PORTC |= (1 << PC0) | (1 << PC1) | (1 << PC2);
		atraso();
    }
}

