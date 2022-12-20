/*
 * teste_modo_pwm_rapido_pwm_periodo_v.c
 *
 * Created: 13/10/2022 17:41:45
 * Author : Matheus de Oliveira Ribeiro
 */ 

#include "definicoes_principais.h"
#include "timers.h"


int main(void) //fun��o principal
{
	DDRD = 0xFF;  //configura todo o PORTD como sa�da
	PORTD = 0x00; //coloca todo o PORTD em n�vel baixo
	
	Inicializa_TC0(TC0_modo_pwm_r_TOP_OCR0A, TC0_OC0A_toggle&TC0_OC0B_n_inv&TC0_CLK_1, TC0_INT_OFF);
	OCR0A = 100; //controle do per�odo do sinal pwm
	OCR0B = 30;  //controle o duty cycle do sinal pwm
	/* modo pwm r�pido com TOP igual a 100 (OCR0A), f_osc igual a 16MHz, 
	   prescaler igual a 1, sa�da comparadora A mudando de  estado a cada igualdade e
	   sa�da comparadora B gerando pwm com sinal n�o invertido:
	   f_OC0A_pwm = 158,42kHz, T_OC0A_pwm = 6,3125us e duty_cycle = 1,0375us */
	/* o registrador OCR0A controla o per�odo do sinal pwm gerado pela sa�da comparadora
	   B, gerando, assim, apenas uma onda quadrada, j� o registrador OCR0B controla o
	   ciclo ativo do sinal pwm gerado pela sa�da comparadora B */
	
    while(1); //loop infinito
}
