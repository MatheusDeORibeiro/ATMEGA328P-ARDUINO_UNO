/*
 * s09_e10_frequencimetro.c
 *
 * Created: 09/11/2022 17:42:27
 * Author : Matheus de Oliveira Ribeiro
 *
 * 9.10 � Elaborar um programa para que o ATmega328 funcione como frequenc�metro
 * digital, apresentando o valor lido em um LCD 16 � 2. Empregue um circuito
 * adequado (sugest�o na fig. 9.13).
 * Sugest�o: utilize um TC de 8 bits para gerar uma base de tempo de 1 s e o TC1,
 * de 16 bits, para contar os eventos externos. A frequ�ncia ser� dada pelo n�mero
 * de contagem do TC de 16 bits (se houver estouro, o programa deve levar esse
 * fato em conta no c�mputo da frequ�ncia).
 *
 * Gerando a base de tempo de 1 segundo (1 Hz) com um TC de 8bits:
 * t_estouro = (N * (TOP + 1)) / f_osc = 1 = (1024 * (255 + 1) * cont) / 16M
 * cont = 61,035 -> cont = 61 -> t_estouro = 0,999s ~= 1s
 */ 


#include "definicoes_principais.h" //inclui as defini��es principais
#include "LCD_4bits.h"             //inclui a biblioteca de fun��es do display LCD 16x2 com interface de 4 bits
#include "timers.h"                //inclui a biblioteca de fun��es dos m�dulos temporizadores/contadores
#include <stdio.h>                 //inclui a biblioteca de fun��es de entrada e sa�da padr�o


//declara��o de hardware
#define sinal_tempo PD3 //substitui sinal_tempo por PD3 na programa��o

//declara��o de vari�veis globais
volatile unsigned char cont = 0;          //vari�vel auxiliar da gera��o da base de tempo
volatile unsigned int quant_estouros = 0; //vari�vel que armazena a quantidade de estouros do TC1
volatile unsigned char borda_captura = 0; //vari�vel que controla a borda que ser� capturada
volatile unsigned int tempo_final;        //vari�vel que armazena o valor de ICR1 na segunda borda de descida
volatile long double frequencia;          //vari�vel que armazena o valor calculado da frequenica
char buffer[16];

ISR(TIMER2_OVF_vect);  //rotina de tratamento do estouro do TCNT2
ISR(TIMER1_OVF_vect);  //rotina de tratamento do estouro do TCNT1
ISR(TIMER1_CAPT_vect); //rotina de tratamento da captura no pino ICP1(PB0) 

int main(void) //fun��o principal
{
    DDRD = 0xFF;  //configura todo o PORTD como sa�da
    DDRB = 0xFE;  //configura o pino PB0(ICP1) como entrada e os pinos restantes do PORTB como sa�da
    DDRC = 0xFF;  //configura todo o PORTC como sa�da
    PORTD = 0x00; //coloca todo o PORTD em n�vel baixo
    PORTB = 0x00; //coloca todo o PORTB em n�vel baixo
    PORTC = 0x00; //coloca todo o PORTC em n�vel baixo

    Inicializa_LCD(LCD_1_N); //inicializa display LCD com uma linha
    Display_Cursor_ON_OFF(LCD_DSP_ON_CS_OFF); //mat�m a tela do display ligada e desliga o cursor
    Limpa_Display(); //limpa a tela do display
    Cursor_Home();   //posiciona o cursor na primeira coluna da primeira linha

    Inicializa_TC2(TC2_modo_normal, TC2_OC2A_normal&TC2_OC2B_normal&TC2_CLK_1024, TC2_TOIE2);
    /* modo normal, f_osc igual a 16MHz, prescaler igual a 1024, sa�da comparadoras
       desabilitadas, interrup��o por estouro do TC2 habilitada: t_est = 16,38ms
       a cada estouro a vari�vel global cont � incrementada, e quando ela for igual a 61,
       o estado da sa�da sinal_tempo ter� seu estado complementado a cada 61 * 16,38m = 
       0,999s ~= 1s, gerando, por fim, um sinal com frequ�ncia de 0,5Hz */

    Inicializa_TC1(TC1_modo_normal, TC1_OC1A_normal&TC1_OC1B_normal&TC1_ICP1_FLG&TC1_CLK_1, TC1_TOIE1&TC1_ICIE1);
    /* modo normal(captura), captura na borda de descida, prescaler igual a 1, 
       interrup��o por captura no pino ICP1: */

    sei(); //habilita todas as fontes de interrup��o setando o bit I do registrador SREG

    while(1); //loop infinito
}

ISR(TIMER2_OVF_vect) //implementa��o da rotina de tratamento do estouro do TCNT2
{
    /* s�o necess�rios 61 estouros do TCNT2 para gerar uma base de tempo de 1 segundo */
    cont++;        //incrementa vari�vel auxiliar
    if(cont == 61) //se a vari�vel auxiliar for igual a 61...
    {
        cpl_bit(PORTD, sinal_tempo); //muda estado do pino que gerar� o sinal
        cont = 0;                    //zera a vari�vel auxiliar
    }
}

ISR(TIMER1_OVF_vect) //implementa��o da rotina de tratamento do estouro do TCNT1
{
    /* esta vari�vel armazena a quantidade de estouros do TCNT1 para gerar uma base de
       tempo que ser� utilizada no calculo da frequ�nica */
    quant_estouros++; //incrementa a vari�vel que armazena a quantidade de estouros
}

ISR(TIMER1_CAPT_vect) //implementa��o da rotina de tratamento da captura no pino ICP1(PB0)
{
    if(borda_captura == 0) //se a borda capturada � a primeira borda de descida...
    {
        quant_estouros = 0; //zera a quantidade de estouros
        borda_captura = 1;  //a pr�xima borda a ser capturada ser� a segunda borda de descida
        TCNT1 = 0;          //zera o registrador do TC1
    }
    else //se a borda capturada � a segunda borda de descida...
	{
		tempo_final = ICR1; //armazena o valor de ICR1 no instante da segunda interrup��o por captura
	    frequencia = (tempo_final + quant_estouros * 65536); //calculo da quantidade de ciclos de m�quina
	    frequencia = (long double)(F_CPU / frequencia);      //calculo da frequ�nica do evento em mH
	    sprintf(buffer, "Freq: %06ld,%01ldHz", (unsigned long)frequencia, (unsigned long)((frequencia - (unsigned long)frequencia) * 10));
	    //a fun��o sprinf formata uma string com o valor calculado da frequ�ncia
	    Posiciona_Cursor(1, 1);     //posiciona o cursor na primeira linha da segunda coluna
        Escreve_S_memdados(buffer); //imprime a string formatada
	    borda_captura = 0;          //a pr�xima borda a ser capturada ser� a primeira borda de descida
    }
}
