/*
 * s05_e19_caca_niquel_eletronico.c
 *
 * Created: 20/09/2022 15:08:11
 * Author : Matheus
 *
 * 5.19 � Crie um ca�a-n�quel eletr�nico empregando 3 caracteres diferentes apresentados
 * em 3 posi��es do LCD. Utilize um bot�o no pino PB2 do ATmega para o sorteio.
 */ 

#include "definicoes_principais.h" //inclui as defini��es principais
#include "LCD_4bits.h"             //inclui a biblioteca de fun��es do display LCD 16x2 com interface de 4 bits


//defini��o de hardware
#define SORTEIO PB3 //substitui SELECAO por PB3 na programa��o

//defini��o de caracteres do ca�a-n�quel
const unsigned char caca_niquel[] PROGMEM = {0b00100100, 0b11101100, 0b00100010};

int main(void) //fun��o principal
{	
	unsigned char jogar = 0, caractere = 0;

	DDRD = 0xFF;   //configura todo o PORTD como sa�da
	DDRB = 0xF7;   //configura o pino PB3 como entrada e o restande do PORTB como sa�da

	PORTD = 0x00;  //coloca todo o PORTD em n�vel baixo
	PORTB = 0x08;  //habilita o resistor pull-up da entrada e coloca em n�vel baixo o restante do PORTB

	Inicializa_LCD(LCD_2_N);                  //inicializa o display com duas linhas
	Display_Cursor_ON_OFF(LCD_DSP_ON_CS_OFF); //mant�m a tela do display ligada e cursor n�o piscante

	while(1) //loop infinito
	{
		Limpa_Display();    //limpa a tela do display
		Cursor_Home();      //posiciona o cursor na primeira coluna da primeira linha
		Escreve_S_memdados("Caca-niquel AVR$");
		Posiciona_Cursor(2, 1);
		Escreve_S_memdados("Press o botao!!!");

		if(tst_bit(PINB, SORTEIO)) //se o bot�o for pressionado inicia o sorteio
		{
			while(tst_bit(PINB, SORTEIO));
			_delay_ms(10);
			Posiciona_Cursor(2, 1);
			Escreve_S_memdados("******");
			Escreve_Dado(0xFF);
			Escreve_Dado(0xFF);
			Escreve_Dado(0xFF);
			Escreve_S_memdados("*******");
			Posiciona_Cursor(2, 7);
			jogar = 1;

			//o bot�o deve ser pressionado uma vez para iniciar, tr�s vezes para jogar e mais uma vez para sair
			while(jogar < 5)
			{
				if(tst_bit(PINB, SORTEIO))
				{
					while(tst_bit(PINB, SORTEIO));
					_delay_ms(10);
					Escreve_Dado(pgm_read_byte(&caca_niquel[caractere]));
					jogar++;
				}
				caractere++;                      //gerar o caractere aleat�riamente
				if(caractere == 3) caractere = 0; //reiniciar a gera��o
			}
		}
	}
}
