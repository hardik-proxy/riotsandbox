/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "cpu.h"

#include "vtimer.h"
#include "board.h"
#include "periph/uart.h"

static int baudrates[] = {115200, 57600, 9600, 38400, 115200, 115200};


void uart_print(uart_t dev, char *str)
{
    int i = 0;

    while (str[i] != '\0') {
        uart_write_blocking(dev, str[i++]);
    }
}

int main(void)
{
    char *hello = "Testing this UART device:\n";
    char *ask = "Please enter a few chars with newline in the end\n";
    char buf[128];
    int p, i;
    char tmp;

    printf("Test UART driver in blocking mode\n\n");
    printf("Setting up remaining UART devices:\n");
    for (int i = UART_0; i < UART_NUMOF; i++) {
        if (i != STDIO) {
            printf("Setting up UART_%i @ %i", i, baudrates[i]);
            if (uart_init_blocking(i, baudrates[i]) >= 0) {
                printf("   ...ok\n");
            } else {
                printf("   ...failed\n");
                return 1;
            }
        }
    }
    printf("\n");

    for (i = UART_0; i < UART_NUMOF; i++) {
        if (i != STDIO) {
            printf("Please Connect to UART_%i @ %i now, press return when done\n", i, baudrates[i]);
            do {
                tmp = getchar();
            } while (tmp != '\n');

            uart_print(i, hello);
            uart_print(i, ask);
            p = 0;
            memset(buf, 0, 128);
            printf("Input was: ");
            do {
                uart_read_blocking(i, &buf[p++]);
                printf("%c", buf[p - 1]);
            } while (buf[p - 1] != '\n');
            buf[p] = '\0';
            uart_print(i, buf);
            printf("\n");
        }
    }

    printf("If you were able to see your inputs on each UART device, the test was successful!\n");

    return 0;
}
