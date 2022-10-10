/*
 * s07_e01_escrevendo_xAA_em_toda_a_EEPROM.c
 *
 * Created: 28/09/2022 16:50:28
 * Author : Matheus
 *
 * 7.1 – Faça um programa para gravar 0xAA em toda a memória EEPROM do ATmega328.
 * Certifique-se que os dados foram corretamente gravados, lendo-os após a
 * gravação. Pode ser empregado um display para informar sobre o sucesso da
 * operação ou um LED. Depois disso, com base no manual do ATmega328, empregue a
 * interrupção da EEPROM para repetir o processo.
 */

#include "definicoes_principais.h" //inclui a biblioteca de definições principais


//definição de hardware
#define LED PB5 //substitui LED por PB5 na programação

//rotina de tratamento de interrupção
ISR(EE_READY_vect);

int main(void) //função principal
{
	DDRB = 0xFF; //define todo o PORTB como saída
	PORTB = 0x00; //coloca todo o PORTB em nível baixo

	EECR = (1 << EERIE); //habilita a interrupção da EEPROM
	sei(); //seta o bit I do registrador SREG para habilitar todas as interrupções

	while(1); //loop infinito
}

//implementando rotina de tratamento de interrupção

ISR(EE_READY_vect)
{
    static unsigned int end_eeprom = 0x00; //declaração de variável local estática
	
	if(end_eeprom <= 0x03FF) //escrita de 0x0000 até 0x3FF
	{
		cli(); //limba o bit I do registrador SREG desabilitando todas as interrupções
		/* Aguarda completar a escrita anterior */
		while(EECR & (1 << EEPE));
		/* Carrega os registradores de endereço e dado */
		EEAR = end_eeprom++;
		EEDR = 0xAA;
		/* Escreve um lógico em EEMPE */
		EECR |= (1 << EEMPE);
		/* Inicia a escrita na eeprom setando EEPE */
		EECR |= (1 << EEPE);
		sei(); //seta o bit I do registrador SREG para habilitar todas as interrupções
	}
	else if(end_eeprom <= 0x07FE) //leitura de 0x04000 até 0x07FE
	{
		unsigned char leitura_eeprom; //variável auxiliar
		/* Aguarda completar a escrita anterior */
		while(EECR & (1 << EEPE));
		/* Carrega o registrador de endereço */
		EEAR = end_eeprom - 0x03FF; //end_eeprom++;
		/* Inicia a leitrua da eeprom setando EERE */
		EECR |= (1 << EERE);
		/* Armazena o dado dReturn data from Data Register */
		leitura_eeprom = EEDR;
		if(leitura_eeprom == 0xAA) set_bit(PORTB, LED); //se o dado foi gravado corretamente, liga o led
		else
		{
			clr_bit(PORTB, LED); //se o dado foi gravado incorretamente, desliga o led
			EECR = ~(1 << EERIE); //desativa a interrupção da eeprom
			cli(); //limpa o bit I do registrador SREG desabilitando todas as interrupções
		}
		end_eeprom++; //incremeta o endereço da eeprom
	}
	else
	{
		EECR = ~(1 << EERIE); //desabilita a interrupção da eeprom
		cli(); //limpa o bit I do registrador SREG desabilitando todas as interrupções
	}
}
