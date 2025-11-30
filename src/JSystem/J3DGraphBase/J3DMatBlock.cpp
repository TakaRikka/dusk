#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphBase/J3DMatBlock.h"
#include "JSystem/J3DGraphBase/J3DPacket.h"
#include "JSystem/J3DGraphBase/J3DSys.h"
#include "JSystem/J3DGraphBase/J3DTransform.h"
#include "global.h"
#include <string.h>

inline void loadMatColors(const J3DGXColor* color) {
    J3DGDWriteXFCmdHdr(0x100C, 2);
    J3DGDWrite_u32(*(u32*)color);
    J3DGDWrite_u32(*(u32*)(color + 1));
}

inline void loadAmbColors(const J3DGXColor* color) {
    J3DGDWriteXFCmdHdr(0x100A, 2);
    J3DGDWrite_u32(*(u32*)color);
    J3DGDWrite_u32(*(u32*)(color + 1));
}

inline void loadTexCoordScale(GXTexCoordID coord, const J3DTexCoordScaleInfo& info) {
    J3DGDSetTexCoordScale2(coord, info.field_0x00, info.field_0x04 == 1, 0, info.field_0x02, info.field_0x06 == 1, 0);
}

inline void loadTevColor(u32 reg, const J3DGXColorS10& color) {
    J3DGDSetTevColorS10(GXTevRegID(reg + 1), color);
}

inline void loadTevKColor(u32 reg, const J3DGXColor& color) {
    J3DGDSetTevKColor(GXTevKColorID(reg), color);
}

void J3DColorBlockLightOff::initialize() {
    mColorChanNum = 0;
    for (int i = 0; i < ARRAY_SIZE(mMatColor); i++) {
        mMatColor[i] = j3dDefaultColInfo;
    }
    mMatColorOffset = 0;
    mColorChanOffset = 0;
}

void J3DColorBlockAmbientOn::initialize() {
    mColorChanNum = 0;
    for (int i = 0; i < ARRAY_SIZE(mMatColor); i++) {
        mMatColor[i] = j3dDefaultColInfo;
    }
    for (int i = 0; i < ARRAY_SIZE(mAmbColor); i++) {
        mAmbColor[i] = j3dDefaultAmbInfo;
    }
    mMatColorOffset = 0;
    mColorChanOffset = 0;
}

void J3DColorBlockLightOn::initialize() {
    mColorChanNum = 0;
    for (int i = 0; i < ARRAY_SIZE(mMatColor); i++) {
        mMatColor[i] = j3dDefaultColInfo;
    }
    for (int i = 0; i < ARRAY_SIZE(mAmbColor); i++) {
        mAmbColor[i] = j3dDefaultAmbInfo;
    }
    for (int i = 0; i < ARRAY_SIZE(mLight); i++) {
        mLight[i] = NULL;
    }
    mMatColorOffset = 0;
    mColorChanOffset = 0;
}

void J3DTexGenBlockPatched::initialize() {
    mTexGenNum = 0;
    for (int i = 0; i < 8; i++)
        mTexMtx[i] = NULL;
    mTexMtxOffset = 0;
}

void J3DTexGenBlock4::initialize() {
    mTexGenNum = 0;
    for (int i = 0; i < 4; i++)
        mTexMtx[i] = NULL;
    mTexMtxOffset = 0;
}

void J3DTexGenBlockBasic::initialize() {
    mTexGenNum = 0;
    for (int i = 0; i < 8; i++)
        mTexMtx[i] = NULL;
    mTexMtxOffset = 0;
}

void J3DTevBlockNull::initialize() {
    mTexNoOffset = 0;
}

void J3DTevBlockPatched::initialize() {
    for (int i = 0; i < ARRAY_SIZE(mTexNo); i++) {
        mTexNo[i] = 0xFFFF;
    }
    for (int i = 0; i < ARRAY_SIZEU(mTevStage); i++) {
        mTevStage[i].setStageNo(i);
    }
    for (int i = 0; i < 3; i++) {
        mTevColor[i] = j3dDefaultTevColor;
    }
    for (int i = 0; i < ARRAY_SIZE(mTevKColor); i++) {
        mTevKColor[i] = j3dDefaultTevKColor;
    }
    for (int i = 0; i < ARRAY_SIZE(mTevKColorSel); i++) {
        mTevKColorSel[i] = GX_TEV_KCSEL_K0;
    }

    mTevStageNum = 1;
    mTexNoOffset = 0;
    mTevRegOffset = 0;
}

void J3DTevBlock1::initialize() {
    mTexNo[0] = 0xFFFF;
    mTevStage[0].setStageNo(0);
    mTexNoOffset = 0;
}

void J3DTevBlock2::initialize() {
    mTexNo[0] = 0xFFFF;
    mTexNo[1] = 0xFFFF;
    mTevStageNum = 1;

    mTevStage[0].setStageNo(0);
    mTevStage[1].setStageNo(1);
    mTevKColorSel[0] = GX_TEV_KCSEL_K0;
    mTevKColorSel[1] = GX_TEV_KCSEL_K0;
    mTevKAlphaSel[0] = GX_TEV_KASEL_K0_A;
    mTevKAlphaSel[1] = GX_TEV_KASEL_K0_A;

    for (int i = 0; i < 3; i++) {
        mTevColor[i] = j3dDefaultTevColor;
    }
    for (int i = 0; i < ARRAY_SIZE(mTevKColor); i++) {
        mTevKColor[i] = j3dDefaultTevKColor;
    }

    mTexNoOffset = 0;
    mTevRegOffset = 0;
}

void J3DTevBlock4::initialize() {
    mTexNo[0] = 0xFFFF;
    mTexNo[1] = 0xFFFF;
    mTexNo[2] = 0xFFFF;
    mTexNo[3] = 0xFFFF;
    mTevStageNum = 1;

    mTevStage[0].setStageNo(0);
    mTevStage[1].setStageNo(1);
    mTevStage[2].setStageNo(2);
    mTevStage[3].setStageNo(3);

    mTevKColorSel[0] = GX_TEV_KCSEL_K0;
    mTevKColorSel[1] = GX_TEV_KCSEL_K0;
    mTevKColorSel[2] = GX_TEV_KCSEL_K0;
    mTevKColorSel[3] = GX_TEV_KCSEL_K0;
    mTevKAlphaSel[0] = GX_TEV_KASEL_K0_A;
    mTevKAlphaSel[1] = GX_TEV_KASEL_K0_A;
    mTevKAlphaSel[2] = GX_TEV_KASEL_K0_A;
    mTevKAlphaSel[3] = GX_TEV_KASEL_K0_A;

    for (int i = 0; i < 3; i++) {
        mTevColor[i] = j3dDefaultTevColor;
    }
    for (int i = 0; i < ARRAY_SIZE(mTevKColor); i++) {
        mTevKColor[i] = j3dDefaultTevKColor;
    }

    mTexNoOffset = 0;
    mTevRegOffset = 0;
}

void J3DTevBlock16::initialize() {
    for (int i = 0; i < ARRAY_SIZE(mTexNo); i++) {
        mTexNo[i] = 0xFFFF;
    }

    mTevStageNum = 1;

    for (int i = 0; i < 3; i++) {
        mTevColor[i] = j3dDefaultTevColor;
    }
    for (int i = 0; i < ARRAY_SIZE(mTevKColor); i++) {
        mTevKColor[i] = j3dDefaultTevKColor;
    }
    for (int i = 0; i < ARRAY_SIZE(mTevKColorSel); i++) {
        mTevKColorSel[i] = GX_TEV_KCSEL_K0;
    }
    for (int i = 0; i < ARRAY_SIZE(mTevKColorSel); i++) {
        mTevKAlphaSel[i] = GX_TEV_KASEL_K0_A;
    }
    for (int i = 0; i < ARRAY_SIZEU(mTevStage); i++) {
        mTevStage[i].setStageNo(i);
    }

    mTexNoOffset = 0;
    mTevRegOffset = 0;
}

void J3DIndBlockFull::initialize() {
    mIndTexStageNum = 0;
}

void J3DPEBlockFogOff::initialize() {
    mAlphaComp = 0xFFFF;
    mZMode = 0xFFFF;
    mZCompLoc = 0xFF;
    mDither = 1;
}

void J3DPEBlockFull::initialize() {
    mAlphaComp = 0xFFFF;
    mZMode = 0xFFFF;
    mZCompLoc = 0xFF;
    mDither = 1;
    mFogOffset = 0;
}

s32 J3DColorBlockLightOff::countDLSize() {
    return 34;
}

s32 J3DColorBlockAmbientOn::countDLSize() {
    return 47;
}

s32 J3DColorBlockLightOn::countDLSize() {
    return 623;
}

s32 J3DTexGenBlockPatched::countDLSize() {
    return 424;
}

s32 J3DTexGenBlock4::countDLSize() {
    return 254;
}

s32 J3DTexGenBlockBasic::countDLSize() {
    return 498;
}

s32 J3DTevBlockPatched::countDLSize() {
    return 560;
}

s32 J3DTevBlock1::countDLSize() {
    return 105;
}

s32 J3DTevBlock2::countDLSize() {
    return 335;
}

s32 J3DTevBlock4::countDLSize() {
    return 580;
}

s32 J3DTevBlock16::countDLSize() {
    return 1000;
}

s32 J3DIndBlockFull::countDLSize() {
    return 140;
}

s32 J3DPEBlockOpa::countDLSize() {
    return 30;
}

s32 J3DPEBlockTexEdge::countDLSize() {
    return 30;
}

s32 J3DPEBlockXlu::countDLSize() {
    return 30;
}

s32 J3DPEBlockFogOff::countDLSize() {
    return 30;
}

s32 J3DPEBlockFull::countDLSize() {
    return 85;
}

static u32 SizeOfLoadMatColors = 13;

static u32 SizeOfLoadAmbColors = 13;

static u32 SizeOfLoadColorChans = 21;

static u8 sdata_padding[4] = {};

