#ifndef J3DCLUSTERLOADER_H
#define J3DCLUSTERLOADER_H

#include "JSystem/J3DGraphAnimator/J3DAnimation.h"

#include "JSystem/J3DGraphAnimator/J3DAnimation.h"
#include "dusk/endian.h"

#if TARGET_PC
#define OFFSET_PTR BE(u32)
#else
#define OFFSET_PTR void*
#endif

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DClusterLoaderDataBase {
    static void* load(void const*);
};

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DClusterBlock : public JUTDataBlockHeader {
public:
    /* 0x08 */ BE(u16) mClusterNum;
    /* 0x0A */ BE(u16) mClusterKeyNum;
    /* 0x0C */ BE(u16) mClusterVertexNum;
    /* 0x0E */ BE(u16) mVtxPosNum;
    /* 0x10 */ BE(u16) mVtxNrmNum;
    /* 0x14 */ OFFSET_PTR mClusterPointer;
    /* 0x18 */ OFFSET_PTR mClusterKeyPointer;
    /* 0x1C */ OFFSET_PTR mClusterVertex;
    /* 0x20 */ OFFSET_PTR mVtxPos;
    /* 0x24 */ OFFSET_PTR mVtxNrm;
    /* 0x28 */ OFFSET_PTR mClusterName;
    /* 0x2C */ OFFSET_PTR mClusterKeyName;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DClusterLoader {
public:
    virtual void* load(const void*) = 0;
    virtual ~J3DClusterLoader() {}
};

class J3DDeformData;

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DClusterLoader_v15 : public J3DClusterLoader {
public:
    J3DClusterLoader_v15();
    void readCluster(J3DClusterBlock const*);

    virtual void* load(void const*);
    virtual ~J3DClusterLoader_v15();

    /* 0x04 */ J3DDeformData* mpDeformData;
};

#undef OFFSET_PTR

#endif /* J3DCLUSTERLOADER_H */
