/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * varman.h
 *
 * varman include
 *
 * Copyright (C) 2024 Alex Pogostin <alex.pogostin@outlook.com>
 *
 */
#ifndef VARMAN_H
#define VARMAN_H

int *getStrVarNum(void);
void setStrVarNum(int value);
void incStrVarNum(void);

int *getIntVarNum(void);
void setIntVarNum(int value);
void incIntVarNum(void);

#endif
