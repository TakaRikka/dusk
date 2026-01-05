#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_tag_kago_fall.h"

#include "d/actor/d_a_player.h"
#include "d/d_camera.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"
#include "d/d_msg_object.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_graphic.h"

static u32 m_master_id = -1;

int daTagKagoFall_c::create() {
    fopAcM_ct(this, daTagKagoFall_c);

    if (dComIfG_play_c::getLayerNo(0) == 13) {
        mExitID = 4;

        if (dComIfGp_getStartStagePoint() == 0) {
            mStartPoint = 0;
        } else {
            mStartPoint = 12;
        }
        setActionMode(ACTION_MODE_RIVER, 0);
    } else if (dComIfG_play_c::getLayerNo(0) == 14) {
        mExitID = 2;
        mStartPoint = 0;

        mDoMtx_trans(mDoMtx_stack_c::get(), current.pos.x, current.pos.y, current.pos.z);
        mDoMtx_stack_c::YrotM(shape_angle.y);
        mDoMtx_inverse(mDoMtx_stack_c::get(), mMtx);

        scale.x *= 75.0f;
        scale.z *= 75.0f;
        scale.y *= 150.0f;

        setActionMode(ACTION_MODE_FALL, 0);
    }

    if (m_master_id == -1) {
        m_master_id = fopAcM_GetID(this);
    }

    return cPhs_COMPLEATE_e;
}

int daTagKagoFall_c::execute() {
    if (mActionMode == ACTION_MODE_RIVER) {
        actionWaitRiver();
    } else {
        actionWaitFall();
    }

    return TRUE;
}

void daTagKagoFall_c::setActionMode(u8 mode, u8 state) {
    mActionMode = mode;
    mActionState = state;
}

