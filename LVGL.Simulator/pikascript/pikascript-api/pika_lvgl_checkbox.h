/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#ifndef __pika_lvgl_checkbox__H
#define __pika_lvgl_checkbox__H
#include <stdio.h>
#include <stdlib.h>
#include "PikaObj.h"

PikaObj *New_pika_lvgl_checkbox(Args *args);

void pika_lvgl_checkbox___init__(PikaObj *self, PikaObj* parent);
void pika_lvgl_checkbox_set_text(PikaObj *self, char* txt);

#endif
