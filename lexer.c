/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * lexer.c
 *
 * Lexer routines.
 *
 * Copyright (C) 2022 Alex Pogostin <alex.pogostin@outlook.com>
 *
 */

#include "twine.h"

/*****************************************************************************/
/* global declarations                                                       */
/*****************************************************************************/
extern unsigned char uartTxBuf[UART_TX_BUF_ARRAY_SIZE][UART_TX_BUF_SIZE];

unsigned char strVars[4][32];
unsigned char intVars[4][8];

unsigned char tokenTree[MAX_TOKEN_TREE_SIZE];
unsigned char tokenTreeAst[16];
int tokenTreeIndex;

int strVarsIndex;
int strNum;

int intVarsIndex;
int intNum;

/*****************************************************************************/
/* static (local) declarations                                               */
/*****************************************************************************/
static unsigned char *token[MAX_NUM_TOKENS] = {TOKEN_IS,
                                               TOKEN_REPEAT,
                                               TOKEN_SET,
                                               TOKEN_PAUSE,
                                               TOKEN_CALL,
                                               TOKEN_EQ,
                                               TOKEN_IN,
                                               TOKEN_UART,
                                               TOKEN_ON,
                                               TOKEN_OFF,
                                               TOKEN_YES,
                                               TOKEN_NO,
                                               TOKEN_LED1,
                                               TOKEN_LED2,
                                               TOKEN_BUT1,
                                               TOKEN_BUT2,
                                               TOKEN_PRESSED,
                                               TOKEN_FINISH,
                                               TOKEN_RERUN,
                                               TOKEN_END,
                                               TOKEN_LCD,
                                               TOKEN_FINAL,
                                               };

/*****************************************************************************/
int lexer(unsigned char *program)
{
    int i = 0;
    int progLineLen;
    unsigned char *p = program;
    int tokenNum = 0;
    int tokenIndex = 0;

    for(progLineLen=0;progLineLen<MAX_PROG_LINE_LEN;progLineLen++)
    {
        if(!*(program + progLineLen) || *(program + progLineLen) < SPACE)
            break;
    }

    for(tokenNum=0;tokenNum<MAX_NUM_TOKENS;) // search through list of tokens
    {
        if(*(p) == 0x0D)
            break;

        while(*(p) == 0x20)
            p++;

        if(*(p) == 0x28)
        {
            p++;

            i = 0;
            while(*(p) != 0x29)
            {
                if(*(p) == 0x0D)
                {
                    goto endOfLineReached;
                }
                intVars[intVarsIndex][i++] = *(p++);
            }
            p++;
            intVarsIndex++;
            tokenTree[tokenTreeIndex++] = 'V';
            tokenTree[tokenTreeIndex++] = intNum++;
            tokenNum = 0;
            continue;
        }

        if(*(p) == 0x22)
        {
            p++;
            i = 0;
            while(*(p) != 0x22)
            {
                if(*(p) == 0x0D)
                {
                    goto endOfLineReached;
                }
                strVars[strVarsIndex][i++] = *(p++);
            }
            p++;
            strVarsIndex++;
            tokenTree[tokenTreeIndex++] = 'X';
            tokenTree[tokenTreeIndex++] = strNum++;
            tokenNum = 0;
            continue;
        }

        if(*(p) == *(token[tokenNum] + TOKEN_HEADER_LEN)) // does character match the first letter of token
        {
            for(tokenIndex=0;tokenIndex < *(token[tokenNum] + TOKEN_SIZE);tokenIndex++) // match each character of token
            {
                if(*(p + tokenIndex) != *(token[tokenNum] + TOKEN_HEADER_LEN + tokenIndex))
                {
                    goto tokenNotFound;
                }
            }
            tokenTree[tokenTreeIndex++] = *(token[tokenNum] + TOKEN);
            p += *(token[tokenNum] + TOKEN_SIZE);
            tokenNum = 0;
            continue;
        }

tokenNotFound:
        tokenNum++;
    }
    return TRUE;

endOfLineReached:
    return FALSE;
}

int ast(unsigned char *tokenTree)
{
    short statementPos = 0;
    short conditionalPos;
    short blockPos;
    short i;
    short j;
    short k;

    debug(uartTxBuf, 0, "AST\r\n");
    debug(uartTxBuf, 0, tokenTree);
    debug(uartTxBuf, 0, "\r\n");

    // uart "test"
    // pause
    // repeat (10).
    // HX0DBV0T
    // BV0DHX0T

    // pause
    // uart "test"
    // repeat (10).
    // DHX0BV0T
    // BV0DHX0T

    // look for terminator (.)
    // look for control flow (repeat)
    // look for block (uart)

    for(i=0;i<MAX_TOKEN_TREE_SIZE;i++)
        tokenTreeAst[i] = 0;

    /*
    0123456789012
    DTDTHX0BV0TRT

    tokenTreeIndex = 0
    statementLen   = 1

    tokenTreeIndex = 2
    statementLen   = 1

    tokenTreeIndex = 4
    statementLen   = 3

    tokenTreeIndex = 7
    statementLen   = 3

    tokenTreeIndex = 11
    statementLen   = 1
    */

    tokenTreeIndex = 0;

    do
    {
        for(statementPos=tokenTreeIndex;statementPos<MAX_TOKEN_TREE_SIZE;statementPos++)
        {
            if(tokenTree[statementPos] == 'T')
            {
                break;
            }
        }

        for(conditionalPos=tokenTreeIndex;conditionalPos<statementPos;conditionalPos++)
        {
            if(tokenTree[conditionalPos] == 'B') // repeat
            {
                for(i=0;i<3;i++)
                {
                    tokenTreeAst[i+tokenTreeIndex] = tokenTree[conditionalPos+i];
                }
            }
        }

        for(blockPos=tokenTreeIndex;blockPos<statementPos;blockPos++)
        {
            if(tokenTree[blockPos] == 'D') // pause
            {
                tokenTreeAst[k+i+tokenTreeIndex] = tokenTree[blockPos];
                continue;
            }

            if(tokenTree[blockPos] == 'R') // finish
            {
                tokenTreeAst[k+i+tokenTreeIndex] = tokenTree[blockPos];
                continue;
            }

            if(tokenTree[blockPos] == 'H') // uart
            {
                for(k=0;k<3;k++)
                {
                    tokenTreeAst[k+i+tokenTreeIndex] = tokenTree[blockPos+k];
                }
            }
        }

        tokenTreeAst[statementPos] = 'T';
        tokenTreeIndex += statementPos;
    }
    while(tokenTree[tokenTreeIndex]);

    debug(uartTxBuf, 0, tokenTreeAst);
    debug(uartTxBuf, 0, "\r\n");

    return TRUE;
}
