/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "pika_lvgl_checkbox.h"
#include "pika_lvgl_lv_obj.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void pika_lvgl_checkbox___init__Method(PikaObj *self, Args *args){
    PikaObj* parent = args_getPtr(args, "parent");
    pika_lvgl_checkbox___init__(self, parent);
}

void pika_lvgl_checkbox_set_textMethod(PikaObj *self, Args *args){
    char* txt = args_getStr(args, "txt");
    pika_lvgl_checkbox_set_text(self, txt);
}

PikaObj *New_pika_lvgl_checkbox(Args *args){
    PikaObj *self = New_pika_lvgl_lv_obj(args);
    class_defineMethod(self, "__init__(parent:lv_obj)", pika_lvgl_checkbox___init__Method);
    class_defineMethod(self, "set_text(txt:str)", pika_lvgl_checkbox_set_textMethod);
    return self;
}

Arg *pika_lvgl_checkbox(PikaObj *self){
    return obj_newObjInPackage(New_pika_lvgl_checkbox);
}
