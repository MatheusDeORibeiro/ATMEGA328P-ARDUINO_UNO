/*
 * LCD_8bits.c
 *
 * Created: 12/09/2022 18:35:38
 *  Author: Matheus de Oliveira Ribeiro
 */  

/*
 * Esta biblioteca contém funções para a utilização de displays de LCD 16x2 na interface 8bit
 * compatíveis com os lcd controllers HD44780, KS0066, SED1278 e ST7066.
 */


#include "definicoes_principais.h" //inclui as definições principais
#include "LCD_8bits.h"             //inclui a biblioteca de funções do display LCD 16x2 8bits

//implementando funções

/* Esta função escreve uma instrução no display */
void Escreve_Instrucao(unsigned char instrucao)
{
	DIR_DADOS = 0xFF;        //configura o barramento de dados como saída
	clr_bit(PORT_CONT, _RS); //instrução
	clr_bit(PORT_CONT, _RW); //escrita
	PORT_DADOS = instrucao;  //carrega a instrução no barramento de dados
	Pulso_Enable();          //envia a instrução
	clr_bit(PORT_CONT, _RS); 
	_delay_us(45);           //aguarda o tempo de execução da instrução
	DIR_DADOS = 0x00;        //configura o barramento de dados como entrada
}

/* Esta função retorna o status o Busy Flag do dispay: 1 - ocupado e 0 - livre */
unsigned char Status_BF(void)
{
	unsigned char leitura_bf;        //vairável auxiliar
	clr_bit(PORT_CONT, _RS);         //instrução
	set_bit(PORT_CONT, _RW);         //leitura
	_delay_us(1);
	set_bit(PORT_CONT, _EN);
	_delay_us(1);
	leitura_bf = tst_bit(PIND, _BF); //armazena o status do busy flag na variável
	if(leitura_bf == 1)              //se variável igual a 1, LCD ocupado
	{
		clr_bit(PORT_CONT, _EN);
		clr_bit(PORT_CONT, _RW);
		return 1;                    //ocupado
	}
	else                             //se variável igual a 0, LCD livre
	{
		clr_bit(PORT_CONT, _EN);
		clr_bit(PORT_CONT, _RW);
		return 0;		             //livre
	}	
}

/* Esta função escreve um dado para no display */
void Escreve_Dado(unsigned char dado)
{
	DIR_DADOS = 0xFF;        //configura o barramento de dados como saída
	set_bit(PORT_CONT, _RS); //dado
	clr_bit(PORT_CONT, _RW); //escrita
	PORT_DADOS = dado;       //carrega dado no barramento de dados
	Pulso_Enable();          //envia dado
	clr_bit(PORT_CONT, _RS);
	_delay_us(45);           //aguarda o tempo de execução da instrução
	DIR_DADOS = 0x00;        //configura o barramento de dados como entrada
}

/* Esta função lê um dado do display */
unsigned char Leitura_Dado(void)
{
	unsigned char leitura;   //variável auxiliar
	set_bit(PORT_CONT, _RS); //dado
	set_bit(PORT_CONT, _RW); //leitura
	_delay_us(1);
	set_bit(PORT_CONT, _EN);
	_delay_us(1);
	leitura = PIND;          //armazena byte na variável auxiliar
	clr_bit(PORT_CONT, _EN);
	_delay_us(45);           //aguarda o tempo de execução da instrução
	clr_bit(PORT_CONT, _RS);
	clr_bit(PORT_CONT, _RW);
	return leitura;          //retorna o valor lido
}

/* Esta função envia um pulso para o pino Enable */
void Pulso_Enable(void)
{
	_delay_us(1);
	set_bit(PORT_CONT, _EN);
	_delay_us(1);
	clr_bit(PORT_CONT, _EN);
}

