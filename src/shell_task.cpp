#include "shell_task.h"
#include <FreeRTOS.h>
#include <task.h>
#include "tusb.h"
#include "usb_itf.h"
#include "usb_task.h"
#include "shell/Console.h"


extern const Console::Handler handlers[];


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

void vTaskShell(__unused void *pvParams) {
    Console *console = new Console(handlers);

    for (;;) {
        wait_usb();
        print_welcome();

        console->reset();
        console->start();

        while (usb_is_connected()) {
            char rx[8];
            size_t count = tud_cdc_n_read(ITF_CONSOLE, rx, sizeof(rx));

            if (count > 0) {
                for (int i = 0; i < count; i++) {
                    int c = rx[i];
                    if (c == '\x1B') {
                        c = console->resolve_key(&rx[i], count);
                        i = count;
                    }
                    console->update(c);
                }
            }

            vTaskDelay(1);
        }
    }
}
