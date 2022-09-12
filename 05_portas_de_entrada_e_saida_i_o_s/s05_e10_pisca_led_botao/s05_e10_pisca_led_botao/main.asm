;
; s05_e10_pisca_led_botao.asm
;
; Created: 12/08/2022 15:19:18
; Author : Matheus
;
; 5.10 – Elaborar um programa que troque o estado do LED se o botão continuar 
; sendo pressionado. Utilize uma frequência que torne agradável o piscar do LED.


;definições
.EQU LED    = PD2         ;LED é substituído por PD2 no código
.EQU BOTAO  = PD7         ;BOTAO é substituído por PD7 no código

.ORG 0x00                 ;endereço de início de escrita do código

INICIO:
    LDI   R16, 0b00000100 ;carrega o registrador R16 com o valor 0b00000100
    OUT   DDRD, R16       ;configura o pino RD2 do PORTD como saída e os restantes como entrada
    LDI   R16, 0b11111011 ;carrega o registrador R16 com o valor 0b11111011
    OUT   PORTD, R16      ;habilita os resistores pull-up para as entradas e desliga o led
    NOP                   ;sincronização do PORT para permitir uma leitura imediatamente após uma escrita
PRINCIPAL:
    SBIC  PIND, BOTAO     ;verifica se o botão foi pressionado
    RJMP  PRINCIPAL       ;retorna para a rotina principal, loop infinito
    SBI   PIND, LED       ;liga led
    RCALL ATRASO          ;chama sub-rotina de atraso    
    CBI   PORTD, LED      ;apaga o LED
    RCALL ATRASO          ;chama sub-rotina de atraso
    RJMP  PRINCIPAL       ;retorna para a rotina principal, loop infinito
ATRASO:	                  ;gera um atraso de aproximadamente 500ms
    LDI   R19, 41         ;carrega o registrador R19 com o valor 41
 volta:
    DEC   R17             ;decrementa o registrador R17
    BRNE  volta           ;enquanto R17 > 0 decrementa R17
    DEC   R18             ;decrementa o registrador R18
    BRNE  volta           ;enquanto R18 > 0 decrementa R18
    DEC   R19             ;decrementa o registrador R19
    BRNE  volta           ;enquanto R19 > 0 vai para volta
    RET
