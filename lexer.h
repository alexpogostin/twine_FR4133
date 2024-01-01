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

#define MAX_NUM_TOKENS 20
#define MAX_TOKEN_LEN 8
#define MAX_TOKEN_TREE_SIZE 32
#define TOKEN_SIZE 0
#define TOKEN 1
#define TOKEN_HEADER_LEN 2

#define MAX_STRING_VARS 4
#define MAX_INT_VARS 4

#define MAX_STRING_LEN 32
#define MAX_INT_LEN 8

#define TOKEN_IS      "\x02" "\x41" "is"      // A
#define TOKEN_REPEAT  "\x06" "\x42" "repeat"  // B
#define TOKEN_SET     "\x03" "\x43" "set"     // C
#define TOKEN_PAUSE   "\x05" "\x44" "pause"   // D
#define TOKEN_CALL    "\x04" "\x45" "call"    // E
#define TOKEN_EQ      "\x01" "\x46" "="       // F
#define TOKEN_IN      "\x02" "\x47" "in"      // G
#define TOKEN_UART    "\x04" "\x48" "uart"    // H
#define TOKEN_ON      "\x02" "\x49" "on"      // I
#define TOKEN_OFF     "\x03" "\x4A" "off"     // J
#define TOKEN_YES     "\x04" "\x4B" "yes:"    // K
#define TOKEN_NO      "\x03" "\x4C" "no:"     // L
#define TOKEN_LED1    "\x04" "\x4D" "led1"    // M
#define TOKEN_LED2    "\x04" "\x4E" "led2"    // N
#define TOKEN_BUT1    "\x04" "\x4F" "but1"    // O
#define TOKEN_BUT2    "\x04" "\x50" "but2"    // P
#define TOKEN_PRESSED "\x07" "\x51" "pressed" // Q
#define TOKEN_FINISH  "\x06" "\x52" "finish"  // R
#define TOKEN_RERUN   "\x05" "\x53" "rerun"   // S
#define TOKEN_END     "\x01" "\x54" "."       // T
#define TOKEN_LCD     "\x03" "\x55" "lcd"     // U
#define TOKEN_FINAL   "\x00" "\x00" ""        // 0x00

int lexer(unsigned char *program);
int ast(unsigned char *tokenTree);

#endif
