#include "d/dolzel.h" // IWYU pragma: keep

#include "JSystem/J2DGraph/J2DAnimation.h"
#include "JSystem/J2DGraph/J2DGrafContext.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "JSystem/J3DGraphBase/J3DDrawBuffer.h"
#include "SSystem/SComponent/c_bg_s_shdw_draw.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_com_inf_game.h"
#include "d/d_drawlist.h"
#include "d/d_s_play.h"
#include "m_Do/m_Do_graphic.h"
#include "m_Do/m_Do_lib.h"
#include "m_Do/m_Do_mtx.h"

class dDlst_blo_c : public dDlst_base_c {
public:
    virtual void draw();
    bool create(JKRArchive* param_1, char* param_2) {
        return mScreen.setPriority(param_2,0x20000,param_1) != 0;
    }

    J2DPane* getPane(u64 i_tag) {
        return mScreen.search(i_tag);
    }

    J2DPicture* getPicture(u64 i_tag) {
        J2DPane* pane = getPane(i_tag);
        JUT_ASSERT(1553, pane != NULL);
        if (pane->getTypeID() != 0x12) {
            return NULL;
        }
        return (J2DPicture*)pane;
    }

    /* 0x004 */ int field_0x4;
    /* 0x008 */ J2DScreen mScreen;
    /* 0x120 */ f32 field_0x120;
    /* 0x124 */ f32 field_0x124;
};

class dDlst_2Dm_c {
public:
    virtual void draw();

    /* 0x04 */ s16 field_0x4;
    /* 0x06 */ s16 field_0x6;
    /* 0x08 */ s16 field_0x8;
    /* 0x0A */ s16 field_0xa;
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ GXColor field_0x14;
    /* 0x18 */ GXColor field_0x18;
    /* 0x1C */ GXTexObj field_0x1c;
    /* 0x3C */ GXTlutObj field_0x3c;
    /* 0x48 */ s16 field_0x48;
    /* 0x4A */ s16 field_0x4a;
    /* 0x4C */ u8 field_0x4c;
    /* 0x50 */ GXTexObj field_0x50;
    /* 0x70 */ GXTlutObj field_0x70;
    /* 0x7C */ s16 field_0x7c;
    /* 0x7E */ s16 field_0x7e;
    /* 0x80 */ u8 field_0x80;
};

class dDlst_2DTri_c {
public:
    virtual void draw();

    /* 0x04 */ s16 field_0x4;
    /* 0x06 */ s16 field_0x6;
    /* 0x08 */ GXColor field_0x8;
    /* 0x0C */ s16 field_0xc;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
};

class dDlst_2DT_c : public dDlst_base_c {
public:
    dDlst_2DT_c() {}
    virtual void draw();

    /* 0x04 */ u8* field_0x4;
    /* 0x08 */ u32 field_0x8;
    /* 0x0C */ u8 field_0xc;
    /* 0x0D */ u8 field_0xd;
    /* 0x0E */ u16 field_0xe;
    /* 0x10 */ u16 field_0x10;
    /* 0x12 */ s16 field_0x12;
    /* 0x14 */ s16 field_0x14;
    /* 0x16 */ s16 field_0x16;
    /* 0x18 */ s16 field_0x18;
    /* 0x1C */ f32 field_0x1c;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ f32 field_0x28;
};

class dDlst_2DPoint_c {
public:
    virtual void draw();

    /* 0x04 */ s16 field_0x4;
    /* 0x06 */ s16 field_0x6;
    /* 0x08 */ GXColor field_0x8;
    /* 0x0C */ u8 field_0xc;
};

class dDlst_2DMt_tex_c {
public:
    u8 check() { return field_0x0; }
    int getCI() { return mCI; }
    GXTexObj* getTexObj() { return &mTexObj; }
    GXTlutObj* getTlutObj() { return &mTlutObj; }
    GXColor* getColor() { return &mColor; }
    f32 getS() { return mS; }
    f32 getT() { return mT; }
    f32 getSw() { return mSw; }
    f32 getTw() { return mTw; }

    /* 0x00 */ u8 field_0x0;
    /* 0x01 */ u8 mCI;
    /* 0x04 */ GXTexObj mTexObj;
    /* 0x24 */ GXTlutObj mTlutObj;
    /* 0x30 */ GXColor mColor;
    /* 0x34 */ f32 mS;
    /* 0x38 */ f32 mT;
    /* 0x3C */ f32 mSw;
    /* 0x40 */ f32 mTw;
};

class dDlst_2DMt_c {
public:
    virtual void draw();

    /* 0x04 */ u8 field_0x4;
    /* 0x08 */ dDlst_2DMt_tex_c* field_0x8;
    /* 0x0C */ s16 field_0xc;
    /* 0x0E */ s16 field_0xe;
    /* 0x10 */ s16 field_0x10;
    /* 0x12 */ s16 field_0x12;
};

class dDlst_2DM_c {
public:
    virtual void draw();

    /* 0x04 */ s16 field_0x4;
    /* 0x06 */ s16 field_0x6;
    /* 0x08 */ s16 field_0x8;
    /* 0x0A */ s16 field_0xa;
    /* 0x0C */ u8 field_0xc;
    /* 0x0D */ GXColor field_0xd;
    /* 0x11 */ GXColor field_0x11;
    /* 0x18 */ void* field_0x18;
    /* 0x1C */ u8 field_0x1c;
    /* 0x1E */ u16 field_0x1e;
    /* 0x20 */ u16 field_0x20;
    /* 0x22 */ s16 field_0x22;
    /* 0x24 */ s16 field_0x24;
    /* 0x28 */ void* field_0x28;
    /* 0x2C */ u8 field_0x2c;
    /* 0x2E */ u16 field_0x2e;
    /* 0x30 */ u16 field_0x30;
    /* 0x32 */ s16 field_0x32;
    /* 0x34 */ s16 field_0x34;
};

class ShdwDrawPoly_c : public cBgS_ShdwDraw {
public:
    virtual ~ShdwDrawPoly_c() {}

    void setCenter(cXyz* center) { mCenter = center; }
    cXyz* getCenter() { return mCenter; }
    void setLightVec(cXyz* lightVec) { mLightVec = lightVec; }
    cXyz* getLightVec() { return mLightVec; }
    void setPoly(dDlst_shadowPoly_c* poly) { mPoly = poly; }
    dDlst_shadowPoly_c* getPoly() { return mPoly; }

    /* 0x34 */ cXyz* mCenter;
    /* 0x38 */ cXyz* mLightVec;
    /* 0x3c */ dDlst_shadowPoly_c* mPoly;
};

void dDlst_window_c::setViewPort(f32 xOrig, f32 yOrig, f32 width, f32 height, f32 nearZ, f32 farZ) {
    mViewport.x_orig = xOrig;
    mViewport.y_orig = yOrig;
    mViewport.width = width;
    mViewport.height = height;
    mViewport.near_z = nearZ;
    mViewport.far_z = farZ;
}

void dDlst_window_c::setScissor(f32 xOrig, f32 yOrig, f32 width, f32 height) {
    mViewport.scissor.x_orig = xOrig;
    mViewport.scissor.y_orig = yOrig;
    mViewport.scissor.width = width;
    mViewport.scissor.height = height;
}

void dDlst_2DTri_c::draw() {
    f32 f4;
    f32 f5;
    f32 f2 = cM_scos(field_0xc);
    f32 f3 = cM_ssin(field_0xc);
    s16 x[3];
    s16 y[3];
    int r8 = 0;
    for (int i = 0; i < 3; i++) {
        f4 = field_0x10 * cM_ssin(r8);
        f5 = field_0x14 * cM_scos(r8);
        x[i] = field_0x4 + s16(f4 * f2 + f5 * f3);
        y[i] = field_0x6 + s16(f5 * f2 - f4 * f3);
        r8 -= 0x5555;
    }
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, field_0x8);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    GXLoadPosMtxImm(cMtx_getIdentity(), GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXBegin(GX_TRIANGLES, GX_VTXFMT0, 3);
    GXPosition3s16(x[0], y[0], 0);
    GXPosition3s16(x[1], y[1], 0);
    GXPosition3s16(x[2], y[2], 0);
    GXEnd();
    dComIfGp_getCurrentGrafPort()->setup2D();
}

void dDlst_2DQuad_c::draw() {
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, mColor);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    GXLoadPosMtxImm(cMtx_getIdentity(), GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3s16((s32)mPosX, (s32)mPosY, 0);
    GXPosition3s16((s32)mSizeX, (s32)mPosY, 0);
    GXPosition3s16((s32)mSizeX, (s32)mSizeY, 0);
    GXPosition3s16((s32)mPosX, (s32)mSizeY, 0);
    GXEnd();
    dComIfGp_getCurrentGrafPort()->setup2D();
}

