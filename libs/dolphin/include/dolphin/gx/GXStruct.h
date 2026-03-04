#ifndef _DOLPHIN_GX_GXSTRUCT_H_
#define _DOLPHIN_GX_GXSTRUCT_H_

#include <dolphin/gx/GXEnum.h>
#include <dolphin/vi/vitypes.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _GXRenderModeObj {
    /* 0x00 */ VITVMode viTVmode;
    /* 0x04 */ u16 fbWidth;
    /* 0x06 */ u16 efbHeight;
    /* 0x08 */ u16 xfbHeight;
    /* 0x0A */ u16 viXOrigin;
    /* 0x0C */ u16 viYOrigin;
    /* 0x0E */ u16 viWidth;
    /* 0x10 */ u16 viHeight;
    /* 0x14 */ VIXFBMode xFBmode;
    /* 0x18 */ u8 field_rendering;
    /* 0x19 */ u8 aa;
    /* 0x20 */ u8 sample_pattern[12][2];
    /* 0x38 */ u8 vfilter[7];
} GXRenderModeObj;

typedef struct _GXColor {
    u8 r, g, b, a;
} GXColor;

typedef struct _GXColorS10 {
    s16 r, g, b, a;
} GXColorS10;

typedef struct _GXTexObj {
#ifdef TARGET_PC
    u32 dummy[22];  // Aurora's GXTexObj_ contains std::shared_ptr + many fields (~80 bytes)
#else
    u32 dummy[8];
#endif
} GXTexObj;

typedef struct _GXLightObj {
    u32 dummy[16];
} GXLightObj;

typedef struct _GXTexRegion {
    u32 dummy[4];
} GXTexRegion;

typedef struct _GXTlutObj {
#ifdef TARGET_PC
    u32 dummy[4];   // Aurora's GXTlutObj_ contains std::shared_ptr (8+ bytes)
#else
    u32 dummy[3];
#endif
} GXTlutObj;

typedef struct _GXTlutRegion {
    u32 dummy[4];
} GXTlutRegion;

typedef struct _GXFogAdjTable {
    u16 r[10];
} GXFogAdjTable;

typedef struct _GXVtxDescList {
    GXAttr attr;
    GXAttrType type;
} GXVtxDescList;

typedef struct _GXVtxAttrFmtList {
    GXAttr attr;
    GXCompCnt cnt;
    GXCompType type;
    u8 frac;
} GXVtxAttrFmtList;

#ifdef __cplusplus
}
#endif

#endif
