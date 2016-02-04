#include <p24FJ128GB206.h>
#include <stdio.h>
#include "uart.h"
#include "config.h"
#include "common.h"
#include "ui.h"
#include "timer.h"
#include "spi.h"
#include "pin.h"
#include "oc.h"


//SPI pins
// #define MOSI 0
// #define MISO 1
// #define SCLK 2
// #define CS 3


int16_t main(void) {
    init_clock();
    init_ui();
    init_spi();
    init_oc();
    init_pin();
    init_uart();
    init_timer();

    ////Motor code ---------------------------

    // oc_pwm(&oc1, &D[8], &timer2, 2000, .9);
    
//     pin_digitalOut(&D[6]);
//     pin_digitalOut(&D[7]);
//     pin_digitalOut(&D[8]);
//     pin_digitalOut(&D[5]);

//     while (1){
//     led_on(&led1);//LED to tell if it is in run mod
//     pin_set(&D[6]);
//     pin_set(&D[7]);
//     pin_clear(&D[8]);
//     pin_clear(&D[5]);
// }

    ///End Motor code ------------------------


    ///SPI not working ----------------------

    led_on(&led1);//LED to tell if it is in run mode

    timer_setPeriod(&timer2, 0.5);
    timer_start(&timer2);
    
    
    pin_digitalOut(&D[3]); //set chip select as dig out
    pin_set(&D[3]); //set chip select high
    float freq = 1e6; //SCLK frequency
    spi_open(&spi1, &D[0], &D[1], &D[2], freq); //open SPI
    pin_clear(&D[3]); //set CS low
    spi_transfer(&spi1, 0x3F);//transfer address to read
    spi_transfer(&spi1, 0xFF);

    pin_set(&D[3]);


    while (1) {
        if (timer_flag(&timer2)) {
            pin_set(&D[3]);
            pin_clear(&D[3]);
            timer_lower(&timer2);
            // char string[8] = "hello\n";
            // printf("%31s\n",string);
            WORD temp;
            temp.b[1] = spi_transfer(&spi1, 0x00); //get angle info
            temp.b[0] = spi_transfer(&spi1, 0x00);
            printf("%i\n",temp.i);
            // uart_puts(&uart1, string);
        }
    }

    ///End SPI not working ----------------------------

}

