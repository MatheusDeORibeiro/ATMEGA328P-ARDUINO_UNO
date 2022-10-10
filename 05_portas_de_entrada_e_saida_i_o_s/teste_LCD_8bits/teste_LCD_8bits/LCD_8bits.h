/*
 * LCD_8bits.h
 *
 * Created: 12/09/2022 18:21:29
 * Author: Matheus de Oliveira Ribeiro
 */


#ifndef LCD_8BITS_H_
#define LCD_8BITS_H_

//definições de hardware

#define PORT_DADOS PORTD //define o barramento de dados
#define DIR_DADOS  DDRD  //define a direção do barramento de dados
#define _BF        PD7   //define o pino do busy flag
#define PORT_CONT  PORTB //define o barramento de controle
#define DIR_CONT   DDRB  //define a direção do barramento de controle
#define _EN        PB0   //define o pino enable
#define _RW        PB1   //define o pino de escrita/leitura
#define _RS        PB2   //define o pino de instrução/dado

//definição de macros para o número de linhas do display

#define LCD_1_N 0  //configura display com uma linha
#define LCD_2_N 1  //configura display com duas linhas

//definição de macros para ligar/desligar o display e/ou cursor
 
#define LCD_DSP_OFF_CS_OFF 0x08 //desliga o display e o cursor
#define LCD_DSP_ON_CS_OFF  0x0C //liga display e desliga cursor
#define LCD_DSP_ON_CS_ON   0x0E //liga display e cursor não piscante
#define LCD_DSP_ON_CS_BLK  0x0F //liga display e cursor piscante

//definição de macros para o deslocamento do cursor e da mensagem

#define LCD_DSL_CS_LF 0x10 //desloca o cursor para a esquerda
#define LCD_DSL_CS_RG 0x14 //desloca o cursor para a direita
#define LCD_DSL_MS_LF 0x18 //desloca o cursor e o texto para a esquerda
#define LCD_DSL_MS_RG 0x1C //desloca o cursor e o texto para a direita

//definição de protótipos de funções

void Escreve_Instrucao(unsigned char instrucao);
unsigned char Status_BF(void);
void Escreve_Dado(unsigned char dado);
unsigned char Leitura_Dado(void);
void Pulso_Enable(void);
void Inicializa_LCD(unsigned char num_linhas);
void Limpa_Display(void);
void Cursor_Home(void);
void Display_Cursor_ON_OFF(unsigned char display_cursor);
void Desloca_Mensagem(unsigned char deslocamento);
void Posiciona_Cursor(signed char linha, signed char coluna);
void Testa_Display(void);
void Escreve_S_memdados(char *string);
void Escreve_S_memprogm(const unsigned char *string);

#endif /* LCD_8BITS_H_ */
