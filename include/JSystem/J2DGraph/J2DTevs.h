#ifndef J2DTEVS_H
#define J2DTEVS_H

#include <dolphin/gx.h>
#include "dolphin/mtx.h"
#include "global.h"

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DTextureSRTInfo {
    /* 0x00 */ f32 mScaleX;
    /* 0x04 */ f32 mScaleY;
    /* 0x08 */ f32 mRotationDeg;
    /* 0x0C */ f32 mTranslationX;
    /* 0x10 */ f32 mTranslationY;
};  // Size: 0x14

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DTexMtxInfo {
    enum {
        /* 0x0 */ DCC_NONE,
        /* 0x1 */ DCC_MAYA,
    };

    /* 0x00 */ u8 mTexMtxType;
    /* 0x01 */ u8 mTexMtxDCC;
    /* 0x02 */ u8 field_0x2;  // padding ?
    /* 0x03 */ u8 field_0x3;  // padding ?
    /* 0x04 */ Vec mCenter;
    /* 0x10 */ J2DTextureSRTInfo mTexSRTInfo;

    GXTexMtxType getTexMtxType() const { return (GXTexMtxType)mTexMtxType; }
    J2DTexMtxInfo& operator=(const J2DTexMtxInfo& other) {
        mCenter = other.mCenter;
        mTexMtxType = other.mTexMtxType;
        mTexMtxDCC = other.mTexMtxDCC;
        mTexSRTInfo = other.mTexSRTInfo;
        return *this;
    }

};  // Size: 0x24

extern J2DTexMtxInfo const j2dDefaultTexMtxInfo;

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DTexMtx {
public:
    J2DTexMtx() { mInfo = j2dDefaultTexMtxInfo; }
    ~J2DTexMtx() {}
    J2DTexMtx(const J2DTexMtxInfo& info) { mInfo = info; }
    void load(u32);
    void calc();
    void getTextureMtx(J2DTextureSRTInfo const&, Vec, Mtx);
    void getTextureMtxMaya(J2DTextureSRTInfo const&, Mtx);
    J2DTexMtxInfo& getTexMtxInfo() { return mInfo; }
    void setTexMtxInfo(J2DTexMtxInfo info) { mInfo = info; }

private:
    /* 0x00 */ J2DTexMtxInfo mInfo;
    /* 0x24 */ Mtx mTexMtx;
};

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DIndTexOrderInfo {
    /* 0x0 */ u8 mTexCoordID;
    /* 0x1 */ u8 mTexMapID;

    J2DIndTexOrderInfo& operator=(const J2DIndTexOrderInfo& other) {
        mTexCoordID = other.mTexCoordID;
        mTexMapID = other.mTexMapID;
        return *this;
    }
    GXTexCoordID getTexCoordID() const { return (GXTexCoordID)mTexCoordID; }
    GXTexMapID getTexMapID() const { return (GXTexMapID)mTexMapID; }
};

extern const J2DIndTexOrderInfo j2dDefaultIndTexOrderNull;

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DIndTexOrder {
public:
    J2DIndTexOrder() {
        mInfo = j2dDefaultIndTexOrderNull;
    }
    J2DIndTexOrder(const J2DIndTexOrderInfo& info) {
        mInfo = info;
    }
    J2DIndTexOrder& operator=(const J2DIndTexOrderInfo& info) {
        mInfo = info;
        return *this;
    }
    void load(u8);

private:
    /* 0x0 */ J2DIndTexOrderInfo mInfo;
};

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DIndTexMtxInfo {
    /* 0x00 */ Mtx23 mMtx;
    /* 0x18 */ s8 mScaleExp;

    J2DIndTexMtxInfo& operator=(const J2DIndTexMtxInfo& other) {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 3; j++) {
                mMtx[i][j] = other.mMtx[i][j];
            }
        }
        mScaleExp = other.mScaleExp;
        return *this;
    }
};

