/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "pika_lvgl_lv_event.h"
#include "TinyObj.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

PikaObj *New_pika_lvgl_lv_event(Args *args){
    PikaObj *self = New_TinyObj(args);
    return self;
}

Arg *pika_lvgl_lv_event(PikaObj *self){
    return obj_newObjInPackage(New_pika_lvgl_lv_event);
}
