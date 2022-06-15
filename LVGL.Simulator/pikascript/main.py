import pika_lvgl as lv
import PikaStdLib
import pika_lvgl

print('hello PikaScript!')

mem = PikaStdLib.MemChecker()
print('mem used max:')
mem.max()

roller1 = lv.roller(lv.scr_act())
roller1.set_options("January\nFebruary\nMarch\nApril\nMay\nJune\nJuly", 0)
roller1.set_visible_row_count(4)
roller1.center()

mem.now()