extern J2DIndTexMtxInfo const j2dDefaultIndTexMtxInfo;

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DIndTexMtx {
public:
    ~J2DIndTexMtx() {}
    J2DIndTexMtx() {
        mIndTexMtxInfo = j2dDefaultIndTexMtxInfo;
    }
    J2DIndTexMtx(const J2DIndTexMtxInfo& info) {
        mIndTexMtxInfo = info;
    }
    J2DIndTexMtx& operator=(const J2DIndTexMtxInfo& info) {
        mIndTexMtxInfo = info;
        return *this;
    }
    void load(u8);
    void load(u32);
    void calc();

private:
    /* 0x0 */ J2DIndTexMtxInfo mIndTexMtxInfo;
};  // Size: 0x1C

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DIndTexCoordScaleInfo {
    /* 0x0 */ u8 mScaleS;
    /* 0x1 */ u8 mScaleT;

    J2DIndTexCoordScaleInfo& operator=(const J2DIndTexCoordScaleInfo& other) {
        mScaleS = other.mScaleS;
        mScaleT = other.mScaleT;
        return *this;
    }
    GXIndTexScale getScaleS() const { return (GXIndTexScale)mScaleS; }
    GXIndTexScale getScaleT() const { return (GXIndTexScale)mScaleT; }
};

extern const J2DIndTexCoordScaleInfo j2dDefaultIndTexCoordScaleInfo;

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DIndTexCoordScale {
public:
    ~J2DIndTexCoordScale() {}
    J2DIndTexCoordScale() {
        mInfo = j2dDefaultIndTexCoordScaleInfo;
    }
    J2DIndTexCoordScale(const J2DIndTexCoordScaleInfo& info) {
        mInfo = info;
    }
    J2DIndTexCoordScale& operator=(const J2DIndTexCoordScaleInfo& info) {
        mInfo = info;
        return *this;
    }
    void load(u8);

private:
    /* 0x0 */ J2DIndTexCoordScaleInfo mInfo;
};  // Size: 0x2

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DIndTevStageInfo {
    /* 0x0 */ u8 mIndStage;
    /* 0x1 */ u8 mIndFormat;
    /* 0x2 */ u8 mBiasSel;
    /* 0x3 */ u8 mMtxSel;
    /* 0x4 */ u8 mWrapS;
    /* 0x5 */ u8 mWrapT;
    /* 0x6 */ u8 mPrev;
    /* 0x7 */ u8 mLod;
    /* 0x8 */ u8 mAlphaSel;
    /* 0x9 */ u8 field_0x9;
    /* 0xa */ u8 field_0xa;
    /* 0xb */ u8 field_0xb;

    J2DIndTevStageInfo& operator=(const J2DIndTevStageInfo& other) {
        mIndStage = other.mIndStage;
        mIndFormat = other.mIndFormat;
        mBiasSel = other.mBiasSel;
        mMtxSel = other.mMtxSel;
        mWrapS = other.mWrapS;
        mWrapT = other.mWrapT;
        mPrev = other.mPrev;
        mLod = other.mLod;
        mAlphaSel = other.mAlphaSel;
        return *this;
    }
};

inline u32 J2DCalcIndTevStage(J2DIndTevStageInfo info) {
    return (info.mAlphaSel << 22) | (info.mLod << 21) | (info.mPrev << 20) |
           (info.mMtxSel << 16) | (info.mWrapT << 11) | (info.mWrapS << 8) |
           (info.mBiasSel << 4)  | (info.mIndFormat << 2)  | (info.mIndStage);
}

