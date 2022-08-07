/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "pika_lvgl_roller.h"
#include "pika_lvgl_lv_obj.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void pika_lvgl_roller___init__Method(PikaObj *self, Args *args){
    PikaObj* parent = args_getPtr(args, "parent");
    pika_lvgl_roller___init__(self, parent);
}

void pika_lvgl_roller_set_optionsMethod(PikaObj *self, Args *args){
    char* options = args_getStr(args, "options");
    int mode = args_getInt(args, "mode");
    pika_lvgl_roller_set_options(self, options, mode);
}

void pika_lvgl_roller_set_visible_row_countMethod(PikaObj *self, Args *args){
    int row_cnt = args_getInt(args, "row_cnt");
    pika_lvgl_roller_set_visible_row_count(self, row_cnt);
}

PikaObj *New_pika_lvgl_roller(Args *args){
    PikaObj *self = New_pika_lvgl_lv_obj(args);
    class_defineMethod(self, "__init__(parent:lv_obj)", pika_lvgl_roller___init__Method);
    class_defineMethod(self, "set_options(options:str,mode:int)", pika_lvgl_roller_set_optionsMethod);
    class_defineMethod(self, "set_visible_row_count(row_cnt:int)", pika_lvgl_roller_set_visible_row_countMethod);
    return self;
}

Arg *pika_lvgl_roller(PikaObj *self){
    return obj_newObjInPackage(New_pika_lvgl_roller);
}
