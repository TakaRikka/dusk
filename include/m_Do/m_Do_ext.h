#ifndef M_DO_M_DO_EXT_H
#define M_DO_M_DO_EXT_H

#include "JSystem/J3DGraphAnimator/J3DAnimation.h"
#include "JSystem/J3DGraphAnimator/J3DModel.h"
#include "Z2AudioLib/Z2SoundObject.h"
#include "m_Do/m_Do_audio.h"
#include "m_Do/m_Do_mtx.h"

class JKRArchive;
class JKRAssertHeap;
class JKRExpHeap;
class JKRHeap;
class JKRSolidHeap;
struct ResTIMG;
class Z2Creature;

namespace mDoExt {
    extern u8 HeapAdjustVerbose;
    extern u8 HeapAdjustQuiet;
};

#if DEBUG
void DummyCheckHeap_create();
void DummyCheckHeap_destroy();
#endif

class mDoExt_baseAnm {
public:
    mDoExt_baseAnm() {}
    int initPlay(s16 i_frameMax, int i_attribute, f32 i_rate, s16 i_startF,
                                s16 i_endF);
    int play();

    J3DFrameCtrl* getFrameCtrl() { return &mFrameCtrl; }
    f32 getPlaySpeed() { return mFrameCtrl.getRate(); }
    void setPlaySpeed(f32 speed) { mFrameCtrl.setRate(speed); }
    f32 getFrame() { return mFrameCtrl.getFrame(); }
    f32 getEndFrame() { return mFrameCtrl.getEnd(); }
    f32 getStartFrame() { return mFrameCtrl.getStart(); }
    void setEndFrame(f32 frame) { mFrameCtrl.setEnd(frame); }
    void setFrame(f32 frame) { mFrameCtrl.setFrame(frame); }
    void setPlayMode(int i_mode) { mFrameCtrl.setAttribute(i_mode); }
    void setLoopFrame(f32 i_frame) { mFrameCtrl.setLoop(i_frame); }
    bool isStop() { return mFrameCtrl.checkState(1) || mFrameCtrl.getRate() == 0.0f; }
    bool isLoop() { return mFrameCtrl.checkState(2); }
    void reset() { mFrameCtrl.reset(); }
    BOOL checkFrame(f32 frame) { return mFrameCtrl.checkPass(frame); }

private:
    /* 0x0 */ J3DFrameCtrl mFrameCtrl;
};  // Size: 0x14

class mDoExt_transAnmBas : public J3DAnmTransformKey {
public:
    mDoExt_transAnmBas(void* bas) { mBas = bas; }
    virtual ~mDoExt_transAnmBas() {}

    void* getBas() { return mBas; }

private:
    /* 0x28 */ void* mBas;
};

class mDoExt_btkAnm : public mDoExt_baseAnm {
public:
    mDoExt_btkAnm() { mpAnm = 0; }
    int init(J3DMaterialTable* i_matTable, J3DAnmTextureSRTKey* i_btk, int i_anmPlay,
                            int i_attribute, f32 i_rate, s16 i_start, s16 param_6);
    void entry(J3DMaterialTable* i_matTable, f32 i_frame);

    void entry(J3DModelData* i_modelData) { entry(i_modelData, getFrame()); }
    void entry(J3DModelData* i_modelData, f32 i_frame) {
        entry(&i_modelData->getMaterialTable(), i_frame);
    }
    int init(J3DModelData* i_modelData, J3DAnmTextureSRTKey* i_btk, int i_anmPlay, int i_attribute,
             f32 i_rate, s16 i_start, s16 param_6) {
        return init(&i_modelData->getMaterialTable(), i_btk, i_anmPlay, i_attribute, i_rate,
                    i_start, param_6);
    }

    int remove(J3DModelData* i_modelData) { return i_modelData->removeTexMtxAnimator(mpAnm); }
    void entryFrame() { entryFrame(getFrame()); }
    void entryFrame(f32 frame) { mpAnm->setFrame(frame); }

    J3DAnmTextureSRTKey* getBtkAnm() const { return mpAnm; }

private:
    /* 0x14 */ J3DAnmTextureSRTKey* mpAnm;
};  // Size: 0x18

STATIC_ASSERT(sizeof(mDoExt_btkAnm) == 0x18);

class mDoExt_brkAnm : public mDoExt_baseAnm {
public:
    mDoExt_brkAnm() { mpAnm = 0; }
    int init(J3DMaterialTable* i_matTable, J3DAnmTevRegKey* i_brk, int i_anmPlay,
                            int i_attribute, f32 i_rate, s16 i_start, s16 param_6);
    void entry(J3DMaterialTable* i_matTable, f32 i_frame);

