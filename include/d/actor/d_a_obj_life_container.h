#ifndef D_A_OBJ_LIFE_CONTAINER_H
#define D_A_OBJ_LIFE_CONTAINER_H

#include "d/actor/d_a_itembase.h"
#include "d/d_particle.h"

/**
 * @ingroup actors-objects
 * @class daObjLife_c
 * @brief Heart Piece
 *
 * @details
 *
 */
class daObjLife_c : public daItemBase_c {
public:
    enum Status_e {
        STATUS_WAIT_e,
        STATUS_ORDER_GET_DEMO_e,
        STATUS_GET_DEMO_e,
        STATUS_SW_ON_WAIT_e,
        STATUS_BOOMERANG_CARRY_e,
        STATUS_WAIT_2_e,
    };

    void initBaseMtx();
    void setBaseMtx();
    int Create();
    void setEffect();
    void endEffect00();
    void endEffect02();
    int create();
    void bg_check();
    int actionWaitInit();
    int actionWait();
    int initActionOrderGetDemo();
    int actionOrderGetDemo();
    int actionGetDemo();
    int actionSwOnWait();
    int actionInitBoomerangCarry();
    int actionBoomerangCarry();
    int actionInitWait2();
    int actionWait2();
    void calcScale();
    int execute();
    int draw();
    int _delete();

    virtual int __CreateHeap();
    virtual void setListStart();

    bool chkStatus(u8 i_status) { return mStatus == i_status; }
    void setStatus(u8 i_status) { mStatus = i_status; }

    u8 getSaveBitNo() { return fopAcM_GetParamBit(this, 8, 8); }
    u8 getItemNo() { return fopAcM_GetParamBit(this, 0, 8); }
    int getSwNo() { return 0xFF; }

    void startCtrl() {
        actionInitWait2();
    }

    void setPos(cXyz new_pos) {
        current.pos = new_pos;
    }

private:
    /* 0x92C */ fpc_ProcID mItemId;
    /* 0x930 */ int mCounter;
    /* 0x934 */ u8 mStatus;
    /* 0x935 */ u8 field_0x935;
    /* 0x936 */ u8 mGndLandCount;
    /* 0x937 */ u8 mIsPrmsInit;
    /* 0x938 */ s16 field_0x938;
    /* 0x93A */ s16 field_0x93a;
    /* 0x93C */ cXyz mPrevSpeed;
    /* 0x948 */ u16 mLv5Counter;
    /* 0x94C */ f32 field_0x94c;
    /* 0x950 */ f32 field_0x950;
    /* 0x954 */ f32 field_0x954;
    /* 0x958 */ u8 field_0x958[0x95E - 0x958];
    /* 0x95E */ s16 field_0x95e;
    /* 0x960 */ u8 field_0x960[0x962 - 0x960];
    /* 0x962 */ s16 mRotateSpeed;
    /* 0x964 */ u8 field_0x964[0x974 - 0x964];
    /* 0x974 */ dPa_followEcallBack mEffect0;
    /* 0x988 */ dPa_followEcallBack mEffect1;
    /* 0x99C */ dPa_followEcallBack mEffect2;
    /* 0x9B0 */ Z2SoundObjSimple mSound;
    /* 0x9D0 */ u8 mIsHookCarry;
};

STATIC_ASSERT(sizeof(daObjLife_c) == 0x9d4);


#endif /* D_A_OBJ_LIFE_CONTAINER_H */
