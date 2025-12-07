/**
* @file d_a_obj_drop.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_drop.h"
#include "SSystem/SComponent/c_math.h"
#include "cmath.h"
#include "d/actor/d_a_player.h"
#include "d/d_com_inf_game.h"
#include "d/d_camera.h"
#include "d/d_item.h"
#include "d/d_tresure.h"
#include "d/d_s_play.h"
#include "m_Do/m_Do_graphic.h"
#include "d/actor/d_a_e_ym.h"
#include "d/actor/d_a_e_ymb.h"
#include "f_op/f_op_camera_mng.h"

#if DEBUG
daObjDrop_HIO_c l_HIO;
#endif

static void* searchParentSub(void* pproc, void* pdata) {
    daObjDrop_c* pdrop = (daObjDrop_c*)pdata;
    fopAc_ac_c* pym = (fopAc_ac_c*)pproc;

    if (pym != NULL && fopAcM_IsActor(pym)) {
        if (fopAcM_GetProfName(pym) == PROC_E_YM) {
            u8 swbit = ((daE_YM_c*)pym)->getSwitchBit();
            if (swbit != 0xFF && swbit == pdrop->getYmSwbit()) {
                pdrop->field_0x6b7 = 1;
                return pym;
            }
        } else if (fopAcM_GetProfName(pym) == PROC_E_YMB) {
            u8 swbit = ((daE_YMB_c*)pym)->getSwitchBit();
            if (swbit != 0xFF && swbit == pdrop->getYmSwbit()) {
                pdrop->field_0x6b7 = 2;
                return pym;
            }
        }
    }

    return NULL;
}

static const u16 l_sizuku_body_effect_list[6] = {0x838B, 0x838C, 0x838D, 0x838E, 0x838F, 0x842B};

static const u16 l_sizuku_app_effect_list[] = {0x8388, 0x8389};

const static dCcD_SrcSph l_sph_src = {
    {
        {0x0, {{AT_TYPE_0, 0x0, 0x0}, {0x0, 0x0}, 0x79}},  // mObj
        {dCcD_SE_NONE, 0x1, 0x0, dCcD_MTRL_NONE, 0x0},     // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, dCcD_MTRL_NONE, 0x6},     // mGObjTg
        {0x0},                                             // mGObjCo
    },                                                     // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 30.0f}  // mSph
    }  // mSphAttr
};

int daObjDrop_c::Create() {
    mAcchCir.SetWall(50.0f, 50.0f);
    mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
              fopAcM_GetSpeed_p(this), NULL, NULL);

    mCcStts.Init(0, 0xFF, this);
    mCcSph.Set(l_sph_src);
    mCcSph.SetStts(&mCcStts);
    mTargetPos = current.pos;
    mSound.init(&current.pos, 2);
    return 1;
}

int daObjDrop_c::create() {
    fopAcM_ct(this, daObjDrop_c);

    if (dComIfGs_isTbox(getSave())) {
        return cPhs_ERROR_e;
    }

    modeInit();

    if (!Create()) {
        return cPhs_ERROR_e;
    }

    OS_REPORT("DROP PARAM %x\n", fopAcM_GetParam(this));
    return cPhs_COMPLEATE_e;
}

void daObjDrop_c::dropGet() {
    if (mSetCollectDrop) {
        dComIfGs_onTbox(getSave());

        u8 num = dComIfGs_getLightDropNum(dComIfGp_getStartStageDarkArea());
        dComIfGs_setLightDropNum(dComIfGp_getStartStageDarkArea(), num + 1);

        if (dComIfGp_getStartStageDarkArea() == 2 &&
            dComIfGs_getLightDropNum(dComIfGp_getStartStageDarkArea()) == 15)
        {
            /* dSv_event_flag_c::F_0005 - Misc. - Gathered 14 Tears of Light in area 4 */
            dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[9]);
        }

        u8 need_num = dComIfGp_getNeedLightDropNum();
        s8 area = dComIfGp_getStartStageDarkArea();
        // "Drop of Light No%d<%d/%d> Get Area<%d>!\n"
        OS_REPORT("光の雫No%d<%d/%d>ゲットArea<%d>！\n", getSave(), num + 1, need_num, area);
        execItemGet(fpcNm_ITEM_HEART);
        mSetCollectDrop = false;
    }
}

