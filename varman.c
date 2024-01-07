/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * varman.c
 *
 * Variable manager routines.
 *
 * Copyright (C) 2024 Alex Pogostin <alex.pogostin@outlook.com>
 *
 */

#include "twine.h"

static int intVarNum; // integer variable number
static int strVarNum; // string variable number

/*
 * The varman code is intended to remove globals and instead
 * manage variables using setters and getters. These routines
 * would also allow for the management (reads and writes) of
 * these variables from a single source file. In the future
 * it may neccassary to "atomize" (make atomic) some of these
 * values using mutexes, escpecially those found in the UART
 * interrupt routine (RX and TX buffs).
*/

int *getIntVarNum(void)
{
    return &intVarNum;
}

void setIntVarNum(int value)
{
    intVarNum = value;
}

void incIntVarNum(void)
{
    intVarNum++;
}

int *getStrVarNum(void)
{
    return &strVarNum;
}

void setStrVarNum(int value)
{
    strVarNum = value;
}

void incStrVarNum(void)
{
    strVarNum++;
}
