import PikaStdLib
import pika_lvgl

print('hello PikaScript!')

mem = PikaStdLib.MemChecker()
print('mem used max:')
mem.max()

import pika_lvgl as lv
# Create an Arc
arc = lv.arc(lv.scr_act())
arc.set_end_angle(200)
arc.set_size(150, 150)
arc.center()

print('lvgl code finished')
print('mem used now')
mem.now()
