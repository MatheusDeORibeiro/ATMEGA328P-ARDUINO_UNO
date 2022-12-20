/*
 * LCD_4bits.c
 *
 * Created: 15/09/2022 16:42:37
 *  Author: Matheus de Oliveira Ribeiro
 */ 

/*
 * Esta biblioteca contém funções para a utilização de displays LCD 16x2 na interface de 4bits
 * compatível os LCD controllers HD44780, KS0066U, SED1278 e ST7066.
 */


#include "LCD_4bits.h"             //inclui a biblioteca de funções do display LCD 16x2 4bits
#include "definicoes_principais.h" //inclui a biblioteca de definições principais

//implementação das funções

/* Esta função envia um comando para o barramento de dados do display */
void Escreve_CMD(unsigned char comando)
{
	//4 bits mais significativos
	#if (_slc_nibble) 
	    PORT_DADOS = ((PORT_DADOS & 0x0F) | (comando & 0xF0));        //msn
	#else 
	    PORT_DADOS = ((PORT_DADOS & 0xF0) | ((comando >> 4) & 0x0F)); //lsn
	#endif
	Pulso_Enable();
	//4 bits menos significativos
	#if (_slc_nibble)
	    PORT_DADOS = ((PORT_DADOS & 0x0F) | ((comando << 4) & 0xF0)); //msn
	#else 
	    PORT_DADOS = ((PORT_DADOS & 0xF0) | (comando & 0x0F));        //lsn
	#endif
	Pulso_Enable();
}

/* Esta escreve uma instrução para o display */
void Escreve_Instrucao(unsigned char instrucao)
{
    #if (_slc_nibble)
	    DIR_DADOS |= 0xF0; //configura o ms nibble do barramento de dados como saída
    #else 
	    DIR_DADOS |= 0x0F; //configura o ls nibble do barramento de dados como saída
	#endif
	//seleciona operação
	clr_bit(PORT_CONT, _RS); //instrução
	clr_bit(PORT_CONT, _RW); //escrita
	Escreve_CMD(instrucao);
	_delay_us(45);           //aguarda o tempo de execução da instrução
	clr_bit(PORT_CONT, _RW);          
	#if (_slc_nibble)
	    DIR_DADOS &= 0x0F; //configura o ms nibble do barramento de dados como entrada
    #else 
	    DIR_DADOS &= 0xF0; //configura o ls nibble do barramento de dados como entrada
    #endif
}

/* Esta função retorna o status do busy flag: 1 - ocupado e 0 - livre */
unsigned char Status_BF(void)
{
	unsigned char leitura_bf;    //variável auxiliar
	clr_bit(PORT_CONT, _RS);
	set_bit(PORT_CONT, _RW);
	_delay_us(1);
	set_bit(PORT_CONT, _EN);
	_delay_us(1);
	#if (_slc_nibble)
    	leitura_bf = LE_DADOS & 0xF0;        //carrega o nibble mais significativo
	#else
	    leitura_bf = (LE_DADOS & 0x0F) << 4; //carrega o nibble menos significativo
	#endif
	clr_bit(PORT_CONT, _EN);
	_delay_us(1);
	set_bit(PORT_CONT, _EN);
	_delay_us(1);
	//leitura_bf = (leitura_bf << 4) | (PIND & 0xF0); //carrega o nibble menos significativo
	if(leitura_bf & 0x80)        //se o bit 7 for igual a 1, o display está ocupado
	{
		clr_bit(PORT_CONT, _EN);
		clr_bit(PORT_CONT, _RW);
		return 1;                //ocupado
	}
	else                         //se não, o display está livre
	{
		clr_bit(PORT_CONT, _EN);
		clr_bit(PORT_CONT, _RW);
		return 0;                //livre
	}
}

