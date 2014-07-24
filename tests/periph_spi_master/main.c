/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup tests
 * @{
 *
 * @file
 * @brief       SPI test application
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "vtimer.h"
#include "shell.h"
#include "periph/spi.h"
#include "periph/gpio.h"

#define SHELL_BUFSIZE       (128U)
#define SPI_BUFFER          (256U)

#define DEV                 SPI_0
#define MODE                SPI_CONF_FIRST_RISING
#define CLOCK               SPI_SPEED_1MHZ
#define CS                  GPIO_3


static void send_hello(int argc, char **argv);
static void write_reg(int argc, char **argv);
static void read_reg(int argc, char **argv);
static void write_regs(int argc, char **argv);
static void read_regs(int argc, char **argv);
static void write_block(int argc, char **argv);
static void read_block(int argc, char **argv);


static char buffer[SPI_BUFFER];
static char *hello = " Hello Slave!!";


static const shell_command_t shell_commands[] = {
    { "send_hello", "", send_hello },
    { "write_reg", "", write_reg },
    { "read_reg", "", read_reg },
    { "write_regs", "", write_regs },
    { "read_regs", "", read_regs },
    { "write_block", "", write_block },
    { "read_block", "", read_block },
    { NULL, NULL, NULL }
};

static void send_hello(int argc, char **argv)
{
    int res;

    gpio_clear(CS);
    res = spi_transfer_bytes(DEV, hello, buffer, strlen(hello));
    gpio_set(CS);

    buffer[res] = '\0';
    printf("Response: %s\n", buffer);
}

static void write_reg(int argc, char **argv)
{

}

static void read_reg(int argc, char **argv)
{

}

static void write_regs(int argc, char **argv)
{

}

static void read_regs(int argc, char **argv)
{

}

static void write_block(int argc, char **argv)
{

}

static void read_block(int argc, char **argv)
{

}

int shell_getchar(void)
{
    return (int)getchar();
}

void shell_putchar(int c)
{
    putchar((char)c);
}

int main(void)
{
    int res;
    shell_t shell;

    puts("RIOT SPI Master test");
    puts("desc...\n");

    /* initialize the CS line */
    printf("Initializing the CS line");
    res = gpio_init_out(CS, GPIO_PULLUP);
    if (res >= 0) {
        puts("     ...[ok]\n");
    }
    else {
        puts("     ...[failed]");
        return -1;
    }
    gpio_set(CS);

    /* initialize the SPI master */
    printf("Initializing the SPI device");
    res = spi_init_master(DEV, MODE, CLOCK);
    if (res >= 0) {
        puts("     ...[ok]\n");
    }
    else {
        puts("     ...[failed]");
    }

    /* run the shell */
    puts("Starting the shell     ...[ok]");         /* we trust it... */
    shell_init(&shell, shell_commands, SHELL_BUFSIZE, shell_getchar, shell_putchar);
    shell_run(&shell);

    return 0;
}
