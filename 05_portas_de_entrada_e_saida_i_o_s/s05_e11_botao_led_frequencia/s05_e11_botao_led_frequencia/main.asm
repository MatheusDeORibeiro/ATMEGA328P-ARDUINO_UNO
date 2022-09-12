;
; s05_e11_botao_led_frequencia.asm
;
; Created: 23/08/2022 17:51:36
; Author : Matheus
;
; 5.11 – Elaborar um programa para aumentar a frequência em que um LED liga e 
; desliga, enquanto um botão estiver sendo pressionado, até o momento em que o 
; LED ficará continuamente ligado. Quando o botão é solto o LED deve ser desligado.
; Qual a aplicação prática dessa técnica, imaginando que o botão pode ser o sinal 
; proveniente de algum sensor e o LED algum dispositivo sinalizador?

;definições
.EQU LED    = PD2         ;LED é substituído por PD2 no código
.EQU BOTAO  = PD7         ;BOTAO é substituído por PD7 no código

.ORG 0x00                 ;endereço de início de escrita do código

INICIO:
    LDI   R16, 0b00000100 ;carrega o registrador R16 com o valor 0b00000100
    OUT   DDRD, R16       ;confira o pino RD2 do PORTD como saída e os restantes como entrada
    LDI   R16, 0b11111011 ;carrega o registrador R16 com o valor 0b11111011
    OUT   PORTD, R16      ;habilita os resistores pull-up para as entradas e desliga o led
    NOP                   ;sincronização do PORT para permitir uma leitura imediatamente após uma escrita
PRINCIPAL:
    LDI   R16, 20         ;carrega o registrador R16 com o valor 42
 testa_botao:
    SBIC  PIND, BOTAO     ;verifica se o botão foi pressionado
    RJMP  testa_botao     ;senão foi pressionado, vai para testa_botao
    SBI   PORTD, LED      ;liga led
    RCALL ATRASO          ;chama sub-rotina de atraso
    CBI   PORTD, LED      ;desliga led
    RCALL ATRASO          ;chama sub-rotina de atraso
    DEC   R16             ;decrementa o registrador R16
    CPI   R16, 0          ;verifica se o registrador R16 é igual a 0
    BRNE  testa_botao     ;senão é igual a 0, vai para testa_botao
    SBI   PORTD, LED      ;liga led 
 mantem_ligado:
    SBIS  PIND, BOTAO     ;verifica se o botão foi solto
    RJMP  mantem_ligado   ;senão foi pressionado, vai para testa_botao
    CBI   PORTD, LED      ;desliga led
    RJMP  PRINCIPAL       ;retorna para o início da rotina, loop infinito
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
