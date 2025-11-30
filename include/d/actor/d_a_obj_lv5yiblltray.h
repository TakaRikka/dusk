#ifndef D_A_OBJ_LV5YIBLLTRAY_H
#define D_A_OBJ_LV5YIBLLTRAY_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daObjYIblltray_c
 * @brief Snowpeak Ruins Cannonball Transporter
 *
 * @details
 *
 */
class daObjYIblltray_c : public dBgS_MoveBgActor, public request_of_phase_process_class {
public:
    enum daObjYIblltray_MODE {
        MODE_FRONT_WAIT,
        MODE_FRONT_RIDE,
        MODE_INIT_ROT,
        MODE_SEND_BALL,
        MODE_BACK_RIDE,
    };

    int create1st();
    void setMtx();
    void rideActor(fopAc_ac_c*);
    void sendBall(fpc_ProcID);
    void onAttention();
    void offAttention();
    void initRotTo();

    virtual int CreateHeap();
    virtual int Create();
    virtual int Execute(Mtx**);
    virtual int Draw();
    virtual int Delete();

    virtual ~daObjYIblltray_c() {}

    int getType() { return fopAcM_GetParamBit(this, 0, 1); }
    int getSwNo() { return fopAcM_GetParamBit(this, 1, 8); }
    int getNoSameRoom() { return fopAcM_GetParamBit(this, 9, 1); }

    cXyz& getHandlePos() { return mHandlePos; }
    void onHang() { mIsHang = true; }

    /* 0x5A8 */ cXyz mInBallPos;
    /* 0x5B4 */ cXyz mBackPos;
    /* 0x5C0 */ Mtx field_0x5c0;
    /* 0x5F0 */ Mtx mTrayMtx;
    /* 0x620 */ J3DModel* mpTrayModel;
    /* 0x624 */ s16 mRotAmount;
    /* 0x628 */ cXyz mBasePos;
    /* 0x634 */ fpc_ProcID mBallID;
    /* 0x638 */ fpc_ProcID mTrayID;
    /* 0x63C */ f32 field_0x63c;
    /* 0x640 */ int mMode;
    /* 0x644 */ dCcD_Cyl mCcCyl[3];
    /* 0x9F8 */ dCcD_Stts mCcStts;
    /* 0xA34 */ J3DModel* mpHandleModel;
    /* 0xA38 */ mDoExt_bckAnm mBck;
    /* 0xA54 */ Mtx mHandleMtx;
    /* 0xA84 */ u8 mIsHang;
    /* 0xA88 */ cXyz mHandlePos;
};

STATIC_ASSERT(sizeof(daObjYIblltray_c) == 0xa94);


#endif /* D_A_OBJ_LV5YIBLLTRAY_H */
