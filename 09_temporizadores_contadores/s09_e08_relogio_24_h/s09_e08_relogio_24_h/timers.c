/*
 * timers.c
 *
 * Created: 12/10/2022 16:44:49
 *  Author: Matheus de Oliveira Ribeiro
 */ 

#include "definicoes_principais.h"
#include "timers.h"

/*
 * Esta fun��o inicializa o Temporizador/Contador 0:
 * - _TC0_modo: seleciona o modo de opera��o
 * - _TC0_N:    seleciona o prescaler e configura as sa�das de compara��o
 * - _TC0_INT:  configura as interrup��es
 */
void Inicializa_TC0(unsigned char _TC0_modo, unsigned char _TC0_OCAB_N, unsigned char _TC0_INT)
{
	TCCR0A = 0x00;
	TCCR0B = 0x00;
	TIMSK0 &= 0xF7;
	if(_TC0_modo == 0) TCCR0B |= (_TC0_OCAB_N & 0x07);
	else if(_TC0_modo < 5)
	{		
		TCCR0A |= ((_TC0_OCAB_N & 0xF0) | (_TC0_modo & 0x03));
		TCCR0B |= (_TC0_OCAB_N & 0x07);
	}
	else
	{
		TCCR0A |= ((_TC0_OCAB_N & 0xF0) | (_TC0_modo & 0x03));
		TCCR0B |= (((_TC0_modo << 1) & 0x08) | (_TC0_OCAB_N & 0x07));
		
	}
	TIMSK0 |= ~(_TC0_INT);
}

/*
 * Esta fun��o inicializa o Temporizador/Contador 2:
 * - _TC2_modo: seleciona o modo de opera��o
 * - _TC2_N:    seleciona o prescaler e configura as sa�das de compara��o
 * - _TC2_INT:  configura as interrup��es
 */
void Inicializa_TC2(unsigned char _TC2_modo, unsigned char _TC2_OCAB_N, unsigned char _TC2_INT)
{
	TCCR2A = 0x00;
	TCCR2B = 0x00;
	ASSR = 0x00;
	TIMSK2 &= 0xF7;
	if(_TC2_modo == 0) TCCR2B |= (_TC2_OCAB_N & 0x07);
	else if(_TC2_modo < 5)
	{		
		TCCR2A |= ((_TC2_OCAB_N & 0xF0) | (_TC2_modo & 0x03));
		TCCR2B |= (_TC2_OCAB_N & 0x07);
	}
	else if(_TC2_modo < 8)
	{
		TCCR2A |= ((_TC2_OCAB_N & 0xF0) | (_TC2_modo & 0x03));
		TCCR2B |= (((_TC2_modo << 1) & 0x08) | (_TC2_OCAB_N & 0x07));
		
	}
	else
	{
	    ASSR = 1 << AS2;
	    TCCR2A |= (_TC2_OCAB_N & 0xF0);
	    TCCR2B |= (_TC2_OCAB_N & 0x07);
	}
	TIMSK2 |= ~(_TC2_INT);
}
