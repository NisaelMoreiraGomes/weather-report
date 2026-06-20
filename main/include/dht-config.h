#pragma once

#include "dht.h"

#define DHT_GPIO_PIN GPIO_NUM_5
#define DHT_SENSOR_TYPE DHT_TYPE_AM2301 // DHT22

esp_err_t dht_read_temp(int16_t *temp, int16_t *humidity);
