#ifndef _DOLPHIN_GD_INDIRECT_H_
#define _DOLPHIN_GD_INDIRECT_H_

#include <dolphin/types.h>
#include <dolphin/gx/GXEnum.h>
#include <dolphin/mtx.h>

#ifdef __cplusplus
extern "C" {
#endif

#define BP_TEV_INDIRECT(ind_stage, format, bias_sel, alpha_sel, mtx_sel, wrap_s, wrap_t, utc_lod, add_prev, id) \
    ( \
        (u32)(ind_stage) << 0 | \
        (u32)(format) << 2 | \
        (u32)(bias_sel) << 4 | \
        (u32)(alpha_sel) << 7 | \
        (u32)(mtx_sel) << 9 | \
        (u32)(wrap_s) << 13 | \
        (u32)(wrap_t) << 16 | \
        (u32)(utc_lod) << 19 | \
        (u32)(add_prev) << 20 | \
        (u32)(id) << 24 \
    )

#define BP_IND_MTX(m0, m1, scale_exp, id) \
    ( \
        (u32)(m0) << 0 | \
        (u32)(m1) << 11 | \
        (u32)(scale_exp) << 22 | \
        (u32)(id) << 24 \
    )

#define BP_IND_TEXCOORD_SCALE(scaleS0, scaleT0, scaleS1, scaleT1, id) \
    ( \
        (u32)(scaleS0) << 0 | \
        (u32)(scaleT0) << 4 | \
        (u32)(scaleS1) << 8 | \
        (u32)(scaleT1) << 12 | \
        (u32)(id) << 24 \
    )

#define BP_IND_TEX_ORDER(map0, coord0, map1, coord1, map2, coord2, map3, coord3, id) \
    ( \
        (u32)(map0) << 0 | \
        (u32)(coord0) << 3 | \
        (u32)(map1) << 6 | \
        (u32)(coord1) << 9 | \
        (u32)(map2) << 12 | \
        (u32)(coord2) << 15 | \
        (u32)(map3) << 18 | \
        (u32)(coord3) << 21 | \
        (u32)(id) << 24 \
    )

#define BP_IND_MASK(mask, id) \
    ( \
        (u32)(mask) << 0 | \
        (u32)(id) << 24 \
    )

void GDSetTevIndirect(GXTevStageID tev_stage, GXIndTexStageID ind_stage,
                      GXIndTexFormat format, GXIndTexBiasSel bias_sel,
                      GXIndTexMtxID matrix_sel, GXIndTexWrap wrap_s,
                      GXIndTexWrap wrap_t, u8 add_prev, u8 utc_lod,
                      GXIndTexAlphaSel alpha_sel);
void GDSetIndTexMtx(GXIndTexMtxID mtx_id, const f32 offset[2][3], s8 scale_exp);
void GDSetIndTexCoordScale(GXIndTexStageID indStageEven, GXIndTexScale scaleS0,
                           GXIndTexScale scaleT0, GXIndTexScale scaleS1,
                           GXIndTexScale scaleT1);
void GDSetIndTexOrder(GXTexCoordID texCoord0, GXTexMapID texMap0,
                      GXTexCoordID texCoord1, GXTexMapID texMap1,
                      GXTexCoordID texCoord2, GXTexMapID texMap2,
                      GXTexCoordID texCoord3, GXTexMapID texMap3);
void GDSetTevDirect(GXTevStageID tev_stage);
void GDSetTevIndWarp(GXTevStageID tev_stage, GXIndTexStageID ind_stage, u8 signed_offset, u8 replace_mode, GXIndTexMtxID matrix_sel);
void GDSetTevIndTile(GXTevStageID tev_stage, GXIndTexStageID ind_stage,
                     u16 tilesize_s, u16 tilesize_t, u16 tilespacing_s,
                     u16 tilespacing_t, GXIndTexFormat format,
                     GXIndTexMtxID matrix_sel, GXIndTexBiasSel bias_sel,
                     GXIndTexAlphaSel alpha_sel);
void GDSetTevIndBumpST(GXTevStageID tev_stage, GXIndTexStageID ind_stage, GXIndTexMtxID matrix_sel);
void GDSetTevIndBumpXYZ(GXTevStageID tev_stage, GXIndTexStageID ind_stage, GXIndTexMtxID matrix_sel);
void GDSetTevIndRepeat(GXTevStageID tev_stage);
void __GDSetIndTexMask(u32 mask);

#ifdef __cplusplus
}
#endif

#endif
