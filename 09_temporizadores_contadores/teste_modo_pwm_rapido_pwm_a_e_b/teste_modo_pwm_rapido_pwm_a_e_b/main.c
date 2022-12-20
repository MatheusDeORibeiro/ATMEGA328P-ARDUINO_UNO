/*
 * teste_modo_pwm_rapido_pwm_a_e_b.c
 *
 * Created: 13/10/2022 17:33:43
 * Author : Matheus de Oliveira Ribeiro
 */ 

#include "definicoes_principais.h"
#include "timers.h"


int main(void) //fun��o principal
{
	DDRD = 0xFF;  //configura todo o PORTD como sa�da
	PORTD = 0x00; //coloca todo o PORTD em n�vel baixo
	
	Inicializa_TC0(TC0_modo_pwm_r_TOP_255, TC0_OC0A_n_inv&TC0_OC0B_n_inv&TC0_CLK_64, TC0_INT_OFF);
	OCR0A = 200; //ciclo ativo(duty cycle) da sa�da OC0A
	OCR0B = 50;  //ciclo ativo(duty cycle) da sa�da OC0B
	/* modo pwm r�pido com TOP igual a 255, f_osc de 16MHz, prescaler igual a 64 e
	   sa�das de compara��o n�o invertidas: 
	   f_OC0a_pwm = 976,56Hz, T_OC0a_pwm = 1,024ms e t_estouro_OC0b = 204us */
	/* O per�odo de ambos os sinais de pwm gerados pelos comparadores ser� de 
	   1,024ms, ele � fixo e determinado por TOP, que � igual a 255, e pelo 
	   prescaler */
	
	
    while(1); //loop infinito
}

