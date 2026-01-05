#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JAudio2/JASRegisterParam.h"
#include "JSystem/JSupport/JSupport.h"

JASRegisterParam::JASRegisterParam() {
    for (int i = 0; i < 14; i++) {
        field_0x0[i] = 0;
    }
}

void JASRegisterParam::init() {
    for (int i = 0; i < 14; i++) {
        field_0x0[i] = 0;
    }
}


void JASRegisterParam::write(JASRegisterParam::RegID param_0, u32 param_1) {
    if (param_0 < 14) {
        if (param_0 <= 2) {
            field_0x0[param_0] = (u8)param_1;
        } else {
            field_0x0[param_0] = param_1;
        }
        return;
    }

    switch (param_0) {
    case 32:
        field_0x0[0] = JSUHiByte(param_1);
        field_0x0[1] = JSULoByte(param_1);
        break;
    case 33:
        field_0x0[4] = JSUHiHalf(param_1);
        field_0x0[5] = JSULoHalf(param_1);
        break;
    case 34:
        field_0x0[4] = u8(field_0x0[4]);
        field_0x0[4] |= u16((param_1 & 0xff) << 8);
        break;
    case 35:
        field_0x0[4] &= 0xff00;
        field_0x0[4] |= u8(param_1);
        break;
    case 36:
        field_0x0[5] = u8(field_0x0[5]);
        field_0x0[5] |= u16((param_1 & 0xff) << 8);
        break;
    case 37:
        field_0x0[5] &= 0xff00;
        field_0x0[5] |= u8(param_1);
        break;
    case 38:
        field_0x0[6] = JSUHiHalf(param_1);
        field_0x0[7] = JSULoHalf(param_1);
        break;
    case 39:
        field_0x0[8] = JSUHiHalf(param_1);
        field_0x0[9] = JSULoHalf(param_1);
        break;
    case 40:
        field_0x0[10] = JSUHiHalf(param_1);
        field_0x0[11] = JSULoHalf(param_1);
        break;
    case 41:
        field_0x0[12] = JSUHiHalf(param_1);
        field_0x0[13] = JSULoHalf(param_1);
        break;
    default:
        JUT_WARN(92, "%s", "Warning: Unsupported REG id");
        break;
    }
}


u32 JASRegisterParam::read(JASRegisterParam::RegID param_0) {
    if (param_0 < 14) {
        return field_0x0[param_0];
    }
    u32 rv = 0;
    switch (param_0) {
    case 32:
        rv = ((field_0x0[0] & 0xff) << 8) | (field_0x0[1] & 0xff) ;
        break;
    case 33:
        rv = field_0x0[4];
        rv <<= 16;
        rv |= field_0x0[5];
        break;
    case 34:
        rv = JSUHiByte(field_0x0[4]);
        break;
    case 35:
        rv = JSULoByte(field_0x0[4]);
        break;
    case 36:
        rv = JSUHiByte(field_0x0[5]);
        break;
    case 37:
        rv = JSULoByte(field_0x0[5]);
        break;
    case 38:
        rv = field_0x0[6];
        rv <<= 16;
        rv |= field_0x0[7];
        break;
    case 39:
        rv = field_0x0[8];
        rv <<= 16;
        rv |= field_0x0[9];
        break;
    case 40:
        rv = field_0x0[10];
        rv <<= 16;
        rv |= field_0x0[11];
        break;
    case 41:
        // !@bug I think this should get field_0x0[12] and field_0x0[13]. Looks like a copy paste bug.
        rv = field_0x0[10];
        rv <<= 16;
        rv |= field_0x0[11];
        break;
    default:
        JUT_WARN(160, "%s", "Warning: Unsupported REG id");
        break;
    }
    return rv;
}
