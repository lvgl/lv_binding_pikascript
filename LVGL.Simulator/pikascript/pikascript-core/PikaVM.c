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

#include "PikaVM.h"
#include "BaseObj.h"
#include "PikaCompiler.h"
#include "PikaObj.h"
#include "PikaParser.h"
#include "PikaPlatform.h"
#include "dataStrs.h"
#if PIKA_MATH_ENABLE
#include <math.h>
#endif

/* head declear start */
static uint8_t VMState_getInputArgNum(VMState* vm);
static VMParameters* __pikaVM_runByteCodeFrameWithState(
    PikaObj* self,
    VMParameters* locals,
    VMParameters* globals,
    ByteCodeFrame* bytecode_frame,
    uint16_t pc,
    TryInfo* try_info);

/* head declear end */

static InstructUnit* VMState_getInstructNow(VMState* vm) {
    return instructArray_getByOffset(&(vm->bytecode_frame->instruct_array),
                                     vm->pc);
}

static void VMState_setErrorCode(VMState* vm, uint8_t error_code) {
    vm->error_code = error_code;
}

static InstructUnit* VMState_getInstructWithOffset(VMState* vm,
                                                   int32_t offset) {
    return instructArray_getByOffset(&(vm->bytecode_frame->instruct_array),
                                     vm->pc + offset);
}

static int VMState_getBlockDeepthNow(VMState* vm) {
    /* support run byteCode */
    InstructUnit* ins_unit = VMState_getInstructNow(vm);
    return instructUnit_getBlockDeepth(ins_unit);
}

static char* VMState_getConstWithInstructUnit(VMState* vm,
                                              InstructUnit* ins_unit) {
    return constPool_getByOffset(&(vm->bytecode_frame->const_pool),
                                 instructUnit_getConstPoolIndex(ins_unit));
}

static int32_t VMState_getAddrOffsetOfJmpBack(VMState* vm) {
    int offset = 0;
    int loop_deepth = -1;

    /* find loop deepth */
    while (1) {
        offset -= instructUnit_getSize(ins_unit_now);
        InstructUnit* ins_unit_now = VMState_getInstructWithOffset(vm, offset);
        uint16_t invoke_deepth = instructUnit_getInvokeDeepth(ins_unit_now);
        enum Instruct ins = instructUnit_getInstruct(ins_unit_now);
        char* data = VMState_getConstWithInstructUnit(vm, ins_unit_now);
        if ((0 == invoke_deepth) && (JEZ == ins) && strEqu(data, "2")) {
            loop_deepth = instructUnit_getBlockDeepth(ins_unit_now);
            break;
        }
    }

    offset = 0;
    while (1) {
        offset += instructUnit_getSize(ins_unit_now);
        InstructUnit* ins_unit_now = VMState_getInstructWithOffset(vm, offset);
        enum Instruct ins = instructUnit_getInstruct(ins_unit_now);
        char* data = VMState_getConstWithInstructUnit(vm, ins_unit_now);
        int block_deepth_now = instructUnit_getBlockDeepth(ins_unit_now);
        if ((block_deepth_now == loop_deepth) && (JMP == ins) &&
            strEqu(data, "-1")) {
            return offset;
        }
    }
}

static size_t VMState_getInstructArraySize(VMState* vm) {
    return instructArray_getSize(&(vm->bytecode_frame->instruct_array));
}

static int32_t VMState_getAddrOffsetFromJmp(VMState* vm) {
    int offset = 0;
    /* run byte Code */
    InstructUnit* this_ins_unit = VMState_getInstructNow(vm);
    int thisBlockDeepth = instructUnit_getBlockDeepth(this_ins_unit);
    int8_t blockNum = 0;

    if (vm->jmp > 0) {
        offset = 0;
        while (1) {
            offset += instructUnit_getSize();
            /* reach the end */
            if (vm->pc + offset >= (int)VMState_getInstructArraySize(vm)) {
                break;
            }
            this_ins_unit = VMState_getInstructWithOffset(vm, offset);
            if (instructUnit_getIsNewLine(this_ins_unit)) {
                uint8_t blockDeepth =
                    instructUnit_getBlockDeepth(this_ins_unit);
                if (blockDeepth <= thisBlockDeepth) {
                    blockNum++;
                }
            }
            if (blockNum >= vm->jmp) {
                break;
            }
        }
    }
    if (vm->jmp < 0) {
        while (1) {
            offset -= instructUnit_getSize();
            this_ins_unit = VMState_getInstructWithOffset(vm, offset);
            if (instructUnit_getIsNewLine(this_ins_unit)) {
                uint8_t blockDeepth =
                    instructUnit_getBlockDeepth(this_ins_unit);
                if (blockDeepth == thisBlockDeepth) {
                    blockNum--;
                }
            }
            if (blockNum <= vm->jmp) {
                break;
            }
        }
    }
    return offset;
}

static int32_t VMState_getAddrOffsetOfBreak(VMState* vm) {
    int32_t offset = VMState_getAddrOffsetOfJmpBack(vm);
    /* byteCode */
    offset += instructUnit_getSize();
    return offset;
}

static int32_t VMState_getAddrOffsetOfRaise(VMState* vm) {
    int offset = 0;
    InstructUnit* ins_unit_now = VMState_getInstructNow(vm);
    while (1) {
        offset += instructUnit_getSize(ins_unit_now);
        if (vm->pc + offset >= (int)VMState_getInstructArraySize(vm)) {
            return 0;
        }
        ins_unit_now = VMState_getInstructWithOffset(vm, offset);
        enum Instruct ins = instructUnit_getInstruct(ins_unit_now);
        if ((NTR == ins)) {
            return offset;
        }
    }
}

static int32_t VMState_getAddrOffsetOfContinue(VMState* vm) {
    int32_t offset = VMState_getAddrOffsetOfJmpBack(vm);
    /* byteCode */
    return offset;
}

static void VMState_delLReg(VMState* vm, uint8_t index) {
    PikaObj* obj = vm->lreg[index];
    if (NULL != obj) {
        obj_refcntDec(obj);
        vm->lreg[index] = NULL;
        if (0 == obj_refcntNow(obj)) {
            obj_deinit(obj);
        }
    }
}

static void VMState_initReg(VMState* vm) {
    for (uint8_t i = 0; i < PIKA_REGIST_SIZE; i++) {
        vm->lreg[i] = NULL;
        vm->ireg[i] = 0;
    }
}

static PIKA_BOOL _checkLReg(char* data) {
    if ((data[0] == '_') && (data[1] == 'l') && (data[2] >= '0') &&
        (data[2] <= '9')) {
        return PIKA_TRUE;
    }
    return PIKA_FALSE;
}

static uint8_t _getLRegIndex(char* data) {
    return data[2] - '0';
}

static void VMState_setLReg(VMState* vm, uint8_t index, PikaObj* obj) {
    obj_refcntInc(obj);
    vm->lreg[index] = obj;
}

typedef Arg* (*VM_instruct_handler)(PikaObj* self,
                                    VMState* vm,
                                    char* data,
                                    Arg* arg_ret_reg);

static Arg* VM_instruction_handler_NON(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    return NULL;
}

Arg* __vm_get(PikaObj* self, Arg* key, Arg* obj) {
    ArgType obj_type = arg_getType(obj);
    int index = 0;
    if (ARG_TYPE_INT == arg_getType(key)) {
        index = arg_getInt(key);
    }
    if (ARG_TYPE_STRING == obj_type) {
        char* str_pyload = arg_getStr(obj);
        char char_buff[] = " ";
        if (index < 0) {
            index = strGetSize(str_pyload) + index;
        }
        char_buff[0] = str_pyload[index];
        return arg_newStr(char_buff);
    }
    if (ARG_TYPE_BYTES == obj_type) {
        uint8_t* bytes_pyload = arg_getBytes(obj);
        uint8_t byte_buff[] = " ";
        if (index < 0) {
            index = arg_getBytesSize(obj) + index;
        }
        byte_buff[0] = bytes_pyload[index];
        return arg_newBytes(byte_buff, 1);
    }
    if (argType_isObject(obj_type)) {
        PikaObj* arg_obj = arg_getPtr(obj);
        obj_setArg(arg_obj, "__key", key);
        /* clang-format off */
        PIKA_PYTHON(
        __res = __getitem__(__key)
        )
        /* clang-format on */
        const uint8_t bytes[] = {
            0x0c, 0x00, /* instruct array size */
            0x10, 0x81, 0x01, 0x00, 0x00, 0x02, 0x07, 0x00, 0x00, 0x04, 0x13,
            0x00,
            /* instruct array */
            0x19, 0x00, /* const pool size */
            0x00, 0x5f, 0x5f, 0x6b, 0x65, 0x79, 0x00, 0x5f, 0x5f, 0x67, 0x65,
            0x74, 0x69, 0x74, 0x65, 0x6d, 0x5f, 0x5f, 0x00, 0x5f, 0x5f, 0x72,
            0x65, 0x73, 0x00,
            /* const pool */
        };
        pikaVM_runByteCode(arg_obj, (uint8_t*)bytes);
        return arg_copy(args_getArg(arg_obj->list, "__res"));
    }
    return arg_newNull();
}

Arg* __vm_slice(PikaObj* self, Arg* end, Arg* obj, Arg* start, int step) {
#if PIKA_SYNTAX_SLICE_ENABLE
    /* No interger index only support __getitem__ */
    if (!(arg_getType(start) == ARG_TYPE_INT &&
          arg_getType(end) == ARG_TYPE_INT)) {
        return __vm_get(self, start, obj);
    }

    int start_i = arg_getInt(start);
    int end_i = arg_getInt(end);

    /* __slice__ is equal to __getitem__ */
    if (end_i - start_i == 1) {
        return __vm_get(self, start, obj);
    }

    if (ARG_TYPE_STRING == arg_getType(obj)) {
        size_t len = strGetSize(arg_getStr(obj));
        if (start_i < 0) {
            start_i += len;
        }
        if (end_i < 0) {
            end_i += len + 1;
        }
        Arg* sliced_arg = arg_newStr("");
        for (int i = start_i; i < end_i; i++) {
            Arg* i_arg = arg_newInt(i);
            Arg* item_arg = __vm_get(self, i_arg, obj);
            sliced_arg = arg_strAppend(sliced_arg, arg_getStr(item_arg));
            arg_deinit(item_arg);
            arg_deinit(i_arg);
        }
        return sliced_arg;
    }

    if (ARG_TYPE_BYTES == arg_getType(obj)) {
        size_t len = arg_getBytesSize(obj);
        if (start_i < 0) {
            start_i += len;
        }
        if (end_i < 0) {
            end_i += len + 1;
        }
        Arg* sliced_arg = arg_newBytes(NULL, 0);
        for (int i = start_i; i < end_i; i++) {
            Arg* i_arg = arg_newInt(i);
            Arg* item_arg = __vm_get(self, i_arg, obj);
            uint8_t* bytes_origin = arg_getBytes(sliced_arg);
            size_t size_origin = arg_getBytesSize(sliced_arg);
            Arg* sliced_arg_new = arg_newBytes(NULL, size_origin + 1);
            __platform_memcpy(arg_getBytes(sliced_arg_new), bytes_origin,
                              size_origin);
            __platform_memcpy(arg_getBytes(sliced_arg_new) + size_origin,
                              arg_getBytes(item_arg), 1);
            arg_deinit(sliced_arg);
            sliced_arg = sliced_arg_new;
            arg_deinit(item_arg);
            arg_deinit(i_arg);
        }
        return sliced_arg;
    }
    return arg_newNull();
#else
    return __vm_get(self, start, obj);
#endif
}

