#ifndef J3DTEVS_H
#define J3DTEVS_H

#include <dolphin/types.h>
#include <dolphin/gx.h>
#include "JSystem/J3DGraphBase/J3DGD.h"
#include "JSystem/J3DGraphBase/J3DStruct.h"

extern u8 j3dTevSwapTableTable[1024];

extern const J3DLightInfo j3dDefaultLightInfo;
extern const J3DTexCoordInfo j3dDefaultTexCoordInfo[8];
extern const J3DTexMtxInfo j3dDefaultTexMtxInfo;
extern const J3DIndTexMtxInfo j3dDefaultIndTexMtxInfo;
extern const J3DTevStageInfo j3dDefaultTevStageInfo;
extern const J3DIndTevStageInfo j3dDefaultIndTevStageInfo;
extern const J3DFogInfo j3dDefaultFogInfo;
extern const J3DNBTScaleInfo j3dDefaultNBTScaleInfo;

extern const GXColor j3dDefaultColInfo;
extern const GXColor j3dDefaultAmbInfo;
extern const u8 j3dDefaultColorChanNum;
extern const J3DTevOrderInfo j3dDefaultTevOrderInfoNull;
extern const J3DIndTexOrderInfo j3dDefaultIndTexOrderNull;
extern const GXColorS10 j3dDefaultTevColor;
extern const J3DIndTexCoordScaleInfo j3dDefaultIndTexCoordScaleInfo;
extern const GXColor j3dDefaultTevKColor;
extern const J3DTevSwapModeInfo j3dDefaultTevSwapMode;
extern const J3DTevSwapModeTableInfo j3dDefaultTevSwapModeTable;
extern const J3DBlendInfo j3dDefaultBlendInfo;
extern const J3DColorChanInfo j3dDefaultColorChanInfo;
extern const u8 j3dDefaultTevSwapTableID;
extern const u16 j3dDefaultAlphaCmpID;
extern const u16 j3dDefaultZModeID;

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DTevStage {
    J3DTevStage() {
        setTevStageInfo(j3dDefaultTevStageInfo);
        setTevSwapModeInfo(j3dDefaultTevSwapMode);
    }

    void setTevColorOp(u8 param_1, u8 param_2, u8 param_3, u8 param_4, u8 param_5) {
        mTevColorOp = mTevColorOp & ~(0x01 << 2) | param_1 << 2;
        if (param_1 <= 1) {
            mTevColorOp = mTevColorOp & ~(0x03 << 4) | param_3 << 4;
            mTevColorOp = mTevColorOp & ~0x03 | param_2;
        } else {
            mTevColorOp = mTevColorOp & ~(0x03 << 4) | (param_1 >> 1 & 3) << 4;
            mTevColorOp = mTevColorOp & ~0x03 | 3;
        }
        mTevColorOp = mTevColorOp & ~(0x01 << 3) | param_4 << 3;
        mTevColorOp = mTevColorOp & ~(0x03 << 6) | param_5 << 6;
    }

    void setTevColorAB(u8 a, u8 b) { mTevColorAB = a << 4 | b; }
    void setTevColorCD(u8 c, u8 d) { mTevColorCD = c << 4 | d; }
    void setAlphaA(u8 a) { mTevAlphaAB = mTevAlphaAB & ~(0x07 << 5) | a << 5; }
    void setAlphaB(u8 b) { mTevAlphaAB = mTevAlphaAB & ~(0x07 << 2) | b << 2; }
    void setAlphaC(u8 c) {
        mTevAlphaAB = mTevAlphaAB & ~0x03 | c >> 1;
        mTevSwapModeInfo = mTevSwapModeInfo & ~(0x01 << 7) | c << 7;
    }
    void setAlphaD(u8 d) { mTevSwapModeInfo = mTevSwapModeInfo & ~(0x07 << 4) | d << 4; }
    void setAlphaABCD(u8 a, u8 b, u8 c, u8 d) {
        setAlphaA(a);
        setAlphaB(b);
        setAlphaC(c);
        setAlphaD(d);
    }

    void setTevAlphaOp(u8 param_1, u8 param_2, u8 param_3, u8 param_4, u8 param_5) {
        mTevAlphaOp = mTevAlphaOp & ~(0x01 << 2) | param_1 << 2;
        if (param_1 <= 1) {
            mTevAlphaOp = mTevAlphaOp & ~0x03 | param_2;
            mTevAlphaOp = mTevAlphaOp & ~(0x03 << 4) | param_3 << 4;
        } else {
            mTevAlphaOp = mTevAlphaOp & ~(0x03 << 4) | (param_1 >> 1 & 3) << 4;
            mTevAlphaOp = mTevAlphaOp & ~0x03 | 3;
        }
        mTevAlphaOp = mTevAlphaOp & ~(0x01 << 3) | param_4 << 3;
        mTevAlphaOp = mTevAlphaOp & ~(0x03 << 6) | param_5 << 6;
    }

    void setTevStageInfo(const J3DTevStageInfo& info) {
        setTevColorOp(info.mTevAlphaOp, info.mTevAlphaAB, info.mTevSwapModeInfo, info.field_0x8, info.field_0x9);
        setTevColorAB(info.mTevColorOp, info.mTevColorAB);
        setTevColorCD(info.mTevColorCD, info.field_0x4);
        setAlphaABCD(info.field_0xa, info.field_0xb, info.field_0xc, info.field_0xd);
        setTevAlphaOp(info.field_0xe, info.field_0xf, info.field_0x10, info.field_0x11, info.field_0x12);
    }

    J3DTevStage(J3DTevStageInfo const& param_0) {
        setTevStageInfo(param_0);
        setTevSwapModeInfo(j3dDefaultTevSwapMode);
    }

    void setTevSwapModeInfo(J3DTevSwapModeInfo const& param_0) {
        setTexSel(param_0.mTexSel);
        setRasSel(param_0.mRasSel);
    }

    void setStageNo(u32 param_0) {
        field_0x0 = 0xC0 + param_0 * 2;
        field_0x4 = 0xC1 + param_0 * 2;
    }

    void setRasSel(u8 ras_sel) { mTevSwapModeInfo = (mTevSwapModeInfo & ~3) | ras_sel; }
    void setTexSel(u8 tex_sel) { mTevSwapModeInfo = (mTevSwapModeInfo & ~0xc) | (tex_sel << 2); }

    void load(u32 param_1) const {
        J3DGDWriteBPCmd(*(u32*)&field_0x0);
        J3DGDWriteBPCmd(*(u32*)&field_0x4);
    }

    J3DTevStage& operator=(const J3DTevStage& other) {
        mTevColorOp = other.mTevColorOp;
        mTevColorAB = other.mTevColorAB;
        mTevColorCD = other.mTevColorCD;
        mTevAlphaOp = other.mTevAlphaOp;
        mTevAlphaAB = other.mTevAlphaAB;
        mTevSwapModeInfo = other.mTevSwapModeInfo;
        return *this;
    }

    J3DTevStage& operator=(J3DTevStage& other) {
        mTevColorOp = other.mTevColorOp;
        mTevColorAB = other.mTevColorAB;
        mTevColorCD = other.mTevColorCD;
        mTevAlphaOp = other.mTevAlphaOp;
        mTevAlphaAB = other.mTevAlphaAB;
        mTevSwapModeInfo = other.mTevSwapModeInfo;
        return *this;
    }

    /* 0x0 */ u8 field_0x0;
    /* 0x1 */ u8 mTevColorOp;
    /* 0x2 */ u8 mTevColorAB;
    /* 0x3 */ u8 mTevColorCD;
    /* 0x4 */ u8 field_0x4;
    /* 0x5 */ u8 mTevAlphaOp;
    /* 0x6 */ u8 mTevAlphaAB;
    /* 0x7 */ u8 mTevSwapModeInfo;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DIndTevStage {
    J3DIndTevStage() : mInfo(0) { setIndTevStageInfo(j3dDefaultIndTevStageInfo); }
    J3DIndTevStage(J3DIndTevStageInfo const& info) : mInfo(0) { setIndTevStageInfo(info); }
    void setIndTevStageInfo(J3DIndTevStageInfo const& info) {
        setIndStage(info.mIndStage);
        setIndFormat(info.mIndFormat);
        setBiasSel(info.mBiasSel);
        setMtxSel(info.mMtxSel);
        setWrapS(info.mWrapS);
        setWrapT(info.mWrapT);
        setPrev(info.mPrev);
        setLod(info.mLod);
        setAlphaSel(info.mAlphaSel);
    }
    void setIndStage(u8 indStage) { mInfo = (mInfo & ~3) | indStage; }
    void setIndFormat(u8 indFormat) { mInfo = (mInfo & ~0xc) | (indFormat << 2); }
    void setBiasSel(u8 biasSel) { mInfo = (mInfo & ~0x70) | (biasSel << 4); }
    void setMtxSel(u8 mtxSel) { mInfo = (mInfo & ~0x1e00) | (mtxSel << 9); }
    void setWrapS(u8 wrapS) { mInfo = (mInfo & ~0xe000) | (wrapS << 13); }
    void setWrapT(u8 wrapT) { mInfo = (mInfo & ~0x70000) | (wrapT << 16); }
    void setPrev(u8 prev) { mInfo = (mInfo & ~0x100000) | (prev << 20); }
    void setLod(u8 lod) { mInfo = (mInfo & ~0x80000) | (lod << 19); }
    void setAlphaSel(u8 alphaSel) { mInfo = (mInfo & ~0x180) | (alphaSel << 7); }

    void load(u32 param_1) const {
        J3DGDWriteBPCmd(mInfo | (param_1 + 0x10) * 0x1000000);
    }

    J3DIndTevStage& operator=(const J3DIndTevStage& other) {
        mInfo = other.mInfo;
        return *this;
    }

    J3DIndTevStage& operator=(J3DIndTevStage& other) {
        mInfo = other.mInfo;
        return *this;
    }

    /* 0x0 */ u32 mInfo;
};

extern const J3DTevOrderInfo j3dDefaultTevOrderInfoNull;

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DTevOrder : public J3DTevOrderInfo {
    J3DTevOrder() {
        J3DTevOrderInfo::operator=(j3dDefaultTevOrderInfoNull);
    }
    J3DTevOrder(const J3DTevOrderInfo& info) : J3DTevOrderInfo(info) {}
    J3DTevOrderInfo& getTevOrderInfo() { return *this; }

    u8 getTexMap() { return mTexMap; }
};

extern u8 j3dTevSwapTableTable[1024];
extern u8 const j3dDefaultTevSwapTableID;

inline u8 calcTevSwapTableID(u8 param_0, u8 param_1, u8 param_2, u8 param_3) {
    return 0x40 * (u8)param_0 + 0x10 * (u8)param_1 + 4 * (u8)param_2 + param_3;
}

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DTevSwapModeTable {
    J3DTevSwapModeTable() { mIdx = j3dDefaultTevSwapTableID; }
    J3DTevSwapModeTable(J3DTevSwapModeTableInfo const& info) {
        mIdx = calcTevSwapTableID(info.field_0x0, info.field_0x1, info.field_0x2, info.field_0x3);
    }

    J3DTevSwapModeTable& operator=(const J3DTevSwapModeTable& rhs) {
        mIdx = rhs.mIdx;
        return *this;
    }

    J3DTevSwapModeTable& operator=(J3DTevSwapModeTable& other) {
        mIdx = other.mIdx;
        return *this;
    }

    u8 getR() const { return *(&j3dTevSwapTableTable[mIdx * 4] + 0); }
    u8 getG() const { return *(&j3dTevSwapTableTable[mIdx * 4] + 1); }
    u8 getB() const { return *(&j3dTevSwapTableTable[mIdx * 4] + 2); }
    u8 getA() const { return *(&j3dTevSwapTableTable[mIdx * 4] + 3); }

    /* 0x0 */ u8 mIdx;
};  // Size: 0x1

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DLightObj {
public:
    J3DLightObj() { mInfo = j3dDefaultLightInfo; }
    void load(u32) const;

    J3DLightInfo* getLightInfo() { return &mInfo; }
    J3DLightObj& operator=(J3DLightObj const& other) {
        mInfo = other.mInfo;
        return *this;
    }

    /* 0x00 */ J3DLightInfo mInfo;
    /* 0x34 */ GXLightObj mLightObj;
};  // Size = 0x74

extern const J3DNBTScaleInfo j3dDefaultNBTScaleInfo;

/**
 * @ingroup jsystem-j3d
 *
 */
struct J3DNBTScale : public J3DNBTScaleInfo {
    J3DNBTScale() {
        mbHasScale = j3dDefaultNBTScaleInfo.mbHasScale;
        mScale.x = j3dDefaultNBTScaleInfo.mScale.x;
        mScale.y = j3dDefaultNBTScaleInfo.mScale.y;
        mScale.z = j3dDefaultNBTScaleInfo.mScale.z;
    }

    J3DNBTScale(J3DNBTScaleInfo const& info) {
        mbHasScale = info.mbHasScale;
        mScale.x = info.mScale.x;
        mScale.y = info.mScale.y;
        mScale.z = info.mScale.z;
    }

    Vec* getScale() { return &mScale; }
};

extern const GXColor j3dDefaultColInfo;
extern const GXColor j3dDefaultAmbInfo;
extern const GXColorS10 j3dDefaultTevColor;
extern const GXColor j3dDefaultTevKColor;
extern u8 j3dAlphaCmpTable[768];
extern const u8 j3dDefaultNumChans;

struct J3DNBTScale;
struct J3DTexCoord;
void loadNBTScale(J3DNBTScale& param_0);
void loadTexCoordGens(u32 param_0, J3DTexCoord* param_1);
void loadTexNo(u32 param_0, u16 const& param_1);
void patchTexNo_PtrToIdx(u32 texID, u16 const& idx);
bool isTexNoReg(void* param_0);
u16 getTexNoReg(void* param_0);
void makeTexCoordTable();
void makeAlphaCmpTable();
void makeZModeTable();
void makeTevSwapTable();

#endif /* J3DTEVS_H */
