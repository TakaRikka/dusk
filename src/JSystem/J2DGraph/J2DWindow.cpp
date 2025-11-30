#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J2DGraph/J2DWindow.h"
#include "JSystem/JSupport/JSURandomInputStream.h"
#include "JSystem/JUtility/JUTPalette.h"
#include "JSystem/JUtility/JUTResource.h"
#include "JSystem/JUtility/JUTTexture.h"

J2DWindow::J2DWindow()
    : field_0x100(NULL), field_0x104(NULL), field_0x108(NULL), field_0x10c(NULL), field_0x110(NULL),
      mPalette(NULL) {
    field_0x144 = 0;
    setContentsColor(JUtility::TColor(0xffffffff));
    mBlack = JUtility::TColor(0);
    mWhite = JUtility::TColor(0xffffffff);
}

J2DWindow::J2DWindow(J2DPane* param_0, JSURandomInputStream* param_1, JKRArchive* param_2)
    : field_0x100(NULL), field_0x104(NULL), field_0x108(NULL), field_0x10c(NULL), field_0x110(NULL),
      mPalette(NULL) {
    private_readStream(param_0, param_1, param_2);
}

struct J2DWindowData {
    u8 field_0x0[0x10];
    u16 field_0x10[4];
    u8 field_0x18;
    u8 field_0x19;
    u16 field_0x1a;
    u16 field_0x1c;
    u16 field_0x1e;
    u16 field_0x20;
    u16 field_0x22;
    u16 field_0x24;
    u8 field_0x26[0xa];
    u32 field_0x30[4];
};

J2DWindow::J2DWindow(J2DPane* param_0, JSURandomInputStream* param_1, J2DMaterial* param_2) : 
    field_0x100(NULL),
    field_0x104(NULL),
    field_0x108(NULL),
    field_0x10c(NULL),
    field_0x110(NULL),
    mPalette(NULL) {
    u32 auStack_78[2];
    u32 position = param_1->getPosition();
    param_1->read(auStack_78, 8);
    mKind = 'WIN1';
    u32 local_dc = param_1->getPosition();
    int auStack_b8[2];
    param_1->peek(auStack_b8, 8);
    makePaneExStream(param_0, param_1);
    param_1->seek(local_dc + auStack_b8[1], JSUStreamSeekFrom_SET);
    J2DWindowData buffer;
    param_1->read(&buffer, sizeof(J2DWindowData));

    JUtility::TColor* colors[4] = {
        &field_0x128,
        &field_0x12C,
        &field_0x130,
        &field_0x134,
    };

    u16 local_c0[4];
    J2DMaterial *local_98[4];
    for (int iVar7 = 0; iVar7 < 4; iVar7++) {
        local_c0[iVar7] = buffer.field_0x10[iVar7];
        local_98[iVar7] = NULL;
        if (local_c0[iVar7] != 0xffff) {
            local_98[iVar7] = param_2 + local_c0[iVar7];
        }
        *colors[iVar7] = JUtility::TColor(buffer.field_0x30[iVar7]);
    }

    field_0x144 = buffer.field_0x18;
    field_0x114.set(
        buffer.field_0x1a,
        buffer.field_0x1c,
        buffer.field_0x1a + buffer.field_0x1e,
        buffer.field_0x1c + buffer.field_0x20);
    u16 temp = buffer.field_0x24;
    J2DMaterial* pJVar5 = NULL;
    if (temp != 0xffff) {
        pJVar5 = param_2 + temp;
    }

    param_1->seek(position + auStack_78[1], JSUStreamSeekFrom_SET);
    if (local_98[0] != NULL) {
        mAlpha = local_98[0]->getColorBlock()->getMatColor(0)->a;
    }

    mBlack = JUtility::TColor(0);
    mWhite = JUtility::TColor(0xffffffff);

    if (local_98[0] != NULL && local_98[0]->getTevBlock() != NULL) {
        if (local_98[0]->getTevBlock()->getTevStageNum() != 1) {
            J2DGXColorS10* local_c8 = local_98[0]->getTevBlock()->getTevColor(0);
            s16 blackr = local_c8->r;
            s16 blackg = local_c8->g;
            s16 blackb = local_c8->b;
            s16 blacka = local_c8->a;
            J2DGXColorS10* local_d0 = local_98[0]->getTevBlock()->getTevColor(1);
            s16 whiter = local_d0->r;
            s16 whiteg = local_d0->g;
            s16 whiteb = local_d0->b;
            s16 whitea = local_d0->a;
            mBlack = JUtility::TColor(
                (((u8)blackr) << 24) | (((u8)blackg) << 16) | (((u8)blackb) << 8) |
                 ((u8)blacka));
            mWhite = JUtility::TColor(
                (((u8)whiter) << 24) | (((u8)whiteg) << 16) | (((u8)whiteb) << 8) |
                 ((u8)whitea));
        }
    }

    JUTTexture** textures[4] = {
        &field_0x100,
        &field_0x104,
        &field_0x108,
        &field_0x10c,
    };

    J2DMaterial * mat;
    for (u32 uVar6 = 0; uVar6 < 4; uVar6++) {
        mat = local_98[uVar6];
        if (mat != NULL && mat->getTevBlock() != NULL) {
            JUTTexture* local_e0 = mat->getTevBlock()->getTexture(0);
            if (local_e0 != NULL) {
                *textures[uVar6] = local_e0;
                mat->getTevBlock()->setUndeleteFlag(0xfe);
            }
        }
    }

    field_0x110 = NULL;
    if (pJVar5 != NULL && pJVar5->getTevBlock() != NULL) {
        JUTTexture* local_e0 = pJVar5->getTevBlock()->getTexture(0);
        if (local_e0 != NULL) {
            field_0x110 = local_e0;
            pJVar5->getTevBlock()->setUndeleteFlag(0xfe);
        }
    }
    initinfo2();
}

