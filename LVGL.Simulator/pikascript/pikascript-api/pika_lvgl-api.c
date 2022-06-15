/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "pika_lvgl.h"
#include "TinyObj.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void pika_lvgl_arcMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_arc(self);
    method_returnArg(args, res);
}

void pika_lvgl_barMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_bar(self);
    method_returnArg(args, res);
}

void pika_lvgl_btnMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_btn(self);
    method_returnArg(args, res);
}

void pika_lvgl_checkboxMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_checkbox(self);
    method_returnArg(args, res);
}

void pika_lvgl_lv_objMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_lv_obj(self);
    method_returnArg(args, res);
}

void pika_lvgl_scr_actMethod(PikaObj *self, Args *args){
    PikaObj* res = pika_lvgl_scr_act(self);
    method_returnObj(args, res);
}

PikaObj *New_pika_lvgl(Args *args){
    PikaObj *self = New_TinyObj(args);
    class_defineConstructor(self, "arc()->any", pika_lvgl_arcMethod);
    class_defineConstructor(self, "bar()->any", pika_lvgl_barMethod);
    class_defineConstructor(self, "btn()->any", pika_lvgl_btnMethod);
    class_defineConstructor(self, "checkbox()->any", pika_lvgl_checkboxMethod);
    class_defineConstructor(self, "lv_obj()->any", pika_lvgl_lv_objMethod);
    class_defineMethod(self, "scr_act()->lv_obj", pika_lvgl_scr_actMethod);
    return self;
}

