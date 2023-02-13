/*
 * s12_e01_display_grafico_animacao.c
 *
 * Created: 07/02/2023 16:57:36
 * Author : Matheus
 *
 * 12.1 – Elaborar um programa para gerar uma animação gráfica com 5 quadros para
 * o circuito da fig. 12.13.
 */ 

#include "definicoes_principais.h"  //inclui a biblioteca de definições principais
#include "display_grafico_128x64.h" //inclui a biblioteca de funções para utilizar o display gráfico
#include "ryu.h"                    //inclui a biblioteca que contém as imagens que serão exibidas


int main() //função principal
{
	DDRD = 0xFF;   //configura todo o PORTD como saída
	DDRB = 0xFF;   //configura todo o PORTB como saída
	DDRC = 0xFF;   //configura todo o PORTC como saída
	UCSR0B = 0x00; //configura os pinos PD0 e PD1 como I/Os desabilitando as funções de RX e TX

	Inicializacao_Display();                    //Inicializa o display gráfico
	Testa_Display();                            //liga todos os pixels do display
	_delay_ms(100);
	Limpa_Display();                            //limpa a tela do display
	_delay_ms(100);
	Escreve_S_memdados("Display pronto", 0, 0); //exibe uma mensagem

	while(1) //loop infinito
	{
		for(unsigned char frame = 0; frame < 6; frame++)         //percorre o ponteiro que aponta para os vetores que contém as imagens
		{
			Imprime_Imagem((PGM_P)pgm_read_word(&p_ryu[frame])); //imprime a imagem 'frame' na tela do display gráfico
			_delay_ms(3);                                        //delay de 3ms para o efeito de persistência da visão
		}
	}
}
