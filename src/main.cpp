#include <stdio.h>
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include <FreeRTOS.h>
#include <task.h>
#include "tusb.h"

#include "usb/usb_task.h"
#include "usb/usb_itf.h"


extern "C" void usbd_serial_init(void);

#ifdef PICO_DEFAULT_LED_PIN

void vTaskBlink(void *pvParams) {
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);

    for (;;) {
        gpio_put(PICO_DEFAULT_LED_PIN, tud_cdc_n_connected(ITF_CONSOLE));
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

#endif

void vTaskHello(void *pvParams) {
    for (;;) {
        printf("hello printf!\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void init_hardware() {
    set_sys_clock_khz(configCPU_CLOCK_HZ / 1000, false);
    usbd_serial_init();
    tusb_init();
    stdio_usb_init();
}

int main() {
    init_hardware();

#ifdef PICO_DEFAULT_LED_PIN
    xTaskCreate(
            vTaskBlink,
            "led",
            configMINIMAL_STACK_SIZE,
            NULL,
            1,
            NULL
    );
#endif

    xTaskCreate(
            vTaskUsb,
            "usb",
            configMINIMAL_STACK_SIZE * 2,
            NULL,
            1,
            NULL
    );

    xTaskCreate(
            vTaskHello,
            "hello",
            configMINIMAL_STACK_SIZE * 2,
            NULL,
            1,
            NULL
    );

    vTaskStartScheduler();

    return 0;
}
