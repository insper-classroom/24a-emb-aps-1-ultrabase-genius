/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "hardware/gpio.h"
#include "pico/stdlib.h"

#include "aux.h"


#define DEBOUNCE_TIME 300

#define LED_B 12
#define LED_G 15
#define LED_R 10
#define LED_Y 14

#define BTN_B 3
#define BTN_G 5
#define BTN_R 2
#define BTN_Y 4

#define B_FREQ 261
#define G_FREQ 293
#define R_FREQ 329
#define Y_FREQ 392

#define BUZZER 6

#define D4 16
#define A 17
#define B 18
#define C 19
#define D 20
#define E 21
#define F 22
#define G 28
#define D3 27
#define D2 13
#define D1 11


//const int INPUT_TIMEOUT = 10000;

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

// void led_beep(int led_pin, int frequency, int duration) {
//     gpio_put(led_pin, 1);
//     emit_sound(frequency, duration);
//     gpio_put(led_pin, 0);
// }

// void blue_beep() {
//     uint32_t comparison_time1;
//     comparison_time1 = to_ms_since_boot(get_absolute_time());
//     for (int x = 0; x < 300; x++) {
//         gpio_put(BUZZER, 1);
//         sleep_us(1000);
//         gpio_put(BUZZER, 0);
//         sleep_us(1000);
//     }
//     while (comparison_time1 + 1000 > to_ms_since_boot(get_absolute_time())) {
//     }
// }
// void green_beep() {
//     uint32_t comparison_time2;
//     comparison_time2 = to_ms_since_boot(get_absolute_time());
//     for (int x = 0; x < 300; x++) {
//         gpio_put(BUZZER, 1);
//         sleep_us(666);
//         gpio_put(BUZZER, 0);
//         sleep_us(666);
//     }
//     while (comparison_time2 + 1000 > to_ms_since_boot(get_absolute_time())) {
//     }
// }
// void red_beep() {
//     uint32_t comparison_time3;
//     comparison_time3 = to_ms_since_boot(get_absolute_time());
//     for (int x = 0; x < 300; x++) {
//         gpio_put(BUZZER, 1);
//         sleep_us(444);
//         gpio_put(BUZZER, 0);
//         sleep_us(444);
//     }
//     while (comparison_time3 + 1000 > to_ms_since_boot(get_absolute_time())) {
//     }
// }
// void yellow_beep() {
//     uint32_t comparison_time4;
//     comparison_time4 = to_ms_since_boot(get_absolute_time());
//     for (int x = 0; x < 300; x++) {
//         gpio_put(BUZZER, 1);
//         sleep_us(290);
//         gpio_put(BUZZER, 0);
//         sleep_us(290);
//     }
//     while (comparison_time4 + 1000 > to_ms_since_boot(get_absolute_time())) {
//     }
// }

int get_random_button_code() {
    srand(to_ms_since_boot(get_absolute_time()));
    return rand() % 4;
}

// int button_code_to_frequency(int button_code) {
//     if (button_code == 0) return B_FREQ;
//     if (button_code == 1) return G_FREQ;
//     if (button_code == 2) return R_FREQ;
//     if (button_code == 3) return Y_FREQ;
//     return -1;
// }

int button_code_to_led_pin(int button_code) {
    if (button_code == 0) return LED_B;
    if (button_code == 1) return LED_G;
    if (button_code == 2) return LED_R;
    if (button_code == 3) return LED_Y;
    return -1;
}

