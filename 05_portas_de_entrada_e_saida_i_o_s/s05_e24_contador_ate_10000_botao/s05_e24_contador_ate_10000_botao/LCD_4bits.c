/*
 * LCD_4bits.c
 *
 * Created: 15/09/2022 16:42:37
 *  Author: Matheus de Oliveira Ribeiro
 */ 

/*
 * Esta biblioteca cont�m fun��es para a utiliza��o de displays LCD 16x2 na interface de 4bits
 * compat�vel os LCD controllers HD44780, KS0066U, SED1278 e ST7066.
 */


#include "LCD_4bits.h"             //inclui a biblioteca de fun��es do display LCD 16x2 4bits
#include "definicoes_principais.h" //inclui a biblioteca de defini��es principais

//implementa��o das fun��es

/* Esta escreve uma instru��o para o display */
void Escreve_Instrucao(unsigned char instrucao)
{
    DIR_DADOS |= 0xF0;       //configura o barramento de dados como sa�da
	//seleciona opera��o
	clr_bit(PORT_CONT, _RS); //instru��o
	clr_bit(PORT_CONT, _RW); //escrita
	PORT_DADOS = ((PORT_DADOS & 0x0F) | (instrucao & 0xF0)); //n�o altera o valor do nibble menos significativo do PORTD
	                         //carrega o nibble mais significativo
	Pulso_Enable();          //envia o nibble mais significativo
	PORT_DADOS = ((PORT_DADOS & 0x0F) | ((instrucao << 4) & 0xF0));
	                         //carrega o nibble menos significativo
	Pulso_Enable();          //envia o nibble mais significativo
	_delay_us(45);           //aguarda o tempo de execu��o da instru��o
	clr_bit(PORT_CONT, _RW);          
	//configura o barramento de dados como entrada
	clr_bit(DIR_DADOS, _DB7);
	clr_bit(DIR_DADOS, _DB6);
	clr_bit(DIR_DADOS, _DB5);
	clr_bit(DIR_DADOS, _DB4);
}

/* Esta fun��o retorna o status do busy flag: 1 - ocupado e 0 - livre */
unsigned char Status_BF(void)
{
	unsigned char leitura_bf;    //vari�vel auxiliar
	clr_bit(PORT_CONT, _RS);
	set_bit(PORT_CONT, _RW);
	_delay_us(1);
	set_bit(PORT_CONT, _EN);
	_delay_us(1);
	leitura_bf = PIND & 0xF0;    //carrega o nibble mais significativo
	clr_bit(PORT_CONT, _EN);
	_delay_us(1);
	set_bit(PORT_CONT, _EN);
	_delay_us(1);
	//leitura_bf =(leitura_bf << 4) | (PIND & 0xF0); //carrega o nibble menos significativo
	if(leitura_bf & 0x80)        //se o bit 7 for igual a 1, o display est� ocupado
	{
		clr_bit(PORT_CONT, _EN);
		clr_bit(PORT_CONT, _RW);
		return 1;                //ocupado
	}
	else                         //se n�o, o display est� livre
	{
		clr_bit(PORT_CONT, _EN);
		clr_bit(PORT_CONT, _RW);
		return 0;                //livre
	}
}

/* Esta fun��o escreve um dado no display */
void Escreve_Dado(unsigned char dado)
{
	DIR_DADOS |= 0xF0;         //configura o barramento de dados como sa�da
	//seleciiona opera��o
	set_bit(PORT_CONT, _RS);   //dado
	clr_bit(PORT_CONT, _RW);   //escrita
	PORT_DADOS = ((PORT_DADOS & 0x0F) | (dado & 0xF0));
	                           //carrega o nibble mais significativo no barramento de dados
	Pulso_Enable();            //envia o nibble mais significativo
	PORT_DADOS = ((PORT_DADOS & 0x0F) | ((dado << 4) & 0xF0));
	                           //carrega o nibbe menos significativo no barramento de dados
	Pulso_Enable();            //envia o nibble menos significativo
	clr_bit(PORT_CONT, _RS);
	_delay_us(45);             //aguarda o tempo de execu��o da instru��o
	//configura o barramento de dados como entrada
	clr_bit(DIR_DADOS, _DB7);
	clr_bit(DIR_DADOS, _DB6);
	clr_bit(DIR_DADOS, _DB5);
	clr_bit(DIR_DADOS, _DB4);
}

