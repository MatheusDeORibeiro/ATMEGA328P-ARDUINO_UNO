/*
 * s10_e01_teclado_4_oitava.c
 *
 * Created: 16/12/2022 16:44:33
 * Author : Matheus
 *
 * 10.1 – Utilizando o modo de geração de frequência do ATmega328, faça um programa
 * para gerar as notas musicais básicas de um teclado com 1 oitava (pode ser a 4ª).
 * Empregue um circuito adequado, incluindo as teclas. Na fig. 10.3, é ilustrada a
 * organização das notas musicais no teclado (para aumentar o teclado basta colocar
 * outra oitava em série com a primeira). Obs.: só pode ser tocada uma nota por vez.
 * Fig. 10.3 – Organização de uma oitava em um teclado musical.
 */ 

#include "definicoes_principais.h" //inclui a biblioteca de definições principais
#include "timers.h"                //inclui a biblioteca de funções dos temporizadores/contadores


//definições de hardware
#define som PB1 //substitui som por PB1 na programação
//define os pinos do teclado
#define coluna_1 PD0
#define coluna_2 PD1
#define coluna_3 PD2
#define coluna_4 PD3
#define linha_1  PD4
#define linha_2  PD5
#define linha_3  PD6
#define linha_4  PD7

//definições de controle
//#define o_padrao  4 //define a escala padrão
#define d_padrao 16 //define a duração padrão
#define b        63 //define o número de batidas por minuto

#define t_min (7500 / b) * 10 //define o tempo minímo de duração da fusa

volatile unsigned char conta_tempo = 0;

void toca_nota(unsigned int duracao_nota); //declaração de função para tocar uma nota
ISR(TIMER0_OVF_vect); //declaração da rotina de tratamento da interrupção do estouro do TCNT0

int main(void) //função principal
{
    DDRD = 0x0F;   //define os pinos PD7:4 como entradas e PD3:0 como saídas
    DDRB = 0xFF;   //define todo o PORTB como saída
    PORTD = 0xFF;  //habilita o resistores de pull-up das entradas e coloca as saídas em nível alto
    PORTB = 0x00;  //coloca todo o PORTB em nível baixo
    UCSR0B = 0x00; //configura os pinos PD0 e PD1 como IOs digitas

    Inicializa_TC0(TC0_modo_normal, TC0_OC0A_normal&TC0_OC0B_normal&TC0_CLK_1024, TC0_TOIE0);
    TCNT0 = 100;
    /* modo normal, f_osc igual a 16MHz, prescaler igual a 1024, saídas comparadoras desabilitadas,
       interrupção por estouro do TCNT0 habilitada: 
       t_estouro = 1024 * ((255 - 100) + 1) / 16M = 9,98ms */    
    sei(); //habilita todas as fontes de interrupção setando o bit I do registrador SREG

    while(1) //loop infinito
    { }
}

void toca_nota(unsigned int duracao_nota) //implementando a função para tocar uma nota
{
    //unsigned char o, d, n; //declaração de variáveis
    unsigned char d, n; //declaração de variáveis
    //habilita o TC1
    Inicializa_TC1(TC1_modo_PWM_r_TOP_ICR1, TC1_OC1A_n_inv&TC1_OC1B_normal&TC1_CLK_1, TC1_INT_OFF);
    /* modo PWM rápido com valor TOP igual a ICR1 e ciclo ativo igual a OCR1A, prescaler igual a 1,
       saídas comparadoras desabilitadas e interrupções desabilitadas */
    //o = o_padrao;              //carrega a escala padrão
    d = d_padrao;              //carrega a durção da nota
    OCR1A = duracao_nota >> 1; //carrega a nota
    n = 32 / d;                //calcula quantas vezes a nota será repetida
    ICR1 = duracao_nota;       //carrega a oitava
    //pausa da CPU pelo tempo de execução da nota
    for(unsigned char k = 0; k < n; k++)         //duração de uma nota com uma base de tempo de 1 fusa
        for(unsigned int t = t_min; t != 0; t--) //duração de uma fusa com uma base de tempo de 100us
            _delay_us(100);
    //desabilita o TC1
    TCCR1A = 0x00;
    TCCR1B = 0x00;
}

ISR(TIMER0_OVF_vect) //implementação da rotina de tratamento da interrupção do estouro do TCNT0
{
    conta_tempo++;        //incrementa a variável auxiliar de contagem do tempo
    TCNT0 = 100;          //reinicia o registrador do TC0
    if(conta_tempo == 10) //varredura do teclado a cada 99,8ms
    {
        static unsigned char controle_coluna = 0x01;            //variável auxiliar para leitura do teclado
        if(tst_bit(PORTD, coluna_1) && controle_coluna == 0x01) //verifica as teclas da primeira linha
        {
	        controle_coluna = 0x02;
	        clr_bit(PORTD, coluna_1);
	        set_bit(PORTD, coluna_2);
	        clr_bit(PORTD, coluna_3);
	        clr_bit(PORTD, coluna_4);
	        if(!tst_bit(PIND, linha_1))      toca_nota(15287); //Dó
	        else if(!tst_bit(PIND, linha_2)) toca_nota(10199); //Sol
	        else if(!tst_bit(PIND, linha_3)) toca_nota(12852); //Ré#
        }
        else if(tst_bit(PORTD, coluna_2) && controle_coluna == 0x02)
        {
	        controle_coluna = 0x03;
	        clr_bit(PORTD, coluna_1);
	        clr_bit(PORTD, coluna_2);
	        set_bit(PORTD, coluna_3);
	        clr_bit(PORTD, coluna_4);
	        if(!tst_bit(PIND, linha_1))      toca_nota(13618); //Ré
	        else if(!tst_bit(PIND, linha_2)) toca_nota(18181); //Lá
	        else if(!tst_bit(PIND, linha_3)) toca_nota(10807); //Fá#
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
	        if(!tst_bit(PIND, linha_1))      toca_nota(11499); //Fá
	        else if(!tst_bit(PIND, linha_2)) toca_nota(14429); //Dó#
	        else if(!tst_bit(PIND, linha_3)) toca_nota(17159); //Lá#
        }
        conta_tempo = 0; //reinicia a variável auxiliar de contagem do tempo
    }
}