static u32 SizeOfJ3DColorBlockLightOffLoad = SizeOfLoadMatColors + SizeOfLoadColorChans;

void J3DColorBlockLightOff::load() {
    GDOverflowCheck(SizeOfJ3DColorBlockLightOffLoad);
    mMatColorOffset = GDGetCurrOffset();
    loadMatColors(mMatColor);
    mColorChanOffset = GDGetCurrOffset();
    J3DGDWriteXFCmdHdr(XF_REG_COLOR0CNTRL_ID, 4);

    mColorChan[0].load();
    mColorChan[2].load();
    mColorChan[1].load();
    mColorChan[3].load();
}

static u32 SizeOfJ3DColorBlockAmbientOnLoad = SizeOfLoadMatColors + SizeOfLoadAmbColors + SizeOfLoadColorChans;

void J3DColorBlockAmbientOn::load() {
    GDOverflowCheck(SizeOfJ3DColorBlockAmbientOnLoad);
    mMatColorOffset = GDGetCurrOffset();
    loadMatColors(mMatColor);
    loadAmbColors(mAmbColor);
    mColorChanOffset = GDGetCurrOffset();
    J3DGDWriteXFCmdHdr(XF_REG_COLOR0CNTRL_ID, 4);

    mColorChan[0].load();
    mColorChan[2].load();
    mColorChan[1].load();
    mColorChan[3].load();
}

void J3DColorBlockLightOn::load() {
    GDOverflowCheck(SizeOfJ3DColorBlockAmbientOnLoad);
    mMatColorOffset = GDGetCurrOffset();
    loadMatColors(mMatColor);
    loadAmbColors(mAmbColor);
    mColorChanOffset = GDGetCurrOffset();
    J3DGDWriteXFCmdHdr(XF_REG_COLOR0CNTRL_ID, 4);

    mColorChan[0].load();
    mColorChan[2].load();
    mColorChan[1].load();
    mColorChan[3].load();

    for (u32 i = 0; i < 8; i++) {
        if (mLight[i] != NULL) {
            mLight[i]->load(i);
        }
    }
}

void J3DColorBlockLightOff::patch() {
    patchMatColor();
    patchLight();
}

void J3DColorBlockLightOff::patchMatColor() {
    GDSetCurrOffset(mMatColorOffset);
    void* start = GDGetCurrPointer();
    GDOverflowCheck(SizeOfLoadMatColors);
    loadMatColors(mMatColor);
    void* end = GDGetCurrPointer();
    DCStoreRange(start, (uintptr_t)end - (uintptr_t)start);
}

void J3DColorBlockLightOff::patchLight() {
    GDSetCurrOffset(mColorChanOffset);
    void* start = GDGetCurrPointer();
    GDOverflowCheck(SizeOfLoadColorChans);
    J3DGDWriteXFCmdHdr(XF_REG_COLOR0CNTRL_ID, 4);

    mColorChan[0].load();
    mColorChan[2].load();
    mColorChan[1].load();
    mColorChan[3].load();

    void* end = GDGetCurrPointer();
    DCStoreRange(start, (uintptr_t)end - (uintptr_t)start);
}

void J3DColorBlockLightOn::patch() {
    patchMatColor();
    patchLight();
}

void J3DColorBlockLightOn::patchMatColor() {
    GDSetCurrOffset(mMatColorOffset);
    void* start = GDGetCurrPointer();
    GDOverflowCheck(SizeOfLoadMatColors);
    loadMatColors(mMatColor);
    void* end = GDGetCurrPointer();
    DCStoreRange(start, (uintptr_t)end - (uintptr_t)start);
}

void J3DColorBlockLightOn::patchLight() {
    GDSetCurrOffset(mColorChanOffset);
    void* start = GDGetCurrPointer();
    GDOverflowCheck(SizeOfLoadColorChans);
    J3DGDWriteXFCmdHdr(XF_REG_COLOR0CNTRL_ID, 4);

    mColorChan[0].load();
    mColorChan[2].load();
    mColorChan[1].load();
    mColorChan[3].load();

    for (u32 i = 0; i < 8; i++) {
        if (mLight[i] != NULL) {
            mLight[i]->load(i);
        }
    }

    void* end = GDGetCurrPointer();
    DCStoreRange(start, (uintptr_t)end - (uintptr_t)start);
}

void J3DColorBlockLightOff::diff(u32 diffFlags) {
    if (diffFlags & J3DDiffFlag_MatColor)
        diffMatColor();

    if (diffFlags & J3DDiffFlag_ColorChan)
        diffColorChan();
}

void J3DColorBlockLightOff::diffMatColor() {
    GDOverflowCheck(SizeOfLoadMatColors);
    loadMatColors(mMatColor);
}

void J3DColorBlockLightOff::diffColorChan() {
    GDOverflowCheck(SizeOfLoadColorChans);
    J3DGDWriteXFCmdHdr(XF_REG_COLOR0CNTRL_ID, 4);
    mColorChan[0].load();
    mColorChan[2].load();
    mColorChan[1].load();
    mColorChan[3].load();
}

void J3DColorBlockLightOn::diff(u32 diffFlags) {
    if (diffFlags & J3DDiffFlag_AmbColor)
        diffAmbColor();

    if (diffFlags & J3DDiffFlag_MatColor)
        diffMatColor();
    
    if (diffFlags & J3DDiffFlag_ColorChan)
        diffColorChan();

    s32 lightObjNum = getDiffFlag_LightObjNum(diffFlags);
    if (lightObjNum != 0)
        diffLightObj(lightObjNum);
}

void J3DColorBlockLightOn::diffAmbColor() {
    GDOverflowCheck(SizeOfLoadAmbColors);
    loadAmbColors(mAmbColor);
}

void J3DColorBlockLightOn::diffMatColor() {
    GDOverflowCheck(SizeOfLoadMatColors);
    loadMatColors(mMatColor);
}

void J3DColorBlockLightOn::diffColorChan() {
    GDOverflowCheck(SizeOfLoadColorChans);
    J3DGDWriteXFCmdHdr(XF_REG_COLOR0CNTRL_ID, 4);
    mColorChan[0].load();
    mColorChan[2].load();
    mColorChan[1].load();
    mColorChan[3].load();
}

void J3DColorBlockLightOn::diffLightObj(u32 lightObjNum) {
    for (u32 i = 0; i < lightObjNum; i ++) {
        if (mLight[i] != NULL) {
            mLight[i]->load(i);
        }
    }
}

void J3DTexGenBlock4::load() {
    mTexMtxOffset = GDGetCurrOffset();
    for (u32 i = 0; i < 4; i++) {
        if (mTexMtx[i] && mTexCoord[i].getTexGenMtx() != GX_IDENTITY) {
            mTexMtx[i]->load(i);
        }
    }

    if (mTexGenNum != 0) {
        loadTexCoordGens(mTexGenNum, mTexCoord);
    }
}

void J3DTexGenBlockBasic::load() {
    mTexMtxOffset = GDGetCurrOffset();
    for (u32 i = 0; i < 8; i++) {
        if (mTexMtx[i] && mTexCoord[i].getTexGenMtx() != GX_IDENTITY) {
            mTexMtx[i]->load(i);
        }
    }

    if (mTexGenNum != 0) {
        loadTexCoordGens(mTexGenNum, mTexCoord);
    }
}

void J3DTexGenBlockPatched::patch() {
    GDSetCurrOffset(mTexMtxOffset);
    void* start = GDGetCurrPointer();

    for (u32 i = 0; i < 8; i++) {
        if (mTexMtx[i]) {
            mTexMtx[i]->load(i);
        }
    }

    void* end = GDGetCurrPointer();
    DCStoreRange(start, (uintptr_t)end - (uintptr_t)start);
}

void J3DTexGenBlock4::patch() {
    GDSetCurrOffset(mTexMtxOffset);
    void* start = GDGetCurrPointer();

    for (u32 i = 0; i < 4; i++) {
        if (mTexMtx[i] && mTexCoord[i].getTexGenMtx() != GX_IDENTITY) {
            mTexMtx[i]->load(i);
        }
    }

    void* end = GDGetCurrPointer();
    DCStoreRange(start, (uintptr_t)end - (uintptr_t)start);
}

void J3DTexGenBlockBasic::patch() {
    GDSetCurrOffset(mTexMtxOffset);
    void* start = GDGetCurrPointer();

    for (u32 i = 0; i < 8; i++) {
        if (mTexMtx[i] && mTexCoord[i].getTexGenMtx() != GX_IDENTITY) {
            mTexMtx[i]->load(i);
        }
    }

    void* end = GDGetCurrPointer();
    DCStoreRange(start, (uintptr_t)end - (uintptr_t)start);
}

void J3DTexGenBlockPatched::diff(u32 diffFlags) {
    if (getDiffFlag_TexGenNum(diffFlags)) {
        diffTexMtx();

        if (diffFlags & J3DDiffFlag_TexGen) {
            diffTexGen();
        }
    }
}

void J3DTexGenBlockPatched::diffTexMtx() {
    for (u32 i = 0; i < ARRAY_SIZEU(mTexMtx); ++i) {
        if (mTexMtx[i] != NULL) {
            mTexMtx[i]->load(i);
        }
    }
}

void J3DTexGenBlockPatched::diffTexGen() {
    if (mTexGenNum > 0) {
        loadTexCoordGens(mTexGenNum, mTexCoord);
    }
}

void J3DTevBlock1::load() {
    mTexNoOffset = GDGetCurrOffset();
    GDOverflowCheck(0x69);

    if (mTexNo[0] != 0xffff) {
        loadTexNo(0, mTexNo[0]);
    }

    J3DGDSetTevOrder(
        GX_TEVSTAGE0,
        GXTexCoordID(mTevOrder[0].getTevOrderInfo().mTexCoord),
        GXTexMapID(mTevOrder[0].getTevOrderInfo().mTexMap),
        GXChannelID(mTevOrder[0].getTevOrderInfo().mColorChan),
        GX_TEXCOORD_NULL,
        GX_TEXMAP_NULL,
        GX_COLOR_NULL
    );

    loadTexCoordScale(
        GXTexCoordID(mTevOrder[0].getTevOrderInfo().mTexCoord),
        J3DSys::sTexCoordScaleTable[mTevOrder[0].getTevOrderInfo().mTexMap & 7]
    );

    mTevStage[0].load(0);
    mIndTevStage[0].load(0);
}

