#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J2DGraph/J2DWindowEx.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "JSystem/JSupport/JSURandomInputStream.h"

struct J2DWindowExDef {
    u32 field_0x0[4];
    u16 field_0x10[4];
    u8 field_0x18;
    u8 field_0x19;
    u16 field_0x1A;
    u16 field_0x1C;
    u16 field_0x1E;
    u16 field_0x20;
    u16 field_0x22;
    u16 field_0x24;
    u16 field_0x26;
    u16 field_0x28[4];
    u32 field_0x30[4];
};

STATIC_ASSERT(sizeof(J2DWindowExDef) == 0x40);

J2DWindowEx::J2DWindowEx(J2DPane* param_0, JSURandomInputStream* param_1, u32 param_2,
                             J2DMaterial* param_3) : J2DWindow() {
    mAnmVisibilityFull = NULL;
    mAnmVtxColor = NULL;
    s32 position = param_1->getPosition();
    int uStack_88[2];
    param_1->read(uStack_88, 8);
    mKind = uStack_88[0];
    s32 iVar2 = param_1->getPosition();
    int auStack_90[2];
    param_1->peek(auStack_90, 8);
    makePaneExStream(param_0, param_1);
    param_1->seek(iVar2 + auStack_90[1], JSUStreamSeekFrom_SET);
    J2DWindowExDef auStack_70;
    param_1->read(&auStack_70, sizeof(J2DWindowExDef));

    JUtility::TColor* colors[4] = {NULL};
    colors[0] = &field_0x128;
    colors[1] = &field_0x12C;
    colors[2] = &field_0x130;
    colors[3] = &field_0x134;

    for (int i = 0; i < 4; i++) {
        field_0x158[i] = auStack_70.field_0x10[i];
        mFrameMaterial[i] = 0;
        if (field_0x158[i] != 0xffff) {
            mFrameMaterial[i] = param_3 + field_0x158[i];
            (param_3 + field_0x158[i])->field_0x4 = this;
        }
        field_0x168[i] = auStack_70.field_0x28[i];
        *(colors[i]) = JUtility::TColor(auStack_70.field_0x30[i]);
    }

    field_0x144 = auStack_70.field_0x18;
    field_0x114.set(auStack_70.field_0x1A, auStack_70.field_0x1C,
                    auStack_70.field_0x1A + auStack_70.field_0x1E,
                    auStack_70.field_0x1C + auStack_70.field_0x20);
    field_0x166 = auStack_70.field_0x22;
    field_0x164 = auStack_70.field_0x24;
    mContentsMaterial = NULL;

    if (field_0x164 != 0xffff) {
        mContentsMaterial = &param_3[field_0x164];
        param_3[field_0x164].field_0x4 = this;
    }

    param_1->seek(position + uStack_88[1], JSUStreamSeekFrom_SET);
    rewriteAlpha();
    field_0x100 = NULL;
    field_0x104 = NULL;
    field_0x108 = NULL;
    field_0x10c = NULL;
    mPalette = NULL;
    field_0x110 = NULL;
    field_0x170 = 0;
    setMinSize();
}

void J2DWindowEx::setMinSize() {
    field_0x140 = 1;
    field_0x142 = 1;

    if (mFrameMaterial[0] == NULL) return;
    if (mFrameMaterial[1] == NULL) return;
    if (mFrameMaterial[2] == NULL) return;
    if (mFrameMaterial[3] == NULL) return;
    if (mFrameMaterial[0]->getTevBlock() == NULL) return;
    if (mFrameMaterial[1]->getTevBlock() == NULL) return;
    if (mFrameMaterial[2]->getTevBlock() == NULL) return;
    if (mFrameMaterial[3]->getTevBlock() == NULL) return;
    if (mFrameMaterial[0]->getTevBlock()->getTexture(0) == NULL) return;
    if (mFrameMaterial[1]->getTevBlock()->getTexture(0) == NULL) return;
    if (mFrameMaterial[2]->getTevBlock()->getTexture(0) == NULL) return;
    if (mFrameMaterial[3]->getTevBlock()->getTexture(0) == NULL) return;

    field_0x140 = mFrameMaterial[0]->getTevBlock()->getTexture(0)->getWidth()
                + mFrameMaterial[1]->getTevBlock()->getTexture(0)->getWidth();
    field_0x142 = mFrameMaterial[0]->getTevBlock()->getTexture(0)->getHeight()
                + mFrameMaterial[2]->getTevBlock()->getTexture(0)->getHeight();
}

