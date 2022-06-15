import pika_lvgl as lv
import PikaStdLib
import pika_lvgl

print('hello PikaScript!')

mem = PikaStdLib.MemChecker()
print('mem used max:')
mem.max()

# Create an Arc
cb = lv.checkbox(lv.scr_act())
cb.set_text("Apple")

cb = lv.checkbox(lv.scr_act())
cb.set_text("Banana")

cb = lv.checkbox(lv.scr_act())
cb.set_text("Lemon")

cb = lv.checkbox(lv.scr_act())
cb.set_text("Melon")

cb.update_layout()

mem.now()
