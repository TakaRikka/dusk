#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JGadget/binary.h"

const void*
JGadget::binary::parseVariableUInt_16_32_following(void const* buffer, u32* param_1, u32* param_2,
                                                   JGadget::binary::TEBit* param_3) {
    JGadget::binary::TEBit temp;
    if (param_3 == NULL) {
        param_3 = &temp;
    }
    u32 uVar1 = *(u16*)buffer;
    const void* rv;
    if ((uVar1 & 0x8000) == 0) {
        param_3->value = 0x10;
        *param_1 = uVar1;
        *param_2 =  *(u16*)((u8*)buffer + 2);
        rv = (u8*)buffer + 4;
    } else {
        param_3->value = 0x20;
        uVar1 <<= 16;
        uVar1 &= 0x7fff0000;
        uVar1 |= *(u16*)((u8*)buffer + 2);
        *param_1 = uVar1;
        *param_2 = *(u32*)((u8*)buffer + 4);
        rv = (u8*)buffer + 8;
    }
    return rv;
}

JGadget::binary::TParse_header_block::~TParse_header_block() {
}

bool JGadget::binary::TParse_header_block::parse_next(void const** ptrLocation, u32 idx) {
    u32 headerEnd, blockEnd;
    
    if ((ptrLocation == NULL) || (*ptrLocation == NULL)) {
        return false;
    }
    bool check, checkLastBlock;
    checkLastBlock = check = false;

    check = checkNext(ptrLocation, &headerEnd, idx);
    
    checkLastBlock = check;
    if (!(idx & 1) && (check == false)) {
        return check;
    }
    
    while (headerEnd > 0) {
        check = false;
        if (parseBlock_next(ptrLocation, &blockEnd, idx) && checkLastBlock) {
            check = true;
        }
        checkLastBlock = check;
        if (((idx & 2) == 0) && (check == false)) {
            return check;
        }
        headerEnd--;
    }
    return checkLastBlock;
}
