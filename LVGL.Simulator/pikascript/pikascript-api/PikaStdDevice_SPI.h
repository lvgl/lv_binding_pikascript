/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#ifndef __PikaStdDevice_SPI__H
#define __PikaStdDevice_SPI__H
#include <stdio.h>
#include <stdlib.h>
#include "PikaObj.h"

PikaObj *New_PikaStdDevice_SPI(Args *args);

void PikaStdDevice_SPI___init__(PikaObj *self);
void PikaStdDevice_SPI_disable(PikaObj *self);
void PikaStdDevice_SPI_enable(PikaObj *self);
void PikaStdDevice_SPI_platformDisable(PikaObj *self);
void PikaStdDevice_SPI_platformEnable(PikaObj *self);
void PikaStdDevice_SPI_platformRead(PikaObj *self);
void PikaStdDevice_SPI_platformReadBytes(PikaObj *self);
void PikaStdDevice_SPI_platformWrite(PikaObj *self);
void PikaStdDevice_SPI_platformWriteBytes(PikaObj *self);
char* PikaStdDevice_SPI_read(PikaObj *self, int length);
Arg* PikaStdDevice_SPI_readBytes(PikaObj *self, int length);
void PikaStdDevice_SPI_setBaudRate(PikaObj *self, int baudRate);
void PikaStdDevice_SPI_setId(PikaObj *self, int id);
void PikaStdDevice_SPI_setName(PikaObj *self, char* name);
void PikaStdDevice_SPI_setPhase(PikaObj *self, int phase);
void PikaStdDevice_SPI_setPinMISO(PikaObj *self, char* pin);
void PikaStdDevice_SPI_setPinMOSI(PikaObj *self, char* pin);
void PikaStdDevice_SPI_setPinSCK(PikaObj *self, char* pin);
void PikaStdDevice_SPI_setPolarity(PikaObj *self, int polarity);
void PikaStdDevice_SPI_write(PikaObj *self, char* data);
void PikaStdDevice_SPI_writeBytes(PikaObj *self, uint8_t* data, int length);

#endif
