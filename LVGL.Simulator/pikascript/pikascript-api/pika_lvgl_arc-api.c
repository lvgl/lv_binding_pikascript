/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "pika_lvgl_arc.h"
#include "pika_lvgl_lv_obj.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void pika_lvgl_arc___init__Method(PikaObj *self, Args *args){
    PikaObj* parent = args_getPtr(args, "parent");
    pika_lvgl_arc___init__(self, parent);
}

void pika_lvgl_arc_centerMethod(PikaObj *self, Args *args){
    pika_lvgl_arc_center(self);
}

void pika_lvgl_arc_set_end_angleMethod(PikaObj *self, Args *args){
    int angle = args_getInt(args, "angle");
    pika_lvgl_arc_set_end_angle(self, angle);
}

void pika_lvgl_arc_set_sizeMethod(PikaObj *self, Args *args){
    int size_x = args_getInt(args, "size_x");
    int size_y = args_getInt(args, "size_y");
    pika_lvgl_arc_set_size(self, size_x, size_y);
}

PikaObj *New_pika_lvgl_arc(Args *args){
    PikaObj *self = New_pika_lvgl_lv_obj(args);
    class_defineMethod(self, "__init__(parent:lv_obj)", pika_lvgl_arc___init__Method);
    class_defineMethod(self, "center()", pika_lvgl_arc_centerMethod);
    class_defineMethod(self, "set_end_angle(angle:int)", pika_lvgl_arc_set_end_angleMethod);
    class_defineMethod(self, "set_size(size_x:int,size_y:int)", pika_lvgl_arc_set_sizeMethod);
    return self;
}

Arg *pika_lvgl_arc(PikaObj *self){
    return obj_newObjInPackage(New_pika_lvgl_arc);
}
