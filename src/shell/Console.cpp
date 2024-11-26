#include "shell/Console.h"
#include "shell/console_colors.h"
#include "pico/time.h"
#include <stdio.h>


#define CONTROL_ARROW_UP        "\x1B[A"
#define CONTROL_ARROW_DOWN      "\x1B[B"
#define CONTROL_ARROW_RIGHT     "\x1B[C"
#define CONTROL_ARROW_LEFT      "\x1B[D"
#define CONTROL_HOME            "\x1B[H"
#define CONTROL_HOME_ALT        "\x1B[1~"
#define CONTROL_END             "\x1B[F"
#define CONTROL_END_ALT         "\x1B[4~"
#define CONTROL_PAGE_UP         "\x1B[5~"
#define CONTROL_PAGE_DOWN       "\x1B[6~"
#define CONTROL_DELETE          "\x1B[3~"


Console::Console(const Handler *handlers) : handlers(handlers) {
    history = new History(8);
    control_pos = 0;
}

Console::~Console() {
    if (history) {
        delete history;
        history = NULL;
    }
}

void Console::reset() {}

__unused void Console::dump(const void *buf, size_t length) {
    const uint8_t *ptr = static_cast<const uint8_t *>(buf);
    size_t i;

    for (i = 0; i < length; i++) {
        printf("%02X ", ptr[i]);

        if ((i + 1) % 16 == 0) {
            printf("\r\n");
        }
    }

    if (i % 16 != 0) {
        printf("\r\n");
    }
}

void Console::start() {
    printf("%s ", ">");
}

void Console::eol() {
    printf("\r\n");
}

bool Console::dispatch_command() {
    for (int i = 0;; i++) {
        if (!handlers[i].name || !handlers[i].handler) break;

        const Handler *h = &handlers[i];
        if (packet.match_word(h->name)) {
            if (history) history->add(packet.buf);
            h->handler(*this);
            return true;
        }
    }
    printf(COLOR_RED("Command %s not handled\r\n"), packet.buf);
    return false;
}

//------------------------------------------------------------------------------

void Console::update(int c) {
    if (is_control_sequence(c)) return;

    if (c == '\t') {
        this->autocomplete();
        return;
    }

    if (c == '\r') c = '\n';

    if (c == '\x7F') {
        // backspace
        if (packet.cursor2 > packet.buf) {
            printf("\b \b");
            packet.cursor2--;
            packet.size--;
            *packet.cursor2 = 0;
        }

        return;
    }

    if (c == '\x03' || c == '\x04') {
        // Ctrl + C | Ctrl + D
        this->eol();

        packet.clear();
        this->start();

        return;
    }

    if (c == '\n') {
        packet.buf[packet.size] = 0;
        packet.cursor2 = packet.buf;

        this->eol();

        if (packet.buf[0] != '\0') {
            absolute_time_t time_before = get_absolute_time();
            bool is_handled = dispatch_command();
            absolute_time_t time_after = get_absolute_time();

            if (is_handled) {
                printf("Command took %lld us\n", absolute_time_diff_us(time_before, time_after));
                if ((packet.cursor2 - packet.buf) < packet.size) {
                    printf(COLOR_RED("Leftover text in packet - {%s}\r\n"), packet.cursor2);
                }
            }
        }

        packet.clear();
        this->start();

        return;
    }

    putchar(c);
    packet.put(c);
}

bool Console::is_control_sequence(int c) {
    if (c == '\x1B') {
        // this is the beginning of control sequence
        control_pos = 0;
        control_buf[control_pos++] = c;

        return true;
    }

    if (control_pos == 1) {
        if (c == '[') {
            control_buf[control_pos++] = c;
        } else {
            // probably incorrect
            // ignore current sequence
            control_pos = 0;
        }

        return true;
    }

    if (control_pos > 1) {
        control_buf[control_pos++] = c;

        if (c >= 'A' && c <= 'Z') {
            // end of control sequence
            control_buf[control_pos] = 0;
            control_pos = 0;
        } else if (c == '~') {
            // end of control sequence [F1-F12]
            control_buf[control_pos] = 0;
            control_pos = 0;
        } else if (control_pos >= count_of(control_buf)) {
            // buffer overflow
            control_pos = 0;
        }

        if (control_pos == 0) {
            handle_control_sequence(control_buf);
        }

        return true;
    }

    return false;
}

void Console::handle_control_sequence(const char *control) {
    if (strcmp(control, CONTROL_ARROW_UP) == 0) {
        this->replace_command(history->prev());
    } else if (strcmp(control, CONTROL_ARROW_DOWN) == 0) {
        this->replace_command(history->next());
    } else if (strcmp(control, CONTROL_ARROW_LEFT) == 0) {
        if (packet.cursor2 > packet.buf) {
            packet.cursor2--;
            puts(control);
        }
    } else if (strcmp(control, CONTROL_ARROW_RIGHT) == 0) {
        if (*packet.cursor2 != '\0') {
            packet.cursor2++;
            puts(control);
        }
    } else if (strcmp(control, CONTROL_PAGE_UP) == 0) {
    } else if (strcmp(control, CONTROL_PAGE_DOWN) == 0) {
    } else if (strcmp(control, CONTROL_HOME) == 0 || strcmp(control, CONTROL_HOME_ALT) == 0) {
    } else if (strcmp(control, CONTROL_END) == 0 || strcmp(control, CONTROL_END_ALT) == 0) {
    } else if (strcmp(control, CONTROL_DELETE) == 0) {
    } else {
        // TODO: unhandled sequence
    }
}

void Console::replace_command(const char *command) {
    size_t length = strlen(packet.buf);
    putchar('\r');
    for (auto i = 0; i < length + 4; i++) {
        putchar(' ');
    }
    putchar('\r');
    this->start();

    if (command) {
        printf("%s", command);
        packet.set_packet(command);
    } else {
        packet.clear();
    }
}

void Console::autocomplete() {
    size_t length = strlen(packet.buf);
    if (length == 0 || packet.buf[length - 1] == ' ') {
        return;
    }

    const char *found = NULL;
    int found_count = 0;
    for (int i = 0;; i++) {
        if (!handlers[i].name || !handlers[i].handler) break;

        if (strncmp(handlers[i].name, packet.buf, length) == 0) {
            if (!found) found = handlers[i].name;
            found_count++;
        }
    }

    if (found_count == 1) {
        int i = length;
        if (found[i] == '\0') {
            putchar(' ');
            packet.put(' ');
        } else {
            for (;; i++) {
                char c = found[i];
                if (c == '\0') break;
                putchar(c);
                packet.put(c);
            }
        }

    }
}


//------------------------------------------------------------------------------
