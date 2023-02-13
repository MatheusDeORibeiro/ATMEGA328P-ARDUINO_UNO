/*
 * s11_e1_sequencial_de_leds.c
 *
 * Created: 23/12/2022 14:53:09
 * Author : Matheus
 *
 * 11.1 – Baseado no exercício 5.12, fig. 5.11a, ligue sequencialmente os 8 LEDs.
 * Comece com uma frequência visível e a aumente progressivamente até que os LEDs
 * pareçam estar todos ligados. Qual o tempo que cada LED ficou ligado para a
 * persistência da visão?
 *
 */ 

#include "definicoes_principais.h" //inclui as definições principais
#include "timers.h"                //inclui a biblioteca de funções dos módulos temporizador/contador


ISR(TIMER0_OVF_vect); //declaração da rotina de tratamento do estouro do TCNT0

int main(void) //função principal
{
    DDRD = 0xFF;  //define todo o PORTD como saída
    PORTD = 0x00; //inicializa todos os pinos do PORTD em nível baixo

    UCSR0B = 0x00; //configura os pinos PD0 e PD1 como I/Os digitais

    Inicializa_TC0(TC0_modo_normal, TC0_OC0A_normal&TC0_OC0B_normal&TC0_CLK_1024, TC0_TOIE0);
    /* modo normal, f_osc igual a 16MH, prescaler igual a 1024, saídas comparadoras desabilitadas
       interrupção por estouro do TCNT0 habilitada */
    sei(); //habilita todas a fontes de interrupção setando o bit I do registrador SREG

    while(1); //loop infinito
}

ISR(TIMER0_OVF_vect) //declaração da rotina de tratamento do estouro do TCNT0
{
    static unsigned char seq_leds = 0x01;  //variável auxiliar para rotacionar os leds
    static unsigned char cont_freq = 0x00; //variável auxiliar para controle da frequência
    TCNT0 = cont_freq;                     //atualiza o valor da frequência
    PORTD = seq_leds;                      //atualiza o PORTD
    seq_leds <<= 1;                        //rotaciona os leds
    if(seq_leds == 0x00)
    {
	    seq_leds = 0x01;                     //reinicializa a variável auxiliar
	    if(cont_freq < 250) cont_freq += 10; //aumenta a frequência visível
    }
}
