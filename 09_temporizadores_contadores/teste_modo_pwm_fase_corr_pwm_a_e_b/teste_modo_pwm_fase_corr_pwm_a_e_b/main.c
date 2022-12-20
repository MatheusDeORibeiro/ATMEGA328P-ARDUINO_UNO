/*
 * teste_modo_pwm_fase_corr_pwm_a_e_b.c
 *
 * Created: 13/10/2022 18:59:19
 * Author : Matheus de Oliveira Ribeiro
 */ 

#include "definicoes_principais.h"
#include "timers.h"


int main(void) //função principal
{
	DDRD = 0xFF;  //configura todo o PORTD como saída
	PORTD = 0x00; //coloca todo o PORTD em nível baixo
	
	Inicializa_TC0(TC0_modo_pwm_fc_TOP_255, TC0_OC0A_n_inv&TC0_OC0B_n_inv&TC0_CLK_1, TC0_INT_OFF);
	OCR0A = 100; //tempo ativo do sinal pwm gerado pela saída comparadora A
    OCR0B = 50;  //tempo ativo do sinal pwm gerado pela saída comparadora B
	/* modo pwm com fase corrigida, f_osc igual a 16MHz, prescaler igual a 1,
	   TOP igual a 255, saidas comparadoras A e B modo não invertido: */
	/* f_oc0a_pwm = 31,25kHz e T_oc0A_pwm = 32us, dc_oc0a = 12,625us e dc_oc0b = 6,375us */
	
	while(1); //loop infinito
}

