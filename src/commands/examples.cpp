#include "commands.h"

#include <stdio.h>
#include "hardware/adc.h"
#include "hardware/structs/sysinfo.h"
#include "hardware/flash.h"
#include "pico/unique_id.h"


static struct {
    uint8_t manufacturer;
    uint8_t memory_type;
    uint8_t capacity;
} flash_JEDEC_ID;

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

static void __attribute__((constructor)) _read_flash_JEDEC_ID_on_boot() {
    uint8_t txbuf[4] = {0};
    uint8_t rxbuf[4] = {0};
    // Winbond W25Q series pdf
    // 8.2.29 Read JEDEC ID (9Fh)
    txbuf[0] = 0x9f;
    flash_do_cmd(txbuf, rxbuf, 4);

    flash_JEDEC_ID.manufacturer = rxbuf[1];
    flash_JEDEC_ID.memory_type = rxbuf[2];
    flash_JEDEC_ID.capacity = rxbuf[3];
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

    printf("Flash Manufacturer: 0x%02X\n", flash_JEDEC_ID.manufacturer);
    printf("Flash Memory Type: 0x%02X\n", flash_JEDEC_ID.memory_type);
    printf("Flash Capacity: 0x%02X (%d MB)\n", flash_JEDEC_ID.capacity, (1 << flash_JEDEC_ID.capacity) / (1 << 20));
}
