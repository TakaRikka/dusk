// This code is copied/modified from the decompgz project and has been modified to fit the needs of this framework.
// https://github.com/zsrtp/decompgz

#include "draw.hpp"
#include "d/d_com_inf_game.h"
#include "m_Do/m_Do_mtx.h"

static void setupGXPrimitive() {
    GXSetNumChans(1);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, 0, GX_DF_NONE, GX_AF_NONE);

    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_OR);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetCullMode(GX_CULL_NONE);

    GXLoadPosMtxImm(g_mDoMtx_identity, GX_PNMTX0);
    GXSetCurrentMtx(0);

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
}

void drawFilledRect(f32 x, f32 y, f32 w, f32 h, GXColor color) {
    setupGXPrimitive();

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition2f32(x, y);
    GXColor4u8(color.r, color.g, color.b, color.a);
    GXPosition2f32(x + w, y);
    GXColor4u8(color.r, color.g, color.b, color.a);
    GXPosition2f32(x + w, y + h);
    GXColor4u8(color.r, color.g, color.b, color.a);
    GXPosition2f32(x, y + h);
    GXColor4u8(color.r, color.g, color.b, color.a);
    GXEnd();
}
