#ifndef D_A_OBJ_SMALLKEY_H
#define D_A_OBJ_SMALLKEY_H

#include "f_op/f_op_actor_mng.h"
#include "d/actor/d_a_itembase.h"
#include "d/d_particle.h"

/**
 * @ingroup actors-objects
 * @class daKey_c
 * @brief Small Key
 *
 * @details
 *
 */
class daKey_c : public daItemBase_c {
private:
    void seStartTwinkle_private(u32);

public:
    enum daKey_Status_e {
        STATUS_INIT_e,
        STATUS_PARENT_WAIT_e,
        STATUS_WAIT_e,
        STATUS_ORDER_GET_DEMO_e,
        STATUS_GET_DEMO_e,
        STATUS_SW_ON_WAIT_e,
        STATUS_BOOMERANG_CARRY_e,
        STATUS_E_GB_e,
    };

    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int __CreateHeap();
    int create();
    void bg_check();
    int actionInitInit();
    int actionInit();
    int actionParentWaitInit();
    int actionParentWait();
    int actionWaitInit();
    int actionWait();
    int initActionOrderGetDemo();
    int actionOrderGetDemo();
    int actionGetDemo();
    int actionInitSwOnWait();
    int actionSwOnWait();
    int actionInitBoomerangCarry();
    int actionBoomerangCarry();
    void effectSet();
    void effectStop();
    void effectCtrl();
    int actionInitE_GB();
    int actionE_GB();
    int execute();
    int draw();
    int _delete();

    int getSwNo() { return fopAcM_GetParamBit(this, 16, 8); }
    int getSaveBitNo() { return fopAcM_GetParamBit(this, 8, 8); }

    u16 getSwOnWaitTimer() { return field_0x93a & 0x3F; }

    void setStatus(u8 i_status) { mStatus = i_status; }
    bool chkStatus(u8 i_status) { return mStatus == i_status; }

    void setPos(cXyz i_pos) {
        current.pos = i_pos;
    }

    void startMove(f32 i_speedF, f32 i_speedY) {
        speedF = i_speedF;
        speed.y = i_speedY;
        gravity = -6.0f;
        actionWaitInit();
    }

    void seStartTwinkle(u32 i_soundId, cXyz* param_1) {
        field_0x978 = *param_1;
        mDoAud_seStartLevel(i_soundId, &field_0x978, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        field_0x984 = 1;
    }

    /* 0x92C */ fpc_ProcID mItemId;
    /* 0x930 */ int field_0x930;
    /* 0x934 */ u8 mStatus;
    /* 0x935 */ u8 mSwOnTimer;
    /* 0x936 */ u8 field_0x936;
    /* 0x937 */ u8 mIsPrmInit;
    /* 0x938 */ s16 field_0x938;
    /* 0x93A */ u16 field_0x93a;
    /* 0x93C */ dPa_followEcallBack mEffect[2];
    /* 0x964 */ csXyz mEffRot;
    /* 0x96C */ cXyz mPrevSpeed;
    /* 0x978 */ cXyz field_0x978;
    /* 0x984 */ u8 field_0x984;
};

STATIC_ASSERT(sizeof(daKey_c) == 0x988);


#endif /* D_A_OBJ_SMALLKEY_H */