void J3DTevBlock2::load() {
    u32 tevStageNum = mTevStageNum;
    mTexNoOffset = GDGetCurrOffset();

    for (u32 i = 0; i < 2; i++) {
        if (mTexNo[i] != 0xffff) {
            loadTexNo(i, mTexNo[i]);
        }
    }

    J3DGDSetTevOrder(
        GX_TEVSTAGE0,
        GXTexCoordID(mTevOrder[0].getTevOrderInfo().mTexCoord),
        GXTexMapID(mTevOrder[0].getTevOrderInfo().mTexMap),
        GXChannelID(mTevOrder[0].getTevOrderInfo().mColorChan),
        GXTexCoordID(mTevOrder[1].getTevOrderInfo().mTexCoord),
        GXTexMapID(mTevOrder[1].getTevOrderInfo().mTexMap),
        GXChannelID(mTevOrder[1].getTevOrderInfo().mColorChan)
    );

    loadTexCoordScale(
        GXTexCoordID(mTevOrder[0].getTevOrderInfo().mTexCoord),
        J3DSys::sTexCoordScaleTable[mTevOrder[0].getTevOrderInfo().mTexMap & 7]
    );

    loadTexCoordScale(
        GXTexCoordID(mTevOrder[1].getTevOrderInfo().mTexCoord & 7),
        J3DSys::sTexCoordScaleTable[mTevOrder[1].getTevOrderInfo().mTexMap & 7]
    );

    mTevRegOffset = GDGetCurrOffset();

    for (u32 i = 0; i < 3; i++) {
        loadTevColor(i, mTevColor[i]);
    }
    for (u32 i = 0; i < 4; i++) {
        loadTevKColor(i, mTevKColor[i]);
    }
    for (u32 i = 0; i < tevStageNum; i++) {
        mTevStage[i].load(i);
        mIndTevStage[i].load(i);
    }

    for (u32 i = 0; i < 16; i += 4) {
        J3DGDSetTevKonstantSel_SwapModeTable(
            GXTevStageID(i),
            GXTevKColorSel(mTevKColorSel[0]),
            GXTevKAlphaSel(mTevKAlphaSel[0]),
            GXTevKColorSel(mTevKColorSel[1]),
            GXTevKAlphaSel(mTevKAlphaSel[1]),
            GXTevColorChan(mTevSwapModeTable[i / 4].getR()),
            GXTevColorChan(mTevSwapModeTable[i / 4].getG())
        );

        J3DGDSetTevKonstantSel_SwapModeTable(
            GXTevStageID(i + 2),
            GXTevKColorSel(mTevKColorSel[0]),
            GXTevKAlphaSel(mTevKAlphaSel[0]),
            GXTevKColorSel(mTevKColorSel[1]),
            GXTevKAlphaSel(mTevKAlphaSel[1]),
            GXTevColorChan(mTevSwapModeTable[i / 4].getB()),
            GXTevColorChan(mTevSwapModeTable[i / 4].getA())
        );
    }
}

void J3DTevBlock4::load() {
    u32 tevStageNum = mTevStageNum;
    mTexNoOffset = GDGetCurrOffset();

    for (u32 i = 0; i < 4; i++) {
        if (mTexNo[i] != 0xffff) {
            loadTexNo(i, mTexNo[i]);
        }
    }

    for (u32 i = 0; i < tevStageNum; i += 2) {
        J3DGDSetTevOrder(
            GXTevStageID(i),
            GXTexCoordID(mTevOrder[i].getTevOrderInfo().mTexCoord),
            GXTexMapID(mTevOrder[i].getTevOrderInfo().mTexMap),
            GXChannelID(mTevOrder[i].getTevOrderInfo().mColorChan),
            GXTexCoordID(mTevOrder[i + 1].getTevOrderInfo().mTexCoord),
            GXTexMapID(mTevOrder[i + 1].getTevOrderInfo().mTexMap),
            GXChannelID(mTevOrder[i + 1].getTevOrderInfo().mColorChan)
        );

        loadTexCoordScale(
            GXTexCoordID(mTevOrder[i].getTevOrderInfo().mTexCoord & 7),
            J3DSys::sTexCoordScaleTable[mTevOrder[i].getTevOrderInfo().mTexMap & 7]
        );

        loadTexCoordScale(
            GXTexCoordID(mTevOrder[i + 1].getTevOrderInfo().mTexCoord & 7),
            J3DSys::sTexCoordScaleTable[mTevOrder[i + 1].getTevOrderInfo().mTexMap & 7]
        );
    }

    mTevRegOffset = GDGetCurrOffset();

    for (u32 i = 0; i < 3; i++) {
        loadTevColor(i, mTevColor[i]);
    }
    for (u32 i = 0; i < 4; i++) {
        loadTevKColor(i, mTevKColor[i]);
    }
    for (u32 i = 0; i < tevStageNum; i++) {
        mTevStage[i].load(i);
        mIndTevStage[i].load(i);
    }

    for (u32 i = 0; i < 16; i += 4) {
        J3DGDSetTevKonstantSel_SwapModeTable(
            GXTevStageID(i),
            GXTevKColorSel(mTevKColorSel[0]),
            GXTevKAlphaSel(mTevKAlphaSel[0]),
            GXTevKColorSel(mTevKColorSel[1]),
            GXTevKAlphaSel(mTevKAlphaSel[1]),
            GXTevColorChan(mTevSwapModeTable[i / 4].getR()),
            GXTevColorChan(mTevSwapModeTable[i / 4].getG())
        );

        J3DGDSetTevKonstantSel_SwapModeTable(
            GXTevStageID(i + 2),
            GXTevKColorSel(mTevKColorSel[2]),
            GXTevKAlphaSel(mTevKAlphaSel[2]),
            GXTevKColorSel(mTevKColorSel[3]),
            GXTevKAlphaSel(mTevKAlphaSel[3]),
            GXTevColorChan(mTevSwapModeTable[i / 4].getB()),
            GXTevColorChan(mTevSwapModeTable[i / 4].getA())
        );
    }
}

void J3DTevBlock16::load() {
    u32 tevStageNum = mTevStageNum;
    mTexNoOffset = GDGetCurrOffset();

    for (u32 i = 0; i < 8; i++) {
        if (mTexNo[i] != 0xffff) {
            loadTexNo(i, mTexNo[i]);
        }
    }

    for (u32 i = 0; i < tevStageNum; i += 2) {
        J3DGDSetTevOrder(
            GXTevStageID(i),
            GXTexCoordID(mTevOrder[i].getTevOrderInfo().mTexCoord),
            GXTexMapID(mTevOrder[i].getTevOrderInfo().mTexMap),
            GXChannelID(mTevOrder[i].getTevOrderInfo().mColorChan),
            GXTexCoordID(mTevOrder[i + 1].getTevOrderInfo().mTexCoord),
            GXTexMapID(mTevOrder[i + 1].getTevOrderInfo().mTexMap),
            GXChannelID(mTevOrder[i + 1].getTevOrderInfo().mColorChan)
        );

        loadTexCoordScale(
            GXTexCoordID(mTevOrder[i].getTevOrderInfo().mTexCoord & 7),
            J3DSys::sTexCoordScaleTable[mTevOrder[i].getTevOrderInfo().mTexMap & 7]
        );

        loadTexCoordScale(
            GXTexCoordID(mTevOrder[i + 1].getTevOrderInfo().mTexCoord & 7),
            J3DSys::sTexCoordScaleTable[mTevOrder[i + 1].getTevOrderInfo().mTexMap & 7]
        );
    }

    mTevRegOffset = GDGetCurrOffset();

    for (u32 i = 0; i < 3; i++) {
        loadTevColor(i, mTevColor[i]);
    }
    for (u32 i = 0; i < 4; i++) {
        loadTevKColor(i, mTevKColor[i]);
    }
    for (u32 i = 0; i < tevStageNum; i++) {
        mTevStage[i].load(i);
        mIndTevStage[i].load(i);
    }

    for (u32 i = 0; i < 16; i += 4) {
        J3DGDSetTevKonstantSel_SwapModeTable(
            GXTevStageID(i),
            GXTevKColorSel(mTevKColorSel[i]),
            GXTevKAlphaSel(mTevKAlphaSel[i]),
            GXTevKColorSel(mTevKColorSel[i + 1]),
            GXTevKAlphaSel(mTevKAlphaSel[i + 1]),
            GXTevColorChan(mTevSwapModeTable[i / 4].getR()),
            GXTevColorChan(mTevSwapModeTable[i / 4].getG())
        );

        J3DGDSetTevKonstantSel_SwapModeTable(
            GXTevStageID(i + 2),
            GXTevKColorSel(mTevKColorSel[i + 2]),
            GXTevKAlphaSel(mTevKAlphaSel[i + 2]),
            GXTevKColorSel(mTevKColorSel[i + 3]),
            GXTevKAlphaSel(mTevKAlphaSel[i + 3]),
            GXTevColorChan(mTevSwapModeTable[i / 4].getB()),
            GXTevColorChan(mTevSwapModeTable[i / 4].getA())
        );
    }
}


void J3DTevBlockPatched::patchTexNo() {
    GDSetCurrOffset(mTexNoOffset);
    void* start = GDGetCurrPointer();

    for (u32 i = 0; i < 8; i++) {
        if (mTexNo[i] != 0xffff) {
            loadTexNo(i, mTexNo[i]);
        }
    }

    void* end = GDGetCurrPointer();
    DCStoreRange(start, (uintptr_t)end - (uintptr_t)start);
}

