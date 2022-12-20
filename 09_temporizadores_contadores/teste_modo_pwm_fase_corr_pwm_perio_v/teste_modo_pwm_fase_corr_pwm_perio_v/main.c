/*
 * teste_modo_pwm_fase_corr_pwm_perio_v.c
 *
 * Created: 15/10/2022 17:04:19
 * Author : Matheus de Oliveira Ribeiro
 */ 

#include "definicoes_principais.h"
#include "timers.h"


int main(void) //função principal
{
	DDRD = 0xFF;  //configura todo o PORTD como saída
	PORTD = 0x00; //coloca todo o PORTD em nível baixo
	
	Inicializa_TC0(TC0_modo_pwm_fc_TOP_OCR0A, TC0_OC0A_toggle&TC0_OC0B_n_inv&TC0_CLK_1, TC0_INT_OFF);
	OCR0A = 200; //periodo do sinal pwm gerado pela saída comparadora B
	OCR0B = 10;  //tempo ativo do sinal pwm gerado pela saída comparadora B
	/* modo pwm com fase corrigida, fosc é igual a 16MHz, prescaler iguala 1,
	   TOP igual a OCR0A, saída de comparação A muda de estado a cada igualdade
	   de comparação e saída de comparação B em modo não inverso: */
	/* f_OC0a_pwm = 39,8kHz e T_OC0a_pwm = 25,125us, tc_OC0b = 1,375us*/
    
	while(1); //loop infinito
}
