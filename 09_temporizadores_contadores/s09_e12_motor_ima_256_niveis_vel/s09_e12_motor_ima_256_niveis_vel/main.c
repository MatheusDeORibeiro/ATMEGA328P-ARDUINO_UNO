/*
 * s09_e12_motor_ima_256_niveis_vel.c
 *
 * Created: 18/11/2022 16:25:22
 * Author : Matheus
 *
 * 9.12 � Utilizando o circuito da fig. 9.16, elaborar um programa para controlar,
 * atrav�s de um sinal PWM, um motor DC com 256 n�veis de velocidade. O n�vel da
 * velocidade selecionado deve ser apresentado no display (valor hexadecimal) e
 * armazenado na mem�ria EEPROM para a inicializa��o do motor. O display de 7
 * segmentos deve apresentar um n�mero entre 00 e FF.
 *
 * Obs.: � aconselhado que a alimenta��o do motor seja independente do circuito de
 * controle, pois podem haver picos de corrente prejudiciais. No caso do
 * microcontrolador, ele pode ser reinicializado. Tudo vai depender das
 * caracter�sticas da fonte de alimenta��o.
 */ 

#include "definicoes_principais.h" //inclui a biblioteca de defini��es principais
#include "timers.h"                //inclui a biblioteca de fun��es do m�dulo temporizador contador


//defini��es de hardware
#define PORT_DISPLAY PORTD //define a porta em que os display de 7 segmentos ser�o conectados
//#define PINO_PWM     PB1   //define o pino de sa�da do sinal PWM
#define UP           PB2   //define o pino do bot�o que incrementar� a velocidade
#define DOWN         PB3   //define o pino do bot�o que decrementar� a velocidade
#define DISPLAY_1    PC4   //define o pino do display 1
#define DISPLAY_2    PC5   //define o pino do display 2

//declara��o de vari�veis globais para leitura e grava��o da EEPROM
unsigned int endereco_EEPROM = 0x0010; //endere�o onde a velocidade do motor ser� armazenada
volatile unsigned char velocidade;     //valor lido da mem�ria EEPROM

volatile unsigned char conta_tempo = 0;  //vari�vel auxiliar para varrer os bot�es a cada 99,8ms
volatile unsigned char atualiza_vel = 0; //vari�vel auxiliar para atualizar o ciclo ativo(OCRA0)

const unsigned char NumerosDisplays[] PROGMEM = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,  //de 0 at� 7
                                                 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71}; //de 8 at� F(16)

void decodifica_displays(unsigned char velocidade_motor); //fun��o que decodifica o valor e mostra no display
ISR(TIMER0_OVF_vect);                                     //declara��o do prot�tipo da rotina de tratemento do estouro do TCNT0

int main(void) //fun��o principal
{
    DDRD = 0xFF;   //configura todo o PORTD como sa�da
    DDRB = 0xF3;   //configura os pino PB4:3 como entradas e os pinos restantes do PORTB como sa�das
    DDRC = 0xFF;   //configura todo o PORTC como sa�da
    PORTD = 0x00;  //coloca todo o PORTD em n�vel baixo
    PORTB = 0x0C;  //habilita os resistores pull-up das entradas e coloca os pinos restantes em n�vel baixo
    PORTC = 0x00;  //coloca todo o PORTC em n�vel baixo
    UCSR0B = 0x00; //configura os pinos PD1:0 como IOs(desabilita as fun��es RX e TX)

    Inicializa_TC0(TC0_modo_normal, TC0_CLK_1024, TC0_TOIE0); //para a varredura dos bot�es
    /* modo normal, f_osc igual a 16MHz, prescaler igual a 1024, sa�das comparadoras desabilitadas
       interrup��o por estouro do TCNT0 habilitada: t_estouro igual a 16,4ms e f igual a 61Hz */
    TCNT0 = 100; //inicializando o registrador do TC0

    Inicializa_TC1(TC1_modo_PWM_fc_8bit, TC1_OC1A_n_inv&TC1_CLK_256, TC1_INT_OFF); //para gerar o sinal PWM
    /* modo PWM com fase corrigida com valor TOP igual a 255, f_osc igual a 16MHz,
       prescaler igual a 256: per�odo do sinal igual a 8,2ms e frequ�ncia igual a  122Hz, 
       sa�da comparadora A n�o inversora, interrup��o desabilitadas */

    __EEGET(velocidade, endereco_EEPROM);     //leitura da mem�ria EEPROM
    OCR1A = velocidade;                       //ciclo ativo do sinal PWM

    sei(); //habilita todas as fontes de interrup��o setando o bit I do registrador SREG    

    while(1) //loop infinito
    {
        decodifica_displays(velocidade); //exibindo o n�vel de velocidade
    }
}

void decodifica_displays(unsigned char velocidade_motor) //implementa��o da fun��o que decodifica o valor e mostra no display
{
    PORTD = pgm_read_byte(&NumerosDisplays[velocidade_motor & 0x0F]);        //carrega o dado menos significativo
    set_bit(PORTC, DISPLAY_1); //habilita o display menos significativo
    clr_bit(PORTC, DISPLAY_2); //desabilita o display meis significativo
    _delay_ms(8);              //delay para visualiza��o (60 ^ -1 / 2 = 8ms)
    PORTD = pgm_read_byte(&NumerosDisplays[(velocidade_motor & 0xF0) >> 4]); //carrega o dado mais significativo
    clr_bit(PORTC, DISPLAY_1); //desabilita o display menos significativo
    set_bit(PORTC, DISPLAY_2); //habilita o display mais significativo
    _delay_ms(8);              //delay para visualiza��o (60 ^ -1 / 2 = 8ms)
}

ISR(TIMER0_OVF_vect) //implementa��o da rotina de tratemento do estouro do TCNT1
{
    conta_tempo++;                                  //incrementa a vari�vel auxiliar
    TCNT0 = 100;                                    //reiniciando o valor do registrador do TCNT0
    if(conta_tempo == 10)                           //t_estouro = 10 * 1024 * ((255 - 100) + 1) / 16M = 99,8ms
    {
        if(!tst_bit(PINB, UP) && velocidade < 255)      //se o bot�o UP for pressionado...
        {
            velocidade++;                               //incrementa a velocidade
            atualiza_vel = 1;                           //o valor da velocidade deve ser atualizado
        }
        else if(!tst_bit(PINB, DOWN) && velocidade > 0) //se o bot�o DOWN for pressionado...
        {
            velocidade--;                               //decrementa a velocidade
            atualiza_vel = 1;                           //o valor da velocidade deve ser atualizado
        }
        if(atualiza_vel)                                //se a velocidade deve ser atualizada...
        {
            OCR1A = velocidade;                         //atualiza o ciclo ativo do sinal PWM
            __EEPUT(endereco_EEPROM, velocidade);       //escrita na mem�ria EEPROM
            atualiza_vel = 0;                           //o valor da velocidade foi atualizado
        }
        conta_tempo = 0;                            //reiniciando o valor da vari�vel auxiliar
    }
}
