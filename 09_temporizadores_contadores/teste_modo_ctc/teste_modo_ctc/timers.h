/*
 * timers.h
 *
 * Created: 12/10/2022 14:01:33
 *  Author: Matheus de Oliveira Ribeiro
 */ 


#ifndef TIMERS_H_
#define TIMERS_H_

//definição de macros do modo de operação do TC0

#define TC0_modo_normal 0x00 //modo normal com comparação desabilitada
#define TC0_modo_CTC    0x02 //modo CTC

//definição de macros das saídas de comparação

#define TC0_OC0A_normal 0b00111111 //modo normal e CTC: desconecta OC0A
#define TC0_OC0A_toggle 0b01111111 //modo normal e CTC(WGM02 = 1): muda estado de OC0A na igualdade da comparação
#define TC0_OC0A_n_inv  0b10111111 //modo normal e CTC: limpa OC0A na igualdade da comparação
#define TC0_OC0A_inv    0b11111111 //modo normal e CTC: seta OC0A na igualdade da comparação

#define TC0_OC0B_normal 0b11001111 //modo normal e CTC: desconecta OC0B
#define TC0_OC0B_toggle 0b11011111 //modo normal: muda estado  de OC0B na igualdade da comparação
#define TC0_OC0B_n_inv  0b11101111 //modo normal e CTC: limpa OC0B na igualdade da comparação, modo não-invertido
#define TC0_OC0B_inv    0b11111111 //modo normal e CTC: seta OC0B na igualdade da comparação, modo invertido

//definição de macros do prescaler

#define TC0_NO_CLOCK_SOURCE 0b11111000 //Temporizador/Contador desabilitado
#define TC0_CLK_1           0b11111001 //prescaler igual a 1
#define TC0_CLK_8           0b11111010 //prescaler igual a 8
#define TC0_CLK_64          0b11111011 //prescaler igual a 64
#define TC0_CLK_256         0b11111100 //prescaler igual a 256
#define TC0_CLK_1024        0b11111101 //prescaler igual a 1024
#define TC0_T0_FLG          0b11111110 //fonte externa no pino T0 na borda de descida
#define TC0_T0_RSG          0b11111111 //fonte externa no pino T0 na borda de subida

//definição de macros para a interrupção

#define TC0_INT_OFF 0x00 //desabilita as fontes de interrupção do TC0
#define TC0_TOIE0   0x01 //habilita a interrupção por estouro do TC0
#define TC0_OCIE0A  0x02 //habilita a interrupção por comparação da saída A
#define TC0_OCIE0B  0x04 //habilita a interrupção por comparação da saída B

//definição de protótipos de funções

void Inicializa_TC0(unsigned char _TC0_modo, unsigned char _TC0_OCAB_N, unsigned char _TC0_INT);


#endif /* TIMERS_H_ */