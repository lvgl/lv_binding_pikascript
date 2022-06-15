#include "pika_lvgl_lv_obj.h"
#include "BaseObj.h"
#include "lvgl.h"
#include "pika_lvgl.h"
#include "pika_lvgl_arc.h"

void pika_lvgl_lv_obj_center(PikaObj* self) {
    lv_obj_t* lv_obj = obj_getPtr(self, "lv_obj");
    lv_obj_center(lv_obj);
}

void pika_lvgl_lv_obj_set_size(PikaObj* self, int size_x, int size_y) {
    lv_obj_t* lv_obj = obj_getPtr(self, "lv_obj");
    lv_obj_set_size(lv_obj, size_x, size_y);
}