static f32 dummy() {
    f32 temp = -4.0f;
    return temp;
}

BOOL daObjDrop_c::checkGetArea() {
    f32 dist_to_player = current.pos.abs(daPy_getPlayerActorClass()->current.pos);

    if ((daPy_getPlayerActorClass()->checkCargoCarry() &&
         strcmp(dComIfGp_getStartStageName(), "F_SP112") == 0) ||
        (dist_to_player < 250.0f && dComIfGs_isLightDropGetFlag(dComIfGp_getStartStageDarkArea())))
    {
        return true;
    }

    return false;
}

void daObjDrop_c::checkCompleteDemo() {
    u8 need_num = dComIfGp_getNeedLightDropNum();
    u8 num = dComIfGs_getLightDropNum(dComIfGp_getStartStageDarkArea());

    if (need_num - 1 == num) {
        field_0x6ac = fpcM_ERROR_PROCESS_ID_e;
        setDemoMode(DEMOMODE_ORDER_COMPLETE_DEMO_e);
    } else {
        mSetCollectDrop = true;
        dropGet();
    }
}

void daObjDrop_c::createLineEffect() {
    for (int i = 0; i < 3; i++) {
        mpLineEffEmtrs[i] = dComIfGp_particle_set(0x838A, &mLinePos[i], NULL, &scale, 0xFF,
                                                  &mLineEffCallbacks[i], -1, NULL, NULL, NULL);
        if (mpLineEffEmtrs[i] == NULL) {
            // "Drop of Light: Line Effect Set Failed!!\n"
            OS_REPORT_ERROR("光の雫：光線エフェクトセット失敗！！\n");
        }
    }
}

void daObjDrop_c::removeLineEffect() {
    for (int i = 0; i < 3; i++) {
        mLineEffCallbacks[i].remove();
    }
}

void daObjDrop_c::createBodyEffect() {
    for (int i = 0; i < 6; i++) {
        mpBodyEffEmtrs[i] =
            dComIfGp_particle_set(l_sizuku_body_effect_list[i], &current.pos, NULL, &scale, 0xFF,
                                  &mBodyEffCallbacks[i], -1, NULL, NULL, NULL);
        if (mpBodyEffEmtrs[i] == NULL) {
            // "Drop of Light: Effect Set Failed!!!\n"
            OS_REPORT_ERROR("光の雫：エフェクトセット失敗！！！\n");
        }
    }
}

void daObjDrop_c::removeBodyEffect() {
    for (int i = 0; i < 6; i++) {
        mBodyEffCallbacks[i].remove();
    }
}

int daObjDrop_c::modeInit() {
    u8 ym_swbit = getYmSwbit();

    if (ym_swbit == 0xFF || fopAcM_isSwitch(this, ym_swbit)) {
        createBodyEffect();
        if (ym_swbit != 0xFF) {
            field_0x6b7 = 3;
        }

        home.pos.y = current.pos.y + 75.0f;
        mModeAction = 2;
        setMode(MODE_WAIT_e);
        return 1;
    }

    mModeAction = 0;

    fopAc_ac_c* pparent = (fopAc_ac_c*)fpcM_Search(searchParentSub, this);
    if (pparent != NULL) {
        parentActorID = fopAcM_GetID(pparent);
        setMode(MODE_PARENT_WAIT_e);
        return 1;
    }

    setMode(MODE_NO_PARENT_e);
    return 1;
}

int daObjDrop_c::modeNoParent() {
    fopAc_ac_c* pparent = (fopAc_ac_c*)fpcM_Search(searchParentSub, this);
    if (pparent != NULL) {
        parentActorID = fopAcM_GetID(pparent);
        setMode(MODE_PARENT_WAIT_e);
    } else {
#if DEBUG
        if (!l_HIO.field_0x17) {
            // "Drop of Light: Waiting for Shadow Insect Creation! SW=%d\n"
            OS_REPORT("光の雫：闇虫生成待ち！SW=%d\n", getYmSwbit());
            l_HIO.field_0x17 = true;
        }
#endif
    }

    return 1;
}

