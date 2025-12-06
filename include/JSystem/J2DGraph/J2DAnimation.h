#ifndef J2DANIMATION_H
#define J2DANIMATION_H

#include "JSystem/JMath/JMath.h"
#include "JSystem/J3DGraphAnimator/J3DAnimation.h"

typedef struct _GXColor GXColor;
typedef struct _GXColorS10 GXColorS10;
class J2DScreen;
class JUTPalette;
struct ResTIMG;

enum J2DAnmKind {
    KIND_TRANSFORM = 0,
    KIND_COLOR = 1,
    KIND_TEX_PATTERN = 2,
    KIND_TEXTURE_SRT = 4,
    KIND_TEV_REG = 5,
    KIND_VISIBILITY = 6,
    KIND_VTX_COLOR = 7
};

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DAnmBase {
public:
    J2DAnmBase() {
        mFrame = 0;
        mFrameMax = 0;
    }
    virtual ~J2DAnmBase() {}
    virtual void searchUpdateMaterialID(J2DScreen*) {}

    s16 getFrameMax() const { return mFrameMax; }
    void setFrame(f32 frame) { mFrame = frame; }
    f32 getFrame() const { return mFrame; }
    J2DAnmKind getKind() const { return mKind; }

    /* 0x0 */  // vtable
    /* 0x4 */ u8 field_0x4;
    /* 0x5 */ u8 field_0x5;
    /* 0x6 */ s16 mFrameMax;
    /* 0x8 */ f32 mFrame;
    /* 0xC */ J2DAnmKind mKind;
};  // Size: 0x10

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DAnmVtxColor : public J2DAnmBase {
public:
    J2DAnmVtxColor() {
        mKind = KIND_VTX_COLOR;
        for (s32 i = 0; i < ARRAY_SIZEU(mAnmTableNum); i++) {
            mAnmTableNum[i] = NULL;
        }
        for (s32 i = 0; i < ARRAY_SIZEU(mVtxColorIndexData); i++) {
            mVtxColorIndexData[i] = NULL;
        }
    }
    virtual ~J2DAnmVtxColor() {}
    virtual void getColor(u8, u16, _GXColor*) const {}
    u16 getAnmTableNum(u8 param_0) const { return mAnmTableNum[param_0]; }
    J3DAnmVtxColorIndexData* getAnmVtxColorIndexData(u8 param_1, u16 param_2) const {
        J3D_PANIC(344, param_1 < 2, "Error : range over.");
        J3D_PANIC(345, param_2 < mAnmTableNum[param_1], "Error : range over.");
        return &mVtxColorIndexData[param_1][param_2];
    }
    u16* getVtxColorIndexPointer(u8 param_0) const { return mVtxColorIndexPointer[param_0]; }

    /* 0x10 */ u16 mAnmTableNum[2];
    /* 0x14 */ J3DAnmVtxColorIndexData* mVtxColorIndexData[2];
    /* 0x1C */ u16* mVtxColorIndexPointer[2];
};  // Size: 0x24

