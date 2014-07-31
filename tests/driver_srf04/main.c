/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @file        main.c
 * @brief       short test program for the SRF04 ultra-sonic sensor
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 */

#include <stdio.h>

#include "hwtimer.h"
#include "periph/gpio.h"

#define TRIGGER         GPIO_4
#define ECHO            GPIO_5

#define SPEED_OF_SOUND  (340290000UL)    /* m/us */


int main(void)
{
    uint32_t time1, time2;
    uint32_t pulse, distance;

    puts("SRF04 TESTING");

    /* here comes a very naive implementation */
    gpio_init_out(TRIGGER, GPIO_NOPULL);
    gpio_init_in(ECHO, GPIO_PULLDOWN);

    gpio_clear(TRIGGER);

    while (1) {
        /* step a: send a pulse with min. 20us on the trigger pin */
        gpio_set(TRIGGER);
        hwtimer_wait(25);
        gpio_clear(TRIGGER);

        while (gpio_read == 0);
        time1 = hwtimer_now();
        while (gpio_read > 0);
        time2 = hwtimer_now();

        if (time2 > time1) {
            pulse = time2 - time1;
        } else {
            pulse = ((1 << 32) - 1) - time1 + time2;
        }

        distance = SPEED_OF_SOUND * pulse / 2000;

        printf("Distance: %imm\n", distance);

        hwtimer_wait(100000);
    }

    return 0;
}
