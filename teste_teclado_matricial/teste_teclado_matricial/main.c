/*
 * teste_teclado_matricial.c
 *
 * Created: 29/09/2022 20:34:58
 * Author : Matheus de Oliveira Ribeiro
 */ 

#include "definicoes_principais.h" //biblioteca de definições principais
#include "LCD_4bits.h"             //biblioteca de funções para o display LCD com interface de 4 bits
#include "teclado_hexadecimal.h"   //biblioteca de 

//declaração de variáveis armazenadas na memória de programa
const unsigned char mensagem[] PROGMEM = "Teclado 4 X 4\0";
const unsigned char mensagem1[] PROGMEM = "tecla =\0";

int main(void)
{
	unsigned char n_teclado; //variável de leitura do teclado
	
    DDRD = 0x0F;  //configura os pinos PD3:0 como entradas e PD4:7 como saída
	DDRC = 0x0F;  //configura os pinos PC3:0 como saídas e os pinos restantes como entradas
	DDRB = 0x07;  //configura os pinos PB2:0 como saídas e os pinos restantes como entradas
	PORTD = 0xFF; //habilita os resistores de pull-up das entradas e coloca os pinos restantes em nível baixo
	PORTC = 0x00; //coloca todo o PORTC em nível baixo
	PORTB = 0x00; //coloca todo o PORTB em nível baixo
	
	UCSR0B = 0x00; //configura os pinos RX e TX como pinos I/0s
	
	Inicializa_LCD(LCD_2_N);       //inicia dispay com duas linhas
    Display_Cursor_ON_OFF(LCD_DSP_ON_CS_OFF); //mantém a tela do display ligada e desliga o cursor
	Limpa_Display();               //limpa a tela do dispay
	Cursor_Home();	               //posiciona o cursor na primeira coluna da promeira linha
	Escreve_S_memprogm(mensagem); 
	Posiciona_Cursor(2, 8);        //posiciona o cursor na primeira coluna da segunda linha
	Escreve_S_memprogm(mensagem1);
	
	while(1)
	{
		n_teclado = Leitura_Teclado();
		if(n_teclado != '\0')
		{
			Posiciona_Cursor(2, 16); //desloca o cursor para a última coluna da segunda linha
			Escreve_Dado(n_teclado); //escreve valor da tecla no formato ASCII
		}
	}
}

