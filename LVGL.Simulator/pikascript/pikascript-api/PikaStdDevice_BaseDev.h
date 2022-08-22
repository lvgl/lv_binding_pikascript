/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#ifndef __PikaStdDevice_BaseDev__H
#define __PikaStdDevice_BaseDev__H
#include <stdio.h>
#include <stdlib.h>
#include "PikaObj.h"

PikaObj *New_PikaStdDevice_BaseDev(Args *args);

void PikaStdDevice_BaseDev_addEventCallBack(PikaObj *self, Arg* eventCallback);
void PikaStdDevice_BaseDev_platformGetEventId(PikaObj *self);

#endif
