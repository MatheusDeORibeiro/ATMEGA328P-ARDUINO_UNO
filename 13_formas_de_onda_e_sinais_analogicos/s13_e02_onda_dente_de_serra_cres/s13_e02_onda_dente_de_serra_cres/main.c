/*
 * s13_e02_onda_dente_de_serra_cres.c
 *
 * Created: 13/02/2023 15:53:15
 * Author : Matheus
 */ 

#include "definicoes_principais.h" //inclui a biblioteca de defini��es principais
#include "timers.h"                //inclui a biblioteca de fun��es dos m�dulos temporizador/contador


//defini��es de hardware
#define AUMENTA PC0
#define DIMINUI PC1

//vari�vel auxiliar de controle da frequ�ncia do sinal de sa�da
unsigned char periodo = 195;

//declara��o de vetor armazenado na mem�ria de programa que cont�m os valores de uma onda dente de serra crescente discretizada
const char onda_dente_de_serra_cres[] PROGMEM = 
{
      0,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
     15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,
     31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,
     47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,
     63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,
     79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,
     95,  96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
    111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126,
    127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142,
    143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158,
    159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174,
    175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190,
    191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206,
    207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222,
    223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238,
    239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253,   0
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
    PORTD = pgm_read_byte(&onda_dente_de_serra_cres[n]);                //envia o valor da sen�ide discretizada para o PORTD
    //pisca o LED na frequ�ncia do sinal de sa�da
    if(n < 127) clr_bit(PORTB, PB0);
    else        set_bit(PORTB, PB0);
}
