#include <revolution/gx.h>
#include <revolution/os.h>

#include "__gx.h"

#if DEBUG
#define GX_WRITE_SOME_REG5(a, b) \
do { \
    GX_WRITE_U8(a); \
    GX_WRITE_U32(b); \
    __gxVerif->rasRegs[(b & 0xFF000000) >> 24] = b; \
} while (0)
#else
#define GX_WRITE_SOME_REG5(a, b) \
do { \
    GX_WRITE_U8(a); \
    GX_WRITE_U32(b); \
} while (0)
#endif

void GXSetTevIndirect(GXTevStageID tev_stage, GXIndTexStageID ind_stage, GXIndTexFormat format, GXIndTexBiasSel bias_sel, GXIndTexMtxID matrix_sel, GXIndTexWrap wrap_s, GXIndTexWrap wrap_t, GXBool add_prev, GXBool utc_lod, GXIndTexAlphaSel alpha_sel) {
    u32 reg;

    CHECK_GXBEGIN(150, "GXInitIndTexture");
    reg = 0;
    SC_BP_CMD_SET_BT(152, reg, ind_stage);
    SC_BP_CMD_SET_FMT(153, reg, format);
    SC_BP_CMD_SET_BIAS(154, reg, bias_sel);
    SC_BP_CMD_SET_BS(155, reg, alpha_sel);
    SC_BP_CMD_SET_M(156, reg, matrix_sel);
    SC_BP_CMD_SET_SW(157, reg, wrap_s);
    SC_BP_CMD_SET_TW(158, reg, wrap_t);
    SC_BP_CMD_SET_LB(159, reg, utc_lod);
    SC_BP_CMD_SET_FB(160, reg, add_prev);
    SC_BP_CMD_SET_RID(161, reg, (0x10 + tev_stage));
    GX_WRITE_SOME_REG5(GX_LOAD_BP_REG, reg);
    __GXData->bpSentNot = 0;
}

void GXSetIndTexMtx(GXIndTexMtxID mtx_id, const f32 offset[2][3], s8 scale_exp) {
    s32 mtx[6];
    u32 reg;
    u32 id;

    CHECK_GXBEGIN(190, "GXSetIndTexMtx");

    switch (mtx_id) {
    case GX_ITM_0:
    case GX_ITM_1:
    case GX_ITM_2:
        id = mtx_id - 1;
        break;
    case GX_ITM_S0:
    case GX_ITM_S1:
    case GX_ITM_S2:
        id = mtx_id - 5;
        break;
    case GX_ITM_T0:
    case GX_ITM_T1:
    case GX_ITM_T2:
        id = mtx_id - 9;
        break;
    default:
        id = 0;
        break;
    }

    mtx[0] = (int)(1024.0f * offset[0][0]) & 0x7FF;
    mtx[1] = (int)(1024.0f * offset[1][0]) & 0x7FF;
    scale_exp += 17;
    reg = 0;
    SC_BP_MTXA_SET_MA(212, reg, mtx[0]);
    SC_BP_MTXA_SET_MB(213, reg, mtx[1]);
    SC_BP_MTXA_SET_S(214, reg, scale_exp & 3);
    SC_BP_MTXA_SET_RID(215, reg, id * 3 + 6);
    GX_WRITE_SOME_REG5(GX_LOAD_BP_REG, reg);

    mtx[2] = (int)(1024.0f * offset[0][1]) & 0x7FF;
    mtx[3] = (int)(1024.0f * offset[1][1]) & 0x7FF;
    reg = 0;
    SC_BP_MTXB_SET_MC(221, reg, mtx[2]);
    SC_BP_MTXB_SET_MD(222, reg, mtx[3]);
    SC_BP_MTXB_SET_S(223, reg, (scale_exp >> 2) & 3);
    SC_BP_MTXB_SET_RID(224, reg, id * 3 + 7);
    GX_WRITE_SOME_REG5(GX_LOAD_BP_REG, reg);

    mtx[4] = (int)(1024.0f * offset[0][2]) & 0x7FF;
    mtx[5] = (int)(1024.0f * offset[1][2]) & 0x7FF;
    reg = 0;
    SC_BP_MTXC_SET_ME(230, reg, mtx[4]);
    SC_BP_MTXC_SET_MF(231, reg, mtx[5]);
    SC_BP_MTXC_SET_S(232, reg, (scale_exp >> 4) & 3);
    SC_BP_MTXC_SET_RID(233, reg, id * 3 + 8);
    GX_WRITE_SOME_REG5(GX_LOAD_BP_REG, reg);

    __GXData->bpSentNot = 0;
}

