//======================================================================== //
// LEITURA DE UM TECLADO 4 x 4                                             //
//======================================================================== //
#include "def_principais.h" //inclusão do arquivo com as principais definições
#include "LCD_4bits.h"
#include "teclado.h"


//definição para acessar a memória flash como ponteiro
//prog_char mensagem1[] = "Teclado 4 x 4\0"; //mensagem armazenada na memória flash
//prog_char mensagem2[] = "tecla =\0";       //mensagem armazenada na memória flash
const char mensagem1[] PROGMEM = "Teclado 4 x 4\0"; //mensagem armazenada na memória flash
const char mensagem2[] PROGMEM = "tecla =\0";       //mensagem armazenada na memória flash

int main()
{
	unsigned char nr;
	
	DDRB = 0xFF;   //LCD esta no PORTB
	DDRD = 0x0F;   //definições das entradas e saídas para o teclado
	PORTD= 0xFF;   //habilita os pull-ups do PORTD e coloca colunas em 1
	UCSR0B = 0x00; //para uso dos PORTD no Arduino
	
	inic_LCD_4bits();
	escreve_LCD_Flash(mensagem1);
	cmd_LCD(0xC7, 0); //desloca cursor para a 2a linha do LCD
	escreve_LCD_Flash(mensagem2);
	
	while(1)
	{
		nr = ler_teclado(); //lê constantemente o teclado
		if(nr != 0xFF)      //se alguma tecla foi pressionada mostra seu valor
		{
			cmd_LCD(0xCF, 0); //desloca cursor para a última posição da 2a linha
			cmd_LCD(nr, 1);   //nr já está em formato ASCII
		}
	}
}
//=============================================================================