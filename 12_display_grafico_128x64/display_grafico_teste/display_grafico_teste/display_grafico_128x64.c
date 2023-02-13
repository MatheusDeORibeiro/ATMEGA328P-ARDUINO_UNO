/*
 * display_grafico_128x64.c
 *
 * Created: 01/02/2023 14:55:58
 *  Author: Matheus
 */ 

#include "display_grafico_128x64.h"
#include "tabela_ASCII.h"

/* Esta função inicializa o display gráfico 128x64 */
void Inicializacao_Display(void)
{
	DIR_DADOS = 0xFF;         //configura o barramento de dados como saída
	DIR_CONT = 0xFF;          //configura o barramento de controle como saída
	set_bit(PORT_CONT, _CS1); //habilita o chip 1
	set_bit(PORT_CONT, _CS2); //habilita o chip 2
	pulso_reset();            //reseta o controlador do display gráfico
	clr_bit(PORT_CONT, _RS);  //instrução
	clr_bit(PORT_CONT, _RW);  //escrita
	PORT_DADOS = 0x3F;        //liga a tela do display
	pulso_enable();           //envia um pulso no pino enable
	PORT_DADOS = 0x40;        //ajusta o endereço Y(contador de coluna)
	pulso_enable();           //envia um pulso no pino enable
	PORT_DADOS = 0xB8;        //ajusta o endereço X(página)
	pulso_enable();           //envia um pulso no pino enable
	PORT_DADOS = 0xC0;        //indica os dados mostrados no topo da tela
	pulso_enable();           //envia um pulso no pino enable
}

/* Esta função encreve uma instrução no display */
void Escreve_Instrucao(char instrucao)
{
	while(Status_BF());       //aguarda o display estar livre
	set_bit(PORT_CONT, _CS1); //habilita o chip 1
	set_bit(PORT_CONT, _CS2); //habilita o chip 2
	clr_bit(PORT_CONT, _RS);  //instrução
	clr_bit(PORT_CONT, _RW);  //escrita
	PORT_DADOS = instrucao;   //escreve instrução no barramento de dados
	pulso_enable();           //envia um pulso no pino enable
}

/* Esta função verifica o status do busy flag: 1 - ocupado; 0 - livre */
char Status_BF(void)
{
	DIR_DADOS = 0x00;                  //configura o barramento de dados como entrada
	//enviando a instrução
	set_bit(PORT_CONT, _CS1);          //habilita o chip 1
	set_bit(PORT_CONT, _CS2);          //habilita o chip 2
	clr_bit(PORT_CONT, _RS);           //instrução
	set_bit(PORT_CONT, _RW);           //leitura
	pulso_enable();                    //envia um pulso no pino enable
	//le o bit 7 do barramento de dados
	if(tst_bit(LE_DADOS, 7)) return 1; //controlador ocupado
	else                     return 0; //controlador livre
	DIR_DADOS = 0xFF;                  //configura o barramento de dados como saída
}

/* Esta função escreve um byte de dados no display */
void Escreve_Dado(char dado, unsigned char pagina, unsigned char coluna)
{
	while(Status_BF());       //aguarda o display estar livre
	Posiciona_Pagina(pagina); //ajusta a página
	Posiciona_Coluna(coluna); //ajusta a coluna
	clr_bit(PORT_CONT, _CS1); //desabilita o chip 1
	clr_bit(PORT_CONT, _CS2); //desabilita o chip 2
	if(coluna < 64) set_bit(PORT_CONT, _CS1); //habilita o chip 1
	else            set_bit(PORT_CONT, _CS2); //habilita o chip 2
	set_bit(PORT_CONT, _RS);  //dado
	clr_bit(PORT_CONT, _RW);  //escrita
	PORT_DADOS = dado;        //carrega o dado no barramento de dados
	pulso_enable();           //envia um pulso no pino enable
}

/* Esta função lê um byte de dados do display */
char Leitura_Dado(unsigned char pagina, unsigned char coluna)
{
	char leitura;             //variável auxiliar
	while(Status_BF());       //aguarda o display estar livre
	DIR_DADOS = 0x00;         //configura o barramento de dados como entrada
	Posiciona_Pagina(pagina); //ajusta a página
	Posiciona_Coluna(coluna); //ajusta a coluna
	clr_bit(PORT_CONT, _CS1); //desabilita o chip 1
	clr_bit(PORT_CONT, _CS2); //desabilita o chip 2
	//envia instrução
	if(coluna < 64) set_bit(PORT_CONT, _CS1);
	else            set_bit(PORT_CONT, _CS2);
	set_bit(PORT_CONT, _RS);  //dado
	set_bit(PORT_CONT, _RW);  //leitura
	pulso_enable();           //envia um pulso no pino enable
	leitura = LE_DADOS;       //leitura 'dummy'
	//lê o dado
	pulso_enable();           //envia um pulso no pino enable
	leitura = LE_DADOS;       //leitura efetiva
	DIR_DADOS = 0xFF;         //configura o barramento de dados como saída
	return leitura;           //retorna o dado lido
}

