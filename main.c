/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * main.c
 *
 * C entry point for twine.
 *
 * Copyright (C) 2022 Alex Pogostin <alex.pogostin@outlook.com>
 *
 */

#include "twine.h"

int main( void )
{
    WDTCTL = WDTPW + WDTHOLD;       // [stop] disable watchdog timer

    initGpios();
    startXt1Clk();
    startRtcClk();

    __enable_interrupt();

    taskManager();

    return 0;
}