void GXSetIndTexCoordScale(GXIndTexStageID ind_state, GXIndTexScale scale_s, GXIndTexScale scale_t) {
    CHECK_GXBEGIN(253, "GXSetIndTexScale");

    switch (ind_state) {
    case GX_INDTEXSTAGE0:
        SET_REG_FIELD(257, __GXData->IndTexScale0, 4, 0, scale_s);
        SET_REG_FIELD(258, __GXData->IndTexScale0, 4, 4, scale_t);
        SET_REG_FIELD(258, __GXData->IndTexScale0, 8, 24, 0x25);
        GX_WRITE_SOME_REG5(GX_LOAD_BP_REG, __GXData->IndTexScale0);
        break;
    case GX_INDTEXSTAGE1:
        SET_REG_FIELD(263, __GXData->IndTexScale0, 4, 8, scale_s);
        SET_REG_FIELD(264, __GXData->IndTexScale0, 4, 12, scale_t);
        SET_REG_FIELD(264, __GXData->IndTexScale0, 8, 24, 0x25);
        GX_WRITE_SOME_REG5(GX_LOAD_BP_REG, __GXData->IndTexScale0);
        break;
    case GX_INDTEXSTAGE2:
        SET_REG_FIELD(269, __GXData->IndTexScale1, 4, 0, scale_s);
        SET_REG_FIELD(270, __GXData->IndTexScale1, 4, 4, scale_t);
        SET_REG_FIELD(270, __GXData->IndTexScale1, 8, 24, 0x26);
        GX_WRITE_SOME_REG5(GX_LOAD_BP_REG, __GXData->IndTexScale1);
        break;
    case GX_INDTEXSTAGE3:
        SET_REG_FIELD(275, __GXData->IndTexScale1, 4, 8, scale_s);
        SET_REG_FIELD(276, __GXData->IndTexScale1, 4, 12, scale_t);
        SET_REG_FIELD(276, __GXData->IndTexScale1, 8, 24, 0x26);
        GX_WRITE_SOME_REG5(GX_LOAD_BP_REG, __GXData->IndTexScale1);
        break;
    default:
        ASSERTMSGLINE(281, 0, "GXSetIndTexCoordScale: Invalid Indirect Stage Id");
        break;
    }
    __GXData->bpSentNot = 0;
}

void GXSetIndTexOrder(GXIndTexStageID ind_stage, GXTexCoordID tex_coord, GXTexMapID tex_map) {
    CHECK_GXBEGIN(306, "GXSetIndTexOrder");

    if (tex_map == GX_TEXMAP_NULL) {
        tex_map = GX_TEXMAP0;
    }

    if (tex_coord == GX_TEXCOORD_NULL) {
        tex_coord = GX_TEXCOORD0;
    }

    ASSERTMSGLINE(318, tex_map < GX_MAX_TEXMAP, "GXSetIndTexOrder: Invalid direct texture Id");
    ASSERTMSGLINE(319, tex_coord < GX_MAX_TEXCOORD, "GXSetIndTexOrder: Invalid texture coord");

    switch (ind_stage) {
    case GX_INDTEXSTAGE0:
        SET_REG_FIELD(323, __GXData->iref, 3, 0, tex_map);
        SET_REG_FIELD(324, __GXData->iref, 3, 3, tex_coord);
        break;
    case GX_INDTEXSTAGE1:
        SET_REG_FIELD(327, __GXData->iref, 3, 6, tex_map);
        SET_REG_FIELD(328, __GXData->iref, 3, 9, tex_coord);
        break;
    case GX_INDTEXSTAGE2:
        SET_REG_FIELD(331, __GXData->iref, 3, 12, tex_map);
        SET_REG_FIELD(332, __GXData->iref, 3, 15, tex_coord);
        break;
    case GX_INDTEXSTAGE3:
        SET_REG_FIELD(335, __GXData->iref, 3, 18, tex_map);
        SET_REG_FIELD(336, __GXData->iref, 3, 21, tex_coord);
        break;
    default:
        ASSERTMSGLINE(339, 0, "GXSetIndTexOrder: Invalid Indirect Stage Id");
        break;
    }
    GX_WRITE_SOME_REG5(GX_LOAD_BP_REG, __GXData->iref);
    __GXData->dirtyState |= 3;
    __GXData->bpSentNot = 0;
}

