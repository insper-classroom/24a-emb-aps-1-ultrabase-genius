/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>

#include "hardware/gpio.h"
#include "pico/stdlib.h"

#define DEBOUNCE_TIME 300

#define LED_B 28
#define LED_G 27
#define LED_R 22
#define LED_Y 20

#define BTN_B 7
#define BTN_G 9
#define BTN_R 10
#define BTN_Y 13

#define B_FREQ 261
#define G_FREQ 293
#define R_FREQ 329
#define Y_FREQ 392

#define BUZZER 5

const int INPUT_TIMEOUT = 10000;

volatile int listening_to_clicks = 1;
volatile int blue_flag = 0;
volatile int green_flag = 0;
volatile int red_flag = 0;
volatile int yellow_flag = 0;
volatile uint32_t blue_flag_triggered_at = 0;
volatile uint32_t green_flag_triggered_at = 0;
volatile uint32_t red_flag_triggered_at = 0;
volatile uint32_t yellow_flag_triggered_at = 0;

void gpio_callback(uint gpio, uint32_t events) {
    if (events == 0x4) {  // fall edge
        if (listening_to_clicks) {
            if (gpio == BTN_B) {
                blue_flag = 1;
                blue_flag_triggered_at = to_ms_since_boot(get_absolute_time());
            }
            if (gpio == BTN_G) {
                green_flag = 1;
                green_flag_triggered_at = to_ms_since_boot(get_absolute_time());
            }
            if (gpio == BTN_R) {
                red_flag = 1;
                red_flag_triggered_at = to_ms_since_boot(get_absolute_time());
            }
            if (gpio == BTN_Y) {
                yellow_flag = 1;
                yellow_flag_triggered_at =
                    to_ms_since_boot(get_absolute_time());
            }
        }
    }
}

int get_output_speed_based_on_player_round(int round) {
    if (round > 10) {
        return 150;
    }
    return 500 - 35 * round;
}

void emit_sound(int frequency, int duration) {
    /*
     * Frequency [Hz]
     * Durantion [ms]
     */
    double period_in_ms = 1000.0 / frequency;
    double half_period_in_ms = (period_in_ms / 2.0);
    int half_period_in_us = (int)(half_period_in_ms * 1000);
    int total_cycles = frequency * duration / 1000;

    for (int i = 0; i < total_cycles; i++) {
        gpio_put(BUZZER, 1);
        sleep_us(half_period_in_us);
        gpio_put(BUZZER, 0);
        sleep_us(half_period_in_us);
    }
}

void led_beep(int led_pin, int frequency, int duration) {
    gpio_put(led_pin, 1);
    emit_sound(frequency, duration);
    gpio_put(led_pin, 0);
}

