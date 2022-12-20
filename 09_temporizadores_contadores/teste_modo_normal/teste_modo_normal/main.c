/*
 * teste_modo_normal.c
 *
 * Created: 12/10/2022 13:56:48
 * Author : Matheus de Oliveira Ribeiro
 */ 

#include "definicoes_principais.h"
#include "timers.h"


#define LED PB5 //substitui LED por RB5 na programa��o

ISR(TIMER0_OVF_vect) //rotina de interrup��o
{
	cpl_bit(PORTB, LED); //muda o estado do LED
}

int main(void) //fun��o principal
{
    DDRB = 0xFF;  //configura todo o PORTB como sa�da
	PORTB = 0x00; //coloca todo o PORTB em n�vel baixo
	
	Inicializa_TC0(TC0_modo_normal, TC0_CLK_1024, TC0_TOIE0);
	/* modo normal, f_osc de 16MHZ, prescaler igual a 1024:
	   t_estouro = 16,384us */
    /* o LED pisca a cada 16,384us, que � o tempo que leva para o estouro 
       do TC0 */
	sei(); //habilita todas as fontes de interrup��o
	
	while(1); //loop infinito
}
