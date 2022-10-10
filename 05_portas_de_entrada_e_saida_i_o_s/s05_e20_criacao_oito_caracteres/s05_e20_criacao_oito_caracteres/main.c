/*
 * s05_e20_criacao_oito_caracteres.c
 *
 * Created: 21/09/2022 17:04:35
 * Author : Matheus
 *
 * 5.20 – Crie oito caracteres novos para o LCD 16 × 2. Comece a criar seu próprio
 * conjunto de funções.
 */ 

#include "definicoes_principais.h" //biblioteca de definições principais
#include "LCD_4bits.h"             //biblioteca de funções do display de LCD com interface de 4 bits

//definição de caracteres
const unsigned char novos_caracteres[] PROGMEM =
{
	0b00000110, 0b00000101, 0b00000110, 0b00010101, 0b00001110, 0b00001110, 0b00010101, //Chi Rho
	0b00010110,	0b00001011, 0b00001001, 0b00001011, 0b00001110, 0b00001000, 0b00010110, //Palmeiras
	0b00000001, 0b00000100, 0b00011100, 0b00011101, 0b00011100, 0b00000100, 0b00000001, //Buzzer
	0b00001110, 0b00010001, 0b00010001, 0b00010001, 0b00001010, 0b00001110, 0b00000100, //Lâmpada acesa
	0b00001110, 0b00011111, 0b00011111, 0b00011111, 0b00001110, 0b00001110, 0b00000100, //Lâmpada apagada
	0b00001110, 0b00011011, 0b00010001, 0b00010001, 0b00010001, 0b00010001, 0b00011111, //Bateria 0
	//0b00001110, 0b00011011, 0b00010001, 0b00010001, 0b00010001, 0b00011111, 0b00011111, //Bateria 20
	//0b00001110, 0b00011011, 0b00010001, 0b00010001, 0b00011111, 0b00011111, 0b00011111, //Bateria 40
	0b00001110, 0b00011011, 0b00010001, 0b00011111, 0b00011111, 0b00011111, 0b00011111, //Bateria 60
	//0b00001110, 0b00011011, 0b00011111, 0b00011111, 0b00011111, 0b00011111, 0b00011111, //Bateria 80
	0b00001110, 0b00011111, 0b00011111, 0b00011111, 0b00011111, 0b00011111, 0b00011111  //Bateria 100
};

int main(void) //função principal
{
	unsigned char c; //declaração de variável auxiliar

	DDRD = 0xFF; //define todo o PORTD como saída
	DDRB = 0xFF; //define todo o PORTB como saída
	
	PORTD = 0x00; //coloca todo o PORTD em nível baixo
	PORTB = 0x00; //coloca todo o PORTB em nível baixo

	UCSR0B = 0x00; //desabilita os pinos RX e Tx do Arduino UNO

	Inicializa_LCD(LCD_2_N);                   //inicializa o display LCD com duas linhas
	Cria_Caracteres_memprogm(novos_caracteres); //cria caracteres
	Limpa_Display();                           //limpa a tela do display
	Cursor_Home();                             //posiciona o cursor na primeira coluna da primeira linha

	Escreve_S_memdados("Cacacteres novos");
	Posiciona_Cursor(2, 1);

	for(c = 0; c < 8; c++)
	{
		Escreve_Dado(c);    //escreve caractere criado
		while(Status_BF());
		Escreve_Dado(' ');
		while(Status_BF());
	}

	while(1);
}
