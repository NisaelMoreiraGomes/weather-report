#pragma once

#include <stdint.h>

typedef enum
{
    CELSIUS,
    FAHRENHEIT
} UNIT_MEASUREMENT;

void ui_setup(void);
void ui_set_unit_measurement(UNIT_MEASUREMENT unit);
void ui_update(int16_t temp, int16_t humidity);