static Arg* VM_instruction_handler_SLC(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
#if PIKA_SYNTAX_SLICE_ENABLE
    int arg_num_input = VMState_getInputArgNum(vm);
    if (arg_num_input < 2) {
        return arg_newNull();
    }
    if (arg_num_input == 2) {
        Arg* key = stack_popArg_alloc(&vm->stack);
        Arg* obj = stack_popArg_alloc(&vm->stack);
        Arg* res = __vm_get(self, key, obj);
        arg_deinit(key);
        arg_deinit(obj);
        return res;
    }
    if (arg_num_input == 3) {
        Arg* end = stack_popArg_alloc(&vm->stack);
        Arg* start = stack_popArg_alloc(&vm->stack);
        Arg* obj = stack_popArg_alloc(&vm->stack);
        Arg* res = __vm_slice(self, end, obj, start, 1);
        arg_deinit(end);
        arg_deinit(obj);
        arg_deinit(start);
        return res;
    }
    return arg_newNull();
#else
    Arg* key = stack_popArg_alloc(&vm->stack);
    Arg* obj = stack_popArg_alloc(&vm->stack);
    Arg* res = __vm_get(self, key, obj);
    arg_deinit(key);
    arg_deinit(obj);
    return res;
#endif
}

static Arg* VM_instruction_handler_TRY(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    pika_assert(NULL != vm->try_info);
    vm->try_info->try_state = TRY_STATE_TOP;
    return NULL;
}

static Arg* VM_instruction_handler_EXP(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    return NULL;
}

static Arg* VM_instruction_handler_NTR(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    vm->try_info->try_state = TRY_STATE_NONE;
    return NULL;
}

static Arg* VM_instruction_handler_NEW(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    Arg* origin_arg = obj_getArg(vm->locals, data);
    Arg* new_arg = arg_copy(origin_arg);
    origin_arg = arg_setType(origin_arg, ARG_TYPE_OBJECT);
    arg_setType(new_arg, ARG_TYPE_OBJECT_NEW);
    return new_arg;
}

static Arg* VM_instruction_handler_REF(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    if (strEqu(data, (char*)"True")) {
        return arg_setInt(arg_ret_reg, "", 1);
    }
    if (strEqu(data, (char*)"False")) {
        return arg_setInt(arg_ret_reg, "", 0);
    }
    if (strEqu(data, (char*)"None")) {
        return arg_setNull(arg_ret_reg);
    }
    if (strEqu(data, (char*)"RuntimeError")) {
        return arg_setInt(arg_ret_reg, "", PIKA_RES_ERR_RUNTIME_ERROR);
    }

    Arg* arg = NULL;
    if (data[0] == '.') {
        /* find host from stack */
        Arg* host_obj = stack_popArg_alloc(&(vm->stack));
        if (argType_isObject(arg_getType(host_obj))) {
            arg = arg_copy_noalloc(obj_getArg(arg_getPtr(host_obj), data + 1),
                                   arg_ret_reg);
        }
        arg_deinit(host_obj);
        goto exit;
    }

    /* find in local list first */
    arg = arg_copy_noalloc(obj_getArg(vm->locals, data), arg_ret_reg);
    if (NULL == arg) {
        /* find in global list second */
        arg = arg_copy_noalloc(obj_getArg(vm->globals, data), arg_ret_reg);
    }

exit:
    if (NULL == arg) {
        VMState_setErrorCode(vm, PIKA_RES_ERR_ARG_NO_FOUND);
        __platform_printf("NameError: name '%s' is not defined\r\n", data);
    }
    return arg;
}

static Arg* VM_instruction_handler_GER(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    PIKA_RES err = (PIKA_RES)vm->try_error_code;
    Arg* err_arg = arg_newInt(err);
    return err_arg;
}

static int32_t __foreach_handler_deinitTuple(Arg* argEach, Args* context) {
    if (arg_getType(argEach) == ARG_TYPE_TUPLE) {
        PikaTuple* tuple = arg_getPtr(argEach);
        tuple_deinit(tuple);
    }
    return PIKA_RES_OK;
}

Arg* _obj_runMethodArgWithState(PikaObj* self,
                                PikaObj* method_args_obj,
                                Arg* method_arg,
                                TryInfo* try_state,
                                Arg* ret_arg_reg) {
    pika_assert(NULL != try_state);
    Arg* return_arg = NULL;
    /* get method Ptr */
    Method method_ptr = methodArg_getPtr(method_arg);
    /* get method type list */
    ArgType method_type = arg_getType(method_arg);
    /* error */
    if (ARG_TYPE_NONE == method_type) {
        return NULL;
    }
    ByteCodeFrame* method_bytecodeFrame =
        methodArg_getBytecodeFrame(method_arg);
    PikaObj* method_context = methodArg_getDefContext(method_arg);
    if (NULL != method_context) {
        self = method_context;
    }
    obj_setErrorCode(self, PIKA_RES_OK);

    /* run method */
    if (method_type == ARG_TYPE_METHOD_NATIVE) {
        /* native method */
        method_ptr(self, method_args_obj->list);
        /* get method return */
        return_arg = arg_copy_noalloc(
            args_getArg(method_args_obj->list, (char*)"return"), ret_arg_reg);
    } else if (method_type == ARG_TYPE_METHOD_NATIVE_CONSTRUCTOR) {
        /* native method */
        method_ptr(self, method_args_obj->list);
        /* get method return */
        return_arg = arg_copy_noalloc(
            args_getArg(method_args_obj->list, (char*)"return"), ret_arg_reg);
    } else {
        /* static method and object method */
        /* byteCode */
        uintptr_t insturctArray_start = (uintptr_t)instructArray_getByOffset(
            &(method_bytecodeFrame->instruct_array), 0);
        uint16_t pc = (uintptr_t)method_ptr - insturctArray_start;
        method_args_obj = __pikaVM_runByteCodeFrameWithState(
            self, method_args_obj, self, method_bytecodeFrame, pc, try_state);

        /* get method return */
        return_arg = arg_copy_noalloc(
            args_getArg(method_args_obj->list, (char*)"return"), ret_arg_reg);
    }
    args_foreach(method_args_obj->list, __foreach_handler_deinitTuple, NULL);
    return return_arg;
}

Arg* obj_runMethodArgWithState(PikaObj* self,
                               PikaObj* method_args_obj,
                               Arg* method_arg,
                               TryInfo* try_state) {
    return _obj_runMethodArgWithState(self, method_args_obj, method_arg,
                                      try_state, NULL);
}

Arg* obj_runMethodArgWithState_noalloc(PikaObj* self,
                                       PikaObj* method_args_obj,
                                       Arg* method_arg,
                                       TryInfo* try_state,
                                       Arg* ret_arg_reg) {
    return _obj_runMethodArgWithState(self, method_args_obj, method_arg,
                                      try_state, ret_arg_reg);
}

Arg* obj_runMethodArg(PikaObj* self,
                      PikaObj* method_args_obj,
                      Arg* method_arg) {
    TryInfo try_info = {.try_state = TRY_STATE_NONE,
                        .try_result = TRY_RESULT_NONE};
    return obj_runMethodArgWithState(self, method_args_obj, method_arg,
                                     &try_info);
}

static int VMState_loadArgsFromMethodArg(VMState* vm,
                                         PikaObj* method_host_obj,
                                         Args* args,
                                         Arg* method_arg,
                                         char* method_name,
                                         int arg_num_used) {
    char _buffs1[PIKA_LINE_BUFF_SIZE] = {0};
    char* buffs1 = (char*)_buffs1;
    char _buffs2[PIKA_LINE_BUFF_SIZE / 2] = {0};
    char* buffs2 = (char*)_buffs2;
    uint8_t arg_num_dec = 0;
    PIKA_BOOL is_variable = PIKA_FALSE;
    PIKA_BOOL is_get_variable_arg = PIKA_FALSE;
    uint8_t arg_num = 0;
    ArgType method_type = ARG_TYPE_UNDEF;
    uint8_t arg_num_input = 0;
    PikaTuple* tuple = NULL;
    char* variable_tuple_name = NULL;
    char* type_list_buff = NULL;
    int variable_arg_start = 0;
    /* get method type list */
    char* type_list =
        methodArg_getTypeList(method_arg, buffs1, sizeof(_buffs1));
    if (NULL == type_list) {
        __platform_printf(
            "OverflowError: type list is too long, please use bigger "
            "PIKA_LINE_BUFF_SIZE\r\n");
        while (1)
            ;
    }
    method_type = arg_getType(method_arg);

    /* check variable */
    if (strIsContain(type_list, '*')) {
        is_variable = PIKA_TRUE;
    }

    /* get arg_num_dec */
    if (strEqu("", type_list)) {
        arg_num_dec = 0;
    } else {
        arg_num_dec = strCountSign(type_list, ',') + 1;
    }
    if (method_type == ARG_TYPE_METHOD_OBJECT) {
        /* delete the 'self' */
        arg_num_dec--;
    }
    arg_num_input = VMState_getInputArgNum(vm);

    /* check arg num */
    if (method_type == ARG_TYPE_METHOD_NATIVE_CONSTRUCTOR ||
        method_type == ARG_TYPE_METHOD_CONSTRUCTOR ||
        is_variable == PIKA_TRUE) {
        /* skip for constrctor */
        /* skip for variable args */
    } else {
        /* check arg num decleard and input */
        if (arg_num_dec != arg_num_input - arg_num_used) {
            VMState_setErrorCode(vm, PIKA_RES_ERR_INVALID_PARAM);
            __platform_printf(
                "TypeError: %s() takes %d positional argument but %d were "
                "given\r\n",
                method_name, arg_num_dec, arg_num_input);
            goto exit;
        }
    }

    if (PIKA_TRUE == is_variable) {
        arg_num = arg_num_input;
    } else {
        arg_num = arg_num_dec;
    }

    if (is_variable) {
        /* get variable tuple name */
        type_list_buff = strCopy(buffs2, type_list);
        variable_arg_start = 0;
        for (int i = 0; i < arg_num_dec; i++) {
            char* arg_def = strPopLastToken(type_list_buff, ',');
            if (strIsStartWith(arg_def, "*")) {
                /* skip the '*' */
                variable_tuple_name = arg_def + 1;
                variable_arg_start = arg_num_dec - i - 1;
                is_get_variable_arg = PIKA_TRUE;
                break;
            }
        }
    }

    /* found variable arg */
    if (PIKA_TRUE == is_get_variable_arg) {
        tuple = New_tuple();
        strPopLastToken(type_list, ',');
    }

    /* load pars */
    for (int i = 0; i < arg_num; i++) {
        char* arg_name = NULL;
        if (arg_num - i <= variable_arg_start) {
            is_get_variable_arg = PIKA_FALSE;
        }
        if (PIKA_FALSE == is_get_variable_arg) {
            char* arg_def = strPopLastToken(type_list, ',');
            strPopLastToken(arg_def, ':');
            arg_name = arg_def;
        } else {
            /* clear the variable arg name */
            arg_name = "";
        }
        Arg* call_arg = stack_popArg_alloc(&(vm->stack));
        call_arg = arg_setName(call_arg, arg_name);
        /* load the variable arg */
        if (PIKA_TRUE == is_get_variable_arg) {
            list_append(&tuple->super, call_arg);
            /* the append would copy the arg */
            arg_deinit(call_arg);
        } else {
            /* load normal arg */
            args_setArg(args, call_arg);
        }
    }

    if (PIKA_TRUE == is_variable) {
        list_reverse(&tuple->super);
        /* load variable tuple */
        PikaObj* New_PikaStdData_Tuple(Args * args);
        PikaObj* tuple_obj = newNormalObj(New_PikaStdData_Tuple);
        obj_setPtr(tuple_obj, "list", tuple);
        args_setPtrWithType(args, variable_tuple_name, ARG_TYPE_OBJECT,
                            tuple_obj);
    }

    /* load 'self' as the first arg when call object method */
    if (method_type == ARG_TYPE_METHOD_OBJECT) {
        Arg* call_arg = arg_setRef(NULL, "self", method_host_obj);
        args_setArg(args, call_arg);
    }
exit:
    return arg_num;
}

