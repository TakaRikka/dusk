#ifndef D_A_TAG_LANTERN_H
#define D_A_TAG_LANTERN_H

#include "d/d_com_inf_game.h"
#include "d/d_msg_flow.h"

class daTag_Lantern_c : public fopAc_ac_c {
public:
    int create();
    int Delete();
    int Execute();
    int Draw();
    void initialize();

    BOOL chkClimbDownLadderStatus() {
        f32 var_f31 = fopAcM_searchPlayerDistanceY(this);
        BOOL rt = false;

        if (fopAcM_searchPlayerDistanceXZ(this) < scale.x && 0.0f <= var_f31 && var_f31 < scale.y) {
            rt = dComIfGp_checkItemGet(fpcNm_ITEM_KANTERA, 1) && dComIfGs_getOil() != 0;
            field_0x5f8 = rt == true ? 0 : 1;
        } else {
            rt = true;
        }

        return rt;
    }

    /* 0x568 */ dMsgFlow_c mMsgFlow;
    /* 0x5B4 */ cBgS_GndChk mGndChk;
    /* 0x5F0 */ u32 field_0x5f0;
    /* 0x5F4 */ f32 mGroundCross;
    /* 0x5F8 */ s32 field_0x5f8;
    /* 0x5FC */ u32 field_0x5fc;
    /* 0x600 */ int mFlowIndex;
    /* 0x604 */ u8 field_0x604;
    
    virtual ~daTag_Lantern_c();
}; // Size: 0x60C

class daTag_Lantern_Param_c {
public:
    inline virtual ~daTag_Lantern_Param_c() {}
};

#endif /* D_A_TAG_LANTERN_H */
