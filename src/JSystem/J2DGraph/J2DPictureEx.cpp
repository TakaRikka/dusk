#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J2DGraph/J2DPictureEx.h"
#include "JSystem/J2DGraph/J2DMaterial.h"
#include "JSystem/J2DGraph/J2DScreen.h"
#include "JSystem/JSupport/JSURandomInputStream.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "dolphin/types.h"

void J2DPictureEx::initiate(ResTIMG const* param_0, ResTLUT const* param_1) {
    J2DTexGenBlock* this_00 = mMaterial->getTexGenBlock();
    if (this_00->getTexGenNum() == 0 && append(param_0, 1.0f) && param_1 != NULL) {
        if (mMaterial->getTevBlock()->getPalette(0) == NULL) {
            mMaterial->getTevBlock()->setPalette(0, param_1);
        }
    }
}

J2DPictureEx::J2DPictureEx(J2DPane* param_0, JSURandomInputStream* param_1, u32 param_2,
                               J2DMaterial* param_3) {
    field_0x194 = 0;
    field_0x198 = 0;
    s32 origPosition = param_1->getPosition();
    u32 local_68[2];
    param_1->read(local_68, 8);
    mKind = local_68[0];
    s32 iVar2 = param_1->getPosition();
    u32 auStack_70[2];
    param_1->peek(auStack_70, 8);
    makePaneExStream(param_0, param_1);
    param_1->seek(iVar2 + auStack_70[1], JSUStreamSeekFrom_SET);
    J2DScrnBlockPictureParameter aJStack_60;
    param_1->read(&aJStack_60, sizeof(J2DScrnBlockPictureParameter));
    field_0x154 = aJStack_60.mMaterialNum;
    field_0x156 = aJStack_60.field_0x4;

    for (int i = 0; i < 4; i++) {
        field_0x158[i] = aJStack_60.field_0x8[i];
        field_0x10a[i] = aJStack_60.field_0x10[i];
        mCornerColor[i] = JUtility::TColor(aJStack_60.mCornerColor[i]);
    }

    param_1->seek(origPosition + local_68[1], JSUStreamSeekFrom_SET);
    mMaterial = NULL;
    if (field_0x156 != 0xffff) {
        mMaterial = param_3 + field_0x156;
        (param_3 + field_0x156)->field_0x4 = this;
    }
    field_0x190 = 0;
    mAlpha = 0xff;
    rewriteAlpha();
    for (int iVar1 = 0; iVar1 < 2; iVar1++) {
        field_0x11c[iVar1] = 1.0f;
        field_0x124[iVar1] = 1.0f;
    }
    for (int iVar1 = 0; iVar1 < 6; iVar1++) {
        field_0x160[iVar1] = 1.0f;
        field_0x178[iVar1] = 1.0f;
    }
}

J2DPictureEx::~J2DPictureEx() {
    if (field_0x190) {
        delete mMaterial;
    }
}

bool J2DPictureEx::prepareTexture(u8 param_0) {
    if (!mMaterial->getTevBlock()->prepareTexture(param_0)) {
        return false;
    } 

    for (u8 i = 0; i < param_0; i++) {
        J2DTexMtx aJStack_68;
        mMaterial->getTexGenBlock()->setTexMtx(i, aJStack_68);
    }
    return true;
}

void J2DPictureEx::drawSelf(f32 param_0, f32 param_1, f32 (*param_2)[3][4]) {
    if (mMaterial != NULL) {
        mMaterial->setGX();
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        drawFullSet(param_0, param_1, getWidth(), getHeight(), param_2);
    }
}

void J2DPictureEx::drawFullSet(f32 param_0, f32 param_1, f32 param_2, f32 param_3,
                                   f32 (*param_4)[3][4]) {
    drawTexCoord(mBounds.i.x, mBounds.i.y, param_2, param_3, field_0x10a[0].x, field_0x10a[0].y,
                 field_0x10a[1].x, field_0x10a[1].y, field_0x10a[2].x, field_0x10a[2].y,
                 field_0x10a[3].x, field_0x10a[3].y, param_4);
}

void J2DPictureEx::drawTexCoord(f32 param_0, f32 param_1, f32 param_2, f32 param_3, s16 param_4,
                                    s16 param_5, s16 param_6, s16 param_7, s16 param_8, s16 param_9,
                                    s16 param_10, s16 param_11, f32 (*param_12)[3][4]) {
    f32 dVar12 = param_0 + param_2;
    f32 dVar11 = param_1 + param_3;
    Mtx auStack_88;
    MTXConcat(*param_12, mGlobalMtx, auStack_88);
    if (mMaterial == NULL || mMaterial->isVisible()) {
        GXLoadPosMtxImm(auStack_88, 0);
        JUtility::TColor TStack_8c = mCornerColor[0];
        JUtility::TColor TStack_90 = mCornerColor[1];
        JUtility::TColor TStack_94 = mCornerColor[2];
        JUtility::TColor TStack_98 = mCornerColor[3];
        if (mMaterial != NULL) {
            if (mMaterial->getColorBlock()->getColorChan(1)->getMatSrc() == 1) {
                if (mMaterial->getMaterialAlphaCalc() == 1) {
                    TStack_8c.a = TStack_8c.a * mColorAlpha / 255;
                    TStack_90.a = TStack_90.a * mColorAlpha / 255;
                    TStack_94.a = TStack_94.a * mColorAlpha / 255;
                    TStack_98.a = TStack_98.a * mColorAlpha / 255;
                }
            } else if (mIsInfluencedAlpha) {
                GXSetChanMatColor(GX_ALPHA0, JUtility::TColor(mColorAlpha));
            }
        }
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBA4, 8);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(param_0, param_1, 0.0f);
        GXColor1u32(TStack_8c);
        GXTexCoord2s16(param_4, param_5);
        GXPosition3f32(dVar12, param_1, 0.0f);
        GXColor1u32(TStack_90);
        GXTexCoord2s16(param_6, param_7);
        GXPosition3f32(dVar12, dVar11, 0.0f);
        GXColor1u32(TStack_98);
        GXTexCoord2s16(param_10, param_11);
        GXPosition3f32(param_0, dVar11, 0.0f);
        GXColor1u32(TStack_94);
        GXTexCoord2s16(param_8, param_9);
        GXEnd();
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBX8, 0xf);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGBA4, 0);
    }
}


