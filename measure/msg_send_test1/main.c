/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     measure
 * @{
 *
 * @file
 * @brief       Hello World application
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 * @author      Ludwig Ortmann <ludwig.ortmann@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>

#include "thread.h"
#include "msg.h"
#include "xtimer.h"
#include "periph/gpio.h"

#define PIN_BTN         GPIO(PORT_A, 0)
#define PIN_LED1        GPIO(PORT_D, 12)
#define PIN_LED2        GPIO(PORT_D, 14)

#define PIN_C0          GPIO(PORT_D, 0)
#define PIN_C1          GPIO(PORT_D, 1)
#define PIN_C2          GPIO(PORT_D, 2)
#define PIN_C3          GPIO(PORT_D, 3)

#define C0_ON           (GPIOD->BSRRL = (1 << 0))//gpio_set(PIN_C0)
#define C0_OFF          (GPIOD->BSRRH = (1 << 0))//gpio_clear(PIN_C0)
#define C1_ON           gpio_set(PIN_C1)
#define C1_OFF          gpio_clear(PIN_C1)
#define C2_ON           gpio_set(PIN_C2)
#define C2_OFF          gpio_clear(PIN_C2)
#define C3_ON           gpio_set(PIN_C3)
#define C3_OFF          gpio_clear(PIN_C3)

/* stacks */
static char s1[THREAD_STACKSIZE_DEFAULT];
static char s2[THREAD_STACKSIZE_DEFAULT];
static char s3[THREAD_STACKSIZE_DEFAULT];

/* toggle ipc thread pid */
static kernel_pid_t toggle_pid;

/* forward function declarations */
static void *thread_toggle_led(void *arg);
static void *thread_button_press(void *arg);
static void *thread_toggle_ipc(void *arg);
static void init_hw(void);

int main(void)
{
    init_hw();

    puts("hello world");

    /* skip creation of msg queue */

    /* create tasks */
    thread_create(s1, sizeof(s1), THREAD_PRIORITY_MAIN - 2, 0,
                  thread_toggle_led, NULL, "Task1");
    toggle_pid = thread_create(s3, sizeof(s3), THREAD_PRIORITY_MAIN - 2, 0,
                               thread_toggle_ipc, NULL, "Task3");
    thread_create(s2, sizeof(s2), THREAD_PRIORITY_MAIN - 2, 0,
                  thread_button_press, NULL, "Task2");

    return 0;
}

void *thread_toggle_led(void *arg)
{
    (void)arg;
    uint32_t last = xtimer_now();

    while (1) {
        gpio_toggle(PIN_LED1);
        xtimer_usleep_until(&last, (1500 * 1000));
    }

    return NULL;
}

void *thread_button_press(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        if (gpio_read(PIN_BTN)) {
            while (gpio_read(PIN_BTN) > 0) {
                xtimer_usleep(100 * 1000);
            }
            while (gpio_read(PIN_BTN) == 0) {
                xtimer_usleep(100 * 1000);
            }

            C0_ON;
            msg_send(&msg, toggle_pid);
        }
    }
    return NULL;
}

static void *thread_toggle_ipc(void *arg)
{
    (void)arg;
    msg_t msg;

    while (1) {
        msg_receive(&msg);
        C0_OFF;

        gpio_toggle(PIN_LED2);
    }
    return NULL;
}

static void init_hw(void)
{
    gpio_init(PIN_LED1, GPIO_DIR_OUT, GPIO_NOPULL);
    gpio_init(PIN_LED2, GPIO_DIR_OUT, GPIO_NOPULL);
    gpio_init(PIN_BTN, GPIO_DIR_IN, GPIO_NOPULL);

    /* debug pins */
    gpio_init(PIN_C0, GPIO_DIR_OUT, GPIO_NOPULL);
    gpio_init(PIN_C1, GPIO_DIR_OUT, GPIO_NOPULL);
    gpio_init(PIN_C2, GPIO_DIR_OUT, GPIO_NOPULL);
    gpio_init(PIN_C3, GPIO_DIR_OUT, GPIO_NOPULL);
}
