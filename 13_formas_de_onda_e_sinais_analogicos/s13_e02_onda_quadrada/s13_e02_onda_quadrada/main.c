/*
 * s13_e02_onda_quadrada.c
 *
 * Created: 13/02/2023 16:24:48
 * Author : Mathe
 */ 

#include "definicoes_principais.h" //inclui a biblioteca de defini��es principais
#include "timers.h"                //inclui a biblioteca de fun��es dos m�dulos temporizador/contador


//defini��es de hardware
#define AUMENTA PC0
#define DIMINUI PC1

//vari�vel auxiliar de controle da frequ�ncia do sinal de sa�da
unsigned char periodo = 195;

//declara��o de vetor armazenado na mem�ria de programa que cont�m os valores de uma onda quadrada discretizada
const char onda_quadrada[] PROGMEM = 
{
    254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254,
    254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254,
    254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254,
    254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254,
    254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254,
    254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254,
    254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254,
    254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0
};

ISR(TIMER0_OVF_vect); //declara��o da rotina de tratamento do estouro do TCNT0

int main() //fun��o principal
{
    DDRD = 0xFF;   //configura todo o PORTD como sa�da
    DDRC = 0xFC;   //configura os pinos PC1:0 como entradas e os pinos restantes como sa�da
    DDRB = 0xFF;   //configura todo o PORTB como sa�da
    PORTD = 0x00;  //inicializa todo o PORTD em n�vel baixo
    PORTC = 0x03;  //habilita os resistores de pull-up das entradas e inicializa os pinos restantes em n�vel baixo
    PORTB = 0x00;  //inicializa todo o PORTB em n�vel baixo
    UCSR0B = 0x00; //configura os pinos PD1:0 como I/Os digitais

    /* frequ�ncia do sinal igual a 1Hz, assim, o per�odo de amostragem m�nimo deve ser igual a:
       f = 1 / (255 * dt) -> 1 = 1 / (255 * dt) -> dt = 3,92ms */
    Inicializa_TC0(TC0_modo_normal, TC0_OC0A_normal&TC0_OC0B_normal&TC0_CLK_1024, TC0_TOIE0);
    /* TC1 modo normal, f_osc igual a 16MHz, prescaler igual a 1024, sa�das comparadoras desabilitadas,
       interrup��o por estouro do registrador TCNT0 habilitada:
       t_estouro = dt = N * (255 - TCNT0 + 1) / f_osc
       3,92m = 1024 * (65535 - TCNT0 + 1) / 16M -> TCNT0 = 195
    */
    TCNT0 = periodo;
    sei(); //habilita todas as fontes de interrup��o setando o bit I do registrador SREG

    while(1) //loop infinito
    {    }
}

ISR(TIMER0_OVF_vect) //implementa��o da rotina de tratamento do estouro do TCNT0
{
    static unsigned char conta_tempo = 0, n = 0; //vari�veis auxiliar
    TCNT0 = periodo;                             //atualiza valor do registrador do TC0
    conta_tempo++;                               //incrementa a vari�vel auxiliar de contagem de tempo
    n++;                                         //incrementa n para selecionar o pr�ximo elemento do vetor
    if(conta_tempo == 25)                        //se passou 100ms?...
    {
        if(!tst_bit(PINC, AUMENTA))              //se AUMENTA for pressionado...
            periodo =- 10;                       //aumenta a frequ�nica, logo, diminui o per�odo
        else if(!tst_bit(PINC, DIMINUI))         //se DIMINUI for pressionado...
            periodo =+ 10;                       //diminui a frequ�nica, logo, aumenta o per�odo
    }
    PORTD = pgm_read_byte(&onda_quadrada[n]);                //envia o valor da sen�ide discretizada para o PORTD
    //pisca o LED na frequ�ncia do sinal de sa�da
    if(n < 127) clr_bit(PORTB, PB0);
    else        set_bit(PORTB, PB0);
}
