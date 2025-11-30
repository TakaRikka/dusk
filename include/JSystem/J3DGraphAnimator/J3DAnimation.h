#ifndef J3DANIMATION_H
#define J3DANIMATION_H

#include "JSystem/J3DGraphAnimator/J3DModelData.h"
#include "JSystem/JUtility/JUTAssert.h"
#include "JSystem/JUtility/JUTNameTab.h"
#include "global.h"

struct JUTDataBlockHeader {
    /* 0x0 */ u32 mType;
    /* 0x4 */ u32 mSize;

    const JUTDataBlockHeader* getNext() const {  // fake inline
        return reinterpret_cast<const JUTDataBlockHeader*>(reinterpret_cast<const u8*>(this) +
                                                           mSize);
    }
};

struct JUTDataFileHeader {  // actual struct name unknown
    /* 0x00 */ u32 mMagic;
    /* 0x04 */ u32 mType;
    /* 0x08 */ u32 mFileSize;
    /* 0x0C */ u32 mBlockNum;
    /* 0x10 */ u8 _10[0x1C - 0x10];
    /* 0x1C */ u32 mSeAnmOffset;  // Only exists for some BCKs
    /* 0x20 */ JUTDataBlockHeader mFirstBlock;
};

// unknown name. refers to ANK1 chunk of BCK files
struct J3DAnmTransform_ANK1 {
    /* 0x00 */ u32 magic;
    /* 0x04 */ u32 size;
    /* 0x08 */ u8 attribute;
    /* 0x09 */ u8 rotation_frac;
    /* 0x0A */ s16 duration;
    /* 0x0C */ s16 keyframe_num;
    /* 0x0E */ s16 scale_entries;
    /* 0x10 */ s16 rotation_entries;
    /* 0x12 */ s16 translation_entries;
    /* 0x14 */ u32 anm_data_offset;
    /* 0x18 */ u32 scale_data_offset;
    /* 0x1C */ u32 rotation_data_offset;
    /* 0x20 */ u32 translation_data_offset;
};

