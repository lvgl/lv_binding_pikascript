/*
 * This file is part of the PikaScript project.
 * http://github.com/pikastech/pikascript
 *
 * MIT License
 *
 * Copyright (c) 2021 lyon 李昂 liang6516@outlook.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "dataArg.h"
#include "PikaObj.h"
#include "dataArgs.h"
#include "dataMemory.h"
#include "dataString.h"
#include "stdlib.h"

uint32_t arg_getTotleSize(Arg* self) {
    return arg_totleSize(self);
}

/**
 * time33 hash
 */
Hash hash_time33(char* str) {
    Hash hash = 5381;
    while (*str) {
        hash += (hash << 5) + (*str++);
    }
    return (hash & 0x7FFFFFFF);
}

static Arg* arg_init_hash(Hash nameHash,
                          ArgType type,
                          uint8_t* content,
                          uint32_t size,
                          Arg* next) {
    Arg* self = (Arg*)pikaMalloc(sizeof(Arg) + size);
    arg_setNext(self, next);
    self->size = size;
    self->name_hash = nameHash;
    self->type = type;
    self->serialized = PIKA_TRUE;
    __platform_memset(arg_getContent(self), 0,
                      aline_by(size, sizeof(uint32_t)));
    if (NULL != content) {
        __platform_memcpy(arg_getContent(self), content, size);
    }

    return self;
}

static Arg* arg_init(char* name,
                     ArgType type,
                     uint8_t* content,
                     uint32_t size,
                     Arg* next) {
    Hash nameHash = hash_time33(name);
    return arg_init_hash(nameHash, type, content, size, next);
}

void arg_init_stack(Arg* self, uint8_t* buffer, uint32_t size) {
    self->_.buffer = buffer;
    self->size = size;
    self->type = ARG_TYPE_UNDEF;
    self->name_hash = 0;
    self->serialized = PIKA_FALSE;
}

uint32_t arg_totleSize(Arg* self) {
    return ((Arg*)self)->size + sizeof(Arg);
}

void arg_freeContent(Arg* self) {
    if (NULL != self) {
        uint32_t totleSize = arg_totleSize(self);
        pikaFree(self, totleSize);
        return;
    }
}

Arg* arg_setContent(Arg* self, uint8_t* content, uint32_t size) {
    if (NULL == self) {
        /* malloc */
        return arg_init("", ARG_TYPE_NONE, content, size, NULL);
    }

    /* only copy */
    if (arg_getSize(self) >= size) {
        __platform_memcpy(arg_getContent((Arg*)self), content, size);
        return self;
    }

    /* realloc */
    Hash nameHash = arg_getNameHash(self);
    ArgType type = arg_getType(self);
    Arg* next = arg_getNext(self);
    Arg* newContent = arg_init_hash(nameHash, type, content, size, next);
    arg_freeContent(self);
    return newContent;
}

Arg* arg_setNameHash(Arg* self, Hash nameHash) {
    if (NULL == self) {
        return arg_init_hash(nameHash, ARG_TYPE_NONE, NULL, 0, NULL);
    }
    Arg* arg = (Arg*)self;
    arg->name_hash = nameHash;
    return self;
}

Arg* arg_setName(Arg* self, char* name) {
    return arg_setNameHash(self, hash_time33(name));
}

Arg* arg_setType(Arg* self, ArgType type) {
    if (NULL == self) {
        return arg_init("", type, NULL, 0, NULL);
    }
    self->type = type;
    return self;
}

Arg* arg_setBytes(Arg* self, char* name, uint8_t* src, size_t size) {
    self = arg_newContent(self, size + sizeof(size_t) + 1);
    if (NULL == self) {
        return NULL;
    }
    self = arg_setName(self, name);
    self = arg_setType(self, ARG_TYPE_BYTES);
    void* dir = arg_getContent(self);
    /* set content all to 0 */
    __platform_memset(dir, 0, size + sizeof(size_t) + 1);
    /* setsize */
    __platform_memcpy(dir, &size, sizeof(size_t));

    /* set init value */
    if (NULL != src) {
        __platform_memcpy((void*)((uintptr_t)dir + sizeof(size_t)), src, size);
    }
    return self;
}

Arg* arg_newContent(Arg* self, uint32_t size) {
    arg_freeContent(self);
    Arg* newContent = arg_init("", ARG_TYPE_NONE, NULL, size, NULL);
    return newContent;
}

uint8_t* arg_getBytes(Arg* self) {
    return arg_getContent(self) + sizeof(size_t);
}

