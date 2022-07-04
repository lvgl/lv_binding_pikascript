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
            "import PikaStdLib\n"
            "mem = PikaStdLib.MemChecker()\n"
            "label1 = lv.label(lv.scr_act())\n"
            "LV_LABEL_LONG_WRAP = 0\n"
            "label1.set_long_mode(LV_LABEL_LONG_WRAP)      # Break the long lines*/\n"
            "# Enable re-coloring by commands in the text\n"
            "label1.set_recolor(True)\n"
            "label1.set_text(\"#0000ff Re-color# #ff00ff words# #ff0000 of a# label, \\\n"
            "align the lines to the center and  wrap long text automatically.\")\n"
            "# Set smaller width to make the lines wrap\n"
            "label1.set_width(150)\n"
            "label1.set_style_text_align(lv.ALIGN.CENTER, 0)\n"
            "label1.align(lv.ALIGN.CENTER, 0, -40)\n"
            "LV_LABEL_LONG_SCROLL_CIRCULAR = 3\n"
            "label2 = lv.label(lv.scr_act())\n"
            "label2.set_long_mode(LV_LABEL_LONG_SCROLL_CIRCULAR)  # Circular scroll\n"
            "label2.set_width(150)\n"
            "label2.set_text(\"It is a circularly scrolling text. \")\n"
            "label2.align(lv.ALIGN.CENTER, 0, 40)\n"
            "print('mem used max: %0.2f kB' % (mem.getMax()))\n"
            "print('mem used now: %0.2f kB' % (mem.getNow()))\n"
            "\n");
#else 
    obj_runModule(__pikaMain, "main");
#endif
    return __pikaMain;
}

