/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * tasks.h
 *
 * Copyright (C) 2022 Alex Pogostin <alex.pogostin@outlook.com>
 *
 */

#ifndef TASKS_H
#define TASKS_H

#define TASK1 1
#define TASK2 2
#define TASK3 3
#define TASK4 4

#define ENABLE 1
#define DISABLE 0

void taskManager(void);

static void taskSleep(int);
static void taskTimeoutLock(short task, short count);
static void taskSuspend(int task);
static void taskContinue(int task);
static void taskControl(int task, short control);

char* binToAscii(char* line, short val);

static short task_1(void);
static short task_2(void);
static short task_3(void);
static short task_4(void);

#endif
