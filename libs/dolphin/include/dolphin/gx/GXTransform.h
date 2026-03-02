#ifndef _DOLPHIN_GX_GXTRANSFORM_H_
#define _DOLPHIN_GX_GXTRANSFORM_H_

#include <dolphin/gx/GXEnum.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GX_PROJECTION_SZ  7
#define GX_VIEWPORT_SZ  6

void GXProject(f32 x, f32 y, f32 z, const f32 mtx[3][4], const f32* pm, const f32* vp, f32* sx, f32* sy, f32* sz);
void GXSetProjection(const f32 mtx[4][4], GXProjectionType type);
void GXSetProjectionv(const f32* ptr);
void GXLoadPosMtxImm(const f32 mtx[3][4], u32 id);
void GXLoadPosMtxIndx(u16 mtx_indx, u32 id);
void GXLoadNrmMtxImm(const f32 mtx[3][4], u32 id);
void GXLoadNrmMtxImm3x3(const f32 mtx[3][3], u32 id);
void GXLoadNrmMtxIndx3x3(u16 mtx_indx, u32 id);
void GXSetCurrentMtx(u32 id);
void GXLoadTexMtxImm(const f32 mtx[][4], u32 id, GXTexMtxType type);
void GXLoadTexMtxIndx(u16 mtx_indx, u32 id, GXTexMtxType type);
void GXSetViewportJitter(f32 left, f32 top, f32 wd, f32 ht, f32 nearz, f32 farz, u32 field);
void GXSetViewport(f32 left, f32 top, f32 wd, f32 ht, f32 nearz, f32 farz);
void GXSetScissorBoxOffset(s32 x_off, s32 y_off);
void GXSetClipMode(GXClipMode mode);
void GXSetZScaleOffset(f32 scale, f32 offset);

#ifdef __cplusplus
}
#endif

#endif
