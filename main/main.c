#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "dht-config.h"
#include "display-config.h"

#include "ui.h"

#include "esp_log.h"

static lv_color_t *buf1;
static lv_color_t *buf2;

static lv_display_t *display;

static uint32_t get_millis(void);
static void vReadTempTask(void *pvParameters);

static const char *TAG = "render";
typedef struct
{
    int16_t temp;
    int16_t humidity;
} sensor_data_t;

static QueueHandle_t queue_handle = NULL;

void app_main(void)
{
    queue_handle = xQueueCreate(2, sizeof(sensor_data_t));

    if (queue_handle == NULL)
    {
        ESP_LOGE(TAG, "It was not possible to create the queue.");
        return;
    }

    display_setup();
    display_set_brightness(100);
    lvgl_setup(display, buf1, buf2, get_millis);
    ui_setup();

    xTaskCreate(vReadTempTask, "read_temp", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

    sensor_data_t received;

    while (true)
    {

        // We don't wait any time; if there's nothing there, refresh the screen as quickly as possible.
        if (xQueueReceive(queue_handle, &received, 0) == pdPASS)
        {
            ESP_LOGI(TAG, "temp: %d, humidity: %d", received.temp, received.humidity);
            ui_update(received.temp, received.humidity);
        }

        lv_timer_handler();

        vTaskDelay(
            // Approximately 60 fps
            pdMS_TO_TICKS(16) // This setting avoids having to wait too long; a shorter wait time might cause Watchdog to restart.
        );
    }
}

static uint32_t get_millis(void)
{
    return pdTICKS_TO_MS(xTaskGetTickCount());
}

static void vReadTempTask(void *pvParameters)
{
    sensor_data_t sensor = {.temp = 0, .humidity = 0};

    // Minimum time for components to start and avoid overloading the queue.
    vTaskDelay(pdMS_TO_TICKS(1000));

    while (true)
    {
        ESP_ERROR_CHECK(dht_read_temp(&sensor.temp, &sensor.humidity));

        sensor.temp /= 10;
        sensor.humidity /= 10;

        if (xQueueSend(queue_handle, &sensor, pdMS_TO_TICKS(100)) != pdPASS)
        {
            ESP_LOGE(TAG, "Error sending data to queue.");
        }

        // It is recommended to wait 2 seconds before starting the next reading.
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
