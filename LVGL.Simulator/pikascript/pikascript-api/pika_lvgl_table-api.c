/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "pika_lvgl_table.h"
#include "pika_lvgl_lv_obj.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void pika_lvgl_table___init__Method(PikaObj *self, Args *args){
    PikaObj* parent = args_getPtr(args, "parent");
    pika_lvgl_table___init__(self, parent);
}

void pika_lvgl_table_set_cell_valueMethod(PikaObj *self, Args *args){
    int col = args_getInt(args, "col");
    int row = args_getInt(args, "row");
    char* txt = args_getStr(args, "txt");
    pika_lvgl_table_set_cell_value(self, col, row, txt);
}

PikaObj *New_pika_lvgl_table(Args *args){
    PikaObj *self = New_pika_lvgl_lv_obj(args);
    class_defineMethod(self, "__init__(parent:lv_obj)", pika_lvgl_table___init__Method);
    class_defineMethod(self, "set_cell_value(row:int,col:int,txt:str)", pika_lvgl_table_set_cell_valueMethod);
    return self;
}

Arg *pika_lvgl_table(PikaObj *self){
    return obj_newObjInPackage(New_pika_lvgl_table);
}
