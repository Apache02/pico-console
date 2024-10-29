#include "commands/commands.h"
#include <stdio.h>


static void help(Console &console);

extern const Console::Handler handlers[] = {
        {"help",       help},
        {"board_help", command_board_help},
        {"echo",       command_echo},
        {"sensors",    command_sensors},
        {"chip_id",    command_chip_id},
        {"clocks",     command_clocks},
        {"bench",      command_benchmark},
        {"i2c_scan",   command_i2c_scan},
        // required at the end
        {nullptr,      nullptr},
};


void help(Console &console) {
    printf("Commands:\r\n");
    for (int i = 0;; i++) {
        if (!handlers[i].name || !handlers[i].handler) {
            break;
        }

        printf("  %s\r\n", handlers[i].name);
    }
}
