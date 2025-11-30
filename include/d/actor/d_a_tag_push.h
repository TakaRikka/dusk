#ifndef D_A_TAG_PUSH_H
#define D_A_TAG_PUSH_H

#include "d/actor/d_a_npc.h"
#include "f_op/f_op_actor_mng.h"

class daTag_Push_c : public fopAc_ac_c {
public:
    static void* srchActor(void*, void*);
    int create();
    int Delete();
    int Execute();
    int Draw();
    int isDelete();
    int chkPointInArea(cXyz);

    u32 getId() {
        u32 id = fopAcM_GetParam(this) >> 0x18;
        if (id != 0xFF) {
            return id;
        }
        return -1;
    }

    u8 getBitSW() { return fopAcM_GetParam(this) & 0xFF; }

    u8 getBitSW2() { return (fopAcM_GetParam(this) >> 8) & 0xFF; }

    u32 getFlowNodeNo() {
        if (home.angle.x != 0xFFFF) {
            return (u16)home.angle.x;
        }
        return -1;
    }

    int getPlayerListNo() {
        u16 rv = (fopAcM_GetParam(this) & 0xff0000) >> 16;
        return (rv == 0xff) ? -1 : rv;
    }

    void pushBackPlayer(int param_1) {
        if (getPlayerListNo() > -1) {
            cXyz cStack_1c;
            csXyz cStack_24;
            if (daNpcT_getPlayerInfoFromPlayerList(getPlayerListNo(), current.roomNo, &cStack_1c,
                                                   &cStack_24) != 0)
            {
                daPy_getPlayerActorClass()->setPlayerPosAndAngle(&cStack_1c, cStack_24.y, 0);
                if (param_1 != 0) {
                    daPy_getPlayerActorClass()->changeDemoMoveAngle(cStack_24.y);
                    dComIfGp_evmng_setGoal(&cStack_1c);
                }
            }
        }
    }

    /* 0x568 */ daNpcT_ActorMngr_c mActorMngr;

    virtual ~daTag_Push_c() {}
};

STATIC_ASSERT(sizeof(daTag_Push_c) == 0x574);

#endif /* D_A_TAG_PUSH_H */
