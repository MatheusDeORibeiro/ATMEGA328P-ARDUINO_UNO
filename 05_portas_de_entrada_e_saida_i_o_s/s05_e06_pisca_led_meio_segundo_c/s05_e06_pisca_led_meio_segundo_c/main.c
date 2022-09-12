/*
 * s05_e06_pisca_led_meio_segundo_c.c
 *
 * Created: 03/08/2022 15:37:14
 * Author : Matheus
 *
 * 5.6 – Utilizando a macro para complementar um bit (cpl_bit), faça um
 * programa para piscar um LED a cada 500 ms.
 */ 

#define F_CPU 16000000UL //define frequência do microcontrolador de 16MHz

#include <avr/io.h>
#include <util/delay.h>

#define set_bit(Y, bit_x) (Y |= (1 << bit_x))  //seta o bit x do PORTY
#define clr_bit(Y, bit_x) (Y &= ~(1 << bit_x)) //limpa o bit x do PORTY
#define tst_bit(Y, bit_x) (Y & (1 << bit_x))   //retorna o bit x do PORTY
#define cpl_bit(Y, bit_x) (Y ^= (1 << bit_x))  //complemanta o bit x do PORTY

#define LED PB5 //LED é substituído por PB5 na programação


int main()
{
	DDRB = 0xFF;  //configura todo o PORTB como saída
	PORTB = 0x00; //inicializa o PORTB em nível baixo
	
	while(1)
	{
		cpl_bit(PORTB, LED); //complementa o estado do LED
		_delay_ms(500);      //atraso de 500 ms
	}
}
