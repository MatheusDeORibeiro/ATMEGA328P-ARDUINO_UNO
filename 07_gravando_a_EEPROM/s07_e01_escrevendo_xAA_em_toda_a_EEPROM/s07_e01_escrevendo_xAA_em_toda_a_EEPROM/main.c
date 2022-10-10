/*
 * s07_e01_escrevendo_xAA_em_toda_a_EEPROM.c
 *
 * Created: 28/09/2022 16:50:28
 * Author : Matheus
 *
 * 7.1 � Fa�a um programa para gravar 0xAA em toda a mem�ria EEPROM do ATmega328.
 * Certifique-se que os dados foram corretamente gravados, lendo-os ap�s a
 * grava��o. Pode ser empregado um display para informar sobre o sucesso da
 * opera��o ou um LED. Depois disso, com base no manual do ATmega328, empregue a
 * interrup��o da EEPROM para repetir o processo.
 */

#include "definicoes_principais.h" //inclui a biblioteca de defini��es principais


//defini��o de hardware
#define LED PB5 //substitui LED por PB5 na programa��o

//rotina de tratamento de interrup��o
ISR(EE_READY_vect);

int main(void) //fun��o principal
{
	DDRB = 0xFF; //define todo o PORTB como sa�da
	PORTB = 0x00; //coloca todo o PORTB em n�vel baixo

	EECR = (1 << EERIE); //habilita a interrup��o da EEPROM
	sei(); //seta o bit I do registrador SREG para habilitar todas as interrup��es

	while(1); //loop infinito
}

//implementando rotina de tratamento de interrup��o

ISR(EE_READY_vect)
{
    static unsigned int end_eeprom = 0x00; //declara��o de vari�vel local est�tica
	
	if(end_eeprom <= 0x03FF) //escrita de 0x0000 at� 0x3FF
	{
		cli(); //limba o bit I do registrador SREG desabilitando todas as interrup��es
		/* Aguarda completar a escrita anterior */
		while(EECR & (1 << EEPE));
		/* Carrega os registradores de endere�o e dado */
		EEAR = end_eeprom++;
		EEDR = 0xAA;
		/* Escreve um l�gico em EEMPE */
		EECR |= (1 << EEMPE);
		/* Inicia a escrita na eeprom setando EEPE */
		EECR |= (1 << EEPE);
		sei(); //seta o bit I do registrador SREG para habilitar todas as interrup��es
	}
	else if(end_eeprom <= 0x07FE) //leitura de 0x04000 at� 0x07FE
	{
		unsigned char leitura_eeprom; //vari�vel auxiliar
		/* Aguarda completar a escrita anterior */
		while(EECR & (1 << EEPE));
		/* Carrega o registrador de endere�o */
		EEAR = end_eeprom - 0x03FF; //end_eeprom++;
		/* Inicia a leitrua da eeprom setando EERE */
		EECR |= (1 << EERE);
		/* Armazena o dado dReturn data from Data Register */
		leitura_eeprom = EEDR;
		if(leitura_eeprom == 0xAA) set_bit(PORTB, LED); //se o dado foi gravado corretamente, liga o led
		else
		{
			clr_bit(PORTB, LED); //se o dado foi gravado incorretamente, desliga o led
			EECR = ~(1 << EERIE); //desativa a interrup��o da eeprom
			cli(); //limpa o bit I do registrador SREG desabilitando todas as interrup��es
		}
		end_eeprom++; //incremeta o endere�o da eeprom
	}
	else
	{
		EECR = ~(1 << EERIE); //desabilita a interrup��o da eeprom
		cli(); //limpa o bit I do registrador SREG desabilitando todas as interrup��es
	}
}
