#include <p24FJ128GB206.h>
#include "config.h"
#include "common.h"
#include "ui.h"
#include "oc.h"
#include "pin.h"
#include "md.h"
#include "timer.h"

uint8_t direction = 1;
int16_t main(void) {
    init_clock();
    init_ui();
    init_timer();
    init_pin();
    init_oc();
    init_md();

    led_on(&led2);
    led_on(&led3);

    timer_setPeriod(&timer1, 1);
    timer_start(&timer1);

    while (1) {
        if (timer_flag(&timer1)) {
            timer_lower(&timer1);
            direction = !direction;

            md_speed(&mdp, 0x2000);
            md_direction(&mdp, direction);
        }
    }
}