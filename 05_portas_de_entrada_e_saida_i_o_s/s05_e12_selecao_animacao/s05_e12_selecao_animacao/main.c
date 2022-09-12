/*
 * s05_e12_selecao_animacao.c
 *
 * Created: 24/08/2022 17:56:06
 * Author : Matheus
 * 5.12 � No exerc�cio 5.8, foram propostas 7 anima��es com 8 LEDs. Crie outra
 * anima��o, totalizando 8. Depois empregue dois bot�es: um ser� o AJUSTE, que
 * quando pressionado permitir� que o outro bot�o (SELE��O) selecione a fun��o
 * desejada, ver a fig. 5.11. Cada vez que o bot�o SELE��O for pressionado, um
 * dos oito LEDs dever� acender para indicar a fun��o escolhida; exemplo:
 * 00000100 => LED 3 ligado, fun��o 3 selecionada. Quando o bot�o de AJUSTE for
 * solto, o sistema come�a a funcionar conforme a fun��o escolhida.
 * Desenvolva a programa��o por partes, unindo-as e testando com cuidado. N�o
 * esque�a: um bom programa � bem comentado e organizado!
 */ 

#define F_CPU 16000000UL  //define frequ�ncia do oscilador de 16MHz

#include <avr/io.h>
#include <util/delay.h>

//defini��es de macros
#define set_bit(Y, bit_x) (Y |= (1 << bit_x)  //seta o bit x do PORTY
#define clr_bit(Y, bit_x) (Y &= ~(1 << bit_x) //limpa o bit x do PORTY
#define cpl_bit(Y, bit_x) (Y ^= (1 << bit_x)  //complementa o bit x do PORTY
#define tst_bit(Y, bit_x) (Y & (1 << bit_x))  //testa o bit x do PORTY

#define AJUSTE  PB0 //AJUSTE � substitu�do por PB0 na programa��o
#define SELECAO PB1 //SELECAO � substitu�do por PB1 na programa��o

//declara��es de prot�tipos de fun��es
void atraso_10ms(void);
void atraso_250ms(void);
void atraso_500ms(void);
void animacao_1(void);
void animacao_2(void);
void animacao_3(void);
void animacao_4(void);
void animacao_5(void);
void animacao_6(void);
void animacao_7(void);
void animacao_8(void);

int main()
{
	DDRD = 0xFF;        //todo o PORTD � configurado como sa�da
	DDRB = 0b11111100;  //os pinos PB0 e PB1 do PORTB s�o configurados como entrada e os demais como sa�da
	PORTB = 0b00000011; //habilita os resistores pull-up das entradas
	UCSR0B = 0x00;      //desabilita RXD e TXD

	unsigned char animacao = 0; //vari�vel local inicializada

	while(1)
	{
		while(!tst_bit(PINB, AJUSTE))   //verifica se o bot�o AJUSTE foi pressionado
		{
			PORTD = animacao;           //acende led correspondente a anima��o selecionada
			if(!tst_bit(PINB, SELECAO)) //verifica se o bot�o SELECAO foi pressionado
			{
				while(!tst_bit(PINB, SELECAO)); //verifica se o bot�o SELECAO foi solto
				atraso_10ms();                  //atraso de 10ms para eliminar ru�do do bot�o
				animacao <<= 1;                 //seleciona a pr�xima anima��o
				if(animacao == 0) animacao = 1;
			} //teste do bot�o SELECAO
		} //teste do bot�o AJUSTE
		PORTD = 0x00;        //desliga todos os leds
		atraso_500ms();      //atraso de 500ms
		switch (animacao)    //verifica qual anima��o foi selecionada
		{
			case 0x01:
			animacao_1();    //anima��o 1
			break;
			case 0x02:
			animacao_2();    //anima��o 2
			break;
			case 0x04:
			animacao_3();    //anima��o 3
			break;
			case 0x08:
			animacao_4();    //anima��o 4
			break;
			case 0x10:
			animacao_5();    //anima��o 5
			break;
			case 0x20:
			animacao_6();    //anima��o 6
			break;
			case 0x40:
			animacao_7();    //anima��o 7
			break;
			case 0x80:
			animacao_8();    //anima��o 8
			break;
			default:
			PORTD = 0x00;    //caso nenhuma anima��o esteja selecionada, desliga todos os leds
			break;
		} //teste anima��o
		PORTD = 0x00;       //desliga todos os leds
		animacao = 0;       //atribui novo valor � vari�vel animacao
	} //la�o infinito
}

//implementa��o das fun��es

//fun��es de tempo

void atraso_10ms(void)
{
	_delay_ms(10);    //atraso de 10ms
}

void atraso_250ms(void)
{
	_delay_ms(250);    //atraso de 250ms
}

