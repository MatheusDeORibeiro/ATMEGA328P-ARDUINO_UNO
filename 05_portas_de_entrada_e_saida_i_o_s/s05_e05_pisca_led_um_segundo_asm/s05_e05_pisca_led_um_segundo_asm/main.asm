;
; s05_e05_pisca_led_um_segundo_asm.asm
;
; Created: 03/08/2022 15:28:59
; Author : Matheus
;
; 5.5 � Fa�a um programa em assembly para piscar um LED a cada 1 s.
;

.EQU LED = PB5       ;LED � substitu�do por PB5 na programa��o
.ORG 0x00            ;endere�o de escrita do in�cio do c�digo

INICIO:
    LDI   R16, 0xFF  ;carrega o registrador R16 com 0xFF
    OUT   DDRB, R16  ;configura todo a PORTD como sa�da
PRINCIPAL:
    SBI   PORTB, LED ;liga o LED
    RCALL ATRASO     ;chama a sub-rotina de atraso
    CBI   PORTB, LED ;desliga o LED
    RCALL ATRASO     ;chama a sub-rotina de atraso
    RJMP  PRINCIPAL  ;retorna para o in�cio do programa principal, loop infinito
ATRASO:
    LDI   R19, 82    ;carrega o registrador R19 com 82
 volta:
    DEC   R17        ;decrementa o registrador R17 em uma unidade
    BRNE  volta      ;enquanto R17 > 0 fica decrementando R17
    DEC   R18        ;decrementa o registrador R18 em uma unidade
    BRNE  volta      ;enquanto R18 > 0 fica decrementando R18
    DEC   R19        ;decrementa o registrador R19 em uma unidade
    BRNE  volta      ;enquanto R19 > 0 vai para volta
    RET              ;retorna para a rotina principal
