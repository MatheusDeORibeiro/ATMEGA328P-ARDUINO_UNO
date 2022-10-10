/*
 * s08_e03_calculadora_basica.c
 *
 * Created: 07/10/2022 18:34:14
 * Author : Matheus
 *
 * 8.3 � Elaborar um programa para executar as fun��es matem�ticas b�sicas de uma
 * calculadora (n�meros inteiros), conforme circuito da fig. 8.3. Consulte o
 * ap�ndice B para a tabela de instru��es do LCD, mude o sentido de deslocamento
 * da mensagem para a esquerda na entrada de um novo caractere (0x07) e comece a
 * escrita dos caracteres na �ltima coluna da primeira linha.
 */ 

#include "definicoes_principais.h" //inclui biblioteca de defini��es principais
#include "LCD_4bits.h"             //inclui biblioteca de fun��es do display LCD com interface de 4 bits
#include "teclado_hexadecimal.h"   //inclui biblioteca de fun��es do teclado matricial 4x4


//declara��o de prot�tipo de fun��o
void exibe_resultado(int res, unsigned char *dig);

unsigned char dig_ind[_tam_vetor]; //declara��o de vetor para armazenar os digitos individualmente

int main(void) //fun��o principal
{
	unsigned char digitos, tecla, operacao = 0, negativo, div_erro;
	int valor1, valor2, resultado = 0, resto;

	DDRD = 0x0F; //configura os pinos PD3:0 como sa�das e os pinos PD7:4 como entradas
	DDRB = 0xFF; //configura todo o PORTB como sa�da
	DDRC = 0xFF; //configura todo o PORTC como sa�da
	PORTD = 0xF0; //habilita os resistores de pull-up das entradas e coloca as sa�das em n�vel baixo
	PORTB = 0x00; //coloca o PORTB em n�vel baixo
	PORTC = 0x00; //coloca o PORTC em n�vel baixo

	UCSR0B = 0x00; //desabilita as fun��es de RX e TX dos pinos PD0 e PD1

	Inicializa_LCD(LCD_2_N);                 //inicializa o display LCD com duas linhas
	Display_Cursor_ON_OFF(LCD_DSP_ON_CS_ON); //mant�m a tela do display ligada e o cursor n�o piscante
	Limpa_Display();         //limpa a tela do display
	Posiciona_Cursor(1, 16); //posiciona o cursor na �ltima coluna da primeira linha

	while(1) //loop infinito
	{
		digitos = 0;
		valor1 = 0;
		valor2 = 0;
		negativo = 0;
		div_erro = 0;
		Limpa_Display();
		Posiciona_Cursor(1, 16);
		//recebe o valor do primeiro inteiro
		while(digitos < 6)
		{
			tecla = Leitura_Teclado();
			_delay_ms(20);
			if(tecla != '\0' && tecla != ',' && tecla != '=') //exclui ',', que n�o ser� usada, e '=', que ainda n�o ser� lido
			{
				if(tecla == '-' && digitos == 0) //para um inteiro negativo
				{
					negativo = 1;
					Escreve_Dado(tecla);
				}
				else if(tecla != '+' && tecla != '-' && tecla != '/' && tecla != 'X' && digitos < 5) //l� os cinco digitos do valor
				{
					valor1 = (valor1 * 10) + (int)(tecla - _valor_decod);
					Escreve_Dado(tecla);
					digitos++;
				}
				else if((tecla == '+' || tecla == '-' || tecla == '/' || tecla == 'X') && digitos < 6) //l� a opera��o antes ou depois dos cinco digitos serem informados
				{
					operacao = tecla;
					Escreve_Dado(operacao);
					digitos = 6;
				}
			}
		}
		if(negativo) valor1 *= -1;
		//recebe o valor do segundo inteiro
		digitos = 0;
		negativo = 0;
		while(digitos < 6)
		{
			tecla = Leitura_Teclado();
			_delay_ms(20);
			if(tecla != '\0' && tecla != ',') //exclui ',', que n�o ser� usada
			{
				if(tecla == '-' && digitos == 0) //para um inteiro negativo
				{
					negativo = 1;
					Escreve_Dado(tecla);
				}
				else if(tecla != '=' && tecla != '+' && tecla != '-' && tecla != '/' && tecla != 'X' && digitos < 5) //l� os cinco digitos do valor
				{
					valor2 = (valor2 * 10) + (int)(tecla - _valor_decod);
					Escreve_Dado(tecla);
					digitos++;
				}
				else if(tecla == '=' && digitos < 6) //finaliza o processo antes ou depois de receber os cinco digitos
				{
					digitos = 6;
				} 
			}
		}
		if(negativo) valor2 *= -1;
		//realiza opera��o selecionada
		switch(operacao)
		{
			case '+':
			    resultado = valor1 + valor2;
			    break;
			case '-':
			    resultado = valor1 - valor2;
			    break;
			case '/':
			    if(valor2 != 0) resultado = valor1 / valor2;
			    else div_erro = 1;
			    break;
			case 'X':
			    resultado = valor1 * valor2;
			break;
		}
		//exibe o resultado da opera��o
		Escreve_Dado('=');
		if(resultado < 0) Escreve_Dado('-'); //para um resultado negativo
		if(operacao != '/') exibe_resultado(resultado, dig_ind); //para o resultado diferente de uma opera��o de uma divis�o
		else                                                     //para o resultado de uma opera��o de divis�o
		{
			if(!div_erro)
			{
				resto = valor1 % valor2;
				if(((valor1 < 0 && valor2 > 0) || (valor1 > 0 && valor2 < 0)) && (resultado == 0)) Escreve_Dado('-'); //para um resultado real negativo
				exibe_resultado(resultado, dig_ind);
				digitos = 0;
				if(resto != 0) Escreve_Dado(','); //para '/' com resto
				while(resto != 0 && digitos < 5)
				{
					valor1 = resto * 10;
					resultado = valor1 / valor2;
					exibe_resultado(resultado, dig_ind);
					resto = valor1 % valor2;
					digitos++;
				}
			}
			else //para o caso do denominador ser igual a 0
			{
				Limpa_Display();
				Posiciona_Cursor(2, 16);
				Escreve_S_memdados("Math ERROR");
			}
		}
		_delay_ms(1000);
	}
}


//implementa��o de fun��o

//Esta fun��o exibe o resulado da opera��o
void exibe_resultado(int res, unsigned char *dig)
{
	if(res < 0) res *= -1;
	Decodifica_Dig_Ind(res, dig);
	if(res >= 10000)
	{
		Escreve_Dado(dig[4]);
		Escreve_Dado(dig[3]);
		Escreve_Dado(dig[2]);
		Escreve_Dado(dig[1]);
		Escreve_Dado(dig[0]);
	}
	if(res >= 1000)
	{
		Escreve_Dado(dig[3]);
		Escreve_Dado(dig[2]);
		Escreve_Dado(dig[1]);
		Escreve_Dado(dig[0]);
	}
	if(res >= 100)
	{
		Escreve_Dado(dig[2]);
		Escreve_Dado(dig[1]);
		Escreve_Dado(dig[0]);
	}
	if(res >= 10)
	{
		Escreve_Dado(dig[1]);
		Escreve_Dado(dig[0]);
	}
	else
	{
		Escreve_Dado(dig[0]);
	}
}
