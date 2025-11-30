//
// J2DTevs
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J2DGraph/J2DTevs.h"
#include "JSystem/J2DGraph/J2DMatBlock.h"
#include "math.h"
#include "dolphin/gx.h"

void J2DTexMtx::load(u32 mtxIdx) {
    GXLoadTexMtxImm(mTexMtx, mtxIdx * 3 + GX_TEXMTX0, mInfo.getTexMtxType());
}

void J2DTexMtx::calc() {
    if (mInfo.mTexMtxDCC == J2DTexMtxInfo::DCC_NONE) {
        getTextureMtx(mInfo.mTexSRTInfo, mInfo.mCenter, mTexMtx);
    } else if (mInfo.mTexMtxDCC == J2DTexMtxInfo::DCC_MAYA) {
        getTextureMtxMaya(mInfo.mTexSRTInfo, mTexMtx);
    }
}

void J2DTexMtx::getTextureMtx(J2DTextureSRTInfo const& param_0, Vec param_1, Mtx param_2) {
    float f31 = (param_0.mRotationDeg * M_PI) / 180.0f;
    param_2[0][0] = param_0.mScaleX * i_cosf(f31);
    param_2[0][1] = -param_0.mScaleX * i_sinf(f31);
    param_2[0][2] = 0.0f;
    param_2[0][3] = -param_0.mScaleX * i_cosf(f31) * param_1.x + param_1.y * (param_0.mScaleX * i_sinf(f31)) +
                    param_1.x + param_0.mTranslationX;
    param_2[1][0] = param_0.mScaleY * i_sinf(f31);
    param_2[1][1] = param_0.mScaleY * i_cosf(f31);
    param_2[1][2] = 0.0f;
    param_2[1][3] = -param_0.mScaleY * i_sinf(f31) * param_1.x - param_1.y * (param_0.mScaleY * i_cosf(f31)) +
                    param_1.y + param_0.mTranslationY;
    param_2[2][0] = 0.0f;
    param_2[2][1] = 0.0f;
    param_2[2][2] = 1.0f;
    param_2[2][3] = 0.0f;
}

void J2DTexMtx::getTextureMtxMaya(J2DTextureSRTInfo const& param_0, Mtx param_1) {
    float f31 = (param_0.mRotationDeg * M_PI) / 180.0f;
    param_1[0][0] = param_0.mScaleX * i_cosf(f31);
    param_1[0][1] = param_0.mScaleY * i_sinf(f31);
    param_1[0][2] = 0.0f;
    param_1[0][3] = (param_0.mTranslationX - 0.5f) * i_cosf(f31) -
                    (param_0.mTranslationY - 0.5f + param_0.mScaleY) * i_sinf(f31) + 0.5f;
    param_1[1][0] = -param_0.mScaleX * i_sinf(f31);
    param_1[1][1] = param_0.mScaleY * i_cosf(f31);
    param_1[1][2] = 0.0f;
    param_1[1][3] = -(param_0.mTranslationX - 0.5f) * i_sinf(f31) -
                    (param_0.mTranslationY - 0.5f + param_0.mScaleY) * i_cosf(f31) + 0.5f;
    param_1[2][0] = 0.0f;
    param_1[2][1] = 0.0f;
    param_1[2][2] = 1.0f;
    param_1[2][3] = 0.0f;
}

void J2DIndTevStage::load(u8 tevStage) {
    GXSetTevIndirect((GXTevStageID)tevStage, getIndStage(), getIndFormat(), getBiasSel(),
                     getMtxSel(), getWrapS(), getWrapT(), getPrev(), getLod(), getAlphaSel());
}

void J2DIndTexMtx::load(u8 indTexMtx) {
    GXSetIndTexMtx((GXIndTexMtxID)(GX_ITM_0 + indTexMtx), mIndTexMtxInfo.mMtx,
                   mIndTexMtxInfo.mScaleExp);
}

void J2DIndTexCoordScale::load(u8 indTexStage) {
    GXSetIndTexCoordScale((GXIndTexStageID)indTexStage, mInfo.getScaleS(), mInfo.getScaleT());
}

