/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "pika_lvgl_EVENT.h"
#include "TinyObj.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void pika_lvgl_EVENT___init__Method(PikaObj *self, Args *args){
    pika_lvgl_EVENT___init__(self);
}

PikaObj *New_pika_lvgl_EVENT(Args *args){
    PikaObj *self = New_TinyObj(args);
    class_defineMethod(self, "__init__()", pika_lvgl_EVENT___init__Method);
    return self;
}

Arg *pika_lvgl_EVENT(PikaObj *self){
    return obj_newObjInPackage(New_pika_lvgl_EVENT);
}