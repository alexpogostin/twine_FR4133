/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * interrupts.c
 *
 * Twine interrupt routine(s)
 *
 * Copyright (C) 2022 Alex Pogostin <alex.pogostin@outlook.com>
 *
 */

#include "twine.h"

/*****************************************************************************/
/* externs - these global variables are declared in tasks.c                  */
/*****************************************************************************/
extern unsigned short task_1_status;
extern unsigned short task_2_status;
extern unsigned short task_3_status;
extern unsigned short task_4_status;
extern short task_1_timeout;
extern short task_2_timeout;
extern short task_3_timeout;
extern short task_4_timeout;

/*****************************************************************************/
/* global declarations                                                       */
/*****************************************************************************/
unsigned char uartRxBuf[UART_RX_BUF_SIZE];
unsigned char uartTxBuf[UART_TX_BUF_ARRAY_SIZE][UART_TX_BUF_SIZE];

/*****************************************************************************/
/* Reference to the vector pragmas can be found in slau132y.pdf v21.6.0.LTS   */
/*****************************************************************************/
#pragma vector = RTC_VECTOR
/*****************************************************************************/
/* RTC interrupt, initialization is done in clocks.c                         */
/**********************************************************AJP****************/
__interrupt void RTCIntervalInterrupt(void)
{
    PM5CTL0 &= ~LOCKLPM5;

    __data16_read_addr(RTCIV);

    RTCMOD = RTC_MOD_COUNT;

    if(task_1_status & BIT0)
        task_1_timeout--;

    if(task_2_status & BIT0)
        task_2_timeout--;

    if(task_3_status & BIT0)
        task_3_timeout--;

    if(task_4_status & BIT0)
        task_4_timeout--;
}

/*****************************************************************************/
#pragma vector = USCI_A0_VECTOR
/*****************************************************************************/
__interrupt void USCIA0Interrupt(void)
{

    volatile unsigned char rxReg;
    static unsigned int i, j;
    static unsigned l; // used to count steps needed for backspace
    static unsigned s; // used to stop backspace at uartRxBuf[0]
    unsigned int k;

    switch(__even_in_range(UCA0IV, 18))
    {
        case 0x00: // No interrupts
        break;

        case 0x02: // RX interrupt
            rxReg = read_reg_8(EUSCI_A0_BASE + 0xC);
            if(i < UART_RX_BUF_SIZE - 1)
            {
                if(rxReg == BACKSPACE)
                {
                    uartRxBuf[i] = NULL;
                    if(i > 0)
                    {
                        i--;
                        s = 0;
                    }
                }
                else if(rxReg == CR)
                {
                    uartRxBuf[i++] = rxReg;
                    i = 0;
                }
                else if(rxReg == 0x03)
                {
                    uartRxBuf[0] = CTRL_C;
                    i = 0;
                }
                else
                {
                    uartRxBuf[i++] = rxReg;
                }
            }
            else
            {
                uartRxBuf[i] = rxReg;
                i = 0;
            }

            if(!(UCA0IFG & UCTXIE))
                UCA0IFG |= UCTXIE;
        break;

        case 0x04: // TX interrupt

            if(rxReg == BACKSPACE && l < 3 && i >= 0 && s == 0)
            {
                if(l == 0)
                {
                    l++;
                    UCA0TXBUF = BACKSPACE;
                }
                else if(l == 1)
                {
                    l++;
                    UCA0TXBUF = SPACE;
                }
                else if(l == 2)
                {
                    UCA0TXBUF = BACKSPACE;
                    rxReg = NULL;
                    l = 0;
                    if(i == 0)
                        s = 1;
                }
            }

            if((rxReg >= SPACE) && (rxReg < DEL))
            {
                UCA0TXBUF = rxReg;
                rxReg = NULL;
            }

loop:
            if(k < 4)
            {
                if(uartTxBuf[k][j])
                {
                    UCA0TXBUF = uartTxBuf[k][j++];
                    uartTxBuf[k][j-1] = NULL;
                }
                else
                {
                    j = 0;
                    k++;
                    goto loop;
                }
            }
            else
                k = 0;
        break;

        case 0x06: // Start bit interrupt
            UCA0IFG &= ~UCSTTIFG;
        break;

        case 0x08: // TX complete interrupt
            UCA0IFG &= ~UCTXCPTIFG;
        break;

        default:
        break;
    }
}
