/*
 * s05_e18_animacao_sequencial.c
 *
 * Created: 20/09/2022 14:18:14
 * Author : Matheus
 *
 * 5.18 � Desenvolva um programa para realizar uma anima��o sequencial na linha
 * superior e inferior de um LCD 16 � 2. Escreva um caractere por vez.
 */ 

#include "definicoes_principais.h" //inclui as defini��es principais
#include "LCD_4bits.h"             //inclui a biblioteca de fun��es do display LCD 16x2 interface de 4 bits


int main(void) //fun��o principal
{
	unsigned char i; //declara��o de vari�vel n�o inicializada

	DDRD = 0xFF; //configura todo o PORTD como sa�da
	DDRB = 0xFF; //configura todo o PORTB como sa�da

	PORTD = 0x00; //coloca o PORTD em n�vel baixo
	PORTB = 0x00; //coloca o PORTB em n�vel baixo

	Inicializa_LCD(LCD_2_N); //inicializa o display LCD com duas linhas

	while(1) //loop infinito
	{
		Limpa_Display(); //limpa a tela do display
		Cursor_Home();   //posiciona o cursor na primeira coluna da primeira linha
		_delay_ms(1000);
		for(i = 0; i < 32; i++)
		{
			if(i < 16)
			{
				while(Status_BF());
				Escreve_Dado(0xFF);
			}
			if(i == 16)
			{
				while(Status_BF());
				Posiciona_Cursor(2, 1);
				Escreve_Dado(0xFF);
			}
			else
			{
				while(Status_BF());
				Escreve_Dado(0xFF);
			}
			_delay_ms(1000);
		}
	}
}