    void entry(J3DModelData* i_modelData) { entry(i_modelData, getFrame()); }
    void entry(J3DModelData* i_modelData, f32 i_frame) {
        entry(&i_modelData->getMaterialTable(), i_frame);
    }

    int init(J3DModelData* i_modelData, J3DAnmTevRegKey* i_brk, int i_anmPlay, int i_attribute,
             f32 i_rate, s16 i_start, s16 param_6) {
        return init(&i_modelData->getMaterialTable(), i_brk, i_anmPlay, i_attribute, i_rate,
                    i_start, param_6);
    }

    int remove(J3DModelData* i_modelData) { return i_modelData->removeTevRegAnimator(mpAnm); }
    void entryFrame() { entryFrame(getFrame()); }
    void entryFrame(f32 frame) { mpAnm->setFrame(frame); }

    J3DAnmTevRegKey* getBrkAnm() const { return mpAnm; }

private:
    /* 0x14 */ J3DAnmTevRegKey* mpAnm;
};  // Size: 0x18

STATIC_ASSERT(sizeof(mDoExt_brkAnm) == 0x18);

class mDoExt_bpkAnm : public mDoExt_baseAnm {
public:
    mDoExt_bpkAnm() { mpAnm = 0; }
    int init(J3DMaterialTable* i_matTable, J3DAnmColor* i_bpk, int i_anmPlay,
                            int i_attribute, f32 i_rate, s16 i_start, s16 param_6);
    void entry(J3DMaterialTable* i_matTable, f32 i_frame);

    void entry(J3DModelData* i_modelData) { entry(i_modelData, getFrame()); }
    void entry(J3DModelData* i_modelData, f32 i_frame) {
        entry(&i_modelData->getMaterialTable(), i_frame);
    }

    int init(J3DModelData* i_modelData, J3DAnmColor* i_bpk, int i_anmPlay, int i_attribute,
             f32 i_rate, s16 i_start, s16 param_6) {
        return init(&i_modelData->getMaterialTable(), i_bpk, i_anmPlay, i_attribute, i_rate,
                    i_start, param_6);
    }

    int remove(J3DModelData* i_modelData) { return i_modelData->removeMatColorAnimator(mpAnm); }
    J3DAnmColor* getBpkAnm() { return mpAnm; }

private:
    /* 0x14 */ J3DAnmColor* mpAnm;
};  // Size: 0x18

STATIC_ASSERT(sizeof(mDoExt_bpkAnm) == 0x18);

class mDoExt_bckAnm : public mDoExt_baseAnm {
public:
    mDoExt_bckAnm() { mpMtxCalc = NULL; }
    int init(J3DAnmTransform* i_bck, int i_play, int i_attr, f32 i_rate,
                            s16 i_startF, s16 i_endF, bool i_modify);
    void changeBckOnly(J3DAnmTransform* i_bck);
    void entry(J3DModelData* i_modelData, f32 i_frame);
    void entryJoint(J3DModelData* i_modelData, u16 i_jntNo, f32 i_frame);
    inline void entryJoint(J3DModelData* i_modelData, u16 i_jntNo) {
        entryJoint(i_modelData, i_jntNo, getFrame());
    }

    void entry(J3DModelData* i_modelData) { entry(i_modelData, getFrame()); }

    void remove(J3DModelData* i_modelData) {
        i_modelData->getJointNodePointer(0)->setMtxCalc(NULL);
    }

    J3DAnmTransform* getBckAnm() { return mAnm; }

    void removeJoint(J3DModelData* i_modelData, u16 i_idx) {
        i_modelData->getJointNodePointer(i_idx)->setMtxCalc(0);
    }

private:
    /* 0x14 */ J3DAnmTransform* mAnm;
    /* 0x18 */ J3DMtxCalc* mpMtxCalc;
};  // Size: 0x1C

STATIC_ASSERT(sizeof(mDoExt_bckAnm) == 0x1C);

class mDoExt_btpAnm : public mDoExt_baseAnm {
public:
    mDoExt_btpAnm() { mpAnm = NULL; }
    int init(J3DMaterialTable* i_matTable, J3DAnmTexPattern* i_btk, int i_anmPlay,
                            int i_attribute, f32 i_rate, s16 i_start, s16 param_6);
    void entry(J3DMaterialTable* i_matTable, s16 i_frame);

    void entry(J3DModelData* i_modelData) { entry(i_modelData, getFrame()); }
    void entry(J3DModelData* i_modelData, s16 i_frame) {
        entry(&i_modelData->getMaterialTable(), i_frame);
    }

    int init(J3DModelData* i_modelData, J3DAnmTexPattern* i_btk, int i_anmPlay, int i_attribute,
             f32 i_rate, s16 i_start, s16 param_6) {
        return init(&i_modelData->getMaterialTable(), i_btk, i_anmPlay, i_attribute, i_rate,
                    i_start, param_6);
    }