extern const J2DIndTevStageInfo j2dDefaultIndTevStageInfo;

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DIndTevStage {
public:
    void load(u8);
    J2DIndTevStage() {
        mFlags = J2DCalcIndTevStage(j2dDefaultIndTevStageInfo);
    }
    J2DIndTevStage(const J2DIndTevStageInfo& info) {
        mFlags = J2DCalcIndTevStage(info);
    }

    void setIndTevStageInfo(const J2DIndTevStageInfo& info) { mFlags = J2DCalcIndTevStage(info); }

private:
    /* 0x0 */ u32 mFlags;

    GXIndTexStageID getIndStage() const { return (GXIndTexStageID)(mFlags & 0x03); }
    GXIndTexFormat getIndFormat() const { return (GXIndTexFormat)((mFlags >> 2) & 0x03); }
    GXIndTexBiasSel getBiasSel() const { return (GXIndTexBiasSel)((mFlags >> 4) & 0x07); }
    GXIndTexWrap getWrapS() const { return (GXIndTexWrap)((mFlags >> 8) & 0x07); }
    GXIndTexWrap getWrapT() const { return (GXIndTexWrap)((mFlags >> 11) & 0x07); }
    GXIndTexMtxID getMtxSel() const { return (GXIndTexMtxID)((mFlags >> 16) & 0x0F); }
    GXBool getPrev() const { return (GXBool)((mFlags >> 20) & 0x01); }
    GXBool getLod() const { return (GXBool)((mFlags >> 21) & 0x01); }
    GXIndTexAlphaSel getAlphaSel() const { return (GXIndTexAlphaSel)((mFlags >> 22) & 0x03); }
};

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DTexCoordInfo {
    /* 0x0 */ u8 mTexGenType;
    /* 0x1 */ u8 mTexGenSrc;
    /* 0x2 */ u8 mTexGenMtx;
    u8 padding;  // ?

    J2DTexCoordInfo& operator=(const J2DTexCoordInfo& other) {
        mTexGenType = other.mTexGenType;
        mTexGenSrc = other.mTexGenSrc;
        mTexGenMtx = other.mTexGenMtx;
        return *this;
    }
};

extern J2DTexCoordInfo const j2dDefaultTexCoordInfo[8];

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DTexCoord {
public:
    inline J2DTexCoord() {
        mTexCoordInfo = j2dDefaultTexCoordInfo[0];
    }

    J2DTexCoord(const J2DTexCoordInfo& info) {
        mTexCoordInfo = info;
    }
    void setTexCoordInfo(const J2DTexCoordInfo& info) { mTexCoordInfo = info; }
    void setTexGenMtx(u8 texGenMtx) { mTexCoordInfo.mTexGenMtx = texGenMtx; }
    u8 getTexGenType() const { return mTexCoordInfo.mTexGenType; }
    u8 getTexGenSrc() const { return mTexCoordInfo.mTexGenSrc; }
    u8 getTexGenMtx() const { return mTexCoordInfo.mTexGenMtx; }

private:
    /* 0x0 */ J2DTexCoordInfo mTexCoordInfo;
};

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DTevOrderInfo {
    /* 0x0 */ u8 mTexCoord;
    /* 0x1 */ u8 mTexMap;
    /* 0x2 */ u8 mColor;
    /* 0x3 */ u8 field_0x3;

    J2DTevOrderInfo& operator=(const J2DTevOrderInfo& other) {
        mTexCoord = other.mTexCoord;
        mTexMap = other.mTexMap;
        mColor = other.mColor;
        return *this;
    }
};

