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

static unsigned char crlf[]    = "\r\n";
static unsigned char prompt[]  = "\r\n# ";
static unsigned char prompt2[] = "n# ";
static unsigned char line[]    = "\r\n  : ";
static unsigned char print[]   = "\x01\x01  : ";
static unsigned char help[]    = "\r\nh: help"
                                 "\r\nv: version"
                                 "\r\ne: edit program"
                                 "\r\np: print program"
                                 "\r\n--------------------"
                                 "\r\n.: exit program edit";
static unsigned char version[] = "\r\nv:1.1";
static unsigned char list[]    = "\r\nlist not implemented";

static short lineNumber;
static unsigned char program[MAX_PROG_LINES][MAX_PROG_LINE_LEN];

// task_1_status = 0x01 0=!sleeping 1=sleep
// task_1_status = 0x02 0=!suspended 1=suspended
// task_1_status = 0x04 0=ignored 1=continue
// task_1_status = 0x08 0=disable 1=enable

/*****************************************************************************/
void taskManager(void)
{
    short i;

    taskControl(TASK1,ENABLE);
    taskControl(TASK2,DISABLE);
    taskControl(TASK3,DISABLE);
    taskControl(TASK4,DISABLE);

    while(1)
    {
        switch (++task_current)
        {
        case 1:
            if((task_1_status & BIT3) == 0)
            {
                break;
            }
            else if((task_1_status & BIT0) == 0 && (task_1_status & BIT1) == 0)
            {
                taskManager_stack_1 = (unsigned short) __get_SP_register();
                task_1();
                break;
            }
            else if(task_1_status & BIT2)
            {
                task_1_status &= ~(BIT1 | BIT2);

                SP_current = (unsigned short*) __get_SP_register();

                for(i=task_1_stack_size - 1;i >= 0;i--)
                {
                    *--SP_current = (unsigned short) task_1_stack[i];
                }

                __set_SP_register((unsigned short)SP_current);

                return;

            }
            else if(task_1_timeout <= 0 && !(task_1_status & BIT1))
            {
                task_1_status &= ~BIT0;
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
            if((task_2_status & BIT3) == 0)
            {
                break;
            }
            else if((task_2_status & BIT0) == 0 && (task_2_status & BIT1) == 0)
            {
                taskManager_stack_2 = (unsigned short) __get_SP_register();
                task_2();
                break;
            }
            else if(task_2_status & BIT2)
            {
                task_2_status &= ~(BIT1 | BIT2);

                SP_current = (unsigned short*) __get_SP_register();

                for(i=task_2_stack_size - 1;i >= 0;i--)
                {
                    *--SP_current = (unsigned short) task_2_stack[i];
                }

                __set_SP_register((unsigned short)SP_current);

                return;

            }
            else if(task_2_timeout <= 0 && !(task_2_status & BIT1))
            {
                task_2_status &= ~BIT0;
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
            if((task_3_status & BIT3) == 0)
            {
                break;
            }
            else if((task_3_status & BIT0) == 0 && (task_3_status & BIT1) == 0)
            {
                taskManager_stack_3 = (unsigned short) __get_SP_register();
                task_3();
                break;
            }
            else if(task_3_status & BIT2)
            {
                task_3_status &= ~(BIT1 | BIT2);

                SP_current = (unsigned short*) __get_SP_register();

                for(i=task_3_stack_size - 1;i >= 0;i--)
                {
                    *--SP_current = (unsigned short) task_3_stack[i];
                }

                __set_SP_register((unsigned short)SP_current);

                return;

            }
            else if(task_3_timeout <= 0 && !(task_3_status & BIT1))
            {
                task_3_status &= ~BIT0;
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
            if((task_4_status & BIT3) == 0)
            {
                break;
            }
            else if((task_4_status & BIT0) == 0 && (task_4_status & BIT1) == 0)
            {
                taskManager_stack_4 = (unsigned short) __get_SP_register();
                task_4();
                break;
            }
            else if(task_4_status & BIT2)
            {
                task_4_status &= ~(BIT1 | BIT2);

                SP_current = (unsigned short*) __get_SP_register();

                for(i=task_4_stack_size - 1;i >= 0;i--)
                {
                    *--SP_current = (unsigned short) task_4_stack[i];
                }

                __set_SP_register((unsigned short)SP_current);

                return;

            }
            else if(task_4_timeout <= 0 && !(task_4_status & BIT1))
            {
                task_4_status &= ~BIT0;
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
            task_1_status |= BIT0;
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
            task_2_status |= BIT0;
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
            task_3_status |= BIT0;
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
            task_4_status |= BIT0;
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
static void taskSuspend(int task)
{
    short i;

    switch(task)
    {
        case 1:
            task_1_status |= BIT1;

            SP_current = (unsigned short*) __get_SP_register();

            for(i=0;i < task_1_stack_size;i++)
            {
                task_1_stack[i] = (short) *(SP_current++);
            }

            SP_current--;
            __set_SP_register((unsigned short) SP_current);
            break;

        case 2:
            task_2_status |= BIT1;

            SP_current = (unsigned short*) __get_SP_register();

            for(i=0;i < task_2_stack_size;i++)
            {
                task_2_stack[i] = (short) *(SP_current++);
            }

            SP_current--;
            __set_SP_register((unsigned short) SP_current);
            break;

        case 3:
            task_3_status |= BIT1;

            SP_current = (unsigned short*) __get_SP_register();

            for(i=0;i < task_3_stack_size;i++)
            {
                task_3_stack[i] = (short) *(SP_current++);
            }

            SP_current--;
            __set_SP_register((unsigned short) SP_current);
            break;

        case 4:
            task_4_status |= BIT1;

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
static void taskContinue(int task)
{
    __disable_interrupt();

    switch(task)
    {
        case 1:
            task_1_status |= BIT2;
            break;
        case 2:
            task_2_status |= BIT2;
            break;
        case 3:
            task_3_status |= BIT2;
            break;
        case 4:
            task_4_status |= BIT2;
            break;
        default:
            break;
    }

    __enable_interrupt();

}

/*****************************************************************************/
// 0=off, 1=on
static void taskControl(int task, short control)
{
    __disable_interrupt();

    switch(task)
    {
        case 1:
            task_1_status = (control ? task_1_status | BIT3 : task_1_status & ~BIT3);
            break;
        case 2:
            task_2_status = (control ? task_2_status | BIT3 : task_2_status & ~BIT3);
            break;
        case 3:
            task_3_status = (control ? task_3_status | BIT3 : task_3_status & ~BIT3);
            break;
        case 4:
            task_4_status = (control ? task_4_status | BIT3 : task_4_status & ~BIT3);
            break;
        default:
            break;
    }

    __enable_interrupt();
}

/*****************************************************************************/
void taskTimeoutLock(short task, short count)
{
    __disable_interrupt();

    switch(task)
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
    unsigned int k = 0;

    task_1_stack_size = ((taskManager_stack_1 - (unsigned short) __get_SP_register())>>1) + 1;

    while(uartRxBuf[j])
    {
        if(uartRxBuf[j++] == CR)
        {
            for(i=0;i<j;i++)
            {
                if(uartRxBuf[i] == 'h')
                {
                    uartTx(uartTxBuf, 0, help);
                }
                else if(uartRxBuf[i] == 'v')
                {
                    uartTx(uartTxBuf, 0, version);
                }
                else if(uartRxBuf[i] == 'p')
                {
                    uartTx(uartTxBuf, 0, crlf);
                    for(k=1;k<lineNumber;k++)
                    {
                        binToAscii((char*) print, k);
                        uartTx(uartTxBuf, 0, print);
                        uartTx(uartTxBuf, 0, program[k]);
                    }
                    break;
                }
                else if(uartRxBuf[i] == 'e') // edit thread
                {
                    lineNumber = 0;
                    for(k=0;k<UART_RX_BUF_SIZE;k++)
                    {
                        uartRxBuf[k] = NULL;
                    }
                    uartRxBuf[0] = CR;
                    taskControl(TASK1,DISABLE);
                    taskControl(TASK2,ENABLE);
                    goto exit;
                }
            }

            uartTx(uartTxBuf, 0, prompt);

            for(i=0;i<UART_RX_BUF_SIZE;i++)
                uartRxBuf[i] = NULL;
        }
    };

exit:
    return 0;
}

/*****************************************************************************/
short task_2(void)
{
    unsigned short i = 0;
    unsigned short j = 0;
    unsigned short k = 0;

    task_2_stack_size = ((taskManager_stack_2 - (unsigned short) __get_SP_register())>>1) + 1;

    while(uartRxBuf[j])
    {
        if(uartRxBuf[j++] == CR)
        {
            for(k=0;k<j;k++)
            {
                program[lineNumber][k] = uartRxBuf[k];
            }
            program[lineNumber][j] = LF;

            lineNumber++;

            for(i=0;i<j;i++)
            {
                if(uartRxBuf[i] == '.') // exit edit thread
                {
                    for(k=0;k<UART_RX_BUF_SIZE;k++)
                    {
                        uartRxBuf[k] = NULL;
                    }

                    for(k=0;k<MAX_PROG_LINE_LEN;k++)
                    {
                        program[lineNumber-1][k] = NULL;
                    }

                    lineNumber--;
                    uartRxBuf[0] = CR;
                    taskControl(TASK1,ENABLE);
                    taskControl(TASK2,DISABLE);
                    goto exit;
                }
            }

            binToAscii((char*) line, lineNumber);
            uartTx(uartTxBuf, 0, line);

            for(i=0;i<UART_RX_BUF_SIZE;i++)
                uartRxBuf[i] = NULL;
        }
    };

exit:
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

/*****************************************************************************/
char* binToAscii(char* line, short val)
{

    if(val < 10)
    {
        line[2] = ' ';
        line[3] = (unsigned char) val + 0x30;
    }
    else if(val > 9)
    {

        line[2] = (unsigned char) (val/10) + 0x30;
        line[3] = (unsigned char) (val - ((val/10) * 10)) + 0x30;
    }

    return line;
}
