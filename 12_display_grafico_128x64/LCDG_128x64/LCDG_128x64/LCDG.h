/*
 * LCDG.h
 *
 * Created: 27/01/2023 16:13:04
 *  Author: Matheus
 */ 


#ifndef _LCDG_H
#define _LCDG_H

#include "def_principais.h"

#define LIGA_LCD 0x3F
#define DESL_LCD 0x3E
#define LIN_INIC 0xC0
#define Y_INIC   0x40
#define PAG_INIC 0xB8

#define DADOS PORTD //Px0:7 nos pinos de dados DB0:DB7
#define CS1   PC0   //ativo em alto
#define CS2   PC1   //ativo em alto
#define DI    PC2   //DI=1 dados, DI=0 instrução
#define RW    PC3   //RW=0 escrita, RW=1 leitura
#define EN    PC4   //ativo em alto
#define RST   PC5   //ativo em baixo

#define set_escrita() _delay_us(1); clr_bit(PORTC, RW)
#define set_leitura() _delay_us(1); set_bit(PORTC, RW)
#define set_dado() _delay_us(1); set_bit(PORTC, DI)
#define set_instrucao() _delay_us(1); clr_bit(PORTC, DI)
#define set_CS1() _delay_us(1); set_bit(PORTC, CS1)
#define set_CS2() _delay_us(1); set_bit(PORTC, CS2)
#define clr_CS1() _delay_us(1); clr_bit(PORTC, CS1)
#define clr_CS2() _delay_us(1); clr_bit(PORTC, CS2)
#define set_enable() _delay_us(5); set_bit(PORTC, EN)
#define clr_enable() _delay_us(5); clr_bit(PORTC, EN)
#define set_reset() _delay_us(10); set_bit(PORTC, RST)
#define clr_reset() _delay_us(10); clr_bit(PORTC, RST)

#define reset_LCDG() clr_reset(); set_reset()
#define enable_LCDG() set_enable(); clr_enable()

void inic_LCDG();
void escreve_LCDG(unsigned char dado, unsigned char coluna, unsigned char pagina);
void limpa_LCDG(unsigned char limpa);
void escreve_caractere_LCDG(unsigned char caractere, unsigned char col,unsigned char pag);
void escreve_stringFLASH_LCDG(char *c, unsigned char col, unsigned char pag);
void escreve_stringRAM_LCDG(char *c, unsigned char col, unsigned char pag);
void escreve_todo_LCDG(const unsigned char *figura);

#endif
