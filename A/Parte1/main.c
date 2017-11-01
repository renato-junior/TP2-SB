/******************************************************************************
 *                   UNIVERSIDADE FEDERAL DE MINAS GERAIS                     *
 *                      ESCOLA DE VERÃO EM COMPUTAÇÃO                         *
 ******************************************************************************
 *                      MINICURSO SISTEMAS EMBARCADOS                         *
 *             PROF. JOSÉ AUGUSTO MIRANDA NACIF - JNACIF@UFV.BR               *
 *                        PRÁTICA #3 - TEMPORIZADOR                           *
 ******************************************************************************
 *                          DESCRIÇÃO DO EXEMPLO                              *
 *     O exemplo  configura  o Timer A para gerar uma interrupção periódica a *
 * cada 1 segundo. Quando a rotina de tratamento de interrupção é  executada, *
 * o valor do LED vermelho (P1.0) é invertido.                                *
 ******************************************************************************
 *                        ATIVIDADE PRÁTICA PROPOSTA                          *
 *     Altere o exemplo, utilizando o temporizador para simular um  semafóro, * 
 * no qual:                                                                   *
 * - O LED vermelho (P1.0) permaneça ativado por 10 segundos                  *
 * - O LED verde (P1.6) permaneça ativado por 20 segundos                     *
 ******************************************************************************/
#include "msp430g2553.h"

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Desliga Watchdog timer
    P1DIR = 0x01 + 0x40;      // Define pinos 1.0 e 1.6 como saída (0100 0001)
    P1REN = 0x08;             // Habilita pullup/pulldown do pino 1.3 (0000 1000)
    P1OUT = 0x08;             // Define pullup para o pino 1.3 (0000 1000)

    CCTL0 = CCIE;                   // Habilita interrupção de comparação do timer A
    TACTL = TASSEL_2 + MC_3 + ID_3; // SMCLK = 1 MHz, SMCLK/8 = 125 KHz (8 us)
    CCR0 = 62500;                   // Modo up/down: chega no valor e depois volta
                                    // para zero, portanto cada interrupção acontece
                                    // 2 * 62500 * 8 us = 1 segundo
    _BIS_SR(CPUOFF + GIE);
    while (1);
}
// Timer A0 interrupt service routine
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    P1OUT = P1OUT ^ 0x01; // Inverte saida no pino 1.0 com ou-exclusivo
}