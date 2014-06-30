/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License. See the file LICENSE in the top level directory for more
 * details.
 */

/**
 * @ingroup tests
 * @{
 *
 * @file
 * @brief       Test input and output functionality of low-level GPIO driver
 *
 * @author      Hauke Petersen <hauke.petersen@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>

#include "periph_conf.h"
#include "periph/gpio.h"

#define ACTIVE_FLANK        GPIO_RISING
#define PULL                GPIO_PULLDOWN

uint32_t state = 0;
uint32_t old_state = 0;

void g0_cb(void){
    printf("GPIO_0 triggered\n");
}

void g1_cb(void){
    printf("GPIO_1 triggered\n");
}

void g2_cb(void){
    printf("GPIO_2 triggered\n");
}

void g3_cb(void){
    printf("GPIO_3 triggered\n");
}

void g4_cb(void){
    printf("GPIO_4 triggered\n");
}

void g5_cb(void){
    printf("GPIO_5 triggered\n");
}

void g6_cb(void){
    printf("GPIO_6 triggered\n");
}

void g7_cb(void){
    printf("GPIO_7 triggered\n");
}

void g8_cb(void){
    printf("GPIO_8 triggered\n");
}

void g9_cb(void){
    printf("GPIO_9 triggered\n");
}

void g10_cb(void){
    printf("GPIO_10 triggered\n");
}

void g11_cb(void){
    printf("GPIO_11 triggered\n");
}

int main(void)
{
    puts("GPIO driver test - external interrupts");

    puts("Initializing all GPIOs as external interrupt sources");
    gpio_init_int(GPIO_0, PULL, ACTIVE_FLANK, g0_cb);
    gpio_init_int(GPIO_1, PULL, ACTIVE_FLANK, g1_cb);
    gpio_init_int(GPIO_2, PULL, ACTIVE_FLANK, g2_cb);
    gpio_init_int(GPIO_3, PULL, ACTIVE_FLANK, g3_cb);
    gpio_init_int(GPIO_4, PULL, ACTIVE_FLANK, g4_cb);
    gpio_init_int(GPIO_5, PULL, ACTIVE_FLANK, g5_cb);
    gpio_init_int(GPIO_6, PULL, ACTIVE_FLANK, g6_cb);
    gpio_init_int(GPIO_7, PULL, ACTIVE_FLANK, g7_cb);
    gpio_init_int(GPIO_8, PULL, ACTIVE_FLANK, g8_cb);
    gpio_init_int(GPIO_9, PULL, ACTIVE_FLANK, g9_cb);
    gpio_init_int(GPIO_10, PULL, ACTIVE_FLANK, g10_cb);
    gpio_init_int(GPIO_11, PULL, ACTIVE_FLANK, g11_cb);

    puts("Initialization done, going to sleep now");

    return 0;
}
