/* ******************************** */
/* Warning! Don't modify this file! */
/* ******************************** */
#include "PikaStdData_List.h"
#include "TinyObj.h"
#include <stdio.h>
#include <stdlib.h>
#include "BaseObj.h"

void PikaStdData_List___del__Method(PikaObj *self, Args *args){
    PikaStdData_List___del__(self);
}

void PikaStdData_List___get__Method(PikaObj *self, Args *args){
    Arg* __key = args_getArg(args, "__key");
    Arg* res = PikaStdData_List___get__(self, __key);
    method_returnArg(args, res);
}

void PikaStdData_List___init__Method(PikaObj *self, Args *args){
    PikaStdData_List___init__(self);
}

void PikaStdData_List___iter__Method(PikaObj *self, Args *args){
    Arg* res = PikaStdData_List___iter__(self);
    method_returnArg(args, res);
}

void PikaStdData_List___len__Method(PikaObj *self, Args *args){
    int res = PikaStdData_List___len__(self);
    method_returnInt(args, res);
}

void PikaStdData_List___next__Method(PikaObj *self, Args *args){
    Arg* res = PikaStdData_List___next__(self);
    method_returnArg(args, res);
}

void PikaStdData_List___set__Method(PikaObj *self, Args *args){
    Arg* __key = args_getArg(args, "__key");
    Arg* __val = args_getArg(args, "__val");
    PikaStdData_List___set__(self, __key, __val);
}

void PikaStdData_List___str__Method(PikaObj *self, Args *args){
    char* res = PikaStdData_List___str__(self);
    method_returnStr(args, res);
}

void PikaStdData_List_appendMethod(PikaObj *self, Args *args){
    Arg* arg = args_getArg(args, "arg");
    PikaStdData_List_append(self, arg);
}

void PikaStdData_List_getMethod(PikaObj *self, Args *args){
    int i = args_getInt(args, "i");
    Arg* res = PikaStdData_List_get(self, i);
    method_returnArg(args, res);
}

void PikaStdData_List_lenMethod(PikaObj *self, Args *args){
    int res = PikaStdData_List_len(self);
    method_returnInt(args, res);
}

void PikaStdData_List_setMethod(PikaObj *self, Args *args){
    Arg* arg = args_getArg(args, "arg");
    int i = args_getInt(args, "i");
    PikaStdData_List_set(self, arg, i);
}

PikaObj *New_PikaStdData_List(Args *args){
    PikaObj *self = New_TinyObj(args);
    class_defineMethod(self, "__del__()", PikaStdData_List___del__Method);
    class_defineMethod(self, "__get__(__key:any)->any", PikaStdData_List___get__Method);
    class_defineMethod(self, "__init__()", PikaStdData_List___init__Method);
    class_defineMethod(self, "__iter__()->any", PikaStdData_List___iter__Method);
    class_defineMethod(self, "__len__()->int", PikaStdData_List___len__Method);
    class_defineMethod(self, "__next__()->any", PikaStdData_List___next__Method);
    class_defineMethod(self, "__set__(__key:any,__val:any)", PikaStdData_List___set__Method);
    class_defineMethod(self, "__str__()->str", PikaStdData_List___str__Method);
    class_defineMethod(self, "append(arg:any)", PikaStdData_List_appendMethod);
    class_defineMethod(self, "get(i:int)->any", PikaStdData_List_getMethod);
    class_defineMethod(self, "len()->int", PikaStdData_List_lenMethod);
    class_defineMethod(self, "set(i:int,arg:any)", PikaStdData_List_setMethod);
    return self;
}

Arg *PikaStdData_List(PikaObj *self){
    return obj_newObjInPackage(New_PikaStdData_List);
}