void J2DWindow::private_readStream(J2DPane* param_0, JSURandomInputStream* param_1,
                                   JKRArchive* param_2) {
    s32 local_188 = param_1->getPosition();
    u32 local_180[2];
    param_1->read(local_180, 8);
    mKind = local_180[0];
    makePaneStream(param_0, param_1);
    JUTResReference stack_178;
    u8 r27 = param_1->readU8();
    f32 f31 = param_1->read16b();
    f32 f30 = param_1->read16b();
    f32 f29 = f31 + param_1->read16b();
    f32 f28 = f30 + param_1->read16b();
    field_0x114.set(f31, f30, f29, f28);
    ResTIMG* timg = (ResTIMG*)getPointer(param_1, 'TIMG', param_2);
    if (timg) {
        field_0x100 = new JUTTexture(timg, 0);
    }
    timg = (ResTIMG*)getPointer(param_1, 'TIMG', param_2);
    if (timg) {
        field_0x104 = new JUTTexture(timg, 0);
    }
    timg = (ResTIMG*)getPointer(param_1, 'TIMG', param_2);
    if (timg) {
        field_0x108 = new JUTTexture(timg, 0);
    }
    timg = (ResTIMG*)getPointer(param_1, 'TIMG', param_2);
    if (timg) {
        field_0x10c = new JUTTexture(timg, 0);
    }
    ResTLUT* tlut = (ResTLUT*)getPointer(param_1, 'TLUT', param_2);
    if (tlut) {
        mPalette = new JUTPalette(GX_TLUT0, tlut);
    }
    field_0x144 = param_1->read8b();
    field_0x128.set(param_1->read32b());
    field_0x12C.set(param_1->read32b());
    field_0x130.set(param_1->read32b());
    field_0x134.set(param_1->read32b());
    r27 -= 14;
    field_0x110 = NULL;
    if (r27) {
        timg = (ResTIMG*)getPointer(param_1, 'TIMG', param_2);
        if (timg) {
            field_0x110 = new JUTTexture(timg, 0);
        }
        r27--;
    }
    mBlack = JUtility::TColor(0);
    mWhite = JUtility::TColor(0xffffffff);
    if (r27) {
        mBlack = JUtility::TColor(param_1->readU32());
        r27--;
    }
    if (r27) {
        mWhite = JUtility::TColor(param_1->readU32());
    }
    param_1->seek(local_188 + local_180[1], JSUStreamSeekFrom_SET);
    initinfo2();
}

