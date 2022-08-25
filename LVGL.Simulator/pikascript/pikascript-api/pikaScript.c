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
            "class ArcLoader():\n"
            "    def __init__(self):\n"
            "        self.a = 270\n"
            "    def arc_loader_cb(self,tim,arc):\n"
            "        # print(tim,arc)\n"
            "        self.a += 5\n"
            "        arc.set_end_angle(self.a)\n"
            "        if self.a >= 270 + 360:\n"
            "            tim._del()\n"
            "            mem.max()\n"
            "#\n"
            "# Create an arc which acts as a loader.\n"
            "#\n"
            "# Create an Arc\n"
            "arc = lv.arc(lv.scr_act())\n"
            "arc.set_bg_angles(0, 360)\n"
            "arc.set_angles(270, 270)\n"
            "arc.center()\n"
            "# create the loader\n"
            "arc_loader = ArcLoader()\n"
            "# Create an `lv_timer` to update the arc.\n"
            "timer = lv.timer_create_basic()\n"
            "timer.set_period(20)\n"
            "def cb(src):\n"
            "    arc_loader.arc_loader_cb(src,arc)\n"
            "timer.set_cb(cb)\n"
            "\n");
#else 
    obj_runModule(__pikaMain, "main");
#endif
    return __pikaMain;
}

