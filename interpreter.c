/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * interpreter.c
 *
 * Interpreter routines.
 *
 * Copyright (C) 2022 Alex Pogostin <alex.pogostin@outlook.com>
 *
 */

#include "twine.h"

/*****************************************************************************/
/* global declarations                                                       */
/*****************************************************************************/
extern unsigned char strVars[4][32];

extern unsigned char uartRxBuf[UART_RX_BUF_SIZE];
extern unsigned char uartTxBuf[UART_TX_BUF_ARRAY_SIZE][UART_TX_BUF_SIZE];

extern unsigned short task_4_stack_size;
extern unsigned short task_4_stack[8];
extern unsigned short taskManager_stack_4;

/*****************************************************************************/
/* static (local) declarations                                               */
/*****************************************************************************/
static unsigned short runState;

/*****************************************************************************/
int interpreter(unsigned char *tokenTree) // abstract syntax tree
{
    static int i = 0;
    int j = 0;
    int k = 0;

    task_4_stack_size = ((taskManager_stack_4 - (unsigned short) __get_SP_register())>>1) + 1;

    if(!i)
    {
        runState = tokenTree[0];
    }

    while(uartRxBuf[j])
    {
        if(uartRxBuf[j] == CTRL_C)
        {
            for(k=0;k<UART_RX_BUF_SIZE;k++)
            {
                uartRxBuf[k] = NULL;
            }
            debug(uartTxBuf, 0, "ctrl-c\r\n");
            uartRxBuf[0] = CR;
            runState = 0x01; // program terminated
            break;
        }
        j++;
    };

    switch(runState)
    {
        case 'A': // is
        debug(uartTxBuf, 0, "A\r\n");
        runState = tokenTree[++i];
        break;

        case 'D': // pause
        debug(uartTxBuf, 0, "D\r\n");
        taskSleep(1);
        runState = tokenTree[++i];
        break;

        case 'H': // out
        debug(uartTxBuf, 0, "H\r\n");
        runState = tokenTree[++i];
        break;

        case 'K': // yes:
        debug(uartTxBuf, 0, "K\r\n");
        runState = tokenTree[++i];
        break;

        case 'L': // no:
        debug(uartTxBuf, 0, "L\r\n");
        runState = tokenTree[++i];
        break;

        case 'M': // led1
        debug(uartTxBuf, 0, "M\r\n");
        runState = tokenTree[++i];
        break;

        case 'N': // led2
        debug(uartTxBuf, 0, "N\r\n");
        runState = tokenTree[++i];
        break;

        case 'O': // but1
        debug(uartTxBuf, 0, "O\r\n");
        runState = tokenTree[++i];
        break;

        case 'P': // but2
        debug(uartTxBuf, 0, "P\r\n");
        runState = tokenTree[++i];
        break;

        case 'Q': // pressed
        debug(uartTxBuf, 0, "Q\r\n");
        runState = tokenTree[++i];
        break;

        case 'R': // finish
        uartTx(uartTxBuf, 0, "program finished\r\n");
        taskControl(TASK4, DISABLE);
        taskControl(TASK1, ENABLE);
        i = 0;
        break;

        case 'S': // rerun
        debug(uartTxBuf, 0, "S\r\n");
        i = 0;
        break;

        case 'X': // string #1
        debug(uartTxBuf, 0, "1\r\n");
        j = tokenTree[++i];
        uartTx(uartTxBuf, 0, &strVars[j & 0x0F][0]);
        uartTx(uartTxBuf, 0, CRLF);
        runState = tokenTree[++i];
        break;

        case 0x01: // ctrl-c
        uartTx(uartTxBuf, 0, "program terminated\r\n");
        taskControl(TASK4, DISABLE);
        taskControl(TASK1, ENABLE);
        i = 0;
        break;

        default:
        uartTx(uartTxBuf, 0, "program error\r\n");
        taskControl(TASK4, DISABLE);
        taskControl(TASK1, ENABLE);
        i = 0;
        break;
    }

    return 0;
}

/*****************************************************************************/
/* TODO (maybe?): include calls below in switch(...) above??
/*****************************************************************************/

/*****************************************************************************/
int _is(void) // but1 or but2
{
    // RULE: 'is' only tests inputs, for example but1 or but2.
    return TRUE;
}

/*****************************************************************************/
int _equal(void)
{
    // RULE: can only set an output, for example led1 or led2
    return TRUE;
}

/*****************************************************************************/
int _yes(void)
{
    return TRUE;
}

/*****************************************************************************/
int _no(void)
{
    return TRUE;
}

/*****************************************************************************/
int _led1(void)
{
    // turn on or off led1
    return TRUE;
}

/*****************************************************************************/
int _led2(void)
{
    // turn on or off led2
    return TRUE;
}

/*****************************************************************************/
int _but1(void)
{
    // is but1 pressed?
    return TRUE;
}

/*****************************************************************************/
int _but2(void)
{
    // is but2 pressed?
    return TRUE;
}

/*****************************************************************************/
int _pressed(void)
{
    return TRUE;
}