int daObjDrop_c::modeParentWait() {
    fopAc_ac_c* pparent = fopAcM_SearchByID(parentActorID);
    if (pparent != NULL) {
        if (fopAcM_GetProfName(pparent) == PROC_E_YM) {
            current.pos = pparent->current.pos;
            return 1;
        }

        if (fopAcM_GetProfName(pparent) == PROC_E_YMB && !mAppear) {
            return 1;
        }
    }

    if (daPy_getPlayerActorClass()->checkCargoCarry()) {
        mModeAction = 50;
        setMode(MODE_WAIT_e);
        return 1;
    }

    switch (mModeAction) {
    case 0:
        current.angle.set(0, 0, 0);

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 2; j++) {
                dComIfGp_particle_set(l_sizuku_app_effect_list[j], &current.pos, &current.angle,
                                      &scale);
            }

            current.angle.y += 0x5555;
        }

        mModeAction = 1;
        mModeTimer = 40;
        current.pos.y += 300.0f;
        mSound.startSound(Z2SE_OBJ_LIGHTDROP_APPEAR, 0, -1);
        break;
    case 1:
        if (cLib_calcTimer<s8>(&mModeTimer) == 0) {
            mModeAction = 2;
        }
        break;
    case 2:
        createBodyEffect();
        mModeTimer = 45;
        mModeAction = 0;
        setMode(MODE_WAIT_e);
        break;
    }

    return 1;
}

