#ifndef J3DANMLOADER_H
#define J3DANMLOADER_H

#include "JSystem/J3DGraphAnimator/J3DAnimation.h"

enum J3DAnmLoaderDataBaseFlag {
    J3DLOADER_UNK_FLAG0,
    J3DLOADER_UNK_FLAG1,
};

class J3DAnmBase;

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DAnmLoaderDataBase {
    static J3DAnmBase* load(void const*, J3DAnmLoaderDataBaseFlag);
    static void setResource(J3DAnmBase*, void const*);

    static J3DAnmBase* load(void const* param_0) { return load(param_0, J3DLOADER_UNK_FLAG0); }
};

class J3DAnmLoader {
public:
    virtual J3DAnmBase* load(const void*) = 0;
    virtual void setResource(J3DAnmBase*, const void*) = 0;
    virtual ~J3DAnmLoader() {}
};

class J3DAnmKeyLoader_v15 : public J3DAnmLoader {
public:
    J3DAnmKeyLoader_v15();
    void readAnmTransform(J3DAnmTransformKeyData const*);
    void setAnmTransform(J3DAnmTransformKey*, J3DAnmTransformKeyData const*);
    void readAnmTextureSRT(J3DAnmTextureSRTKeyData const*);
    void setAnmTextureSRT(J3DAnmTextureSRTKey*, J3DAnmTextureSRTKeyData const*);
    void readAnmColor(J3DAnmColorKeyData const*);
    void setAnmColor(J3DAnmColorKey*, J3DAnmColorKeyData const*);
    void readAnmCluster(J3DAnmClusterKeyData const*);
    void setAnmCluster(J3DAnmClusterKey*, J3DAnmClusterKeyData const*);
    void readAnmTevReg(J3DAnmTevRegKeyData const*);
    void setAnmTevReg(J3DAnmTevRegKey*, J3DAnmTevRegKeyData const*);
    void readAnmVtxColor(J3DAnmVtxColorKeyData const*);
    void setAnmVtxColor(J3DAnmVtxColorKey*, J3DAnmVtxColorKeyData const*);

    virtual J3DAnmBase* load(void const*);
    virtual void setResource(J3DAnmBase*, void const*);
    virtual ~J3DAnmKeyLoader_v15();

    /* 0x4 */ J3DAnmBase* mAnm;
};

class J3DAnmFullLoader_v15 : public J3DAnmLoader {
public:
    J3DAnmFullLoader_v15();
    void readAnmTransform(J3DAnmTransformFullData const*);
    void setAnmTransform(J3DAnmTransformFull*, J3DAnmTransformFullData const*);
    void readAnmColor(J3DAnmColorFullData const*);
    void setAnmColor(J3DAnmColorFull*, J3DAnmColorFullData const*);
    void readAnmTexPattern(J3DAnmTexPatternFullData const*);
    void setAnmTexPattern(J3DAnmTexPattern*, J3DAnmTexPatternFullData const*);
    void readAnmVisibility(J3DAnmVisibilityFullData const*);
    void setAnmVisibility(J3DAnmVisibilityFull*, J3DAnmVisibilityFullData const*);
    void readAnmCluster(J3DAnmClusterFullData const*);
    void setAnmCluster(J3DAnmClusterFull*, J3DAnmClusterFullData const*);
    void readAnmVtxColor(J3DAnmVtxColorFullData const*);
    void setAnmVtxColor(J3DAnmVtxColorFull*, J3DAnmVtxColorFullData const*);

    virtual J3DAnmBase* load(void const*);
    virtual void setResource(J3DAnmBase*, void const*);
    virtual ~J3DAnmFullLoader_v15();

    /* 0x4 */ J3DAnmBase* mAnm;
};

#endif /* J3DANMLOADER_H */
