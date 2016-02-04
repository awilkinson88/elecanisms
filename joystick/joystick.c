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
#include "usb.h"

#define HELLO       0   // Vendor request that prints "Hello World!"
#define SET_VALS    1   // Vendor request that receives 2 unsigned integer values
#define GET_VALS    2   // Vendor request that returns 2 unsigned integer values
#define PRINT_VALS  3   // Vendor request that prints 2 unsigned integer values 

uint16_t val1, val2;

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

void VendorRequests(void) {
    WORD temp;

    switch (USB_setup.bRequest) {
        case HELLO:
            printf("Hello World!\n");
            BD[EP0IN].bytecount = 0;    // set EP0 IN byte count to 0 
            BD[EP0IN].status = 0xC8;    // send packet as DATA1, set UOWN bit
            break;
        case SET_VALS:
            val1 = USB_setup.wValue.w;
            val2 = USB_setup.wIndex.w;
            BD[EP0IN].bytecount = 0;    // set EP0 IN byte count to 0 
            BD[EP0IN].status = 0xC8;    // send packet as DATA1, set UOWN bit
            break;
        case GET_VALS:
            temp.w = val1;
            BD[EP0IN].address[0] = temp.b[0];
            BD[EP0IN].address[1] = temp.b[1];
            temp.w = val2;
            BD[EP0IN].address[2] = temp.b[0];
            BD[EP0IN].address[3] = temp.b[1];
            BD[EP0IN].bytecount = 4;    // set EP0 IN byte count to 4
            BD[EP0IN].status = 0xC8;    // send packet as DATA1, set UOWN bit
            break;            
        case PRINT_VALS:
            printf("val1 = %u, val2 = %u\n", val1, val2);
            BD[EP0IN].bytecount = 0;    // set EP0 IN byte count to 0
            BD[EP0IN].status = 0xC8;    // send packet as DATA1, set UOWN bit
            break;
        default:
            USB_error_flags |= 0x01;    // set Request Error Flag
    }
}

void VendorRequestsIn(void) {
    switch (USB_request.setup.bRequest) {
        default:
            USB_error_flags |= 0x01;                    // set Request Error Flag
    }
}

void VendorRequestsOut(void) {
    switch (USB_request.setup.bRequest) {
        default:
            USB_error_flags |= 0x01;                    // set Request Error Flag
    }
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

    // timer_setPeriod(&timer2, 0.5);
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
    float temp;
    val2 = 0; 

    InitUSB();                              // initialize the USB registers and serial interface engine
    while (USB_USWSTAT!=CONFIG_STATE) {     // while the peripheral is not configured...
        ServiceUSB();                       // ...service USB requests
    }

    while (1) {
        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            res = enc_readReg(address);
            temp = res.i;
            val1 = 360.0*(temp)/pow(2,14);

        }
        ServiceUSB();  
    }


    ///End SPI not working ----------------------------

}

