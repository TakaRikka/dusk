#ifndef D_A_DOOR_PUSH_H
#define D_A_DOOR_PUSH_H

#include "d/actor/d_a_player.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_com_inf_game.h"

/**
 * @ingroup actors-doors
 * @class daDoorPush_c
 * @brief Push Door
 *
 * @details
 *
 */
class daDoorPush_c : public dBgS_MoveBgActor {
public:
    enum act_e {
        WAIT_e,
        OPEN_e,
        SCENE_CHG_e,
    };

    enum action_e {
        ACTION_OPEN_WAIT_e = 1,
        ACTION_ORDER_EVENT_e,
    };

    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create1st();
    int Execute(Mtx**);
    void action();
    void init_modeWait();
    void modeWait();
    void event_proc_call();
    void actionOpenWait();
    void actionOrderEvent();
    void actionEvent();
    int demoProc();
    void rotateInit();
    int rotate();
    void setGoal();
    int Draw();
    int Delete();

    void setAction(u8 i_action) { mAction = i_action; }
    u32 getSceneNo() { return fopAcM_GetParamBit(this, 4, 6); }

// private:
    /* 0x5A0 */ request_of_phase_process_class mPhaseReq;
    /* 0x5A8 */ J3DModel* mpModel[2];
    /* 0x5B0 */ dBgW* mpDoorBgW;
    /* 0x5B4 */ Mtx mDoorMtx;
    /* 0x5E4 */ Mtx mActorMtx;
    /* 0x614 */ cXyz mXyz[2];
    /* 0x62C */ u8 field_0x62c[2];
    /* 0x62E */ u8 field_0x62e[2];
    /* 0x630 */ s16 field_0x630[2];
    /* 0x634 */ s16 field_0x634[2];
    /* 0x638 */ u8 field_0x638;
    /* 0x639 */ u8 mIndex;
    /* 0x63A */ s16 field_0x63a[2];
    /* 0x63E */ u8 field_0x63e[2];
    /* 0x640 */ u8 mAction;
    /* 0x641 */ u8 field_0x641;
    /* 0x644 */ s32 field_0x644;
    /* 0x648 */ u8 field_0x648;
    /* 0x649 */ u8 field_0x649;
    /* 0x64A */ s16 field_0x64a;
    /* 0x64C */ s16 field_0x64c;
    /* 0x64E */ s16 field_0x64e[2];
    /* 0x654 */ cXyz mPosition;
};

STATIC_ASSERT(sizeof(daDoorPush_c) == 0x660);

#endif /* D_A_DOOR_PUSH_H */
