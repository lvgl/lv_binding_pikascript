/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "PikaStdDevice_BaseDev.h"
#include "TinyObj.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void PikaStdDevice_BaseDev_addEventCallBackMethod(PikaObj *self, Args *args){
    Arg* eventCallback = args_getArg(args, "eventCallback");
    PikaStdDevice_BaseDev_addEventCallBack(self, eventCallback);
}

void PikaStdDevice_BaseDev_platformGetEventIdMethod(PikaObj *self, Args *args){
    PikaStdDevice_BaseDev_platformGetEventId(self);
}

PikaObj *New_PikaStdDevice_BaseDev(Args *args){
    PikaObj *self = New_TinyObj(args);
    class_defineMethod(self, "addEventCallBack(eventCallback:any)", PikaStdDevice_BaseDev_addEventCallBackMethod);
    class_defineMethod(self, "platformGetEventId()", PikaStdDevice_BaseDev_platformGetEventIdMethod);
    return self;
}

Arg *PikaStdDevice_BaseDev(PikaObj *self){
    return obj_newObjInPackage(New_PikaStdDevice_BaseDev);
}
