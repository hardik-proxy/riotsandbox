/*
 * Copyright (C) 2014 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License v2.1. See the file LICENSE in the top level directory for more
 * details.
 */




#include <stdio.h>
#include <color.h>


void print_hsv(color_hsv_t *hsv)
{
    int hh = (int)hsv->h;
    int hl = (int)((hsv->h - hh) * 100);
    int sh = (int)hsv->s;
    int sl = (int)((hsv->s - sh) * 100);
    int vh = (int)hsv->v;
    int vl = (int)((hsv->v - vh) * 100);

    printf("H: %3i.%02i S: %01i.%02i V: %01i.%02i\n", hh, hl, sh, sl, vh, vl);
}

void print_rgb(color_rgb_t *rgb)
{
    printf("R:   0x%02x G: 0x%02x B: 0x%02x\n", rgb->r, rgb->g, rgb->b);
}

void hsv_set(color_hsv_t *hsv, float h, float s, float v)
{
    hsv->h = h;
    hsv->s = s;
    hsv->v = v;
}

void rgb_set(color_rgb_t *rgb, uint8_t r, uint8_t g, uint8_t b)
{
    rgb->r = r;
    rgb->g = g;
    rgb->b = b;
}


int main(void)
{
    color_hsv_t hsv1, hsv2;
    color_rgb_t rgb1, rgb2;

    printf("Test the color module\n");

    printf("\nInput: ");
    rgb_set(&rgb1, 255, 255, 255);
    print_rgb(&rgb1);
    color_rgb2hsv(&rgb1, &hsv1);
    printf("OUT1 : ");
    print_hsv(&hsv1);
    color_hsv2rgb(&hsv1, &rgb2);
    printf("OUT2 : ");
    print_rgb(&rgb2);

    printf("\nInput: ");
    rgb_set(&rgb1, 255, 0, 0);
    print_rgb(&rgb1);
    color_rgb2hsv(&rgb1, &hsv1);
    printf("OUT1 : ");
    print_hsv(&hsv1);
    color_hsv2rgb(&hsv1, &rgb2);
    printf("OUT2 : ");
    print_rgb(&rgb2);

    printf("\nInput: ");
    rgb_set(&rgb1, 0, 255, 0);
    print_rgb(&rgb1);
    color_rgb2hsv(&rgb1, &hsv1);
    printf("OUT1 : ");
    print_hsv(&hsv1);
    color_hsv2rgb(&hsv1, &rgb2);
    printf("OUT2 : ");
    print_rgb(&rgb2);

    printf("\nInput: ");
    rgb_set(&rgb1, 0, 0, 255);
    print_rgb(&rgb1);
    color_rgb2hsv(&rgb1, &hsv1);
    printf("OUT1 : ");
    print_hsv(&hsv1);
    color_hsv2rgb(&hsv1, &rgb2);
    printf("OUT2 : ");
    print_rgb(&rgb2);

    printf("\nInput: ");
    rgb_set(&rgb1, 100, 100, 100);
    print_rgb(&rgb1);
    color_rgb2hsv(&rgb1, &hsv1);
    printf("OUT1 : ");
    print_hsv(&hsv1);
    color_hsv2rgb(&hsv1, &rgb2);
    printf("OUT2 : ");
    print_rgb(&rgb2);

    printf("\nInput: ");
    rgb_set(&rgb1, 100, 0, 0);
    print_rgb(&rgb1);
    color_rgb2hsv(&rgb1, &hsv1);
    printf("OUT1 : ");
    print_hsv(&hsv1);
    color_hsv2rgb(&hsv1, &rgb2);
    printf("OUT2 : ");
    print_rgb(&rgb2);

    printf("\nInput: ");
    rgb_set(&rgb1, 0, 100, 0);
    print_rgb(&rgb1);
    color_rgb2hsv(&rgb1, &hsv1);
    printf("OUT1 : ");
    print_hsv(&hsv1);
    color_hsv2rgb(&hsv1, &rgb2);
    printf("OUT2 : ");
    print_rgb(&rgb2);

    printf("\nInput: ");
    rgb_set(&rgb1, 0, 0, 100);
    print_rgb(&rgb1);
    color_rgb2hsv(&rgb1, &hsv1);
    printf("OUT1 : ");
    print_hsv(&hsv1);
    color_hsv2rgb(&hsv1, &rgb2);
    printf("OUT2 : ");
    print_rgb(&rgb2);



    printf("\nInput: ");
    hsv_set(&hsv1, 360.0, 1.0, 1.0);
    print_hsv(&hsv1);
    color_hsv2rgb(&hsv1, &rgb1);
    printf("OUT1 : ");
    print_rgb(&rgb1);
    color_rgb2hsv(&rgb1, &hsv2);
    printf("OUT2 : ");
    print_hsv(&hsv2);

    printf("\nInput: ");
    hsv_set(&hsv1, 0.0, 1.0, 1.0);
    print_hsv(&hsv1);
    color_hsv2rgb(&hsv1, &rgb1);
    printf("OUT1 : ");
    print_rgb(&rgb1);
    color_rgb2hsv(&rgb1, &hsv2);
    printf("OUT2 : ");
    print_hsv(&hsv2);

    printf("\nInput: ");
    hsv_set(&hsv1, 120.0, 1.0, 1.0);
    print_hsv(&hsv1);
    color_hsv2rgb(&hsv1, &rgb1);
    printf("OUT1 : ");
    print_rgb(&rgb1);
    color_rgb2hsv(&rgb1, &hsv2);
    printf("OUT2 : ");
    print_hsv(&hsv2);

    printf("\nInput: ");
    hsv_set(&hsv1, 240.0, 1.0, 1.0);
    print_hsv(&hsv1);
    color_hsv2rgb(&hsv1, &rgb1);
    printf("OUT1 : ");
    print_rgb(&rgb1);
    color_rgb2hsv(&rgb1, &hsv2);
    printf("OUT2 : ");
    print_hsv(&hsv2);

    printf("\nInput: ");
    hsv_set(&hsv1, 60.0, 1.0, 1.0);
    print_hsv(&hsv1);
    color_hsv2rgb(&hsv1, &rgb1);
    printf("OUT1 : ");
    print_rgb(&rgb1);
    color_rgb2hsv(&rgb1, &hsv2);
    printf("OUT2 : ");
    print_hsv(&hsv2);

    printf("\nInput: ");
    hsv_set(&hsv1, 180.0, 1.0, 1.0);
    print_hsv(&hsv1);
    color_hsv2rgb(&hsv1, &rgb1);
    printf("OUT1 : ");
    print_rgb(&rgb1);
    color_rgb2hsv(&rgb1, &hsv2);
    printf("OUT2 : ");
    print_hsv(&hsv2);

    printf("\nInput: ");
    hsv_set(&hsv1, 300.0, 1.0, 1.0);
    print_hsv(&hsv1);
    color_hsv2rgb(&hsv1, &rgb1);
    printf("OUT1 : ");
    print_rgb(&rgb1);
    color_rgb2hsv(&rgb1, &hsv2);
    printf("OUT2 : ");
    print_hsv(&hsv2);



    return 0;
}
