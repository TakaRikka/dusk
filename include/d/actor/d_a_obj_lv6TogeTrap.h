#ifndef D_A_OBJ_LV6TOGETRAP_H
#define D_A_OBJ_LV6TOGETRAP_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daLv6TogeTrap_c
 * @brief Temple of Time Spiked Trap
 *
 * @details
 *
 */
class daLv6TogeTrap_c : public dBgS_MoveBgActor {
public:
    void setBaseMtx();
    int create();
    static void* searchSekizoAct(void*, void*);
    void moveLift();
    void executeType1();
    void init_modeBound();
    void modeBound();
    void init_modeBoundWait();
    void modeBoundWait();
    void init_modeAcc();
    void modeAcc();
    void init_modeMove();
    void modeMove();
    void init_modeBrk();
    void modeBrk();
    void init_modeMove2();
    void modeMove2();
    void init_modeWaitInit();
    void modeWaitInit();
    void init_modeWait();
    void modeWait();
    void executeType2();
    void init_modeWaitType2();
    void modeWaitType2();
    void init_modeMoveInitType2();
    void modeMoveInitType2();
    void init_modeAccType2();
    void modeAccType2();
    void init_modeWaitAttackType2();
    void modeWaitAttackType2();
    void init_modeRotate();
    void modeRotate();
    void init_modeBreak();
    void modeBreak();
    void setNextPoint();

    virtual int CreateHeap();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    int getSpeed() { return fopAcM_GetParamBit(this, 8, 4); }
    int getType() { return fopAcM_GetParamBit(this, 12, 4); }
    int getHankei() { return fopAcM_GetParamBit(this, 16, 4); }
    int getModelType() { return fopAcM_GetParamBit(this, 20, 4); }
    int getHankei2() { return fopAcM_GetParamBit(this, 24, 8); }

    static const dCcD_SrcGObjInf mCcDObjInfo;
    static const dCcD_SrcGObjInf mCcDObjInfo2;
    static const f32 mSpeed[16];
    static dCcD_SrcSph mCcDSph;
    static dCcD_SrcCyl mCcDCyl;

    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ u8 mPathNo;
    /* 0x5AD */ s8 mRotateDirection;
    /* 0x5AE */ s16 field_0x5ae;
    /* 0x5B0 */ cXyz field_0x5b0;
    /* 0x5BC */ cXyz field_0x5bc;
    /* 0x5C8 */ cXyz field_0x5c8;
    /* 0x5D4 */ f32 mMoveSpeed;
    /* 0x5D8 */ u8 mMode;
    /* 0x5D9 */ u8 field_0x5d9;
    /* 0x5DA */ u8 mType;
    /* 0x5DB */ u8 field_0x5db;
    /* 0x5DC */ cM3dGLin mLine;
    /* 0x5F8 */ u8 mIsPathClosed;
    /* 0x5F9 */ u8 field_0x5f9;
    /* 0x5FC */ f32 mRadius;
    /* 0x600 */ s16 field_0x600;
    /* 0x604 */ f32 field_0x604;
    /* 0x608 */ u8 field_0x608;
    /* 0x609 */ u8 field_0x609;
    /* 0x60A */ u8 field_0x60a;
    /* 0x60B */ u8 field_0x60b;
    /* 0x60C */ u8 mModelType;
    /* 0x610 */ f32 field_0x610;
    /* 0x614 */ dCcD_Stts mCcStts;
    /* 0x650 */ dCcD_Sph mCcSph;
    /* 0x788 */ dCcD_Cyl mCcCyl;
    /* 0x8C4 */ cXyz field_0x8c4;
    /* 0x8D0 */ u8 field_0x8d0;
    /* 0x8D4 */ Z2SoundObjSimple mSound;
};

STATIC_ASSERT(sizeof(daLv6TogeTrap_c) == 0x8f4);

#endif /* D_A_OBJ_LV6TOGETRAP_H */
