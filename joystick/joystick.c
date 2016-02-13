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
#include "md.h"


#define HELLO       0   // Vendor request that prints "Hello World!"
#define SET_VALS    1   // Vendor request that receives 2 unsigned integer values
#define GET_VALS    2   // Vendor request that returns 2 unsigned integer values
#define PRINT_VALS  3   // Vendor request that prints 2 unsigned integer values 

uint8_t direction = 1;
uint16_t val1, val2, prevVal1;

_PIN *ENC_SCK, *ENC_MISO, *ENC_MOSI;
_PIN *ENC_NCS;

typedef struct {
    uint16_t angle;
    uint16_t fraction;
} Float16;

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

// Gear ratio = 1/4.7
Float16 *gearRatio; 
gearRatio->angle = 0b000000000; // 9 MSB = integer portion
gearRatio->fraction = 0b0011011; // 7 LSB = fraction portion
// 4=0b0000000010000000
// 0.7=0b0000001001011010
// 1/4.7=0b0000000000011011

int16_t convAngle(int16_t currVal, int16_t prevVal){

    // // Concatenate integer and fraction portions of counter
    // uint16_t shaftInt = (shaftAng->angle << 7) || shaftAng->fraction;

    // Concatenate integer and fraction portions of gearRatio
    // uint16_t gearInt = (gearRatio->angle << 7) || gearRatio->fraction;
    uint16_t gearInt = gearRatio->angle;

    // Generate shifted versions of currVal and prevVal
    uint16_t shiftCurr = currVal << 7;
    uint16_t shiftPrev = prevVal << 7;

    int16_t counter;//change this to int!
    //to use "floats" bit shift by one place and divide by 2
    ///Might need to handle cases where it jumps more than a single deg
    
    // Transition from 0 to 359 => increment shaft by negative one
    if (prevVal=0 && currVal=359){
        counter = -gearInt; // -1/4.7
    }
    // Transition from 359 to 0 => increment shaft by positive one
    else if (prevVal=359 && currVal=0){
        counter = gearInt; // +1/4.7
        }
    // Otherwise => add difference b/n curr and prev value
    else{
        counter = (shiftCurr - shiftPrev)*gearInt; //scale factor
        }
    }
    return counter;    
};


int16_t main(void) {
    init_clock();
    init_ui();
    init_spi();
    init_oc();
    init_pin();
    init_uart();
    init_timer();
    init_md();

    uint16_t ShaftAng = 0;

    led_on(&led1);//LED to tell if it is in run mode

    timer_setPeriod(&timer2, .25);
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

    md_speed(&mdp, 0xA000);

    while (1) {
        if (timer_flag(&timer2)) {
            timer_lower(&timer2);
            md_brake(&mdp);
            direction = !direction;
            md_speed(&mdp, 0xA000);
            md_direction(&mdp, direction);
        }
        res = enc_readReg(address);
        temp = res.i;
        val1 = 360.0*(temp)/pow(2,14);
        shaftAng = shaftAng+convAngle(val1, prevVal1);        
        val2 = shaftAng;
        ServiceUSB(); 
        prevVal1 = val1; 
    }
}
