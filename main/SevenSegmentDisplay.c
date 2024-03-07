/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <math.h>


#define D4 16
#define A 17
#define B 18
#define C 19
#define D 20
#define E 21
#define F 22
#define G 28
#define D3 27
#define D2 15
#define D1 14

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

int main(){
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

    int display_state = 0;
    while(true){
        printf("%d ---\n", display_state);
        display_handler(38,display_state);

        display_state = (display_state<3) ? (display_state+1): 0;
        
        

    }
}