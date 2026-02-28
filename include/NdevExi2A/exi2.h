#ifndef NDEVEXI2A_EXI2_H
#define NDEVEXI2A_EXI2_H

#include "types.h"

#ifdef __cplusplus
extern "C" {
#endif

BOOL __EXI2Select();

BOOL __EXI2Deselect();

BOOL __EXI2Sync();

BOOL __EXI2Imm(u32* param_0, int width, int param_2);

void __DBEXIInit();

BOOL __DBEXIReadReg(u32 param_0, void* dst, u32 param_2);

BOOL __DBEXIWriteReg(u32 param_0, void* src, u32 width);

BOOL __DBEXIReadRam(u32 param_0, void* dst, int count);

BOOL __DBEXIWriteRam(u32 param_0, void* src, int count);

u32 __EXISwap32(u32 v);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* NDEVEXI2A_EXI2_H */
