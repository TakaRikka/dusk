#ifndef J2DMATERIALFACTORY_H
#define J2DMATERIALFACTORY_H

#include "JSystem/J2DGraph/J2DManage.h"
#include "JSystem/J2DGraph/J2DMatBlock.h"

#include "dusk/endian.h"

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DMaterialBlock {
    BE(u32) field_0x0;
    BE(u32) field_0x4;
    BE(u16) field_0x8;
    BE(u16) field_0xa;
    BE(u32) field_0xc;
    BE(u32) field_0x10;
    BE(u32) field_0x14;
    BE(u32) field_0x18;
    BE(u32) field_0x1c;
    BE(u32) field_0x20;
    BE(u32) field_0x24;
    BE(u32) field_0x28;
    BE(u32) field_0x2c;
    BE(u32) field_0x30;
    BE(u32) field_0x34;
    BE(u32) field_0x38;
    BE(u32) field_0x3c;
    BE(u32) field_0x40;
    BE(u32) field_0x44;
    BE(u32) field_0x48;
    BE(u32) field_0x4c;
    BE(u32) field_0x50;
    BE(u32) field_0x54;
    BE(u32) field_0x58;
    BE(u32) field_0x5c;
    BE(u32) field_0x60;
    BE(u32) field_0x64;
};

struct J2DAlphaCompInfo;
struct J2DBlendInfo;
struct J2DColorChanInfo;
class J2DMaterial;

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DIndInitData {
    u8 field_0x0;
    u8 field_0x1;
    u8 field_0x2[2];
    J2DIndTexOrderInfo field_0x4[4];
    J2DIndTexMtxInfo field_0xc[3];
    J2DIndTexCoordScaleInfo field_0x60[4];
    J2DIndTevStageInfo field_0x68[4];
    u8 field_0xac[0x90];
};

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DMaterialInitData {
    u8 field_0x0;
    u8 field_0x1;
    u8 field_0x2;
    u8 field_0x3;
    u8 field_0x4;
    u8 field_0x5;
    u8 field_0x6;
    u8 field_0x7;
    BE(u16) field_0x8[2];
    BE(u16) field_0xc[4];
    BE(u16) field_0x14[8];
    BE(u16) field_0x24[0xa];
    BE(u16) field_0x38[8];
    BE(u16) field_0x48;
    BE(u16) field_0x4a[4];
    u8 field_0x52[0x10];
    u8 field_0x62[0x10];
    BE(u16) field_0x72[0x10];
    BE(u16) field_0x92[0x4];
    BE(u16) field_0x9a[0x10];
    BE(u16) field_0xba[0x10];
    BE(u16) field_0xda[0x4];
    BE(u16) field_0xe2;
    BE(u16) field_0xe4;
    BE(u16) field_0xe6;
};
struct J2DTevStageInfo;
struct J2DTevSwapModeTableInfo;
struct J2DTevSwapModeInfo;
struct J2DTevOrderInfo;
struct J2DTexCoordInfo;
struct J2DTexMtxInfo;
class JKRArchive;

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DMaterialFactory {
public:
    J2DMaterialFactory(J2DMaterialBlock const&);
    u32 countStages(int) const;
    J2DMaterial* create(J2DMaterial*, int, u32, J2DResReference*, J2DResReference*,
                               JKRArchive*) const;
    JUtility::TColor newMatColor(int, int) const;
    u8 newColorChanNum(int) const;
    J2DColorChan newColorChan(int, int) const;
    u32 newTexGenNum(int) const;
    J2DTexCoord newTexCoord(int, int) const;
    J2DTexMtx* newTexMtx(int, int) const;
    u8 newCullMode(int) const;
    u16 newTexNo(int, int) const;
    u16 newFontNo(int) const;
    J2DTevOrder newTevOrder(int, int) const;
    J2DGXColorS10 newTevColor(int, int) const;
    JUtility::TColor newTevKColor(int, int) const;
    u8 newTevStageNum(int) const;
    J2DTevStage newTevStage(int, int) const;
    J2DTevSwapModeTable newTevSwapModeTable(int, int) const;
    u8 newIndTexStageNum(int) const;
    J2DIndTexOrder newIndTexOrder(int, int) const;
    J2DIndTexMtx newIndTexMtx(int, int) const;
    J2DIndTevStage newIndTevStage(int, int) const;
    J2DIndTexCoordScale newIndTexCoordScale(int, int) const;
    J2DAlphaComp newAlphaComp(int) const;
    J2DBlend newBlend(int) const;
    u8 newDither(int) const;

    u8 getMaterialMode(int idx) const {
        return mpMaterialInitData[mpMaterialID[idx]].field_0x0;
    }

    u8 getMaterialAlphaCalc(int idx) const {
        return mpMaterialInitData[mpMaterialID[idx]].field_0x6;
    }

private:
    /* 0x00 */ u16 mMaterialNum;
    /* 0x02 */ u16 field_0x2;
    /* 0x04 */ J2DMaterialInitData* mpMaterialInitData;
    /* 0x08 */ BE(u16)* mpMaterialID;
    /* 0x0C */ J2DIndInitData* mpIndInitData;
    /* 0x10 */ GXColor* mpMatColor;
    /* 0x14 */ u8* mpColorChanNum;
    /* 0x18 */ J2DColorChanInfo* mpColorChanInfo;
    /* 0x1C */ u8* mpTexGenNum;
    /* 0x20 */ J2DTexCoordInfo* mpTexCoordInfo;
    /* 0x24 */ J2DTexMtxInfo* mpTexMtxInfo;
    /* 0x28 */ BE(u16)* mpTexNo;
    /* 0x2C */ BE(u16)* mpFontNo;
    /* 0x30 */ BE(GXCullMode)* mpCullMode;
    /* 0x34 */ J2DTevOrderInfo* mpTevOrderInfo;
    /* 0x38 */ BE(GXColorS10)* mpTevColor;
    /* 0x3C */ GXColor* mpTevKColor;
    /* 0x40 */ u8* mpTevStageNum;
    /* 0x44 */ J2DTevStageInfo* mpTevStageInfo;
    /* 0x48 */ J2DTevSwapModeInfo* mpTevSwapModeInfo;
    /* 0x4C */ J2DTevSwapModeTableInfo* mpTevSwapModeTableInfo;
    /* 0x50 */ J2DAlphaCompInfo* mpAlphaCompInfo;
    /* 0x54 */ J2DBlendInfo* mpBlendInfo;
    /* 0x58 */ u8* mpDither;
};

#endif /* J2DMATERIALFACTORY_H */
