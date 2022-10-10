/*
 * LCD_4bits.h
 *
 * Created: 15/09/2022 14:54:44
 *  Author: Matheus de Oliveira Ribeiro
 */ 


#ifndef LCD_4BITS_H_
#define LCD_4BITS_H_

//definição de hardware
#define PORT_DADOS PORTD //define o barramento de dados
#define DIR_DADOS  DDRD  //define a direção do barramento de dados
#define _DB7       PD7   //define o pino do DB7(busy flag)
#define _DB6       PD6   //define o pino do DB6
#define _DB5       PD5   //define o pino do DB5
#define _DB4       PD4   //define o pino do DB4
#define PORT_CONT  PORTB //define o barramento de controle
#define DIR_CONT   DDRB  //define a direção do barramento de controle 
#define _EN        PB0   //define o pino enable
#define _RW        PB1   //define o pino leitura/escrita
#define _RS        PB2   //define o pino dado/instrução

//macros para número de linhas do dispay

#define LCD_1_N 0x00 //display de uma linha
#define LCD_2_N 0x01 //display de duas linhas

//macros para ligar/desligar o display e ou o cursor

#define LCD_DSP_CS_OFF    0x08 //desliga o display e o cursor
#define LCD_DSP_ON_CS_OFF 0x0C //liga o display e desliga o cursor
#define LCD_DSP_ON_CS_ON  0x0E //liga o dispay e o cursor
#define LCD_DSP_ON_CS_BLK 0x0F //liga o display e o cursor piscante

//macros para deslocamento do display/cursor

#define LCD_DSL_CS_LF 0x10 //desloca o cursor para a esquerda
#define LCD_DSL_CS_RG 0x14 //desloca o cursor para a direita
#define LCD_DSL_MS_LF 0x18 //desloca o cursor e a mensagem para a esquerda
#define LCD_DSL_MS_RG 0x1C //desloca o cursor e a mensagem para a direita

//definição de protótipos de funções

void Escreve_Instrucao(unsigned char instrucao);
unsigned char Status_BF(void);
void Escreve_Dado(unsigned char dado);
unsigned char Leitura_Dado(void);
void Pulso_Enable(void);
void Inicializa_LCD(unsigned char numero_linhas);
void Limpa_Display(void);
void Cursor_Home(void);
void Display_Cursor_ON_OFF(unsigned char display_cursor);
void Desloca_Mensagem(unsigned char deslocamento);
void Posiciona_Cursor(unsigned char linha, unsigned coluna);
void Testa_Display(void);
void Escreve_S_memdados(char *string);
void Escreve_S_memprogm(const unsigned char *string);
void Cria_Caracteres_memdados(char *caracteres);
void Cria_Caracteres_memprogm(const unsigned char *caracteres);

#endif /* LCD_4BITS_H_ */
