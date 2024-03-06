/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "hardware/gpio.h"
#include "pico/stdlib.h"

const int BTN_PIN_Y = 6;
const int BTN_PIN_R = 7;
const int BTN_PIN_B = 13;
const int BTN_PIN_G = 9;

volatile int yellow_flag = 0;
volatile int red_flag = 0;
volatile int blue_flag = 0;
volatile int green_flag = 0;

void gpio_callback(uint gpio, uint32_t events) {
    if (events == 0x4) {  // Fall edge
        if (gpio == BTN_PIN_Y)
            yellow_flag = 1;
        else if (gpio == BTN_PIN_R)
            red_flag = 1;
        else if (gpio == BTN_PIN_B)
            blue_flag = 1;
        else if (gpio == BTN_PIN_G)
            green_flag = 1;
    }
}

int main() {
    /* Game state management:
     * --> (0) idle
     * --> (1) ongoing: displaying instructions
     * --> (2) ongoing: receiving commands
     */
    int game_state = 0;

    stdio_init_all();

    gpio_init(BTN_PIN_Y);
    gpio_set_dir(BTN_PIN_Y, GPIO_IN);
    gpio_pull_up(BTN_PIN_Y);

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_init(BTN_PIN_B);
    gpio_set_dir(BTN_PIN_B, GPIO_IN);
    gpio_pull_up(BTN_PIN_B);

    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);

    gpio_set_irq_enabled_with_callback(BTN_PIN_B, GPIO_IRQ_EDGE_FALL, true,
                                     &gpio_callback);

    printf("Initializing script...");

    while (true) {
        if (game_state == 0) {
            if (yellow_flag || red_flag || green_flag || blue_flag) {
                printf("Player wants to start a new game!\n");


                //sleep_ms(100);
                yellow_flag = 0;
                red_flag = 0;
                green_flag = 0;
                blue_flag = 0;
                sleep_ms(100);
            }
        }
    }
}
