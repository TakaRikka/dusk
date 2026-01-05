#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_wljump.h"
#include "d/d_path.h"
#include "d/d_procname.h"
#include "d/actor/d_a_player.h"
#include "d/actor/d_a_midna.h"

int daTagWljump_c::create() {
    fopAcM_ct(this, daTagWljump_c);

    field_0x571 = (fopAcM_GetParam(this) >> 8) & 0xFF;

    int path_no = fopAcM_GetParam(this) & 0xFF;
    if (path_no == 0xFF) {
        return 5;
    }

    field_0x5c4 = dPath_GetRoomPath(path_no, fopAcM_GetRoomNo(this));
    if (field_0x5c4 == NULL || field_0x5c4->m_num < 2) {
        return cPhs_ERROR_e;
    }

    attention_info.distances[fopAc_attn_LOCK_e] = 50;
    attention_info.distances[fopAc_attn_ETC_e] = 50;

    shape_angle.z = 0;
    field_0x568 = -1;

    field_0x56c = (fopAcM_GetParam(this) >> 16) & 0xF;
    if (field_0x56c != 1) {
        field_0x56c = 0;
    }

    if (field_0x56c == 0 && field_0x571 != 0xFF) {
        if (!fopAcM_isSwitch(this, field_0x571)) {
            field_0x573 = 1;
        }
    }

    return cPhs_COMPLEATE_e;
}

static int daTagWljump_Create(fopAc_ac_c* i_this) {
    daTagWljump_c* a_this = (daTagWljump_c*)i_this;
    fpc_ProcID id = fopAcM_GetID(i_this);

    return a_this->create();
}

daTagWljump_c::~daTagWljump_c() {}

static int daTagWljump_Delete(daTagWljump_c* i_this) {
    fpc_ProcID id = fopAcM_GetID(i_this);

    i_this->~daTagWljump_c();
    return 1;
}

