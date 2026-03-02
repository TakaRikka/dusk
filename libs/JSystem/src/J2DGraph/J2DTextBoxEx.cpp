//
// J2DTextBoxEx
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J2DGraph/J2DTextBoxEx.h"
#include "JSystem/J2DGraph/J2DPrint.h"
#include "JSystem/JSupport/JSURandomInputStream.h"
#include "JSystem/JUtility/JUTResFont.h"

J2DTextBoxEx::J2DTextBoxEx(J2DPane* p_pane, JSURandomInputStream* p_stream, u32 param_2,
                               J2DMaterial* p_material) {
    J2DTextBoxInfo info;

    mVisibilityAnm = NULL;

    int startPos = p_stream->getPosition();

    J2DTbxBlockHeader header;
    p_stream->read(&header, 8);
    mKind = header.mTag;

    int panHeaderPos = p_stream->getPosition();
    J2DTbxBlockHeader panHeader;
    p_stream->peek(&panHeader, 8);
    makePaneExStream(p_pane, p_stream);
    p_stream->seek(panHeaderPos + panHeader.mSize, JSUStreamSeekFrom_SET);

    p_stream->read(&info, 0x20);
    field_0x13c = info.field_0x2;
    field_0x13e = info.mMaterialNum;
    mMaterial = NULL;
    
    if (field_0x13e != 0xFFFF) {
        mMaterial = &p_material[field_0x13e];
        p_material[field_0x13e].field_0x4 = this;
        rewriteAlpha();

        if (mMaterial != NULL) {
            if (mMaterial->getTevBlock() != NULL) {
                mFont = mMaterial->getTevBlock()->getFont();
            }
        }
    }

    mCharSpacing = info.mCharSpace;
    mLineSpacing = info.mLineSpace;
    mFontSizeX = info.mFontSizeX;
    mFontSizeY = info.mFontSizeY;
    mFlags = (info.mHBind << 2) | info.mVBind;
    mCharColor = JUtility::TColor(info.mCharColor);
    mGradientColor = JUtility::TColor(info.mGradColor);
    setConnectParent(info.mConnected);

    u16 strLength = 0;
    if (!(param_2 & 0x2000000)) {
        strLength = info.field_0x1c;
        if ((s16)info.field_0x1c == -1) {
            strLength = info.field_0x1e + 1;
        }
    }

    mStringLength = 0;
    mStringPtr = NULL;

    if (strLength != 0) {
        mStringPtr = new char[strLength];
    }

    if (mStringPtr != NULL) {
        mStringLength = strLength;

        u16 var_r26_2 = strLength - 1 < info.field_0x1e ? u16(strLength - 1) : info.field_0x1e;

        p_stream->peek(mStringPtr, var_r26_2);
        mStringPtr[var_r26_2] = 0;
    }

    p_stream->skip(info.field_0x1e);
    p_stream->seek(startPos + header.mSize, JSUStreamSeekFrom_SET);

    field_0x10c = 0.0f;
    field_0x110 = 0.0f;
    mTextFontOwned = false;
    field_0x140 = 0;
}

J2DTextBoxEx::~J2DTextBoxEx() {
    if (field_0x140 != 0) {
        delete mMaterial;
    }
}

