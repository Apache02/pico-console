#include "commands.h"

#include <stdio.h>


void command_board_help(Console &c) {
    printf("                             --------------                       \n");
    printf("           [I2C 0 SDA |  0] | o   |__|   o | [%s]                 \n", COLOR_RED("VBUS"));
    printf("           [I2C 0 SCL |  1] |              | [%s]                 \n", COLOR_RED("VSYS"));
    printf("                      [GND] |              | [GND]                \n");
    printf("           [I2C 1 SDA |  2] |              | [%s]                 \n", COLOR_YELLOW("3V3_EN"));
    printf("           [I2C 1 SCL |  3] |              | [%s]                 \n", COLOR_RED("3V3(OUT)"));
    printf("           [I2C 0 SDA |  4] |              | ---------[ADC_VREF]  \n");
    printf("           [I2C 0 SCL |  5] |              | [28]                 \n");
    printf("                      [GND] |              | [GND]                \n");
    printf("           [I2C 1 SDA |  6] |              | [27 | I2C 1 SCL]     \n");
    printf("           [I2C 1 SCL |  7] |              | [26 | I2C 1 SDA]     \n");
    printf("           [I2C 0 SDA |  8] |              | [RUN]                \n");
    printf("           [I2C 0 SCL |  9] |              | [22 | ]              \n");
    printf("                      [GND] |              | [GND]                \n");
    printf("           [I2C 1 SDA | 10] |              | [21 | I2C 0 SCL]     \n");
    printf("           [I2C 1 SCL | 11] |              | [20 | I2C 0 SDA]     \n");
    printf("           [I2C 0 SDA | 12] |              | [19 | I2C 1 SCL]     \n");
    printf("           [I2C 0 SCL | 13] |              | [18 | I2C 1 SDA]     \n");
    printf("                      [GND] |              | [GND]                \n");
    printf("           [I2C 1 SDA | 14] |              | [17 | I2C 0 SCL]     \n");
    printf("           [I2C 1 SCL | 15] | o          o | [16 | I2C 0 SDA]     \n");
    printf("                             --------------                       \n");
    printf("\n");
}
