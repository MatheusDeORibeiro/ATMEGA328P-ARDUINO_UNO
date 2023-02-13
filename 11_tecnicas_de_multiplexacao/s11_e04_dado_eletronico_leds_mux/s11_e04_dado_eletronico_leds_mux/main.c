/*
 * s11_e04_dado_eletronico_leds_mux.c
 *
 * Created: 18/01/2023 15:24:58
 * Author : Matheus
 * 
 * 11.4 – Baseado no exercício 5.14, crie um dado eletrônico empregando
 * somente 3 pinos para o controle dos LEDs.
 */ 


#include "definicoes_principais.h" //inclui a biblioteca de definições principais
#include "timers.h"                //inclui a biblioteca de funções dos módulos temporizadores/contadores

//definição de hardware
#define BOTAO PB0

unsigned char numero = 1;

ISR(TIMER0_OVF_vect); //declaração da rotina de tratamento do estouro do TCNT0

int main() //função principal
{
    DDRC = 0xFF;  //configura todo o PORTC como saída
    DDRB = 0xFE;  //configura PB0 como entrada e os pinos restantes do PORTB como saídas
    PORTC = 0x00; //inicializa o PORTC em nível baixo
    PORTB = 0x01; //habilita o resistor de pull-up da entrada e inicializa as saídas em nível baixo

    Inicializa_TC0(TC0_modo_normal, TC0_OC0A_normal&TC0_OC0B_normal&TC0_CLK_1024, TC0_TOIE0);
    TCNT0 = 100;
    /* TC0 no modo normal, f_osc igual a 16MHz, prescaler igual a 1024, saídas comparadoras desabilitadas,
       interrupção por estouro do TCNT0 habilitada:
       t_estouro = N * (TOP + 1) / f_osc = 1024 * (255 - 100 + 1) / 16M = 9,98ms 
       t_varredura = conta_tempo * t_estouro = 10 * 9,98m = 99,8ms */
    sei(); //habilita todas as fontes de interrupção setando o bit I do registrador SREG

    while(1) //loop infinito
    {
        numero++;                   //incrementa 'numero'
        if(numero > 6) numero = 1;  //se 'numero' for maior do que 6, então a variável é reinicializada
    }
}

ISR(TIMER0_OVF_vect) //implementação da rotina de tratamento do estouro do TCNT0
{
    static unsigned char conta_tempo = 0, face = 0, led_face = 1; //declaração de variáveis auxiliares
    TCNT0 = 100;                          //reinicia o registrador do TC0
    conta_tempo++;                        //incrementa a variável auxiliar de contagem de tempo
    if(conta_tempo == 10)                 //se passou 100ms?...
    {
        if(!tst_bit(PINB, BOTAO))         //o botão foi pressionado?...
        {
            face = numero;                //armazena o número da face na variável 'face'
        }
        conta_tempo = 0;                  //reinicializa a variável auxiliar de contagem de tempo
    }
    PORTC = 0x00;                         //limpa o PORTC 
    DDRC = 0xFF;                          //configura todo o PORTC como saída
    if(face == 1)                         //exibe a face 1
    {
        //liga led 7
        DDRC &= ~((1 << PC1) | (1 << PC0)); 
        PORTC |= 1 << PC3;
    }
    else if(face == 2)                    //exibe a face 2
    {
        if(led_face == 1)
        {
            //liga led 2
            DDRC &= ~((1 << PC3) | (1 << PC2));
            PORTC |= 1 << PC0;
        }
        else
        {
            //liga led 5
            DDRC &= ~((1 << PC2) | (1 << PC1));
            PORTC |= 1 << PC3;
        }
    }
    else if(face == 3)                    //exibe a face 3
    {
        if(led_face == 1)
        {
            //liga led 2
            DDRC &= ~((1 << PC3) | (1 << PC2));
            PORTC |= 1 << PC0;
        }
        else if(led_face == 2)
        {
            //liga led 5
            DDRC &= ~((1 << PC2) | (1 << PC1));
            PORTC |= 1 << PC3;
        }
        else
        {
            //liga led 7
            DDRC &= ~((1 << PC1) | (1 << PC0));
            PORTC |= 1 << PC3;
        }
    }
    else if(face == 4)                    //exibe a face 4
    {
        if(led_face == 1)
        {
            //liga led 1
            DDRC &= ~((1 << PC3) | (1 << PC2));
            PORTC |= 1 << PC1;
        }
        else if(led_face == 2)
        {
            //liga led 2
            DDRC &= ~((1 << PC3) | (1 << PC2));
            PORTC |= 1 << PC0;
        }
        else if(led_face == 3)
        {
            //liga led 5
            DDRC &= ~((1 << PC2) | (1 << PC1));
            PORTC |= 1 << PC3;
        }
        else
        {
            //liga led 6
            DDRC &= ~((1 << PC2) | (1 << PC1));
            PORTC |= 1 << PC0;
        }       
    }
    else if(face == 5)                    //exibe a face 5
    {
        if(led_face == 1)
        {
            //liga led 1
            DDRC &= ~((1 << PC3) | (1 << PC2));
            PORTC |= 1 << PC1;
        }
        else if(led_face == 2)
        {
            //liga led 2
            DDRC &= ~((1 << PC3) | (1 << PC2));
            PORTC |= 1 << PC0;
        }
        else if(led_face == 3)
        {
            //liga led 5
            DDRC &= ~((1 << PC2) | (1 << PC1));
            PORTC |= 1 << PC3;
        }
        else if(led_face == 4)
        {
            //liga led 6
            DDRC &= ~((1 << PC2) | (1 << PC1));
            PORTC |= 1 << PC0;
        }
        else
        {
            //liga led 7
            DDRC &= ~((1 << PC1) | (1 << PC0)); 
            PORTC |= 1 << PC3;
        }
    }
    else if(face == 6)                    //exibe a face 6
    {
        if(led_face == 1)
        {
            //liga led 1
            DDRC &= ~((1 << PC3) | (1 << PC2));
            PORTC |= 1 << PC1;
        }
        else if(led_face == 2)
        {
            //liga led 2
            DDRC &= ~((1 << PC3) | (1 << PC2));
            PORTC |= 1 << PC0;
        }
        else if(led_face == 3)
        {
            //liga led 3
            DDRC &= ~((1 << PC3) | (1 << PC0));
            PORTC |= 1 << PC1;
        }
        else if(led_face == 4)
        {
            //liga led 4
            DDRC &= ~((1 << PC3) | (1 << PC0));
            PORTC |= 1 << PC2;
        }
        else if(led_face == 5)
        {
            //liga led 5
            DDRC &= ~((1 << PC2) | (1 << PC1));
            PORTC |= 1 << PC3;
        }
        else
        {
            //liga led 6
            DDRC &= ~((1 << PC2) | (1 << PC1));
            PORTC |= 1 << PC0;
        }
    }
    else PORTC = 0x00;
    led_face++;                           //incrementa a variável auxiliar de acionamento dos leds
    if(led_face > face) led_face = 0;     //reinicia a variável auxiliar de acionamento dos leds
}
