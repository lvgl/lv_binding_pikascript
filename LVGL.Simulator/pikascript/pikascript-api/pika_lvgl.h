/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#ifndef __pika_lvgl__H
#define __pika_lvgl__H
#include <stdio.h>
#include <stdlib.h>
#include "PikaObj.h"

PikaObj *New_pika_lvgl(Args *args);

void pika_lvgl___init__(PikaObj *self);
Arg* pika_lvgl_arc(PikaObj *self);
Arg* pika_lvgl_bar(PikaObj *self);
Arg* pika_lvgl_btn(PikaObj *self);
Arg* pika_lvgl_checkbox(PikaObj *self);
Arg* pika_lvgl_dropdown(PikaObj *self);
Arg* pika_lvgl_label(PikaObj *self);
Arg* pika_lvgl_lv_obj(PikaObj *self);
PikaObj* pika_lvgl_obj(PikaObj *self, PikaObj* parent);
Arg* pika_lvgl_roller(PikaObj *self);
PikaObj* pika_lvgl_scr_act(PikaObj *self);
Arg* pika_lvgl_slider(PikaObj *self);
Arg* pika_lvgl_switch(PikaObj *self);
Arg* pika_lvgl_table(PikaObj *self);
Arg* pika_lvgl_textarea(PikaObj *self);

#endif