J2DWindowEx::~J2DWindowEx() {
    for (u8 i = 0; i < 4; i++) {
        if (field_0x170 & (1 << i)) {
            delete mFrameMaterial[i];
        }
    }

    if (field_0x170 & 0x10) {
        delete mContentsMaterial;
    }
}

void J2DWindowEx::drawSelf(f32 param_0, f32 param_1, f32 (*param_2)[3][4]) {
    JGeometry::TBox2<f32> aTStack_50(mBounds);
    Mtx auStack_40;
    aTStack_50.addPos(JGeometry::TVec2<f32>(param_0, param_1));
    MTXConcat(*param_2, mGlobalMtx, auStack_40);
    GXLoadPosMtxImm(auStack_40, 0);
    draw_private(aTStack_50, field_0x114);
    clip(field_0x114);
}

void J2DWindowEx::draw_private(JGeometry::TBox2<f32> const& param_0,
                               JGeometry::TBox2<f32> const& param_1) {
    if (param_0.getWidth() >= field_0x140 && param_0.getHeight() >= field_0x142) {
        JUTTexture* textures[4];
        bool anyFrameMaterialNull = false;
        for (int i = 0; i < 4; i++) {
            if (mFrameMaterial[i] == NULL) {
                return;
            }
            J2DTevBlock* block = mFrameMaterial[i]->getTevBlock();
            if (block == NULL) {
                return;
            }

            textures[i] = block->getTexture(0);
            if (textures[i] == NULL) {
                anyFrameMaterialNull = true;
            }
        }

        JGeometry::TBox2<f32> aTStack_b8(param_1);
        aTStack_b8.addPos(param_0.i);
        drawContents(aTStack_b8);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

        if (!anyFrameMaterialNull) {
            f32 dVar16 = param_0.i.x;
            f32 dVar15 = param_0.i.y;
            f32 dVar18 = param_0.f.x - textures[3]->getWidth();
            f32 dVar17 = param_0.f.y - textures[3]->getHeight();
            f32 dVar14 = dVar16 + textures[0]->getWidth();
            f32 dVar12 = dVar15 + textures[0]->getHeight();
            u16 local_c4;
            if (field_0x144 & 0x80) {
                local_c4 = 0;
            } else {
                local_c4 = 0x8000;
            }
            u16 local_c6;
            if (field_0x144 & 0x40) {
                local_c6 = 0;
            } else {
                local_c6 = 0x8000;
            }

            drawFrameTexture(dVar16, dVar15, textures[0]->getWidth(), textures[0]->getHeight(),
                             local_c4, local_c6, 0x8000 - local_c4, 0x8000 - local_c6,
                             mFrameMaterial[0], true);
            bool r9 = mFrameMaterial[1] != mFrameMaterial[0];
            u16 local_c8;
            if (field_0x144 & 0x20) {
                local_c8 = 0;
            } else {
                local_c8 = 0x8000;
            }
            u16 local_ca;
            if (field_0x144 & 0x10) {
                local_ca = 0;
            } else {
                local_ca = 0x8000;
            }
            drawFrameTexture(dVar18, dVar15, textures[3]->getWidth(), textures[0]->getHeight(),
                             local_c8, local_ca, 0x8000 - local_c8, 0x8000 - local_ca,
                             mFrameMaterial[1], r9);

            u16 local_cc;
            if (field_0x144 & 0x20) {
                local_cc = 0x8000;
            } else {
                local_cc = 0;
            }

            u16 local_ce;
            if (field_0x144 & 0x10) {
                local_ce = 0;
            } else {
                local_ce = 0x8000;
            }
            drawFrameTexture(dVar14, dVar15, dVar18 - dVar14, textures[0]->getHeight(), local_cc,
                             local_ce, local_cc, local_ce ^ 0x8000, mFrameMaterial[1], false);

            r9 = mFrameMaterial[3] != mFrameMaterial[1];
            u16 local_d0;
            if (field_0x144 & 2) {
                local_d0 = 0;
            } else {
                local_d0 = 0x8000;
            }
            u16 local_d2;
            if (field_0x144 & 1) {
                local_d2 = 0;
            } else {
                local_d2 = 0x8000;
            }

            drawFrameTexture(dVar18, dVar17, textures[3]->getWidth(), textures[3]->getHeight(),
                             local_d0, local_d2, 0x8000 - local_d0, 0x8000 - local_d2,
                             mFrameMaterial[3], r9);

            u16 local_d4;
            if (field_0x144 & 2) {
                local_d4 = 0x8000;
            } else {
                local_d4 = 0;
            }

            u16 local_d6;
            if (field_0x144 & 1) {
                local_d6 = 0;
            } else {
                local_d6 = 0x8000;
            }
            drawFrameTexture(dVar14, dVar17, dVar18 - dVar14, textures[3]->getHeight(), local_d4,
                             local_d6, local_d4, local_d6 ^ 0x8000, mFrameMaterial[3], false);

            u16 local_d8;
            if (field_0x144 & 2) {
                local_d8 = 0;
            } else {
                local_d8 = 0x8000;
            }

            u16 local_da;
            if (field_0x144 & 1) {
                local_da = 0x8000;
            } else {
                local_da = 0;
            }
            drawFrameTexture(dVar18, dVar12, textures[3]->getWidth(), dVar17 - dVar12, local_d8,
                             local_da, local_d8 ^ 0x8000, local_da, mFrameMaterial[3], false);

            r9 = mFrameMaterial[2] != mFrameMaterial[3];
            u16 local_dc;
            if (field_0x144 & 8) {
                local_dc = 0;
            } else {
                local_dc = 0x8000;
            }

            u16 local_de;
            if (field_0x144 & 4) {
                local_de = 0;
            } else {
                local_de = 0x8000;
            }
            drawFrameTexture(dVar16, dVar17, textures[0]->getWidth(), textures[3]->getHeight(),
                             local_dc, local_de, 0x8000 - local_dc, 0x8000 - local_de,
                             mFrameMaterial[2], r9);

            u16 local_e0;
            if (field_0x144 & 8) {
                local_e0 = 0;
            } else {
                local_e0 = 0x8000;
            }

            u16 local_e2;
            if (field_0x144 & 4) {
                local_e2 = 0x8000;
            } else {
                local_e2 = 0;
            }
            drawFrameTexture(dVar16, dVar12, textures[0]->getWidth(), dVar17 - dVar12, local_e0,
                             local_e2, local_e0 ^ 0x8000, local_e2, mFrameMaterial[2], false);
        }

        GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetNumTexGens(0);
        GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
    }
}