void J2DWindow::initinfo2() {
    if (field_0x100 && field_0x104 && field_0x108 && field_0x10c) {
        field_0x140 = field_0x100->getWidth() + field_0x104->getWidth();
        field_0x142 = field_0x100->getHeight() + field_0x108->getHeight();
    } else {
        field_0x140 = 1;
        field_0x142 = 1;
        return;
    }
    field_0x145 = 0;
    JUTTexture* r30 = field_0x100;
    if (*field_0x104 != *r30) {
        field_0x145 |= 1;
        r30 = field_0x104;
    }
    if (*field_0x10c != *r30) {
        field_0x145 |= 2;
        r30 = field_0x10c;
    }
    if (*field_0x108 != *r30) {
        field_0x145 |= 4;
    }
}

J2DWindow::~J2DWindow() {
    delete field_0x100;
    delete field_0x104;
    delete field_0x108;
    delete field_0x10c;
    delete mPalette;
    delete field_0x110;
}

void J2DWindow::draw(JGeometry::TBox2<f32> const& param_0) {
    JGeometry::TBox2<f32> stack_58;
    if (field_0x100 && field_0x104 && field_0x108 && field_0x10c) {
        stack_58.set(field_0x100->getWidth(), field_0x100->getHeight(),
                     param_0.getWidth() - field_0x104->getWidth(),
                     param_0.getHeight() - field_0x108->getHeight());
    } else {
        stack_58.set(0.0f, 0.0f, param_0.getWidth(), param_0.getHeight());
    }
    draw(param_0, stack_58);
}

