/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#ifndef __PikaStdDevice_ADC__H
#define __PikaStdDevice_ADC__H
#include <stdio.h>
#include <stdlib.h>
#include "PikaObj.h"

PikaObj *New_PikaStdDevice_ADC(Args *args);

void PikaStdDevice_ADC___init__(PikaObj *self);
void PikaStdDevice_ADC_disable(PikaObj *self);
void PikaStdDevice_ADC_enable(PikaObj *self);
void PikaStdDevice_ADC_init(PikaObj *self);
void PikaStdDevice_ADC_platformDisable(PikaObj *self);
void PikaStdDevice_ADC_platformEnable(PikaObj *self);
void PikaStdDevice_ADC_platformRead(PikaObj *self);
double PikaStdDevice_ADC_read(PikaObj *self);
void PikaStdDevice_ADC_setPin(PikaObj *self, char* pin);

#endif
