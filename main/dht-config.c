#include "dht-config.h"

esp_err_t dht_read_temp(int16_t *temp, int16_t *humidity)
{
    return dht_read_data(DHT_SENSOR_TYPE, DHT_GPIO_PIN, humidity, temp);
}