void J2DWindow::draw_private(JGeometry::TBox2<f32> const& param_0,
                             JGeometry::TBox2<f32> const& param_1) {
    JGeometry::TBox2<f32> stack_a0(param_1);
    stack_a0.addPos(param_0.i);
    drawContents(stack_a0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    GXSetNumTexGens(1);
    if (field_0x100 && field_0x104 && field_0x108 && field_0x10c) {
        f32 f29 = param_0.i.x;
        f32 f28 = param_0.i.y;
        f32 f31 = param_0.f.x - field_0x10c->getWidth();
        f32 f30 = param_0.f.y - field_0x10c->getHeight();
        f32 f27 = f29 + field_0x100->getWidth();
        f32 f26 = f28 + field_0x100->getHeight();
        drawFrameTexture(field_0x100, f29, f28, field_0x144 & 0x80, field_0x144 & 0x40, true);
        bool r24 = isField0x145Set(1);
        drawFrameTexture(field_0x104, f31, f28, field_0x144 & 0x20, field_0x144 & 0x10, r24);
        u16 local_a6;
        if (field_0x144 & 0x20) {
            local_a6 = 0x8000;
        } else {
            local_a6 = 0;
        }
        u16 local_a8;
        if (field_0x144 & 0x10) {
            local_a8 = 0;
        } else {
            local_a8 = 0x8000;
        }
        u16 local_a82 = local_a8 ^ 0x8000;
        
        drawFrameTexture(field_0x104, f27, f28, f31 - f27, field_0x104->getHeight(), local_a6,
                         local_a8, local_a6, local_a82, false);
        bool isset2 = isField0x145Set(2);
        drawFrameTexture(field_0x10c, f31, f30, field_0x144 & 2, field_0x144 & 1, isset2);
        u16 local_aa;
        if (field_0x144 & 2) {
            local_aa = 0x8000;
        } else {
            local_aa = 0;
        }
        u16 local_ac;
        if (field_0x144 & 1) {
            local_ac = 0;
        } else {
            local_ac = 0x8000;
        }
        local_a82 = local_ac ^ 0x8000;
        drawFrameTexture(field_0x10c, f27, f30, f31 - f27, field_0x10c->getHeight(), local_aa,
                         local_ac, local_aa, local_a82, false);
        u16 local_ae;
        if (field_0x144 & 2) {
            local_ae = 0;
        } else {
            local_ae = 0x8000;
        }
        u16 local_ae2 = local_ae ^ 0x8000;
        u16 local_b0;
        if (field_0x144 & 1) {
            local_b0 = 0x8000;
        } else {
            local_b0 = 0;
        }
        drawFrameTexture(field_0x10c, f31, f26, field_0x10c->getWidth(), f30 - f26, local_ae,
                         local_b0, local_ae2, local_b0, false);
        bool isset4 = isField0x145Set(4);
        drawFrameTexture(field_0x108, f29, f30, field_0x144 & 8, field_0x144 & 4, isset4);

        u16 local_b2;
        if (field_0x144 & 8) {
            local_b2 = 0;
        } else {
            local_b2 = 0x8000;
        }
        u16 local_b22 = local_b2 ^ 0x8000;
        u16 local_b4;
        if (field_0x144 & 4) {
            local_b4 = 0x8000;
        } else {
            local_b4 = 0;
        }
        drawFrameTexture(field_0x108, f29, f26, field_0x108->getWidth(), f30 - f26, local_b2,
                         local_b4, local_b22, local_b4, false);
    }
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetNumTexGens(0);
    GXSetVtxDesc(GX_VA_TEX0, GX_NONE);
}

void J2DWindow::draw(JGeometry::TBox2<f32> const& param_0, JGeometry::TBox2<f32> const& param_1) {
    if (param_0.getWidth() >= field_0x140 && param_0.getHeight() >= field_0x142 && isVisible()) {
        makeMatrix(param_0.i.x, param_0.i.y, 0, 0);
        GXLoadPosMtxImm(mPositionMtx, GX_PNMTX0);
        GXSetCurrentMtx(0);
        mColorAlpha = mAlpha;
        JGeometry::TBox2<f32> stack_70(0.0f, 0.0f, param_0.getWidth(), param_0.getHeight());
        draw_private(stack_70, param_1);
        Mtx stack_60;
        MTXIdentity(stack_60);
        GXLoadPosMtxImm(stack_60, GX_PNMTX0);
    }
}

void J2DWindow::resize(f32 param_0, f32 param_1) {
    f32 f31 = getWidth();
    f32 f30 = getHeight();
    J2DPane::resize(param_0, param_1);
    field_0x114.f.x += param_0 - f31;
    field_0x114.f.y += param_1 - f30;
    for (JSUTreeIterator<J2DPane> it(getFirstChild()); it != getEndChild(); it++) {
        if (it->getTypeID() == 19 && it->isConnectParent()) {
            f32 f29 = param_0 - f31 + it->getWidth();
            f32 f28 = param_1 - f30 + it->getHeight();
            it->J2DPane::resize(f29, f28);
        }
    }
}

/* 802FB12C-802FB1D8 2F5A6C 00AC+00 1/1 0/0 0/0 .text
 * setContentsColor__9J2DWindowFQ28JUtility6TColorQ28JUtility6TColorQ28JUtility6TColorQ28JUtility6TColor
 */
void J2DWindow::setContentsColor(JUtility::TColor param_0, JUtility::TColor param_1,
                                 JUtility::TColor param_2, JUtility::TColor param_3) {
    field_0x128.set(param_0);
    field_0x12C.set(param_1);
    field_0x130.set(param_2);
    field_0x134.set(param_3);
}

void J2DWindow::drawSelf(f32 param_0, f32 param_1) {
    Mtx stack_38;
    MTXIdentity(stack_38);
    drawSelf(param_0, param_1, &stack_38);
}

void J2DWindow::drawSelf(f32 param_0, f32 param_1, Mtx* param_2) {
    JGeometry::TBox2<f32> stack_50(mBounds);
    stack_50.addPos(JGeometry::TVec2<f32>(param_0, param_1));
    if (stack_50.getWidth() >= field_0x140 && stack_50.getHeight() >= field_0x142) {
        Mtx stack_40;
        MTXConcat(*param_2, mGlobalMtx, stack_40);
        GXLoadPosMtxImm(stack_40, GX_PNMTX0);
        draw_private(stack_50, field_0x114);
    }
    clip(field_0x114);
}

void J2DWindow::drawContents(JGeometry::TBox2<f32> const& param_0) {
    if (param_0.isValid()) {
        GXSetNumChans(1);
        GXSetNumTexGens(0);
        GXSetNumTevStages(1);
        GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        if ((field_0x128 & 0xff) == 0xff && (field_0x12C & 0xff) == 0xff &&
            (field_0x130 & 0xff) == 0xff && (field_0x134 & 0xff) == 0xff && mColorAlpha == 0xff)
        {
            GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_SET);
        } else {
            GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
        }
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE,
                      GX_AF_NONE);
        GXSetNumIndStages(0);
        for (int i = 0; i < 16; i++) {
            GXSetTevDirect(GXTevStageID(i));
        }
        if (field_0x110 == NULL) {
            JUtility::TColor color1(field_0x128);
            JUtility::TColor color2(field_0x130);
            JUtility::TColor color3(field_0x12C);
            JUtility::TColor color4(field_0x134);
            if (mColorAlpha != 0xff) {
                color1.a = color1.a * mColorAlpha / 0xff;
                color2.a = color2.a * mColorAlpha / 0xff;
                color3.a = color3.a * mColorAlpha / 0xff;
                color4.a = color4.a * mColorAlpha / 0xff;
            }
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(param_0.i.x, param_0.i.y, 0.0f);
            GXColor1u32(color1);
            GXPosition3f32(param_0.f.x, param_0.i.y, 0.0f);
            GXColor1u32(color3);
            GXPosition3f32(param_0.f.x, param_0.f.y, 0.0f);
            GXColor1u32(color4);
            GXPosition3f32(param_0.i.x, param_0.f.y, 0.0f);
            GXColor1u32(color2);
            GXEnd();
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
        } else {
            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
            GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
            GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
            GXSetNumTexGens(1);
            drawContentsTexture(param_0.i.x, param_0.i.y, param_0.getWidth(), param_0.getHeight());
        }
    }
}

