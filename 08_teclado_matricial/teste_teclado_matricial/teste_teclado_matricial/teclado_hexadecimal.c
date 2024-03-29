/*
 * teclado_hexadecimal.c
 *
 * Created: 30/09/2022 15:26:15
 *  Author: Matheus de Oliveira Ribeiro
 */ 

/* Esta biblioteca cont�m a fun��o para leitura de um teclado hexadecimal */
/*
 *      C1   C2   C3   C4
 * L1    1    2    3  x F
 * L2    4    5    6  / E
 * L3    7    8    9  - D
 * L4  , A    0  = B  + C
 */


#include "teclado_hexadecimal.h"   //inclui a biblioteca de defini��es principais
#include "definicoes_principais.h" //inclui a biblioteca de fun��es do teclado hexadecimal

/* Esta fun��o retorna o caractere correspondente � tecla pressionada */
unsigned char Leitura_Teclado(void)
{
    DIR_TECLADO = 0x0F;  //configura as linhas como entradas e as colunas como sa�da
	PORT_TECLADO = 0xFF; //habilita pull-ups das entrada e coloca entradas em n�vel alto
	clr_bit(PORT_TECLADO, _C1); //seleciona a coluna 1(PD4)
	_delay_ms(10);
	if(!tst_bit(LE_TECLADO, _L1)) return '1';
	if(!tst_bit(LE_TECLADO, _L2)) return '4';
	if(!tst_bit(LE_TECLADO, _L3)) return '7';
	if(!tst_bit(LE_TECLADO, _L4)) return 'A';
	set_bit(PORT_TECLADO, _C1);
	clr_bit(PORT_TECLADO, _C2); //seleciona a coluna 2(PD5)
	_delay_ms(10);
	if(!tst_bit(LE_TECLADO, _L1)) return '2';
	if(!tst_bit(LE_TECLADO, _L2)) return '5';
	if(!tst_bit(LE_TECLADO, _L3)) return '8';
	if(!tst_bit(LE_TECLADO, _L4)) return '0';
	set_bit(PORT_TECLADO, _C2);
	clr_bit(PORT_TECLADO, _C3); //seleciona a coluna 3(PD6)
	_delay_ms(10);
	if(!tst_bit(LE_TECLADO, _L1)) return '3';
	if(!tst_bit(LE_TECLADO, _L2)) return '6';
	if(!tst_bit(LE_TECLADO, _L3)) return '9';
	if(!tst_bit(LE_TECLADO, _L4)) return 'B';
	set_bit(PORT_TECLADO, _C3);
	clr_bit(PORT_TECLADO, _C4); //seleciona a coluna 4(PD7)
	_delay_ms(10);
	if(!tst_bit(LE_TECLADO, _L1)) return 'F';
	if(!tst_bit(LE_TECLADO, _L2)) return 'E';
	if(!tst_bit(LE_TECLADO, _L3)) return 'D';
	if(!tst_bit(LE_TECLADO, _L4)) return 'C';
	DIR_TECLADO = 0xF0; //configura PD3:0 como entrada e PD7:4 como entrada
	return '\0';
}