void __vm_List_append(PikaObj* self, Arg* arg) {
    PikaList* list = obj_getPtr(self, "list");
    list_append(list, arg);
}

void __vm_List___init__(PikaObj* self) {
    if (!obj_isArgExist(self, "list")) {
        PikaList* list = New_list();
        obj_setPtr(self, "list", list);
    }
}

#if PIKA_BUILTIN_STRUCT_ENABLE
PikaObj* New_PikaStdData_List(Args* args);
PikaObj* New_PikaStdData_Tuple(Args* args);
#endif

static Arg* _vm_create_list_or_tuple(PikaObj* self,
                                     VMState* vm,
                                     PIKA_BOOL is_list) {
#if PIKA_BUILTIN_STRUCT_ENABLE
    NewFun constructor = is_list ? New_PikaStdData_List : New_PikaStdData_Tuple;
    uint8_t arg_num = VMState_getInputArgNum(vm);
    PikaObj* list = newNormalObj(constructor);
    __vm_List___init__(list);
    Stack stack = {0};
    stack_init(&stack);
    /* load to local stack to change sort */
    for (int i = 0; i < arg_num; i++) {
        Arg* arg = stack_popArg_alloc(&(vm->stack));
        pika_assert(arg != NULL);
        stack_pushArg(&stack, arg);
    }
    for (int i = 0; i < arg_num; i++) {
        Arg* arg = stack_popArg_alloc(&stack);
        pika_assert(arg != NULL);
        __vm_List_append(list, arg);
        arg_deinit(arg);
    }
    stack_deinit(&stack);
    return arg_newPtr(ARG_TYPE_OBJECT, list);
#else
    return VM_instruction_handler_NON(self, vm, "", NULL);
#endif
}

static Arg* VM_instruction_handler_LST(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    return _vm_create_list_or_tuple(self, vm, PIKA_TRUE);
}

void __vm_Dict___init__(PikaObj* self) {
    PikaDict* dict = New_dict();
    PikaDict* keys = New_dict();
    obj_setPtr(self, "dict", dict);
    obj_setPtr(self, "_keys", keys);
}

void __vm_Dict_set(PikaObj* self, Arg* arg, char* key) {
    PikaDict* dict = obj_getPtr(self, "dict");
    PikaDict* keys = obj_getPtr(self, "_keys");
    Arg* arg_key = arg_setStr(NULL, key, key);
    Arg* arg_new = arg_copy(arg);
    arg_setName(arg_new, key);
    dict_setArg(dict, arg_new);
    dict_setArg(keys, arg_key);
}

#if PIKA_BUILTIN_STRUCT_ENABLE
PikaObj* New_PikaStdData_Dict(Args* args);
#endif

static Arg* VM_instruction_handler_DCT(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
#if PIKA_BUILTIN_STRUCT_ENABLE
    uint8_t arg_num = VMState_getInputArgNum(vm);
    PikaObj* dict = newNormalObj(New_PikaStdData_Dict);
    __vm_Dict___init__(dict);
    Stack stack = {0};
    stack_init(&stack);
    /* load to local stack to change sort */
    for (int i = 0; i < arg_num; i++) {
        Arg* arg = stack_popArg_alloc(&(vm->stack));
        stack_pushArg(&stack, arg);
    }
    for (int i = 0; i < arg_num / 2; i++) {
        Arg* key_arg = stack_popArg_alloc(&stack);
        Arg* val_arg = stack_popArg_alloc(&stack);
        __vm_Dict_set(dict, val_arg, arg_getStr(key_arg));
        arg_deinit(key_arg);
        arg_deinit(val_arg);
    }
    stack_deinit(&stack);
    return arg_newPtr(ARG_TYPE_OBJECT, dict);
#else
    return VM_instruction_handler_NON(self, vm, data, arg_ret_reg);
#endif
}

static Arg* VM_instruction_handler_RET(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    /* exit jmp signal */
    vm->jmp = VM_JMP_EXIT;
    Arg* return_arg = stack_popArg_alloc(&(vm->stack));
    method_returnArg(vm->locals->list, return_arg);
    return NULL;
}

static Arg* VM_instruction_handler_RUN(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    Arg* return_arg = NULL;
    VMParameters* sub_locals = NULL;
    char* methodPath = data;
    PikaObj* method_host_obj = NULL;
    Arg* method_arg = NULL;
    Arg* host_arg = NULL;
    PIKA_BOOL isClass = PIKA_FALSE;
    char* sys_out;
    int arg_num_used = 0;
    arg_newReg(arg_reg1, 64);
    TryInfo sub_try_info = {.try_state = TRY_STATE_NONE,
                            .try_result = TRY_RESULT_NONE};
    pika_assert(NULL != vm->try_info);
    if (vm->try_info->try_state == TRY_STATE_TOP ||
        vm->try_info->try_state == TRY_STATE_INNER) {
        sub_try_info.try_state = TRY_STATE_INNER;
    }

    /* tuple or single arg */
    if (data[0] == 0) {
        if (VMState_getInputArgNum(vm) < 2) {
            /* return arg directly */
            Arg* arg1 = stack_popArg(&(vm->stack), &arg_reg1);
            return_arg = arg_copy_noalloc(arg1, arg_ret_reg);
            arg_deinit(arg1);
            goto exit;
        }
        /* create a tuple */
        return_arg = _vm_create_list_or_tuple(self, vm, PIKA_FALSE);
        goto exit;
    }

    /* return tiny obj */
    if (strEqu(data, "TinyObj")) {
        return_arg = arg_newMetaObj(New_TinyObj);
        goto exit;
    }

    /* get method host obj from reg */
    if (NULL == method_host_obj && _checkLReg(data)) {
        uint8_t reg_index = _getLRegIndex(data);
        method_host_obj = vm->lreg[reg_index];
    }

    /* get method host obj from stack */
    if (NULL == method_host_obj && methodPath[0] == '.') {
        /* get method host obj from stack */
        Arg* stack_tmp[PIKA_ARG_NUM_MAX] = {0};
        int arg_num = VMState_getInputArgNum(vm);
        if (arg_num > PIKA_ARG_NUM_MAX) {
            __platform_printf(
                "[ERROR] Too many args in RUN instruction, please use bigger "
                "#define PIKA_ARG_NUM_MAX\n");
            while (1) {
            }
        }
        for (int i = 0; i < arg_num; i++) {
            stack_tmp[i] = stack_popArg_alloc(&(vm->stack));
        }
        host_arg = stack_tmp[arg_num - 1];
        if (argType_isObject(arg_getType(host_arg))) {
            method_host_obj = arg_getPtr(host_arg);
            arg_num_used++;
        }
        /* push back other args to stack */
        for (int i = arg_num - 2; i >= 0; i--) {
            stack_pushArg(&(vm->stack), stack_tmp[i]);
        }
    }

    /* get method host obj from self */
    if (NULL == method_host_obj) {
        method_host_obj = obj_getHostObjWithIsClass(self, methodPath, &isClass);
    }

    /* get method host obj from local scope */
    if (NULL == method_host_obj) {
        method_host_obj =
            obj_getHostObjWithIsClass(vm->locals, methodPath, &isClass);
    }

    /* method host obj is not found */
    if (NULL == method_host_obj) {
        /* error, not found object */
        VMState_setErrorCode(vm, PIKA_RES_ERR_ARG_NO_FOUND);
        __platform_printf("Error: method '%s' no found.\r\n", data);
        goto exit;
    }

    /* get method in local */
    method_arg =
        obj_getMethodArg_noalloc(method_host_obj, methodPath, &arg_reg1);

    if (NULL == method_arg) {
        /* get method in locals */
        method_arg =
            obj_getMethodArg_noalloc(vm->locals, methodPath, &arg_reg1);
    }

    if (NULL == method_arg) {
        /* get method in global */
        method_arg =
            obj_getMethodArg_noalloc(vm->globals, methodPath, &arg_reg1);
    }

    /* assert method type */
    if (NULL == method_arg || ARG_TYPE_NONE == arg_getType(method_arg)) {
        /* error, method no found */
        VMState_setErrorCode(vm, PIKA_RES_ERR_ARG_NO_FOUND);
        __platform_printf("NameError: name '%s' is not defined\r\n", data);
        goto exit;
    }

    /* create sub local scope */
    sub_locals = New_PikaObj();

    /* load args from vmState to sub_local->list */
    arg_num_used += VMState_loadArgsFromMethodArg(
        vm, method_host_obj, sub_locals->list, method_arg, data, arg_num_used);

    /* load args faild */
    if (vm->error_code != 0) {
        goto exit;
    }

    /* run method arg */
    return_arg = obj_runMethodArgWithState_noalloc(
        method_host_obj, sub_locals, method_arg, &sub_try_info, arg_ret_reg);

    /* __init__() */
    if (ARG_TYPE_OBJECT_NEW == arg_getType(return_arg)) {
        arg_setType(return_arg, ARG_TYPE_OBJECT);
        /* init object */
        PikaObj* new_obj = arg_getPtr(return_arg);
        Arg* method_arg =
            obj_getMethodArg_noalloc(new_obj, "__init__", &arg_reg1);
        PikaObj* sub_locals = New_PikaObj();
        Arg* return_arg_init = NULL;
        if (NULL == method_arg) {
            goto init_exit;
        }
        VMState_loadArgsFromMethodArg(vm, new_obj, sub_locals->list, method_arg,
                                      "__init__", arg_num_used);
        /* load args faild */
        if (vm->error_code != 0) {
            goto init_exit;
        }
        return_arg_init = obj_runMethodArgWithState(new_obj, sub_locals,
                                                    method_arg, &sub_try_info);
    init_exit:
        if (NULL != return_arg_init) {
            arg_deinit(return_arg_init);
        }
        obj_deinit(sub_locals);
        arg_deinit(method_arg);
    }

    /* transfer sysOut */
    sys_out = obj_getSysOut(method_host_obj);
    if (NULL != sys_out) {
        args_setSysOut(vm->locals->list, sys_out);
    }

    /* transfer errCode */
    if (0 != obj_getErrorCode(method_host_obj)) {
        /* method error */
        VMState_setErrorCode(vm, PIKA_RES_ERR_RUNTIME_ERROR);
    }

    /* check try result */
    if (sub_try_info.try_result == TRY_RESULT_RAISE) {
        /* try error */
        VMState_setErrorCode(vm, PIKA_RES_ERR_RUNTIME_ERROR);
        if (vm->try_info->try_state == TRY_STATE_TOP) {
            vm->jmp = VM_JMP_RAISE;
        } else if (vm->try_info->try_state == TRY_STATE_INNER) {
            vm->try_info->try_result = TRY_RESULT_RAISE;
            goto exit;
        }
    }

    goto exit;
exit:
    if (NULL != method_arg) {
        arg_deinit(method_arg);
    }
    if (NULL != sub_locals) {
        obj_deinit(sub_locals);
    }
    if (NULL != host_arg) {
        arg_deinit(host_arg);
    }
    if (NULL != method_host_obj && isClass) {
        /* class method */
        obj_deinit(method_host_obj);
    }

    return return_arg;
}

