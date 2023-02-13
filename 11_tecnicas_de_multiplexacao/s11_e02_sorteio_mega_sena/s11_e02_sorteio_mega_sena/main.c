/*
 * s11_e02_sorteio_mega_sena.c
 *
 * Created: 23/12/2022 16:35:52
 * Author : Matheus
 *
 * 11.2 � Empregando dois displays de 7 segmentos e um bot�o, desenvolva um
 * programa para o sorteio aleat�rio e com mesma probabilidade de ocorr�ncia dos
 * n�meros de 1 at� 60 (Mega Sena). O n�mero sorteado n�o deve voltar ao sorteio.
 */ 

#include "definicoes_principais.h" //inclui as defini��es principais
#include "timers.h"                //inclui a biblioteca de fun��es dos m�dulos temporizador/contador


//defini��es de hardware
//#define DISP_1 PB0
//#define DISP_2 PB1
#define BOTAO PB4

//declara��o de vari�veis globais
unsigned char numero = 1;
volatile unsigned char quant = 0, sorteio[6] = {0, 0, 0, 0, 0, 0};

//declara��o de constante armazenada na mem�ria FLASH
const unsigned char display[] PROGMEM =
{
    0x3F, //0
    0x06, //1
    0x5B, //2
    0x4F, //3
    0x66, //4
    0x6D, //5
    0x7D, //6
    0x07, //7
    0x7F, //8
    0x6F  //9
};

//declara��o de rotinas de tratamento de interrup��es
ISR(TIMER0_OVF_vect);
ISR(TIMER2_OVF_vect);

int main(void) //fun��o principal
{
    DDRD = 0xFF;  //configura todo o PORTD como sa�da
    PORTD = 0x40; //inicializa o PORTD em 0x40(segmento 'g' emitindo)
    DDRB = 0xEF;  //configura PB4 como entrada e os pinos restantes do PORTB como sa�das
    PORTB = 0x10; //habilita o resistor pull-up da entrada e inicializa os pinos restantes em n�vel baixo
    UCSR0B = 0x00; //configura os pinos PD1:0 como I/Os digitais
    
    Inicializa_TC0(TC0_modo_normal, TC0_OC0A_normal&TC0_OC0B_normal&TC0_CLK_1024, TC0_TOIE0);
    TCNT0 = 100;
    /* modo normal, f_osc igual a 16MHz, prescaler igual a 1024, sa�das comparadoras desabilitadas,
       t_estouro = N * (255 - TCNT0 + 1) * conta_tempo / 16M -> 100m = 1024 * (255 - TCNT0 + 1) * 10 / 16M
       TCNT0 = 100 */
    Inicializa_TC2(TC2_modo_normal, TC2_OC2A_normal&TC2_OC2B_normal&TC2_CLK_1024, TC2_TOIE2);
    TCNT2 = 126;
    /* modo normal, f_osc igual a 16MHz, prescaler igual a 1024, sa�das comparadoras desabilitadas,
       interrup��o por estouro do TCNT2 habilitada:
       t_estouro = (f ^ -1) / n_disp = (60 ^ -1) / 2 = 8,33ms -> t_estouro = 8ms
       t_estouro = N * (255 - TCNT2 + 1) / f_osc -> 8m = 1024 * (255 - TCNT2 + 1) / 16M 
       TCNT2 = 126 */
    sei(); //habilita todas as fontes de interrup��o setando o bit I do registrador SREG

    while(1) //loop infinito
    {
        while(quant <= 6)               //enquanto n�o forem sorteados os 6 n�meros...
        {
            numero++;                   //incrementa o n�mero que ser� sorteado
            if(numero > 60) numero = 1; //gera n�meros de 1 at� 60
			//_delay_ms(150);
        }
        PORTB &= 0xFC;                  //desabilita os transistores dos displays
        PORTD = 0x00;                   //carrega '0x00' no barramento de dados
        quant = 0;                      //reinicia a vari�vel que armazena a quantidade de n�meros sorteados
        for(unsigned char i = 0; i < 6; i++) //estrutura de repeti��o para limpar 'sorteio'
            sorteio[i] = 0;
    }
}

ISR(TIMER0_OVF_vect) //implementando rotina de tratamento do estouro do TCNT0 - varredura bot�o
{
    static unsigned char conta_tempo = 0; //vari�vel auxiliar de contagem de tempo
    conta_tempo++;                        //incrementa a vari�vel auxiliar
    if(conta_tempo == 10)                 //se passou 98,8ms?...
    {
        if(!tst_bit(PINB, BOTAO))         //o bot�o foi pressionado?...
        {
            unsigned char repet = 0;        //vari�vel auxiliar
            for(unsigned char i = 0; i <= quant; i++) //varre o vetor 'sorteio'
                if(sorteio[i] == numero) repet = 1;   //verifica se 'sorteio' j� cont�m 'numero'
            if(repet == 0)                //n�o ocorreu repeti��o?...
            {
                sorteio[quant] = numero;  //armazena 'numero' em 'sorteio'
                quant++;                  //sorteia o pr�ximo n�mero
            }
        }
        conta_tempo = 0;                  //reinicia a vari�vel auxiliar
    }
}

ISR(TIMER2_OVF_vect) //implementando rotina de tratamento do estouro do TCNT2 - acionamento dos displays
{
    static unsigned char disp = 0; //vari�vel auxiliar de acionamento dos displays
    TCNT2 = 126;                   //reinicia o registrador do TC2
    PORTB &= 0xFC;                 //desabilita todos os displays
    if(quant == 0)                 //algum n�mero j� foi sorteado?...
    {
        PORTD = 0x40;              //se nenhum n�mero foi sorteado, apenas o segmento 'g' emite
    }
    else                           //se algum n�mero ja foi sorteado...
    {
	    if(disp == 0) PORTD = pgm_read_byte(&display[(sorteio[quant - 1] % 10)]); //exibe a unidade
        else          PORTD = pgm_read_byte(&display[(sorteio[quant - 1] / 10)]); //exibe a dezena
    }
    set_bit(PORTB, disp);          //aciona o display 'disp'
    disp++;                        //o pr�ximo display que ser� acionado
    if(disp == 2) disp = 0;        //volta para o primeiro display
}
