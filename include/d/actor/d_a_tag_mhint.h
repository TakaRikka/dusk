#ifndef D_A_TAG_MHINT_H
#define D_A_TAG_MHINT_H

#include "d/d_msg_flow.h"
#include "f_op/f_op_actor_mng.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"

class daTagMhint_c : public fopAc_ac_c {
public:
    int create();
    ~daTagMhint_c();
    void eventOrder();
    int execute();

    bool checkNoAttention() const { return field_0x56d == 0xFF; }
    bool checkEventID() const { return mToolEventID != 0xFF; }
    s16 getEventID() const { return mEventID; }
    u8 getToolEventID() const { return mToolEventID; }

    static void createPortalWarpMissTag(u16 param_0, fpc_ProcID i_parentID) {
        csXyz sp18(-1, -1, param_0);

        fopAcM_createChild(PROC_Tag_Mhint, i_parentID, 0x800FFFFF,
                           &daPy_getLinkPlayerActorClass()->current.pos, dComIfGp_roomControl_getStayNo(), &sp18, NULL, -1,
                           NULL);
    }

private:
    /* 0x568 */ u8 field_0x568;
    /* 0x569 */ u8 mSwitch;
    /* 0x56A */ u8 field_0x56a;
    /* 0x56B */ u8 field_0x56b;
    /* 0x56C */ u8 mToolEventID;
    /* 0x56D */ u8 field_0x56d;
    /* 0x56E */ u8 field_0x56e;
    /* 0x56F */ u8 field_0x56f;
    /* 0x570 */ u8 field_0x570;
    /* 0x571 */ u8 field_0x571;
    /* 0x572 */ u8 field_0x572;
    /* 0x574 */ u16 field_0x574;
    /* 0x576 */ s16 mEventID;
    /* 0x578 */ u16 field_0x578;
    /* 0x57A */ u16 field_0x57a;
    /* 0x57C */ u16 field_0x57c;
    /* 0x57E */ u16 field_0x57e;
    /* 0x580 */ dMsgFlow_c mMsgFlow;
    /* 0x5CC */ f32 field_0x5cc;
    /* 0x5D0 */ f32 field_0x5d0;
};

#endif /* D_A_TAG_MHINT_H */
