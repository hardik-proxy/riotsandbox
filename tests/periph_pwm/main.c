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
 * @brief Float test application
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>

#include "cpu.h"
#include "board.h"
#include "vtimer.h"
#include "periph/pwm.h"
#include "periph/gpio.h"

#define SEC         (1000 * 1000)

#define DEV         PWM_0
#define CHAN        (4U)
#define MODE        PWM_LEFT

#define FREQU       (1000U)
#define STEPS       (1000U)

static int test_sequ[] = {0, 50, 100, 150, 200, 255, 1000};
static int current = 0;
static int length = 7;

void event(void)
{
    printf("set duty cycle to %i\n", test_sequ[current]);

    for (int chan = 0; chan < CHAN; chan++) {
        pwm_set(DEV, chan, test_sequ[current]);
    }
    current = (++current == length) ? 0 : current;
}

int main(void)
{
    int res;

    printf("RIOT PWM test\n");
    printf("Connect an RGB-LED or similar to PWM pins to see something\n");

    //gpio_init_int(GPIO_0, GPIO_PULLDOWN, GPIO_RISING, event);

    res = pwm_init(DEV, MODE, FREQU, STEPS);
    if (res == 0) {
        printf("PWM successfully initialized.\n");
    }
    else {
        printf("Errors while initializing PWM");
        return -1;
    }

    return 0;
}
