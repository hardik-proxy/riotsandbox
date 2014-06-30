/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup tests
 * @{
 *
 * @file
 * @brief       Test input and output functionality of low-level GPIO driver
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>
#include <string.h>

#include "board.h"
#include "periph/uart.h"
#include "ringbuffer.h"
#include "hwtimer.h"

#define UART        UART_1
#define BAUDRATE    115200
#define BUFSIZE     128

char in_buf[BUFSIZE];
char out_buf[BUFSIZE];

ringbuffer_t in;
ringbuffer_t out;

volatile int datrdy = 0;

volatile int rx_count = 0;
volatile int tx_count = 0;

void rx_cb(char c)
{
    rx_count++;
    ringbuffer_add_one(&in, c);
    if (c == '\n') {
        datrdy = 1;
    }
}

void tx_cb(void)
{
    tx_count++;
    char c;
    if (out.avail > 0) {
        c = (char)ringbuffer_get_one(&out);
        uart_write(UART, c);
    } else {
        uart_tx_end(UART);
    }
}

int main(void)
{
    char *sendme = "I will be send every 2 seconds.\n";
    char rec;

    puts("UART driver test");

    puts("Initializing ringbuffers");
    ringbuffer_init(&in, in_buf, BUFSIZE);
    ringbuffer_init(&out, out_buf, BUFSIZE);

    puts("Initializing UART");
    uart_init(UART, BAUDRATE, rx_cb, tx_cb);



    while (1) {
        printf("RX interrupt count: %i\n", rx_count);
        printf("TX interrupt count: %i\n", tx_count);
        tx_count = 0;
        rx_count = 0;

        ringbuffer_add(&out, sendme, strlen(sendme));
        uart_tx_begin(UART);

        if (datrdy) {
            while (in.avail > 0) {
                rec = (char)ringbuffer_get_one(&in);
                printf("%c", rec);
            }
            printf("\n");
            datrdy = 0;
        }

        hwtimer_wait(2000 * 1000);
    }

}
