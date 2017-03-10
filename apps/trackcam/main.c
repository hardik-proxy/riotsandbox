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
 * @brief       Control a camera's view angle with servo and stepper
 *
 * @author      Hauke Petersen
 *
 * @}
 */

#include <stdio.h>

#include "servo.h"

int main(void)
{
    servo_t yaw;
    puts("Hello World!");

    printf("You are running RIOT on a(n) %s board.\n", RIOT_BOARD);
    printf("This board features a(n) %s MCU.\n", RIOT_MCU);

    servo_init(&yaw, PWM_0, 0, 1500, 2000);

    return 0;
}
