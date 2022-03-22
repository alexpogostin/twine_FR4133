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
unsigned char ast[MAX_TOKEN_TREE_SIZE];

/*****************************************************************************/
/* static (local) declarations                                               */
/*****************************************************************************/
enum states{
            STOP,
            ASSIGN,
            CONDITIONAL,
            RUN,
           };

/*****************************************************************************/
int interpreter(unsigned char *token_tree)
{

    enum states i_state = ASSIGN;

    while(i_state)
    {
        switch(i_state)
        {
            case CONDITIONAL:
                interpreter_ast(token_tree);
                i_state = ASSIGN;
            break;

            case ASSIGN:
                interpreter_ast(token_tree);
                i_state = RUN;
            break;

            case RUN:
            break;

            default:
            break;
        }
    }

    return TRUE;
}

/*****************************************************************************/
int interpreter_ast(unsigned char *token_tree) // abstract syntax tree
{
    int i, j;

    for (j=0;j<MAX_TOKEN_TREE_SIZE;j++)
    {
        if(!token_tree[j])
        {
            break;
        }
    }

    for(i=0;i < j;i++)
    {
        switch(token_tree[i])
        {
            case 'A': // is
            break;

            case 'K': // yes:
            break;

            case 'L': // no:
            break;

            case 'M': // led1
            break;

            case 'N': // led2
            break;

            case 'O': // but1
            break;

            case 'P': // but2
            break;

            case 'Q': // pressed
            break;

            default:
            break;
        }
    }

    return TRUE;
}

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
