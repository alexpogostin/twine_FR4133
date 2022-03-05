/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * lexer.h
 *
 * lexer include
 *
 * Copyright (C) 2022 Alex Pogostin <alex.pogostin@outlook.com>
 *
 */

#ifndef LEXER_H
#define LEXER_H

#define MAX_NUM_TOKENS 17
#define MAX_TOKEN_LEN 8
#define TOKEN_TREE_SIZE 32

#define TOKEN_IF      "\x02" "\x41" "if"      // A
#define TOKEN_REPEAT  "\x06" "\x42" "repeat"  // B
#define TOKEN_SET     "\x03" "\x43" "set"     // C
#define TOKEN_SLEEP   "\x05" "\x44" "sleep"   // D
#define TOKEN_CALL    "\x04" "\x45" "call"    // E
#define TOKEN_EQ      "\x01" "\x46" "="       // F
#define TOKEN_IN      "\x02" "\x47" "in"      // G
#define TOKEN_OUT     "\x03" "\x48" "out"     // H
#define TOKEN_ON      "\x02" "\x49" "on"      // I
#define TOKEN_OFF     "\x03" "\x4A" "off"     // J
#define TOKEN_GPIO1   "\x05" "\x4B" "gpio1"   // K
#define TOKEN_GPIO2   "\x05" "\x4C" "gpio2"   // L
#define TOKEN_LED1    "\x04" "\x4D" "led1"    // M
#define TOKEN_LED2    "\x04" "\x4E" "led2"    // N
#define TOKEN_BUT1    "\x04" "\x4F" "but1"    // O
#define TOKEN_BUT2    "\x04" "\x50" "but2"    // P
#define TOKEN_PRESSED "\x07" "\x51" "pressed" // Q

int lexer(unsigned char *program);

#endif
