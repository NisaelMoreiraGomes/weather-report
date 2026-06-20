#include "ui.h"
#include "lvgl.h"
#include "assets/fonts/inter_semibold_48.c"
#include "assets/fonts/inter_semibold_64.c"
#include "assets/images/logo.c"

static lv_obj_t *temp_label = NULL;
static lv_obj_t *unity_label = NULL;
static lv_obj_t *humidity_label = NULL;

void ui_setup(void)
{
    lv_obj_t *screen = lv_screen_active();

    /**
     * BACKGROUND
     */
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x1504D0), LV_PART_MAIN);
    lv_obj_set_style_bg_grad_color(screen, lv_color_hex(0x210B86), LV_PART_MAIN);
    lv_obj_set_style_bg_grad_dir(screen, LV_GRAD_DIR_VER, LV_PART_MAIN);

    /**
     * Remove Scrollbar
     */
    lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);

    /**
     * PANEL
     */
    lv_obj_t *panel = lv_obj_create(screen);
    lv_obj_set_size(panel, 315, 100);

    lv_obj_set_style_bg_color(panel, lv_color_black(), LV_PART_MAIN);
    lv_obj_set_style_border_width(panel, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(panel, 100, LV_PART_MAIN);
    lv_obj_set_style_radius(panel, 20, LV_PART_MAIN);
    lv_obj_align(panel, LV_ALIGN_TOP_MID, 0, 7);
    lv_obj_set_style_text_color(panel, lv_color_white(), LV_PART_MAIN);
    lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(
        panel,
        LV_FLEX_ALIGN_SPACE_BETWEEN,
        LV_FLEX_ALIGN_CENTER,
        LV_FLEX_ALIGN_CENTER);

    /**
     * TEMP Label
     */
    temp_label = lv_label_create(panel);
    lv_label_set_text(temp_label, "00°");
    lv_obj_set_style_text_font(temp_label, &inter_semibold_64, 0);

    /**
     * UNITY Label
     */
    unity_label = lv_label_create(panel);
    lv_label_set_text(unity_label, "C");
    lv_obj_set_style_text_font(unity_label, &inter_semibold_48, 0);
    lv_obj_add_flag(unity_label, LV_OBJ_FLAG_FLOATING);
    lv_obj_set_x(unity_label, (43 * 2) - 5);
    lv_obj_set_y(unity_label, 43 - 3);

    /**
     * HUMIDITY Label
     */
    humidity_label = lv_label_create(panel);
    lv_label_set_text(humidity_label, "00%");
    lv_obj_set_style_text_font(humidity_label, &inter_semibold_64, 0);

    /**
     * Logo
     */
    lv_obj_t *img_logo = lv_image_create(screen);
    lv_image_set_src(img_logo, &logo);

    lv_obj_align(img_logo, LV_ALIGN_BOTTOM_MID, 0, 55);
}

void ui_set_unit_measurement(UNIT_MEASUREMENT unit)
{
    if (unit == CELSIUS)
        lv_label_set_text(unity_label, "C");
    else
        lv_label_set_text(unity_label, "F");
}

void ui_update(int16_t temp, int16_t humidity)
{
    lv_label_set_text_fmt(temp_label, "%d°", temp);
    lv_label_set_text_fmt(humidity_label, "%d%%", humidity);
}
