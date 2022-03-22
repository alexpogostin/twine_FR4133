/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * string.h
 *
 * string include
 *
 * Copyright (C) 2022 Alex Pogostin <alex.pogostin@outlook.com>
 *
 */

#ifndef STRING_H
#define STRING_H

#define NULL      0x00
#define CTRL_C    0x03
#define BACKSPACE 0x08
#define LF        0x0A
#define CR        0x0D
#define SPACE     0x20
#define ASCII_0   0x30
#define DEL       0x7F

#define CRLF "\r\n"
#define BANNER "twine"
#define PROMPT "# "
#define EDIT_LINE "  : "
#define PRINT_LINE "\x01\x01  : "
#define VERSION "v:1.1"

char* binToAscii(char* line, short val);

#endif
