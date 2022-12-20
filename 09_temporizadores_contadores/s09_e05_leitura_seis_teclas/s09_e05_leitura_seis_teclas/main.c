/*
 * s09_e05_leitura_seis_teclas.c
 *
 * Created: 15/10/2022 17:49:12
 * Author : Matheus
 *
 * 9.5 – Elaborar um programa para ler 6 teclas utilizando a interrupção externa
 * INT0 do ATmega328 (ver o capítulo 6). O programa principal não deve ficar
 * responsável pela varredura do teclado (ver o capítulo 8); a interrupção do TC0
 * deve ser empregada para esse fim. O tratamento do teclado será transparente ao
 * programa principal, devendo ser feito nas interrupções. Assim, quando uma tecla
 * for pressionada será requisitado um pedido de interrupção e o LED correspondente
 * deve ser ligado. O circuito abaixo ilustra o circuito necessário.
 * (Seis teclas gerando um pedido de interrupção com o emprego de apenas uma
 * interrupção externa)
 */ 

#include "definicoes_principais.h" //inclui a biblioteca de definições principais
#include "timers.h"                //inclui a biblioteca de funções para utilizar o TC0


//definições de hardware
#define BT_1 PC0 //substitui BT_1 por PC0 na programação
#define BT_2 PC1 //substitui BT_2 por PC1 na programação
#define BT_3 PC2 //substitui BT_3 por PC2 na programação
#define BT_4 PC3 //substitui BT_4 por PC3 na programação
#define BT_5 PC4 //substitui BT_5 por PC4 na programação
#define BT_6 PC5 //substitui BT_6 por PC5 na programação
#define LED_1 PB0 //substitui LED_1 por PB0 na programação
#define LED_2 PB1 //substitui LED_2 por PB1 na programação
#define LED_3 PB2 //substitui LED_3 por PB2 na programação
#define LED_4 PB3 //substitui LED_4 por PB3 na programação
#define LED_5 PB4 //substitui LED_5 por PB4 na programação
#define LED_6 PB5 //substitui LED_6 por PB5 na programação

//declaração das rotinas de tratamento de interrupção
ISR(INT0_vect);
ISR(TIMER0_OVF_vect);

int main(void) //função principal
{
    DDRB = 0xFF; //configura todo o PORTB como saída
    DDRC = 0xFF; //configura todo o PORTC como saída
    DDRD = 0xFB; //configura PD2 como entrada e os pinos do PORTD como restantes como saída
    PORTB = 0x00; //coloca todo o PORTB em nível baixo
    PORTC = 0xFE; //coloca PC0 em nível baixo e o restante do PORTC em nível alto
    PORTD = 0x04; //habilita o resistor pull-up da entrada e coloca os pinos restantes em nível baixo

    //configurando INT0
    EICRA = 1 << ISC01; //uma borda de descida em PD2(INT0) gera uma solicitação de interrupção
    EIMSK = 1 << INT0;  //habilita a interrupção externa do pino PD2

    //configurando o TC0
    Inicializa_TC0(TC0_modo_normal, TC0_OC0A_normal&TC0_OC0B_normal&TC0_CLK_1024, TC0_TOIE0);
    /* para um t_estouro de 10ms -> TOP = 155 -> TCNT0 = 100 */
    /* t_estouro = 9,984ms ~= 10ms */
    TCNT0 = 100; //inicializa o registrador do TC0
    /* modo normal, f_osc igual a 16MHz, TOP igual a 156, prescaler igual a 1024,
       saída de comparação desabilitadas, interrupção do estouro do TC0 habilitada: */

    sei(); //seta o bit I do registrador SREG habilitando as interrupção globais

    while(1); //loop infinito
}

//implementando as rotinas de tratamento de interrupção

ISR(INT0_vect) //verifica se alguma tecla foi pressionada
{
	if(!tst_bit(PINC, BT_1))        //testa botão 1
	{
		while(tst_bit(PINC, BT_1)); //aguarda botão 1 ser liberado
		cpl_bit(PORTB, LED_1);      //muda o estado do LED 1
	}
	else if(!tst_bit(PINC, BT_2))   //testa botão 2
	{
		while(tst_bit(PINC, BT_2)); //aguarda botão 2 ser liberado
		cpl_bit(PORTB, LED_2);      //muda o estado do LED 2
	}
	else if(!tst_bit(PINC, BT_3))   //testa botão 3
	{
		while(tst_bit(PINC, BT_3)); //aguarda botão 3 ser liberado
		cpl_bit(PORTB, LED_3);      //muda o estado do LED 3
	}
	else if(!tst_bit(PINC, BT_4))   //testa botão 5
	{
		while(tst_bit(PINC, BT_4)); //aguarda botão 4 ser liberado
		cpl_bit(PORTB, LED_4);      //muda o estado do LED 4
	}
	else if(!tst_bit(PINC, BT_5))   //testa botão 5
	{
		while(tst_bit(PINC, BT_5)); //aguarda botão 5 ser liberado
		cpl_bit(PORTB, LED_5);      //muda o estado do LED 5
	}
	else if(!tst_bit(PINC, BT_6))   //testa botão 6
	{
		while(tst_bit(PINC, BT_6)); //aguarda botão 6 ser liberado
		cpl_bit(PORTB, LED_6);      //muda o estado do LED 6
	}
	_delay_ms(150);
}

ISR(TIMER0_OVF_vect) //varredura do teclado
{
    TCNT0 = 100; //reiniciando o valor do registrador do TC0
    PORTC = (PORTC << 1) + 1;                //deslocamento lógico para a esquerda
    if(PORTC == 0xBF) PORTC = (0xFE & 0x3F); //coloca o pino PC0 em nível lógico 1
}
