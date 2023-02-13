//================================================================================ //
// VARREDURA DE DISPLAYS DE 7 SEGMENTOS                                            //
//================================================================================ //
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#define set_bit(Y, bit_x) (Y |= (1 << bit_x))

unsigned char DISP[4]; //valores para os displays
//---------------------------------------------------------------------------------
//INTERRUPCAO - VARREDURA DOS DISPLAYS DE 7 SEGMENTOS
//---------------------------------------------------------------------------------
ISR(TIMER0_OVF_vect)
{
	static unsigned char x;
	
	PORTB &= 0xF0;     //apaga todos os displays (o controle dos displays está nos pinos (PB0:PB3)
	PORTD = DISP[x];   //coloca a informação do display no porta correspondente
	set_bit(PORTB, x); //habilita o display correspondente (PB0:PB3)
	x++;
	
	if(x == 4) x = 0;  //após 4 rotações inicializa para o primeiro display
}
//---------------------------------------------------------------------------------
int main()
{
	DDRD = 0xFF;  //dados dos displays
	DDRB = 0x0F;  //controle dos displays
	PORTB = 0xF0; //apaga displays e liga pull-ups
	
	UCSR0B = 0x00; //para usar os pinos do PORTD no Arduino
	
	//TC0 gerando interrupção
	TIMSK0 = 1 << TOIE0; //habilita a interrupção por estouro do timer 0
	TCCR0B = 1 << CS02;  //CLK/256 prescaler (CLK=16MHz), estouro a cada 4ms
	sei();               //habilita a interrupção global
	
	while(1) //qualquer escrita em DISP[x] é automaticamente apresentada nos displays
	{
		DISP[0] = 0x3F; //valor para o número zero
		DISP[1] = 0x06; //valor para o número um
		DISP[2] = 0x5B; //valor para o número dois
		DISP[3] = 0x4F; //valor para o número três
	}
}
//=================================================================================