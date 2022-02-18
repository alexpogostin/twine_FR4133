/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * main.c
 *
 * C entry point for twine.
 *
 * Copyright (C) 2022 Alex Pogostin <alex.pogostin@outlook.com>
 *
 */

#include "twine.h"

/*****************************************************************************/
/* extern declarations                                                       */
/*****************************************************************************/
extern unsigned char uartTxBuf[UART_TX_BUF_ARRAY_SIZE][UART_TX_BUF_SIZE];

/*****************************************************************************/
/* static (local) declarations                                               */
/*****************************************************************************/
static unsigned char banner[]  = "\r\ntwine\r\n# ";

/*****************************************************************************/
int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;       // disable watchdog timer

    gpiosInit();
    powerInit();
    startXt1Clk();
    startRtcClk();
    uartInit();

    // If this is a power up then wait for USB enumeration.
    // Allows use with "backchannel" UART-over-USB at power-up.
    RTCMOD = 0xF00;
    RTCCTL |= RTCSR;
    if(SYSRSTIV == 0x0002) // test for BOR
    {
        while(read_reg_16(RTC_BASE + 0x0C) < RTC_MOD_COUNT);
    }
    RTCMOD = RTC_MOD_COUNT;

    __enable_interrupt();

    uartTx(uartTxBuf, 0, banner);

    taskManager();

    return 0;
}
