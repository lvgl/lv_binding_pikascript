/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "PikaStdDevice_ADC.h"
#include "PikaStdDevice_BaseDev.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void PikaStdDevice_ADC___init__Method(PikaObj *self, Args *args){
    PikaStdDevice_ADC___init__(self);
}

void PikaStdDevice_ADC_disableMethod(PikaObj *self, Args *args){
    PikaStdDevice_ADC_disable(self);
}

void PikaStdDevice_ADC_enableMethod(PikaObj *self, Args *args){
    PikaStdDevice_ADC_enable(self);
}

void PikaStdDevice_ADC_initMethod(PikaObj *self, Args *args){
    PikaStdDevice_ADC_init(self);
}

void PikaStdDevice_ADC_platformDisableMethod(PikaObj *self, Args *args){
    PikaStdDevice_ADC_platformDisable(self);
}

void PikaStdDevice_ADC_platformEnableMethod(PikaObj *self, Args *args){
    PikaStdDevice_ADC_platformEnable(self);
}

void PikaStdDevice_ADC_platformReadMethod(PikaObj *self, Args *args){
    PikaStdDevice_ADC_platformRead(self);
}

void PikaStdDevice_ADC_readMethod(PikaObj *self, Args *args){
    double res = PikaStdDevice_ADC_read(self);
    method_returnFloat(args, res);
}

void PikaStdDevice_ADC_setPinMethod(PikaObj *self, Args *args){
    char* pin = args_getStr(args, "pin");
    PikaStdDevice_ADC_setPin(self, pin);
}

PikaObj *New_PikaStdDevice_ADC(Args *args){
    PikaObj *self = New_PikaStdDevice_BaseDev(args);
    class_defineMethod(self, "__init__()", PikaStdDevice_ADC___init__Method);
    class_defineMethod(self, "disable()", PikaStdDevice_ADC_disableMethod);
    class_defineMethod(self, "enable()", PikaStdDevice_ADC_enableMethod);
    class_defineMethod(self, "init()", PikaStdDevice_ADC_initMethod);
    class_defineMethod(self, "platformDisable()", PikaStdDevice_ADC_platformDisableMethod);
    class_defineMethod(self, "platformEnable()", PikaStdDevice_ADC_platformEnableMethod);
    class_defineMethod(self, "platformRead()", PikaStdDevice_ADC_platformReadMethod);
    class_defineMethod(self, "read()->float", PikaStdDevice_ADC_readMethod);
    class_defineMethod(self, "setPin(pin:str)", PikaStdDevice_ADC_setPinMethod);
    return self;
}

Arg *PikaStdDevice_ADC(PikaObj *self){
    return obj_newObjInPackage(New_PikaStdDevice_ADC);
}
