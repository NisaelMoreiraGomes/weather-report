#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "dht-config.h"
#include "display-config.h"

#include "ui.h"

#include "esp_log.h"

#define QUEUE_SEND_DELAY_MS 100
#define QUEUE_RECEIVE_DELAY_MS 0

static uint32_t get_millis(void);

static void vReadTempTask(void *pvParameters);
static void vDrawTask(void *pvParameters);

static const char *TAG = "queue";

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

    ESP_ERROR_CHECK(
        // I find the brightness at 100% (255) too strong on this display,
        // so I reduced it slightly to make it more comfortable.
        display_setup(100));
    lvgl_setup(get_millis);
    ui_setup();

    xTaskCreate(vReadTempTask, "vReadTempTask", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
    /**
     * I preferred to prioritize reading the sensor.
     * Rendering can wait a bit; the data is more important.
     */
    xTaskCreate(vDrawTask, "vDrawTask", configMINIMAL_STACK_SIZE * 3, NULL, 1, NULL);
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

    for (;;)
    {
        ESP_ERROR_CHECK(dht_read_temp(&sensor.temp, &sensor.humidity));

        sensor.temp /= 10;
        sensor.humidity /= 10;

        if (xQueueSend(queue_handle, &sensor, pdMS_TO_TICKS(QUEUE_SEND_DELAY_MS)) != pdPASS)
        {
            ESP_LOGE(TAG, "Error sending data to queue.");
        }

        // It is recommended to wait 2 seconds before starting the next reading.
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

static void vDrawTask(void *pvParameters)
{
    sensor_data_t received;

    for (;;)
    {
        // We don't wait any time; if there's nothing there, refresh the screen as quickly as possible.
        if (xQueueReceive(queue_handle, &received, pdMS_TO_TICKS(QUEUE_RECEIVE_DELAY_MS)) == pdPASS)
        {
            ESP_LOGI(TAG, "temp: %d, humidity: %d", received.temp, received.humidity);
            ui_update(received.temp, received.humidity);
        }

        uint32_t time = lv_timer_handler();

        /**
         * This is equivalent to an average of 60 FPS (1000ms/60 FPS == 16ms).
         *
         * If LVGL requires more time, we will respect that as well,
         * even if it means dropping below 60 FPS.
         *
         * We also guarantee a minimum delay of 16ms to avoid excessive
         * CPU usage, reducing the load on FreeRTOS and preventing
         * unnecessary watchdog triggers on the ESP32.
         */
        if (time == UINT32_MAX || time < 16)
            time = 16;

        vTaskDelay(pdMS_TO_TICKS(time));
    }
}
