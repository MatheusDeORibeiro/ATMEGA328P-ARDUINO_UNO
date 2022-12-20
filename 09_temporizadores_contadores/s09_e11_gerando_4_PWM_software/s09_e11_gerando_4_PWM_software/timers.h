/*
 * timers.h
 *
 * Created: 12/10/2022 14:01:33
 *  Author: Matheus de Oliveira Ribeiro
 */ 


#ifndef TIMERS_H_
#define TIMERS_H_

//definição de macros do modo de operação do TC0

#define TC0_modo_normal           0x00 //modo normal com comparação desabilitada
#define TC0_modo_pwm_fc_TOP_255   0x01 //modo pwm com fase corrigida com valor TOP igual a 255
#define TC0_modo_CTC              0x02 //modo CTC
#define TC0_modo_pwm_r_TOP_255    0x03 //modo pwm rápido com valor TOP igual a 255
#define TC0_modo_pwm_fc_TOP_OCR0A 0x05 //modo pwm com fase corrigida com valor TOP igual a OCR0A
#define TC0_modo_pwm_r_TOP_OCR0A  0x07 //modo pwm rápido com valor TOP igual a OCR0A

//definição de macros das saídas de comparação

#define TC0_OC0A_normal 0b00111111 //modo normal, CTC e pwm rápido e com fase corrigida: desconecta OC0A
#define TC0_OC0A_toggle 0b01111111 //modo normal, CTC e pwm rápido e com fase corrigida(WGM02 = 1): muda estado de OC0A na igualdade da comparação
#define TC0_OC0A_n_inv  0b10111111 //modo normal, CTC e pwm rápido e com fase corrigida: limpa OC0A na igualdade da comparação, contador crescente, modo não-invertido
#define TC0_OC0A_inv    0b11111111 //modo normal, CTC e pwm rápido e com fase corrigida: seta OC0A na igualdade da comparação, contador decrescente, modo invertodo

#define TC0_OC0B_normal 0b11001111 //modo normal, CTC e pwm rápido e com fase corrigida: desconecta OC0B
#define TC0_OC0B_toggle 0b11011111 //modo normal: muda estado  de OC0B na igualdade da comparação
#define TC0_OC0B_n_inv  0b11101111 //modo normal, CTC e pwm rápido e com fase corrigida: limpa OC0B na igualdade da comparação, contador crescente, modo não-invertido
#define TC0_OC0B_inv    0b11111111 //modo normal, CTC e pwm rápido e com fase corrigida: seta OC0B na igualdade da comparação, contador decrescente, modo invertido

//definição de macros do prescaler

#define TC0_NO_CLOCK_SOURCE 0b11111000 //Temporizador/Contador desabilitado
#define TC0_CLK_1           0b11111001 //prescaler igual a 1
#define TC0_CLK_8           0b11111010 //prescaler igual a 8
#define TC0_CLK_64          0b11111011 //prescaler igual a 64
#define TC0_CLK_256         0b11111100 //prescaler igual a 256
#define TC0_CLK_1024        0b11111101 //prescaler igual a 1024
#define TC0_T0_FLG          0b11111110 //fonte externa na borda de descida
#define TC0_T0_RSG          0b11111111 //fonte externa na borda de subida

//definição de macros para a interrupção

#define TC0_INT_OFF 0xFF       //desabilita as fontes de interrupção do T0
#define TC0_TOIE0   0b11111110 //habilita a interrupção por estouro do TCNT0
#define TC0_OCIE0A  0b11111101 //habilita a interrupção por igualdade de comparação com registrador OCR0A
#define TC0_OCIE0B  0b11111011 //habilita a interrupção por igualdade de comparação com registrador OCR0B

