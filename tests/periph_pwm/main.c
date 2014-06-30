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

#include "board.h"
#include "vtimer.h"
#include "periph/pwm.h"

#define SEC         (1000 * 1000)

int main(void)
{
    int res;

    printf("RIOT PWM test\n");
    printf("Connect an RGB-LED or similar to PWM pins to see something\n");

    res = pwm_init(PWM_0, PWM_LEFT, 1000, 255);
    if (res == 0) {
        printf("PWM_0 successfully initialized.\n");
    }
    else {
        printf("Errors while initializing PWM_0");
        return -1;
    }


    while (1) {
        vtimer_usleep(SEC);
        printf("test\n");
        pwm_set(PWM_0, 0, 50);
        pwm_set(PWM_0, 1, 0);
        pwm_set(PWM_0, 2, 0);

        vtimer_usleep(SEC);
        printf("test\n");
        pwm_set(PWM_0, 0, 0);
        pwm_set(PWM_0, 1, 0);
        pwm_set(PWM_0, 2, 125);

        vtimer_usleep(SEC);
        printf("test\n");
        pwm_set(PWM_0, 0, 0);
        pwm_set(PWM_0, 1, 200);
        pwm_set(PWM_0, 2, 0);
    }

    return 0;
}
