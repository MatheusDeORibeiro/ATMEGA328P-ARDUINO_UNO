/*
 * s05_e24_contador_ate_10000_botao.c
 *
 * Created: 26/09/2022 13:50:54
 * Author : Matheus
 *
 * 5.24 – Conte o número de vezes que um botão foi pressionado e apresente o valor
 * em um LCD 16 × 2. O botão pode ser conectado ao pino PB2 do ATmega e o circuito
 * do LCD pode ser o mesmo da fig. 5.23a.
 *
 */ 

#include "definicoes_principais.h" //biblioteca de definições principais
#include "LCD_4bits.h"             //biblioteca de funções do display de LCD 16x2 com interface de 4 bits


//definição de hardware
#define BOTAO PB3 //BOTAO é substituído por PB4 na programação

//declaração de vetor para armazenar digitos individualmente
unsigned char digitos[_tam_vetor];

int main(void) //função principal
{
	unsigned int contador; //declaração de variável local não inicializada
	
	DDRD = 0xFF; //configura todo o PORTD como saída
	DDRB = 0xF7; //configura o pino 4 do PORTB como entrada e os pinos restantes como saída

	PORTD = 0x00; //coloca todo o PORTD em nível baixo
	PORTB = 0x08; //habilita o resisitor pull-up da entrada e coloca os pinos restantes em nível baixo
	
	Inicializa_LCD(LCD_1_N);                  //inicializa display com uma linha
	Display_Cursor_ON_OFF(LCD_DSP_ON_CS_OFF); //mantém a tela do display ligada e desliga o cursor
	Limpa_Display();                          //limpa a tela do display
	Cursor_Home();                            //posiciona o cursor na primeira coluna da primeira linha
	Escreve_S_memdados("Press. o botao!");

	while(1) //loop infinito
	{
		contador = 0; //inicia variável
		while(contador < 10001)
		{
			if(!tst_bit(PINB, BOTAO)) //verifica se o botão foi pressionado
			{
				while(!tst_bit(PINB, BOTAO));
				_delay_ms(10);
				contador++;                            //incrementa o contador
				Decodifica_Dig_Ind(contador, digitos); //decodifica digitos individualmente
				Limpa_Display();                       //limpa tela do displa
				if(contador == 10000)                  //imprime a dezena de milhar
				{
					Posiciona_Cursor(1, 12);
					Escreve_Dado(digitos[4]);
					Escreve_Dado(digitos[3]);
					Escreve_Dado(digitos[2]);
					Escreve_Dado(digitos[1]);
					Escreve_Dado(digitos[0]);
				}
				if(contador >= 1000)                   //imprime a unidade de milhar
				{
					Posiciona_Cursor(1, 13);
					Escreve_Dado(digitos[3]);
					Escreve_Dado(digitos[2]);
					Escreve_Dado(digitos[1]);
					Escreve_Dado(digitos[0]);
				}
				if(contador >= 100)                    //imprime a centena
				{
					Posiciona_Cursor(1, 14);
					Escreve_Dado(digitos[2]);
					Escreve_Dado(digitos[1]);
					Escreve_Dado(digitos[0]);
				}
				if(contador >= 10)                     //imprime a dezena
				{
					Posiciona_Cursor(1, 15);
					Escreve_Dado(digitos[1]);
					Escreve_Dado(digitos[0]);
				}
				else                                   //imprime a unidade
				{
					Posiciona_Cursor(1, 16);
					Escreve_Dado(digitos[0]);
				}
				_delay_ms(100);                        //atraso para incrementar
			}
		}
	}
}