static f32 dummy_literal() {
    return 0.0f;
}

void J2DWindowEx::drawContents(JGeometry::TBox2<f32> const& param_1) {
    if (!param_1.isValid() || mContentsMaterial == NULL)  {
        return;
    }
    if (!mContentsMaterial->isVisible()) {
        return;
    }
    mContentsMaterial->setGX();
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    JUtility::TColor TStack_b4(field_0x128);
    JUtility::TColor TStack_b8(field_0x130);
    JUtility::TColor TStack_bc(field_0x12C);
    JUtility::TColor TStack_c0(field_0x134);
    if (mContentsMaterial->getColorBlock()->getColorChan(1)->getMatSrc() == 1) {
        if (mContentsMaterial->getMaterialAlphaCalc() == 1) {
            TStack_b4.a = (TStack_b4.a * mColorAlpha) / 0xff;
            TStack_b8.a = (TStack_b8.a * mColorAlpha) / 0xff;
            TStack_bc.a = (TStack_bc.a * mColorAlpha) / 0xff;
            TStack_c0.a = (TStack_c0.a * mColorAlpha) / 0xff;
        }
    } else if (mIsInfluencedAlpha) {
        GXSetChanMatColor(GX_ALPHA0, JUtility::TColor(mColorAlpha));
    }
    bool bVar5 = false;
    f32 in_f31;
    f32 in_f30;
    f32 in_f29;
    f32 in_f28;
    if (mContentsMaterial->getTevBlock() != NULL) {
        if (mContentsMaterial->getTevBlock()->getTexture(0)) {
            bVar5 = true;
            GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
            f32 dVar15 = mContentsMaterial->getTevBlock()->getTexture(0)->getWidth();
            f32 dVar14 = mContentsMaterial->getTevBlock()->getTexture(0)->getHeight();

            f32 dVar12 = param_1.getWidth();
            f32 dVar13 = param_1.getHeight();
            in_f31 = -(dVar12 / dVar15 - 1.0f) / 2;
            in_f30 = -((dVar13 / dVar14) - 1.0f) / 2;
            in_f29 = in_f31 + (dVar12 / dVar15);
            in_f28 = in_f30 + (dVar13 / dVar14);
        }
    }
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_F32, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(param_1.i.x, param_1.i.y,
                        0.0f);
    GXColor1u32(TStack_b4);
    if (bVar5) {
        GXTexCoord2f32(in_f31, in_f30);
    }
    GXPosition3f32(param_1.f.x, param_1.i.y,
                        0.0f);
    GXColor1u32(TStack_bc);
    if (bVar5) {
        GXTexCoord2f32(in_f29, in_f30);
    }
    GXPosition3f32(param_1.f.x, param_1.f.y,
                        0.0f);
    GXColor1u32(TStack_c0);
    if (bVar5) {
        GXTexCoord2f32(in_f29, in_f28);
    }
    GXPosition3f32(param_1.i.x, param_1.f.y,
                        0.0f);
    GXColor1u32(TStack_b8);
    if (bVar5) {
        GXTexCoord2f32(in_f31, in_f28);
    }
    GXEnd();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBX8, 0xf);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGBA4, 0);
}