void J3DTevBlockPatched::patchTevReg() {
    GDSetCurrOffset(mTevRegOffset);
    void* start = GDGetCurrPointer();

    for (u32 i = 0; i < ARRAY_SIZEU(mTevColor) - 1; i++) {
        loadTevColor(i, mTevColor[i]);
    }
    for (u32 i = 0; i < ARRAY_SIZEU(mTevKColor); i++) {
        loadTevKColor(i, mTevKColor[i]);
    }

    void* end = GDGetCurrPointer();
    DCStoreRange(start, (uintptr_t)end - (uintptr_t)start);
}

void J3DTevBlockPatched::patchTexNoAndTexCoordScale() {
    GDSetCurrOffset(mTexNoOffset);
    void* start = GDGetCurrPointer();

    u32 tevStageNum = mTevStageNum;
    for (u32 i = 0; i < 8; i++) {
        if (mTexNo[i] != 0xffff) {
            loadTexNo(i, mTexNo[i]);
        }
    }

    for (u32 i = 0; i < tevStageNum; i += 2) {
        J3DGDSetTevOrder(
            GXTevStageID(i),
            GXTexCoordID(mTevOrder[i].getTevOrderInfo().mTexCoord),
            GXTexMapID(mTevOrder[i].getTevOrderInfo().mTexMap),
            GXChannelID(mTevOrder[i].getTevOrderInfo().mColorChan),
            GXTexCoordID(mTevOrder[i + 1].getTevOrderInfo().mTexCoord),
            GXTexMapID(mTevOrder[i + 1].getTevOrderInfo().mTexMap),
            GXChannelID(mTevOrder[i + 1].getTevOrderInfo().mColorChan)
        );

        loadTexCoordScale(
            GXTexCoordID(mTevOrder[i].getTevOrderInfo().mTexCoord & 7),
            J3DSys::sTexCoordScaleTable[mTevOrder[i].getTevOrderInfo().mTexMap & 7]
        );

        loadTexCoordScale(
            GXTexCoordID(mTevOrder[i + 1].getTevOrderInfo().mTexCoord & 7),
            J3DSys::sTexCoordScaleTable[mTevOrder[i + 1].getTevOrderInfo().mTexMap & 7]
        );
    }

    void* end = GDGetCurrPointer();
    DCStoreRange(start, (uintptr_t)end - (uintptr_t)start);
}

void J3DTevBlockPatched::patch() {
    patchTexNo();
    patchTevReg();
}

void J3DTevBlock1::patchTexNo() {
    GDSetCurrOffset(mTexNoOffset);
    void* start = GDGetCurrPointer();

    if (mTexNo[0] != 0xFFFF) {
        loadTexNo(0, mTexNo[0]);
    }

    void* end = GDGetCurrPointer();
    DCStoreRange(start, (uintptr_t)end - (uintptr_t)start);
}

void J3DTevBlock1::patchTevReg() {}

void J3DTevBlock1::patchTexNoAndTexCoordScale() {
    GDSetCurrOffset(mTexNoOffset);
    void* start = GDGetCurrPointer();

    if (mTexNo[0] != 0xffff) {
        loadTexNo(0, mTexNo[0]);
    }

    J3DGDSetTevOrder(
        GX_TEVSTAGE0,
        GXTexCoordID(mTevOrder[0].getTevOrderInfo().mTexCoord),
        GXTexMapID(mTevOrder[0].getTevOrderInfo().mTexMap),
        GXChannelID(mTevOrder[0].getTevOrderInfo().mColorChan),
        GX_TEXCOORD_NULL,
        GX_TEXMAP_NULL,
        GX_COLOR_NULL
    );

    loadTexCoordScale(
        GXTexCoordID(mTevOrder[0].getTevOrderInfo().mTexCoord),
        J3DSys::sTexCoordScaleTable[mTevOrder[0].getTevOrderInfo().mTexMap & 7]
    );

    void* end = GDGetCurrPointer();
    DCStoreRange(start, (uintptr_t)end - (uintptr_t)start);
}

void J3DTevBlock1::patch() {
    patchTexNo();
}

void J3DTevBlock2::patchTexNo() {
    GDSetCurrOffset(mTexNoOffset);
    void* start = GDGetCurrPointer();

    for (u32 i = 0; i < 2; i++) {
        if (mTexNo[i] != 0xffff) {
            loadTexNo(i, mTexNo[i]);
        }
    }

    void* end = GDGetCurrPointer();
    DCStoreRange(start, (uintptr_t)end - (uintptr_t)start);
}

void J3DTevBlock2::patchTevReg() {
    GDSetCurrOffset(mTevRegOffset);
    void* start = GDGetCurrPointer();

    for (u32 i = 0; i < ARRAY_SIZEU(mTevColor) - 1; i++) {
        loadTevColor(i, mTevColor[i]);
    }
    for (u32 i = 0; i < ARRAY_SIZEU(mTevKColor); i++) {
        loadTevKColor(i, mTevKColor[i]);
    }

    void* end = GDGetCurrPointer();
    DCStoreRange(start, (uintptr_t)end - (uintptr_t)start);
}

void J3DTevBlock2::patchTexNoAndTexCoordScale() {
    GDSetCurrOffset(mTexNoOffset);
    void* start = GDGetCurrPointer();

    for (u32 i = 0; i < 2; i++) {
        if (mTexNo[i] != 0xffff) {
            loadTexNo(i, mTexNo[i]);
        }
    }

    J3DGDSetTevOrder(
        GX_TEVSTAGE0,
        GXTexCoordID(mTevOrder[0].getTevOrderInfo().mTexCoord),
        GXTexMapID(mTevOrder[0].getTevOrderInfo().mTexMap),
        GXChannelID(mTevOrder[0].getTevOrderInfo().mColorChan),
        GXTexCoordID(mTevOrder[1].getTevOrderInfo().mTexCoord),
        GXTexMapID(mTevOrder[1].getTevOrderInfo().mTexMap),
        GXChannelID(mTevOrder[1].getTevOrderInfo().mColorChan)
    );

    loadTexCoordScale(
        GXTexCoordID(mTevOrder[0].getTevOrderInfo().mTexCoord),
        J3DSys::sTexCoordScaleTable[mTevOrder[0].getTevOrderInfo().mTexMap & 7]
    );

    loadTexCoordScale(
        GXTexCoordID(mTevOrder[1].getTevOrderInfo().mTexCoord & 7),
        J3DSys::sTexCoordScaleTable[mTevOrder[1].getTevOrderInfo().mTexMap & 7]
    );

    void* end = GDGetCurrPointer();
    DCStoreRange(start, (uintptr_t)end - (uintptr_t)start);
}

void J3DTevBlock2::patch() {
    patchTexNo();
    patchTevReg();
}

void J3DTevBlock4::patchTexNo() {
    GDSetCurrOffset(mTexNoOffset);
    void* start = GDGetCurrPointer();

    for (u32 i = 0; i < 4; i++) {
        if (mTexNo[i] != 0xffff) {
            loadTexNo(i, mTexNo[i]);
        }
    }

    void* end = GDGetCurrPointer();
    DCStoreRange(start, (uintptr_t)end - (uintptr_t)start);
}

void J3DTevBlock4::patchTevReg() {
    GDSetCurrOffset(mTevRegOffset);
    void* start = GDGetCurrPointer();

    for (u32 i = 0; i < ARRAY_SIZEU(mTevColor) - 1; i++) {
        loadTevColor(i, mTevColor[i]);
    }
    for (u32 i = 0; i < ARRAY_SIZEU(mTevKColor); i++) {
        loadTevKColor(i, mTevKColor[i]);
    }

    void* end = GDGetCurrPointer();
    DCStoreRange(start, (uintptr_t)end - (uintptr_t)start);
}

void J3DTevBlock4::patchTexNoAndTexCoordScale() {
    GDSetCurrOffset(mTexNoOffset);
    void* start = GDGetCurrPointer();

    u32 tevStageNum = mTevStageNum;
    for (u32 i = 0; i < 4; i++) {
        if (mTexNo[i] != 0xffff) {
            loadTexNo(i, mTexNo[i]);
        }
    }

    for (u32 i = 0; i < tevStageNum; i += 2) {
        J3DGDSetTevOrder(
            GXTevStageID(i),
            GXTexCoordID(mTevOrder[i].getTevOrderInfo().mTexCoord),
            GXTexMapID(mTevOrder[i].getTevOrderInfo().mTexMap),
            GXChannelID(mTevOrder[i].getTevOrderInfo().mColorChan),
            GXTexCoordID(mTevOrder[i + 1].getTevOrderInfo().mTexCoord),
            GXTexMapID(mTevOrder[i + 1].getTevOrderInfo().mTexMap),
            GXChannelID(mTevOrder[i + 1].getTevOrderInfo().mColorChan)
        );

        loadTexCoordScale(
            GXTexCoordID(mTevOrder[i].getTevOrderInfo().mTexCoord & 7),
            J3DSys::sTexCoordScaleTable[mTevOrder[i].getTevOrderInfo().mTexMap & 7]
        );

        loadTexCoordScale(
            GXTexCoordID(mTevOrder[i + 1].getTevOrderInfo().mTexCoord & 7),
            J3DSys::sTexCoordScaleTable[mTevOrder[i + 1].getTevOrderInfo().mTexMap & 7]
        );
    }

    void* end = GDGetCurrPointer();
    DCStoreRange(start, (uintptr_t)end - (uintptr_t)start);
}
void J3DTevBlock4::patch() {
    patchTexNo();
    patchTevReg();
}

void J3DTevBlock16::patchTexNo() {
    GDSetCurrOffset(mTexNoOffset);
    void* start = GDGetCurrPointer();

    for (u32 i = 0; i < 8; i++) {
        if (mTexNo[i] != 0xffff) {
            loadTexNo(i, mTexNo[i]);
        }
    }

    void* end = GDGetCurrPointer();
    DCStoreRange(start, (uintptr_t)end - (uintptr_t)start);
}

