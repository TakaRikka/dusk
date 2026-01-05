/**
 * d_a_kytag11.cpp
 * Time Control Tag
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_kytag11.h"
#include "d/d_procname.h"
#include "d/d_kankyo_static.h"

static int daKytag11_Draw(kytag11_class* i_this) {
    return 1;
}

static int daKytag11_Execute(kytag11_class* i_this) {
    dScnKy_env_light_c* env_light = dKy_getEnvlight();

    i_this->mChangeTime = false;
    i_this->mChangeTime = true;

    if (i_this->mOnEventID != 0xFFF) {
        if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[i_this->mOnEventID])) {
            i_this->mChangeTime = false;
        }
    } else if (i_this->mOnSwitchNo != 0xFF) {
        if (!dComIfGs_isSwitch(i_this->mOnSwitchNo, dComIfGp_roomControl_getStayNo())) {
            i_this->mChangeTime = false;
        }
    }

    if (i_this->mOffEventID != 0xFFF) {
        if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[i_this->mOffEventID])) {
            i_this->mChangeTime = false;
        }
    } else if (i_this->mOffSwitchNo != 0xFF) {
        if (dComIfGs_isSwitch(i_this->mOffSwitchNo, dComIfGp_roomControl_getStayNo())) {
            i_this->mChangeTime = false;
        }
    }

    if (!i_this->mChangeTime) {
        return 1;
    }

    if (!i_this->mInitTimeChange) {
        if (i_this->mNewTime != 0x1F) {
            env_light->daytime = dComIfGs_getTime();

            f32 new_time = i_this->mNewTime * 15.0f;
            env_light->daytime = new_time;
            dComIfGs_setTime(new_time);
        }

        i_this->mInitTimeChange = true;
    }

    env_light->using_time_control_tag = 1;

    if (i_this->mStopTime != 0x1F && i_this->mStopTime == (u8)dKy_getdaytime_hour()) {
        return 1;
    }

    f32 new_env_time = ((i_this->mEnvTime / 255.0f) * 0.5f);
    f32 current_time = dComIfGs_getTime();

    env_light->daytime = current_time;
    env_light->mDate = dComIfGs_getDate();

    new_env_time = current_time + new_env_time;
    env_light->daytime = new_env_time;

    if ((u32)new_env_time >= 360.0f) {
        env_light->daytime = 0.0f;
        env_light->mDate++;
        dKankyo_DayProc();
    }

    dComIfGs_setTime(env_light->daytime);
    dComIfGs_setDate(env_light->mDate);

    return 1;
}

static int daKytag11_IsDelete(kytag11_class* i_this) {
    return 1;
}

static int daKytag11_Delete(kytag11_class* i_this) {
    return 1;
}

static int daKytag11_Create(fopAc_ac_c* i_this) {
    kytag11_class* a_this = (kytag11_class*)i_this;
    fopAcM_ct(i_this, kytag11_class);

    a_this->mNewTime = fopAcM_GetParam(i_this) & 0x1F;
    a_this->mStopTime = (fopAcM_GetParam(i_this) >> 5) & 0x1F;
    a_this->mEnvTime = (fopAcM_GetParam(i_this) >> 10) & 0xFF;
    a_this->mOnEventID = i_this->current.angle.x & 0xFFF;
    a_this->mOffEventID = i_this->current.angle.y & 0xFFF;
    a_this->mOnSwitchNo = i_this->current.angle.z & 0xFF;
    a_this->mOffSwitchNo = (i_this->current.angle.z >> 8) & 0xFF;
    a_this->mChangeTime = false;
    a_this->mInitTimeChange = false;

    return cPhs_COMPLEATE_e;
}

static actor_method_class l_daKytag11_Method = {
    (process_method_func)daKytag11_Create,  (process_method_func)daKytag11_Delete,
    (process_method_func)daKytag11_Execute, (process_method_func)daKytag11_IsDelete,
    (process_method_func)daKytag11_Draw,
};

actor_process_profile_definition g_profile_KYTAG11 = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_KYTAG11,
    &g_fpcLf_Method.base,
    sizeof(kytag11_class),
    0,
    0,
    &g_fopAc_Method.base,
    105,
    &l_daKytag11_Method,
    0x44000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
