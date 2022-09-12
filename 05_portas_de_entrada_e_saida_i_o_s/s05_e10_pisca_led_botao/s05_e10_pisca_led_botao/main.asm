;
; s05_e10_pisca_led_botao.asm
;
; Created: 12/08/2022 15:19:18
; Author : Matheus
;
; 5.10 � Elaborar um programa que troque o estado do LED se o bot�o continuar 
; sendo pressionado. Utilize uma frequ�ncia que torne agrad�vel o piscar do LED.


;defini��es
.EQU LED    = PD2         ;LED � substitu�do por PD2 no c�digo
.EQU BOTAO  = PD7         ;BOTAO � substitu�do por PD7 no c�digo

.ORG 0x00                 ;endere�o de in�cio de escrita do c�digo

INICIO:
    LDI   R16, 0b00000100 ;carrega o registrador R16 com o valor 0b00000100
    OUT   DDRD, R16       ;configura o pino RD2 do PORTD como sa�da e os restantes como entrada
    LDI   R16, 0b11111011 ;carrega o registrador R16 com o valor 0b11111011
    OUT   PORTD, R16      ;habilita os resistores pull-up para as entradas e desliga o led
    NOP                   ;sincroniza��o do PORT para permitir uma leitura imediatamente ap�s uma escrita
PRINCIPAL:
    SBIC  PIND, BOTAO     ;verifica se o bot�o foi pressionado
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
