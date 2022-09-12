;
; s05_e09_botao_led_debounce_10ms.asm
;
; Created: 12/08/2022 14:26:52
; Author : Matheus
;
; 5.9 – Elaborar um programa para ligar imediatamente um LED após o pressionar de 
; um botão, com uma rotina de atraso de 10 ms para eliminação do bounce.


;definições
.EQU   LED   = PD2         ;LED é substituído por PD2 no código
.EQU   BOTAO = PD7         ;BOTAO é subistituído por PD7 no código

.ORG 0x00                 ;endereço de início de escrita de código

INICIO:
    LDI   R16, 0b00000100 ;carrega o registrador R16 com o valor 0b00000100
    OUT   DDRD, R16       ;configura o pino 2 do PORTD como saída e os restantes como entrada
    LDI   R16, 0b11111011 ;carrega o registrador R16 com o valor 0b11111011
    OUT   PORTD, R16      ;habilita os resistores de pull-up para as entradass e desliga o led
    NOP                   ;sincronização do PORT para permitir uma leitura imediatamente após uma escrita
PRINCIPAL:
    SBIC  PIND, BOTAO     ;verifica se o botão foi pressionado
    RJMP  PRINCIPAL       ;retorna para o rotina principal, loop infinito
    SBI   PORTD, LED      ;liga o led
    RCALL DEBOUNCE        ;chama a sub-rotina de atraso
    RJMP  PRINCIPAL       ;retorna para o rotina principal, loop infinito   
DEBOUNCE:                 ;gera um atraso de aproximadamente 10ms
    LDI   R18, 208        ;carrega o registrador R18 com o valor 208
 volta:
    DEC   R17             ;decrementa o registrador R17
    BRNE  volta           ;enquanto R17 > 0 decrementa R17
    DEC   R18             ;decrementa o registrador R18
    BRNE  volta           ;enquanto R18 > 0 vai para volta
    RET                   ;retorna para a rotina principal
