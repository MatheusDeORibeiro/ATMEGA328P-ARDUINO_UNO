/*
 * teste_modo_pwm_rapido_pwm_periodo_v.c
 *
 * Created: 13/10/2022 17:41:45
 * Author : Matheus de Oliveira Ribeiro
 */ 

#include "definicoes_principais.h"
#include "timers.h"


int main(void) //função principal
{
	DDRD = 0xFF;  //configura todo o PORTD como saída
	PORTD = 0x00; //coloca todo o PORTD em nível baixo
	
	Inicializa_TC0(TC0_modo_pwm_r_TOP_OCR0A, TC0_OC0A_toggle&TC0_OC0B_n_inv&TC0_CLK_1, TC0_INT_OFF);
	OCR0A = 100; //controle do período do sinal pwm
	OCR0B = 30;  //controle o duty cycle do sinal pwm
	/* modo pwm rápido com TOP igual a 100 (OCR0A), f_osc igual a 16MHz, 
	   prescaler igual a 1, saída comparadora A mudando de  estado a cada igualdade e
	   saída comparadora B gerando pwm com sinal não invertido:
	   f_OC0A_pwm = 158,42kHz, T_OC0A_pwm = 6,3125us e duty_cycle = 1,0375us */
	/* o registrador OCR0A controla o período do sinal pwm gerado pela saída comparadora
	   B, gerando, assim, apenas uma onda quadrada, já o registrador OCR0B controla o
	   ciclo ativo do sinal pwm gerado pela saída comparadora B */
	
    while(1); //loop infinito
}