//definição de macros do modo de operação do TC1
#define TC1_modo_normal            0x00 //modo normal e captura
#define TC1_modo_PWM_fc_8bit       0x01 //modo PWM com fase corrigida com valor TOP igual a 0x00FF
#define TC1_modo_PWM_fc_9bit       0x02 //modo PWM com fase corrigida com valor TOP igual a 0x01FF
#define TC1_modo_PWm_fc_10bit      0x03 //modo PWM com fase corrigida com valor TOP igual a 0x03FF
#define TC1_modo_CTC_TOP_OCR1A     0x04 //modo CTC com valor TOP igual a OCR1A
#define TC1_modo_PWM_r_TOP_8bit    0x05 //modo PWM rápido com valor TOP igual a 0x00FF
#define TC1_modo_PWM_r_TOP_9bit    0x06 //modo PWM rápido com valor TOP igual a 0x01FF
#define TC1_modo_PWM_r_TOP_10bit   0x07 //modo PWM rápido com valor TOP igual a 0x03FF
#define TC1_modo_PWM_ffc_TOP_ICR1  0x08 //modo PWM com fase e frequência corrigida com valor TOP igual a ICR1
#define TC1_modo_PWM_ffc_TOP_OCR1A 0x09 //modo PWM com fase e frequência corrigida com valor TOP igual a OCR1A
#define TC1_modo_PWM_fc_TOP_ICR1   0x0A //modo PWM com fase corrigida com valor TOP igual a ICR1
#define TC1_modo_PWM_fc_TOP_OCR1A  0x0B //modo PWM com fase corrigida com valor TOP igual a ICR1A
#define TC1_modo_CTC_TOP_ICR1      0x0C //modo CTC com valor TOP igual a ICR1
#define TC1_modo_PWM_r_TOP_ICR1    0x0E //modo PWM rápido com valor top igual a ICR1
#define TC1_modo_PWM_r_TOP_OCRA1   0x0F //modo PWM rápido com valor top igual a OCRA1

//definição de macros das saída de comparação

#define TC1_OC1A_normal 0b00111111 //modo normal, CTC e pwm rápido e com fase corrigida: desconecta OC1A
#define TC1_OC1A_toggle 0b01111111 //modo normal, CTC e pwm rápido e com fase corrigida(WGM13:0 = 9, 11, 14 ou 15): muda estado de OC1A na igualdade da comparação
#define TC1_OC1A_n_inv  0b10111111 //modo normal, CTC e pwm rápido e com fase corrigida: limpa OC1A na igualdade da comparação, contador crescente, modo não-invertido
#define TC1_OC1A_inv    0b11111111 //modo normal, CTC e pwm rápido e com fase corrigida: seta OC1A na igualdade da comparação, contador decrescente, modo invertodo

#define TC1_OC1B_normal 0b11001111 //modo normal, CTC e pwm rápido e com fase corrigida: desconecta OC1B
#define TC1_OC1B_toggle 0b11011111 //modo normal: muda estado  de OC1B na igualdade da comparação
#define TC1_OC1B_n_inv  0b11101111 //modo normal, CTC e pwm rápido e com fase corrigida: limpa OC1B na igualdade da comparação, contador crescente, modo não-invertido
#define TC1_OC1B_inv    0b11111111 //modo normal, CTC e pwm rápido e com fase corrigida: seta OC1B na igualdade da comparação, contador decrescente, modo invertido

//definição de macros do prescaler

#define TC1_NO_CLOCK_SOURCE 0b11111000 //Temporizador contador desabilitado
#define TC1_CLK_1           0b11111001 //prescaler igual a 1
#define TC1_CLK_8           0b11111010 //prescaler igual a 8
#define TC1_CLK_64          0b11111011 //prescaler igual a 64
#define TC1_CLK_256         0b11111100 //prescaler igual a 256
#define TC1_CLK_1024        0b11111101 //prescaler igual a 1024
#define TC1_T1_FLG          0b11111110 //fonte de clock externa com borda de descida
#define TC1_T1_RSG          0b11111111 //fonte de clock externa com borda de subida
#define TC1_ICP1_FLG        0b10111111 //captura ocorre na borda de descida
#define TC1_ICP1_RSG        0b11111111 //captura ocorre na borda de subida

//definição de macros para a interrupção

#define TC1_INT_OFF 0xFF       //desabilita as fontes de interrupção do TC1
#define TC1_TOIE1   0b11111110 //habilita a interrupção por estouro do TCNT1
#define TC1_OCIE1A  0b11111101 //habilita a interrupção por igualdade de comparação com registrador OCR1A
#define TC1_OCIE1B  0b11111011 //habilita a interrupção por igualdade de comparação com registrador OCR1B
#define TC1_ICIE1   0b11011111 //habilita a interrupção por captura na entrada ICP1 ou nos comparadores analógicos