int daObjDrop_c::modeWait() {
    daPy_py_c* pplayer = daPy_getPlayerActorClass();

    cXyz collect_pos(pplayer->current.pos);
    cXyz spF0;
    mDoMtx_stack_c::copy(pplayer->getWolfBackbone2Matrix());
    mDoMtx_stack_c::multVecZero(&collect_pos);

    switch (mModeAction) {
    case 0:
        if (mModeTimer > 0) {
            mModeTimer--;
        } else {
            mTargetPos = home.pos;
            speedF = 0.0f;
            mModeAction = 1;
        }
        break;
    case 1:
    case 2:
    case 50:
        cLib_chaseF(&speedF, 7.5f, 0.4f);
        if (mModeAction == 1) {
            cLib_chasePos(&current.pos, mTargetPos, speedF);
        }

        speed.y = -cM_scos((KREG_S(3) + 1000) * mCounter) * 2.5f;
        current.pos.y += speed.y;

        if (mTargetPos.abs(current.pos) < 30.0f) {
            mModeAction = 2;
        }

        if (checkGetArea()) {
            for (int i = 0; i < 3; i++) {
                mLinePos[i] = current.pos;
                mLineMovePos[i] = cXyz::Zero;
                mTargetAngle[i] = csXyz::Zero;
            }

            createLineEffect();
            mModeAction = 3;
        }
        break;
    case 3:
        removeBodyEffect();

        static const s16 target_rel_angle[] = {0xC000, 0x0000, 0x4000};
        static const s16 target_angle_x[] = {0x0000, 0x2000, 0x0000};

        for (int i = 0; i < 3; i++) {
            mTargetAngle[i].x = target_angle_x[i];
            mTargetRelAngle[i] = target_rel_angle[i];
        }

        mModeAction = 4;
        mDrawInTimer = 60;
        mSound.startSound(Z2SE_OBJ_LIGHTDROP_DRAW_IN, 0, -1);
        break;
    case 4:
        mDrawInTimer--;
        for (int i = 0; i < 3; i++) {
            speedF = 60.0f;
            if (daPy_getPlayerActorClass()->checkCargoCarry()) {
                speedF = 250.0f;
            }

            f32 var_f25 = cM_scos(mTargetAngle[i].x) * speedF;
            f32 var_f24 = cM_ssin(mTargetAngle[i].x) * speedF;

            s16 sp150 = cM_atan2s(collect_pos.x - mLinePos[i].x, collect_pos.z - mLinePos[i].z);
            sp150 += mTargetRelAngle[i];

            cXyz target;
            target.x = var_f25 * cM_ssin(sp150);
            target.y = var_f24;
            target.z = var_f25 * cM_scos(sp150);

            f32 step_scale = KREG_F(10) + 0.5f;
            f32 step_max = KREG_F(11) + 5.0f;
            f32 step_min = KREG_F(12) + 1.0f;

            cLib_addCalc(&mLineMovePos[i].x, target.x, step_scale, step_max, step_min);
            cLib_addCalc(&mLineMovePos[i].y, target.y, step_scale, step_max, step_min);
            cLib_addCalc(&mLineMovePos[i].z, target.z, step_scale, step_max, step_min);
            cLib_addCalcAngleS(&mTargetRelAngle[i], 0, 10, 900, 200);

            cXyz prev_line_pos(mLinePos[i]);
            mLinePos[i].x += mLineMovePos[i].x;
            mLinePos[i].y += mLineMovePos[i].y;
            mLinePos[i].z += mLineMovePos[i].z;

            cM3dGLin line;
            cM3dGSph sph;
            line.SetStartEnd(prev_line_pos, mLinePos[i]);
            sph.Set(collect_pos, KREG_F(17) + 50.0f);

            Vec sp114;
            if (cM3d_Cross_LinSph(&line, &sph, &sp114)) {
                mLinePos[i] = collect_pos;
                mLineIsCross[i] = true;
            }

            cLib_addCalcAngleS(
                &mTargetAngle[i].x,
                cM_atan2s(collect_pos.y - mLinePos[i].y, mLinePos[i].absXZ(collect_pos)), 5, 1500,
                500);
        }

        if (mLineIsCross[0] || mLineIsCross[1] || mLineIsCross[2] || mDrawInTimer == 0) {
            pplayer->onWolfLightDropGet();
            removeLineEffect();
            mDeleteTimer = 70;
            checkCompleteDemo();
            mModeAction = 5;
        }
        break;
    case 5:
        if (cLib_calcTimer<u8>(&mDeleteTimer) == 0) {
            mModeAction = 6;

            if (chkDemoMode() == DEMOMODE_COMPLETE_WAIT_e) {
                fopAcM_delete(this);
            }
        }
        break;
    case 6:
        break;
    }

    if (mModeAction == 0 || mModeAction == 1 || mModeAction == 2 || mModeAction == 3) {
        mSound.startLevelSound(Z2SE_OBJ_LIGHTDROP_FLOAT, 0, -1);
    }

    if (mModeAction == 2) {
        if (mTargetPos.absXZ(current.pos) < (KREG_F(4) + 500.0f) && pplayer->getSpeedF() != 0.0f) {
            mCcSph.SetC(current.pos);
            dComIfG_Ccsp()->Set(&mCcSph);
        }

        cXyz* cc_movep = mCcStts.GetCCMoveP();
        if (cc_movep != NULL) {
            cXyz move_vec;
            cXyz sp12C;

            if (!cc_movep->isZero()) {
                current.angle.y = cM_atan2s(cc_movep->x, cc_movep->z);
                speedF = 5.0f;
                sp12C = cXyz::Zero;
            } else {
                f32 var_f26 = KREG_F(2) + 1.0f;
                if (current.pos.absXZ(mTargetPos) < 10.0f) {
                    var_f26 = 0.0f;
                    speedF = 0.0f;
                }

                s16 sp154 = cM_atan2s(mTargetPos.x - current.pos.x, mTargetPos.z - current.pos.z);
                sp12C.set(cM_ssin(sp154) * var_f26, 0.0f, cM_scos(sp154) * var_f26);
            }

            move_vec.set(cM_ssin(current.angle.y) * speedF, 0.0f,
                         cM_scos(current.angle.y) * speedF);
            move_vec += sp12C;
            speedF = std::sqrt(move_vec.x * move_vec.x + move_vec.z * move_vec.z);
            if (speedF > 5.0f) {
                speedF = 5.0f;
            }

            current.pos.x += move_vec.x;
            current.pos.z += move_vec.z;
            *cc_movep = cXyz::Zero;
        }
    }

    return 1;
}

int daObjDrop_c::actionCompleteWait() {
    return 1;
}

