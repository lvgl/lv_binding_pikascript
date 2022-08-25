/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#ifndef __PikaStdDevice_PWM__H
#define __PikaStdDevice_PWM__H
#include <stdio.h>
#include <stdlib.h>
#include "PikaObj.h"

PikaObj *New_PikaStdDevice_PWM(Args *args);

void PikaStdDevice_PWM___init__(PikaObj *self);
void PikaStdDevice_PWM_disable(PikaObj *self);
void PikaStdDevice_PWM_enable(PikaObj *self);
int PikaStdDevice_PWM_getChannel(PikaObj *self);
double PikaStdDevice_PWM_getDuty(PikaObj *self);
int PikaStdDevice_PWM_getFrequency(PikaObj *self);
char* PikaStdDevice_PWM_getName(PikaObj *self);
void PikaStdDevice_PWM_platformDisable(PikaObj *self);
void PikaStdDevice_PWM_platformEnable(PikaObj *self);
void PikaStdDevice_PWM_platformSetDuty(PikaObj *self);
void PikaStdDevice_PWM_platformSetFrequency(PikaObj *self);
void PikaStdDevice_PWM_setChannel(PikaObj *self, int ch);
void PikaStdDevice_PWM_setDuty(PikaObj *self, double duty);
void PikaStdDevice_PWM_setFreq(PikaObj *self, int freq);
void PikaStdDevice_PWM_setFrequency(PikaObj *self, int freq);
void PikaStdDevice_PWM_setName(PikaObj *self, char* name);
void PikaStdDevice_PWM_setPin(PikaObj *self, char* pin);

#endif
