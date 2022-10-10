//=============================================================================== //
// Cada vez que um bot�o � pressionado o LED correspondente troca de estado //
//=============================================================================== //

#include "definicoes_principais.h" //inclui arquivo com as defini��es principais

//Defini��es de macros para o trabalho com bits
#define LED0 PD5
#define LED1 PD6
#define LED2 PD7

ISR(PCINT1_vect);

int main()
{
	DDRC = 0x00;       //PORTC como entrada, 3 bot�es
	PORTC = 0xFF;      //habilita pull-ups
	DDRD = 0b11100000; //pinos PD5:7 do PORTC como sa�da (leds)
	PORTD = 0x1F;      //apaga leds e habilita pull-ups dos pinos n�o utilizados
	
	PCICR = 1 << PCIE1; //habilita interrup��o por qualquer mudan�a de sinal no PORTC
	PCMSK1 = (1 << PCINT10) | (1 << PCINT9) | (1 << PCINT8); /*habilita os pinos PCINT8:10 para
	                                                           gerar interrup��o*/
	sei(); //habilita as interrup��es
	
	while(1){}
}
//----------------------------------------------------------------------------------
ISR(PCINT1_vect)
{
	//quando houver mais de um pino que possa gerar a interrup��o � necess�rio testar qual foi
	if(!tst_bit(PINC, PC0))
	    cpl_bit(PORTD, LED0);
	else if(!tst_bit(PINC, PC1))
	    cpl_bit(PORTD, LED1);
	else if(!tst_bit(PINC, PC2))
	    cpl_bit(PORTD, LED2);
	_delay_ms(200);
}
//===================================================================================