void J3DTevBlock16::patchTevReg() {
    GDSetCurrOffset(mTevRegOffset);
    void* start = GDGetCurrPointer();

    for (u32 i = 0; i < ARRAY_SIZEU(mTevColor) - 1; i++) {
        loadTevColor(i, mTevColor[i]);
    }
    for (u32 i = 0; i < ARRAY_SIZEU(mTevKColor); i++) {
        loadTevKColor(i, mTevKColor[i]);
    }

    void* end = GDGetCurrPointer();
    DCStoreRange(start, (uintptr_t)end - (uintptr_t)start);
}

void J3DTevBlock16::patchTexNoAndTexCoordScale() {
    GDSetCurrOffset(mTexNoOffset);
    void* start = GDGetCurrPointer();

    u32 tevStageNum = mTevStageNum;
    for (u32 i = 0; i < 8; i++) {
        if (mTexNo[i] != 0xffff) {
            loadTexNo(i, mTexNo[i]);
        }
    }

    for (u32 i = 0; i < tevStageNum; i += 2) {
        J3DGDSetTevOrder(
            GXTevStageID(i),
            GXTexCoordID(mTevOrder[i].getTevOrderInfo().mTexCoord),
            GXTexMapID(mTevOrder[i].getTevOrderInfo().mTexMap),
            GXChannelID(mTevOrder[i].getTevOrderInfo().mColorChan),
            GXTexCoordID(mTevOrder[i + 1].getTevOrderInfo().mTexCoord),
            GXTexMapID(mTevOrder[i + 1].getTevOrderInfo().mTexMap),
            GXChannelID(mTevOrder[i + 1].getTevOrderInfo().mColorChan)
        );

        loadTexCoordScale(
            GXTexCoordID(mTevOrder[i].getTevOrderInfo().mTexCoord & 7),
            J3DSys::sTexCoordScaleTable[mTevOrder[i].getTevOrderInfo().mTexMap & 7]
        );

        loadTexCoordScale(
            GXTexCoordID(mTevOrder[i + 1].getTevOrderInfo().mTexCoord & 7),
            J3DSys::sTexCoordScaleTable[mTevOrder[i + 1].getTevOrderInfo().mTexMap & 7]
        );
    }

    void* end = GDGetCurrPointer();
    DCStoreRange(start, (uintptr_t)end - (uintptr_t)start);
}

void J3DTevBlock16::patch() {
    patchTexNo();
    patchTevReg();
}

void J3DTevBlock::diff(u32 diffFlags) {
    if (getDiffFlag_TexNoNum(diffFlags)) {
        diffTexNo();
    }

    if (diffFlags & J3DDiffFlag_TexCoordScale) {
        diffTexCoordScale();
    }

    if (getDiffFlag_TevStageNum(diffFlags)) {
        diffTevStage();

        if (diffFlags & J3DDiffFlag_TevStageIndirect) {
            diffTevStageIndirect();
        }
    }

    if (diffFlags & J3DDiffFlag_TevReg) {
        diffTevReg();
    }
}

void J3DTevBlockPatched::diffTexNo() {
    for (u32 i = 0; i < ARRAY_SIZEU(mTexNo); i++) {
        if (mTexNo[i] != 0xFFFF) {
            loadTexNo(i, mTexNo[i]);
        }
    }
}

void J3DTevBlockPatched::diffTevStage() {
    u32 tevStageNum = mTevStageNum;
    for (u32 i = 0; i < tevStageNum; i++) {
        mTevStage[i].load(i);
    }
}

void J3DTevBlockPatched::diffTevStageIndirect() {
    u32 tevStageNum = mTevStageNum;
    for (u32 i = 0; i < tevStageNum; i++) {
        mIndTevStage[i].load(i);
    }
}

void J3DTevBlockPatched::diffTevReg() {
    for (u32 i = 0; i < ARRAY_SIZEU(mTevColor) - 1; i++) {
        loadTevColor(i, mTevColor[i]);
    }
    for (u32 i = 0; i < ARRAY_SIZEU(mTevKColor); i++) {
        loadTevKColor(i, mTevKColor[i]);
    }
}

void J3DTevBlockPatched::diffTexCoordScale() {
    u32 tevStageNum = mTevStageNum;
    for (u32 i = 0; i < tevStageNum; i += 2) {
        loadTexCoordScale(
            GXTexCoordID(mTevOrder[i].getTevOrderInfo().mTexCoord & 7),
            J3DSys::sTexCoordScaleTable[mTevOrder[i].getTevOrderInfo().mTexMap & 7]
        );
        loadTexCoordScale(
            GXTexCoordID(mTevOrder[i + 1].getTevOrderInfo().mTexCoord & 7),
            J3DSys::sTexCoordScaleTable[mTevOrder[i + 1].getTevOrderInfo().mTexMap & 7]
        );
    }
}

void J3DTevBlock1::diffTexNo() {
    if (mTexNo[0] != 0xFFFF) {
        loadTexNo(0, mTexNo[0]);
    }
}

void J3DTevBlock1::diffTevReg() {}

void J3DTevBlock1::diffTevStage() {
    mTevStage[0].load(0);
}

void J3DTevBlock1::diffTevStageIndirect() {
    mIndTevStage[0].load(0);
}

void J3DTevBlock1::diffTexCoordScale() {
    loadTexCoordScale(
        GXTexCoordID(mTevOrder[0].getTevOrderInfo().mTexCoord),
        J3DSys::sTexCoordScaleTable[mTevOrder[0].getTevOrderInfo().mTexMap & 7]
    );
}

void J3DTevBlock2::diffTexNo() {
    for (u32 i = 0; i < ARRAY_SIZEU(mTexNo); i++) {
        if (mTexNo[i] != 0xFFFF) {
            loadTexNo(i, mTexNo[i]);
        }
    }
}

void J3DTevBlock2::diffTevReg() {
    for (u32 i = 0; i < ARRAY_SIZEU(mTevColor) - 1; i++) {
        loadTevColor(i, mTevColor[i]);
    }
    for (u32 i = 0; i < ARRAY_SIZEU(mTevKColor); i++) {
        loadTevKColor(i, mTevKColor[i]);
    }
}

void J3DTevBlock2::diffTevStage() {
    u32 tevStageNum = mTevStageNum;
    for (u32 i = 0; i < tevStageNum; i++) {
        mTevStage[i].load(i);
    }
}

void J3DTevBlock2::diffTevStageIndirect() {
    u32 tevStageNum = mTevStageNum;
    for (u32 i = 0; i < tevStageNum; i++) {
        mIndTevStage[i].load(i);
    }
}

void J3DTevBlock2::diffTexCoordScale() {
    loadTexCoordScale(
        GXTexCoordID(mTevOrder[0].getTevOrderInfo().mTexCoord),
        J3DSys::sTexCoordScaleTable[mTevOrder[0].getTevOrderInfo().mTexMap & 7]
    );
    loadTexCoordScale(
        GXTexCoordID(mTevOrder[1].getTevOrderInfo().mTexCoord & 7),
        J3DSys::sTexCoordScaleTable[mTevOrder[1].getTevOrderInfo().mTexMap & 7]
    );
}

void J3DTevBlock4::diffTexNo() {
    for (u32 i = 0; i < ARRAY_SIZEU(mTexNo); i++) {
        if (mTexNo[i] != 0xFFFF) {
            loadTexNo(i, mTexNo[i]);
        }
    }
}

void J3DTevBlock4::diffTevReg() {
    for (u32 i = 0; i < ARRAY_SIZEU(mTevColor) - 1; i++) {
        loadTevColor(i, mTevColor[i]);
    }
    for (u32 i = 0; i < ARRAY_SIZEU(mTevKColor); i++) {
        loadTevKColor(i, mTevKColor[i]);
    }
}

void J3DTevBlock4::diffTevStage() {
    u32 tevStageNum = mTevStageNum;
    for (u32 i = 0; i < tevStageNum; i++) {
        mTevStage[i].load(i);
    }
}

void J3DTevBlock4::diffTevStageIndirect() {
    u32 tevStageNum = mTevStageNum;
    for (u32 i = 0; i < tevStageNum; i++) {
        mIndTevStage[i].load(i);
    }
}

void J3DTevBlock4::diffTexCoordScale() {
    u32 tevStageNum = mTevStageNum;
    for (u32 i = 0; i < tevStageNum; i += 2) {
        loadTexCoordScale(
            GXTexCoordID(mTevOrder[i].getTevOrderInfo().mTexCoord & 7),
            J3DSys::sTexCoordScaleTable[mTevOrder[i].getTevOrderInfo().mTexMap & 7]
        );
        loadTexCoordScale(
            GXTexCoordID(mTevOrder[i + 1].getTevOrderInfo().mTexCoord & 7),
            J3DSys::sTexCoordScaleTable[mTevOrder[i + 1].getTevOrderInfo().mTexMap & 7]
        );
    }
}
void J3DTevBlock16::diffTexNo() {
    for (u32 i = 0; i < ARRAY_SIZEU(mTexNo); i++) {
        if (mTexNo[i] != 0xFFFF) {
            loadTexNo(i, mTexNo[i]);
        }
    }
}

void J3DTevBlock16::diffTevReg() {
    for (u32 i = 0; i < ARRAY_SIZEU(mTevColor) - 1; i++) {
        loadTevColor(i, mTevColor[i]);
    }
    for (u32 i = 0; i < ARRAY_SIZEU(mTevKColor); i++) {
        loadTevKColor(i, mTevKColor[i]);
    }
}

void J3DTevBlock16::diffTevStage() {
    u32 tevStageNum = mTevStageNum;
    for (u32 i = 0; i < tevStageNum; i++) {
        mTevStage[i].load(i);
    }
}

void J3DTevBlock16::diffTevStageIndirect() {
    u32 tevStageNum = mTevStageNum;
    for (u32 i = 0; i < tevStageNum; i++) {
        mIndTevStage[i].load(i);
    }
}

