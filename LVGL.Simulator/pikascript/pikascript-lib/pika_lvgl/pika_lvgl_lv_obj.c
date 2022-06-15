#include "pika_lvgl_lv_obj.h"
#include "BaseObj.h"
#include "lvgl.h"
#include "pika_lvgl.h"
#include "pika_lvgl_arc.h"

extern PikaObj* pika_lv_event_listener;

void pika_lvgl_lv_obj_center(PikaObj* self) {
    lv_obj_t* lv_obj = obj_getPtr(self, "lv_obj");
    lv_obj_center(lv_obj);
}

void pika_lvgl_lv_obj_set_size(PikaObj* self, int size_x, int size_y) {
    lv_obj_t* lv_obj = obj_getPtr(self, "lv_obj");
    lv_obj_set_size(lv_obj, size_x, size_y);
}

void pika_lvgl_lv_obj_align(PikaObj* self, int align, int x_ofs, int y_ofs) {
    lv_obj_t* lv_obj = obj_getPtr(self, "lv_obj");
    lv_obj_align(lv_obj, align, x_ofs, y_ofs);
}

void pika_lvgl_lv_obj_set_hight(PikaObj* self, int h) {
    lv_obj_t* lv_obj = obj_getPtr(self, "lv_obj");
    lv_obj_set_height(lv_obj, h);
}

void pika_lvgl_lv_obj_update_layout(PikaObj* self) {
    lv_obj_t* lv_obj = obj_getPtr(self, "lv_obj");
    lv_obj_update_layout(lv_obj);
}

void pika_lvgl_lv_obj_set_width(PikaObj* self, int w) {
    lv_obj_t* lv_obj = obj_getPtr(self, "lv_obj");
    lv_obj_set_width(lv_obj, w);
}

void pika_lvgl_lv_obj_add_state(PikaObj* self, int state) {
    lv_obj_t* lv_obj = obj_getPtr(self, "lv_obj");
    lv_obj_add_state(lv_obj, state);
}

static void __pika_event_cb(lv_event_t* e) {}

void pika_lvgl_lv_obj_add_event_cb(PikaObj* self,
                                   Arg* event_cb,
                                   int filter,
                                   void* user_data) {
    lv_obj_t* lv_obj = obj_getPtr(self, "lv_obj");
    lv_obj_add_event_cb(lv_obj, __pika_event_cb, LV_EVENT_ALL, NULL);
    obj_setArg(self, "_event_cb", event_cb);
    obj_setInt(self, "_evnet_filter", filter);
    obj_setPtr(self, "_event_user_data", user_data);
}