    int remove(J3DModelData* i_modelData) { return i_modelData->removeTexNoAnimator(mpAnm); }

    J3DAnmTexPattern* getBtpAnm() const { return mpAnm; }

private:
    /* 0x14 */ J3DAnmTexPattern* mpAnm;
};

class mDoExt_blkAnm : public mDoExt_baseAnm {
public:
    mDoExt_blkAnm() {
        mpAnm = NULL;
    }

    int init(J3DDeformData* i_deformData, J3DAnmCluster* i_blk, int i_anmPlay,
                            int i_attribute, f32 i_rate, s16 i_start, s16 param_6);

    J3DAnmCluster* getBlkAnm() { return mpAnm; }

    void entryFrame() { entryFrame(getFrame()); }
    void entryFrame(f32 frame) { mpAnm->setFrame(frame); }

private:
    /* 0x14 */ J3DAnmCluster* mpAnm;
};

class J3DTransformInfo;

class mDoExt_AnmRatioPack {
public:
    ~mDoExt_AnmRatioPack() {}
    mDoExt_AnmRatioPack() {
        mRatio = 0.0f;
        mAnmTransform = NULL;
    }

    void setRatio(f32 ratio) { mRatio = ratio; }
    f32 getRatio() { return mRatio; }
    J3DAnmTransform* getAnmTransform() { return mAnmTransform; }
    void setAnmTransform(J3DAnmTransform* anm) { mAnmTransform = anm; }

private:
    /* 0x0 */ f32 mRatio;
    /* 0x4 */ J3DAnmTransform* mAnmTransform;
};  // Size: 0x8

class mDoExt_invJntPacket : public J3DPacket {
public:
    mDoExt_invJntPacket() {}

    virtual void draw();
    virtual ~mDoExt_invJntPacket() {}

    void setJoint(J3DModel* i_model, u16 param_1, u8 param_2) {
        field_0x10 = i_model;
        field_0x14 = param_1;
        field_0x16 = param_2;
    }

private:
    /* 0x10 */ J3DModel* field_0x10;
    /* 0x14 */ u16 field_0x14;
    /* 0x16 */ u8 field_0x16;
};  // Size: 0x18

class mDoExt_invisibleModel {
public:
    mDoExt_invisibleModel() {}
    int create(J3DModel* i_model, u8 param_1);
    void entryJoint(cXyz*);
    void entryDL(cXyz*);

    /* 0x0 */ J3DModel* mModel;
    /* 0x4 */ mDoExt_invJntPacket* mpPackets;
};

class mDoExt_zelAnime : public Z2SoundObjAnime {
public:
    void* field_0x48;
};

class mDoExt_McaMorfCallBack1_c {
public:
    virtual int execute(u16, J3DTransformInfo*) = 0;
};

class mDoExt_McaMorfCallBack2_c {
public:
    virtual void execute(u16) = 0;
};

class mDoExt_morf_c
    : public J3DMtxCalcNoAnm<J3DMtxCalcCalcTransformMaya, J3DMtxCalcJ3DSysInitMaya> {
public:
    mDoExt_morf_c();
    void setMorf(f32 i_morf);
    void frameUpdate();

    virtual ~mDoExt_morf_c();

    J3DAnmTransform* getAnm() { return mpAnm; }
    void changeAnm(J3DAnmTransform* anm) { mpAnm = anm; }
    int getPlayMode() { return mFrameCtrl.getAttribute(); }
    void setPlayMode(int mode) { mFrameCtrl.setAttribute(mode); }
    BOOL isStop() { return mFrameCtrl.checkState(1) || mFrameCtrl.getRate() == 0.0f; }
    bool isLoop() { return mFrameCtrl.checkState(2); }
    f32 getStartFrame() { return mFrameCtrl.getStart(); }
    void setStartFrame(f32 frame) { mFrameCtrl.setStart(frame); }
    f32 getEndFrame() { return mFrameCtrl.getEnd(); }
    void setEndFrame(f32 frame) { mFrameCtrl.setEnd(frame); }
    f32 getLoopFrame() { return mFrameCtrl.getLoop(); }
    void setLoopFrame(f32 frame) { mFrameCtrl.setLoop(frame); }
    f32 getPlaySpeed() { return mFrameCtrl.getRate(); }
    void setPlaySpeed(f32 speed) { mFrameCtrl.setRate(speed); }
    f32 getFrame() { return mFrameCtrl.getFrame(); }
    void setFrame(f32 frame) { mFrameCtrl.setFrame((s16)frame); }
    void setFrameF(f32 frame) { mFrameCtrl.setFrame(frame); }
    BOOL checkFrame(f32 frame) { return mFrameCtrl.checkPass(frame); }
    f32 getMorfRatio() { return mCurMorf; }

    /* 0x04 */ J3DModel* mpModel;
    /* 0x08 */ J3DAnmTransform* mpAnm;
    /* 0x0C */ J3DFrameCtrl mFrameCtrl;
    /* 0x20 */ J3DTransformInfo* mpTransformInfo;
    /* 0x24 */ Quaternion* mpQuat;
    /* 0x28 */ f32 mCurMorf;
    /* 0x2C */ f32 mPrevMorf;
    /* 0x30 */ f32 mMorfStep;
    /* 0x34 */ f32 field_0x34;
};

