#ifndef D_A_TAG_KMSG_H
#define D_A_TAG_KMSG_H

#include "dolphin/types.h"
#include "f_op/f_op_actor.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_msg_flow.h"
#include "d/d_com_inf_game.h"

struct daTag_KMsg_c : public fopAc_ac_c {
public:
    enum KMsg_Type {
        KMSG_TYPE_0,
        KMSG_TYPE_1,
        KMSG_TYPE_2,
        KMSG_TYPE_3,
        KMSG_TYPE_4,
        KMSG_TYPE_5,
    };

    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ dMsgFlow_c mMsgFlow;
    /* 0x5BC */ u32 mFlowNodeNo;
    /* 0x5C0 */ s16 mEventIdx;
    /* 0x5C2 */ u8 field_0x5c2;
    /* 0x5C3 */ u8 field_0x5c3;
    /* 0x5C4 */ bool field_0x5c4;

    int create();
    int Delete();
    int Execute();
    int Draw();
    int isDelete();
    virtual ~daTag_KMsg_c() {
        if (getType() == KMSG_TYPE_3) {
            dComIfG_resDelete(&mPhase, "Lv6Gate");
        }
    }

    void stabMasterSword() { field_0x5c2 = 1; }

    void pullMasterSword() {
        field_0x5c2 = 0;
        field_0x5c3 = 1;
    }
    
    int getType() {
        int param = fopAcM_GetParam(this) >> 0x18;
        int type;

        if (param == 0xFF) {
            type = 0;
        } else {
            type = param;
        }

        return type;
    }

    u32 getFlowNodeNo() {
        u16 rv = home.angle.z;
        if (rv == 0xffff) {
            return -1;
        }
        return rv;
    }

    u8 getBitSW() {
        return home.angle.x;
    }

    u32 getTalkAngle() {
        u32 talkAngle = (fopAcM_GetParam(this) >> 5) & 0x7;
        if (talkAngle == 7) {
            return 0;
        } else {
            return talkAngle > 6 ? 6 : talkAngle;
        }
    }

    u32 getTalkDis() {
        u32 talkDis = fopAcM_GetParam(this) & 0x1f;
        if (talkDis == 0x1f) {
            return 0;
        } else {
            return talkDis > 0x13 ? 0x13 : talkDis;
        }
    }

    f32 getAttnPosOffset() {
        u32 attnPosOffset = (fopAcM_GetParam(this) & 0xff0000) >> 0x10;
        if (attnPosOffset == 0xff) {
            return G_CM3D_F_INF;
        } else {
            return attnPosOffset;
        }
    }

    u32 getChkType() {
        return home.angle.x & 0x8000;
    }

    f32 getEyePosOffset() {
        u32 eyeOffset = (fopAcM_GetParam(this) & 0xff00) >> 8;
        return (eyeOffset == 0xff) ? 0 : eyeOffset;
    }
};

#endif /* D_A_TAG_KMSG_H */