void J2DWindowEx::drawFrameTexture(f32 param_1, f32 param_2, f32 param_3, f32 param_4, u16 param_5,
                                   u16 param_6, u16 param_7, u16 param_8, J2DMaterial* param_9,
                                   bool param_10) {
    if (param_9 != NULL && param_9->isVisible()) {
        f32 dVar15 = param_1 + param_3;
        f32 dVar14 = param_2 + param_4;
        if (param_10) {
            param_9->setGX();
        }
        JUtility::TColor aTStack_84(0xffffffff);
        J2DPane* parentPane = getParentPane();
        if (param_9->getColorBlock()->getColorChan(1)->getMatSrc() == 1) {
            if (param_9->getMaterialAlphaCalc() == 1) {
                u8 uVar11 = 0xff;
                if (param_9->getColorBlock()->getMatColor(0) != 0) {
                    uVar11 = param_9->getColorBlock()->getMatColor(0)->a;
                }
                if (parentPane != NULL && mIsInfluencedAlpha != 0) {
                    uVar11 = ((uVar11 * parentPane->mColorAlpha) / 0xff);
                }
                aTStack_84 = JUtility::TColor((u32)uVar11 | 0xffffff00);
            }
        } else if (parentPane != NULL && mIsInfluencedAlpha != 0 && param_10) {
            s32 matColorAlpha = param_9->getColorBlock()->getMatColor(0)->a;
            s32 colorAlpha = parentPane->mColorAlpha;
            GXSetChanMatColor(GX_ALPHA0, JUtility::TColor((matColorAlpha * colorAlpha / 0xff) & 0xff));
        }
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_F32, 0);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GXPosition3f32(param_1, param_2, 0.0f);
        GXColor1u32(aTStack_84);
        GXTexCoord2u16(param_7, param_8);
        GXPosition3f32(dVar15, param_2, 0.0f);
        GXColor1u32(aTStack_84);
        GXTexCoord2u16(param_5, param_8);
        GXPosition3f32(dVar15, dVar14, 0.0f);
        GXColor1u32(aTStack_84);
        GXTexCoord2u16(param_5, param_6);
        GXPosition3f32(param_1, dVar14, 0.0f);
        GXColor1u32(aTStack_84);
        GXTexCoord2u16(param_7, param_6);
        GXEnd();
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_CLR_RGBA, GX_RGBA4, 0);
    }
}

void J2DWindowEx::draw(JGeometry::TBox2<f32> const& param_1) {
    bool isMissingTexture = false;
    JUTTexture* local_68[4];
    for (int i = 0; i < 4; i++) {
        if (mFrameMaterial[i] == NULL) {
            return;
        }
        if (mFrameMaterial[i]->getTevBlock() == NULL) {
            return;
        }
        local_68[i] = mFrameMaterial[i]->getTevBlock()->getTexture(0);
        if (local_68[i] == NULL) {
            isMissingTexture = true;
        }
    }
    JGeometry::TBox2<f32> aTStack_78;
    if (!isMissingTexture) {
        aTStack_78.set(local_68[0]->getWidth(), local_68[0]->getHeight(),
                       param_1.getWidth() - local_68[1]->getWidth(),
                       param_1.getHeight() - local_68[2]->getHeight());
    } else {
        aTStack_78.set(0.0f, 0.0f, param_1.getWidth(), param_1.getHeight());
    }
    draw(param_1, aTStack_78);
}

