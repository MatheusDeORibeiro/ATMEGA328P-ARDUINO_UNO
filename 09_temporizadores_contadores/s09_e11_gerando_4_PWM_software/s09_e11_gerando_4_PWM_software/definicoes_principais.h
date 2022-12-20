/*
 * definicoes_principais.h
 *
 * Created: 12/09/2022 18:32:34
 *  Author: Matheus de Oliveira Ribeiro
 */ 

/*
 * Esta biblioteca cont�m a inclus�o de algumas bibliotecas e a declara��o de macros para
 * manipula��o de bits dos registradores de I/Os.
 */


#define F_CPU 16000000UL  //frequ�ncia de 16MHz do cristal oscilador

#include <avr/io.h>        //inclui biblioteca de hardware
#include <util/delay.h>    //inclui biblioteca de fun��es de delay
//#include <avr/pgmspace.h>  //inclui bibiioteca de fun��es de mem�ria de programa
#include <avr/interrupt.h> //inclui macros para interrup��es
//#include <avr/eeprom.h>    //inclui biblioteca de fun��es de escrita e leitura da mem�ria EEPROM

#ifndef DEFINICOES_PRINCIPAIS_H_
#define DEFINICOES_PRINCIPAIS_H_

//defini��o de macros para leitura e escrita de um bit de um PORT(byte)

#define set_bit(PORTY, bit_x) (PORTY |= (1 << bit_x))  //seta o bit_x do PORTY
#define clr_bit(PORTY, bit_x) (PORTY &= ~(1 << bit_x)) //limpa o bit_x do PORTY
#define cpl_bit(PORTY, bit_x) (PORTY ^= (1 << bit_x))  //complementa o bit_x do PORTY
#define tst_bit(PORTY, bit_x) (PORTY & (1 << bit_x))   //testa o estado do bit_x do PORTY

#endif /* DEFINICOES_PRINCIPAIS_H_ */