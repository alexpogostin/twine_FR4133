/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * interpreter.h
 *
 * interpreter include
 *
 * Copyright (C) 2022 Alex Pogostin <alex.pogostin@outlook.com>
 *
 */

#ifndef INTERPRETER_H
#define INTERPRETER_H

int interpreter(int *tokenTreeIndex, unsigned char *token_tree);
int _uart_lcd(int *tokenTreeIndex, unsigned char *tokenTree);
int _string(int *tokenTreeIndex, unsigned char *tokenTree);
int _repeat(int *tokenTreeIndex, unsigned char *tokenTree);
int _value(int *val, int *tokenTreeIndex, unsigned char *tokenTree);
int _finish(int *tokenTreeIndex);
int _rerun(int *tokenTreeIndex);
int _end(int *tokenTreeIndex, unsigned char *tokenTree);
int _led(int *tokenTreeIndex, unsigned char *tokenTree);
int _on(int *tokenTreeIndex, unsigned char *tokenTree);
int _off(int *tokenTreeIndex, unsigned char *tokenTree);

#endif
