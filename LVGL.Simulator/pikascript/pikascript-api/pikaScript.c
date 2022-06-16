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
            "style = lv.style_t()\n"
            "style.init()\n"
            "# Set a background color and a radius\n"
            "style.set_radius(5)\n"
            "style.set_bg_opa(lv.OPA.COVER)\n"
            "style.set_bg_color(lv.palette_lighten(lv.PALETTE.GREY, 1))\n"
            "# Add outline\n"
            "style.set_outline_width(2)\n"
            "style.set_outline_color(lv.palette_main(lv.PALETTE.BLUE))\n"
            "style.set_outline_pad(8)\n"
            "# Create an object with the new style\n"
            "obj = lv.obj(lv.scr_act())\n"
            "obj.add_style(style, 0)\n"
            "obj.center()\n"
            "print('mem used max: %0.2f kB' % (mem.getMax()))\n"
            "print('mem used now: %0.2f kB' % (mem.getNow()))\n"
            "\n");
#else 
    obj_runModule(__pikaMain, "main");
#endif
    return __pikaMain;
}