class mDoExt_McaMorfSO : public mDoExt_morf_c {
public:
    mDoExt_McaMorfSO(J3DModelData*, mDoExt_McaMorfCallBack1_c*,
                                    mDoExt_McaMorfCallBack2_c*, J3DAnmTransform*, int, f32, int,
                                    int, Z2Creature*, u32, u32);
    int create(J3DModelData*, mDoExt_McaMorfCallBack1_c*, mDoExt_McaMorfCallBack2_c*,
                              J3DAnmTransform*, int, f32, int, int, Z2Creature*, u32, u32);
    void setAnm(J3DAnmTransform* i_anm, int i_attr, f32 i_morf, f32 i_rate,
                               f32 i_start, f32 i_end);
    int play(u32, s8);
    void updateDL();
    void entryDL();
    void modelCalc();
    void getTransform(u16, J3DTransformInfo*);
    void stopZelAnime();

    virtual ~mDoExt_McaMorfSO();
    virtual void calc();

    J3DModel* getModel() { return mpModel; }
    void offMorfNone() { mMorfNone = false; }
    void onMorfNone() { mMorfNone = true; }
    inline void setTranslateScale(cXyz& i_scale) { mTranslateScale = i_scale; }
    J3DTransformInfo* getOldTransInfo() { return mpTransformInfo; }
    Quaternion* getOldQuaternion() { return mpQuat; }
    void offTranslate() { mTranslate = true; }
    void setTranslateScale(const cXyz& scale) { mTranslateScale = scale; }

private:
    /* 0x38 */ mDoExt_McaMorfCallBack1_c* mpCallback1;
    /* 0x3C */ mDoExt_McaMorfCallBack2_c* mpCallback2;
    /* 0x40 */ cXyz mTranslateScale;
    /* 0x4C */ Z2Creature* mpSound;
    /* 0x50 */ void* mpBas;
    /* 0x54 */ bool mTranslate;
    /* 0x55 */ bool mMorfNone;
};

class mDoExt_McaMorf : public mDoExt_morf_c {
public:
    mDoExt_McaMorf(J3DModelData*, mDoExt_McaMorfCallBack1_c*,
                                  mDoExt_McaMorfCallBack2_c*, J3DAnmTransform*, int, f32, int, int,
                                  int, void*, u32, u32);
    int create(J3DModelData*, mDoExt_McaMorfCallBack1_c*, mDoExt_McaMorfCallBack2_c*,
                              J3DAnmTransform*, int, f32, int, int, int, void*, u32, u32);
    void setAnm(J3DAnmTransform* i_bck, int i_mode, f32 i_morf, f32 i_speed, f32 i_startF, f32 i_endF, void* i_bas);
    u32 play(Vec*, u32, s8);
    void entryDL();
    void modelCalc();
    void getTransform(u16, J3DTransformInfo*);

    virtual ~mDoExt_McaMorf();
    virtual void calc();

    J3DModel* getModel() { return mpModel; }

private:
    /* 0x38 */ mDoExt_McaMorfCallBack1_c* mpCallback1;
    /* 0x3C */ mDoExt_McaMorfCallBack2_c* mpCallback2;
    /* 0x40 */ cXyz mTranslateScale;
    /* 0x4C */ mDoExt_zelAnime* mpSound;
    /* 0x50 */ bool field_0x50;
    /* 0x51 */ bool field_0x51;
    /* 0x52 */ bool field_0x52;
};

class mDoExt_McaMorf2 : public mDoExt_morf_c {
public:
    mDoExt_McaMorf2(J3DModelData*, mDoExt_McaMorfCallBack1_c*,
                                   mDoExt_McaMorfCallBack2_c*, J3DAnmTransform*, J3DAnmTransform*,
                                   int, f32, int, int, Z2Creature*, u32, u32);
    int create(J3DModelData*, mDoExt_McaMorfCallBack1_c*,
                               mDoExt_McaMorfCallBack2_c*, J3DAnmTransform*, J3DAnmTransform*, int,
                               f32, int, int, Z2Creature*, u32, u32);
    void ERROR_EXIT();
    void setAnm(J3DAnmTransform*, J3DAnmTransform*, f32, int, f32, f32, f32, f32);
    void setAnmRate(f32);
    int play(u32, s8);
    void entryDL();
    void modelCalc();
    void stopZelAnime();

