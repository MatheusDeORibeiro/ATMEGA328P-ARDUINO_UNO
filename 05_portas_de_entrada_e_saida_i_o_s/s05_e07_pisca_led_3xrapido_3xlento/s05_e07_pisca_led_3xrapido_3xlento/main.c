/*
 * s05_e07_pisca_led_3xrapido_3xlento.c
 *
 * Created: 03/08/2022 15:48:14
 * Author : Matheus
 *
 * 5.7 � Desenvolva um programa para piscar um LED rapidamente 3 vezes e
 * 3 vezes lentamente.
 */ 

#define F_CPU 16000000UL //define frequ�ncia do microcontrolador de 16MHz
#include <avr/io.h>
#include <util/delay.h>

#define set_bit(Y, bit_x) (Y |= (1 << bit_x))  //seta o bit x do PORTY
#define clr_bit(Y, bit_x) (Y &= ~(1 << bit_x)) //limpa o bit x do PORTY
#define tst_bit(Y, bit_x) (Y & (1 << bit_x))   //retorna o bit x do PORTY
#define cpl_bit(Y, bit_x) (Y ^= (1 << bit_x))  //complemanta o bit x do PORTY

#define LED PB5 //LED � substitu�do por PB5 na programa��o

int main()
{
	DDRB = 0xFF;  //configura todo o PORTB como sa�da
	
	while(1)
	{
		//utilizando estruturas de repeti��o e otimiza��o o programa consumiu 204 bytes
		//piscando led rapidamente
		for(unsigned int i = 6; i > 0; i--)
		{
			cpl_bit(PORTB, LED);
			_delay_ms(200);
		}
		//piscando led lentamente
		for(unsigned int i = 6; i > 0; i--)
		{
			cpl_bit(PORTB, LED);
			_delay_ms(1000);
		}
	}
}
