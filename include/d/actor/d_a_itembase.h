#ifndef D_A_D_A_ITEMBASE_H
#define D_A_D_A_ITEMBASE_H

#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_a_itembase_static.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_ext.h"

struct daItemBase_data {
    /* 0x00 */ f32 mGravity;
    /* 0x04 */ f32 mGroundReflect;
    /* 0x08 */ f32 mLaunchSpeed;
    /* 0x0C */ f32 mScalingTime;
    /* 0x10 */ f32 mSpeedH;
    /* 0x14 */ s16 mFlashCycleTime;
    /* 0x16 */ s16 mWaitTime;
    /* 0x18 */ s16 mDisappearTime;
    /* 0x1A */ s16 mRotateXSpeed;
    /* 0x1C */ s16 mRotateYSpeed;
    /* 0x20 */ f32 mHeartFallSpeed;
    /* 0x24 */ f32 mHeartAmplitude;
    /* 0x28 */ s16 mHeartFallCycleTime;
    /* 0x2A */ s16 mHeartTilt;
    /* 0x2C */ f32 mGetDemoLaunchSpeed;
    /* 0x30 */ f32 mGetDemoGravity;
    /* 0x34 */ s16 mSimpleExistTime;
    /* 0x36 */ s16 mNoGetTime;
};

class daItemBase_c : public fopAc_ac_c {
public:
    u8 getItemNo();
    void hide();
    void show();
    void changeDraw();
    bool chkDraw();
    void dead();
    bool chkDead();
    int CreateItemHeap(char const*, s16, s16, s16, s16, s16, s16, s16);
    int DeleteBase(char const*);
    void setListEnd();
    void animPlay(f32, f32, f32, f32, f32, f32);
    const daItemBase_data& getData();

    virtual int DrawBase();
    virtual void setListStart();
    virtual void settingBeforeDraw();
    virtual void setTevStr();
    virtual void setShadow();
    virtual void animEntry();
    virtual void RotateYBase();
    virtual int clothCreate();
    virtual int __CreateHeap();
    virtual BOOL chkFlag(int);
    virtual s8 getTevFrm();
    virtual s8 getBtpFrm();
    virtual u8 getShadowSize();
    virtual u8 getCollisionH();
    virtual u8 getCollisionR();

    static daItemBase_data const m_data;

    /* 0x56C */ request_of_phase_process_class mPhase;
    /* 0x574 */ J3DModel* mpModel;
    /* 0x578 */ mDoExt_btkAnm* mpBtkAnm;
    /* 0x57C */ mDoExt_bpkAnm* mpBpkAnm;
    /* 0x580 */ mDoExt_brkAnm* mpBrkAnm;
    /* 0x584 */ mDoExt_bckAnm* mpBckAnm;
    /* 0x588 */ mDoExt_btpAnm* mpBtpAnm;
    /* 0x58C */ dBgS_ObjAcch mAcch;
    /* 0x764 */ dBgS_AcchCir mAcchCir;
    /* 0x7A4 */ dCcD_Stts mCcStts;
    /* 0x7E0 */ dCcD_Cyl mCcCyl;
    /* 0x91C */ u32 mShadowKey;
    /* 0x920 */ u32 mItemBitNo;
    /* 0x924 */ int m_timer;
    /* 0x928 */ s16 m_get_timer;
    /* 0x92A */ u8 m_itemNo;
    /* 0x92B */ u8 field_0x92b;
};

#endif /* D_A_D_A_ITEMBASE_H */
