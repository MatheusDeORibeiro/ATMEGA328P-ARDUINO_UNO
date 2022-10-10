/*
 * teclado_hexadecimal.c
 *
 * Created: 30/09/2022 15:26:15
 *  Author: Matheus de Oliveira Ribeiro
 */ 

/* Esta biblioteca contém a função para leitura de um teclado hexadecimal */
/*
 *      C1   C2   C3   C4
 * L1    1    2    3    L
 * L2    4    5    6    A
 * L3    7    8    9    U
 * L4    *    0    #    D
 */


#include "teclado_hexadecimal.h"   //inclui a biblioteca de definições principais
#include "definicoes_principais.h" //inclui a biblioteca de funções do teclado hexadecimal

/* Esta função retorna o caractere correspondente à tecla pressionada */
unsigned char Leitura_Teclado(void)
{
	//declaração de variável auxiliar
	static char tecla_ant = 0;       //armazena o status da tecla	
    DIR_TECLADO = 0x0F;  //configura as linhas como entradas e as colunas como saída
	PORT_TECLADO = 0xFF; //habilita pull-ups das entrada e coloca saídas em nível alto
	clr_bit(PORT_TECLADO, _C1); //seleciona a coluna 1(PD4)
	_delay_ms(10);
	if(!tst_bit(LE_TECLADO, _L1))
	{
		if(tecla_ant) return '\0';
		else
		{
			tecla_ant = 1;
			return '1';
		}
	}
	if(!tst_bit(LE_TECLADO, _L2))
	{
		if(tecla_ant) return '\0';
		else
		{
			tecla_ant = 1;
			return '4';
		}
	}
	if(!tst_bit(LE_TECLADO, _L3))
	{
		if(tecla_ant) return '\0';
		else
		{
			tecla_ant = 1;
			return '7';
		}
	}
	if(!tst_bit(LE_TECLADO, _L4))
	{
		if(tecla_ant) return '\0';
		else
		{
			tecla_ant = 1;
			return '*';
		}
	}
	set_bit(PORT_TECLADO, _C1);
	clr_bit(PORT_TECLADO, _C2); //seleciona a coluna 2(PD5)
	_delay_ms(10);
	if(!tst_bit(LE_TECLADO, _L1))
	{
		if(tecla_ant) return '\0';
		else
		{
			tecla_ant = 1;
			return '2';
		}
	}
	if(!tst_bit(LE_TECLADO, _L2))
	{
		if(tecla_ant) return '\0';
		else
		{
			tecla_ant = 1;
			return '5';
		}
	}
	if(!tst_bit(LE_TECLADO, _L3))
	{
		if(tecla_ant) return '\0';
		else
		{
			tecla_ant = 1;
			return '8';
		}
	}
	if(!tst_bit(LE_TECLADO, _L4))
	{
		if(tecla_ant) return '\0';
		else
		{
			tecla_ant = 1;
			return '0';
		}
	}
	set_bit(PORT_TECLADO, _C2);
	clr_bit(PORT_TECLADO, _C3); //seleciona a coluna 3(PD6)
	_delay_ms(10);
	if(!tst_bit(LE_TECLADO, _L1))
	{
		if(tecla_ant) return '\0';
		else
		{
			tecla_ant = 1;
			return '3';
		}
	}
	if(!tst_bit(LE_TECLADO, _L2))
	{
		if(tecla_ant) return '\0';
		else
		{
			tecla_ant = 1;
			return '6';
		}
	}
	if(!tst_bit(LE_TECLADO, _L3))
	{
		if(tecla_ant) return '\0';
		else
		{
			tecla_ant = 1;
			return '9';
		}
	}
	if(!tst_bit(LE_TECLADO, _L4))
	{
		if(tecla_ant) return '\0';
		else
		{
			tecla_ant = 1;
			return '#';
		}
	}
	set_bit(PORT_TECLADO, _C3);
	clr_bit(PORT_TECLADO, _C4); //seleciona a coluna 4(PD7)
	_delay_ms(10);
	if(!tst_bit(LE_TECLADO, _L1))
	{
		if(tecla_ant) return '\0';
		else
		{
			tecla_ant = 1;
			return 'L';
		}
	}
	if(!tst_bit(LE_TECLADO, _L2))
	{
		if(tecla_ant) return '\0';
		else
		{
			tecla_ant = 1;
			return 'A';
		}
	}
	if(!tst_bit(LE_TECLADO, _L3))
	{
		if(tecla_ant) return '\0';
		else
		{
			tecla_ant = 1;
			return 'U';
		}
	}
	if(!tst_bit(LE_TECLADO, _L4))
	{
		if(tecla_ant) return '\0';
		else
		{
			tecla_ant = 1;
			return 'D';
		}
	}
	DIR_TECLADO = 0xF0; //configura PD3:0 como entrada e PD7:4 como entrada
	tecla_ant = 0;      //nenhuma tecla foi pressionada	
	return '\0';
}