extern const J2DTevOrderInfo j2dDefaultTevOrderInfoNull;

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DTevOrder {
public:
    J2DTevOrder() {
        mTevOrderInfo = j2dDefaultTevOrderInfoNull;
    }

    J2DTevOrder(const J2DTevOrderInfo& info) {
        *(J2DTevOrderInfo*)this = info;
    }

    void setTevOrderInfo(const J2DTevOrderInfo& info) {mTevOrderInfo = info; }
    u8 getColor() const { return mTevOrderInfo.mColor; }
    u8 getTexMap() const { return mTevOrderInfo.mTexMap; }
    u8 getTexCoord() const { return mTevOrderInfo.mTexCoord; }

    /* 0x0 */ J2DTevOrderInfo mTevOrderInfo;
};

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DTevStageInfo {
    /* 0x00 */ u8 field_0x0;
    /* 0x01 */ u8 mColorA;
    /* 0x02 */ u8 mColorB;
    /* 0x03 */ u8 mColorC;
    /* 0x04 */ u8 mColorD;
    /* 0x05 */ u8 mCOp;
    /* 0x06 */ u8 mCBias;
    /* 0x07 */ u8 mCScale;
    /* 0x08 */ u8 mCClamp;
    /* 0x09 */ u8 mCReg;
    /* 0x0A */ u8 mAlphaA;
    /* 0x0B */ u8 mAlphaB;
    /* 0x0C */ u8 mAlphaC;
    /* 0x0D */ u8 mAlphaD;
    /* 0x0E */ u8 mAOp;
    /* 0x0F */ u8 mABias;
    /* 0x10 */ u8 mAScale;
    /* 0x11 */ u8 mAClamp;
    /* 0x12 */ u8 mAReg;
    /* 0x13 */ u8 field_0x13;
};

extern J2DTevStageInfo const j2dDefaultTevStageInfo;

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DTevSwapModeInfo {
    /* 0x0 */ u8 mRasSel;
    /* 0x1 */ u8 mTexSel;
    /* 0x2 */ u8 field_0x2;
    /* 0x3 */ u8 field_0x3;
};

