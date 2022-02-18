/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * uart.h
 *
 * Copyright (C) 2022 Alex Pogostin <alex.pogostin@outlook.com>
 *
 */

#ifndef UART_H
#define UART_H

#define BAUD_RATE_115200

#define UART_RX_BUF_SIZE 32
#define UART_TX_BUF_ARRAY_SIZE 4
#define UART_TX_BUF_SIZE 32

void uartInit(void);
short uartTx(unsigned char (*txStr)[UART_RX_BUF_SIZE], unsigned int node, unsigned char *str);

#endif
