/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "PikaStdDevice_Time.h"
#include "PikaStdDevice_BaseDev.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void PikaStdDevice_Time___init__Method(PikaObj *self, Args *args){
    PikaStdDevice_Time___init__(self);
}

void PikaStdDevice_Time_asctimeMethod(PikaObj *self, Args *args){
    PikaStdDevice_Time_asctime(self);
}

void PikaStdDevice_Time_ctimeMethod(PikaObj *self, Args *args){
    double unix_time = args_getFloat(args, "unix_time");
    PikaStdDevice_Time_ctime(self, unix_time);
}

void PikaStdDevice_Time_gmtimeMethod(PikaObj *self, Args *args){
    double unix_time = args_getFloat(args, "unix_time");
    PikaStdDevice_Time_gmtime(self, unix_time);
}

void PikaStdDevice_Time_localtimeMethod(PikaObj *self, Args *args){
    double unix_time = args_getFloat(args, "unix_time");
    PikaStdDevice_Time_localtime(self, unix_time);
}

void PikaStdDevice_Time_mktimeMethod(PikaObj *self, Args *args){
    int res = PikaStdDevice_Time_mktime(self);
    method_returnInt(args, res);
}

void PikaStdDevice_Time_platformGetTickMethod(PikaObj *self, Args *args){
    PikaStdDevice_Time_platformGetTick(self);
}

void PikaStdDevice_Time_sleepMethod(PikaObj *self, Args *args){
    double s = args_getFloat(args, "s");
    PikaStdDevice_Time_sleep(self, s);
}

void PikaStdDevice_Time_sleep_msMethod(PikaObj *self, Args *args){
    int ms = args_getInt(args, "ms");
    PikaStdDevice_Time_sleep_ms(self, ms);
}

void PikaStdDevice_Time_sleep_sMethod(PikaObj *self, Args *args){
    int s = args_getInt(args, "s");
    PikaStdDevice_Time_sleep_s(self, s);
}

void PikaStdDevice_Time_timeMethod(PikaObj *self, Args *args){
    double res = PikaStdDevice_Time_time(self);
    method_returnFloat(args, res);
}

void PikaStdDevice_Time_time_nsMethod(PikaObj *self, Args *args){
    int res = PikaStdDevice_Time_time_ns(self);
    method_returnInt(args, res);
}

PikaObj *New_PikaStdDevice_Time(Args *args){
    PikaObj *self = New_PikaStdDevice_BaseDev(args);
    class_defineMethod(self, "__init__()", PikaStdDevice_Time___init__Method);
    class_defineMethod(self, "asctime()", PikaStdDevice_Time_asctimeMethod);
    class_defineMethod(self, "ctime(unix_time:float)", PikaStdDevice_Time_ctimeMethod);
    class_defineMethod(self, "gmtime(unix_time:float)", PikaStdDevice_Time_gmtimeMethod);
    class_defineMethod(self, "localtime(unix_time:float)", PikaStdDevice_Time_localtimeMethod);
    class_defineMethod(self, "mktime()->int", PikaStdDevice_Time_mktimeMethod);
    class_defineMethod(self, "platformGetTick()", PikaStdDevice_Time_platformGetTickMethod);
    class_defineMethod(self, "sleep(s:float)", PikaStdDevice_Time_sleepMethod);
    class_defineMethod(self, "sleep_ms(ms:int)", PikaStdDevice_Time_sleep_msMethod);
    class_defineMethod(self, "sleep_s(s:int)", PikaStdDevice_Time_sleep_sMethod);
    class_defineMethod(self, "time()->float", PikaStdDevice_Time_timeMethod);
    class_defineMethod(self, "time_ns()->int", PikaStdDevice_Time_time_nsMethod);
    return self;
}

Arg *PikaStdDevice_Time(PikaObj *self){
    return obj_newObjInPackage(New_PikaStdDevice_Time);
}