void J3DTevBlock16::diffTexCoordScale() {
    u32 tevStageNum = mTevStageNum;
    for (u32 i = 0; i < tevStageNum; i += 2) {
        loadTexCoordScale(
            GXTexCoordID(mTevOrder[i].getTevOrderInfo().mTexCoord & 7),
            J3DSys::sTexCoordScaleTable[mTevOrder[i].getTevOrderInfo().mTexMap & 7]
        );
        loadTexCoordScale(
            GXTexCoordID(mTevOrder[i + 1].getTevOrderInfo().mTexCoord & 7),
            J3DSys::sTexCoordScaleTable[mTevOrder[i + 1].getTevOrderInfo().mTexMap & 7]
        );
    }
}

void J3DTevBlock16::ptrToIndex() {
    GDSetCurrOffset(mTexNoOffset);
    void* start = GDGetCurrPointer();

    u32 offs = 0;
    for (u32 i = 0; i < 8; i++) {
        if (mTexNo[i] != 0xFFFF) {
            GDSetCurrOffset(mTexNoOffset + offs);
            patchTexNo_PtrToIdx(i, mTexNo[i]);

            ResTIMG* img = j3dSys.getTexture()->getResTIMG(mTexNo[i]);
            J3D_ASSERT_NULLPTR(2088, img != NULL);

            offs += 0x14;
            if (img->indexTexture == 1) {
                offs += 0x23;
            }
        }
    }

    void* end = GDGetCurrPointer();
    DCStoreRange(start, (uintptr_t)end - (uintptr_t)start);
}

void J3DTevBlockPatched::ptrToIndex() {
    GDSetCurrOffset(mTexNoOffset);
    void* start = GDGetCurrPointer();

    u32 offs = 0;
    for (u32 i = 0; i < 8; i++) {
        if (mTexNo[i] != 0xFFFF) {
            GDSetCurrOffset(mTexNoOffset + offs);
            patchTexNo_PtrToIdx(i, mTexNo[i]);

            ResTIMG* img = j3dSys.getTexture()->getResTIMG(mTexNo[i]);
            J3D_ASSERT_NULLPTR(2129, img != NULL);

            offs += 0x14;
            if (img->indexTexture == 1) {
                offs += 0x23;
            }
        }
    }

    void* end = GDGetCurrPointer();
    DCStoreRange(start, (uintptr_t)end - (uintptr_t)start);
}

void J3DTevBlock::indexToPtr_private(u32 offs) {
    GDSetCurrOffset(offs);
    void* start = GDGetCurrPointer();

    for (u32 i = 0; ; i++) {
        void* pDL = GDGetCurrPointer();
        if (!isTexNoReg(pDL)) {
            break;
        }

        u16 texNoReg = getTexNoReg(pDL);
        loadTexNo(i, texNoReg);
    }

    void* end = GDGetCurrPointer();
    DCStoreRange(start, (uintptr_t)end - (uintptr_t)start);
}

void J3DIndBlockFull::load() {
    int indTexStageNum = mIndTexStageNum;
    for (u32 i = 0; i < indTexStageNum; i++) {
        mIndTexMtx[i].load(i);
    }

    for (u32 i = 0; i < indTexStageNum; i += 2) {
        J3DGDSetIndTexCoordScale(
            GXIndTexStageID(i),
            (GXIndTexScale)mIndTexCoordScale[i].getScaleS(),
            (GXIndTexScale)mIndTexCoordScale[i].getScaleT(),
            (GXIndTexScale)mIndTexCoordScale[i + 1].getScaleS(),
            (GXIndTexScale)mIndTexCoordScale[i + 1].getScaleT()
        );
    }

    loadTexCoordScale((GXTexCoordID)mIndTexOrder[0].getCoord(), J3DSys::sTexCoordScaleTable[mIndTexOrder[0].getMap() & 7]);
    loadTexCoordScale((GXTexCoordID)mIndTexOrder[1].getCoord(), J3DSys::sTexCoordScaleTable[mIndTexOrder[1].getMap() & 7]);
    loadTexCoordScale((GXTexCoordID)mIndTexOrder[2].getCoord(), J3DSys::sTexCoordScaleTable[mIndTexOrder[2].getMap() & 7]);
    loadTexCoordScale((GXTexCoordID)mIndTexOrder[3].getCoord(), J3DSys::sTexCoordScaleTable[mIndTexOrder[3].getMap() & 7]);

    J3DGDSetIndTexOrder(
        indTexStageNum,
        (GXTexCoordID)mIndTexOrder[0].getCoord(),
        (GXTexMapID)mIndTexOrder[0].getMap(),
        (GXTexCoordID)mIndTexOrder[1].getCoord(),
        (GXTexMapID)mIndTexOrder[1].getMap(),
        (GXTexCoordID)mIndTexOrder[2].getCoord(),
        (GXTexMapID)mIndTexOrder[2].getMap(),
        (GXTexCoordID)mIndTexOrder[3].getCoord(),
        (GXTexMapID)mIndTexOrder[3].getMap()
    );
}

void J3DIndBlockFull::diff(u32 diffFlags) {
    if (diffFlags & J3DDiffFlag_TevStageIndirect) {
        u32 indTexStageNum = mIndTexStageNum;
        mIndTexMtx[0].load(0);

        J3DGDSetIndTexCoordScale(
            GX_INDTEXSTAGE0,
            (GXIndTexScale)mIndTexCoordScale[0].getScaleS(),
            (GXIndTexScale)mIndTexCoordScale[0].getScaleT(),
            (GXIndTexScale)mIndTexCoordScale[1].getScaleS(),
            (GXIndTexScale)mIndTexCoordScale[1].getScaleT()
        );

        loadTexCoordScale((GXTexCoordID)mIndTexOrder[0].getCoord(), J3DSys::sTexCoordScaleTable[mIndTexOrder[0].getMap() & 7]);

        J3DGDSetIndTexOrder(
            indTexStageNum,
            (GXTexCoordID)mIndTexOrder[0].getCoord(),
            (GXTexMapID)mIndTexOrder[0].getMap(),
            (GXTexCoordID)mIndTexOrder[1].getCoord(),
            (GXTexMapID)mIndTexOrder[1].getMap(),
            (GXTexCoordID)mIndTexOrder[2].getCoord(),
            (GXTexMapID)mIndTexOrder[2].getMap(),
            (GXTexCoordID)mIndTexOrder[3].getCoord(),
            (GXTexMapID)mIndTexOrder[3].getMap()
        );
    }
}

void J3DPEBlockOpa::load() {
    GDOverflowCheck(0x1e);
    J3DGDSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    J3DGDSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_COPY);
    J3DGDSetZMode(1, GX_LEQUAL, 1);
    J3DGDSetZCompLoc(1);
}

void J3DPEBlockTexEdge::load() {
    GDOverflowCheck(0x1e);
    J3DGDSetAlphaCompare(GX_GEQUAL, 0x80, GX_AOP_AND, GX_LEQUAL, 0xff);
    J3DGDSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_COPY);
    J3DGDSetZMode(1, GX_LEQUAL, 1);
    J3DGDSetZCompLoc(0);
}

void J3DPEBlockXlu::load() {
    GDOverflowCheck(0x1e);
    J3DGDSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND,GX_ALWAYS, 0);
    J3DGDSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_COPY);
    J3DGDSetZMode(1, GX_LEQUAL, 0);
    J3DGDSetZCompLoc(1);
}

inline void loadZCompLoc(u8 compLoc) {
    J3DGDSetZCompLoc(compLoc);
}

void J3DPEBlockFogOff::load() {
    GDOverflowCheck(0x1e);
    mAlphaComp.load();
    mBlend.load(mDither);
    mZMode.load();
    loadZCompLoc(mZCompLoc);
}

void J3DPEBlockFogOff::diffBlend() {
    GDOverflowCheck(0xf);
    mBlend.load(mDither);
    mZMode.load();
}

void J3DPEBlockFull::load() {
    mFogOffset = GDGetCurrOffset();
    GDOverflowCheck(0x55);
    mFog.load();
    mAlphaComp.load();
    mBlend.load(mDither);
    mZMode.load();
    loadZCompLoc(mZCompLoc);
}

void J3DPEBlockFull::patch() {
    GDSetCurrOffset(mFogOffset);
    GDOverflowCheck(0x37);
    void* start = GDGetCurrPointer();
    mFog.load();
    void* end = GDGetCurrPointer();
    DCStoreRange(start, (uintptr_t)end - (uintptr_t)start);
}

void J3DPEBlockFull::diffFog() {
    GDOverflowCheck(0x37);
    mFog.load();
}

void J3DPEBlockFull::diffBlend() {
    GDOverflowCheck(0xf);
    mBlend.load(mDither);
    mZMode.load();
}

void J3DPEBlockFull::diff(u32 diffFlags) {
    if ((diffFlags & J3DDiffFlag_Fog)) {
        diffFog();
    }

    if ((diffFlags & J3DDiffFlag_Blend)) {
        diffBlend();
    }
}

void J3DColorBlockLightOff::reset(J3DColorBlock* pBlock) {
    J3D_ASSERT_NULLPTR(2434, pBlock != NULL);
    mColorChanNum = pBlock->getColorChanNum();

    for (u32 i = 0; i < ARRAY_SIZEU(mMatColor); i++) {
        mMatColor[i] = *pBlock->getMatColor(i);
    }
    for (u32 i = 0; i < ARRAY_SIZEU(mColorChan); i++) {
        mColorChan[i] = *pBlock->getColorChan(i);
    }
}

