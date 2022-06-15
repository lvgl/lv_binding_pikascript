/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#ifndef __pika_lvgl__H
#define __pika_lvgl__H
#include <stdio.h>
#include <stdlib.h>
#include "PikaObj.h"

PikaObj *New_pika_lvgl(Args *args);

Arg* pika_lvgl_arc(PikaObj *self);
Arg* pika_lvgl_bar(PikaObj *self);
Arg* pika_lvgl_btn(PikaObj *self);
Arg* pika_lvgl_checkbox(PikaObj *self);
Arg* pika_lvgl_lv_obj(PikaObj *self);
PikaObj* pika_lvgl_scr_act(PikaObj *self);

#endif
