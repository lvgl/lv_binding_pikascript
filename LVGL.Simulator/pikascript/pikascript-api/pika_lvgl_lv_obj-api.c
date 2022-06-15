/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "pika_lvgl_lv_obj.h"
#include "TinyObj.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void pika_lvgl_lv_obj_centerMethod(PikaObj *self, Args *args){
    pika_lvgl_lv_obj_center(self);
}

void pika_lvgl_lv_obj_set_sizeMethod(PikaObj *self, Args *args){
    int size_x = args_getInt(args, "size_x");
    int size_y = args_getInt(args, "size_y");
    pika_lvgl_lv_obj_set_size(self, size_x, size_y);
}

PikaObj *New_pika_lvgl_lv_obj(Args *args){
    PikaObj *self = New_TinyObj(args);
    class_defineMethod(self, "center()", pika_lvgl_lv_obj_centerMethod);
    class_defineMethod(self, "set_size(size_x:int,size_y:int)", pika_lvgl_lv_obj_set_sizeMethod);
    return self;
}

Arg *pika_lvgl_lv_obj(PikaObj *self){
    return obj_newObjInPackage(New_pika_lvgl_lv_obj);
}
