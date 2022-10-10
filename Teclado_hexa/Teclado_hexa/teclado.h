#ifndef _TECLADO_H
#define _TECLADO_H

#include "def_principais.h"

#define LINHA PIND   //registrador para a leitura das linhas
#define COLUNA PORTD //registrador para a escrita nas colunas

//protótipo da função
unsigned char ler_teclado();

#endif