void J2DWindow::drawFrameTexture(JUTTexture* param_0, f32 param_1, f32 param_2, f32 param_3,
                                 f32 param_4, u16 param_5, u16 param_6, u16 param_7, u16 param_8,
                                 bool param_9) {
    f32 f31 = param_1 + param_3;
    f32 f30 = param_2 + param_4;
    if (param_9) {
        param_0->load(GX_TEXMAP0);
        setTevMode(param_0, mBlack, mWhite);
    }
    JUtility::TColor stack_64(mColorAlpha | 0xffffff00);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(param_1, param_2, 0.0f);
    GXColor1u32(stack_64);
    GXTexCoord2u16(param_7, param_8);
    GXPosition3f32(f31, param_2, 0.0f);
    GXColor1u32(stack_64);
    GXTexCoord2u16(param_5, param_8);
    GXPosition3f32(f31, f30, 0.0f);
    GXColor1u32(stack_64);
    GXTexCoord2u16(param_5, param_6);
    GXPosition3f32(param_1, f30, 0.0f);
    GXColor1u32(stack_64);
    GXTexCoord2u16(param_7, param_6);
    GXEnd();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
}

void J2DWindow::drawFrameTexture(JUTTexture* param_0, f32 param_1, f32 param_2, bool param_3,
                                 bool param_4, bool param_5) {
    u16 r31 = param_4 ? u16(0x8000) : u16(0);
    u16 r30 = param_3 ? u16(0x8000) : u16(0);
    u16 r29 = param_4 ? u16(0) : u16(0x8000);
    u16 r28 = param_3 ? u16(0) : u16(0x8000);
    drawFrameTexture(param_0, param_1, param_2, param_0->getWidth(), param_0->getHeight(), r28, r29,
                     r30, r31, param_5);
}