    virtual ~mDoExt_McaMorf2();
    virtual void calc();

    inline f32 getAnmRate() { return mAnmRate; }
    inline void changeAnm(J3DAnmTransform* anm, J3DAnmTransform* anm2) {
        mpAnm = anm;
        field_0x40 = anm2;
    }
    inline J3DModel* getModel() { return mpModel; }
private:
    /* 0x38 */ mDoExt_McaMorfCallBack1_c* mpCallback1;
    /* 0x3C */ mDoExt_McaMorfCallBack2_c* mpCallback2;
    /* 0x40 */ J3DAnmTransform* field_0x40;
    /* 0x44 */ f32 mAnmRate;
    /* 0x48 */ Z2Creature* mpSound;
    /* 0x4C */ void* mpBas;
};

class mDoExt_MtxCalcOldFrame {
public:
    void initOldFrameMorf(f32, u16, u16);
    void decOldFrameMorfCounter();

    mDoExt_MtxCalcOldFrame(J3DTransformInfo* i_transinf, Quaternion* i_quat) {
        mOldFrameTransInfo = i_transinf;
        mOldFrameQuaternion = i_quat;
        mOldFrameRate = 0.0f;
        mOldFrameFlg = false;
        mOldFrameStartJoint = 0;
        mOldFrameEndJoint = 0;
        mOldFrameMorfCounter = 0.0f;
        field_0x8 = 0.0f;
        field_0x10 = 0.0f;
        field_0x14 = 0.0f;
    }

    u8 getOldFrameFlg() { return mOldFrameFlg; }
    void onOldFrameFlg() { mOldFrameFlg = true; }
    void offOldFrameFlg() { mOldFrameFlg = false; }
    f32 getOldFrameRate() { return mOldFrameRate; }
    J3DTransformInfo* getOldFrameTransInfo(int i) { return &mOldFrameTransInfo[i]; }
    u16 getOldFrameStartJoint() { return mOldFrameStartJoint; }
    u16 getOldFrameEndJoint() { return mOldFrameEndJoint; }
    Quaternion* getOldFrameQuaternion(int i_no) { return &mOldFrameQuaternion[i_no]; }
    f32 getOldFrameMorfCounter() { return mOldFrameMorfCounter; }

private:
    /* 0x00 */ u8 mOldFrameFlg;
    /* 0x04 */ f32 mOldFrameMorfCounter;
    /* 0x08 */ f32 field_0x8;
    /* 0x0C */ f32 mOldFrameRate;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ u16 mOldFrameStartJoint;
    /* 0x1A */ u16 mOldFrameEndJoint;
    /* 0x1C */ J3DTransformInfo* mOldFrameTransInfo;
    /* 0x20 */ Quaternion* mOldFrameQuaternion;
};  // Size: 0x24

struct mDoExt_MtxCalcAnmBlendTbl
    : public J3DMtxCalcNoAnm<J3DMtxCalcCalcTransformMaya, J3DMtxCalcJ3DSysInitMaya> {
    mDoExt_MtxCalcAnmBlendTbl(int num, mDoExt_AnmRatioPack* anmRatio) {
        mNum = num;
        mAnmRatio = anmRatio;
        for (int i = 0; i < mNum; i++) {
            if (!mAnmRatio[i].getAnmTransform()) {
                mAnmRatio[i].setRatio(0.0f);
            }
        }
    }

    J3DAnmTransform* getAnm(int i) { return mAnmRatio[i].getAnmTransform(); }
    f32 getRatio(int i) { return mAnmRatio[i].getRatio(); }

    virtual ~mDoExt_MtxCalcAnmBlendTbl() {}
    virtual void calc();

    /* 0x4 */ int mNum;
    /* 0x8 */ mDoExt_AnmRatioPack* mAnmRatio;
};  // Size: 0x0C

STATIC_ASSERT(sizeof(mDoExt_MtxCalcAnmBlendTbl) == 0x0C);

struct mDoExt_MtxCalcAnmBlendTblOld : public mDoExt_MtxCalcAnmBlendTbl {
    mDoExt_MtxCalcAnmBlendTblOld(mDoExt_MtxCalcOldFrame* oldFrame, int num,
                                 mDoExt_AnmRatioPack* anmRatio)
        : mDoExt_MtxCalcAnmBlendTbl(num, anmRatio) {
        mOldFrame = oldFrame;
    }
    virtual ~mDoExt_MtxCalcAnmBlendTblOld() {}
    virtual void calc();

    /* 0xC */ mDoExt_MtxCalcOldFrame* mOldFrame;
};  // Size: 0x10

