//================================================================================ //
// INTERRUP��O INT1 INTERROMPENDO A INT0 //
//================================================================================ //
#include "definicoes_principais.h"

//Defini��es de macros para o trabalho com bits
#define LED1 PD6
#define LED2 PD7

ISR(INT0_vect);
ISR(INT1_vect);
//----------------------------------------------------------------------------------
int main()
{
	DDRD = 0b11000000;  //configurando os pinos de entrada e sa�da
	PORTD = 0b00111111; //desligando leds e habilitando pull-ups
	
	UCSR0B = 0x00;      //desabilitando RX e TX para trabalho com os pinos do Arduino
	
	EICRA = 0x00;       //interrup��es externas INT0 e INT1 no n�vel zero.
	EIMSK = (1 << INT1) | (1 << INT0); //habilita as duas interrup��es
	sei(); //habilita interrup��es globais, ativando o bit I do SREG
	
	while(1) //pisca led numa velocidade muito grande (visualmente fica ligado)
	    cpl_bit(PORTD, LED2);
} 
//----------------------------------------------------------------------------------
ISR(INT0_vect) //interrup��o externa 0, quando o bot�o � pressionado o LED pisca
{
	unsigned char sreg;
	
	sreg = SREG; //salva SREG porque a interrup��o pode alterar o seu valor
	
	clr_bit(EIMSK, INT0); //desabilita INTO para que ele n�o chame a si mesmo
	sei();                //habilita a interrup��o geral, agora INT1 pode interromper INT0
	
	cpl_bit(PORTD, LED1); //pisca led a cada 300 ms
	_delay_ms(300);
	set_bit(EIMSK, INT0); //habilita novamente a interrup��o INT0
	
	SREG = sreg; //restaura o valor de SREG que pode ter sido alterado
}
//----------------------------------------------------------------------------------
ISR(INT1_vect) //interrup��o externa 1, mantendo o bot�o pressionado o LED pisca
{
	cpl_bit(PORTD, LED1); //pisca led a cada 200 ms
	_delay_ms(200);
}
//==================================================================================