/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "pika_lvgl_lv_timer_t.h"
#include "TinyObj.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void pika_lvgl_lv_timer_t__delMethod(PikaObj *self, Args *args){
    pika_lvgl_lv_timer_t__del(self);
}

void pika_lvgl_lv_timer_t_set_cbMethod(PikaObj *self, Args *args){
    Arg* cb = args_getArg(args, "cb");
    pika_lvgl_lv_timer_t_set_cb(self, cb);
}

void pika_lvgl_lv_timer_t_set_periodMethod(PikaObj *self, Args *args){
    int period = args_getInt(args, "period");
    pika_lvgl_lv_timer_t_set_period(self, period);
}

PikaObj *New_pika_lvgl_lv_timer_t(Args *args){
    PikaObj *self = New_TinyObj(args);
    class_defineMethod(self, "_del()", pika_lvgl_lv_timer_t__delMethod);
    class_defineMethod(self, "set_cb(cb:any)", pika_lvgl_lv_timer_t_set_cbMethod);
    class_defineMethod(self, "set_period(period:int)", pika_lvgl_lv_timer_t_set_periodMethod);
    return self;
}

Arg *pika_lvgl_lv_timer_t(PikaObj *self){
    return obj_newObjInPackage(New_pika_lvgl_lv_timer_t);
}
