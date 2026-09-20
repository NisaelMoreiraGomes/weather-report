#pragma once

#include "esp_err.h"

esp_err_t dht_read_temp(int16_t *temp, int16_t *humidity);
