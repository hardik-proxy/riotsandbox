/*
 * Copyright (C) 2014 Hauke Petersen
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     trackcam
 * @{
 *
 * @file
 * @brief       Forward data from STDIO to another UART device
 *
 * @author      Hauke Petersen
 *
 * @}
 */

#include <stdio.h>

#include "kernel.h"
#include "thread.h"
#include "periph/uart.h"

#define OTHER           UART_1
#define OTHER_BAUD      (115200U)

char stdin_stack[KERNEL_CONF_STACKSIZE_MAIN];


void *stdin_thread(void *arg)
{
    (void)arg;

    puts("STDIN thread started");

    while (1) {
        char c = getchar();
        uart_write_blocking(OTHER, c);
    }

    return NULL;
}

void event(void *arg, char c)
{
    (void)arg;

    printf("%c", c);
}

int main(void)
{
    puts("UART relay/forwarder");

    if (uart_init(OTHER, OTHER_BAUD, event, NULL, NULL) < 0) {
        puts("ERROR: unable to initialize other UART");
        return 1;
    }
    else {
        printf("UART_%i initialized\n", OTHER);
    }

    (void) thread_create(stdin_stack, sizeof(stdin_stack), PRIORITY_MAIN - 1, 0,
                         stdin_thread, NULL, "other");
    return 0;
}
