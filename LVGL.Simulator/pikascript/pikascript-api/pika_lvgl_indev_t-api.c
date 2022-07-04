/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "pika_lvgl_indev_t.h"
#include "TinyObj.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void pika_lvgl_indev_t_get_vectMethod(PikaObj *self, Args *args){
    PikaObj* point = args_getPtr(args, "point");
    pika_lvgl_indev_t_get_vect(self, point);
}

PikaObj *New_pika_lvgl_indev_t(Args *args){
    PikaObj *self = New_TinyObj(args);
    class_defineMethod(self, "get_vect(point:point_t)", pika_lvgl_indev_t_get_vectMethod);
    return self;
}

Arg *pika_lvgl_indev_t(PikaObj *self){
    return obj_newObjInPackage(New_pika_lvgl_indev_t);
}