static char* __get_transferd_str(Args* buffs, char* str, size_t* iout_p) {
    char* str_rep = strsReplace(buffs, str, "\\n", "\n");
    str_rep = strsReplace(buffs, str_rep, "\\r", "\r");
    str_rep = strsReplace(buffs, str_rep, "\\t", "\t");

    char* transfered_str = args_getBuff(buffs, strGetSize(str_rep));
    size_t i_out = 0;
    for (size_t i = 0; i < strGetSize(str_rep); i++) {
        /* eg. replace '\x33' to '3' */
        if ((str_rep[i] == '\\') && (str_rep[i + 1] == 'x')) {
            char hex_str[] = "0x00";
            hex_str[2] = str_rep[i + 2];
            hex_str[3] = str_rep[i + 3];
            char hex = (char)strtol(hex_str, NULL, 0);
            transfered_str[i_out++] = hex;
            i += 3;
            continue;
        }
        /* normal char */
        transfered_str[i_out++] = str_rep[i];
    }
    *iout_p = i_out;
    return transfered_str;
}

static Arg* VM_instruction_handler_STR(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    if (strIsContain(data, '\\')) {
        Args buffs = {0};
        size_t i_out = 0;
        char* transfered_str = __get_transferd_str(&buffs, data, &i_out);
        Arg* return_arg = arg_ret_reg;
        return_arg = arg_setStr(return_arg, "", transfered_str);
        strsDeinit(&buffs);
        return return_arg;
    }
    return arg_setStr(arg_ret_reg, "", data);
}

static Arg* VM_instruction_handler_BYT(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    if (strIsContain(data, '\\')) {
        Args buffs = {0};
        size_t i_out = 0;
        char* transfered_str = __get_transferd_str(&buffs, data, &i_out);
        Arg* return_arg = New_arg(NULL);
        return_arg =
            arg_setBytes(return_arg, "", (uint8_t*)transfered_str, i_out);
        strsDeinit(&buffs);
        return return_arg;
    }
    return arg_newBytes((uint8_t*)data, strGetSize(data));
}

static Arg* VM_instruction_handler_OUT(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    arg_newReg(outArg_reg, PIKA_ARG_BUFF_SIZE);
    Arg* outArg = stack_popArg(&vm->stack, &outArg_reg);
    // Arg* outArg = stack_popArg_alloc(&vm->stack);
    ArgType outArg_type = arg_getType(outArg);

    if (_checkLReg(data)) {
        uint8_t index = _getLRegIndex(data);
        if (argType_isObject(outArg_type)) {
            PikaObj* obj = arg_getPtr(outArg);
            VMState_setLReg(vm, index, obj);
            arg_deinit(outArg);
        }
        return NULL;
    }

    PikaObj* hostObj = vm->locals;
    /* match global_list */
    if (args_isArgExist(vm->locals->list, "__gl")) {
        char* global_list = args_getStr(vm->locals->list, "__gl");
        /* use a arg as buff */
        Arg* global_list_arg = arg_newStr(global_list);
        char* global_list_buff = arg_getStr(global_list_arg);
        /* for each arg arg in global_list */
        char token_buff[32] = {0};
        for (int i = 0; i < strCountSign(global_list, ',') + 1; i++) {
            char* global_arg = strPopToken(token_buff, global_list_buff, ',');
            /* matched global arg, hostObj set to global */
            if (strEqu(global_arg, data)) {
                hostObj = vm->globals;
            }
        }
        arg_deinit(global_list_arg);
    }
    /* use RunAs object */
    if (args_isArgExist(vm->locals->list, "__runAs")) {
        hostObj = args_getPtr(vm->locals->list, "__runAs");
    }
    /* set free object to nomal object */
    if (ARG_TYPE_OBJECT_NEW == outArg_type) {
        arg_setType(outArg, ARG_TYPE_OBJECT);
    }

    /* ouput arg to locals */
    obj_setArg_noCopy(hostObj, data, outArg);
    return NULL;
}

/* run as */
static Arg* VM_instruction_handler_RAS(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    if (strEqu(data, "$origin")) {
        /* use origin object to run */
        obj_removeArg(vm->locals, "__runAs");
        return NULL;
    }
    /* use "data" object to run */
    PikaObj* runAs = obj_getObj(vm->locals, data);
    args_setRef(vm->locals->list, "__runAs", runAs);
    return NULL;
}

static Arg* VM_instruction_handler_NUM(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    Arg* numArg = arg_ret_reg;
    /* hex */
    if (data[1] == 'x' || data[1] == 'X') {
        return arg_setInt(numArg, "", strtol(data, NULL, 0));
    }
    if (data[1] == 'o' || data[1] == 'O') {
        char strtol_buff[10] = {0};
        strtol_buff[0] = '0';
        __platform_memcpy(strtol_buff + 1, data + 2, strGetSize(data) - 2);
        return arg_setInt(numArg, "", strtol(strtol_buff, NULL, 0));
    }
    if (data[1] == 'b' || data[1] == 'B') {
        char strtol_buff[10] = {0};
        __platform_memcpy(strtol_buff, data + 2, strGetSize(data) - 2);
        return arg_setInt(numArg, "", strtol(strtol_buff, NULL, 2));
    }
    /* float */
    if (strIsContain(data, '.') ||
        (strIsContain(data, 'e') || strIsContain(data, 'E'))) {
        return arg_setFloat(numArg, "", strtod(data, NULL));
    }
    /* int */
    return arg_setInt(numArg, "", fast_atoi(data));
}

static Arg* VM_instruction_handler_JMP(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    vm->jmp = fast_atoi(data);
    return NULL;
}

static Arg* VM_instruction_handler_SER(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    vm->try_error_code = fast_atoi(data);
    return NULL;
}

static Arg* VM_instruction_handler_JEZ(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    int thisBlockDeepth;
    thisBlockDeepth = VMState_getBlockDeepthNow(vm);
    int jmp_expect = fast_atoi(data);
    arg_newReg(pika_assertArg_reg, PIKA_ARG_BUFF_SIZE);
    Arg* pika_assertArg = stack_popArg(&(vm->stack), &pika_assertArg_reg);
    int pika_assert = 0;
    if (NULL != pika_assertArg) {
        pika_assert = arg_getInt(pika_assertArg);
    }
    arg_deinit(pika_assertArg);
    vm->ireg[thisBlockDeepth] = !pika_assert;

    if (0 == pika_assert) {
        /* jump */
        vm->jmp = jmp_expect;
    }

    /* restore loop deepth */
    if (2 == jmp_expect && 0 == pika_assert) {
        int block_deepth_now = VMState_getBlockDeepthNow(vm);
        vm->loop_deepth = block_deepth_now;
    }

    return NULL;
}

static uint8_t VMState_getInputArgNum(VMState* vm) {
    InstructUnit* ins_unit_now = VMState_getInstructNow(vm);
    uint8_t invoke_deepth_this = instructUnit_getInvokeDeepth(ins_unit_now);
    int32_t pc_this = vm->pc;
    uint8_t num = 0;
    while (1) {
        ins_unit_now--;
        pc_this -= instructUnit_getSize(ins_unit_now);
        uint8_t invode_deepth = instructUnit_getInvokeDeepth(ins_unit_now);
        if (pc_this < 0) {
            break;
        }
        if (invode_deepth == invoke_deepth_this + 1) {
            num++;
        }
        if (instructUnit_getIsNewLine(ins_unit_now)) {
            break;
        }
        if (invode_deepth <= invoke_deepth_this) {
            break;
        }
    }
    return num;
}

void operatorInfo_init(OperatorInfo* info,
                       PikaObj* self,
                       VMState* vm,
                       char* data,
                       Arg* arg_ret_reg) {
    info->opt = data;
    info->res = arg_ret_reg;
    info->t1 = arg_getType(info->a1);
    info->t2 = arg_getType(info->a2);
    info->vm = vm;
    if (info->t1 == ARG_TYPE_INT) {
        info->i1 = arg_getInt(info->a1);
        info->f1 = (float)info->i1;
    } else if (info->t1 == ARG_TYPE_FLOAT) {
        info->f1 = arg_getFloat(info->a1);
        info->i1 = (int)info->f1;
    }
    if (info->t2 == ARG_TYPE_INT) {
        info->i2 = arg_getInt(info->a2);
        info->f2 = (float)info->i2;
    } else if (info->t2 == ARG_TYPE_FLOAT) {
        info->f2 = arg_getFloat(info->a2);
        info->i2 = (int)info->f2;
    }
}

