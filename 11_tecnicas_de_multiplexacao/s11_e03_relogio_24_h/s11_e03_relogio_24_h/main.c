/*
 * s11_e03_relogio_24_h.c
 *
 * Created: 16/01/2023 12:47:09
 * Author : Matheus
 *
 * 11.3 – Elaborar um programa para que o hardware da fig. 11.10 funcione como
 * relógio 24 h. A entrada de sinal para contagem dos segundos é de 60 Hz. O
 * ajuste do horário deve ser feito nos botões específicos.
 */ 

#include "definicoes_principais.h" //inclui a biblioteca de definições principais
#include "timers.h"                //inclui a biblioteca de funções dos temporizadores/contadores


//definições de hardware
#define SEL       PD7
#define HORA      PD6
#define MIN       PD5
//#define SINAL_IN  PD4
#define SINAL_OUT PD3

//declaração de variáveis globais
volatile unsigned char horas = 0, minutos = 0, segundos = 0;

//declaração de constante armazenada na memória FLASH
const unsigned char display[] PROGMEM =
{
    0x3F, //0
    0x06, //1
    0x5B, //2
    0x4F, //3
    0x66, //4
    0x6D, //5
    0x7D, //6
    0x07, //7
    0x7F, //8
    0x6F  //9
};

ISR(TIMER0_OVF_vect); //declaração da rotina de tratamento do estouro do TCNT0
ISR(PCINT2_vect);     //declaração da rotina de tratamento da interrupção externa
ISR(TIMER2_OVF_vect); //declaração da rotina de tratamento do estouro do TCNT2

int main(void) //função principal
{
    DDRD = 0x0F;  //configura PD3:0 como saídas e PD7:PD4 como entradas
    DDRC = 0xFF;  //configura todo o PORTC como saída
    DDRB = 0xFF;  //configura todo o PORTB como saída
    PORTD = 0xF0; //habilita os resistores pull-up das entradas
    PORTC = 0x00; //inicializa todo o PORTC em nível baixo
    PORTB = 0x00; //inicializa todo o PORTB em nível baixo

    Inicializa_TC0(TC0_modo_normal, TC0_OC0A_normal&TC0_OC0B_normal&TC0_CLK_256, TC0_TOIE0);
    /* modo normal, f_osc igual a 16MHz, prescaler igual a 256, saídas comparadoras desabilitadas,
       interrupção por estouro do TCNT0 habiliatada:
       p_sinal = 1 / f_sinal = 1 / 60 = 16,7ms
       t_estouro = p_sinal / 2 = N * (TOP + 1) / f_osc -> 16,7m / 2 = conta_tempo * 256 * (255 + 1) / 16M
       conta_tempo = 2 */
    PCICR = 0x04;  //habilita as interrupções dos pinos PCINT[23:16](PORTD)
    PCMSK2 = 0x10; //habilita a interrupção do pino PCINT20(PD4)
    /* t_um_seg = n_capturas * t_estouro -> 1 = n_capturas * 8,3m -> n_capturas = 120 */
    Inicializa_TC2(TC2_modo_normal, TC2_OC2A_normal&TC2_OC2B_normal&TC2_CLK_1024, TC2_TOIE2);
    TCNT2 = 212;
    /* modo normal, f_osc igual a 16MHz, prescaler igua a 1024, saídas comparadoras desabilitadas,
       interrupção por estouro do TCNT2 habilitada:
       t_estouro = (f ^ -1) / n_dsp = (60 ^ -1) / 6 = 2,77ms -> t_estouro = 2,8ms
       t_estouro = N * (TOP + 1) / f_osc -> 2,8m = 1024 * (255 - TCNT2 + 1) / 16M
       TCNT2 = 212 */
    sei(); //habilita todas as fontes de inerrupção setando o bit I do registrador SREG

    while(1) //loop infinito
    {
        while(!tst_bit(PIND, SEL))           //o botão 'SEL' foi pressionado?...
        {
            if(!tst_bit(PIND, MIN))          //o botão 'MIN' foi pressionado?...
            {
                while(!tst_bit(PIND, MIN));
                _delay_ms(150);
                minutos++;                    //incrementa os minutos
                if(minutos == 60) minutos = 0; 
            }
            else if(!tst_bit(PIND, HORA))    //o botão 'HORA' foi pressionado?...
            {
                while(!tst_bit(PIND, HORA));
                _delay_ms(150);
                horas++;                      //incrementa as horas
                if(horas == 24) horas = 0; 
            }
        }
    }
}

ISR(TIMER0_OVF_vect) //implementação da rotina de tratamento do estouro do TCNT0
{
    static unsigned char conta_tempo = 0; //variável auxiliar para contagem de tempo
    conta_tempo++;
    if(conta_tempo == 2)
    {
        cpl_bit(PORTD, SINAL_OUT);
        conta_tempo = 0; 
    }
}

ISR(PCINT2_vect) //declaração da rotina de tratamento da interrupção externa
{
    static unsigned char captura = 0; //variável auxiliar para contagem do tempo
    captura++;                        //incrementa a variável auxiliar a cada borda do sinal
    if(captura == 120)                //se passou um segundo(ocorreram 120 capturas)?...
    {
        segundos++;                   //incrementa 'segundos'
        if(segundos == 60)            //se passou um minuto?...
        {
            segundos = 0;             //reinicializa 'segundos'
            minutos++;                //incrementa 'minutos'
            if(minutos == 60)         //se passou uma hora?...
            {
                minutos = 0;          //reinicializa 'minutos'
                horas++;              //incrementa 'horas'
                if(horas == 24)       //se passou um dia?...
                {
                    horas = 0;        //reinicializa 'horas'
                    minutos = 0;      //reinicializa 'minutos'
                    segundos = 0;     //reinicializa 'segundos'
                }
            }
        }
        captura = 0;                  //reinicializa 'captura'
    }  
}

ISR(TIMER2_OVF_vect) //implementação da rotina de tratamento do estouro do TCNT2
{
    static unsigned char disp = 0; //variável auxiliar de acionamento dos displays
    TCNT2 = 212;                   //reinicia o registrador do TC2
    PORTC &= 0xC0;                 //desabilita todos os display
    if(disp == 0)      PORTB = pgm_read_byte(&display[segundos % 10]); //carrega a unidade dos segundos
    else if(disp == 1) PORTB = pgm_read_byte(&display[segundos / 10]); //carrega a dezena dos segundos
    else if(disp == 2) PORTB = pgm_read_byte(&display[minutos % 10]);  //carrega a unidade dos minutos
    else if(disp == 3) PORTB = pgm_read_byte(&display[minutos / 10]);  //carrega a dezena dos minutos
    else if(disp == 4) PORTB = pgm_read_byte(&display[horas % 10]);    //carrega a unidade das horas
    else if(disp == 5) PORTB = pgm_read_byte(&display[horas / 10]);    //carrega a dezena das horas
    if(disp == 4 || disp == 2) set_bit(PORTC, PC6);                    //separador das horas dos minutos e dos minutos dos segundos
    else clr_bit(PORTC, PC6);
	set_bit(PORTC, disp);          //habilita o display 'disp'
    disp++;                        //passa para o próximo display que será habilitado
    if(disp == 6) disp = 0;        //se o número de displays for excedido, volta para o primeiro
}
