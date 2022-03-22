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

int interpreter(unsigned char *token_tree);
int interpreter_ast(unsigned char *token_tree);

#endif
