/*
 * utilizando_mais_de_um_led_por_pino.c
 *
 * Created: 16/01/2023 18:45:13
 * Author : Matheus
 */

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
    while (1) 
    {
		//desliga os dois leds - alta impedância
		DDRB &= ~(1 << PB0);
		_delay_ms(100);
		//liga apenas o led com ânodo ligado ao pino PB0
		DDRB |= (1 << PB0);
		PORTB |= 1 << PB0;
		_delay_ms(100);
		//liga apenas o led com o cátodo ligado ao pino PB0
		PORTB &= ~(1 << PB0);
		_delay_ms(100);
    }
}
