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
    __platform_printf("PikaStdDevice==v1.10.3\r\n");
    __platform_printf("PikaStdLib==v1.10.3\r\n");
    __platform_printf("===========================================\r\n");
    __pikaMain = newRootObj("pikaMain", New_PikaMain);
    extern unsigned char pikaModules_py_a[];
    obj_linkLibrary(__pikaMain, pikaModules_py_a);
#if PIKA_INIT_STRING_ENABLE
    obj_run(__pikaMain,
            "import pika_lvgl as lv\n"
            "import PikaStdLib\n"
            "mem = PikaStdLib.MemChecker()\n"
            "def event_cb_1(evt):\n"
            "    print('in evt1')\n"
            "    print('mem used now: %0.2f kB' % (mem.getNow()))\n"
            "def event_cb_2(evt):\n"
            "    print('in evt2')\n"
            "    print('mem used now: %0.2f kB' % (mem.getNow()))\n"
            "btn1 = lv.btn(lv.scr_act())\n"
            "btn1.align(lv.ALIGN.TOP_MID, 0, 10)\n"
            "btn2 = lv.btn(lv.scr_act())\n"
            "btn2.align(lv.ALIGN.TOP_MID, 0, 50)\n"
            "btn1.add_event_cb(event_cb_1, lv.EVENT.CLICKED, 0)\n"
            "btn2.add_event_cb(event_cb_2, lv.EVENT.CLICKED, 0)\n"
            "print('mem used max: %0.2f kB' % (mem.getMax()))\n"
            "print('mem used now: %0.2f kB' % (mem.getNow()))\n"
            "\n");
#else 
    obj_runModule(__pikaMain, "main");
#endif
    return __pikaMain;
}

