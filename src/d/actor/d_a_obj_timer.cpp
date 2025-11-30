/**
 * @file d_a_obj_timer.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_timer.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"

int daObjTimer::Act_c::_create() {
    fopAcM_ct(this, Act_c);
    fopAcM_offDraw(this);

    if (fopAcM_isSwitch(this, prm_get_sw2Save())) {
        return cPhs_ERROR_e;
    } else {
        if (fopAcM_isSwitch(this, prm_get_swSave())) {
            mode_count_init();
        } else {
            mode_wait_init();
        }
        return cPhs_COMPLEATE_e;
    }
}

bool daObjTimer::Act_c::_delete() {
    return true;
}

void daObjTimer::Act_c::mode_wait_init() {
    field_0x56c = 0;
    field_0x568 = 0;
}

void daObjTimer::Act_c::mode_wait() {
    if (fopAcM_isSwitch(this, prm_get_swSave())) {
        mode_count_init();
    }
}

void daObjTimer::Act_c::mode_count_init() {
    field_0x56c = prm_get_time() * 15;
    field_0x568 = 1;
}

void daObjTimer::Act_c::mode_count() {
    Z2SoundID soundID;
    
    if (!field_0x570) {
        field_0x56c--;
        if (prm_get_SeStop() && field_0x56c % 30 == 0) {
                if (dComIfGp_event_runCheck() && prm_get_demoStop()) {
                    field_0x56c += 30;
                }

                int iVar1 = field_0x56c / 30;
                if (iVar1 <= 20) {
                    if (iVar1 > 10) {
                        soundID = JA_SE_SYS_EV_TIMER_20;
                    } else if (iVar1 > 5) {
                        soundID = JA_SE_SYS_EV_TIMER_10;
                    } else if (iVar1 > 0) {
                        soundID = JA_SE_SYS_EV_TIMER_5;
                    } else {
                        soundID = JA_SE_SYS_EV_TIMER_0;
                    }

                    mDoAud_seStart(soundID, 0, 0, 0);
                }
        }

        if (field_0x56c > 0) {
            if (fopAcM_isSwitch(this, prm_get_swSave())) {
                return;
            }
        }

        fopAcM_offSwitch(this, prm_get_swSave());
        mode_wait_init();
    }
}

bool daObjTimer::Act_c::_execute() {
    static daObjTimer::Act_c::modeProc const mode_proc[2] = {
        &daObjTimer::Act_c::mode_wait,
        &daObjTimer::Act_c::mode_count,
    };

    if (fopAcM_isSwitch(this, prm_get_sw2Save())) {
        fopAcM_delete(this);
        return true;
    } else {
        (this->*mode_proc[field_0x568])();
        return true;
    }
}

namespace daObjTimer {

namespace {
    /* 804855A4-804855C0 .text                  daObjTimer::@unnamed@d_a_obj_timer_cpp@::Mthd_Create */
    int Mthd_Create(void* i_this) {
        return static_cast<daObjTimer::Act_c*>(i_this)->_create();
    }

    /* 804855C4-804855E4 .text                  daObjTimer::@unnamed@d_a_obj_timer_cpp@::Mthd_Delete */
    int Mthd_Delete(void* i_this) {
        return static_cast<daObjTimer::Act_c*>(i_this)->_delete();
    }

    /* 804855E8-80485608 .text                  daObjTimer::@unnamed@d_a_obj_timer_cpp@::Mthd_Execute */
    int Mthd_Execute(void* i_this) {
        return static_cast<daObjTimer::Act_c*>(i_this)->_execute();
    }

    /* 8048560C-80485610 .text                      daObjTimer::@unnamed@d_a_obj_timer_cpp@::Mthd_Draw */
    int Mthd_Draw(void* i_this) {
        return 1;
    }

    /* 80485614-80485618 .text                  daObjTimer::@unnamed@d_a_obj_timer_cpp@::Mthd_IsDelete */
    int Mthd_IsDelete(void* i_this) {
        return 1;
    }

    /* 80485698-804856B8 -00001 0020+00 1/0 0/0 0/0 .data Mthd_Table__Q210daObjTimer27@unnamed@d_a_obj_timer_cpp@ */
    actor_method_class l_daTimer_Method = {
        (process_method_func)Mthd_Create,
        (process_method_func)Mthd_Delete,
        (process_method_func)Mthd_Execute, 
        (process_method_func)Mthd_IsDelete,
        (process_method_func)Mthd_Draw,
    };
}  // namespace

};  // namespace daObjTimer

extern actor_process_profile_definition g_profile_Obj_Timer = {
  fpcLy_CURRENT_e,          // mLayerID
  10,                       // mListID
  fpcPi_CURRENT_e,          // mListPrio
  PROC_Obj_Timer,           // mProcName
  &g_fpcLf_Method.base,    // sub_method
  sizeof(daObjTimer::Act_c), // mSize
  0,                        // mSizeOther
  0,                        // mParameters
  &g_fopAc_Method.base,     // sub_method
  22,                       // mPriority
  &daObjTimer::l_daTimer_Method,           // sub_method
  0x00040000,               // mStatus
  fopAc_ACTOR_e,            // mActorType
  fopAc_CULLBOX_0_e,        // cullType
};
