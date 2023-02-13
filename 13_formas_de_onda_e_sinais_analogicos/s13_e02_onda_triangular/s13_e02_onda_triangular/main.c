/*
 * s13_e02_onda_triangular.c
 *
 * Created: 12/02/2023 17:56:12
 * Author : Matheus
 */ 

#include "definicoes_principais.h" //inclui a biblioteca de defini��es principais
#include "timers.h"                //inclui a biblioteca de fun��es dos m�dulos temporizador/contador


//defini��es de hardware
#define AUMENTA PC0
#define DIMINUI PC1

//vari�vel auxiliar de controle da frequ�ncia do sinal de sa�da
unsigned char periodo = 195;

//declara��o de vetor armazenado na mem�ria de programa que cont�m os valores de uma onda triangular discretizada
const char onda_triangular[] PROGMEM = 
{
     0,   1,    3,   5,   7,   9,  11,  13,  15,  17,  19,  21,  23,  25,  27,  29,
    31,  33,   35,  37,  39,  41,  43,  45,  47,  49,  51,  53,  55,  57,  59,  61,
    63,  65,   67,  69,  71,  73,  75,  77,  79,  81,  83,  85,  87,  89,  91,  93,
    95,  97,   99, 101, 103, 105, 107, 109, 111, 113, 115, 117, 119, 121, 123, 125,
    127, 129, 131, 133, 135, 137, 139, 141, 143, 145, 147, 149, 151, 153, 155, 157,
    159, 161, 163, 165, 167, 169, 171, 173, 175, 177, 179, 181, 183, 185, 187, 189,
    191, 193, 195, 197, 199, 201, 203, 205, 207, 209, 211, 213, 215, 217, 219, 221,
    223, 225, 227, 229, 231, 233, 235, 237, 239, 241, 243, 245, 247, 249, 251, 253,
    253, 251, 249, 247, 245, 243, 241, 239, 237, 235, 233, 231, 229, 227, 225, 223,
    221, 219, 217, 215, 213, 211, 209, 207, 205, 203, 201, 199, 197, 195, 193, 191,
    189, 187, 185, 183, 181, 179, 177, 175, 173, 171, 169, 167, 165, 163, 161, 159,
    157, 155, 153, 151, 149, 147, 145, 143, 141, 139, 137, 135, 133, 131, 129, 127,
    125, 123, 121, 119, 117, 115, 113, 111, 109, 107, 105, 103, 101,  99,  97,  95,
     93,  91,  89,  87,  85,  83,  81,  79,  77,  75,  73,  71,  69,  67,  65,  63,
     61,  59,  57,  55,  53,  51,  49,  47,  45,  43,  41,  39,  37,  35,  33,  31,
     29,  27,  25,  23,  21,  19,  17,  15,  13,  11,   9,   7,   5,   3,   1,   0
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
    PORTD = pgm_read_byte(&onda_triangular[n]); //envia o valor da sen�ide discretizada para o PORTD
    //pisca o LED na frequ�ncia do sinal de sa�da
    if(n < 127) clr_bit(PORTB, PB0);
    else        set_bit(PORTB, PB0);
}
