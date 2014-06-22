/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

#include <stdio.h>
#include <string.h>

#include "cpu.h"
#include "msg.h"
#include "thread.h"
#include "vtimer.h"
#include "board.h"
#include "ringbuffer.h"
#include "periph/uart.h"


#define DEV             UART_1
#define BAUD            9600

#define MSG_LINE_RDY    0
#define MSG_TICK        1

static volatile int main_pid;

static char rx_mem[128];
static char tx_mem[128];
static ringbuffer_t rx_buf;
static ringbuffer_t tx_buf;


void rx(void *ptr, char data)
{
    msg_t msg;
    msg.type = MSG_LINE_RDY;

    ringbuffer_add_one(&rx_buf, data);
    if (data == '\n') {
        printf("got line end \n");
        msg_send(&msg, 1, 0);
    }
}

int tx(void *ptr)
{
    char data;

    if (tx_buf.avail > 0) {
        data = ringbuffer_get_one(&tx_buf);
        uart_write(DEV, data);
        return 1;
    }

    return 0;
}


int main(void)
{
    //char *status = "I am written to the UART every 2 seconds\n";
    char buf[128];
    int res;
    msg_t msg;

    main_pid = thread_getpid();
    printf("Main thread pid %i \n", main_pid);

    printf("Testing interrupt driven mode of UART driver\n\n");

    printf("Setting up buffers...\n");
    ringbuffer_init(&rx_buf, rx_mem, 128);
    ringbuffer_init(&tx_buf, tx_mem, 128);

    printf("Initializing UART @ %i", BAUD);
    if (uart_init(DEV, BAUD, rx, NULL, tx, NULL) >= 0) {
        printf("   ...done\n");
    }
    else {
        printf("   ...failed\n");
        return 1;
    }

    while (1) {
        printf("Going into receive message state\n");
        msg_receive(&msg);

        printf("got message");

        if (msg.type == MSG_LINE_RDY) {
            printf("INPUT: ");
            res = ringbuffer_get(&rx_buf, buf, rx_buf.avail);
            buf[res] = '\0';
            printf("%s", buf);
        }
    }

    return 0;
}
