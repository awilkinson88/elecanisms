/*
** Copyright (c) 2016, Evan Dorsky
** All rights reserved.
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met: 
** 
**     1. Redistributions of source code must retain the above copyright 
**        notice, this list of conditions and the following disclaimer. 
**     2. Redistributions in binary form must reproduce the above copyright 
**        notice, this list of conditions and the following disclaimer in the 
**        documentation and/or other materials provided with the distribution. 
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
** POSSIBILITY OF SUCH DAMAGE.
*/
#include <p24FJ128GB206.h>
#include "common.h"
#include "md.h"

_MD md;

void init_md(void) {
    md_init();
}

// dir == 0 means
// pin[0] is active, pin[1] is 0
// dir == 1 means
// pin[0] is 0, pin[1] is active
void md_init() {
    md.m1dir = 0;
    md.m1speed = 0;
    md.m1freq = 0;
    md.m1pins[0] = 7;
    md.m1pins[1] = 8;

    md.m2dir = 0;
    md.m2speed = 0;
    md.m2freq = 0;
    md.m2pins[0] = 5;
    md.m2pins[1] = 6;

    oc_pwm(&oc7, &D[md.m1pins[0]], &timer4, 1e3, 0);
    oc_pwm(&oc8, &D[md.m1pins[1]], &timer4, 1e3, 0);
    
    oc_pwm(&oc5, &D[md.m2pins[0]], &timer5, 1e3, 0);
    oc_pwm(&oc6, &D[md.m2pins[1]], &timer5, 1e3, 0);
}

void md_free() {
    // clear owned pins/oc's
}

// changes speed, direction unchanged
void md_speed(uint16_t motor, uint16_t speed) {
    if (motor == 1) {
        md.m1speed = speed;

        pin_write(&D[md.m1pins[md.m1dir]], speed);
    }
}

// changes direction, speed unchanged
void md_direction(uint16_t motor, uint16_t dir) {
    if (motor == 1) {
        if (md.m1dir == dir) {
            return;
        }
        md.m1dir = dir;

        uint16_t tmp = pin_read(&D[md.m1pins[dir]]);
        pin_write(&D[md.m1pins[dir]],
            pin_read(&D[md.m1pins[!dir]]));
        pin_write(&D[md.m1pins[!dir]], tmp);
    }
}

// changes speed and direction
void md_velocity(uint16_t motor, uint16_t speed, uint16_t dir) {
}