void J2DWindowEx::draw(JGeometry::TBox2<f32> const& param_1, JGeometry::TBox2<f32> const& param_2) {
    rewriteAlpha();
    mColorAlpha = mAlpha;
    makeMatrix(param_1.i.x, param_1.i.y, 0.0f, 0.0f);
    GXLoadPosMtxImm(mPositionMtx, 0);
    GXSetCurrentMtx(0);
    JGeometry::TBox2<f32> aTStack_70(0.0f, 0.0f, param_1.getWidth(), param_1.getHeight());
    draw_private(aTStack_70, param_2);
    for (int i = GX_TEV_SWAP0; i < GX_MAX_TEVSWAP; i++) {
        GXSetTevSwapModeTable((GXTevSwapSel)i, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
    }
    GXSetNumIndStages(0);
    for (int i = GX_TEVSTAGE0; i < GX_MAX_TEVSTAGE; i++) {
        GXSetTevDirect((GXTevStageID)i);
    }
    Mtx auStack_60;
    MTXIdentity(auStack_60);
    GXLoadPosMtxImm(auStack_60, 0);
}

void J2DWindowEx::setTevOrder(bool param_0) {
    u16 local_28[2];
    if (!param_0) {
        local_28[0] = 4;
        local_28[1] = 0xffff;
    } else {
        local_28[0] = 0xff;
        local_28[1] = 0xff04;
    }
    for (u8 i = 0; i < 2; i++) {
        s32 uVar1 = local_28[i];
        J2DTevOrderInfo info;
        info.mTexCoord = uVar1 >> 8;
        info.mTexMap = uVar1 >> 8;
        info.mColor = uVar1;
        J2DTevOrder local_30(info);
        for (int j = 0; j < 4; j++) {
            if (mFrameMaterial[j]->getTevBlock()->getMaxStage() > i) {
                mFrameMaterial[j]->getTevBlock()->setTevOrder(i, local_30);
            }
        }
    }
}

void J2DWindowEx::setTevStage(bool param_1) {
    for (int i = 0; i < 4; i++) {
        J2DTevStage* pJVar3 = mFrameMaterial[i]->getTevBlock()->getTevStage(0);
        JUTTexture* this_00 = mFrameMaterial[i]->getTevBlock()->getTexture(0);
        bool bVar1 = false;
        if (this_00 != NULL && ((s32)this_00->getFormat() == 0 || (s32)this_00->getFormat() == 1) &&
            this_00->getTransparency() == 0)
        {
            bVar1 = true;
        }
        if (!param_1) {
            setStage(pJVar3, bVar1 ? STAGE_ENUM_1 : STAGE_ENUM_0);
        } else {
            stage_enum sVar5;
            if (bVar1) {
                sVar5 = STAGE_ENUM_4;
            } else {
                sVar5 = STAGE_ENUM_3;
            }
            setStage(pJVar3, sVar5);
            setStage(mFrameMaterial[i]->getTevBlock()->getTevStage(1), STAGE_ENUM_2);
        }
    }
}

static u8 const lit_1557[16] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static u8 const lit_1566[16] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static u8 const lit_1575[16] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


static u8 const lit_1581[16] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


static u8 const lit_1587[16] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static u8 const lit_1596[16] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static u8 const lit_1605[16] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static u8 const lit_1612[16] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

void J2DWindowEx::setStage(J2DTevStage* param_0, J2DWindowEx::stage_enum param_1) {
    s8 local_30[6][4] = {
        {0x0f, 0x08, 0x0a, 0x0f}, {0x0f, 0x08, 0x0a, 0x0f}, {0x0f, 0x0a, 0x00, 0x0f},
        {0x02, 0x04, 0x08, 0x0f}, {0x02, 0x04, 0x08, 0x0f}, {0x0f, 0x0f, 0x0f, 0x0a},
    };
    s8 local_48[6][4] = {
        {0x07, 0x04, 0x05, 0x07}, {0x05, 0x07, 0x07, 0x07}, {0x07, 0x05, 0x00, 0x07},
        {0x01, 0x02, 0x04, 0x07}, {0x07, 0x07, 0x07, 0x02}, {0x07, 0x07, 0x07, 0x05},
    };
    s8 local_68[6][5] = {
        {0, 0, 0, 1, 0}, {0, 0, 0, 1, 0}, {0, 0, 0, 1, 0},
        {0, 0, 0, 1, 0}, {0, 0, 0, 1, 0}, {0, 0, 0, 1, 0},
    };

    param_0->setTevColorAB(local_30[param_1][0], local_30[param_1][1]);
    param_0->setTevColorCD(local_30[param_1][2], local_30[param_1][3]);
    param_0->setTevColorOp(local_68[param_1][0], local_68[param_1][1], local_68[param_1][2],
                           local_68[param_1][3], local_68[param_1][4]);
    param_0->setAlphaABCD(local_48[param_1][0], local_48[param_1][1], local_48[param_1][2],
                          local_48[param_1][3]);
    param_0->setTevAlphaOp(local_68[param_1][0], local_68[param_1][1], local_68[param_1][2],
                           local_68[param_1][3], local_68[param_1][4]);
}


bool J2DWindowEx::setBlack(JUtility::TColor black) {
    JUtility::TColor dummy,white;

    if (!getBlackWhite(&dummy, &white)) {
        return false;
    }

    return setBlackWhite(black, white);
}

bool J2DWindowEx::setWhite(JUtility::TColor white) {
    JUtility::TColor black,dummy;

    if (!getBlackWhite(&black, &dummy)) {
        return false;
    }

    return setBlackWhite(black, white);
}

// NONMATCHING - J2DGXColorS10 issue
bool J2DWindowEx::setBlackWhite(JUtility::TColor black, JUtility::TColor white) {
    for (int i = 0; i < 4; i++) {
        if (mFrameMaterial[i] == NULL) {
            return false;
        }

        if (mFrameMaterial[i]->getTevBlock() == NULL) {
            return false;
        }
    }

    if (!isSetBlackWhite(black, white)) {
        return false;
    }

    bool bVar1 = (u32)black != 0 || (u32)white != 0xffffffff;
    u8 uVar3 = bVar1 ? 2 : 1;
    for (int i = 0; i < 4; i++) {
        mFrameMaterial[i]->getTevBlock()->setTevStageNum(uVar3);
    }
    setTevOrder(bVar1);
    setTevStage(bVar1);
    if (bVar1) {
        J2DGXColorS10 color0, color1;
        color0.r = black.r;
        color0.g = black.g;
        color0.b = black.b;
        color0.a = black.a;
        color1.r = white.r;
        color1.g = white.g;
        color1.b = white.b;
        color1.a = white.a;

        for (int i = 0; i < 4; i++) {
            mFrameMaterial[i]->getTevBlock()->setTevColor(0, color0);
            mFrameMaterial[i]->getTevBlock()->setTevColor(1, color1);
        }
    }
    return true;
}

bool J2DWindowEx::getBlackWhite(JUtility::TColor* param_0, JUtility::TColor* param_1) const {
    if (mFrameMaterial[0] == NULL) {
        return false;
    }

    if (mFrameMaterial[0]->getTevBlock() == NULL) {
        return false;
    }

    bool cVar6 = mFrameMaterial[0]->getTevBlock()->getTevStageNum() != 1;
    *param_0 = JUtility::TColor(0);
    *param_1 = JUtility::TColor(0xffffffff);
    if (cVar6) {
        J2DGXColorS10* color0p = mFrameMaterial[0]->getTevBlock()->getTevColor(0);
        GXColorS10 color0;
        color0.r = color0p->r;
        color0.g = color0p->g;
        color0.b = color0p->b;
        color0.a = color0p->a;
        J2DGXColorS10* color1p = mFrameMaterial[0]->getTevBlock()->getTevColor(1);
        GXColorS10 color1;
        color1.r = color1p->r;
        color1.g = color1p->g;
        color1.b = color1p->b;
        color1.a = color1p->a;
        *param_0 = JUtility::TColor((((u8)color0.r) << 0x18) | (((u8)color0.g) << 0x10)
                                    | (((u8)color0.b) << 0x8) | (((u8)color0.a)));
        *param_1 = JUtility::TColor((((u8)color1.r) << 0x18) | (((u8)color1.g) << 0x10)
                                    | (((u8)color1.b) << 0x8) | (((u8)color1.a)));
    }
    return true;
}

bool J2DWindowEx::isSetBlackWhite(JUtility::TColor param_0, JUtility::TColor param_1) const {
    if ((u32)param_0 == 0 && (u32)param_1 == 0xffffffff) {
        return true;
    }
    for (int i = 0; i < 4; i++) {
        if (mFrameMaterial[i]->getTevBlock()->getMaxStage() == 1) {
            return false;
        }
    }
    return true;
}

JUtility::TColor J2DWindowEx::getBlack() const {
    JUtility::TColor black, white;
    if (!getBlackWhite(&black, &white)) {
        return JUtility::TColor(0);
    }

    return black;
}

JUtility::TColor J2DWindowEx::getWhite() const {
    JUtility::TColor black, white;
    if (!getBlackWhite(&black, &white)) {
        return JUtility::TColor(0xffffffff);
    }

    return white;
}

void J2DWindowEx::setAlpha(u8 param_0) {
    mAlpha = param_0;

    for (int i = 0; i < 4; i++) {
        if (mFrameMaterial[i] != NULL) {
            if (mFrameMaterial[i]->getColorBlock()->getMatColor(0) != NULL) {
                mFrameMaterial[i]->getColorBlock()->getMatColor(0)->a = param_0;
            }
        }
    }

    if (mContentsMaterial != NULL) {
        if (mContentsMaterial->getColorBlock()->getMatColor(0) != 0) {
            mContentsMaterial->getColorBlock()->getMatColor(0)->a = param_0;
        }
    }
}

void J2DWindowEx::setCullBack(_GXCullMode param_0) {
    mCullMode = param_0;

    for (int i = 0; i < 4; i++) {
        if (mFrameMaterial[i] != NULL) {
            mFrameMaterial[i]->getColorBlock()->setCullMode(param_0);
        }
    }

    if (mContentsMaterial != NULL) {
        mContentsMaterial->getColorBlock()->setCullMode(param_0);
    }
    J2DPane::setCullBack(param_0);
}

void J2DWindowEx::rewriteAlpha() {
    if (mContentsMaterial != NULL) {
        mAlpha = mContentsMaterial->getColorBlock()->getMatColor(0)->a;
    }
}

JUTTexture* J2DWindowEx::getFrameTexture(u8 param_0, u8 param_1) const {
    J2DMaterial* frameMaterial = getFrameMaterial(param_0);
    if (frameMaterial != NULL && frameMaterial->getTevBlock() != NULL) {
        return frameMaterial->getTevBlock()->getTexture(param_1);
    }
    return NULL;
}

JUTTexture* J2DWindowEx::getContentsTexture(u8 param_0) const {
    J2DMaterial* frameMaterial = getContentsMaterial();
    if (frameMaterial != NULL && frameMaterial->getTevBlock() != NULL) {
        return frameMaterial->getTevBlock()->getTexture(param_0);
    }
    return NULL;
}

bool J2DWindowEx::isUsed(ResTIMG const* timg) {
    for (u8 i = 0; i < 4; i++) {
        if (mFrameMaterial[i] != NULL && mFrameMaterial[i]->getTevBlock() != NULL) {
            for (u32 j = 0; j < 8; j++) {
                JUTTexture* texture = mFrameMaterial[i]->getTevBlock()->getTexture(j);
                if (texture != NULL && texture->getTexInfo() == timg) {
                    return true;
                }
            }
        }
    }

    if (mContentsMaterial != NULL && mContentsMaterial->getTevBlock() != NULL) {
        for (u32 j = 0; j < 8; j++) {
            JUTTexture* texture = mContentsMaterial->getTevBlock()->getTexture(j);
            if (texture != NULL && texture->getTexInfo() == timg) {
                return true;
            }
        }
    }

    return J2DPane::isUsed(timg);
}

void J2DWindowEx::setAnimation(J2DAnmColor* anmColor) {
    for (u8 i = 0; i < 4; i++) {
        if (isNeedSetAnm(i)) {
            mFrameMaterial[i]->setAnimation(anmColor);
        }
    }
    if (mContentsMaterial != NULL) {
        mContentsMaterial->setAnimation(anmColor);
    }
}

void J2DWindowEx::setAnimation(J2DAnmTextureSRTKey* param_0) {
    for (u8 i = 0; i < 4; i++) {
        if (isNeedSetAnm(i)) {
            mFrameMaterial[i]->setAnimation(param_0);
        }
    }
    if (mContentsMaterial != NULL) {
        mContentsMaterial->setAnimation(param_0);
    }
}

void J2DWindowEx::setAnimation(J2DAnmTexPattern* param_0) {
    for (u8 i = 0; i < 4; i++) {
        if (isNeedSetAnm(i)) {
            mFrameMaterial[i]->setAnimation(param_0);
        }
    }
    if (mContentsMaterial != NULL) {
        mContentsMaterial->setAnimation(param_0);
    }
}

void J2DWindowEx::setAnimation(J2DAnmTevRegKey* param_0) {
    for (u8 i = 0; i < 4; i++) {
        if (isNeedSetAnm(i)) {
            mFrameMaterial[i]->setAnimation(param_0);
        }
    }
    if (mContentsMaterial != NULL) {
        mContentsMaterial->setAnimation(param_0);
    }
}

bool J2DWindowEx::isNeedSetAnm(u8 param_0) {
    for (u8 i = 0; i < param_0; i++) {
        if (mFrameMaterial[i] == mFrameMaterial[param_0]) {
            return false;
        }
    }
    return mFrameMaterial[param_0] != NULL;
}

void J2DWindowEx::setAnimation(J2DAnmVisibilityFull* param_0) {
    mAnmVisibilityFull = param_0;
}

void J2DWindowEx::setAnimation(J2DAnmVtxColor* param_0) {
    mAnmVtxColor = param_0;
    field_0x17c = 0;
    if (param_0 != NULL) {
        u16 uVar3 = param_0->getAnmTableNum(0);
        for (u8 i = 0; i < 4; i++) {
            if (field_0x168[i] != 0xffff) {
                for (u16 j = 0; j < uVar3; j++) {
                    J3DAnmVtxColorIndexData* puVar1 = param_0->getAnmVtxColorIndexData(0, j);
                    u16* indexPointer = param_0->getVtxColorIndexPointer(0);
                    u16* indexPointer2 = indexPointer + (uintptr_t)puVar1->mpData;
                    for (u16 k = 0; k < puVar1->mNum; k++) {
                        if (indexPointer2[k] == field_0x168[i]) {
                            mAnmVtxColor = param_0;
                            field_0x17c |= 1 << i;
                            goto nexti;
                        }
                    }
                }
            }
        nexti:;
        }
    }

    if (field_0x17c == 0) {
        mAnmVtxColor = NULL;
    }
}

const J2DAnmTransform* J2DWindowEx::animationPane(J2DAnmTransform const* param_0) {
    if (mAnmVisibilityFull != 0 && field_0x166 != 0xffff) {
        u8 visibility;
        mAnmVisibilityFull->getVisibility(field_0x166, &visibility);
        if (visibility) {
            J2DPane::show();
        } else {
            J2DPane::hide();
        }
    }

    JUtility::TColor* local_38[4] = {NULL};
    local_38[0] = &field_0x128;
    local_38[1] = &field_0x12C;
    local_38[2] = &field_0x130;
    local_38[3] = &field_0x134;
    if (mAnmVtxColor != NULL) {
        u16 uVar3 = mAnmVtxColor->getAnmTableNum(0);
        for (u8 i = 0; i < 4; i++) {
            if ((field_0x17c & (1 << i))) {
                for (u16 j = 0; j < uVar3; j++) {
                    J3DAnmVtxColorIndexData* puVar1 = mAnmVtxColor->getAnmVtxColorIndexData(0, j);
                    u16* indexPointer = mAnmVtxColor->getVtxColorIndexPointer(0);
                    u16* indexPointer2 = indexPointer + (uintptr_t)puVar1->mpData;
                    for (u16 k = 0; k < puVar1->mNum; k++) {
                        if (indexPointer2[k] == field_0x168[i]) {
                            mAnmVtxColor->getColor(0, j, local_38[i]);
                            goto nexti;
                        }
                    }
                }
            }
            nexti:;
        }
    }
    return J2DPane::animationPane(param_0);
}

J2DMaterial* J2DWindowEx::getFrameMaterial(u8 index) const {
    if (index >= 4) {
        return NULL;
    }

    return mFrameMaterial[index];
}

J2DMaterial* J2DWindowEx::getContentsMaterial() const {
    return mContentsMaterial;
}

void J2DWindowEx::getMaterial(J2DWindow::TMaterial& param_0) const {
    param_0.field_0x0 = mFrameMaterial[0];
    param_0.field_0x4 = mFrameMaterial[1];
    param_0.field_0x8 = mFrameMaterial[2];
    param_0.field_0xc = mFrameMaterial[3];
    param_0.field_0x10 = mContentsMaterial;
}

void J2DWindowEx::draw(f32 param_0, f32 param_1, f32 param_2, f32 param_3) {
    draw(JGeometry::TBox2<f32>(JGeometry::TVec2<f32>(param_0, param_1),
                               JGeometry::TVec2<f32>(param_0 + param_2, param_1 + param_3)));
}

void J2DWindowEx::setCullBack(bool param_0) {
    _GXCullMode mode;
    if (param_0) {
        mode = GX_CULL_BACK;
    } else {
        mode = GX_CULL_NONE;
    }
    setCullBack(mode);
}

bool J2DWindowEx::isUsed(ResFONT const* param_0) {
    return J2DPane::isUsed(param_0);
}

void J2DWindowEx::setAnimation(J2DAnmBase* param_0) {
    J2DPane::setAnimation(param_0);
}
