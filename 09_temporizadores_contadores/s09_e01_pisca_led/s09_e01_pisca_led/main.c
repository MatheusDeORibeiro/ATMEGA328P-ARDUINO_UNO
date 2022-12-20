/*
 * s09_e01_pisca_led.c
 *
 * Created: 11/10/2022 18:31:59
 * Author : Matheus
 * 
 * 9.1 � Considerando o TC0 trabalhando a 8 MHz com um prescaler de 256, quanto
 * tempo leva para o TC0 gerar um estouro de contagem?
 *
 * t_estouro = ((TOP + 1) * prescaler) / f_osc =
 * - TC0 -> 8 bits -> TOP = 255
 * t_estouro = ((255 + 1) * 256) / 8M
 * t_estouro = 8,192ms.
 */ 

#include "definicoes_principais.h" //inclui as defini��es principais


//defini��o de hardware
#define LED PB5 //substitui LED por PB5 na programa��o

ISR(TIMER0_OVF_vect) //rotina de interrup��o
{
	cpl_bit(PORTB, LED); //muda o estado do LED a cada 8,192ms
}

int main(void) //fun��o principal
{
	DDRB = 0xFF;  //configura todo o PORTB como sa�da
	PORTB = 0x00; //coloca todo o PORTB em n�vel baixo

	TCCR0B = 1 << CS02;  //configura o prescaler igual a 256
	/* modo normal, prescaler igual a 256 e f_osc de 8MHz: t_estouro = 8,192ms*/
	
	TIMSK0 = 1 << TOIE0; //habilita a interrup��o do estouro do TC0
	sei();               //habilita todas as fontes de interrup��o

	while(1); //loop infinito
}
