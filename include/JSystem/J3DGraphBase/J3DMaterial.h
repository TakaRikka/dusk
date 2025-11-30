#ifndef J3DMATERIAL_H
#define J3DMATERIAL_H

#include "JSystem/J3DGraphBase/J3DMatBlock.h"
#include "JSystem/J3DGraphBase/J3DPacket.h"
#include "JSystem/J3DGraphBase/J3DShape.h"
#include <stdint.h>

class J3DJoint;
class J3DMaterialAnm;

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DMaterial {
public:
    static J3DColorBlock* createColorBlock(u32);
    static J3DTexGenBlock* createTexGenBlock(u32);
    static J3DTevBlock* createTevBlock(int);
    static J3DIndBlock* createIndBlock(int);
    static J3DPEBlock* createPEBlock(u32, u32);
    static u32 calcSizeColorBlock(u32);
    static u32 calcSizeTexGenBlock(u32);
    static u32 calcSizeTevBlock(int);
    static u32 calcSizeIndBlock(int);
    static u32 calcSizePEBlock(u32, u32);
    void initialize();
    u32 countDLSize();
    void makeDisplayList_private(J3DDisplayListObj*);
    void setCurrentMtx();
    void calcCurrentMtx();
    void copy(J3DMaterial*);
    s32 newSharedDisplayList(u32);
    s32 newSingleSharedDisplayList(u32);

    virtual void calc(f32 const (*)[4]);
    virtual void calcDiffTexMtx(f32 const (*)[4]);
    virtual void makeDisplayList();
    virtual void makeSharedDisplayList();
    virtual void load();
    virtual void loadSharedDL();
    virtual void patch();
    virtual void diff(u32);
    virtual void reset();
    virtual void change();

    J3DMaterial() { initialize(); }
    ~J3DMaterial() {}
    J3DMaterial* getNext() { return mNext; }
    J3DShape* getShape() { return mShape; }
    J3DTevBlock* getTevBlock() { return mTevBlock; }
    J3DColorBlock* getColorBlock() { return mColorBlock; }
    J3DTexGenBlock* getTexGenBlock() { return mTexGenBlock; }
    J3DDisplayListObj* getSharedDisplayListObj() { return mSharedDLObj; }
    J3DIndBlock* getIndBlock() { return mIndBlock; }
    J3DJoint* getJoint() { return mJoint; }
    J3DMaterialAnm* getMaterialAnm() {
        if ((uintptr_t)mMaterialAnm < 0xC0000000) {
            return mMaterialAnm;
        } else {
            return NULL;
        }
    }
    J3DNBTScale* getNBTScale() { return mTexGenBlock->getNBTScale(); }
    u16 getTexNo(u32 idx) { return mTevBlock->getTexNo(idx); }
    J3DGXColor* getTevKColor(u32 param_0) { return mTevBlock->getTevKColor(param_0); }
    J3DGXColorS10* getTevColor(u32 param_0) { return mTevBlock->getTevColor(param_0); }
    J3DFog* getFog() { return mPEBlock->getFog(); }
    J3DTexMtx* getTexMtx(u32 idx) { return mTexGenBlock->getTexMtx(idx); }
    u16 getIndex() { return mIndex; }
    bool isDrawModeOpaTexEdge() { return (mMaterialMode & 3) == 0; }
    J3DPEBlock* getPEBlock() { return mPEBlock; }
    void onInvalid() { mInvalid = 1; }
    u32 getTexGenNum() const { return mTexGenBlock->getTexGenNum(); }
    u8 getTevStageNum() const { return mTevBlock->getTevStageNum(); }
    J3DTexCoord* getTexCoord(u32 idx) { return mTexGenBlock->getTexCoord(idx); }
    J3DZMode* getZMode() { return mPEBlock->getZMode(); }
    J3DBlend* getBlend() { return mPEBlock->getBlend(); }
    J3DColorChan* getColorChan(u32 idx) { return getColorBlock()->getColorChan(idx); }
    J3DGXColor* getMatColor(u32 i) { return mColorBlock->getMatColor(i); }

    void setTevColor(u32 i, const J3DGXColorS10* i_color) { mTevBlock->setTevColor(i, i_color); }
    void setTevKColor(u32 i, const J3DGXColor* i_color) { mTevBlock->setTevKColor(i, i_color); }
    void setAmbColor(u32 i, const J3DGXColor* i_color) { mColorBlock->setAmbColor(i, i_color); }
    void setLight(u32 i, J3DLightObj* i_lightobj) { mColorBlock->setLight(i, i_lightobj); }
    void setMaterialAnm(J3DMaterialAnm* i_anm) { mMaterialAnm = i_anm; }
    void setCullMode(u8 i_mode) { mColorBlock->setCullMode(i_mode); }
    void setTexMtx(u32 idx, J3DTexMtx* mtx) { mTexGenBlock->setTexMtx(idx, mtx); }
    void setZCompLoc(u8 i_comploc) { mPEBlock->setZCompLoc(i_comploc); }
    void setMaterialMode(u32 i_mode) { mMaterialMode = i_mode; }

    void addShape(J3DShape* pShape) {
        J3D_ASSERT_NULLPTR(618, pShape != NULL);
        mShape = pShape;
    }

    void setNext(J3DMaterial* pMaterial) {
        J3D_ASSERT_NULLPTR(623, pMaterial != NULL);
        mNext = pMaterial;
    }

    void setJoint(J3DJoint* pJoint) {
        J3D_ASSERT_NULLPTR(628, pJoint != NULL);
        mJoint = pJoint;
    }

public:
    /* 0x04 */ J3DMaterial* mNext;
    /* 0x08 */ J3DShape* mShape;
    /* 0x0C */ J3DJoint* mJoint;
    /* 0x10 */ u32 mMaterialMode;
    /* 0x14 */ u16 mIndex;
    /* 0x18 */ u32 mInvalid;
    /* 0x1C */ u32 field_0x1c;
    /* 0x20 */ u32 mDiffFlag;
    /* 0x24 */ J3DColorBlock* mColorBlock;
    /* 0x28 */ J3DTexGenBlock* mTexGenBlock;
    /* 0x2C */ J3DTevBlock* mTevBlock;
    /* 0x30 */ J3DIndBlock* mIndBlock;
    /* 0x34 */ J3DPEBlock* mPEBlock;
    /* 0x38 */ J3DMaterial* mpOrigMaterial;
    /* 0x3C */ J3DMaterialAnm* mMaterialAnm;
    /* 0x40 */ J3DCurrentMtx mCurrentMtx;
    /* 0x48 */ J3DDisplayListObj* mSharedDLObj;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DPatchedMaterial : public J3DMaterial {
public:
    J3DPatchedMaterial() { initialize(); }
    void initialize();

    virtual void makeDisplayList();
    virtual void makeSharedDisplayList();
    virtual void load();
    virtual void loadSharedDL();
    virtual void reset();
    virtual void change();
};

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DLockedMaterial : public J3DMaterial {
public:
    J3DLockedMaterial() { initialize(); }
    void initialize();

    virtual void calc(f32 const (*)[4]);
    virtual void makeDisplayList();
    virtual void makeSharedDisplayList();
    virtual void load();
    virtual void loadSharedDL();
    virtual void patch();
    virtual void diff(u32);
    virtual void reset();
    virtual void change();
};

#endif /* J3DMATERIAL_H */
