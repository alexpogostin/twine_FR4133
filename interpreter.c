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
extern unsigned char intVars[4][8];

extern unsigned char uartRxBuf[UART_RX_BUF_SIZE];
extern unsigned char uartTxBuf[UART_TX_BUF_ARRAY_SIZE][UART_TX_BUF_SIZE];

extern unsigned short task_4_stack_size;
extern unsigned short task_4_stack[8];
extern unsigned short taskManager_stack_4;

/*****************************************************************************/
/* static (local) declarations                                               */
/*****************************************************************************/
unsigned short runState;
int blockRunIndex;
int val;
int blockRepeat;

/*****************************************************************************/
int interpreter(int *tokenTreeIndex, unsigned char *tokenTree) // abstract syntax tree
{
    unsigned short i = 0;
    unsigned short j = 0;

    task_4_stack_size = ((taskManager_stack_4 - (unsigned short) __get_SP_register())>>1) + 1;

    while(uartRxBuf[i])
    {
        if(uartRxBuf[i] == CTRL_C)
        {
            for(j=0;j<UART_RX_BUF_SIZE;j++)
            {
                uartRxBuf[j] = NULL;
            }
            debug(uartTxBuf, 0, "ctrl-c\r\n");
            uartRxBuf[0] = CR;
            runState = 0x01; // program terminated
            break;
        }
        i++;
    };

    if(*(tokenTreeIndex) == 0)
    {
        runState = tokenTree[0];
    }

    // get value
    // get run state from token after value, "next run state"
    // run state = token token end ".", TOKEN_END="T"
    // 1: uart "Hello World"
    // 2: repeat (10).
    // 3: uart "This is a test"
    // 4: uart "What a world"
    // 5: repeat (10).

    // BV0 HX0
    // repeat, value, 10, uart, string, "test"

    switch(runState)
    {
        case 'H': // 0x48 uart
        debug(uartTxBuf, 0, "H\r\n");
        runState = _uart_lcd(tokenTreeIndex, tokenTree);
        break;

        case 'U': // 0x55 lcd
        debug(uartTxBuf, 0, "U\r\n");
        runState = _uart_lcd(tokenTreeIndex, tokenTree);
        break;

        case 'X': // 0x58 string
        debug(uartTxBuf, 0, "X\r\n");
        runState = _string(tokenTreeIndex, tokenTree);
        break;

        case 'B': // 0x42 repeat
        debug(uartTxBuf, 0, "B\r\n");
        runState = _repeat(tokenTreeIndex, tokenTree);
        break;

        case 'V': // 0x56 value
        debug(uartTxBuf, 0, "V\r\n");
        runState = _value(&val, tokenTreeIndex, tokenTree);
        blockRepeat = val;
        blockRunIndex = *(tokenTreeIndex);
        break;

        case 'S': // 0x53 rerun
        debug(uartTxBuf, 0, "S\r\n");
        _rerun(tokenTreeIndex);
        break;

        case 'T': // 0x54 end of block
        debug(uartTxBuf, 0, "T\r\n");
        blockRepeat--;
        if(blockRepeat <= 0)
        {
            runState = _end(tokenTreeIndex, tokenTree);
        }
        else
        {
            *(tokenTreeIndex) = blockRunIndex;
            runState = tokenTree[*(tokenTreeIndex)];
        }
        break;

        case 'D': // 0x44 pause
        debug(uartTxBuf, 0, "D\r\n");
        taskSleep(1);
        runState = _end(tokenTreeIndex, tokenTree);
        break;

        case 'R': // 0x52 finish
        uartTx(uartTxBuf, 0, "program finished\r\n");
        _finish(tokenTreeIndex);
        break;

        case 'M': // 0x4D led
        debug(uartTxBuf, 0, "M\r\n");
        runState = _led(tokenTreeIndex, tokenTree);
        break;

        case 'I': // 0x49 on
        debug(uartTxBuf, 0, "I\r\n");
        runState = _on(tokenTreeIndex, tokenTree);
        break;

        case 'J': // 0x4A off
        debug(uartTxBuf, 0, "J\r\n");
        runState = _off(tokenTreeIndex, tokenTree);
        break;

        case 0x01: // ctrl-c
        uartTx(uartTxBuf, 0, "program terminated\r\n");
        _finish(tokenTreeIndex);
        break;

#if 0
        case 'A': // is
        debug(uartTxBuf, 0, "A\r\n");
        runState = tokenTree[tokenTreeIndex++];
        break;

        case 'K': // yes:
        debug(uartTxBuf, 0, "K\r\n");
        runState = tokenTree[tokenTreeIndex++];
        break;

        case 'L': // no:
        debug(uartTxBuf, 0, "L\r\n");
        runState = tokenTree[tokenTreeIndex++];
        break;

        case 'O': // but1
        debug(uartTxBuf, 0, "O\r\n");
        runState = tokenTree[tokenTreeIndex++];
        break;

        case 'P': // but2
        debug(uartTxBuf, 0, "P\r\n");
        runState = tokenTree[tokenTreeIndex++];
        break;

        case 'Q': // pressed
        debug(uartTxBuf, 0, "Q\r\n");
        runState = tokenTree[tokenTreeIndex++];
        break;

#endif

        default:
        uartTx(uartTxBuf, 0, "program error\r\n");
        _finish(tokenTreeIndex);
        break;
    }

    return 0;

}

