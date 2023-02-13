/*
 * s13_e01_gerando_uma_senoide.c
 *
 * Created: 09/02/2023 15:00:10
 * Author : Matheus
 */ 

#include "definicoes_principais.h" //inclui a biblioteca de defini��es principais
#include "timers.h"                //inclui a biblioteca de fun��es dos m�dulos temporizador/contador


//defini��es de hardware
#define AUMENTA PC0
#define DIMINUI PC1

//vari�vel auxiliar de controle da frequ�ncia do sinal de sa�da
unsigned char periodo = 195;

//declara��o de vetor armazenado na mem�ria de programa que cont�m os valores de uma sen�ide discretizada
const char Y[] PROGMEM =
{
    127, 130, 133, 136, 139, 142, 145, 148, 151, 154, 157, 161, 164, 166, 169, 172,
    175, 178, 181, 184, 187, 189, 192, 195, 197, 200, 202, 205, 207, 210, 212, 214,
    217, 219, 221, 223, 225, 227, 229, 231, 232, 234, 236, 237, 239, 240, 242, 243,
    244, 245, 246, 247, 248, 249, 250, 251, 251, 252, 252, 253, 253, 253, 253, 253,
    253, 253, 253, 253, 253, 252, 252, 251, 251, 250, 249, 249, 248, 247, 246, 245,
    243, 242, 241, 239, 238, 236, 235, 233, 231, 230, 228, 226, 224, 222, 220, 218,
    215, 213, 211, 209, 206, 204, 201, 199, 196, 193, 191, 188, 185, 182, 180, 177,
    174, 171, 168, 165, 162, 159, 156, 153, 150, 147, 144, 141, 137, 134, 131, 128,
    125, 122, 119, 116, 112, 109, 106, 103, 100,  97,  94,  91,  88,  85,  82,  79,
     76,  73,  71,  68,  65,  62,  60,  57,  54,  52,  49,  47,  44,  42,  40,  38,
     35,  33,  31,  29,  27,  25,  23,  22,  20,  18,  17,  15,  14,  12,  11,  10,
      8,   7,   6,   5,   4,   4,   3,   2,   2,   1,   1,   0,   0,   0,   0,   0,
      0,   0,   0,   0,   0,   1,   1,   2,   2,   3,   4,   5,   6,   7,   8,   9,
     10,  11,  13,  14,  16,  17,  19,  21,  22,  24,  26,  28,  30,  32,  34,  36,
     39,  41,  43,  46,  48,  51,  53,  56,  58,  61,  64,  66,  69,  72,  75,  78,
     81,  84,  87,  89,  92,  96,  99, 102, 105, 108, 111, 114, 117, 120, 123, 126
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
            periodo =- 10;                           //aumenta a frequ�nica, logo, diminui o per�odo
        else if(!tst_bit(PINC, DIMINUI))         //se DIMINUI for pressionado...
            periodo =+ 10;                           //diminui a frequ�nica, logo, aumenta o per�odo
    }
    PORTD = pgm_read_byte(&Y[n]);                //envia o valor da sen�ide discretizada para o PORTD
    //pisca o LED na frequ�ncia do sinal de sa�da
    if(n < 127) clr_bit(PORTB, PB0);
    else        set_bit(PORTB, PB0);
}
