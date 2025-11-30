#ifndef J3DMATERIALATTACH_H
#define J3DMATERIALATTACH_H

#include "JSystem/J3DAssert.h"

class J3DMaterial;
class J3DTexture;
class J3DAnmColor;
class J3DAnmTexPattern;
class J3DAnmTextureSRTKey;
class J3DAnmTevRegKey;
class JUTNameTab;

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DMaterialTable {
public:
    void clear();
    J3DMaterialTable();
    int removeMatColorAnimator(J3DAnmColor*);
    int removeTexNoAnimator(J3DAnmTexPattern*);
    int removeTexMtxAnimator(J3DAnmTextureSRTKey*);
    int removeTevRegAnimator(J3DAnmTevRegKey*);
    int createTexMtxForAnimator(J3DAnmTextureSRTKey*);
    int entryMatColorAnimator(J3DAnmColor*);
    int entryTexNoAnimator(J3DAnmTexPattern*);
    int entryTexMtxAnimator(J3DAnmTextureSRTKey*);
    int entryTevRegAnimator(J3DAnmTevRegKey*);

    virtual ~J3DMaterialTable();

    J3DMaterial* getMaterialNodePointer(u16 idx) const {
        J3D_ASSERT_RANGE(92, idx < mMaterialNum);
        return mMaterialNodePointer[idx];
    }

    J3DTexture* getTexture() const { return mTexture; }
    JUTNameTab* getTextureName() const { return mTextureName; }

    JUTNameTab* getMaterialName() const { return mMaterialName; }

    u16 getMaterialNum() const { return mMaterialNum; }
    bool isLocked() const { return field_0x1c == 1; }

private:
    friend class J3DModelLoader;
    friend class J3DModelLoader_v26;
    friend class J3DModelLoader_v21;

    /* 0x04 */ u16 mMaterialNum;
    /* 0x06 */ u16 mUniqueMatNum;
    /* 0x08 */ J3DMaterial** mMaterialNodePointer;
    /* 0x0C */ JUTNameTab* mMaterialName;
    /* 0x10 */ J3DMaterial* field_0x10;
    /* 0x14 */ J3DTexture* mTexture;
    /* 0x18 */ JUTNameTab* mTextureName;
    /* 0x1C */ u16 field_0x1c;
};  // Size: 0x20

#endif /* J3DMATERIALATTACH_H */
