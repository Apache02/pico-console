#pragma once

#include "Packet.h"

struct Console {
    Console();

    ~Console();

    void reset();

    void dump();

    void start();

    void print_help();

    void update(char * input, unsigned int count);

    bool dispatch_command();

    Packet packet;
};
