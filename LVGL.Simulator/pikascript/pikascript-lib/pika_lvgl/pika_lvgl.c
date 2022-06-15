#include "pika_lvgl.h"
#include "BaseObj.h"
#include "lvgl.h"
#include "pika_lvgl_arc.h"
#include "pika_lvgl_lv_obj.h"

PikaObj* pika_lvgl_scr_act(PikaObj* self) {
    PikaObj* new_obj = newNormalObj(New_TinyObj);
    lv_obj_t* lv_obj = lv_scr_act();
    obj_setPtr(new_obj, "lv_obj", lv_obj);
    return new_obj;
}

void pika_lvgl___init__(PikaObj* self) {
    /* set const */
    obj_newDirectObj(self, "ALIGN", New_TinyObj);
    obj_setInt(self, "ALIGN.CENTER", LV_ALIGN_CENTER);
    obj_setInt(self, "ALIGN.DEFAULT", LV_ALIGN_DEFAULT);
    obj_setInt(self, "ALIGN.TOP_LEFT", LV_ALIGN_TOP_LEFT);
    obj_setInt(self, "ALIGN.TOP_MID", LV_ALIGN_TOP_MID);
    obj_setInt(self, "ALIGN.TOP_RIGHT", LV_ALIGN_TOP_RIGHT);
    obj_setInt(self, "ALIGN.BOTTOM_LEFT", LV_ALIGN_BOTTOM_LEFT);
    obj_setInt(self, "ALIGN.BOTTOM_MID", LV_ALIGN_BOTTOM_MID);
    obj_setInt(self, "ALIGN.BOTTOM_RIGHT", LV_ALIGN_BOTTOM_RIGHT);
    obj_setInt(self, "ALIGN.LEFT_MID", LV_ALIGN_LEFT_MID);
    obj_setInt(self, "ALIGN.RIGHT_MID", LV_ALIGN_RIGHT_MID);
    obj_setInt(self, "ALIGN.OUT_TOP_LEFT", LV_ALIGN_OUT_TOP_LEFT);
    obj_setInt(self, "ALIGN.OUT_TOP_MID", LV_ALIGN_OUT_TOP_MID);
    obj_setInt(self, "ALIGN.OUT_TOP_RIGHT", LV_ALIGN_OUT_TOP_RIGHT);
    obj_setInt(self, "ALIGN.OUT_BOTTOM_LEFT", LV_ALIGN_OUT_BOTTOM_LEFT);
    obj_setInt(self, "ALIGN.OUT_BOTTOM_MID", LV_ALIGN_OUT_BOTTOM_MID);
    obj_setInt(self, "ALIGN.OUT_BOTTOM_RIGHT", LV_ALIGN_OUT_BOTTOM_RIGHT);
    obj_setInt(self, "ALIGN.OUT_LEFT_TOP", LV_ALIGN_OUT_LEFT_TOP);
    obj_setInt(self, "ALIGN.OUT_LEFT_MID", LV_ALIGN_OUT_LEFT_MID);
    obj_setInt(self, "ALIGN.OUT_LEFT_BOTTOM", LV_ALIGN_OUT_LEFT_BOTTOM);
    obj_setInt(self, "ALIGN.OUT_RIGHT_TOP", LV_ALIGN_OUT_RIGHT_TOP);
    obj_setInt(self, "ALIGN.OUT_RIGHT_MID", LV_ALIGN_OUT_RIGHT_MID);
    obj_setInt(self, "ALIGN.OUT_RIGHT_BOTTOM", LV_ALIGN_OUT_RIGHT_BOTTOM);
}
