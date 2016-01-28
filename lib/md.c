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

_MD md1, md2;

void init_md(void) {
    md_init(&md1, 7, 8, &oc7, &oc8);
    md_init(&md2, 5, 6, &oc5, &oc6);
}

// dir == 0 means
// pin[0] is active, pin[1] is 0
// dir == 1 means
// pin[0] is 0, pin[1] is active
void md_init(_MD *self, uint16_t pin1, uint16_t pin2, _OC *oc1, _OC *oc2) {
    self->dir = 0;
    self->speed = 0;
    self->freq = 0;
    self->pins[0] = 7;
    self->pins[1] = 8;

    oc_pwm(oc1, &D[self->pins[0]], NULL, 1e3, 0);
    oc_pwm(oc2, &D[self->pins[1]], NULL, 1e3, 0);
}

void md_free(_MD *self) {
    // clear owned pins/oc's
}

// changes speed, direction unchanged
void md_speed(_MD *self, uint16_t speed) {
    self->speed = speed;

    pin_write(&D[self->pins[self->dir]], speed);
}

// changes direction, speed unchanged
void md_direction(_MD *self, uint16_t dir) {
    if (self->dir == dir) {
        return;
    }
    self->dir = dir;

    uint16_t tmp = pin_read(&D[self->pins[dir]]);
    pin_write(&D[self->pins[dir]], pin_read(&D[self->pins[!dir]]));
    pin_write(&D[self->pins[!dir]], tmp);
}

// changes speed and direction
void md_velocity(_MD *self, uint16_t speed, uint16_t dir) {
    md_speed(self, speed);
    md_direction(self, dir);
}