/*
 * s05_ex13_dado_eletronico.c
 *
 * Created: 05/09/2022 19:02:55
 * Author : Matheus
 *
 * 5.13 – Elaborar um programa para apresentar em um display de 7 segmentos um número
 * aleatório entre 1 e 6 quando um botão for pressionado, ou seja, crie um dado eletrônico.
 * Empregue o mesmo circuito da fig. 5.14.
 */ 

#define F_CPU 16000000UL //definição da frequência do cristal de 16MHz

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

//declaração de macro para leitura de um bit
#define tst_bit(Y, bit_x) (Y & (1 << bit_x)) //retorna o estado do bit_x do PORTY

#define BOTAO   PC0   //BOTAO é substituído por PC0 na programação
#define DISPLAY PORTD //DISPLAY é substituído por PORTD na programação

//declaração de variável inicializada gravada na memória de programa (flag EEPROM)
const unsigned char Dado_Eletronico[] PROGMEM = {0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7C};
//                                                  1     2     3     4     5     6

int main()
{
	unsigned char dado = 1;
	DDRC = 0xFE;   //configura o pino PB0 como entrada e o restante do PORTB como saída
	PORTC = 0x01;  //habilita o pull-up da entrada
	DDRD = 0xFF;   //configura todo o PORTD como saída
	UCSR0B = 0x00; //desabilita TX e RX, e configura PD0 e PD1 como pinos I/Os digitais para o Arduino
	
	while(1)
	{
		if(!tst_bit(PINC, BOTAO))         //verifica se o botão foi pressionado
		{
			while(!tst_bit(PINC, BOTAO)); //aguarda botão ser solto
			DISPLAY = pgm_read_byte(&Dado_Eletronico[dado - 1]);
			//quando o botão for solto exibe no display o valor gerado do dado eletrônico
			_delay_ms(150);              //aguarda 500ms
		}
		dado++;                 //incrementa valor de dado
		if(dado == 7) dado = 1; //reinicia a contagem se valor de dado for igual a 7
	}
}