void arg_printBytes(Arg* self) {
    size_t bytes_size = arg_getBytesSize(self);
    uint8_t* bytes = arg_getBytes(self);
    __platform_printf("b\'");
    for (size_t i = 0; i < bytes_size; i++) {
        __platform_printf("\\x%02x", bytes[i]);
    }
    __platform_printf("\'\r\n");
}

size_t arg_getBytesSize(Arg* self) {
    size_t mem_size = 0;
    void* content = (void*)arg_getContent(self);
    if (NULL == content) {
        return 0;
    }
    __platform_memcpy(&mem_size, content, sizeof(size_t));
    return mem_size;
}

Arg* arg_setStruct(Arg* self,
                   char* name,
                   void* struct_ptr,
                   uint32_t struct_size) {
    if (NULL == struct_ptr) {
        return NULL;
    }
    Arg* struct_arg = arg_setContent(NULL, (uint8_t*)struct_ptr, struct_size);
    struct_arg = arg_setType(struct_arg, ARG_TYPE_STRUCT);
    struct_arg = arg_setName(struct_arg, name);
    return struct_arg;
}

Arg* arg_setHeapStruct(Arg* self,
                       char* name,
                       void* struct_ptr,
                       uint32_t struct_size,
                       void* struct_deinit_fun) {
    if (NULL == struct_ptr) {
        return NULL;
    }
    Arg* struct_arg =
        arg_setContent(NULL, (uint8_t*)&struct_deinit_fun, sizeof(void*));
    struct_arg = arg_append(struct_arg, (uint8_t*)struct_ptr, struct_size);
    struct_arg = arg_setType(struct_arg, ARG_TYPE_STRUCT_HEAP);
    struct_arg = arg_setName(struct_arg, name);
    return struct_arg;
}

void* arg_getHeapStructDeinitFun(Arg* self) {
    void* deinit_fun = NULL;
    __platform_memcpy(&deinit_fun, arg_getContent(self), sizeof(void*));
    return deinit_fun;
}

Arg* arg_setInt(Arg* self, char* name, int64_t val) {
    if (NULL == self) {
        return arg_init(name, ARG_TYPE_INT, (uint8_t*)&val, sizeof(val), NULL);
    }
    self = arg_setContent(self, (uint8_t*)&val, sizeof(val));
    self = arg_setType(self, ARG_TYPE_INT);
    self = arg_setName(self, name);
    return self;
}

Arg* arg_setNull(Arg* self) {
    return arg_init("", ARG_TYPE_NONE, NULL, 0, NULL);
}

Arg* arg_setFloat(Arg* self, char* name, double val) {
    if (NULL == self) {
        return arg_init(name, ARG_TYPE_FLOAT, (uint8_t*)&val, sizeof(val),
                        NULL);
    }
    self = arg_setContent(self, (uint8_t*)&val, sizeof(val));
    self = arg_setType(self, ARG_TYPE_FLOAT);
    self = arg_setName(self, name);
    return self;
}

double arg_getFloat(Arg* self) {
    if (NULL == arg_getContent(self)) {
        return -999.999;
    }

    return *(double*)arg_getContent(self);
}

Arg* arg_setPtr(Arg* self, char* name, ArgType type, void* pointer) {
    return arg_init(name, type, (uint8_t*)&pointer, sizeof(uintptr_t), NULL);
}

Arg* arg_setStr(Arg* self, char* name, char* string) {
    if (NULL == string) {
        return NULL;
    }
    return arg_init(name, ARG_TYPE_STRING, (uint8_t*)string,
                    strGetSize(string) + 1, NULL);
}

int64_t arg_getInt(Arg* self) {
    pika_assert(NULL!=self);
    if (NULL == arg_getContent(self)) {
        return -999999;
    }
    return *(int64_t*)arg_getContent(self);
}

void* arg_getPtr(Arg* self) {
    if (NULL == arg_getContent(self)) {
        return NULL;
    }
    return *(void**)arg_getContent(self);
}
char* arg_getStr(Arg* self) {
    return (char*)arg_getContent(self);
}

Hash arg_getNameHash(Arg* self) {
    if (NULL == self) {
        return 999999;
    }
    return self->name_hash;
}

ArgType arg_getType(Arg* self) {
    if (NULL == self) {
        return ARG_TYPE_NONE;
    }
    return (ArgType)self->type;
}

uint32_t arg_getContentSize(Arg* self) {
    return arg_getSize(self);
}

Arg* New_arg(void* voidPointer) {
    return NULL;
}

