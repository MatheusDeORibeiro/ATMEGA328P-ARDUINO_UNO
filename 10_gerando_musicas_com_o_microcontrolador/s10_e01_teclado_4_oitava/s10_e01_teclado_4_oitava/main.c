/*
 * s10_e01_teclado_4_oitava.c
 *
 * Created: 16/12/2022 16:44:33
 * Author : Matheus
 *
 * 10.1 � Utilizando o modo de gera��o de frequ�ncia do ATmega328, fa�a um programa
 * para gerar as notas musicais b�sicas de um teclado com 1 oitava (pode ser a 4�).
 * Empregue um circuito adequado, incluindo as teclas. Na fig. 10.3, � ilustrada a
 * organiza��o das notas musicais no teclado (para aumentar o teclado basta colocar
 * outra oitava em s�rie com a primeira). Obs.: s� pode ser tocada uma nota por vez.
 * Fig. 10.3 � Organiza��o de uma oitava em um teclado musical.
 */ 

#include "definicoes_principais.h" //inclui a biblioteca de defini��es principais
#include "timers.h"                //inclui a biblioteca de fun��es dos temporizadores/contadores


//defini��es de hardware
#define som PB1 //substitui som por PB1 na programa��o
//define os pinos do teclado
#define coluna_1 PD0
#define coluna_2 PD1
#define coluna_3 PD2
#define coluna_4 PD3
#define linha_1  PD4
#define linha_2  PD5
#define linha_3  PD6
#define linha_4  PD7

//defini��es de controle
//#define o_padrao  4 //define a escala padr�o
#define d_padrao 16 //define a dura��o padr�o
#define b        63 //define o n�mero de batidas por minuto

#define t_min (7500 / b) * 10 //define o tempo min�mo de dura��o da fusa

volatile unsigned char conta_tempo = 0;

void toca_nota(unsigned int duracao_nota); //declara��o de fun��o para tocar uma nota
ISR(TIMER0_OVF_vect); //declara��o da rotina de tratamento da interrup��o do estouro do TCNT0

int main(void) //fun��o principal
{
    DDRD = 0x0F;   //define os pinos PD7:4 como entradas e PD3:0 como sa�das
    DDRB = 0xFF;   //define todo o PORTB como sa�da
    PORTD = 0xFF;  //habilita o resistores de pull-up das entradas e coloca as sa�das em n�vel alto
    PORTB = 0x00;  //coloca todo o PORTB em n�vel baixo
    UCSR0B = 0x00; //configura os pinos PD0 e PD1 como IOs digitas

    Inicializa_TC0(TC0_modo_normal, TC0_OC0A_normal&TC0_OC0B_normal&TC0_CLK_1024, TC0_TOIE0);
    TCNT0 = 100;
    /* modo normal, f_osc igual a 16MHz, prescaler igual a 1024, sa�das comparadoras desabilitadas,
       interrup��o por estouro do TCNT0 habilitada: 
       t_estouro = 1024 * ((255 - 100) + 1) / 16M = 9,98ms */    
    sei(); //habilita todas as fontes de interrup��o setando o bit I do registrador SREG

    while(1) //loop infinito
    { }
}

void toca_nota(unsigned int duracao_nota) //implementando a fun��o para tocar uma nota
{
    //unsigned char o, d, n; //declara��o de vari�veis
    unsigned char d, n; //declara��o de vari�veis
    //habilita o TC1
    Inicializa_TC1(TC1_modo_PWM_r_TOP_ICR1, TC1_OC1A_n_inv&TC1_OC1B_normal&TC1_CLK_1, TC1_INT_OFF);
    /* modo PWM r�pido com valor TOP igual a ICR1 e ciclo ativo igual a OCR1A, prescaler igual a 1,
       sa�das comparadoras desabilitadas e interrup��es desabilitadas */
    //o = o_padrao;              //carrega a escala padr�o
    d = d_padrao;              //carrega a dur��o da nota
    OCR1A = duracao_nota >> 1; //carrega a nota
    n = 32 / d;                //calcula quantas vezes a nota ser� repetida
    ICR1 = duracao_nota;       //carrega a oitava
    //pausa da CPU pelo tempo de execu��o da nota
    for(unsigned char k = 0; k < n; k++)         //dura��o de uma nota com uma base de tempo de 1 fusa
        for(unsigned int t = t_min; t != 0; t--) //dura��o de uma fusa com uma base de tempo de 100us
            _delay_us(100);
    //desabilita o TC1
    TCCR1A = 0x00;
    TCCR1B = 0x00;
}

ISR(TIMER0_OVF_vect) //implementa��o da rotina de tratamento da interrup��o do estouro do TCNT0
{
    conta_tempo++;        //incrementa a vari�vel auxiliar de contagem do tempo
    TCNT0 = 100;          //reinicia o registrador do TC0
    if(conta_tempo == 10) //varredura do teclado a cada 99,8ms
    {
        static unsigned char controle_coluna = 0x01;            //vari�vel auxiliar para leitura do teclado
        if(tst_bit(PORTD, coluna_1) && controle_coluna == 0x01) //verifica as teclas da primeira linha
        {
	        controle_coluna = 0x02;
	        clr_bit(PORTD, coluna_1);
	        set_bit(PORTD, coluna_2);
	        clr_bit(PORTD, coluna_3);
	        clr_bit(PORTD, coluna_4);
	        if(!tst_bit(PIND, linha_1))      toca_nota(15287); //D�
	        else if(!tst_bit(PIND, linha_2)) toca_nota(10199); //Sol
	        else if(!tst_bit(PIND, linha_3)) toca_nota(12852); //R�#
        }
        else if(tst_bit(PORTD, coluna_2) && controle_coluna == 0x02)
        {
	        controle_coluna = 0x03;
	        clr_bit(PORTD, coluna_1);
	        clr_bit(PORTD, coluna_2);
	        set_bit(PORTD, coluna_3);
	        clr_bit(PORTD, coluna_4);
	        if(!tst_bit(PIND, linha_1))      toca_nota(13618); //R�
	        else if(!tst_bit(PIND, linha_2)) toca_nota(18181); //L�
	        else if(!tst_bit(PIND, linha_3)) toca_nota(10807); //F�#
        }
        else if(tst_bit(PORTD, coluna_3) && controle_coluna == 0x03)
        {
	        controle_coluna = 0x04;
	        clr_bit(PORTD, coluna_1);
	        clr_bit(PORTD, coluna_2);
	        clr_bit(PORTD, coluna_3);
	        set_bit(PORTD, coluna_4);
	        if(!tst_bit(PIND, linha_1))      toca_nota(12131); //Mi
	        else if(!tst_bit(PIND, linha_2)) toca_nota(16197); //Si
	        else if(!tst_bit(PIND, linha_3)) toca_nota(9627);  //Sol#
        }
        else if(tst_bit(PORTD, coluna_4) && controle_coluna == 0x04)
        {
	        controle_coluna = 0x01;
	        set_bit(PORTD, coluna_1);
	        clr_bit(PORTD, coluna_2);
	        clr_bit(PORTD, coluna_3);
	        clr_bit(PORTD, coluna_4);
	        if(!tst_bit(PIND, linha_1))      toca_nota(11499); //F�
	        else if(!tst_bit(PIND, linha_2)) toca_nota(14429); //D�#
	        else if(!tst_bit(PIND, linha_3)) toca_nota(17159); //L�#
        }
        conta_tempo = 0; //reinicia a vari�vel auxiliar de contagem do tempo
    }
}
