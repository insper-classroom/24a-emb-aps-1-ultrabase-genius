/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "hardware/gpio.h"
#include "pico/stdlib.h"

#define DEBOUNCE_TIME 200

#define LED_B 28
#define LED_G 27
#define LED_R 22
#define LED_Y 20

#define BTN_B 7
#define BTN_G 9
#define BTN_R 10
#define BTN_Y 13

#define BUZZER 5

volatile int blue_flag = 0;
volatile int green_flag = 0;
volatile int red_flag = 0;
volatile int yellow_flag = 0;

void gpio_callback(uint gpio, uint32_t events) {
    if (events == 0x4) {  // fall edge
        if (gpio == BTN_B) {
            blue_flag = 1;
        }
        if (gpio == BTN_G) {
            green_flag = 1;
        }
        if (gpio == BTN_R) {
            red_flag = 1;
        }
        if (gpio == BTN_Y) {
            yellow_flag = 1;
        }
    }
}

bool any_flag() { return yellow_flag || red_flag || green_flag || blue_flag; }

void blue_bip() {
    uint32_t comparison_time1;
    comparison_time1 = to_ms_since_boot(get_absolute_time());
    for (int x = 0; x < 300; x++) {
        gpio_put(BUZZER, 1);
        sleep_us(1000);
        gpio_put(BUZZER, 0);
        sleep_us(1000);
    }
    while (comparison_time1 + 1000 > to_ms_since_boot(get_absolute_time())) {
    }
}

void green_bip() {
    uint32_t comparison_time2;
    comparison_time2 = to_ms_since_boot(get_absolute_time());
    for (int x = 0; x < 300; x++) {
        gpio_put(BUZZER, 1);
        sleep_us(666);
        gpio_put(BUZZER, 0);
        sleep_us(666);
    }
    while (comparison_time2 + 1000 > to_ms_since_boot(get_absolute_time())) {
    }
}
void red_bip() {
    uint32_t comparison_time3;
    comparison_time3 = to_ms_since_boot(get_absolute_time());
    for (int x = 0; x < 300; x++) {
        gpio_put(BUZZER, 1);
        sleep_us(444);
        gpio_put(BUZZER, 0);
        sleep_us(444);
    }
    while (comparison_time3 + 1000 > to_ms_since_boot(get_absolute_time())) {
    }
}
void yellow_bip() {
    uint32_t comparison_time4;
    comparison_time4 = to_ms_since_boot(get_absolute_time());
    for (int x = 0; x < 300; x++) {
        gpio_put(BUZZER, 1);
        sleep_us(290);
        gpio_put(BUZZER, 0);
        sleep_us(290);
    }
    while (comparison_time4 + 1000 > to_ms_since_boot(get_absolute_time())) {
    }
}

int main() {
    stdio_init_all();

    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);

    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);

    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);

    gpio_init(LED_Y);
    gpio_set_dir(LED_Y, GPIO_OUT);

    gpio_init(BTN_B);
    gpio_set_dir(BTN_B, GPIO_IN);
    gpio_pull_up(BTN_B);

    gpio_init(BTN_G);
    gpio_set_dir(BTN_G, GPIO_IN);
    gpio_pull_up(BTN_G);

    gpio_init(BTN_R);
    gpio_set_dir(BTN_R, GPIO_IN);
    gpio_pull_up(BTN_R);

    gpio_init(BTN_Y);
    gpio_set_dir(BTN_Y, GPIO_IN);
    gpio_pull_up(BTN_Y);

    gpio_init(BUZZER);
    gpio_set_dir(BUZZER, GPIO_OUT);

    gpio_set_irq_enabled_with_callback(
        BTN_B, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled(BTN_G, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(BTN_R, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(BTN_Y, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_FALL, true);

    int last_blue_flag = 0;
    int last_green_flag = 0;
    int last_red_flag = 0;
    int last_yellow_flag = 0;

    while (true) {
        // if (any_flag()) {
        //     printf("[%d] sleeping to avoid double clicks...\n", i);
        //     sleep_ms(200);
        // }

        if (blue_flag) {
            blue_flag = 0;
            if (to_ms_since_boot(get_absolute_time()) - last_blue_flag >
                DEBOUNCE_TIME) {
                last_blue_flag = to_ms_since_boot(get_absolute_time());
                printf("Blue click\n");
            }
            // gpio_put(LED_B, 1);
            // blue_bip();
            // gpio_put(LED_B, 0);
        }
        if (green_flag) {
            green_flag = 0;
            if (to_ms_since_boot(get_absolute_time()) - last_green_flag >
                DEBOUNCE_TIME) {
                last_green_flag = to_ms_since_boot(get_absolute_time());
                printf("Green click\n");
            }
            // gpio_put(LED_G, 1);
            // green_bip();
            // gpio_put(LED_G, 0);
        }
        if (red_flag) {
            red_flag = 0;
            if (to_ms_since_boot(get_absolute_time()) - last_red_flag >
                DEBOUNCE_TIME) {
                last_red_flag = to_ms_since_boot(get_absolute_time());
                printf("Red click\n");
            }
            // gpio_put(LED_R, 1);
            // red_bip();
            // gpio_put(LED_R, 0);
        }
        if (yellow_flag) {
            yellow_flag = 0;
            if (to_ms_since_boot(get_absolute_time()) - last_yellow_flag >
                DEBOUNCE_TIME) {
                last_yellow_flag = to_ms_since_boot(get_absolute_time());
                printf("Yellow click\n");
            }
            // gpio_put(LED_Y, 1);
            // yellow_bip();
            // gpio_put(LED_Y, 0);
        }
    }
}
