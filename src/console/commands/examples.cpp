#include "commands.h"

#include <stdio.h>
#include "hardware/adc.h"
#include "hardware/structs/sysinfo.h"
#include "pico/unique_id.h"


void command_echo(Console &c) {
    printf("%s\n", c.packet.take_rest_string());
}

void command_sensors(Console &c) {
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4);

    /* 12-bit conversion, assume max value == ADC_VREF == 3.3 V */
    const float conversionFactor = 3.3f / (1 << 12);

    float adc = (float) adc_read() * conversionFactor;
    float tempC = 27.0f - (adc - 0.706f) / 0.001721f;

    printf("Onboard temperature: %+.01f °C\n", tempC);
}

void command_chip_id(Console &c) {
    uint32_t chip_id = sysinfo_hw->chip_id;
    uint16_t manufacturer = (chip_id >> 0) & 0x0fff;
    uint16_t part_id = (chip_id >> 12) & 0xffff;
    uint16_t revision = (chip_id >> 28) & 0x0f;

    printf("Manufacturer: 0x%03X\n", manufacturer);
    printf("Part ID: 0x%04X\n", part_id);
    printf("Revision: %s\n", revision == 2 ? "B2" : "B0/B1");

    char board_id[32];
    pico_get_unique_board_id_string(board_id, sizeof(board_id));
    printf("Unique board identifier: %s\n", board_id);
}
