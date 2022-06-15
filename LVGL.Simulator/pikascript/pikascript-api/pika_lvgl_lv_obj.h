/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#ifndef __pika_lvgl_lv_obj__H
#define __pika_lvgl_lv_obj__H
#include <stdio.h>
#include <stdlib.h>
#include "PikaObj.h"

PikaObj *New_pika_lvgl_lv_obj(Args *args);

void pika_lvgl_lv_obj_add_event_cb(PikaObj *self, Arg* event_cb, int filter, void* user_data);
void pika_lvgl_lv_obj_add_state(PikaObj *self, int state);
void pika_lvgl_lv_obj_align(PikaObj *self, int align, int x_ofs, int y_ofs);
void pika_lvgl_lv_obj_center(PikaObj *self);
void pika_lvgl_lv_obj_set_hight(PikaObj *self, int h);
void pika_lvgl_lv_obj_set_size(PikaObj *self, int size_x, int size_y);
void pika_lvgl_lv_obj_set_width(PikaObj *self, int w);
void pika_lvgl_lv_obj_update_layout(PikaObj *self);

#endif
