#pragma once

#include "display-st7789.h"
#include "lvgl.h"

esp_err_t display_setup(uint8_t brightness);
void lvgl_setup(lv_tick_get_cb_t millis);
