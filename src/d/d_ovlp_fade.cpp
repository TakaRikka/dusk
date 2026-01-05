/**
 * d_ovlp_fade.cpp
 *
 */

#include "d/dolzel.h" // IWYU pragma: keep

#include "d/d_ovlp_fade.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_overlap_mng.h"
#include "m_Do/m_Do_audio.h"
#include "m_Do/m_Do_graphic.h"

class overlap1_class : public overlap_task_class {
public:
    /* 0xCC */ int field_0xcc;
    /* 0xCC */ int field_0xd0;
    /* 0xCC */ int field_0xd4;
};

static int dOvlpFd_Draw(overlap1_class* i_this) {
    return 1;
}

static void dOvlpFd_startFadeIn(int param_0) {
    JUTFader* fader = JFWDisplay::getManager()->getFader();
    JUT_ASSERT(0, fader != NULL);

    fader->setStatus(JUTFader::UNKSTATUS_0, 0);
    fader->setStatus(JUTFader::UNKSTATUS_0, -1);
    fader->startFadeIn(param_0);
}

static int dOvlpFd_FadeOut(overlap1_class* i_this) {
    int var_r31 = i_this->field_0xd4;

    if (i_this->field_0xcc == 0) {
        mDoAud_setFadeInStart(0);

        if (fpcM_GetProfName(i_this) == PROC_OVERLAP11) {
            i_this->field_0xcc = 10;
        } else if (fpcM_GetProfName(i_this) != PROC_OVERLAP6) {
            dOvlpFd_startFadeIn(var_r31);
            i_this->field_0xcc = var_r31;
        } else {
            dOvlpFd_startFadeIn(0);
            i_this->field_0xcc = 1;
        }
    }

    if (fpcM_GetProfName(i_this) == PROC_OVERLAP11 && i_this->field_0xcc == 7) {
        dOvlpFd_startFadeIn(7);
    }

    fopOvlpM_SceneIsStart();

    if (--i_this->field_0xcc == 0) {
        fopOvlpM_Done(i_this);
        i_this->field_0xcc++;
    }

    return 1;
}

static int (*dOvlpFd_execute_f)(overlap1_class*);

static int dOvlpFd_Wait(overlap1_class* i_this) {
    if (fopOvlpM_IsOutReq(i_this)) {
        dOvlpFd_execute_f = dOvlpFd_FadeOut;
    }

    return 1;
}

static int dOvlpFd_FadeIn(overlap1_class* i_this) {
    int var_r30 = 30;

    if (i_this->field_0xd0 == 0) {
        if (fpcM_GetProfName(i_this) == PROC_OVERLAP0 || fpcM_GetProfName(i_this) == PROC_OVERLAP7)
        {
            if (dComIfGp_isEnableNextStage() == 1) {
                var_r30 = dComIfGp_getNextStageWipeSpeed();
            }

            if (!JFWDisplay::getManager()->startFadeOut(var_r30)) {
                return 1;
            }

            i_this->field_0xd0 = var_r30;
        } else if (fpcM_GetProfName(i_this) == PROC_OVERLAP10 ||
                   fpcM_GetProfName(i_this) == PROC_OVERLAP11)
        {
            i_this->field_0xd0 = 1;
        } else {
            i_this->field_0xd0 = 30;
        }

        i_this->field_0xd4 = var_r30;
        mDoAud_setFadeOutStart(0);
    }

    if (--i_this->field_0xd0 == 0) {
        dOvlpFd_execute_f = dOvlpFd_Wait;
        mDoGph_gInf_c::startFadeOut(0);
        fopOvlpM_Done(i_this);
    }

    return 1;
}

static int dOvlpFd_Execute(overlap1_class* i_this) {
    dOvlpFd_execute_f(i_this);
    return 1;
}

static int dOvlpFd_IsDelete(overlap1_class* i_this) {
    return 1;
}

static int dOvlpFd_Delete(overlap1_class* i_this) {
    return 1;
}

static int dOvlpFd_Create(void* i_this) {
    dOvlpFd_execute_f = dOvlpFd_FadeIn;
    return cPhs_COMPLEATE_e;
}

static leafdraw_method_class l_dOvlpFd_Method = {
    (process_method_func)dOvlpFd_Create,  (process_method_func)dOvlpFd_Delete,
    (process_method_func)dOvlpFd_Execute, (process_method_func)dOvlpFd_IsDelete,
    (process_method_func)dOvlpFd_Draw,
};

overlap_process_profile_definition g_profile_OVERLAP0 = {
    fpcLy_ROOT_e,
    0,
    fpcPi_CURRENT_e,
    PROC_OVERLAP0,
    &g_fpcLf_Method.base,
    sizeof(overlap1_class),
    0,
    0,
    &g_fopOvlp_Method,
    773,
    &l_dOvlpFd_Method,
};

overlap_process_profile_definition g_profile_OVERLAP1 = {
    fpcLy_ROOT_e,
    0,
    fpcPi_CURRENT_e,
    PROC_OVERLAP1,
    &g_fpcLf_Method.base,
    sizeof(overlap1_class),
    0,
    0,
    &g_fopOvlp_Method,
    774,
    &l_dOvlpFd_Method,
};

overlap_process_profile_definition g_profile_OVERLAP6 = {
    fpcLy_ROOT_e,
    0,
    fpcPi_CURRENT_e,
    PROC_OVERLAP6,
    &g_fpcLf_Method.base,
    sizeof(overlap1_class),
    0,
    0,
    &g_fopOvlp_Method,
    777,
    &l_dOvlpFd_Method,
};

overlap_process_profile_definition g_profile_OVERLAP7 = {
    fpcLy_ROOT_e,
    0,
    fpcPi_CURRENT_e,
    PROC_OVERLAP7,
    &g_fpcLf_Method.base,
    sizeof(overlap1_class),
    0,
    0,
    &g_fopOvlp_Method,
    778,
    &l_dOvlpFd_Method,
};

overlap_process_profile_definition g_profile_OVERLAP8 = {
    fpcLy_ROOT_e,
    0,
    fpcPi_CURRENT_e,
    PROC_OVERLAP8,
    &g_fpcLf_Method.base,
    sizeof(overlap1_class),
    0,
    0,
    &g_fopOvlp_Method,
    779,
    &l_dOvlpFd_Method,
};

overlap_process_profile_definition g_profile_OVERLAP9 = {
    fpcLy_ROOT_e,
    0,
    fpcPi_CURRENT_e,
    PROC_OVERLAP9,
    &g_fpcLf_Method.base,
    sizeof(overlap1_class),
    0,
    0,
    &g_fopOvlp_Method,
    780,
    &l_dOvlpFd_Method,
};

overlap_process_profile_definition g_profile_OVERLAP10 = {
    fpcLy_ROOT_e,
    0,
    fpcPi_CURRENT_e,
    PROC_OVERLAP10,
    &g_fpcLf_Method.base,
    sizeof(overlap1_class),
    0,
    0,
    &g_fopOvlp_Method,
    781,
    &l_dOvlpFd_Method,
};

overlap_process_profile_definition g_profile_OVERLAP11 = {
    fpcLy_ROOT_e,
    0,
    fpcPi_CURRENT_e,
    PROC_OVERLAP11,
    &g_fpcLf_Method.base,
    sizeof(overlap1_class),
    0,
    0,
    &g_fopOvlp_Method,
    782,
    &l_dOvlpFd_Method,
};
