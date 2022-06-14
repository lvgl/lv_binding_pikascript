#include "pika_lvgl.h"
#include "BaseObj.h"
#include "lvgl.h"
#include "pika_lvgl_arc.h"
#include "pika_lvgl_lv_obj.h"

void pika_lvgl_arc___init__(PikaObj* self, PikaObj* parent) {
    lv_obj_t* lv_parent = obj_getPtr(parent, "lv_obj");
    lv_obj_t* lv_obj = lv_arc_create(lv_parent);
    obj_setPtr(self, "lv_obj", lv_obj);
}

void pika_lvgl_arc_center(PikaObj* self) {
    lv_obj_t* lv_obj = obj_getPtr(self, "lv_obj");
    lv_obj_center(lv_obj);
}

void pika_lvgl_arc_set_end_angle(PikaObj* self, int angle) {
    lv_obj_t* lv_obj = obj_getPtr(self, "lv_obj");
    lv_arc_set_end_angle(lv_obj, angle);
}

void pika_lvgl_arc_set_size(PikaObj* self, int size_x, int size_y) {
    lv_obj_t* lv_obj = obj_getPtr(self, "lv_obj");
    lv_obj_set_size(lv_obj, size_x, size_y);
}

PikaObj* pika_lvgl_scr_act(PikaObj* self) {
    PikaObj* new_obj = newNormalObj(New_TinyObj);
    lv_obj_t* lv_obj = lv_scr_act();
    obj_setPtr(new_obj, "lv_obj", lv_obj);
    return new_obj;
}
