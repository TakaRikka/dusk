#ifndef J3DMTXBUFFER_H
#define J3DMTXBUFFER_H

#include "JSystem/J3DGraphAnimator/J3DModelData.h"

class J3DModelData;

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DMtxBuffer {
public:
    J3DMtxBuffer() { initialize(); }

    void initialize();
    s32 create(J3DModelData*, u32);
    J3DError createAnmMtx(J3DModelData*);
    s32 createWeightEnvelopeMtx(J3DModelData*);
    s32 setNoUseDrawMtx();
    s32 createDoubleDrawMtx(J3DModelData*, u32);
    s32 createBumpMtxArray(J3DModelData*, u32);
    void calcWeightEnvelopeMtx();
    void calcDrawMtx(u32, Vec const&, f32 const (&)[3][4]);
    void calcNrmMtx();
    void calcBBoardMtx();

    MtxP getAnmMtx(int idx) { return mpAnmMtx[idx]; }
    void setAnmMtx(int i, Mtx m) { MTXCopy(m, (MtxP)mpAnmMtx[i]); }
    MtxP getWeightAnmMtx(int idx) { return mpWeightEvlpMtx[idx]; }
    MtxP getUserAnmMtx(int idx) { return mpUserAnmMtx[idx]; }

    void setScaleFlag(int idx, u8 flag) { mpScaleFlagArr[idx] = flag; }
    u32* getCurrentViewNoPtr() { return &mCurrentViewNo; }
    u8* getScaleFlagArray() const { return mpScaleFlagArr; }
    u8 getScaleFlag(int idx) const { return mpScaleFlagArr[idx]; }
    u8 getEnvScaleFlag(int idx) const { return mpEvlpScaleFlagArr[idx]; }
    Mtx** getDrawMtxPtrPtr() { return mpDrawMtxArr[1]; }
    Mtx* getDrawMtxPtr() { return mpDrawMtxArr[1][mCurrentViewNo]; }
    Mtx* getDrawMtx(int idx) { return &mpDrawMtxArr[1][mCurrentViewNo][idx]; }
    Mtx33** getNrmMtxPtrPtr() { return mpNrmMtxArr[1]; }
    Mtx33* getNrmMtxPtr() { return mpNrmMtxArr[1][mCurrentViewNo]; }
    Mtx33* getNrmMtx(int idx) { return &mpNrmMtxArr[1][mCurrentViewNo][idx]; }
    Mtx33*** getBumpMtxPtrPtr() const { return mpBumpMtxArr[1]; }
    Mtx33* getBumpMtxPtr(int idx) { return mpBumpMtxArr[1][idx][mCurrentViewNo]; }
    J3DJointTree* getJointTree() const { return mJointTree; }

    void setNrmMtx(int idx, Mtx mtx) {
        J3DPSMtx33CopyFrom34(mtx, mpNrmMtxArr[1][mCurrentViewNo][idx]);
    }

    void swapDrawMtx() {
        Mtx* tmp = mpDrawMtxArr[0][mCurrentViewNo];
        mpDrawMtxArr[0][mCurrentViewNo] = mpDrawMtxArr[1][mCurrentViewNo];
        mpDrawMtxArr[1][mCurrentViewNo] = tmp;
    }

    void swapNrmMtx() {
        Mtx33* tmp = mpNrmMtxArr[0][mCurrentViewNo];
        mpNrmMtxArr[0][mCurrentViewNo] = mpNrmMtxArr[1][mCurrentViewNo];
        mpNrmMtxArr[1][mCurrentViewNo] = tmp;
    }

    static Mtx sNoUseDrawMtx;
    static Mtx33 sNoUseNrmMtx;
    static Mtx* sNoUseDrawMtxPtr;
    static Mtx33* sNoUseNrmMtxPtr;

    /* 0x00 */ J3DJointTree* mJointTree;
    /* 0x04 */ u8* mpScaleFlagArr;
    /* 0x08 */ u8* mpEvlpScaleFlagArr;
    /* 0x0C */ Mtx* mpAnmMtx;
    /* 0x10 */ Mtx* mpWeightEvlpMtx;
    /* 0x14 */ Mtx** mpDrawMtxArr[2];
    /* 0x1C */ Mtx33** mpNrmMtxArr[2];
    /* 0x24 */ Mtx33*** mpBumpMtxArr[2];
    /* 0x2C */ u32 mMtxNum;
    /* 0x30 */ u32 mCurrentViewNo;
    /* 0x34 */ Mtx* mpUserAnmMtx;

public:
    virtual ~J3DMtxBuffer() {}
};

void J3DCalcViewBaseMtx(f32 (*param_0)[4], Vec const& param_1, f32 const (&param_2)[3][4],
                            f32 (*param_3)[4]);

#endif /* J3DMTXBUFFER_H */