/* Esta função iniicaliza o display controller */
void Inicializa_LCD(unsigned char num_linhas)
{
	unsigned char i; //declaração de variável auxiliar
	unsigned char ajuste[3] = {0x0F, 0x01, 0x06}; 
		          //display ON e cursor piscante, limpa display e deslocamento para a direita
	
	//configuração de hardware
	clr_bit(PORT_CONT, _RS);
	clr_bit(PORT_CONT, _RW);
	clr_bit(PORT_CONT, _EN);
	DIR_DADOS = 0xFF; //configura o barramento de dados como saída
	DIR_CONT |= 0x07; //configura o barramento de controle como saída
	
	_delay_ms(15);    //depois de Vcc atingir no mínimo 4,5V, aguarda 15ms
	
	//ajuste de função
	for(i = 0; i < 3; i++)
	{
		PORT_DADOS = 0x30; //carrega instrução no barramento
		Pulso_Enable();    //envia instrução
		_delay_ms(5);
	}
	
	//ajuste de função - 8bits e 5x8 pontos
	if(num_linhas) PORT_DADOS = 0x38; //duas linhas 
	else PORT_DADOS = 0x30;                //uma linha
	Pulso_Enable();
	_delay_ms(5);
	
	//configurações do vetor ajuste
	for(i = 0; i < 3; i++)
	{
		PORT_DADOS = ajuste[i]; //carrega instrução no barramento
		Pulso_Enable();         //envia instrução
		_delay_ms(5);
	}
	
	DIR_DADOS = 0x00; //configura o barramento de dados como saída
}

/* Esta função limpa a tela do display */
void Limpa_Display(void)
{
	Escreve_Instrucao(0x01);
	_delay_ms(2);
	//while(Status_BF());
}

/* Esta função posiciona o cursor na primeira coluna da primeira linha */
void Cursor_Home(void)
{
	Escreve_Instrucao(0x02);
	//_delay_ms(2);
	while(Status_BF());
}

/* Esta função liga/desliga o display e/ou cursor */
void Display_Cursor_ON_OFF(unsigned char display_cursor)
{
	Escreve_Instrucao(display_cursor);
	//_delay_us(40);
	while(Status_BF());
}

/* Esta função desloca o cursor e/ou o display */
void Desloca_Mensagem(unsigned char deslocamento)
{
	Escreve_Instrucao(deslocamento);
	//_delay_us(40);
	while(Status_BF());
}

/* Esta função posiciona o cursor em uma coluna de uma linha */
void Posiciona_Cursor(signed char linha, signed char coluna)
{
	switch(linha)
	{
		case 1: //caso seja na linha 1
		    Escreve_Instrucao(0x80 + (coluna - 1));
		    break;
		case 2: //caso seja na linha 2
	    	Escreve_Instrucao(0xC0 + (coluna - 1));
		    break;
	}
}

/* Esta função liga todos os pixels do display */
void Testa_Display(void)
{
	unsigned char i;
	
	Posiciona_Cursor(1, 1);
	while(Status_BF());
	
	for(i = 0; i < 32; i++)
	{
		if(i < 16)                  //preenche a primeira linha do display
		{
			Escreve_Dado(0xFF);
			while(Status_BF());
		}
		if(i == 16)                 //preenche o último caractere da primeira linha
		{
			Posiciona_Cursor(2, 1);
			while(Status_BF());
			Escreve_Dado(0xFF);
			while(Status_BF());
		}
		else                       //preenche toda a segunda linha do display
		{
			Escreve_Dado(0xFF);
			while(Status_BF());
		}
	}	
}

/* Esta função escreve uma string salva na mamória de dados */
void Escreve_S_memdados(char *string)
{
	while(*string) //percorre a string até encontrar o caractere nulo
	{
		while(Status_BF());      //aguarda o envio do dado anterior
		Escreve_Dado(*string++); //envia o caracter apontado e incrementa
	}
}

/* Esta função escreve uma string salva na mamória de programa */
void Escreve_S_memprogm(const unsigned char *string)
{
	while(pgm_read_byte(&(*string))) //percorre a string até encontrar o caractere nulo
	{
		while(Status_BF());                        //aguarda o envio do dado anterior
		Escreve_Dado(pgm_read_byte(&(*string++))); //envia o caracter apontado e incrementa
	}	
}
