/*
 * s09_e15_cont_sent_vel_motor_passo.c
 *
 * Created: 28/11/2022 18:43:40
 * Author : Matheus
 *
 * 9.15 – Elaborar um programa para controlar o motor de passo unipolar da
 * fig. 9.25. Dois botões controlam a direção de rotação e outros dois, a
 * velocidade. Primeiro, deve-se consultar o manual do fabricante do ULN2803 para
 * entender suas características e funcionamento.
 */

#include "definicoes_principais.h" //inclui a biblioteca de definições principais
#include "timers.h"                //inclui a biblioteca de funções dos módulos contadores/temporizadores


//definições de hardware
#define DIR         PD2   //substitui DIR por PD2 na programação
#define ESQ         PD3   //substitui ESQ por PD3 na programação
#define UP          PD6   //substitui UP por PD6 na programação
#define DOWN        PD7   //substitui DOWN por PD7 na programação
#define MOTOR_PASSO PORTB //define a porta em que o motor de passo unipolar será conectado

//definição de variáveis globais
volatile unsigned char conta_tempo = 0;      //variável auxiliar para gerar atraso para varredura de botões
volatile unsigned char sentido_giro = 1;     //definição do sentido de giro
volatile unsigned char velocidade_motor = 0; //definição da velocidade

ISR(TIMER0_OVF_vect); //definição da rotina de tratamento do estouro do TCNT0
ISR(TIMER2_OVF_vect); //definição da rotina de tratamento do estouro do TCNT2

int main(void) //função principal
{
    DDRD = 0x33;  //configura os pinos PD3:2 e PD7:6 como entradas e os pinos restantes como saída
    DDRB = 0xFF;  //configura todo o PORTB como saída
    PORTD = 0xCC; //habilita os resistores pull-up das entradas
    PORTB = 0x00; //coloca todo o PORTB em nível baixo

    Inicializa_TC0(TC0_modo_normal, TC0_OC0A_normal&TC0_OC0A_normal&TC0_CLK_1024, TC0_TOIE0);
    TCNT0 = 100;
    /* modo normal, f_osc igual a 16MHz, prescaler igual a 1024, saídas comparadoras desabilitadas,
       interrupção por estouro do TCNT0 habilitada, TCNT0 inicializado em 100 para jundo de 
       conta_tempo igual a 10 gerar um atraso de aproximadamente 100ms para varres as teclas:
       t_estouro = (1024 * ((255 - 100) + 1) * 10) / 16M = 99,8ms */

    Inicializa_TC2(TC2_modo_normal, TC2_OC2A_normal&TC2_OC2B_normal&TC2_CLK_1024, TC2_TOIE2);
    TCNT2 = velocidade_motor;
    /* modo normal, f_osc igual a 16MHz, prescaler igual a 1024, saídas comparadoras desabilitadas,
       interrupção por estouro do TCNT2 habilitada, TCNT2 inicializada em 0 para iniciar o motor
       com aproximadamente 61 passos por segundo */

    sei(); //habilita todas as fontes de interrupção setando o bit I do registrador SREG

    while(1); //loop infinito
}

ISR(TIMER0_OVF_vect) //implementação da rotina de tratamento do estouro do TCNT0
{
    conta_tempo++;        //incrmenta a variável de tempo auxiliar
    TCNT0 = 100;          //reinicia o registrador do T0
    if(conta_tempo == 10) //se a variável auxiliar for igual a 10...
    {
        if(!tst_bit(PIND, DIR)) sentido_giro = 1;      //sentido de giro para a direita
        else if(!tst_bit(PIND, ESQ)) sentido_giro = 0; //sentido de giro para a esquerda
        else if(!tst_bit(PIND, UP) && velocidade_motor < 250) velocidade_motor += 5; //incrementa a velocidade
        else if(!tst_bit(PIND, DOWN) && velocidade_motor > 0) velocidade_motor -= 5; //decrementa a velocidade
        conta_tempo = 0;  //reinicia a variável auxiliar
    }    
}

ISR(TIMER2_OVF_vect) //implementação da rotina de tratamento do estouro do TCNT2
{
    static unsigned char passo_motor = 0x08; //variável que armazena os passos do motor
    TCNT2 = velocidade_motor; //atualiza a velocidade
    MOTOR_PASSO = passo_motor & 0x0F;        //acionamento do motor
    if(sentido_giro) //se o sentido de giro for para a direita...
    {
        passo_motor >>= 1; //próximo passo
        if(passo_motor == 0x00) passo_motor = 0x08;
    }
    else             //se o sentido de giro for para a esquerda...
    {
        passo_motor <<= 1; //próximo passo
        if(passo_motor == 0x10) passo_motor = 0x01;    
    }
}