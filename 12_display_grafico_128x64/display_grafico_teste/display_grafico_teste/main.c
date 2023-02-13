/*
 * display_grafico_teste.c
 *
 * Created: 30/01/2023 16:19:28
 * Author : Matheus
 */ 

#include "definicoes_principais.h"
#include "display_grafico_128x64.h"
#include "ryu.h"


const char teste_memprog[] PROGMEM = "Teste!\0";
const char str_memprog[] PROGMEM = "memoria de programa!\0";

int main() //fun��o principal
{
	DDRD = 0xFF;   //configura todo o PORTD como sa�da
	DDRB = 0xFF;   //configura todo o PORTB como sa�da
	DDRC = 0xFF;   //configura todo o PORTC como sa�da
	UCSR0B = 0x00; //configura os pinos PD0 e PD1 como I/Os desabilitando as fun��es de RX e TX

	Inicializacao_Display(); //Inicializa o display gr�fico
	Testa_Display();         //liga todos os pixels do display
	_delay_ms(500);
	Limpa_Display();         //limpa a tela do display
	_delay_ms(500);
	Escreve_S_memdados("Display pronto", 0, 0); //exibe uma mensagem
	Escreve_S_memdados("Teste!", 1, 0);
	Escreve_S_memprogm(teste_memprog, 2, 0);
	_delay_ms(1000);

	while(1) //loop infinito
	{
		for(unsigned char frame = 0; frame < 6; frame++)         //percorre o ponteiro que aponta para os vetores que cont�m as imagens
		{
			Imprime_Imagem((PGM_P)pgm_read_word(&p_ryu[frame])); //imprime a imagem 'frame' na tela do display gr�fico
			_delay_ms(3);                                        //delay de 3ms para o efeito de persist�ncia da vis�o
		}
		
		char dado_lido;	                          //vari�vel para ler um byte	
		while(Status_BF());                       //aguarda o display estar livre
		Limpa_Display();                          //limpa a tela do display
		while(Status_BF());                       //aguarda o display estar livre
		Escreve_Dado(0xFF, 0x00, 0x00);           //escreve '0xFF' na coluna 0 da p�gina 0
		for(unsigned char i = 0; i < 7; i++)      //l� o byte das colunas 0 at� 6
		{		
		    while(Status_BF());                   //aguarda o display estar livre
		    dado_lido = Leitura_Dado(0x00, i);    //l� o byte da coluna 'i' da p�gina 0
		    dado_lido = dado_lido << 1;           //desloca o byte um bit para a esquerda
		    while(Status_BF());                   //aguarda o display estar livre
		    Escreve_Dado(dado_lido, 0x00, i + 1); //ecreve o byte lido em 'i' em 'i + 1'
	    }
		
		Testa_Display();
		Escreve_S_memdados_light("memoria de dados!", 0x00, 0x00);
		Escreve_S_memprogm_light(str_memprog, 0x01, 0x00);
			
		while(1);
	}
}
