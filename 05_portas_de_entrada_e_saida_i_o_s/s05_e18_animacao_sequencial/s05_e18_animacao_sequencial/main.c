/*
 * s05_e18_animacao_sequencial.c
 *
 * Created: 20/09/2022 14:18:14
 * Author : Matheus
 *
 * 5.18 – Desenvolva um programa para realizar uma animação sequencial na linha
 * superior e inferior de um LCD 16 × 2. Escreva um caractere por vez.
 */ 

#include "definicoes_principais.h" //inclui as definições principais
#include "LCD_4bits.h"             //inclui a biblioteca de funções do display LCD 16x2 interface de 4 bits


int main(void) //função principal
{
	unsigned char i; //declaração de variável não inicializada

	DDRD = 0xFF; //configura todo o PORTD como saída
	DDRB = 0xFF; //configura todo o PORTB como saída

	PORTD = 0x00; //coloca o PORTD em nível baixo
	PORTB = 0x00; //coloca o PORTB em nível baixo

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
