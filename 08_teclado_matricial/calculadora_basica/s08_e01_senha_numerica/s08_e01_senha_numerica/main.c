/*
 * s08_e01_senha_numerica.c
 *
 * Created: 05/10/2022 15:53:33
 * Author : Matheus
 *
 * 8.1 – Elaborar um programa para um controle de acesso por senha numérica. A
 * senha pode conter 3 dígitos. Toda vez que uma tecla for pressionada, um pequeno
 * alto-falante deve ser acionado. Quando a senha for correta, o relé deve ser
 * ligado por um pequeno tempo (utilize um LED de sinalização). Preveja que a
 * senha possa ser alterada e salva na EEPROM. O circuito abaixo exemplifica o
 * hardware de controle.
 */ 

#include "definicoes_principais.h" //inclui biblioteca de definições principais
#include "teclado_hexadecimal.h"   //inclui biblioteca de funções do teclado matricial 4x4


//definições de hardware
#define RELE   PB3 //substitui RELE por PD3 na programação
#define LED    PB4 //substitui LED por PD4 na programação
#define BUZZER PB5 //substitui BUZZER por PD5 na programação

//declaração de protótipos de funções
void aciona_buzzer(void);
void aciona_rele(void);

int main(void) //função principal
{
	unsigned int endereco_eeprom;  //endereço da senha
	unsigned char senha_eeprom[3] = {'1', '5', '9'};   //senha que será salva na eeprom;

	unsigned char senha_s[3]; //declaração de variável local não inicializada
	unsigned char tecla;      //declaração de variável local não inicializada
	unsigned char digitos;    //declaração de variável local não inicializada
	unsigned char senha_u[3]; //declaração de variável local não inicializada
	unsigned char verifica;   //declaração de variável local não inicializada

	DDRD = 0x0F;  //define os pinos PD0:3 como saídas e PD4:7 como entradas
	DDRB = 0xFF;  //define todo o PORTB como saída
	PORTD = 0xF0; //habilita os resistores de pull-up das entradas e coloca as saída no nível baixo
	PORTB = 0x00; //coloca todo o PORTB em nível baixo

	UCSR0B = 0x00; //desabilita as funções de RX e TX dos pinos PD0 e PD1

	//lê o conteúdo da memória eeprom e armazena em senha_s
	endereco_eeprom = 0x0000;
	digitos = 0;
	while(digitos < 3)
	{
		__EEGET(senha_s[digitos++], endereco_eeprom++);
	}

	//verifica se a senha_s é igual à senha_eeprom
	endereco_eeprom = 0x0000; //restaura endereço
	digitos = 0;              //restaura digitos
	verifica = 0;             //inicializa verifica
	while(digitos < 3)
	{
		if(senha_s[digitos] == senha_eeprom[digitos])
		verifica++;
		digitos++;
	}
	//se forem diferentes, uma novo senha é gravada e liga em seguida da eeprom
	if(verifica < 3)
	{
		endereco_eeprom = 0x0000; //restaura endereço
		digitos = 0;              //restaura digitos
		while(digitos < 3)
		{
			__EEPUT(endereco_eeprom++, senha_eeprom[digitos++]);
		}
		endereco_eeprom = 0x0000; //restaura endereço
		digitos = 0;              //restaura digitos
		while(digitos < 3)
		{
			__EEGET(senha_s[digitos++], endereco_eeprom++);
		}
	}
	
	while(1) //loop infinito
	{
		digitos = 0;         //inicializa variável local
		verifica = 0;        //inicializa variável local
		set_bit(PORTB, LED); //acende led para indicar que o programa está pronto para receber a senha
		while(digitos < 3)   //recebe uma senha de três digitos do usuário
		{
			tecla = Leitura_Teclado(); //realiza a leitura do teclado
			_delay_ms(10);             //elimina debounce
			if(tecla != '\0')          //verifica se alguma tecla foi pressinada
			{
				senha_u[digitos++] = tecla;
				aciona_buzzer();
			}
		}
		clr_bit(PORTB, LED); //apaga o led para indicar que a senha está sendo verificada
		for(digitos = 0; digitos < 3; digitos++) //verifica se a senha informada é correta
		{
			if(senha_u[digitos] == senha_s[digitos])
			verifica++;
		}
		if(verifica == 3)    //se a senha for correta, aciona o relé
		aciona_rele();
		else                 //se a senha for incorreta, aciona o buzzer três vezes e pisca o LED
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

//implementando funções

//Esta função aciona o buzzer
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

//Esta função mantém o relé acionado por um segundo
void aciona_rele(void)
{
	set_bit(PORTB, RELE);
	_delay_ms(1000);
	clr_bit(PORTB, RELE);
	_delay_ms(10);
}