extern const J2DTevSwapModeInfo j2dDefaultTevSwapMode;

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DTevStage {
public:
    J2DTevStage(J2DTevStageInfo const& param_0) {
        setTevStageInfo(param_0);
        setTevSwapModeInfo(j2dDefaultTevSwapMode);
    }
    J2DTevStage() {
        setTevStageInfo(j2dDefaultTevStageInfo);
        setTevSwapModeInfo(j2dDefaultTevSwapMode);
    }
    void setTevStageInfo(J2DTevStageInfo const& info) {
        setColorABCD(info.mColorA, info.mColorB, info.mColorC, info.mColorD);
        setTevColorOp(info.mCOp, info.mCBias, info.mCScale, info.mCClamp, info.mCReg);
        setAlphaABCD(info.mAlphaA, info.mAlphaB, info.mAlphaC, info.mAlphaD);
        setTevAlphaOp(info.mAOp, info.mABias, info.mAScale, info.mAClamp, info.mAReg);
    }

    void setStageNo(u32 param_0) {
        field_0x0 = (param_0 << 1) + 0xc0;
        field_0x4 = (param_0 << 1) + 0xc1;
    }

    void setTevSwapModeInfo(const J2DTevSwapModeInfo& swapInfo) {
        setTexSel(swapInfo.mTexSel);
        setRasSel(swapInfo.mRasSel);
    }

    void setTexSel(u8 param_0) {
        field_0x7 = (field_0x7 & ~0x0c) | (param_0 * 4);
    }

    void setRasSel(u8 param_0) {
        field_0x7 = (field_0x7 & ~0x03) | param_0;
    }

    void setColorABCD(u8 a, u8 b, u8 c, u8 d) {
        setTevColorAB(a, b);
        setTevColorCD(c, d);
    }

    void setTevColorAB(u8 a, u8 b) { field_0x2 = a << 4 | b; }
    void setTevColorCD(u8 c, u8 d) { field_0x3 = c << 4 | d; }

    void setTevColorOp(u8 op, u8 bias, u8 scale, u8 clamp, u8 reg) {
        field_0x1 = field_0x1 & ~0x04 | op << 2;
        if (op <= 1) {
            field_0x1 = field_0x1 & ~0x30 | scale << 4;
            field_0x1 = field_0x1 & ~0x03 | bias;
        } else {
            field_0x1 = field_0x1 & ~0x30 | (op >> 1 & 3) << 4;
            field_0x1 = field_0x1 & ~0x03 | 3;
        }
        field_0x1 = field_0x1 & ~0x08 | clamp << 3;
        field_0x1 = field_0x1 & ~0xc0 | reg << 6;
    }

    void setAlphaABCD(u8 a, u8 b, u8 c, u8 d) {
        setAlphaA(a);
        setAlphaB(b);
        setAlphaC(c);
        setAlphaD(d);
    }

    void setAlphaA(u8 a) {
        field_0x6 = field_0x6 & ~0xe0 | a << 5;
    }
    void setAlphaB(u8 b) {
        field_0x6 = field_0x6 & ~0x1c | b << 2;
    }
    void setAlphaC(u8 c) {
        field_0x6 = field_0x6 & ~0x03 | c >> 1;
        field_0x7 = field_0x7 & ~0x80 | c << 7;
    }
    void setAlphaD(u8 d) {
        field_0x7 = field_0x7 & ~0x70 | d << 4;
    }
    void setTevAlphaOp(u8 op, u8 bias, u8 scale, u8 clamp, u8 reg) {
        field_0x5 = field_0x5 & ~0x04 | op << 2;
        if (op <= 1) {
            field_0x5 = field_0x5 & ~0x03 | bias;
            field_0x5 = field_0x5 & ~0x30 | scale << 4;
        } else {
            field_0x5 = field_0x5 & ~0x30 | (op >> 1 & 3) << 4;
            field_0x5 = field_0x5 & ~0x03 | 3;
        }
        field_0x5 = field_0x5 & ~0x08 | clamp << 3;
        field_0x5 = field_0x5 & ~0xc0 | reg << 6;
    }

    u8 getColorA() const { return (field_0x2 & 0xf0) >> 4; }
    u8 getColorB() const { return field_0x2 & 0x0f; }
    u8 getColorC() const { return (field_0x3 & 0xf0) >> 4; }
    u8 getColorD() const { return field_0x3 & 0x0f; }
    u8 getAlphaA() const { return (field_0x6 & 0xe0) >> 5; }
    u8 getAlphaB() const { return (field_0x6 & 0x1c) >> 2; }
    u8 getAlphaC() const { return (field_0x6 & 0x03) << 1 | (field_0x7 & 0x80) >> 7; }
    u8 getAlphaD() const { return (field_0x7 & 0x70) >> 4; }
    u8 getCOp() const {
        if (getCBias() != 3) {
            return (field_0x1 & 4) >> 2;
        }
        return ((field_0x1 & 4) >> 2) + 8 + ((field_0x1 & 0x30) >> 3);
    }
    u8 getCBias() const { return field_0x1 & 0x03; }
    u8 getCScale() const { return (field_0x1 & 0x30) >> 4; }
    u8 getCClamp() const { return (field_0x1 & 0x08) >> 3; }
    u8 getCReg() const { return (field_0x1 & 0xc0) >> 6; }
    u8 getAOp() const {
        if (getABias() != 3) {
            return (field_0x5 & 4) >> 2;
        }
        return ((field_0x5 & 4) >> 2) + 8 + ((field_0x5 & 0x30) >> 3);
    }
    u8 getABias() const { return field_0x5 & 0x03; }
    u8 getAScale() const { return (field_0x5 & 0x30) >> 4; }
    u8 getAClamp() const { return (field_0x5 & 0x08) >> 3; }
    u8 getAReg() const { return (field_0x5 & 0xc0) >> 6; }
    u8 getRasSel() const { return field_0x7 & 3; }
    u8 getTexSel() const { return (field_0x7 & 0x0c) >> 2; }

    void operator=(J2DTevStage const& other) {
        field_0x1 = other.field_0x1;
        field_0x2 = other.field_0x2;
        field_0x3 = other.field_0x3;
        field_0x5 = other.field_0x5;
        field_0x6 = other.field_0x6;
        field_0x7 = other.field_0x7;
    }

private:
    /* 0x0 */ u8 field_0x0;
    /* 0x1 */ u8 field_0x1;
    /* 0x2 */ u8 field_0x2;
    /* 0x3 */ u8 field_0x3;
    /* 0x4 */ u8 field_0x4;
    /* 0x5 */ u8 field_0x5;
    /* 0x6 */ u8 field_0x6;
    /* 0x7 */ u8 field_0x7;
};

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DTevSwapModeTableInfo {
    /* 0x0 */ u8 field_0x0;
    /* 0x1 */ u8 field_0x1;
    /* 0x2 */ u8 field_0x2;
    /* 0x3 */ u8 field_0x3;

    J2DTevSwapModeTableInfo& operator=(const J2DTevSwapModeTableInfo& other) {
        field_0x0 = other.field_0x0;
        field_0x1 = other.field_0x1;
        field_0x2 = other.field_0x2;
        field_0x3 = other.field_0x3;
        return *this;
    }
};