static void _OPT_ADD(OperatorInfo* op) {
    if (argType_isObject(op->t1)) {
        if (!argType_isObject(op->t2)) {
            VMState_setErrorCode(op->vm, PIKA_RES_ERR_OPERATION_FAILED);
            __platform_printf("TypeError: unsupported operand +\n");
            op->res = NULL;
            return;
        }
        PikaObj* obj1 = arg_getPtr(op->a1);
        Arg* method_add = obj_getMethodArg(obj1, "__add__");
        if (NULL == method_add) {
            VMState_setErrorCode(op->vm, PIKA_RES_ERR_OPERATION_FAILED);
            __platform_printf("TypeError: unsupported operand +\n");
            op->res = NULL;
            return;
        }
        arg_deinit(method_add);
        PikaObj* obj2 = arg_getPtr(op->a2);
        obj_setPtr(obj1, "__others", obj2);
        /* clang-format off */
        PIKA_PYTHON(
        __res = __add__(__others)
        )
        /* clang-format on */
        const uint8_t bytes[] = {
            0x0c, 0x00, /* instruct array size */
            0x10, 0x81, 0x01, 0x00, 0x00, 0x02, 0x0a, 0x00, 0x00, 0x04, 0x12,
            0x00,
            /* instruct array */
            0x18, 0x00, /* const pool size */
            0x00, 0x5f, 0x5f, 0x6f, 0x74, 0x68, 0x65, 0x72, 0x73, 0x00, 0x5f,
            0x5f, 0x61, 0x64, 0x64, 0x5f, 0x5f, 0x00, 0x5f, 0x5f, 0x72, 0x65,
            0x73, 0x00, /* const pool */
        };
        pikaVM_runByteCode(obj1, (uint8_t*)bytes);
        Arg* __res = arg_copy(obj_getArg(obj1, "__res"));
        op->res = __res;
        obj_removeArg(obj1, "__others");
        obj_removeArg(obj1, "__res");
        return;
    }

    if ((op->t1 == ARG_TYPE_STRING) && (op->t2 == ARG_TYPE_STRING)) {
        char* num1_s = NULL;
        char* num2_s = NULL;
        Args str_opt_buffs = {0};
        num1_s = arg_getStr(op->a1);
        num2_s = arg_getStr(op->a2);
        char* opt_str_out = strsAppend(&str_opt_buffs, num1_s, num2_s);
        op->res = arg_setStr(op->res, "", opt_str_out);
        strsDeinit(&str_opt_buffs);
        return;
    }
    /* match float */
    if ((op->t1 == ARG_TYPE_FLOAT) || op->t2 == ARG_TYPE_FLOAT) {
        op->res = arg_setFloat(op->res, "", op->f1 + op->f2);
        return;
    }
    /* int is default */
    op->res = arg_setInt(op->res, "", op->i1 + op->i2);
    return;
}

static void _OPT_SUB(OperatorInfo* op) {
    if (argType_isObject(op->t1)) {
        if (!argType_isObject(op->t2)) {
            VMState_setErrorCode(op->vm, PIKA_RES_ERR_OPERATION_FAILED);
            __platform_printf("TypeError: unsupported operand +\n");
            op->res = NULL;
            return;
        }
        PikaObj* obj1 = arg_getPtr(op->a1);
        Arg* method_sub = obj_getMethodArg(obj1, "__sub__");
        if (NULL == method_sub) {
            VMState_setErrorCode(op->vm, PIKA_RES_ERR_OPERATION_FAILED);
            __platform_printf("TypeError: unsupported operand +\n");
            op->res = NULL;
            return;
        }
        arg_deinit(method_sub);
        PikaObj* obj2 = arg_getPtr(op->a2);
        obj_setPtr(obj1, "__others", obj2);
        /* clang-format off */
        PIKA_PYTHON(
        __res = __sub__(__others)
        )
        /* clang-format on */
        const uint8_t bytes[] = {
            0x0c, 0x00, /* instruct array size */
            0x10, 0x81, 0x01, 0x00, 0x00, 0x02, 0x0a, 0x00, 0x00, 0x04, 0x12,
            0x00,
            /* instruct array */
            0x18, 0x00, /* const pool size */
            0x00, 0x5f, 0x5f, 0x6f, 0x74, 0x68, 0x65, 0x72, 0x73, 0x00, 0x5f,
            0x5f, 0x73, 0x75, 0x62, 0x5f, 0x5f, 0x00, 0x5f, 0x5f, 0x72, 0x65,
            0x73, 0x00, /* const pool */
        };
        pikaVM_runByteCode(obj1, (uint8_t*)bytes);
        Arg* __res = arg_copy(obj_getArg(obj1, "__res"));
        op->res = __res;
        obj_removeArg(obj1, "__others");
        obj_removeArg(obj1, "__res");
        return;
    }
    if (op->t2 == ARG_TYPE_NONE) {
        if (op->t1 == ARG_TYPE_INT) {
            op->res = arg_setInt(op->res, "", -op->i1);
            return;
        }
        if (op->t1 == ARG_TYPE_FLOAT) {
            op->res = arg_setFloat(op->res, "", -op->f1);
            return;
        }
    }
    if ((op->t1 == ARG_TYPE_FLOAT) || op->t2 == ARG_TYPE_FLOAT) {
        op->res = arg_setFloat(op->res, "", op->f1 - op->f2);
        return;
    }
    op->res = arg_setInt(op->res, "", op->i1 - op->i2);
    return;
}

static void _OPT_EQU(OperatorInfo* op) {
    int8_t is_equ = -1;
    if (op->t1 == ARG_TYPE_NONE && op->t2 == ARG_TYPE_NONE) {
        is_equ = 1;
        goto exit;
    }
    /* type not equl, and type is not int or float */
    if (op->t1 != op->t2) {
        if ((op->t1 != ARG_TYPE_FLOAT) && (op->t1 != ARG_TYPE_INT)) {
            is_equ = 0;
            goto exit;
        }
        if ((op->t2 != ARG_TYPE_FLOAT) && (op->t2 != ARG_TYPE_INT)) {
            is_equ = 0;
            goto exit;
        }
    }
    /* string compire */
    if (op->t1 == ARG_TYPE_STRING) {
        is_equ = strEqu(arg_getStr(op->a1), arg_getStr(op->a2));
        goto exit;
    }
    /* bytes compire */
    if (op->t1 == ARG_TYPE_BYTES) {
        if (arg_getBytesSize(op->a1) != arg_getBytesSize(op->a2)) {
            is_equ = 0;
            goto exit;
        }
        is_equ = 1;
        for (size_t i = 0; i < arg_getBytesSize(op->a1); i++) {
            if (arg_getBytes(op->a1)[i] != arg_getBytes(op->a2)[i]) {
                is_equ = 0;
                goto exit;
            }
        }
        goto exit;
    }
    /* default: int and float */
    is_equ = ((op->f1 - op->f2) * (op->f1 - op->f2) < (float)0.000001);
    goto exit;
exit:
    if (strEqu("==", op->opt)) {
        op->res = arg_setInt(op->res, "", is_equ);
    } else {
        op->res = arg_setInt(op->res, "", !is_equ);
    }
    return;
}

static void _OPT_POW(OperatorInfo* op) {
    if (op->t1 == ARG_TYPE_INT && op->t2 == ARG_TYPE_INT) {
        int res = 1;
        for (int i = 0; i < op->i2; i++) {
            res = res * op->i1;
        }
        op->res = arg_setInt(op->res, "", res);
        return;
    } else if (op->t1 == ARG_TYPE_FLOAT && op->t2 == ARG_TYPE_INT) {
        float res = 1;
        for (int i = 0; i < op->i2; i++) {
            res = res * op->f1;
        }
        op->res = arg_setFloat(op->res, "", res);
        return;
    } else {
#if PIKA_MATH_ENABLE
        float res = 1;
        res = pow(op->f1, op->f2);
        op->res = arg_setFloat(op->res, "", res);
        return;
#else
        VMState_setErrorCode(op->vm, PIKA_RES_ERR_OPERATION_FAILED);
        __platform_printf(
            "Operation float ** float is not enabled, please set "
            "PIKA_MATH_ENABLE\n");
#endif
    }
}

static Arg* VM_instruction_handler_OPT(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    OperatorInfo op = {0};
    op.num = VMState_getInputArgNum(vm);
    arg_newReg(arg_reg1, PIKA_ARG_BUFF_SIZE);
    arg_newReg(arg_reg2, PIKA_ARG_BUFF_SIZE);
    if (op.num == 2) {
        /* tow input */
        op.a2 = stack_popArg(&(vm->stack), &arg_reg2);
        op.a1 = stack_popArg(&(vm->stack), &arg_reg1);
    } else if (op.num == 1) {
        /* only one input */
        op.a2 = stack_popArg(&(vm->stack), &arg_reg2);
        op.a1 = NULL;
    }
    /* init operator info */
    operatorInfo_init(&op, self, vm, data, arg_ret_reg);
    if (strEqu("+", data)) {
        _OPT_ADD(&op);
        goto exit;
    }
    if (strEqu("-", data)) {
        _OPT_SUB(&op);
        goto exit;
    }
    if (strEqu("*", data)) {
        if ((op.t1 == ARG_TYPE_FLOAT) || op.t2 == ARG_TYPE_FLOAT) {
            op.res = arg_setFloat(op.res, "", op.f1 * op.f2);
            goto exit;
        }
        op.res = arg_setInt(op.res, "", op.i1 * op.i2);
        goto exit;
    }
    if (strEqu(" in ", data)) {
        if (op.t1 == ARG_TYPE_STRING && op.t2 == ARG_TYPE_STRING) {
            if (strstr(arg_getStr(op.a2), arg_getStr(op.a1))) {
                op.res = arg_setInt(op.res, "", 1);
            } else {
                op.res = arg_setInt(op.res, "", 0);
            }
            goto exit;
        }
        VMState_setErrorCode(vm, PIKA_RES_ERR_OPERATION_FAILED);
        args_setSysOut(vm->locals->list,
                       "Operation 'in' is not supported for this "
                       "type\n");
        op.res = NULL;
        goto exit;
    }
    if (strEqu("/", data)) {
        if (0 == op.f2) {
            VMState_setErrorCode(vm, PIKA_RES_ERR_OPERATION_FAILED);
            args_setSysOut(vm->locals->list,
                           "ZeroDivisionError: division by zero");
            op.res = NULL;
            goto exit;
        }
        op.res = arg_setFloat(op.res, "", op.f1 / op.f2);
        goto exit;
    }
    if (strEqu("<", data)) {
        op.res = arg_setInt(op.res, "", op.f1 < op.f2);
        goto exit;
    }
    if (strEqu(">", data)) {
        op.res = arg_setInt(op.res, "", op.f1 > op.f2);
        goto exit;
    }
    if (strEqu("%", data)) {
        if ((op.t1 == ARG_TYPE_INT) && (op.t2 == ARG_TYPE_INT)) {
            op.res = arg_setInt(op.res, "", op.i1 % op.i2);
            goto exit;
        }
        VMState_setErrorCode(vm, PIKA_RES_ERR_OPERATION_FAILED);
        __platform_printf(
            "TypeError: unsupported operand type(s) for %: 'float'\n");
        op.res = NULL;
        goto exit;
    }
    if (strEqu("**", data)) {
        _OPT_POW(&op);
        goto exit;
    }
    if (strEqu("//", data)) {
        if ((op.t1 == ARG_TYPE_INT) && (op.t2 == ARG_TYPE_INT)) {
            op.res = arg_setInt(op.res, "", op.i1 / op.i2);
            goto exit;
        }
        VMState_setErrorCode(vm, PIKA_RES_ERR_OPERATION_FAILED);
        __platform_printf(
            "TypeError: unsupported operand type(s) for //: 'float'\n");
        op.res = NULL;
        goto exit;
    }
    if (strEqu("==", data) || strEqu("!=", data)) {
        _OPT_EQU(&op);
        goto exit;
    }
    if (strEqu(" is ", data)) {
        if (argType_isObject(op.t1) && argType_isObject(op.t2)) {
            if (arg_getPtr(op.a1) == arg_getPtr(op.a2)) {
                op.res = arg_setInt(op.res, "", 1);
            } else {
                op.res = arg_setInt(op.res, "", 0);
            }
            goto exit;
        }
        op.opt = "==";
        _OPT_EQU(&op);
        goto exit;
    }
    if (strEqu(">=", data)) {
        op.res = arg_setInt(
            op.res, "",
            (op.f1 > op.f2) ||
                ((op.f1 - op.f2) * (op.f1 - op.f2) < (float)0.000001));
        goto exit;
    }
    if (strEqu("<=", data)) {
        op.res = arg_setInt(
            op.res, "",
            (op.f1 < op.f2) ||
                ((op.f1 - op.f2) * (op.f1 - op.f2) < (float)0.000001));
        goto exit;
    }
    if (strEqu("&", data)) {
        if ((op.t1 == ARG_TYPE_INT) && (op.t2 == ARG_TYPE_INT)) {
            op.res = arg_setInt(op.res, "", op.i1 & op.i2);
            goto exit;
        }
        VMState_setErrorCode(vm, PIKA_RES_ERR_OPERATION_FAILED);
        __platform_printf(
            "TypeError: unsupported operand type(s) for &: 'float'\n");
        op.res = NULL;
        goto exit;
    }
    if (strEqu("|", data)) {
        if ((op.t1 == ARG_TYPE_INT) && (op.t2 == ARG_TYPE_INT)) {
            op.res = arg_setInt(op.res, "", op.i1 | op.i2);
            goto exit;
        }
        VMState_setErrorCode(vm, PIKA_RES_ERR_OPERATION_FAILED);
        __platform_printf(
            "TypeError: unsupported operand type(s) for |: 'float'\n");
        op.res = NULL;
        goto exit;
    }
    if (strEqu("~", data)) {
        if (op.t2 == ARG_TYPE_INT) {
            op.res = arg_setInt(op.res, "", ~op.i2);
            goto exit;
        }
        VMState_setErrorCode(vm, PIKA_RES_ERR_OPERATION_FAILED);
        __platform_printf(
            "TypeError: unsupported operand type(s) for ~: 'float'\n");
        op.res = NULL;
        goto exit;
    }
    if (strEqu(">>", data)) {
        if ((op.t1 == ARG_TYPE_INT) && (op.t2 == ARG_TYPE_INT)) {
            op.res = arg_setInt(op.res, "", op.i1 >> op.i2);
            goto exit;
        }
        VMState_setErrorCode(vm, PIKA_RES_ERR_OPERATION_FAILED);
        __platform_printf(
            "TypeError: unsupported operand type(s) for >>: 'float'\n");
        op.res = NULL;
        goto exit;
    }
    if (strEqu("<<", data)) {
        if ((op.t1 == ARG_TYPE_INT) && (op.t2 == ARG_TYPE_INT)) {
            op.res = arg_setInt(op.res, "", op.i1 << op.i2);
            goto exit;
        }
        VMState_setErrorCode(vm, PIKA_RES_ERR_OPERATION_FAILED);
        __platform_printf(
            "TypeError: unsupported operand type(s) for <<: 'float'\n");
        op.res = NULL;
        goto exit;
    }
    if (strEqu(" and ", data)) {
        op.res = arg_setInt(op.res, "", op.i1 && op.i2);
        goto exit;
    }
    if (strEqu(" or ", data)) {
        op.res = arg_setInt(op.res, "", op.i1 || op.i2);
        goto exit;
    }
    if (strEqu(" not ", data)) {
        op.res = arg_setInt(op.res, "", !op.i2);
        goto exit;
    }
    if (strEqu(" import ", data)) {
        op.res = NULL;
        goto exit;
    }
exit:
    arg_deinit(op.a1);
    arg_deinit(op.a2);
    if (NULL != op.res) {
        return op.res;
    }
    return NULL;
}

