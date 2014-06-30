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
 * @brief       Test application for testing the RGB-LED driver
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>

#include "vtimer.h"
#include "rgbled.h"
#include "color.h"
#include "periph/pwm.h"


int main(void)
{
    rgbled_t led;
    color_rgb_t rgb;
    color_hsv_t hsv = {0.0f, 1.0f, 1.0f};

    printf("RIOT RGB-LED test\n");

    rgbled_init(&led, PWM_0, 0, 1, 2);

    while(1) {
        color_hsv2rgb(&hsv, &rgb);
        rgbled_set(&led, &rgb);

        hsv.h += 0.5f;
        if (hsv.h > 360.0f) {
            hsv.h = 0.0f;
        }

        vtimer_usleep(5 * 1000);
    }

    return 0;
}
