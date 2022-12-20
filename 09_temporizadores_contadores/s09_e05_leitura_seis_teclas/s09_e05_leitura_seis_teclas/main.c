/*
 * s09_e05_leitura_seis_teclas.c
 *
 * Created: 15/10/2022 17:49:12
 * Author : Matheus
 *
 * 9.5 � Elaborar um programa para ler 6 teclas utilizando a interrup��o externa
 * INT0 do ATmega328 (ver o cap�tulo 6). O programa principal n�o deve ficar
 * respons�vel pela varredura do teclado (ver o cap�tulo 8); a interrup��o do TC0
 * deve ser empregada para esse fim. O tratamento do teclado ser� transparente ao
 * programa principal, devendo ser feito nas interrup��es. Assim, quando uma tecla
 * for pressionada ser� requisitado um pedido de interrup��o e o LED correspondente
 * deve ser ligado. O circuito abaixo ilustra o circuito necess�rio.
 * (Seis teclas gerando um pedido de interrup��o com o emprego de apenas uma
 * interrup��o externa)
 */ 

#include "definicoes_principais.h" //inclui a biblioteca de defini��es principais
#include "timers.h"                //inclui a biblioteca de fun��es para utilizar o TC0


//defini��es de hardware
#define BT_1 PC0 //substitui BT_1 por PC0 na programa��o
#define BT_2 PC1 //substitui BT_2 por PC1 na programa��o
#define BT_3 PC2 //substitui BT_3 por PC2 na programa��o
#define BT_4 PC3 //substitui BT_4 por PC3 na programa��o
#define BT_5 PC4 //substitui BT_5 por PC4 na programa��o
#define BT_6 PC5 //substitui BT_6 por PC5 na programa��o
#define LED_1 PB0 //substitui LED_1 por PB0 na programa��o
#define LED_2 PB1 //substitui LED_2 por PB1 na programa��o
#define LED_3 PB2 //substitui LED_3 por PB2 na programa��o
#define LED_4 PB3 //substitui LED_4 por PB3 na programa��o
#define LED_5 PB4 //substitui LED_5 por PB4 na programa��o
#define LED_6 PB5 //substitui LED_6 por PB5 na programa��o

//declara��o das rotinas de tratamento de interrup��o
ISR(INT0_vect);
ISR(TIMER0_OVF_vect);

int main(void) //fun��o principal
{
    DDRB = 0xFF; //configura todo o PORTB como sa�da
    DDRC = 0xFF; //configura todo o PORTC como sa�da
    DDRD = 0xFB; //configura PD2 como entrada e os pinos do PORTD como restantes como sa�da
    PORTB = 0x00; //coloca todo o PORTB em n�vel baixo
    PORTC = 0xFE; //coloca PC0 em n�vel baixo e o restante do PORTC em n�vel alto
    PORTD = 0x04; //habilita o resistor pull-up da entrada e coloca os pinos restantes em n�vel baixo

    //configurando INT0
    EICRA = 1 << ISC01; //uma borda de descida em PD2(INT0) gera uma solicita��o de interrup��o
    EIMSK = 1 << INT0;  //habilita a interrup��o externa do pino PD2

    //configurando o TC0
    Inicializa_TC0(TC0_modo_normal, TC0_OC0A_normal&TC0_OC0B_normal&TC0_CLK_1024, TC0_TOIE0);
    /* para um t_estouro de 10ms -> TOP = 155 -> TCNT0 = 100 */
    /* t_estouro = 9,984ms ~= 10ms */
    TCNT0 = 100; //inicializa o registrador do TC0
    /* modo normal, f_osc igual a 16MHz, TOP igual a 156, prescaler igual a 1024,
       sa�da de compara��o desabilitadas, interrup��o do estouro do TC0 habilitada: */

    sei(); //seta o bit I do registrador SREG habilitando as interrup��o globais

    while(1); //loop infinito
}

//implementando as rotinas de tratamento de interrup��o

ISR(INT0_vect) //verifica se alguma tecla foi pressionada
{
	if(!tst_bit(PINC, BT_1))        //testa bot�o 1
	{
		while(tst_bit(PINC, BT_1)); //aguarda bot�o 1 ser liberado
		cpl_bit(PORTB, LED_1);      //muda o estado do LED 1
	}
	else if(!tst_bit(PINC, BT_2))   //testa bot�o 2
	{
		while(tst_bit(PINC, BT_2)); //aguarda bot�o 2 ser liberado
		cpl_bit(PORTB, LED_2);      //muda o estado do LED 2
	}
	else if(!tst_bit(PINC, BT_3))   //testa bot�o 3
	{
		while(tst_bit(PINC, BT_3)); //aguarda bot�o 3 ser liberado
		cpl_bit(PORTB, LED_3);      //muda o estado do LED 3
	}
	else if(!tst_bit(PINC, BT_4))   //testa bot�o 5
	{
		while(tst_bit(PINC, BT_4)); //aguarda bot�o 4 ser liberado
		cpl_bit(PORTB, LED_4);      //muda o estado do LED 4
	}
	else if(!tst_bit(PINC, BT_5))   //testa bot�o 5
	{
		while(tst_bit(PINC, BT_5)); //aguarda bot�o 5 ser liberado
		cpl_bit(PORTB, LED_5);      //muda o estado do LED 5
	}
	else if(!tst_bit(PINC, BT_6))   //testa bot�o 6
	{
		while(tst_bit(PINC, BT_6)); //aguarda bot�o 6 ser liberado
		cpl_bit(PORTB, LED_6);      //muda o estado do LED 6
	}
	_delay_ms(150);
}

ISR(TIMER0_OVF_vect) //varredura do teclado
{
    TCNT0 = 100; //reiniciando o valor do registrador do TC0
    PORTC = (PORTC << 1) + 1;                //deslocamento l�gico para a esquerda
    if(PORTC == 0xBF) PORTC = (0xFE & 0x3F); //coloca o pino PC0 em n�vel l�gico 1
}