STATIC_ASSERT(sizeof(mDoExt_MtxCalcAnmBlendTblOld) == 0x10);

struct mDoExt_3Dline_field_0x10_c {
    s8 x;
    s8 y;
    s8 z;
};

struct mDoExt_3Dline_field_0x18_c {
    f32 field_0x0;
    f32 field_0x4;
};

class mDoExt_3Dline_c {
public:
    int init(u16, int, BOOL);
    mDoExt_3Dline_c() {}

    /* 0x00 */ cXyz* field_0x0;
    /* 0x04 */ f32* field_0x4;
    /* 0x08 */ cXyz* field_0x8;
    /* 0x0C */ cXyz* field_0xc;
    /* 0x10 */ mDoExt_3Dline_field_0x10_c* field_0x10;
    /* 0x14 */ mDoExt_3Dline_field_0x10_c* field_0x14;
    /* 0x18 */ mDoExt_3Dline_field_0x18_c* field_0x18;
    /* 0x1C */ mDoExt_3Dline_field_0x18_c* field_0x1c;
};

class mDoExt_3DlineMat_c {
public:
    virtual int getMaterialID() = 0;
    virtual void setMaterial() = 0;
    virtual void draw() = 0;

    /* 0x4 */ mDoExt_3DlineMat_c* field_0x4;
};

class mDoExt_3DlineMatSortPacket : public J3DPacket {
public:
    mDoExt_3DlineMatSortPacket() { mp3DlineMat = NULL; }

    void reset() { mp3DlineMat = NULL; }
    void setMatDark(mDoExt_3DlineMat_c* i_mat) { setMat(i_mat); }

    void setMat(mDoExt_3DlineMat_c*);
    virtual void draw();
    virtual ~mDoExt_3DlineMatSortPacket() {}

private:
    /* 0x10 */ mDoExt_3DlineMat_c* mp3DlineMat;
};

class dKy_tevstr_c;
class mDoExt_3DlineMat1_c : public mDoExt_3DlineMat_c {
public:
    int init(u16, u16, ResTIMG*, int);
    void update(int, GXColor&, dKy_tevstr_c*);
    void update(int, f32, GXColor&, u16, dKy_tevstr_c*);
    int getMaterialID() { return 1; }
    void setMaterial();
    void draw();

    cXyz* getPos(int i_idx) { return mpLines[i_idx].field_0x0; }
    f32* getSize(int i_idx) { return mpLines[i_idx].field_0x4; }

private:
    /* 0x08 */ GXTexObj mTextureObject;
    /* 0x28 */ GXColor mColor;
    /* 0x2C */ dKy_tevstr_c* mpTevStr;
    /* 0x30 */ u16 mNumLines;
    /* 0x32 */ u16 field_0x32;
    /* 0x34 */ u16 field_0x34;
    /* 0x36 */ u8 mIsDrawn;
    /* 0x38 */ mDoExt_3Dline_c* mpLines;
};

class mDoExt_3DlineMat0_c : public mDoExt_3DlineMat_c {
public:
    int init(u16, u16, int);
    void update(int, f32, _GXColor&, u16, dKy_tevstr_c*);
    void update(int, _GXColor&, dKy_tevstr_c*);

    virtual int getMaterialID() { return 0; }
    virtual void setMaterial();
    virtual void draw();

    cXyz* getPos(int param_0) { return field_0x18[param_0].field_0x0; }
    f32* getSize(int param_0) { return field_0x18[param_0].field_0x4; }

private:
    /* 0x08 */ GXColor field_0x8;
    /* 0x0C */ dKy_tevstr_c* field_0xc;
    /* 0x10 */ u16 field_0x10;
    /* 0x12 */ u16 field_0x12;
    /* 0x14 */ u16 field_0x14;
    /* 0x16 */ u8 field_0x16;
    /* 0x18 */ mDoExt_3Dline_c* field_0x18;
};

class mDoExt_cubePacket : public J3DPacket {
public:
    mDoExt_cubePacket(cXyz& i_position, cXyz& i_size, csXyz& i_angle, const GXColor& i_color);

    virtual void draw();
    virtual ~mDoExt_cubePacket() {}

    /* 0x10 */ cXyz mPosition;
    /* 0x1C */ cXyz mSize;
    /* 0x28 */ csXyz mAngle;
    /* 0x2E */ GXColor mColor;
};

class mDoExt_cylinderPacket : public J3DPacket {
public:
    mDoExt_cylinderPacket(cXyz& i_position, f32 i_radius, f32 i_height, const GXColor& i_color, u8 i_clipZ);

    virtual void draw();
    virtual ~mDoExt_cylinderPacket() {}

    /* 0x10 */ cXyz mPosition;
    /* 0x1C */ f32 mRadius;
    /* 0x20 */ f32 mHeight;
    /* 0x24 */ GXColor mColor;
    /* 0x28 */ u8 mClipZ;
};

