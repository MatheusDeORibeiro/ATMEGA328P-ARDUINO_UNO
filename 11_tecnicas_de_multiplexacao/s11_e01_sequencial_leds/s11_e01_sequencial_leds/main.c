/*
 * s11_e1_sequencial_de_leds.c
 *
 * Created: 23/12/2022 14:53:09
 * Author : Matheus
 *
 * 11.1 � Baseado no exerc�cio 5.12, fig. 5.11a, ligue sequencialmente os 8 LEDs.
 * Comece com uma frequ�ncia vis�vel e a aumente progressivamente at� que os LEDs
 * pare�am estar todos ligados. Qual o tempo que cada LED ficou ligado para a
 * persist�ncia da vis�o?
 *
 */ 

#include "definicoes_principais.h" //inclui as defini��es principais
#include "timers.h"                //inclui a biblioteca de fun��es dos m�dulos temporizador/contador


ISR(TIMER0_OVF_vect); //declara��o da rotina de tratamento do estouro do TCNT0

int main(void) //fun��o principal
{
    DDRD = 0xFF;  //define todo o PORTD como sa�da
    PORTD = 0x00; //inicializa todos os pinos do PORTD em n�vel baixo

    UCSR0B = 0x00; //configura os pinos PD0 e PD1 como I/Os digitais

    Inicializa_TC0(TC0_modo_normal, TC0_OC0A_normal&TC0_OC0B_normal&TC0_CLK_1024, TC0_TOIE0);
    /* modo normal, f_osc igual a 16MH, prescaler igual a 1024, sa�das comparadoras desabilitadas
       interrup��o por estouro do TCNT0 habilitada */
    sei(); //habilita todas a fontes de interrup��o setando o bit I do registrador SREG

    while(1); //loop infinito
}

ISR(TIMER0_OVF_vect) //declara��o da rotina de tratamento do estouro do TCNT0
{
    static unsigned char seq_leds = 0x01;  //vari�vel auxiliar para rotacionar os leds
    static unsigned char cont_freq = 0x00; //vari�vel auxiliar para controle da frequ�ncia
    TCNT0 = cont_freq;                     //atualiza o valor da frequ�ncia
    PORTD = seq_leds;                      //atualiza o PORTD
    seq_leds <<= 1;                        //rotaciona os leds
    if(seq_leds == 0x00)
    {
	    seq_leds = 0x01;                     //reinicializa a vari�vel auxiliar
	    if(cont_freq < 250) cont_freq += 10; //aumenta a frequ�ncia vis�vel
    }
}
