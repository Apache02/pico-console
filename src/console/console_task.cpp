#include "console_task.h"
#include "Console.h"
#include "colors.h"
#include <FreeRTOS.h>
#include <task.h>
#include "tusb.h"
#include "usb_itf.h"
#include "usb_task.h"


Console *console;

static void wait_usb() {
    vTaskDelay(pdMS_TO_TICKS(500));
    while (!usb_is_connected()) {
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    vTaskDelay(pdMS_TO_TICKS(100));
}

static void print_welcome() {
    printf("\n%s.\n\n", COLOR_WHITE("Pico console is ready"));
}

void vTaskConsole(__unused void *pvParams) {
    console = new Console();


    for (;;) {
        wait_usb();
        print_welcome();

        console->reset();
        console->start();

        while (usb_is_connected()) {
            char rx[6];
            size_t count = tud_cdc_n_read(ITF_CONSOLE, rx, sizeof(rx));
            console->update(rx, count);

            vTaskDelay(1);
        }
    }
}
