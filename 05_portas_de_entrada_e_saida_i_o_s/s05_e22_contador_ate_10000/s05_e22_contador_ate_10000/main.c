/*
 * s05_e22_contador_ate_10000.c
 *
 * Created: 23/09/2022 18:53:43
 * Author : Matheus
 *
 * 5.22 – Crie um programa que conte até 10.000 apresentando a contagem decimal em
 * um LCD 16 × 2. Utilize um tempo de 100 ms para o incremento dos números, os
 * dígitos devem ficar à direita do LCD e o número zero na frente do digito mais
 * significativo não deve ser apresentado.
 */ 

#include "definicoes_principais.h" //biblioteca de definições principais
#include "LCD_4bits.h"             //biblioteca de funções do display LCD 16x2 com interface de 4 bits


//declaração de variável
unsigned char digitos[_tam_vetor]; //vetor para armazenar os digitos individualmente

int main(void) //função principal
{
	unsigned int contador; //declaração de variável local

	DDRD = 0xFF; //configura todo o PORTD como saída
	DDRB = 0xFF; //configura todo o PORTB como saída

	PORTD = 0x00; //coloca todo o PORTD em nível baixo
	PORTB = 0x00; //coloca todo o PORTB em nível baixo

	Inicializa_LCD(LCD_1_N);                  //inicializa dispay LCD com uma linha
	Display_Cursor_ON_OFF(LCD_DSP_ON_CS_OFF); //matém tela do display ligada e desliga o cursor
	Limpa_Display();                          //limpa tela do display
	Cursor_Home();                            //posiciona o cursor na primeira coluna da primeira linha
	
	while(1) //loop infinito
	{
		for(contador = 0; contador < 10001; contador++)
		{
			Limpa_Display();                       //limpa tela do dispay
			Decodifica_Dig_Ind(contador, digitos); //decodifica o número inteiro
			if(contador == 10000)                  //escreve a dezena de milhar
			{
				Posiciona_Cursor(1, 12);
				Escreve_Dado(digitos[4]);
				Escreve_Dado(digitos[3]);
				Escreve_Dado(digitos[2]);
				Escreve_Dado(digitos[1]);
				Escreve_Dado(digitos[0]);
			}
			if(contador >= 1000)                   //escreve a unidade de milhar
			{
				Posiciona_Cursor(1, 13);
				Escreve_Dado(digitos[3]);
				Escreve_Dado(digitos[2]);
				Escreve_Dado(digitos[1]);
				Escreve_Dado(digitos[0]);
			}
			if(contador >= 100)                    //escreve a centena
			{
				Posiciona_Cursor(1, 14);
				Escreve_Dado(digitos[2]);
				Escreve_Dado(digitos[1]);
				Escreve_Dado(digitos[0]);
			}
			if(contador >= 10)                     //escreve a dezena
			{
				Posiciona_Cursor(1, 15);
				Escreve_Dado(digitos[1]);
				Escreve_Dado(digitos[0]);
			}
			else                                   //escreve a unidade
			{
				Posiciona_Cursor(1, 16);
				Escreve_Dado(digitos[0]);
			}
			_delay_ms(100);                        //atraso para o incremento
		}
	}
}
