import pika_lvgl as lv
import PikaStdLib
import pika_lvgl

print('hello PikaScript!')

mem = PikaStdLib.MemChecker()
print('mem used max:')
mem.max()

dd = lv.dropdown(lv.scr_act())
dd.set_options("Apple\nBanana\nOrange\nCherry")
dd.center()

mem.now()
