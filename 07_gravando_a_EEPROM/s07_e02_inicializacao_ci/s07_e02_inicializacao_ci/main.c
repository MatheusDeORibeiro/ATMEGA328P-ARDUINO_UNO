/*
 * s07_e02_inicializacao_ci.c
 *
 * Created: 29/09/2022 18:49:05
 * Author : Matheus
 * 
 * 7.2 � Supondo que exista um circuito integrado ligado ao ATmega328 e que a
 * primeira vez que o sistema � energizado esse circuito necessite uma
 * inicializa��o espec�fica. O programa do microcontrolador deve interagir com o
 * usu�rio e perguntar se a inicializa��o do circuito integrado deve ser feita;
 * caso afirmativo, essa a��o ser� feita somente uma vez e, na pr�xima vez que o
 * microcontrolador for inicializado, o programa n�o far� mais nenhuma pergunta.
 *
 * Fa�a um programa que utilize um byte da EEPROM para armazenar a informa��o de
 * que a inicializa��o do circuito integrado foi feita. Empregue dois LEDs e um
 * bot�o. A primeira vez, um dos LEDs acende, indicando que a inicializa��o
 * necessita ser feita; quando o bot�o for pressionado, o segundo LED deve ser
 * ligado. Ao se reenergizar o circuito, somente o segundo LED deve ficar ligado,
 * indicando que a inicializa��o j� foi feita e o processo de inicializa��o n�o �
 * mais necess�rio.
 */ 

#include "definicoes_principais.h" //inclui a biblioteca de defini��es principais


//defini��es de hardware
#define LED1  PD0 //substitui LED1 por PD0 na programa��o
#define LED2  PD1 //substitui LED2 por PD1 na programa��o
#define BOTAO PD2 //substitui BOTAO por PD2 na programa��o

int main(void) //fun��o principal
{
	unsigned char dado = 0x50;
	unsigned int endereco_eeprom = 0x000F; //endere�o que a informa��o da inicializa��o ser� armazenada
	unsigned char leitura_eeprom;

	DDRD = 0xFB; //configura o pino PD2 como entrada e os pinos restantes como sa�da
	PORTD = 0x04; //habilita o resistor pull-up da entrada e coloca os pinos restantes em n�vel baixo

	UCSR0B = 0x00; //configura os pinos RX e TX (PD0 e PD1) como I/Os digitais para o Arduino Uno

	__EEGET(leitura_eeprom, endereco_eeprom);
	
	if(leitura_eeprom == dado) //verifica se a inicializa��o � necess�ria
	{
		set_bit(PORTD, LED2); //liga LED que indica que a inicializa��o n�o precisa ser mais feita
	}
	else
	{
		while(leitura_eeprom != dado)
		{
			set_bit(PORTD, LED1);      //acende o LED que indica que a inicializa��o deve ser feita
			if(!tst_bit(PIND, BOTAO)) //aguarda o bot�o ser pressionado
			{
				while(!tst_bit(PIND, BOTAO));  //aguarda o bot�o ser solto
				__EEPUT(endereco_eeprom, dado); //armazena a informa��o na EEPROM
				//inicializa��o do circuito integrado
			}
			__EEGET(leitura_eeprom, endereco_eeprom); //leitua da informa��o armazenada
			if(leitura_eeprom == dado)                //verifica se a informa��o armazenada est� correta
			{
				clr_bit(PORTD, LED1); //apaga LED1
				set_bit(PORTD, LED2); //acende LED2
			}
		}
	}
	while(1) //loop inifinito
	{
	}
}