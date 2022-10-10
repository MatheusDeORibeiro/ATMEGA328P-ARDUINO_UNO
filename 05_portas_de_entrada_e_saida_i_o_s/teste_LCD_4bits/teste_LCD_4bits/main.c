/*
 * teste_LCD_4bits.c
 *
 * Created: 15/09/2022 14:51:10
 * Author : Matheus de Oliveira Ribeiro
 */ 


#include "definicoes_principais.h"
#include "LCD_4bits.h"

const unsigned char mensagem[] PROGMEM = "ABCDEFGHIJKLMNOP";

int main(void)
{
	UCSR0B = 0x00;
	DDRD = 0xFF;
	DDRB = 0xFF;
	
    Inicializa_LCD(LCD_2_N);
	Testa_Dispay();
	_delay_ms(500);
	Limpa_Display();
	Escreve_Dado('A');
	Escreve_Dado('B');
	Escreve_Dado('C');
	Posiciona_Cursor(2, 1);
	Escreve_Dado('1');
	Escreve_Dado('2');
	Escreve_Dado('3');
	_delay_ms(500);
	Desloca_Mensagem(LCD_DSL_CS_RG);
	_delay_ms(500);
	Desloca_Mensagem(LCD_DSL_CS_LF);
	_delay_ms(500);
	Display_Cursor_ON_OFF(LCD_DSP_ON_CS_ON);
	_delay_ms(500);
	Display_Cursor_ON_OFF(LCD_DSP_ON_CS_OFF);
	Limpa_Display();
	Cursor_Home();
	Posiciona_Cursor(1, 1);
	Escreve_S_memprogm(mensagem);
	Posiciona_Cursor(2, 1);
	Escreve_S_memdados("QRSTUVWXYZ012345");
	unsigned char i = 40;
	while(i)
	{
		Desloca_Mensagem(LCD_DSL_MS_RG);
		_delay_ms(50);
		i--;
	}
	_delay_ms(500);
	Limpa_Display();
	Cursor_Home();
	Escreve_S_memdados("Microcontrolador");
	Posiciona_Cursor(1, 3);
	unsigned char letra;
	letra = Leitura_Dado();
	Posiciona_Cursor(2, 1);
	Escreve_S_memdados("POS(1, 3) = ");
	Escreve_Dado(letra);
	
    while(1);
}

