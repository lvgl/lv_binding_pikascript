/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#ifndef __PikaStdDevice_GPIO__H
#define __PikaStdDevice_GPIO__H
#include <stdio.h>
#include <stdlib.h>
#include "PikaObj.h"

PikaObj *New_PikaStdDevice_GPIO(Args *args);

void PikaStdDevice_GPIO___init__(PikaObj *self);
void PikaStdDevice_GPIO_disable(PikaObj *self);
void PikaStdDevice_GPIO_enable(PikaObj *self);
int PikaStdDevice_GPIO_getId(PikaObj *self);
char* PikaStdDevice_GPIO_getMode(PikaObj *self);
char* PikaStdDevice_GPIO_getPin(PikaObj *self);
void PikaStdDevice_GPIO_high(PikaObj *self);
void PikaStdDevice_GPIO_init(PikaObj *self);
void PikaStdDevice_GPIO_low(PikaObj *self);
void PikaStdDevice_GPIO_platformDisable(PikaObj *self);
void PikaStdDevice_GPIO_platformEnable(PikaObj *self);
void PikaStdDevice_GPIO_platformHigh(PikaObj *self);
void PikaStdDevice_GPIO_platformLow(PikaObj *self);
void PikaStdDevice_GPIO_platformRead(PikaObj *self);
void PikaStdDevice_GPIO_platformSetMode(PikaObj *self);
int PikaStdDevice_GPIO_read(PikaObj *self);
void PikaStdDevice_GPIO_setId(PikaObj *self, int id);
void PikaStdDevice_GPIO_setMode(PikaObj *self, char* mode);
void PikaStdDevice_GPIO_setPin(PikaObj *self, char* pinName);
void PikaStdDevice_GPIO_setPull(PikaObj *self, char* pull);

#endif