void J2DWindow::drawContentsTexture(f32 param_0, f32 param_1, f32 param_2, f32 param_3) {
    f32 f29 = param_0 + param_2;
    f32 f28 = param_1 + param_3;
    f32 width = field_0x110->getWidth();
    f32 height = field_0x110->getHeight();
    f32 f27 = -(param_2 / width - 1.0f) / 2.0f;
    f32 f26 = -(param_3 / height - 1.0f) / 2.0f;
    f32 f25 = f27 + param_2 / width;
    f32 f24 = f26 + param_3 / height;
    TContentsColor stack_f0;
    getContentsColor(stack_f0);
    if (mColorAlpha != 0xff) {
        stack_f0.field_0x0.a = stack_f0.field_0x0.a * mColorAlpha / 0xff;
        stack_f0.field_0x4.a = stack_f0.field_0x4.a * mColorAlpha / 0xff;
        stack_f0.field_0x8.a = stack_f0.field_0x8.a * mColorAlpha / 0xff;
        stack_f0.field_0xc.a = stack_f0.field_0xc.a * mColorAlpha / 0xff;
    }
    field_0x110->load(GX_TEXMAP0);
    setTevMode(field_0x110, 0, 0xffffffff);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBA6, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(param_0, param_1, 0.0f);
    GXColor1u32(stack_f0.field_0x0);
    GXTexCoord2f32(f27, f26);
    GXPosition3f32(f29, param_1, 0.0f);
    GXColor1u32(stack_f0.field_0x4);
    GXTexCoord2f32(f25, f26);
    GXPosition3f32(f29, f28, 0.0f);
    GXColor1u32(stack_f0.field_0xc);
    GXTexCoord2f32(f25, f24);
    GXPosition3f32(param_0, f28, 0.0f);
    GXColor1u32(stack_f0.field_0x8);
    GXTexCoord2f32(f27, f24);
    GXEnd();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_CLR_RGBA, GX_RGBX8, 0xf);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_S16, 0);
}

void J2DWindow::setTevMode(JUTTexture* param_0, JUtility::TColor param_1,
                           JUtility::TColor param_2) {
    if (param_1 == 0 && param_2 == 0xffffffff) {
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
        if (param_0->getTransparency()) {
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
        } else {
            GXSetTevColor(GX_TEVREG2, JUtility::TColor(0xffffffff));
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_A2, GX_CA_RASA, GX_CA_ZERO);
        }
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetNumTevStages(1);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    } else {
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColor(GX_TEVREG0, param_1);
        GXSetTevColor(GX_TEVREG1, param_2);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
        if (param_0->getTransparency()) {
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_A1, GX_CA_TEXA, GX_CA_ZERO);
        } else {
            GXSetTevColor(GX_TEVREG2, JUtility::TColor(0xffffffff));
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_A1, GX_CA_A2, GX_CA_ZERO);
        }
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
        GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetNumTevStages(2);
    }
    GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
}

JUTTexture* J2DWindow::getFrameTexture(u8 param_0, u8 param_1) const {
    JUTTexture* tmp[4] = {NULL, NULL, NULL, NULL};
    tmp[0] = field_0x100;
    tmp[1] = field_0x104;
    tmp[2] = field_0x108;
    tmp[3] = field_0x10c;
    if (param_0 >= 4 || param_1 != 0) {
        return NULL;
    }
    return tmp[param_0];
}

bool J2DWindow::isUsed(ResTIMG const* param_0) {
    JUTTexture* tmp[5] = {NULL, NULL, NULL, NULL, NULL};
    tmp[0] = field_0x100;
    tmp[1] = field_0x104;
    tmp[2] = field_0x108;
    tmp[3] = field_0x10c;
    tmp[4] = field_0x110;
    for (u8 i = 0; i < 5; i++) {
        if (tmp[i] && tmp[i]->getTexInfo() == param_0) {
            return true;
        }
    }
    return J2DPane::isUsed(param_0);
}

u16 J2DWindow::getTypeID() const {
    return 17;
}

void J2DWindow::draw(f32 param_0, f32 param_1, f32 param_2, f32 param_3) {
    draw(JGeometry::TBox2<f32>(param_0, param_1, param_0 + param_2, param_1 + param_3));
}

JUTTexture* J2DWindow::getContentsTexture(u8 param_0) const {
    if (param_0 != 0) {
        return NULL;
    }
    return field_0x110;
}

void J2DWindow::getMaterial(J2DWindow::TMaterial& param_0) const {
    param_0.field_0x0 = NULL;
    param_0.field_0x4 = NULL;
    param_0.field_0x8 = NULL;
    param_0.field_0xc = NULL;
    param_0.field_0x10 = NULL;
}

J2DMaterial* J2DWindow::getFrameMaterial(u8 param_0) const {
    return NULL;
}

J2DMaterial* J2DWindow::getContentsMaterial() const {
    return NULL;
}

bool J2DWindow::isUsed(ResFONT const* param_0) {
    return J2DPane::isUsed(param_0);
}

void J2DWindow::rewriteAlpha() {
    /* empty function */
}
