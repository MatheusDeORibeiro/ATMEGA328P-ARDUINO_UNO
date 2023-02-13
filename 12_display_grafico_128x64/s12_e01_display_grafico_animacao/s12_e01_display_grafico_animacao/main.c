/*
 * s12_e01_display_grafico_animacao.c
 *
 * Created: 07/02/2023 16:57:36
 * Author : Matheus
 *
 * 12.1 � Elaborar um programa para gerar uma anima��o gr�fica com 5 quadros para
 * o circuito da fig. 12.13.
 */ 

#include "definicoes_principais.h"  //inclui a biblioteca de defini��es principais
#include "display_grafico_128x64.h" //inclui a biblioteca de fun��es para utilizar o display gr�fico
#include "ryu.h"                    //inclui a biblioteca que cont�m as imagens que ser�o exibidas


int main() //fun��o principal
{
	DDRD = 0xFF;   //configura todo o PORTD como sa�da
	DDRB = 0xFF;   //configura todo o PORTB como sa�da
	DDRC = 0xFF;   //configura todo o PORTC como sa�da
	UCSR0B = 0x00; //configura os pinos PD0 e PD1 como I/Os desabilitando as fun��es de RX e TX

	Inicializacao_Display();                    //Inicializa o display gr�fico
	Testa_Display();                            //liga todos os pixels do display
	_delay_ms(100);
	Limpa_Display();                            //limpa a tela do display
	_delay_ms(100);
	Escreve_S_memdados("Display pronto", 0, 0); //exibe uma mensagem

	while(1) //loop infinito
	{
		for(unsigned char frame = 0; frame < 6; frame++)         //percorre o ponteiro que aponta para os vetores que cont�m as imagens
		{
			Imprime_Imagem((PGM_P)pgm_read_word(&p_ryu[frame])); //imprime a imagem 'frame' na tela do display gr�fico
			_delay_ms(3);                                        //delay de 3ms para o efeito de persist�ncia da vis�o
		}
	}
}
