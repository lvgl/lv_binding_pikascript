/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "PikaStdLib_RangeObj.h"
#include "TinyObj.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void PikaStdLib_RangeObj___next__Method(PikaObj *self, Args *args){
    Arg* res = PikaStdLib_RangeObj___next__(self);
    method_returnArg(args, res);
}

PikaObj *New_PikaStdLib_RangeObj(Args *args){
    PikaObj *self = New_TinyObj(args);
    class_defineMethod(self, "__next__()->any", PikaStdLib_RangeObj___next__Method);
    return self;
}

Arg *PikaStdLib_RangeObj(PikaObj *self){
    return obj_newObjInPackage(New_PikaStdLib_RangeObj);
}
