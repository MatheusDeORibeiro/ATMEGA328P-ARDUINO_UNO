/*
 * s05_e19_caca_niquel_eletronico.c
 *
 * Created: 20/09/2022 15:08:11
 * Author : Matheus
 *
 * 5.19 – Crie um caça-níquel eletrônico empregando 3 caracteres diferentes apresentados
 * em 3 posições do LCD. Utilize um botão no pino PB2 do ATmega para o sorteio.
 */ 

#include "definicoes_principais.h" //inclui as definições principais
#include "LCD_4bits.h"             //inclui a biblioteca de funções do display LCD 16x2 com interface de 4 bits


//definição de hardware
#define SORTEIO PB3 //substitui SELECAO por PB3 na programação

//definição de caracteres do caça-níquel
const unsigned char caca_niquel[] PROGMEM = {0b00100100, 0b11101100, 0b00100010};

int main(void) //função principal
{	
	unsigned char jogar = 0, caractere = 0;

	DDRD = 0xFF;   //configura todo o PORTD como saída
	DDRB = 0xF7;   //configura o pino PB3 como entrada e o restande do PORTB como saída

	PORTD = 0x00;  //coloca todo o PORTD em nível baixo
	PORTB = 0x08;  //habilita o resistor pull-up da entrada e coloca em nível baixo o restante do PORTB

	Inicializa_LCD(LCD_2_N);                  //inicializa o display com duas linhas
	Display_Cursor_ON_OFF(LCD_DSP_ON_CS_OFF); //mantém a tela do display ligada e cursor não piscante

	while(1) //loop infinito
	{
		Limpa_Display();    //limpa a tela do display
		Cursor_Home();      //posiciona o cursor na primeira coluna da primeira linha
		Escreve_S_memdados("Caca-niquel AVR$");
		Posiciona_Cursor(2, 1);
		Escreve_S_memdados("Press o botao!!!");

		if(tst_bit(PINB, SORTEIO)) //se o botão for pressionado inicia o sorteio
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

			//o botão deve ser pressionado uma vez para iniciar, três vezes para jogar e mais uma vez para sair
			while(jogar < 5)
			{
				if(tst_bit(PINB, SORTEIO))
				{
					while(tst_bit(PINB, SORTEIO));
					_delay_ms(10);
					Escreve_Dado(pgm_read_byte(&caca_niquel[caractere]));
					jogar++;
				}
				caractere++;                      //gerar o caractere aleatóriamente
				if(caractere == 3) caractere = 0; //reiniciar a geração
			}
		}
	}
}
