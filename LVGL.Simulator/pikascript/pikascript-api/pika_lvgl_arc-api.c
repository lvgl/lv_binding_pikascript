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

void pika_lvgl_arc_set_anglesMethod(PikaObj *self, Args *args){
    int end = args_getInt(args, "end");
    int start = args_getInt(args, "start");
    pika_lvgl_arc_set_angles(self, end, start);
}

void pika_lvgl_arc_set_bg_anglesMethod(PikaObj *self, Args *args){
    int end = args_getInt(args, "end");
    int start = args_getInt(args, "start");
    pika_lvgl_arc_set_bg_angles(self, end, start);
}

void pika_lvgl_arc_set_end_angleMethod(PikaObj *self, Args *args){
    int angle = args_getInt(args, "angle");
    pika_lvgl_arc_set_end_angle(self, angle);
}

PikaObj *New_pika_lvgl_arc(Args *args){
    PikaObj *self = New_pika_lvgl_lv_obj(args);
    class_defineMethod(self, "__init__(parent:lv_obj)", pika_lvgl_arc___init__Method);
    class_defineMethod(self, "set_angles(start:int,end:int)", pika_lvgl_arc_set_anglesMethod);
    class_defineMethod(self, "set_bg_angles(start:int,end:int)", pika_lvgl_arc_set_bg_anglesMethod);
    class_defineMethod(self, "set_end_angle(angle:int)", pika_lvgl_arc_set_end_angleMethod);
    return self;
}

Arg *pika_lvgl_arc(PikaObj *self){
    return obj_newObjInPackage(New_pika_lvgl_arc);
}