inline u8 J2DCalcTevSwapTable(u8 param_0, u8 param_1, u8 param_2, u8 param_3) {
    return (param_0 << 6) + (param_1 << 4) + (param_2 << 2) + param_3;
}

extern const J2DTevSwapModeTableInfo j2dDefaultTevSwapModeTable;
extern const u8 data_804561AC;

/**
 * @ingroup jsystem-j2d
 * 
*/

class J2DTevSwapModeTable {
public:
    J2DTevSwapModeTable() { field_0x0 = data_804561AC; }
    J2DTevSwapModeTable(const J2DTevSwapModeTableInfo& info) {
        field_0x0 = J2DCalcTevSwapTable(info.field_0x0, info.field_0x1, info.field_0x2, info.field_0x3);
    }

    void setTevSwapModeTableInfo(const J2DTevSwapModeTableInfo& info) {
        field_0x0 = J2DCalcTevSwapTable(info.field_0x0, info.field_0x1, info.field_0x2, info.field_0x3);
    }
    u8 getR() const { return field_0x0 >> 6 & 3; }
    u8 getG() const { return field_0x0 >> 4 & 3; }
    u8 getB() const { return field_0x0 >> 2 & 3; }
    u8 getA() const { return field_0x0 & 3; }

private:
    /* 0x0 */ u8 field_0x0;
};

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DColorChanInfo {
    /* 0x0 */ u8 field_0x0;
    /* 0x0 */ u8 field_0x1;
    /* 0x0 */ u8 field_0x2;
    /* 0x0 */ u8 field_0x3;

    J2DColorChanInfo& operator=(const J2DColorChanInfo& other) {
        field_0x0 = other.field_0x0;
        field_0x1 = other.field_0x1;
        field_0x2 = other.field_0x2;
        field_0x3 = other.field_0x3;
        return *this;
    }
};

inline u16 J2DCalcColorChanID(u8 param_0) { return param_0; }
extern const J2DColorChanInfo j2dDefaultColorChanInfo;

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DColorChan {
public:
    J2DColorChan() {
        setColorChanInfo(j2dDefaultColorChanInfo);
    }
    J2DColorChan(const J2DColorChanInfo& info) {
        mColorChan = J2DCalcColorChanID(info.field_0x1);
    }

    void setColorChanInfo(const J2DColorChanInfo& info) {
        mColorChan = J2DCalcColorChanID(info.field_0x1);
    }

    u8 getMatSrc() const { return mColorChan & 1; }

private:
    /* 0x0 */ u16 mColorChan;
};

extern const GXColor j2dDefaultColInfo;
extern const GXColorS10 j2dDefaultTevColor;
extern const GXColor j2dDefaultTevKColor;
extern const J2DTevOrderInfo j2dDefaultTevOrderInfoNull;
extern const u8 j2dDefaultPEBlockDither;
extern const u8 data_804561AC;
extern const u16 j2dDefaultAlphaCmp;

#endif /* J2DTEVS_H */
