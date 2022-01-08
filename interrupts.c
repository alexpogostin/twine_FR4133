/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * interrupts.c
 *
 * Twine interrupt routine(s)
 *
 * Copyright (C) 2022 Alex Pogostin <alex.pogostin@outlook.com>
 *
 */

#include "twine.h"

/*****************************************************************************/
/* Externs - these global variables are declared in tasks.c                  */
/*****************************************************************************/
extern unsigned short task_1_status;
extern unsigned short task_2_status;
extern unsigned short task_3_status;
extern unsigned short task_4_status;
extern short task_1_timeout;
extern short task_2_timeout;
extern short task_3_timeout;
extern short task_4_timeout;

/*****************************************************************************/
/* Reference to the vector pragma can be found in slau132y.pdf v21.6.0.LTS   */
/*****************************************************************************/
#pragma vector = RTC_VECTOR

/*****************************************************************************/
/* RTC interrupt, initialization is done in clocks.c                         */
/**********************************************************AJP****************/
__interrupt void RTCIntervalInterrupt(void)
{
    PM5CTL0 &= ~LOCKLPM5;

    __data16_read_addr(RTCIV);

    RTCMOD = RTC_MOD_COUNT;

    if(task_1_status)
        task_1_timeout--;

    if(task_2_status)
        task_2_timeout--;

    if(task_3_status)
        task_3_timeout--;

    if(task_4_status)
        task_4_timeout--;
}
