/*
 * timers.c
 *
 * Created: 12/10/2022 16:44:49
 *  Author: Matheus de Oliveira Ribeiro
 */ 

#include "definicoes_principais.h"
#include "timers.h"

/*
 * Esta função inicializa o Temporizador/Contador 0:
 * - _TC0_modo: seleciona o modo de operação
 * - _TC0_N:    seleciona o prescaler
 * - _TC0_INT:  configura as interrupções
 */
void Inicializa_TC0(unsigned char _TC0_modo, unsigned char _TC0_N, unsigned char _TC0_INT)
{
	TCCR0A = 0x00;
	TCCR0B = 0x00;
	TIMSK0 &= 0xF7;
	
	switch(_TC0_modo)
	{
		case 0:
		    TCCR0B |= _TC0_N;
		    break;
	}
	TIMSK0 |= ~(_TC0_INT);
}

