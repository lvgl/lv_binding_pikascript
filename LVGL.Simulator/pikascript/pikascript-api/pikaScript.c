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
            "table = lv.table(lv.scr_act())\n"
            "# Fill the first column\n"
            "table.set_cell_value(0, 0, \"Name\")\n"
            "table.set_cell_value(1, 0, \"Apple\")\n"
            "table.set_cell_value(2, 0, \"Banana\")\n"
            "table.set_cell_value(3, 0, \"Lemon\")\n"
            "table.set_cell_value(4, 0, \"Grape\")\n"
            "table.set_cell_value(5, 0, \"Melon\")\n"
            "table.set_cell_value(6, 0, \"Peach\")\n"
            "table.set_cell_value(7, 0, \"Nuts\")\n"
            "# Fill the second column\n"
            "table.set_cell_value(0, 1, \"Price\")\n"
            "table.set_cell_value(1, 1, \"$7\")\n"
            "table.set_cell_value(2, 1, \"$4\")\n"
            "table.set_cell_value(3, 1, \"$6\")\n"
            "table.set_cell_value(4, 1, \"$2\")\n"
            "table.set_cell_value(5, 1, \"$5\")\n"
            "table.set_cell_value(6, 1, \"$1\")\n"
            "table.set_cell_value(7, 1, \"$9\")\n"
            "table.set_hight(200)\n"
            "table.center()\n"
            "mem.now()\n"
            "\n");
#else 
    obj_runModule(__pikaMain, "main");
#endif
    return __pikaMain;
}

