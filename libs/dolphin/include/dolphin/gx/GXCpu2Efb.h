#ifndef _DOLPHIN_GX_GXCPU2EFB_H_
#define _DOLPHIN_GX_GXCPU2EFB_H_

#include <dolphin/gx/GXEnum.h>

#ifdef __cplusplus
extern "C" {
#endif

void GXPokeAlphaMode(GXCompare func, u8 threshold);
void GXPokeAlphaRead(GXAlphaReadMode mode);
void GXPokeAlphaUpdate(GXBool update_enable);
void GXPokeBlendMode(GXBlendMode type, GXBlendFactor src_factor, GXBlendFactor dst_factor, GXLogicOp op);
void GXPokeColorUpdate(GXBool update_enable);
void GXPokeDstAlpha(GXBool enable, u8 alpha);
void GXPokeDither(GXBool dither);
void GXPokeZMode(GXBool compare_enable, GXCompare func, GXBool update_enable);
void GXPeekARGB(u16 x, u16 y, u32* color);
void GXPokeARGB(u16 x, u16 y, u32 color);
void GXPeekZ(u16 x, u16 y, u32* z);
void GXPokeZ(u16 x, u16 y, u32 z);
u32 GXCompressZ16(u32 z24, GXZFmt16 zfmt);
u32 GXDecompressZ16(u32 z16, GXZFmt16 zfmt);

#ifdef __cplusplus
}
#endif

#endif
