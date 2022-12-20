#define F_CPU 16000000UL
#include <avr/io.h>

int main(void)
{
	DDRB = 0b00000110;  //pinos OC1B e OC1A (PB2 e PB1) como saída
	PORTB = 0b11111001; //zera saídas e habilita pull-ups nos pinos não utilizados
	                    //MODO CTC - TOP = ICR1
	TCCR1A = 0b01010000; /*habilita OC1A e OC1B para trocar de estado na igualdade de
	                       comparação*/
	TCCR1B = 0b00011011; //liga TC1 com prescaler = 64.
	ICR1 = 10000;        //valor máximo de contagem
	
	while(1)
	{} //o programa principal vai aqui
}
