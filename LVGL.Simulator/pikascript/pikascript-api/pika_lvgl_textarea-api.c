/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "pika_lvgl_textarea.h"
#include "pika_lvgl_lv_obj.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void pika_lvgl_textarea___init__Method(PikaObj *self, Args *args){
    PikaObj* parent = args_getPtr(args, "parent");
    pika_lvgl_textarea___init__(self, parent);
}

void pika_lvgl_textarea_set_one_lineMethod(PikaObj *self, Args *args){
    int en = args_getInt(args, "en");
    pika_lvgl_textarea_set_one_line(self, en);
}

PikaObj *New_pika_lvgl_textarea(Args *args){
    PikaObj *self = New_pika_lvgl_lv_obj(args);
    class_defineMethod(self, "__init__(parent:lv_obj)", pika_lvgl_textarea___init__Method);
    class_defineMethod(self, "set_one_line(en:int)", pika_lvgl_textarea_set_one_lineMethod);
    return self;
}

Arg *pika_lvgl_textarea(PikaObj *self){
    return obj_newObjInPackage(New_pika_lvgl_textarea);
}
