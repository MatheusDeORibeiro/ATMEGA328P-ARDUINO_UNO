#define F_CPU 16000000UL
#include <avr/io.h>

int main(void)
{
	DDRD = 0b01100000;  //pinos OC0B e OC0A (PD5 e PD6) como sa�da
	PORTD = 0b10011111; //zera sa�das e habilita pull-ups nos pinos n�o utilizados
	
	//MODO CTC
	TCCR0A = 0b01010010; /*habilita OC0A e OC0B para trocar de estado na igualdade de
	                       compara��o*/
	TCCR0B = 0b00000001; //liga TC0 com prescaler = 1.
	
	OCR0A = 200; //m�ximo valor de contagem
	OCR0B = 100; //deslocamento de OC0B em rela��o a OC0A
	
	while(1)
	{
		//O programa principal vai aqui
	}
}
