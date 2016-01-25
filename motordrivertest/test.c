#include <p24FJ128GB206.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "oc.h"
#include "pin.h"
#include "timer.h"

int16_t main(void) {
    init_clock();
    init_ui();
    init_timer();
    init_pin();
    init_oc();

    led_on(&led3);

    timer_setPeriod(&timer1, 0.5);
    timer_start(&timer1);

    pin_digitalOut(&D[7]);
    pin_digitalOut(&D[8]);

    pin_set(&D[7]);
    pin_clear(&D[8]);

    while (1) {
        if (timer_flag(&timer1)) {
            timer_lower(&timer1);

            pin_toggle(&D[7]);
            pin_toggle(&D[8]);
        }
    }
}