int daObjDrop_c::actionOrderCompleteDemo() {
    camera_class* pcamera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));

    if (eventInfo.checkCommandDemoAccrpt()) {
        pcamera->mCamera.SetTrimTypeForce(1);
        setDemoMode(DEMOMODE_COMPLETE_DEMO_e);
        mFadeoutTimer = KREG_S(8) + 135;
        mSetCollectDrop = true;
        dropGet();
    } else {
        fopAcM_orderPotentialEvent(this, 2, 0, 0);
        eventInfo.onCondition(dEvtCnd_CANDEMO_e);
    }

    return 1;
}

int daObjDrop_c::actionCompleateDemo() {
    camera_class* pcamera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    pcamera->mCamera.SetTrimTypeForce(1);

    if (eventInfo.checkCommandDemoAccrpt()) {
        int timer = cLib_calcTimer<u8>(&mFadeoutTimer);
        if (timer == 0) {
            u8 need_num = dComIfGp_getNeedLightDropNum();
            u8 num = dComIfGs_getLightDropNum(dComIfGp_getStartStageDarkArea());
            if (need_num == num) {
                fopAcM_OnStatus(this, 0x4000);
                dComIfGs_onSaveSwitch(13);
            }
        } else if (timer == 1) {
            dComIfGs_onSaveSwitch(13);
        }
    }

    if (mFadeoutTimer == KREG_S(9) + 30) {
        mDoGph_gInf_c::fadeOut_f(1.0f / 30.0f, g_saftyWhiteColor);
    }

    return 1;
}

int daObjDrop_c::actionWaitCompleteGetDemo() {
    if (eventInfo.checkCommandItem()) {
        setDemoMode(DEMOMODE_COMPLETE_GET_DEMO_e);

        if (mPtI_Id != fpcM_ERROR_PROCESS_ID_e) {
            dComIfGp_event_setItemPartnerId(mPtI_Id);
        }

        dComIfGs_onSaveSwitch(13);
    } else {
        fopAcM_orderItemEvent(this, 0, 0);
        eventInfo.onCondition(dEvtCnd_CANGETITEM_e);
    }

    return 1;
}

int daObjDrop_c::actionCompleteGetDemo() {
    if (dComIfGp_evmng_endCheck("DEFAULT_GETITEM")) {
        dComIfGp_event_reset();
        fopAcM_delete(this);
    }

    return 1;
}

int daObjDrop_c::execute() {
    typedef int (daObjDrop_c::*exeFunc)();
    static exeFunc l_exeFunc[] = {
        &daObjDrop_c::modeInit,
        &daObjDrop_c::modeParentWait,
        &daObjDrop_c::modeNoParent,
        &daObjDrop_c::modeWait,
    };
    static exeFunc l_completeDemoFunc[] = {
        &daObjDrop_c::actionCompleteWait,    &daObjDrop_c::actionOrderCompleteDemo,
        &daObjDrop_c::actionCompleateDemo,   &daObjDrop_c::actionWaitCompleteGetDemo,
        &daObjDrop_c::actionCompleteGetDemo,
    };

    attention_info.position = current.pos;
    eyePos = current.pos;

    mCounter++;

    (this->*l_exeFunc[mMode])();
    (this->*l_completeDemoFunc[mDemoMode])();

    mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    dTres_c::setLightDropPostion(getSave(), &current.pos);
    return 1;
}

int daObjDrop_c::_delete() {
    removeLineEffect();
    removeBodyEffect();
    mSound.deleteObject();
    return 1;
}

static int daObjDrop_Execute(daObjDrop_c* i_this) {
    return i_this->execute();
}

static int daObjDrop_Delete(daObjDrop_c* i_this) {
    return i_this->_delete();
}

static int daObjDrop_Create(fopAc_ac_c* i_this) {
    return ((daObjDrop_c*)i_this)->create();
}

static actor_method_class l_daObjDrop_Method = {
    (process_method_func)daObjDrop_Create,
    (process_method_func)daObjDrop_Delete,
    (process_method_func)daObjDrop_Execute,
    (process_method_func)NULL,
    (process_method_func)NULL,
};

extern actor_process_profile_definition g_profile_Obj_Drop = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_Drop,           // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daObjDrop_c),     // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    243,                     // mPriority
    &l_daObjDrop_Method,     // sub_method
    0x00040000,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
