/*
 * s05_e21_cronometro.c
 *
 * Created: 22/09/2022 14:44:12
 * Author : Matheus
 *
 * 5.21 – Usando as duas linhas do LCD, crie um cronômetro com passo de 1 s. Utilize
 * os números grandes (4 caracteres por digito) e o pino PB2 do ATmega para o
 * início/parada. Essa ideia é exemplificada na fig. 5.30.
 */ 

#include "definicoes_principais.h" //biblioteca de definições principais
#include "LCD_4bits.h"             //biblioteca de funções do display LCD 16x2 com interface de 4 bits


//definição de hardware
#define BOTAO PB3 //subistitui BOTAO por PB3 na programação

//definição de vetor contendo novos caracteres armazenado na memória de programa
const unsigned char novos_caracteres[] PROGMEM = {
	0b00011111, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, //0x00
	0b00001111, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, //0x01
	0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00011111, //0x02
	0b00011111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00011111, //0x03
	0b00011111, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00011111, //0x04
	0b00011111, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00010000, 0b00011111, //0x05
	0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000001, //0x06
	0b00011111, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000  //0x07
};

//definição de vetor contendo números grandes armazenado na memória de programa
const unsigned char numeros_grandes[10][4] PROGMEM = {
	{0x00, 0x01, 0x4C, 0x02}, //0
	{0x20, 0x7C, 0x20, 0x7C}, //1
	{0x03, 0x04, 0x4C, 0x5F}, //2
	{0x03, 0x04, 0x5F, 0x02}, //3
	{0x4C, 0x02, 0x20, 0x06}, //4
	{0x05, 0x03, 0x5F, 0x02}, //5
	{0x05, 0x03, 0x4C, 0x02}, //6
	{0x07, 0x01, 0x20, 0x06}, //7
	{0x05, 0x04, 0x4C, 0x02}, //8
	{0x05, 0x04, 0x20, 0x06}  //9
};

//declaração de variável global
unsigned char inicia;

//protótipos de funções
void status_botao(void);
void Escreve_Numero_Grande(unsigned char numero, unsigned char coluna);

int main(void) //função principal
{
	inicia = 0; //inicializa a variável global
	unsigned char hora = 0, minuto = 0, segundo = 0, dezena, unidade; //variáveis locais inicializadas

	DDRD = 0xFF; //configura todo o PORTD como saída
	DDRB = 0xF7; //configura o pino 3 do PORTB como entrada e os pinos restantes como saída

	PORTD = 0x00; //coloca todo o PORTD em nível baixo
	PORTB = 0x08; //habilita o resistor pull-up da entrada e coloca os pinos restantes em nível baixo

	Inicializa_LCD(LCD_2_N);                    //inicializa o dispay com duas linhas
	Display_Cursor_ON_OFF(LCD_DSP_ON_CS_OFF);   //mantém a tela dos dispay ligada e desliga o cursor
	Cria_Caracteres_memprogm(novos_caracteres); //armazena os caracteres na memória do display
	Limpa_Display();                            //limpa a tela do display
	Cursor_Home();                              //posiciona o cursor na primeira coluna da primeira linha
	Posiciona_Cursor(1, 4);                     //posiciona o cursor na quarta coluna da primeira linha
	Escreve_S_memdados("Cronometro");           //escrve uma string
	Posiciona_Cursor(2, 1);                     //posicione o cursor na primeira coluna da segunda linha
	Escreve_S_memdados("Pressione botao!");     //escreve uma string
	
	while(1) //loop infinito
	{
		hora = 0, minuto = 0, segundo = 0; //zera o cronômetro
		status_botao();                    //verifica status do botão

		while(inicia) //inicia a contagem com o cronômetro zerado
		{
			Limpa_Display(); //limpa a tela do display
			Cursor_Home();   //posiciona o cursor na primeira coluna da primeira linha
			unidade = hora % 10;
			dezena = (unsigned char)((hora - unidade) / 10);

			Escreve_Numero_Grande(dezena, 1); //escreve as horas
			Escreve_Numero_Grande(unidade, 3);
			Posiciona_Cursor(1, 5);
			Escreve_Dado(0xA5); //ponto
			Posiciona_Cursor(2, 5);
			Escreve_Dado(0xA5); //ponto

			unidade = minuto % 10;
			dezena = (unsigned char)((minuto - unidade) / 10);

			Escreve_Numero_Grande(dezena, 6); //escreve os minutos
			Escreve_Numero_Grande(unidade, 8);
			Posiciona_Cursor(1, 10);
			Escreve_Dado(0xA5); //ponto
			Posiciona_Cursor(2, 10);
			Escreve_Dado(0xA5); //ponto

			unidade = segundo % 10;
			dezena = (unsigned char)((segundo - unidade) / 10);

			Escreve_Numero_Grande(dezena, 11);  //escreve os segundos
			Escreve_Numero_Grande(unidade, 13);
			Posiciona_Cursor(2, 15);
			Escreve_Dado('s');

			_delay_ms(1000); //delay de um segundo

			segundo++;
			if(segundo == 60)
			{
				segundo = 0;
				minuto++;
			}
			if(minuto == 60)
			{
				minuto = 0;
				hora++;
			}
			if(hora == 99)
			{
				hora = 0;
				minuto = 0;
				segundo = 0;
			}

			status_botao(); //verifica status do botão
		}
	}
}

//implementando funções

/* Esta função testa o status do botão e complementa o valor da variável inicia */
void status_botao(void)
{
	if(!tst_bit(PINB, BOTAO))         //verifica se o botão foi pressionado
	{
		while(!tst_bit(PINB, BOTAO)); //aguarda botão ser solto
		_delay_ms(10);                //atraso para eliminar ruídos
		inicia = !inicia;             //complementa o valor da variável inicia
	}
}

/* Esta função escreve um número grande */
void Escreve_Numero_Grande(unsigned char numero, unsigned char coluna)
{
	Posiciona_Cursor(1, coluna); //posicina o cursor na coluna da primeira linha
	Escreve_Dado(pgm_read_byte(&numeros_grandes[numero][0]));
	Escreve_Dado(pgm_read_byte(&numeros_grandes[numero][1]));

	Posiciona_Cursor(2, coluna); //posicina o cursor na coluna da segunda linha
	Escreve_Dado(pgm_read_byte(&numeros_grandes[numero][2]));
	Escreve_Dado(pgm_read_byte(&numeros_grandes[numero][3]));
}
