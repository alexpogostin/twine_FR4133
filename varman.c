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

static int intVarNum;   // integer variable number (in ASCII 0x30-0x39)
static int strVarNum;   // string variable number (in ASCII 0x30-0x39)
static int intVarIndex; // index into strVars[][] array
static int strVarIndex; // index into intVars[][] array

/*
 * The varman code is intended to remove globals and instead
 * manage variables using setters and getters. These routines
 * would also allow for the management (reads and writes) of
 * these variables from a single source file. In the future
 * it may neccassary to "atomize" (make atomic) some of these
 * values using mutexes, escpecially those found in the UART
 * interrupt routine (RX and TX buffs).
 */

/* 
 * A note regarding coding style used here. Its important to
 * remain consistant thoughout you code, but in this case,
 * the use of a single line to set, get or increment a value
 * makes it less error prone.
 */

int *getIntVarNum(void)      {return &intVarNum;}
void setIntVarNum(int value) {intVarNum = value;}
void incIntVarNum(void)      {intVarNum++;}

int *getStrVarNum(void)      {return &strVarNum;}
void setStrVarNum(int value) {strVarNum = value;}
void incStrVarNum(void)      {strVarNum++;}

int *getIntVarIndex(void)      {return &intVarIndex;}
void setIntVarIndex(int value) {intVarIndex = value;}
void incIntVarIndex(void)      {intVarIndex++;}

int *getStrVarIndex(void)      {return &strVarIndex;}
void setStrVarIndex(int value) {strVarIndex = value;}
void incStrVarIndex(void)      {strVarIndex++;}

