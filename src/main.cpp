#include <stdio.h>
#include "pico/stdlib.h"

#define INTERVAL 150

void dot() {
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    sleep_ms(INTERVAL);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
    sleep_ms(INTERVAL);
}

void dash() {
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    sleep_ms(INTERVAL * 3);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
    sleep_ms(INTERVAL);
}

int main() {
    stdio_init_all();
    sleep_ms(2000);

    printf("ready\n");

    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    for (;;) {
        dot();
        dot();
        dot();
        dash();
        dash();
        dash();
        dot();
        dot();
        dot();

        sleep_ms(INTERVAL * 2);
    }

    return 0;
}
