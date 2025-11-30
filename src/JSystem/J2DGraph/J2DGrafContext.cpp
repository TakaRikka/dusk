#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J2DGraph/J2DGrafContext.h"
#include "dolphin/gx.h"

J2DGrafContext::J2DGrafContext(f32 x, f32 y, f32 width, f32 height)
    : mBounds(x, y, x + width, y + height), mScissorBounds(x, y, x + width, y + height) {
    JUtility::TColor color(-1);
    setColor(color);
    setLineWidth(6);
}

void J2DGrafContext::setPort() {
    setScissor();
    setup2D();

    JGeometry::TBox2<float> bounds(mBounds);

    if (bounds.i.x < 0.0f) {
        bounds.i.x = 0.0f;
    }
    if (bounds.i.y < 0.0f) {
        bounds.i.y = 0.0f;
    }
    GXSetViewport(bounds.i.x, bounds.i.y, bounds.getWidth(), bounds.getHeight(), 0.0f, 1.0f);
}

void J2DGrafContext::setup2D() {
    GXSetNumIndStages(0);
    for (int i = 0; i < GX_MAX_TEVSTAGE; i++) {
        GXSetTevDirect((GXTevStageID)i);
    }
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
    GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetNumChans(1);
    GXSetNumTevStages(1);
    GXSetNumTexGens(0);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetCullMode(GX_CULL_NONE);
    GXLoadPosMtxImm(mPosMtx, 0);
    Mtx mtx;
    MTXIdentity(mtx);
    GXLoadTexMtxImm(mtx, GX_IDENTITY, GX_MTX3x4);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE,
                  GX_AF_NONE);
    GXSetCurrentMtx(0);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGBA4, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBX8, 0xf);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_CLR_RGBA, GX_RGBX8, 0xf);
    GXSetLineWidth(mLineWidth, GX_TO_ZERO);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
}

void J2DGrafContext::setScissor() {
    JGeometry::TBox2<f32> bounds(0, 0, 1024, 1024);
    JGeometry::TBox2<f32> curBounds(mScissorBounds);
    mScissorBounds.intersect(bounds);
    curBounds.absolute();
    if (curBounds.intersect(bounds)) {
        curBounds.i.x = (u32)curBounds.i.x;
        curBounds.i.y = (u32)curBounds.i.y;
        curBounds.f.x = ceil(curBounds.f.x);
        curBounds.f.y = ceil(curBounds.f.y);
        GXSetScissor(curBounds.i.x, curBounds.i.y, curBounds.getWidth(), curBounds.getHeight());
    } else {
        GXSetScissor(0, 0, 0, 0);
    }
}

void J2DGrafContext::scissor(JGeometry::TBox2<f32> const& bounds) {
    mScissorBounds = bounds;
}

void J2DGrafContext::place(JGeometry::TBox2<f32> const& bounds) {
    mBounds = bounds;
    mScissorBounds = bounds;
}

/* 802E9118-802E9234 2E3A58 011C+00 1/1 4/4 0/0 .text
 * setColor__14J2DGrafContextFQ28JUtility6TColorQ28JUtility6TColorQ28JUtility6TColorQ28JUtility6TColor
 */