typedef struct _GXColor GXColor;
typedef struct _GXColorS10 GXColorS10;

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmKeyTableBase {
    /* 0x00 */ u16 mMaxFrame;
    /* 0x02 */ u16 mOffset;
    /* 0x04 */ u16 mType;
};  // Size = 0x6

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmColorKeyTable {
    J3DAnmKeyTableBase mRInfo;
    J3DAnmKeyTableBase mGInfo;
    J3DAnmKeyTableBase mBInfo;
    J3DAnmKeyTableBase mAInfo;
};  // Size = 0x18

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmVtxColorIndexData {
    /* 0x00 */ u16 mNum;
    /* 0x04 */ void* mpData;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmColorFullTable {
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
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmVisibilityFullTable {
    u16 _0;
    u16 _2;
};  // Size = 0x4

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmTransformKeyTable {
    J3DAnmKeyTableBase mScaleInfo;
    J3DAnmKeyTableBase mRotationInfo;
    J3DAnmKeyTableBase mTranslateInfo;
};  // Size = 0x12

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmTransformFullTable {
    /* 0x00 */ u16 mScaleMaxFrame;
    /* 0x02 */ u16 mScaleOffset;
    /* 0x04 */ u16 mRotationMaxFrame;
    /* 0x06 */ u16 mRotationOffset;
    /* 0x08 */ u16 mTranslateMaxFrame;
    /* 0x0A */ u16 mTranslateOffset;
};  // Size = 0xC

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmTexPatternFullTable {
    /* 0x00 */ u16 mMaxFrame;
    /* 0x02 */ u16 mOffset;
    /* 0x04 */ u8 mTexNo;
    /* 0x06 */ u16 _6;
};  // Size = 0x8

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmCRegKeyTable {
    /* 0x00 */ J3DAnmKeyTableBase mRTable;
    /* 0x06 */ J3DAnmKeyTableBase mGTable;
    /* 0x0C */ J3DAnmKeyTableBase mBTable;
    /* 0x12 */ J3DAnmKeyTableBase mATable;
    /* 0x18 */ u8 mColorId;
    u8 padding[3];
};  // Size = 0x1C

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmKRegKeyTable {
    /* 0x00 */ J3DAnmKeyTableBase mRTable;
    /* 0x06 */ J3DAnmKeyTableBase mGTable;
    /* 0x0C */ J3DAnmKeyTableBase mBTable;
    /* 0x12 */ J3DAnmKeyTableBase mATable;
    /* 0x18 */ u8 mColorId;
    u8 padding[3];
};  // Size = 0x1C

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmDataBlockHeader {  // actual name unknown
    /* 0x0 */ u32 mType;
    /* 0x4 */ u32 mNextOffset;
};  // Size = 0x8

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmDataHeader {  // actual name unknown
    /* 0x00 */ u32 mMagic;
    /* 0x04 */ u32 mType;
    /* 0x08 */ u8 _8[4];
    /* 0x0C */ u32 mCount;
    /* 0x10 */ u8 _10[0x20 - 0x10];
    /* 0x20 */ J3DAnmDataBlockHeader mFirst;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmVtxColorFullData {
    /* 0x00 */ J3DAnmDataBlockHeader mHeader;
    /* 0x08 */ u8 field_0x8;
    /* 0x09 */ u8 field_0x9;  // padding?
    /* 0x0A */ s16 mFrameMax;
    /* 0x0C */ u16 mAnmTableNum[2];
    /* 0x10 */ u8 field_0x10[0x18 - 0x10];
    /* 0x18 */ void* mTableOffsets[2];
    /* 0x20 */ void* mVtxColorIndexDataOffsets[2];
    /* 0x28 */ void* mVtxColorIndexPointerOffsets[2];
    /* 0x30 */ void* mRValuesOffset;
    /* 0x34 */ void* mGValuesOffset;
    /* 0x38 */ void* mBValuesOffset;
    /* 0x3C */ void* mAValuesOffset;
};  // Size = 0x40

STATIC_ASSERT(sizeof(J3DAnmVtxColorFullData) == 0x40);

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmVisibilityFullData {
    /* 0x00 */ J3DAnmDataBlockHeader mHeader;
    /* 0x08 */ u8 field_0x8;
    /* 0x09 */ u8 field_0x9;  // padding?
    /* 0x0A */ s16 mFrameMax;
    /* 0x0C */ u16 field_0xc;
    /* 0x0E */ u16 field_0xe;
    /* 0x10 */ void* mTableOffset;
    /* 0x14 */ void* mValuesOffset;
};  // Size = 0x18

STATIC_ASSERT(sizeof(J3DAnmVisibilityFullData) == 0x18);

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmTransformFullData {
    /* 0x00 */ J3DAnmDataBlockHeader mHeader;
    /* 0x08 */ u8 field_0x8;
    /* 0x09 */ u8 field_0x9;
    /* 0x0A */ s16 mFrameMax;
    /* 0x0C */ u16 field_0xc;
    /* 0x0E */ u8 field_0xe[0x14 - 0xe];
    /* 0x14 */ void* mTableOffset;
    /* 0x18 */ void* mScaleValOffset;
    /* 0x1C */ void* mRotValOffset;
    /* 0x20 */ void* mTransValOffset;
};  // Size = 0x24

STATIC_ASSERT(sizeof(J3DAnmTransformFullData) == 0x24);

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmColorKeyData {
    /* 0x00 */ J3DAnmDataBlockHeader mHeader;
    /* 0x08 */ u8 field_0x8;
    /* 0x09 */ u8 field_0x9[3];
    /* 0x0C */ s16 mFrameMax;
    /* 0x0E */ u16 mUpdateMaterialNum;
    /* 0x10 */ u16 field_0x10;
    /* 0x12 */ u16 field_0x12;
    /* 0x14 */ u16 field_0x14;
    /* 0x16 */ u16 field_0x16;
    /* 0x18 */ void* mTableOffset;
    /* 0x1C */ void* mUpdateMaterialIDOffset;
    /* 0x20 */ void* mNameTabOffset;
    /* 0x24 */ void* mRValOffset;
    /* 0x28 */ void* mGValOffset;
    /* 0x2C */ void* mBValOffset;
    /* 0x30 */ void* mAValOffset;
};  // Size = 0x34

STATIC_ASSERT(sizeof(J3DAnmColorKeyData) == 0x34);

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmTextureSRTKeyData {
    /* 0x00 */ J3DAnmDataBlockHeader mHeader;
    /* 0x08 */ u8 field_0x8;
    /* 0x09 */ u8 field_0x9;
    /* 0x0A */ s16 field_0xa;
    /* 0x0C */ u16 field_0xc;
    /* 0x0E */ u16 field_0xe;
    /* 0x10 */ u16 field_0x10;
    /* 0x12 */ u16 field_0x12;
    /* 0x14 */ void* mTableOffset;
    /* 0x18 */ void* mUpdateMatIDOffset;
    /* 0x1C */ void* mNameTab1Offset;
    /* 0x20 */ void* mUpdateTexMtxIDOffset;
    /* 0x24 */ void* unkOffset;
    /* 0x28 */ void* mScaleValOffset;
    /* 0x2C */ void* mRotValOffset;
    /* 0x30 */ void* mTransValOffset;
    /* 0x34 */ u16 field_0x34;
    /* 0x36 */ u16 field_0x36;
    /* 0x38 */ u16 field_0x38;
    /* 0x3A */ u16 field_0x3a;
    /* 0x3C */ void* mInfoTable2Offset;
    /* 0x40 */ void* field_0x40;
    /* 0x44 */ void* mNameTab2Offset;
    /* 0x48 */ void* field_0x48;
    /* 0x4C */ void* field_0x4c;
    /* 0x50 */ void* field_0x50;
    /* 0x54 */ void* field_0x54;
    /* 0x58 */ void* field_0x58;
    /* 0x5C */ s32 field_0x5c;
};  // Size = 0x60

STATIC_ASSERT(sizeof(J3DAnmTextureSRTKeyData) == 0x60);

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmVtxColorKeyData {
    /* 0x00 */ J3DAnmDataBlockHeader mHeader;
    /* 0x08 */ u8 field_0x8;
    /* 0x09 */ u8 field_0x9;
    /* 0x0A */ s16 mFrameMax;
    /* 0x0C */ u16 mAnmTableNum[2];
    /* 0x10 */ u8 field_0x10[0x18 - 0x10];
    /* 0x18 */ void* mTableOffsets[2];
    /* 0x20 */ void* mVtxColoIndexDataOffset[2];
    /* 0x28 */ void* mVtxColoIndexPointerOffset[2];
    /* 0x30 */ void* mRValOffset;
    /* 0x34 */ void* mGValOffset;
    /* 0x38 */ void* mBValOffset;
    /* 0x3C */ void* mAValOffset;
};  // Size = 0x40

STATIC_ASSERT(sizeof(J3DAnmVtxColorKeyData) == 0x40);

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmTexPatternFullData {
    /* 0x00 */ J3DAnmDataBlockHeader mHeader;
    /* 0x08 */ u8 field_0x8;
    /* 0x09 */ u8 field_0x9;
    /* 0x0A */ s16 mFrameMax;
    /* 0x0C */ u16 field_0xc;
    /* 0x0E */ u16 field_0xe;
    /* 0x10 */ void* mTableOffset;
    /* 0x14 */ void* mValuesOffset;
    /* 0x18 */ void* mUpdateMaterialIDOffset;
    /* 0x1C */ void* mNameTabOffset;
};  // Size = 0x20

STATIC_ASSERT(sizeof(J3DAnmTexPatternFullData) == 0x20);

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmTevRegKeyData {
    /* 0x00 */ J3DAnmDataBlockHeader mHeader;
    /* 0x08 */ u8 field_0x8;
    /* 0x09 */ u8 field_0x9;  // maybe padding
    /* 0x0A */ s16 mFrameMax;
    /* 0x0C */ u16 mCRegUpdateMaterialNum;
    /* 0x0E */ u16 mKRegUpdateMaterialNum;
    /* 0x10 */ u16 field_0x10;
    /* 0x12 */ u16 field_0x12;
    /* 0x14 */ u16 field_0x14;
    /* 0x16 */ u16 field_0x16;
    /* 0x18 */ u16 field_0x18;
    /* 0x1A */ u16 field_0x1a;
    /* 0x1C */ u16 field_0x1c;
    /* 0x1E */ u16 field_0x1e;
    /* 0x20 */ void* mCRegTableOffset;
    /* 0x24 */ void* mKRegTableOffset;
    /* 0x28 */ void* mCRegUpdateMaterialIDOffset;
    /* 0x2C */ void* mKRegUpdateMaterialIDOffset;
    /* 0x30 */ void* mCRegNameTabOffset;
    /* 0x34 */ void* mKRegNameTabOffset;
    /* 0x38 */ void* mCRValuesOffset;
    /* 0x3C */ void* mCGValuesOffset;
    /* 0x40 */ void* mCBValuesOffset;
    /* 0x44 */ void* mCAValuesOffset;
    /* 0x48 */ void* mKRValuesOffset;
    /* 0x4C */ void* mKGValuesOffset;
    /* 0x50 */ void* mKBValuesOffset;
    /* 0x54 */ void* mKAValuesOffset;
};  // Size = 0x58

STATIC_ASSERT(sizeof(J3DAnmTevRegKeyData) == 0x58);

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmColorFullData { /* PlaceHolder Structure */
    /* 0x00 */ J3DAnmDataBlockHeader mHeader;
    /* 0x08 */ u8 field_0x8;
    /* 0x09 */ u8 field_0x9[3];
    /* 0x0C */ s16 mFrameMax;
    /* 0x0E */ u16 mUpdateMaterialNum;
    /* 0x10 */ u8 field_0x10[0x18 - 0x10];
    /* 0x18 */ void* mTableOffset;
    /* 0x1C */ void* mUpdateMaterialIDOffset;
    /* 0x20 */ void* mNameTabOffset;
    /* 0x24 */ void* mRValuesOffset;
    /* 0x28 */ void* mGValuesOffset;
    /* 0x2C */ void* mBValuesOffset;
    /* 0x30 */ void* mAValuesOffset;
};  // Size = 0x34

STATIC_ASSERT(sizeof(J3DAnmColorFullData) == 0x34);

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmClusterKeyTable {
    /* 0x00 */ J3DAnmKeyTableBase mWeightTable;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmTransformKeyData {
    /* 0x00 */ JUTDataBlockHeader mHeader;
    /* 0x08 */ u8 field_0x8;
    /* 0x09 */ u8 field_0x9;
    /* 0x0A */ s16 mFrameMax;
    /* 0x0C */ u16 field_0xc;
    /* 0x10 */ int field_0x10;
    /* 0x14 */ void* mTableOffset;
    /* 0x18 */ void* field_0x18;
    /* 0x1c */ void* field_0x1c;
    /* 0x20 */ void* field_0x20;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmClusterKeyData {
    /* 0x00 */ JUTDataBlockHeader mHeader;
    /* 0x08 */ u8 field_0x8;
    /* 0x0A */ s16 mFrameMax;
    /* 0x0C */ s32 field_0xc;
    /* 0x10 */ void* mTableOffset;
    /* 0x14 */ void* mWeightOffset;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmClusterFullData {
    /* 0x00 */ JUTDataBlockHeader mHeader;
    /* 0x08 */ u8 field_0x8;
    /* 0x0A */ s16 mFrameMax;
    /* 0x0C */ s32 field_0xc;
    /* 0x10 */ void* mTableOffset;
    /* 0x14 */ void* mWeightOffset;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmClusterFullTable {
    u16 mMaxFrame;
    u16 mOffset;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DAnmBase {
public:
    J3DAnmBase() {
        mAttribute = 0;
        field_0x5 = 0;
        mFrameMax = 0;
        mFrame = 0.0f;
    }

    J3DAnmBase(s16 frameMax) {
        mAttribute = 0;
        field_0x5 = 0;
        mFrameMax = frameMax;
        mFrame = 0.0f;
    }

    virtual ~J3DAnmBase() {}
    virtual s32 getKind() const = 0;

    u8 getAttribute() const { return mAttribute; }
    s16 getFrameMax() const { return mFrameMax; }
    f32 getFrame() const { return mFrame; }
    void setFrame(f32 frame) { mFrame = frame; }

    /* 0x4 */ u8 mAttribute;
    /* 0x5 */ u8 field_0x5;
    /* 0x6 */ s16 mFrameMax;
    /* 0x8 */ f32 mFrame;
};  // Size: 0xC

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DAnmTransform : public J3DAnmBase {
public:
    J3DAnmTransform(s16, f32*, s16*, f32*);

    virtual ~J3DAnmTransform() {}
    virtual s32 getKind() const { return 0; }
    virtual void getTransform(u16, J3DTransformInfo*) const = 0;

    /* 0x0C */ f32* mScaleData;
    /* 0x10 */ s16* mRotData;
    /* 0x14 */ f32* mTransData;
    /* 0x18 */ s16 field_0x18;
    /* 0x1A */ s16 field_0x1a;
    /* 0x1C */ u16 field_0x1c;
    /* 0x1E */ u16 field_0x1e;
};  // Size: 0x20

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DAnmTransformKey : public J3DAnmTransform {
public:
    J3DAnmTransformKey() : J3DAnmTransform(0, NULL, NULL, NULL) {
        mDecShift = 0;
        mAnmTable = 0;
    }

    void calcTransform(f32, u16, J3DTransformInfo*) const;

    virtual ~J3DAnmTransformKey() {}
    virtual s32 getKind() const { return 8; }
    virtual void getTransform(u16 jointNo, J3DTransformInfo* pTransform) const {
        calcTransform(mFrame, jointNo, pTransform);
    }

    /* 0x20 */ int mDecShift;
    /* 0x24 */ J3DAnmTransformKeyTable* mAnmTable;
};  // Size: 0x28

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DAnmTransformFull : public J3DAnmTransform {
public:
    J3DAnmTransformFull() : J3DAnmTransform(0, NULL, NULL, NULL) { mAnmTable = NULL; }

    virtual ~J3DAnmTransformFull() {}
    virtual s32 getKind() const { return 9; }
    virtual void getTransform(u16, J3DTransformInfo*) const;

    /* 0x20 */ J3DAnmTransformFullTable* mAnmTable;
};  // Size: 0x24

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DAnmTransformFullWithLerp : public J3DAnmTransformFull {
public:
    virtual ~J3DAnmTransformFullWithLerp() {}
    virtual s32 getKind() const { return 16; }
    virtual void getTransform(u16, J3DTransformInfo*) const;
};  // Size: 0x24

struct J3DTextureSRTInfo;
class J3DModelData;
class J3DMaterialTable;

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DAnmTextureSRTKey : public J3DAnmBase {
public:
    J3DAnmTextureSRTKey();
    void calcTransform(f32, u16, J3DTextureSRTInfo*) const;
    void searchUpdateMaterialID(J3DMaterialTable*);
    void searchUpdateMaterialID(J3DModelData*);

    virtual ~J3DAnmTextureSRTKey() {}
    virtual s32 getKind() const { return 4; }

    void getTransform(u16 jointNo, J3DTextureSRTInfo* pSRTInfo) const {
        calcTransform(getFrame(), jointNo, pSRTInfo);
    }

    u16 getUpdateMaterialID(u16 idx) const {
        J3D_ASSERT_RANGE(1029, idx < mTrackNum / 3);
        return mUpdateMaterialID[idx];
    }
    u16 getUpdateMaterialNum() const { return mTrackNum / 3; }
    u16 getPostUpdateMaterialNum() const { return field_0x4a / 3; }

    int getUpdateTexMtxID(u16 idx) const {
        J3D_ASSERT_RANGE(1017, idx < mTrackNum / 3);
        return mUpdateTexMtxID[idx];
    }
    bool isValidUpdateMaterialID(u16 idx) const { return mUpdateMaterialID[idx] != 0xffff; }
    u32 getTexMtxCalcType() { return mTexMtxCalcType; }
    Vec* getSRTCenter(u16 idx) {
        J3D_ASSERT_RANGE(1047, idx < mTrackNum / 3);
        return &mSRTCenter[idx];
    }

    /* 0x0C */ int mDecShift;
    /* 0x10 */ J3DAnmTransformKeyTable* mAnmTable;
    /* 0x14 */ u16 mTrackNum;
    /* 0x16 */ u16 mScaleNum;
    /* 0x18 */ u16 mRotNum;
    /* 0x1A */ u16 mTransNum;
    /* 0x1C */ f32* mScaleData;
    /* 0x20 */ s16* mRotData;
    /* 0x24 */ f32* mTransData;
    /* 0x28 */ u8* mUpdateTexMtxID;
    /* 0x2C */ u16* mUpdateMaterialID;
    /* 0x30 */ JUTNameTab mUpdateMaterialName;
    /* 0x40 */ Vec* mSRTCenter;
    /* 0x44 */ u16 field_0x44;
    /* 0x46 */ u16 field_0x46;
    /* 0x48 */ u16 field_0x48;
    /* 0x4A */ u16 field_0x4a;
    /* 0x4C */ void* field_0x4c;
    /* 0x50 */ void* field_0x50;
    /* 0x54 */ void* field_0x54;
    /* 0x58 */ void* field_0x58;
    /* 0x5C */ u8* mPostUpdateTexMtxID;
    /* 0x60 */ u16* mPostUpdateMaterialID;
    /* 0x64 */ JUTNameTab mPostUpdateMaterialName;
    /* 0x74 */ Vec* mPostSRTCenter;
    /* 0x78 */ u32 mTexMtxCalcType;
};  // Size: 0x7C

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DAnmTexPattern : public J3DAnmBase {
public:
    J3DAnmTexPattern();
    void getTexNo(u16, u16*) const;
    void searchUpdateMaterialID(J3DMaterialTable*);
    void searchUpdateMaterialID(J3DModelData*);

    virtual ~J3DAnmTexPattern() {}
    virtual s32 getKind() const { return 2; }

    u16 getUpdateMaterialID(u16 idx) const {
        J3D_ASSERT_RANGE(2288, idx < mUpdateMaterialNum);
        return mUpdateMaterialID[idx];
    }
    u16 getUpdateMaterialNum() const { return mUpdateMaterialNum; }
    bool isValidUpdateMaterialID(u16 id) const { return mUpdateMaterialID[id] != 0xFFFF; }
    J3DAnmTexPatternFullTable* getAnmTable() { return mAnmTable; }

    /* 0x0C */ u16* mTextureIndex;
    /* 0x10 */ J3DAnmTexPatternFullTable* mAnmTable;
    /* 0x14 */ u16 field_0x14;
    /* 0x16 */ u16 mUpdateMaterialNum;
    /* 0x18 */ u16* mUpdateMaterialID;
    /* 0x1C */ JUTNameTab mUpdateMaterialName;
};  // Size: 0x2C

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DAnmTevRegKey : public J3DAnmBase {
public:
    J3DAnmTevRegKey();
    void getTevColorReg(u16, _GXColorS10*) const;
    void getTevKonstReg(u16, _GXColor*) const;
    void searchUpdateMaterialID(J3DMaterialTable*);
    void searchUpdateMaterialID(J3DModelData*);

    virtual ~J3DAnmTevRegKey() {}
    virtual s32 getKind() const { return 5; }

    u16 getCRegUpdateMaterialNum() const { return mCRegUpdateMaterialNum; }
    u16 getKRegUpdateMaterialNum() const { return mKRegUpdateMaterialNum; }

    u16 getCRegUpdateMaterialID(u16 idx) const {
        J3D_ASSERT_RANGE(2100, idx < mCRegUpdateMaterialNum);
        return mCRegUpdateMaterialID[idx];
    }
    u16 getKRegUpdateMaterialID(u16 idx) const {
        J3D_ASSERT_RANGE(2140, idx < mKRegUpdateMaterialNum);
        return mKRegUpdateMaterialID[idx];
    }

    const J3DAnmCRegKeyTable* getAnmCRegKeyTable() { return mAnmCRegKeyTable; }
    const J3DAnmKRegKeyTable* getAnmKRegKeyTable() { return mAnmKRegKeyTable; }

    bool isValidCRegUpdateMaterialID(u16 idx) const { return mCRegUpdateMaterialID[idx] != 0xffff; }
    bool isValidKRegUpdateMaterialID(u16 idx) const { return mKRegUpdateMaterialID[idx] != 0xffff; }

    /* 0x0C */ u16 mCRegUpdateMaterialNum;
    /* 0x0E */ u16 mKRegUpdateMaterialNum;
    /* 0x10 */ u16 mCRegDataCountR;
    /* 0x12 */ u16 mCRegDataCountG;
    /* 0x14 */ u16 mCRegDataCountB;
    /* 0x16 */ u16 mCRegDataCountA;
    /* 0x18 */ u16 mKRegDataCountR;
    /* 0x1A */ u16 mKRegDataCountG;
    /* 0x1C */ u16 mKRegDataCountB;
    /* 0x1E */ u16 mKRegDataCountA;
    /* 0x20 */ u16* mCRegUpdateMaterialID;
    /* 0x24 */ JUTNameTab mCRegUpdateMaterialName;
    /* 0x34 */ u16* mKRegUpdateMaterialID;
    /* 0x38 */ JUTNameTab mKRegUpdateMaterialName;
    /* 0x48 */ J3DAnmCRegKeyTable* mAnmCRegKeyTable;
    /* 0x4C */ J3DAnmKRegKeyTable* mAnmKRegKeyTable;
    /* 0x50 */ s16* mAnmCRegDataR;
    /* 0x54 */ s16* mAnmCRegDataG;
    /* 0x58 */ s16* mAnmCRegDataB;
    /* 0x5C */ s16* mAnmCRegDataA;
    /* 0x60 */ s16* mAnmKRegDataR;
    /* 0x64 */ s16* mAnmKRegDataG;
    /* 0x68 */ s16* mAnmKRegDataB;
    /* 0x6C */ s16* mAnmKRegDataA;
};  // Size: 0x70

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DAnmColor : public J3DAnmBase {
public:
    J3DAnmColor();
    void searchUpdateMaterialID(J3DMaterialTable*);

    virtual ~J3DAnmColor() {}
    virtual s32 getKind() const { return 1; }
    virtual void getColor(u16, GXColor*) const {}

    u16 getUpdateMaterialNum() const { return mUpdateMaterialNum; }
    bool isValidUpdateMaterialID(u16 id) const { return mUpdateMaterialID[id] != 0xFFFF; }
    u16 getUpdateMaterialID(u16 idx) const { 
        J3D_ASSERT_RANGE(1578, idx < mUpdateMaterialNum);
        return mUpdateMaterialID[idx];
    }

    /* 0x0C */ u16 field_0xc;
    /* 0x0E */ u16 field_0xe;
    /* 0x10 */ u16 field_0x10;
    /* 0x12 */ u16 field_0x12;
    /* 0x14 */ u16 mUpdateMaterialNum;
    /* 0x18 */ u16* mUpdateMaterialID;
    /* 0x1C */ JUTNameTab mUpdateMaterialName;
};  // Size: 0x2C

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DAnmColorKey : public J3DAnmColor {
public:
    J3DAnmColorKey();

    virtual ~J3DAnmColorKey() {}
    virtual s32 getKind() const { return 11; }
    virtual void getColor(u16, GXColor*) const;

    /* 0x2C */ s16* mColorR;
    /* 0x30 */ s16* mColorG;
    /* 0x34 */ s16* mColorB;
    /* 0x38 */ s16* mColorA;
    /* 0x3C */ J3DAnmColorKeyTable* mAnmTable;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DAnmColorFull : public J3DAnmColor {
public:
    J3DAnmColorFull();

    virtual ~J3DAnmColorFull() {}
    virtual s32 getKind() const { return 10; }
    virtual void getColor(u16, GXColor*) const;

    /* 0x2C */ u8* mColorR;
    /* 0x30 */ u8* mColorG;
    /* 0x34 */ u8* mColorB;
    /* 0x38 */ u8* mColorA;
    /* 0x3C */ J3DAnmColorFullTable* mAnmTable;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DAnmVtxColor : public J3DAnmBase {
public:
    J3DAnmVtxColor();

    virtual ~J3DAnmVtxColor() {}
    virtual s32 getKind() const { return 7; }
    virtual void getColor(u8, u16, GXColor*) const {}

    /* 0x0C */ u16 mAnmTableNum[2];
    /* 0x10 */ J3DAnmVtxColorIndexData* mAnmVtxColorIndexData[2];
};  // Size: 0x18

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DAnmVtxColorKey : public J3DAnmVtxColor {
public:
    J3DAnmVtxColorKey();

    virtual ~J3DAnmVtxColorKey() {}
    virtual s32 getKind() const { return 15; }
    virtual void getColor(u8, u16, GXColor*) const;

    /* 0x18 */ J3DAnmColorKeyTable* mpTable[2];
    /* 0x20 */ s16* mColorR;
    /* 0x24 */ s16* mColorG;
    /* 0x28 */ s16* mColorB;
    /* 0x2C */ s16* mColorA;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DAnmVtxColorFull : public J3DAnmVtxColor {
public:
    J3DAnmVtxColorFull();

    virtual ~J3DAnmVtxColorFull() {}
    virtual s32 getKind() const { return 14; }
    virtual void getColor(u8, u16, GXColor*) const;

    /* 0x18 */ J3DAnmColorFullTable* mpTable[2];
    /* 0x20 */ u8* mColorR;
    /* 0x24 */ u8* mColorG;
    /* 0x28 */ u8* mColorB;
    /* 0x2C */ u8* mColorA;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DAnmCluster : public J3DAnmBase {
public:
    J3DAnmCluster(s16 frameMax, f32* pWeight) : J3DAnmBase(frameMax) { mWeight = pWeight; }

    virtual ~J3DAnmCluster() {}
    virtual s32 getKind() const { return 3; }
    virtual f32 getWeight(u16) const { return 1.0f; }

    /* 0x0C */ f32* mWeight;
};  // Size: 0x10

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DAnmClusterFull : public J3DAnmCluster {
public:
    J3DAnmClusterFull() : J3DAnmCluster(0, NULL) { mAnmTable = NULL; }

    virtual ~J3DAnmClusterFull() {}
    virtual s32 getKind() const { return 12; }
    virtual f32 getWeight(u16) const;

    /* 0x10 */ J3DAnmClusterFullTable* mAnmTable;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DAnmClusterKey : public J3DAnmCluster {
public:
    J3DAnmClusterKey() : J3DAnmCluster(0, NULL) { mAnmTable = NULL; }

    virtual ~J3DAnmClusterKey() {}
    virtual s32 getKind() const { return 13; }
    virtual f32 getWeight(u16) const;

    /* 0x10 */ J3DAnmClusterKeyTable* mAnmTable;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DAnmVisibilityFull : public J3DAnmBase {
public:
    J3DAnmVisibilityFull() : J3DAnmBase(0) {
        mUpdateMaterialNum = 0;
        field_0xe = 0;
        mAnmTable = NULL;
        mVisibility = NULL;
    }

    virtual ~J3DAnmVisibilityFull() {}
    virtual s32 getKind() const { return 6; }

    /* 0x0C */ u16 mUpdateMaterialNum;
    /* 0x0E */ u16 field_0xe;
    /* 0x10 */ J3DAnmVisibilityFullTable* mAnmTable;
    /* 0x14 */ u8* mVisibility;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DFrameCtrl {
public:
    enum Attribute_e {
        /*  -1 */ EMode_NULL = -1,
        /* 0x0 */ EMode_NONE,
        /* 0x1 */ EMode_RESET,
        /* 0x2 */ EMode_LOOP,
        /* 0x3 */ EMode_REVERSE,
        /* 0x4 */ EMode_LOOP_REVERSE,
    };

    J3DFrameCtrl() { this->init(0); }
    void init(s16);
    void init(int endFrame) { init((s16)endFrame); }
    BOOL checkPass(f32);
    void update();
    virtual ~J3DFrameCtrl() {}

    u8 getAttribute() const { return mAttribute; }
    void setAttribute(u8 attr) { mAttribute = attr; }
    u8 getState() const { return mState; }
    bool checkState(u8 state) const { return mState & state ? true : false; }
    s16 getStart() const { return mStart; }
    void setStart(s16 start) {
        mStart = start;
        mFrame = start;
    }
    s16 getEnd() const { return mEnd; }
    void setEnd(s16 end) { mEnd = end; }
    s16 getLoop() const { return mLoop; }
    void setLoop(s16 loop) { mLoop = loop; }
    f32 getRate() const { return mRate; }
    void setRate(f32 rate) { mRate = rate; }
    f32 getFrame() const { return mFrame; }
    void setFrame(f32 frame) { mFrame = frame; }
    void reset() {
        mFrame = mStart;
        mRate = 1.0f;
        mState = 0;
    }

    /* 0x04 */ u8 mAttribute;
    /* 0x05 */ u8 mState;
    /* 0x06 */ s16 mStart;
    /* 0x08 */ s16 mEnd;
    /* 0x0A */ s16 mLoop;
    /* 0x0C */ f32 mRate;
    /* 0x10 */ f32 mFrame;
};  // Size: 0x14

struct J3DMtxCalcAnmBase: public J3DMtxCalc {
    J3DMtxCalcAnmBase(J3DAnmTransform* pAnmTransform) { mAnmTransform = pAnmTransform; }
    ~J3DMtxCalcAnmBase() {}
    J3DAnmTransform* getAnmTransform() { return mAnmTransform; }
    void setAnmTransform(J3DAnmTransform* pAnmTransform) { mAnmTransform = pAnmTransform; }

    J3DAnmTransform* mAnmTransform;
};

struct J3DMtxCalcAnimationAdaptorBase {
    J3DMtxCalcAnimationAdaptorBase() {}
    void change(J3DAnmTransform*) {}
};

template <typename A0>
struct J3DMtxCalcAnimationAdaptorDefault : public J3DMtxCalcAnimationAdaptorBase {
    J3DMtxCalcAnimationAdaptorDefault(J3DAnmTransform* pAnmTransform) {}

    void calc(J3DMtxCalcAnmBase* pMtxCalc) {
        J3DTransformInfo transform;
        J3DTransformInfo* transform_p;
        if (pMtxCalc->getAnmTransform() != NULL) {
            pMtxCalc->getAnmTransform()->getTransform(J3DMtxCalc::getJoint()->getJntNo(), &transform);
            transform_p = &transform;
        } else {
            transform_p = &J3DMtxCalc::getJoint()->getTransformInfo();
        }

        A0::calcTransform(*transform_p);
    }
};

template <typename A0, typename B0>
struct J3DMtxCalcAnimation : public J3DMtxCalcAnmBase {
    J3DMtxCalcAnimation(J3DAnmTransform* pAnmTransform) : J3DMtxCalcAnmBase(pAnmTransform), field_0x8(pAnmTransform) {}
    ~J3DMtxCalcAnimation() {}

    void setAnmTransform(J3DAnmTransform* pAnmTransform) {
        mAnmTransform = pAnmTransform;
        field_0x8.change(pAnmTransform);
    }

    void init(const Vec& param_0, const Mtx& param_1) { B0::init(param_0, param_1); }
    void calc() { field_0x8.calc(this); }

    A0 field_0x8;
};

#endif /* J3DANIMATION_H */