/* Esta função ajusta o endereço X(página)*/
void Posiciona_Pagina(char pagina)
{
	while(Status_BF());               //aguarda o display estar livre
	Escreve_Instrucao(0xB8 + pagina); //ajusta o endereço X para 'pagina'
}

/* Esta função ajusta o endereço Y(coluna) */
void Posiciona_Coluna(char coluna)
{
	while(Status_BF());               //aguarda o display estar livre
	if(coluna > 63) coluna -= 64;     //ajusta o valor de 'coluna', para 'coluna' maior do que 63
	Escreve_Instrucao(0x40 + coluna); //ajusta o endereço Y para 'coluna'
}

/* Esta função imprime uma string armazenada na memória de dados */
void Escreve_S_memdados(char *string, unsigned char pagina, unsigned char coluna)
{
	while(*string) //enquanto não encontrar o caractere nulo("\0")...
	{
		while(Status_BF());                   //aguarda o display estar livre
		for(unsigned char i = 0; i < 5; i++)  //varre as 5 colunas
		    Escreve_Dado(pgm_read_byte(&tabela_ASCII[*string - 0x20][i]), pagina, coluna++);
		Escreve_Dado(0x00, pagina, coluna++); //espaço em branco
		string++;                             //próximo elemento da string
	}
}

/* Esta função imprime uma string armazenada na memória de programa */
void Escreve_S_memprogm(const char *string, unsigned char pagina, unsigned char coluna)
{
	while(pgm_read_byte(string))               //enquanto não encontrar o caractere nulo("\0")...
	{
		while(Status_BF());                    //aguarda o display estar livre
		for(unsigned char i = 0; i < 5; i++)   //varre as 5 colunas
		    Escreve_Dado(pgm_read_byte(&tabela_ASCII[pgm_read_byte(string) - 0x20][i]), pagina, coluna++);
		Escreve_Dado(0x00, pagina, coluna++);  //espaço em branco
		string++;                              //próximo elemento da string
	}
}

/* Esta função imprime uma imagem armazenada na memória de programa */
void Imprime_Imagem(const char *imagem)
{
	for(unsigned char pagina = 0; pagina < 8; pagina++)             //varre as 8 páginas
	{
		for(unsigned char coluna = 0; coluna < 128; coluna++)       //varre as 128 colunas
		    Escreve_Dado(pgm_read_byte(imagem++), pagina, coluna);  //escreve o dado
	}
}

/* Esta função liga todos os pixels do display */
void Testa_Display(void)
{
	for(unsigned char pagina = 0; pagina < 8; pagina++)       //varre as 128 colunas
	{
		for(unsigned char coluna = 0; coluna < 128; coluna++) //varre as 8 páginas
		    Escreve_Dado(0xFF, pagina, coluna);               //escreve o dado
	}
}

/* Esta função desliga todos os pixels do display gráfico */
void Limpa_Display(void)
{
	for(unsigned char pagina = 0; pagina < 8; pagina++)       //varre as 8 páginas
	{
		for(unsigned char coluna = 0; coluna < 128; coluna++) //varre as 128 colunas
		    Escreve_Dado(0x00, pagina, coluna);               //escreve o dado
	}
}

/* Esta função escreve uma string com letras claras e fundo escuro da memória de dados */
void Escreve_S_memdados_light(char *string, unsigned char pagina, unsigned char coluna)
{
	while(*string)
	{
		while(Status_BF());       //aguarda o display estar livre
		for(unsigned char i = 0; i < 5; i++)
		    Escreve_Dado(~(pgm_read_byte(&tabela_ASCII[*string - 0x20][i])), pagina, coluna++);
		Escreve_Dado(0xFF, pagina, coluna++);
		string++;
	}
}

/* Esta função escreve uma string com letras claras e fundo escuro da memória de programa */
void Escreve_S_memprogm_light(const char *string, unsigned char pagina, unsigned char coluna)
{
	while(pgm_read_byte(string))              //percorre a string enquanto não encontrar '\0'
	{
		while(Status_BF());                   //aguarda o display estar livre
		for(unsigned char i = 0; i < 5; i++)  //escreve os 5 bytes que formarão o caractere da string
		    Escreve_Dado(~(pgm_read_byte(&tabela_ASCII[pgm_read_byte(string) - 0x20][i])), pagina, coluna++);
		Escreve_Dado(0xFF, pagina, coluna++); //escreve o espaço para o próximo caractere da string
		string++;                             //próximo caractere da string
	}
}
