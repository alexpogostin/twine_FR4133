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


#endif