void J3DColorBlockAmbientOn::reset(J3DColorBlock* pBlock) {
    J3D_ASSERT_NULLPTR(2451, pBlock != NULL);
    mColorChanNum = pBlock->getColorChanNum();

    for (u32 i = 0; i < ARRAY_SIZEU(mMatColor); i++) {
        mMatColor[i] = *pBlock->getMatColor(i);
    }
    for (u32 i = 0; i < ARRAY_SIZEU(mColorChan); i++) {
        mColorChan[i] = *pBlock->getColorChan(i);
    }
    for (u32 i = 0; i < ARRAY_SIZEU(mAmbColor); i++) {
        if (pBlock->getAmbColor(i) != NULL) {
            mAmbColor[i] = *pBlock->getAmbColor(i);
        }
    }
}

void J3DColorBlockLightOn::reset(J3DColorBlock* pBlock) {
    J3D_ASSERT_NULLPTR(2476, pBlock != NULL);
    mColorChanNum = pBlock->getColorChanNum();

    for (u32 i = 0; i < ARRAY_SIZEU(mMatColor); i++) {
        mMatColor[i] = *pBlock->getMatColor(i);
    }
    for (u32 i = 0; i < ARRAY_SIZEU(mColorChan); i++) {
        mColorChan[i] = *pBlock->getColorChan(i);
    }
    for (u32 i = 0; i < ARRAY_SIZEU(mAmbColor); i++) {
        if (pBlock->getAmbColor(i) != NULL) {
            mAmbColor[i] = *pBlock->getAmbColor(i);
        }
    }
}

void J3DTexGenBlockPatched::reset(J3DTexGenBlock* pBlock) {
    J3D_ASSERT_NULLPTR(2504, pBlock != NULL);

    mTexGenNum = pBlock->getTexGenNum();
    for (u32 i = 0; i < 8; i++) {
        mTexCoord[i] = *pBlock->getTexCoord(i);
    }

    for (u32 i = 0; i < 8; i++) {
        if (pBlock->getTexMtx(i) != NULL) {
            if (mTexMtx[i] != NULL) {
                memcpy(mTexMtx[i], pBlock->getTexMtx(i), sizeof(*mTexMtx[i]));
                DCStoreRange(mTexMtx[i], sizeof(*mTexMtx[i]));
            } else {
                OSReport("Error : TexMtx[%d] is Null.\n", i);
            }
        }
    }
}

void J3DTexGenBlock4::reset(J3DTexGenBlock* pBlock) {
    J3D_ASSERT_NULLPTR(2537, pBlock != NULL);

    mTexGenNum = pBlock->getTexGenNum();
    for (u32 i = 0; i < 4; i++) {
        mTexCoord[i] = *pBlock->getTexCoord(i);
    }

    for (u32 i = 0; i < 4; i++) {
        if (pBlock->getTexMtx(i) != NULL) {
            if (mTexMtx[i] != NULL) {
                memcpy(mTexMtx[i], pBlock->getTexMtx(i), sizeof(*mTexMtx[i]));
                DCStoreRange(mTexMtx[i], sizeof(*mTexMtx[i]));
            } else {
                OSReport("Error : TexMtx[%d] is Null.\n", i);
            }
        }
    }

    mNBTScale = *pBlock->getNBTScale();
}

void J3DTexGenBlockBasic::reset(J3DTexGenBlock* pBlock) {
    J3D_ASSERT_NULLPTR(2572, pBlock != NULL);

    mTexGenNum = pBlock->getTexGenNum();
    for (u32 i = 0; i < 8; i++) {
        mTexCoord[i] = *pBlock->getTexCoord(i);
    }

    for (u32 i = 0; i < 8; i++) {
        if (pBlock->getTexMtx(i) != NULL) {
            if (mTexMtx[i] != NULL) {
                memcpy(mTexMtx[i], pBlock->getTexMtx(i), sizeof(*mTexMtx[i]));
                DCStoreRange(mTexMtx[i], sizeof(*mTexMtx[i]));
            } else {
                OSReport("Error : TexMtx[%d] is Null.\n", i);
            }
        }
    }

    mNBTScale = *pBlock->getNBTScale();
}

void J3DTevBlockPatched::reset(J3DTevBlock* pBlock) {
    J3D_ASSERT_NULLPTR(2608, pBlock != NULL);

    mTevStageNum = pBlock->getTevStageNum();
    for (u32 i = 0; i < 8; i++) {
        mTexNo[i] = pBlock->getTexNo(i);
    }
    for (u32 i = 0; i < 4; i++) {
        mTevColor[i] = *pBlock->getTevColor(i);
    }
    for (u32 i = 0; i < 4; i++) {
        mTevKColor[i] = *pBlock->getTevKColor(i);
    }
    for (u32 i = 0; i < 8; i++) {
        mTevStage[i] = *pBlock->getTevStage(i);
        mIndTevStage[i] = *pBlock->getIndTevStage(i);
    }
}

void J3DTevBlock1::reset(J3DTevBlock* pBlock) {
    J3D_ASSERT_NULLPTR(2633, pBlock != NULL);

    mTexNo[0] = pBlock->getTexNo(0);
    mTevOrder[0] = *pBlock->getTevOrder(0);
    mTevStage[0] = *pBlock->getTevStage(0);
    mIndTevStage[0] = *pBlock->getIndTevStage(0);
}

void J3DTevBlock2::reset(J3DTevBlock* pBlock) {
    J3D_ASSERT_NULLPTR(2649, pBlock != NULL);

    mTevStageNum = pBlock->getTevStageNum();
    mTexNo[0] = pBlock->getTexNo(0);
    mTexNo[1] = pBlock->getTexNo(1);
    mTevStage[0] = *pBlock->getTevStage(0);
    mTevStage[1] = *pBlock->getTevStage(1);
    mIndTevStage[0] = *pBlock->getIndTevStage(0);
    mIndTevStage[1] = *pBlock->getIndTevStage(1);
    mTevOrder[0] = *pBlock->getTevOrder(0);
    mTevOrder[1] = *pBlock->getTevOrder(1);
    mTevKColorSel[0] = pBlock->getTevKColorSel(0);
    mTevKColorSel[1] = pBlock->getTevKColorSel(1);
    mTevKAlphaSel[0] = pBlock->getTevKAlphaSel(0);
    mTevKAlphaSel[1] = pBlock->getTevKAlphaSel(1);

    for (u32 i = 0; i < 4; i++) {
        mTevColor[i] = *pBlock->getTevColor(i);
    }
    for (u32 i = 0; i < 4; i++) {
        mTevKColor[i] = *pBlock->getTevKColor(i);
    }
    for (u32 i = 0; i < 4; i++) {
        mTevSwapModeTable[i] = *pBlock->getTevSwapModeTable(i);
    }
}

void J3DTevBlock4::reset(J3DTevBlock* pBlock) {
    J3D_ASSERT_NULLPTR(2682, pBlock != NULL);

    mTevStageNum = pBlock->getTevStageNum();
    mTexNo[0] = pBlock->getTexNo(0);
    mTexNo[1] = pBlock->getTexNo(1);
    mTexNo[2] = pBlock->getTexNo(2);
    mTexNo[3] = pBlock->getTexNo(3);
    mTevStage[0] = *pBlock->getTevStage(0);
    mTevStage[1] = *pBlock->getTevStage(1);
    mTevStage[2] = *pBlock->getTevStage(2);
    mTevStage[3] = *pBlock->getTevStage(3);
    mIndTevStage[0] = *pBlock->getIndTevStage(0);
    mIndTevStage[1] = *pBlock->getIndTevStage(1);
    mIndTevStage[2] = *pBlock->getIndTevStage(2);
    mIndTevStage[3] = *pBlock->getIndTevStage(3);
    mTevOrder[0] = *pBlock->getTevOrder(0);
    mTevOrder[1] = *pBlock->getTevOrder(1);
    mTevOrder[2] = *pBlock->getTevOrder(2);
    mTevOrder[3] = *pBlock->getTevOrder(3);
    mTevKColorSel[0] = pBlock->getTevKColorSel(0);
    mTevKColorSel[1] = pBlock->getTevKColorSel(1);
    mTevKColorSel[2] = pBlock->getTevKColorSel(2);
    mTevKColorSel[3] = pBlock->getTevKColorSel(3);
    mTevKAlphaSel[0] = pBlock->getTevKAlphaSel(0);
    mTevKAlphaSel[1] = pBlock->getTevKAlphaSel(1);
    mTevKAlphaSel[2] = pBlock->getTevKAlphaSel(2);
    mTevKAlphaSel[3] = pBlock->getTevKAlphaSel(3);

    for (u32 i = 0; i < 4; i++)
        mTevColor[i] = *pBlock->getTevColor(i);
    for (u32 i = 0; i < 4; i++)
        mTevKColor[i] = *pBlock->getTevKColor(i);
    for (u32 i = 0; i < 4; i++)
        mTevSwapModeTable[i] = *pBlock->getTevSwapModeTable(i);
}

void J3DTevBlock16::reset(J3DTevBlock* pBlock) {
    J3D_ASSERT_NULLPTR(2728, pBlock != NULL);

    mTevStageNum = pBlock->getTevStageNum();
    for (u32 i = 0; i < 8; i++) {
        mTexNo[i] = pBlock->getTexNo(i);
    }
    for (u32 i = 0; i < 16; i++) {
        mTevOrder[i] = *pBlock->getTevOrder(i);
    }
    for (u32 i = 0; i < 16; i++) {
        mTevStage[i] = *pBlock->getTevStage(i);
        mIndTevStage[i] = *pBlock->getIndTevStage(i);
    }
    for (u32 i = 0; i < 4; i++) {
        mTevColor[i] = *pBlock->getTevColor(i);
    }
    for (u32 i = 0; i < 4; i++) {
        mTevKColor[i] = *pBlock->getTevKColor(i);
    }
    for (u32 i = 0; i < 16; i++) {
        mTevKColorSel[i] = pBlock->getTevKColorSel(i);
    }
    for (u32 i = 0; i < 16; i++) {
        mTevKAlphaSel[i] = pBlock->getTevKAlphaSel(i);
    }
    for (u32 i = 0; i < 4; i++) {
        mTevSwapModeTable[i] = *pBlock->getTevSwapModeTable(i);
    }
}