void GXSetNumIndStages(u8 nIndStages) {
    CHECK_GXBEGIN(357, "GXSetNumIndStages");
    ASSERTMSGLINE(359, nIndStages <= 4, "GXSetNumIndStages: Exceeds max. number of indirect texture stages");
    SET_REG_FIELD(360, __GXData->genMode, 3, 16, nIndStages);
    __GXData->dirtyState |= 6;
}

void GXSetTevDirect(GXTevStageID tev_stage) {
    CHECK_GXBEGIN(377, "GXSetTevDirect");
    GXSetTevIndirect(tev_stage, GX_INDTEXSTAGE0, GX_ITF_8, GX_ITB_NONE, GX_ITM_OFF, GX_ITW_OFF, GX_ITW_OFF, GX_FALSE, GX_FALSE, GX_ITBA_OFF);
}

void GXSetTevIndWarp(GXTevStageID tev_stage, GXIndTexStageID ind_stage, u8 signed_offset, u8 replace_mode, GXIndTexMtxID matrix_sel) {
    GXIndTexWrap wrap = (replace_mode != 0) ? GX_ITW_0 : GX_ITW_OFF;

    CHECK_GXBEGIN(395, "GXSetTevIndWarp");
    GXSetTevIndirect(tev_stage, ind_stage, GX_ITF_8, (signed_offset != 0) ? GX_ITB_STU : GX_ITB_NONE, matrix_sel, wrap, wrap, GX_FALSE, GX_FALSE, GX_ITBA_OFF);
}

void GXSetTevIndTile(GXTevStageID tev_stage, GXIndTexStageID ind_stage, u16 tilesize_s,
    u16 tilesize_t, u16 tilespacing_s, u16 tilespacing_t, GXIndTexFormat format,
    GXIndTexMtxID matrix_sel, GXIndTexBiasSel bias_sel, GXIndTexAlphaSel alpha_sel)
{
    GXIndTexWrap wrap_s;
    GXIndTexWrap wrap_t;
    f32 mtx[2][3];

    CHECK_GXBEGIN(429, "GXSetTevIndTile");
    ASSERTMSGLINE(430, tev_stage < GX_MAX_TEVSTAGE, "GXSetTevIndTile: Invalid tev stage id");
    ASSERTMSGLINE(431, ind_stage < GX_MAX_INDTEXSTAGE, "GXSetTevIndTile: Invalid indirect stage id");

    switch (tilesize_s) {
    case 256:
        wrap_s = GX_ITW_256;
        break;
    case 128:
        wrap_s = GX_ITW_128;
        break;
    case 64:
        wrap_s = GX_ITW_64;
        break;
    case 32:
        wrap_s = GX_ITW_32;
        break;
    case 16:
        wrap_s = GX_ITW_16;
        break;
    default:
        ASSERTMSGLINE(440, 0, "GXSetTevIndTile: Invalid tilesize for S coordinate");
        wrap_s = GX_ITW_OFF;
        break;
    }

    switch (tilesize_t) {
    case 256:
        wrap_t = GX_ITW_256;
        break;
    case 128:
        wrap_t = GX_ITW_128;
        break;
    case 64:
        wrap_t = GX_ITW_64;
        break;
    case 32:
        wrap_t = GX_ITW_32;
        break;
    case 16:
        wrap_t = GX_ITW_16;
        break;
    default:
        ASSERTMSGLINE(452, 0, "GXSetTevIndTile: Invalid tilesize for T coordinate");
        wrap_t = GX_ITW_OFF;
        break;
    }

    mtx[0][0] = tilespacing_s / 1024.0f;
    mtx[0][1] = mtx[0][2] = 0.0f;
    mtx[1][1] = tilespacing_t / 1024.0f;
    mtx[1][0] = mtx[1][2] = 0.0f;
    GXSetIndTexMtx(matrix_sel, mtx, 10);
    GXSetTevIndirect(tev_stage, ind_stage, format, bias_sel, matrix_sel, wrap_s, wrap_t, GX_FALSE, GX_TRUE, alpha_sel);
}