void J2DIndTexOrder::load(u8 indTexStage) {
    GXSetIndTexOrder((GXIndTexStageID)indTexStage, mInfo.getTexCoordID(), mInfo.getTexMapID());
}

static void dummyVirtual(J2DTevBlock* block, J2DIndBlock* indBlock) {
    indBlock->getIndTexStageNum();
    indBlock->getIndTexOrder(0);
    indBlock->getIndTexStageNum();
    indBlock->getIndTexOrder(0);
    indBlock->getIndTexMtx(0);
    indBlock->getIndTexCoordScale(0);
    block->getIndTevStage(0);
    block->getTevStageNum();
    block->getTexNo(0);
    block->getFontNo();
    block->getTevKColor(0);
    block->getTevKColorSel(0);
    block->getTevKAlphaSel(0);
    block->getTevOrder(0);
    block->getTevColor(0);
    block->getTevStage(0);
    block->getTevSwapModeTable(0);
}

J2DTexCoordInfo const j2dDefaultTexCoordInfo[8] = {
    {GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, 0}, {GX_TG_MTX2x4, GX_TG_TEX1, GX_IDENTITY, 0},
    {GX_TG_MTX2x4, GX_TG_TEX2, GX_IDENTITY, 0}, {GX_TG_MTX2x4, GX_TG_TEX3, GX_IDENTITY, 0},
    {GX_TG_MTX2x4, GX_TG_TEX4, GX_IDENTITY, 0}, {GX_TG_MTX2x4, GX_TG_TEX5, GX_IDENTITY, 0},
    {GX_TG_MTX2x4, GX_TG_TEX6, GX_IDENTITY, 0}, {GX_TG_MTX2x4, GX_TG_TEX7, GX_IDENTITY, 0},
};

J2DTexMtxInfo const j2dDefaultTexMtxInfo = {1,    1,    255,  255,  0.5f, 0.5f,
                                            0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f};

J2DIndTexMtxInfo const j2dDefaultIndTexMtxInfo = {{0.5f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f}, 1};

extern J2DTevStageInfo const j2dDefaultTevStageInfo = {
    4,          GX_CC_RASC,    GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV,   GX_TEV_ADD,
    GX_TB_ZERO, GX_CS_SCALE_1, 1,          GX_TEVPREV, GX_CA_RASA,    GX_CA_ZERO,
    GX_CA_ZERO, GX_CA_APREV,   GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1,
    GX_TEVPREV};

extern const J2DIndTevStageInfo j2dDefaultIndTevStageInfo = {
    GX_INDTEXSTAGE0, GX_ITB_NONE, GX_ITB_NONE, GX_ITM_OFF,  GX_ITW_OFF,
    GX_ITW_OFF,      0,           0,           GX_ITBA_OFF,
};

extern const GXColor j2dDefaultColInfo = {255, 255, 255, 255};

extern const J2DTevOrderInfo j2dDefaultTevOrderInfoNull = {
    GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL, 0};

extern const J2DIndTexOrderInfo j2dDefaultIndTexOrderNull = {
    GX_TEXCOORD_NULL,
    GX_TEXMAP_NULL,
};

extern const GXColorS10 j2dDefaultTevColor = {255, 255, 255, 255};

extern const J2DIndTexCoordScaleInfo j2dDefaultIndTexCoordScaleInfo = {
    GX_ITS_1,
    GX_ITS_1,
};

extern const GXColor j2dDefaultTevKColor = {255, 255, 255, 255};

extern const J2DTevSwapModeInfo j2dDefaultTevSwapMode = {GX_TEV_SWAP0, GX_TEV_SWAP0, 0, 0};

extern const J2DTevSwapModeTableInfo j2dDefaultTevSwapModeTable = {
    GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA};

extern const J2DBlendInfo j2dDefaultBlendInfo = {GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA,
                                                 GX_LO_NOOP};

extern const u8 j2dDefaultPEBlockDither = 0;

extern const J2DColorChanInfo j2dDefaultColorChanInfo = {0, 3, 0, 0};

extern const u8 data_804561AC = 0x1B;

extern const u16 j2dDefaultAlphaCmp = 0x00E7;