void atraso_500ms(void)
{
	_delay_ms(500);    //atraso de 500ms
}

//fun��es de anima��es

//sequencial de leds deslocando da direita para a esquerda
void animacao_1(void)
{
	unsigned char cont = 0x01;  //vari�vel local inicializada
	PORTD = cont;               //liga leds
	atraso_500ms();             //atraso de 500ms
	while(1)
	{
		cont <<= 1;             //deslocamento l�gico para a esquerda
		PORTD = cont;           //liga leds
		atraso_500ms();         //atraso de 500ms
		if(cont == 0x00) break; //sai ao completar o deslocamento
	}
}


//sequencial de leds deslocando da esquerda para a direita
void animacao_2(void)
{
	unsigned char cont = 0x80;  //vari�vel local inicializada
	PORTD = cont;               //liga ledas
	atraso_500ms();             //atraso de 500ms
	while(1)
	{
		PORTD = cont;           //liga ledas
		cont >>= 1;             //deslocamento l�gico para a direita
		atraso_500ms();         //atraso de 500ms
		if(cont == 0x00) break; //sa� ao completar o deslocamento
	}
}


//ligar os leds sequencialmente da esquerda para a direita (vai-e-volta)
void animacao_3(void)
{
	unsigned char cont = 0x80;  //vari�vel local inicializada
	while(1)
	{
		PORTD = cont;           //liga leds
		atraso_500ms();         //atraso de 500ms
		cont >>= 1;             //deslocamento l�gico para a direita
		if(cont == 0x00) break; //sai ao completar o deslocamento
	} //vai
	cont = 0x02;
	while(1)
	{
		PORTD = cont;           //liga leds
		atraso_500ms();         //atraso de 500ms
		cont <<= 1;             //deslocamento l�gico para a esquerda
		if(cont == 0x80) break; //sai ao completar o deslocamento
	} //volta
	PORTD = cont;               //liga leds
	atraso_500ms();             //atraso de 500ms
}


//ligar todos os leds e apagar sequencialmente da direita para a esquerda (vai-e-volta)
void animacao_4(void)
{
	unsigned char cont = 0xFF;  //vari�vel local inicializada
	while(1)
	{
		PORTD = cont;           //liga leds
		atraso_500ms();         //L de 500ms
		cont <<= 1;             //deslocamento l�gico para a esquerda
		if(cont == 0x00) break; //sai ao apagar todos os leds
	} //vai
	PORTD = cont;
	atraso_500ms();             //atraso de 500ms
	cont = 0xFF;
	while(1)
	{
		PORTD = cont;           //liga leds
		atraso_500ms();         //atraso de 500ms
		cont >>= 1;             //deslocamento l�gico para a direita
		if(cont == 0x00) break; //sai ao apagar todos os leds
	} //volta
	PORTD = cont;               //liga leds
	atraso_500ms();             //atraso de 500ms
}

//ligar leds sequencialmente da direita para a esquerda um de cada vez mantendo o anterior aceso
void animacao_5(void)
{
	unsigned char cont = 0x00;  //vari�vel local inicializada
	PORTD = cont;               //liga leds
	atraso_500ms();             //atraso de 500ms
	while(1)
	{
		cont = (cont << 1) + 1; //deslocamento para a esquerda e incremento
		PORTD = cont;           //liga leds
		atraso_500ms();         //atraso de 500ms
		if(cont == 0xFF) break; //sai se todos os led estiverem acesos
	}
}

//ligar leds sequencialmente da esquerda para a direita um de cada vez mantendo o anterior aceso
void animacao_6(void)
{
	unsigned char cont = 0x00;  //vari�vel local inicializada
	PORTD = cont;               //liga leds
	atraso_500ms();             //atraso de 500ms
	while(1)
	{
		cont = (cont >> 1) + 0x80; //deslocamento para a direita e incremento de 0x80
		PORTD = cont;               //liga leds
		atraso_500ms();             //atraso de 500ms
		if(cont == 0xFF) break; //sai se todos os led estiverem acesos
	}
}

//contagem bin�ria crescente de 0 - 255
void animacao_7(void)
{
	unsigned char cont = 0; //vari�vel local inicializada
	while(cont < 256)
	{
		PORTD = cont;      //liga leds
		atraso_250ms();    //atraso de 250ms
		cont++;            //incrementa contador
	}
}

//contagem bin�ria decrescente de 255 - 0
void animacao_8(void)
{
	unsigned char cont = 255; //vari�vel local inicializada
	while(cont >= 0)
	{
		PORTD = cont;         //liga leds
		atraso_250ms();       //atraso de 250ms
		cont--;               //decrementa vari�vel
	}
}
