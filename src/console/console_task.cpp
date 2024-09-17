#include "console_task.h"
#include "Console.h"
#include "colors.h"
#include <FreeRTOS.h>
#include <task.h>
#include "tusb.h"
#include "usb_itf.h"
#include "usb_task.h"


Console *console;

void vTaskConsole(__unused void *pvParams) {
    console = new Console();

    vTaskDelay(pdMS_TO_TICKS(500));
    while (!usb_is_connected()) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    vTaskDelay(pdMS_TO_TICKS(100));

    printf("%s.\n\n", COLOR_WHITE("Pico console ready"));

    console->reset();
    console->start();

    char rx[6];

    for (;;) {
        size_t count = tud_cdc_n_read(ITF_CONSOLE, rx, sizeof(rx));
        console->update(rx, count);

        vTaskDelay(1);
    }
}
