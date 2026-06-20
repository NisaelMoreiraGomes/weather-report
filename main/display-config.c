#include "display-config.h"
#include "esp_heap_caps.h"

esp_err_t display_setup(void)
{
    return display_init(&(display_config_t){
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
}

void lvgl_setup(lv_display_t *display, lv_color_t *buf1, lv_color_t *buf2, lv_tick_get_cb_t millis)
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

void flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    ESP_ERROR_CHECK(display_draw_bitmap(area->x1, area->y1, area->x2 + 1, area->y2 + 1, px_map));
    lv_display_flush_ready(disp);
}
