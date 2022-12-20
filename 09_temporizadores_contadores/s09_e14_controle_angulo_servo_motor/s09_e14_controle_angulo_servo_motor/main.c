/*
 * s09_e14_controle_angulo_servo_motor.c
 *
 * Created: 22/11/2022 19:21:07
 * Author : Matheus
 *
 * 9.14 – Elaborar um programa para controlar o ângulo de giro de um micro servo
 * motor de acordo com dois botões, fig. 9.20. Começando com um pulso de 0,5 ms
 * até 2,5 ms, com passo de incremento de 0,1 ms. Repita o procedimento para um
 * passo de 0,05 ms. Qual a relação angular de giro com a largura de pulso? R: 90°/s.
 * Obs.: é aconselhado que a alimentação do motor seja independente do circuito
 * de controle, pois podem haver picos de corrente prejudiciais, no caso do
 * microcontrolador, ele pode ser inicializado. Tudo vai depender das
 * características da fonte de alimentação.
 *
 */ 

#include "definicoes_principais.h" //inclui as definições principais
#include "timers.h"                //inclui a biblioteca de funções dos timers


//definições de hardware
#define INCR PD2      //substitui INCR por PD2 na programação
#define DECR PD3      //substitui DECR por PD3 na programação
//#define saida_PWM PB1 //substitui saida_PWM por PB1 na programação

#define PASSO 100     //definindo o passo do incremento do motor
                      //passo de 0,1ms -> 200
                      //passo de 0,05ms -> 100

volatile unsigned char conta_tempo = 0; //declaração de variável auxiliar para contagem do tempo

ISR(TIMER0_OVF_vect); //declaração de rotina de tratamento do estouro do TCNT0

int main(void) //função principal
{
    DDRD = 0xF3;  //configura PD3:2 como entradas e os pinos restantes como saídas
    DDRB = 0xFF;  //configura todo o PORTB como saída
    DDRC = 0xFF;  //configura todo o PORTC como saída
    PORTD = 0x0C; //habilita os resistores pull-up das entradas e coloca os pinos restantes em nível baixo
    PORTB = 0x00; //coloca todo o PORTB em nível baixo
    PORTC = 0x00; //coloca todo o PORTC em nível baixo

    Inicializa_TC0(TC0_modo_normal, TC0_CLK_1024, TC0_TOIE0);
    /* modo normal com valor TOP igual a 100(TCNT0), f_osc igual 16MHz, prescaler igual a 1024, 
       saídas comparadoras desabilitadas, interrupção por estouro do TCNT0 habilitada:
       t_estouro = 9,98ms, com a variável conta_temp igual a 10, t_estouro = 99,8ms */
    TCNT0 = 100; //para gerar varrer os botões a cada 99,8ms

    Inicializa_TC1(TC1_modo_PWM_r_TOP_ICR1, TC1_OC1A_n_inv&TC1_CLK_8, TC1_INT_OFF);
    /* modo PWM rápido com valor TOP igual a 39999(ICR1), f_osc igual a 16MHz, prescaler igual a 8,
       saída comparadora A não inversora, interrupções desabilitadas:
       T_PWM = 20ms*/
    ICR1 = 39999; //definição do período
    OCR1A = 1000; //definição do ciclo ativo mínimo

    sei(); //habilita todas as fontes de interrupção setando o bit I do registrador SREG

    while(1); //loop infinito
}

ISR(TIMER0_OVF_vect) //implementando a rotina de tratamento do estouro do TCNT0
{
    conta_tempo++;        //incrementa a variável auxiliar do tempo
    if(conta_tempo == 10) //se a variável auxiliar for igual a 10...
    {
        if(!tst_bit(PIND, INCR) && OCR1A < 5000)      OCR1A += PASSO; //incrementa o ângulo de giro do motor
        else if(!tst_bit(PIND, DECR) && OCR1A > 1000) OCR1A -= PASSO; //decrementa o ângulo do giro do motor
        conta_tempo = 0;                                              //reinicia a variável auxiliar do tempo
    }
}
