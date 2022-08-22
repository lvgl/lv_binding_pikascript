/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "PikaStdDevice.h"
#include "TinyObj.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void PikaStdDevice_ADCMethod(PikaObj *self, Args *args){
    Arg* res = PikaStdDevice_ADC(self);
    method_returnArg(args, res);
}

void PikaStdDevice_BaseDevMethod(PikaObj *self, Args *args){
    Arg* res = PikaStdDevice_BaseDev(self);
    method_returnArg(args, res);
}

void PikaStdDevice_CANMethod(PikaObj *self, Args *args){
    Arg* res = PikaStdDevice_CAN(self);
    method_returnArg(args, res);
}

void PikaStdDevice_GPIOMethod(PikaObj *self, Args *args){
    Arg* res = PikaStdDevice_GPIO(self);
    method_returnArg(args, res);
}

void PikaStdDevice_IICMethod(PikaObj *self, Args *args){
    Arg* res = PikaStdDevice_IIC(self);
    method_returnArg(args, res);
}

void PikaStdDevice_PWMMethod(PikaObj *self, Args *args){
    Arg* res = PikaStdDevice_PWM(self);
    method_returnArg(args, res);
}

void PikaStdDevice_SPIMethod(PikaObj *self, Args *args){
    Arg* res = PikaStdDevice_SPI(self);
    method_returnArg(args, res);
}

void PikaStdDevice_TimeMethod(PikaObj *self, Args *args){
    Arg* res = PikaStdDevice_Time(self);
    method_returnArg(args, res);
}

void PikaStdDevice_UARTMethod(PikaObj *self, Args *args){
    Arg* res = PikaStdDevice_UART(self);
    method_returnArg(args, res);
}

PikaObj *New_PikaStdDevice(Args *args){
    PikaObj *self = New_TinyObj(args);
    class_defineConstructor(self, "ADC()->any", PikaStdDevice_ADCMethod);
    class_defineConstructor(self, "BaseDev()->any", PikaStdDevice_BaseDevMethod);
    class_defineConstructor(self, "CAN()->any", PikaStdDevice_CANMethod);
    class_defineConstructor(self, "GPIO()->any", PikaStdDevice_GPIOMethod);
    class_defineConstructor(self, "IIC()->any", PikaStdDevice_IICMethod);
    class_defineConstructor(self, "PWM()->any", PikaStdDevice_PWMMethod);
    class_defineConstructor(self, "SPI()->any", PikaStdDevice_SPIMethod);
    class_defineConstructor(self, "Time()->any", PikaStdDevice_TimeMethod);
    class_defineConstructor(self, "UART()->any", PikaStdDevice_UARTMethod);
    return self;
}

