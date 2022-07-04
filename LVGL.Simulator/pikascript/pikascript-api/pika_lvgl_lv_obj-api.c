/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "pika_lvgl_lv_obj.h"
#include "TinyObj.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void pika_lvgl_lv_obj_add_event_cbMethod(PikaObj *self, Args *args){
    Arg* event_cb = args_getArg(args, "event_cb");
    int filter = args_getInt(args, "filter");
    void* user_data = args_getPtr(args, "user_data");
    pika_lvgl_lv_obj_add_event_cb(self, event_cb, filter, user_data);
}

void pika_lvgl_lv_obj_add_stateMethod(PikaObj *self, Args *args){
    int state = args_getInt(args, "state");
    pika_lvgl_lv_obj_add_state(self, state);
}

void pika_lvgl_lv_obj_add_styleMethod(PikaObj *self, Args *args){
    int selector = args_getInt(args, "selector");
    PikaObj* style = args_getPtr(args, "style");
    pika_lvgl_lv_obj_add_style(self, selector, style);
}

void pika_lvgl_lv_obj_alignMethod(PikaObj *self, Args *args){
    int align = args_getInt(args, "align");
    int x_ofs = args_getInt(args, "x_ofs");
    int y_ofs = args_getInt(args, "y_ofs");
    pika_lvgl_lv_obj_align(self, align, x_ofs, y_ofs);
}

void pika_lvgl_lv_obj_centerMethod(PikaObj *self, Args *args){
    pika_lvgl_lv_obj_center(self);
}

void pika_lvgl_lv_obj_get_xMethod(PikaObj *self, Args *args){
    int res = pika_lvgl_lv_obj_get_x(self);
    method_returnInt(args, res);
}

void pika_lvgl_lv_obj_get_yMethod(PikaObj *self, Args *args){
    int res = pika_lvgl_lv_obj_get_y(self);
    method_returnInt(args, res);
}

void pika_lvgl_lv_obj_set_hightMethod(PikaObj *self, Args *args){
    int h = args_getInt(args, "h");
    pika_lvgl_lv_obj_set_hight(self, h);
}

void pika_lvgl_lv_obj_set_posMethod(PikaObj *self, Args *args){
    int x = args_getInt(args, "x");
    int y = args_getInt(args, "y");
    pika_lvgl_lv_obj_set_pos(self, x, y);
}

void pika_lvgl_lv_obj_set_sizeMethod(PikaObj *self, Args *args){
    int size_x = args_getInt(args, "size_x");
    int size_y = args_getInt(args, "size_y");
    pika_lvgl_lv_obj_set_size(self, size_x, size_y);
}

void pika_lvgl_lv_obj_set_widthMethod(PikaObj *self, Args *args){
    int w = args_getInt(args, "w");
    pika_lvgl_lv_obj_set_width(self, w);
}

void pika_lvgl_lv_obj_update_layoutMethod(PikaObj *self, Args *args){
    pika_lvgl_lv_obj_update_layout(self);
}

PikaObj *New_pika_lvgl_lv_obj(Args *args){
    PikaObj *self = New_TinyObj(args);
    class_defineMethod(self, "add_event_cb(event_cb:any,filter:int,user_data:pointer)", pika_lvgl_lv_obj_add_event_cbMethod);
    class_defineMethod(self, "add_state(state:int)", pika_lvgl_lv_obj_add_stateMethod);
    class_defineMethod(self, "add_style(style:style_t,selector:int)", pika_lvgl_lv_obj_add_styleMethod);
    class_defineMethod(self, "align(align:int,x_ofs:int,y_ofs:int)", pika_lvgl_lv_obj_alignMethod);
    class_defineMethod(self, "center()", pika_lvgl_lv_obj_centerMethod);
    class_defineMethod(self, "get_x()->int", pika_lvgl_lv_obj_get_xMethod);
    class_defineMethod(self, "get_y()->int", pika_lvgl_lv_obj_get_yMethod);
    class_defineMethod(self, "set_hight(h:int)", pika_lvgl_lv_obj_set_hightMethod);
    class_defineMethod(self, "set_pos(x:int,y:int)", pika_lvgl_lv_obj_set_posMethod);
    class_defineMethod(self, "set_size(size_x:int,size_y:int)", pika_lvgl_lv_obj_set_sizeMethod);
    class_defineMethod(self, "set_width(w:int)", pika_lvgl_lv_obj_set_widthMethod);
    class_defineMethod(self, "update_layout()", pika_lvgl_lv_obj_update_layoutMethod);
    return self;
}

Arg *pika_lvgl_lv_obj(PikaObj *self){
    return obj_newObjInPackage(New_pika_lvgl_lv_obj);
}
