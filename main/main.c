/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define LED_B 28 
#define LED_G 27
#define LED_R 22 
#define LED_Y 20 

#define BTN_B 7
#define BTN_G 9
#define BTN_R 10
#define BTN_Y 13

#define BUZZER 5

volatile int blue_press = 0;
volatile int green_press = 0;
volatile int red_press = 0;
volatile int yellow_press = 0;
void gpio_callback(uint gpio, uint32_t events) {
    if (events == 0x4) {         // fall edge
        if (gpio == BTN_B){
            blue_press = 1;
        }
        if (gpio == BTN_G){
            green_press = 1;
        } 
        if (gpio == BTN_R){
            red_press = 1;
        }
        if (gpio == BTN_Y){
            yellow_press = 1;
        }
    } else if (events == 0x8) {  // rise edge

    }
}

void blue_bip(){
    uint32_t comparison_time1;
    comparison_time1 = to_ms_since_boot(get_absolute_time());
    for(int x = 0; x<300; x++){
            gpio_put(BUZZER, 1);
            sleep_us(1000);
            gpio_put(BUZZER, 0);
            sleep_us(1000);

        }
    while (comparison_time1+1000>to_ms_since_boot(get_absolute_time()))
        {
            
        }
    
}
void green_bip(){
    uint32_t comparison_time2;
    comparison_time2 = to_ms_since_boot(get_absolute_time());
    for(int x = 0; x<300; x++){
            gpio_put(BUZZER, 1);
            sleep_us(666);
            gpio_put(BUZZER, 0);
            sleep_us(666);

        }
    while (comparison_time2+1000>to_ms_since_boot(get_absolute_time()))
        {
            
        }

}
void red_bip(){
    uint32_t comparison_time3;
    comparison_time3 = to_ms_since_boot(get_absolute_time());
    for(int x = 0; x<300; x++){
            gpio_put(BUZZER, 1);
            sleep_us(444);
            gpio_put(BUZZER, 0);
            sleep_us(444);

        }
    while (comparison_time3+1000>to_ms_since_boot(get_absolute_time()))
        {
            
        }
}
void yellow_bip(){
    uint32_t comparison_time4;
    comparison_time4 = to_ms_since_boot(get_absolute_time());
    for(int x = 0; x<300; x++){
            gpio_put(BUZZER, 1);
            sleep_us(290);
            gpio_put(BUZZER, 0);
            sleep_us(290);

        }
    while (comparison_time4+1000>to_ms_since_boot(get_absolute_time()))
        {
            
        }
}

int main() {
    stdio_init_all();

    // Initialize all inputs and outputs
    gpio_init(LED_B);
    gpio_init(LED_G);
    gpio_init(LED_R);
    gpio_init(LED_Y);
    gpio_init(BTN_B);
    gpio_init(BTN_G);
    gpio_init(BTN_R);
    gpio_init(BTN_Y);
    gpio_init(BUZZER);

    // Set as output
    gpio_set_dir(LED_B, GPIO_OUT);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_set_dir(LED_Y, GPIO_OUT);
    gpio_set_dir(BUZZER, GPIO_OUT);
    // Set as input
    gpio_set_dir(BTN_B, GPIO_IN);
    gpio_pull_up(BTN_B);
    gpio_set_dir(BTN_G, GPIO_IN);
    gpio_pull_up(BTN_G);
    gpio_set_dir(BTN_R, GPIO_IN);
    gpio_pull_up(BTN_R);
    gpio_set_dir(BTN_Y, GPIO_IN);
    gpio_pull_up(BTN_Y);

    gpio_set_irq_enabled_with_callback(BTN_B, 
                                     GPIO_IRQ_EDGE_RISE | 
                                     GPIO_IRQ_EDGE_FALL, 
                                     true,
                                     &gpio_callback);
    gpio_set_irq_enabled(BTN_G,
                        GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_FALL,
                        true);
    gpio_set_irq_enabled(BTN_R,
                        GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_FALL ,
                        true);
    gpio_set_irq_enabled(BTN_Y,
                        GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_FALL,
                        true);
    
    
    while (true) {
        if (blue_press){
            blue_press = 0;
            gpio_put(LED_B, 1);
            blue_bip();
            gpio_put(LED_B, 0);
        }
        if (green_press){
            green_press = 0;
            gpio_put(LED_G, 1);
            green_bip();
            gpio_put(LED_G, 0);
        }
        if (red_press){
            red_press = 0;
            gpio_put(LED_R, 1);
            red_bip();
            gpio_put(LED_R, 0);
        }
        if (yellow_press){
            yellow_press = 0;
            printf("double click?\n");
            gpio_put(LED_Y, 1);
            yellow_bip();
            gpio_put(LED_Y, 0);
        }
    }
}