/* Esta fun��o l� um dado do display */
unsigned char Leitura_Dado(void)
{
	unsigned char leitura;                    //vari�vel auxiliar
	set_bit(PORT_CONT, _RS);                  //dado
	set_bit(PORT_CONT, _RW);                  //instru��o
	_delay_ms(1);
	set_bit(PORT_CONT, _EN);
	_delay_ms(1);
	leitura = PIND & 0xF0;                    //carrega nibble mais significativo
	clr_bit(PORT_CONT, _EN);
	_delay_ms(1);
	set_bit(PORT_CONT, _EN);
	_delay_ms(1);
	leitura = leitura | ((PIND & 0xF0) >> 4); //carrega nibble menos significativo
	clr_bit(PORT_CONT, _EN);
	_delay_us(45);                            //aguarda tempo de execu��o da isntru��o
	clr_bit(PORT_CONT, _RS);
	clr_bit(PORT_CONT, _RW);
	return leitura;                           //retorna o dado lido
}

/* Esta fun��o envia um pulso para o pino Enable */
void Pulso_Enable(void)
{
	_delay_us(2);
	set_bit(PORT_CONT, _EN);
	_delay_us(2);
	clr_bit(PORT_CONT, _EN);
}

/* Esta fun��o inicializa o display com a interface de 4 bits */
void Inicializa_LCD(unsigned char numero_linhas)
{
	unsigned char i;
	unsigned char ajuste[3] = {0x0F, 0x01, 0x06};
                  //display ON e cursor piscante, limpa display e deslocamento para a direita
	
	//configura��o de hardware
	clr_bit(PORT_CONT, _RS);
	clr_bit(PORT_CONT, _RW);
	clr_bit(PORT_CONT, _EN);
	DIR_DADOS |= 0xF0;       //configura o barramento de dados como sa�da
	DIR_CONT  |= 0x07;       //configura o barramento de controle como sa�da
	
	_delay_ms(15);           //aguarda 15ms ap�s Vcc atingir no m�nimo 4,5V
	
	//ajuste de fun��o
	for(i = 0; i < 3; i++)
	{
		PORT_DADOS = ((PORT_DADOS & 0x0F) | (0x30 & 0xF0));
		Pulso_Enable();
		_delay_ms(5);
	}
	
	//ajuste de fun��o
	PORT_DADOS = ((PORT_DADOS & 0x0F) | (0x20 & 0xF0));
	Pulso_Enable();
	_delay_us(40);
	
	//ajuste de fun��o - 4bits e 5x8 pontos
	if(numero_linhas) 
	{
		PORT_DADOS = ((PORT_DADOS & 0x0F) | (0x28 & 0xF0));
		Pulso_Enable();
		PORT_DADOS = ((PORT_DADOS & 0x0F) | ((0x28 << 4) & 0xF0));
		Pulso_Enable();
	}
	else
	{
		PORT_DADOS = ((PORT_DADOS & 0x0F) | (0x20 & 0xF0));
		Pulso_Enable();
		PORT_DADOS = ((PORT_DADOS & 0x0F) | ((0x20 << 4) & 0xF0));
		Pulso_Enable();
	}
	_delay_ms(5);
	
	//configura��es do vetor ajustes
	for(i = 0; i < 3; i++)
	{
		PORT_DADOS = ((PORT_DADOS & 0x0F) | (ajuste[i] & 0xF0));
		Pulso_Enable();
		PORT_DADOS = ((PORT_DADOS & 0x0F) | ((ajuste[i] << 4) & 0xF0));
		Pulso_Enable();
		clr_bit(PORT_CONT, _EN);
		_delay_ms(2);
	}
	
	//configura o barramento de dados como entrada
	clr_bit(DIR_DADOS, _DB7);
	clr_bit(DIR_DADOS, _DB6);
	clr_bit(DIR_DADOS, _DB5);
	clr_bit(DIR_DADOS, _DB4);
}

/* Esta fun��o limpa a tela do display */
void Limpa_Display(void)
{
	Escreve_Instrucao(0x01);
	//_delay_ms(2);
	while(Status_BF());
}

/* Esta fun��o posiciona o cursor na primeira coluna da primeira linha */
void Cursor_Home(void)
{
	Escreve_Instrucao(0x02);
	_delay_ms(2);
	//while(Status_BF());
}

