/*
 * Copyright (C) 2016 Hauke Petersen
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
 * @brief       Playbox for Morten
 *
 * @author      Hauke Petersen <devel@haukepetersen.de>
 *
 * @}
 */

#include <stdint.h>
#include <stdio.h>

#include "xtimer.h"

typedef struct {
    uint16_t duration;
    uint16_t freq;
} tone_t;

enum {
    P  = UINT16_MAX,
    C4 = 261,
    D4 = 294,
    E4 = 330,
    F4 = 349,
    G4 = 392,
    A4 = 440,
    B4 = 494,
    C5 = 523
};

static uint16_t moin[] = {
    { G4, G4, G4, D4, G4, G4, G4, D4, 0 },
    { 1, 1, 1, 4, 1, 1, 1, 4, 0 }
};

static void play(pwm_t pwm, tone_t *tune, int bpm)
{
    int i = 0;

    while (tune[0][i] > 0) {
        pwm_init(pwm, PWM_LEFT, tune[0][i], 100);
        pwm_set(pwm, 0, 50);
        xtimer_usleep((bpm * 2000) * tune[1][i]);
        pwm_stop(pwm);
    }
}


int main(void)
{

    play(&moin);


    return 0;
}