int daTagWljump_c::execute() {
    attention_info.flags = 0;

    if (field_0x56d) {
        field_0x56d--;
    }

    dPnt* point_p;

    daPy_py_c* player = daPy_getLinkPlayerActorClass();
    daMidna_c* midna = daPy_py_c::getMidnaActor();
    if (midna == NULL) {
        return 1;
    }

    if (eventInfo.checkCommandTalk()) {
        BOOL spC = TRUE;
        if (!midna->checkShadowModeTalkWait()) {
            if (shape_angle.x != 0 && (field_0x571 == 0xff || !fopAcM_isSwitch(this, field_0x571))) {
                if (field_0x56f == 0) {
                    mMsgFlow.init(this, (u16)shape_angle.x, 0, NULL);
                    field_0x56f = 1;
                    mDoAud_seStart(Z2SE_NAVI_TALK_START, NULL, 0, 0);
                } else {
                    if (mMsgFlow.doFlow(this, NULL, 0)) {
                        mDoAud_seStart(Z2SE_NAVI_TALK_END, NULL, 0, 0);
                        shape_angle.x = 0;
                    }
                }
            } else {
                field_0x568 = field_0x570;
                if (midna->current.pos.abs(eyePos) < 5.0f) {
                    spC = FALSE;
                }
            } 
        }

        if (spC) {
            return 1;
        }

        field_0x56f = 0;
        dComIfGp_event_reset();
        field_0x56c = 0;
        field_0x572 = 1;
        if (field_0x571 != 0xff) {
            fopAcM_onSwitch(this, field_0x571);
        }
    } else if (!dComIfGp_getEvent()->isOrderOK()) {
        field_0x572 = 0;
    } else {
        if (!midna->checkWolfNoPos()) {
            field_0x574++;
            if (field_0x574 >= 5) {
                field_0x572 = 0;
            }
        } else {
            field_0x574 = 0;
        }
    }

    if (!player->checkNowWolf() || !daPy_py_c::checkFirstMidnaDemo() || midna->checkMidnaTired()) {
        return 1;
    } 

    if (field_0x571 != 0xff && fopAcM_isSwitch(this, field_0x571)) {
        field_0x56c = 0;
        if (field_0x573) {
            field_0x573 = 0;
            field_0x572 = 1;
            field_0x574 = 0;
        }
    }

    if (field_0x56c != 0 || field_0x572 != 0 || field_0x571 == 0xff || fopAcM_isSwitch(this, field_0x571)) {
        int var_r28;
        point_p = field_0x5c4->m_points;

        if (!player->checkWolfTagLockJumpLand()) {
            if (!player->checkWolfTagLockJump()) {
                for (var_r28 = 0; var_r28 < field_0x5c4->m_num; var_r28++, point_p++) {
                    if (player->current.pos.abs2(point_p->m_position) < point_p->mArg1 * point_p->mArg1 * 10.0f * 10.0f) {
                        field_0x56a = var_r28;
                        if (var_r28 == 0) {
                            field_0x568 = 1;
                        } else if (var_r28 == field_0x5c4->m_num - 1) {
                            field_0x568 = var_r28 - 1;
                        } else {
                            field_0x568 = var_r28 + 1;
                        }
                        break;
                    }
                }

                if (var_r28 == field_0x5c4->m_num) {
                    field_0x568 = -1;
                }
            } else if (mNextCheckFlg) {
                mNextCheckFlg = 0;
                if (field_0x56a < field_0x568) {
                    field_0x568++;
                    if (field_0x5c4->m_num == field_0x568) {
                        field_0x568 = 0xff;
                    }
                } else {
                    field_0x568--;
                }
            }
        }

        if (field_0x568 >= 0) {
            point_p = &field_0x5c4->m_points[field_0x568];

            eyePos.set(point_p->m_position.x, point_p->m_position.y, point_p->m_position.z);
            attention_info.position = eyePos;
            attention_info.position.y += 220.0f;

            mLandArea = point_p->mArg0 * 10.0f;

            if (point_p->mArg2 == 1) {
                shape_angle.z = 1;
            } else {
                shape_angle.z = 0;
            }

            if (field_0x572 == 0) {
                if (!dComIfGp_event_runCheck()) {
                    eventInfo.onCondition(dEvtCnd_CANTALK_e);
                    if (!player->checkPlayerFly() && player->eventInfo.chkCondition(dEvtCnd_CANTALK_e)) {
                        dComIfGp_att_ZHintRequest(this, 0x1FF);

                        if (field_0x56e == 0)  {
                            field_0x56e = 1;
                            if (field_0x56d == 0) {
                                mDoAud_seStart(Z2SE_NAVI_CALLVOICE, 0, 0, 0);
                            }
                            field_0x56d = 60;
                        }
                    }
                }

                field_0x570 = field_0x568;
                field_0x568 = -1;
            } else {
                attention_info.flags |= fopAc_AttnFlag_ETC_e | fopAc_AttnFlag_LOCK_e;
            }
        } else {
            field_0x572 = 0;
        }
    } else {
        field_0x572 = 0;
        field_0x568 = -1;
    }

    current.pos = attention_info.position;

    if (!eventInfo.chkCondition(dEvtCnd_CANTALK_e)) {
        field_0x56e = 0;
    }
    
    return 1;
}

static int daTagWljump_Execute(daTagWljump_c* i_this) {
    return i_this->execute();
}

int daTagWljump_c::draw() {
    return 1;
}

static int daTagWljump_Draw(daTagWljump_c* i_this) {
    return i_this->draw();
}

static actor_method_class l_daTagWljump_Method = {
    (process_method_func)daTagWljump_Create,
    (process_method_func)daTagWljump_Delete,
    (process_method_func)daTagWljump_Execute,
    NULL,
    (process_method_func)daTagWljump_Draw,
};

actor_process_profile_definition g_profile_Tag_Wljump = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Tag_Wljump,
    &g_fpcLf_Method.base,
    sizeof(daTagWljump_c),
    0,
    0,
    &g_fopAc_Method.base,
    260,
    &l_daTagWljump_Method,
    0x44000,
    fopAc_ENV_e,
    fopAc_CULLBOX_CUSTOM_e,
};
