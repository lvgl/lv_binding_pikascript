/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#ifndef __pika_lvgl_dropdown__H
#define __pika_lvgl_dropdown__H
#include <stdio.h>
#include <stdlib.h>
#include "PikaObj.h"

PikaObj *New_pika_lvgl_dropdown(Args *args);

void pika_lvgl_dropdown___init__(PikaObj *self, PikaObj* parent);
void pika_lvgl_dropdown_set_options(PikaObj *self, char* options);

#endif