static Arg* __VM_instruction_handler_DEF(PikaObj* self,
                                         VMState* vm,
                                         char* data,
                                         uint8_t is_class) {
    int thisBlockDeepth = VMState_getBlockDeepthNow(vm);

    PikaObj* hostObj = vm->locals;
    uint8_t is_in_class = 0;
    /* use RunAs object */
    if (args_isArgExist(vm->locals->list, "__runAs")) {
        hostObj = args_getPtr(vm->locals->list, "__runAs");
        is_in_class = 1;
    }
    int offset = 0;
    /* byteCode */
    while (1) {
        InstructUnit* ins_unit_now = VMState_getInstructWithOffset(vm, offset);
        if (!instructUnit_getIsNewLine(ins_unit_now)) {
            offset += instructUnit_getSize();
            continue;
        }
        if (instructUnit_getBlockDeepth(ins_unit_now) == thisBlockDeepth + 1) {
            if (is_in_class) {
                class_defineObjectMethod(hostObj, data, (Method)ins_unit_now,
                                         self, vm->bytecode_frame);
            } else {
                if (is_class) {
                    class_defineRunTimeConstructor(hostObj, data,
                                                   (Method)ins_unit_now, self,
                                                   vm->bytecode_frame);
                } else {
                    class_defineStaticMethod(hostObj, data,
                                             (Method)ins_unit_now, self,
                                             vm->bytecode_frame);
                }
            }
            break;
        }
        offset += instructUnit_getSize();
    }

    return NULL;
}

static Arg* VM_instruction_handler_DEF(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    return __VM_instruction_handler_DEF(self, vm, data, 0);
}

static Arg* VM_instruction_handler_CLS(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    return __VM_instruction_handler_DEF(self, vm, data, 1);
}

static Arg* VM_instruction_handler_RIS(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    Arg* err_arg = stack_popArg_alloc(&(vm->stack));
    PIKA_RES err = (PIKA_RES)arg_getInt(err_arg);
    VMState_setErrorCode(vm, err);
    arg_deinit(err_arg);
    /* raise jmp */
    if (vm->try_info->try_state == TRY_STATE_TOP) {
        vm->jmp = VM_JMP_RAISE;
    } else if (vm->try_info->try_state == TRY_STATE_INNER) {
        vm->try_info->try_result = TRY_RESULT_RAISE;
        return VM_instruction_handler_RET(self, vm, data, arg_ret_reg);
    }
    return NULL;
}

static Arg* VM_instruction_handler_ASS(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    arg_newReg(reg1, PIKA_ARG_BUFF_SIZE);
    arg_newReg(reg2, PIKA_ARG_BUFF_SIZE);
    Arg* arg1 = NULL;
    Arg* arg2 = NULL;
    Arg* res = NULL;
    int arg_num = VMState_getInputArgNum(vm);
    if (arg_num == 1) {
        arg1 = stack_popArg(&vm->stack, &reg1);
    }
    if (arg_num == 2) {
        arg2 = stack_popArg(&vm->stack, &reg2);
        arg1 = stack_popArg(&vm->stack, &reg1);
    }
    /* assert faild */
    if (arg_getType(arg1) == ARG_TYPE_INT && arg_getInt(arg1) == 0) {
        stack_pushArg(&vm->stack, arg_newInt(PIKA_RES_ERR_ASSERT));
        res = VM_instruction_handler_RIS(self, vm, data, arg_ret_reg);
        if (vm->try_info->try_state == TRY_STATE_NONE) {
            if (arg_num == 1) {
                __platform_printf("AssertionError\n", data);
            }
            if (arg_num == 2) {
                __platform_printf("AssertionError: %s\n", arg_getStr(arg2));
            }
        }
        goto exit;
    }
exit:
    arg_deinit(arg1);
    arg_deinit(arg2);
    return res;
}

static Arg* VM_instruction_handler_NEL(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    int thisBlockDeepth = VMState_getBlockDeepthNow(vm);
    if (0 == vm->ireg[thisBlockDeepth]) {
        /* set __else flag */
        vm->jmp = fast_atoi(data);
    }
    return NULL;
}

static Arg* VM_instruction_handler_DEL(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    if (_checkLReg(data)) {
        uint8_t reg_index = _getLRegIndex(data);
        VMState_delLReg(vm, reg_index);
        return NULL;
    }
    if (obj_isArgExist(vm->locals, data)) {
        obj_removeArg(vm->locals, data);
        return NULL;
    }
    if (obj_isArgExist(vm->globals, data)) {
        obj_removeArg(vm->globals, data);
        return NULL;
    }
    VMState_setErrorCode(vm, PIKA_RES_ERR_OPERATION_FAILED);
    __platform_printf("NameError: name '%s' is not defined\n", data);
    return NULL;
}

static Arg* VM_instruction_handler_EST(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    Arg* arg = obj_getArg(vm->locals, data);
    if (arg == NULL) {
        return arg_setInt(arg_ret_reg, "", 0);
    }
    if (ARG_TYPE_NONE == arg_getType(arg)) {
        return arg_setInt(arg_ret_reg, "", 0);
    }
    return arg_setInt(arg_ret_reg, "", 1);
}

static Arg* VM_instruction_handler_BRK(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    /* break jmp signal */
    vm->jmp = VM_JMP_BREAK;
    return NULL;
}

static Arg* VM_instruction_handler_CTN(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    /* continue jmp signal */
    vm->jmp = VM_JMP_CONTINUE;
    return NULL;
}

static Arg* VM_instruction_handler_GLB(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    Arg* global_list_buff = NULL;
    char* global_list = args_getStr(vm->locals->list, "__gl");
    /* create new global_list */
    if (NULL == global_list) {
        args_setStr(vm->locals->list, "__gl", data);
        goto exit;
    }
    /* append to exist global_list */
    global_list_buff = arg_newStr(global_list);
    global_list_buff = arg_strAppend(global_list_buff, ",");
    global_list_buff = arg_strAppend(global_list_buff, data);
    args_setStr(vm->locals->list, "__gl", arg_getStr(global_list_buff));
    goto exit;
exit:
    if (NULL != global_list_buff) {
        arg_deinit(global_list_buff);
    }
    return NULL;
}

static Arg* VM_instruction_handler_IMP(PikaObj* self,
                                       VMState* vm,
                                       char* data,
                                       Arg* arg_ret_reg) {
    /* the module is already imported, skip. */
    if (obj_isArgExist(self, data)) {
        return NULL;
    }
    /* find cmodule in root object */
    extern PikaObj* __pikaMain;
    if (obj_isArgExist(__pikaMain, data)) {
        obj_setArg(self, data, obj_getArg(__pikaMain, data));
        return NULL;
    }
    /* import module from '__lib' */
    if (0 == obj_importModule(self, data)) {
        return NULL;
    }
    VMState_setErrorCode(vm, PIKA_RES_ERR_ARG_NO_FOUND);
    __platform_printf("ModuleNotFoundError: No module named '%s'\r\n", data);
    return NULL;
}

const VM_instruct_handler VM_instruct_handler_table[__INSTRCUTION_CNT] = {
#define __INS_TABLE
#include "__instruction_table.cfg"
};

enum Instruct pikaVM_getInstructFromAsm(char* ins_str) {
#define __INS_COMPIRE
#include "__instruction_table.cfg"
    return NON;
}

