#include "PikaStdDevice_ADC.h"
#include "BaseObj.h"

void PikaStdDevice_ADC_enable(PikaObj* self) {
    obj_runNativeMethod(self, "platformEnable", NULL);
}

void PikaStdDevice_ADC_disable(PikaObj* self) {
    obj_runNativeMethod(self, "platformDisable", NULL);
}

void PikaStdDevice_ADC_init(PikaObj* self) {
    obj_setStr(self, "pin", "PA0");
}

void PikaStdDevice_ADC___init__(PikaObj* self) {
    PikaStdDevice_ADC_init(self);
}

double PikaStdDevice_ADC_read(PikaObj* self) {
    obj_runNativeMethod(self, "platformRead", NULL);
    return obj_getFloat(self, "val");
}

void PikaStdDevice_ADC_setPin(PikaObj* self, char* pin) {
    obj_setStr(self, "pin", pin);
}

void PikaStdDevice_ADC_platformEnable(PikaObj* self) {
    obj_setErrorCode(self, 1);
    obj_setSysOut(self, "[error] platform method need to be override.");
}

void PikaStdDevice_ADC_platformDisable(PikaObj* self) {
    obj_setErrorCode(self, 1);
    obj_setSysOut(self, "[error] platform method need to be override.");
}

void PikaStdDevice_ADC_platformRead(PikaObj* self) {
    obj_setErrorCode(self, 1);
    obj_setSysOut(self, "[error] platform method need to be override.");
}
