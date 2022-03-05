/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * string.c
 *
 * String routines.
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
char* binToAscii(char* line, short val)
{
    if(val < 10)
    {
        line[2] = ' ';
        line[3] = (unsigned char) val + ASCII_0;
    }
    else if(val > 9)
    {

        line[2] = (unsigned char) (val/10) + ASCII_0;
        line[3] = (unsigned char) (val - ((val/10) * 10)) + ASCII_0;
    }

    return line;
}
