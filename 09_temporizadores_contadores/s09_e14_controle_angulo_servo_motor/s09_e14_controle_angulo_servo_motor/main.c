/*
 * s09_e14_controle_angulo_servo_motor.c
 *
 * Created: 22/11/2022 19:21:07
 * Author : Matheus
 *
 * 9.14 � Elaborar um programa para controlar o �ngulo de giro de um micro servo
 * motor de acordo com dois bot�es, fig. 9.20. Come�ando com um pulso de 0,5 ms
 * at� 2,5 ms, com passo de incremento de 0,1 ms. Repita o procedimento para um
 * passo de 0,05 ms. Qual a rela��o angular de giro com a largura de pulso? R: 90�/s.
 * Obs.: � aconselhado que a alimenta��o do motor seja independente do circuito
 * de controle, pois podem haver picos de corrente prejudiciais, no caso do
 * microcontrolador, ele pode ser inicializado. Tudo vai depender das
 * caracter�sticas da fonte de alimenta��o.
 *
 */ 

#include "definicoes_principais.h" //inclui as defini��es principais
#include "timers.h"                //inclui a biblioteca de fun��es dos timers


//defini��es de hardware
#define INCR PD2      //substitui INCR por PD2 na programa��o
#define DECR PD3      //substitui DECR por PD3 na programa��o
//#define saida_PWM PB1 //substitui saida_PWM por PB1 na programa��o

#define PASSO 100     //definindo o passo do incremento do motor
                      //passo de 0,1ms -> 200
                      //passo de 0,05ms -> 100

volatile unsigned char conta_tempo = 0; //declara��o de vari�vel auxiliar para contagem do tempo

ISR(TIMER0_OVF_vect); //declara��o de rotina de tratamento do estouro do TCNT0

int main(void) //fun��o principal
{
    DDRD = 0xF3;  //configura PD3:2 como entradas e os pinos restantes como sa�das
    DDRB = 0xFF;  //configura todo o PORTB como sa�da
    DDRC = 0xFF;  //configura todo o PORTC como sa�da
    PORTD = 0x0C; //habilita os resistores pull-up das entradas e coloca os pinos restantes em n�vel baixo
    PORTB = 0x00; //coloca todo o PORTB em n�vel baixo
    PORTC = 0x00; //coloca todo o PORTC em n�vel baixo

    Inicializa_TC0(TC0_modo_normal, TC0_CLK_1024, TC0_TOIE0);
    /* modo normal com valor TOP igual a 100(TCNT0), f_osc igual 16MHz, prescaler igual a 1024, 
       sa�das comparadoras desabilitadas, interrup��o por estouro do TCNT0 habilitada:
       t_estouro = 9,98ms, com a vari�vel conta_temp igual a 10, t_estouro = 99,8ms */
    TCNT0 = 100; //para gerar varrer os bot�es a cada 99,8ms

    Inicializa_TC1(TC1_modo_PWM_r_TOP_ICR1, TC1_OC1A_n_inv&TC1_CLK_8, TC1_INT_OFF);
    /* modo PWM r�pido com valor TOP igual a 39999(ICR1), f_osc igual a 16MHz, prescaler igual a 8,
       sa�da comparadora A n�o inversora, interrup��es desabilitadas:
       T_PWM = 20ms*/
    ICR1 = 39999; //defini��o do per�odo
    OCR1A = 1000; //defini��o do ciclo ativo m�nimo

    sei(); //habilita todas as fontes de interrup��o setando o bit I do registrador SREG

    while(1); //loop infinito
}

ISR(TIMER0_OVF_vect) //implementando a rotina de tratamento do estouro do TCNT0
{
    conta_tempo++;        //incrementa a vari�vel auxiliar do tempo
    if(conta_tempo == 10) //se a vari�vel auxiliar for igual a 10...
    {
        if(!tst_bit(PIND, INCR) && OCR1A < 5000)      OCR1A += PASSO; //incrementa o �ngulo de giro do motor
        else if(!tst_bit(PIND, DECR) && OCR1A > 1000) OCR1A -= PASSO; //decrementa o �ngulo do giro do motor
        conta_tempo = 0;                                              //reinicia a vari�vel auxiliar do tempo
    }
}
