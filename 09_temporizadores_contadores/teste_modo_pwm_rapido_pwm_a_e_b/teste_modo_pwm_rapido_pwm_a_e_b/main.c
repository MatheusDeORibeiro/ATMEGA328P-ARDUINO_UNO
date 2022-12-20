/*
 * teste_modo_pwm_rapido_pwm_a_e_b.c
 *
 * Created: 13/10/2022 17:33:43
 * Author : Matheus de Oliveira Ribeiro
 */ 

#include "definicoes_principais.h"
#include "timers.h"


int main(void) //função principal
{
	DDRD = 0xFF;  //configura todo o PORTD como saída
	PORTD = 0x00; //coloca todo o PORTD em nível baixo
	
	Inicializa_TC0(TC0_modo_pwm_r_TOP_255, TC0_OC0A_n_inv&TC0_OC0B_n_inv&TC0_CLK_64, TC0_INT_OFF);
	OCR0A = 200; //ciclo ativo(duty cycle) da saída OC0A
	OCR0B = 50;  //ciclo ativo(duty cycle) da saída OC0B
	/* modo pwm rápido com TOP igual a 255, f_osc de 16MHz, prescaler igual a 64 e
	   saídas de comparação não invertidas: 
	   f_OC0a_pwm = 976,56Hz, T_OC0a_pwm = 1,024ms e t_estouro_OC0b = 204us */
	/* O período de ambos os sinais de pwm gerados pelos comparadores será de 
	   1,024ms, ele é fixo e determinado por TOP, que é igual a 255, e pelo 
	   prescaler */
	
	
    while(1); //loop infinito
}

