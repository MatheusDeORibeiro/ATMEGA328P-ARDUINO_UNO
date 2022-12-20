/*
 * teste_modo_ctc.c
 *
 * Created: 12/10/2022 17:32:14
 * Author : Matheus de Oliveira Ribeiro
 */ 

#include "definicoes_principais.h"
#include "timers.h"


int main(void) //fun��o principal
{
	DDRD = 0xFF;  //configura todo o PORTD como sa�da
	PORTD = 0x00; //coloca todo o PORTD em n�vel baixo
	
	Inicializa_TC0(TC0_modo_CTC, TC0_OC0A_toggle&TC0_OC0B_toggle&TC0_CLK_1, TC0_INT_OFF);
	OCR0A = 200; //TOP(valor m�ximo)
	OCR0B = 100; //defasagem da sa�da B em rala��o � A
	/* modo CTC, f_osc de 16MHz, prescaler igual a 1,
	   TOP igual a 200, e valor de compara��o igual a 100:
	   f_oc0a = 39,8kHz, T_oc0a = 25,125us e t_estouro_oc0b = 6,3125us */
	/* a frequ�ncia e o per�odo � igual para ambas as ondas quadradas geradas pelas
	   sa�das e o defasagem da onda B em rela��o � onda A � igual ao tempo que leva 
	   para a igualdade de compara��o do comparador B */
	
    while(1); //loop infinito
}