void blue_beep() {
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
void green_beep() {
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
void red_beep() {
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
void yellow_beep() {
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

int get_random_button_code() {
    srand(to_ms_since_boot(get_absolute_time()));
    return rand() % 4;
}

int button_code_to_frequency(int button_code) {
    if (button_code == 0) return B_FREQ;
    if (button_code == 1) return G_FREQ;
    if (button_code == 2) return R_FREQ;
    if (button_code == 3) return Y_FREQ;
    return -1;
}

int button_code_to_led_pin(int button_code) {
    if (button_code == 0) return LED_B;
    if (button_code == 1) return LED_G;
    if (button_code == 2) return LED_R;
    if (button_code == 3) return LED_Y;
    return -1;
}

int main() {
    /* Game state management:
     * --> (0) idle
     * --> (1) ongoing: introduction
     * --> (2) ongoing: receiving commands
     */
    int game_state = 0;
    int sequence[100];
    int score = 0;

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

    bool blue_click = false;
    bool green_click = false;
    bool red_click = false;
    bool yellow_click = false;

    /* Game state (3) variables: input control */
    int current_input_id = 0;
    uint32_t last_input_sent_at = 0;

    while (true) {
        printf("qualquer merda...");

        /* Detect if there have been any button clicks */
        if (blue_flag) {
            blue_flag = 0;
            if (blue_flag_triggered_at - last_blue_flag > DEBOUNCE_TIME) {
                last_blue_flag = blue_flag_triggered_at;
                blue_click = true;
            }
        }

        if (green_flag) {
            green_flag = 0;
            if (green_flag_triggered_at - last_green_flag > DEBOUNCE_TIME) {
                last_green_flag = green_flag_triggered_at;
                green_click = true;
            }
        }

        if (red_flag) {
            red_flag = 0;
            if (red_flag_triggered_at - last_red_flag > DEBOUNCE_TIME) {
                last_red_flag = red_flag_triggered_at;
                red_click = true;
            }
        }

        if (yellow_flag) {
            yellow_flag = 0;
            if (yellow_flag_triggered_at - last_yellow_flag > DEBOUNCE_TIME) {
                last_yellow_flag = yellow_flag_triggered_at;
                yellow_click = true;
            }
        }

        if (game_state == 0) {
            if (blue_click || green_click || red_click || yellow_click) {
                // printf("[0] Start game (%d, %d, %d, %d).\n", blue_click,
                // green_click, red_click, yellow_click);

                blue_click = false;
                green_click = false;
                red_click = false;
                yellow_click = false;

                game_state = 1;
                listening_to_clicks = 0;
                continue;
            }
        }

        if (game_state == 1) {
            // printf("[1] Start introduction.\n");

            for (int i = 0; i < 4; i++) {
                gpio_put(LED_B, 1);
                gpio_put(LED_G, 1);
                gpio_put(LED_R, 1);
                gpio_put(LED_Y, 1);
                emit_sound(349, 250);
                gpio_put(LED_B, 0);
                gpio_put(LED_G, 0);
                gpio_put(LED_R, 0);
                gpio_put(LED_Y, 0);
                sleep_ms(250);
            }

            // printf("[1] End introduction.\n");
            sleep_ms(1000);
            game_state = 2;
            continue;
        }

        if (game_state == 2) {
            printf("[2] Output sequence.\n");

            // Generate a random button to add to the end of the sequence
            sequence[score] = get_random_button_code();

            // Execute sequence (all at once)
            for (int i = 0; i < score + 1; i++) {
                printf("[2] Sequence: %d\n", sequence[i]);
                int led_pin = button_code_to_led_pin(sequence[i]);
                // led_beep(button_code_to_led_pin(sequence[i]),
                //          button_code_to_frequency(sequence[i]),
                //          get_output_speed_based_on_player_round(score));
                gpio_put(led_pin, 1);
                if (sequence[i] == 0) {
                    emit_sound(261,
                               get_output_speed_based_on_player_round(score));
                } else if (sequence[i] == 1) {
                    emit_sound(329,
                               get_output_speed_based_on_player_round(score));
                } else if (sequence[i] == 2) {
                    emit_sound(392,
                               get_output_speed_based_on_player_round(score));
                } else {
                    emit_sound(523,
                               get_output_speed_based_on_player_round(score));
                }

                // sleep_ms(get_output_speed_based_on_player_round(score));
                gpio_put(led_pin, 0);
                sleep_ms(get_output_speed_based_on_player_round(score));
            }

            game_state = 3;
            listening_to_clicks = 1;
            last_input_sent_at = to_ms_since_boot(get_absolute_time());
            blue_click = false;
            green_click = false;
            red_click = false;
            yellow_click = false;
            continue;
        }

        if (game_state == 3) {
            if (current_input_id > score) {
                // This is an indicator that we have to go to the output state.
                // printf("You got it! Next sequence up ahead...\n");
                current_input_id = 0;
                score++;
                listening_to_clicks = 0;
                game_state = 2;
                sleep_ms(1000);
                continue;
            }

            if (to_ms_since_boot(get_absolute_time()) - last_input_sent_at >
                INPUT_TIMEOUT) {
                // This is an indicator that the user has timed out and lost the
                // game.
                current_input_id = 0;
                game_state = 5;
                continue;
            }

            int clicked_button_code = -1;

            if (blue_click) {
                clicked_button_code = 0;
                blue_click = false;
            }

            if (green_click) {
                clicked_button_code = 1;
                green_click = false;
            }

            if (red_click) {
                clicked_button_code = 2;
                red_click = false;
            }

            if (yellow_click) {
                clicked_button_code = 3;
                yellow_click = false;
            }

            if (clicked_button_code != -1) {
                last_input_sent_at = to_ms_since_boot(get_absolute_time());
                printf("YOU PRESSED: %d\n", clicked_button_code);
                printf("RIGHT ANSWER: %d\n", sequence[current_input_id]);
                if (sequence[current_input_id] == clicked_button_code) {
                    current_input_id++;
                    gpio_put(button_code_to_led_pin(clicked_button_code), 1);
                    // sleep_ms(get_output_speed_based_on_player_round(score));
                    if (clicked_button_code == 0) {
                        emit_sound(261, get_output_speed_based_on_player_round(score));
                    } else if (clicked_button_code == 1) {
                        emit_sound(329, get_output_speed_based_on_player_round(score));
                    } else if (clicked_button_code == 2) {
                        emit_sound(392, get_output_speed_based_on_player_round(score));
                    } else {
                        emit_sound(523, get_output_speed_based_on_player_round(score));
                    }
                    gpio_put(button_code_to_led_pin(clicked_button_code), 0);
                    continue;
                } else {
                    current_input_id = 0;
                    game_state = 5;
                    continue;
                }
            }

            game_state = 3;
            continue;
        }

        if (game_state == 4) {
            // printf("[4] End game.\n");
            // printf("Final score: %d\n", score);

            game_state = 0;
            score = 0;
            listening_to_clicks = 1;
            blue_click = false;
            green_click = false;
            red_click = false;
            yellow_click = false;
            sleep_ms(2500);
            continue;
        }

        if (game_state == 5) {
            // Player lost... play a song.
            gpio_put(LED_B, 1);
            gpio_put(LED_G, 1);
            gpio_put(LED_R, 1);
            gpio_put(LED_Y, 1);
            emit_sound(349, 1000);
            gpio_put(LED_B, 0);
            gpio_put(LED_G, 0);
            gpio_put(LED_R, 0);
            gpio_put(LED_Y, 0);

            current_input_id = 0;
            game_state = 4;
            continue;
        }
    }
}
