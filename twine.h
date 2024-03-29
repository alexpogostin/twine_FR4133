/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * twine.h
 *
 * Twine include files and configuration parameters.
 *
 * Twine is a strong thread or string consisting of two or more strands.
 *
 * Copyright (C) 2024 Alex Pogostin <alex.pogostin@outlook.com>
 *
 */

#ifndef TWINE_H
#define TWINE_H

#include <msp430.h>

#include "clocks.h"
#include "gpios.h"
#include "tasks.h"
#include "uart.h"
#include "power.h"
#include "string.h"
#include "lexer.h"
#include "interpreter.h"
#include "lcd.h"
#include "varman.h"

#define read_reg_8(x) (*((volatile unsigned char *)((unsigned short)x)))
#define read_reg_16(x) (*((volatile unsigned short *)((unsigned short)x)))

#define write_reg_16(x) (*((volatile unsigned short *)((unsigned short)x)))

#define MAX_PROG_LINES 32
#define MAX_PROG_LINE_LEN 32

#define TRUE 1
#define FALSE 0

#define DEBUG FALSE
#define debug(args ...) if (DEBUG) uartTx(args)

// there is a warning, #179-D casued by both taskContinue(...) and taskSuspend(...)
// not being referenced. Need to keep these in the code for now. So using this
// macro to remove this warning.
#undef REMOVE_FUNCTION_NEVER_REFERENCED_WARNING

#endif