static int pikaVM_runInstructUnit(PikaObj* self,
                                  VMState* vm,
                                  InstructUnit* ins_unit) {
    enum Instruct instruct = instructUnit_getInstruct(ins_unit);
    arg_newReg(ret_reg, PIKA_ARG_BUFF_SIZE);
    Arg* return_arg = &ret_reg;
    // char invode_deepth1_str[2] = {0};
    int32_t pc_next = vm->pc + instructUnit_getSize();
    char* data = VMState_getConstWithInstructUnit(vm, ins_unit);
    /* run instruct */
    pika_assert(NULL != vm->try_info);
    return_arg = VM_instruct_handler_table[instruct](self, vm, data, &ret_reg);
    if (NULL != return_arg) {
        stack_pushArg(&(vm->stack), return_arg);
    }
    goto nextLine;
nextLine:
    /* exit */
    if (VM_JMP_EXIT == vm->jmp) {
        pc_next = VM_PC_EXIT;
        goto exit;
    }
    /* break */
    if (VM_JMP_BREAK == vm->jmp) {
        pc_next = vm->pc + VMState_getAddrOffsetOfBreak(vm);
        goto exit;
    }
    /* continue */
    if (VM_JMP_CONTINUE == vm->jmp) {
        pc_next = vm->pc + VMState_getAddrOffsetOfContinue(vm);
        goto exit;
    }
    /* raise */
    if (VM_JMP_RAISE == vm->jmp) {
        int offset = VMState_getAddrOffsetOfRaise(vm);
        if (0 == offset) {
            /* can not found end of try, return */
            pc_next = VM_PC_EXIT;
            goto exit;
        }
        pc_next = vm->pc + offset;
        goto exit;
    }
    /* static jmp */
    if (vm->jmp != 0) {
        pc_next = vm->pc + VMState_getAddrOffsetFromJmp(vm);
        goto exit;
    }
    /* not jmp */
    pc_next = vm->pc + instructUnit_getSize();

    /* jump to next line */
    if (vm->error_code != 0) {
        while (1) {
            if (pc_next >= (int)VMState_getInstructArraySize(vm)) {
                pc_next = VM_PC_EXIT;
                goto exit;
            }
            InstructUnit* ins_next = instructArray_getByOffset(
                &vm->bytecode_frame->instruct_array, pc_next);
            if (instructUnit_getIsNewLine(ins_next)) {
                goto exit;
            }
            pc_next = pc_next + instructUnit_getSize();
        }
    }

    goto exit;
exit:
    vm->jmp = 0;
    /* reach the end */
    if (pc_next >= (int)VMState_getInstructArraySize(vm)) {
        return VM_PC_EXIT;
    }
    return pc_next;
}

VMParameters* pikaVM_runAsm(PikaObj* self, char* pikaAsm) {
    ByteCodeFrame bytecode_frame;
    byteCodeFrame_init(&bytecode_frame);
    byteCodeFrame_appendFromAsm(&bytecode_frame, pikaAsm);
    VMParameters* res = pikaVM_runByteCodeFrame(self, &bytecode_frame);
    byteCodeFrame_deinit(&bytecode_frame);
    return res;
}

static VMParameters* __pikaVM_runPyLines_or_byteCode(PikaObj* self,
                                                     char* py_lines,
                                                     uint8_t* bytecode) {
    uint8_t is_run_py;
    if (NULL != py_lines) {
        is_run_py = 1;
    } else if (NULL != bytecode) {
        is_run_py = 0;
    } else {
        return NULL;
    }
    Args buffs = {0};
    VMParameters* globals = NULL;
    ByteCodeFrame bytecode_frame_stack = {0};
    ByteCodeFrame* bytecode_frame_p = NULL;
    uint8_t is_use_heap_bytecode = 0;

    /*
     * the first obj_run, cache bytecode to heap, to support 'def' and 'class'
     */
    if (!args_isArgExist(self->list, "__first_bytecode")) {
        is_use_heap_bytecode = 1;
        /* load bytecode to heap */
        args_setHeapStruct(self->list, "__first_bytecode", bytecode_frame_stack,
                           byteCodeFrame_deinit);
        /* get bytecode_ptr from heap */
        bytecode_frame_p = args_getHeapStruct(self->list, "__first_bytecode");
    } else {
        /* not the first obj_run */
        /* save 'def' and 'class' to heap */
        if ((strIsStartWith(py_lines, "def ")) ||
            (strIsStartWith(py_lines, "class "))) {
            char* declear_name = strsGetFirstToken(&buffs, py_lines, ':');
            /* load bytecode to heap */
            args_setHeapStruct(self->list, declear_name, bytecode_frame_stack,
                               byteCodeFrame_deinit);
            /* get bytecode_ptr from heap */
            bytecode_frame_p = args_getHeapStruct(self->list, declear_name);
        } else {
            /* get bytecode_ptr from stack */
            bytecode_frame_p = &bytecode_frame_stack;
        }
    }

    /* load or generate byte code frame */
    if (is_run_py) {
        /* generate byte code */
        byteCodeFrame_init(bytecode_frame_p);
        if (1 == bytecodeFrame_fromLines(bytecode_frame_p, py_lines)) {
            __platform_printf("Error: Syntax error.\r\n");
            globals = NULL;
            goto exit;
        }
    } else {
        /* load bytecode */
        byteCodeFrame_loadByteCode(bytecode_frame_p, bytecode);
    }

    /* run byteCode */
    globals = pikaVM_runByteCodeFrame(self, bytecode_frame_p);
    goto exit;
exit:
    if (!is_use_heap_bytecode) {
        byteCodeFrame_deinit(&bytecode_frame_stack);
    }
    strsDeinit(&buffs);
    return globals;
}

VMParameters* pikaVM_runSingleFile(PikaObj* self, char* filename) {
    Args buffs = {0};
    Arg* file_arg = arg_loadFile(NULL, filename);
    pika_assert(NULL != file_arg);
    if (NULL == file_arg) {
        return NULL;
    }
    char* lines = (char*)arg_getBytes(file_arg);
    /* clear the void line */
    VMParameters* res = pikaVM_run(self, lines);
    arg_deinit(file_arg);
    strsDeinit(&buffs);
    return res;
}

VMParameters* pikaVM_run(PikaObj* self, char* py_lines) {
    return __pikaVM_runPyLines_or_byteCode(self, py_lines, NULL);
}

VMParameters* pikaVM_runByteCode(PikaObj* self, uint8_t* bytecode) {
    return __pikaVM_runPyLines_or_byteCode(self, NULL, bytecode);
}

void constPool_update(ConstPool* self) {
    self->content_start = (void*)arg_getContent(self->arg_buff);
}

void constPool_init(ConstPool* self) {
    self->arg_buff = arg_newStr("");
    constPool_update(self);
    self->content_offset_now = 0;
    self->size = strGetSize(constPool_getStart(self)) + 1;
    self->output_redirect_fun = NULL;
    self->output_f = NULL;
}

void constPool_deinit(ConstPool* self) {
    if (NULL != self->arg_buff) {
        arg_deinit(self->arg_buff);
    }
}

void constPool_append(ConstPool* self, char* content) {
    uint16_t size = strGetSize(content) + 1;
    if (NULL == self->output_redirect_fun) {
        self->arg_buff = arg_append(self->arg_buff, content, size);
    } else {
        self->output_redirect_fun(self, content);
    };
    constPool_update(self);
    self->size += size;
}

char* constPool_getNow(ConstPool* self) {
    if (self->content_offset_now >= self->size) {
        /* is the end */
        return NULL;
    }
    return (char*)((uintptr_t)constPool_getStart(self) +
                   (uintptr_t)(self->content_offset_now));
}

uint16_t constPool_getOffsetByData(ConstPool* self, char* data) {
    uint16_t ptr_befor = self->content_offset_now;
    /* set ptr_now to begin */
    self->content_offset_now = 0;
    uint16_t offset_out = 65535;
    while (1) {
        if (NULL == constPool_getNext(self)) {
            goto exit;
        }
        if (strEqu(data, constPool_getNow(self))) {
            offset_out = self->content_offset_now;
            goto exit;
        }
    }
exit:
    /* retore ptr_now */
    self->content_offset_now = ptr_befor;
    return offset_out;
}

char* constPool_getNext(ConstPool* self) {
    self->content_offset_now += strGetSize(constPool_getNow(self)) + 1;
    return constPool_getNow(self);
}

char* constPool_getByIndex(ConstPool* self, uint16_t index) {
    uint16_t ptr_befor = self->content_offset_now;
    /* set ptr_now to begin */
    self->content_offset_now = 0;
    for (uint16_t i = 0; i < index; i++) {
        constPool_getNext(self);
    }
    char* res = constPool_getNow(self);
    /* retore ptr_now */
    self->content_offset_now = ptr_befor;
    return res;
}

void constPool_print(ConstPool* self) {
    uint16_t ptr_befor = self->content_offset_now;
    /* set ptr_now to begin */
    self->content_offset_now = 0;
    while (1) {
        if (NULL == constPool_getNext(self)) {
            goto exit;
        }
        uint16_t offset = self->content_offset_now;
        __platform_printf("%d: %s\r\n", offset, constPool_getNow(self));
    }
exit:
    /* retore ptr_now */
    self->content_offset_now = ptr_befor;
    return;
}

void byteCodeFrame_init(ByteCodeFrame* self) {
    /* init to support append,
       if only load static bytecode,
       can not init */
    constPool_init(&(self->const_pool));
    instructArray_init(&(self->instruct_array));
}

void byteCodeFrame_loadByteCode(ByteCodeFrame* self, uint8_t* bytes) {
    uint16_t* ins_size_p = (uint16_t*)bytes;
    void* ins_start_p = (uint16_t*)(bytes + 2);
    uint16_t* const_size_p =
        (uint16_t*)((uintptr_t)ins_start_p + (uintptr_t)(*ins_size_p));
    self->instruct_array.size = *ins_size_p;
    self->instruct_array.content_start = ins_start_p;
    self->const_pool.size = *const_size_p;
    self->const_pool.content_start = (char*)((uintptr_t)const_size_p + 2);
}

void byteCodeFrame_deinit(ByteCodeFrame* self) {
    constPool_deinit(&(self->const_pool));
    instructArray_deinit(&(self->instruct_array));
}

void instructArray_init(InstructArray* self) {
    self->arg_buff = arg_newNull();
    instructArray_update(self);
    self->size = 0;
    self->content_offset_now = 0;
    self->output_redirect_fun = NULL;
    self->output_f = NULL;
}

void instructArray_deinit(InstructArray* self) {
    if (NULL != self->arg_buff) {
        arg_deinit(self->arg_buff);
    }
}

void instructArray_append(InstructArray* self, InstructUnit* ins_unit) {
    if (NULL == self->output_redirect_fun) {
        self->arg_buff =
            arg_append(self->arg_buff, ins_unit, instructUnit_getSize());
    } else {
        self->output_redirect_fun(self, ins_unit);
    };
    instructArray_update(self);
    self->size += instructUnit_getSize();
}

