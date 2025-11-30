#ifndef J3DCLUSTERLOADER_H
#define J3DCLUSTERLOADER_H

#include "JSystem/J3DGraphAnimator/J3DAnimation.h"

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
    /* 0x08 */ u16 mClusterNum;
    /* 0x0A */ u16 mClusterKeyNum;
    /* 0x0C */ u16 mClusterVertexNum;
    /* 0x0E */ u16 mVtxPosNum;
    /* 0x10 */ u16 mVtxNrmNum;
    /* 0x14 */ void* mClusterPointer;
    /* 0x18 */ void* mClusterKeyPointer;
    /* 0x1C */ void* mClusterVertex;
    /* 0x20 */ void* mVtxPos;
    /* 0x24 */ void* mVtxNrm;
    /* 0x28 */ void* mClusterName;
    /* 0x2C */ void* mClusterKeyName;
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

#endif /* J3DCLUSTERLOADER_H */
