/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "pika_lvgl_dropdown.h"
#include "pika_lvgl_lv_obj.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void pika_lvgl_dropdown___init__Method(PikaObj *self, Args *args){
    PikaObj* parent = args_getPtr(args, "parent");
    pika_lvgl_dropdown___init__(self, parent);
}

void pika_lvgl_dropdown_set_optionsMethod(PikaObj *self, Args *args){
    char* options = args_getStr(args, "options");
    pika_lvgl_dropdown_set_options(self, options);
}

PikaObj *New_pika_lvgl_dropdown(Args *args){
    PikaObj *self = New_pika_lvgl_lv_obj(args);
    class_defineMethod(self, "__init__(parent:lv_obj)", pika_lvgl_dropdown___init__Method);
    class_defineMethod(self, "set_options(options:str)", pika_lvgl_dropdown_set_optionsMethod);
    return self;
}

Arg *pika_lvgl_dropdown(PikaObj *self){
    return obj_newObjInPackage(New_pika_lvgl_dropdown);
}
