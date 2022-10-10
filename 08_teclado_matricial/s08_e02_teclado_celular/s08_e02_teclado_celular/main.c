/*
 * s08_e02_teclado_celular.c
 *
 * Created: 06/10/2022 18:35:27
 * Author : Matheus
 *
 * 8.2 – Elaborar um programa para ler um teclado alfanumérico, similar ao usado
 * nos telefones celulares.
 */ 

#include "definicoes_principais.h" //inclui as definições principais
#include "teclado_hexadecimal.h"   //inclui a biblioteca de funções do teclado maticial 4X4
#include "LCD_4bits.h"             //inclui a biblioteca de funções do display LCD com interface de 4 bits


int main(void) //função principal
{
	unsigned char tecla;   //declaração de variável local não inicializada
	unsigned char digitos; //declaração de variável local não inicializada
	DDRD = 0x0F;  //configura PD3:0 como saídas e PD7:4 como entradas
	DDRB = 0xFF;  //configura todo o PORTB como saída
	DDRC = 0xFF;  //conifgura todo o PORTC como saída
	PORTD = 0xF0; //habilita os resultados pull-up das entradas e coloca as saída em nível baixo
	PORTB = 0x00; //coloca todo o PORTB em nível baixo
	PORTC = 0x00; //coloca todo o PORTC em nível baixo

	UCSR0B = 0x00; //desabilita as funções de RX e TX dos pinos PD0 e PD1

	Inicializa_LCD(LCD_1_N);                 //inicializa o display com uma linha
	Display_Cursor_ON_OFF(LCD_DSP_ON_CS_ON); //mantém a tela do display ligada e o cursor não piscante
	Limpa_Display(); //limpa a tela do display
	Cursor_Home();   //posiciona o cursor na primeira coluna da primeira linha

	while(1) //loop infinito
	{
		digitos = 1;
		Limpa_Display();
		Cursor_Home();
		while(digitos < 10)
		{
			tecla = Leitura_Teclado();
			if(tecla != '\0' && tecla != 'L' && tecla != 'A' && tecla != 'U' && tecla != 'D')
			{
				Escreve_Dado(tecla);
				digitos++;
			}
			if(tecla == 'L') //faz ligação
			{
				Limpa_Display();
				Cursor_Home();
				Escreve_S_memdados("Ligando...");
				_delay_ms(1000);
				Limpa_Display();
				Cursor_Home();
				digitos = 0;
			}
			if(tecla == 'A') //limpa tela
			{
				Limpa_Display();
				Cursor_Home();
				digitos = 0;
			}
			if(tecla == 'U') //apaga o último caractere
			{
				if(digitos > 1)
				{
					digitos--;
					Posiciona_Cursor(1, digitos);
					Escreve_Dado(' ');
					Posiciona_Cursor(1, digitos);
				}
			}
			//if(tecla == 'D')
			//{
			//}
		}
	}
}
