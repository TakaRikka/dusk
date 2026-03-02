#ifndef _DOLPHIN_GD_TRANSFORM_H_
#define _DOLPHIN_GD_TRANSFORM_H_

#include <dolphin/types.h>
#include <dolphin/gx/GXEnum.h>
#include <dolphin/mtx.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CP_MTX_REG_A_ID 0x30
#define CP_MTX_REG_B_ID 0x40

#define XF_REG_MATRIXINDEX0_ID 0x1018
#define XF_REG_MATRIXINDEX1_ID 0x1019
#define XF_REG_PROJECTIONA_ID  0x1020
#define XF_REG_PROJECTIONB_ID  0x1021
#define XF_REG_PROJECTIONC_ID  0x1022
#define XF_REG_PROJECTIOND_ID  0x1023
#define XF_REG_PROJECTIONE_ID  0x1024
#define XF_REG_PROJECTIONF_ID  0x1025
#define XF_REG_PROJECTORTHO_ID 0x1026

#define CP_MTX_REG_A(pn, t0, t1, t2, t3) \
    ( \
        (u32)(pn) << 0 | \
        (u32)(t0) << 6 | \
        (u32)(t1) << 12 | \
        (u32)(t2) << 18 | \
        (u32)(t3) << 24 \
    )

#define CP_MTX_REG_B(t4, t5, t6, t7) \
    ( \
        (u32)(t4) << 0 | \
        (u32)(t5) << 6 | \
        (u32)(t6) << 12 | \
        (u32)(t7) << 18 \
    )

void GDLoadPosMtxImm(const Mtx mtx, u32 id);
void GDLoadPosMtxIndx(u16 mtx_indx, u32 id);
void GDLoadNrmMtxImm(const Mtx mtx, u32 id);
void GDLoadNrmMtxImm3x3(const f32 mtx[3][3], u32 id);
void GDLoadNrmMtxIndx3x3(u16 mtx_indx, u32 id);
void GDLoadTexMtxImm(const Mtx mtx, u32 id, GXTexMtxType type);
void GDLoadTexMtxIndx(u16 mtx_indx, u32 id, GXTexMtxType type);
void GDSetCurrentMtx(u32 pn, u32 t0, u32 t1, u32 t2, u32 t3, u32 t4, u32 t5, u32 t6, u32 t7);
void GDSetProjection(const Mtx44 mtx, GXProjectionType type);

#ifdef __cplusplus
}
#endif

#endif
