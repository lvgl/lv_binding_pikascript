/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#ifndef __pika_lvgl_arc__H
#define __pika_lvgl_arc__H
#include <stdio.h>
#include <stdlib.h>
#include "PikaObj.h"

PikaObj *New_pika_lvgl_arc(Args *args);

void pika_lvgl_arc___init__(PikaObj *self, PikaObj* parent);
void pika_lvgl_arc_center(PikaObj *self);
void pika_lvgl_arc_set_end_angle(PikaObj *self, int angle);
void pika_lvgl_arc_set_size(PikaObj *self, int size_x, int size_y);

#endif
