;
; s05_e08b_sequencial_leds_dir_esq.asm
;
; Created: 04/08/2022 16:47:30
; Author : Matheus
;
; b) Ligue sequencialmente 1 LED da esquerda para a direita, mesma
; lógica da letra a.

; 128 64 32 16 8 4 2 1

.ORG  0x00           ;endereço de escrita do início do código

INICIO:
    LDI   R16, 0xFF  ;carrega o registrador R16 com 0xFF
    LDI   R17, 0x80  ;carrega o registrador R17 com 0x80 (0b10000000)
    OUT   DDRD, R16  ;configura todo o PORTB como saída
    STS   UCSR0B, R1 ;desabilita as funções de TX e RX dos pinos D0 e D1 do Arduino UNO
PRINCIPAL:
    LDI   R16, 0x00  ;carrega o registrador R16 com 0x00
    OUT   PORTD, R16 ;desliga todos os leds
    RCALL ATRASO     ;chama sub-rotina de atraso  
 sequencial:
    LSR   R16        ;deslocamento lógico para a direita
    ADD   R16, R17   ;armazena em R16 a soma entre R16 e R17
    OUT   PORTD, R16 ;liga os leds correspondentes ao valor de R16
    RCALL ATRASO     ;chama sub-rotina de atraso  
    CPI   R16, 0xFF  ;compara o valor R16 com 0xFF(0b11111111)
    BRNE  sequencial ;enquanto R16 != 0xFF vai para sequencial
    RJMP  PRINCIPAL  ;retorna para a rotina principal, loop infinito
ATRASO:              ;atraso de aproximadamente 500 ms
    LDI   R20, 41    ;carrega o registrador R20 com 41
 volta:
    DEC   R18        ;decrementa o registrador R18
    BRNE  volta      ;enquanto R18 > 0 decrementa R18
    DEC   R19        ;decrementa o registrador R19
    BRNE  volta      ;enquanto R19 > 0 decrementa R19
    DEC   R20        ;decrementa o registrador R20
    BRNE  volta      ;enquanto R20 > 0 vai para volta
    RET              ;retorna para a rotina principal