//definição de macros do modo de operação do TC2

#define TC2_modo_normal           0x00 //modo normal com comparação desabilitada
#define TC2_modo_pwm_fc_TOP_255   0x01 //modo pwm com fase corrigida com valor TOP igual a 255
#define TC2_modo_CTC              0x02 //modo CTC
#define TC2_modo_pwm_r_TOP_255    0x03 //modo pwm rápido com valor TOP igual a 255
#define TC2_modo_pwm_fc_TOP_OCR2A 0x05 //modo pwm com fase corrigida com valor TOP igual a OCR2A
#define TC2_modo_pwm_r_TOP_OCR2A  0x07 //modo pwm rápido com valor TOP igual a OCR2A
#define TC2_modo_assincrono       0x08 //modo assíncrono

//definição de macros das saídas de comparação

#define TC2_OC2A_normal 0b00111111 //modo normal, CTC e pwm rápido e com fase corrigida: desconecta OC0A
#define TC2_OC2A_toggle 0b01111111 //modo normal, CTC e pwm rápido e com fase corrigida(WGM02 = 1): muda estado de OC2A na igualdade da comparação
#define TC2_OC2A_n_inv  0b10111111 //modo normal, CTC e pwm rápido e com fase corrigida: limpa OC2A na igualdade da comparação, contador crescente, modo não-invertido
#define TC2_OC2A_inv    0b11111111 //modo normal, CTC e pwm rápido e com fase corrigida: seta OC2A na igualdade da comparação, contador decrescente, modo invertodo

#define TC2_OC2B_normal 0b11001111 //modo normal, CTC e pwm rápido e com fase corrigida: desconecta OC2B
#define TC2_OC2B_toggle 0b11011111 //modo normal: muda estado  de OC2B na igualdade da comparação
#define TC2_OC2B_n_inv  0b11101111 //modo normal, CTC e pwm rápido e com fase corrigida: limpa OC0B na igualdade da comparação, contador crescente, modo não-invertido
#define TC2_OC2B_inv    0b11111111 //modo normal, CTC e pwm rápido e com fase corrigida: seta OC0B na igualdade da comparação, contador decrescente, modo invertido

//definição de macros do prescaler

#define TC2_NO_CLOCK_SOURCE 0b11111000 //Temporizador/Contador desabilitado
#define TC2_CLK_1           0b11111001 //prescaler igual a 1
#define TC2_CLK_8           0b11111010 //prescaler igual a 8
#define TC2_CLK_32          0b11111011 //prescaler igual a 32
#define TC2_CLK_64          0b11111100 //prescaler igual a 64
#define TC2_CLK_128         0b11111101 //prescaler igual a 128
#define TC2_CLK_256         0b11111110 //prescaler igual a 256
#define TC2_CLK_1024        0b11111111 //prescaler igual a 1024

//definição de macros para a interrupção
   
#define TC2_INT_OFF 0xFF       //desabilita as fontes de interrupção do TC2
#define TC2_TOIE2   0b11111110 //habilita a interrupção por estouro do TCNT2
#define TC2_OCIE2A  0b11111101 //habilita a interrupção por igualdade de comparação com registrador OCR2A
#define TC2_OCIE2B  0b11111011 //habilita a interrupção por igualdade de comparação com registrador OCR2B

//definição de protótipos de funções

void Inicializa_TC0(unsigned char _TC0_modo, unsigned char _TC0_OCAB_N, unsigned char _TC0_INT);
void Inicializa_TC1(unsigned char _TC1_modo, unsigned char _TC1_OCAB_N, unsigned char _TC1_INT);
void Inicializa_TC2(unsigned char _TC2_modo, unsigned char _TC2_OCAB_N, unsigned char _TC2_INT);

//definição de macros para configurar a borda de detecção no modo captura do TC1

#define CAPTURA_FLG clr_bit(TCCR1B, ICES1)//captura ocorre na borda de descida
#define CAPTURA_RSG set_bit(TCCR1B, ICES1)//a captura ocorre na borda de subida


#endif /* TIMERS_H_ */