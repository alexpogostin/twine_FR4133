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
unsigned char token_tree[TOKEN_TREE_SIZE];
int token_pointer;

/*****************************************************************************/
/* static (local) declarations                                               */
/*****************************************************************************/
static unsigned char *token[MAX_NUM_TOKENS] = {TOKEN_IF,
                                               TOKEN_REPEAT,
                                               TOKEN_SET,
                                               TOKEN_SLEEP,
                                               TOKEN_CALL,
                                               TOKEN_EQ,
                                               TOKEN_IN,
                                               TOKEN_OUT,
                                               TOKEN_ON,
                                               TOKEN_OFF,
                                               TOKEN_GPIO1,
                                               TOKEN_GPIO2,
                                               TOKEN_LED1,
                                               TOKEN_LED2,
                                               TOKEN_BUT1,
                                               TOKEN_BUT2,
                                               TOKEN_PRESSED};

/*****************************************************************************/
int lexer(unsigned char *program)
{
    int i, j, k;
    int progLineLen;
    int temp = 0;

    j = 0;

    for(progLineLen=0;progLineLen<MAX_PROG_LINE_LEN;progLineLen++)
    {
        if(!*(program + progLineLen) | *(program + progLineLen) < SPACE)
            break;
    }

    for(i=0;i<MAX_NUM_TOKENS;i++)
    {
        if(*(token[i]) > progLineLen)
        {
            break;
        }

        do
        {
            for(k=2;k < *(token[i]) + 2;k++)
            {
                if(*(program + (k - 2) + j) == *(token[i] + k))
                {
                    temp++;
                }
            }

            if(temp < *(token[i]) || temp > *(token[i]))
            {
                temp = 0;
            }

            if(temp == *(token[i]))
            {
                token_tree[token_pointer++] = *(token[i] + 1);
                break;
            }
        }
        while(*(program + j++));

        temp = 0;
        j = 0;
    }

    return TRUE;
}
