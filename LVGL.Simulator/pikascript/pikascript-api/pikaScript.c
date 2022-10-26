/*
 * [Warning!] This file is auto-generated by pika compiler.
 * Do not edit it manually.
 * The source code is *.pyi file.
 * More details: 
 * English Doc:
 * https://pikadoc.readthedocs.io/en/latest/PikaScript%20%E6%A8%A1%E5%9D%97%E6%A6%82%E8%BF%B0.html
 * Chinese Doc:
 * https://pikadoc.readthedocs.io/zh/latest/PikaScript%20%E6%A8%A1%E5%9D%97%E6%A6%82%E8%BF%B0.html
 */

#include "PikaMain.h"
#include <stdio.h>
#include <stdlib.h>

volatile PikaObj *__pikaMain;
PikaObj *pikaScriptInit(void){
    __platform_printf("======[pikascript packages installed]======\r\n");
    pks_printVersion();
    __platform_printf("PikaStdLib==v1.11.5\r\n");
    __platform_printf("pika_lvgl==v0.4.0\r\n");
    __platform_printf("===========================================\r\n");
    PikaObj* pikaMain = newRootObj("pikaMain", New_PikaMain);
    __pikaMain = pikaMain;
    extern unsigned char pikaModules_py_a[];
    obj_linkLibrary(pikaMain, pikaModules_py_a);
#if PIKA_INIT_STRING_ENABLE
    obj_run(pikaMain,
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
    obj_runModule((PikaObj*)pikaMain, "main");
#endif
    return pikaMain;
}

