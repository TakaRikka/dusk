#ifndef J3DCLUSTER_H
#define J3DCLUSTER_H

#include "JSystem/J3DAssert.h"
#include "JSystem/J3DGraphLoader/J3DClusterLoader.h"
#include "dusk/endian.h"

class J3DDeformer;
class J3DClusterKey;
class J3DClusterVertex;
class J3DVertexBuffer;
class J3DModel;
class J3DAnmCluster;
class JUTNameTab;

#if TARGET_PC
#define OFFSET_PTR BE(u32)
#else
#define OFFSET_PTR void*
#endif

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DCluster {
public:
    void operator=(const J3DCluster& other) {
        mMaxAngle = other.mMaxAngle;
        mMinAngle = other.mMinAngle;
        mClusterKey = other.mClusterKey;
        mFlags = other.mFlags;
        mKeyNum = other.mKeyNum;
        mPosNum = other.mPosNum;
        field_0x14 = other.field_0x14;
        field_0x16 = other.field_0x16;
        field_0x18 = other.field_0x18;
        mClusterVertex = other.mClusterVertex;
        mDeformer = other.mDeformer;
    }

#if !TARGET_PC
    J3DDeformer* getDeformer() { return mDeformer; }
    void setDeformer(J3DDeformer* deformer) {
        J3D_ASSERT_NULLPTR(111, deformer);
        mDeformer = deformer;
    }
#endif

    /* 0x00 */ BE(f32) mMaxAngle;
    /* 0x04 */ BE(f32) mMinAngle;
    /* 0x08 */ OFFSET_PTR mClusterKey;
    /* 0x0C */ u8 mFlags;
    /* 0x0E */ u8 field_0xe[0x10 - 0xD];
    /* 0x10 */ BE(u16) mKeyNum;
    /* 0x12 */ BE(u16) mPosNum;
    /* 0x14 */ BE(u16) field_0x14;
    /* 0x16 */ BE(u16) field_0x16;
#if TARGET_PC
    OFFSET_PTR field_0x18;
    OFFSET_PTR mClusterVertex;
    OFFSET_PTR mDeformer;
#else
    /* 0x18 */ u16* field_0x18;
    /* 0x1C */ J3DClusterVertex* mClusterVertex;
    /* 0x20 */ J3DDeformer* mDeformer;
#endif
};

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DClusterKey {
public:
    void operator=(const J3DClusterKey& other) {
        mPosNum = other.mPosNum;
        mNrmNum = other.mNrmNum;
        field_0x4 = other.field_0x4;
        field_0x8 = other.field_0x8;
    }

    /* 0x00 */ BE(u16) mPosNum;
    /* 0x02 */ BE(u16) mNrmNum;
    /* 0x04 */ OFFSET_PTR field_0x4;
    /* 0x08 */ OFFSET_PTR field_0x8;
};  // Size: 0x0C

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DDeformData {
public:
    J3DDeformData();
    void offAllFlag(u32);
    void deform(J3DVertexBuffer*);
    void deform(J3DModel*);
    void setAnm(J3DAnmCluster*);

    J3DCluster* getClusterPointer(u16 index) {
        J3D_ASSERT_RANGE(186, (index < mClusterNum));
        return &mClusterPointer[index];
    }
    u16 getClusterNum() const { return mClusterNum; }
    u16 getClusterKeyNum() const { return mClusterKeyNum; }
    J3DClusterKey* getClusterKeyPointer(u16 i) {
        J3D_ASSERT_RANGE(199, (i < mClusterKeyNum));
        return &mClusterKeyPointer[i];
    }
    BE(f32)* getVtxPos() { return mVtxPos; }
    BE(f32)* getVtxNrm() { return mVtxNrm; }

    /* 0x00 */ u16 mClusterNum;
    /* 0x02 */ u16 mClusterKeyNum;
    /* 0x04 */ u16 mClusterVertexNum;
    /* 0x08 */ J3DCluster* mClusterPointer;
    /* 0x0C */ J3DClusterKey* mClusterKeyPointer;
    /* 0x10 */ J3DClusterVertex* mClusterVertex;
    /* 0x14 */ u16 mVtxPosNum;
    /* 0x16 */ u16 mVtxNrmNum;
    /* 0x18 */ BE(f32)* mVtxPos;
    /* 0x1C */ BE(f32)* mVtxNrm;
    /* 0x20 */ JUTNameTab* mClusterName;
    /* 0x24 */ JUTNameTab* mClusterKeyName;

#if TARGET_PC
    J3DDeformer** mDeformers;
#endif

};  // Size: 0x28

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DClusterVertex {
public:
    void operator=(const J3DClusterVertex& other) {
        mNum = other.mNum;
        field_0x4 = other.field_0x4;
        field_0x8 = other.field_0x8;
    }

    /* 0x00 */ BE(u16) mNum;
#if TARGET_PC
    /* 0x04 */ OFFSET_PTR field_0x4;
    /* 0x08 */ OFFSET_PTR field_0x8;
#else
    /* 0x04 */ u16* field_0x4;
    /* 0x08 */ u16* field_0x8;
#endif
};  // Size: 0x0C

#undef OFFSET_PTR

#endif /* J3DCLUSTER_H */
