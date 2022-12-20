/*
 * timers.h
 *
 * Created: 12/10/2022 14:01:33
 *  Author: Matheus de Oliveira Ribeiro
 */ 


#ifndef TIMERS_H_
#define TIMERS_H_

//defini��o de macros do modo de opera��o do TC0

#define TC0_modo_normal           0x00 //modo normal com compara��o desabilitada
#define TC0_modo_pwm_fc_TOP_255   0x01 //modo pwm com fase corrigida com valor TOP igual a 255
#define TC0_modo_CTC              0x02 //modo CTC
#define TC0_modo_pwm_r_TOP_255    0x03 //modo pwm r�pido com valor TOP igual a 255
#define TC0_modo_pwm_fc_TOP_OCR0A 0x05 //modo pwm com fase corrigida com valor TOP igual a OCR0A
#define TC0_modo_pwm_r_TOP_OCR0A  0x07 //modo pwm r�pido com valor TOP igual a OCR0A

//defini��o de macros das sa�das de compara��o

#define TC0_OC0A_normal 0b00111111 //modo normal, CTC e pwm r�pido e com fase corrigida: desconecta OC0A
#define TC0_OC0A_toggle 0b01111111 //modo normal, CTC e pwm r�pido e com fase corrigida(WGM02 = 1): muda estado de OC0A na igualdade da compara��o
#define TC0_OC0A_n_inv  0b10111111 //modo normal, CTC e pwm r�pido e com fase corrigida: limpa OC0A na igualdade da compara��o, contador crescente, modo n�o-invertido
#define TC0_OC0A_inv    0b11111111 //modo normal, CTC e pwm r�pido e com fase corrigida: seta OC0A na igualdade da compara��o, contador decrescente, modo invertodo

#define TC0_OC0B_normal 0b11001111 //modo normal, CTC e pwm r�pido e com fase corrigida: desconecta OC0B
#define TC0_OC0B_toggle 0b11011111 //modo normal: muda estado  de OC0B na igualdade da compara��o
#define TC0_OC0B_n_inv  0b11101111 //modo normal, CTC e pwm r�pido e com fase corrigida: limpa OC0B na igualdade da compara��o, contador crescente, modo n�o-invertido
#define TC0_OC0B_inv    0b11111111 //modo normal, CTC e pwm r�pido e com fase corrigida: seta OC0B na igualdade da compara��o, contador decrescente, modo invertido

//defini��o de macros do prescaler

#define TC0_NO_CLOCK_SOURCE 0b11111000 //Temporizador/Contador desabilitado
#define TC0_CLK_1           0b11111001 //prescaler igual a 1
#define TC0_CLK_8           0b11111010 //prescaler igual a 8
#define TC0_CLK_64          0b11111011 //prescaler igual a 64
#define TC0_CLK_256         0b11111100 //prescaler igual a 256
#define TC0_CLK_1024        0b11111101 //prescaler igual a 1024
#define TC0_T0_FLG          0b11111110 //fonte externa na borda de descida
#define TC0_T0_RSG          0b11111111 //fonte externa na borda de subida

//defini��o de macros para a interrup��o

#define TC0_INT_OFF 0xFF       //desabilita as fontes de interrup��o do T0
#define TC0_TOIE0   0b11111110 //habilita a interrup��o por estouro do TCNT0
#define TC0_OCIE0A  0b11111101 //habilita a interrup��o por igualdade de compara��o com registrador OCR0A
#define TC0_OCIE0B  0b11111011 //habilita a interrup��o por igualdade de compara��o com registrador OCR0B

//defini��o de prot�tipos de fun��es

void Inicializa_TC0(unsigned char _TC0_modo, unsigned char _TC0_OCAB_N, unsigned char _TC0_INT);


#endif /* TIMERS_H_ */