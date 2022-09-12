;
; s05_e08g_contagem_binaria_decres.asm
;
; Created: 11/08/2022 15:40:06
; Author : Mathe
;
; g) Mostre uma contagem binária decrescente (255-0) com passo de
; 250 ms.


.ORG 0x00                  ;endereço do iníco do código

INICIO:
    LDI   R16, 0xFF        ;carrega o registrador R16 com 0xFF
    OUT   DDRD, R16        ;configura todo o PORTD como saída
    STS   UCSR0B, R1       ;desabilita as funções de TX e RX dos pinos D0 e D1 do Arduino UNO
PRINCIPAL:
    LDI   R16, 0xFF        ;carrega o registrador R16 com 0xFF
 cont_decrescente:
    OUT   PORTD, R16       ;acende os leds correspondentes ao valor de R16
    RCALL ATRASO           ;chama a sub-rotina de atraso
    DEC   R16              ;decrementa o registrador R16
    CPI   R16, 0x00        ;compara o valor de R16 com 0
    BRNE  cont_decrescente ;enquanto R16 > 0 vai para cont_decrescente
    OUT   PORTD, R16       ;acende os leds correspondentes ao valor de R16
    RCALL ATRASO           ;chama a sub-rotina de atraso
    RJMP  PRINCIPAL        ;retorna para a rotina principal, loop infinito
ATRASO:                    ;rotina de atraso de aproximadamente 250ms
    LDI   R19, 21          ;carrega o registrador R16 com 21
 volta:
    DEC   R17              ;decrementa o registrador R17
    BRNE  volta            ;enquanto R17 > 0 decrementa R17
    DEC   R18              ;decrementa o registrador R18
    BRNE  volta            ;enquanto R18 > 0 decrementa R18
    DEC   R19              ;decrementa o registrador R19
    BRNE  volta            ;enquanto R19 > 0 vai para volta
    RET                    ;retorna para a rotina principal

