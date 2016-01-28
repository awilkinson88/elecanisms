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

    pin_digitalOut(&D[3]); //set chip select as dig out
    pin_set(&D[3]); //set chip select high
    float freq = 1e6; //SCLK frequency
    spi_open(&spi1, &D[0], &D[1], &D[2], freq); //open SPI
    pin_clear(&D[3]); //set CS low
    spi_transfer(&spi1, 0x3F);//transfer address to read
    spi_transfer(&spi1, 0xFF);
    pin_set(&D[3]);
    pin_clear(&D[3]);

    WORD temp;
    temp.b[1] = spi_transfer(&spi1, 0x00); //get angle info
    temp.b[0] = spi_transfer(&spi1, 0x00);

    pin_set(&D[3]);
    // printf("%i\n",temp.i);
    char *string = "hello processor";
    printf("%s\n",string);


    ////Begin motor controller stuff



}