bool J2DPictureEx::append(ResTIMG const* param_0, JUTPalette* param_1, f32 param_2) {
    if (mMaterial == NULL) {
        return false;
    }

    return insert(param_0, param_1, mMaterial->getTexGenBlock()->getTexGenNum(), param_2);
}

bool J2DPictureEx::append(char const* param_0, JUTPalette* param_1, f32 param_2) {
    if (mMaterial == NULL) {
        return false;
    }

    return insert(param_0, param_1, mMaterial->getTexGenBlock()->getTexGenNum(), param_2);
}

bool J2DPictureEx::append(JUTTexture* param_0, f32 param_1) {
    if (mMaterial == NULL) {
        return false;
    }

    return insert(param_0,  mMaterial->getTexGenBlock()->getTexGenNum(), param_1);
}

bool J2DPictureEx::insert(ResTIMG const* param_0, JUTPalette* param_1, u8 param_2,
                              f32 param_3) {
    if (param_0 == NULL) {
        return false;
    }

    if (!isInsert(param_2)) {
        return false;
    }

    insertCommon(param_2, param_3);
    mMaterial->getTevBlock()->insertTexture(param_2, param_0, param_1);
    return true;
}

bool J2DPictureEx::insert(char const* param_0, JUTPalette* param_1, u8 param_2, f32 param_3) {
    ResTIMG const* resource = (ResTIMG const*)J2DScreen::getNameResource(param_0);
    return insert(resource, param_1, param_2, param_3);
}

bool J2DPictureEx::insert(JUTTexture* param_0, u8 param_1, f32 param_2) {
    if (param_0 == NULL) {
        return false;
    }

    if (!isInsert(param_1)) {
        return false;
    }

    insertCommon(param_1, param_2);
    mMaterial->getTevBlock()->insertTexture(param_1, param_0);
    return true;
}

void J2DPictureEx::insertCommon(u8 pos, f32 param_1) {
    u8 new_tev_stage_num;
    u8 tex_gen_num = mMaterial->getTexGenBlock()->getTexGenNum();
    mMaterial->getTevBlock()->getMaxStage();
    int tev_stage_num = mMaterial->getTevBlock()->getTevStageNum();
    bool bVar1;
    if (tex_gen_num <= 1) {
        bVar1 = tev_stage_num != 1;
    } else {
        bVar1 = tev_stage_num != tex_gen_num + 1;
    }

    shiftSetBlendRatio(pos, param_1, true, true);
    shiftSetBlendRatio(pos, param_1, false, true);

    tex_gen_num++;
    mMaterial->getTexGenBlock()->setTexGenNum(tex_gen_num);

    J2DTexCoordInfo tex_coord_info;
    tex_coord_info.mTexGenType = GX_TG_MTX2x4;
    tex_coord_info.mTexGenSrc = GX_TG_TEX0;
    for (int i = tex_gen_num - 1; i >= pos; i--) {
        tex_coord_info.mTexGenMtx = i * 3 + 30;
        J2DTexCoord tex_coord(tex_coord_info);
        mMaterial->getTexGenBlock()->setTexCoord((u8)i, tex_coord);
    }

    for (int i = tex_gen_num - 1; i > pos; i--) {
        mMaterial->getTexGenBlock()->setTexMtx((u8)i, mMaterial->getTexGenBlock()->getTexMtx((u8)(i - 1)));
    }

    J2DTexMtx tex_mtx;
    mMaterial->getTexGenBlock()->setTexMtx(pos, tex_mtx);

    if (tex_gen_num == 1) {
        new_tev_stage_num = 1;
    } else {
        new_tev_stage_num = tex_gen_num + (bVar1 ? 2 : 1);
    }
    mMaterial->getTevBlock()->setTevStageNum(new_tev_stage_num);

    setTevOrder(tex_gen_num, new_tev_stage_num, bVar1);
    setTevStage(tex_gen_num, new_tev_stage_num, bVar1);
    setTevKColor(tex_gen_num);
    setTevKColorSel(tex_gen_num);
    setTevKAlphaSel(tex_gen_num);
}

bool J2DPictureEx::isInsert(u8 pos) const {
    if (mMaterial == NULL) {
        return false;
    } 
    if (mMaterial->getTevBlock() == NULL) {
        return false;
    } 

    u8 texGenNum = mMaterial->getTexGenBlock()->getTexGenNum();
    if (texGenNum >= 8 || pos >= 8 || pos > texGenNum) {
        return false;
    }
    u8 maxStage = mMaterial->getTevBlock()->getMaxStage();
    if (maxStage <= 2 && texGenNum != 0) {
        return false;
    } 

    if (maxStage == mMaterial->getTevBlock()->getTevStageNum() && texGenNum != 0) {
        return false;
    }
    
    return true;
}

