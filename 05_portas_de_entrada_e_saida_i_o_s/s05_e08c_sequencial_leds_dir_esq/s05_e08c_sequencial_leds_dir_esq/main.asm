;
; s05_e08c_sequencial_leds_dir_esq.asm
;
; Created: 05/08/2022 17:52:25
; Author : Matheus
;
; c) Ligue sequencialmente 1 LED da direita para a esquerda, desta vez
; somente um LED deve ser ligado por vez.

.ORG   0x00          ;endereço de escrita do início do código

INICIO:
    LDI   R16, 0xFF  ;carrega o registrador R16 com 0xFF
    OUT   DDRD, R16  ;configura todo o PORTD como saída
    STS   UCSR0B, R1 ;desabilita as funções de TX e RX dos pinos D0 e D1 do Arduino UNO  
PRINCIPAL:
    LDI   R16, 0x01  ;carrega o registrador R16 com 0x01
 sequencial:
    OUT   PORTD, R16 ;liga o led correspondente ao valor de R16
    RCALL ATRASO     ;chama a sub-rotina de atraso       
    LSL   R16        ;deslocamento lógico para a esquerda
    CPI   R16, 0x00  ;compara o valor de R16 com 0x00
    BRNE  sequencial ;enquanto R16 != 0x00 vai para sequencial
    RJMP  PRINCIPAL  ;retorna para a rotina principal, loop infinito
ATRASO:              ;atraso de aproximadamente 500 ms
    LDI   R19, 41    ;carrega o registrador R19 com 0x41
 volta:
    DEC   R17        ;decrementa o registrador R17
    BRNE  volta      ;enquanto R17 > 0 decrementa R17
    DEC   R18        ;decrementa o registrador R18
    BRNE  volta      ;enquanto R18 > 0 decrementa R18
    DEC   R19        ;decrementa o registrador R19
    BRNE  volta      ;enquanto R19 > 0 vai para volta
    RET              ;retorna para a rotina principal
