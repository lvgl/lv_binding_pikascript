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
            "# create a simple button\n"
            "btn1 = lv.btn(lv.scr_act())\n"
            "btn1.align(lv.ALIGN.CENTER,0,-40)\n"
            "label=lv.label(btn1)\n"
            "label.set_text(\"Button\")\n"
            "# create a toggle button\n"
            "btn2 = lv.btn(lv.scr_act())\n"
            "btn2.align(lv.ALIGN.CENTER,0,40)\n"
            "btn2.add_flag(lv.obj.FLAG.CHECKABLE)\n"
            "btn2.set_height(lv.SIZE.CONTENT)\n"
            "label=lv.label(btn2)\n"
            "label.set_text(\"Toggle\")\n"
            "label.center()\n"
            "print('mem used max: %0.2f kB' % (mem.getMax()))\n"
            "print('mem used now: %0.2f kB' % (mem.getNow()))\n"
            "\n");
#else 
    obj_runModule(__pikaMain, "main");
#endif
    return __pikaMain;
}