/* Esta fun��o liga/desliga o display e/ou o cursor */
void Display_Cursor_ON_OFF(unsigned char display_cursor)
{
	Escreve_Instrucao(display_cursor);
	while(Status_BF());
}

/* Esta fun��o desloca o display/cursor */
void Desloca_Mensagem(unsigned char deslocamento)
{
	Escreve_Instrucao(deslocamento);
	while(Status_BF());
}

/* Esta fun��o posiciona o cursor em uma determinda coluna de determinada linha */
void Posiciona_Cursor(unsigned char linha, unsigned coluna)
{
	switch(linha)
	{
		case 1:                                     //posiciona na primeira linha
		    Escreve_Instrucao(0x80 | (coluna - 1));
			break;
		case 2:			                            //posiciona na segunda linha
			Escreve_Instrucao(0xC0 | (coluna - 1));
			break;
		default:
		    break;
	}
}

/* Esta fun��o testa o display ligando todos os seus pixels */
void Testa_Display(void)
{
	unsigned char i; //vari�vel auxiliar
	
	Cursor_Home();
	while(Status_BF());
	
	for(i = 0; i < 32; i++)
	{
		if(i < 16)  //preenche a primeira linha
		{
			while(Status_BF());
			Escreve_Dado(0xFF);
		}
		if(i == 16) //posiciona o cursor na primeira coluna da segunda linha
		{
			while(Status_BF());
			Posiciona_Cursor(2, 1);
			Escreve_Dado(0xFF);
		}
		else       //preenche a segunda linha
		{
			while(Status_BF());
			Escreve_Dado(0xFF);
		}
	}
}

/* Esta fun��o escreve uma string da mem�ria de dados */
void Escreve_S_memdados(char *string)
{
	while(*string) //percorre a string at� encontrar o cractere nulo
	{
		while(Status_BF());
		Escreve_Dado(*string++); //envia caractere
	}
}

/* Esta fun��o escreve uma string da mem�ria de programa */
void Escreve_S_memprogm(const unsigned char *string)
{
	while(pgm_read_byte(&(*string))) //percorre a string at� encontrar o cractere nulo
	{
		while(Status_BF());
		Escreve_Dado(pgm_read_byte(&(*string++))); //envia um caractere
	}
}

/* Esta fun��o cria oito caracteres da mem�ria de dados */
void Cria_Caracteres_memdados(char *caracteres)
{
	unsigned char i, j, k = 0, end_CGRAM = 0x40; //declara��o de vari�veis
	for(i = 0; i < 8; i++)
	{
		Escreve_Instrucao(end_CGRAM);
		for(j = 0; j < 7; j++)
		{
			Escreve_Dado(caracteres[k + j]);
		}
		Escreve_Dado(0x00);
		k += 7;         //pr�ximos caractere armazenados no vetor
		end_CGRAM += 8; //endere�o do pr�ximo caractere
	}
}

/* Esta fun��o cria oito caracteres da mem�ria de programa */
void Cria_Caracteres_memprogm(const unsigned char *caracteres)
{
	unsigned char i, j, k = 0, end_CGRAM = 0x40; //declara��o de vari�veis
	for(i = 0; i < 8; i++)
	{
		Escreve_Instrucao(end_CGRAM);
		for(j = 0; j < 7; j++)
		{
			Escreve_Dado(pgm_read_byte(&caracteres[k + j]));
		}
		Escreve_Dado(0x00);
		k += 7;         //pr�ximo caractere armazenados no vetor
		end_CGRAM += 8; //endere�o do pr�ximo caractere
	}
}

/* Esta fun��o armazena os digitos individuais de um n�mero inteiro em um vetor */
void Decodifica_Dig_Ind(unsigned int numero, unsigned char *digitos)
{
	unsigned char n;                             //declara��o de vari�vel auxiliar
	for(n = 0; n < _tam_vetor; n++)
	    digitos[n] = 0 + _valor_decod;           //limpa o vetor
	do
	{
		*digitos = (numero % 10) + _valor_decod; //armazena o resto da divis�o
		numero /= 10;                            //divide o valor por 10
		digitos++;
	}while(numero != 0);                         //decodifica enquanto o n�mero for diferente de 0
}
