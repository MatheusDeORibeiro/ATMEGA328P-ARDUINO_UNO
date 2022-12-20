/*
 * s09_e02_2_pwm.c
 *
 * Created: 12/10/2022 14:35:43
 * Author : Matheus
 *
 * 9.2 – Considerando-se o PWM rápido configurado para que o valor máximo de
 * contagem do TC0 seja dado pelo valor de OCR0A e que o pino OC0B esteja
 * configurado para gerar o sinal PWM, determine:
 *
 * - Supondo que OCR0A é 200, qual deve ser o valor de OCR0B para que o ciclo
 * ativo do sinal PWM (OC0B) seja de 75 %?
 * T_oc0a_pwm = f_oc0a_pwm ^ -1 = (f_osc / (N * (1 + TOP))) ^ -1
 * T_oc0a_pwm = (12M / (1 * (1 + 200))) ^ -1
 * f_oc0a_pwm = 59,70kHz e T_oc0a_pwm = 16,75us
 *
 * T_ativo = duty_cycle / T_oc0a_pwm -> 75/100 = duty_cycle / 16,75u
 * duty_cycle = 12,56us
 *
 * t_estouro_0c0b = ((OCR0B + 1) * N )/ f_osc -> 12,56us = ((OCR0B + 1) * N) / 12M
 * OCR0B = 149,72 -> 0CR0B = 150
 */ 

#include "definicoes_principais.h" //inclui as definições principais


int main(void) //função principal
{
	DDRD = 0xFF;  //configura todo o PORTD como saída
	PORTD = 0x00; //coloca todo o PORTD em nível baixo

	TCCR0A = 0b01100011; //TOP = 0CR0A, PWM é gerado por OCR0B e o modo PWM rápido
	TCCR0B = 0b00001001; //compara TCNT0 com OCR0A e prescaler = 1
	OCR0A = 200;
	OCR0B = 150;
    /* modo PWM rápido, f_osc de 12MHz, prescaler igual a 1, 
       TOP igual a 200(período) e OCR0B igual a 150: 
       f_oc0a_PWM = 59,70kHz, T_oc0a_pwm = 16,75us e t_duty_cycle = 12,58us*/

	while(1); //loop infinito
}
