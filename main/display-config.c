#include "display-config.h"
#include "esp_heap_caps.h"

#define DISPLAY_WIDTH 320
#define DISPlAY_HEIGHT 170

#define DISPlAY_OFFSET_X 0
#define DISPlAY_OFFSET_Y 35

#define DISPLAY_HORIZONTAL true
#define DISPLAY_INVERTED_COLOR true
#define DISPLAY_BGR false

#define DISPLAY_BUFFER DISPLAY_WIDTH *DISPlAY_HEIGHT / 10 * 2

#define DISPLAY_HOST_ID SPI2_HOST
#define DISPLAY_PCLK DISPLAY_PCLK_HZ_60
#define DISPLAY_MOSI GPIO_NUM_23
#define DISPLAY_MISO GPIO_NUM_NC
#define DISPLAY_SCLK GPIO_NUM_18
#define DISPLAY_CS GPIO_NUM_15
#define DISPLAY_DC GPIO_NUM_2
#define DISPLAY_RST GPIO_NUM_4
#define DISPLAY_BLK GPIO_NUM_32

static void flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map);

static lv_color_t *buf1;
static lv_color_t *buf2;

static lv_display_t *display;

esp_err_t display_setup(uint8_t brightness)
{
    esp_err_t result = display_init(&(display_config_t){
        .host_id = DISPLAY_HOST_ID,
        .pclk_hz = DISPLAY_PCLK,
        .mosi = DISPLAY_MOSI,
        .miso = DISPLAY_MISO,
        .sclk = DISPLAY_SCLK,
        .cs = DISPLAY_CS,
        .dc = DISPLAY_DC,
        .rst = DISPLAY_RST,
        .blk = DISPLAY_BLK,
        .width = DISPLAY_WIDTH,
        .height = DISPlAY_HEIGHT,
        .offset_x = DISPlAY_OFFSET_X,
        .offset_y = DISPlAY_OFFSET_Y,
        .is_horizontal = DISPLAY_HORIZONTAL,
        .is_inverted_color = DISPLAY_INVERTED_COLOR,
        .is_bgr = DISPLAY_BGR});

    if (result == ESP_OK)
        display_set_brightness(brightness);

    return result;
}

void lvgl_setup(lv_tick_get_cb_t millis)
{
    lv_init();
    lv_tick_set_cb(millis);

    display = lv_display_create(DISPLAY_WIDTH, DISPlAY_HEIGHT);

    buf1 = heap_caps_malloc((DISPLAY_BUFFER) * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf1);
    buf2 = heap_caps_malloc((DISPLAY_BUFFER) * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf2);

    lv_display_set_flush_cb(display, flush_cb);
    lv_display_set_buffers(display, buf1, buf2,
                           (DISPLAY_BUFFER) * sizeof(lv_color_t),
                           LV_DISPLAY_RENDER_MODE_PARTIAL);
}

static void flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    ESP_ERROR_CHECK(display_draw_bitmap(area->x1, area->y1, area->x2 + 1, area->y2 + 1, px_map));
    lv_display_flush_ready(disp);
}
