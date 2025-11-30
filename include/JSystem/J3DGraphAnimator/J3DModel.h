#ifndef J3DMODEL_H
#define J3DMODEL_H

#include "JSystem/J3DGraphAnimator/J3DSkinDeform.h"
#include "JSystem/J3DGraphBase/J3DPacket.h"
#include "dolphin/types.h"

enum J3DMdlFlag {
    J3DMdlFlag_None                           = 0x0,
    /* 0x00001 */ J3DMdlFlag_Unk1             = 0x1,
    /* 0x00002 */ J3DMdlFlag_UseDefaultJ3D    = 0x2,
    /* 0x00004 */ J3DMdlFlag_SkinPosCpu       = 0x4,
    /* 0x00008 */ J3DMdlFlag_SkinNrmCpu       = 0x8,
    /* 0x00010 */ J3DMdlFlag_EnableLOD        = 0x10, 
    /* 0x20000 */ J3DMdlFlag_UseSharedDL      = 0x20000,
    /* 0x40000 */ J3DMdlFlag_UseSingleDL      = 0x40000,
    /* 0x80000 */ J3DMdlFlag_DifferedDLBuffer = 0x80000,
};

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DUnkCalc1 {
    virtual void calc(J3DModel* model);
};

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DUnkCalc2 {
    virtual void unk();
    virtual void calc(J3DModelData* mpModelData);
};

typedef void (*J3DCalcCallBack)(J3DModel*, u32 timing);

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DModel {
public:
    J3DModel() {
        initialize();
    }

    J3DModel(J3DModelData* pModelData, u32 mdlFlags, u32 mtxBufferFlag) {
        initialize();
        entryModelData(pModelData, mdlFlags, mtxBufferFlag);
    }

    void initialize();
    s32 entryModelData(J3DModelData*, u32, u32);
    s32 createShapePacket(J3DModelData*);
    s32 createMatPacket(J3DModelData*, u32);
    s32 newDifferedDisplayList(u32);
    void lock();
    void unlock();
    void diff();
    s32 setDeformData(J3DDeformData*, u32);
    s32 setSkinDeform(J3DSkinDeform*, u32);
    void calcAnmMtx();
    void calcWeightEnvelopeMtx();
    void calcNrmMtx();
    void calcBumpMtx();
    void calcBBoardMtx();
    void prepareShapePackets();
    void ptrToIndex();
    void makeDL();

    virtual void update();
    virtual void entry();
    virtual void calc();
    virtual void calcMaterial();
    virtual void calcDiffTexMtx();
    virtual void viewCalc();
    virtual ~J3DModel() {}

    J3DModelData* getModelData() { return mModelData; }

    void onFlag(u32 flag) { mFlags |= flag; }
    void offFlag(u32 flag) { mFlags &= ~flag; }
    bool checkFlag(u32 flag) const { return (mFlags & flag) ? true : false; }

    bool isCpuSkinningOn() const { return (mFlags & J3DMdlFlag_SkinPosCpu) && (mFlags & J3DMdlFlag_SkinNrmCpu); }

    Mtx& getBaseTRMtx() { return mBaseTransformMtx; }
    void setBaseTRMtx(Mtx m) { MTXCopy(m, mBaseTransformMtx); }
    u32 getMtxCalcMode() { return mFlags & (J3DMdlFlag_Unk1 | J3DMdlFlag_UseDefaultJ3D); }
    J3DVertexBuffer* getVertexBuffer() { return (J3DVertexBuffer*)&mVertexBuffer; }
    J3DMatPacket* getMatPacket(u16 idx) const { return &mMatPacket[idx]; }
    J3DShapePacket* getShapePacket(u16 idx) const { return &mShapePacket[idx]; }
    J3DMtxBuffer* getMtxBuffer() { return mMtxBuffer; }
    void setScaleFlag(int idx, u8 flag) { mMtxBuffer->setScaleFlag(idx, flag); }
    Mtx33* getBumpMtxPtr(int idx) { return mMtxBuffer->getBumpMtxPtr(idx); }
    Mtx33* getNrmMtxPtr() { return mMtxBuffer->getNrmMtxPtr(); }
    Mtx* getDrawMtxPtr() { return mMtxBuffer->getDrawMtxPtr(); }
    void setBaseScale(const Vec& scale) { mBaseScale = scale; }
    void setUserArea(uintptr_t area) { mUserArea = area; }
    uintptr_t getUserArea() const { return mUserArea; }
    Vec* getBaseScale() { return &mBaseScale; }
    void setAnmMtx(int jointNo, Mtx m) { mMtxBuffer->setAnmMtx(jointNo, m); }
    MtxP getAnmMtx(int jointNo) { return mMtxBuffer->getAnmMtx(jointNo); }
    MtxP getWeightAnmMtx(int i) { return mMtxBuffer->getWeightAnmMtx(i); }
    J3DSkinDeform* getSkinDeform() { return mSkinDeform; }

    /* 0x04 */ J3DModelData* mModelData;
    /* 0x08 */ u32 mFlags;
    /* 0x0C */ u32 mDiffFlag;
    /* 0x10 */ J3DCalcCallBack mCalcCallBack;
    /* 0x14 */ uintptr_t mUserArea;
    /* 0x18 */ Vec mBaseScale;
    /* 0x24 */ Mtx mBaseTransformMtx;
    /* 0x54 */ Mtx mInternalView;
    /* 0x84 */ J3DMtxBuffer* mMtxBuffer;
    /* 0x88 */ J3DVertexBuffer mVertexBuffer;
    /* 0xC0 */ J3DMatPacket* mMatPacket;
    /* 0xC4 */ J3DShapePacket* mShapePacket;
    /* 0xC8 */ J3DDeformData* mDeformData;
    /* 0xCC */ J3DSkinDeform* mSkinDeform;
    /* 0xD0 */ J3DVtxColorCalc* mVtxColorCalc;
    /* 0xD4 */ J3DUnkCalc1* mUnkCalc1;
    /* 0xD8 */ J3DUnkCalc2* mUnkCalc2;
};

#endif /* J3DMODEL_H */
