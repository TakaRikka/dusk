/**
 * @file d_a_scene_exit.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_scene_exit.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"
#include "m_Do/m_Do_mtx.h"

int daScex_c::checkWork() {
    if (getArg1() == 0xFF || getArg1() == 0 || getArg1() == 3) {
        if (fopAcM_isSwitch(this, getSwNo())) {
            return 0;
        }
    } else if ((getArg1() == 1 || getArg1() == 2 || getArg1() == 4) && getSwNo() != 0xFF) {
        if (!fopAcM_isSwitch(this, getSwNo())) {
            return 0;
        }
    }

    u16 eventBit = getOffEventBit();
    if (eventBit != 0x0FFF && dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[eventBit])) {
        return 0;
    }

    eventBit = getOnEventBit();
    if (eventBit != 0x0FFF && !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[eventBit])) {
        return 0;
    }

    return 1;
}

inline int daScex_c::create() {
    fopAcM_ct(this, daScex_c);

    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_inverse(mDoMtx_stack_c::get(), mMatrix);
    scale.x *= 75.0f;
    scale.z *= 75.0f;
    scale.y *= 150.0f;

    return cPhs_COMPLEATE_e;
}

static int daScex_Create(fopAc_ac_c* i_this) {
    daScex_c* scx = static_cast<daScex_c*>(i_this);
    return scx->create();
}

static int daScex_Execute(daScex_c* i_this) {
    return i_this->execute();
}

int daScex_c::execute() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz spC;

    if (checkWork()) {
        mDoMtx_multVec(mMatrix, &player->current.pos, &spC);

        if (spC.y >= 0.0f && spC.y <= scale.y && fabsf(spC.x) <= scale.x && fabsf(spC.z) <= scale.z) {
            switch (getArg1()) {
            case 0xFF:
            case 1:
                player->onSceneChangeArea(getArg0(), getPathID(), this);
                break;
            case 2:
            case 0:
                player->onSceneChangeAreaJump(getArg0(), getPathID(), this);
                break;
            case 3:
            case 4:
                player->onSceneChangeAreaJump(getArg0(), getPathID(), this);
                break;
            }
        }
    }

    if (mSceneChangeOK && player->checkSceneChangeAreaStart()) {
        if ((getArg1() == 3 || getArg1() == 4) && field_0x598 == 0) {
            mDoAud_seStart(Z2SE_FORCE_BACK, NULL, 0, 0);
            player->voiceStart(Z2SE_WL_V_FALL_TO_RESTART);
            field_0x598 = 1;
        }

        if (getArg1() == 0xFF || getArg1() == 0 || getArg1() == 3) {
            if (getSwNo() != 0xFF) {
                fopAcM_onSwitch(this, getSwNo());
            }
        }
    }

    return 1;
}

static actor_method_class l_daScex_Method = {
    (process_method_func)daScex_Create,
    NULL,
    (process_method_func)daScex_Execute,
};

actor_process_profile_definition2 g_profile_SCENE_EXIT = {
  fpcLy_CURRENT_e,       // mLayerID
  10,                    // mListID
  fpcPi_CURRENT_e,       // mListPrio
  PROC_SCENE_EXIT,       // mProcName
  &g_fpcLf_Method.base, // sub_method
  0x0000059C,            // mSize
  0,                     // mSizeOther
  0,                     // mParameters
  &g_fopAc_Method.base,  // sub_method
  761,                   // mPriority
  &l_daScex_Method,      // sub_method
  0x00060000,            // mStatus
  fopAc_UNK_GROUP_5_e,   // mActorType
  fopAc_CULLBOX_0_e,     // cullType
  0                      // field_0x30
};
