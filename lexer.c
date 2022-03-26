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
unsigned char token_tree[MAX_TOKEN_TREE_SIZE];
int tokenPointer;

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
                                               TOKEN_OUT,
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
                                               TOKEN_FINAL,
                                               };

/*****************************************************************************/
int lexer(unsigned char *program)
{
    unsigned int i;
    int progLineLen;
    int progLineIndex = 0;
    int tokenIndex = 0;
    int tokenNum = 0;
    int matchLen = 0;

    for(progLineLen=0;progLineLen<MAX_PROG_LINE_LEN;progLineLen++)
    {
        if(!*(program + progLineLen) || *(program + progLineLen) < SPACE)
            break;
    }

    while(token[tokenNum])
    {

        // if line contains yes:, but token=pressed
        if(*(token[tokenNum]) > progLineLen)
        {
            tokenNum++;
            continue;
        }

        for(i=progLineIndex, tokenIndex = 0;i<progLineLen;i++, tokenIndex++)
        {
            if(*(program + i) == SPACE)
            {
                tokenIndex--; // if skipping space don't increment token pointer
                continue;
            }

            if(*(program + i) == *(token[tokenNum] + TOKEN_HEADER_LEN + tokenIndex))
            {
                matchLen++;
            }
            else if(*(token[tokenNum] + TOKEN_HEADER_LEN + tokenIndex) == 0x0) // end of token
            {
                break;
            }
        }

        if(matchLen < *(token[tokenNum]) || matchLen > *(token[tokenNum]))
        {
            matchLen = 0;
            tokenNum++;
            continue;
        }

        if(matchLen == *(token[tokenNum]))
        {
            token_tree[tokenPointer++] = *(token[tokenNum] + 1);

            if(progLineLen == i)
            {
                break;
            }

            matchLen = 0;
            progLineIndex = i;
            tokenNum = 0;
        }
    }

    return TRUE;
}
