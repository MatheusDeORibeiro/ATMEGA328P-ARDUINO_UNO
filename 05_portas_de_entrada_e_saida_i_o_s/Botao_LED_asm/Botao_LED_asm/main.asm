;
; Botao_LED.asm
;
; Created: 11/08/2022 17:04:19
; Author : Matheus
;


//================================================================================ //
// LIGANDO E DESLIGANDO UM LED QUANDO UM BOTÃO É PRESSIONADO //
//================================================================================ //
//DEFINIÇÕES
.equ LED = PD2            //LED é o substituto de PD2 na programação
.equ BOTAO = PD7          //BOTAO é o substituto de PD7 na programação
.def AUX = R16            /*R16 tem agora o nome de AUX (nem todos os 32 registradores
                            de uso geral podem ser empregados em todas as instruções) */
//-----------------------------------------------------------------------------------
.ORG 0x000                /*endereço de início de escrita do código na memória flash,
                            após o reset o contador do programa aponta para cá.*/
Inicializacoes:
    LDI   AUX, 0b00000100 //carrega AUX com o valor 0x04 (1 = saída e 0 = entrada)
    OUT   DDRD, AUX       //configura PORTD, PD2 saída e demais pinos entradas
    LDI   AUX, 0b11111011 /*habilita o pull-up para o botão e apaga o LED (pull-up em
                            todas as entradas)*/
    OUT   PORTD, AUX
    NOP                   /*sincronização dos dados do PORT. Necessário somente para
                            uma leitura imediatamente após uma escrita no PORT*/
//------------------------------------------------------------------------------------
//LAÇO PRINCIPAL
//------------------------------------------------------------------------------------
Principal:
    SBIC  PIND, BOTAO     //verifica se o botão foi pressionado, senão
    RJMP  Principal       //volta e fica preso no laço Principal
 Esp_Soltar:
    SBIS  PIND, BOTAO     //se o botão não foi solto, espera soltar
    RJMP  Esp_Soltar
    RCALL Atraso          /*após o botão ser solto gasta um tempo para eliminar o
                            ruído proveniente do mesmo*/
    SBIS  PORTD, LED      //se o LED estiver ligado, liga e vice-versa
    RJMP  Liga
    CBI   PORTD, LED      //apaga o LED
    RJMP  Principal       //volta ler botão
 Liga:
    SBI   PORTD, LED      //liga LED
    RJMP  Principal       //volta ler botão
//------------------------------------------------------------------------------------
//SUB-ROTINA DE ATRASO - Aprox. 12 ms a 16 MHz
//------------------------------------------------------------------------------------
Atraso: 
    DEC   R3              //decrementa R3, começa com 0x00
    BRNE  Atraso          //enquanto R3 > 0 fica decrementando R3
    DEC   R2
    BRNE  Atraso          //enquanto R2 > 0 volta a decrementar R3
    RET
//===================================================================================