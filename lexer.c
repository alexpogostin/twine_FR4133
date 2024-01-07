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

unsigned char strVars[MAX_STRING_VARS][MAX_STRING_LEN];
unsigned char intVars[MAX_INT_VARS][MAX_INT_LEN];

unsigned char tokenTree[MAX_TOKEN_TREE_SIZE];
unsigned char tokenTreeAst[MAX_TOKEN_TREE_SIZE];
int tokenTreeIndex;

int strVarsIndex;
int strNum;

int intVarsIndex;
int intNum;

#pragma PERSISTENT(token)

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
                                               TOKEN_LED,
                                               TOKEN_BUT1,
                                               TOKEN_BUT2,
                                               TOKEN_PRESSED,
                                               TOKEN_FINISH,
                                               TOKEN_RERUN,
                                               TOKEN_END,
                                               TOKEN_LCD,
                                               TOKEN_FINAL
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
        if(*(p) == CR) // 0x0D
            break;

        while(*(p) == SPACE) // 0x20
            p++;

        if(*(p) == OPEN_PARENTHESIS) // 0x28
        {
            p++;
            i = 0;
            while(*(p) != CLOSE_PARENTHESIS) // 0x29
            {
                if(*(p) == CR)
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

        if(*(p) == DOUBLE_QUOTE) // 0x22
        {
            p++;
            i = 0;
            while(*(p) != DOUBLE_QUOTE) // 0x22
            {
                if(*(p) == CR)
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
    short statementEnd;
    short conditionalPos;
    short blockPos;
    short i;
    short k;

    debug(uartTxBuf, 0, "AST\r\n");
    debug(uartTxBuf, 0, tokenTree);
    debug(uartTxBuf, 0, "\r\n");

    for(i=0;i<MAX_TOKEN_TREE_SIZE;i++)
        tokenTreeAst[i] = 0;

    tokenTreeIndex = 0;

    do
    {
        i = 0;
        k = 0;

        for(statementEnd=tokenTreeIndex;statementEnd<MAX_TOKEN_TREE_SIZE;statementEnd++)
        {
            if(tokenTree[statementEnd] == 'T') // token end
            {
                break;
            }
        }

        if(statementEnd == MAX_TOKEN_TREE_SIZE)
        {
            break;
        }

        for(conditionalPos=tokenTreeIndex;conditionalPos<statementEnd;conditionalPos++)
        {
            if(tokenTree[conditionalPos] == 'B') // repeat
            {
                for(i=0;i<3;i++)
                {
                    tokenTreeAst[tokenTreeIndex+i] = tokenTree[conditionalPos+i];
                }
            }
        }

        for(blockPos=tokenTreeIndex;blockPos<statementEnd;blockPos++)
        {
            if(tokenTree[blockPos] == 'D') // pause
            {
                tokenTreeAst[tokenTreeIndex+i+k] = tokenTree[blockPos];
                i++;
                continue;
            }

            if(tokenTree[blockPos] == 'R') // finish
            {
                tokenTreeAst[tokenTreeIndex+i+k] = tokenTree[blockPos];
                i++;
                continue;
            }

            if(tokenTree[blockPos] == 'M') // led
            {
                tokenTreeAst[tokenTreeIndex+i+k] = tokenTree[blockPos];
                i++;
                continue;
            }

            if(tokenTree[blockPos] == 'I') // on
            {
                tokenTreeAst[tokenTreeIndex+i+k] = tokenTree[blockPos];
                i++;
                continue;
            }

            if(tokenTree[blockPos] == 'J') // off
            {
                tokenTreeAst[tokenTreeIndex+i+k] = tokenTree[blockPos];
                i++;
                continue;
            }

            if(tokenTree[blockPos] == 'S') // rerun
            {
                tokenTreeAst[tokenTreeIndex+i+k] = tokenTree[blockPos];
                i++;
                continue;
            }

            if(tokenTree[blockPos] == 'H') // uart
            {
                for(k=0;k<3;k++)
                {
                    tokenTreeAst[tokenTreeIndex+i+k] = tokenTree[blockPos+k];
                }
                k = 0;
                i += 3;
            }

            if(tokenTree[blockPos] == 'U') // lcd
            {
                for(k=0;k<3;k++)
                {
                    tokenTreeAst[tokenTreeIndex+i+k] = tokenTree[blockPos+k];
                }
                k = 0;
                i += 3;
            }
        }

        tokenTreeAst[statementEnd] = 'T';
        tokenTreeIndex = statementEnd + 1;
    }
    while(tokenTree[tokenTreeIndex]);

    debug(uartTxBuf, 0, tokenTreeAst);
    debug(uartTxBuf, 0, "\r\n");

    return TRUE;
}
