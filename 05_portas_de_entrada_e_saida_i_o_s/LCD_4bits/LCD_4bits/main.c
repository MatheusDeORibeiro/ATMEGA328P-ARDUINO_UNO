//================================================================================ //
// ACIONANDO UM DISPLAY DE CRISTAL LIQUIDO DE 16x2 //
// Interface de dados de 4 bits //
//================================================================================ //

#include "def_principais.h" //inclus�o do arquivo com as principais defini��es
#include "LCD_4bits.h"

//defini��o para acessar a mem�ria flash
//prog_char mensagem[] = " DADOS DE 4BITS!\0"; //mensagem armazenada na mem�ria flash
const char mensagem[] PROGMEM = " DADOS DE 4BITS!\0"; //mensagem armazenada na mem�ria flash
//-----------------------------------------------------------------------------------
int main()
{
	DDRD = 0xFF; //PORTD como sa�da
	DDRB = 0xFF; //PORTB como sa�da
	
	inic_LCD_4bits();             //inicializa o LCD
	escreve_LCD(" INTERFACE DE"); //string armazenada na RAM
	cmd_LCD(0xC0,0);              //desloca cursor para a segunda linha
	escreve_LCD_Flash(mensagem);  //string armazenada na flash
	
	for(;;){} //la�o infinito, aqui vai o c�digo principal
}
//===================================================================================