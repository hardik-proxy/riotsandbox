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
#include <stdint.h>

#include "periph_conf.h"
#include "periph/gpio.h"

#define ACTIVE_FLANK        GPIO_RISING
#define PULL                GPIO_NOPULL

uint32_t state = 0;
uint32_t old_state = 0;

void g0_cb(void *arg){
    printf("GPIO_0 triggered\n");
}

void g1_cb(void *arg){
    printf("GPIO_1 triggered\n");
}

void g2_cb(void *arg){
    printf("GPIO_2 triggered\n");
}

void g3_cb(void *arg){
    printf("GPIO_3 triggered\n");
}

void g4_cb(void *arg){
    printf("GPIO_4 triggered\n");
}

void g5_cb(void *arg){
    printf("GPIO_5 triggered\n");
}

void g6_cb(void *arg){
    printf("GPIO_6 triggered\n");
}

void g7_cb(void *arg){
    printf("GPIO_7 triggered\n");
}

void g8_cb(void *arg){
    printf("GPIO_8 triggered\n");
}

void g9_cb(void *arg){
    printf("GPIO_9 triggered\n");
}

void g10_cb(void *arg){
    printf("GPIO_10 triggered\n");
}

void g11_cb(void *arg){
    printf("GPIO_11 triggered\n");
}

#if GPIO_12_EN
void g12_cb(void *arg){
    printf("GPIO_12 triggered\n");
}
#endif

#if GPIO_13_EN
void g13_cb(void *arg){
    printf("GPIO_13 triggered\n");
}
#endif

#if GPIO_14_EN
void g14_cb(void *arg){
    printf("GPIO_14 triggered\n");
}
#endif

#if GPIO_15_EN
void g15_cb(void *arg){
    printf("GPIO_15 triggered\n");
}
#endif

int main(void)
{
    puts("GPIO driver test - external interrupts");

    puts("Initializing all GPIOs as external interrupt sources");
    gpio_init_int(GPIO_0, PULL, ACTIVE_FLANK, g0_cb, 0);
    gpio_init_int(GPIO_1, PULL, ACTIVE_FLANK, g1_cb, 0);
    gpio_init_int(GPIO_2, PULL, ACTIVE_FLANK, g2_cb, 0);
    gpio_init_int(GPIO_3, PULL, ACTIVE_FLANK, g3_cb, 0);
    gpio_init_int(GPIO_4, PULL, ACTIVE_FLANK, g4_cb, 0);
    gpio_init_int(GPIO_5, PULL, ACTIVE_FLANK, g5_cb, 0);
    gpio_init_int(GPIO_6, PULL, ACTIVE_FLANK, g6_cb, 0);
    gpio_init_int(GPIO_7, PULL, ACTIVE_FLANK, g7_cb, 0);
    gpio_init_int(GPIO_8, PULL, ACTIVE_FLANK, g8_cb, 0);
    gpio_init_int(GPIO_9, PULL, ACTIVE_FLANK, g9_cb, 0);
    gpio_init_int(GPIO_10, PULL, ACTIVE_FLANK, g10_cb, 0);
    gpio_init_int(GPIO_11, PULL, ACTIVE_FLANK, g11_cb, 0);
#if GPIO_12_EN
    gpio_init_int(GPIO_12, PULL, ACTIVE_FLANK, g12_cb, 0);
#endif
#if GPIO_13_EN
    gpio_init_int(GPIO_13, PULL, ACTIVE_FLANK, g13_cb, 0);
#endif
#if GPIO_14_EN
    gpio_init_int(GPIO_14, PULL, ACTIVE_FLANK, g14_cb, 0);
#endif
#if GPIO_15_EN
    gpio_init_int(GPIO_15, PULL, ACTIVE_FLANK, g15_cb, 0);
#endif

    puts("Initialization done, going to sleep now");

    return 0;
}