void display_handler(int n, int state){
    
    int digit;
    if (state == 0) {
        digit = n % 10; // last digit
    } else {
        int divisor = (int)pow(10, state);
        digit = (n / divisor) % 10;
    }


    if (digit == 0){
            gpio_put(A, 0);
            gpio_put(B, 0);
            gpio_put(C, 0);
            gpio_put(D, 0);
            gpio_put(E, 0);
            gpio_put(F, 0);
            gpio_put(G, 1);
    }            
    if (digit == 1){
            gpio_put(A, 1);
            gpio_put(B, 0);
            gpio_put(C, 0);
            gpio_put(D, 1);
            gpio_put(E, 1);
            gpio_put(F, 1);
            gpio_put(G, 1);
    }
    if (digit == 2){
            gpio_put(A, 0);
            gpio_put(B, 0);
            gpio_put(C, 1);
            gpio_put(D, 0);
            gpio_put(E, 0);
            gpio_put(F, 1);
            gpio_put(G, 0);
    }
    if (digit == 3){
        gpio_put(A, 0);
        gpio_put(B, 0);
        gpio_put(C, 0);
        gpio_put(D, 0);
        gpio_put(E, 1);
        gpio_put(F, 1);
        gpio_put(G, 0);
    }
    if (digit == 4){
            gpio_put(A, 1);
            gpio_put(B, 0);
            gpio_put(C, 0);
            gpio_put(D, 1);
            gpio_put(E, 1);
            gpio_put(F, 0);
            gpio_put(G, 0);
    }
    if (digit == 5){
            gpio_put(A, 0);
            gpio_put(B, 1);
            gpio_put(C, 0);
            gpio_put(D, 0);
            gpio_put(E, 1);
            gpio_put(F, 0);
            gpio_put(G, 0);
    }
    if (digit == 6){
            gpio_put(A, 0);
            gpio_put(B, 1);
            gpio_put(C, 0);
            gpio_put(D, 0);
            gpio_put(E, 0);
            gpio_put(F, 0);
            gpio_put(G, 0);
    }
    if (digit == 7){
            gpio_put(A, 0);
            gpio_put(B, 0);
            gpio_put(C, 0);
            gpio_put(D, 1);
            gpio_put(E, 1);
            gpio_put(F, 1);
            gpio_put(G, 1);
    }
    if (digit == 8){
            gpio_put(A, 0);
            gpio_put(B, 0);
            gpio_put(C, 0);
            gpio_put(D, 0);
            gpio_put(E, 0);
            gpio_put(F, 0);
            gpio_put(G, 0);
    }
    if (digit == 9){
            gpio_put(A, 0);
            gpio_put(B, 0);
            gpio_put(C, 0);
            gpio_put(D, 0);
            gpio_put(E, 1);
            gpio_put(F, 0);
            gpio_put(G, 0);
    }

    if (state == 0){
        gpio_put(D4, 1);
        gpio_put(D3, 0);
        gpio_put(D2, 0);
        gpio_put(D1, 0);
    } 
    if (state == 1){
        gpio_put(D3, 1);
        gpio_put(D4, 0);
        gpio_put(D2, 0);
        gpio_put(D1, 0);

    }
    if (state == 2){
        gpio_put(D2, 1);
        gpio_put(D4, 0);
        gpio_put(D3, 0);
        gpio_put(D1, 0);
    } 
    if (state == 3){
        gpio_put(D1, 1);
        gpio_put(D2, 0);
        gpio_put(D3, 0);
        gpio_put(D4, 0);

    }
}

void intro() {
    int tempo = 144;
    int notes = sizeof(melody) / sizeof(melody[0]) / 2;
    int wholenote = (60000 * 4) / tempo;
    int noteDuration;

    for (int thisNote = 0; thisNote < notes * 2; thisNote += 2) {
        int divider = melody[thisNote + 1];
        if (divider > 0) {
            noteDuration = wholenote / divider;
        } else if (divider < 0) {
            noteDuration = wholenote / abs(divider);
            noteDuration *= 1.5; 
        }

        int playDuration = noteDuration * 0.9;
        emit_sound(melody[thisNote], (int)playDuration);
        sleep_ms(noteDuration - (int)playDuration);
    }
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

    gpio_init(D4);
    gpio_init(D3);
    gpio_init(D2);
    gpio_init(D1);
    gpio_init(A);
    gpio_init(B);
    gpio_init(C);
    gpio_init(D);
    gpio_init(E);
    gpio_init(F);
    gpio_init(G);
    gpio_set_dir(D4, GPIO_OUT);
    gpio_set_dir(D3, GPIO_OUT);
    gpio_set_dir(D2, GPIO_OUT);
    gpio_set_dir(D1, GPIO_OUT);
    gpio_set_dir(A, GPIO_OUT);
    gpio_set_dir(B, GPIO_OUT);
    gpio_set_dir(C, GPIO_OUT);
    gpio_set_dir(D, GPIO_OUT);
    gpio_set_dir(E, GPIO_OUT);
    gpio_set_dir(F, GPIO_OUT);
    gpio_set_dir(G, GPIO_OUT);

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
    gpio_set_irq_enabled(BTN_G, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(BTN_R, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(BTN_Y, GPIO_IRQ_EDGE_FALL, true);

    int last_blue_flag = 0;
    int last_green_flag = 0;
    int last_red_flag = 0;
    int last_yellow_flag = 0;

    bool blue_click = false;
    bool green_click = false;
    bool red_click = false;
    bool yellow_click = false;

    int display_state = 0;

    /* Game state (3) variables: input control */
    int current_input_id = 0;
    uint32_t last_input_sent_at = 0;

    const int INPUT_TIMEOUT = 10000;
    while (true) {
        printf("score");
        
        if (game_state == 3){
            display_handler(score,display_state);
        } else{
            gpio_put(D1, 0);
            gpio_put(D2, 0);
            gpio_put(D3, 0);
            gpio_put(D4, 0);
        }
        display_state = (display_state<3) ? (display_state+1): 0;
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
            intro();

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
                gpio_put(D1, 0);
                gpio_put(D2, 0);
                gpio_put(D3, 0);
                gpio_put(D4, 0);
                display_state = (display_state<3) ? (display_state+1): 0;
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
                gpio_put(D1, 0);
                gpio_put(D2, 0);
                gpio_put(D3, 0);
                gpio_put(D4, 0);
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
