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
/* global declarations                                                       */
/*****************************************************************************/
#define OFS_UCA0RXBUF 0x0C
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

// Ran into an issue (bug??) which seemed to overwrite rxReg when it was
// defined within the USCIA0Interrupt(...) interrupt routine. Moving
// into global space fixed. In some cases the character 'd' would be
// printed to the UART. Breaking into the USCIA0Interrupt(...) found
// that rxReg was changing to a 'd'. Disassembling the code, found that
// the MOV.B used to assign it from 0x50C was the SP, which was pointing
// to address 0x0000?? Odd issue - not sure if its a coding thing, a compiler
// thing or something else?? Update: clearly a stack pointer getting out
// of whack - found that the 'd' (0x64) was in the stack, somehow
// the stack pointer is being changed when coming back into the UART ISR,
// but can't be sure its part of the twine's task manager, or something
// in how the TI430 is managing things in the ISR??
volatile unsigned char rxReg;

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
    static unsigned int i, j;
    static unsigned int l; // used to count steps needed for backspace
    static unsigned int s; // used to stop backspace at uartRxBuf[0]
    unsigned int k;

    switch(__even_in_range(UCA0IV, 18)) // Interrupt Vector Register
    {
        case 0x00: // No interrupts
        break;

        case 0x02: // RX interrupt
            rxReg = read_reg_8(EUSCI_A0_BASE + OFS_UCA0RXBUF);
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
                else if(rxReg == CTRL_C)
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
            if(rxReg == BACKSPACE && l < 3 && (i > 0 || i == 0) && s == 0)
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
