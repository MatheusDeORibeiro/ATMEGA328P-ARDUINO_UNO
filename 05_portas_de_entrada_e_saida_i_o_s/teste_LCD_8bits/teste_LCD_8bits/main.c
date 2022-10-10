/*
 * teste_LCD_8bits.c
 *
 * Created: 12/09/2022 17:58:51
 * Author : Matheus de Oliveira Ribeiro
 */ 


#include "definicoes_principais.h" //inclui a biblioteca de definições
#include "LCD_8bits.h"             //inclui a biblioteca de funções do dispay LCD 16x2

const unsigned char mensagem[] PROGMEM = "ABCDEFGHIJKLMNOP";

int main(void)
{   
	UCSR0B = 0x00;
	
    Inicializa_LCD(LCD_2_N);
	
	Testa_Display();
	_delay_ms(3000);
	
	Posiciona_Cursor(1, 1);
	Limpa_Display();
	Display_Cursor_ON_OFF(LCD_DSP_ON_CS_OFF);
	Escreve_S_memprogm(mensagem);
	Posiciona_Cursor(2, 1);	
	Escreve_S_memdados("QRSTUVWXYZ012345");
	_delay_ms(2000);
	
	unsigned char i = 40;
	while(i)
	{
		Desloca_Mensagem(LCD_DSL_MS_RG);
		_delay_ms(50);
		i--;
	}	
	
	_delay_ms(1000);
	
	unsigned char letra;
	Limpa_Display();
	Posiciona_Cursor(1, 1);
	Escreve_S_memdados("Microcontrolador");
	Posiciona_Cursor(1, 3);
	letra = Leitura_Dado();
	Posiciona_Cursor(2, 1);
	Escreve_S_memdados("POS(1, 3): ");
	Escreve_Dado(letra);
	
    while(1);
}

