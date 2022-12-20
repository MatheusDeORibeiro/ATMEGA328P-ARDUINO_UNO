/*
 * s09_e12_motor_ima_256_niveis_vel.c
 *
 * Created: 18/11/2022 16:25:22
 * Author : Matheus
 *
 * 9.12 – Utilizando o circuito da fig. 9.16, elaborar um programa para controlar,
 * através de um sinal PWM, um motor DC com 256 níveis de velocidade. O nível da
 * velocidade selecionado deve ser apresentado no display (valor hexadecimal) e
 * armazenado na memória EEPROM para a inicialização do motor. O display de 7
 * segmentos deve apresentar um número entre 00 e FF.
 *
 * Obs.: é aconselhado que a alimentação do motor seja independente do circuito de
 * controle, pois podem haver picos de corrente prejudiciais. No caso do
 * microcontrolador, ele pode ser reinicializado. Tudo vai depender das
 * características da fonte de alimentação.
 */ 

#include "definicoes_principais.h" //inclui a biblioteca de definições principais
#include "timers.h"                //inclui a biblioteca de funções do módulo temporizador contador


//definições de hardware
#define PORT_DISPLAY PORTD //define a porta em que os display de 7 segmentos serão conectados
//#define PINO_PWM     PB1   //define o pino de saída do sinal PWM
#define UP           PB2   //define o pino do botão que incrementará a velocidade
#define DOWN         PB3   //define o pino do botão que decrementará a velocidade
#define DISPLAY_1    PC4   //define o pino do display 1
#define DISPLAY_2    PC5   //define o pino do display 2

//declaração de variáveis globais para leitura e gravação da EEPROM
unsigned int endereco_EEPROM = 0x0010; //endereço onde a velocidade do motor será armazenada
volatile unsigned char velocidade;     //valor lido da memória EEPROM

volatile unsigned char conta_tempo = 0;  //variável auxiliar para varrer os botões a cada 99,8ms
volatile unsigned char atualiza_vel = 0; //variável auxiliar para atualizar o ciclo ativo(OCRA0)

const unsigned char NumerosDisplays[] PROGMEM = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,  //de 0 até 7
                                                 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71}; //de 8 até F(16)

void decodifica_displays(unsigned char velocidade_motor); //função que decodifica o valor e mostra no display
ISR(TIMER0_OVF_vect);                                     //declaração do protótipo da rotina de tratemento do estouro do TCNT0

int main(void) //função principal
{
    DDRD = 0xFF;   //configura todo o PORTD como saída
    DDRB = 0xF3;   //configura os pino PB4:3 como entradas e os pinos restantes do PORTB como saídas
    DDRC = 0xFF;   //configura todo o PORTC como saída
    PORTD = 0x00;  //coloca todo o PORTD em nível baixo
    PORTB = 0x0C;  //habilita os resistores pull-up das entradas e coloca os pinos restantes em nível baixo
    PORTC = 0x00;  //coloca todo o PORTC em nível baixo
    UCSR0B = 0x00; //configura os pinos PD1:0 como IOs(desabilita as funções RX e TX)

    Inicializa_TC0(TC0_modo_normal, TC0_CLK_1024, TC0_TOIE0); //para a varredura dos botões
    /* modo normal, f_osc igual a 16MHz, prescaler igual a 1024, saídas comparadoras desabilitadas
       interrupção por estouro do TCNT0 habilitada: t_estouro igual a 16,4ms e f igual a 61Hz */
    TCNT0 = 100; //inicializando o registrador do TC0

    Inicializa_TC1(TC1_modo_PWM_fc_8bit, TC1_OC1A_n_inv&TC1_CLK_256, TC1_INT_OFF); //para gerar o sinal PWM
    /* modo PWM com fase corrigida com valor TOP igual a 255, f_osc igual a 16MHz,
       prescaler igual a 256: período do sinal igual a 8,2ms e frequência igual a  122Hz, 
       saída comparadora A não inversora, interrupção desabilitadas */

    __EEGET(velocidade, endereco_EEPROM);     //leitura da memória EEPROM
    OCR1A = velocidade;                       //ciclo ativo do sinal PWM

    sei(); //habilita todas as fontes de interrupção setando o bit I do registrador SREG    

    while(1) //loop infinito
    {
        decodifica_displays(velocidade); //exibindo o nível de velocidade
    }
}

void decodifica_displays(unsigned char velocidade_motor) //implementação da função que decodifica o valor e mostra no display
{
    PORTD = pgm_read_byte(&NumerosDisplays[velocidade_motor & 0x0F]);        //carrega o dado menos significativo
    set_bit(PORTC, DISPLAY_1); //habilita o display menos significativo
    clr_bit(PORTC, DISPLAY_2); //desabilita o display meis significativo
    _delay_ms(8);              //delay para visualização (60 ^ -1 / 2 = 8ms)
    PORTD = pgm_read_byte(&NumerosDisplays[(velocidade_motor & 0xF0) >> 4]); //carrega o dado mais significativo
    clr_bit(PORTC, DISPLAY_1); //desabilita o display menos significativo
    set_bit(PORTC, DISPLAY_2); //habilita o display mais significativo
    _delay_ms(8);              //delay para visualização (60 ^ -1 / 2 = 8ms)
}

ISR(TIMER0_OVF_vect) //implementação da rotina de tratemento do estouro do TCNT1
{
    conta_tempo++;                                  //incrementa a variável auxiliar
    TCNT0 = 100;                                    //reiniciando o valor do registrador do TCNT0
    if(conta_tempo == 10)                           //t_estouro = 10 * 1024 * ((255 - 100) + 1) / 16M = 99,8ms
    {
        if(!tst_bit(PINB, UP) && velocidade < 255)      //se o botão UP for pressionado...
        {
            velocidade++;                               //incrementa a velocidade
            atualiza_vel = 1;                           //o valor da velocidade deve ser atualizado
        }
        else if(!tst_bit(PINB, DOWN) && velocidade > 0) //se o botão DOWN for pressionado...
        {
            velocidade--;                               //decrementa a velocidade
            atualiza_vel = 1;                           //o valor da velocidade deve ser atualizado
        }
        if(atualiza_vel)                                //se a velocidade deve ser atualizada...
        {
            OCR1A = velocidade;                         //atualiza o ciclo ativo do sinal PWM
            __EEPUT(endereco_EEPROM, velocidade);       //escrita na memória EEPROM
            atualiza_vel = 0;                           //o valor da velocidade foi atualizado
        }
        conta_tempo = 0;                            //reiniciando o valor da variável auxiliar
    }
}
