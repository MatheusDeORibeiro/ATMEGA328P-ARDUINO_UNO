;
; s05_e08d_sequencial_leds_vai_volta.asm
;
; Created: 05/08/2022 18:32:39
; Author : Matheus
;
; d) Ligue sequencialmente 1 LED da esquerda para a direita e vice-versa
; (vai e volta), só um LED deve ser ligado por vez.


.ORG 0x00                ;endereço de escrita do iníco do código

INICIO:
    LDI   R16, 0xFF      ;carrega R16 com 0xFF
    OUT   DDRD, R16      ;configura todo o PORTD como saída
    STS   UCSR0B, R1     ;desabilita as funções de TX e RX dos pinos D0 e D1 do Arduino UNO
PRINCIPAL:
    LDI   R16, 0x80      ;carrega R16 com 0x80
 sequencial_dir:
    OUT   PORTD, R16     ;acende o led correspondente ao valor de R16
    RCALL ATRASO         ;chama a sub-rotina de atraso
    LSR   R16            ;deslocamento lógico para a direita
    CPI   R16, 0x00      ;compara o valor de R16 com 0x00
    BRNE  sequencial_dir ;enquanto R16 != 0x01 vai para sequencial_dir
    LDI   R16, 0x02      ;carrega R16 com 0x02
 sequencial_esq:
    OUT   PORTD, R16     ;acende o led correspondente ao valor de R16
    RCALL ATRASO         ;chama a sub-rotina de atraso
    LSL   R16            ;deslocamento lógico para a direita
    CPI   R16, 0x80      ;compara o valor de R16 com 0x08
    BRNE  sequencial_esq ;enquanto R16 != 0x08 vai para sequencial_esq
    RJMP  PRINCIPAL      ;retorna para a rotina principal, loop infinito
ATRASO:                  ;atraso de aproximadamente 500 ms
    LDI   R19, 41        ;carrega o registrador R19 com 0x41
 volta:
    DEC   R17            ;decrementa o registrador R17
    BRNE  volta          ;enquanto R17 > 0 decrementa R17
    DEC   R18            ;decrementa o registrador R18
    BRNE  volta          ;enquanto R18 > 0 decrementa R18
    DEC   R19            ;decrementa o registrador R19
    BRNE  volta          ;enquanto R19 > 0 vai para volta
    RET                  ;retorna para a rotina principal
