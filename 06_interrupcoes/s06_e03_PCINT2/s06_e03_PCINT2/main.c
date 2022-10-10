/*
 * s06_e03_PCINT2.c
 *
 * Created: 27/09/2022 18:34:22
 * Author : Matheus
 *
 * 6.3 – Faça um programa para testar a interrupção PCINT2, usando dois botões nos
 * pinos PD6 e PD7, onde cada um deve ligar um LED nos pinos PD0 e PD1.
 */ 

#include "definicoes_principais.h" //biblioteca de definições principais

//rotina de tratamento de interrupção
ISR(PCINT2_vect);

int main(void) //função principal
{
	DDRD = 0x3F;   //configura os pinos PD7:6 como entradas e os pinos PD5:0 como saídas
	PORTD = 0xC0;  //habilita os resistores pull-up dos pinos PD7:6 e coloca os pinos restantes em nível baixo

	UCSR0B = 0x00; //configura os pinos PD1:0 como I/Os digitais para o Arduino UNO

	PCICR = 1 << PCIE2; //habilita a interrupção externa nos pinos de I/O do PCINT2(PORTD)
	PCMSK2 = (1 << PCINT23) | (1 << PCINT22); //habilita a interrupção externas nos pinos PD7:PD6
	sei(); //seta o bit I do registrador SREG habilitanto todas as interrupções

	while(1); //loop infinito
}

//implementando rotina de tratamento de interrupção
ISR(PCINT2_vect)
{
	if(!tst_bit(PIND, PD7)) //verifica se a interrupção foi gerada por PCINT23
	cpl_bit(PORTD, PD1);    //complementa o LED ligado ao pino PD1
	if(!tst_bit(PIND, PD6)) //verifica se a interrupção foi gerada por PCINT22
	cpl_bit(PORTD, PD0);    //complementa o LED ligado ao pino PD0
	_delay_ms(200);         //atraso para evitar que a interrupção seja acionada ao soltar o botão
}
