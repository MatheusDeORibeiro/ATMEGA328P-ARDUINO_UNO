/*
 * LCDG_128x64.c
 *
 * Created: 27/01/2023 16:09:21
 * Author : Matheus
 */ 

#include "def_principais.h"
#include "LCDG.h"
#include "dragao.h" //figura com 1024 bytes

int main(void)
{
	DDRD = 0xFF;  //pinos do PORTD como saída
	DDRC = 0xFF;  //pinos do PORTC como saída
	UCSR0B= 0x00; //para uso dos pinos do PORTD no Arduino
	
	inic_LCDG();
	escreve_todo_LCDG(dragao);
	
	while(1){}    //código principal
}
