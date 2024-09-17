#include "Console.h"
#include "colors.h"

#include <stdio.h>
#include <functional>
#include "pico/stdlib.h"

#include "commands/commands.h"


//------------------------------------------------------------------------------

struct ConsoleHandler {
    const char *name;
    std::function<void(Console &)> handler;
};

static const ConsoleHandler handlers[] = {
        {
                "help",
                [](Console &c) {
                    c.print_help();
                },
        },
        {
                "board_help",
                command_board_help,
        },
        {
                "echo",
                command_echo,
        },
        {
                "sensors",
                command_sensors,
        },
        {
                "chip_id",
                command_chip_id,
        },
        {
                "clocks",
                command_clocks,
        },
        {
                "bench",
                command_benchmark,
        },
        {
                "i2c_scan",
                command_i2c_scan,
        },
};

static const int handler_count = sizeof(handlers) / sizeof(handlers[0]);

//------------------------------------------------------------------------------

Console::Console() {}

Console::~Console() {}

void Console::reset() {}

void Console::dump() {}

void Console::start() {
    printf("%s %s ", COLOR_GREEN("pico"), ">");
}

void Console::print_help() {
    printf("Commands:\n");
    for (int i = 0; i < handler_count; i++) {
        printf("  %s\n", handlers[i].name);
    }
}

bool Console::dispatch_command() {
    for (int i = 0; i < handler_count; i++) {
        auto &h = handlers[i];
        if (packet.match_word(h.name)) {
            h.handler(*this);
            return true;
        }
    }
    printf(COLOR_RED("Command %s not handled\n"), packet.buf);
    return false;
}

//------------------------------------------------------------------------------

void Console::update(char *input, unsigned int count) {
    if (!count) return;

    if (input[0] == '\t') {
        input[0] = ' ';
    }

    if (input[0] == 0x7F /*backspace*/) {
        if (packet.cursor2 > packet.buf) {
            printf("\b \b");
            packet.cursor2--;
            packet.size--;
            *packet.cursor2 = 0;
        }
    } else if (input[0] == '\n' || input[0] == '\r') {
        packet.buf[packet.size] = 0;
        packet.cursor2 = packet.buf;

        putchar('\n');

        if (packet.buf[0] != '\0') {
            absolute_time_t time_before = get_absolute_time();
            bool is_handled = dispatch_command();
            absolute_time_t time_after = get_absolute_time();

            if (is_handled) {
                printf("Command took %lld us\n", absolute_time_diff_us(time_before, time_after));
                if ((packet.cursor2 - packet.buf) < packet.size) {
                    printf(COLOR_RED("Leftover text in packet - {%s}\n"), packet.cursor2);
                }
            }
        }

        packet.clear();
        this->start();
    } else if (count > 1) {
        bool process_putchar = false;
        if (count == 3 && input[0] == '\x1B' && input[1] == '\x5B') {
            switch (input[2]) {
                case '\x41':        // arrow up
                case '\x42':        // arrow down
                    break;
                case '\x43':        // arrow right
                    if (*packet.cursor2 != '\0') {
                        packet.cursor2++;
                        process_putchar = true;
                    }
                    break;
                case '\x44':        // arrow left
                    if (packet.cursor2 > packet.buf) {
                        packet.cursor2--;
                        process_putchar = true;
                    }
                    break;
                default:
                    // unknown key
                    break;
            }
        }

        if (process_putchar) {
            for (auto i = 0; i < count; i++) {
                putchar(input[i]);
            }
        }
    } else {
        putchar(input[0]);
        packet.put(input[0]);
    }
}

//------------------------------------------------------------------------------