Arg* arg_copy(Arg* arg_src) {
    if (NULL == arg_src) {
        return NULL;
    }
    ArgType arg_type = arg_getType(arg_src);
    if (ARG_TYPE_OBJECT == arg_type) {
        obj_refcntInc((PikaObj*)arg_getPtr(arg_src));
    }
    Arg* argCopied = New_arg(NULL);
    argCopied = arg_setContent(argCopied, arg_getContent(arg_src),
                               arg_getContentSize(arg_src));
    argCopied = arg_setNameHash(argCopied, arg_getNameHash(arg_src));
    argCopied = arg_setType(argCopied, arg_getType(arg_src));
    return argCopied;
}

Arg* arg_copy_noalloc(Arg* arg_src, Arg* arg_dict) {
    if (NULL == arg_src) {
        return NULL;
    }
    if (NULL == arg_dict){
        return arg_copy(arg_src);
    }
    /* size is too big to be copied by noalloc */
    if (arg_getSize(arg_src) > arg_getSize(arg_dict)) {
        return arg_copy(arg_src);
    }
    ArgType arg_type = arg_getType(arg_src);
    if (ARG_TYPE_OBJECT == arg_type) {
        obj_refcntInc((PikaObj*)arg_getPtr(arg_src));
    }
    arg_dict->serialized = PIKA_FALSE;
    arg_dict = arg_setContent(arg_dict, arg_getContent(arg_src),
                              arg_getContentSize(arg_src));
    arg_dict = arg_setNameHash(arg_dict, arg_getNameHash(arg_src));
    arg_dict = arg_setType(arg_dict, arg_getType(arg_src));
    return arg_dict;
}

Arg* arg_append(Arg* self, void* new_content, size_t new_size) {
    uint8_t* old_content = arg_getContent(self);
    size_t old_size = arg_getContentSize(self);
    /* create arg_out */
    Arg* arg_out = arg_setContent(NULL, NULL, old_size + new_size);
    arg_setType(arg_out, arg_getType(self));
    arg_setNameHash(arg_out, arg_getNameHash(self));
    /* copy old content */
    __platform_memcpy(arg_getContent(arg_out), old_content, old_size);
    /* copy new content */
    __platform_memcpy(arg_getContent(arg_out) + old_size, new_content,
                      new_size);
    arg_deinit(self);
    return arg_out;
}

void* arg_getHeapStruct(Arg* self) {
    return arg_getContent(self) + sizeof(void*);
}

void arg_deinitHeap(Arg* self) {
    ArgType type = arg_getType(self);
    /* deinit heap struct */
    if (type == ARG_TYPE_STRUCT_HEAP) {
        /* deinit heap strcut */
        StructDeinitFun struct_deinit_fun =
            (StructDeinitFun)arg_getHeapStructDeinitFun(self);
        struct_deinit_fun(arg_getHeapStruct(self));
    }
    /* deinit sub object */
    if (ARG_TYPE_OBJECT == type) {
        PikaObj* subObj = arg_getPtr(self);
        obj_refcntDec(subObj);
        int ref_cnt = obj_refcntNow(subObj);
        if (ref_cnt <= 0) {
            obj_deinit(subObj);
        }
    }
}

/* load file as byte array */
Arg* arg_loadFile(Arg* self, char* filename) {
    size_t file_size = 0;
    char* file_buff = __platform_malloc(PIKA_READ_FILE_BUFF_SIZE);
    Arg* res = New_arg(NULL);
    __platform_memset(file_buff, 0, PIKA_READ_FILE_BUFF_SIZE);
    FILE* input_file = __platform_fopen(filename, "rb");
    if (NULL == input_file) {
        __platform_printf("Error: Couldn't open file '%s'\n", filename);
        res = NULL;
        goto exit;
    }
    file_size =
        __platform_fread(file_buff, 1, PIKA_READ_FILE_BUFF_SIZE, input_file);

    if (file_size >= PIKA_READ_FILE_BUFF_SIZE) {
        __platform_printf("Error: Not enough buff for input file.\r\n");
        return NULL;
    }
    /* add '\0' to the end of the string */
    res = arg_setBytes(res, "", (uint8_t*)file_buff, file_size + 1);

exit:
    __platform_free(file_buff);
    if (NULL != input_file) {
        __platform_fclose(input_file);
    }
    return res;
}

void arg_deinit(Arg* self) {
    if (NULL == self) {
        return;
    }
    /* deinit arg pointed heap */
    arg_deinitHeap(self);
    if (!self->serialized) {
        return;
    }
    /* free the ref */
    arg_freeContent(self);
}
