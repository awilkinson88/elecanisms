#include <p24FJ128GB206.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "timer.h"
#include "spi.h"
#include "pin.h"

//SPI pins
// #define MOSI 0
// #define MISO 1
// #define SCLK 2
// #define CS 3


int16_t main(void) {
    init_clock();
    init_ui();
    init_spi();
    float freq = 9000;
    // spi_open(&spi1, &MISO, &MOSI, &SCLK, freq);
    spi_open(&spi1, &D[0], &D[1], &D[2], freq);
    //Transfer command to zero the system.
    spi_transfer(&spi1, )
    //Transfer command to read angle
    spi_transfer(&spi1, 0x3FFF)

    led_on(&led1);
    timer_setPeriod(&timer2, 0.5);
    timer_start(&timer2);

    while (1) {
        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            led_toggle(&led1);
            led_toggle(&led2);
        }
        led_write(&led3, !sw_read(&sw3));
    }
}

