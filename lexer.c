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
unsigned char strVars[4][32];
unsigned char intVars[4][8];

unsigned char tokenTree[MAX_TOKEN_TREE_SIZE];
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