void daTagKagoFall_c::actionWaitRiver() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    dCamera_c* camera = dCam_getBody();

    if (mTimer) {
        mTimer--;
    }
    if (mRiverTimer) {
        mRiverTimer--;
    }

    switch (mActionState) {
    case 0:
        if (m_master_id == fopAcM_GetID(this)) {
            if (!daPy_getPlayerActorClass()->checkCargoCarry()) {
                mNoCarryTimer--;

                if (mNoCarryTimer == 0) {
                    mActionState = 1;
                    mActionMode = ACTION_MODE_FALL;
                    return;
                }
            } else {
                mNoCarryTimer = 150;
            }
        }

        if (dComIfGp_checkPlayerStatus0(0, 0x100000) && dComIfGs_getLife()) {
            if (!eventInfo.checkCommandDemoAccrpt()) {
                fopAcM_orderPotentialEvent(this, 1, -1, 3);
                eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            } else {
                camera->Stop();
                camera->SetTrimSize(3);

                mRestartPos = dCam_getBody()->Eye();
                mActionState = 1;
                mTimer = 30;
                player->onNoResetFlg0(daPy_py_c::FLG0_UNK_10000);
                mRiverTimer = 60;
            }
        }
        break;

    case 1:
        if (mTimer == 1) {
            player->voiceStart(Z2SE_WL_V_FALL_TO_RESTART);
        }

        if (mRiverTimer == 0) {
            Z2GetAudioMgr()->seStart(Z2SE_FORCE_BACK, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            mDoGph_gInf_c::fadeOut(0.05f, static_cast<JUtility::TColor&>(g_blackColor));
            mRiverTimer = 20;
            mActionState = 2;
        }

        dCam_getBody()->Set(player->eyePos, mRestartPos);
        break;

    case 2:
        if (mRiverTimer == 0) {
            daPy_getPlayerActorClass()->offNoResetFlg0(daPy_py_c::FLG0_UNK_10000);
            mActionState = 3;
            mTimer = 40;
        }
        dCam_getBody()->Set(player->eyePos, mRestartPos);
        break;

    case 3:
        if (mTimer == 0) {
            mMsgFlow.init(this, 0x7d4, 0, NULL);
            mActionState = 4;
        }
        break;

    case 4: {
        daPy_getPlayerActorClass()->offNoResetFlg0(daPy_py_c::FLG0_UNK_10000);

        int msg = mMsgFlow.doFlow(this, NULL, 0);
        if (msg != 0) {
            if (dMsgObject_getSelectCursorPos() != 0) {
                dStage_changeScene(mExitID, 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
            } else {
                int room = dStage_roomControl_c::mStayNo;
                dComIfGp_setNextStage("F_SP112", mStartPoint, room, dComIfG_play_c::getLayerNo(0),
                                      0.0f, 10, 1, 0, 0, 1, 0);
            }
            mActionState = 5;
        }
        break;
    }

    case 5:
        // Maybe contained some stripped out debug code?
        break;
    }
}

void daTagKagoFall_c::actionWaitFall() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz v;

    switch (mActionState) {
    case 0:
        mDoMtx_multVec(mMtx, &player->current.pos, &v);

        if (v.y <= scale.y && v.y >= 0.0f && fabsf(v.x) <= scale.x && fabsf(v.z) <= scale.z) {
            mActionState = 1;
        }

        if (m_master_id == fopAcM_GetID(this)) {
            if (!daPy_getPlayerActorClass()->checkCargoCarry()) {
                mNoCarryTimer--;

                if (mNoCarryTimer == 0) {
                    mActionState = 1;
                }
            } else {
                mNoCarryTimer = 150;
            }
        }
        break;

    case 1:
        if (dComIfGs_getLife() == 0) {
            player->onSceneChangeAreaJump(mExitID, -1, NULL);

            if (player->checkNoResetFlg2(daPy_py_c::FLG2_SCN_CHG_START) && !mPlayedSceneChangeSfx)
            {
                Z2GetAudioMgr()->seStart(Z2SE_FORCE_BACK, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
                player->voiceStart(Z2SE_WL_V_FALL_TO_RESTART);
                mPlayedSceneChangeSfx = true;
            }
        } else if (!eventInfo.checkCommandDemoAccrpt()) {
            mRestartPos = player->current.pos;
            fopAcM_orderPotentialEvent(this, 1, -1, 0);
            eventInfo.onCondition(dEvtCnd_CANDEMO_e);
        } else {
            mDoGph_gInf_c::fadeOut(0.05f, static_cast<JUtility::TColor&>(g_blackColor));
            mTimer = 60;
            mActionState = 2;
            Z2GetAudioMgr()->seStart(Z2SE_FORCE_BACK, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            player->voiceStart(Z2SE_WL_V_FALL_TO_RESTART);
        }
        break;

    case 2:
        if (mTimer) {
            mTimer--;
        }

        if (mTimer <= 45) {
            player->setPlayerPosAndAngle(&mRestartPos, 0, 0);
        }

        if (mTimer == 0) {
            dCam_getBody()->Stop();
            dCam_getBody()->SetTrimSize(3);

            mMsgFlow.init(this, 0x7d4, 0, NULL);
            mActionState = 3;

            dBgS_LinChk lin_chk;
            mRestartPos.y += 3000.0f;
            lin_chk.Set(&player->current.pos, &mRestartPos, NULL);

            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                mRestartPos.y = lin_chk.GetCross().y;
            }
        }

        break;

    case 3: {
        player->setPlayerPosAndAngle(&mRestartPos, 0, 0);
        int msg = mMsgFlow.doFlow(this, NULL, 0);
        if (msg != 0) {
            if (dMsgObject_getSelectCursorPos() != 0) {
                dStage_changeScene(mExitID, 0.0f, 0, fopAcM_GetRoomNo(this), 0, -1);
            } else {
                int room = dStage_roomControl_c::mStayNo;
                dComIfGp_setNextStage("F_SP112", mStartPoint, room, dComIfG_play_c::getLayerNo(0),
                                      0.0f, 10, 1, 0, 0, 1, 0);
            }
            mActionState = 10;
        }
        break;
    }

    case 10:
        // Maybe contained some stripped out debug code?
        break;
    }
}

int daTagKagoFall_c::_delete() {
    return TRUE;
}

static int daTagKagoFall_Execute(daTagKagoFall_c* i_this) {
    return i_this->execute();
}

static int daTagKagoFall_Delete(daTagKagoFall_c* i_this) {
    return i_this->_delete();
}

static int daTagKagoFall_Create(daTagKagoFall_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daTagKagoFall_Method = {
    (process_method_func)daTagKagoFall_Create,
    (process_method_func)daTagKagoFall_Delete,
    (process_method_func)daTagKagoFall_Execute,
};

actor_process_profile_definition g_profile_Tag_KagoFall = {
    fpcLy_CURRENT_e,          // mLayerID
    7,                        // mListID
    fpcPi_CURRENT_e,          // mListPrio
    PROC_Tag_KagoFall,        // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daTagKagoFall_c),  // mSize
    0,                        // mSizeOther
    0,                        // mParameters
    &g_fopAc_Method.base,     // sub_method
    557,                      // mPriority
    &l_daTagKagoFall_Method,  // sub_method
    0x40000,                  // mStatus
    fopAc_ACTOR_e,            // mActorType
    fopAc_CULLBOX_CUSTOM_e,   // cullType
};
