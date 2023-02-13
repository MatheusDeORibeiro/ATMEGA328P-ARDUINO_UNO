/*
 * LCD_21x8.c
 *
 * Created: 30/01/2023 14:48:50
 * Author : Matheus
 */ 

#include "def_principais.h"
#include "LCDG.h"

//teste da escrita com os caracteres ASCII
//prog_char ascii0[] = " !\"#$%&'()*+,-./01234\0";
//prog_char ascii1[] = "56789:;<=>?@ABCDEFGHI\0";
//prog_char ascii2[] = "JKLMNOPQRSTUVWXYZ[\\]^\0";
//prog_char ascii3[] = "_`abcdefghijklmnopqrs\0";
//prog_char ascii4[] = "tuvwxyz{|}~\0";
const char ascii0[] PROGMEM = " !\"#$%&'()*+,-./01234\0";
const char ascii1[] PROGMEM = "56789:;<=>?@ABCDEFGHI\0";
const char ascii2[] PROGMEM = "JKLMNOPQRSTUVWXYZ[\\]^\0";
const char ascii3[] PROGMEM = "_`abcdefghijklmnopqrs\0";
const char ascii4[] PROGMEM = "tuvwxyz{|}~\0";
	
int main(void)
{
	DDRD = 0xFF;  //pinos do PORTD como saída
	DDRC = 0xFF;  //pinos do PORTC como saída
	UCSR0B= 0x00; //para uso dos pinos do PORTD no Arduino
	
	inic_LCDG();
	
	escreve_stringFLASH_LCDG(ascii0, 0, 0);
	escreve_stringFLASH_LCDG(ascii1, 0, 1);
	escreve_stringFLASH_LCDG(ascii2, 0, 2);
	escreve_stringFLASH_LCDG(ascii3, 0, 3);
	escreve_stringFLASH_LCDG(ascii4, 0, 4);
	escreve_stringRAM_LCDG("LCD 128x64 COMO UM", 10, 6);
	escreve_stringRAM_LCDG("ALFANUMERICO 21x8", 13, 7);
	
	while(1){} //código principal
}
