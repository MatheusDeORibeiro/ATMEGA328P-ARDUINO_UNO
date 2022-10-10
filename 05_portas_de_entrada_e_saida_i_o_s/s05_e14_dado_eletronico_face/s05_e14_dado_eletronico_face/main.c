/*
 * s05_ex14_dado_eletronico_face.c
 *
 * Created: 06/09/2022 18:03:11
 * Author : Matheus
 *
 * 5.14 – Elaborar um programa para apresentar nos LEDs da fig. 5.15 um
 * número aleatório entre 1 e 6, formando os números de um dado
 * (mesma lógica do exercício acima).
 */ 

#define F_CPU 16000000UL //define frequênica do cristal de 16MHz

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

//definição de macros
#define tst_bit(Y, bit_x) (Y & (1 << bit_x)) //testa o estado do bit_x do PORTY

//definições de hardware
#define BOTAO   PC0   //substitui BOTAO por PC0 na programação
#define DISPLAY PORTD //substitui DISPLAY por PORTD na programação

//declaração de variável inicializada armazenada na memória de programa (flash EEPROM)
const unsigned char Dado_Eletronico[] PROGMEM = {0x40, 0x12, 0x52, 0x36, 0x76, 0x3F};
//                                                  1     2     3     4     5     6

int main(void)
{
	unsigned char dado = 1; //declaração de variável local inicializada
	DDRC = 0xFE;   //configura o pino RC0 como entrada e o restante do PORTC como saída
	PORTC = 0x01;  //habilita o resistor pull-up do pino RC0 e desliga os pinos restantes do PORTC
	DDRD = 0xFF;   //configura todo o PORTD como saída
	UCSR0B = 0x00; //desabilita os pinos RX e TX e configura PD0 e PD1 como pinos I/Os digitais para o Arduino

	while(1)
	{
		if(!tst_bit(PINC, BOTAO))         //verifica se o botão foi pressionado
		{
			while(!tst_bit(PINC, BOTAO)); //verifica se o botão foi solto
			//quando o botão for solto, será exibido o valor da face do dado
			DISPLAY = pgm_read_byte(&Dado_Eletronico[dado - 1]);
			_delay_ms(150);               //delay de 150ms
		}
		dado++;                           //incrementa variável
		if(dado == 7) dado = 1;           //reinicia valor da variável
	}
}
