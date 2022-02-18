/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * tasks.c
 *
 * Tasks and task management routines.
 *
 * Copyright (C) 2022 Alex Pogostin <alex.pogostin@outlook.com>
 *
 */

#include "twine.h"

/*****************************************************************************/
/* extern declarations                                                       */
/*****************************************************************************/
extern unsigned char uartRxBuf[UART_RX_BUF_SIZE];
extern unsigned char uartTxBuf[UART_TX_BUF_ARRAY_SIZE][UART_TX_BUF_SIZE];

/*****************************************************************************/
/* global declarations                                                       */
/*****************************************************************************/
unsigned short task_1_status = 0;
unsigned short task_2_status = 0;
unsigned short task_3_status = 0;
unsigned short task_4_status = 0;

short task_1_timeout = 0;
short task_2_timeout = 0;
short task_3_timeout = 0;
short task_4_timeout = 0;

/*****************************************************************************/
/* static (local) declarations                                               */
/*****************************************************************************/
static unsigned short *SP_current;
static unsigned short task_current = 0;

static unsigned short taskManager_stack_1;
static unsigned short taskManager_stack_2;
static unsigned short taskManager_stack_3;
static unsigned short taskManager_stack_4;

static unsigned short task_1_stack_size;
static unsigned short task_2_stack_size;
static unsigned short task_3_stack_size;
static unsigned short task_4_stack_size;

static unsigned short task_1_stack[8];
static unsigned short task_2_stack[8];
static unsigned short task_3_stack[8];
static unsigned short task_4_stack[8];

static unsigned char prompt[]  = "\r\n# ";
static unsigned char help[]    = "\r\nh: help\r\nv: version";
static unsigned char version[] = "\r\nv:1.1";

/*****************************************************************************/
void taskManager(void)
{
    short i;

    while(1)
    {
        switch (++task_current)
        {
        case 1:
            if((task_1_status & BIT0) == 0)
            {
                taskManager_stack_1 = (unsigned short) __get_SP_register();
                task_1();
                break;
            }
            else if(task_1_timeout <= 0)
            {
                task_1_status = 0;
                taskTimeoutLock(1,0);

                SP_current = (unsigned short*) __get_SP_register();

                for(i=task_1_stack_size - 1;i >= 0;i--)
                {
                    *--SP_current = (unsigned short) task_1_stack[i];
                }

                __set_SP_register((unsigned short)SP_current);

                return;
            }
            break;

        case 2:
            if((task_2_status & BIT0) == 0)
            {
                taskManager_stack_2 = (unsigned short) __get_SP_register();
                task_2();
                break;
            }
            else if(task_2_timeout <= 0)
            {
                task_2_status = 0;
                taskTimeoutLock(2,0);

                SP_current = (unsigned short*) __get_SP_register();

                for(i=task_2_stack_size - 1;i >= 0;i--)
                {
                    *--SP_current = (unsigned short) task_2_stack[i];
                }

                __set_SP_register((unsigned short)SP_current);

                return;
            }
            break;

        case 3:
            if((task_3_status & BIT0) == 0)
            {
                taskManager_stack_3 = (unsigned short) __get_SP_register();
                task_3();
                break;
            }
            else if(task_3_timeout <= 0)
            {
                task_3_status = 0;
                taskTimeoutLock(3,0);

                SP_current = (unsigned short*) __get_SP_register();

                for(i=task_3_stack_size - 1;i >= 0;i--)
                {
                    *--SP_current = (unsigned short) task_3_stack[i];
                }

                __set_SP_register((unsigned short)SP_current);

                return;
            }
            break;

        case 4:
            if((task_4_status & BIT0) == 0)
            {
                taskManager_stack_4 = (unsigned short) __get_SP_register();
                task_4();
                break;
            }
            else if(task_4_timeout <= 0)
            {
                task_4_status = 0;
                taskTimeoutLock(4,0);

                SP_current = (unsigned short*) __get_SP_register();

                for(i=task_4_stack_size - 1;i >= 0;i--)
                {
                    *--SP_current = (unsigned short) task_4_stack[i];
                }

                __set_SP_register((unsigned short)SP_current);

                return;
            }
            break;

            default:
                task_current = 0;
            break;

        } // switch
    } // while
}

/***************************************AJP***********************************/
static void taskSleep(int count)
{
    short i;

    switch(task_current)
    {
        case 1:
            task_1_status = 1;
            taskTimeoutLock(1,count);

            SP_current = (unsigned short*) __get_SP_register();

            for(i=0;i < task_1_stack_size;i++)
            {
                task_1_stack[i] = (short) *(SP_current++);
            }

            SP_current--;
            __set_SP_register((unsigned short) SP_current);
            break;

        case 2:
            task_2_status = 1;
            taskTimeoutLock(2,count);

            SP_current = (unsigned short*) __get_SP_register();

            for(i=0;i < task_2_stack_size;i++)
            {
                task_2_stack[i] = (short) *(SP_current++);
            }

            SP_current--;
            __set_SP_register((unsigned short) SP_current);
            break;

        case 3:
            task_3_status = 1;
            taskTimeoutLock(3,count);

            SP_current = (unsigned short*) __get_SP_register();

            for(i=0;i < task_3_stack_size;i++)
            {
                task_3_stack[i] = (short) *(SP_current++);
            }

            SP_current--;
            __set_SP_register((unsigned short) SP_current);
            break;

        case 4:
            task_4_status = 1;
            taskTimeoutLock(4,count);

            SP_current = (unsigned short*) __get_SP_register();

            for(i=0;i < task_4_stack_size;i++)
            {
                task_4_stack[i] = (short) *(SP_current++);
            }

            SP_current--;
            __set_SP_register((unsigned short) SP_current);
            break;

         default:
             break;
         }
}

/*****************************************************************************/
void taskTimeoutLock(short num, short count)
{
    __disable_interrupt();

    switch(num)
    {
    case 1:
        task_1_timeout = count;
        break;
    case 2:
        task_2_timeout = count;
        break;
    case 3:
        task_3_timeout = count;
        break;
    case 4:
        task_4_timeout = count;
        break;
    default:
        break;
    }

    __enable_interrupt();
}

/*****************************************************************************/
short task_1(void)
{
    unsigned short i = 0;
    unsigned short j = 0;

    task_1_stack_size = ((taskManager_stack_1 - (unsigned short) __get_SP_register())>>1) + 1;

    while(uartRxBuf[j])
    {
        if(uartRxBuf[j++] == 0x0d)
        {
            for(i=0;i<j;i++)
            {
                if(uartRxBuf[i] == 'h')
                    uartTx(uartTxBuf, 0, help);
                else if(uartRxBuf[i] == 'v')
                    uartTx(uartTxBuf, 0, version);
            }

            uartTx(uartTxBuf, 0, prompt);

            for(i=0;i<UART_RX_BUF_SIZE;i++)
                uartRxBuf[i] = 0x00;
        }
    };

    return 0;
}

/*****************************************************************************/
short task_2(void)
{
    task_2_stack_size = ((taskManager_stack_2 - (unsigned short) __get_SP_register())>>1) + 1;
    return 0;
}

/*****************************************************************************/
short task_3(void)
{
    task_3_stack_size = ((taskManager_stack_3 - (unsigned short) __get_SP_register())>>1) + 1;
    return 0;
}

/*****************************************************************************/
short task_4(void)
{
    task_4_stack_size = ((taskManager_stack_4 - (unsigned short) __get_SP_register())>>1) + 1;
    return 0;
}
