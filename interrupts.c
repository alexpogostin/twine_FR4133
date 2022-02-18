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

    if(task_1_status)
        task_1_timeout--;

    if(task_2_status)
        task_2_timeout--;

    if(task_3_status)
        task_3_timeout--;

    if(task_4_status)
        task_4_timeout--;
}

/*****************************************************************************/
#pragma vector = USCI_A0_VECTOR
/*****************************************************************************/
__interrupt void USCIA0Interrupt(void)
{

    volatile unsigned char rxReg;
    static unsigned int i, j, k;

    switch(__even_in_range(UCA0IV, 18))
    {
        case 0x00: // No interrupts
        break;

        case 0x02: // RX interrupt
            rxReg = read_reg_8(EUSCI_A0_BASE + 0xC);
            if(i < UART_RX_BUF_SIZE - 1)
            {
                uartRxBuf[i++] = rxReg;

                if(rxReg == 0x0D)
                    i = 0;
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

            if((rxReg >= 0x20) && (rxReg < 0x7f))
            {
                UCA0TXBUF = rxReg;
                rxReg = 0;
            }

loop:
            if(k < 4)
            {
                if(uartTxBuf[k][j])
                {
                    UCA0TXBUF = uartTxBuf[k][j++];
                    uartTxBuf[k][j-1] = 0x00;
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
