import pika_lvgl as lv
import PikaStdLib

print('hello PikaScript!')

mem = PikaStdLib.MemChecker()
print('mem used max:')
mem.max()

ta = lv.textarea(lv.scr_act())
ta.set_one_line(True)
ta.align(lv.ALIGN.BOTTOM_RIGHT, 10, 0)

mem.now()
