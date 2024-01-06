/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * lcd.h
 *
 * Copyright (C) 2024 Alex Pogostin <alex.pogostin@outlook.com>
 *
 */

#ifndef LCD_H
#define LCD_H

// 0 = ABCDEFKQ
// 1 = KBC
// 2 = ABGMED
// 3 = ABMCD
// 4 = FGMBC
// 5 = AFGMDC
// 6 = AFGMCDE
// 7 = ABC
// 8 = ABCDEFGM
// 9 = ABGMFCD

// LCDM1   1   0   0   0   0   1   0   0 = 0x84 = COM3 COM2
// LCDM0   0   0   1   0   0   0   0   1 = 0x21 = COM1 COM0

// LCDM5 A1H A1J A1K A1P A1Q NEG A1N A1DP
//         0   0   0   0   0   0   0   0
// LCDM4 A1A A1B A1C A1D A1E A1F A1G A1M
//         0   0   0   0   0   0   0   0

// '0'
// LCDM1   1   0   0   0   0   1   0   0 = 0x84
// LCDM0   0   0   1   0   0   0   0   1 = 0x20
// LCDM5   0   0   1   0   1   0   0   0 = 0x28
// LCDM4   1   1   1   1   1   1   0   0 = 0xFC

// '1'
// LCDM1   1   0   0   0   0   1   0   0 = 0x84
// LCDM0   0   0   1   0   0   0   0   0 = 0x20
// LCDM5   0   0   1   0   0   0   0   0 = 0x20
// LCDM4   0   1   1   0   0   0   0   0 = 0x60

// '2'
// LCDM1   1   0   0   0   0   1   0   0 = 0x84
// LCDM0   0   0   1   0   0   0   0   1 = 0x21
// LCDM5   0   0   0   0   0   0   0   0 = 0x00
// LCDM4   1   1   0   1   1   0   1   1 = 0xDB

// '3'
// LCDM1   1   0   0   0   0   1   0   0 = 0x84
// LCDM0   0   0   1   0   0   0   0   1 = 0x21
// LCDM5   0   0   0   0   0   0   0   0 = 0x00
// LCDM4   1   1   1   1   0   0   0   1 = 0xF1

// '4'
// LCDM1   1   0   0   0   0   1   0   0 = 0x04
// LCDM0   0   0   1   0   0   0   0   1 = 0x21
// LCDM5   0   0   0   0   0   0   0   0 = 0x00
// LCDM4   0   1   1   0   0   1   1   1 = 0x67

// '5'
// LCDM1   1   0   0   0   0   1   0   0 = 0x84
// LCDM0   0   0   1   0   0   0   0   1 = 0x21
// LCDM5   0   0   0   0   0   0   0   0 = 0x00
// LCDM4   1   0   1   1   0   1   1   1 = 0xB7

// '6'
// LCDM1   1   0   0   0   0   1   0   0 = 0x84
// LCDM0   0   0   1   0   0   0   0   1 = 0x21
// LCDM5   0   0   0   0   0   0   0   0 = 0x00
// LCDM4   1   0   1   1   1   1   1   1 = 0xBF

// '7'
// LCDM1   1   0   0   0   0   1   0   0 = 0x84
// LCDM0   0   0   1   0   0   0   0   0 = 0x20
// LCDM5   0   0   0   0   0   0   0   0 = 0x00
// LCDM4   1   1   1   0   0   0   0   0 = 0xE0

// '8'
// LCDM1   1   0   0   0   0   1   0   0 = 0x84
// LCDM0   0   0   1   0   0   0   0   0 = 0x20
// LCDM5   0   0   0   0   0   0   0   0 = 0x00
// LCDM4   1   1   1   1   1   1   1   1 = 0xFF

// '9'
// LCDM1   1   0   0   0   0   1   0   0 = 0x84
// LCDM0   0   0   1   0   0   0   0   1 = 0x21
// LCDM5   0   0   0   0   0   0   0   0 = 0x00
// LCDM4   1   1   1   1   0   1   1   1 = 0xF7

// 1, 7 and 8 have COM0 cleared, but that messes
// up the display when outputting these numbers
// in a certain pattern?? setting them with COM0=1
// as other ASCII charaters are output, there
// may be some patter - or understanding
// as to why this is occuring.

#define ASCII_0x20 "\x00" "\x00" "\x00" "\x00" // space
#define ASCII_0x21 "\x00" "\x00" "\x00" "\x00" // !
#define ASCII_0x22 "\x00" "\x00" "\x00" "\x00" // ""
#define ASCII_0x23 "\x00" "\x00" "\x00" "\x00" // #
#define ASCII_0x24 "\x00" "\x00" "\x00" "\x00" // $
#define ASCII_0x25 "\x00" "\x00" "\x00" "\x00" // %
#define ASCII_0x26 "\x00" "\x00" "\x00" "\x00" // &
#define ASCII_0x27 "\x00" "\x00" "\x00" "\x00" // '
#define ASCII_0x28 "\x00" "\x00" "\x00" "\x00" // (
#define ASCII_0x29 "\x00" "\x00" "\x00" "\x00" // )
#define ASCII_0x2A "\x00" "\x00" "\x00" "\x00" // *
#define ASCII_0x2B "\x00" "\x00" "\x00" "\x00" // +
#define ASCII_0x2C "\x00" "\x00" "\x00" "\x00" // `
#define ASCII_0x2D "\x00" "\x00" "\x00" "\x00" // -
#define ASCII_0x2E "\x00" "\x00" "\x00" "\x00" // .
#define ASCII_0x2F "\x00" "\x00" "\x00" "\x00" // /
#define ASCII_0x30 "\x84" "\x21" "\x28" "\xFC" // 0
#define ASCII_0x31 "\x84" "\x21" "\x20" "\x60" // 1
#define ASCII_0x32 "\x84" "\x21" "\x00" "\xDB" // 2
#define ASCII_0x33 "\x84" "\x21" "\x00" "\xF1" // 3
#define ASCII_0x34 "\x04" "\x21" "\x00" "\x67" // 4
#define ASCII_0x35 "\x84" "\x21" "\x00" "\xB7" // 5
#define ASCII_0x36 "\x84" "\x21" "\x00" "\xBF" // 6
#define ASCII_0x37 "\x84" "\x21" "\x00" "\xE0" // 7
#define ASCII_0x38 "\x84" "\x21" "\x00" "\xFF" // 8
#define ASCII_0x39 "\x84" "\x21" "\x00" "\xF7" // 9

void initLcd(void);
void clearLcd(void);
short writeLcd(unsigned char ascii, unsigned char location);

#endif
