#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J2DGraph/J2DMatBlock.h"
#include "JSystem/J2DGraph/J2DTevs.h"
#include "JSystem/JUtility/JUTPalette.h"
#include "JSystem/JUtility/JUTResFont.h"
#include "JSystem/JUtility/JUTTexture.h"
#include "dolphin/gx.h"

void J2DColorBlock::initialize() {
    for (int i = 0; i < 2; i++) {
        mMatColor[i] = JUtility::TColor(j2dDefaultColInfo);
    }
    mColorChanNum = 2;
    for (int i = 0; i < 4; i++) {
        mColorChan[i].setColorChanInfo(j2dDefaultColorChanInfo);
    }
    mCullMode = GX_CULL_NONE;
}

void J2DColorBlock::setGX() {
    for (int i = 0; i < 2; i++) {
        GXSetChanMatColor(GXChannelID(GX_COLOR0A0 + i), mMatColor[i]);
    }
    GXSetNumChans(mColorChanNum);
    const GXChannelID mapping[4] = {GX_COLOR0, GX_ALPHA0, GX_COLOR1, GX_ALPHA1};
    for (int i = 0; i < mColorChanNum << 1; i++) {
        GXColorSrc mat_src = (GXColorSrc)mColorChan[i].getMatSrc();
        GXSetChanCtrl(mapping[i], GX_DISABLE, GX_SRC_REG, mat_src, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    }
    GXSetCullMode((GXCullMode)mCullMode);
}

void J2DTexGenBlock::initialize() {
    mTexGenNum = 0;
    for (int i = 0; i < 8; i++) {
        mTexGenCoord[i].setTexCoordInfo(j2dDefaultTexCoordInfo[0]);
    }
    for (int i = 0; i < 8; i++) {
        mTexMtx[i] = NULL;
    }
}

void J2DTexGenBlock::setGX() {
    GXSetNumTexGens(mTexGenNum);
    for (int i = 0; i < mTexGenNum; i++) {
        GXSetTexCoordGen((GXTexCoordID)i, (GXTexGenType)mTexGenCoord[i].getTexGenType(), (GXTexGenSrc)mTexGenCoord[i].getTexGenSrc(), mTexGenCoord[i].getTexGenMtx());
    }
    
    for (u8 i = 0; i < 8; i++) {
        if (mTexMtx[i]) {
            mTexMtx[i]->load(i);
        }
    }
}

J2DTexGenBlock::~J2DTexGenBlock() {
    for (int i = 0; i < 8; i++) {
        delete mTexMtx[i];
    }
}

void J2DTexGenBlock::setTexMtx(u32 param_0, J2DTexMtx& param_1) {
    J3D_PANIC(103, param_0 < 8, "Error : range over.");
    
    if (!mTexMtx[param_0]) {
        mTexMtx[param_0] = new J2DTexMtx(param_1.getTexMtxInfo());
        if (!mTexMtx[param_0]) {
            return;
        }
    } else {
        mTexMtx[param_0]->setTexMtxInfo(param_1.getTexMtxInfo());
    }
    mTexMtx[param_0]->calc();
}

void J2DTexGenBlock::getTexMtx(u32 param_0, J2DTexMtx& param_1) {
    J3D_PANIC(123, param_0 < 8, "Error : range over.");
    
    if (mTexMtx[param_0]) {
        J2DTexMtxInfo& texMtxInfo = mTexMtx[param_0]->getTexMtxInfo();
        param_1.setTexMtxInfo(texMtxInfo);
    }
}

JUTTexture* J2DTevBlock::getTexture(u32) {
    return NULL;
}

JUTPalette* J2DTevBlock::getPalette(u32) {
    return NULL;
}

J2DTevBlock1::J2DTevBlock1() {
    mTexture[0] = NULL;
    mUndeleteFlag = 0;
    initialize();
}

J2DTevBlock1::~J2DTevBlock1() {
    if (mUndeleteFlag & 1) {
        delete mTexture[0];
    }

    delete mPalette[0];

    if (mUndeleteFlag & 0x80) {
        delete mFont;
    }
}

void J2DTevBlock1::initialize() {
    mTexNo[0] = -1;
    mFontNo = -1;
    mTevOrder[0].setTevOrderInfo(j2dDefaultTevOrderInfoNull);
    for (int i = 0; i < 4; i++) {
        mTevColor[i] = j2dDefaultTevColor;
    }
    mTevStage[0].setStageNo(0);
    for (int i = 0; i < 4; i++) {
        mTevKColor[i] = j2dDefaultTevKColor;
    }
    mTevKColorSel[0] = -1;
    mTevKAlphaSel[0] = -1;
    for (int i = 0; i < 4; i++) {
        mTevSwapModeTable[i].setTevSwapModeTableInfo(j2dDefaultTevSwapModeTable);
    }
    mIndTevStage[0].setIndTevStageInfo(j2dDefaultIndTevStageInfo);
    mPalette[0] = NULL;
    mFont = NULL;
}

bool J2DTevBlock1::prepareTexture(u8 param_0) {
    bool rv;
    if (param_0 == 0) {
        return true;
    }

    if (mTexture[0] == NULL) {
        mTexture[0] = new JUTTexture();

        if (mTexture[0] == NULL) {
            return false;
        }
        mUndeleteFlag = (mUndeleteFlag & 0x80) | 1;
    }

    if (param_0 > 1) {
        rv = false;
    } else {
        rv = true;
    }
    
    return rv;
}

bool J2DTevBlock1::insertTexture(u32 param_0, ResTIMG const* p_timg, JUTPalette* p_tlut) {
    if (param_0 != 0 || p_timg == NULL) {
        return false;
    }

    if (mTexture[0] != NULL && mTexture[0]->getTexInfo() != NULL) {
        return false;
    }

    if (mTexture[0] == NULL) {
        mTexture[0] = new JUTTexture(p_timg, 0);

        if (mTexture[0] == NULL) {
            return false;
        }

        if (p_tlut != NULL) {
            mTexture[0]->storeTIMG(p_timg, p_tlut);
        }
    } else if (p_tlut == NULL) {
        mTexture[0]->storeTIMG(p_timg, (u8)0);
    } else {
        mTexture[0]->storeTIMG(p_timg, p_tlut);
    }

    mUndeleteFlag = (mUndeleteFlag & 0x80) | 1;
    mPalette[0] = NULL;
    mTexNo[0] = -1;
    return true;
}

bool J2DTevBlock1::insertTexture(u32 param_0, JUTTexture* p_tex) {
    if (mTexture[0] != NULL && mTexture[0]->getTexInfo() != NULL) {
        return false;
    }

    if (param_0 != 0 || p_tex == NULL) {
        return false;
    }

    if (mUndeleteFlag & 1) {
        delete mTexture[0];
    }

    mTexture[0] = p_tex;
    mUndeleteFlag &= 0x80;
    mPalette[0] = NULL;
    mTexNo[0] = -1;
    return true;
}

bool J2DTevBlock1::setTexture(u32 param_0, ResTIMG const* p_timg) {
    if (param_0 != 0) {
        return false;
    }

    if (mTexture[0] == NULL) {
        if (p_timg != NULL) {
            mTexture[0] = new JUTTexture(p_timg, 0);

            if (mTexture[0] == NULL) {
                return false;
            }

            mUndeleteFlag = (mUndeleteFlag & 0x80) | 1;
        } else {
            mUndeleteFlag &= 0x80;
        }
    } else if (mUndeleteFlag & 1) {
        if (p_timg != NULL) {
            mTexture[0]->storeTIMG(p_timg, (u8)0);
        } else {
            delete mTexture[0];
            mTexture[0] = NULL;
            mUndeleteFlag &= 0x80;
        }
    } else {
        mTexture[0] = NULL;

        if (p_timg != NULL) {
            mTexture[0] = new JUTTexture(p_timg, 0);

            if (mTexture[0] == NULL) {
                return false;
            }

            mUndeleteFlag = (mUndeleteFlag & 0x80) | 1;
        }
    }

    delete mPalette[0];
    mPalette[0] = NULL;
    mTexNo[0] = -1;
    return true;
}

bool J2DTevBlock1::setTexture(u32 param_0, JUTTexture* p_tex) {
    if (param_0 != 0) {
        return false;
    }

    if (mUndeleteFlag & 1) {
        delete mTexture[0];
    }

    mTexture[0] = p_tex;
    mUndeleteFlag &= 0x80;
    delete mPalette[0];
    mPalette[0] = NULL;
    mTexNo[0] = -1;
    return true;
}

bool J2DTevBlock1::removeTexture(u32 param_0) {
    if (param_0 >= 1) {
        return false;
    }

    if (mUndeleteFlag & 1) {
        delete mTexture[0];
    }

    mTexture[0] = NULL;
    mUndeleteFlag &= 0x80;
    delete mPalette[0];
    mTexNo[0] = -1;
    return true;
}

bool J2DTevBlock1::setFont(ResFONT* p_font) {
    if (p_font == NULL) {
        return false;
    }

    JUTResFont* resFont = new JUTResFont(p_font, NULL);
    if (resFont == NULL) {
        return false;
    }

    if (mUndeleteFlag & 0x80) {
        delete mFont;
    }

    mFont = resFont;
    mUndeleteFlag = (mUndeleteFlag & 0x7F) | 0x80;
    return true;
}

bool J2DTevBlock1::setFont(JUTFont* p_font) {
    if (p_font == NULL) {
        return false;
    }

    if (mUndeleteFlag & 0x80) {
        delete mFont;
    }

    mFont = (JUTResFont*)p_font;
    mUndeleteFlag = mUndeleteFlag & 0x7F;
    return true;
}

bool J2DTevBlock1::setPalette(u32 param_0, ResTLUT const* p_tlut) {
    if (param_0 != 0) {
        return false;
    }

    if (p_tlut != NULL) {
        if (mPalette[0] == NULL) {
            mPalette[0] = new JUTPalette(GX_TLUT0, (ResTLUT*)p_tlut);

            if (mPalette[0] == NULL) {
                return false;
            }
        } else {
            mPalette[0]->storeTLUT(GX_TLUT0, (ResTLUT*)p_tlut);
        }

        if (mTexture[0] != NULL) {
            mTexture[0]->attachPalette(mPalette[0]);
        }
    } else {
        delete mPalette[0];
        mPalette[0] = NULL;
    }

    return true;
}

void J2DTevBlock1::shiftDeleteFlag(u8, bool) {
    mUndeleteFlag &= 0x80;
}

void J2DTevBlock1::setGX() {
    loadTexture(GX_TEXMAP0, 0);
    GXSetTevOrder(GX_TEVSTAGE0, (GXTexCoordID)mTevOrder[0].getTexCoord(), (GXTexMapID)mTevOrder[0].getTexMap(), (GXChannelID)mTevOrder[0].getColor());

    for (int i = 0; i < 4; i++) {
        GXSetTevColorS10(i != 3 ? GXTevRegID(i + 1) : GX_TEVPREV, mTevColor[i]);
    }

    GXSetNumTevStages(1);
    GXSetTevColorIn(GX_TEVSTAGE0, GXTevColorArg(mTevStage[0].getColorA()),
                    GXTevColorArg(mTevStage[0].getColorB()),
                    GXTevColorArg(mTevStage[0].getColorC()),
                    GXTevColorArg(mTevStage[0].getColorD()));
    GXSetTevAlphaIn(GX_TEVSTAGE0, GXTevAlphaArg(mTevStage[0].getAlphaA()),
                    GXTevAlphaArg(mTevStage[0].getAlphaB()),
                    GXTevAlphaArg(mTevStage[0].getAlphaC()),
                    GXTevAlphaArg(mTevStage[0].getAlphaD()));
    GXSetTevColorOp(GX_TEVSTAGE0, GXTevOp(mTevStage[0].getCOp()),
                    GXTevBias(mTevStage[0].getCBias()), GXTevScale(mTevStage[0].getCScale()),
                    mTevStage[0].getCClamp(), GXTevRegID(mTevStage[0].getCReg()));
    GXSetTevAlphaOp(GX_TEVSTAGE0, (GXTevOp)mTevStage[0].getAOp(),
                    GXTevBias(mTevStage[0].getABias()), GXTevScale(mTevStage[0].getAScale()),
                    mTevStage[0].getAClamp(), GXTevRegID(mTevStage[0].getAReg()));
    GXSetTevKColorSel(GX_TEVSTAGE0, mTevKColorSel[0] != 0xff ? GXTevKColorSel(mTevKColorSel[0]) : GX_TEV_KCSEL_1);
    GXSetTevKAlphaSel(GX_TEVSTAGE0, mTevKAlphaSel[0] != 0xff ? GXTevKAlphaSel(mTevKAlphaSel[0]) : GX_TEV_KASEL_1);
    for (int i = 0; i < 4; i++) {
        GXSetTevKColor(GXTevKColorID(i), mTevKColor[i]);
    }
    GXSetTevSwapMode(GX_TEVSTAGE0, GXTevSwapSel(mTevStage[0].getRasSel()), GXTevSwapSel(mTevStage[0].getTexSel()));
    for (int i = 0; i < 4; i++) {
        GXSetTevSwapModeTable(GXTevSwapSel(i), GXTevColorChan(mTevSwapModeTable[i].getR()),
        GXTevColorChan(mTevSwapModeTable[i].getG()),
        GXTevColorChan(mTevSwapModeTable[i].getB()),
        GXTevColorChan(mTevSwapModeTable[i].getA()));
    }
    mIndTevStage->load(0);
}

void J2DTevBlock1::loadTexture(GXTexMapID texmapID, u32 texIndex) {
    if (texIndex == 0 && mTexture[texIndex] != NULL && mTexture[texIndex]->getTexInfo() != NULL) {
        mTexture[texIndex]->load(texmapID);
    }
}

J2DTevBlock2::J2DTevBlock2() {
    mTexture[0] = NULL;
    mTexture[1] = NULL;
    mUndeleteFlag = 0;
    initialize();
}

J2DTevBlock2::~J2DTevBlock2() {
    if (mUndeleteFlag & 1) {
        delete mTexture[0];
    }

    if (mUndeleteFlag & 2) {
        delete mTexture[1];
    }

    delete mPalette[0];
    delete mPalette[1];

    if (mUndeleteFlag & 0x80) {
        delete mFont;
    }
}

void J2DTevBlock2::initialize() {
    mTexNo[0] = 0xffff;
    mTexNo[1] = 0xffff;
    mFontNo = 0xffff;
    mTevOrder[0].setTevOrderInfo(j2dDefaultTevOrderInfoNull);
    mTevOrder[1].setTevOrderInfo(j2dDefaultTevOrderInfoNull);
    for (int i = 0; i < 4; i++) {
        mTevColor[i] = j2dDefaultTevColor;
    }
    mTevStageNum = 1;
    mTevStage[0].setStageNo(0);
    mTevStage[1].setStageNo(1);
    for (int i = 0; i < 4; i++) {
        mTevKColor[i] = j2dDefaultTevKColor;
    }
    mTevKColorSel[0] = 0xff;
    mTevKColorSel[1] = 0xff;
    mTevKAlphaSel[0] = 0xff;
    mTevKAlphaSel[1] = 0xff;
    for (int i = 0; i < 4; i++) {
        mTevSwapModeTable[i].setTevSwapModeTableInfo(j2dDefaultTevSwapModeTable);
    }
    mIndTevStage[0].setIndTevStageInfo(j2dDefaultIndTevStageInfo);
    mIndTevStage[1].setIndTevStageInfo(j2dDefaultIndTevStageInfo);
    mPalette[0] = NULL;
    mPalette[1] = NULL;
    mFont = NULL;
}

bool J2DTevBlock2::prepareTexture(u8 param_0) {
    for (u8 i = 0; i < param_0; i++) {
        if (i >= 2) {
            return false;
        }

        if (mTexture[i] == NULL) {
            mTexture[i] = new JUTTexture();

            if (mTexture[i] == NULL) {
                return false;
            }
            mUndeleteFlag |= (1 << i);
        }
    }

    return true;
}

bool J2DTevBlock2::insertTexture(u32 param_0, ResTIMG const* p_timg, JUTPalette* p_tlut) {
    if (param_0 >= 2 || p_timg == NULL) {
        return false;
    }

    u8 texNo = 0;
    for (; texNo < 2; texNo++) {
        if (mTexture[texNo] == NULL || mTexture[texNo]->getTexInfo() == NULL) {
            break;
        }
    }

    if (texNo < param_0) {
        return false;
    }
    
    if (texNo == 2) {
        return false;
    }

    JUTTexture* tex;
    u8 tlutid = 0;
    if (p_timg->indexTexture && p_tlut == NULL) {
        tex = mTexture[0];
        if (tex != NULL && tex->getTexInfo() != NULL && tex->getTexInfo()->indexTexture) {
            int tlutname = tex->getTlutName();
            if (tlutname == GX_TLUT0 || tlutname == GX_BIGTLUT0) {
                tlutid = 1;
            }
        }
    }

    tex = mTexture[texNo];
    if (tex == NULL) {
        JUTTexture* newtex = new JUTTexture(p_timg, tlutid);

        if (newtex == NULL) {
            return false;
        }

        if (p_tlut != NULL) {
            newtex->storeTIMG(p_timg, p_tlut);
        }

        if (param_0 == 0) {
            mTexture[1] = mTexture[0];
            mPalette[1] = mPalette[0];
            mTexNo[1] = mTexNo[0];
        }
        mTexture[param_0] = newtex;
        shiftDeleteFlag(param_0, true);
        mUndeleteFlag |= (1 << param_0);
    } else {
        if (p_tlut == NULL) {
            tex->storeTIMG(p_timg, tlutid);
        } else {
            tex->storeTIMG(p_timg, p_tlut);
        }

        bool local_28[2];
        for (u8 i = 0; i < 2; i++) {
            local_28[i] = mUndeleteFlag & (1 << i);
        }

        if (param_0 == 0 && texNo == 1) {
            mTexture[1] = mTexture[0];
            mPalette[1] = mPalette[0];
            mTexNo[1] = mTexNo[0];
            local_28[1] = local_28[0];
        }
        mTexture[param_0] = tex;
        mUndeleteFlag &= 0x80;

        for (u8 i = 0; i < 2; i++) {
            if (local_28[i]) {
                mUndeleteFlag |= (1 << i);
            }
        }

        mUndeleteFlag |= (1 << param_0);
    }

    mPalette[param_0] = NULL;
    mTexNo[param_0] = -1;
    return true;
}

bool J2DTevBlock2::insertTexture(u32 param_0, JUTTexture* p_tex) {
    if (param_0 >= 2 || p_tex == NULL) {
        return false;
    }

    u8 texNo = 0;
    for (; texNo < 2; texNo++) {
        if (mTexture[texNo] == NULL || mTexture[texNo]->getTexInfo() == NULL) {
            break;
        }
    }

    if (texNo < param_0) {
        return false;
    }
    
    if (texNo == 2) {
        return false;
    }

    if (mTexture[1] != NULL && mTexture[1]->getTexInfo() == NULL) {
        if (mUndeleteFlag & 2) {
            delete mTexture[1];
        }
        
        mUndeleteFlag &= ~2;
    }

    if (param_0 == 0) {
        mTexture[1] = mTexture[0];
        mPalette[1] = mPalette[0];
        mTexNo[1] = mTexNo[0];
    }

    mTexture[param_0] = p_tex;
    mPalette[param_0] = NULL;
    mTexNo[param_0] = -1;
    shiftDeleteFlag(param_0, true);
    mUndeleteFlag &= ~(1 << param_0);
    return true;
}

bool J2DTevBlock2::setTexture(u32 param_0, ResTIMG const* p_timg) {
    if (param_0 >= 2) {
        return false;
    }

    u8 tlutid = 0;
    if (p_timg != NULL && p_timg->indexTexture) {
        u8 idx = param_0 == 0 ? 1 : 0;
        if (mTexture[idx] != NULL) {
            const ResTIMG* timg = mTexture[idx]->getTexInfo();
            if (timg != NULL && timg->indexTexture) {
                int tlutname = mTexture[idx]->getTlutName();
                u8 tlut_no = tlutname - (tlutname >= GX_BIGTLUT0 ? GX_BIGTLUT0 : GX_TLUT0);

                if (tlut_no == 0) {
                    tlutid = 1;
                }
            }
        }
    }

    if (mTexture[param_0] == NULL) {
        if (p_timg != NULL) {
            mTexture[param_0] = new JUTTexture(p_timg, tlutid);

            if (mTexture[param_0] == NULL) {
                return false;
            }

            mUndeleteFlag |= (1 << param_0);
        } else {
            mUndeleteFlag &= ~(1 << param_0);
        }
    } else if (mUndeleteFlag & (1 << param_0)) {
        if (p_timg != NULL) {
            mTexture[param_0]->storeTIMG(p_timg, tlutid);
        } else {
            delete mTexture[param_0];
            mTexture[param_0] = NULL;
            mUndeleteFlag &= ~(1 << param_0);
        }
    } else {
        mTexture[param_0] = NULL;

        if (p_timg != NULL) {
            mTexture[param_0] = new JUTTexture(p_timg, 0);

            if (mTexture[param_0] == NULL) {
                return false;
            }

            mUndeleteFlag |= (1 << param_0);
        }
    }

    delete mPalette[param_0];
    mPalette[param_0] = NULL;
    mTexNo[param_0] = -1;
    return true;
}

bool J2DTevBlock2::setTexture(u32 param_0, JUTTexture* p_tex) {
    if (param_0 >= 2) {
        return false;
    }

    if (mUndeleteFlag & (1 << param_0)) {
        delete mTexture[param_0];
    }

    mTexture[param_0] = p_tex;
    mUndeleteFlag &= ~(1 << param_0);
    delete mPalette[param_0];
    mPalette[param_0] = NULL;
    mTexNo[param_0] = -1;
    return true;
}

bool J2DTevBlock2::removeTexture(u32 param_0) {
    if (param_0 >= 2) {
        return false;
    }

    if (mUndeleteFlag & (1 << param_0)) {
        delete mTexture[param_0];
    }
    delete mPalette[param_0];

    if (param_0 == 0) {
        mTexture[0] = mTexture[1];
        mPalette[0] = mPalette[1];
        mTexNo[0] = mTexNo[1];
    }

    mTexture[1] = NULL;
    mPalette[1] = NULL;
    mTexNo[1] = -1;
    shiftDeleteFlag(param_0, false);
    return true;
}

bool J2DTevBlock2::setFont(ResFONT* p_font) {
    if (p_font == NULL) {
        return false;
    }

    JUTResFont* resFont = new JUTResFont(p_font, NULL);
    if (resFont == NULL) {
        return false;
    }

    if (mUndeleteFlag & 0x80) {
        delete mFont;
    }

    mFont = resFont;
    mUndeleteFlag = (mUndeleteFlag & 0x7F) | 0x80;
    return true;
}

bool J2DTevBlock2::setFont(JUTFont* p_font) {
    if (p_font == NULL) {
        return false;
    }

    if (mUndeleteFlag & 0x80) {
        delete mFont;
    }

    mFont = (JUTResFont*)p_font;
    mUndeleteFlag = mUndeleteFlag & 0x7F;
    return true;
}

bool J2DTevBlock2::setPalette(u32 param_0, ResTLUT const* p_tlut) {
    if (param_0 >= 2) {
        return false;
    }

    if (p_tlut != NULL) {
        if (mPalette[param_0] == NULL) {
            mPalette[param_0] = new JUTPalette((GXTlut)param_0, (ResTLUT*)p_tlut);

            if (mPalette[param_0] == NULL) {
                return false;
            }
        } else {
            mPalette[param_0]->storeTLUT(GX_TLUT0, (ResTLUT*)p_tlut);
        }

        if (mTexture[param_0] != NULL) {
            mTexture[param_0]->attachPalette(mPalette[param_0]);
        }
    } else {
        delete mPalette[param_0];
        mPalette[param_0] = NULL;
    }

    return true;
}

void J2DTevBlock2::shiftDeleteFlag(u8 param_0, bool param_1) {
    u8 tmpFlags = mUndeleteFlag & 0x80;
    mUndeleteFlag = mUndeleteFlag & 0x7F;

    if (param_1) {
        mUndeleteFlag = (mUndeleteFlag & ((1 << param_0) - 1)) | ((mUndeleteFlag & ~((1 << param_0) - 1)) * 2);
    } else {
        mUndeleteFlag = (mUndeleteFlag & ((1 << param_0) - 1)) | ((mUndeleteFlag & ~((1 << (param_0 + 1)) - 1)) >> 1);
    }

    mUndeleteFlag = (mUndeleteFlag | tmpFlags) & 0xFF;
}

void J2DTevBlock2::setGX() {
    for (int i = 0; i < 2; i++) {
        loadTexture(GXTexMapID(i), i);
    }

    for (int i = 0; i < mTevStageNum; i++) {
        GXSetTevOrder(GXTevStageID(i), (GXTexCoordID)mTevOrder[i].getTexCoord(), (GXTexMapID)mTevOrder[i].getTexMap(), (GXChannelID)mTevOrder[i].getColor());
    }

    for (int i = 0; i < 4; i++) {
        GXSetTevColorS10(i != 3 ? GXTevRegID(i + 1) : GX_TEVPREV, mTevColor[i]);
    }

    GXSetNumTevStages(mTevStageNum);
    for (int i = 0; i < mTevStageNum; i++) {
        GXSetTevColorIn(GXTevStageID(i), GXTevColorArg(mTevStage[i].getColorA()),
                        GXTevColorArg(mTevStage[i].getColorB()),
                        GXTevColorArg(mTevStage[i].getColorC()),
                        GXTevColorArg(mTevStage[i].getColorD()));
        GXSetTevAlphaIn(GXTevStageID(i), GXTevAlphaArg(mTevStage[i].getAlphaA()),
                        GXTevAlphaArg(mTevStage[i].getAlphaB()),
                        GXTevAlphaArg(mTevStage[i].getAlphaC()),
                        GXTevAlphaArg(mTevStage[i].getAlphaD()));
        GXSetTevColorOp(GXTevStageID(i), GXTevOp(mTevStage[i].getCOp()),
                        GXTevBias(mTevStage[i].getCBias()), GXTevScale(mTevStage[i].getCScale()),
                        mTevStage[i].getCClamp(), GXTevRegID(mTevStage[i].getCReg()));
        GXSetTevAlphaOp(GXTevStageID(i), (GXTevOp)mTevStage[i].getAOp(),
                        GXTevBias(mTevStage[i].getABias()), GXTevScale(mTevStage[i].getAScale()),
                        mTevStage[i].getAClamp(), GXTevRegID(mTevStage[i].getAReg()));
        GXSetTevKColorSel(GXTevStageID(i), mTevKColorSel[i] != 0xff ? GXTevKColorSel(mTevKColorSel[i]) : GX_TEV_KCSEL_1);
        GXSetTevKAlphaSel(GXTevStageID(i), mTevKAlphaSel[i] != 0xff ? GXTevKAlphaSel(mTevKAlphaSel[i]) : GX_TEV_KASEL_1);
    }

    for (int i = 0; i < 4; i++) {
        GXSetTevKColor(GXTevKColorID(i), mTevKColor[i]);
    }

    for (int i = 0; i < mTevStageNum; i++) {
        GXSetTevSwapMode(GXTevStageID(i), GXTevSwapSel(mTevStage[i].getRasSel()), GXTevSwapSel(mTevStage[i].getTexSel()));
    }

    for (int i = 0; i < 4; i++) {
        GXSetTevSwapModeTable(GXTevSwapSel(i), GXTevColorChan(mTevSwapModeTable[i].getR()),
                              GXTevColorChan(mTevSwapModeTable[i].getG()),
                              GXTevColorChan(mTevSwapModeTable[i].getB()),
                              GXTevColorChan(mTevSwapModeTable[i].getA()));
    }

    for (u8 i = 0; i < mTevStageNum; i++) {
        mIndTevStage[i].load(i);
    }
}

void J2DTevBlock2::loadTexture(GXTexMapID texmapID, u32 texIndex) {
    if (texIndex < 2 && mTexture[texIndex] != NULL && mTexture[texIndex]->getTexInfo() != NULL) {
        mTexture[texIndex]->load(texmapID);
    }
}

J2DTevBlock4::J2DTevBlock4() {
    for (int i = 0; i < 4; i++) {
        mTexture[i] = NULL;
    }

    mUndeleteFlag = 0;
    initialize();
}

J2DTevBlock4::~J2DTevBlock4() {
    for (int i = 0; i < 4; i++) {
        if (mUndeleteFlag & (1 << i)) {
            delete mTexture[i];
        }
        delete mPalette[i];
    }

    if (mUndeleteFlag & 0x80) {
        delete mFont;
    }
}

void J2DTevBlock4::initialize() {
    for (int i = 0; i < 4; i++) {
        mTexNo[i] = -1;
    }
    mFontNo = -1;
    for (int i = 0; i < 4; i++) {
        mTevOrder[i].setTevOrderInfo(j2dDefaultTevOrderInfoNull);
    }
    for (int i = 0; i < 4; i++) {
        mTevColor[i] = j2dDefaultTevColor;
    }
    mTevStageNum = 1;
    for (int i = 0; i < 4; i++) {
        mTevStage[i].setStageNo(i);
    }
    for (int i = 0; i < 4; i++) {
        mTevKColor[i] = j2dDefaultTevKColor;
    }
    for (int i = 0; i < 4; i++) {
        mTevKColorSel[i] = -1;
        mTevKAlphaSel[i] = -1;
    }
    for (int i = 0; i < 4; i++) {
        mTevSwapModeTable[i].setTevSwapModeTableInfo(j2dDefaultTevSwapModeTable);
    }
    for (int i = 0; i < 4; i++) {
        mIndTevStage[i].setIndTevStageInfo(j2dDefaultIndTevStageInfo);
    }
    for (int i = 0; i < 4; i++) {
        mPalette[i] = NULL;
    }
    mFont = NULL;
}

bool J2DTevBlock4::prepareTexture(u8 param_0) {
    for (u8 i = 0; i < param_0; i++) {
        if (i >= 4) {
            return 0;
        }

        if (mTexture[i] == NULL) {
            mTexture[i] = new JUTTexture();

            if (mTexture[i] == NULL) {
                return false;
            }
            mUndeleteFlag |= (1 << i);
        }
    }

    return true;
}

bool J2DTevBlock4::insertTexture(u32 param_0, ResTIMG const* p_timg, JUTPalette* p_tlut) {
    if (param_0 >= 4 || !p_timg) {
        return false;
    }
    u8 idx;
    for (idx = 0; idx < 4; idx++) {
        if (!mTexture[idx] || !mTexture[idx]->getTexInfo()) {
            break;
        }
    }
    if (idx < param_0) {
        return false;
    }
    if (idx == 4) {
        return false;
    }
    u8 local_43 = 0;
    if (p_timg->indexTexture && !p_tlut) {
        u8 local_44 = 0;
        for (int i = 0; i < 3; i++) {
            if (!mTexture[i]) {
                continue;
            }
            const ResTIMG* texInfo = mTexture[i]->getTexInfo();
            if (!texInfo) {
                continue;
            }
            if (!texInfo->indexTexture) {
                continue;
            }
            int tlutName = mTexture[i]->getTlutName();
            u8 tlut_no = tlutName - (tlutName >= GX_BIGTLUT0 ? GX_BIGTLUT0 : GX_TLUT0);
            if (tlut_no < 4) {
                local_44 |= 1 << tlut_no;
            }
        }
        for (u8 i = 0; i < 4; i++) {
            if ((local_44 & 1 << i) == 0) {
                local_43 = i;
                break;
            }
        }
    }
    if (!mTexture[idx]) {
        JUTTexture* texture = new JUTTexture(p_timg, local_43);
        if (!texture) {
            return false;
        }
        if (p_tlut) {
            texture->storeTIMG(p_timg, p_tlut);
        }
        for (u8 i = 3; i > param_0; i--) {
            mTexture[i] = mTexture[i - 1];
            mPalette[i] = mPalette[i - 1];
            mTexNo[i] = mTexNo[i - 1];
        }
        mTexture[param_0] = texture;
        shiftDeleteFlag(param_0, true);
        mUndeleteFlag |= 1 << param_0;
    } else {
        JUTTexture* texture = mTexture[idx];
        if (!p_tlut) {
            texture->storeTIMG(p_timg, local_43);
        } else {
            texture->storeTIMG(p_timg, p_tlut);
        }
        bool local_38[4];
        for (u8 i = 0; i < 4; i++) {
            local_38[i] = (mUndeleteFlag & 1 << i) != 0;
        }
        for (u8 i = idx; i > param_0; i--) {
            mTexture[i] = mTexture[i - 1];
            mPalette[i] = mPalette[i - 1];
            mTexNo[i] = mTexNo[i - 1];
            local_38[i] = local_38[i - 1];
        }
        mTexture[param_0] = texture;
        mUndeleteFlag &= 0x80;
        for (u8 i = 0; i < 4; i++) {
            if (local_38[i]) {
                mUndeleteFlag |= 1 << i;
            }
        }
        mUndeleteFlag |= 1 << param_0;
    }
    mPalette[param_0] = NULL;
    mTexNo[param_0] = -1;
    return true;
}

bool J2DTevBlock4::insertTexture(u32 param_0, JUTTexture* p_tex) {
    if (param_0 >= 4 || p_tex == NULL) {
        return false;
    }

    u8 texNo = 0;
    for (; texNo < 4; texNo++) {
        if (mTexture[texNo] == NULL || mTexture[texNo]->getTexInfo() == NULL) {
            break;
        }
    }

    if (texNo < param_0) {
        return false;
    }
    
    if (texNo == 4) {
        return false;
    }

    if (mTexture[3] != NULL && mTexture[3]->getTexInfo() == NULL) {
        if (mUndeleteFlag & 8) {
            delete mTexture[3];
        }
        
        mUndeleteFlag &= ~8;
    }

    for (u8 i = 3; i > param_0; i--) {
        mTexture[i] = mTexture[i - 1];
        mPalette[i] = mPalette[i - 1];
        mTexNo[i] = mTexNo[i - 1];
    }

    mTexture[param_0] = p_tex;
    mPalette[param_0] = NULL;
    mTexNo[param_0] = -1;
    shiftDeleteFlag(param_0, true);
    mUndeleteFlag &= ~(1 << param_0);
    return true;
}

bool J2DTevBlock4::setTexture(u32 param_0, ResTIMG const* p_timg) {
    if (param_0 >= 4) {
        return false;
    }

    u8 used_tlut;
    u8 tlutid = 0;
    if (p_timg != NULL && p_timg->indexTexture) {
        used_tlut = 0;
        for (int i = 0; i < 4; i++) {
            if (i != param_0 && mTexture[i] != NULL) {
                const ResTIMG* timg = mTexture[i]->getTexInfo();
                if (timg != NULL && timg->indexTexture) {
                    int tlutname = mTexture[i]->getTlutName();
                    u8 tlut_no = tlutname - (tlutname >= GX_BIGTLUT0 ? GX_BIGTLUT0 : GX_TLUT0);

                    if (tlut_no < 4) {
                        used_tlut |= 1 << tlut_no;
                    }
                }
            }
        }
        
        for (u8 i = 0; i < 4; i++) {
            if (!(used_tlut & 1 << i)) {
                tlutid = i;
                break;
            }
        }
    }

    if (mTexture[param_0] == NULL) {
        if (p_timg != NULL) {
            mTexture[param_0] = new JUTTexture(p_timg, tlutid);

            if (mTexture[param_0] == NULL) {
                return false;
            }

            mUndeleteFlag |= (1 << param_0);
        } else {
            mUndeleteFlag &= ~(1 << param_0);
        }
    } else if (mUndeleteFlag & (1 << param_0)) {
        if (p_timg != NULL) {
            mTexture[param_0]->storeTIMG(p_timg, tlutid);
        } else {
            delete mTexture[param_0];
            mTexture[param_0] = NULL;
            mUndeleteFlag &= ~(1 << param_0);
        }
    } else {
        mTexture[param_0] = NULL;

        if (p_timg != NULL) {
            mTexture[param_0] = new JUTTexture(p_timg, 0);

            if (mTexture[param_0] == NULL) {
                return false;
            }

            mUndeleteFlag |= (1 << param_0);
        }
    }

    delete mPalette[param_0];
    mPalette[param_0] = NULL;
    mTexNo[param_0] = -1;
    return true;
}

bool J2DTevBlock4::setTexture(u32 param_0, JUTTexture* p_tex) {
    if (param_0 >= 4) {
        return false;
    }

    if (mUndeleteFlag & (1 << param_0)) {
        delete mTexture[param_0];
    }

    mTexture[param_0] = p_tex;
    mUndeleteFlag &= ~(1 << param_0);
    delete mPalette[param_0];
    mPalette[param_0] = NULL;
    mTexNo[param_0] = -1;
    return true;
}

bool J2DTevBlock4::removeTexture(u32 param_0) {
    if (param_0 >= 4) {
        return false;
    }

    if (mUndeleteFlag & (1 << param_0)) {
        delete mTexture[param_0];
    }
    delete mPalette[param_0];

    for (u32 i = param_0; i < 3; i++) {
        mTexture[i] = mTexture[i + 1];
        mPalette[i] = mPalette[i + 1];
        mTexNo[i] = mTexNo[i + 1];
    }

    mTexture[3] = NULL;
    mPalette[3] = NULL;
    mTexNo[3] = -1;
    shiftDeleteFlag(param_0, false);
    return true;
}

bool J2DTevBlock4::setFont(ResFONT* p_font) {
    if (p_font == NULL) {
        return false;
    }

    JUTResFont* resFont = new JUTResFont(p_font, NULL);
    if (resFont == NULL) {
        return false;
    }

    if (mUndeleteFlag & 0x80) {
        delete mFont;
    }

    mFont = resFont;
    mUndeleteFlag = (mUndeleteFlag & 0x7F) | 0x80;
    return true;
}

bool J2DTevBlock4::setFont(JUTFont* p_font) {
    if (p_font == NULL) {
        return false;
    }

    if (mUndeleteFlag & 0x80) {
        delete mFont;
    }

    mFont = (JUTResFont*)p_font;
    mUndeleteFlag = mUndeleteFlag & 0x7F;
    return true;
}

bool J2DTevBlock4::setPalette(u32 param_0, ResTLUT const* p_tlut) {
    if (param_0 >= 4) {
        return false;
    }

    if (p_tlut != NULL) {
        if (mPalette[param_0] == NULL) {
            mPalette[param_0] = new JUTPalette((GXTlut)param_0, (ResTLUT*)p_tlut);

            if (mPalette[param_0] == NULL) {
                return false;
            }
        } else {
            mPalette[param_0]->storeTLUT(GX_TLUT0, (ResTLUT*)p_tlut);
        }

        if (mTexture[param_0] != NULL) {
            mTexture[param_0]->attachPalette(mPalette[param_0]);
        }
    } else {
        delete mPalette[param_0];
        mPalette[param_0] = NULL;
    }

    return true;
}

void J2DTevBlock4::shiftDeleteFlag(u8 param_0, bool param_1) {
    u8 tmpFlags = mUndeleteFlag & 0x80;
    mUndeleteFlag = mUndeleteFlag & 0x7F;

    if (param_1) {
        mUndeleteFlag = (mUndeleteFlag & ((1 << param_0) - 1)) | ((mUndeleteFlag & ~((1 << param_0) - 1)) * 2);
    } else {
        mUndeleteFlag = (mUndeleteFlag & ((1 << param_0) - 1)) | ((mUndeleteFlag & ~((1 << (param_0 + 1)) - 1)) >> 1);
    }

    mUndeleteFlag |= tmpFlags;
}

void J2DTevBlock4::setGX() {
    for (int i = 0; i < 4; i++) {
        loadTexture(GXTexMapID(i), i);
    }
    for (int i = 0; i < mTevStageNum; i++) {
        GXSetTevOrder(GXTevStageID(i), (GXTexCoordID)mTevOrder[i].getTexCoord(), (GXTexMapID)mTevOrder[i].getTexMap(), (GXChannelID)mTevOrder[i].getColor());
    }
    for (int i = 0; i < 4; i++) {
        GXSetTevColorS10(i != 3 ? GXTevRegID(i + 1) : GX_TEVPREV, mTevColor[i]);
    }
    GXSetNumTevStages(mTevStageNum);
    for (int i = 0; i < mTevStageNum; i++) {
        GXSetTevColorIn(GXTevStageID(i), GXTevColorArg(mTevStage[i].getColorA()),
                        GXTevColorArg(mTevStage[i].getColorB()),
                        GXTevColorArg(mTevStage[i].getColorC()),
                        GXTevColorArg(mTevStage[i].getColorD()));
        GXSetTevAlphaIn(GXTevStageID(i), GXTevAlphaArg(mTevStage[i].getAlphaA()),
                        GXTevAlphaArg(mTevStage[i].getAlphaB()),
                        GXTevAlphaArg(mTevStage[i].getAlphaC()),
                        GXTevAlphaArg(mTevStage[i].getAlphaD()));
        GXSetTevColorOp(GXTevStageID(i), GXTevOp(mTevStage[i].getCOp()),
                        GXTevBias(mTevStage[i].getCBias()), GXTevScale(mTevStage[i].getCScale()),
                        mTevStage[i].getCClamp(), GXTevRegID(mTevStage[i].getCReg()));
        GXSetTevAlphaOp(GXTevStageID(i), (GXTevOp)mTevStage[i].getAOp(),
                        GXTevBias(mTevStage[i].getABias()), GXTevScale(mTevStage[i].getAScale()),
                        mTevStage[i].getAClamp(), GXTevRegID(mTevStage[i].getAReg()));
        GXSetTevKColorSel(GXTevStageID(i), mTevKColorSel[i] != 0xff ? GXTevKColorSel(mTevKColorSel[i]) : GX_TEV_KCSEL_1);
        GXSetTevKAlphaSel(GXTevStageID(i), mTevKAlphaSel[i] != 0xff ? GXTevKAlphaSel(mTevKAlphaSel[i]) : GX_TEV_KASEL_1);
    }
    for (int i = 0; i < 4; i++) {
        GXSetTevKColor(GXTevKColorID(i), mTevKColor[i]);
    }
    for (int i = 0; i < mTevStageNum; i++) {
        GXSetTevSwapMode(GXTevStageID(i), GXTevSwapSel(mTevStage[i].getRasSel()), GXTevSwapSel(mTevStage[i].getTexSel()));
    }
    for (int i = 0; i < 4; i++) {
        GXSetTevSwapModeTable(GXTevSwapSel(i), GXTevColorChan(mTevSwapModeTable[i].getR()),
                              GXTevColorChan(mTevSwapModeTable[i].getG()),
                              GXTevColorChan(mTevSwapModeTable[i].getB()),
                              GXTevColorChan(mTevSwapModeTable[i].getA()));
    }
    for (u8 i = 0; i < mTevStageNum; i++) {
        mIndTevStage[i].load(i);
    }
}

void J2DTevBlock4::loadTexture(GXTexMapID texmapID, u32 texIndex) {
    if (texIndex < 4 && mTexture[texIndex] != NULL && mTexture[texIndex]->getTexInfo() != NULL) {
        mTexture[texIndex]->load(texmapID);
    }
}

J2DTevBlock8::J2DTevBlock8() {
    for (int i = 0; i < 8; i++) {
        mTexture[i] = NULL;
    }

    mUndeleteFlag = 0;
    initialize();
}

J2DTevBlock8::~J2DTevBlock8() {
    for (int i = 0; i < 8; i++) {
        if (mUndeleteFlag & (1 << i)) {
            delete mTexture[i];
        }
        delete mPalette[i];
    }

    if (mFontUndeleteFlag) {
        delete mFont;
    }
}

void J2DTevBlock8::initialize() {
    for (int i = 0; i < 8; i++) {
        mTexNo[i] = 0xffff;
    }
    mFontNo = 0xffff;
    for (int i = 0; i < 8; i++) {
        mTevOrder[i].setTevOrderInfo(j2dDefaultTevOrderInfoNull);
    }
    for (int i = 0; i < 4; i++) {
        mTevColor[i] = j2dDefaultTevColor;
    }
    mTevStageNum = 1;
    for (int i = 0; i < 8; i++) {
        mTevStage[i].setStageNo(i);
    }
    for (int i = 0; i < 4; i++) {
        mTevKColor[i] = j2dDefaultTevKColor;
    }
    for (int i = 0; i < 8; i++) {
        mTevKColorSel[i] = 0xff;
        mTevKAlphaSel[i] = 0xff;
    }
    for (int i = 0; i < 4; i++) {
        mTevSwapModeTable[i].setTevSwapModeTableInfo(j2dDefaultTevSwapModeTable);
    }
    for (int i = 0; i < 8; i++) {
        mIndTevStage[i].setIndTevStageInfo(j2dDefaultIndTevStageInfo);
    }
    for (int i = 0; i < 8; i++) {
        mPalette[i] = 0;
    }
    mFont = NULL;
    mFontUndeleteFlag = 0;
}

bool J2DTevBlock8::prepareTexture(u8 param_0) {
    for (u8 i = 0; i < param_0; i++) {
        if (i >= 8) {
            return 0;
        }

        if (mTexture[i] == NULL) {
            mTexture[i] = new JUTTexture();

            if (mTexture[i] == NULL) {
                return false;
            }
            mUndeleteFlag |= (1 << i);
        }
    }

    return true;
}

bool J2DTevBlock8::insertTexture(u32 param_0, ResTIMG const* p_timg, JUTPalette* p_tlut) {
    if (param_0 >= 8 || !p_timg) {
        return false;
    }
    u8 idx;
    for (idx = 0; idx < 8; idx++) {
        if (!mTexture[idx] || !mTexture[idx]->getTexInfo()) {
            break;
        }
    }
    if (idx < param_0) {
        return false;
    }
    if (idx == 8) {
        return false;
    }
    u8 local_43 = 0;
    if (p_timg->indexTexture && !p_tlut) {
        u8 local_44 = 0;
        for (int i = 0; i < 7; i++) {
            if (!mTexture[i]) {
                continue;
            }
            const ResTIMG* texInfo = mTexture[i]->getTexInfo();
            if (!texInfo) {
                continue;
            }
            if (!texInfo->indexTexture) {
                continue;
            }
            int tlutName = mTexture[i]->getTlutName();
            u8 tlut_no = tlutName - (tlutName >= GX_BIGTLUT0 ? GX_BIGTLUT0 : GX_TLUT0);
            if (tlut_no < 8) {
                local_44 |= 1 << tlut_no;
            }
        }
        for (u8 i = 0; i < 8; i++) {
            if ((local_44 & 1 << i) == 0) {
                local_43 = i;
                break;
            }
        }
    }
    if (!mTexture[idx]) {
        JUTTexture* texture = new JUTTexture(p_timg, local_43);
        if (!texture) {
            return false;
        }
        if (p_tlut) {
            texture->storeTIMG(p_timg, p_tlut);
        }
        for (u8 i = 7; i > param_0; i--) {
            mTexture[i] = mTexture[i - 1];
            mPalette[i] = mPalette[i - 1];
            mTexNo[i] = mTexNo[i - 1];
        }
        mTexture[param_0] = texture;
        shiftDeleteFlag(param_0, true);
        mUndeleteFlag |= 1 << param_0;
    } else {
        JUTTexture* texture = mTexture[idx];
        if (!p_tlut) {
            texture->storeTIMG(p_timg, local_43);
        } else {
            texture->storeTIMG(p_timg, p_tlut);
        }
        bool local_38[4];
        for (u8 i = 0; i < 8; i++) {
            local_38[i] = (mUndeleteFlag & 1 << i) != 0;
        }
        for (u8 i = idx; i > param_0; i--) {
            mTexture[i] = mTexture[i - 1];
            mPalette[i] = mPalette[i - 1];
            mTexNo[i] = mTexNo[i - 1];
            local_38[i] = local_38[i - 1];
        }
        mTexture[param_0] = texture;
        mUndeleteFlag = 0;
        for (u8 i = 0; i < 8; i++) {
            if (local_38[i]) {
                mUndeleteFlag |= 1 << i;
            }
        }
        mUndeleteFlag |= 1 << param_0;
    }
    mPalette[param_0] = NULL;
    mTexNo[param_0] = -1;
    return true;
}

bool J2DTevBlock8::insertTexture(u32 param_0, JUTTexture* p_tex) {
    if (param_0 >= 8 || p_tex == NULL) {
        return false;
    }

    u8 texNo = 0;
    for (; texNo < 8; texNo++) {
        if (mTexture[texNo] == NULL || mTexture[texNo]->getTexInfo() == NULL) {
            break;
        }
    }

    if (texNo < param_0) {
        return false;
    }
    
    if (texNo == 8) {
        return false;
    }

    if (mTexture[7] != NULL && mTexture[7]->getTexInfo() == NULL) {
        if (mUndeleteFlag & 0x80) {
            delete mTexture[7];
        }
        
        mUndeleteFlag &= 0x7F;
    }

    for (u8 i = 7; i > param_0; i--) {
        mTexture[i] = mTexture[i - 1];
        mPalette[i] = mPalette[i - 1];
        mTexNo[i] = mTexNo[i - 1];
    }

    mTexture[param_0] = p_tex;
    mPalette[param_0] = NULL;
    mTexNo[param_0] = -1;
    shiftDeleteFlag(param_0, true);
    mUndeleteFlag &= ~(1 << param_0);
    return true;
}

bool J2DTevBlock8::setTexture(u32 param_0, ResTIMG const* p_timg) {
    if (param_0 >= 8) {
        return false;
    }

    u8 used_tlut;
    u8 tlutid = 0;
    if (p_timg != NULL && p_timg->indexTexture) {
        used_tlut = 0;
        for (int i = 0; i < 8; i++) {
            if (i != param_0 && mTexture[i] != NULL) {
                const ResTIMG* timg = mTexture[i]->getTexInfo();
                if (timg != NULL && timg->indexTexture) {
                    int tlutname = mTexture[i]->getTlutName();
                    u8 tlut_no = tlutname - (tlutname >= GX_BIGTLUT0 ? GX_BIGTLUT0 : GX_TLUT0);

                    if (tlut_no < 8) {
                        used_tlut |= 1 << tlut_no;
                    }
                }
            }
        }
        
        for (u8 i = 0; i < 4; i++) {
            if (!(used_tlut & 1 << i)) {
                tlutid = i;
                break;
            }
        }
    }

    if (mTexture[param_0] == NULL) {
        if (p_timg != NULL) {
            mTexture[param_0] = new JUTTexture(p_timg, tlutid);

            if (mTexture[param_0] == NULL) {
                return false;
            }

            mUndeleteFlag |= (1 << param_0);
        } else {
            mUndeleteFlag &= ~(1 << param_0);
        }
    } else if (mUndeleteFlag & (1 << param_0)) {
        if (p_timg != NULL) {
            mTexture[param_0]->storeTIMG(p_timg, tlutid);
        } else {
            delete mTexture[param_0];
            mTexture[param_0] = NULL;
            mUndeleteFlag &= ~(1 << param_0);
        }
    } else {
        mTexture[param_0] = NULL;

        if (p_timg != NULL) {
            mTexture[param_0] = new JUTTexture(p_timg, 0);

            if (mTexture[param_0] == NULL) {
                return false;
            }

            mUndeleteFlag |= (1 << param_0);
        }
    }

    delete mPalette[param_0];
    mPalette[param_0] = NULL;
    mTexNo[param_0] = -1;
    return true;
}

bool J2DTevBlock8::setTexture(u32 param_0, JUTTexture* p_tex) {
    if (param_0 >= 8) {
        return false;
    }

    if (mUndeleteFlag & (1 << param_0)) {
        delete mTexture[param_0];
    }

    mTexture[param_0] = p_tex;
    mUndeleteFlag &= ~(1 << param_0);
    delete mPalette[param_0];
    mPalette[param_0] = NULL;
    mTexNo[param_0] = -1;
    return true;
}

bool J2DTevBlock8::removeTexture(u32 param_0) {
    if (param_0 >= 8) {
        return false;
    }

    if (mUndeleteFlag & (1 << param_0)) {
        delete mTexture[param_0];
    }
    delete mPalette[param_0];

    for (u32 i = param_0; i < 7; i++) {
        mTexture[i] = mTexture[i + 1];
        mPalette[i] = mPalette[i + 1];
        mTexNo[i] = mTexNo[i + 1];
    }

    mTexture[7] = NULL;
    mPalette[7] = NULL;
    mTexNo[7] = -1;
    shiftDeleteFlag(param_0, false);
    return true;
}

bool J2DTevBlock8::setFont(ResFONT* p_font) {
    if (p_font == NULL) {
        return false;
    }

    JUTResFont* resFont = new JUTResFont(p_font, NULL);
    if (resFont == NULL) {
        return false;
    }

    if (mFontUndeleteFlag) {
        delete mFont;
    }

    mFont = resFont;
    mFontUndeleteFlag = true;
    return true;
}

bool J2DTevBlock8::setFont(JUTFont* p_font) {
    if (p_font == NULL) {
        return false;
    }

    if (mFontUndeleteFlag) {
        delete mFont;
    }

    mFont = (JUTResFont*)p_font;
    mFontUndeleteFlag = false;
    return true;
}

bool J2DTevBlock8::setPalette(u32 param_0, ResTLUT const* p_tlut) {
    if (param_0 >= 8) {
        return false;
    }

    if (p_tlut != NULL) {
        if (mPalette[param_0] == NULL) {
            mPalette[param_0] = new JUTPalette((GXTlut)param_0, (ResTLUT*)p_tlut);

            if (mPalette[param_0] == NULL) {
                return false;
            }
        } else {
            mPalette[param_0]->storeTLUT(GX_TLUT0, (ResTLUT*)p_tlut);
        }

        if (mTexture[param_0] != NULL) {
            mTexture[param_0]->attachPalette(mPalette[param_0]);
        }
    } else {
        delete mPalette[param_0];
        mPalette[param_0] = NULL;
    }

    return true;
}

void J2DTevBlock8::shiftDeleteFlag(u8 param_0, bool param_1) {
    if (param_1) {
        mUndeleteFlag = (mUndeleteFlag & ((1 << param_0) - 1)) | ((mUndeleteFlag & ~((1 << param_0) - 1)) * 2);
    } else {
        mUndeleteFlag = (mUndeleteFlag & ((1 << param_0) - 1)) | ((mUndeleteFlag & ~((1 << (param_0 + 1)) - 1)) >> 1);
    }
}

void J2DTevBlock8::setGX() {
    for (int i = 0; i < 8; i++) {
        loadTexture(GXTexMapID(i), i);
    }
    for (int i = 0; i < mTevStageNum; i++) {
        GXSetTevOrder(GXTevStageID(i), (GXTexCoordID)mTevOrder[i].getTexCoord(), (GXTexMapID)mTevOrder[i].getTexMap(), (GXChannelID)mTevOrder[i].getColor());
    }
    for (int i = 0; i < 4; i++) {
        GXSetTevColorS10(i != 3 ? GXTevRegID(i + 1) : GX_TEVPREV, mTevColor[i]);
    }
    GXSetNumTevStages(mTevStageNum);
    for (int i = 0; i < mTevStageNum; i++) {
        GXSetTevColorIn(GXTevStageID(i), GXTevColorArg(mTevStage[i].getColorA()),
                        GXTevColorArg(mTevStage[i].getColorB()),
                        GXTevColorArg(mTevStage[i].getColorC()),
                        GXTevColorArg(mTevStage[i].getColorD()));
        GXSetTevAlphaIn(GXTevStageID(i), GXTevAlphaArg(mTevStage[i].getAlphaA()),
                        GXTevAlphaArg(mTevStage[i].getAlphaB()),
                        GXTevAlphaArg(mTevStage[i].getAlphaC()),
                        GXTevAlphaArg(mTevStage[i].getAlphaD()));
        GXSetTevColorOp(GXTevStageID(i), GXTevOp(mTevStage[i].getCOp()),
                        GXTevBias(mTevStage[i].getCBias()), GXTevScale(mTevStage[i].getCScale()),
                        mTevStage[i].getCClamp(), GXTevRegID(mTevStage[i].getCReg()));
        GXSetTevAlphaOp(GXTevStageID(i), (GXTevOp)mTevStage[i].getAOp(),
                        GXTevBias(mTevStage[i].getABias()), GXTevScale(mTevStage[i].getAScale()),
                        mTevStage[i].getAClamp(), GXTevRegID(mTevStage[i].getAReg()));
        GXSetTevKColorSel(GXTevStageID(i), mTevKColorSel[i] != 0xff ? GXTevKColorSel(mTevKColorSel[i]) : GX_TEV_KCSEL_1);
        GXSetTevKAlphaSel(GXTevStageID(i), mTevKAlphaSel[i] != 0xff ? GXTevKAlphaSel(mTevKAlphaSel[i]) : GX_TEV_KASEL_1);
    }
    for (int i = 0; i < 4; i++) {
        GXSetTevKColor(GXTevKColorID(i), mTevKColor[i]);
    }
    for (int i = 0; i < mTevStageNum; i++) {
        GXSetTevSwapMode(GXTevStageID(i), GXTevSwapSel(mTevStage[i].getRasSel()), GXTevSwapSel(mTevStage[i].getTexSel()));
    }
    for (int i = 0; i < 4; i++) {
        GXSetTevSwapModeTable(GXTevSwapSel(i), GXTevColorChan(mTevSwapModeTable[i].getR()),
                              GXTevColorChan(mTevSwapModeTable[i].getG()),
                              GXTevColorChan(mTevSwapModeTable[i].getB()),
                              GXTevColorChan(mTevSwapModeTable[i].getA()));
    }
    for (u8 i = 0; i < mTevStageNum; i++) {
        mIndTevStage[i].load(i);
    }
}

void J2DTevBlock8::loadTexture(GXTexMapID texmapID, u32 texIndex) {
    if (texIndex < 8 && mTexture[texIndex] != NULL && mTexture[texIndex]->getTexInfo() != NULL) {
        mTexture[texIndex]->load(texmapID);
    }
}

J2DTevBlock16::J2DTevBlock16() {
    for (int i = 0; i < 8; i++) {
        mTexture[i] = NULL;
    }

    mUndeleteFlag = 0;
    initialize();
}

J2DTevBlock16::~J2DTevBlock16() {
    for (int i = 0; i < 8; i++) {
        if (mUndeleteFlag & (1 << i)) {
            delete mTexture[i];
        }
        delete mPalette[i];
    }

    if (mFontUndeleteFlag) {
        delete mFont;
    }
}

void J2DTevBlock16::initialize() {
    for (int i = 0; i < 8; i++) {
        mTexNo[i] = 0xffff;
    }
    mFontNo = 0xffff;
    for (int i = 0; i < 16; i++) {
        mTevOrder[i].setTevOrderInfo(j2dDefaultTevOrderInfoNull);
    }
    for (int i = 0; i < 4; i++) {
        mTevColor[i] = j2dDefaultTevColor;
    }
    mTevStageNum = 1;
    for (int i = 0; i < 16; i++) {
        mTevStage[i].setStageNo(i);
    }
    for (int i = 0; i < 4; i++) {
        mTevKColor[i] = j2dDefaultTevKColor;
    }
    for (int i = 0; i < 16; i++) {
        mTevKColorSel[i] = 0xff;
        mTevKAlphaSel[i] = 0xff;
    }
    for (int i = 0; i < 4; i++) {
        mTevSwapModeTable[i].setTevSwapModeTableInfo(j2dDefaultTevSwapModeTable);
    }
    for (int i = 0; i < 16; i++) {
        mIndTevStage[i].setIndTevStageInfo(j2dDefaultIndTevStageInfo);
    }
    for (int i = 0; i < 8; i++) {
        mPalette[i] = 0;
    }
    mFont = NULL;
    mFontUndeleteFlag = 0;
}

bool J2DTevBlock16::prepareTexture(u8 param_0) {
    for (u8 i = 0; i < param_0; i++) {
        if (i >= 8) {
            return 0;
        }

        if (mTexture[i] == NULL) {
            mTexture[i] = new JUTTexture();

            if (mTexture[i] == NULL) {
                return false;
            }
            mUndeleteFlag |= (1 << i);
        }
    }

    return true;
}

bool J2DTevBlock16::insertTexture(u32 param_0, ResTIMG const* p_timg, JUTPalette* p_tlut) {
    if (param_0 >= 8 || !p_timg) {
        return false;
    }
    u8 idx;
    for (idx = 0; idx < 8; idx++) {
        if (!mTexture[idx] || !mTexture[idx]->getTexInfo()) {
            break;
        }
    }
    if (idx < param_0) {
        return false;
    }
    if (idx == 8) {
        return false;
    }
    u8 local_43 = 0;
    if (p_timg->indexTexture && !p_tlut) {
        u8 local_44 = 0;
        for (int i = 0; i < 7; i++) {
            if (!mTexture[i]) {
                continue;
            }
            const ResTIMG* texInfo = mTexture[i]->getTexInfo();
            if (!texInfo) {
                continue;
            }
            if (!texInfo->indexTexture) {
                continue;
            }
            int tlutName = mTexture[i]->getTlutName();
            u8 tlut_no = tlutName - (tlutName >= GX_BIGTLUT0 ? GX_BIGTLUT0 : GX_TLUT0);
            if (tlut_no < 8) {
                local_44 |= 1 << tlut_no;
            }
        }
        for (u8 i = 0; i < 8; i++) {
            if ((local_44 & 1 << i) == 0) {
                local_43 = i;
                break;
            }
        }
    }
    if (!mTexture[idx]) {
        JUTTexture* texture = new JUTTexture(p_timg, local_43);
        if (!texture) {
            return false;
        }
        if (p_tlut) {
            texture->storeTIMG(p_timg, p_tlut);
        }
        for (u8 i = 7; i > param_0; i--) {
            mTexture[i] = mTexture[i - 1];
            mPalette[i] = mPalette[i - 1];
            mTexNo[i] = mTexNo[i - 1];
        }
        mTexture[param_0] = texture;
        shiftDeleteFlag(param_0, true);
        mUndeleteFlag |= 1 << param_0;
    } else {
        JUTTexture* texture = mTexture[idx];
        if (!p_tlut) {
            texture->storeTIMG(p_timg, local_43);
        } else {
            texture->storeTIMG(p_timg, p_tlut);
        }
        bool local_38[8];
        for (u8 i = 0; i < 8; i++) {
            local_38[i] = (mUndeleteFlag & 1 << i) != 0;
        }
        for (u8 i = idx; i > param_0; i--) {
            mTexture[i] = mTexture[i - 1];
            mPalette[i] = mPalette[i - 1];
            mTexNo[i] = mTexNo[i - 1];
            local_38[i] = local_38[i - 1];
        }
        mTexture[param_0] = texture;
        mUndeleteFlag = 0;
        for (u8 i = 0; i < 8; i++) {
            if (local_38[i]) {
                mUndeleteFlag |= 1 << i;
            }
        }
        mUndeleteFlag |= 1 << param_0;
    }
    mPalette[param_0] = NULL;
    mTexNo[param_0] = -1;
    return true;
}

bool J2DTevBlock16::insertTexture(u32 param_0, JUTTexture* p_tex) {
    if (param_0 >= 8 || p_tex == NULL) {
        return false;
    }

    u8 texNo = 0;
    for (; texNo < 8; texNo++) {
        if (mTexture[texNo] == NULL || mTexture[texNo]->getTexInfo() == NULL) {
            break;
        }
    }

    if (texNo < param_0) {
        return false;
    }
    
    if (texNo == 8) {
        return false;
    }

    if (mTexture[7] != NULL && mTexture[7]->getTexInfo() == NULL) {
        if (mUndeleteFlag & 0x80) {
            delete mTexture[7];
        }
        
        mUndeleteFlag &= 0x7F;
    }

    for (u8 i = 7; i > param_0; i--) {
        mTexture[i] = mTexture[i - 1];
        mPalette[i] = mPalette[i - 1];
        mTexNo[i] = mTexNo[i - 1];
    }

    mTexture[param_0] = p_tex;
    mPalette[param_0] = NULL;
    mTexNo[param_0] = -1;
    shiftDeleteFlag(param_0, true);
    mUndeleteFlag &= ~(1 << param_0);
    return true;
}

bool J2DTevBlock16::setTexture(u32 param_0, ResTIMG const* p_timg) {
    if (param_0 >= 8) {
        return false;
    }

    u8 used_tlut;
    u8 tlutid = 0;
    if (p_timg != NULL && p_timg->indexTexture) {
        used_tlut = 0;
        for (int i = 0; i < 8; i++) {
            if (i != param_0 && mTexture[i] != NULL) {
                const ResTIMG* timg = mTexture[i]->getTexInfo();
                if (timg != NULL && timg->indexTexture) {
                    int tlutname = mTexture[i]->getTlutName();
                    u8 tlut_no = tlutname - (tlutname >= GX_BIGTLUT0 ? GX_BIGTLUT0 : GX_TLUT0);

                    if (tlut_no < 8) {
                        used_tlut |= 1 << tlut_no;
                    }
                }
            }
        }
        
        for (u8 i = 0; i < 4; i++) {
            if (!(used_tlut & 1 << i)) {
                tlutid = i;
                break;
            }
        }
    }

    if (mTexture[param_0] == NULL) {
        if (p_timg != NULL) {
            mTexture[param_0] = new JUTTexture(p_timg, tlutid);

            if (mTexture[param_0] == NULL) {
                return false;
            }

            mUndeleteFlag |= (1 << param_0);
        } else {
            mUndeleteFlag &= ~(1 << param_0);
        }
    } else if (mUndeleteFlag & (1 << param_0)) {
        if (p_timg != NULL) {
            mTexture[param_0]->storeTIMG(p_timg, tlutid);
        } else {
            delete mTexture[param_0];
            mTexture[param_0] = NULL;
            mUndeleteFlag &= ~(1 << param_0);
        }
    } else {
        mTexture[param_0] = NULL;

        if (p_timg != NULL) {
            mTexture[param_0] = new JUTTexture(p_timg, 0);

            if (mTexture[param_0] == NULL) {
                return false;
            }

            mUndeleteFlag |= (1 << param_0);
        }
    }

    delete mPalette[param_0];
    mPalette[param_0] = NULL;
    mTexNo[param_0] = -1;
    return true;
}

bool J2DTevBlock16::setTexture(u32 param_0, JUTTexture* p_tex) {
    if (param_0 >= 8) {
        return false;
    }

    if (mUndeleteFlag & (1 << param_0)) {
        delete mTexture[param_0];
    }

    mTexture[param_0] = p_tex;
    mUndeleteFlag &= ~(1 << param_0);
    delete mPalette[param_0];
    mPalette[param_0] = NULL;
    mTexNo[param_0] = -1;
    return true;
}

bool J2DTevBlock16::removeTexture(u32 param_0) {
    if (param_0 >= 8) {
        return false;
    }

    if (mUndeleteFlag & (1 << param_0)) {
        delete mTexture[param_0];
    }
    delete mPalette[param_0];

    for (u32 i = param_0; i < 7; i++) {
        mTexture[i] = mTexture[i + 1];
        mPalette[i] = mPalette[i + 1];
        mTexNo[i] = mTexNo[i + 1];
    }

    mTexture[7] = NULL;
    mPalette[7] = NULL;
    mTexNo[7] = -1;
    shiftDeleteFlag(param_0, false);
    return true;
}

bool J2DTevBlock16::setFont(ResFONT* p_font) {
    if (p_font == NULL) {
        return false;
    }

    JUTResFont* resFont = new JUTResFont(p_font, NULL);
    if (resFont == NULL) {
        return false;
    }

    if (mFontUndeleteFlag) {
        delete mFont;
    }

    mFont = resFont;
    mFontUndeleteFlag = true;
    return true;
}

bool J2DTevBlock16::setFont(JUTFont* p_font) {
    if (p_font == NULL) {
        return false;
    }

    if (mFontUndeleteFlag) {
        delete mFont;
    }

    mFont = (JUTResFont*)p_font;
    mFontUndeleteFlag = false;
    return true;
}

bool J2DTevBlock16::setPalette(u32 param_0, ResTLUT const* p_tlut) {
    if (param_0 >= 8) {
        return false;
    }

    if (p_tlut != NULL) {
        if (mPalette[param_0] == NULL) {
            mPalette[param_0] = new JUTPalette((GXTlut)param_0, (ResTLUT*)p_tlut);

            if (mPalette[param_0] == NULL) {
                return false;
            }
        } else {
            mPalette[param_0]->storeTLUT(GX_TLUT0, (ResTLUT*)p_tlut);
        }

        if (mTexture[param_0] != NULL) {
            mTexture[param_0]->attachPalette(mPalette[param_0]);
        }
    } else {
        delete mPalette[param_0];
        mPalette[param_0] = NULL;
    }

    return true;
}

void J2DTevBlock16::shiftDeleteFlag(u8 param_0, bool param_1) {
    if (param_1) {
        mUndeleteFlag = (mUndeleteFlag & ((1 << param_0) - 1)) | ((mUndeleteFlag & ~((1 << param_0) - 1)) * 2);
    } else {
        mUndeleteFlag = (mUndeleteFlag & ((1 << param_0) - 1)) | ((mUndeleteFlag & ~((1 << (param_0 + 1)) - 1)) >> 1);
    }
}

void J2DTevBlock16::setGX() {
    for (int i = 0; i < 8; i++) {
        loadTexture(GXTexMapID(i), i);
    }
    for (int i = 0; i < mTevStageNum; i++) {
        GXSetTevOrder(GXTevStageID(i), (GXTexCoordID)mTevOrder[i].getTexCoord(), (GXTexMapID)mTevOrder[i].getTexMap(), (GXChannelID)mTevOrder[i].getColor());
    }
    for (int i = 0; i < 4; i++) {
        GXSetTevColorS10(i != 3 ? GXTevRegID(i + 1) : GX_TEVPREV, mTevColor[i]);
    }
    GXSetNumTevStages(mTevStageNum);
    for (int i = 0; i < mTevStageNum; i++) {
        GXSetTevColorIn(GXTevStageID(i), GXTevColorArg(mTevStage[i].getColorA()),
                        GXTevColorArg(mTevStage[i].getColorB()),
                        GXTevColorArg(mTevStage[i].getColorC()),
                        GXTevColorArg(mTevStage[i].getColorD()));
        GXSetTevAlphaIn(GXTevStageID(i), GXTevAlphaArg(mTevStage[i].getAlphaA()),
                        GXTevAlphaArg(mTevStage[i].getAlphaB()),
                        GXTevAlphaArg(mTevStage[i].getAlphaC()),
                        GXTevAlphaArg(mTevStage[i].getAlphaD()));
        GXSetTevColorOp(GXTevStageID(i), GXTevOp(mTevStage[i].getCOp()),
                        GXTevBias(mTevStage[i].getCBias()), GXTevScale(mTevStage[i].getCScale()),
                        mTevStage[i].getCClamp(), GXTevRegID(mTevStage[i].getCReg()));
        GXSetTevAlphaOp(GXTevStageID(i), (GXTevOp)mTevStage[i].getAOp(),
                        GXTevBias(mTevStage[i].getABias()), GXTevScale(mTevStage[i].getAScale()),
                        mTevStage[i].getAClamp(), GXTevRegID(mTevStage[i].getAReg()));
        GXSetTevKColorSel(GXTevStageID(i), mTevKColorSel[i] != 0xff ? GXTevKColorSel(mTevKColorSel[i]) : GX_TEV_KCSEL_1);
        GXSetTevKAlphaSel(GXTevStageID(i), mTevKAlphaSel[i] != 0xff ? GXTevKAlphaSel(mTevKAlphaSel[i]) : GX_TEV_KASEL_1);
    }
    for (int i = 0; i < 4; i++) {
        GXSetTevKColor(GXTevKColorID(i), mTevKColor[i]);
    }
    for (int i = 0; i < mTevStageNum; i++) {
        GXSetTevSwapMode(GXTevStageID(i), GXTevSwapSel(mTevStage[i].getRasSel()), GXTevSwapSel(mTevStage[i].getTexSel()));
    }
    for (int i = 0; i < 4; i++) {
        GXSetTevSwapModeTable(GXTevSwapSel(i), GXTevColorChan(mTevSwapModeTable[i].getR()),
                              GXTevColorChan(mTevSwapModeTable[i].getG()),
                              GXTevColorChan(mTevSwapModeTable[i].getB()),
                              GXTevColorChan(mTevSwapModeTable[i].getA()));
    }
    for (u8 i = 0; i < mTevStageNum; i++) {
        mIndTevStage[i].load(i);
    }
}

void J2DTevBlock16::loadTexture(GXTexMapID texmapID, u32 texIndex) {
    if (texIndex < 8 && mTexture[texIndex] != NULL && mTexture[texIndex]->getTexInfo() != NULL) {
        mTexture[texIndex]->load(texmapID);
    }
}

void J2DIndBlockFull::initialize() {
    mIndTexStageNum = 0;
    for (int i = 0; i < 4; i++) {
        mIndTexOrder[i] = j2dDefaultIndTexOrderNull;
    }
    for (int i = 0; i < 3; i++) {
        mIndTexMtx[i] = j2dDefaultIndTexMtxInfo;
    }
    for (int i = 0; i < 4; i++) {
        mTexCoordScale[i] = j2dDefaultIndTexCoordScaleInfo;
    }
}

void J2DIndBlockFull::setGX() {
    GXSetNumIndStages(mIndTexStageNum);
    for (u8 i = 0; i < mIndTexStageNum; i++) {
        mIndTexMtx[i].load(i);
    }
    for (u8 i = 0; i < mIndTexStageNum; i++) {
        mTexCoordScale[i].load(i);
    }
    for (u8 i = 0; i < mIndTexStageNum; i++) {
        mIndTexOrder[i].load(i);
    }
}

void J2DPEBlock::initialize() {
    mAlphaComp = J2DAlphaComp();
    mBlend.setBlendInfo(j2dDefaultBlendInfo);
    mDither = j2dDefaultPEBlockDither;
}

void J2DPEBlock::setGX() {
    GXSetAlphaCompare(GXCompare(mAlphaComp.getComp0()), mAlphaComp.getRef0(), GXAlphaOp(mAlphaComp.getOp()), GXCompare(mAlphaComp.getComp1()), mAlphaComp.getRef1());
    GXSetBlendMode(GXBlendMode(mBlend.getType()), GXBlendFactor(mBlend.getSrcFactor()), GXBlendFactor(mBlend.getDstFactor()), GXLogicOp(mBlend.getOp()));
    GXSetDither(mDither);
}
