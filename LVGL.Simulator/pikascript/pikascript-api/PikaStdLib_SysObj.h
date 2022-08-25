/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#ifndef __PikaStdLib_SysObj__H
#define __PikaStdLib_SysObj__H
#include <stdio.h>
#include <stdlib.h>
#include "PikaObj.h"

PikaObj *New_PikaStdLib_SysObj(Args *args);

Arg* PikaStdLib_SysObj___getitem__(PikaObj *self, Arg* obj, Arg* key);
Arg* PikaStdLib_SysObj___setitem__(PikaObj *self, Arg* obj, Arg* key, Arg* val);
Arg* PikaStdLib_SysObj___slice__(PikaObj *self, Arg* obj, Arg* start, Arg* end, int step);
Arg* PikaStdLib_SysObj_bytes(PikaObj *self, Arg* val);
char* PikaStdLib_SysObj_cformat(PikaObj *self, char* fmt, PikaTuple* var);
char* PikaStdLib_SysObj_chr(PikaObj *self, int val);
Arg* PikaStdLib_SysObj_dict(PikaObj *self);
PikaObj* PikaStdLib_SysObj_dir(PikaObj *self, PikaObj* obj);
void PikaStdLib_SysObj_exec(PikaObj *self, char* code);
double PikaStdLib_SysObj_float(PikaObj *self, Arg* arg);
Arg* PikaStdLib_SysObj_getattr(PikaObj *self, PikaObj* obj, char* name);
char* PikaStdLib_SysObj_hex(PikaObj *self, int val);
int PikaStdLib_SysObj_id(PikaObj *self, Arg* obj);
int PikaStdLib_SysObj_int(PikaObj *self, Arg* arg);
Arg* PikaStdLib_SysObj_iter(PikaObj *self, Arg* arg);
int PikaStdLib_SysObj_len(PikaObj *self, Arg* arg);
Arg* PikaStdLib_SysObj_list(PikaObj *self);
PikaObj* PikaStdLib_SysObj_open(PikaObj *self, char* path, char* mode);
int PikaStdLib_SysObj_ord(PikaObj *self, char* val);
void PikaStdLib_SysObj_print(PikaObj *self, PikaTuple* val);
void PikaStdLib_SysObj_printNoEnd(PikaObj *self, Arg* val);
Arg* PikaStdLib_SysObj_range(PikaObj *self, int a1, int a2);
void PikaStdLib_SysObj_setattr(PikaObj *self, PikaObj* obj, char* name, Arg* val);
char* PikaStdLib_SysObj_str(PikaObj *self, Arg* arg);
Arg* PikaStdLib_SysObj_type(PikaObj *self, Arg* arg);

#endif
