#include <p24FJ128GB206.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
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

_PIN *ENC_SCK, *ENC_MISO, *ENC_MOSI;
_PIN *ENC_NCS;

WORD enc_readReg(WORD address) {
    WORD cmd, result;
    cmd.w = 0x4000|address.w; //set 2nd MSB to 1 for a read
    cmd.w |= parity(cmd.w)<<15; //calculate even parity for

    pin_clear(ENC_NCS);
    spi_transfer(&spi1, cmd.b[1]);
    spi_transfer(&spi1, cmd.b[0]);
    pin_set(ENC_NCS);

    pin_clear(ENC_NCS);
    result.b[1] = spi_transfer(&spi1, 0) & 0b00111111;
    result.b[0] = spi_transfer(&spi1, 0);
    pin_set(ENC_NCS);
    return result;
}


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

    ENC_MISO = &D[1];
    ENC_MOSI = &D[0];
    ENC_SCK = &D[2];
    ENC_NCS = &D[3];

    pin_digitalOut(ENC_NCS);
    pin_set(ENC_NCS);

    spi_open(&spi1, ENC_MISO, ENC_MOSI, ENC_SCK, 2e6,1);
    WORD address;
    WORD res;
    address.w = 0x3FFF;
    double val;

    while (1) {
        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            res = enc_readReg(address);
            val = 720*(res.i)/pow(2,14);
            // printf("%i\r\n",val);
            uart_putc(&uart1, res.b[0]);
        }
    }

    ///End SPI not working ----------------------------

}

