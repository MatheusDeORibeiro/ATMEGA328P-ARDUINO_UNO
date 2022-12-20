/*
 * s09_e10_frequencimetro.c
 *
 * Created: 09/11/2022 17:42:27
 * Author : Matheus de Oliveira Ribeiro
 *
 * 9.10 – Elaborar um programa para que o ATmega328 funcione como frequencímetro
 * digital, apresentando o valor lido em um LCD 16 × 2. Empregue um circuito
 * adequado (sugestão na fig. 9.13).
 * Sugestão: utilize um TC de 8 bits para gerar uma base de tempo de 1 s e o TC1,
 * de 16 bits, para contar os eventos externos. A frequência será dada pelo número
 * de contagem do TC de 16 bits (se houver estouro, o programa deve levar esse
 * fato em conta no cômputo da frequência).
 *
 * Gerando a base de tempo de 1 segundo (1 Hz) com um TC de 8bits:
 * t_estouro = (N * (TOP + 1)) / f_osc = 1 = (1024 * (255 + 1) * cont) / 16M
 * cont = 61,035 -> cont = 61 -> t_estouro = 0,999s ~= 1s
 */ 


#include "definicoes_principais.h" //inclui as definições principais
#include "LCD_4bits.h"             //inclui a biblioteca de funções do display LCD 16x2 com interface de 4 bits
#include "timers.h"                //inclui a biblioteca de funções dos módulos temporizadores/contadores
#include <stdio.h>                 //inclui a biblioteca de funções de entrada e saída padrão


//declaração de hardware
#define sinal_tempo PD3 //substitui sinal_tempo por PD3 na programação

//declaração de variáveis globais
volatile unsigned char cont = 0;          //variável auxiliar da geração da base de tempo
volatile unsigned int quant_estouros = 0; //variável que armazena a quantidade de estouros do TC1
volatile unsigned char borda_captura = 0; //variável que controla a borda que será capturada
volatile unsigned int tempo_final;        //variável que armazena o valor de ICR1 na segunda borda de descida
volatile long double frequencia;          //variável que armazena o valor calculado da frequenica
char buffer[16];

ISR(TIMER2_OVF_vect);  //rotina de tratamento do estouro do TCNT2
ISR(TIMER1_OVF_vect);  //rotina de tratamento do estouro do TCNT1
ISR(TIMER1_CAPT_vect); //rotina de tratamento da captura no pino ICP1(PB0) 

int main(void) //função principal
{
    DDRD = 0xFF;  //configura todo o PORTD como saída
    DDRB = 0xFE;  //configura o pino PB0(ICP1) como entrada e os pinos restantes do PORTB como saída
    DDRC = 0xFF;  //configura todo o PORTC como saída
    PORTD = 0x00; //coloca todo o PORTD em nível baixo
    PORTB = 0x00; //coloca todo o PORTB em nível baixo
    PORTC = 0x00; //coloca todo o PORTC em nível baixo

    Inicializa_LCD(LCD_1_N); //inicializa display LCD com uma linha
    Display_Cursor_ON_OFF(LCD_DSP_ON_CS_OFF); //matém a tela do display ligada e desliga o cursor
    Limpa_Display(); //limpa a tela do display
    Cursor_Home();   //posiciona o cursor na primeira coluna da primeira linha

    Inicializa_TC2(TC2_modo_normal, TC2_OC2A_normal&TC2_OC2B_normal&TC2_CLK_1024, TC2_TOIE2);
    /* modo normal, f_osc igual a 16MHz, prescaler igual a 1024, saída comparadoras
       desabilitadas, interrupção por estouro do TC2 habilitada: t_est = 16,38ms
       a cada estouro a variável global cont é incrementada, e quando ela for igual a 61,
       o estado da saída sinal_tempo terá seu estado complementado a cada 61 * 16,38m = 
       0,999s ~= 1s, gerando, por fim, um sinal com frequência de 0,5Hz */

    Inicializa_TC1(TC1_modo_normal, TC1_OC1A_normal&TC1_OC1B_normal&TC1_ICP1_FLG&TC1_CLK_1, TC1_TOIE1&TC1_ICIE1);
    /* modo normal(captura), captura na borda de descida, prescaler igual a 1, 
       interrupção por captura no pino ICP1: */

    sei(); //habilita todas as fontes de interrupção setando o bit I do registrador SREG

    while(1); //loop infinito
}

ISR(TIMER2_OVF_vect) //implementação da rotina de tratamento do estouro do TCNT2
{
    /* são necessários 61 estouros do TCNT2 para gerar uma base de tempo de 1 segundo */
    cont++;        //incrementa variável auxiliar
    if(cont == 61) //se a variável auxiliar for igual a 61...
    {
        cpl_bit(PORTD, sinal_tempo); //muda estado do pino que gerará o sinal
        cont = 0;                    //zera a variável auxiliar
    }
}

ISR(TIMER1_OVF_vect) //implementação da rotina de tratamento do estouro do TCNT1
{
    /* esta variável armazena a quantidade de estouros do TCNT1 para gerar uma base de
       tempo que será utilizada no calculo da frequênica */
    quant_estouros++; //incrementa a variável que armazena a quantidade de estouros
}

ISR(TIMER1_CAPT_vect) //implementação da rotina de tratamento da captura no pino ICP1(PB0)
{
    if(borda_captura == 0) //se a borda capturada é a primeira borda de descida...
    {
        quant_estouros = 0; //zera a quantidade de estouros
        borda_captura = 1;  //a próxima borda a ser capturada será a segunda borda de descida
        TCNT1 = 0;          //zera o registrador do TC1
    }
    else //se a borda capturada é a segunda borda de descida...
	{
		tempo_final = ICR1; //armazena o valor de ICR1 no instante da segunda interrupção por captura
	    frequencia = (tempo_final + quant_estouros * 65536); //calculo da quantidade de ciclos de máquina
	    frequencia = (long double)(F_CPU / frequencia);      //calculo da frequênica do evento em mH
	    sprintf(buffer, "Freq: %06ld,%01ldHz", (unsigned long)frequencia, (unsigned long)((frequencia - (unsigned long)frequencia) * 10));
	    //a função sprinf formata uma string com o valor calculado da frequência
	    Posiciona_Cursor(1, 1);     //posiciona o cursor na primeira linha da segunda coluna
        Escreve_S_memdados(buffer); //imprime a string formatada
	    borda_captura = 0;          //a próxima borda a ser capturada será a primeira borda de descida
    }
}
