#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JGadget/binary.h"
#include "JSystem/JGadget/define.h"
#include <stdint.h>

#if DEBUG
static void dummyString() {
    // probably some stripped function that called JUT_ASSERT here
    DEAD_STRING("Halt");
}
#endif

const void* JGadget::binary::parseVariableUInt_16_32_following(const void* pBuffer, u32* pu32First, u32* pu32Second,
                                                               JGadget::binary::TEBit* pTEBit) {
    u16* pu16 = (u16*)pBuffer;
    JUT_ASSERT(122, pu16!=NULL);
    JUT_ASSERT(123, pu32First!=NULL);
    JUT_ASSERT(124, pu32Second!=NULL);

    JGadget::binary::TEBit spC;
    if (pTEBit == NULL) {
        pTEBit = &spC;
    }

    u32 var_r30 = *pu16;
    if ((var_r30 & 0x8000) == 0) {
        pTEBit->value = 0x10;

        *pu32First = var_r30;
        pu16++;
        *pu32Second =  *pu16;

        return pu16 + 1;
    } else {
        pTEBit->value = 0x20;

        var_r30 &= 0x7FFF;
        var_r30 <<= 16;
        pu16++;
        var_r30 |= *pu16;

        *pu32First = var_r30;
        pu16++;
        *pu32Second = *(u32*)pu16;

        return pu16 + 2;
    }
}

JGadget::binary::TParse_header_block::~TParse_header_block() {}

bool JGadget::binary::TParse_header_block::parse_next(const void** ppData_inout, u32 idx) {
    u32 uBlock, uData;
    
    if (ppData_inout == NULL || *ppData_inout == NULL) {
        JGADGET_WARNMSG(172, "data not specified");
        return false;
    }

    bool var_r29 = true;
    var_r29 = parseHeader_next(ppData_inout, &uBlock, idx) && var_r29;

    if (!(idx & 1) && !var_r29) {
        return var_r29;
    }
    
    while (uBlock > 0) {
        const void* p = *ppData_inout;
        var_r29 = parseBlock_next(ppData_inout, &uData, idx) && var_r29;

        JUT_ASSERT(192, std::uintptr_t(*ppData_inout)==std::uintptr_t(p)+uData);

        if ((idx & 2) == 0 && !var_r29) {
            return var_r29;
        }
        uBlock--;
    }

    return var_r29;
}
