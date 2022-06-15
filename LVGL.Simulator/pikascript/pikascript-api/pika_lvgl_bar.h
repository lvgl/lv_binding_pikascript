/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#ifndef __pika_lvgl_bar__H
#define __pika_lvgl_bar__H
#include <stdio.h>
#include <stdlib.h>
#include "PikaObj.h"

PikaObj *New_pika_lvgl_bar(Args *args);

void pika_lvgl_bar___init__(PikaObj *self, PikaObj* parent);
void pika_lvgl_bar_set_value(PikaObj *self, int anim, int value);

#endif
