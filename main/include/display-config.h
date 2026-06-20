#pragma once

#include "display-st7789.h"
#include "lvgl.h"

#define DISPLAY_WIDTH 320
#define DISPlAY_HEIGHT 170

#define DISPlAY_OFFSET_X 0
#define DISPlAY_OFFSET_Y 35

#define DISPLAY_HORIZONTAL true
#define DISPLAY_INVERTED_COLOR true
#define DISPLAY_BGR false

#define DISPLAY_BUFFER DISPLAY_WIDTH * DISPlAY_HEIGHT / 10 * 2

#define DISPLAY_HOST_ID SPI2_HOST
#define DISPLAY_PCLK DISPLAY_PCLK_HZ_60
#define DISPLAY_MOSI GPIO_NUM_23
#define DISPLAY_MISO GPIO_NUM_NC
#define DISPLAY_SCLK GPIO_NUM_18
#define DISPLAY_CS GPIO_NUM_15
#define DISPLAY_DC GPIO_NUM_2
#define DISPLAY_RST GPIO_NUM_4
#define DISPLAY_BLK GPIO_NUM_32

esp_err_t display_setup(void);
void lvgl_setup(lv_display_t *display, lv_color_t *buf1, lv_color_t *buf2, lv_tick_get_cb_t millis);
void flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map);
