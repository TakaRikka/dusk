#ifndef D_A_HOZELDA_H
#define D_A_HOZELDA_H

#include "f_op/f_op_actor_mng.h"
#include "d/actor/d_a_player.h"
#include "JSystem/J3DGraphAnimator/J3DMaterialAnm.h"

class daHoZelda_matAnm_c : public J3DMaterialAnm {
public:
    daHoZelda_matAnm_c() {
        init();
    }

    void init();

    void setNowOffsetX(f32 i_offsetX) { mNowOffsetX = i_offsetX; }
    void setNowOffsetY(f32 i_offsetY) { mNowOffsetY = i_offsetY; }
    f32* getNowOffsetXP() { return &mNowOffsetX; }
    f32* getNowOffsetYP() { return &mNowOffsetY; }

    virtual ~daHoZelda_matAnm_c() {}
    virtual void calc(J3DMaterial*) const;

    static u8 getEyeMoveFlg() { return mEyeMoveFlg; }
    static void offEyeMoveFlg() { mEyeMoveFlg = false; }
    static void onEyeMoveFlg() { mEyeMoveFlg = true; }

    static u8 getMorfFrame() { return mMorfFrame; }
    static void setMorfFrame(u8 i_frame) { mMorfFrame = i_frame; }
    static void decMorfFrame() { cLib_calcTimer<u8>(&mMorfFrame); }

    static u8 mEyeMoveFlg;
    static u8 mMorfFrame;

    /* 0x0F4 */ f32 field_0xf4;
    /* 0x0F8 */ f32 field_0xf8;
    /* 0x0FC */ f32 mNowOffsetX;
    /* 0x100 */ f32 mNowOffsetY;
};

struct daHoZelda_hio_c1 {
    /* 0x0 */ s16 bow_search_y_angle;
    /* 0x2 */ s16 bow_start_angle;
    /* 0x4 */ s16 bow_end_angle;
    /* 0x6 */ s16 field_0x6;
    /* 0x8 */ f32 bow_start_distance;
    /* 0xC */ f32 bow_end_distance;
};

struct daHoZelda_hio_c0 {
    static daHoZelda_hio_c1 const m;
};

class daHoZelda_hio_c : public JORReflexible {
public:
    daHoZelda_hio_c() {
#if DEBUG
        mParameters = daHoZelda_hio_c0::m;
#endif
    }
    
    /* 0x4 */ s8 mID;

#if DEBUG
    void genMessage(JORMContext*);
    virtual ~daHoZelda_hio_c() {}
    /* 0x8 */ daHoZelda_hio_c1 mParameters;
#endif
};

/**
 * @ingroup actors-unsorted
 * @class daHoZelda_c
 * @brief Zelda (Horseback)
 *
 * @details
 *
 */
class daHoZelda_c : public fopAc_ac_c {
public:
    int createHeap();
    void modelCallBack(u16);
    int create();
    ~daHoZelda_c();
    int setDoubleAnime(f32, f32, f32, u16, u16, f32);
    int setUpperAnime(u16);
    void resetUpperAnime();
    int setSingleAnime(u16, f32, f32, s16, f32);
    void animePlay();
    void setEyeBtp(u16);
    void setEyeBtk(u16, u8);
    void setNormalFace();
    void setAnm();
    void setBowModel();
    void setMatrix();
    void shootArrow();
    void deleteArrow();
    void setBowBck(u16);
    void setRideOffset();
    void clearEyeMove();
    void setEyeMove(cXyz const*, s16, s16);
    void setNeckAngle();
    void searchBodyAngle();
    int execute();
    int draw();

    MtxP getRightHandMtx() { return mpZeldaModel->getAnmMtx(22); }
    MtxP getRightFingerMtx() { return mpZeldaModel->getAnmMtx(23); }

    u8 checkBowMode() const { return mBowMode; }
    void setDamageInit() { mDamageInit = 1; }
    u8 checkSingleRide() const { return mIsSingleRide; }

    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ J3DModel* mpZeldaModel;
    /* 0x574 */ J3DModel* mpBowModel;
    /* 0x578 */ mDoExt_btpAnm mEyeBtp;
    /* 0x590 */ mDoExt_btkAnm mEyeBtk;
    /* 0x5A8 */ mDoExt_MtxCalcAnmBlendTblOld* field_0x5a8;
    /* 0x5AC */ mDoExt_AnmRatioPack mAnmRatioPack[3];
    /* 0x5C4 */ mDoExt_MtxCalcOldFrame* field_0x5c4;
    /* 0x5C8 */ daPy_frameCtrl_c mFrameCtrl[3];
    /* 0x610 */ daHoZelda_matAnm_c* mpMatAnm[2];
    /* 0x618 */ Z2Creature mSound;
    /* 0x6A8 */ mDoExt_bckAnm mBowBck;
    /* 0x6C4 */ daPy_actorKeep_c mArrowAcKeep;
    /* 0x6CC */ daPy_actorKeep_c mGndAcKeep;
    /* 0x6D4 */ daHoZelda_hio_c* mpHIO;
    /* 0x6D8 */ u8 mBowMode;
    /* 0x6D9 */ u8 mAnmTimer;
    /* 0x6DA */ u8 field_0x6da;
    /* 0x6DB */ u8 mDamageInit;
    /* 0x6DC */ u8 field_0x6dc;
    /* 0x6DD */ u8 field_0x6dd;
    /* 0x6DE */ u8 field_0x6de;
    /* 0x6DF */ u8 mIsSingleRide;
    /* 0x6E0 */ s8 mReverb;
    /* 0x6E1 */ u8 field_0x6E1[0x6E4 - 0x6E1];
    /* 0x6E4 */ u16 field_0x6e4[2];
    /* 0x6E8 */ u16 mUpperAnmID;
    /* 0x6EA */ u16 mBowAnmID;
    /* 0x6EC */ csXyz mNeckAngle;
    /* 0x6F2 */ csXyz mBodyAngle;
    /* 0x6F8 */ cXyz field_0x6f8;
    /* 0x704 */ const Vec* mpRideOffset;
};

#endif /* D_A_HOZELDA_H */
