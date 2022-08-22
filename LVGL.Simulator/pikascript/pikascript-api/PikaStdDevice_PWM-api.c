/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "PikaStdDevice_PWM.h"
#include "PikaStdDevice_BaseDev.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void PikaStdDevice_PWM___init__Method(PikaObj *self, Args *args){
    PikaStdDevice_PWM___init__(self);
}

void PikaStdDevice_PWM_disableMethod(PikaObj *self, Args *args){
    PikaStdDevice_PWM_disable(self);
}

void PikaStdDevice_PWM_enableMethod(PikaObj *self, Args *args){
    PikaStdDevice_PWM_enable(self);
}

void PikaStdDevice_PWM_getChannelMethod(PikaObj *self, Args *args){
    int res = PikaStdDevice_PWM_getChannel(self);
    method_returnInt(args, res);
}

void PikaStdDevice_PWM_getDutyMethod(PikaObj *self, Args *args){
    double res = PikaStdDevice_PWM_getDuty(self);
    method_returnFloat(args, res);
}

void PikaStdDevice_PWM_getFrequencyMethod(PikaObj *self, Args *args){
    int res = PikaStdDevice_PWM_getFrequency(self);
    method_returnInt(args, res);
}

void PikaStdDevice_PWM_getNameMethod(PikaObj *self, Args *args){
    char* res = PikaStdDevice_PWM_getName(self);
    method_returnStr(args, res);
}

void PikaStdDevice_PWM_initMethod(PikaObj *self, Args *args){
    PikaStdDevice_PWM_init(self);
}

void PikaStdDevice_PWM_platformDisableMethod(PikaObj *self, Args *args){
    PikaStdDevice_PWM_platformDisable(self);
}

void PikaStdDevice_PWM_platformEnableMethod(PikaObj *self, Args *args){
    PikaStdDevice_PWM_platformEnable(self);
}

void PikaStdDevice_PWM_platformSetDutyMethod(PikaObj *self, Args *args){
    PikaStdDevice_PWM_platformSetDuty(self);
}

void PikaStdDevice_PWM_platformSetFrequencyMethod(PikaObj *self, Args *args){
    PikaStdDevice_PWM_platformSetFrequency(self);
}

void PikaStdDevice_PWM_setChannelMethod(PikaObj *self, Args *args){
    int ch = args_getInt(args, "ch");
    PikaStdDevice_PWM_setChannel(self, ch);
}

void PikaStdDevice_PWM_setDutyMethod(PikaObj *self, Args *args){
    double duty = args_getFloat(args, "duty");
    PikaStdDevice_PWM_setDuty(self, duty);
}

void PikaStdDevice_PWM_setFreqMethod(PikaObj *self, Args *args){
    int freq = args_getInt(args, "freq");
    PikaStdDevice_PWM_setFreq(self, freq);
}

void PikaStdDevice_PWM_setFrequencyMethod(PikaObj *self, Args *args){
    int freq = args_getInt(args, "freq");
    PikaStdDevice_PWM_setFrequency(self, freq);
}

void PikaStdDevice_PWM_setNameMethod(PikaObj *self, Args *args){
    char* name = args_getStr(args, "name");
    PikaStdDevice_PWM_setName(self, name);
}

void PikaStdDevice_PWM_setPinMethod(PikaObj *self, Args *args){
    char* pin = args_getStr(args, "pin");
    PikaStdDevice_PWM_setPin(self, pin);
}

PikaObj *New_PikaStdDevice_PWM(Args *args){
    PikaObj *self = New_PikaStdDevice_BaseDev(args);
    class_defineMethod(self, "__init__()", PikaStdDevice_PWM___init__Method);
    class_defineMethod(self, "disable()", PikaStdDevice_PWM_disableMethod);
    class_defineMethod(self, "enable()", PikaStdDevice_PWM_enableMethod);
    class_defineMethod(self, "getChannel()->int", PikaStdDevice_PWM_getChannelMethod);
    class_defineMethod(self, "getDuty()->float", PikaStdDevice_PWM_getDutyMethod);
    class_defineMethod(self, "getFrequency()->int", PikaStdDevice_PWM_getFrequencyMethod);
    class_defineMethod(self, "getName()->str", PikaStdDevice_PWM_getNameMethod);
    class_defineMethod(self, "init()", PikaStdDevice_PWM_initMethod);
    class_defineMethod(self, "platformDisable()", PikaStdDevice_PWM_platformDisableMethod);
    class_defineMethod(self, "platformEnable()", PikaStdDevice_PWM_platformEnableMethod);
    class_defineMethod(self, "platformSetDuty()", PikaStdDevice_PWM_platformSetDutyMethod);
    class_defineMethod(self, "platformSetFrequency()", PikaStdDevice_PWM_platformSetFrequencyMethod);
    class_defineMethod(self, "setChannel(ch:int)", PikaStdDevice_PWM_setChannelMethod);
    class_defineMethod(self, "setDuty(duty:float)", PikaStdDevice_PWM_setDutyMethod);
    class_defineMethod(self, "setFreq(freq:int)", PikaStdDevice_PWM_setFreqMethod);
    class_defineMethod(self, "setFrequency(freq:int)", PikaStdDevice_PWM_setFrequencyMethod);
    class_defineMethod(self, "setName(name:str)", PikaStdDevice_PWM_setNameMethod);
    class_defineMethod(self, "setPin(pin:str)", PikaStdDevice_PWM_setPinMethod);
    return self;
}

Arg *PikaStdDevice_PWM(PikaObj *self){
    return obj_newObjInPackage(New_PikaStdDevice_PWM);
}
