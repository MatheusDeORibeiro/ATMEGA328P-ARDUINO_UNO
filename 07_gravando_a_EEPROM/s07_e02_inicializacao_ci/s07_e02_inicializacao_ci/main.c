/*
 * s07_e02_inicializacao_ci.c
 *
 * Created: 29/09/2022 18:49:05
 * Author : Matheus
 * 
 * 7.2 – Supondo que exista um circuito integrado ligado ao ATmega328 e que a
 * primeira vez que o sistema é energizado esse circuito necessite uma
 * inicialização específica. O programa do microcontrolador deve interagir com o
 * usuário e perguntar se a inicialização do circuito integrado deve ser feita;
 * caso afirmativo, essa ação será feita somente uma vez e, na próxima vez que o
 * microcontrolador for inicializado, o programa não fará mais nenhuma pergunta.
 *
 * Faça um programa que utilize um byte da EEPROM para armazenar a informação de
 * que a inicialização do circuito integrado foi feita. Empregue dois LEDs e um
 * botão. A primeira vez, um dos LEDs acende, indicando que a inicialização
 * necessita ser feita; quando o botão for pressionado, o segundo LED deve ser
 * ligado. Ao se reenergizar o circuito, somente o segundo LED deve ficar ligado,
 * indicando que a inicialização já foi feita e o processo de inicialização não é
 * mais necessário.
 */ 

#include "definicoes_principais.h" //inclui a biblioteca de definições principais


//definições de hardware
#define LED1  PD0 //substitui LED1 por PD0 na programação
#define LED2  PD1 //substitui LED2 por PD1 na programação
#define BOTAO PD2 //substitui BOTAO por PD2 na programação

int main(void) //função principal
{
	unsigned char dado = 0x50;
	unsigned int endereco_eeprom = 0x000F; //endereço que a informação da inicialização será armazenada
	unsigned char leitura_eeprom;

	DDRD = 0xFB; //configura o pino PD2 como entrada e os pinos restantes como saída
	PORTD = 0x04; //habilita o resistor pull-up da entrada e coloca os pinos restantes em nível baixo

	UCSR0B = 0x00; //configura os pinos RX e TX (PD0 e PD1) como I/Os digitais para o Arduino Uno

	__EEGET(leitura_eeprom, endereco_eeprom);
	
	if(leitura_eeprom == dado) //verifica se a inicialização é necessária
	{
		set_bit(PORTD, LED2); //liga LED que indica que a inicialização não precisa ser mais feita
	}
	else
	{
		while(leitura_eeprom != dado)
		{
			set_bit(PORTD, LED1);      //acende o LED que indica que a inicialização deve ser feita
			if(!tst_bit(PIND, BOTAO)) //aguarda o botão ser pressionado
			{
				while(!tst_bit(PIND, BOTAO));  //aguarda o botão ser solto
				__EEPUT(endereco_eeprom, dado); //armazena a informação na EEPROM
				//inicialização do circuito integrado
			}
			__EEGET(leitura_eeprom, endereco_eeprom); //leitua da informação armazenada
			if(leitura_eeprom == dado)                //verifica se a informação armazenada está correta
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