;
; s05_e08f_contagem_binaria_cres.asm
;
; Created: 11/08/2022 14:41:35
; Author : Mathe
;
; f) Mostre uma contagem binária crescente (0-255) com passo de
; 250 ms.


.ORG 0x00              ;endereço do iníco do código

INICIO:
    LDI   R16, 0xFF    ;carrega o registrador R16 com 0xFF
    OUT   DDRD, R16    ;configura todo o PORTD como saída
    STS   UCSR0B, R1   ;desabilita as funções de TX e RX dos pinos D0 e D1 do Arduino UNO
    LDI   R16, 0x00    ;carrega o registrador R16 com 0x00
PRINCIPAL:
    OUT   PORTD, R16   ;acende os leds correspondentes ao valor de R16
    RCALL ATRASO       ;chama a sub-rotina de atraso
    INC   R16          ;incrementa o registrador R16
    RJMP  PRINCIPAL    ;retorna para a rotina principal, loop infinito
ATRASO:
    LDI   R19, 21      ;rotina de atraso de aproximadamente 250ms  
 volta:
    DEC R17            ;decrementa o registrador R17
    BRNE volta         ;enquanto R18 > 0 decrementa R18
    DEC R18            ;decrementa o registrador R18
    BRNE volta         ;enquanto R17 > 0 decrementa R17
    DEC R19            ;decrementa o registrador R19
    BRNE volta         ;enquanto R19 > 0 vai para volta
    RET                ;retorna para o rotina principal
