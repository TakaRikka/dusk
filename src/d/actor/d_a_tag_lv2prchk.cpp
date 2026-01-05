/**
 * d_a_tag_lv2prchk.cpp
 * Boomerang Switch Puzzle Tag
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_lv2prchk.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

int daTagLv2PrChk_c::Create() {
    mSwbit2 = getSwbit2();
    if (mSwbit2 == 0xFF) {
        return 0;
    }

    if (fopAcM_isSwitch(this, getSwbit())) {
        mAction = ACTION_END_e;
    } else {
        mAction = ACTION_CHECK_e;
    }

    return 1;
}

int daTagLv2PrChk_c::create() {
    fopAcM_ct(this, daTagLv2PrChk_c);

    if (!Create()) {
        return cPhs_ERROR_e;
    }

    return cPhs_COMPLEATE_e;
}

int daTagLv2PrChk_c::execute() {
    action();
    return 1;
}

void daTagLv2PrChk_c::action() {
    typedef void (daTagLv2PrChk_c::*actionFunc)();
    static actionFunc l_func[] = {&daTagLv2PrChk_c::actionCheck, &daTagLv2PrChk_c::actionEnd};

    (this->*l_func[mAction])();
}

void daTagLv2PrChk_c::actionCheck() {
    bool switch_1 = fopAcM_isSwitch(this, mSwbit2);
    bool switch_2 = fopAcM_isSwitch(this, mSwbit2 + 1);
    bool switch_3 = fopAcM_isSwitch(this, mSwbit2 + 2);
    bool switch_4 = fopAcM_isSwitch(this, mSwbit2 + 3);
    int active_switch_num = switch_1 + switch_2 + switch_3 + switch_4;

    switch (mMode) {
    case MODE_CHECK_FIRST_SW:
        mPlayedSound = false;
        if (switch_1 && !switch_2 && !switch_3 && !switch_4) {
            mDirection = 1;
            mMode = MODE_CHECK_SECOND_SW;
        } else if (switch_4 && !switch_1 && !switch_2 && !switch_3) {
            mDirection = -1;
            mMode = MODE_CHECK_SECOND_SW;
        } else if (switch_2 || switch_3) {
            mMode = MODE_CHECK_WRONG_ORDER;
        }
        break;
    case MODE_CHECK_SECOND_SW:
        if (mDirection > 0) {
            if (switch_1 && switch_2) {
                mMode = MODE_CHECK_THIRD_SW;
            } else if (!switch_1) {
                mMode = MODE_CHECK_RESET;
            } else if (switch_3 || switch_4) {
                mMode = MODE_CHECK_WRONG_ORDER;
            }
        } else {
            if (switch_4 && switch_3) {
                mMode = MODE_CHECK_THIRD_SW;
            } else if (!switch_4) {
                mMode = MODE_CHECK_RESET;
            } else if (switch_2 || switch_1) {
                mMode = MODE_CHECK_WRONG_ORDER;
            }
        }
        break;
    case MODE_CHECK_THIRD_SW:
        if (mDirection > 0) {
            if (switch_1 && switch_2 && switch_3) {
                mMode = MODE_CHECK_FOURTH_SW;
            } else if (!switch_1 || !switch_2) {
                field_0x56d = 1;
                mMode = MODE_CHECK_RESET;
            } else if (switch_4) {
                mMode = MODE_CHECK_WRONG_ORDER;
            }
        } else {
            if (switch_4 && switch_3 && switch_2) {
                mMode = MODE_CHECK_FOURTH_SW;
            } else if (!switch_4 || !switch_3) {
                field_0x56d = 1;
                mMode = MODE_CHECK_RESET;
            } else if (switch_1) {
                mMode = MODE_CHECK_WRONG_ORDER;
            }
        }
        break;
    case MODE_CHECK_FOURTH_SW:
        if (switch_1 && switch_2 && switch_3 && switch_4) {
            mMode = MODE_PUZZLE_CLEAR;
        } else if (mDirection > 0) {
            if (!switch_1 || !switch_2 || !switch_3) {
                mMode = MODE_CHECK_RESET;
            }
        } else if (!switch_4 || !switch_3 || !switch_2) {
            mMode = MODE_CHECK_RESET;
        }
        break;
    case MODE_PUZZLE_CLEAR:
        fopAcM_onSwitch(this, getSwbit());
        mAction = ACTION_END_e;
        break;
    case MODE_CHECK_RESET:
        if (!switch_1 && !switch_2 && !switch_3 && !switch_4) {
            mMode = MODE_CHECK_FIRST_SW;
            field_0x56d = 0;
        } else if (active_switch_num > 1 || field_0x56d == 1) {
            fopAcM_onSwitch(this, getSwbit3());
        }
        break;
    case MODE_CHECK_WRONG_ORDER:
        if (switch_1 && switch_2 && switch_3 && switch_4) {
            if (!mPlayedSound) {
                seStartWrong();
                mPlayedSound = true;
            }
        } else {
            mMode = MODE_CHECK_RESET;
        }
        break;
    case 150:
        if (!switch_1 && !switch_2 && !switch_3 && !switch_4) {
            mMode = MODE_CHECK_FIRST_SW;
        }
        break;
    }
}

void daTagLv2PrChk_c::actionEnd() {}

void daTagLv2PrChk_c::seStartWrong() {
    Z2GetAudioMgr()->seStart(Z2SE_SYS_RESULT_WRONG, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
}

int daTagLv2PrChk_c::_delete() {
    return 1;
}

static int daTagLv2PrChk_Execute(daTagLv2PrChk_c* i_this) {
    return i_this->execute();
}

static int daTagLv2PrChk_Delete(daTagLv2PrChk_c* i_this) {
    return i_this->_delete();
}

static int daTagLv2PrChk_Create(daTagLv2PrChk_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daTagLv2PrChk_Method = {
    (process_method_func)daTagLv2PrChk_Create,
    (process_method_func)daTagLv2PrChk_Delete,
    (process_method_func)daTagLv2PrChk_Execute,
    (process_method_func)NULL,
    (process_method_func)NULL,
};

actor_process_profile_definition g_profile_Tag_Lv2PrChk = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Tag_Lv2PrChk,
    &g_fpcLf_Method.base,
    sizeof(daTagLv2PrChk_c),
    0,
    0,
    &g_fopAc_Method.base,
    558,
    &l_daTagLv2PrChk_Method,
    0x40000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
