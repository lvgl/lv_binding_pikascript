/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "pika_lvgl.h"
#include "TinyObj.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void pika_lvgl_ALIGNMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_ALIGN(self);
    method_returnArg(args, res);
}

void pika_lvgl_ANIMMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_ANIM(self);
    method_returnArg(args, res);
}

void pika_lvgl_EVENTMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_EVENT(self);
    method_returnArg(args, res);
}

void pika_lvgl_OPAMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_OPA(self);
    method_returnArg(args, res);
}

void pika_lvgl_PALETTEMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_PALETTE(self);
    method_returnArg(args, res);
}

void pika_lvgl_STATEMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_STATE(self);
    method_returnArg(args, res);
}

void pika_lvgl___init__Method(PikaObj *self, Args *args){
    pika_lvgl___init__(self);
}

void pika_lvgl_arcMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_arc(self);
    method_returnArg(args, res);
}

void pika_lvgl_barMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_bar(self);
    method_returnArg(args, res);
}

void pika_lvgl_btnMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_btn(self);
    method_returnArg(args, res);
}

void pika_lvgl_checkboxMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_checkbox(self);
    method_returnArg(args, res);
}

void pika_lvgl_dropdownMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_dropdown(self);
    method_returnArg(args, res);
}

void pika_lvgl_labelMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_label(self);
    method_returnArg(args, res);
}

void pika_lvgl_lv_color_tMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_lv_color_t(self);
    method_returnArg(args, res);
}

void pika_lvgl_lv_eventMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_lv_event(self);
    method_returnArg(args, res);
}

void pika_lvgl_lv_objMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_lv_obj(self);
    method_returnArg(args, res);
}

void pika_lvgl_objMethod(PikaObj *self, Args *args){
    PikaObj* parent = args_getPtr(args, "parent");
    PikaObj* res = pika_lvgl_obj(self, parent);
    method_returnObj(args, res);
}

void pika_lvgl_palette_lightenMethod(PikaObj *self, Args *args){
    int lvl = args_getInt(args, "lvl");
    int p = args_getInt(args, "p");
    PikaObj* res = pika_lvgl_palette_lighten(self, lvl, p);
    method_returnObj(args, res);
}

void pika_lvgl_palette_mainMethod(PikaObj *self, Args *args){
    int p = args_getInt(args, "p");
    PikaObj* res = pika_lvgl_palette_main(self, p);
    method_returnObj(args, res);
}

void pika_lvgl_rollerMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_roller(self);
    method_returnArg(args, res);
}

void pika_lvgl_scr_actMethod(PikaObj *self, Args *args){
    PikaObj* res = pika_lvgl_scr_act(self);
    method_returnObj(args, res);
}

void pika_lvgl_sliderMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_slider(self);
    method_returnArg(args, res);
}

void pika_lvgl_style_tMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_style_t(self);
    method_returnArg(args, res);
}

void pika_lvgl_switchMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_switch(self);
    method_returnArg(args, res);
}

void pika_lvgl_tableMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_table(self);
    method_returnArg(args, res);
}

void pika_lvgl_textareaMethod(PikaObj *self, Args *args){
    Arg* res = pika_lvgl_textarea(self);
    method_returnArg(args, res);
}

PikaObj *New_pika_lvgl(Args *args){
    PikaObj *self = New_TinyObj(args);
    class_defineConstructor(self, "ALIGN()->any", pika_lvgl_ALIGNMethod);
    class_defineConstructor(self, "ANIM()->any", pika_lvgl_ANIMMethod);
    class_defineConstructor(self, "EVENT()->any", pika_lvgl_EVENTMethod);
    class_defineConstructor(self, "OPA()->any", pika_lvgl_OPAMethod);
    class_defineConstructor(self, "PALETTE()->any", pika_lvgl_PALETTEMethod);
    class_defineConstructor(self, "STATE()->any", pika_lvgl_STATEMethod);
    class_defineMethod(self, "__init__()", pika_lvgl___init__Method);
    class_defineConstructor(self, "arc()->any", pika_lvgl_arcMethod);
    class_defineConstructor(self, "bar()->any", pika_lvgl_barMethod);
    class_defineConstructor(self, "btn()->any", pika_lvgl_btnMethod);
    class_defineConstructor(self, "checkbox()->any", pika_lvgl_checkboxMethod);
    class_defineConstructor(self, "dropdown()->any", pika_lvgl_dropdownMethod);
    class_defineConstructor(self, "label()->any", pika_lvgl_labelMethod);
    class_defineConstructor(self, "lv_color_t()->any", pika_lvgl_lv_color_tMethod);
    class_defineConstructor(self, "lv_event()->any", pika_lvgl_lv_eventMethod);
    class_defineConstructor(self, "lv_obj()->any", pika_lvgl_lv_objMethod);
    class_defineMethod(self, "obj(parent:lv_obj)->lv_obj", pika_lvgl_objMethod);
    class_defineMethod(self, "palette_lighten(p:int,lvl:int)->lv_color_t", pika_lvgl_palette_lightenMethod);
    class_defineMethod(self, "palette_main(p:int)->lv_color_t", pika_lvgl_palette_mainMethod);
    class_defineConstructor(self, "roller()->any", pika_lvgl_rollerMethod);
    class_defineMethod(self, "scr_act()->lv_obj", pika_lvgl_scr_actMethod);
    class_defineConstructor(self, "slider()->any", pika_lvgl_sliderMethod);
    class_defineConstructor(self, "style_t()->any", pika_lvgl_style_tMethod);
    class_defineConstructor(self, "switch()->any", pika_lvgl_switchMethod);
    class_defineConstructor(self, "table()->any", pika_lvgl_tableMethod);
    class_defineConstructor(self, "textarea()->any", pika_lvgl_textareaMethod);
    return self;
}

