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
extern unsigned char token_tree[MAX_TOKEN_TREE_SIZE];
extern int token_pointer;

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
static unsigned short run_state;


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

static unsigned char crlf[]      = CRLF;
static unsigned char edit_line[] = EDIT_LINE;
static unsigned char prompt[]    = CRLF PROMPT;
static unsigned char line[]      = CRLF EDIT_LINE;
static unsigned char print[]     = PRINT_LINE;
static unsigned char key_words[] = CRLF
                                  "is" CRLF
                                  "repeat" CRLF
                                  "set" CRLF
                                  "pause" CRLF
                                  "call" CRLF
                                  "=" CRLF
                                  "in" CRLF
                                  "out" CRLF
                                  "on" CRLF
                                  "off" CRLF
                                  "yes:" CRLF
                                  "no:" CRLF
                                  "led1" CRLF
                                  "led2" CRLF
                                  "but1" CRLF
                                  "but2" CRLF
                                  "pressed" CRLF
                                  "finish" CRLF
                                  "rerun";
static unsigned char help[]      = CRLF
                                  "h: help" CRLF
                                  "v: version" CRLF
                                  "p: print key words" CRLF
                                  "e: edit program" CRLF
                                  "r: run program" CRLF
                                  "l: list program" CRLF
                                  "--------------------" CRLF
                                  ".: exit program edit";
static unsigned char version[] = CRLF VERSION;

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

    while(TRUE)
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
short task_1(void) // command mode
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
                if(uartRxBuf[i] == 'p')
                {
                    uartTx(uartTxBuf, 0, key_words);
                }
                else if(uartRxBuf[i] == 'v')
                {
                    uartTx(uartTxBuf, 0, version);
                }
                else if(uartRxBuf[i] == 'l')
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
                    taskControl(TASK1, DISABLE);
                    taskControl(TASK2, ENABLE);
                    goto exit;
                }
                else if(uartRxBuf[i] == 'r') // run thread
                {
                    taskControl(TASK1, DISABLE);
                    taskControl(TASK3, ENABLE);
                    goto exit;
                }

            }

            uartTx(uartTxBuf, 0, prompt);

            for(i=0;i<UART_RX_BUF_SIZE;i++)
            {
                uartRxBuf[i] = NULL;
            }
        }
    };

exit:
    return 0;
}

/*****************************************************************************/
short task_2(void) // edit mode task
{
    unsigned short i = 0;
    unsigned short j = 0;
    unsigned short k = 0;

    task_2_stack_size = ((taskManager_stack_2 - (unsigned short) __get_SP_register())>>1) + 1;

    while(uartRxBuf[j])
    {
        if(uartRxBuf[j++] == CR)
        {
            for(k=0;k<UART_RX_BUF_SIZE;k++)
            {
                program[lineNumber][k] = NULL;
            }

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
                    taskControl(TASK1, ENABLE);
                    taskControl(TASK2, DISABLE);
                    goto exit;
                }
            }

            binToAscii((char*) line, lineNumber);
            uartTx(uartTxBuf, 0, line);

            for(i=0;i<UART_RX_BUF_SIZE;i++)
            {
                uartRxBuf[i] = NULL;
            }
        }
    };

exit:
    return 0;
}

/*****************************************************************************/
short task_3(void) // interpreter task
{
    unsigned int i;

    task_3_stack_size = ((taskManager_stack_3 - (unsigned short) __get_SP_register())>>1) + 1;

    uartTx(uartTxBuf, 0, crlf);

    for(i=0;i<MAX_TOKEN_TREE_SIZE;i++)
        token_tree[i] = 0;

    token_pointer = 0;

    for(i=1;i<lineNumber;i++)
    {
        if(lexer(program[i]))
        {
            debug(uartTxBuf, 0, token_tree);
            debug(uartTxBuf, 0, edit_line);
            debug(uartTxBuf, 0, program[i]);
        }
    }

    //interpreter((unsigned char*) &token_tree);

    for(i=0;i<UART_RX_BUF_SIZE;i++)
    {
        uartRxBuf[i] = NULL;
    }

    uartRxBuf[0] = CR;

    taskControl(TASK3, DISABLE);
    taskControl(TASK4, ENABLE);

    return 0;
}

/*****************************************************************************/
short task_4(void) // run task
{
    static int i;
    int j = 0, k;

    task_4_stack_size = ((taskManager_stack_4 - (unsigned short) __get_SP_register())>>1) + 1;

    if(!i)
    {
        run_state = token_tree[0];
    }

    while(uartRxBuf[j])
    {
        if(uartRxBuf[j] == CTRL_C)
        {
            for(k=0;k<UART_RX_BUF_SIZE;k++)
            {
                uartRxBuf[k] = NULL;
            }
            debug(uartTxBuf, 0, "ctrl-c\r\n");
            uartRxBuf[0] = CR;
            run_state = 0x00; // program terminated
            break;
        }
        j++;
    };

    switch(run_state)
    {
            case 'A': // is
            debug(uartTxBuf, 0, "A\r\n");
            run_state = token_tree[++i];
            break;

            case 'K': // yes:
            debug(uartTxBuf, 0, "K\r\n");
            run_state = token_tree[++i];
            break;

            case 'L': // no:
            debug(uartTxBuf, 0, "L\r\n");
            run_state = token_tree[++i];
            break;

            case 'M': // led1
            debug(uartTxBuf, 0, "M\r\n");
            run_state = token_tree[++i];
            break;

            case 'N': // led2
            debug(uartTxBuf, 0, "N\r\n");
            run_state = token_tree[++i];
            break;

            case 'O': // but1
            debug(uartTxBuf, 0, "O\r\n");
            run_state = token_tree[++i];
            break;

            case 'P': // but2
            debug(uartTxBuf, 0, "P\r\n");
            run_state = token_tree[++i];
            break;

            case 'Q': // pressed
            debug(uartTxBuf, 0, "Q\r\n");
            run_state = token_tree[++i];
            break;

            case 'R': // finish
            uartTx(uartTxBuf, 0, "program finished\r\n");
            taskControl(TASK4, DISABLE);
            taskControl(TASK1, ENABLE);
            i = 0;
            break;

            case 'S': // rerun
            debug(uartTxBuf, 0, "S\r\n");
            i = 0;
            break;

            default:
            uartTx(uartTxBuf, 0, "program terminated\r\n");
            taskControl(TASK4, DISABLE);
            taskControl(TASK1, ENABLE);
            i = 0;
            break;
    }

    return 0;
}
