//PROGRAMA PARA PISCAR UM LED LIGADO AO PINO PB5 DO ATMEGA328

.equ LED = PB5 //LED é o substituto de PB0 na programação

.ORG 0x000 //endereço na memória flash de início de escrita do código

INICIO:
LDI R16,0xFF //carrega R16 com o valor 0xFF
OUT DDRB,R16 //configura todos os pinos do PORTB como saída

PRINCIPAL:
SBI PORTB,LED //coloca o pino PB0 em 5 V
RCALL ATRASO //chama a sub-rotina de atraso
CBI PORTB,LED //coloca o pino PB0 em 0 V
RCALL ATRASO //chama a sub-rotina de atraso
RJMP PRINCIPAL //vai para PRINCIPAL

ATRASO: //sub-rotina de atraso
LDI R19,0x02 //carrega R19 com o valor 0x02
volta:
DEC R17 //decrementa R17, começa com 0x00
BRNE volta //enquanto R17 > 0 fica decrementando R17
DEC R18 //decrementa R18, começa com 0x00
BRNE volta //enquanto R18 > 0 volta a decrementar R18
DEC R19 //decrementa R19
BRNE volta //enquanto R19 > 0 vai para volta
RET //retorna da sub-rotina
