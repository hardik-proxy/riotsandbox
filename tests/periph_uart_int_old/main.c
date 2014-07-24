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
#include "kernel.h"
#include "vtimer.h"
#include "board.h"
#include "ringbuffer.h"
#include "periph/uart.h"


#define DEV             UART_2
#define BAUD            38400

#define MSG_LINE_RDY    0
#define MSG_TICK        1

static volatile int main_pid;

static char uart_stack[KERNEL_CONF_STACKSIZE_PRINTF];

static char rx_mem[128];
static char tx_mem[128];
static ringbuffer_t rx_buf;
static ringbuffer_t tx_buf;


void rx(char data)
{
    msg_t msg;
    msg.type = MSG_LINE_RDY;

    ringbuffer_add_one(&rx_buf, data);
    if (data == '\n') {
        printf("got line end \n");
        msg_send(&msg, main_pid, 1);
    }
}

void tx(void)
{
    char data;

    if (tx_buf.avail > 0) {
        data = ringbuffer_get_one(&tx_buf);
        uart_write(DEV, data);
    }
    else {
        uart_tx_end(DEV);
    }
}

void* uart_thread(void *arg)
{
    char *status = "I am written to the UART every second\n";

    while (1) {
        ringbuffer_add(&tx_buf, status, strlen(status));
        uart_tx_begin(DEV);

        vtimer_usleep(1000 * 1000);
    }

    return 0;
}

int main(void)
{
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
    if (uart_init(DEV, BAUD, rx, tx) >= 0) {
        printf("   ...done\n");
    }
    else {
        printf("   ...failed\n");
        return 1;
    }

    printf("Creating uart status thread...");
    if (thread_create(uart_stack, KERNEL_CONF_STACKSIZE_PRINTF, PRIORITY_MAIN + 1,
                      0, uart_thread, 0, "UART_status") >= 0) {
        printf("   ...done\n");
    }
    else {
        printf("   ...failed\n");
        return 1;
    }

    while (1) {
        printf("Going into receive message state\n");
        msg_receive(&msg);

        printf("Got message\n");

        if (msg.type == MSG_LINE_RDY) {
            printf("INPUT: ");
            res = ringbuffer_get(&rx_buf, buf, rx_buf.avail);
            buf[res] = '\0';
            printf("%s", buf);
        }
    }

    return 0;
}
