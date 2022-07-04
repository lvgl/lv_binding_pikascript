/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "pika_lvgl_point_t.h"
#include "TinyObj.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void pika_lvgl_point_t___init__Method(PikaObj *self, Args *args){
    pika_lvgl_point_t___init__(self);
}

PikaObj *New_pika_lvgl_point_t(Args *args){
    PikaObj *self = New_TinyObj(args);
    class_defineMethod(self, "__init__()", pika_lvgl_point_t___init__Method);
    return self;
}

Arg *pika_lvgl_point_t(PikaObj *self){
    return obj_newObjInPackage(New_pika_lvgl_point_t);
}
