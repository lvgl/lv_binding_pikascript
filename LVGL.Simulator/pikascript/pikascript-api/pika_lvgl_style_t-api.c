/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "pika_lvgl_style_t.h"
#include "TinyObj.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void pika_lvgl_style_t_initMethod(PikaObj *self, Args *args){
    pika_lvgl_style_t_init(self);
}

void pika_lvgl_style_t_set_bg_colorMethod(PikaObj *self, Args *args){
    int color = args_getInt(args, "color");
    pika_lvgl_style_t_set_bg_color(self, color);
}

void pika_lvgl_style_t_set_bg_opaMethod(PikaObj *self, Args *args){
    int opa = args_getInt(args, "opa");
    pika_lvgl_style_t_set_bg_opa(self, opa);
}

void pika_lvgl_style_t_set_outline_colorMethod(PikaObj *self, Args *args){
    int color = args_getInt(args, "color");
    pika_lvgl_style_t_set_outline_color(self, color);
}

void pika_lvgl_style_t_set_outline_padMethod(PikaObj *self, Args *args){
    int pad = args_getInt(args, "pad");
    pika_lvgl_style_t_set_outline_pad(self, pad);
}

void pika_lvgl_style_t_set_outline_widthMethod(PikaObj *self, Args *args){
    int w = args_getInt(args, "w");
    pika_lvgl_style_t_set_outline_width(self, w);
}

void pika_lvgl_style_t_set_radiusMethod(PikaObj *self, Args *args){
    int radius = args_getInt(args, "radius");
    pika_lvgl_style_t_set_radius(self, radius);
}

PikaObj *New_pika_lvgl_style_t(Args *args){
    PikaObj *self = New_TinyObj(args);
    class_defineMethod(self, "init()", pika_lvgl_style_t_initMethod);
    class_defineMethod(self, "set_bg_color(color:int)", pika_lvgl_style_t_set_bg_colorMethod);
    class_defineMethod(self, "set_bg_opa(opa:int)", pika_lvgl_style_t_set_bg_opaMethod);
    class_defineMethod(self, "set_outline_color(color:int)", pika_lvgl_style_t_set_outline_colorMethod);
    class_defineMethod(self, "set_outline_pad(pad:int)", pika_lvgl_style_t_set_outline_padMethod);
    class_defineMethod(self, "set_outline_width(w:int)", pika_lvgl_style_t_set_outline_widthMethod);
    class_defineMethod(self, "set_radius(radius:int)", pika_lvgl_style_t_set_radiusMethod);
    return self;
}

Arg *pika_lvgl_style_t(PikaObj *self){
    return obj_newObjInPackage(New_pika_lvgl_style_t);
}
