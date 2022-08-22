/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "PikaStdDevice_SPI.h"
#include "PikaStdDevice_BaseDev.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void PikaStdDevice_SPI___init__Method(PikaObj *self, Args *args){
    PikaStdDevice_SPI___init__(self);
}

void PikaStdDevice_SPI_disableMethod(PikaObj *self, Args *args){
    PikaStdDevice_SPI_disable(self);
}

void PikaStdDevice_SPI_enableMethod(PikaObj *self, Args *args){
    PikaStdDevice_SPI_enable(self);
}

void PikaStdDevice_SPI_platformDisableMethod(PikaObj *self, Args *args){
    PikaStdDevice_SPI_platformDisable(self);
}

void PikaStdDevice_SPI_platformEnableMethod(PikaObj *self, Args *args){
    PikaStdDevice_SPI_platformEnable(self);
}

void PikaStdDevice_SPI_platformReadMethod(PikaObj *self, Args *args){
    PikaStdDevice_SPI_platformRead(self);
}

void PikaStdDevice_SPI_platformReadBytesMethod(PikaObj *self, Args *args){
    PikaStdDevice_SPI_platformReadBytes(self);
}

void PikaStdDevice_SPI_platformWriteMethod(PikaObj *self, Args *args){
    PikaStdDevice_SPI_platformWrite(self);
}

void PikaStdDevice_SPI_platformWriteBytesMethod(PikaObj *self, Args *args){
    PikaStdDevice_SPI_platformWriteBytes(self);
}

void PikaStdDevice_SPI_readMethod(PikaObj *self, Args *args){
    int length = args_getInt(args, "length");
    char* res = PikaStdDevice_SPI_read(self, length);
    method_returnStr(args, res);
}

void PikaStdDevice_SPI_readBytesMethod(PikaObj *self, Args *args){
    int length = args_getInt(args, "length");
    Arg* res = PikaStdDevice_SPI_readBytes(self, length);
    method_returnArg(args, res);
}

void PikaStdDevice_SPI_setBaudRateMethod(PikaObj *self, Args *args){
    int baudRate = args_getInt(args, "baudRate");
    PikaStdDevice_SPI_setBaudRate(self, baudRate);
}

void PikaStdDevice_SPI_setIdMethod(PikaObj *self, Args *args){
    int id = args_getInt(args, "id");
    PikaStdDevice_SPI_setId(self, id);
}

void PikaStdDevice_SPI_setNameMethod(PikaObj *self, Args *args){
    char* name = args_getStr(args, "name");
    PikaStdDevice_SPI_setName(self, name);
}

void PikaStdDevice_SPI_setPhaseMethod(PikaObj *self, Args *args){
    int phase = args_getInt(args, "phase");
    PikaStdDevice_SPI_setPhase(self, phase);
}

void PikaStdDevice_SPI_setPinMISOMethod(PikaObj *self, Args *args){
    char* pin = args_getStr(args, "pin");
    PikaStdDevice_SPI_setPinMISO(self, pin);
}

void PikaStdDevice_SPI_setPinMOSIMethod(PikaObj *self, Args *args){
    char* pin = args_getStr(args, "pin");
    PikaStdDevice_SPI_setPinMOSI(self, pin);
}

void PikaStdDevice_SPI_setPinSCKMethod(PikaObj *self, Args *args){
    char* pin = args_getStr(args, "pin");
    PikaStdDevice_SPI_setPinSCK(self, pin);
}

void PikaStdDevice_SPI_setPolarityMethod(PikaObj *self, Args *args){
    int polarity = args_getInt(args, "polarity");
    PikaStdDevice_SPI_setPolarity(self, polarity);
}

void PikaStdDevice_SPI_writeMethod(PikaObj *self, Args *args){
    char* data = args_getStr(args, "data");
    PikaStdDevice_SPI_write(self, data);
}

void PikaStdDevice_SPI_writeBytesMethod(PikaObj *self, Args *args){
    uint8_t* data = args_getBytes(args, "data");
    int length = args_getInt(args, "length");
    PikaStdDevice_SPI_writeBytes(self, data, length);
}

PikaObj *New_PikaStdDevice_SPI(Args *args){
    PikaObj *self = New_PikaStdDevice_BaseDev(args);
    class_defineMethod(self, "__init__()", PikaStdDevice_SPI___init__Method);
    class_defineMethod(self, "disable()", PikaStdDevice_SPI_disableMethod);
    class_defineMethod(self, "enable()", PikaStdDevice_SPI_enableMethod);
    class_defineMethod(self, "platformDisable()", PikaStdDevice_SPI_platformDisableMethod);
    class_defineMethod(self, "platformEnable()", PikaStdDevice_SPI_platformEnableMethod);
    class_defineMethod(self, "platformRead()", PikaStdDevice_SPI_platformReadMethod);
    class_defineMethod(self, "platformReadBytes()", PikaStdDevice_SPI_platformReadBytesMethod);
    class_defineMethod(self, "platformWrite()", PikaStdDevice_SPI_platformWriteMethod);
    class_defineMethod(self, "platformWriteBytes()", PikaStdDevice_SPI_platformWriteBytesMethod);
    class_defineMethod(self, "read(length:int)->str", PikaStdDevice_SPI_readMethod);
    class_defineMethod(self, "readBytes(length:int)->bytes", PikaStdDevice_SPI_readBytesMethod);
    class_defineMethod(self, "setBaudRate(baudRate:int)", PikaStdDevice_SPI_setBaudRateMethod);
    class_defineMethod(self, "setId(id:int)", PikaStdDevice_SPI_setIdMethod);
    class_defineMethod(self, "setName(name:str)", PikaStdDevice_SPI_setNameMethod);
    class_defineMethod(self, "setPhase(phase:int)", PikaStdDevice_SPI_setPhaseMethod);
    class_defineMethod(self, "setPinMISO(pin:str)", PikaStdDevice_SPI_setPinMISOMethod);
    class_defineMethod(self, "setPinMOSI(pin:str)", PikaStdDevice_SPI_setPinMOSIMethod);
    class_defineMethod(self, "setPinSCK(pin:str)", PikaStdDevice_SPI_setPinSCKMethod);
    class_defineMethod(self, "setPolarity(polarity:int)", PikaStdDevice_SPI_setPolarityMethod);
    class_defineMethod(self, "write(data:str)", PikaStdDevice_SPI_writeMethod);
    class_defineMethod(self, "writeBytes(data:bytes,length:int)", PikaStdDevice_SPI_writeBytesMethod);
    return self;
}

Arg *PikaStdDevice_SPI(PikaObj *self){
    return obj_newObjInPackage(New_PikaStdDevice_SPI);
}
