/*
 * s05_e23_contador_ate_0x3FF.c
 *
 * Created: 25/09/2022 17:34:09
 * Author : Matheus
 * 
 * 5.23 – Repita o programa acima, só que desta vez, para apresentar um número
 * hexadecimal em uma contagem até 0x3FF.
 */ 

#include "definicoes_principais.h" //biblioteca de definições principais
#include "LCD_4bits.h"             //biblioteca de funções para o display LCD 16x2 com interface de 4 bits


//declaração de vetor para decodificar contador
unsigned char contador_hex[] = "0123456789ABCDEF";

int main(void) //função principal
{
	unsigned int contador; //declaração de variável local

	DDRD = 0xFF; //configura todo o PORTD como saída
	DDRB = 0xFF; //configura todo o PORTB como saída
	
	PORTD = 0x00; //coloca todo o PORTD em nível baixo
	PORTB = 0x00; //coloca todo o PORTB em nível baixo

	Inicializa_LCD(LCD_1_N);                  //inicialliza o display LCD com uma linha
	Limpa_Display();                          //limpa a tela do display
	Cursor_Home();                            //posiciona o cursor na primeira coluna da primeira linha
	Display_Cursor_ON_OFF(LCD_DSP_ON_CS_OFF); //mantém o display ligado e desliga o cursor

	while(1) //loop infinito
	{
		for(contador = 0; contador < 0x0400; contador++)
		{
			Limpa_Display();       //limpa tela do display
			if(contador >= 0x0100) //imprime de 0x0100 até 0x03FF
			{
				Posiciona_Cursor(1, 14);
				Escreve_Dado(contador_hex[(contador & 0x0F00) >> 8]);
				Escreve_Dado(contador_hex[(contador & 0x00F0) >> 4]);
				Escreve_Dado(contador_hex[contador & 0x000F]);
			}
			if(contador >= 0x0010) //imprime de 0x0010 até 0x00FF
			{
				Posiciona_Cursor(1, 15);
				Escreve_Dado(contador_hex[(contador & 0x00F0) >> 4]);
				Escreve_Dado(contador_hex[contador & 0x000F]);
			}
			else                   //imprime de 0x0000 até 0x000F(16)
			{
				Posiciona_Cursor(1, 16);
				Escreve_Dado(contador_hex[contador & 0x000F]);
			}
			_delay_ms(100);        //atraso para o incremento
		}
	}
}
