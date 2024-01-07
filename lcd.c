
/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * lcd.c
 *
 * LCD routines.
 *
 * Copyright (C) 2024 Alex Pogostin <alex.pogostin@outlook.com>
 *
 */

/*
 * The initLcd(...) parameters below were found in the MSP-EXP430FR4133 LaundPad example code.
 * https://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP-EXP430FR4133/latest/index_FDS.html
 * MSP-EXP430FR4133_Software_Examples_windows.zip
 */

#include "twine.h"

#pragma PERSISTENT(ascii_table)

static unsigned char *ascii_table[26] = {ASCII_0x20,
                                         ASCII_0x21,
                                         ASCII_0x22,
                                         ASCII_0x23,
                                         ASCII_0x24,
                                         ASCII_0x25,
                                         ASCII_0x26,
                                         ASCII_0x27,
                                         ASCII_0x28,
                                         ASCII_0x29,
                                         ASCII_0x2A,
                                         ASCII_0x2B,
                                         ASCII_0x2C,
                                         ASCII_0x2D,
                                         ASCII_0x2E,
                                         ASCII_0x2F,
                                         ASCII_0x30,
                                         ASCII_0x31,
                                         ASCII_0x32,
                                         ASCII_0x33,
                                         ASCII_0x34,
                                         ASCII_0x35,
                                         ASCII_0x36,
                                         ASCII_0x37,
                                         ASCII_0x38,
                                         ASCII_0x39,
                                        };

void initLcd(void)
{
    LCDPCTL0 = 0xFFFF; // L0-L15 set as segment pins
    LCDPCTL1 = 0x07FF; // L16-L26 set as segment pins
    LCDPCTL2 = 0x00F0; // L36-L39 set as segment pins
    LCDCTL0 = 0x181C; // init clocks (4-mux mode), leave LCDON=0 (off)
    LCDVCTL = 0xF0C0; // voltage control register
    LCDCSSEL0 = 0x000F; // L0=COM0, L1=COM1, L2=COM2, L3=COM3
    LCDCTL0 |= LCDON;   // turn on LCD
    SYSCFG2 |= LCDPCTL; // turn on LCD power pin
}

void clearLcd(void)
{
        LCDM1 = 0;
        LCDM0 = 0;
        LCDM5 = 0;
        LCDM4 = 0;
        LCDM7 = 0;
        LCDM6 = 0;
        LCDM9 = 0;
        LCDM8 = 0;
        LCDM11 = 0;
        LCDM10 = 0;
        LCDM3 = 0;
        LCDM2 = 0;
}

short writeLcd(unsigned char ascii, unsigned char location)
{

    ascii -= 32; // offsets first 32 ASCII characters

    LCDM1 = *(ascii_table[ascii] + 0);
    LCDM0 = *(ascii_table[ascii] + 1);

    switch (location)
    {
        case 1:
        LCDM5 = *(ascii_table[ascii] + 2);
        LCDM4 = *(ascii_table[ascii] + 3);
        break;

        case 2:
        LCDM7 = *(ascii_table[ascii] + 2);
        LCDM6 = *(ascii_table[ascii] + 3);
        break;

        case 3:
        LCDM9 = *(ascii_table[ascii] + 2);
        LCDM8 = *(ascii_table[ascii] + 3);
        break;

        case 4:
        LCDM11 = *(ascii_table[ascii] + 2);
        LCDM10 = *(ascii_table[ascii] + 3);
        break;

        case 5:
        LCDM3 = *(ascii_table[ascii] + 2);
        LCDM2 = *(ascii_table[ascii] + 3);
        break;

        default:
        break;
    };

    return TRUE;
}

