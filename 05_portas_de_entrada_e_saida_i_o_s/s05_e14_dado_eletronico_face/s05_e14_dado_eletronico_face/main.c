/*
 * s05_ex14_dado_eletronico_face.c
 *
 * Created: 06/09/2022 18:03:11
 * Author : Matheus
 *
 * 5.14 � Elaborar um programa para apresentar nos LEDs da fig. 5.15 um
 * n�mero aleat�rio entre 1 e 6, formando os n�meros de um dado
 * (mesma l�gica do exerc�cio acima).
 */ 

#define F_CPU 16000000UL //define frequ�nica do cristal de 16MHz

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

//defini��o de macros
#define tst_bit(Y, bit_x) (Y & (1 << bit_x)) //testa o estado do bit_x do PORTY

//defini��es de hardware
#define BOTAO   PC0   //substitui BOTAO por PC0 na programa��o
#define DISPLAY PORTD //substitui DISPLAY por PORTD na programa��o

//declara��o de vari�vel inicializada armazenada na mem�ria de programa (flash EEPROM)
const unsigned char Dado_Eletronico[] PROGMEM = {0x40, 0x12, 0x52, 0x36, 0x76, 0x3F};
//                                                  1     2     3     4     5     6

int main(void)
{
	unsigned char dado = 1; //declara��o de vari�vel local inicializada
	DDRC = 0xFE;   //configura o pino RC0 como entrada e o restante do PORTC como sa�da
	PORTC = 0x01;  //habilita o resistor pull-up do pino RC0 e desliga os pinos restantes do PORTC
	DDRD = 0xFF;   //configura todo o PORTD como sa�da
	UCSR0B = 0x00; //desabilita os pinos RX e TX e configura PD0 e PD1 como pinos I/Os digitais para o Arduino

	while(1)
	{
		if(!tst_bit(PINC, BOTAO))         //verifica se o bot�o foi pressionado
		{
			while(!tst_bit(PINC, BOTAO)); //verifica se o bot�o foi solto
			//quando o bot�o for solto, ser� exibido o valor da face do dado
			DISPLAY = pgm_read_byte(&Dado_Eletronico[dado - 1]);
			_delay_ms(150);               //delay de 150ms
		}
		dado++;                           //incrementa vari�vel
		if(dado == 7) dado = 1;           //reinicia valor da vari�vel
	}
}
