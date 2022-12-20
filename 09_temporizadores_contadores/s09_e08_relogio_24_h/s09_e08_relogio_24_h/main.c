/*
 * s09_e08_relogio_24_h.c
 *
 * Created: 19/10/2022 17:37:15
 * Author : Matheus
 *
 * 9.8 – Elaborar um programa para que o hardware da fig. 9.8 funcione como um
 * relógio 24 h. O ajuste do horário deve ser feito nos botões específicos.
 */ 

#include "definicoes_principais.h" //inclui as definições principais
#include "timers.h"                //inclui a biblioteca de funções dos TC
#include "LCD_4bits.h"             //inclui a biblioteca de funções do display LCD com interface de 4 bits


//definições de hardware
#define AJUSTE PC3 //substitui AJUSTE por PC3 na programação
#define HORA   PC4 //substitui HORA por PC4 na programação
#define MIN    PC5 //substitui MIN por PC5 na programação

//declaração de variáveis globais
volatile unsigned char segundos = 0, minutos = 0, horas = 0;

//função para exibir o relógio
void relogio_24h(void);

//função de tratamento de interrupção
ISR(TIMER2_OVF_vect);


int main(void) //função principal
{
	DDRD = 0xFF;  //configura todo o PORTD como saída
	DDRC = 0xC7;  //configura os pinos PC5:3 como entrada e os pinos restantes como saída
	DDRB = 0xFF;  //configura todo o PORTB como saída
	PORTD = 0x00; //coloca todo o PORTD em nível baixo
	PORTC = 0x38; //habilita os resistores de pull-up dos pinos PC5:3 e coloca os pinos restantes em nível baixo
	PORTB = 0x00; //coloca todo o PORTB em nível baixo

	Inicializa_LCD(LCD_1_N); //inicializa o dispay LCD com uma linha
	Display_Cursor_ON_OFF(LCD_DSP_ON_CS_OFF); //mantém a tela do display ligada e o cursor desligado
	Limpa_Display(); //limpa a tela do display
	Cursor_Home();   //posiciona o cursor na primeira coluna da primeira linha
	
	Inicializa_TC2(TC2_modo_assincrono, TC2_CLK_128, TC2_TOIE2);
	/* modo assíncrono, f_osc igual a 32768Hz, prescaler igual a 128 */
	/* t_estouro = 1s */

	sei(); //habilita todas as fontes de interrupção
	
	while(1) //loop infinito
	{
		relogio_24h();                      //exibe o relógio
		if(!tst_bit(PINC, AJUSTE))          //botão de ajuste pressionado?
		{
			_delay_ms(10);                  //elimina ruído
			cli();                          //desabilita todas as fontes de interrupções
		}
		while(!tst_bit(PINC, AJUSTE))       //se o botão for mantido pressionado
		{
			if(!tst_bit(PINC, HORA))        //botão de ajuste da hora pressionado?
			{
				while(tst_bit(PINC, HORA)); //aguarda o botão ser solto
				_delay_ms(10);              //elimina ruído
				horas++;
				if(horas == 25) horas = 0;
			}
			if(!tst_bit(PINC, MIN))         //botão de ajuste de minuto pressionado?
			{
				while(tst_bit(PINC, MIN));  //aguarda o botão ser solto
				_delay_ms(10);              //elimina ruído
				minutos++;                  //incrementa minutos
				if(minutos == 60) minutos = 0;
			}
			relogio_24h();                  //exibe o relógio
			if(tst_bit(PINC, AJUSTE))           //botão de ajuste solto?
		    {
			    _delay_ms(10);                  //habilita todoas as fontes de interrupção
			    sei();
				segundos = 0;
		    }
		}		
	}
}

//implementando função para exibir o relógio
void relogio_24h(void)
{
	//declaração de variável auxiliar
	unsigned char aux;

	Limpa_Display();        //limpa a tela do display
	Posiciona_Cursor(1, 5); //posiciona o cursor na quinta coluna da primeira linha
	if(horas < 10)          //para exibir as horas
	{
		Escreve_Dado('0');
		Escreve_Dado((horas + _valor_decod));
	}
	else
	{
		aux = (horas / 10) + _valor_decod;
		Escreve_Dado(aux);
		aux = (horas % 10) + _valor_decod;
		Escreve_Dado(aux);
	}
	Escreve_Dado(':');
	if(minutos < 10)        //para exibir os minutos
	{
		Escreve_Dado('0');
		Escreve_Dado((minutos + _valor_decod));
	}
	else
	{
		aux = (minutos / 10) + _valor_decod;
		Escreve_Dado(aux);
		aux = (minutos % 10) + _valor_decod;
		Escreve_Dado(aux);
	}
	Escreve_Dado(':');
	if(segundos < 10)       //para exibir os segundos
	{
		Escreve_Dado('0');
		Escreve_Dado((segundos + _valor_decod));
	}
	else
	{
		aux = (segundos / 10) + _valor_decod;
		Escreve_Dado(aux);
		aux = (segundos % 10) + _valor_decod;
		Escreve_Dado(aux);
	}
}

//impementação da função de tratamento de interrupção
ISR(TIMER2_OVF_vect)
{
	segundos++;             //incrementa os segundos
	if(segundos == 60)      //se passou um minuto?
	{
		segundos = 0;
		minutos++;          //incrementa minutos
		if(minutos == 60)   //se passou uma hora?
		{
			minutos = 0;
			horas++;        //incrementa horas
			if(horas == 24) //se passou um dia?
			{
				segundos = 0;
				minutos = 0;
				horas = 0;
			}
		}
	}
}
