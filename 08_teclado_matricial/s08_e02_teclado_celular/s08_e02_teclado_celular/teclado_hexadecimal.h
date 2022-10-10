/*
 * teclado_hexadecimal.h
 *
 * Created: 30/09/2022 15:21:15
 * Author: Matheus de Oliveira Ribeiro
 */ 


#ifndef TECLADO_MATRICIAL_H_
#define TECLADO_MATRICIAL_H_

//definição de macros

#define PORT_TECLADO PORTD //define o barramento do teclado
#define DIR_TECLADO  DDRD  //define a direção do barramento do teclado
#define LE_TECLADO   PIND  //define a leitura do barramento do teclado
#define _L1          PD4   //define o pino da linha 1
#define _L2          PD5   //define o pino da linha 2
#define _L3          PD6   //define o pino da linha 3
#define _L4          PD7   //define o pino da linha 4
#define _C1          PD0   //define o pino da coluna 1
#define _C2          PD1   //define o pino da coluna 2
#define _C3          PD2   //define o pino da coluna 3
#define _C4          PD3   //define o pino da coluna 4

//protótipo de função

unsigned char Leitura_Teclado(void);

#endif /* TECLADO_MATRICIAL_H_ */