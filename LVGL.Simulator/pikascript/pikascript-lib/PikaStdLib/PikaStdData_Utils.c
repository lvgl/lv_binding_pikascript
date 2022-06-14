#include "PikaStdData_Utils.h"

Arg* PikaStdData_Utils_int_to_bytes(PikaObj* self, int val) {
    if (val > 0xFF) {
        obj_setErrorCode(self, 1);
        __platform_printf(
            "OverflowError: cannot convert value larger than 0xFF to "
            "bytes\r\n");
        return arg_setNull(NULL);
    }
    uint8_t val_bytes = (uint8_t)val;
    return arg_setBytes(NULL, "", &val_bytes, 1);
}
