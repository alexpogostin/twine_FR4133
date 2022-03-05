/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * power.c
 *
 * Power routines.
 *
 * Copyright (C) 2022 Alex Pogostin <alex.pogostin@outlook.com>
 *
 */

#include "twine.h"

void powerInit(void)
{
    PM5CTL0 &= ~LOCKLPM5;
}
