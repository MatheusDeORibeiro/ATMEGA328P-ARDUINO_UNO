/*
 * s05_e16_asterisco_vai_e_vem_2L.c
 *
 * Created: 18/09/2022 19:33:12
 * Author : Matheus
 *
 * 5.16 � Repetir o exerc�cio 5.15 empregando as duas linhas do LCD. Ao chegar ao
 * final da linha superior, o asterisco come�a na linha inferior (endere�o 0xD3).
 * Dessa forma, na linha superior o asterisco se desloca da esquerda para a
 * direita e na linha inferior, da direita para a esquerda.
 */ 

#include "definicoes_principais.h" //adiciona a biblioteca de defini��es principais
#include "LCD_4bits.h"             //adiciona a biblioteca de fun��es do display LCD


int main() //fun��o principal
{
	unsigned char i; //vari�vel auxiliar

	DDRD = 0xFF;   //define todo o PORTD como sa�da
	DDRB = 0xFF;   //define todo o PORTD como sa�da
	
	UCSR0B = 0x00; //desabilita os pinos RX e Tx do Arduino UNO

	Inicializa_LCD(LCD_2_N);                  //inicializa display com interface de 4 bits e duas linhas
	Display_Cursor_ON_OFF(LCD_DSP_ON_CS_OFF); //mant�m o display ligado e desliga o cursor

	while(1) //loop infinito
	{
		Limpa_Display();                                //limpa display
		Cursor_Home();                                  //posiciona o cursor na primeira coluna da primeira linha
		Escreve_Dado('*');                              //imprime o caractere '*' na tela do display
		_delay_ms(200);
		for(i = 1; i < 32; i++)
		{
			if(i < 16) Desloca_Mensagem(LCD_DSL_MS_RG); //desloca o asterisco at� o final da primeira linha
			else 
			    if(i == 16)
			    {
				    Limpa_Display();
				    Posiciona_Cursor(2, 16);            //posiciona o cursor na �ltima coluna da segunda linha
				    Escreve_Dado('*');
			    }
			    else Desloca_Mensagem(LCD_DSL_MS_LF);   //desloca o asterisco at� o in�cio da segunda linha
			_delay_ms(200);                             //atraso de 200ms
		}
	}
}
