/*
 * s08_e01_senha_numerica.c
 *
 * Created: 05/10/2022 15:53:33
 * Author : Matheus
 *
 * 8.1 � Elaborar um programa para um controle de acesso por senha num�rica. A
 * senha pode conter 3 d�gitos. Toda vez que uma tecla for pressionada, um pequeno
 * alto-falante deve ser acionado. Quando a senha for correta, o rel� deve ser
 * ligado por um pequeno tempo (utilize um LED de sinaliza��o). Preveja que a
 * senha possa ser alterada e salva na EEPROM. O circuito abaixo exemplifica o
 * hardware de controle.
 */ 

#include "definicoes_principais.h" //inclui biblioteca de defini��es principais
#include "teclado_hexadecimal.h"   //inclui biblioteca de fun��es do teclado matricial 4x4


//defini��es de hardware
#define RELE   PB3 //substitui RELE por PD3 na programa��o
#define LED    PB4 //substitui LED por PD4 na programa��o
#define BUZZER PB5 //substitui BUZZER por PD5 na programa��o

//declara��o de prot�tipos de fun��es
void aciona_buzzer(void);
void aciona_rele(void);

int main(void) //fun��o principal
{
	unsigned int endereco_eeprom;  //endere�o da senha
	unsigned char senha_eeprom[3] = {'1', '5', '9'};   //senha que ser� salva na eeprom;

	unsigned char senha_s[3]; //declara��o de vari�vel local n�o inicializada
	unsigned char tecla;      //declara��o de vari�vel local n�o inicializada
	unsigned char digitos;    //declara��o de vari�vel local n�o inicializada
	unsigned char senha_u[3]; //declara��o de vari�vel local n�o inicializada
	unsigned char verifica;   //declara��o de vari�vel local n�o inicializada

	DDRD = 0x0F;  //define os pinos PD0:3 como sa�das e PD4:7 como entradas
	DDRB = 0xFF;  //define todo o PORTB como sa�da
	PORTD = 0xF0; //habilita os resistores de pull-up das entradas e coloca as sa�da no n�vel baixo
	PORTB = 0x00; //coloca todo o PORTB em n�vel baixo

	UCSR0B = 0x00; //desabilita as fun��es de RX e TX dos pinos PD0 e PD1

	//l� o conte�do da mem�ria eeprom e armazena em senha_s
	endereco_eeprom = 0x0000;
	digitos = 0;
	while(digitos < 3)
	{
		__EEGET(senha_s[digitos++], endereco_eeprom++);
	}

	//verifica se a senha_s � igual � senha_eeprom
	endereco_eeprom = 0x0000; //restaura endere�o
	digitos = 0;              //restaura digitos
	verifica = 0;             //inicializa verifica
	while(digitos < 3)
	{
		if(senha_s[digitos] == senha_eeprom[digitos])
		verifica++;
		digitos++;
	}
	//se forem diferentes, uma novo senha � gravada e liga em seguida da eeprom
	if(verifica < 3)
	{
		endereco_eeprom = 0x0000; //restaura endere�o
		digitos = 0;              //restaura digitos
		while(digitos < 3)
		{
			__EEPUT(endereco_eeprom++, senha_eeprom[digitos++]);
		}
		endereco_eeprom = 0x0000; //restaura endere�o
		digitos = 0;              //restaura digitos
		while(digitos < 3)
		{
			__EEGET(senha_s[digitos++], endereco_eeprom++);
		}
	}
	
	while(1) //loop infinito
	{
		digitos = 0;         //inicializa vari�vel local
		verifica = 0;        //inicializa vari�vel local
		set_bit(PORTB, LED); //acende led para indicar que o programa est� pronto para receber a senha
		while(digitos < 3)   //recebe uma senha de tr�s digitos do usu�rio
		{
			tecla = Leitura_Teclado(); //realiza a leitura do teclado
			_delay_ms(10);             //elimina debounce
			if(tecla != '\0')          //verifica se alguma tecla foi pressinada
			{
				senha_u[digitos++] = tecla;
				aciona_buzzer();
			}
		}
		clr_bit(PORTB, LED); //apaga o led para indicar que a senha est� sendo verificada
		for(digitos = 0; digitos < 3; digitos++) //verifica se a senha informada � correta
		{
			if(senha_u[digitos] == senha_s[digitos])
			verifica++;
		}
		if(verifica == 3)    //se a senha for correta, aciona o rel�
		aciona_rele();
		else                 //se a senha for incorreta, aciona o buzzer tr�s vezes e pisca o LED
		{
			aciona_buzzer();
			cpl_bit(PORTB, LED);
			_delay_ms(50);
			aciona_buzzer();
			cpl_bit(PORTB, LED);
			_delay_ms(50);
			aciona_buzzer();
			cpl_bit(PORTB, LED);
			_delay_ms(50);
			clr_bit(PORTB, LED);
		}
	}
}

//implementando fun��es

//Esta fun��o aciona o buzzer
void aciona_buzzer(void)
{
	set_bit(PORTB, BUZZER);
	_delay_ms(5);
	clr_bit(PORTB, BUZZER);
	_delay_ms(5);
	set_bit(PORTB, BUZZER);
	_delay_ms(5);
	clr_bit(PORTB, BUZZER);
	_delay_ms(5);
	set_bit(PORTB, BUZZER);
	_delay_ms(5);
	clr_bit(PORTB, BUZZER);
	_delay_ms(5);
}

//Esta fun��o mant�m o rel� acionado por um segundo
void aciona_rele(void)
{
	set_bit(PORTB, RELE);
	_delay_ms(1000);
	clr_bit(PORTB, RELE);
	_delay_ms(10);
}
