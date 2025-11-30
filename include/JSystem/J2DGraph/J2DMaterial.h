#ifndef J2DMATERIAL_H
#define J2DMATERIAL_H

#include "JSystem/J2DGraph/J2DAnimation.h"
#include "JSystem/J2DGraph/J2DMatBlock.h"

/**
 * @ingroup jsystem-j2d
 * 
 */
class J2DMaterial {
public:
    struct J2DMaterialAnmPointer {
        J2DMaterialAnmPointer();

        /* 0x00 */ J2DAnmColor* mColorAnm;
        /* 0x04 */ J2DAnmTextureSRTKey* mSRTAnm;
        /* 0x08 */ J2DAnmTexPattern* mPatternAnm;
        /* 0x0C */ J2DAnmTevRegKey* mTevAnm;
        /* 0x10 */ u16 mColorIds;
        /* 0x12 */ u16 mSRTIds[8];
        /* 0x22 */ u16 mPatternIds[8];
        /* 0x32 */ u16 mTevCRegIds[4];
        /* 0x3A */ u16 mTevKRegIds[4];
    };  // Size: 0x44

    J2DMaterial();
    void setGX();
    static J2DTevBlock* createTevBlock(int, bool);
    static J2DIndBlock* createIndBlock(int, bool);
    void makeAnmPointer();
    void setAnimation(J2DAnmTextureSRTKey*);
    void setAnimation(J2DAnmTexPattern*);
    void setAnimation(J2DAnmTevRegKey*);
    void setAnimation(J2DAnmColor*);
    void animation();

    virtual ~J2DMaterial();

    u16 getIndex() const { return mIndex; }
    J2DColorBlock* getColorBlock() { return &mColorBlock; }
    J2DTexGenBlock* getTexGenBlock() { return &mTexGenBlock; }
    J2DPEBlock* getPEBlock() { return &mPEBlock; }
    J2DTevBlock* getTevBlock() { return mTevBlock; }
    J2DIndBlock* getIndBlock() { return mIndBlock; }
    bool isVisible() const { return mVisible; }
    int getMaterialAlphaCalc() const { return mMaterialAlphaCalc; }

// private:
    /* 0x04 */ void* field_0x4;
    /* 0x08 */ int field_0x8;
    /* 0x0C */ u16 mIndex;
    /* 0x0E */ bool mVisible;
    /* 0x0F */ u8 mMaterialAlphaCalc;
    /* 0x10 */ J2DColorBlock mColorBlock;
    /* 0x28 */ J2DTexGenBlock mTexGenBlock;
    /* 0x70 */ J2DTevBlock* mTevBlock;
    /* 0x74 */ J2DIndBlock* mIndBlock;
    /* 0x78 */ J2DPEBlock mPEBlock;
    /* 0x84 */ J2DMaterialAnmPointer* mAnmPointer;
};

#endif /* J2DMATERIAL_H */
