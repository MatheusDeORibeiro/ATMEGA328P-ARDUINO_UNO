/*
 * s09_e11_gerando_4_PWM_software.c
 *
 * Created: 15/11/2022 13:15:57
 * Author : Matheus
 *
 * 9.11 � Fa�a um programa para gerar 4 sinais PWM com resolu��o de 100, 400, 1000
 * e 5000 pontos. Escolha a forma de trabalho do TC empregado.
 */ 

#include "definicoes_principais.h" //inclui a biblioteca de defini��es principais
#include "timers.h"                //inclui a biblioteca de fun��es dos m�dulos TC


//defini��es de hardware
#define pino_PWM1 PB0 //substitui pino_PWM1 por PB0 na programa��o
#define pino_PWM2 PB1 //substitui pino_PWM2 por PB1 na programa��o
#define pino_PWM3 PB2 //substitui pino_PWM3 por PB2 na programa��o
#define pino_PWM4 PB3 //substitui pino_PWM4 por PB3 na programa��o

//defini��es dos sinais PWM - com o TCNT0 estourando a cada 16us
#define periodo_PWM1 100  //resolu��o do sinal de PWM 1 (per�odo = 1,6ms -> f = 625Hz)
#define periodo_PWM2 400  //resolu��o do sinal de PWM 2 (per�odo = 6,4ms -> f = 156,25Hz)
#define periodo_PWM3 1000 //resolu��o do sinal de PWM 3 (per�odo = 16ms -> f = 62,5Hz)
#define periodo_PWM4 5000 //resolu��o do sinal de PWM 4 (per�odo = 80ms -> f = 12,5Hz)

unsigned char dc_PWM1 = 25;  //ciclo ativo do sinal de PWM 1
unsigned int dc_PWM2 = 100;  //ciclo ativo do sinal de PWM 2
unsigned int dc_PWM3 = 500;  //ciclo ativo do sinal de PWM 3
unsigned int dc_PWM4 = 1250; //ciclo ativo do sinal de PWM 4

volatile unsigned int quant_estouros = 0; //vari�vel que armazena a quantidade de estouros do TCNT0

ISR(TIMER0_OVF_vect); //declara��o do prot�tipo da rotina de tratamento da interrup��o por estouro do TCNT0

int main(void) //fun��o principal
{
    DDRB = 0xFF;  //configura todo o PORTB como sa�da
    PORTB = 0xFF; //coloca todo o PORTB em n�vel alto

    Inicializa_TC0(TC0_modo_normal, TC0_CLK_1, TC0_TOIE0);
    /* temporizador contador 0 no modo normal, f_osc igual a 16MHz, prescaler igual a 1, 
       sa�das comparadoras desabilitadas, interrup��o por estouro do TCNT0 habilitada:
       t_estouro = 16us */
    sei(); //habilita todas as fontes de interrup��o setando o bit I do registrador SREG

    while(1); //loop infinito
}

ISR(TIMER0_OVF_vect) //implementa��o da rotina de tratamento da interrup��o por estouro do TCNT0
{
    static unsigned int n_PWM1, n_PWM2, n_PWM3, n_PWM4 = 0; //vari�veis est�ticas auxiliares
    quant_estouros++; //incrementa a vari�vel que armazena a quantidade de estouros do TCNT0
    //gera PWM1
    if(quant_estouros - n_PWM1 == periodo_PWM1) //controle do per�odo
    {
        set_bit(PORTB, pino_PWM1);
        n_PWM1 = quant_estouros;
    }
    if(quant_estouros - n_PWM1 == dc_PWM1)      //controle do ciclo ativo
        clr_bit(PORTB, pino_PWM1);
    //gera PWM2
    if(quant_estouros - n_PWM2 == periodo_PWM2) //controle do per�odo
    {
        set_bit(PORTB, pino_PWM2);
        n_PWM2 = quant_estouros;
    }
    if(quant_estouros - n_PWM2 == dc_PWM2)      //controle do ciclo ativo
        clr_bit(PORTB, pino_PWM2);
    //gera PWM3
    if(quant_estouros - n_PWM3 == periodo_PWM3) //controle do per�odo
    {
        set_bit(PORTB, pino_PWM3);
        n_PWM3 = quant_estouros;
    }
    if(quant_estouros - n_PWM3 == dc_PWM3)      //controle do ciclo ativo
        clr_bit(PORTB, pino_PWM3);
    //gera PWM4
    if(quant_estouros - n_PWM4 == periodo_PWM4) //controle do per�odo
    {
        set_bit(PORTB, pino_PWM4);
        n_PWM4 = quant_estouros;
    }
    if(quant_estouros - n_PWM4 == dc_PWM4)      //controle do ciclo ativo
        clr_bit(PORTB, pino_PWM4);
    //para evitar que o estouro da vari�vel quant_estouros desincronize os sinais
	if(n_PWM1 == quant_estouros && n_PWM2 == quant_estouros && n_PWM3 == quant_estouros && n_PWM4 == quant_estouros)
	    quant_estouros = 0, n_PWM1 = 0, n_PWM2 = 0, n_PWM3 = 0, n_PWM4 = 0;
}  