void instructUnit_init(InstructUnit* ins_unit) {
    ins_unit->deepth = 0;
    ins_unit->const_pool_index = 0;
    ins_unit->isNewLine_instruct = 0;
}

void instructArray_update(InstructArray* self) {
    self->content_start = (void*)arg_getContent(self->arg_buff);
}

InstructUnit* instructArray_getNow(InstructArray* self) {
    if (self->content_offset_now >= self->size) {
        /* is the end */
        return NULL;
    }
    return (InstructUnit*)((uintptr_t)instructArray_getStart(self) +
                           (uintptr_t)(self->content_offset_now));
}

InstructUnit* instructArray_getNext(InstructArray* self) {
    self->content_offset_now += instructUnit_getSize();
    return instructArray_getNow(self);
}

static char* instructUnit_getInstructStr(InstructUnit* self) {
#define __INS_GET_INS_STR
#include "__instruction_table.cfg"
    return "NON";
}

void instructUnit_print(InstructUnit* self) {
    if (instructUnit_getIsNewLine(self)) {
        __platform_printf("B%d\r\n", instructUnit_getBlockDeepth(self));
    }
    __platform_printf("%d %s #%d\r\n", instructUnit_getInvokeDeepth(self),
                      instructUnit_getInstructStr(self),
                      self->const_pool_index);
}

static void instructUnit_printWithConst(InstructUnit* self,
                                        ConstPool* const_pool) {
    // if (instructUnit_getIsNewLine(self)) {
    //     __platform_printf("B%d\r\n", instructUnit_getBlockDeepth(self));
    // }
    __platform_printf("%s %s \t\t(#%d)\r\n", instructUnit_getInstructStr(self),
                      constPool_getByOffset(const_pool, self->const_pool_index),
                      self->const_pool_index);
}

void instructArray_printWithConst(InstructArray* self, ConstPool* const_pool) {
    uint16_t offset_befor = self->content_offset_now;
    self->content_offset_now = 0;
    while (1) {
        InstructUnit* ins_unit = instructArray_getNow(self);
        if (NULL == ins_unit) {
            goto exit;
        }
        instructUnit_printWithConst(ins_unit, const_pool);
        instructArray_getNext(self);
    }
exit:
    self->content_offset_now = offset_befor;
    return;
}

void instructArray_print(InstructArray* self) {
    uint16_t offset_befor = self->content_offset_now;
    self->content_offset_now = 0;
    while (1) {
        InstructUnit* ins_unit = instructArray_getNow(self);
        if (NULL == ins_unit) {
            goto exit;
        }
        instructUnit_print(ins_unit);
        instructArray_getNext(self);
    }
exit:
    self->content_offset_now = offset_befor;
    return;
}

void instructArray_printAsArray(InstructArray* self) {
    uint16_t offset_befor = self->content_offset_now;
    self->content_offset_now = 0;
    uint8_t line_num = 12;
    uint16_t g_i = 0;
    uint8_t* ins_size_p = (uint8_t*)&self->size;
    __platform_printf("0x%02x, ", *(ins_size_p));
    __platform_printf("0x%02x, ", *(ins_size_p + (uintptr_t)1));
    __platform_printf("/* instruct array size */\n");
    while (1) {
        InstructUnit* ins_unit = instructArray_getNow(self);
        if (NULL == ins_unit) {
            goto exit;
        }
        for (int i = 0; i < (int)instructUnit_getSize(ins_unit); i++) {
            g_i++;
            __platform_printf("0x%02x, ", *((uint8_t*)ins_unit + (uintptr_t)i));
            if (g_i % line_num == 0) {
                __platform_printf("\n");
            }
        }
        instructArray_getNext(self);
    }
exit:
    __platform_printf("/* instruct array */\n");
    self->content_offset_now = offset_befor;
    return;
}

size_t byteCodeFrame_getSize(ByteCodeFrame* bf) {
    return bf->const_pool.size + bf->instruct_array.size;
}

void byteCodeFrame_print(ByteCodeFrame* self) {
    constPool_print(&(self->const_pool));
    instructArray_printWithConst(&(self->instruct_array), &(self->const_pool));
    __platform_printf("---------------\r\n");
    __platform_printf("byte code size: %d\r\n",
                      self->const_pool.size + self->instruct_array.size);
}

void VMState_solveUnusedStack(VMState* vm) {
    uint8_t top = stack_getTop(&(vm->stack));
    for (int i = 0; i < top; i++) {
        Arg* arg = stack_popArg_alloc(&(vm->stack));
        ArgType type = arg_getType(arg);
        if (type == ARG_TYPE_NONE) {
            arg_deinit(arg);
            continue;
        }
        if (vm->line_error_code != 0) {
            arg_deinit(arg);
            continue;
        }
        if (argType_isObject(type)) {
            char* res = obj_toStr(arg_getPtr(arg));
            __platform_printf("%s\r\n", res);
        } else if (type == ARG_TYPE_INT) {
            __platform_printf("%d\r\n", (int)arg_getInt(arg));
        } else if (type == ARG_TYPE_FLOAT) {
            __platform_printf("%f\r\n", arg_getFloat(arg));
        } else if (type == ARG_TYPE_STRING) {
            __platform_printf("'%s'\r\n", arg_getStr(arg));
        } else if (type == ARG_TYPE_BYTES) {
            arg_printBytes(arg);
        } else if (ARG_TYPE_POINTER == type ||
                   ARG_TYPE_METHOD_NATIVE_CONSTRUCTOR) {
            __platform_printf("%p\r\n", arg_getPtr(arg));
        }
        arg_deinit(arg);
    }
}

static VMParameters* __pikaVM_runByteCodeFrameWithState(
    PikaObj* self,
    VMParameters* locals,
    VMParameters* globals,
    ByteCodeFrame* bytecode_frame,
    uint16_t pc,
    TryInfo* try_info) {
    pika_assert(NULL != try_info);
    int size = bytecode_frame->instruct_array.size;
    /* locals is the local scope */
    VMState vm = {
        .bytecode_frame = bytecode_frame,
        .locals = locals,
        .globals = globals,
        .jmp = 0,
        .pc = pc,
        .loop_deepth = 0,
        .error_code = PIKA_RES_OK,
        .line_error_code = PIKA_RES_OK,
        .try_error_code = PIKA_RES_OK,
        .try_info = try_info,
        .ins_cnt = 0,
    };
    stack_init(&(vm.stack));
    VMState_initReg(&vm);
    while (vm.pc < size) {
        if (vm.pc == VM_PC_EXIT) {
            break;
        }
        InstructUnit* this_ins_unit = VMState_getInstructNow(&vm);
        if (instructUnit_getIsNewLine(this_ins_unit)) {
            VMState_solveUnusedStack(&vm);
            stack_reset(&(vm.stack));
            vm.error_code = 0;
            vm.line_error_code = 0;
        }
        vm.pc = pikaVM_runInstructUnit(self, &vm, this_ins_unit);
        vm.ins_cnt++;
#if PIKA_INSTRUCT_HOOK_ENABLE
        if (vm.ins_cnt % PIKA_INSTRUCT_HOOK_PERIOD == 0) {
            __pks_hook_instruct();
        }
#endif
        if (0 != vm.error_code) {
            vm.line_error_code = vm.error_code;
            InstructUnit* head_ins_unit = this_ins_unit;
            /* get first ins of a line */
            while (1) {
                if (instructUnit_getIsNewLine(head_ins_unit)) {
                    break;
                }
                head_ins_unit--;
            }
            if (vm.try_info->try_state) {
                vm.try_error_code = vm.error_code;
            }
            /* print inses of a line */
            if (!vm.try_info->try_state) {
                while (1) {
                    if (head_ins_unit != this_ins_unit) {
                        __platform_printf("   ");
                    } else {
                        __platform_printf(" -> ");
                    }
                    instructUnit_printWithConst(head_ins_unit,
                                                &(bytecode_frame->const_pool));
                    head_ins_unit++;
                    if (head_ins_unit > this_ins_unit) {
                        break;
                    }
                }
            }
            __platform_error_handle();
            vm.error_code = 0;
        }
    }
    VMState_solveUnusedStack(&vm);
    stack_deinit(&(vm.stack));
    return locals;
}

VMParameters* pikaVM_runByteCodeFrame(PikaObj* self,
                                      ByteCodeFrame* byteCode_frame) {
    TryInfo try_info = {.try_state = TRY_STATE_NONE,
                        .try_result = TRY_RESULT_NONE};
    try_info.try_state = TRY_STATE_NONE;
    return __pikaVM_runByteCodeFrameWithState(self, self, self, byteCode_frame,
                                              0, &try_info);
}

InstructUnit* instructArray_getByOffset(InstructArray* self, int32_t offset) {
    return (InstructUnit*)((uintptr_t)instructArray_getStart(self) +
                           (uintptr_t)offset);
}

void constPool_printAsArray(ConstPool* self) {
    uint8_t* const_size_str = (uint8_t*)&(self->size);
    __platform_printf("0x%02x, ", *(const_size_str));
    __platform_printf("0x%02x, ", *(const_size_str + (uintptr_t)1));
    __platform_printf("/* const pool size */\n");
    uint16_t ptr_befor = self->content_offset_now;
    uint8_t line_num = 12;
    uint16_t g_i = 0;
    /* set ptr_now to begin */
    self->content_offset_now = 0;
    __platform_printf("0x00, ");
    while (1) {
        if (NULL == constPool_getNext(self)) {
            goto exit;
        }
        char* data_each = constPool_getNow(self);
        /* todo start */
        for (uint32_t i = 0; i < strGetSize(data_each) + 1; i++) {
            __platform_printf("0x%02x, ", *(data_each + (uintptr_t)i));
            g_i++;
            if (g_i % line_num == 0) {
                __platform_printf("\n");
            }
        }
        /* todo end */
    }
exit:
    /* retore ptr_now */
    __platform_printf("/* const pool */\n");
    self->content_offset_now = ptr_befor;
    return;
}

void byteCodeFrame_printAsArray(ByteCodeFrame* self) {
    __platform_printf("const uint8_t bytes[] = {\n");
    instructArray_printAsArray(&(self->instruct_array));
    constPool_printAsArray(&(self->const_pool));
    __platform_printf("};\n");
    __platform_printf("pikaVM_runByteCode(self, (uint8_t*)bytes);\n");
}

PikaObj* pikaVM_runFile(PikaObj* self, char* file_name) {
    Args buffs = {0};
    char* module_name = strsCopy(&buffs, file_name);
    strPopLastToken(module_name, '.');

    __platform_printf("(pikascript) pika compiler:\r\n");
    PikaMaker* maker = New_PikaMaker();
    pikaMaker_compileModuleWithDepends(maker, module_name);
    pikaMaker_linkCompiledModules(maker, "pikaModules_cache.py.a");
    obj_deinit(maker);
    __platform_printf("(pikascript) all succeed.\r\n\r\n");

    pikaMemMaxReset();
    Obj_linkLibraryFile(self, "pikascript-api/pikaModules_cache.py.a");
    self = pikaVM_runSingleFile(self, file_name);
    strsDeinit(&buffs);
    return self;
}
