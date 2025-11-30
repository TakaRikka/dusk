#ifndef D_A_NPC_SQ_H
#define D_A_NPC_SQ_H

#include "SSystem/SComponent/c_phase.h"
#include "d/d_bg_s_acch.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_msg_flow.h"
#include "f_op/f_op_actor.h"

/**
 * @ingroup actors-npcs
 * @class npc_sq_class
 * @brief Squirrel (Talking, Ordon Village)
 *
 * @details
 *
 */
class npc_sq_class : public fopEn_enemy_c {
public:
    enum Action {
        /* 0 */ ACT_NORMAL,
    };

    /* 0x5AC */ request_of_phase_process_class mPhaseReq;
    /* 0x5B4 */ u8 mParam0;
    /* 0x5B5 */ u8 mParam1;
    /* 0x5B6 */ u8 field_0x5b6[2];
    /* 0x5B8 */ s16 mFlowID;
    /* 0x5BC */ dMsgFlow_c mMsgFlow;
    /* 0x608 */ cXyz field_0x608;
    /* 0x614 */ u8 field_0x614[4];
    /* 0x618 */ f32 mPlayerDist;
    /* 0x61C */ mDoExt_McaMorf* mpMorf;
    /* 0x620 */ mDoExt_btkAnm* mpBtkAnm;
    /* 0x624 */ s8 mBtkFrame;
    /* 0x626 */ s16 mCounter;
    /* 0x628 */ s16 mAction;
    /* 0x62A */ s16 mMode;
    /* 0x62C */ u32 mShadowKey;
    /* 0x630 */ s16 mTimer[4];
    /* 0x638 */ u8 field_0x638[4];
    /* 0x63C */ dBgS_AcchCir mAcchCir;
    /* 0x67C */ dBgS_ObjAcch mAcch;
    /* 0x854 */ dCcD_Stts mCcStts;
    /* 0x890 */ dCcD_Sph mCcSph;
    /* 0x9C8 */ dCcU_AtInfo mAtInfo;
    /* 0x9EC */ s16 mDemoMode;
    /* 0x9EE */ s16 mDemoTimer;
    /* 0x9F0 */ cXyz mCameraEye;
    /* 0x9FC */ cXyz mCameraCenter;
    /* 0xA08 */ cXyz mCameraTargetEye;
    /* 0xA14 */ cXyz mCameraTargetCenter;
    /* 0xA20 */ cXyz mCameraEyeDist;
    /* 0xA2C */ cXyz mCameraCenterDist;
    /* 0xA38 */ f32 mCameraFovY;
    /* 0xA3C */ f32 mCameraSpeed;
    /* 0xA40 */ bool mHIOInit;
};

STATIC_ASSERT(sizeof(npc_sq_class) == 0xA44);

struct daNpc_Sq_HIO_c {
    daNpc_Sq_HIO_c();
    virtual ~daNpc_Sq_HIO_c() {}

    /* 0x4 */ s8 field_0x4;
    /* 0x8 */ f32 mScale;
};

STATIC_ASSERT(sizeof(daNpc_Sq_HIO_c) == 0xC);

#endif /* D_A_NPC_SQ_H */
