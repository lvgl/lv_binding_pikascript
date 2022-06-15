/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "pika_lvgl_label.h"
#include "pika_lvgl_lv_obj.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void pika_lvgl_label___init__Method(PikaObj *self, Args *args){
    PikaObj* parent = args_getPtr(args, "parent");
    pika_lvgl_label___init__(self, parent);
}

void pika_lvgl_label_set_long_modeMethod(PikaObj *self, Args *args){
    int mode = args_getInt(args, "mode");
    pika_lvgl_label_set_long_mode(self, mode);
}

void pika_lvgl_label_set_recolorMethod(PikaObj *self, Args *args){
    int en = args_getInt(args, "en");
    pika_lvgl_label_set_recolor(self, en);
}

void pika_lvgl_label_set_style_text_alignMethod(PikaObj *self, Args *args){
    int selector = args_getInt(args, "selector");
    int value = args_getInt(args, "value");
    pika_lvgl_label_set_style_text_align(self, selector, value);
}

void pika_lvgl_label_set_textMethod(PikaObj *self, Args *args){
    char* txt = args_getStr(args, "txt");
    pika_lvgl_label_set_text(self, txt);
}

PikaObj *New_pika_lvgl_label(Args *args){
    PikaObj *self = New_pika_lvgl_lv_obj(args);
    class_defineMethod(self, "__init__(parent:lv_obj)", pika_lvgl_label___init__Method);
    class_defineMethod(self, "set_long_mode(mode:int)", pika_lvgl_label_set_long_modeMethod);
    class_defineMethod(self, "set_recolor(en:int)", pika_lvgl_label_set_recolorMethod);
    class_defineMethod(self, "set_style_text_align(value:int,selector:int)", pika_lvgl_label_set_style_text_alignMethod);
    class_defineMethod(self, "set_text(txt:str)", pika_lvgl_label_set_textMethod);
    return self;
}

Arg *pika_lvgl_label(PikaObj *self){
    return obj_newObjInPackage(New_pika_lvgl_label);
}
