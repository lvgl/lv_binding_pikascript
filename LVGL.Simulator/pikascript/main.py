import pika_lvgl as lv
import PikaStdLib

print('hello PikaScript!')

mem = PikaStdLib.MemChecker()
print('mem used max:')
mem.max()

ta = lv.textarea(lv.scr_act())
ta.set_one_line(True)
ta.align(9, 0, 10)

mem.now()
