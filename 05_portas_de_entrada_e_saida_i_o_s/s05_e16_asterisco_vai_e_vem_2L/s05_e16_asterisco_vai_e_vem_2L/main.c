/*
 * s05_e16_asterisco_vai_e_vem_2L.c
 *
 * Created: 18/09/2022 19:33:12
 * Author : Matheus
 *
 * 5.16 – Repetir o exercício 5.15 empregando as duas linhas do LCD. Ao chegar ao
 * final da linha superior, o asterisco começa na linha inferior (endereço 0xD3).
 * Dessa forma, na linha superior o asterisco se desloca da esquerda para a
 * direita e na linha inferior, da direita para a esquerda.
 */ 

#include "definicoes_principais.h" //adiciona a biblioteca de definições principais
#include "LCD_4bits.h"             //adiciona a biblioteca de funções do display LCD


int main() //função principal
{
	unsigned char i; //variável auxiliar

	DDRD = 0xFF;   //define todo o PORTD como saída
	DDRB = 0xFF;   //define todo o PORTD como saída
	
	UCSR0B = 0x00; //desabilita os pinos RX e Tx do Arduino UNO

	Inicializa_LCD(LCD_2_N);                  //inicializa display com interface de 4 bits e duas linhas
	Display_Cursor_ON_OFF(LCD_DSP_ON_CS_OFF); //mantém o display ligado e desliga o cursor

	while(1) //loop infinito
	{
		Limpa_Display();                                //limpa display
		Cursor_Home();                                  //posiciona o cursor na primeira coluna da primeira linha
		Escreve_Dado('*');                              //imprime o caractere '*' na tela do display
		_delay_ms(200);
		for(i = 1; i < 32; i++)
		{
			if(i < 16) Desloca_Mensagem(LCD_DSL_MS_RG); //desloca o asterisco até o final da primeira linha
			else 
			    if(i == 16)
			    {
				    Limpa_Display();
				    Posiciona_Cursor(2, 16);            //posiciona o cursor na última coluna da segunda linha
				    Escreve_Dado('*');
			    }
			    else Desloca_Mensagem(LCD_DSL_MS_LF);   //desloca o asterisco até o início da segunda linha
			_delay_ms(200);                             //atraso de 200ms
		}
	}
}
