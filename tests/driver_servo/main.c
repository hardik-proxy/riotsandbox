/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup     tests
 * @{
 *
 * @file
 * @brief       Test servo motor driver
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>

#include "periph/pwm.h"
#include "periph/gpio.h"
#include "servo.h"
#include "vtimer.h"


#define PWM         PWM_0
#define CHANNEL     (0U)

#define INTERVAL    (20 * 1000)     /* 30ms */
#define STEP        (25U)

#define MIN         500
#define MAX         2000
#define CENTER      1250

static servo_t servo;
static volatile int run = 0;
static int state = 0;

void event(void)
{
    run = 0;
    switch (state++) {
        case 0:
            puts("put to center");
            servo_set(&servo, CENTER);
            break;
        case 1:
            puts("put to far right");
            servo_set(&servo, MIN);
            break;
        case 2:
            puts("put to far left");
            servo_set(&servo, MAX);
            break;
        case 3:
            puts("moving back and forth");
            run = 1;
            break;
    }
    if (state == 4) {
        state = 0;
    }
}


int main(void)
{
    int pos = CENTER;
    int step = STEP;

    printf("RIOT Servo driver test\n");
    printf("A connected servo motor will gracefully go from one side to the other\n");

    servo_init(&servo, PWM, CHANNEL, MIN, MAX);

    gpio_init_int(GPIO_0, GPIO_PULLDOWN, GPIO_FALLING, event);

    while (1) {
        if (run) {
            printf("pos %i\n", pos);
            servo_set(&servo, pos);
            pos += step;
            if (pos >= MAX || pos <= MIN) {
                step *= -1;
            }

            vtimer_usleep(INTERVAL);
        }
    }

    return 0;
}
