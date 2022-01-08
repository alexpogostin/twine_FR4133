/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * clocks.c
 *
 * Clock initialization.
 *
 * Copyright (C) 2022 Alex Pogostin <alex.pogostin@outlook.com>
 *
 */

#include "twine.h"

/*****************************************************************************/
void startXt1Clk(void)
{
    // select XIN and XOUT as functional pins on P4.1 and P4.2
    P4SEL0 |= (BIT1 | BIT2);

    while(CSCTL7 & XT1OFFG)
    {
        CSCTL7 &= (unsigned char) ~XT1OFFG;
        SFRIFG1 &= (unsigned char) ~OFIFG;
    }
}

/*****************************************************************************/
/* divide=256, count=128 = 1 second                                          */
/****************************************************AJP**********************/
void startRtcClk()
{
    RTCCTL &= ~RTCSS_3;         // stop clock
    RTCCTL &= ~RTCPS_7;         // clear bits
    RTCCTL |= RTCPS__256;       // set divider
    RTCMOD = RTC_MOD_COUNT;     // set modulo count
    __data16_read_addr(RTCIV);  // clear the RTCIV bit
    RTCCTL |= RTCSS__XT1CLK;    // start clock
    RTCCTL |= RTCSR;            // reset RTC
    RTCCTL |= RTCIE;            // enable RTC interrupts
}