/* Esta função escreve um dado no display */
void Escreve_Dado(unsigned char dado)
{
	#if (_slc_nibble)
	    DIR_DADOS |= 0xF0; //configura o ms nibble do barramento de dados como saída
	#else 
	    DIR_DADOS |= 0x0F; //configura o ls nibble do barramento de dados como saída
	#endif
	//seleciiona operação
	set_bit(PORT_CONT, _RS);   //dado
	clr_bit(PORT_CONT, _RW);   //escrita
	Escreve_CMD(dado);
	clr_bit(PORT_CONT, _RS);
	_delay_us(45);             //aguarda o tempo de execução da instrução
	#if (_slc_nibble)
	    DIR_DADOS &= 0x0F; //configura o ms nibble do barramento de dados como entrada
	#else 
	    DIR_DADOS &= 0xF0; //configura o ls nibble do barramento de dados como entrada
	#endif
}

/* Esta função lê um dado do display */
unsigned char Leitura_Dado(void)
{
	unsigned char leitura;                    //variável auxiliar
	set_bit(PORT_CONT, _RS);                  //dado
	set_bit(PORT_CONT, _RW);                  //instrução
	_delay_ms(1);
	set_bit(PORT_CONT, _EN);
	_delay_ms(1);
	#if (_slc_nibble)
    	leitura = LE_DADOS & 0xF0; //carrega o ms nibble
    #else
	    leitura = LE_DADOS & 0x0F; //carrega o ls nibble
	#endif
	clr_bit(PORT_CONT, _EN);
	_delay_ms(1);
	set_bit(PORT_CONT, _EN);
	_delay_ms(1);
	#if (_slc_nibble)
    	leitura = leitura | ((LE_DADOS & 0xF0) >> 4); //carrega o ms nibble
	#else
	    leitura = (leitura << 4) | (LE_DADOS & 0x0F); //carrega o ls nibble
	#endif
	clr_bit(PORT_CONT, _EN);
	_delay_us(45);                            //aguarda tempo de execução da isntrução
	clr_bit(PORT_CONT, _RS);
	clr_bit(PORT_CONT, _RW);
	return leitura;                           //retorna o dado lido
}

/* Esta função envia um pulso para o pino Enable */
void Pulso_Enable(void)
{
	_delay_us(2);
	set_bit(PORT_CONT, _EN);
	_delay_us(2);
	clr_bit(PORT_CONT, _EN);
}

/* Esta função inicializa o display com a interface de 4 bits */
void Inicializa_LCD(unsigned char numero_linhas)
{
	unsigned char i;
	unsigned char ajuste[3] = {0x0F, 0x01, 0x06};
                  //display ON e cursor piscante, limpa display e deslocamento para a direita
	
	//configuração de hardware
	clr_bit(PORT_CONT, _RS);
	clr_bit(PORT_CONT, _RW);
	clr_bit(PORT_CONT, _EN);	
	#if (_slc_nibble)
	    DIR_DADOS |= 0xF0; //configura o ms nibble do barramento de dados como saída
	#else 
	    DIR_DADOS |= 0x0F; //configura o ls nibble do barramento de dados como saída
	#endif
	DIR_CONT  |= 0x07;       //configura o barramento de controle como saída
	
	_delay_ms(15);           //aguarda 15ms após Vcc atingir no mínimo 4,5V
	
	//ajuste de função
	for(i = 0; i < 3; i++)
	{
		#if (_slc_nibble)
		PORT_DADOS = ((PORT_DADOS & 0x0F) | (0x30 & 0xF0));        //msn
		#else
		PORT_DADOS = ((PORT_DADOS & 0xF0) | ((0x30 >> 4) & 0x0F)); //lsn
		#endif
		Pulso_Enable();
		_delay_ms(5);
	}
	
	//ajuste de função
	#if (_slc_nibble)
	PORT_DADOS = ((PORT_DADOS & 0x0F) | (0x20 & 0xF0));        //msn
	#else
	PORT_DADOS = ((PORT_DADOS & 0xF0) | ((0x20 >> 4) & 0x0F)); //lsn
	#endif
	Pulso_Enable();
	_delay_us(40);
	
	//ajuste de função - 4bits e 5x8 pontos
	if(numero_linhas) 
	{
		Escreve_CMD(0x28);
	}
	else
	{
		Escreve_CMD(0x20);
	}
	_delay_ms(5);
	
	//configurações do vetor ajustes
	for(i = 0; i < 3; i++)
	{
		Escreve_CMD(ajuste[i]);
		clr_bit(PORT_CONT, _EN);
		_delay_ms(2);
	}
	
	#if (_slc_nibble) 
	    DIR_DADOS &= 0x0F; //configura o ms nibble do barramento de dados como entrada
	#else 
	    DIR_DADOS &= 0xF0; //configura o ls nibble do barramento de dados como entrada
    #endif
}

