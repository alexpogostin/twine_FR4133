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

int *getIntVarIndex(void);
void setIntVarIndex(int value);
void incIntVarIndex(void);

int *getStrVarIndex(void);
void setStrVarIndex(int value);
void incStrVarIndex(void);

#endif
