#ifndef _DOLPHIN_OSUTF_H_
#define _DOLPHIN_OSUTF_H_

#include <dolphin/types.h>

#ifdef __cplusplus
extern "C" {
#endif

char* OSUTF8to32(const char* utf8, u32* utf32);
char* OSUTF32to8(u32 utf32, char* utf8);
u16* OSUTF16to32(const u16* utf16, u32* utf32);
u16* OSUTF32to16(u32 utf32, u16* utf16);
u8 OSUTF32toANSI(u32 utf32);
u32 OSANSItoUTF32(u8 ansi);
u16 OSUTF32toSJIS(u32 utf32);
u32 OSSJIStoUTF32(u16 sjis);

#ifdef __cplusplus
}
#endif

#endif // _DOLPHIN_OSUTF_H_