void GXSetTevIndBumpST(GXTevStageID tev_stage, GXIndTexStageID ind_stage, GXIndTexMtxID matrix_sel) {
    GXIndTexMtxID sm;
    GXIndTexMtxID tm;

    CHECK_GXBEGIN(492, "GXSetTevIndBumpST");

    switch (matrix_sel) {
    case GX_ITM_0:
        sm = GX_ITM_S0;
        tm = GX_ITM_T0;
        break;
    case GX_ITM_1:
        sm = GX_ITM_S1;
        tm = GX_ITM_T1;
        break;
    case GX_ITM_2:
        sm = GX_ITM_S2;
        tm = GX_ITM_T2;
        break;
    default:
        ASSERTMSGLINE(509, 0, "GXSetTevIndBumpST: Invalid matrix selection");
        break;
    }

    GXSetTevIndirect(tev_stage, ind_stage, GX_ITF_8, GX_ITB_ST, sm, GX_ITW_0, GX_ITW_0, GX_FALSE, GX_FALSE, GX_ITBA_OFF);
    GXSetTevIndirect(tev_stage + 1, ind_stage, GX_ITF_8, GX_ITB_ST, tm, GX_ITW_0, GX_ITW_0, GX_TRUE, GX_FALSE, GX_ITBA_OFF);
    GXSetTevIndirect(tev_stage + 2, ind_stage, GX_ITF_8, GX_ITB_NONE, GX_ITM_OFF, GX_ITW_OFF, GX_ITW_OFF, GX_TRUE, GX_FALSE, GX_ITBA_OFF);
}

void GXSetTevIndBumpXYZ(GXTevStageID tev_stage, GXIndTexStageID ind_stage, GXIndTexMtxID matrix_sel) {
    CHECK_GXBEGIN(561, "GXSetTevIndBumpXYZ");
    GXSetTevIndirect(tev_stage, ind_stage, GX_ITF_8, GX_ITB_STU, matrix_sel, GX_ITW_OFF, GX_ITW_OFF, GX_FALSE, GX_FALSE, GX_ITBA_OFF);
}

void GXSetTevIndRepeat(GXTevStageID tev_stage) {
    CHECK_GXBEGIN(590, "GXSetTevIndRepeat");
    GXSetTevIndirect(tev_stage, GX_INDTEXSTAGE0, GX_ITF_8, GX_ITB_NONE, GX_ITM_OFF, GX_ITW_0, GX_ITW_0, GX_TRUE, GX_FALSE, GX_ITBA_OFF);
}

void __GXUpdateBPMask(void) {}

void __GXSetIndirectMask(u32 mask) {
    SET_REG_FIELD(668, __GXData->bpMask, 8, ~0xFF, mask);

    GX_WRITE_SOME_REG5(GX_LOAD_BP_REG, __GXData->bpMask);
    __GXData->bpSentNot = 0;
}

void __GXFlushTextureState(void) {
    GX_WRITE_SOME_REG5(GX_LOAD_BP_REG, __GXData->bpMask);
    __GXData->bpSentNot = 0;
}
