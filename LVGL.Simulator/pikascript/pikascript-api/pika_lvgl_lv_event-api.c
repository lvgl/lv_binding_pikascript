/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "pika_lvgl_lv_event.h"
#include "TinyObj.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void pika_lvgl_lv_event_get_codeMethod(PikaObj *self, Args *args){
    int res = pika_lvgl_lv_event_get_code(self);
    method_returnInt(args, res);
}

PikaObj *New_pika_lvgl_lv_event(Args *args){
    PikaObj *self = New_TinyObj(args);
    class_defineMethod(self, "get_code()->int", pika_lvgl_lv_event_get_codeMethod);
    return self;
}

Arg *pika_lvgl_lv_event(PikaObj *self){
    return obj_newObjInPackage(New_pika_lvgl_lv_event);
}
