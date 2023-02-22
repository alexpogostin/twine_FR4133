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

/*****************************************************************************/
int asciiToBin(char* line)
{
    int i;
    int len = 0;
    int mult[4] = {1000, 100, 10, 1};
    int total = 0;

    if(line[0] == 0x00)
    {
        return -1;
    }

    len++;

    for(i=len;i<8;i++)
    {
        if(line[i] == 0)
        {
            break;
        }
        len++;
    }

    for(i=0;i<len;i++)
    {
        if(line[i] < 0x30 || line[i] > 0x39)
        {
            return -1;
        }
    }

    for(i=0;i<len;i++)
    {
        total = total +((line[i] & 0x0F) * mult[i+(4-len)]);
    }

    return total;
}
