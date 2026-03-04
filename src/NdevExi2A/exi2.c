#include "NdevExi2A/exi2.h"

#include <revolution/os.h>

#define IS_TRUE(x) ((x) != FALSE)
#define IS_FALSE(x) !IS_TRUE(x)
#define ROUND_UP(x, align) (((x) + (align)-1) & (-(align)))

BOOL __EXI2Select() {
    u32 regs = __EXIRegs[10];
    regs &= 0x405;
    regs |= 0xC0;
    __EXIRegs[10] = regs;
    return TRUE;
}

BOOL __EXI2Deselect() {
    u32 regs = __EXIRegs[10];
    __EXIRegs[10] = regs & 0x405;
    return TRUE;
}

BOOL __EXI2Sync() {
    while (__EXIRegs[13] & 0x1) {
    }
    return TRUE;
}

BOOL __EXI2Imm(u32* param_0, int width, int param_2) {
    int i1; // r31
    int i2; // r30
    u32 var_r29;
    u8* var_r28;
    u32 var_r25;

    if (param_2 != 0) {
        var_r29 = 0;
        for (i1 = 0; i1 < width; i1++) {
            var_r29 |= ((u8*)param_0)[i1] << ((3 - i1) << 3);
        }
        __EXIRegs[14] = var_r29;
    }

    __EXIRegs[13] = 0x1 | (param_2 << 2) | ((width - 1) << 4);
    __EXI2Sync();

    if (param_2 == 0) {
        var_r28 = (u8*)param_0;
        var_r25 = __EXIRegs[14];
        for (i2 = 0; i2 < width; i2++) {
            *var_r28 = var_r25 >> ((3 - i2) << 3);
            var_r28++;
        }
    }

    return 1;
}

void __DBEXIInit() {
    u8 sp08;

    __OSMaskInterrupts(0x18000);

    while ((__EXIRegs[13] & 0x1) == 0x1) {
    };

    __EXIRegs[10] = 0;
    sp08 = 0xD4;
    __DBEXIWriteReg(0xB4000000, &sp08, 1);
}

BOOL __DBEXIReadReg(u32 param_0, void* dst, u32 param_2) {
    int is_failed = FALSE;
    u32 val = 0;

    is_failed |= IS_FALSE(__EXI2Select());
    is_failed |= IS_FALSE(__EXI2Imm(&param_0, 4, 1));
    is_failed |= IS_FALSE(__EXI2Sync());
    is_failed |= IS_FALSE(__EXI2Imm(&val, 4, 0));
    is_failed |= IS_FALSE(__EXI2Sync());
    is_failed |= IS_FALSE(__EXI2Deselect());

    switch (param_2) {
    case 1:
        *(u8*)dst = val >> 24;
        break;
    case 2:
        *(u16*)dst = (val >> 24) | ((val >> 8) & 0xff00);
        break;
    default:
        *(u32*)dst = __EXISwap32(val);
        break;
    }
    return IS_FALSE(is_failed);
}

BOOL __DBEXIWriteReg(u32 param_0, void* src, u32 width) {
    int is_failed = FALSE;
    u32 val;

    switch (width) {
    case 1:
        val = (*(u8*)src & 0xFF) << 24;
        break;
    case 2:
        val = ((*(u16*)src & 0xFF) << 24) | ((*(u16*)src & 0xff00) << 8);
        break;
    default:
        val = __EXISwap32(*(u32*)src);
        break;
    }

    is_failed |= IS_FALSE(__EXI2Select());
    is_failed |= IS_FALSE(__EXI2Imm(&param_0, 4, 1));
    is_failed |= IS_FALSE(__EXI2Sync());
    is_failed |= IS_FALSE(__EXI2Imm(&val, 4, 1));
    is_failed |= IS_FALSE(__EXI2Sync());
    is_failed |= IS_FALSE(__EXI2Deselect());

    return IS_FALSE(is_failed);
}

BOOL __DBEXIReadRam(u32 param_0, void* dst, int count) {
    int is_failed = FALSE;
    u32 cur_val;
    u32* cursor = (u32*)dst;

    is_failed |= IS_FALSE(__EXI2Select());
    is_failed |= IS_FALSE(__EXI2Imm(&param_0, 4, 1));
    is_failed |= IS_FALSE(__EXI2Sync());

    while (count > 0) {
        is_failed |= IS_FALSE(__EXI2Imm(&cur_val, 4, 0));
        is_failed |= IS_FALSE(__EXI2Sync());
        *cursor++ = cur_val;
        count -= 4;
    }

    is_failed |= IS_FALSE(__EXI2Deselect());

    return IS_FALSE(is_failed);
}

BOOL __DBEXIWriteRam(u32 param_0, void* src, int count) {
    int is_failed = FALSE;
    u32 cur_val;
    u32* cursor = (u32*)src;

    is_failed |= IS_FALSE(__EXI2Select());
    is_failed |= IS_FALSE(__EXI2Imm(&param_0, 4, 1));
    is_failed |= IS_FALSE(__EXI2Sync());

    while (count > 0) {
        cur_val = *cursor++;
        is_failed |= IS_FALSE(__EXI2Imm(&cur_val, 4, 1));
        is_failed |= IS_FALSE(__EXI2Sync());
        count -= 4;
    }

    is_failed |= IS_FALSE(__EXI2Deselect());

    return IS_FALSE(is_failed);
}

u32 __EXISwap32(u32 v) {
    return (v >> 24) | ((v >> 8) & 0xFF00) | ((v << 8) & 0xFF0000) | ((v << 24) & 0xFF000000);
}
