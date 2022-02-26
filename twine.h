/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * twine.h
 *
 * Twine include files and configuration parameters.
 *
 * Twine is a strong thread or string consisting of two or more strands.
 *
 * Copyright (C) 2022 Alex Pogostin <alex.pogostin@outlook.com>
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

#define read_reg_8(x) (*((volatile unsigned char *)((unsigned short)x)))
#define read_reg_16(x) (*((volatile unsigned short *)((unsigned short)x)))

#define MAX_PROG_LINES 32
#define MAX_PROG_LINE_LEN 32

#define NULL      0x00
#define BACKSPACE 0x08
#define LF        0x0A
#define CR        0x0D
#define SPACE     0x20
#define DEL       0x7F

#endif
