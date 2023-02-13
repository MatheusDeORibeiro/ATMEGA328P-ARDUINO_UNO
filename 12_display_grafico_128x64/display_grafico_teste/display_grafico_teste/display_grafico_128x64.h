/*
 * display_grafico_128x64.h
 *
 * Created: 01/02/2023 14:55:40
 *  Author: Matheus
 */ 


#ifndef DISPLAY_GRAFICO_128X64_H_
#define DISPLAY_GRAFICO_128X64_H_

#include "definicoes_principais.h"

//declaração das macros de hardware

#define PORT_DADOS  PORTD //define os pinos do barramento de dados
#define DIR_DADOS   DDRD  //define a direção do barramento de dados
#define LE_DADOS    PIND  //lê o barramento de dados
#define PORT_CONT   PORTC //define os pinos do barramento de controle
#define DIR_CONT    DDRC  //define a direção dos pinos de controle
#define _CS1        PC0   //define o pino do seletor de chip 1
#define _CS2        PC1   //define o pino do seletor de chip 2
#define _RS         PC2   //define o pino do RS
#define _RW         PC3   //define o pino do R/W
#define _EN         PC4   //define o pino do E
#define _RST        PC5   //define o pino do RESET

//declaração de protótipos de funções
void Inicializacao_Display(void);
void Escreve_Instrucao(char instrucao);
char Status_BF(void);
void Escreve_Dado(char dado, unsigned char pagina, unsigned char coluna);
char Leitura_Dado(unsigned char pagina, unsigned char coluna);
void Posiciona_Pagina(char pagina);
void Posiciona_Coluna(char coluna);
void Escreve_S_memdados(char *string, unsigned char pagina, unsigned char coluna);
void Escreve_S_memprogm(const char *string, unsigned char pagina, unsigned char coluna);
void Imprime_Imagem(const char *imagem);
void Testa_Display(void);
void Limpa_Display(void);
void Escreve_S_memdados_light(char *string, unsigned char pagina, unsigned char coluna);
void Escreve_S_memprogm_light(const char *string, unsigned char pagina, unsigned char coluna);

//declaração de outras macros

#define pulso_reset() clr_bit(PORT_CONT, _RST); _delay_us(2); set_bit(PORT_CONT, _RST) //envia um pulso no pino de RESET
#define pulso_enable() _delay_us(10); set_bit(PORT_CONT, _EN); _delay_us(10); clr_bit(PORT_CONT, _EN)  //envia um pulso no pino de ENABLE

#endif /* DISPLAY_GRAFICO_128X64_H_ */
