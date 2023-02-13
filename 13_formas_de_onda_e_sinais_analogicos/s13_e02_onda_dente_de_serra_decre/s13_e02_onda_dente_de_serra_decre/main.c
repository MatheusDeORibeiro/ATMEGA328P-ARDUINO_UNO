/*
 * s13_e02_onda_dente_de_serra_decre.c
 *
 * Created: 13/02/2023 16:11:24
 * Author : Matheus
 */ 

#include "definicoes_principais.h" //inclui a biblioteca de defini��es principais
#include "timers.h"                //inclui a biblioteca de fun��es dos m�dulos temporizador/contador


//defini��es de hardware
#define AUMENTA PC0
#define DIMINUI PC1

//vari�vel auxiliar de controle da frequ�ncia do sinal de sa�da
unsigned char periodo = 195;

//declara��o de vetor armazenado na mem�ria de programa que cont�m os valores de uma onda dente de serra decrescente discretizada
const char onda_dente_de_serra_decres[] PROGMEM = 
{
    254, 253, 252, 251, 250, 249, 248, 247, 246, 245, 244, 243, 242, 241, 240, 239,
    238, 237, 236, 235, 234, 233, 232, 231, 230, 229, 228, 227, 226, 225, 224, 223,
    222, 221, 220, 219, 218, 217, 216, 215, 214, 213, 212, 211, 210, 209, 208, 207,
    206, 205, 204, 203, 202, 201, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191,
    190, 189, 188, 187, 186, 185, 184, 183, 182, 181, 180, 179, 178, 177, 176, 175,
    174, 173, 172, 171, 170, 169, 168, 167, 166, 165, 164, 163, 162, 161, 160, 159,
    158, 157, 156, 155, 154, 153, 152, 151, 150, 149, 148, 147, 146, 145, 144, 143,
    142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127,
    126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111,
    110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100,  99,  98,  97,  96,  95,
     94,  93,  92,  91,  90,  89,  88,  87,  86,  85,  84,  83,  82,  81,  80,  79,
     78,  77,  76,  75,  74,  73,  72,  71,  70,  69,  68,  67,  66,  65,  64,  63,
     62,  61,  60,  59,  58,  57,  56,  55,  54,  53,  52,  51,  50,  49,  48,  47,
     46,  45,  44,  43,  42,  41,  40,  39,  38,  37,  36,  35,  34,  33,  32,  31,
     30,  29,  28,  27,  26,  25,  24,  23,  22,  21,  20,  19,  18,  17,  16,  15,
     14,  13,  12,  11,  10,   9,   8,   7,   6,   5,   4,   3,   2,   1,   0, 254 
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
    PORTD = pgm_read_byte(&onda_dente_de_serra_decres[n]);                //envia o valor da sen�ide discretizada para o PORTD
    //pisca o LED na frequ�ncia do sinal de sa�da
    if(n < 127) clr_bit(PORTB, PB0);
    else        set_bit(PORTB, PB0);
}
