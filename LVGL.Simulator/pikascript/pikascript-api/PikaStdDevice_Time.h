/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#ifndef __PikaStdDevice_Time__H
#define __PikaStdDevice_Time__H
#include <stdio.h>
#include <stdlib.h>
#include "PikaObj.h"

PikaObj *New_PikaStdDevice_Time(Args *args);

void PikaStdDevice_Time___init__(PikaObj *self);
void PikaStdDevice_Time_asctime(PikaObj *self);
void PikaStdDevice_Time_ctime(PikaObj *self, double unix_time);
void PikaStdDevice_Time_gmtime(PikaObj *self, double unix_time);
void PikaStdDevice_Time_localtime(PikaObj *self, double unix_time);
int PikaStdDevice_Time_mktime(PikaObj *self);
void PikaStdDevice_Time_platformGetTick(PikaObj *self);
void PikaStdDevice_Time_sleep(PikaObj *self, double s);
void PikaStdDevice_Time_sleep_ms(PikaObj *self, int ms);
void PikaStdDevice_Time_sleep_s(PikaObj *self, int s);
double PikaStdDevice_Time_time(PikaObj *self);
int PikaStdDevice_Time_time_ns(PikaObj *self);

#endif
