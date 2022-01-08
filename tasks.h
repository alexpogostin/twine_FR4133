/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * tasks.h
 *
 * Copyright (C) 2022 Alex Pogostin <alex.pogostin@outlook.com>
 *
 */

#ifndef TASKS_H
#define TASKS_H

void taskManager(void);

static void taskSleep(int);
static void taskTimeoutLock(short num, short count);

static short task_1(void);
static short task_2(void);
static short task_3(void);
static short task_4(void);

#endif
