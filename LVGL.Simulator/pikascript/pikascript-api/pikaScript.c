/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "PikaMain.h"
#include <stdio.h>
#include <stdlib.h>

PikaObj *__pikaMain;
PikaObj *pikaScriptInit(void){
    __platform_printf("======[pikascript packages installed]======\r\n");
    __platform_printf("PikaStdLib==latest\r\n");
    __platform_printf("pika_lvgl==latest\r\n");
    __platform_printf("pikascript-core==latest\r\n");
    __platform_printf("===========================================\r\n");
    __pikaMain = newRootObj("pikaMain", New_PikaMain);
    extern unsigned char pikaModules_py_a[];
    obj_linkLibrary(__pikaMain, pikaModules_py_a);
#if PIKA_INIT_STRING_ENABLE
    obj_run(__pikaMain,
            "import pika_lvgl as lv\n"
            "import PikaStdLib\n"
            "mem = PikaStdLib.MemChecker()\n"
            "cb = lv.checkbox(lv.scr_act())\n"
            "cb.set_text(\"Apple\")\n"
            "cb.align(lv.ALIGN.TOP_LEFT, 0 ,0)\n"
            "cb = lv.checkbox(lv.scr_act())\n"
            "cb.set_text(\"Banana\")\n"
            "cb.add_state(lv.STATE.CHECKED)\n"
            "cb.align(lv.ALIGN.TOP_LEFT, 0 ,30)\n"
            "cb = lv.checkbox(lv.scr_act())\n"
            "cb.set_text(\"Lemon\")\n"
            "cb.add_state(lv.STATE.DISABLED)\n"
            "cb.align(lv.ALIGN.TOP_LEFT, 0 ,60)\n"
            "cb = lv.checkbox(lv.scr_act())\n"
            "cb.add_state(lv.STATE.CHECKED | lv.STATE.DISABLED)\n"
            "cb.set_text(\"Melon\")\n"
            "cb.align(lv.ALIGN.TOP_LEFT, 0 ,90)\n"
            "print('mem used max: %0.2f kB' % (mem.getMax()))\n"
            "print('mem used now: %0.2f kB' % (mem.getNow()))\n"
            "\n");
#else 
    obj_runModule(__pikaMain, "main");
#endif
    return __pikaMain;
}

