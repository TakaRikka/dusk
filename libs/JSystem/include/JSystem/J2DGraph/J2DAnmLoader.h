#ifndef J2DANMLOADER_H
#define J2DANMLOADER_H

#include "JSystem/J2DGraph/J2DAnimation.h"

/**
 * @ingroup jsystem-j2d
 * 
 */
struct J2DAnmLoaderDataBase {
    static J2DAnmBase* load(void const*);
};

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DAnmLoader {
public:
    virtual void* load(void const*) = 0;
    virtual void setResource(J2DAnmBase*, void const*) = 0;
    virtual ~J2DAnmLoader() {}

    void* mpResource;
};

struct J3DAnmTextureSRTKeyData;
struct J3DAnmColorKeyData;
struct J3DAnmVtxColorKeyData;
struct J3DAnmTevRegKeyData;
struct J3DAnmTransformFullData;
struct J3DAnmColorFullData;
struct J3DAnmTexPatternFullData;
struct J3DAnmVisibilityFullData;
struct J3DAnmVtxColorFullData;
struct J3DAnmTransformKeyData;

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DAnmKeyLoader_v15 : public J2DAnmLoader {
public:
    J2DAnmKeyLoader_v15();
    void readAnmTransform(J3DAnmTransformKeyData const*);
    void setAnmTransform(J2DAnmTransformKey*, J3DAnmTransformKeyData const*);
    void readAnmTextureSRT(J3DAnmTextureSRTKeyData const*);
    void setAnmTextureSRT(J2DAnmTextureSRTKey*, J3DAnmTextureSRTKeyData const*);
    void readAnmColor(J3DAnmColorKeyData const*);
    void setAnmColor(J2DAnmColorKey*, J3DAnmColorKeyData const*);
    void readAnmVtxColor(J3DAnmVtxColorKeyData const*);
    void setAnmVtxColor(J2DAnmVtxColorKey*, J3DAnmVtxColorKeyData const*);
    void readAnmTevReg(J3DAnmTevRegKeyData const*);
    void setAnmTevReg(J2DAnmTevRegKey*, J3DAnmTevRegKeyData const*);

    virtual void* load(void const*);
    virtual void setResource(J2DAnmBase*, void const*);
    virtual ~J2DAnmKeyLoader_v15();

    // /* 0x04 */ void* _4;
};

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DAnmFullLoader_v15 : public J2DAnmLoader {
public:
    J2DAnmFullLoader_v15();
    void readAnmTransform(J3DAnmTransformFullData const*);
    void setAnmTransform(J2DAnmTransformFull*, J3DAnmTransformFullData const*);
    void readAnmColor(J3DAnmColorFullData const*);
    void setAnmColor(J2DAnmColorFull*, J3DAnmColorFullData const*);
    void readAnmTexPattern(J3DAnmTexPatternFullData const*);
    void setAnmTexPattern(J2DAnmTexPattern*, J3DAnmTexPatternFullData const*);
    void readAnmVisibility(J3DAnmVisibilityFullData const*);
    void setAnmVisibility(J2DAnmVisibilityFull*, J3DAnmVisibilityFullData const*);
    void readAnmVtxColor(J3DAnmVtxColorFullData const*);
    void setAnmVtxColor(J2DAnmVtxColorFull*, J3DAnmVtxColorFullData const*);

    virtual void* load(void const*);
    virtual void setResource(J2DAnmBase*, void const*);
    virtual ~J2DAnmFullLoader_v15();

    // void* _4;
};

#endif /* J2DANMLOADER_H */
