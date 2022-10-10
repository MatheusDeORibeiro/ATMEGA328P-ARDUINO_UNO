/*
 * teste_teclado_matricial.c
 *
 * Created: 29/09/2022 20:34:58
 * Author : Matheus de Oliveira Ribeiro
 */ 

#include "definicoes_principais.h" //biblioteca de defini��es principais
#include "LCD_4bits.h"             //biblioteca de fun��es para o display LCD com interface de 4 bits
#include "teclado_hexadecimal.h"   //biblioteca de 

//declara��o de vari�veis armazenadas na mem�ria de programa
const unsigned char mensagem[] PROGMEM = "Teclado 4 X 4\0";
const unsigned char mensagem1[] PROGMEM = "tecla =\0";

int main(void)
{
	unsigned char n_teclado; //vari�vel de leitura do teclado
	
    DDRD = 0x0F;  //configura os pinos PD3:0 como entradas e PD4:7 como sa�da
	DDRC = 0x0F;  //configura os pinos PC3:0 como sa�das e os pinos restantes como entradas
	DDRB = 0x07;  //configura os pinos PB2:0 como sa�das e os pinos restantes como entradas
	PORTD = 0xFF; //habilita os resistores de pull-up das entradas e coloca os pinos restantes em n�vel baixo
	PORTC = 0x00; //coloca todo o PORTC em n�vel baixo
	PORTB = 0x00; //coloca todo o PORTB em n�vel baixo
	
	UCSR0B = 0x00; //configura os pinos RX e TX como pinos I/0s
	
	Inicializa_LCD(LCD_2_N);       //inicia dispay com duas linhas
    Display_Cursor_ON_OFF(LCD_DSP_ON_CS_OFF); //mant�m a tela do display ligada e desliga o cursor
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
			Posiciona_Cursor(2, 16); //desloca o cursor para a �ltima coluna da segunda linha
			Escreve_Dado(n_teclado); //escreve valor da tecla no formato ASCII
		}
	}
}