bool J2DPictureEx::remove(u8 pos) {
    if (!isRemove(pos)) {
        return false;
    }

    u8 tex_gen_num = mMaterial->getTexGenBlock()->getTexGenNum();
    u8 tev_stage_num = mMaterial->getTevBlock()->getTevStageNum();
    bool bVar1 = tev_stage_num == tex_gen_num + 1 ? false : true;

    shiftSetBlendRatio(pos, 0.0f, true, false);
    shiftSetBlendRatio(pos, 0.0f, false, false);

    tex_gen_num--;
    mMaterial->getTexGenBlock()->setTexGenNum(tex_gen_num);
    mMaterial->getTevBlock()->removeTexture(pos);

    if (tex_gen_num != 1) {
        tev_stage_num = tex_gen_num + (bVar1 ? 2 : 1);
    } else {
        tev_stage_num = bVar1 ? 2 : 1;
    }
    mMaterial->getTevBlock()->setTevStageNum(tev_stage_num);

    setTevOrder(tex_gen_num, tev_stage_num, bVar1);
    setTevStage(tex_gen_num, tev_stage_num, bVar1);
    setTevKColor(tex_gen_num);
    setTevKColorSel(tex_gen_num);
    setTevKAlphaSel(tex_gen_num);
    return true;
}

bool J2DPictureEx::remove() {
    if (mMaterial == NULL) {
        return false;
    }

    if (mMaterial->getTevBlock() == NULL) {
        return false;
    }

    u8 texGenNum = mMaterial->getTexGenBlock()->getTexGenNum();
    return remove(texGenNum - 1);
}

bool J2DPictureEx::remove(JUTTexture* param_0) {
    if (mMaterial == NULL) {
        return false;
    }

    if (mMaterial->getTevBlock() == NULL) {
        return false;
    }
    
    u8 texGenNum = mMaterial->getTexGenBlock()->getTexGenNum();
    u8 i;
    for (i = 0; i < texGenNum; i++) {
        if (mMaterial->getTevBlock()->getTexture(i) == param_0)
            break;
    }
    return remove(i);
}

bool J2DPictureEx::isRemove(u8 param_0) const {
    if (mMaterial == NULL) {
        return false;
    }

    if (mMaterial->getTevBlock() == NULL) {
        return false;
    }

    u8 texGenNum = mMaterial->getTexGenBlock()->getTexGenNum();
    if (texGenNum <= param_0 || texGenNum == 1) {
        return false;
    }
    return true;
}

void J2DPictureEx::draw(f32 param_0, f32 param_1, u8 texNo, bool param_3, bool param_4,
                        bool param_5) {
    if (mMaterial == NULL) {
        return;
    }

    if (mMaterial->getTevBlock() == NULL) {
        return;
    }
    
    if (!isVisible()) {
        return;
    }

    if (texNo >= mMaterial->getTexGenBlock()->getTexGenNum()) {
        return;
    }

    JUTTexture* texture = mMaterial->getTevBlock()->getTexture(texNo);
    if (texture != NULL) {
        draw(param_0, param_1, texture->getWidth(), texture->getHeight(),
                param_3, param_4, param_5);
    }
}