/* Esta função limpa a tela do display */
void Limpa_Display(void)
{
	Escreve_Instrucao(0x01);
	//_delay_ms(2);
	while(Status_BF());
}

/* Esta função posiciona o cursor na primeira coluna da primeira linha */
void Cursor_Home(void)
{
	Escreve_Instrucao(0x02);
	_delay_ms(2);
	//while(Status_BF());
}

/* Esta função liga/desliga o display e/ou o cursor */
void Display_Cursor_ON_OFF(unsigned char display_cursor)
{
	Escreve_Instrucao(display_cursor);
	while(Status_BF());
}

/* Esta função desloca o display/cursor */
void Desloca_Mensagem(unsigned char deslocamento)
{
	Escreve_Instrucao(deslocamento);
	while(Status_BF());
}

/* Esta função posiciona o cursor em uma determinda coluna de determinada linha */
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

/* Esta função testa o display ligando todos os seus pixels */
void Testa_Display(void)
{
	unsigned char i; //variável auxiliar
	
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

/* Esta função escreve uma string da memória de dados */
void Escreve_S_memdados(char *string)
{
	while(*string) //percorre a string até encontrar o cractere nulo
	{
		while(Status_BF());
		Escreve_Dado(*string++); //envia caractere
	}
}

/* Esta função escreve uma string da memória de programa */
void Escreve_S_memprogm(const unsigned char *string)
{
	while(pgm_read_byte(&(*string))) //percorre a string até encontrar o cractere nulo
	{
		while(Status_BF());
		Escreve_Dado(pgm_read_byte(&(*string++))); //envia um caractere
	}
}

/* Esta função cria oito caracteres da memória de dados */
void Cria_Caracteres_memdados(char *caracteres)
{
	unsigned char i, j, k = 0, end_CGRAM = 0x40; //declaração de variáveis
	for(i = 0; i < 8; i++)
	{
		Escreve_Instrucao(end_CGRAM);
		for(j = 0; j < 7; j++)
		{
			Escreve_Dado(caracteres[k + j]);
		}
		Escreve_Dado(0x00);
		k += 7;         //próximos caractere armazenados no vetor
		end_CGRAM += 8; //endereço do próximo caractere
	}
}

/* Esta função cria oito caracteres da memória de programa */
void Cria_Caracteres_memprogm(const unsigned char *caracteres)
{
	unsigned char i, j, k = 0, end_CGRAM = 0x40; //declaração de variáveis
	for(i = 0; i < 8; i++)
	{
		Escreve_Instrucao(end_CGRAM);
		for(j = 0; j < 7; j++)
		{
			Escreve_Dado(pgm_read_byte(&caracteres[k + j]));
		}
		Escreve_Dado(0x00);
		k += 7;         //próximo caractere armazenados no vetor
		end_CGRAM += 8; //endereço do próximo caractere
	}
}

/* Esta função armazena os digitos individuais de um número inteiro em um vetor */
void Decodifica_Dig_Ind(unsigned int numero, unsigned char *digitos)
{
	unsigned char n;                             //declaração de variável auxiliar
	for(n = 0; n < _tam_vetor; n++)
	    digitos[n] = 0 + _valor_decod;           //limpa o vetor
	do
	{
		*digitos = (numero % 10) + _valor_decod; //armazena o resto da divisão
		numero /= 10;                            //divide o valor por 10
		digitos++;
	}while(numero != 0);                         //decodifica enquanto o número for diferente de 0
}
