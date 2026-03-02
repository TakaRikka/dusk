#ifndef _REVOLUTION_GX_GXGEOMETRY_H_
#define _REVOLUTION_GX_GXGEOMETRY_H_

#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXStruct.h>

#ifdef __cplusplus
extern "C" {
#endif

void __GXCalculateVLim();
void GXSetVtxDesc(GXAttr attr, GXAttrType type);
void GXSetVtxDescv(const GXVtxDescList* attrPtr);
void GXClearVtxDesc(void);
void GXSetVtxAttrFmt(GXVtxFmt vtxfmt, GXAttr attr, GXCompCnt cnt, GXCompType type, u8 frac);
void GXSetVtxAttrFmtv(GXVtxFmt vtxfmt, const GXVtxAttrFmtList* list);
void GXSetArray(GXAttr attr, void* base_ptr, u8 stride);
void GXInvalidateVtxCache(void);
void GXSetTexCoordGen2(GXTexCoordID dst_coord, GXTexGenType func, GXTexGenSrc src_param, u32 mtx, GXBool normalize, u32 pt_texmtx);
void GXSetNumTexGens(u8 nTexGens);

static inline void GXSetTexCoordGen(GXTexCoordID dst_coord, GXTexGenType func, GXTexGenSrc src_param, u32 mtx) {
    GXSetTexCoordGen2(dst_coord, func, src_param, mtx, GX_FALSE, GX_PTIDENTITY);
}

void GXBegin(GXPrimitive type, GXVtxFmt vtxfmt, u16 nverts);

static inline void GXEnd(void) {
#if DEBUG
    extern GXBool __GXinBegin;
    extern void OSPanic(const char* file, int line, const char* msg, ...);
    if (!__GXinBegin) {
        OSPanic(__FILE__, 127, "GXEnd: called without a GXBegin");
    }
    __GXinBegin = GX_FALSE;
#endif
}

void GXSetLineWidth(u8 width, GXTexOffset texOffsets);
void GXSetPointSize(u8 pointSize, GXTexOffset texOffsets);
void GXEnableTexOffsets(GXTexCoordID coord, u8 line_enable, u8 point_enable);

#ifdef __cplusplus
}
#endif

#endif
