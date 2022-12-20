/*
 * teste_TC1_PWM_fase_e_freq_corrigidas.c
 *
 * Created: 08/11/2022 14:59:27
 * Author : Matheus de Oliveira Ribeiro
 */ 

#include "definicoes_principais.h"
#include "timers.h"


int main(void) //função principal
{
    DDRB = 0xFF;  //configura todo o PORTB como saída
	PORTB = 0x00; //coloca todo o PORTB em nível baixo
	
	Inicializa_TC1(TC1_modo_PWM_ffc_TOP_OCR1A, TC1_OC1A_normal&TC1_OC1B_n_inv&TC1_CLK_1, TC1_INT_OFF);
	OCR1A = 300; //valor máximo para contagem
	OCR1B = 100; //controle o ciclo ativo do OC1B
	/* modo PWM com fase e frequências corrigidas com valor TOP igual a 300 (OCR1A), 
	   com f_osc igual a 16MHz, prescaler igual a 1, com a saída comparadora A dasabilitada 
	   e a saída comparadora B gerando o sinal PWM não invertido:
	   f_PWM_ffc_OCR1A = 26,67kHz, T_PWM_ffc_OCR1A = 37,5ms e t_dc = 12,63ms */
	/* o registrador OCR1A controla o período do sinal PWM gerado pela saída comparadora B,
	   e o registrador OCR1B controla o ciclo ativo do sinal */
	
	while(1); //loop infinito
}