/*****************************************************************************/
int _uart_lcd(int *tokenTreeIndex, unsigned char *tokenTree)
{
    int runState;

    if(tokenTree[++(*tokenTreeIndex)] == 'X') // is next token a string
    {
        runState = tokenTree[(*tokenTreeIndex)]; // set runState to string
    }
    else
    {
        runState = 0xFF;
    }

    return runState;
}

/*****************************************************************************/
int _string(int *tokenTreeIndex, unsigned char *tokenTree)
{
    int i;
    int j;
    int k;

    if(tokenTree[(*tokenTreeIndex - 1)] == 'H')// is previous token uart
    {
        i = tokenTree[++(*tokenTreeIndex)];
        uartTx(uartTxBuf, 0, &strVars[i & 0x0F][0]);
        uartTx(uartTxBuf, 0, CRLF);
        i = tokenTree[++(*tokenTreeIndex)];
    }
    else if (tokenTree[(*tokenTreeIndex - 1)] == 'U') // is previous token lcd
    {
        i = tokenTree[++(*tokenTreeIndex)];

        writeLcd(0,0); // clear all 5 digits

        for(k=0;k<32;k++) // look for end of strVar[][]
        {
            if(strVars[i & 0x0F][k] == 0)
                break;
        }

        for(j=0;j<k;j++)
        {
            writeLcd(strVars[i & 0x0F][j], j + 1);
        }

        i = tokenTree[++(*tokenTreeIndex)];
    }
    else
    {
        ++(*tokenTreeIndex);
        i = 0xFF;
    }

    return i; // runState
}

/*****************************************************************************/
int _led(int *tokenTreeIndex, unsigned char *tokenTree)
{
    int runState;
    unsigned char token;

    token = tokenTree[++(*tokenTreeIndex)];

    if(token == 'I' || token == 'J') // is next token on or off
    {
        runState = tokenTree[(*tokenTreeIndex)]; // set runState to on or off
    }
    else
    {
        runState = 0xFF;
    }

    return runState;
}

/*****************************************************************************/
int _on(int *tokenTreeIndex, unsigned char *tokenTree)
{
    int i;

    if(tokenTree[(*tokenTreeIndex - 1)] == 'M') // is previous token led
    {
        debug(uartTxBuf, 0, "led on\r\n");
        P4OUT |= BIT0; // turn on user LED2
        i = tokenTree[++(*tokenTreeIndex)];
    }
    else
    {
        ++(*tokenTreeIndex);
        i = 0xFF;
    }

    return i; // runState
}

/*****************************************************************************/
int _off(int *tokenTreeIndex, unsigned char *tokenTree)
{
    int i;

    if(tokenTree[(*tokenTreeIndex - 1)] == 'M') // is previous token led
    {
        debug(uartTxBuf, 0, "led off\r\n");
        P4OUT &= ~BIT0; // turn off user LED2
        i = tokenTree[++(*tokenTreeIndex)];
    }
    else
    {
        ++(*tokenTreeIndex);
        i = 0xFF;
    }

    return i; // runState
}

/*****************************************************************************/
int _repeat(int *tokenTreeIndex, unsigned char *tokenTree)
{
    int runState;

    if(tokenTree[++(*tokenTreeIndex)] == 'V') // is next token a value
    {
        runState = tokenTree[(*tokenTreeIndex)]; // set runState to value
    }
    else
    {
        runState = 0xFF;
    }

    return runState;
}

/*****************************************************************************/
int _value(int *val, int *tokenTreeIndex, unsigned char *tokenTree)
{
    short i;

    if(tokenTree[(*tokenTreeIndex - 1)] == 'B') // is previous token repeat
    {
        i = tokenTree[++(*tokenTreeIndex)];
        //uartTx(uartTxBuf, 0, &intVars[i & 0x0F][0]);
        //uartTx(uartTxBuf, 0, CRLF);
        *(val) = asciiToBin(&intVars[i & 0x0F][0]);
        i = tokenTree[++(*tokenTreeIndex)];
    }
    else
    {
        ++(*tokenTreeIndex);
        i = 0xFF;
    }

    return i; // runState
}

/*****************************************************************************/
int _end(int *tokenTreeIndex, unsigned char *tokenTree)
{
    int runState;
    runState = tokenTree[++(*tokenTreeIndex)];
    return runState;
}

/*****************************************************************************/
int _finish(int *tokenTreeIndex)
{
    taskControl(TASK4, DISABLE);
    taskControl(TASK1, ENABLE);
    (*tokenTreeIndex) = 0;
    return TRUE;
}

/*****************************************************************************/
int _rerun(int *tokenTreeIndex)
{
    (*tokenTreeIndex) = 0;
    return TRUE;
}

/*****************************************************************************/
int _pressed(void)
{
    return TRUE;
}
