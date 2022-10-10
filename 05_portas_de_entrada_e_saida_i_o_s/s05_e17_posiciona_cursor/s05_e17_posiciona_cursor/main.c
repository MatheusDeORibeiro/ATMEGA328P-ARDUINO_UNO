/*
 * s05_e17_posiciona_cursor.c
 *
 * Created: 18/09/2022 21:15:12
 * Author : Matheus
 *
 * 5.17 – Elaborar um programa para realizar o movimento de um cursor num
 * LCD 16 × 2 com o uso de 4 botões, conforme fig. 5.23.
 */ 

#include "definicoes_principais.h" //adiciona a biblioteca de definições principais
#include "LCD_4bits.h"             //adiciona a biblioteca de funções do display LCD

//definições de hardware
#define CIMA     PD0
#define ESQUERDA PD1
#define DIREITA  PD2
#define BAIXO    PD3


int main(void) //função principal
{
	unsigned char posx = 1; //declaração de variáveis inicializada
	unsigned char posy = 1; //declaração de variáveis inicializada

	DDRD = 0xF0;   //configura os pinos PD0:PD3 como entradas e PD4:PD7 como saídas
	DDRB = 0xFF;   //configura todo o PORTB como saída

	PORTD = 0x0F;  //habilita os resistores de pull-up dos pinos PD0:PD3 e coloca os pinos PD4:PD7 em nível baixo
	PORTB = 0x00;  //coloca o PORTB em nível baixo

	UCSR0B = 0x00; //desabilita os pinos RX e Tx do Arduino UNO

	Inicializa_LCD(LCD_2_N); //inicializa o dispay LCD com interface de 4 bits com duas linhas
	Limpa_Display();         //limpa display
	Cursor_Home();           //posiciona o cursor na primeira coluna da primeira linha

	while(1) //loop infinito
	{
		Posiciona_Cursor(posy, posx); //posicona o cursor
		if(!tst_bit(PIND, CIMA))
		{
			while(!tst_bit(PIND, CIMA));
			_delay_ms(10);
			posy--;
		}
		if(!tst_bit(PIND, BAIXO))
		{
			while(!tst_bit(PIND, BAIXO));
			_delay_ms(10);
			posy++;
		}
		if(!tst_bit(PIND, DIREITA))
		{
			while(!tst_bit(PIND, DIREITA));
			_delay_ms(10);
			posx++;
		}
		if(!tst_bit(PIND, ESQUERDA))
		{
			while(!tst_bit(PIND, ESQUERDA));
			_delay_ms(10);
			posx--;
		}
		if(posy == 0) posy = 2;  //se o cursor estiver na primeira linha, vai para a segunda
		if(posy == 3) posy = 1;  //se o cursor estiver na segunda linha, vai para a primeira
		if(posx == 17) posx = 1; //se o cursor estiver na última coluna, vai para a primeira
		if(posx == 0) posx = 16; //se o cursor estiver na primeira coluna, vai para a última
	}
}
