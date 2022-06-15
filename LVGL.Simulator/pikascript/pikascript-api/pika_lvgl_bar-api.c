/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "pika_lvgl_bar.h"
#include "pika_lvgl_lv_obj.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void pika_lvgl_bar___init__Method(PikaObj *self, Args *args){
    PikaObj* parent = args_getPtr(args, "parent");
    pika_lvgl_bar___init__(self, parent);
}

void pika_lvgl_bar_set_valueMethod(PikaObj *self, Args *args){
    int anim = args_getInt(args, "anim");
    int value = args_getInt(args, "value");
    pika_lvgl_bar_set_value(self, anim, value);
}

PikaObj *New_pika_lvgl_bar(Args *args){
    PikaObj *self = New_pika_lvgl_lv_obj(args);
    class_defineMethod(self, "__init__(parent:lv_obj)", pika_lvgl_bar___init__Method);
    class_defineMethod(self, "set_value(value:int,anim:int)", pika_lvgl_bar_set_valueMethod);
    return self;
}

Arg *pika_lvgl_bar(PikaObj *self){
    return obj_newObjInPackage(New_pika_lvgl_bar);
}
