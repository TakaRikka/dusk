#ifndef J3DMATERIALFACTORY_V21_H
#define J3DMATERIALFACTORY_V21_H

#include "JSystem/J3DGraphBase/J3DMatBlock.h"
#include "JSystem/J3DGraphLoader/J3DModelLoader.h"
#include <dolphin/gx.h>

class J3DMaterial;
struct J3DTexCoord2Info;
struct J3DCurrentMtxInfo;

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DMaterialInitData_v21 {
    /* 0x000 */ u8 mMaterialMode;
    /* 0x001 */ u8 mCullModeIdx;
    /* 0x002 */ u8 mColorChanNumIdx;
    /* 0x003 */ u8 mTexGenNumIdx;
    /* 0x004 */ u8 mTevStageNumIdx;
    /* 0x005 */ u8 mZCompLocIdx;
    /* 0x006 */ u8 mZModeIdx;
    /* 0x007 */ u8 mDitherIdx;
    /* 0x008 */ u16 mMatColorIdx[2];
    /* 0x00C */ u16 mColorChanIdx[4];
    /* 0x014 */ u16 mTexCoordIdx[8];
    /* 0x024 */ u8 field_0x038[0x10];
    /* 0x034 */ u16 mTexMtxIdx[8];
    /* 0x044 */ u8 field_0x058[0x2c];
    /* 0x070 */ u16 mTexNoIdx[8];
    /* 0x080 */ u16 mTevKColorIdx[4];
    /* 0x088 */ u8 mTevKColorSel[0x10];
    /* 0x098 */ u8 mTevKAlphaSel[0x10];
    /* 0x0A8 */ u16 mTevOrderIdx[0x10];
    /* 0x0C8 */ u16 mTevColorIdx[4];
    /* 0x0D0 */ u16 mTevStageIdx[0x10];
    /* 0x0F0 */ u16 mTevSwapModeIdx[0x10];
    /* 0x110 */ u16 mTevSwapModeTableIdx[4];
    /* 0x118 */ u8 field_0x12c[0x18];
    /* 0x130 */ u16 mFogIdx;
    /* 0x132 */ u16 mAlphaCompIdx;
    /* 0x134 */ u16 mBlendIdx;
    /* 0x136 */ u16 mNBTScaleIdx;
}; // size 0x138

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DMaterialFactory_v21 {
public:
    J3DMaterialFactory_v21(J3DMaterialBlock_v21 const&);
    u16 countUniqueMaterials();
    u32 countTexGens(int) const;
    u32 countStages(int) const;
    J3DMaterial* create(J3DMaterial*, int, u32) const;
    J3DGXColor newMatColor(int, int) const;
    u8 newColorChanNum(int) const;
    J3DColorChan newColorChan(int, int) const;
    u32 newTexGenNum(int) const;
    J3DTexCoord newTexCoord(int, int) const;
    J3DTexMtx* newTexMtx(int, int) const;
    u8 newCullMode(int) const;
    u16 newTexNo(int, int) const;
    J3DTevOrder newTevOrder(int, int) const;
    J3DGXColorS10 newTevColor(int, int) const;
    J3DGXColor newTevKColor(int, int) const;
    u8 newTevStageNum(int) const;
    J3DTevStage newTevStage(int, int) const;
    J3DTevSwapModeTable newTevSwapModeTable(int, int) const;
    J3DFog newFog(int) const;
    J3DAlphaComp newAlphaComp(int) const;
    J3DBlend newBlend(int) const;
    J3DZMode newZMode(int) const;
    u8 newZCompLoc(int) const;
    u8 newDither(int) const;
    J3DNBTScale newNBTScale(int) const;

    u16 getMaterialID(u16 idx) { return mpMaterialID[idx]; }
    u8 getMaterialMode(int idx) const { return mpMaterialInitData[mpMaterialID[idx]].mMaterialMode; }
    
    /* 0x00 */ u16 mMaterialNum;
    /* 0x04 */ J3DMaterialInitData_v21* mpMaterialInitData;
    /* 0x08 */ u16* mpMaterialID;
    /* 0x0C */ GXColor* mpMatColor;
    /* 0x10 */ u8* mpColorChanNum;
    /* 0x14 */ J3DColorChanInfo* mpColorChanInfo;
    /* 0x18 */ u8* mpTexGenNum;
    /* 0x1C */ J3DTexCoordInfo* mpTexCoordInfo;
    /* 0x20 */ J3DTexCoord2Info* mpTexCoord2Info;
    /* 0x24 */ J3DTexMtxInfo* mpTexMtxInfo;
    /* 0x28 */ J3DTexMtxInfo* field_0x28;
    /* 0x2C */ u16* mpTexNo;
    /* 0x30 */ GXCullMode* mpCullMode;
    /* 0x34 */ J3DTevOrderInfo* mpTevOrderInfo;
    /* 0x38 */ GXColorS10* mpTevColor;
    /* 0x3C */ GXColor* mpTevKColor;
    /* 0x40 */ u8* mpTevStageNum;
    /* 0x44 */ J3DTevStageInfo* mpTevStageInfo;
    /* 0x48 */ J3DTevSwapModeInfo* mpTevSwapModeInfo;
    /* 0x4C */ J3DTevSwapModeTableInfo* mpTevSwapModeTableInfo;
    /* 0x50 */ J3DFogInfo* mpFogInfo;
    /* 0x54 */ J3DAlphaCompInfo* mpAlphaCompInfo;
    /* 0x58 */ J3DBlendInfo* mpBlendInfo;
    /* 0x5C */ J3DZModeInfo* mpZModeInfo;
    /* 0x60 */ u8* mpZCompLoc;
    /* 0x64 */ u8* mpDither;
    /* 0x68 */ J3DNBTScaleInfo* mpNBTScaleInfo;
    /* 0x6C */ u16 field_0x6c;
    /* 0x6E */ u8 field_0x6e;
    /* 0x6F */ u8 field_0x6f;
};

#endif /* J3DMATERIALFACTORY_V21_H */
