/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "PikaStdDevice_UART.h"
#include "PikaStdDevice_BaseDev.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void PikaStdDevice_UART___init__Method(PikaObj *self, Args *args){
    PikaStdDevice_UART___init__(self);
}

void PikaStdDevice_UART_disableMethod(PikaObj *self, Args *args){
    PikaStdDevice_UART_disable(self);
}

void PikaStdDevice_UART_enableMethod(PikaObj *self, Args *args){
    PikaStdDevice_UART_enable(self);
}

void PikaStdDevice_UART_initMethod(PikaObj *self, Args *args){
    PikaStdDevice_UART_init(self);
}

void PikaStdDevice_UART_platformDisableMethod(PikaObj *self, Args *args){
    PikaStdDevice_UART_platformDisable(self);
}

void PikaStdDevice_UART_platformEnableMethod(PikaObj *self, Args *args){
    PikaStdDevice_UART_platformEnable(self);
}

void PikaStdDevice_UART_platformReadMethod(PikaObj *self, Args *args){
    PikaStdDevice_UART_platformRead(self);
}

void PikaStdDevice_UART_platformReadBytesMethod(PikaObj *self, Args *args){
    PikaStdDevice_UART_platformReadBytes(self);
}

void PikaStdDevice_UART_platformWriteMethod(PikaObj *self, Args *args){
    PikaStdDevice_UART_platformWrite(self);
}

void PikaStdDevice_UART_platformWriteBytesMethod(PikaObj *self, Args *args){
    PikaStdDevice_UART_platformWriteBytes(self);
}

void PikaStdDevice_UART_readMethod(PikaObj *self, Args *args){
    int length = args_getInt(args, "length");
    char* res = PikaStdDevice_UART_read(self, length);
    method_returnStr(args, res);
}

void PikaStdDevice_UART_readBytesMethod(PikaObj *self, Args *args){
    int length = args_getInt(args, "length");
    Arg* res = PikaStdDevice_UART_readBytes(self, length);
    method_returnArg(args, res);
}

void PikaStdDevice_UART_setBaudRateMethod(PikaObj *self, Args *args){
    int baudRate = args_getInt(args, "baudRate");
    PikaStdDevice_UART_setBaudRate(self, baudRate);
}

void PikaStdDevice_UART_setIdMethod(PikaObj *self, Args *args){
    int id = args_getInt(args, "id");
    PikaStdDevice_UART_setId(self, id);
}

void PikaStdDevice_UART_writeMethod(PikaObj *self, Args *args){
    char* data = args_getStr(args, "data");
    PikaStdDevice_UART_write(self, data);
}

void PikaStdDevice_UART_writeBytesMethod(PikaObj *self, Args *args){
    uint8_t* data = args_getBytes(args, "data");
    int length = args_getInt(args, "length");
    PikaStdDevice_UART_writeBytes(self, data, length);
}

PikaObj *New_PikaStdDevice_UART(Args *args){
    PikaObj *self = New_PikaStdDevice_BaseDev(args);
    class_defineMethod(self, "__init__()", PikaStdDevice_UART___init__Method);
    class_defineMethod(self, "disable()", PikaStdDevice_UART_disableMethod);
    class_defineMethod(self, "enable()", PikaStdDevice_UART_enableMethod);
    class_defineMethod(self, "init()", PikaStdDevice_UART_initMethod);
    class_defineMethod(self, "platformDisable()", PikaStdDevice_UART_platformDisableMethod);
    class_defineMethod(self, "platformEnable()", PikaStdDevice_UART_platformEnableMethod);
    class_defineMethod(self, "platformRead()", PikaStdDevice_UART_platformReadMethod);
    class_defineMethod(self, "platformReadBytes()", PikaStdDevice_UART_platformReadBytesMethod);
    class_defineMethod(self, "platformWrite()", PikaStdDevice_UART_platformWriteMethod);
    class_defineMethod(self, "platformWriteBytes()", PikaStdDevice_UART_platformWriteBytesMethod);
    class_defineMethod(self, "read(length:int)->str", PikaStdDevice_UART_readMethod);
    class_defineMethod(self, "readBytes(length:int)->bytes", PikaStdDevice_UART_readBytesMethod);
    class_defineMethod(self, "setBaudRate(baudRate:int)", PikaStdDevice_UART_setBaudRateMethod);
    class_defineMethod(self, "setId(id:int)", PikaStdDevice_UART_setIdMethod);
    class_defineMethod(self, "write(data:str)", PikaStdDevice_UART_writeMethod);
    class_defineMethod(self, "writeBytes(data:bytes,length:int)", PikaStdDevice_UART_writeBytesMethod);
    return self;
}

Arg *PikaStdDevice_UART(PikaObj *self){
    return obj_newObjInPackage(New_PikaStdDevice_UART);
}