struct J3DTransformInfo;
struct J3DTextureSRTInfo;

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DAnmVtxColorKey : public J2DAnmVtxColor {
public:
    J2DAnmVtxColorKey() {
        for (s32 i = 0; i < ARRAY_SIZEU(mInfoTable); i++) {
            mInfoTable[i] = NULL;
        }
    }
    virtual ~J2DAnmVtxColorKey() {}
    virtual void getColor(u8, u16, _GXColor*) const;

    /* 0x24 */ J3DAnmColorKeyTable* mInfoTable[2];
    /* 0x2C */ s16* mRValues;
    /* 0x30 */ s16* mGValues;
    /* 0x34 */ s16* mBValues;
    /* 0x38 */ s16* mAValues;
};  // Size: 0x3C

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DAnmVtxColorFull : public J2DAnmVtxColor {
public:
    J2DAnmVtxColorFull() {
        for (s32 i = 0; i < ARRAY_SIZEU(mInfoTable); i++) {
            mInfoTable[i] = NULL;
        }
    }
    virtual ~J2DAnmVtxColorFull() {}
    virtual void getColor(u8, u16, _GXColor*) const;

    /* 0x24 */ J3DAnmColorFullTable* mInfoTable[2];
    /* 0x2C */ u8* mRValues;
    /* 0x30 */ u8* mGValues;
    /* 0x34 */ u8* mBValues;
    /* 0x38 */ u8* mAValues;
};  // Size: 0x3C

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DAnmVisibilityFull : public J2DAnmBase {
public:
    J2DAnmVisibilityFull() {
        field_0x10 = 0;
        mTable = NULL;
        field_0x12 = 0;
        mValues = NULL;
        mKind = KIND_VISIBILITY;
    }
    virtual ~J2DAnmVisibilityFull() {}
    void getVisibility(u16, u8*) const;

    /* 0x10 */ u16 field_0x10;
    /* 0x12 */ u16 field_0x12;
    /* 0x14 */ J3DAnmVisibilityFullTable* mTable;
    /* 0x18 */ u8* mValues;
};  // Size: 0x1C

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DAnmTransform : public J2DAnmBase {
public:
    J2DAnmTransform(f32* pScaleValues, s16* pRotationValues, f32* pTranslateValues) {
        mScaleValues = pScaleValues;
        mRotationValues = pRotationValues;
        mTranslateValues = pTranslateValues;
        mKind = KIND_TRANSFORM;
    }
    virtual ~J2DAnmTransform() {}
    virtual void getTransform(u16, J3DTransformInfo*) const {}

    /* 0x10 */ f32* mScaleValues;
    /* 0x14 */ s16* mRotationValues;
    /* 0x18 */ f32* mTranslateValues;
};  // Size: 0x1C

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DAnmTransformKey : public J2DAnmTransform {
public:
    J2DAnmTransformKey() : J2DAnmTransform(NULL, NULL, NULL) {
        field_0x24 = 0;
        mInfoTable = NULL;
    }
    virtual ~J2DAnmTransformKey() {}
    virtual void getTransform(u16 p1, J3DTransformInfo* pInfo) const {
        this->calcTransform(getFrame(), p1, pInfo);
    }
    virtual void calcTransform(f32, u16, J3DTransformInfo*) const;

    /* 0x1C */ u8 field_0x1c[6];
    /* 0x22 */ s16 field_0x22;
    /* 0x24 */ u32 field_0x24;
    /* 0x28 */ J3DAnmTransformKeyTable* mInfoTable;
};

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DAnmTransformFull : public J2DAnmTransform {
public:
    J2DAnmTransformFull() : J2DAnmTransform(NULL, NULL, NULL) { mTableInfo = NULL; }
    virtual ~J2DAnmTransformFull() {}
    virtual void getTransform(u16, J3DTransformInfo*) const;

    /* 0x1C */ u8 field_0x1c[6];
    /* 0x22 */ u16 field_0x22;
    /* 0x24 */ J3DAnmTransformFullTable* mTableInfo;
};

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DAnmTextureSRTKey : public J2DAnmBase {
public:
    J2DAnmTextureSRTKey() {
        field_0x10 = 0;
        field_0x1e = 0;
        field_0x1c = 0;
        field_0x1a = 0;
        mUpdateMaterialNum = 0;
        mInfoTable = NULL;
        mTranslationValues = NULL;
        mScaleValues = NULL;
        mRotationValues = NULL;
        field_0x4c = 0;
        field_0x4a = 0;
        field_0x48 = 0;
        field_0x4e = 0;
        field_0x5c = NULL;
        field_0x58 = NULL;
        field_0x50 = NULL;
        field_0x54 = NULL;
        field_0x7c = 0;
        mKind = KIND_TEXTURE_SRT;
    }
    void calcTransform(f32, u16, J3DTextureSRTInfo*) const;

    virtual ~J2DAnmTextureSRTKey() {}
    virtual void searchUpdateMaterialID(J2DScreen*);
    u16 getUpdateMaterialNum() const { return mUpdateMaterialNum / 3; }
    u16 getUpdateMaterialID(u16 i) const { return mUpdateMaterialID[i]; }
    u8 getUpdateTexMtxID(u16 i) const { return mUpdateTexMtxID[i]; }
    void getTransform(u16 param_1, J3DTextureSRTInfo* param_2) {
        calcTransform(mFrame, param_1, param_2);
    }

    /* 0x10 */ int field_0x10;
    /* 0x14 */ J3DAnmTransformKeyTable* mInfoTable;
    /* 0x18 */ u16 mUpdateMaterialNum;
    /* 0x1A */ u16 field_0x1a;
    /* 0x1C */ u16 field_0x1c;
    /* 0x1E */ u16 field_0x1e;
    /* 0x20 */ f32* mScaleValues;
    /* 0x24 */ s16* mRotationValues;
    /* 0x28 */ f32* mTranslationValues;
    /* 0x2C */ u8* mUpdateTexMtxID;
    /* 0x30 */ u16* mUpdateMaterialID;
    /* 0x34 */ JUTNameTab field_0x34;
    /* 0x44 */ Vec* field_0x44;
    /* 0x48 */ u16 field_0x48;
    /* 0x4A */ u16 field_0x4a;
    /* 0x4C */ u16 field_0x4c;
    /* 0x4E */ u16 field_0x4e;
    /* 0x50 */ f32* field_0x50;
    /* 0x54 */ s16* field_0x54;
    /* 0x58 */ f32* field_0x58;
    /* 0x5C */ J3DAnmTransformKeyTable* field_0x5c;
    /* 0x60 */ u8* field_0x60;
    /* 0x64 */ u16* field_0x64;
    /* 0x68 */ JUTNameTab field_0x68;
    /* 0x78 */ Vec* field_0x78;
    /* 0x7C */ int field_0x7c;
};

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DAnmTexPattern : public J2DAnmBase {
public:
    struct J2DAnmTexPatternTIMGPointer {
        J2DAnmTexPatternTIMGPointer();
        ~J2DAnmTexPatternTIMGPointer();

        /* 0x0 */ ResTIMG* mRes;
        /* 0x4 */ JUTPalette* mPalette;
    };  // Size: 0x8

    J2DAnmTexPattern() {
        mValues = NULL;
        mAnmTable = NULL;
        mKind = KIND_TEX_PATTERN;
        mUpdateMaterialNum = 0;
        mUpdateMaterialID = NULL;
        mTIMGPtrArray = NULL;
    }

    void getTexNo(u16, u16*) const;
    ResTIMG* getResTIMG(u16) const;
    JUTPalette* getPalette(u16) const;

    virtual ~J2DAnmTexPattern() { delete[] mTIMGPtrArray; }
    virtual void searchUpdateMaterialID(J2DScreen*);
    u16 getUpdateMaterialNum() const { return mUpdateMaterialNum; }
    u16 getUpdateMaterialID(u16 i) const { return mUpdateMaterialID[i]; }
    J3DAnmTexPatternFullTable* getAnmTable() const { return mAnmTable; }

    /* 0x10 */ u16* mValues;
    /* 0x14 */ J3DAnmTexPatternFullTable* mAnmTable;
    /* 0x18 */ u16 field_0x18;
    /* 0x1A */ u16 mUpdateMaterialNum;
    /* 0x1C */ u16* mUpdateMaterialID;
    /* 0x20 */ JUTNameTab field_0x20;
    /* 0x30 */ J2DAnmTexPatternTIMGPointer* mTIMGPtrArray;
};

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DAnmTevRegKey : public J2DAnmBase {
public:
    J2DAnmTevRegKey() {
        mKRegUpdateMaterialNum = 0;
        mCRegUpdateMaterialNum = 0;
        field_0x1a = 0;
        field_0x18 = 0;
        field_0x16 = 0;
        field_0x14 = 0;
        field_0x22 = 0;
        field_0x20 = 0;
        field_0x1e = 0;
        field_0x1c = 0;
        mKRegUpdateMaterialID = NULL;
        mCRegUpdateMaterialID = NULL;
        mCAValues = NULL;
        mCBValues = NULL;
        mCGValues = NULL;
        mCRValues = NULL;
        mKAValues = NULL;
        mKBValues = NULL;
        mKGValues = NULL;
        mKRValues = NULL;
        mKind = KIND_TEV_REG;
    }
    void getTevColorReg(u16, _GXColorS10*) const;
    void getTevKonstReg(u16, _GXColor*) const;

    virtual ~J2DAnmTevRegKey() {}
    virtual void searchUpdateMaterialID(J2DScreen* pScreen);

    u16 getCRegUpdateMaterialNum() const { return mCRegUpdateMaterialNum; }
    u16 getCRegUpdateMaterialID(u16 i) const { return mCRegUpdateMaterialID[i]; }

    u16 getKRegUpdateMaterialNum() const { return mKRegUpdateMaterialNum; }
    u16 getKRegUpdateMaterialID(u16 i) const { return mKRegUpdateMaterialID[i]; }

    J3DAnmCRegKeyTable* getAnmCRegKeyTable() const { return mAnmCRegKeyTable; }
    J3DAnmKRegKeyTable* getAnmKRegKeyTable() const { return mAnmKRegKeyTable; }

    /* 0x10 */ u16 mCRegUpdateMaterialNum;
    /* 0x12 */ u16 mKRegUpdateMaterialNum;
    /* 0x14 */ u16 field_0x14;
    /* 0x16 */ u16 field_0x16;
    /* 0x18 */ u16 field_0x18;
    /* 0x1A */ u16 field_0x1a;
    /* 0x1C */ u16 field_0x1c;
    /* 0x1E */ u16 field_0x1e;
    /* 0x20 */ u16 field_0x20;
    /* 0x22 */ u16 field_0x22;
    /* 0x24 */ u16* mCRegUpdateMaterialID;
    /* 0x28 */ JUTNameTab mCRegNameTab;
    /* 0x38 */ u16* mKRegUpdateMaterialID;
    /* 0x3C */ JUTNameTab mKRegNameTab;
    /* 0x4C */ J3DAnmCRegKeyTable* mAnmCRegKeyTable;
    /* 0x50 */ J3DAnmKRegKeyTable* mAnmKRegKeyTable;
    /* 0x54 */ s16* mCRValues;
    /* 0x58 */ s16* mCGValues;
    /* 0x5C */ s16* mCBValues;
    /* 0x60 */ s16* mCAValues;
    /* 0x64 */ s16* mKRValues;
    /* 0x68 */ s16* mKGValues;
    /* 0x6C */ s16* mKBValues;
    /* 0x70 */ s16* mKAValues;
};

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DAnmColor : public J2DAnmBase {
public:
    J2DAnmColor() {
        field_0x16 = 0;
        field_0x14 = 0;
        field_0x12 = 0;
        field_0x10 = 0;
        mUpdateMaterialNum = 0;
        mUpdateMaterialID = NULL;
        mKind = KIND_COLOR;
    }
    virtual ~J2DAnmColor() {}
    virtual void searchUpdateMaterialID(J2DScreen*);
    virtual void getColor(u16, _GXColor*) const {}
    u16 getUpdateMaterialNum() const { return mUpdateMaterialNum; }
    u16 getUpdateMaterialID(u16 i) const { return mUpdateMaterialID[i]; }

    /* 0x10 */ u16 field_0x10;
    /* 0x12 */ u16 field_0x12;
    /* 0x14 */ u16 field_0x14;
    /* 0x16 */ u16 field_0x16;
    /* 0x18 */ u16 mUpdateMaterialNum;
    /* 0x1C */ u16* mUpdateMaterialID;
    /* 0x20 */ JUTNameTab field_0x20;
};  // Size: 0x30

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DAnmColorKey : public J2DAnmColor {
public:
    J2DAnmColorKey() {
        mRValues = NULL;
        mGValues = NULL;
        mBValues = NULL;
        mAValues = NULL;
        mInfoTable = NULL;
    }
    virtual ~J2DAnmColorKey() {}
    virtual void getColor(u16, _GXColor*) const;

    /* 0x30 */ s16* mRValues;
    /* 0x34 */ s16* mGValues;
    /* 0x38 */ s16* mBValues;
    /* 0x3C */ s16* mAValues;
    /* 0x40 */ J3DAnmColorKeyTable* mInfoTable;
};

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DAnmColorFullInfo {
    /* 0x00 */ u16 mRMaxFrame;
    /* 0x02 */ u16 mROffset;
    /* 0x04 */ u16 mGMaxFrame;
    /* 0x06 */ u16 mGOffset;
    /* 0x08 */ u16 mBMaxFrame;
    /* 0x0A */ u16 mBOffset;
    /* 0x0C */ u16 mAMaxFrame;
    /* 0x0E */ u16 mAOffset;
};  // Size = 0x10

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DAnmColorFull : public J2DAnmColor {
public:
    J2DAnmColorFull() {
        mRValues = NULL;
        mGValues = NULL;
        mBValues = NULL;
        mAValues = NULL;
        mInfoTable = NULL;
    }
    virtual ~J2DAnmColorFull() {}
    virtual void getColor(u16, _GXColor*) const;

    /* 0x30 */ u8* mRValues;
    /* 0x34 */ u8* mGValues;
    /* 0x38 */ u8* mBValues;
    /* 0x3C */ u8* mAValues;
    /* 0x40 */ J3DAnmColorFullTable* mInfoTable;
};