class mDoExt_spherePacket : public J3DPacket {
public:
    mDoExt_spherePacket(cXyz& i_position, f32 i_size, const GXColor& i_color, u8 i_clipZ);

    virtual void draw();
    virtual ~mDoExt_spherePacket() {}

    /* 0x10 */ cXyz mPosition;
    /* 0x1C */ f32 mSize;
    /* 0x20 */ GXColor mColor;
    /* 0x24 */ u8 mClipZ;
};

class mDoExt_cube8pPacket : public J3DPacket {
public:
    mDoExt_cube8pPacket(cXyz* i_points, const GXColor& i_color);

    virtual void draw();
    virtual ~mDoExt_cube8pPacket() {}

    /* 0x10 */ cXyz mPoints[8];
    /* 0x70 */ GXColor mColor;
};

class mDoExt_trianglePacket : public J3DPacket {
public:
    mDoExt_trianglePacket(cXyz* i_points, const GXColor& i_color, u8 i_clipZ);

    virtual void draw();
    virtual ~mDoExt_trianglePacket() {}

    /* 0x10 */ cXyz mPoints[3];
    /* 0x34 */ GXColor mColor;
    /* 0x38 */ u8 mClipZ;
};

class mDoExt_quadPacket : public J3DPacket {
public:
    mDoExt_quadPacket(cXyz* i_points, const GXColor& i_color, u8 i_clipZ);

    virtual void draw();
    virtual ~mDoExt_quadPacket() {}

    /* 0x10 */ cXyz mPoints[4];
    /* 0x40 */ GXColor mColor;
    /* 0x44 */ u8 mClipZ;
};

class mDoExt_linePacket : public J3DPacket {
public:
    mDoExt_linePacket(cXyz& i_start, cXyz& i_end, const GXColor& i_color, u8 i_clipZ, u8 i_width);

    virtual void draw();
    virtual ~mDoExt_linePacket() {}

    /* 0x10 */ cXyz mStart;
    /* 0x1C */ cXyz mEnd;
    /* 0x28 */ GXColor mColor;
    /* 0x2C */ u8 mClipZ;
    /* 0x2D */ u8 mWidth;
};

class mDoExt_cylinderMPacket : public J3DPacket {
public:
    mDoExt_cylinderMPacket(Mtx i_mtx, const GXColor& i_color, u8 i_clipZ);

    virtual void draw();
    virtual ~mDoExt_cylinderMPacket() {}

    /* 0x10 */ Mtx mMatrix;
    /* 0x40 */ GXColor mColor;
    /* 0x44 */ u8 mClipZ;
};

class mDoExt_circlePacket : public J3DPacket {
public:
    mDoExt_circlePacket(cXyz& i_position, f32 i_radius, const GXColor& i_color, u8 i_clipZ, u8 i_lineWidth);

    virtual void draw();
    virtual ~mDoExt_circlePacket() {}

    /* 0x10 */ cXyz mPosition;
    /* 0x1C */ f32 mRadius;
    /* 0x20 */ GXColor mColor;
    /* 0x24 */ u8 mClipZ;
    /* 0x25 */ u8 mLineWidth;
};

class mDoExt_ArrowPacket : public J3DPacket {
public:
    mDoExt_ArrowPacket(cXyz& i_position, cXyz& param_1, const GXColor& i_color, u8 i_clipZ, u8 i_lineWidth);

    virtual void draw();
    virtual ~mDoExt_ArrowPacket() {}

    /* 0x10 */ cXyz mStart;
    /* 0x1C */ cXyz mEnd;
    /* 0x28 */ GXColor mColor;
    /* 0x2C */ u8 mClipZ;
    /* 0x2D */ u8 mLineWidth;
};

class mDoExt_pointPacket : public J3DPacket {
public:
    mDoExt_pointPacket(cXyz& i_position, const GXColor& i_color, u8 i_clipZ, u8 i_lineWidth);

    virtual void draw();
    virtual ~mDoExt_pointPacket() {}

    /* 0x10 */ cXyz mPosition;
    /* 0x1C */ GXColor mColor;
    /* 0x20 */ u8 mClipZ;
    /* 0x21 */ u8 mLineWidth;
};

inline void mDoExt_bckAnmRemove(J3DModelData* i_modelData) {
    i_modelData->getJointNodePointer(0)->setMtxCalc(NULL);
}

