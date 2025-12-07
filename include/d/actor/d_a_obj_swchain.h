#ifndef D_A_OBJ_SWCHAIN_H
#define D_A_OBJ_SWCHAIN_H

#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_model.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjSwChain_c
 * @brief Chain Switch
 *
 * @details
 *
*/

class daObjSwChain_c : public fopAc_ac_c {
public:
    struct chain_s {
        /* 0x00 */ dMdl_obj_c mModel;
        /* 0x34 */ cXyz field_0x34;
        /* 0x40 */ csXyz field_0x40;
        /* 0x48 */ f32 field_0x48;
        /* 0x4C */ u8 field_0x4c;
    };

    int Create();
    int CreateHeap();
    int create1st();
    int execute();
    void getChainBasePos(cXyz*);
    void chain_control();
    void chain_control2();
    void initChainMtx();
    void setChainMtx();
    void chain_count_control();
    void setTension();
    int getTopChainNo();
    BOOL checkPlayerPull();
    int draw();
    int _delete();

    u8 getSwbit() { return fopAcM_GetParamBit(this, 0, 8); }
    u8 getSwbit2() { return (mHookshotLength & 0xff00) >> 8; }
    u8 getChainNum() { return fopAcM_GetParamBit(this, 8, 8); }
    u8 getOutNum() { return fopAcM_GetParamBit(this, 16, 8); }
    u8 getChainID() { return fopAcM_GetParamBit(this, 28, 4); }
    u8 getHookShotLength() { return mHookshotLength; }
    int getCurrentChainNum() { return mOutNum; }
    f32 getCurrentChainLength() {
        chain_s* chain_p = &mChains[mChainNum - mOutNum + 1];
        return (getCurrentChainNum() - 1) * 35.0f +
               mChains[0].field_0x34.abs(chain_p->field_0x34);
    }
    BOOL checkDispEmphasis() { return fopAcM_isSwitch(this, getSwbit()) == FALSE; }
    bool checkCarry() { return mCarry == 1; }
    void setRatioForChandelier(f32 ratio) { mRatio = ratio; }

    BOOL checkTight() {
        if (mCarry != 0 && (home.roomNo == 4 || home.roomNo == 6) && mOutNum >= field_0xa69 &&
            field_0xa9c == 0)
        {
            daPy_py_c* player = daPy_getPlayerActorClass();
            chain_s* swChain = &mChains[mChainNum] - mOutNum + 1;
            cXyz cStack_54;
            cXyz cStack_60;
            cStack_54 = swChain->field_0x34;
            cStack_60 = mChains[mChainNum].field_0x34;
            f32 dVar10 = cStack_60.abs(cStack_54);
            f32 dVar13 = (field_0xa67 - 2) * 35.0f;
            cXyz local_6c;
            cXyz cStack_78 = mChains[0].field_0x34 - mChains[mChainNum].field_0x34;
            local_6c.x = player->getSpeedF() * cM_ssin(player->current.angle.y);
            local_6c.z = player->getSpeedF() * cM_scos(player->current.angle.y);
            if (cStack_78.inprodXZ(local_6c) < 0.0f && dVar10 > dVar13 - 35.0f) {
                return TRUE;
            }
        }
        return FALSE;
    }

private:
    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ J3DModel* mpModel;
    /* 0x574 */ J3DModelData* mChainModelData;
    /* 0x578 */ dMdl_c mModel;
    /* 0x598 */ dCcD_Stts mStts;
    /* 0x5D4 */ dCcD_Sph mSph1;
    /* 0x70C */ dCcD_Sph mSph2;
    /* 0x844 */ dBgS_ObjAcch mBgc;
    /* 0xA1C */ dBgS_AcchCir mAcchCir;
    /* 0xA5C */ u16 mHookshotLength;
    /* 0xA5E */ u16 field_0xa5e;
    /* 0xA60 */ u8 field_0xa60;
    /* 0xA60 */ u8 field_0xa61;
    /* 0xA62 */ u8 mCarry;
    /* 0xA63 */ u8 mChainID;
    /* 0xA64 */ u8 mOutNum;
    /* 0xA65 */ u8 mChainNum;
    /* 0xA66 */ u8 mHookShotLength;
    /* 0xA67 */ u8 field_0xa67;
    /* 0xA68 */ u8 field_0xa68;
    /* 0xA69 */ u8 field_0xa69;
    /* 0xA6A */ u8 field_0xa6a;
    /* 0xA6A */ u8 field_0xa6b;
    /* 0xA6C */ u8 field_0xa6c;
    /* 0xA6D */ u8 field_0xa6d;
    /* 0xA6E */ s16 field_0xa6e;
    /* 0xA70 */ s16 field_0xa70;
    /* 0xA74 */ chain_s* mChains;
    /* 0xA78 */ cXyz field_0xa78;
    /* 0xA84 */ u8 field_0xa84[0xa90 - 0xa84];
    /* 0xA90 */ f32 pow;
    /* 0xA94 */ f32 field_0xa94;
    /* 0xA98 */ f32 mRatio;
    /* 0xA9C */ u8 field_0xa9c;
    /* 0xA9D */ u8 field_0xa9d;
    /* 0xA9E */ u8 field_0xa9e;
#if DEBUG
    /* 0xAA0 */ cXyz field_0xac0;
#endif
};

STATIC_ASSERT(sizeof(daObjSwChain_c) == 0xaa0);

#endif /* D_A_OBJ_SWCHAIN_H */
