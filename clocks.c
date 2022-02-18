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

// FLL:    Frequency Locked Loop
// DCO:    Digitally Controlled Oscillator
// VLO:    Very Low-Power Low-Frequency Oscillator (10Khz typical)
//         possible sources for MCLK and SMCLK
// REFO:   Internal Trimmed Low-Frequency Reference Oscillator (32.768 khz typical)
//         can be used as FLLREFCLK, so no need for external crystal
// XT1:    external 32.768 khz crystal, external high-frequency optional
//         can be run in low-frequency (LF) or high-frequency modes (HF)
//         possible sources for MCLK, SMCLK, ACLK and FLLREGCLK
// MODOSC: Module Oscillator (5 mhz typical)
//         internal oscillator
//         sources MODCLK

// ACLK:   Auxiliary clock
//         low-frequency peripherals, typical 32.768 khz (max 40 khz)
// MCLK:   Master clock
//         CPU clock
//         can be sourced from REFO, DCO, XT1 or VLO
// SMCLK:  Subsystem Master clock
//         used by peripherals, always sourced from MCLK
//
// BRCLK:  Baud Rate clock
//         possible sources UCLK and SMCLK
// UCLK:   optional source to eUSCI (uart, i2c, spi), externally provided
//         not much in docs, likely part specific

/*****************************************************************************/
void startXt1Clk(void)
{
    CSCTL0 = 0x0000;

    // select XIN and XOUT as functional pins on P4.1 and P4.2
    P4SEL0 |= (BIT1 | BIT2);

    while(CSCTL7 & XT1OFFG)
    {
        CSCTL7 &= (unsigned char) ~XT1OFFG;
        CSCTL7 &= (unsigned char) ~DCOFFG;
    }

    while (CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK0)); // Poll until FLL is locked
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