void J2DPictureEx::draw(f32 param_0, f32 param_1, f32 width, f32 height, bool param_4,
                        bool param_5, bool param_6) {
    if (!isVisible()) {
        return;
    }
    
    if (mMaterial == NULL) {
        return;
    }
    
    if (mMaterial->getTevBlock() == NULL) {
        return;
    }

    if (mMaterial->getTexGenBlock()->getTexGenNum() == 0) {
        return;
    }

    mMaterial->setGX();
    makeMatrix(param_0, param_1, 0.0f, 0.0f);
    GXLoadPosMtxImm(mPositionMtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    if (!mMaterial->isVisible()) {
        return;
    }

    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    
    mColorAlpha = mAlpha;
    
    JUtility::TColor corner_color[4];
    for (int i = 0; i < 4; i++) {
        corner_color[i] = mCornerColor[i];
    }

    if (mMaterial->getMaterialAlphaCalc() == 1) {
        for (int i = 0; i < 4; i++) {
            corner_color[i].a = (mColorAlpha * corner_color[i].a) / 0xff;
        }
    }

    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    GXPosition3f32(0.0f, 0.0f, 0.0f);
    GXColor1u32(corner_color[0]);
    if (!param_6) {
        GXTexCoord2u16(param_4 ? 0x8000 : 0, param_5 ? 0x8000 : 0);
    } else {
        GXTexCoord2u16(param_4 ? 0x8000 : 0, param_5 ? 0 : 0x8000);
    }

    GXPosition3f32(width, 0.0f, 0.0f);
    GXColor1u32(corner_color[1]);
    if (!param_6) {
        GXTexCoord2u16(param_4 ? 0 : 0x8000, param_5 ? 0x8000 : 0);
    } else {
        GXTexCoord2u16(param_4 ? 0x8000 : 0, param_5 ? 0x8000 : 0);
    }

    GXPosition3f32(width, height, 0.0f);
    GXColor1u32(corner_color[3]);
    if (!param_6) {
        GXTexCoord2u16(param_4 ? 0 : 0x8000, param_5 ? 0 : 0x8000);
    } else {
        GXTexCoord2u16(param_4 ? 0 : 0x8000, param_5 ? 0x8000 : 0);
    }

    GXPosition3f32(0.0f, height, 0.0f);
    GXColor1u32(corner_color[2]);
        if (!param_6) {
        GXTexCoord2u16(param_4 ? 0x8000 : 0, param_5 ? 0 : 0x8000);
    } else {
        GXTexCoord2u16(param_4 ? 0 : 0x8000, param_5 ? 0 : 0x8000);
    }

    GXEnd();

    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    for (int i = 0; i < 4; i++) {
        GXSetTevSwapModeTable((GXTevSwapSel)i, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    }
    GXSetNumIndStages(0);
    for (int i = 0; i < 16; i++) {
        GXSetTevDirect((GXTevStageID)i);
    }
    Mtx mtx;
    MTXIdentity(mtx);
    GXLoadPosMtxImm(mtx, GX_PNMTX0);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
}

void J2DPictureEx::drawOut(f32 param_0, f32 param_1, f32 param_2, f32 param_3, f32 param_4,
                           f32 param_5) {
    if (mMaterial == NULL) {
        return;
    }

    if (mMaterial->getTevBlock() == NULL) {
        return;
    }

    if (!isVisible()) {
        return;
    }

    JUTTexture* texture = mMaterial->getTevBlock()->getTexture(0);
    if (texture != NULL) {
        drawOut(JGeometry::TBox2<f32>(param_0, param_1, param_0 + param_2, param_1 + param_3),
                JGeometry::TBox2<f32>(param_4, param_5, param_4 + texture->getWidth(),
                                      param_5 + texture->getHeight()));
    }
}

void J2DPictureEx::drawOut(JGeometry::TBox2<f32> const& param_0,
                           JGeometry::TBox2<f32> const& param_1) {
    if (mMaterial == NULL) {
        return;
    }

    if (mMaterial->getTevBlock() == NULL) {
        return;
    }

    if (!isVisible()) {
        return;
    }

    mMaterial->setGX();
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    if (!mMaterial->isVisible()) {
        return;
    }

    mColorAlpha = mAlpha;
    JUtility::TColor corner_color[4];
    for (int i = 0; i < 4; i++) {
        corner_color[i] = mCornerColor[i];
    }
    if (mMaterial->getMaterialAlphaCalc() == 1) {
        for (int i = 0; i < 4; i++) {
            corner_color[i].a = (mColorAlpha * corner_color[i].a) / 0xff;
        }
    }

    f32 x1 = (param_0.i.x - param_1.i.x) / param_1.getWidth();
    f32 x2 = (param_0.f.x - param_1.f.x) / param_1.getWidth() + 1.0f;
    f32 y1 = (param_0.i.y - param_1.i.y) / param_1.getHeight();
    f32 y2 = (param_0.f.y - param_1.f.y) / param_1.getHeight() + 1.0f;
    
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);

    GXPosition3f32(param_0.i.x, param_0.i.y, 0.0f);
    GXColor1u32(corner_color[0]);
    GXTexCoord2f32(x1, y1);
    GXPosition3f32(param_0.f.x, param_0.i.y, 0.0f);
    GXColor1u32(corner_color[1]);
    GXTexCoord2f32(x2, y1);
    GXPosition3f32(param_0.f.x, param_0.f.y, 0.0f);
    GXColor1u32(corner_color[3]);
    GXTexCoord2f32(x2, y2);
    GXPosition3f32(param_0.i.x, param_0.f.y, 0.0f);
    GXColor1u32(corner_color[2]);
    GXTexCoord2f32(x1, y2);

    GXEnd();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGBA4, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBX8, 15);
    GXSetNumTexGens(0);
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    for (int i = 0; i < 4; i++) {
        GXSetTevSwapModeTable((GXTevSwapSel)i, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    }
    GXSetNumIndStages(0);
    for (int i = 0; i < 16; i++) {
        GXSetTevDirect((GXTevStageID)i);
    }
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
}

void J2DPictureEx::load(_GXTexMapID param_0, u8 param_1) {
    if (mMaterial == NULL) {
        return;
    }

    if (mMaterial->getTevBlock() == NULL) {
        return;
    }

    mMaterial->getTevBlock()->loadTexture(param_0, param_1);
}

void J2DPictureEx::setTevOrder(u8 param_0, u8 param_1, bool param_2) {
    u16 local_30[16];
    if (param_0 == 1) {
        if (!param_2) {
            local_30[0] = 4;
        } else {
            local_30[0] = 0xff;
            local_30[1] = 0xff04;
        }
    } else {
        for (u8 i = 0; i < param_0; i++) {
            local_30[i] = (i << 8) | 0xff;
        }
        if (!param_2) {
            local_30[param_0] = 0xff04;
        } else {
            local_30[param_0] = 0xffff;
            local_30[param_0 + 1] = 0xff04;
        }
    }

    for (u8 i = 0; i < param_1; i++) {
        J2DTevOrderInfo info;
        info.mTexMap = info.mTexCoord = local_30[i] >> 8;
        info.mColor = local_30[i] & 0xff;
        J2DTevOrder order(info);
        mMaterial->getTevBlock()->setTevOrder(i, order);
    }
}

void J2DPictureEx::setTevStage(u8 param_0, u8 param_1, bool param_2) {
    if (param_0 == 1) {
        J2DTevStage* stage = mMaterial->getTevBlock()->getTevStage(0);
        JUTTexture* texture = mMaterial->getTevBlock()->getTexture(0);
        bool bVar1 = false;
        if (texture != NULL && texture->getTexInfo() != NULL
            && (texture->getFormat() == 0 || texture->getFormat() == 1)
            && texture->getTransparency() == 0)
        {
            bVar1 = true;
        }
        if (!param_2) {
            setStage(stage, bVar1 ? STAGE_1 : STAGE_0);
        } else {
            setStage(stage, bVar1 ? STAGE_6 : STAGE_5);
            stage = mMaterial->getTevBlock()->getTevStage(1);
            setStage(stage, STAGE_4);
        }
    } else if (!param_2) {
        J2DTevStage* stage = mMaterial->getTevBlock()->getTevStage(0);
        setStage(stage, STAGE_2);
        for (u8 i = 1; i < param_0; i++) {
            stage = mMaterial->getTevBlock()->getTevStage(i);
            setStage(stage, STAGE_3);
        }
        stage = mMaterial->getTevBlock()->getTevStage(param_0);
        setStage(stage, STAGE_4);
    } else {
        J2DTevStage* stage = mMaterial->getTevBlock()->getTevStage(0);
        setStage(stage, STAGE_2);
        for (u8 i = 1; i < param_0; i++) {
            stage = mMaterial->getTevBlock()->getTevStage(i);
            setStage(stage, STAGE_3);
        }
        stage = mMaterial->getTevBlock()->getTevStage(param_0);
        setStage(stage, STAGE_7);
        stage = mMaterial->getTevBlock()->getTevStage((u8)(param_0 + 1));
        setStage(stage, STAGE_4);
    }
}

void J2DPictureEx::setStage(J2DTevStage* stage, J2DPictureEx::stage_enum idx) {
    s8 colorData[8][4] = {
        {0x0F, 0x08, 0x0A, 0x0F},
        {0x0F, 0x08, 0x0A, 0x0F},
        {0x0F, 0x08, 0x0E, 0x0F},
        {0x0F, 0x08, 0x0E, 0x00},
        {0x0F, 0x0A, 0x00, 0x0F},
        {0x02, 0x04, 0x08, 0x0F},
        {0x02, 0x04, 0x08, 0x0F},
        {0x02, 0x04, 0x00, 0x0F},
    };
    s8 alphaData[8][4] = {
        {0x07, 0x04, 0x05, 0x07},
        {0x05, 0x07, 0x07, 0x07},
        {0x07, 0x04, 0x06, 0x07},
        {0x07, 0x04, 0x06, 0x00},
        {0x07, 0x05, 0x00, 0x07},
        {0x01, 0x02, 0x04, 0x07},
        {0x07, 0x07, 0x07, 0x02},
        {0x01, 0x02, 0x00, 0x07},
    };
    s8 opData[8][5] = {
        {0x00, 0x00, 0x00, 0x01, 0x00},
        {0x00, 0x00, 0x00, 0x01, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00},
        {0x00, 0x00, 0x00, 0x01, 0x00},
        {0x00, 0x00, 0x00, 0x01, 0x00},
        {0x00, 0x00, 0x00, 0x01, 0x00},
        {0x00, 0x00, 0x00, 0x00, 0x00},
    };

    stage->setTevColorAB(colorData[idx][0], colorData[idx][1]);
    stage->setTevColorCD(colorData[idx][2], colorData[idx][3]);
    stage->setTevColorOp(opData[idx][0], opData[idx][1], opData[idx][2], opData[idx][3], opData[idx][4]);
    stage->setAlphaABCD(alphaData[idx][0], alphaData[idx][1], alphaData[idx][2], alphaData[idx][3]);
    stage->setTevAlphaOp(opData[idx][0], opData[idx][1], opData[idx][2], opData[idx][3], opData[idx][4]);
}

void J2DPictureEx::setTevKColor(u8 param_0) {
    if (param_0 == 1) {
        return;
    }

    f32 fVar2 = 0.0f;
    f32 fVar3 = 0.0f;

    if (param_0 < 2) {
        for (u8 i = 0; i < param_0; i++) {
            fVar2 += field_0x11c[i];
            fVar3 += field_0x124[i];
        }
    } else {
        for (u8 i = 0; i < 2; i++) {
            fVar2 += field_0x11c[i];
            fVar3 += field_0x124[i];
        }
        for (u8 i = 0; i < param_0 - 2; i++) {
            fVar2 += field_0x160[i];
            fVar3 += field_0x178[i];
        }
    }

    u8 local_3c[8];
    u8 local_44[8];

    for (u8 i = 0; i < 8; i++) {
        local_3c[i] = 0xff;
        local_44[i] = 0xff;
    }

    for (u8 i = 0; i < param_0; i++) {
        if (i < 2) {
            local_3c[i] = field_0x11c[i] * 0xff / fVar2;
            local_44[i] = field_0x124[i] * 0xff / fVar3;
        } else {
            local_3c[i] = field_0x160[i - 2] * 0xff / fVar2;
            local_44[i] = field_0x178[i - 2] * 0xff / fVar3;
        }
    }

    JUtility::TColor color[4];
    color[3] = local_3c[3] << 24 | local_3c[2] << 16 | local_3c[1] << 8 | local_3c[0];
    color[2] = local_3c[7] << 24 | local_3c[6] << 16 | local_3c[5] << 8 | local_3c[4];
    color[1] = local_44[3] << 24 | local_44[2] << 16 | local_44[1] << 8 | local_44[0];
    color[0] = local_44[7] << 24 | local_44[6] << 16 | local_44[5] << 8 | local_44[4];

    for (u8 i = 0; i < 4; i++) {
        mMaterial->getTevBlock()->setTevKColor(i, color[i]);
    }
}

void J2DPictureEx::setTevKColorSel(u8 param_0) {
    for (u8 i = 0; i < param_0; i++) {
        if (i < 4) {
            mMaterial->getTevBlock()->setTevKColorSel(i, 31 - 4 * i);
        } else {
            mMaterial->getTevBlock()->setTevKColorSel(i, 30 - 4 * (i - 4));
        }
    }
}

void J2DPictureEx::setTevKAlphaSel(u8 param_0) {
    for (u8 i = 0; i < param_0; i++) {
        if (i < 4) {
            mMaterial->getTevBlock()->setTevKAlphaSel(i, 29 - 4 * i);
        } else {
            mMaterial->getTevBlock()->setTevKAlphaSel(i, 28 - 4 * (i - 4));
        }
    }
}

void J2DPictureEx::shiftSetBlendRatio(u8 param_0, f32 param_1, bool param_2, bool param_3) {
    f32* pJVar4 = param_2 != 0 ? field_0x11c : field_0x124;
    f32* local_44 = param_2 != 0 ? field_0x160 : field_0x178;

    if (param_3) {
        for (int i = 7; i > param_0; i--) {
            if (i > 2) {
                local_44[i - 2] = local_44[i - 3];
            } else if (i == 2) {
                local_44[0] = pJVar4[1];
            } else {
                pJVar4[i] = pJVar4[i - 1];
            }
        }
        if (param_0 >= 2) {
            local_44[param_0 - 2] = param_1;
        } else {
            pJVar4[param_0] = param_1;
        }
    } else {
        for (int i = param_0 + 1; i < 8; i++) {
            if (i > 2) {
                local_44[i - 3] = local_44[i - 2];
            } else if (i == 2) {
                pJVar4[1] = local_44[0];
            } else {
                pJVar4[i - 1] = pJVar4[i];
            }
        }
    }
}

void J2DPictureEx::setBlendColorRatio(f32 param_0, f32 param_1) {
    if (mMaterial == NULL) {
        return;
    }

    field_0x11c[0] = param_0;    
    field_0x11c[1] = param_1;    

    for (int i = 0; i < 6; i++) {
        field_0x160[i] = 1.0f;
    }

    setTevKColor(mMaterial->getTexGenBlock()->getTexGenNum());
}


void J2DPictureEx::setBlendAlphaRatio(f32 param_0, f32 param_1) {
    if (mMaterial == NULL) {
        return;
    }

    field_0x124[0] = param_0;    
    field_0x124[1] = param_1;    

    for (int i = 0; i < 6; i++) {
        field_0x178[i] = 1.0f;
    }

    setTevKColor(mMaterial->getTexGenBlock()->getTexGenNum());
}


const ResTIMG* J2DPictureEx::changeTexture(ResTIMG const* img, u8 param_1) {
    if (mMaterial == NULL || img == NULL) {
        return NULL;
    }

    u32 tex_gen_num = mMaterial->getTexGenBlock()->getTexGenNum();
    if (param_1 > tex_gen_num) {
        return NULL;
    }

    u8 max_stage = mMaterial->getTevBlock()->getMaxStage();
    max_stage = (u8) (max_stage > 8 ? (u8)8 : max_stage);
    if (param_1 >= max_stage) {
        return NULL;
    }

    if (param_1 < tex_gen_num) {
        const ResTIMG* texinfo = getTexture(param_1)->getTexInfo();
        u8 uVar6 = 0;
        if (img->indexTexture) {
            uVar6 = getUsableTlut(param_1);
        }
        getTexture(param_1)->storeTIMG(img, uVar6);
        return texinfo;
    }
    append(img, 1.0f);
    return NULL;
}

const ResTIMG* J2DPictureEx::changeTexture(char const* param_0, u8 param_1) {
    ResTIMG const* resource = (ResTIMG const*)J2DScreen::getNameResource(param_0);
    return changeTexture(resource, param_1);
}

const ResTIMG* J2DPictureEx::changeTexture(ResTIMG const* param_0, u8 param_1, JUTPalette* param_2) {
    if (mMaterial == NULL || param_0 == NULL) {
        return NULL;
    } 
    u32 uVar2 = mMaterial->getTexGenBlock()->getTexGenNum();
    if (param_1 > uVar2) {
        return NULL;
    }
    u8 maxStage = mMaterial->getTevBlock()->getMaxStage();
    maxStage = (u8) (maxStage > 8 ? (u8) 8 : maxStage);
    
    if (param_1 >= maxStage) {
        return NULL;
    }
    if (param_1 < uVar2) {
        const ResTIMG* texInfo = getTexture(param_1)->getTexInfo();
        GXTlut _Var7 = GX_TLUT0;
        if (param_0->indexTexture != 0) {
            _Var7 = getTlutID(param_0, getUsableTlut(param_1));
        }
        getTexture(param_1)->storeTIMG(param_0, param_2, _Var7);
        return texInfo;
    } 
    append(param_0, param_2, 1.0f);
    return NULL;
}

const ResTIMG* J2DPictureEx::changeTexture(char const* param_0, u8 param_1, JUTPalette* param_2) {
    ResTIMG const* resource = (ResTIMG const*)J2DScreen::getNameResource(param_0);
    return changeTexture(resource, param_1, param_2);
}

JUTTexture* J2DPictureEx::getTexture(u8 param_0) const {
    if (mMaterial == NULL) {
        return NULL;
    }

    if (mMaterial->getTevBlock() == NULL) {
        return NULL;
    }

    return mMaterial->getTevBlock()->getTexture(param_0);
}

u8 J2DPictureEx::getTextureCount() const {
    if (mMaterial == NULL) {
        return NULL;
    }

    return mMaterial->getTexGenBlock()->getTexGenNum();
}

bool J2DPictureEx::setBlack(JUtility::TColor param_0) {
    JUtility::TColor black, white;
    if (!getBlackWhite(&black, &white)) {
        return false;
    }

    return setBlackWhite(param_0, white);
}

bool J2DPictureEx::setWhite(JUtility::TColor param_0) {
    JUtility::TColor black, white;
    if (!getBlackWhite(&black, &white)) {
        return false;
    }

    return setBlackWhite(black, param_0);
}

bool J2DPictureEx::setBlackWhite(JUtility::TColor param_0, JUtility::TColor param_1) {
    if (mMaterial == NULL) {
        return false;
    }

    if (mMaterial->getTevBlock() == NULL) {
        return false;
    }

    if (!isSetBlackWhite(param_0, param_1)) {
        return false;
    }

    bool bVar1;
    u8 uVar2;
    u8 texGenNum = mMaterial->getTexGenBlock()->getTexGenNum();
    bVar1 = true;
    if ((param_0 == 0) && (param_1 == 0xffffffff)) {
        bVar1 = false;
    }
    if (texGenNum == 1) {
        uVar2 = (bVar1) ? 2 : 1;
    } else {
        uVar2 = texGenNum + ((bVar1) ? 2 : 1);
    }
    mMaterial->getTevBlock()->setTevStageNum(uVar2);
    setTevOrder(texGenNum, uVar2, bVar1);
    setTevStage(texGenNum, uVar2, bVar1);
    setTevKColor(texGenNum);
    setTevKColorSel(texGenNum);
    setTevKAlphaSel(texGenNum);
    if (bVar1) {
        J2DGXColorS10 local_38;
        local_38.r = param_0.r;
        local_38.g = param_0.g;
        local_38.b = param_0.b;
        local_38.a = param_0.a;
        mMaterial->getTevBlock()->setTevColor(0, local_38);
        local_38.r = param_1.r;
        local_38.g = param_1.g;
        local_38.b = param_1.b;
        local_38.a = param_1.a;
        mMaterial->getTevBlock()->setTevColor(1, local_38);
    }
    return true;
}

bool J2DPictureEx::getBlackWhite(JUtility::TColor* black, JUtility::TColor* white) const {
    // FAKEMATCH
    if (mMaterial == NULL) {
        return false;
    }

    if (mMaterial->getTevBlock() == NULL) {
        return false;
    }

    u8 tex_gen_num = mMaterial->getTexGenBlock()->getTexGenNum();
    u8 tev_stage_num = mMaterial->getTevBlock()->getTevStageNum() & 0xff;
    bool bVar1;
    if (tex_gen_num == 1) {
        bVar1 = tev_stage_num == 1 ? false : true;
    } else {
        bVar1 = tev_stage_num == tex_gen_num + 1 ? false : true;
    }

    *black = 0x00000000;
    *white = 0xffffffff;

    if (bVar1) {
        #if DEBUG || VERSION == VERSION_WII_USA_R0 || VERSION == VERSION_WII_USA_R2
        J2DGXColorS10 tevColor0 = *mMaterial->getTevBlock()->getTevColor(0);
        J2DGXColorS10 tevColor1 = *mMaterial->getTevBlock()->getTevColor(1);
        #else
        J2DGXColorS10 tevColor0, tevColor1;
        tevColor0 = *mMaterial->getTevBlock()->getTevColor(0);
        tevColor1 = *mMaterial->getTevBlock()->getTevColor(1);
        #endif
        *black = (u8)tevColor0.r << 24 | (u8)tevColor0.g << 16 | (u8)tevColor0.b << 8 | (u8)tevColor0.a;
        *white = (u8)tevColor1.r << 24 | (u8)tevColor1.g << 16 | (u8)tevColor1.b << 8 | (u8)tevColor1.a;
    }

    return true;
}

bool J2DPictureEx::isSetBlackWhite(JUtility::TColor black, JUtility::TColor white) const {
    if (black == 0x00000000 && white == 0xffffffff) {
        return true;
    }

    u8 tex_gen_num = mMaterial->getTexGenBlock()->getTexGenNum();
    u32 tev_stage_num = mMaterial->getTevBlock()->getTevStageNum();
    u8 max_tev_stage = mMaterial->getTevBlock()->getMaxStage();

    if (max_tev_stage == 16) {
        return true;
    }

    if (max_tev_stage == 1) {
        return false;
    }

    u8 tmp = (u8)(tex_gen_num == 1 ? 2 : (tex_gen_num + 2));
    return tmp <= max_tev_stage;
}

JUtility::TColor J2DPictureEx::getBlack() const {
    JUtility::TColor black, white;
    if (!getBlackWhite(&black, &white)) {
        return 0x00000000;
    } else {
        return black;
    }
}

JUtility::TColor J2DPictureEx::getWhite() const {
    JUtility::TColor black, white;
    if (!getBlackWhite(&black, &white)) {
        return 0xffffffff;
    } else {
        return white;
    }
}

void J2DPictureEx::setAlpha(u8 alpha) {
    mAlpha = alpha;
    if (mMaterial != NULL && mMaterial->getColorBlock()->getMatColor(0) != NULL) {
        mMaterial->getColorBlock()->getMatColor(0)->a = alpha;
    }
}

void J2DPictureEx::setCullBack(GXCullMode cullMode) {
    mCullMode = cullMode;
    if (mMaterial != NULL) {
        mMaterial->getColorBlock()->setCullMode(cullMode);
    }
    J2DPane::setCullBack(cullMode);
}

void J2DPictureEx::rewriteAlpha() {
    if (mMaterial != NULL) {
        mAlpha = mMaterial->getColorBlock()->getMatColor(0)->a;
    }
}

bool J2DPictureEx::isUsed(ResTIMG const* texInfo) {
    if (mMaterial != NULL && mMaterial->getTevBlock() != NULL) {
        for (u32 i = 0; i < 8; i++) {
            JUTTexture* texture = mMaterial->getTevBlock()->getTexture(i);
            if (texture != NULL && texture->getTexInfo() == texInfo) {
                return true;
            }
        }
    }
    return J2DPane::isUsed(texInfo);
}

void J2DPictureEx::setAnimation(J2DAnmColor* anm) {
    if (mMaterial != NULL) {
        mMaterial->setAnimation(anm);
    }
}

void J2DPictureEx::setAnimation(J2DAnmTextureSRTKey* anm) {
    if (mMaterial != NULL) {
        mMaterial->setAnimation(anm);
    }
}

void J2DPictureEx::setAnimation(J2DAnmTexPattern* anm) {
    if (mMaterial != NULL) {
        mMaterial->setAnimation(anm);
    }
}

void J2DPictureEx::setAnimation(J2DAnmTevRegKey* anm) {
    if (mMaterial != NULL) {
        mMaterial->setAnimation(anm);
    }
}

void J2DPictureEx::setAnimation(J2DAnmVisibilityFull* anm) {
    field_0x194 = anm;
}

void J2DPictureEx::setAnimation(J2DAnmVtxColor* anm) {
    field_0x198 = anm;
    field_0x19c = 0;

    if (anm != NULL) {
        u16 anm_table_num = anm->getAnmTableNum(0);
        for (u8 i = 0; i < 4; i++) {
            if (field_0x158[i] != 0xFFFF) {
                for (u16 j = 0; j < anm_table_num; j++) {
                    #if DEBUG
                    J3DAnmVtxColorIndexData* data = anm->getAnmVtxColorIndexData(0, j);
                    u16* index2 = anm->getVtxColorIndexPointer(0) + (uintptr_t)data->mpData;
                    #else
                    J3DAnmVtxColorIndexData* data = anm->getAnmVtxColorIndexData(0, j);
                    u16* index = anm->getVtxColorIndexPointer(0);
                    u16* index2 = index + (uintptr_t)data->mpData;
                    #endif
                    for (u16 k = 0; k < data->mNum; k++) {
                        if (index2[k] == field_0x158[i]) {
                            field_0x198 = anm;
                            field_0x19c |= 1 << i;
                            goto next;
                        }
                    }
                }
            }
            next:;
        }
    }

    if (field_0x19c == 0) {
        field_0x198 = NULL;
    }
}

const J2DAnmTransform* J2DPictureEx::animationPane(J2DAnmTransform const* anm) {
    if (field_0x194 != NULL && field_0x154 != 0xffff) {
        u8 tmp;
        field_0x194->getVisibility(field_0x154, &tmp);
        if (tmp != 0) {
            show();
        } else {
            hide();
        }
    }

    if (field_0x198 != NULL) {
        u16 anm_table_num = field_0x198->getAnmTableNum(0);
        for (u8 i = 0; i < 4; i++) {
            if (field_0x19c & (1 << i)) {
                for (u16 j = 0; j < anm_table_num; j++) {
                    J3DAnmVtxColorIndexData* data = field_0x198->getAnmVtxColorIndexData(0, j);
                    u16* index = field_0x198->getVtxColorIndexPointer(0) + (uintptr_t)data->mpData;
                    for (u16 k = 0; k < data->mNum; k++) {
                        if (index[k] == field_0x158[i]) {
                            field_0x198->getColor(0, j, &mCornerColor[i]);
                            goto next;
                        }
                    }
                }
            }
            next:;
        }
    }

    return J2DPane::animationPane(anm);
}

u8 J2DPictureEx::getUsableTlut(u8 param_0) {
    u32 tex_gen_num = mMaterial->getTexGenBlock()->getTexGenNum();
    u8 usedTluts = 0;

    for (u8 i = 0; i < tex_gen_num; i++) {
        JUTTexture* texture = mMaterial->getTevBlock()->getTexture(i);
        if (i != param_0 && texture != NULL) {
            const ResTIMG* texInfo = texture->getTexInfo();
            if (texInfo != NULL && texInfo->indexTexture != 0) {
                int tlut_name = texture->getTlutName();
                u8 tlut_index = tlut_name - (tlut_name >= GX_BIGTLUT0 ? GX_BIGTLUT0 : GX_TLUT0);
                if (tlut_index < 8) {
                    usedTluts |= 1 << tlut_index;
                }
            }
        }
    }

    u8 i = 0;
    for (; i < 8; i++) {
        if (!(usedTluts & (1 << i))) {
            return i;
        }
    }
    return 0;
}

bool J2DPictureEx::append(ResTIMG const* param_0, f32 param_1) {
    return append(param_0, NULL, param_1);
}

bool J2DPictureEx::append(char const* param_0, f32 param_1) {
    return append(param_0, NULL, param_1);
}

void J2DPictureEx::prepend(JUTTexture* param_0, f32 param_1) {
    insert(param_0, 0, param_1);
}

void J2DPictureEx::prepend(char const* param_0, JUTPalette* param_1, f32 param_2) {
    insert(param_0, param_1, 0, param_2);
}

void J2DPictureEx::prepend(char const* param_0, f32 param_1) {
    insert(param_0, 0, param_1);
}

void J2DPictureEx::prepend(ResTIMG const* param_0, JUTPalette* param_1, f32 param_2) {
    insert(param_0, param_1, 0, param_2);
}

void J2DPictureEx::prepend(ResTIMG const* param_0, f32 param_1) {
    insert(param_0, 0, param_1);
}

bool J2DPictureEx::insert(char const* param_0, u8 param_1, f32 param_2) {
    return insert(param_0, NULL, param_1, param_2);
}

bool J2DPictureEx::insert(ResTIMG const* param_0, u8 param_1, f32 param_2) {
    return insert(param_0, NULL, param_1, param_2);
}

void J2DPictureEx::draw(f32 param_0, f32 param_1, bool param_2, bool param_3, bool param_4) {
    return draw(param_0, param_1, 0, param_2, param_3, param_4);
}

void J2DPictureEx::drawOut(f32 param_0, f32 param_1, f32 param_2, f32 param_3, f32 param_4,
                               f32 param_5, f32 param_6, f32 param_7) {
    drawOut(JGeometry::TBox2<f32>(param_0, param_1, param_0 + param_2, param_1 + param_3),
            JGeometry::TBox2<f32>(param_4, param_5, param_4 + param_6, param_5 + param_7));
}

void J2DPictureEx::load(u8 param_0) {
    load((GXTexMapID)param_0, param_0);
}

void J2DPictureEx::setCullBack(bool param_0) {
    setCullBack(param_0 ? GX_CULL_BACK : GX_CULL_NONE);
}

bool J2DPictureEx::isUsed(ResFONT const* param_0) {
    return J2DPane::isUsed(param_0);
}

void J2DPictureEx::setAnimation(J2DAnmBase* param_0) {
    J2DPane::setAnimation(param_0);
}