void J2DGrafContext::setColor(JUtility::TColor colorTL, JUtility::TColor colorTR,
                              JUtility::TColor colorBR, JUtility::TColor colorBL) {
    mColorTL = colorTL;
    mColorTR = colorTR;
    mColorBR = colorBR;
    mColorBL = colorBL;
    field_0xb0.mType = GX_BM_BLEND;
    field_0xb0.mSrcFactor = GX_BL_SRCALPHA;
    field_0xb0.mDstFactor = GX_BL_INVSRCALPHA;
    mLinePart.mType = GX_BM_BLEND;
    mLinePart.mSrcFactor = GX_BL_SRCALPHA;
    mLinePart.mDstFactor = GX_BL_INVSRCALPHA;
    mBoxPart.mType = GX_BM_BLEND;
    mBoxPart.mSrcFactor = GX_BL_SRCALPHA;
    mBoxPart.mDstFactor = GX_BL_INVSRCALPHA;
    if ((mColorTL & 0xFF) != 0xFF) {
        return;
    }
    field_0xb0.mType = GX_BM_NONE;
    field_0xb0.mSrcFactor = GX_BL_ONE;
    field_0xb0.mDstFactor = GX_BL_ZERO;
    if ((mColorBR & 0xFF) != 0xFF) {
        return;
    }
    mLinePart.mType = GX_BM_NONE;
    mLinePart.mSrcFactor = GX_BL_ONE;
    mLinePart.mDstFactor = GX_BL_ZERO;
    if ((mColorTR & 0xFF) != 0xFF) {
        return;
    }
    if ((mColorBL & 0xFF) != 0xFF) {
        return;
    }
    mBoxPart.mType = GX_BM_NONE;
    mBoxPart.mSrcFactor = GX_BL_ONE;
    mBoxPart.mDstFactor = GX_BL_ZERO;
}

void J2DGrafContext::setLineWidth(u8 lineWidth) {
    mLineWidth = lineWidth;
    GXSetLineWidth(mLineWidth, GX_TO_ZERO);
}

void J2DGrafContext::fillBox(JGeometry::TBox2<f32> const& box) {
    GXSetBlendMode((GXBlendMode)mBoxPart.mType, (GXBlendFactor)mBoxPart.mSrcFactor,
                   (GXBlendFactor)mBoxPart.mDstFactor, GX_LO_SET);
    GXLoadPosMtxImm(mPosMtx, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(box.i.x, box.i.y, 0);
    GXColor1u32(mColorTL);
    GXPosition3f32(box.f.x, box.i.y, 0);
    GXColor1u32(mColorTR);
    GXPosition3f32(box.f.x, box.f.y, 0);
    GXColor1u32(mColorBL);
    GXPosition3f32(box.i.x, box.f.y, 0);
    GXColor1u32(mColorBR);
    GXEnd();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGBA4, 0);
}

void J2DGrafContext::drawFrame(JGeometry::TBox2<f32> const& box) {
    GXSetBlendMode((GXBlendMode)mBoxPart.mType, (GXBlendFactor)mBoxPart.mSrcFactor,
                   (GXBlendFactor)mBoxPart.mDstFactor, GX_LO_SET);
    GXLoadPosMtxImm(mPosMtx, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    GXBegin(GX_LINESTRIP, GX_VTXFMT0, 5);
    GXPosition3f32(box.i.x, box.i.y, 0);
    GXColor1u32(mColorTL);
    GXPosition3f32(box.f.x, box.i.y, 0);
    GXColor1u32(mColorTR);
    GXPosition3f32(box.f.x, box.f.y, 0);
    GXColor1u32(mColorBL);
    GXPosition3f32(box.i.x, box.f.y, 0);
    GXColor1u32(mColorBR);
    GXPosition3f32(box.i.x, box.i.y, 0);
    GXColor1u32(mColorTL);
    GXEnd();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGBA4, 0);
}

void J2DGrafContext::line(JGeometry::TVec2<f32> start, JGeometry::TVec2<f32> end) {
    GXSetBlendMode((GXBlendMode)mLinePart.mType, (GXBlendFactor)mLinePart.mSrcFactor,
                   (GXBlendFactor)mLinePart.mDstFactor, GX_LO_SET);
    GXLoadPosMtxImm(mPosMtx, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    GXBegin(GX_LINES, GX_VTXFMT0, 2);
    GXPosition3f32(start.x, start.y, 0);
    GXColor1u32(mColorTL);
    GXPosition3f32(end.x, end.y, 0);
    GXColor1u32(mColorBR);
    GXEnd();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGBA4, 0);
}

void J2DGrafContext::lineTo(JGeometry::TVec2<f32> pos) {
    this->line(mPrevPos, pos);
    mPrevPos = pos;
}
