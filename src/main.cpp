#include <stdio.h>
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include <FreeRTOS.h>
#include <task.h>

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

void vTaskBlink(void *pvParams) {
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
}

void init_hw() {
    set_sys_clock_khz(configCPU_CLOCK_HZ / 1000, false);
    stdio_init_all();
}

int main() {
    init_hw();

    sleep_ms(2000);
    printf("ready\n");

    xTaskCreate(
            vTaskBlink,
            "blink",
            configMINIMAL_STACK_SIZE,
            NULL,
            1,
            NULL
    );

    vTaskStartScheduler();

    return 0;
}