void J3DIndBlockFull::reset(J3DIndBlock* pBlock) {
    J3D_ASSERT_NULLPTR(2762, pBlock != NULL);

    mIndTexStageNum = pBlock->getIndTexStageNum();
    for (u32 i = 0; i < 4; i++) {
        mIndTexOrder[i] = *pBlock->getIndTexOrder(i);
    }
    for (u32 i = 0; i < 3; i++) {
        mIndTexMtx[i] = *pBlock->getIndTexMtx(i);
    }
    for (u32 i = 0; i < 4; i++) {
        mIndTexCoordScale[i] = *pBlock->getIndTexCoordScale(i);
    }
}

void J3DPEBlockFogOff::reset(J3DPEBlock* pBlock) {
    J3D_ASSERT_NULLPTR(2783, pBlock != NULL);

    switch (pBlock->getType()) {
    case 'PEFL':
    case 'PEFG':
        mAlphaComp = *pBlock->getAlphaComp();
        mBlend = *pBlock->getBlend();
        mZMode = *pBlock->getZMode();
        mZCompLoc = pBlock->getZCompLoc();
        break;
    }
}

void J3DPEBlockFull::reset(J3DPEBlock* pBlock) {
    J3D_ASSERT_NULLPTR(2807, pBlock != NULL);

    if (pBlock->getFog() != NULL) {
        mFog.setFogInfo(pBlock->getFog()->getFogInfo());
    }

    switch (pBlock->getType()) {
    case 'PEFL':
    case 'PEFG':
        mAlphaComp = *pBlock->getAlphaComp();
        mBlend = *pBlock->getBlend();
        mZMode = *pBlock->getZMode();
        mZCompLoc = pBlock->getZCompLoc();
        break;
    }
}

void J3DTexGenBlockPatched::calc(const Mtx modelMtx) {
    Mtx viewMtx;
    for (int i = 0; i < 8; i++) {
        if (mTexMtx[i] != NULL) {
            u32 texMtxMode = mTexMtx[i]->getTexMtxInfo().mInfo & 0x3f;
            mTexCoord[i].resetTexMtxReg();
            switch (texMtxMode) {
            case J3DTexMtxMode_EnvmapBasic:
            case J3DTexMtxMode_EnvmapOld:
            case J3DTexMtxMode_Envmap:

                if (j3dSys.checkFlag(8) == 0) {
                    MTXConcat(j3dSys.getViewMtx(), modelMtx, viewMtx);
                } else {
                    MTXCopy(j3dSys.getViewMtx(), viewMtx);
                }
                viewMtx[0][3] = 0.0f;
                viewMtx[1][3] = 0.0f;
                viewMtx[2][3] = 0.0f;
                mTexMtx[i]->calc(viewMtx);
                break;
            case J3DTexMtxMode_ProjmapBasic:
            case J3DTexMtxMode_Projmap:
                if (j3dSys.checkFlag(4) == 0) {
                    mTexMtx[i]->calc(modelMtx);
                } else {
                    mTexMtx[i]->calc(j3dDefaultMtx);
                }
                break;
            case J3DTexMtxMode_ViewProjmapBasic:
            case J3DTexMtxMode_ViewProjmap:
                if (j3dSys.checkFlag(4) == 0) {
                    MTXConcat(j3dSys.getViewMtx(), modelMtx, viewMtx);
                    mTexMtx[i]->calc( viewMtx);
                } else {
                    mTexMtx[i]->calc(j3dSys.getViewMtx());
                }
                break;
            case J3DTexMtxMode_Unknown5:
            case J3DTexMtxMode_EnvmapOldEffectMtx:
            case J3DTexMtxMode_EnvmapEffectMtx:
                if (j3dSys.checkFlag(8) == 0) {
                    MTXCopy(modelMtx, viewMtx);
                    viewMtx[0][3] = 0.0f;
                    viewMtx[1][3] = 0.0f;
                    viewMtx[2][3] = 0.0f;
                    mTexMtx[i]->calc(viewMtx);
                } else {
                    mTexMtx[i]->calc(j3dDefaultMtx);
                }
                break;
            default:
                mTexMtx[i]->calc(j3dDefaultMtx);
                break;
            }
        }
    }
}

void J3DTexGenBlockPatched::calcWithoutViewMtx(const Mtx modelMtx) {
    Mtx mtx;
    for (int i = 0; i < 8; i++) {
        if (mTexMtx[i] != NULL) {
            u32 texMtxMode = mTexMtx[i]->getTexMtxInfo().mInfo & 0x3f;
            mTexCoord[i].resetTexMtxReg();
            switch (texMtxMode) {
            case J3DTexMtxMode_EnvmapBasic:
            case J3DTexMtxMode_EnvmapOld:
            case J3DTexMtxMode_Envmap:
                mTexMtx[i]->calc(j3dDefaultMtx);
                break;
            case J3DTexMtxMode_ProjmapBasic:
            case J3DTexMtxMode_Projmap:
                if (j3dSys.checkFlag(4) == 0) {
                    mTexMtx[i]->calc(modelMtx);
                } else {
                    mTexMtx[i]->calc(j3dDefaultMtx);
                }
                break;
            case J3DTexMtxMode_ViewProjmapBasic:
            case J3DTexMtxMode_ViewProjmap:
                mTexMtx[i]->calc(j3dDefaultMtx);
                break;
            case J3DTexMtxMode_Unknown5:
            case J3DTexMtxMode_EnvmapOldEffectMtx:
            case J3DTexMtxMode_EnvmapEffectMtx:
                if (j3dSys.checkFlag(8) == 0) {
                    MTXCopy(modelMtx, mtx);
                    mtx[0][3] = 0.0f;
                    mtx[1][3] = 0.0f;
                    mtx[2][3] = 0.0f;
                    mTexMtx[i]->calc(mtx);
                } else {
                    mTexMtx[i]->calc(j3dDefaultMtx);
                }
                break;
            default:
                mTexMtx[i]->calc(j3dDefaultMtx);
                break;
            }
        }
    }
}

void J3DTexGenBlockPatched::calcPostTexMtx(const Mtx modelMtx) {
    Mtx mtx1, mtx2;
    for (int i = 0; i < 8; i++) {
        if (mTexMtx[i] != NULL) {
            u32 texMtxMode = mTexMtx[i]->getTexMtxInfo().mInfo & 0x3f;
            mTexCoord[i].resetTexMtxReg();
            switch (texMtxMode) {
            case J3DTexMtxMode_EnvmapBasic:
            case J3DTexMtxMode_EnvmapOld:
            case J3DTexMtxMode_Envmap:
                mTexCoord[i].setTexMtxReg(30);
                mTexMtx[i]->calcPostTexMtx(j3dDefaultMtx);
                break;
            case J3DTexMtxMode_ProjmapBasic:
            case J3DTexMtxMode_Projmap:
                PSMTXInverse(j3dSys.getViewMtx(), mtx1);
                mTexCoord[i].setTexMtxReg(0);
                mTexMtx[i]->calcPostTexMtx(mtx1);
                break;
            case J3DTexMtxMode_ViewProjmapBasic:
            case J3DTexMtxMode_ViewProjmap:
                mTexCoord[i].setTexMtxReg(0);
                mTexMtx[i]->calcPostTexMtx(j3dDefaultMtx);
                break;
            case J3DTexMtxMode_Unknown5:
            case J3DTexMtxMode_EnvmapOldEffectMtx:
            case J3DTexMtxMode_EnvmapEffectMtx:
                PSMTXInverse(j3dSys.getViewMtx(), mtx2);
                mtx2[0][3] = 0.0f;
                mtx2[1][3] = 0.0f;
                mtx2[2][3] = 0.0f;
                mTexCoord[i].setTexMtxReg(30);
                mTexMtx[i]->calcPostTexMtx(mtx2);
                break;
            default:
                mTexCoord[i].setTexMtxReg(60);
                mTexMtx[i]->calcPostTexMtx(j3dDefaultMtx);
                continue;
            }
        }
    }
}

void J3DTexGenBlockPatched::calcPostTexMtxWithoutViewMtx(f32 const (*param_0)[4]) {
    for (int i = 0; i < 8; i++) {
        if (mTexMtx[i] != NULL) {
            int texMtxMode = mTexMtx[i]->getTexMtxInfo().mInfo & 0x3f;
            mTexCoord[i].resetTexMtxReg();
            switch (texMtxMode) {
            case J3DTexMtxMode_EnvmapBasic:
            case J3DTexMtxMode_EnvmapOld:
            case J3DTexMtxMode_Envmap:
                mTexCoord[i].setTexMtxReg(30);
                mTexMtx[i]->calcPostTexMtx(j3dDefaultMtx);
                break;
            case J3DTexMtxMode_ProjmapBasic:
            case J3DTexMtxMode_Projmap:
                mTexCoord[i].setTexMtxReg(0);
                mTexMtx[i]->calcPostTexMtx(j3dDefaultMtx);
                break;
            case J3DTexMtxMode_ViewProjmapBasic:
            case J3DTexMtxMode_ViewProjmap:
                mTexCoord[i].setTexMtxReg(0);
                mTexMtx[i]->calcPostTexMtx(j3dDefaultMtx);
                break;
            case J3DTexMtxMode_Unknown5:
            case J3DTexMtxMode_EnvmapOldEffectMtx:
            case J3DTexMtxMode_EnvmapEffectMtx:
                mTexCoord[i].setTexMtxReg(30);
                mTexMtx[i]->calcPostTexMtx(j3dDefaultMtx);
                break;
            default:
                mTexCoord[i].setTexMtxReg(60);
                mTexMtx[i]->calcPostTexMtx(j3dDefaultMtx);
                break;
            }
        }
    }
}

void J3DTevBlock::diffTevReg() {}

void J3DTevBlock::diffTevStageIndirect() {}

void J3DTevBlock::diffTevStage() {}

void J3DTevBlock::diffTexCoordScale() {}

void J3DTevBlock::diffTexNo() {}
