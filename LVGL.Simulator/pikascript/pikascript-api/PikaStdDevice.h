/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#ifndef __PikaStdDevice__H
#define __PikaStdDevice__H
#include <stdio.h>
#include <stdlib.h>
#include "PikaObj.h"

PikaObj *New_PikaStdDevice(Args *args);

Arg* PikaStdDevice_ADC(PikaObj *self);
Arg* PikaStdDevice_BaseDev(PikaObj *self);
Arg* PikaStdDevice_CAN(PikaObj *self);
Arg* PikaStdDevice_GPIO(PikaObj *self);
Arg* PikaStdDevice_IIC(PikaObj *self);
Arg* PikaStdDevice_PWM(PikaObj *self);
Arg* PikaStdDevice_SPI(PikaObj *self);
Arg* PikaStdDevice_Time(PikaObj *self);
Arg* PikaStdDevice_UART(PikaObj *self);

#endif
