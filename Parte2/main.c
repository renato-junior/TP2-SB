#include "msp430g2553.h"

void main(void) {
    int count = 0;
    WDTCTL = WDTPW + WDTHOLD; // Desliga Watchdog timer
    P1DIR = 0x01 + 0x40; // Define pinos 1.0 e 1.6 como saída (0100 0001)
    P1REN = 0x08; // Habilita pullup/pulldown do pino 1.3 (0000 1000)
    P1OUT = 0x08; // Define pullup para o pino 1.3 (0000 1000)
    P1IE = 0x08; // Habilita interrupção no pino 1.3 (00001000)
    P1IFG = 0x00; // Zera flag de interrupção da porta 1 (00000000)
    P1OUT |= 0x40; // Seta o valor do LED verde para 1, ligando-o
    P1OUT &= 0xfe // Seta o valor do LED vermelho para 0, desligando-o

    CCTL0 = CCIE;                   // Habilita interrupÃ§Ã£o de comparaÃ§Ã£o do timer A
    TACTL = TASSEL_2 + MC_3 + ID_3; // SMCLK = 1 MHz, SMCLK/8 = 125 KHz (8 us)
    CCR0 = 62500;                   // Modo up/down: chega no valor e depois volta
                                    // para zero, portanto cada interrupÃ§Ã£o acontece
                                    // 2 * 62500 * 8 us = 1 segundo
      
    _BIS_SR(CPUOFF + GIE); // Entra em modo de baixo consumo e habilita interrupções
    while(1);
}


#pragma vector = #pragma vector=PORT1_VECTOR // Rotina de tratamento de interrupção da porta 1
__interrupt void Port_1(void){
  __delay_cycles(100000); // Gera um atraso
  count++;
  if(count == 8) // se passou 8 segundos com o LED verde ativo
  {              // liga o "amarelo"
      P1OUT |= 0x01; // ativa o LED vermelho para formar o amarelo
  }
  else if(count == 9) // se passou 1 segundos com o LED "amarelo" ativo
  {                   // liga o vermelho
      P1OUT &= ~(0x40); // desativa o LED verde
  }
  else if(count >= 11) // se passou 2 segundos com o LED vermelho ativo
  {                   // retorna a ativar o verde e desligar o vermelho e reseta o contador
      P1OUT |= 0x40;  // Ativa o LED verde
      P1OUT &= 0xfe; // Desativa o LED vermelho
      count = 0;
  }
  P1IFG = 0x00; // Zera flag de interrupção da porta 1 (00000000)
}