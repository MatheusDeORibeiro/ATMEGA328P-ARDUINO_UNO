;
; s05_e11_botao_led_frequencia.asm
;
; Created: 23/08/2022 17:51:36
; Author : Matheus
;
; 5.11 � Elaborar um programa para aumentar a frequ�ncia em que um LED liga e 
; desliga, enquanto um bot�o estiver sendo pressionado, at� o momento em que o 
; LED ficar� continuamente ligado. Quando o bot�o � solto o LED deve ser desligado.
; Qual a aplica��o pr�tica dessa t�cnica, imaginando que o bot�o pode ser o sinal 
; proveniente de algum sensor e o LED algum dispositivo sinalizador?

;defini��es
.EQU LED    = PD2         ;LED � substitu�do por PD2 no c�digo
.EQU BOTAO  = PD7         ;BOTAO � substitu�do por PD7 no c�digo

.ORG 0x00                 ;endere�o de in�cio de escrita do c�digo

INICIO:
    LDI   R16, 0b00000100 ;carrega o registrador R16 com o valor 0b00000100
    OUT   DDRD, R16       ;confira o pino RD2 do PORTD como sa�da e os restantes como entrada
    LDI   R16, 0b11111011 ;carrega o registrador R16 com o valor 0b11111011
    OUT   PORTD, R16      ;habilita os resistores pull-up para as entradas e desliga o led
    NOP                   ;sincroniza��o do PORT para permitir uma leitura imediatamente ap�s uma escrita
PRINCIPAL:
    LDI   R16, 20         ;carrega o registrador R16 com o valor 42
 testa_botao:
    SBIC  PIND, BOTAO     ;verifica se o bot�o foi pressionado
    RJMP  testa_botao     ;sen�o foi pressionado, vai para testa_botao
    SBI   PORTD, LED      ;liga led
    RCALL ATRASO          ;chama sub-rotina de atraso
    CBI   PORTD, LED      ;desliga led
    RCALL ATRASO          ;chama sub-rotina de atraso
    DEC   R16             ;decrementa o registrador R16
    CPI   R16, 0          ;verifica se o registrador R16 � igual a 0
    BRNE  testa_botao     ;sen�o � igual a 0, vai para testa_botao
    SBI   PORTD, LED      ;liga led 
 mantem_ligado:
    SBIS  PIND, BOTAO     ;verifica se o bot�o foi solto
    RJMP  mantem_ligado   ;sen�o foi pressionado, vai para testa_botao
    CBI   PORTD, LED      ;desliga led
    RJMP  PRINCIPAL       ;retorna para o in�cio da rotina, loop infinito
ATRASO:                   ;gera inicialmente um atraso de aproximadamente 250ms 
    MOV   R19, R16        ;move o valor do registrador R16 para o registrador R19
 volta: 
    DEC   R17             ;decrementa o registrador R17
    BRNE  volta           ;enquanto R17 > 0, decrementa R17
    DEC   R18             ;decrementa o registrador R18
    BRNE  volta           ;enquanto R18 > 0, decrementa R18
    DEC   R19             ;decrementa o registrador R19
    BRNE  volta           ;enquanto R19 > 0, vai para volta
    RET                   ;retorna para a rotina principal
