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
            "print('hello PikaScript!')\n"
            "mem = PikaStdLib.MemChecker()\n"
            "print('mem used max:')\n"
            "mem.max()\n"
            "ta = lv.textarea(lv.scr_act())\n"
            "ta.set_one_line(True)\n"
            "ta.align(lv.ALIGN.BOTTOM_RIGHT, 10, 0)\n"
            "mem.now()\n"
            "\n");
#else 
    obj_runModule(__pikaMain, "main");
#endif
    return __pikaMain;
}

