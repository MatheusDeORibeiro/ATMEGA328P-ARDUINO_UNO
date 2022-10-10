/*
 * s05_ex15_asterisco_vai_e_vem.c
 *
 * Created: 17/09/2022 17:51:32
 * Author : Matheus
 *
 * 5.15 – Elaborar um programa para deslocar um caractere ‘ * ’ (asterisco) no LCD
 * da fig. 5.18, da esquerda para a direita, ao chegar ao final da linha o
 * caractere deve retornar (vai e vem).
 */ 

#include "definicoes_principais.h" //adiciona a biblioteca de definições
#include "LCD_4bits.h"             //adiciona a biblioteca de funções do display LCD 


int main(void) //função principal
{
    unsigned char i;
	
	DDRD = 0xFF;   //configura todo o PORTD como saída
	DDRB = 0xFF;   //configura todo o PORTD como saída
	
	PORTD = 0x00;  //PORTD em nível baixo
	PORTB = 0x00;  //PORTB em nível baixo
	
	Inicializa_LCD(LCD_1_N);                //inicializa o display com uma linha
	Display_Cursor_ON_OFF(LCD_DSP_ON_CS_OFF);  //mantém o display ligado e desliga o cursor
	Limpa_Display();                           //limpa o diplay
	Cursor_Home();                             //posiciona o cursor na primeira coluna da primeira linha
	Escreve_Dado('*');
	_delay_ms(200);
	
	while(1) //loop infinito
    {
		for(i = 1; i < 31; i++)
		{
			if(i < 16) Desloca_Mensagem(LCD_DSL_MS_RG); //enquanto i < 16, desloca o asterisco para a direita
			else Desloca_Mensagem(LCD_DSL_MS_LF);       //senão, desloca o asterisco para a esquerda
			_delay_ms(200);                             //atrso de 100ms
		}
    }
}

