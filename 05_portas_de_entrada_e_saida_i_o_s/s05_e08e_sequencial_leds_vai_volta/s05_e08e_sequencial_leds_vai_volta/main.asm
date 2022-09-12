;
; s05_e08e_sequencial_leds_vai_volta.asm
;
; Created: 10/08/2022 20:21:28
; Author : Matheus
;
; e) Ligue todos os LEDs e apague somente 1 LED de cada vez, da direita
; para a esquerda e vice-versa (vai e volta), somente um LED deve ser
; apagado por vez.;

.ORG 0x00              ;endereço de escrita do iníco do código

INICIO:
    LDI   R16, 0xFF    ;carrega o registrador R16 com 0xFF
    OUT   DDRD, R16    ;configura todo o PORTD como saída
    STS   UCSR0B, R1   ;desabilita as funções de TX e RX dos pinos D0 e D1 do Arduino UNO
PRINCIPAL:
    LDI   R16, 0xFF    ;carrega o registrador R16 com 0xFF
 seq_esquerda:
    OUT   PORTD, R16   ;acende os leds correspondentes ao valor de R16
    RCALL ATRASO       ;chama a sub-rotina de atraso  
    LSL   R16          ;deslocamento lógico para a esquerda
    CPI   R16, 0x00    ;compara o valor do registrador R16 com 0x00
    BRNE  seq_esquerda ;enquanto R16 > 0x00 vai para seq_esquerda
    OUT   PORTD, R16   ;acende os leds correspondentes ao valor de R16
    RCALL ATRASO       ;chama a sub-rotina de atraso  
    LDI   R16, 0xFF    ;carrega o registrador R16 com 0xFF
 seq_direita:
    OUT   PORTD, R16   ;acende os leds correspondentes ao valor de R16
    RCALL ATRASO       ;chama a sub-rotina de atraso  
    LSR   R16          ;deslocamento lógico para a direita
    CPI   R16, 0x00    ;compara o valor do registrador R16 com 0x00
    BRNE  seq_direita  ;enquanto R16 > 0 vai para seq_direita
    OUT   PORTD, R16   ;acende os leds correspondentes ao valor de R16
    RCALL ATRASO       ;chama a sub-rotina de atraso  
    RJMP  PRINCIPAL    ;retorna para a rotina principal, loop infinito  
ATRASO:                ;gera um atraso de aproximadamente 500ms
    LDI   R19, 41      ;carrega o registrador R19 com 41
 volta:
    DEC   R17          ;decrementa o registrador R17
    BRNE  volta        ;enquanto R17 > 0 decrementa R17
    DEC   R18          ;decrementa o registrador R18
    BRNE  volta        ;enquanto R18 > 0 decrementa R18
    DEC   R19          ;decrementa o registrador R19
    BRNE  volta        ;enquanto R19 > 0 vai para volta
    RET                ;retorna para a rotina principal
