/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "PikaMain.h"
#include <stdio.h>
#include <stdlib.h>

PikaObj *__pikaMain;
PikaObj *pikaScriptInit(void){
    __platform_printf("======[pikascript packages installed]======\r\n");
    __platform_printf("PikaStdLib==v1.8.6\r\n");
    __platform_printf("pika_lvgl==latest\r\n");
    __platform_printf("pikascript-core==v1.8.6\r\n");
    __platform_printf("===========================================\r\n");
    __pikaMain = newRootObj("pikaMain", New_PikaMain);
    extern unsigned char pikaModules_py_a[];
    obj_linkLibrary(__pikaMain, pikaModules_py_a);
#if PIKA_INIT_STRING_ENABLE
    obj_run(__pikaMain,
            "import pika_lvgl as lv\n"
            "import PikaStdLib\n"
            "mem = PikaStdLib.MemChecker()\n"
            "# Create an Arc\n"
            "arc = lv.arc(lv.scr_act())\n"
            "arc.set_end_angle(200)\n"
            "arc.set_size(150, 150)\n"
            "arc.center()\n"
            "print('mem used max: %0.2f kB' % (mem.getMax()))\n"
            "print('mem used now: %0.2f kB' % (mem.getNow()))\n"
            "\n");
#else 
    obj_runModule(__pikaMain, "main");
#endif
    return __pikaMain;
}

