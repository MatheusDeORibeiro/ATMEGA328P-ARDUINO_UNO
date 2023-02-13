/*
 * LCDG.c
 *
 * Created: 27/01/2023 16:15:39
 *  Author: Matheus
 */ 


#include "LCDG.h"
//#include "tabela_ASCII.h"

//-----------------------------------------------------------------------------------
//Inicialização padrão, as inicializações devem ser feitas conforme necessário.
//-----------------------------------------------------------------------------------
void inic_LCDG()
{
	set_CS1();
	set_CS2();
	reset_LCDG();
	set_instrucao();
	set_escrita();
	DADOS = LIGA_LCD;
	enable_LCDG();
	DADOS = LIN_INIC;
	enable_LCDG();
	DADOS = Y_INIC;
	enable_LCDG();
	DADOS = PAG_INIC;
	enable_LCDG();
	
	limpa_LCDG(0x00);
}
//-----------------------------------------------------------------------------------
//Escreve 1 byte na coluna e pagina especificada
//-----------------------------------------------------------------------------------
void escreve_LCDG(unsigned char dado, unsigned char coluna, unsigned char pagina)
{
	set_instrucao(); //instrução
	if (coluna > 63) //coluna 0-127
	{
		clr_CS1();
		set_CS2();
		DADOS = Y_INIC + coluna - 64;
	}
	else
	{
		clr_CS2();
		set_CS1();
		DADOS = Y_INIC + coluna;
	}
	enable_LCDG();
	DADOS = PAG_INIC + pagina; //página 0 - 7
	enable_LCDG();
	//escreve dado
	set_dado();
	DADOS = dado;
	enable_LCDG();
}
//-----------------------------------------------------------------------------------
//Limpa o LCDG - 0x00 apaga todos os pixels e 0xFF liga
//-----------------------------------------------------------------------------------
void limpa_LCDG(unsigned char limpa)
{
	unsigned char pag, col;
	for(pag = 0; pag < 8; pag++)
	{
		for(col = 0; col < 128; col++)
		escreve_LCDG(limpa, col, pag);
	}
}
//-----------------------------------------------------------------------------------
//Escreve uma imagem completa (1024 bytes)
//-----------------------------------------------------------------------------------
void escreve_todo_LCDG(const unsigned char *figura)
{
	unsigned char pag, col;
	for(pag = 0; pag < 8; pag++)
	{
		for(col = 0; col < 128; col++)
		{
			escreve_LCDG(pgm_read_byte(&(*figura)), col, pag);
			figura++;
		}
	}
}
//-----------------------------------------------------------------------------------
//Escreve um caractere da tabela ASCII (igual ao LCD com controlador HD44780)
//-----------------------------------------------------------------------------------
//void escreve_caractere_LCDG(unsigned char caractere, unsigned char col, unsigned char pag)
//{
	//unsigned char i;
	//for (i = 0; i < 5; i++)
	//escreve_LCDG(pgm_read_byte(&tabela_ASCII[caractere - 32][i]), col + i, pag);
	//escreve_LCDG(0, col + 5, pag);//uma coluna em branco após o caractere
//}
//-----------------------------------------------------------------------------------
//Escreve um conjunto de caracteres gravados na FLAHS, conforme tabela ASCII
//-----------------------------------------------------------------------------------
void escreve_stringFLASH_LCDG(char *c, unsigned char col, unsigned char pag)
{
	unsigned char j = 0;
	for(; pgm_read_byte(&(*c)) != 0; c++)
	{
		escreve_caractere_LCDG(pgm_read_byte(&(*c)), col + 6 * j, pag);
		j++;
	}
}
//-----------------------------------------------------------------------------------
//Escreve um conjunto de caracteres gravados na RAM, conforme tabela ASCII
//-----------------------------------------------------------------------------------
void escreve_stringRAM_LCDG(char *c, unsigned char col, unsigned char pag)
{
	unsigned char j = 0;
	for(; *c != 0; c++)
	{
		escreve_caractere_LCDG(*c, col + 6 * j, pag);
		j++;
	}
}
//-----------------------------------------------------------------------------------
