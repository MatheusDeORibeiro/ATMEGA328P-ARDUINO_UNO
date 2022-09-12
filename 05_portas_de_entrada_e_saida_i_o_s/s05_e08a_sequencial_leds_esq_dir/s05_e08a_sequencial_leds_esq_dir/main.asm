;
; s05_e08a_sequencial_leds_esq_dir.asm
;
; Created: 03/08/2022 16:50:25
; Author : Matheus
;
; a) Ligue sequencialmente 1 LED da direita para a esquerda (o LED deve
; permanecer ligado até que todos os 8 estejam ligados, depois eles
; devem ser desligados e o processo repetido).
;

; 128 64 32 16 8 4 2 1

.ORG 0x00            ;endereço de escrita do início do código

INICIO:
    LDI   R16, 0xFF  ;carrega o registrador R16 com 0xFF
    OUT   DDRD, R16  ;configura todo o PORTD como saída
    STS   UCSR0B, R1 ;desabilita as funções de TX e RX dos pinos D0 e D1 do Arduino UNO
PRINCIPAL:
    LDI   R16, 0x00  ;carrega o registrador R16 com 0x00
    OUT   PORTD, R16 ;apaga leds do PORTD
    RCALL ATRASO     ;chama a sub-rotina de atraso        
 sequencial:
    LSL   R16        ;deslocamento lógico para a esquerda
    INC   R16        ;incrementa o registrador R16
    OUT   PORTD, R16 ;liga os leds correspondentes ao valor de R16
    RCALL ATRASO     ;chama a sub-rotina de atraso
    CPI   R16, 0xFF  ;compara o valor de R16 com 0xFF (0b11111111)
    BRNE  sequencial ;enquanto R16 != 0xFF vai para sequencial
    RJMP  PRINCIPAL  ;retorna para a rotina principal, loop infinito
ATRASO:              ;atraso de aproximadamente 500 ms
    LDI   R19, 41    ;carrega o registrador R19 com 41
 volta:
    DEC   R17        ;decrementa o registrador R17
    BRNE  volta      ;enquanto R17 > 0 decrementa R17
    DEC   R18        ;decrementa o registrador R18
    BRNE  volta      ;enquanto R18 > 0 decremesnta R18
    DEC   R19        ;decrementa R20
    BRNE  volta      ;enquanto R19 > 0 vai para volta
    RET              ;retorna para a rotina principal
