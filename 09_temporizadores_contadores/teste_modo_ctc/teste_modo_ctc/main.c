/*
 * teste_modo_ctc.c
 *
 * Created: 12/10/2022 17:32:14
 * Author : Matheus de Oliveira Ribeiro
 */ 

#include "definicoes_principais.h"
#include "timers.h"


int main(void) //função principal
{
	DDRD = 0xFF;  //configura todo o PORTD como saída
	PORTD = 0x00; //coloca todo o PORTD em nível baixo
	
	Inicializa_TC0(TC0_modo_CTC, TC0_OC0A_toggle&TC0_OC0B_toggle&TC0_CLK_1, TC0_INT_OFF);
	OCR0A = 200; //TOP(valor máximo)
	OCR0B = 100; //defasagem da saída B em ralação à A
	/* modo CTC, f_osc de 16MHz, prescaler igual a 1,
	   TOP igual a 200, e valor de comparação igual a 100:
	   f_oc0a = 39,8kHz, T_oc0a = 25,125us e t_estouro_oc0b = 6,3125us */
	/* a frequência e o período é igual para ambas as ondas quadradas geradas pelas
	   saídas e o defasagem da onda B em relação à onda A é igual ao tempo que leva 
	   para a igualdade de comparação do comparador B */
	
    while(1); //loop infinito
}

