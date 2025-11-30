#ifndef D_A_CROD_H
#define D_A_CROD_H

#include "d/actor/d_a_player.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-items
 * @class daCrod_c
 * @brief Dominion Rod
 *
 * @details Temple of Time dungeon item.
 *
 */
class daCrod_c : public fopAc_ac_c {
public:
    void setControllActorData() {
        mControllActorKeep.setActor();
        mCameraActorKeep.setActor();
    }

    int createHeap();
    int create();
    ~daCrod_c();
    void setRoomInfo();
    void setMatrix();
    void posMove();
    void setBckAnm(u16);
    void setReturn();
    void setLightPower();
    int execute();
    int draw();

    static fopAc_ac_c* makeIronBallDummy(fopAc_ac_c* p_actor) {
        return (fopAc_ac_c*)fopAcM_fastCreate(0x2F4, 6, &p_actor->current.pos,
                                              fopAcM_GetRoomNo(p_actor), NULL, NULL, -1, NULL,
                                              NULL);
    }

    fopAc_ac_c* getControllActor() { return mControllActorKeep.getActor(); }

    fopAc_ac_c* getCameraActor() { return mCameraActorKeep.getActor(); }

    void setThrow() { fopAcM_SetParam(this, 2); }
    void offControll() { mControllActorKeep.clearData(); }

    static f32 getFlyInitY() { return 40.0f; }
    static f32 getFlyInitZ() { return 50.0f; }

private:
    /* 0x568 */ J3DModel* mpBallModel;
    /* 0x56C */ mDoExt_bckAnm mBck;
    /* 0x588 */ J3DAnmTevRegKey* mpBallBrk;
    /* 0x58C */ J3DAnmTextureSRTKey* mpBallBtk;
    /* 0x590 */ dCcD_Stts mCcStts;
    /* 0x5CC */ dCcD_Cps mAtCps;
    /* 0x710 */ LIGHT_INFLUENCE mLight;
    /* 0x730 */ u8 field_0x730;
    /* 0x731 */ u8 field_0x731;
    /* 0x732 */ u8 field_0x732;
    /* 0x734 */ cXyz field_0x734;
    /* 0x740 */ daPy_actorKeep_c mControllActorKeep;
    /* 0x748 */ daPy_actorKeep_c mCameraActorKeep;
};  // Size: 0x750

#endif /* D_A_CROD_H */
