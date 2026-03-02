#ifndef _DOLPHIN_GX_GXVERIFY_H_
#define _DOLPHIN_GX_GXVERIFY_H_

#include <dolphin/gx/GXEnum.h>
#include <dolphin/gx/GXStruct.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    GX_WARN_NONE,
    GX_WARN_SEVERE,
    GX_WARN_MEDIUM,
    GX_WARN_ALL
} GXWarningLevel;

typedef void (*GXVerifyCallback)(GXWarningLevel level, u32 id, char* msg);

void GXSetVerifyLevel(GXWarningLevel level);
GXVerifyCallback GXSetVerifyCallback(GXVerifyCallback cb);

void __GXVerifyVATImm(GXAttr attr, GXCompCnt cnt, GXCompType type, u8 frac);

#ifdef __cplusplus
}
#endif

#endif