void dDlst_2DPoint_c::draw() {
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetChanMatColor(GX_COLOR0A0, field_0x8);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    GXSetPointSize(field_0xc, GX_TO_ZERO);
    GXLoadPosMtxImm(cMtx_getIdentity(), GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXBegin(GX_POINTS, GX_VTXFMT0, 1);
    GXPosition3s16(field_0x4, field_0x6, 0);
    GXEnd();
    dComIfGp_getCurrentGrafPort()->setup2D();
}

void dDlst_2DT_c::draw() {
    static GXColor l_color = {0xFF, 0xFF, 0xFF, 0xE0};
    f32 var5 = field_0xe;
    f32 var6 = field_0x10;
    f32 var11 = (field_0x16 - field_0x12) * 0.5f / field_0x24;
    f32 var12 = (field_0x18 - field_0x14) * 0.5f / field_0x28;
    u16 var1 = (field_0x1c - var11) / var5 * 32768.0f;
    u16 var2 = (field_0x20 - var12) / var6 * 32768.0f;
    u16 var3 = (field_0x1c + var11) / var5 * 32768.0f;
    u16 var4 = (field_0x20 + var12) / var6 * 32768.0f;

    GXTexObj tex;
    GXInitTexObj(&tex, field_0x4, field_0xe, field_0x10, (GXTexFmt)field_0xc, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObjLOD(&tex, GX_LINEAR, GX_LINEAR, 0.0, 0.0, 0.0, 0, 0, GX_ANISO_1);
    GXLoadTexObj(&tex, GX_TEXMAP0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBX8, 0xf);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    l_color.a = field_0xd;
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    GXSetZCompLoc(GX_FALSE);
    GXSetZMode(GX_DISABLE, GX_ALWAYS, GX_DISABLE);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_SET);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, g_clearColor);
    GXSetCullMode(GX_CULL_NONE);
    GXSetDither(GX_TRUE);
    GXLoadPosMtxImm(cMtx_getIdentity(), GX_PNMTX0);
    GXSetChanMatColor(GX_COLOR0A0, l_color);
    GXSetClipMode(GX_CLIP_DISABLE);
    GXSetCurrentMtx(GX_PNMTX0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3s16((s32)field_0x12, (s32)field_0x14, 0);
    GXColor1u32(0xffffffff);
    GXTexCoord2u16(var1, var2);
    GXPosition3s16((s32)field_0x16, (s32)field_0x14, 0);
    GXColor1u32(0xffffffff);
    GXTexCoord2u16(var3, var2);
    GXPosition3s16((s32)field_0x16, (s32)field_0x18, 0);
    GXColor1u32(0xffffffff);
    GXTexCoord2u16(var3, var4);
    GXPosition3s16((s32)field_0x12, (s32)field_0x18, 0);
    GXColor1u32(0xffffffff);
    GXTexCoord2u16(var1, var4);
    GXEnd();
    GXSetClipMode(GX_CLIP_ENABLE);
    dComIfGp_getCurrentGrafPort()->setup2D();
}

void dDlst_2DT2_c::draw() {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBA6, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXLoadTexObj(&mTexObj, GX_TEXMAP0);
    GXSetNumChans(0);
    GXSetTevColor(GX_TEVREG0, field_0x3c);
    GXSetTevColor(GX_TEVREG1, field_0x40);
    GXSetNumTexGens(1);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    if (field_0x46) {
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    } else {
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A1, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    }
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetZCompLoc(GX_FALSE);
    GXSetZMode(GX_DISABLE, GX_ALWAYS, GX_DISABLE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, g_clearColor);
    GXSetCullMode(GX_CULL_NONE);
    GXSetDither(GX_TRUE);
    GXLoadPosMtxImm(cMtx_getIdentity(), GX_PNMTX0);
    GXSetClipMode(GX_CLIP_DISABLE);
    GXSetCurrentMtx(GX_PNMTX0);
    f32 f31 = field_0x24 + field_0x2c;
    f32 f30 = field_0x28 + field_0x30;
    if (field_0x44 && field_0x45) {
        f32 f29 = field_0x24 + field_0x2c * 0.5f;
        f32 f28 = field_0x28 + field_0x30 * 0.5f;
        f32 f27;
        f32 f26;
        if (mScaleX == 0.0f) {
            f27 = 0.0f;
            f26 = 0.0f;
        } else {
            f27 = 1.0f - 1.0f / mScaleX;
            f26 = 1.0f;
        }
        f32 f25;
        f32 f24;
        if (mScaleY == 0.0f) {
            f25 = 0.0f;
            f24 = 0.0f;
        } else {
            f25 = 1.0f - 1.0f / mScaleY;
            f24 = 1.0f;
        }
        GXBegin(GX_QUADS, GX_VTXFMT0, 16);
        GXPosition2f32(field_0x24, field_0x28);
        GXTexCoord2f32(f27, f25);
        GXPosition2f32(f29, field_0x28);
        GXTexCoord2f32(f26, f25);
        GXPosition2f32(f29, f28);
        GXTexCoord2f32(f26, f24);
        GXPosition2f32(field_0x24, f28);
        GXTexCoord2f32(f27, f24);
        GXPosition2f32(f29, field_0x28);
        GXTexCoord2f32(f26, f25);
        GXPosition2f32(f31, field_0x28);
        GXTexCoord2f32(f27, f25);
        GXPosition2f32(f31, f28);
        GXTexCoord2f32(f27, f24);
        GXPosition2f32(f29, f28);
        GXTexCoord2f32(f26, f24);
        GXPosition2f32(field_0x24, f28);
        GXTexCoord2f32(f27, f24);
        GXPosition2f32(f29, f28);
        GXTexCoord2f32(f26, f24);
        GXPosition2f32(f29, f30);
        GXTexCoord2f32(f26, f25);
        GXPosition2f32(field_0x24, f30);
        GXTexCoord2f32(f27, f25);
        GXPosition2f32(f29, f28);
        GXTexCoord2f32(f26, f24);
        GXPosition2f32(f31, f28);
        GXTexCoord2f32(f27, f24);
        GXPosition2f32(f31, f30);
        GXTexCoord2f32(f27, f25);
        GXPosition2f32(f29, f30);
        GXTexCoord2f32(f26, f25);
        GXEnd();
    } else if (field_0x44) {
        f32 f28 = field_0x24 + field_0x2c * 0.5f;
        f32 f29;
        f32 f27;
        if (mScaleX == 0.0f) {
            f29 = 0.0f;
            f27 = 0.0f;
        } else {
            f29 = 1.0f - 1.0f / mScaleX;
            f27 = 1.0f;
        }
        f32 f24;
        f32 stack_38;
        if (mScaleY == 0.0f) {
            f24 = 1.0f;
            stack_38 = 1.0f;
        } else {
            f24 = 0.5f - (1.0f / mScaleY) * 0.5f;
            stack_38 = (1.0f / mScaleY) * 0.5f + 0.5f;
        }
        GXBegin(GX_QUADS, GX_VTXFMT0, 8);
        GXPosition2f32(field_0x24, field_0x28);
        GXTexCoord2f32(f29, f24);
        GXPosition2f32(f28, field_0x28);
        GXTexCoord2f32(f27, f24);
        GXPosition2f32(f28, f30);
        GXTexCoord2f32(f27, 1.0f);
        GXPosition2f32(field_0x24, f30);
        GXTexCoord2f32(f29, 1.0f);
        GXPosition2f32(f28, field_0x28);
        GXTexCoord2f32(f27, f24);
        GXPosition2f32(f31, field_0x28);
        GXTexCoord2f32(f29, f24);
        GXPosition2f32(f31, f30);
        GXTexCoord2f32(f29, 1.0f);
        GXPosition2f32(f28, f30);
        GXTexCoord2f32(f27, 1.0f);
        GXEnd();
    } else if (field_0x45) {
        f32 f24 = field_0x28 + 0.5f * field_0x30;
        f32 f25;
        f32 f26;
        if (mScaleX == 0.0f) {
            f25 = 1.0f;
            f26 = 1.0f;
        } else {
            f25 = 0.5f - (1.0f / mScaleX * 0.5f);
            f26 = 0.5f + (1.0f / mScaleX * 0.5f);
        }
        f32 f27;
        f32 f28;
        if (mScaleY == 0.0f) {
            f27 = 0.0f;
            f28 = 0.0f;
        } else {
            f27 = 1.0f - 1.0f / mScaleY;
            f28 = 1.0f;
        }
        GXBegin(GX_QUADS, GX_VTXFMT0, 8);
        GXPosition2f32(field_0x24, field_0x28);
        GXTexCoord2f32(f25, f27);
        GXPosition2f32(f31, field_0x28);
        GXTexCoord2f32(f26, f27);
        GXPosition2f32(f31, f24);
        GXTexCoord2f32(f26, f28);
        GXPosition2f32(field_0x24, f24);
        GXTexCoord2f32(f25, f28);
        GXPosition2f32(field_0x24, f24);
        GXTexCoord2f32(f25, f28);
        GXPosition2f32(f31, f24);
        GXTexCoord2f32(f26, f28);
        GXPosition2f32(f31, f30);
        GXTexCoord2f32(f26, f27);
        GXPosition2f32(field_0x24, f30);
        GXTexCoord2f32(f25, f27);
        GXEnd();
    } else {
        f32 f24;
        f32 f25;
        if (mScaleX == 0.0f) {
            f24 = 0.0f;
            f25 = 0.0f;
        } else {
            f24 = 0.5f - (1.0f / mScaleX * 0.5f);
            f25 = 0.5f + (1.0f / mScaleX * 0.5f);
        }
        f32 f26;
        f32 f27;
        if (mScaleY == 0.0f) {
            f26 = 0.0f;
            f27 = 0.0f;
        } else {
            f26 = 0.5f - (1.0f / mScaleY * 0.5f);
            f27 = 0.5f + (1.0f / mScaleY * 0.5f);
        }
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition2f32(field_0x24, field_0x28);
        GXTexCoord2f32(f24, f26);
        GXPosition2f32(f31, field_0x28);
        GXTexCoord2f32(f25, f26);
        GXPosition2f32(f31, f30);
        GXTexCoord2f32(f25, f27);
        GXPosition2f32(field_0x24, f30);
        GXTexCoord2f32(f24, f27);
        GXEnd();
    }
    GXSetClipMode(GX_CLIP_ENABLE);
    dComIfGp_getCurrentGrafPort()->setup2D();
}

static u8 l_frontZMat[] ATTRIBUTE_ALIGN(32) = {
    0x61, 0x40, 0x00, 0x00, 0x07, 0x10, 0x00, 0x00, 0x10, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x10,
    0x00, 0x00, 0x10, 0x09, 0x00, 0x00, 0x00, 0x01, 0x61, 0x00, 0x00, 0x40, 0x10, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static u8 l_frontNoZSubMat[] ATTRIBUTE_ALIGN(32) = {
    0x61, 0x40, 0x00, 0x00, 0x06, 0x61, 0x41, 0x00, 0x09, 0x35, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static Vec l_shadowVolPos[] = {
    {-1.0f, -1.0f, 0.0f}, {-1.0f, -1.0f, -300.0f}, {-1.0f, 1.0f, 0.0f}, {-1.0f, 1.0f, -300.0f},
    {1.0f, -1.0f, 0.0f},  {1.0f, -1.0f, -300.0f},  {1.0f, 1.0f, 0.0f},  {1.0f, 1.0f, -300.0f},
};

static u8 l_shadowVolDL[] ATTRIBUTE_ALIGN(32) = {
    0x80, 0x00, 0x18, 0x06, 0x02, 0x03, 0x07, 0x00, 0x04, 0x05, 0x01, 0x06, 0x04, 0x00, 0x02,
    0x07, 0x05, 0x04, 0x06, 0x03, 0x01, 0x05, 0x07, 0x02, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static u8 l_shadowProjMat[] ATTRIBUTE_ALIGN(32) = {
    0x61, 0x28, 0x38, 0x03, 0xC0, 0x61, 0xC0, 0x08, 0xFF, 0xFF, 0x61, 0xC1, 0x08, 0xE6, 0x70,
    0x61, 0x43, 0x00, 0x00, 0x01, 0x61, 0x40, 0x00, 0x00, 0x07, 0x61, 0x41, 0x00, 0x04, 0xAD,
    0x61, 0xF3, 0x64, 0x00, 0x00, 0x10, 0x00, 0x00, 0x10, 0x3F, 0x00, 0x00, 0x00, 0x01, 0x10,
    0x00, 0x00, 0x10, 0x09, 0x00, 0x00, 0x00, 0x00, 0x61, 0x00, 0x00, 0x40, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static u8 l_shadowVolMat[] ATTRIBUTE_ALIGN(32) = {
    0x61, 0x28, 0x38, 0x00, 0x00, 0x61, 0xC0, 0x08, 0xFF, 0xFC, 0x61, 0xC1, 0x08, 0xFF, 0x90,
    0x61, 0x43, 0x00, 0x00, 0x41, 0x61, 0x40, 0x00, 0x00, 0x0D, 0x61, 0x41, 0x00, 0x01, 0x35,
    0x61, 0xF3, 0x7F, 0x00, 0x00, 0x10, 0x00, 0x00, 0x10, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x10,
    0x00, 0x00, 0x10, 0x09, 0x00, 0x00, 0x00, 0x01, 0x61, 0x00, 0x00, 0x80, 0x10, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static u8 l_clearMat[] ATTRIBUTE_ALIGN(32) = {
    0x61, 0x28, 0x38, 0x00, 0x00, 0x61, 0xC0, 0x08, 0xFF, 0xFF, 0x61, 0xC1, 0x08, 0xFF, 0xA0,
    0x61, 0x40, 0x00, 0x00, 0x06, 0x61, 0x41, 0x00, 0x00, 0x14, 0x61, 0xF3, 0x7F, 0x00, 0x00,
    0x10, 0x00, 0x00, 0x10, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x10, 0x09, 0x00,
    0x00, 0x00, 0x01, 0x61, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static u8 l_frontMat[] ATTRIBUTE_ALIGN(32) = {
    0x61, 0x28, 0x38, 0x00, 0x00, 0x61, 0xC0, 0x08, 0xFF, 0xFF, 0x61, 0xC1, 0x08, 0xFF, 0x90,
    0x61, 0x43, 0x00, 0x00, 0x41, 0x61, 0x40, 0x00, 0x00, 0x07, 0x61, 0x41, 0x00, 0x01, 0x15,
    0x10, 0x00, 0x00, 0x10, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x10, 0x09, 0x00,
    0x00, 0x00, 0x01, 0x61, 0x00, 0x00, 0x40, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static u8 l_backSubMat[] ATTRIBUTE_ALIGN(32) = {
    0x61, 0x41, 0x00, 0x09, 0x35, 0x10, 0x00, 0x00, 0x10, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x10,
    0x00, 0x00, 0x10, 0x09, 0x00, 0x00, 0x00, 0x01, 0x61, 0x00, 0x00, 0x80, 0x10, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static Vec l_simpleShadowPos[] = {
    {0.0f, -1.0f, 0.0f},   {0.0f, 0.0f, 0.0f},   {1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, -1.0f},
    {-1.0f, -1.0f, -1.0f}, {-1.0f, 0.0f, -1.0f}, {-1.0f, -1.0f, 1.0f}, {-1.0f, 0.0f, 1.0f},
    {1.0f, -1.0f, 1.0f},   {1.0f, 0.0f, 1.0f},   {-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, -1.0f},
    {1.0f, 1.0f, 1.0f},    {-1.0f, 1.0f, 1.0f},
};

static u8 l_shadowVolumeDL[] ATTRIBUTE_ALIGN(32) = {
    0x98, 0x00, 0x05, 0x03, 0x09, 0x01, 0x07, 0x05, 0x98, 0x00, 0x05, 0x04, 0x06, 0x00, 0x08,
    0x02, 0x98, 0x00, 0x04, 0x04, 0x05, 0x06, 0x07, 0x98, 0x00, 0x04, 0x02, 0x03, 0x04, 0x05,
    0x98, 0x00, 0x04, 0x08, 0x09, 0x02, 0x03, 0x98, 0x00, 0x04, 0x06, 0x07, 0x08, 0x09, 0x98,
    0x00, 0x03, 0x01, 0x05, 0x03, 0x98, 0x00, 0x03, 0x00, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static u8 l_shadowSealTexDL[] ATTRIBUTE_ALIGN(32) = {
    0x61, 0x28, 0x38, 0x03, 0xC0, 0x61, 0x40, 0x00, 0x00, 0x06, 0x61, 0x41, 0x00, 0x06, 0x15,
    0x10, 0x00, 0x00, 0x10, 0x3F, 0x00, 0x00, 0x00, 0x01, 0x10, 0x00, 0x00, 0x10, 0x09, 0x00,
    0x00, 0x00, 0x00, 0x61, 0x00, 0x00, 0x40, 0x01, 0x80, 0x00, 0x04, 0x0A, 0x00, 0x00, 0x0B,
    0x01, 0x00, 0x0C, 0x01, 0x01, 0x0D, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static u8 l_shadowSealTex2DL[] ATTRIBUTE_ALIGN(32) = {
    0x61, 0x28, 0x38, 0x03, 0xC0, 0x61, 0x40, 0x00, 0x00, 0x06, 0x61, 0x41, 0x00, 0x06, 0x15,
    0x10, 0x00, 0x00, 0x10, 0x3F, 0x00, 0x00, 0x00, 0x01, 0x10, 0x00, 0x00, 0x10, 0x09, 0x00,
    0x00, 0x00, 0x00, 0x61, 0x00, 0x00, 0x40, 0x01, 0x80, 0x00, 0x04, 0x0A, 0x00, 0x00, 0x0B,
    0x02, 0x00, 0x0C, 0x02, 0x01, 0x0D, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static u8 l_shadowSealDL[] ATTRIBUTE_ALIGN(32) = {
    0x10, 0x00, 0x00, 0x10, 0x0E, 0x00, 0x00, 0x05, 0x00, 0x10, 0x00, 0x00, 0x10, 0x10, 0x00,
    0x00, 0x05, 0x00, 0x61, 0x28, 0x38, 0x00, 0x00, 0x61, 0xC0, 0x08, 0xFF, 0xFF, 0x61, 0xC1,
    0x08, 0xFF, 0xB0, 0x61, 0x40, 0x00, 0x00, 0x06, 0x61, 0x41, 0x00, 0x06, 0xED, 0x10, 0x00,
    0x00, 0x10, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x10, 0x09, 0x00, 0x00, 0x00,
    0x01, 0x61, 0x00, 0x00, 0x40, 0x10, 0x80, 0x00, 0x04, 0x0A, 0x0B, 0x0C, 0x0D, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

dDlst_2DT2_c::dDlst_2DT2_c() {
    field_0x40 = (GXColor){0, 0, 0, 0};
}

void dDlst_2DT2_c::init(ResTIMG* i_timg, f32 param_1, f32 param_2, f32 param_3, f32 param_4,
                        u8 param_5, u8 param_6, u8 param_7, f32 i_scaleX, f32 i_scaleY) {
    mDoLib_setResTimgObj(i_timg, &mTexObj, 0, NULL);
    field_0x24 = param_1;
    field_0x28 = param_2;
    field_0x2c = param_3;
    field_0x30 = param_4;
    mScaleX = i_scaleX;
    mScaleY = i_scaleY;
    field_0x3c = (GXColor){0, 0, 0, 255};

    field_0x44 = param_6;
    if (field_0x44 != 0 && GXGetTexObjWrapS(&mTexObj) == 2) {
        field_0x44 = 0;
    }

    field_0x45 = param_7;
    if (field_0x45 != 0 && GXGetTexObjWrapT(&mTexObj) == 2) {
        field_0x45 = 0;
    }

    field_0x46 = param_5;
}

void dDlst_2DM_c::draw() {
    s16 r31 = field_0x22;
    s16 r30 = field_0x24;
    int r29 = field_0x22 + 256.0f;
    int r28 = field_0x24 + 256.0f;
    f32 f4 = 256.0f / field_0x2e;
    f32 f3 = 256.0f / field_0x30;
    int r27 = field_0x32 * f4;
    int r26 = field_0x34 * f3;
    s16 r25 = r27 + s16(field_0x1e * f4);
    s16 r24 = r26 + s16(field_0x20 * f3);
    GXTexObj tex[2];
    void** stack_1c = &field_0x18;
    GXInitTexObj(&tex[0], field_0x18, field_0x1e, field_0x20, GXTexFmt(field_0x1c), GX_CLAMP, GX_CLAMP, 0);
    GXInitTexObjLOD(&tex[0], GX_LINEAR, GX_LINEAR, 0.0, 0.0, 0.0, 0, 0, GX_ANISO_1);
    GXTexWrapMode wrap = field_0xc ? GX_REPEAT : GX_CLAMP;
    GXInitTexObj(&tex[1], field_0x28, field_0x2e, field_0x30, GXTexFmt(field_0x2c), wrap, wrap, 0);
    GXInitTexObjLOD(&tex[1], GX_LINEAR, GX_LINEAR, 0.0, 0.0, 0.0, 0, 0, GX_ANISO_1);
    GXLoadTexObj(&tex[0], GX_TEXMAP0);
    GXLoadTexObj(&tex[1], GX_TEXMAP1);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBA4, 8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_CLR_RGBA, GX_RGBA4, 8);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
    GXSetNumChans(0);
    GXSetTevColor(GX_TEVREG0, field_0xd);
    GXSetTevColor(GX_TEVREG1, field_0x11);
    GXSetNumTexGens(2);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_IDENTITY);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A1, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_TEXC, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_TEXA, GX_CA_APREV, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3s16((s32)field_0x4, (s32)field_0x6, 0);
    GXTexCoord2s16(r31, r30);
    GXTexCoord2s16(r27, r26);
    GXPosition3s16((s32)field_0x8, (s32)field_0x6, 0);
    GXTexCoord2s16(r29, r30);
    GXTexCoord2s16(r25, r26);
    GXPosition3s16((s32)field_0x8, (s32)field_0xa, 0);
    GXTexCoord2s16(r29, r28);
    GXTexCoord2s16(r25, r24);
    GXPosition3s16((s32)field_0x4, (s32)field_0xa, 0);
    GXTexCoord2s16(r31, r28);
    GXTexCoord2s16(r27, r24);
    GXEnd();
    dComIfGp_getCurrentGrafPort()->setup2D();
}


void dDlst_2Dm_c::draw() {
    s16 r31 = field_0x48;
    s16 r30 = field_0x4a;
    int r29 = field_0x48 + 256.0f;
    int r28 = field_0x4a + 256.0f;
    f32 f31 = field_0xc * 256.0f / GXGetTexObjWidth(&field_0x50);
    f32 f30 = field_0x10 * 256.0f / GXGetTexObjHeight(&field_0x50);
    int r27 = field_0x7c * f31;
    int r26 = field_0x7e * f30;
    s16 r25 = r27 + s16(f31 * GXGetTexObjWidth(&field_0x1c));
    s16 r24 = r26 + s16(f30 * GXGetTexObjHeight(&field_0x1c));
    GXTexObj* stack_18 = &field_0x1c;
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBA4, 8);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX1, GX_CLR_RGBA, GX_RGBA4, 8);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX1, GX_DIRECT);
    GXLoadTexObj(&field_0x1c, GX_TEXMAP0);
    if (field_0x4c) {
        GXLoadTlut(&field_0x3c, GXGetTexObjTlut(&field_0x1c));
    }
    GXLoadTexObj(&field_0x50, GX_TEXMAP1);
    if (field_0x80) {
        GXLoadTlut(&field_0x70, GXGetTexObjTlut(&field_0x50));
    }
    GXSetNumChans(0);
    GXSetTevColor(GX_TEVREG0, field_0x14);
    GXSetTevColor(GX_TEVREG1, field_0x18);
    GXSetNumTexGens(2);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX1, GX_IDENTITY);
    GXSetNumTevStages(2);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C1, GX_CC_C0, GX_CC_TEXC, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A1, GX_CA_A0, GX_CA_TEXA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_TEXC, GX_CC_CPREV, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_TEXA, GX_CA_APREV, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_OR, GX_GREATER, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    GXLoadPosMtxImm(cMtx_getIdentity(), GX_PNMTX0);
    GXSetCurrentMtx(0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3s16((s32)field_0x4, (s32)field_0x6, 0);
    GXTexCoord2s16(r31, r30);
    GXTexCoord2s16(r27, r26);
    GXPosition3s16((s32)field_0x8, (s32)field_0x6, 0);
    GXTexCoord2s16(r29, r30);
    GXTexCoord2s16(r25, r26);
    GXPosition3s16((s32)field_0x8, (s32)field_0xa, 0);
    GXTexCoord2s16(r29, r28);
    GXTexCoord2s16(r25, r24);
    GXPosition3s16((s32)field_0x4, (s32)field_0xa, 0);
    GXTexCoord2s16(r31, r28);
    GXTexCoord2s16(r27, r24);
    GXEnd();
    dComIfGp_getCurrentGrafPort()->setup2D();
}


void dDlst_2DMt_c::draw() {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    dDlst_2DMt_tex_c* r27 = field_0x8;
    int r28 = 0;
    for (int i = 0; i < field_0x4; i++) {
        if (r27->check()) {
            if (r27->getCI()) {
                GXLoadTlut(r27->getTlutObj(), i);
            }
            GXLoadTexObj(r27->getTexObj(), GXTexMapID(r28));
            GXSetVtxAttrFmt(GX_VTXFMT0, GXAttr(GX_VA_TEX0 + r28), GX_CLR_RGBA, GX_RGBA6, 0);
            GXSetVtxDesc(GXAttr(GX_VA_TEX0 + r28), GX_DIRECT);
            GXSetTevColor(GXTevRegID(GX_TEVREG0 + r28), *r27->getColor());
            GXSetTexCoordGen(GXTexCoordID(r28), GX_TG_MTX2x4, GXTexGenSrc(GX_TG_TEX0 + r28), GX_IDENTITY);
            GXSetTevOrder(GXTevStageID(r28), GXTexCoordID(r28), GXTexMapID(r28), GX_COLOR_NULL);
            GXSetTevColorIn(GXTevStageID(r28), GX_CC_ZERO, GXTevColorArg(GX_CC_C0 + r28*2), GX_CC_TEXC, r28 ? GX_CC_CPREV : GX_CC_ZERO);
            GXSetTevColorOp(GXTevStageID(r28), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
            GXSetTevAlphaIn(GXTevStageID(r28), GX_CA_ZERO, GXTevAlphaArg(GX_CA_A0 + r28), GX_CA_TEXA, r28 ? GX_CA_APREV : GX_CA_ZERO);
            GXSetTevAlphaOp(GXTevStageID(r28), GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
            r28++;
            if (r28 >= 3) {
                break;
            }
        }
        r27++;
    }
    if (r28) {
        GXSetNumChans(0);
        GXSetNumTexGens(r28);
        GXSetNumTevStages(r28);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3s16(field_0xc, field_0xe, 0);
        r27 = field_0x8;
        for (int i = 0; i < field_0x4; i++) {
            if (r27->check()) {
                GXTexCoord2f32(r27->getS(), r27->getT());
            }
            r27++;
        }
        GXPosition3s16(field_0x10, field_0xe, 0);
        r27 = field_0x8;
        for (int i = 0; i < field_0x4; i++) {
            if (r27->check()) {
                GXTexCoord2f32(r27->getS() + r27->getSw(), r27->getT());
            }
            r27++;
        }
        GXPosition3s16(field_0x10, field_0x12, 0);
        r27 = field_0x8;
        for (int i = 0; i < field_0x4; i++) {
            if (r27->check()) {
                GXTexCoord2f32(r27->getS() + r27->getSw(), r27->getT() + r27->getTw());
            }
            r27++;
        }
        GXPosition3s16(field_0xc, field_0x12, 0);
        r27 = field_0x8;
        for (int i = 0; i < field_0x4; i++) {
            if (r27->check()) {
                GXTexCoord2f32(r27->getS(), r27->getT() + r27->getTw());
            }
            r27++;
        }
        GXEnd();
        dComIfGp_getCurrentGrafPort()->setup2D();
    }
}

// stand-in for a stripped function that calls J2DPicture::getTexture
JUTTexture* dummy_getTexture(J2DPicture* picture) {
    return picture->getTexture(0);
}

dDlst_2D_c::dDlst_2D_c(ResTIMG* i_timg, s16 i_posX, s16 i_posY, s16 i_sizeX, s16 i_sizeY,
                       u8 i_alpha) {
    mpPicture.initiate(i_timg, NULL);
    mPosX = i_posX;
    mPosY = i_posY;
    mSizeX = i_sizeX;
    mSizeY = i_sizeY;
    mAlpha = i_alpha;
}

void dDlst_2D_c::draw() {
    mpPicture.setAlpha(mAlpha);
    mpPicture.draw(mPosX, mPosY, mSizeX, mSizeY, false, false, false);
}

void dDlst_blo_c::draw() {
    mScreen.draw(field_0x120, field_0x124, dComIfGp_getCurrentGrafPort());
}

// stand-in for a function that pulls in a bunch of inline functions but was presumably stripped
static void dummy_misc() {
    J2DScreen* screen;
    J2DPane* pane;
    J2DPicture* picture;
    J2DAnmBase* anmBase;
    delete anmBase;
    picture->setBlack(JUtility::TColor(0, 0, 0, 0));
    picture->setWhite(JUtility::TColor(0, 0, 0, 0));
    pane->getTypeID();
    pane->makeMatrix(0.0f, 0.0f);
    pane->calcMtx();
    screen->clearAnmTransform();
}

void cM_rnd_c::init(int s0, int s1, int s2) {
    seed0 = s0;
    seed1 = s1;
    seed2 = s2;
}

f32 cM_rnd_c::get() {
    seed0 = seed0 * 171 % 30269;
    seed1 = seed1 * 172 % 30307;
    seed2 = seed2 * 170 % 30323;
    f32 newSeed = (seed0 / 30269.0f) + (seed1 / 30307.0f) + (seed2 / 30323.0f);

    return fabsf(fmodf(newSeed, 1.0));
}

f32 cM_rnd_c::getF(f32 param_0) {
    return get() * param_0;
}

f32 cM_rnd_c::getFX(f32 param_0) {
    return ((get() - 0.5f) * param_0) * 2.0f;
}

f32 cM_rnd_c::getValue(f32 param_0, f32 param_1) {
    return param_0 + getF(param_1);
}

void dDlst_effectLine_c::draw() {
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0, 0, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevColor(GX_TEVREG0, mLineColor);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetZMode(GX_DISABLE, GX_LEQUAL, GX_DISABLE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetCullMode(GX_CULL_NONE);
    GXLoadPosMtxImm(j3dSys.getViewMtx(), GX_PNMTX0);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetCurrentMtx(0);
    int r31 = getRndValue(field_0x20, field_0x22);
    cXyz local_68;
    cXyz local_74;
    for (int i = 0; i < r31; i++) {
        int r23 = getRndFX(32767.0f);
        float tmp = getRndValue(field_0x28, field_0x2c);
        local_68.x = field_0x10.x + tmp * cM_ssin(r23);
        local_68.y = field_0x10.y + tmp * cM_scos(r23);
        local_68.z = field_0x10.z;
        tmp = getRndValue(field_0x30, field_0x34);
        local_74.x = local_68.x + tmp * cM_ssin(r23);
        local_74.y = local_68.y + tmp * cM_scos(r23);
        local_74.z = field_0x10.z;
        cMtx_multVec(dComIfGd_getInvViewMtx(), &local_68, &local_68);
        cMtx_multVec(dComIfGd_getInvViewMtx(), &local_74, &local_74);
        tmp = getRndValue(field_0x24, field_0x26);
        GXSetLineWidth(tmp, GX_TO_ZERO);
        GXBegin(GX_LINES, GX_VTXFMT0, 2);
        GXPosition3f32(local_68.x, local_68.y, local_68.z);
        GXPosition3f32(local_74.x, local_74.y, local_74.z);
        GXEnd();
    }
}

void dDlst_effectLine_c::update(cXyz& param_0, _GXColor& i_lineColor, u16 param_2, u16 param_3,
                                u16 param_4, u16 param_5, f32 param_6, f32 param_7, f32 param_8,
                                f32 param_9) {
    field_0x10 = param_0;
    mLineColor = i_lineColor;
    field_0x20 = param_2;
    field_0x22 = param_3;
    field_0x24 = param_4;
    field_0x26 = param_5;
    field_0x28 = param_6;
    field_0x2c = param_7;
    field_0x30 = param_8;
    field_0x34 = param_9;

    dComIfGd_set2DOpa(this);
}

int dDlst_shadowPoly_c::set(cBgD_Vtx_t* i_vtx, u16 param_1, u16 param_2, u16 param_3,
                            cM3dGPla* i_plane) {
    if (mCount >= getTriMax()) {
        return 0;
    }

    dDlst_shadowTri_c* dst = getTri() + mCount;

    cBgD_Vtx_t* vtx = i_vtx;
    cXyz b(*i_plane->GetNP());
    f32 temp_f3 = (1.0f - fabsf(b.y));
    temp_f3 *= temp_f3;

    b.x *= temp_f3;
    b.z *= temp_f3;
    b *= 2.0f;

    PSVECAdd(&vtx[param_1], &b, &dst->mPos[0]);
    PSVECAdd(&vtx[param_2], &b, &dst->mPos[1]);
    PSVECAdd(&vtx[param_3], &b, &dst->mPos[2]);
    mCount++;
    return 1;
}


void dDlst_shadowPoly_c::draw() {
    dDlst_shadowTri_c* tri = getTri();

    GXBegin(GX_TRIANGLES, GX_VTXFMT0, mCount * 3);
    for (int i = 0; i < mCount; i++) {
        GXPosition3f32(tri->mPos[2].x, tri->mPos[2].y, tri->mPos[2].z);
        GXPosition3f32(tri->mPos[1].x, tri->mPos[1].y, tri->mPos[1].z);
        GXPosition3f32(tri->mPos[0].x, tri->mPos[0].y, tri->mPos[0].z);
        tri++;
    }

    GXEnd();
}

static J3DDrawBuffer* J3DDrawBuffer__create(u32 size) {
    J3DDrawBuffer* buffer = new J3DDrawBuffer();

    if (buffer) {
        int error = buffer->allocBuffer(size);
        if (error == kJ3DError_Success) {
            return buffer;
        }
        delete buffer;
    }
    return NULL;
}

void dDlst_shadowReal_c::reset() {
    mState = 0;
    mModelNum = 0;
}

void dDlst_shadowReal_c::imageDraw(Mtx param_0) {
    GXSetProjection(mRenderProjMtx, GX_ORTHOGRAPHIC);
    JUT_ASSERT(1916, mModelNum);
    J3DModelData* model_data;
    J3DModel** models = mpModels;
    J3DShapePacket* shape_pkt;

    for (u8 i = 0; i < mModelNum; i++) {
        model_data = (*models)->getModelData();
        model_data->getShapeNodePointer(0)->loadPreDrawSetting();

        for (u16 j = 0; j < model_data->getShapeNum(); j++) {
            if (!model_data->getShapeNodePointer(j)->checkFlag(1)) {
                shape_pkt = (*models)->getShapePacket(j);
                shape_pkt->setBaseMtxPtr(&mViewMtx);
                shape_pkt->drawFast();
                shape_pkt->setBaseMtxPtr((Mtx*)param_0);
            }
        }

        models++;
    }
}

void dDlst_shadowReal_c::draw() {
    static GXColor l_color = {0, 0, 0, 64};
    l_color.a = field_0x1;

    GXSetTevColor(GX_TEVREG0, l_color);
    GXCallDisplayList(l_shadowProjMat, 0x40);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXLoadTexMtxImm(mReceiverProjMtx, GX_TEXMTX0, GX_MTX3x4);
    mShadowRealPoly.draw();
}

static int psdRealCallBack(cBgS_ShdwDraw* param_0, cBgD_Vtx_t* param_1, int param_2,
                               int param_3, int param_4, cM3dGPla* param_5) {
    ShdwDrawPoly_c* shdwDrawPoly = (ShdwDrawPoly_c*)param_0;
    const cXyz* normal = param_5->GetNP();
    if (shdwDrawPoly->getLightVec()->inprod(*normal) < -0.2f) {
        cXyz* center = shdwDrawPoly->getCenter();
        if (normal->x * center->x + normal->y * center->y + normal->z * center->z + param_5->GetD() > -90.f) {
            const cXyz* min = shdwDrawPoly->GetBndP()->GetMinP();
            const cXyz* max = shdwDrawPoly->GetBndP()->GetMaxP();
            cBgD_Vtx_t* vert1 = param_1 + param_2;
            cBgD_Vtx_t* vert2 = param_1 + param_3;
            cBgD_Vtx_t* vert3 = param_1 + param_4;
            if ((vert1->y < min->y && vert2->y < min->y && vert3->y < min->y)
            || (vert1->y > max->y && vert2->y > max->y && vert3->y > max->y)
            || (vert1->x < min->x && vert2->x < min->x && vert3->x < min->x)
            || (vert1->x > max->x && vert2->x > max->x && vert3->x > max->x)
            || (vert1->z < min->z && vert2->z < min->z && vert3->z < min->z)
            || (vert1->z > max->z && vert2->z > max->z && vert3->z > max->z)) {
                return 1;
            } else {
                return shdwDrawPoly->getPoly()->set(param_1, param_2, param_3, param_4, param_5);
            }
        }
    }
    return 1;
}

static BOOL realPolygonCheck(cXyz* param_0, f32 param_1, f32 param_2, cXyz* param_3,
                                 dDlst_shadowPoly_c* param_4) {
    ShdwDrawPoly_c shdwDrawPoly;
    cXyz local_8c;
    cXyz local_98;
    f32 tmp1 = param_1 * param_1 * 0.002f;
    f32 tmp2 = cLib_maxLimit(tmp1, 120.0f);
    f32 var1 = param_1 + param_2 - tmp2;
    local_8c.y = param_0->y - var1;
    local_98.y = param_0->y + param_1 * 0.15f;
    local_98.x = param_0->x + param_3->x * var1;
    if (local_98.x < param_0->x) {
        local_8c.x = local_98.x;
        local_98.x = param_0->x;
    } else {
        local_8c.x = param_0->x;
    }
    local_8c.x -= param_1;
    local_98.x += param_1;
    var1 = param_0->z + param_3->z * var1;
    local_98.z = var1;
    if (var1 < param_0->z) {
        local_8c.z = local_98.z;
        local_98.z = param_0->z;
    } else {
        local_8c.z = param_0->z;
    }
    local_8c.z -= param_1;
    local_98.z += param_1;
    mDoLib_clipper::changeFar(mDoLib_clipper::getFovyRate() * 10000.0f);
    s32 clip = mDoLib_clipper::clip(j3dSys.getViewMtx(), &local_98, &local_8c);
    mDoLib_clipper::resetFar();
    if (clip) {
        return FALSE;
    }
    shdwDrawPoly.Set(local_8c, local_98);
    shdwDrawPoly.SetCallback(psdRealCallBack);
    shdwDrawPoly.setCenter(param_0);
    shdwDrawPoly.setLightVec(param_3);
    shdwDrawPoly.setPoly(param_4);
    dComIfG_Bgsp().ShdwDraw(&shdwDrawPoly);
    return TRUE;
}

static void setkankyoShadow(dKy_tevstr_c* param_0, f32* param_1) {
    dScnKy_env_light_c* env_light = dKy_getEnvlight();

    if (param_0 != NULL) {
        *param_1 *= param_0->field_0x344;
    } else {
        *param_1 *= env_light->field_0x1238;
    }

    if ((env_light->shadow_mode & 4) && param_0 != NULL) {
        *param_1 *= param_0->field_0x344;
    }

    if (env_light->mThunderEff.mState != 0 && env_light->mThunderEff.mState < 10) {
        *param_1 += env_light->mThunderEff.mFlashTimer;
    }

    if (g_env_light.eflight.mState == 2) {
        f32 fvar1 = g_env_light.eflight.mFrame / 15.0f;
        if (fvar1 > 1.0f) {
            fvar1 = 1.0f;
        }

        *param_1 += (1.0f - fvar1) * (0.65f - *param_1);
    }

    if (*param_1 > 1.0f) {
        *param_1 = 1.0f;
    }
}

u8 dDlst_shadowReal_c::setShadowRealMtx(cXyz* param_0, cXyz* param_1, f32 param_2, f32 param_3,
                                            f32 param_4, dKy_tevstr_c* param_5) {
    mShadowRealPoly.reset();
    setkankyoShadow(param_5, &param_4);
    int r29 = g_envHIO.mOther.mShadowDensity * param_4;
    cXyz local_64 = *param_0 - *param_1;
    u8 r28 = 0;
    if (param_5) {
        cLib_chaseUC(&param_5->field_0x385, param_3 > 50.0f ? 0 : 0xff, 60);
        r28 = param_5->field_0x385;
    }
    f32 f29 = local_64.abs2();
    if (!cM3d_IsZero(f29)) {
        f32 tmp = JMAFastSqrt(f29);
        if (local_64.y / tmp < 0.8f) {
            local_64.y = tmp * 0.8f;
            f32 tmp2 = local_64.abs2XZ();
            if (!cM3d_IsZero(tmp2)) {
                f32 tmp3 = JMAFastSqrt((f29 - local_64.y * local_64.y) / tmp2);
                local_64.x *= tmp3;
                local_64.z *= tmp3;
            }
        }
        if (r28 == 0) {
            local_64.x = 0.0f;
            local_64.z = 0.0f;
        } else if (r28 < 0xff) {
            f32 tmp4 = r28 / 255.0f;
            local_64.x *= tmp4;
            local_64.z *= tmp4;
        }
        f29 = local_64.abs();
        if (!cM3d_IsZero(f29)) {
            f29 = (param_2 * 0.5f) / f29;
        }
    }
    local_64 *= f29;
    local_64 += *param_1;
    param_2 = param_2 * 0.4f;
    cXyz local_70 = *param_1 - local_64;
    if (local_70.isZero()) {
        local_70.y = -1.0f;
        local_64.y = param_1->y + 1.0f;
    } else {
        local_70.normalize();
    }
    if (!realPolygonCheck(param_1, param_2, param_3, &local_70, &mShadowRealPoly)) {
        return 0;
    }
    cMtx_lookAt(mViewMtx, &local_64, param_1, 0);
    C_MTXOrtho(mRenderProjMtx, param_2, -param_2, -param_2, param_2, 1.0f, 10000.0f);
    C_MTXLightOrtho(mReceiverProjMtx, param_2, -param_2, -param_2, param_2, 0.5f, -0.5f, 0.5f, 0.5f);
    cMtx_concat(mReceiverProjMtx, mViewMtx, mReceiverProjMtx);
    return r29;
}


u32 dDlst_shadowReal_c::set(u32 i_key, J3DModel* i_model, cXyz* param_2, f32 param_3, f32 param_4,
                            dKy_tevstr_c* param_5, f32 i_cameraZ, f32 param_7) {
    dScnKy_env_light_c* env_light = dKy_getEnvlight();

    if (mModelNum == 0) {
        cXyz sp60;

        if (param_5 != NULL) {
            sp60 = param_5->mLightPosWorld;
        } else {
            sp60 = dKy_plight_near_pos();

            if (!(env_light->shadow_mode & 4) &&
                ((env_light->shadow_mode & 1) || (env_light->shadow_mode & 2)))
            {
                sp60 = g_env_light.field_0x10a0;
                param_5->mLightPosWorld = g_env_light.field_0x10a0;
            }
        }

        field_0x1 = setShadowRealMtx(&sp60, param_2, param_3, param_4, param_7, param_5);
        if (!field_0x1) {
            return 0;
        }

        mState = 1;
        mKey = i_key;
        mModelNum = 0;
        mCameraZ = i_cameraZ;
        mZsortPre = NULL;
        mZsortNext = NULL;
    }

    u8 last_model_num = mModelNum;
    mModelNum++;
    mpModels[last_model_num] = i_model;
    return mKey;
}

bool dDlst_shadowReal_c::add(J3DModel* i_model) {
    u8 cur_model_num = mModelNum;

    if (cur_model_num == 0 || i_model == NULL) {
        return false;
    }

    mModelNum++;
    mpModels[cur_model_num] = i_model;
    return true;
}

void dDlst_shadowSimple_c::draw() {
    static GXColor l_color = {0, 0, 0, 64};
    l_color.a = mAlpha;

    GXSetTevColor(GX_TEVREG0, l_color);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXLoadPosMtxImm(mVolumeMtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXCallDisplayList(l_frontMat, 0x40);
    GXCallDisplayList(l_shadowVolumeDL, 0x40);
    GXCallDisplayList(l_backSubMat, 0x20);
    GXCallDisplayList(l_shadowVolumeDL, 0x40);
    GXLoadPosMtxImm(mMtx, GX_PNMTX1);
    GXSetCurrentMtx(GX_PNMTX1);

    if (mpTexObj != NULL) {
        GXLoadTexObj(mpTexObj, GX_TEXMAP0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_A2, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

        if (GXGetTexObjWidth(mpTexObj) == GXGetTexObjHeight(mpTexObj)) {
            GXCallDisplayList(l_shadowSealTexDL, 0x40);
        } else {
            GXCallDisplayList(l_shadowSealTex2DL, 0x40);
        }
    }

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXCallDisplayList(l_shadowSealDL, 0x60);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_INDEX8);
    GXSetCurrentMtx(GX_PNMTX0);
    GXCallDisplayList(l_clearMat, 0x40);
    GXCallDisplayList(l_shadowVolumeDL, 0x40);
}

void dDlst_shadowSimple_c::set(cXyz* param_0, f32 param_1, f32 param_2, cXyz* param_3,
                                   s16 param_4, f32 param_5, _GXTexObj* param_6) {
    if (param_5 < 0.0f) {
        mAlpha = param_5 * -255.0f;
        param_5 = 1.0f;
    } else {
        float local_a4 = 1.0f - (param_0->y - param_1) * 0.0007f;
        if (local_a4 < 0.0f) {
            local_a4 = 0.0f;
        } else if (local_a4 > 1.0f) {
            local_a4 = 1.0f;
        }
        setkankyoShadow(NULL, &local_a4);
        local_a4 *= 1.4f;
        if (local_a4 > 1.0f) {
            local_a4 = 1.0f;
        }
        mAlpha = g_envHIO.mOther.mShadowDensity * local_a4;
    }
    f32 f30 = param_2 * 16.0f * (1.0f - param_3->y) + 2.0f;
    mDoMtx_stack_c::transS(param_0->x, param_1 + f30, param_0->z);
    mDoMtx_stack_c::YrotM(param_4);
    mDoMtx_stack_c::scaleM(param_2, f30 + f30 + 16.0f, param_2 * param_5);
    cMtx_concat(j3dSys.getViewMtx(), mDoMtx_stack_c::get(), mVolumeMtx);
    f32 f31 = JMAFastSqrt(1.0f - param_3->x * param_3->x);
    f32 f29;
    f32 f28;
    if (0.0f != f31) {
        f29 = param_3->y * f31;
        f28 = -param_3->z * f31;
    } else {
        f29 = 0.0f;
        f28 = 0.0f;
    }
    mDoMtx_stack_c::get()[0][0] = f31;
    mDoMtx_stack_c::get()[0][1] = param_3->x;
    mDoMtx_stack_c::get()[0][2] = 0.0f;
    mDoMtx_stack_c::get()[0][3] = param_0->x;
    mDoMtx_stack_c::get()[1][0] = -param_3->x * f29;
    mDoMtx_stack_c::get()[1][1] = param_3->y;
    mDoMtx_stack_c::get()[1][2] = f28;
    mDoMtx_stack_c::get()[1][3] = param_1;
    mDoMtx_stack_c::get()[2][0] = param_3->x * f28;
    mDoMtx_stack_c::get()[2][1] = param_3->z;
    mDoMtx_stack_c::get()[2][2] = f29;
    mDoMtx_stack_c::get()[2][3] = param_0->z;
    mDoMtx_stack_c::YrotM(param_4);
    mDoMtx_stack_c::scaleM(param_2, 1.0f, param_2 * param_5);
    cMtx_concat(j3dSys.getViewMtx(), mDoMtx_stack_c::get(), mMtx);
    mpTexObj = param_6;
}

void dDlst_shadowControl_c::init() {
    static u16 l_realImageSize[2] = {192, 64};
    for (int i = 0; i < 2; i++) {
        u16 size = l_realImageSize[i];

        u32 buffer_size = GXGetTexBufferSize(size, size, 5, GX_DISABLE, 0);
        field_0x15ef0[i] = new (0x20) u8[buffer_size];
        GXInitTexObj(&field_0x15eb0[i], field_0x15ef0[i], size, size, GX_TF_RGB5A3, GX_CLAMP,
                     GX_CLAMP, GX_DISABLE);
        GXInitTexObjLOD(&field_0x15eb0[i], GX_LINEAR, GX_LINEAR, 0.0f, 0.0f, 0.0f, GX_FALSE,
                        GX_FALSE, GX_ANISO_1);
    }
}


void dDlst_shadowControl_c::reset() {
    dDlst_shadowReal_c* shadowReal = mReal;

    for (int i = 0; i < 8; i++) {
        shadowReal->reset();
        shadowReal++;
    }

    mSimpleNum = 0;
    mRealNum = 0;
    field_0x4 = NULL;
}

void dDlst_shadowControl_c::imageDraw(Mtx param_0) {
    static u8 l_matDL[] ATTRIBUTE_ALIGN(32) = {
        0x10, 0x00, 0x00, 0x10, 0x0E, 0x00, 0x00, 0x04, 0x00, 0x10, 0x00, 0x00, 0x10, 0x10,
        0x00, 0x00, 0x04, 0x00, 0x61, 0x28, 0x38, 0x00, 0x00, 0x61, 0xC0, 0x08, 0xFF, 0xF2,
        0x61, 0xC1, 0x08, 0xFF, 0x90, 0x61, 0x43, 0x00, 0x00, 0x41, 0x61, 0xF3, 0x7F, 0x00,
        0x00, 0x10, 0x00, 0x00, 0x10, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x10,
        0x09, 0x00, 0x00, 0x00, 0x01, 0x61, 0x00, 0x00, 0x40, 0x10, 0x61, 0xEE, 0x00, 0x00,
        0x00, 0x61, 0xEF, 0x00, 0x00, 0x00, 0x61, 0xF0, 0x00, 0x00, 0x00, 0x61, 0xF1, 0x00,
        0x00, 0x00, 0x61, 0xF2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    static GXColor l_imageDrawColor[4] = {
        {255, 0, 0, 0},
        {0, 255, 0, 0},
        {0, 0, 255, 0},
        {0, 0, 0, 255},
    };


    GXCallDisplayList(l_matDL, 0x60);
    GXSetZMode(GX_DISABLE, GX_LEQUAL, GX_DISABLE);
    GXSetZCompLoc(GX_TRUE);
    GXSetBlendMode(GX_BM_LOGIC, GX_BL_ONE, GX_BL_ONE, GX_LO_OR);
    GXSetClipMode(GX_CLIP_DISABLE);
    j3dSys.setDrawModeOpaTexEdge();
    J3DShape::resetVcdVatCache();
    dDlst_shadowReal_c* shadowReal = field_0x4;
    int r29 = 0;
    int tex = 0;
    u16 r27;
    u16 r26;
    for (; shadowReal; shadowReal = shadowReal->getZsortNext()) {
        if (shadowReal->isUse()) {
            if (r29 == 0) {
                r27 = GXGetTexObjWidth(field_0x15eb0 + tex);
                r26 = r27 * 2;
                GXSetViewport(0.0f, 0.0f, r26, r26, 0.0f, 1.0f);
                GXSetScissor(0, 0, r26, r26);
            }
            GXSetTevColor(GX_TEVREG0, l_imageDrawColor[r29]);
            if (r29 == 3) {
                GXSetColorUpdate(GX_DISABLE);
                GXSetAlphaUpdate(GX_ENABLE);
            }
            shadowReal->imageDraw(param_0);
            r29 = (r29 + 1) % 4;
            if (r29 == 0) {
                GXSetTexCopySrc(0, 0, r26, r26);
                GXSetTexCopyDst(r27, r27, GX_TF_RGB5A3, GX_TRUE);
                GXSetColorUpdate(GX_ENABLE);
                GXCopyTex(field_0x15ef0[tex++], GX_TRUE);
                GXPixModeSync();
                GXSetAlphaUpdate(GX_DISABLE);
            }
        }
    }
    if (r29) {
        GXSetTexCopySrc(0, 0, r26, r26);
        GXSetTexCopyDst(r27, r27, GX_TF_RGB5A3, GX_TRUE);
        GXCopyTex(field_0x15ef0[tex], GX_TRUE);
        GXPixModeSync();
        GXSetAlphaUpdate(GX_DISABLE);
    }
    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetDither(GX_TRUE);
}

void dDlst_shadowControl_c::draw(Mtx param_0) {
    static GXTevColorChan l_tevColorChan[4] = {
        GX_CH_RED,
        GX_CH_GREEN,
        GX_CH_BLUE,
        GX_CH_ALPHA,
    };

    j3dSys.reinitGX();
    GXSetNumIndStages(0);
    dKy_GxFog_set();

    GXSetChanCtrl(GX_ALPHA0, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetArray(GX_VA_POS, l_shadowVolPos, 12);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_POS, GX_TEXMTX0);
    GXSetNumTevStages(1);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXLoadPosMtxImm(param_0, GX_PNMTX0);
    GXColor matColor = {0, 0, 0, 0x20};
    GXSetChanMatColor(GX_ALPHA0, matColor);

    static GXColor clearColor = {0x00, 0x00, 0x00, 0x40};
    clearColor.a = mDoGph_gInf_c::getBackColor().a;
    GXSetTevColor(GX_TEVREG1, clearColor);
    GXSetTevColor(GX_TEVREG2, g_whiteColor);

    dDlst_shadowReal_c* real = field_0x4;
    for (int i2 = 0, i3 = 0; real != NULL; real = real->getZsortNext()) {
        if (real->isUse()) {
            if (i2 == 0) {
                GXTexObj* obj = &field_0x15eb0[i3];
                i3++;

                GXLoadTexObj(obj, GX_TEXMAP0);
            }

            GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE,
                                  l_tevColorChan[i2]);
            real->draw();
            i2 = (i2 + 1) % 4;
        }
    }

    GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGB8, 0);
    GXSetArray(GX_VA_POS, l_simpleShadowPos, 12);
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);

    dDlst_shadowSimple_c* simple = mSimple;
    for (int i = 0; i < mSimpleNum; i++) {
        simple->draw();
        simple++;
    }

    GXSetColorUpdate(GX_ENABLE);
    GXSetAlphaUpdate(GX_DISABLE);
}

int dDlst_shadowControl_c::setReal(u32 param_1, s8 param_2, J3DModel* param_3, cXyz* param_4,
                                       f32 param_5, f32 param_6, dKy_tevstr_c* param_7) {
    static Mtx mtx_adj = {
        {0.5f, 0.0f, 0.0f, 0.5f},
        {0.0f, -0.5f, 0.0f, 0.5f},
        {0.0f, 0.0f, 1.0f, 0.0f},
    };

    f32 fVar1;
    f32 dVar16 = (param_2 == 0) ? 1.0f : (1.0f - 0.003000000026077032f * param_6);
    if (dVar16 <= 0.0f) {
        return 0;
    }
    if (dVar16 > 1.0f) {
        dVar16 = 1.0f;
    }
    cXyz acStack_94;
    cMtx_multVec(j3dSys.getViewMtx(), param_4, &acStack_94);
    if ((acStack_94.z - param_5) >= 0.0f) {
        return 0;
    }
    f32 dVar17 = acStack_94.z + param_5;
    if (dVar17 < -1000.0f) {
        f32 fVar1 = 0.0010000000474974513f *
                (-1000.0f - dVar17);
        if (fVar1 >= 1.0f) {
            return 0;
        }
        dVar16 = (dVar16 * (1.0f - fVar1));
    }
    if (param_2 == 0) {
        dVar17 = FLT_MAX;
    }
    dDlst_shadowReal_c* pdVar11;
    dDlst_shadowReal_c* pdVar10 = NULL;
    dDlst_shadowReal_c* local_98 = field_0x4;
    pdVar11 = NULL;
    while (local_98 != NULL) {
        pdVar11 = local_98;
        if (dVar17 > local_98->getCameraZ()) {
            pdVar10 = local_98;
            local_98 = NULL;
        } else {
            local_98 = local_98->getZsortNext();
        }
    }
    dDlst_shadowReal_c* pdVar12;
    if (mRealNum >= 8) {
        if (pdVar10 == NULL) {
            return 0;
        }
        pdVar12 = field_0x8;
        field_0x8 = pdVar12->getZsortPre();
        field_0x8->setZsortNext(NULL);
        if (pdVar12 == pdVar10) {
            pdVar10 = NULL;
            pdVar11 = field_0x8;
        }
        pdVar12->reset();
        mRealNum--;
    } else {
        pdVar12 = mReal;
        for (int i = 0; i < 8; i++, pdVar12++) {
            if (pdVar12->isNoUse()) {
                break;
            }
        }
    }

    do {
    } while (++mNextID == 0);
    u32 rv = pdVar12->set(mNextID, param_3, param_4, param_5, param_6, param_7, dVar17, dVar16);
    if (!rv) {
        return 0;
    } 
    mRealNum++;
    if (pdVar10 == NULL) {
        if (pdVar11 == NULL) {
            field_0x4 = pdVar12;
        } else {
            pdVar11->setZsortNext(pdVar12);
            pdVar12->setZsortPre(pdVar11);
        }
        field_0x8 = pdVar12;
    } else {
        pdVar11 = pdVar10->getZsortPre();
        if (pdVar11 == NULL) {
            field_0x4 = pdVar12;
        } else {
            pdVar11->setZsortNext(pdVar12);
            pdVar12->setZsortPre(pdVar11);
        }
        pdVar12->setZsortNext(pdVar10);
        pdVar10->setZsortPre(pdVar12);
    }
    return rv;
}

bool dDlst_shadowControl_c::addReal(u32 i_key, J3DModel* param_1) {
    if (i_key == 0) {
        return false;
    }

    for (dDlst_shadowReal_c* i = field_0x4; i != NULL; i = i->getZsortNext()) {
        if (!i->isNoUse() && i->checkKey(i_key)) {
            return i->add(param_1);
        }
    }

    return false;
}

int dDlst_shadowControl_c::setSimple(cXyz* param_0, f32 param_1, f32 param_2, cXyz* param_3,
                                     s16 param_4, f32 param_5, GXTexObj* param_6) {
    if (param_3 == NULL || mSimpleNum >= 128) {
        return 0;
    }

    mSimple[mSimpleNum].set(param_0, param_1, param_2, param_3, param_4, param_5, param_6);
    mSimpleNum++;
    return 1;
}

GXTexObj dDlst_shadowControl_c::mSimpleTexObj;

void dDlst_shadowControl_c::setSimpleTex(ResTIMG const* i_timg) {
    mDoLib_setResTimgObj(i_timg, &mSimpleTexObj, 0, NULL);
}

int dDlst_peekZ_c::newData(s16 x, s16 y, u32* dst) {
    if (mCount >= 64 || x < 0 || x > 607 || y < 0 || y > 447) {
        return 0;
    }

    dDlst_peekZ_entry* entry = &mEntries[mCount];
    entry->x = x;
    entry->y = y;
    entry->dst = dst;
    mCount++;
    return 1;
}

void dDlst_peekZ_c::peekData() {
    dDlst_peekZ_entry* entry = mEntries;

    for (int i = 0; i < mCount; i++) {
        GXPeekZ(entry->x, entry->y, entry->dst);
        entry++;
    }

    mCount = 0;
}

dDlst_list_c::dDlst_list_c() {
    mpCopy2DEnd = &mpCopy2DDrawLists[4];
    mp2DOpaTopEnd = &mp2DOpaTopDrawLists[16];
    mp2DOpaEnd = &mp2DOpaDrawLists[64];
    mp2DXluEnd = &mp2DXluDrawLists[32];

    J3DDrawBuffer** buffer = mDrawBuffers;
    for (int i = 0; i < 21; i++) {
        *buffer = NULL;
        buffer++;
    }
}

dDlst_shadowSimple_c::dDlst_shadowSimple_c() {}

static u16 const l_drawlistSize[21] = {
    0x0001, 0x0001, 0x0001, 0x0080, 0x0001, 0x0001, 0x0001, 0x0080, 0x0100, 0x0001, 0x0100,
    0x0001, 0x0020, 0x0010, 0x0010, 0x0020, 0x0020, 0x0100, 0x0001, 0x0004, 0x0004,
};

static u8 const l_nonSortId[9] = {
    0x00, 0x01, 0x02, 0x04, 0x05, 0x06, 0x09, 0x12, 0x0D,
};

static const u8 l_zSortId[6] = {
    0x08, 0x0A, 0x0C, 0x0E, 0x10, 0x11,
};

void dDlst_list_c::init() {
    J3DDrawBuffer** var_r30 = mDrawBuffers;
    const u16* var_r29 = l_drawlistSize;

    for (int i = 0; i < 21; i++) {
        *var_r30 = J3DDrawBuffer__create(*var_r29++);
        var_r30++;
    }

    const u8* var_r5 = l_nonSortId;
    for (int i = 0; i < 9; i++) {
        mDrawBuffers[*var_r5++]->setNonSort();
    }

    const u8* var_r5_2 = l_zSortId;
    for (int i = 0; i < 6; i++) {
        mDrawBuffers[*var_r5_2++]->setZSort();
    }

    setOpaList();
    setXluList();
    mpCopy2DStart = mpCopy2DDrawLists;
    mp2DOpaTopStart = mp2DOpaTopDrawLists;
    mp2DOpaStart = mp2DOpaDrawLists;
    mp2DXluStart = mp2DXluDrawLists;

    mShadowControl.init();
}

dDlst_list_c::~dDlst_list_c() {
    J3DDrawBuffer** buffer = mDrawBuffers;
    for (int i = 0; i < 21; i++) {
        J3DDrawBuffer* tmp = *buffer;
        buffer++;

        delete tmp;
    }
}

void dDlst_list_c::reset() {
    J3DDrawBuffer** buffer = mDrawBuffers;
    for (int i = 0; i < 21; i++) {
        J3DDrawBuffer* tmp = *buffer;
        buffer++;

        tmp->frameInit();
    }

    setOpaList();
    setXluList();

    mpCopy2DStart = mpCopy2DDrawLists;
    mp2DOpaTopStart = mp2DOpaTopDrawLists;
    mp2DOpaStart = mp2DOpaDrawLists;
    mp2DXluStart = mp2DXluDrawLists;
    mShadowControl.reset();

    for (int i = 0; i < 3; i++) {
        m3DLineMatSortPacket[i].reset();
    }
}

void dDlst_list_c::entryZSortXluDrawList(J3DDrawBuffer* param_0, J3DPacket* param_1,
                                         cXyz& param_2) {
    f32 z_value = -J3DCalcZValue(j3dSys.getViewMtx(), param_2);
    u32 size = param_0->getEntryTableSize();
    f32 fsize = size;
    fsize = 9999.0f / fsize;

    u16 var_r0;
    if (fsize + 1.0f < z_value) {
        if (10000.0f - fsize > z_value) {
            var_r0 = z_value / fsize;
        } else {
            var_r0 = size - 1;
        }
    } else {
        var_r0 = 0;
    }

    var_r0 = (size - 1) - var_r0;
    param_0->entryImm(param_1, var_r0);
}


void dDlst_list_c::drawOpaDrawList(J3DDrawBuffer* pDrawBuf) {
    J3DShape::resetVcdVatCache();
    j3dSys.setDrawModeOpaTexEdge();
    pDrawBuf->draw();
}

void dDlst_list_c::drawXluDrawList(J3DDrawBuffer* pDrawBuf) {
    J3DShape::resetVcdVatCache();
    j3dSys.setDrawModeXlu();
    pDrawBuf->draw();
}

void dDlst_list_c::drawOpaListItem3d() {
    drawOpaDrawList(mDrawBuffers[DB_OPA_LIST_ITEM3D]);
}

void dDlst_list_c::drawXluListItem3d() {
    drawXluDrawList(mDrawBuffers[DB_XLU_LIST_ITEM3D]);
}

int dDlst_list_c::set(dDlst_base_c**& p_start, dDlst_base_c**& p_end, dDlst_base_c* p_newDlst) {
    if (p_start >= p_end) {
        return 0;
    }
    *p_start = p_newDlst;
    *p_start++;
    return 1;
}

void dDlst_list_c::draw(dDlst_base_c** p_start, dDlst_base_c** p_end) {
    for (; p_start < p_end; p_start++) {
        dDlst_base_c* dlst = *p_start;
        dlst->draw();
    }
}

dDlst_2DT2_c dDlst_list_c::mWipeDlst;

GXColor dDlst_list_c::mWipeColor = {0, 0, 0, 0};

u8 dDlst_list_c::mWipe;

f32 dDlst_list_c::mWipeRate;

f32 dDlst_list_c::mWipeSpeed;

void dDlst_list_c::wipeIn(f32 i_wipeSpeed, GXColor& i_wipeColor) {
    mWipe = true;
    mWipeSpeed = i_wipeSpeed;
    mWipeColor = i_wipeColor;
    mWipeRate = i_wipeSpeed >= 0.0f ? 0.0f : 1.0f;

    JKRArchive* main2d = dComIfGp_getMain2DArchive();
    ResTIMG* texture = (ResTIMG*)JKRArchive::getGlbResource('TIMG', "wipe_00.bti", main2d);

    mWipeDlst.init(texture, mDoGph_gInf_c::getMinX(), mDoGph_gInf_c::getMinY(),
                   mDoGph_gInf_c::getWidth(), mDoGph_gInf_c::getHeight(), 0, 1, 1, 2.0f, 2.436f);
}


void dDlst_list_c::wipeIn(f32 i_wipeSpeed) {
    wipeIn(-i_wipeSpeed, g_blackColor);
}

void dDlst_list_c::calcWipe() {
    if (mWipe) {
        mWipeRate += mWipeSpeed;
        if (mWipeRate < 0.0f) {
            mWipeRate = 0.0f;
        } else if (mWipeRate > 1.0f) {
            mWipeRate = 1.0f;
            mWipe = false;
        }
        mWipeDlst.setScaleX(mWipeRate * 2.0f);
        mWipeDlst.setScaleY(mWipeDlst.getScaleX() * 1.218f);
        dComIfGd_set2DXlu(&mWipeDlst);
    }
}
