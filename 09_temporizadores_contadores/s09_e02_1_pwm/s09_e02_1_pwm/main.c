/*
 * s09_e02_1_pwm.c
 *
 * Created: 12/10/2022 14:30:30
 * Author : Matheus
 *
 * 9.2 � Considerando-se o PWM r�pido configurado para que o valor m�ximo de
 * contagem do TC0 seja dado pelo valor de OCR0A e que o pino OC0B esteja
 * configurado para gerar o sinal PWM, determine:
 *
 * - Qual o per�odo do sinal PWM se o ATmega328 estiver rodando a 12 MHz e o
 * valor de OCR0A for 99?
 * T_oc0a_pwm = f_oc0a_pwm ^ -1 = (f_osc / (N * (1 + TOP)) ^ -1
 * T_oc0a_pwm = (12M / (1 * (1 + 99)) ^ -1 -> T_oc0a_pwm = 8,33us
 */ 

#include "definicoes_principais.h" //inclui a biblioteca de defini��es principais


int main(void) //fun��o principal
{
    DDRD = 0xFF;  //configura todo o PORTD como sa�da
    PORTD = 0x00; //coloca todo o PORTD em n�vel baixo

    TCCR0A = 0b01100011; //TOP = OCR0A, PWM � gerado por OCR0B e o modo PWM r�pido
    TCCR0B = 0b00001001; //compara TCNT0 com OCR0A e prescaler = 1
    OCR0A = 99; //TOP
    /* modo PWM r�pido, f_osc de 12MHz, prescaler igual a 1, TOP igual a 99: 
       f_oc0a_pwm = 120kHz e T_oc0a_pwm = 8,33us */

    while(1); //loop infinito
}

