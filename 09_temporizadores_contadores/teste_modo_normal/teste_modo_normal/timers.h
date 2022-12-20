/*
 * timers.h
 *
 * Created: 12/10/2022 14:01:33
 *  Author: Matheus de Oliveira Ribeiro
 */ 


#ifndef TIMERS_H_
#define TIMERS_H_

//defini��o de macros do modo de opera��o do TC0

#define TC0_modo_normal       0x00 //modo normal com compara��o desabilitada

//defini��o de macros do prescaler

#define TC0_NO_CLOCK_SOURCE 0x00 //Temporizador/Contador desabilitado
#define TC0_CLK_1           0x01 //prescaler igual a 1
#define TC0_CLK_8           0x02 //prescaler igual a 8
#define TC0_CLK_64          0x03 //prescaler igual a 64
#define TC0_CLK_256         0x04 //prescaler igual a 256
#define TC0_CLK_1024        0x05 //prescaler igual a 1024
#define TC0_T0_FLG          0x06 //fonte externa no pino T0 na borda de descida
#define TC0_T0_RSG          0x07 //fonte externa no pino T0 na borda de subida

//defini��o de macros para a interrup��o

#define TC0_INT_OFF 0xFF       //desabilita as fontes de interrup��o do T0
#define TC0_TOIE0   0b11111110 //habilita a interrup��o por estouro do TCNT0
#define TC0_OCIE0A  0b11111101 //habilita a interrup��o por igualdade de compara��o com registrador OCR0A
#define TC0_OCIE0B  0b11111011 //habilita a interrup��o por igualdade de compara��o com registrador OCR0B

//defini��o de prot�tipos de fun��es

void Inicializa_TC0(unsigned char _TC0_modo, unsigned char _TC0_N, unsigned char _TC0_INT);


#endif /* TIMERS_H_ */