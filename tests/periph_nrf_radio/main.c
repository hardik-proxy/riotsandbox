/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @file
 * @brief       Test for the low-level nrf51822 radio driver
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "shell.h"
#include "shell_commands.h"
#include "nrf51.h"
#include "vtimer.h"
#include "posix_io.h"
#include "board_uart0.h"

#define SHELL_BUFSIZE   (128U)

static char *default_msg = "Hello Other Node";


static void rx_cb(uint8_t *data, int length)
{
    printf("Successfully received %i byte\n", length);
    for (int i = 0; i < length; i++) {
        printf("%c", data[i]);
    }
    printf("\n");
}

static void send(int argc, char **argv)
{
    uint16_t addr;
    int res;

    if (argc < 2) {
        puts("usage: send ADDR [string]");
        return;
    }

    addr = (uint16_t)atoi(argv[1]);
    if (argc < 3) {
        printf("Sending %i byte to addr %i\n", strlen(default_msg), addr);
        res = nrf51_send(addr, (uint8_t *)default_msg, (uint8_t)strlen(default_msg));
    }
    else {
        printf("Sending %i byte to addr %i\n", strlen(argv[2]), addr);
        res = nrf51_send(addr, (uint8_t *)argv[2], (uint8_t)strlen(argv[2]));
    }

    if (res > 0) {
        printf("Successfully sent out %i byte to addr %i\n", res, addr);
    }
    else {
        printf("Sending failed, return code is %i\n", res);
    }
}

static void on(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    if (nrf51_set_state(NRF51_STATE_RX) == 0) {
        puts("Now listening for incoming data");
    } else {
        puts("Error while going into RX state");
    }
}

static void off(int argc, char **argv)
{
    (void)argc;
    (void)argv;

    if (nrf51_set_state(NRF51_STATE_OFF) == 0) {
        puts("Successfully disabled the radio");
    } else {
        puts("Error while disabling the radio");
    }
}

static void addr(int argc, char **argv)
{
    int addr;

    if (argc < 2) {
        addr = nrf51_get_address();
        printf("address: %i\n", addr);
    }
    else {
        addr = atoi(argv[1]);
        if (nrf51_set_address((uint16_t)addr) >= 0) {
            printf("address set to %i\n", addr);
        }
        else {
            puts("Error setting address\n");
        }
    }

}

static void chan(int argc, char **argv)
{
    int chan;

    if (argc < 2) {
        chan = nrf51_get_channel();
        printf("channel: %i\n", chan);
    }
    else {
        chan = atoi(argv[1]);
        if (nrf51_set_channel((uint8_t)chan) >= 0) {
            printf("channel set to %i\n", chan);
        }
        else {
            puts("Error setting channel\n");
        }
    }
}

static int shell_readc(void)
{
    return (int)getchar();
}

static void shell_putchar(int c)
{
    putchar(c);
}

static const shell_command_t shell_commands[] = {
    { "send", "send a packet", send },
    { "on", "put node into receive mode", on },
    { "off", "disable radio", off },
    { "addr", "set/get the radios address", addr },
    { "chan", "set/get the radio channel", chan },
    { NULL, NULL, NULL }
};

int main(void)
{

    printf("NRF RADIO test application\n");

    /* initialize the radio */
    nrf51_init();
    nrf51_register_receive_cb(rx_cb);

    /* run the shell */
    shell_t shell;
    (void) posix_open(uart0_handler_pid, 0);
    shell_init(&shell, shell_commands, SHELL_BUFSIZE, shell_readc, shell_putchar);
    shell_run(&shell);

    return 0;
}