JKRSolidHeap* mDoExt_createSolidHeapFromGame(u32 i_size, u32 i_alignment);
void mDoExt_destroySolidHeap(JKRSolidHeap* i_heap);
u32 mDoExt_adjustSolidHeap(JKRSolidHeap* i_heap);
void mDoExt_restoreCurrentHeap();
JKRExpHeap* mDoExt_getGameHeap();
void mDoExt_destroySolidHeap(JKRSolidHeap* i_heap);
JKRHeap* mDoExt_setCurrentHeap(JKRHeap* i_heap);
JKRExpHeap* mDoExt_getArchiveHeap();
JKRExpHeap* mDoExt_getArchiveHeapPtr();
void mDoExt_destroyExpHeap(JKRExpHeap* i_heap);
JKRAssertHeap* mDoExt_getAssertHeap();
JKRExpHeap* mDoExt_getCommandHeap();
JKRExpHeap* mDoExt_getZeldaHeap();
JKRExpHeap* mDoExt_getJ2dHeap();
JKRExpHeap* mDoExt_getHostIOHeap();
JKRExpHeap* mDoExt_getDbPrintHeap();
JKRExpHeap* mDoExt_createDbPrintHeap(u32 heapSize, JKRHeap* i_heap);
JKRAssertHeap* mDoExt_createAssertHeap(JKRHeap* i_heap);
JKRExpHeap* mDoExt_createCommandHeap(u32 heapSize, JKRHeap* i_heap);
JKRExpHeap* mDoExt_createArchiveHeap(u32 heapSize, JKRHeap* i_heap);
JKRExpHeap* mDoExt_createJ2dHeap(u32 heapSize, JKRHeap* i_heap);
JKRExpHeap* mDoExt_createZeldaHeap(u32 heapSize, JKRHeap* i_heap);
JKRExpHeap* mDoExt_createGameHeap(u32 heapSize, JKRHeap* i_heap);
JKRSolidHeap* mDoExt_createSolidHeapToCurrent(JKRHeap** o_heap, u32 i_size, JKRHeap* i_parent,
                                              u32 i_alignment);
JKRSolidHeap* mDoExt_createSolidHeapToCurrent(u32 i_size, JKRHeap* i_parent, u32 i_alignment);
JKRSolidHeap* mDoExt_createSolidHeapFromGameToCurrent(u32 i_size, u32 i_alignment);
JKRSolidHeap* mDoExt_createSolidHeapFromGameToCurrent(JKRHeap** o_heap, u32 i_size,
                                                      u32 i_alignment);
JKRSolidHeap* mDoExt_createSolidHeapFromSystem(u32 i_size, u32 i_alignment);
u32 mDoExt_adjustSolidHeapToSystem(JKRSolidHeap* i_heap);
JKRHeap* mDoExt_getCurrentHeap();
void mDoExt_removeMesgFont();

// TODO: make a better name. this flag setup is used for a majority of models which makes it seem like it was a macro
// maybe a macro for the mDoExt_J3DModel__create call itself?
#define BMD_DEFAULT_DIFF_FLAGS J3D_DIFF_FLAG(FALSE, FALSE, TRUE, 8, 0, FALSE, 0, 0, TRUE, FALSE, FALSE, FALSE, TRUE, FALSE)

void mDoExt_modelUpdate(J3DModel* i_model);
void mDoExt_modelUpdateDL(J3DModel* i_model);
J3DModel* mDoExt_J3DModel__create(J3DModelData* i_modelData, u32 i_modelFlag,
                                  u32 i_differedDlistFlag);

extern u32 aram_cache_size;
void mDoExt_setAraCacheSize(u32 size);
inline u32 mDoExt_getAraCacheSize() {
    return aram_cache_size;
}

int mDoExt_resIDToIndex(JKRArchive* p_archive, u16 id);
void mDoExt_modelEntryDL(J3DModel* i_model);
void mDoExt_brkAnmRemove(J3DModelData* i_modelData);
void mDoExt_setupStageTexture(J3DModelData* i_modelData);
OSThread* mDoExt_GetCurrentRunningThread();
void mDoExt_setupShareTexture(J3DModelData* i_modelData, J3DModelData* i_shareModelData);
void mDoExt_btkAnmRemove(J3DModelData* i_modelData);
void mDoExt_modelTexturePatch(J3DModelData* i_modelData);

#if VERSION == VERSION_SHIELD_DEBUG
s32 mDoExt_getSafeZeldaHeapSize();
void mDoExt_addSafeZeldaHeapSize(s32);
JKRHeap* mDoExt_createHostIOHeap(u32, JKRHeap*);
#endif

struct JUTFont;
JUTFont* mDoExt_getMesgFont();
JUTFont* mDoExt_getSubFont();
JUTFont* mDoExt_getRubyFont();
void mDoExt_removeSubFont();

extern JKRExpHeap* zeldaHeap;
extern JKRExpHeap* gameHeap;
extern JKRExpHeap* archiveHeap;
extern JKRExpHeap* commandHeap;

#endif /* M_DO_M_DO_EXT_H */
