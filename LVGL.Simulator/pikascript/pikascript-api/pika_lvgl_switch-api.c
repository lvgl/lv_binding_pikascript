/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "pika_lvgl_switch.h"
#include "pika_lvgl_lv_obj.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void pika_lvgl_switch___init__Method(PikaObj *self, Args *args){
    PikaObj* parent = args_getPtr(args, "parent");
    pika_lvgl_switch___init__(self, parent);
}

PikaObj *New_pika_lvgl_switch(Args *args){
    PikaObj *self = New_pika_lvgl_lv_obj(args);
    class_defineMethod(self, "__init__(parent:lv_obj)", pika_lvgl_switch___init__Method);
    return self;
}

Arg *pika_lvgl_switch(PikaObj *self){
    return obj_newObjInPackage(New_pika_lvgl_switch);
}
