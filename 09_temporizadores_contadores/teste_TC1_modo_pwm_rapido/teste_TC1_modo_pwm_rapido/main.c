/*
 * teste_TC1_modo_pwm_rapido.c
 *
 * Created: 05/11/2022 14:46:30
 * Author : Matheus de Oliveira Ribeiro
 */ 

#include "definicoes_principais.h"
#include "timers.h"


int main(void) //fun��o principal
{
	DDRB = 0xFF;  //configura todo o PORTD como sa�da
	PORTB = 0x00; //coloca todo o PORTD em n�vel baixo
	
	Inicializa_TC1(TC1_modo_PWM_r_TOP_ICR1, TC1_OC1A_n_inv&TC1_OC1B_n_inv&TC1_CLK_1, TC1_INT_OFF);
	ICR1 = 35000; //valor m�ximo para contagem
	OCR1A = 2000;  //valor do ciclo ativo do OC1A
	OCR1B = 100;   //valor do ciclo ativo do OC1B
	/* TC1 modo PWM r�pido com valor TOP igual a 35000(OCRA1), com f_osc igual a 16MHz,
	   com as sa�das comparadoras OC1A e OC1B gerando, cada uma, um sinal de PWM n�o invertido:
	   f_PWM_r = 457,13Hz e T_PWM_r = 2,19ms e t_dc_A = 125us e t_dc_B = 6,25us */
	/* o registrador ICR1 controla o per�odo dos dois sinais de PWM gerados pelas sa�das
	   comparadoras A e B, e os registradores OCR1A e OCR1B, das sa�das comparadoras OC1A e OC1B,
	   controlam o per�odo ativo de cada sinal PWM */
	
    while(1); //loop infinito
}
