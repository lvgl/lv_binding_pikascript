/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "pika_lvgl_ALIGN.h"
#include "TinyObj.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void pika_lvgl_ALIGN___init__Method(PikaObj *self, Args *args){
    pika_lvgl_ALIGN___init__(self);
}

PikaObj *New_pika_lvgl_ALIGN(Args *args){
    PikaObj *self = New_TinyObj(args);
    class_defineMethod(self, "__init__()", pika_lvgl_ALIGN___init__Method);
    return self;
}

Arg *pika_lvgl_ALIGN(PikaObj *self){
    return obj_newObjInPackage(New_pika_lvgl_ALIGN);
}
