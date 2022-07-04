/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "PikaMain.h"
#include <stdio.h>
#include <stdlib.h>

PikaObj *__pikaMain;
PikaObj *pikaScriptInit(void){
    __platform_printf("======[pikascript packages installed]======\r\n");
    pks_printVersion();
    __platform_printf("PikaStdLib==v1.9.0\r\n");
    __platform_printf("pika_lvgl==latest\r\n");
    __platform_printf("===========================================\r\n");
    __pikaMain = newRootObj("pikaMain", New_PikaMain);
    extern unsigned char pikaModules_py_a[];
    obj_linkLibrary(__pikaMain, pikaModules_py_a);
#if PIKA_INIT_STRING_ENABLE
    obj_run(__pikaMain,
            "import pika_lvgl as lv\n"
            "from PikaStdLib import MemChecker\n"
            "mem = MemChecker()\n"
            "def drag_event_handler(e):\n"
            "    obj = e.get_target()\n"
            "    indev = lv.indev_get_act()\n"
            "    vect = lv.point_t()\n"
            "    indev.get_vect(vect)\n"
            "    x = obj.get_x() + vect.x\n"
            "    y = obj.get_y() + vect.y\n"
            "    obj.set_pos(x, y)\n"
            "    mem.now()\n"
            "#\n"
            "# Make an object dragable.\n"
            "#\n"
            "obj = lv.obj(lv.scr_act())\n"
            "obj.set_size(150, 100)\n"
            "obj.add_event_cb(drag_event_handler, lv.EVENT.PRESSING, None)\n"
            "label = lv.label(obj)\n"
            "label.set_text(\"Drag me\")\n"
            "label.center()\n"
            "\n");
#else 
    obj_runModule(__pikaMain, "main");
#endif
    return __pikaMain;
}