void J2DTextBoxEx::drawSelf(f32 param_0, f32 param_1, Mtx* p_mtx) {
    Mtx m;

    JUTFont* font = NULL;
    if (mMaterial != NULL && mMaterial->getTevBlock() != NULL) {
        font = mMaterial->getTevBlock()->getFont();
    }

    J2DPrint print(font, (int)mCharSpacing, (int)mLineSpacing, mCharColor, mGradientColor, mBlackColor,
                   mWhiteColor);
    print.setFontSize((int)mFontSizeX, (int)mFontSizeY);

    if (mMaterial != NULL) {
        mMaterial->setGX();
        MTXConcat(*p_mtx, mGlobalMtx, m);

        GXLoadPosMtxImm(m, GX_PNMTX0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

        print.locate(param_0 + mBounds.i.x, param_1 + mBounds.i.y);

        if (mMaterial->isVisible() && mStringPtr != NULL) {
            u8 alpha = 255;

            if (mMaterial->getColorBlock()->getColorChan(1)->getMatSrc() == 1) {
                if (mMaterial->getMaterialAlphaCalc() == 1) {
                    alpha = mColorAlpha;
                }
            } else if (mIsInfluencedAlpha) {
                GXSetChanMatColor(GX_ALPHA0, JUtility::TColor(mColorAlpha));
            }

            print.printReturn(mStringPtr, (int)(mBounds.getWidth() + 0.0001f), (int)mBounds.getHeight(),
                            getHBinding(), getVBinding(), field_0x10c, field_0x110, alpha);
        }
    }
}

void J2DTextBoxEx::draw(f32 posX, f32 posY) {
    Mtx m;

    if (isVisible()) {
        JUTFont* font = NULL;
        if (mMaterial != NULL && mMaterial->getTevBlock() != NULL) {
            font = mMaterial->getTevBlock()->getFont();
        }
        
        J2DPrint print(font, mCharSpacing, mLineSpacing, mCharColor, mGradientColor, mBlackColor,
                       mWhiteColor);
        print.setFontSize(mFontSizeX, mFontSizeY);
        mColorAlpha = mAlpha;

        if (mMaterial != NULL) {
            mMaterial->setGX();
            makeMatrix(posX, posY, 0.0f, 0.0f);

            GXLoadPosMtxImm(mPositionMtx, GX_PNMTX0);
            GXSetCurrentMtx(0);

            if (!mMaterial->isVisible()) {
                return;
            }

            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
            GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
            GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

            if (mStringPtr != NULL) {
                u8 alpha = 255;
                if (mMaterial->getMaterialAlphaCalc() == 1) {
                    alpha = mColorAlpha;
                }
                
                print.print(0.0f, 0.0f, alpha, "%s", mStringPtr);
            }

            for (int i = 0; i < 4; i++) {
                GXSetTevSwapModeTable((GXTevSwapSel)i, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
            }

            GXSetNumIndStages(0);

            for (int i = 0; i < 16; i++) {
                GXSetTevDirect((GXTevStageID)i);
            }

            MTXIdentity(m);
            GXLoadPosMtxImm(m, 0);
        }
    }
}

void J2DTextBoxEx::draw(f32 posX, f32 posY, f32 param_2, J2DTextBoxHBinding hBind) {
    Mtx m;

    if (isVisible()) {
        JUTFont* font = NULL;
        if (mMaterial != NULL && mMaterial->getTevBlock() != NULL) {
            font = mMaterial->getTevBlock()->getFont();
        }
        
        J2DPrint print(font, mCharSpacing, mLineSpacing, mCharColor, mGradientColor, mBlackColor,
                       mWhiteColor);
        print.setFontSize(mFontSizeX, mFontSizeY);
        mColorAlpha = mAlpha;

        if (mMaterial != NULL) {
            mMaterial->setGX();
            makeMatrix(posX, posY, 0.0f, 0.0f);

            GXLoadPosMtxImm(mPositionMtx, GX_PNMTX0);
            GXSetCurrentMtx(0);

            if (!mMaterial->isVisible()) {
                return;
            }

            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
            GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
            GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

            if (mStringPtr != NULL) {
                u8 alpha = 255;
                if (mMaterial->getMaterialAlphaCalc() == 1) {
                    alpha = mColorAlpha;
                }
                
                print.printReturn(mStringPtr, param_2, 0.0f, hBind, VBIND_TOP, 0.0f, -mFontSizeY,
                                alpha);
            }

            for (int i = 0; i < 4; i++) {
                GXSetTevSwapModeTable((GXTevSwapSel)i, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
            }

            GXSetNumIndStages(0);

            for (int i = 0; i < 16; i++) {
                GXSetTevDirect((GXTevStageID)i);
            }

            MTXIdentity(m);
            GXLoadPosMtxImm(m, 0);
        }
    }
}

void J2DTextBoxEx::setFont(JUTFont* p_font) {
    if (p_font != NULL && mMaterial != NULL && mMaterial->getTevBlock() != NULL) {
        mMaterial->getTevBlock()->setFont(p_font);
        mFont = p_font;
    }
}

JUTFont* J2DTextBoxEx::getFont() const {
    if (mMaterial != NULL && mMaterial->getTevBlock() != NULL) {
        return mMaterial->getTevBlock()->getFont();
    }

    return NULL;
}

void J2DTextBoxEx::setTevOrder(bool param_0) {
    u16 local_18[2];
    if (!param_0) {
        local_18[0] = 4;
        local_18[1] = 0xffff;
    } else {
        local_18[0] = 0xff;
        local_18[1] = 0xff04;
    }
    for (u8 i = 0; i < 2; i++) {
        if (mMaterial->getTevBlock()->getMaxStage() > i) {
            J2DTevOrderInfo info;
            info.mTexCoord = (local_18[i]) >> 8;
            info.mTexMap = (local_18[i]) >> 8;
            info.mColor = local_18[i] & 0xff;
            J2DTevOrder tevOrder = info;
            mMaterial->getTevBlock()->setTevOrder(i, tevOrder);
        }
    }
}

void J2DTextBoxEx::setTevStage(bool param_0) {
    J2DTevStage* stage = mMaterial->getTevBlock()->getTevStage(0);

    if (!param_0) {
        setStage(stage, STAGE_0);
    } else {
        setStage(stage, STAGE_1);
        stage = mMaterial->getTevBlock()->getTevStage(1);
        setStage(stage, STAGE_2);
    }
}

void J2DTextBoxEx::setStage(J2DTevStage* param_0, J2DTextBoxEx::stage_enum param_1) {
    const u8 tevColors[3][4] = {
        {0x0F, 0x08, 0x0A, 0x0F}, {0x02, 0x04, 0x08, 0x0F}, {0x0F, 0x0A, 0x00, 0x0F},
    };
    const u8 tevAlpha[3][4] = {
        {0x07, 0x04, 0x05, 0x07}, {0x01, 0x02, 0x04, 0x07}, {0x07, 0x05, 0x00, 0x07},
    };

    const u8 tevColorOps[3][5] = {
        {0x00, 0x00, 0x00, 0x01, 0x00},
        {0x00, 0x00, 0x00, 0x01, 0x00},
        {0x00, 0x00, 0x00, 0x01, 0x00},
    };

    const u8 tevAlphaOps[3][5] = {
        {0x00, 0x00, 0x00, 0x01, 0x00},
        {0x00, 0x00, 0x00, 0x01, 0x00},
        {0x00, 0x00, 0x00, 0x01, 0x00},
    };

    param_0->setTevColorAB(tevColors[param_1][0], tevColors[param_1][1]);
    param_0->setTevColorCD(tevColors[param_1][2], tevColors[param_1][3]);
    param_0->setTevColorOp(
        tevColorOps[param_1][0], tevColorOps[param_1][1], tevColorOps[param_1][2], tevColorOps[param_1][3], tevColorOps[param_1][4]);
    param_0->setAlphaABCD(
        tevAlpha[param_1][0], tevAlpha[param_1][1], tevAlpha[param_1][2], tevAlpha[param_1][3]);
    param_0->setTevAlphaOp(
        tevAlphaOps[param_1][0], tevAlphaOps[param_1][1], tevAlphaOps[param_1][2], tevAlphaOps[param_1][3], tevAlphaOps[param_1][4]);
}

bool J2DTextBoxEx::setBlack(JUtility::TColor black) {
    JUtility::TColor tevBlack;
    JUtility::TColor tevWhite;

    if (!getBlackWhite(&tevBlack, &tevWhite)) {
        return false;
    } else {
        return setBlackWhite(black, tevWhite);
    }
}

bool J2DTextBoxEx::setWhite(JUtility::TColor white) {
    JUtility::TColor tevBlack;
    JUtility::TColor tevWhite;

    if (!getBlackWhite(&tevBlack, &tevWhite)) {
        return false;
    } else {
        return setBlackWhite(tevBlack, white);
    }
}

bool J2DTextBoxEx::setBlackWhite(JUtility::TColor param_0, JUtility::TColor param_1) {
    if (mMaterial == NULL) {
        return false;
    }

    if (mMaterial->getTevBlock() == NULL) {
        return false;
    }

    if (!isSetBlackWhite(param_0, param_1)) {
        return false;
    }

    bool bvar = (param_0 != 0) || (param_1 != -1);

    u8 stageNum = bvar ? 2 : 1;
    mMaterial->getTevBlock()->setTevStageNum(stageNum);
    setTevOrder(bvar);
    setTevStage(bvar);

    if (bvar) {
        J2DGXColorS10 color;
        color.r = param_0.r;
        color.g = param_0.g;
        color.b = param_0.b;
        color.a = param_0.a;
        mMaterial->getTevBlock()->setTevColor(0, color);

        color.r = param_1.r;
        color.g = param_1.g;
        color.b = param_1.b;
        color.a = param_1.a;
        mMaterial->getTevBlock()->setTevColor(1, color); 
    }

    return true;
}

bool J2DTextBoxEx::getBlackWhite(JUtility::TColor* param_0, JUtility::TColor* param_1) const {
    if (mMaterial == NULL) {
        return false;
    }

    if (mMaterial->getTevBlock() == NULL) {
        return false;
    }

    u32 tevStageNum = mMaterial->getTevBlock()->getTevStageNum();
    bool manyTevStages = tevStageNum == 1 ? false : true;
    *param_0 = JUtility::TColor(0);
    *param_1 = JUtility::TColor(0xffffffff);
    if (manyTevStages) {
        J2DGXColorS10 color0(*mMaterial->getTevBlock()->getTevColor(0));
        J2DGXColorS10 color1(*mMaterial->getTevBlock()->getTevColor(1));
        *param_0 = JUtility::TColor(
            (((u8)color0.r) << 24) | (((u8)color0.g) << 16) | (((u8)color0.b) << 8) |
                ((u8)color0.a));
        *param_1 = JUtility::TColor(
            (((u8)color1.r) << 24) | (((u8)color1.g) << 16) | (((u8)color1.b) << 8) |
                ((u8)color1.a));
    }
    return true;
}

bool J2DTextBoxEx::isSetBlackWhite(JUtility::TColor param_0, JUtility::TColor param_1) const {
    if (param_0 == 0 && param_1 == 0xffffffff) {
        return 1;
    }
    u32 tevStageNum = mMaterial->getTevBlock()->getTevStageNum();
    u8 maxStage = mMaterial->getTevBlock()->getMaxStage();
    if (maxStage == 1) {
        return 0;
    }
    return 1;
}

JUtility::TColor J2DTextBoxEx::getBlack() const {
    JUtility::TColor black;
    JUtility::TColor white;
    if (getBlackWhite(&black, &white) == 0) {
        return JUtility::TColor(0);
    }
    return black;
}

JUtility::TColor J2DTextBoxEx::getWhite() const {
    JUtility::TColor black;
    JUtility::TColor white;
    if (getBlackWhite(&black, &white) == 0) {
        return JUtility::TColor(0xffffffff);
    }
    return white;
}

void J2DTextBoxEx::setAlpha(u8 alpha) {
    mAlpha = alpha;

    if (mMaterial == NULL) {
        return;
    }

    if (mMaterial->getColorBlock()->getMatColor(0) == NULL) {
        return;
    }

    mMaterial->getColorBlock()->getMatColor(0)->a = alpha;
}

void J2DTextBoxEx::setCullBack(GXCullMode mode) {
    mCullMode = mode;

    if (mMaterial != NULL) {
        mMaterial->getColorBlock()->setCullMode(mode);
    }

    J2DPane::setCullBack(mode);
}

void J2DTextBoxEx::rewriteAlpha() {
    if (mMaterial == NULL) {
        return;
    }

    mAlpha = mMaterial->getColorBlock()->getMatColor(0)->a;
}

bool J2DTextBoxEx::isUsed(ResFONT const* p_font) {
    if (getFont() != NULL && getFont()->getResFont() == p_font) {
        return true;
    }

    return J2DPane::isUsed(p_font);
}

void J2DTextBoxEx::setAnimation(J2DAnmColor* anm) {
    if (mMaterial != NULL) {
        mMaterial->setAnimation(anm);
    }
}

void J2DTextBoxEx::setAnimation(J2DAnmTextureSRTKey* anm) {
    if (mMaterial != NULL) {
        mMaterial->setAnimation(anm);
    }
}

void J2DTextBoxEx::setAnimation(J2DAnmTexPattern* anm) {
    if (mMaterial != NULL) {
        mMaterial->setAnimation(anm);
    }
}

void J2DTextBoxEx::setAnimation(J2DAnmTevRegKey* anm) {
    if (mMaterial != NULL) {
        mMaterial->setAnimation(anm);
    }
}

void J2DTextBoxEx::setAnimation(J2DAnmVisibilityFull* anm) {
    mVisibilityAnm = anm;
}

const J2DAnmTransform* J2DTextBoxEx::animationPane(J2DAnmTransform const* param_0) {
    if (mVisibilityAnm != NULL && field_0x13c != 0xffff) {
        u8 visibility;
        mVisibilityAnm->getVisibility(field_0x13c, &visibility);
        if (visibility != 0) {
            show();
        } else {
            hide();
        }
    }
    return J2DPane::animationPane(param_0);
}

void J2DTextBoxEx::setCullBack(bool param_0) {
    setCullBack(param_0 ? GX_CULL_BACK : GX_CULL_NONE);
}

bool J2DTextBoxEx::isUsed(ResTIMG const* p_timg) {
    return J2DPane::isUsed(p_timg);
}

void J2DTextBoxEx::setAnimation(J2DAnmVtxColor* param_0) {
    /* empty function */
}

void J2DTextBoxEx::setAnimation(J2DAnmBase* anm) {
    J2DPane::setAnimation(anm);
}