template <typename T>
inline f32 J2DHermiteInterpolation(f32, T*, T*, T*, T*, T*, T*);

template <>
inline f32 J2DHermiteInterpolation<f32>(f32 f1, f32* f2, f32* f3, f32* f4, f32* f5, f32* f6,
                                        f32* f7) {
    return JMAHermiteInterpolation(f1, *f2, *f3, *f4, *f5, *f6, *f7);
}

template <>
inline f32 J2DHermiteInterpolation<s16>(__REGISTER f32 pp1, __REGISTER s16* pp2, __REGISTER s16* pp3,
                                        __REGISTER s16* pp4, __REGISTER s16* pp5, __REGISTER s16* pp6,
                                        __REGISTER s16* pp7) {
#ifdef __MWERKS__
    __REGISTER f32 p1 = pp1;
    __REGISTER f32 ff8;
    __REGISTER f32 ff7;
    __REGISTER f32 ff6;
    __REGISTER f32 ff5;
    __REGISTER f32 ff4;
    __REGISTER f32 ff3;
    __REGISTER f32 ff2;
    __REGISTER f32 ff0;
    __REGISTER f32 fout;
    __REGISTER s16* p2 = pp2;
    __REGISTER s16* p3 = pp3;
    __REGISTER s16* p4 = pp4;
    __REGISTER s16* p5 = pp5;
    __REGISTER s16* p6 = pp6;
    __REGISTER s16* p7 = pp7;
    // clang-format off
    asm {
        psq_l ff2, 0(p2), 0x1, 5
        psq_l ff0, 0(p5), 0x1, 5
        psq_l ff7, 0(p3), 0x1, 5
        fsubs ff5, ff0, ff2
        psq_l ff6, 0(p6), 0x1, 5
        fsubs ff3, p1, ff2
        psq_l ff0, 0(p7), 0x1, 5
        fsubs ff4, ff6, ff7
        fdivs ff3, ff3, ff5
        psq_l fout, 0(p4), 0x1, 5
        fmadds ff0, ff0, ff5, ff7
        fmuls ff2, ff3, ff3
        fnmsubs ff4, ff5, fout, ff4
        fsubs ff0, ff0, ff6
        fsubs ff0, ff0, ff4
        fmuls ff0, ff2, ff0
        fmadds fout, ff5, fout, ff0
        fmadds fout, fout, ff3, ff7
        fmadds fout, ff4, ff2, fout
        fsubs fout, fout, ff0
    }
    // clang-format on
    return fout;
#endif
}

#endif /* J2DANIMATION_H */
