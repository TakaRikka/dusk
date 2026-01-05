/**
 * d_a_sq.cpp
 * Squirrel
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_sq.h"
#include "SSystem/SComponent/c_math.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "d/actor/d_a_player.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_com_inf_game.h"
#include "d/d_procname.h"

static bool hio_set;

static daSq_HIO_c l_HIO;

static u32 wait_bck[4] = {9, 10, 11, 12};

static u32 carry_wait_bck[3] = {9, 9, 9};

daSq_HIO_c::daSq_HIO_c() {
    field_0x4 = -1;
    mScale = 2.0f;
    field_0xc = 20.0f;
    mRunAnmSpeed = 1.0f;
    mRunAway = true;
    mRunDistance = 100.0f;
    mRunSpeed = 25.0f;
    mFallSpeedXZ = 30.0f;
    mFallSpeedY = 30.0f;
    field_0x28 = 1000.0f;
    field_0x2c = 17.0f;
}

static void anm_init(sq_class* i_this, int i_bck, f32 i_morf, u8 i_mode, f32 i_speed, int i_bas) {
    if (i_this->field_0x60c > 1.0f) {
        return;
    }

    if (i_bas >= 0) {
        i_this->mpMorf->setAnm(static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("Sq", i_bck)),
                               i_mode, i_morf, i_speed, 0.0f, -1.0f,
                               dComIfG_getObjectRes("Sq", i_bas));
    } else {
        i_this->mpMorf->setAnm(static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("Sq", i_bck)),
                               i_mode, i_morf, i_speed, 0.0f, -1.0f, NULL);
    }
}

static int daSq_Draw(sq_class* i_this) {
    J3DModel* model = i_this->mpMorf->getModel();
    g_env_light.settingTevStruct(0, &i_this->current.pos, &i_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &i_this->tevStr);
    i_this->mpBtkAnm->entry(model->getModelData());
    i_this->mpMorf->entryDL();
    cXyz shadow_pos;
    shadow_pos.set(i_this->current.pos.x, i_this->current.pos.y + 100.0f, i_this->current.pos.z);
    i_this->mShadowKey =
        dComIfGd_setShadow(i_this->mShadowKey, 1, model, &shadow_pos, 400.0f, 10.0f * l_HIO.mScale,
                           i_this->current.pos.y, i_this->mAcch.GetGroundH(), i_this->mAcch.m_gnd,
                           &i_this->tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    return 1;
}

static BOOL way_bg_check(sq_class* i_this) {
    fopAc_ac_c* _this = static_cast<fopAc_ac_c*>(i_this);
    cXyz vec1, vec2, vec3;
    vec1 = _this->current.pos;
    vec1.y += 30.0f;
    cMtx_YrotS(*calc_mtx, _this->shape_angle.y);
    vec3.x = 0.0f;
    vec3.y = 0.0f;
    vec3.z = 100.0f;
    MtxPosition(&vec3, &vec2);
    vec2 += vec1;
    dBgS_LinChk lin_chk;
    lin_chk.Set(&vec1, &vec2, _this);
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        return true;
    } else {
        return false;
    }
}

static void turn_set(sq_class* i_this) {
    s16 angle_table[3] = {-0x8000, 0x4000, -0x4000};
    s16 angle = angle_table[(int)cM_rndF(2.99f)];
    i_this->mTargetAngleY = i_this->current.angle.y + angle;
}

static void sq_normal(sq_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    f32 target_speed = 0.0f;
    int rnd, anm;

    switch (i_this->mMode) {
    case -1:
    case 0:
        i_this->mTimer[0] = cM_rndF(120.0f) + 60.0f;
        // no break

    case 1:
        i_this->mTimer[1] = cM_rndF(30.0f) + 15.0f;
        rnd = cM_rndF(8.0f);
        if (rnd < 2.0f) {
            anm = 0;
        } else if (rnd < 4.0f) {
            anm = 1;
        } else if (rnd < 6.0f) {
            anm = 2;
        } else {
            anm = 3;
        }
        if (i_this->mMode == 0) {
            anm_init(i_this, wait_bck[anm], 10.0f, 2, 1.0f, -1);
        } else {
            anm_init(i_this, wait_bck[anm], 4.0f, 2, 1.0f, -1);
        }
        i_this->mMode = 2;
        break;

    case 2:
        if (i_this->mTimer[1] == 0 || i_this->mTimer[0] == 0) {
            if (i_this->mTimer[0] != 0) {
                i_this->mMode--;
            } else {
                i_this->mMode = 10;
                if (way_bg_check(i_this)) {
                    turn_set(i_this);
                } else {
                    i_this->mTargetAngleY = i_this->current.angle.y + (s16)cM_rndFX(16000.0f);
                }
            }
        }
        break;

    case 10:
        i_this->mTimer[0] = cM_rndF(40.0f) + 20.0f;
        anm_init(i_this, 7, 4.0f, 2, l_HIO.mRunAnmSpeed, -1);
        i_this->mMode++;
        // no break

    case 11:
        target_speed = l_HIO.field_0xc;
        cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mTargetAngleY, 4, 0x2000);
        if (i_this->mTimer[0] == 0) {
            i_this->mMode = 0;
        } else if (i_this->mTimer[2] == 0 && i_this->mAcch.ChkWallHit()) {
            turn_set(i_this);
            i_this->mTimer[2] = 20;
        }
    }

    cLib_addCalc2(&i_this->speedF, target_speed, 1.0f, 5.0f);
    if (fabsf(player->speedF) < 8.0f && i_this->mRunCheckTimer == 0) {
        i_this->mRunCheckTimer = 1;
    }
    if (dComIfGp_checkPlayerStatus0(0, 0x8000000)) {
        i_this->mRunCheckTimer = 30;
    }
    cXyz player_delta;
    player_delta.x = player->eyePos.x - i_this->current.pos.x;
    player_delta.z = player->eyePos.z - i_this->current.pos.z;
    s16 player_angle = i_this->shape_angle.y - cM_atan2s(player_delta.x, player_delta.z);
    if (player_angle < 0) {
        player_angle = -player_angle;
    }
    if ((u16)player_angle < 0x4000) {
        i_this->mRunCheckTimer = 0;
    }
    if (l_HIO.mRunAway && i_this->mRunCheckTimer == 0
                         && i_this->mPlayerDist < l_HIO.mRunDistance) {
        i_this->mAction = sq_class::ACT_AWAY;
        i_this->mMode = 0;
    }
}

static void sq_away(sq_class* i_this) {
    f32 target_speed = 0.0f;
    cXyz player_delta = i_this->current.pos - dComIfGp_getPlayer(0)->current.pos;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, 7, 4.0f, 2, l_HIO.mRunAnmSpeed * 1.5f, -1);
        i_this->mMode++;
        i_this->mTimer[0] = 0;
        // no break

    case 1:
        target_speed = l_HIO.mRunSpeed;
        if (i_this->mTimer[0] == 0) {
            if (i_this->mPlayerDist > l_HIO.mRunDistance + 50.0f) {
                i_this->mAction = sq_class::ACT_NORMAL;
                i_this->mMode = 0;
            } else {
                i_this->mTimer[0] = cM_rndF(20.0f) + 5.0f;
                if (way_bg_check(i_this)) {
                    turn_set(i_this);
                } else {
                    int rnd = cM_rndFX(8000.0f);
                    s16 player_angle = cM_atan2s(player_delta.x, player_delta.z);
                    i_this->mTargetAngleY = player_angle + rnd;
                }
            }
        } else if (i_this->mTimer[2] == 0 && i_this->mAcch.ChkWallHit()) {
            turn_set(i_this);
            i_this->mTimer[2] = 20;
        }
        break;
    }

    cLib_addCalc2(&i_this->speedF, target_speed, 1.0f, 5.0f);
    cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mTargetAngleY, 2, 0x3000);
}

static void sq_carry(sq_class* i_this) {
    i_this->mCcSph.OffCoSetBit();
    i_this->speed.y = 0.0f;

    switch (i_this->mMode) {
    case 0:
        i_this->mMode++;
        i_this->mTimer[1] = 0;
        // no break

    case 1:
        if (i_this->mTimer[1] == 0) {
            i_this->mTimer[1] = cM_rndF(30.0f) + 15.0f;
            anm_init(i_this, carry_wait_bck[(int)cM_rndF(2.999f)], 4.0f, 2, 1.0f, -1);
        }
        break;
    }

    if (!fopAcM_checkCarryNow(i_this)) {
        if (fopAcM_GetSpeedF(i_this) > 0.0f) {
            i_this->mMode = 0;
            i_this->speedF = l_HIO.mFallSpeedXZ;
            i_this->speed.y = l_HIO.mFallSpeedY;
            i_this->mAction = sq_class::ACT_FLY;
            i_this->mMode = 0;
        } else {
            i_this->mAction = sq_class::ACT_NORMAL;
            i_this->mMode = -1;
        }
    }
}

static void sq_fly(sq_class* i_this) {
    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, 7, 2.0f, 0, 0.0f, -1);
        i_this->mpMorf->setFrame(6.0f);
        i_this->mMode++;
        break;

    case 1:
        break;
    }

    cLib_addCalc2(&i_this->speedF, l_HIO.mFallSpeedXZ, 1.0f, 10.0f);
    if (i_this->mAcch.ChkGroundHit()) {
        i_this->mAction = sq_class::ACT_NORMAL;
        i_this->mMode = -1;
    }
}

static void sq_message(sq_class* i_this) {
    int anm;
    f32 rnd;

    switch (i_this->mMode) {
    case 0:
        rnd = (int)cM_rndF(8.0f);
        if (rnd < 2.0f) {
            anm = 0;
        } else if (rnd < 4.0f) {
            anm = 1;
        } else if (rnd < 6.0f) {
            anm = 2;
        } else {
            anm = 3;
        }
        anm_init(i_this, wait_bck[anm], 4.0f, 2, 1.0f, -1);
        i_this->mMode++;
        break;

    case 1:
        break;
    }

    cLib_addCalc0(&i_this->speedF, 1.0f, 2.0f);
    cLib_addCalcAngleS2(&i_this->current.angle.y, fopAcM_searchPlayerAngleY(i_this), 2, 0x1000);
    if (!dComIfGp_event_runCheck() && i_this->mPlayerDist > 400.0f) {
        i_this->mAction = sq_class::ACT_NORMAL;
        i_this->mMode = 0;
    }
}

static void action(sq_class* i_this) {
    cXyz vec1, vec2;
    i_this->gravity = -7.0f;
    i_this->field_0x5e4 = 0;
    i_this->mCcSph.OnCoSetBit();
    i_this->mPlayerDist = fopAcM_searchPlayerDistance(i_this);
    cLib_addCalcAngleS2(&i_this->current.angle.x, 0, 2, 0x2000);
    i_this->current.angle.z = 0;
    i_this->mMessageState = 0;

    if (i_this->field_0x60c < 1.0f) {
        switch (i_this->mAction) {
        case sq_class::ACT_NORMAL:
            sq_normal(i_this);
            i_this->mMessageState = 1;
            break;
        case sq_class::ACT_AWAY:
            sq_away(i_this);
            i_this->mMessageState = 1;
            break;
        case sq_class::ACT_CARRY:
            sq_carry(i_this);
            break;
        case sq_class::ACT_FLY:
            sq_fly(i_this);
            break;
        case sq_class::ACT_MESSAGE:
            sq_message(i_this);
            i_this->mMessageState = 2;
            break;
        }

        if (i_this->mMessageState == 1 && daPy_py_c::checkNowWolf()
                                       && i_this->mPlayerDist < 300.0f) {
            i_this->mAction = sq_class::ACT_MESSAGE;
            i_this->mMode = 0;
        }
    }

    if (i_this->field_0x5e4 != 0) {
        cMtx_YrotS(*calc_mtx, i_this->current.angle.y);
        cMtx_XrotM(*calc_mtx, i_this->current.angle.x);
        vec1.x = 0.0f;
        vec1.y = 0.0f;
        vec1.z = i_this->speedF;
        MtxPosition(&vec1, &i_this->speed);
        i_this->current.pos += i_this->speed;
    } else {
        cMtx_YrotS(*calc_mtx, i_this->current.angle.y);
        vec1.x = 0.0f;
        vec1.y = 0.0f;
        vec1.z = i_this->speedF;
        MtxPosition(&vec1, &vec2);
        i_this->speed.x = vec2.x;
        i_this->speed.z = vec2.z;
        i_this->speed.y += i_this->gravity;
        i_this->current.pos += i_this->speed;
        if (i_this->speed.y < -120.0f) {
            i_this->speed.y = -120.0f;
        }
        cXyz* move_p = i_this->mCcStts.GetCCMoveP();
        if (move_p != NULL) {
            i_this->current.pos.x += move_p->x;
            i_this->current.pos.y += move_p->y;
            i_this->current.pos.z += move_p->z;
        }
        i_this->mAcch.CrrPos(dComIfG_Bgsp());
    }

    if (i_this->field_0x60c > 1.0f) {
        vec1.x = 0.0f;
        vec1.y = 0.0f;
        vec1.z = -i_this->field_0x60c;
        cMtx_YrotS(*calc_mtx, i_this->field_0x610);
        MtxPosition(&vec1, &vec2);
        i_this->current.pos += vec2;
        cLib_addCalc0(&i_this->field_0x60c, 1.0f, 6.0f);
        i_this->speedF = 0.0f;
    }

    cLib_addCalcAngleS2(&i_this->shape_angle.y, i_this->current.angle.y, 2, 0x4000);
    cLib_addCalcAngleS2(&i_this->shape_angle.x, i_this->current.angle.x, 2, 0x4000);
    i_this->shape_angle.z = i_this->current.angle.z;
}

static int message(sq_class* i_this) {
    if (i_this->mIsTalking != 0) {
        i_this->mMessageTimer = 10;
        if (i_this->mMsgFlow.doFlow(i_this, NULL, 0)) {
            dComIfGp_event_reset();
            i_this->mIsTalking = 0;
        }
        return 1;
    }

    if (dComIfGp_event_runCheck() && i_this->eventInfo.checkCommandTalk()) {
        i_this->mMsgFlow.init(i_this, i_this->mFlowID, 0, NULL);
        i_this->mIsTalking = 1;
    }

    if (i_this->mMessageState == 2 && i_this->mFlowID != -1 && daPy_py_c::checkNowWolf()) {
        fopAcM_OnStatus(i_this, 0);
        cLib_onBit<u32>(i_this->attention_info.flags, 0xa);
        i_this->eventInfo.onCondition(1);
    } else {
        fopAcM_OffStatus(i_this, 0);
        cLib_offBit<u32>(i_this->attention_info.flags, 0xa);
    }

    return 0;
}

static int daSq_Execute(sq_class* i_this) {
    cXyz vec(0.0f, 0.0f, 0.0f);
    cLib_addCalc2(&i_this->scale.x, 1.0f, 1.0f, 0.25f);

    i_this->mCounter++;
    for (int i = 0; i < 4; i++) {
        if (i_this->mTimer[i] != 0) {
            i_this->mTimer[i]--;
        }
    }
    if (i_this->mMessageTimer != 0) {
        i_this->mMessageTimer--;
    }
    if (i_this->mRunCheckTimer != 0) {
        i_this->mRunCheckTimer--;
    }

    action(i_this);

    mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(i_this->shape_angle.x);
    mDoMtx_stack_c::ZrotM(i_this->shape_angle.z);
    f32 scale = i_this->scale.x * l_HIO.mScale;
    mDoMtx_stack_c::scaleM(scale, scale, scale);
    J3DModel* model = i_this->mpMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    i_this->mpMorf->play(&i_this->eyePos, 0, 0);
    i_this->mpBtkAnm->setFrame(i_this->mBtkFrame);
    i_this->mpMorf->modelCalc();
    
    MTXCopy(model->getAnmMtx(2), *calc_mtx);
    MtxPosition(&vec, &i_this->eyePos);
    i_this->attention_info.position = i_this->eyePos;
    i_this->attention_info.position.y += 20.0f * l_HIO.mScale;

    i_this->mCcSph.SetC(i_this->eyePos);
    i_this->mCcSph.SetR(10.0f * l_HIO.mScale);
    dComIfG_Ccsp()->Set(&i_this->mCcSph);

    message(i_this);
    return 1;
}

static int daSq_IsDelete(sq_class* i_this) {
    return 1;
}

static int daSq_Delete(sq_class* i_this) {
    dComIfG_resDelete(&i_this->mPhaseReq, "Sq");
    if (i_this->mHIOInit) {
        hio_set = false;
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    sq_class* _this = static_cast<sq_class*>(i_this);
    
    _this->mpMorf =
        new mDoExt_McaMorf(static_cast<J3DModelData*>(dComIfG_getObjectRes("Sq", 17)), NULL, NULL,
                           static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("Sq", 9)),
                           0, 1.0f, 0, -1, 1, NULL, 0x80000, 0x11000284);
    if (_this->mpMorf == NULL || _this->mpMorf->getModel() == NULL) {
        return 0;
    }

    _this->mpBtkAnm = new mDoExt_btkAnm();
    if (_this->mpBtkAnm == NULL) {
        return 0;
    }
    return _this->mpBtkAnm->init(_this->mpMorf->getModel()->getModelData(),
                                 static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes("Sq", 20)),
                                 1, 0, 1.0f, 0, -1) ? 1 : 0;
}

static cPhs__Step daSq_Create(fopAc_ac_c* i_this) {
    sq_class* _this = static_cast<sq_class*>(i_this);
    fopAcM_ct(i_this, sq_class);
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&_this->mPhaseReq, "Sq");

    if (step == cPhs_COMPLEATE_e) {
        _this->mParam0 = fopAcM_GetParam(_this) & 0xff;
        if (_this->mParam0 == 1) {
            fopAcM_create(PROC_NPC_SQ, fopAcM_GetParam(_this), &_this->home.pos,
                          fopAcM_GetRoomNo(_this), &_this->home.angle, NULL, 0xff);
            return cPhs_ERROR_e;
        }

        _this->mParam1 = (fopAcM_GetParam(_this) >> 8) & 0xff;
        _this->mParam2 = (fopAcM_GetParam(_this) >> 0x10) & 0xff;
        _this->mParam3 = (fopAcM_GetParam(_this) >> 0x18) & 0xff;
        _this->field_0x5b9 = (fopAcM_GetParam(_this) >> 0x10) & 0xff;
        _this->field_0x5b8 = (fopAcM_GetParam(_this) >> 0x18) & 0xff;
        _this->mFlowID = _this->current.angle.z;
        _this->shape_angle.z = 0;
        _this->current.angle.z = 0;

        if (!fopAcM_entrySolidHeap(_this, useHeapInit, 0x17c0)) {
            return cPhs_ERROR_e;
        }
        
        if (!hio_set) {
            _this->mHIOInit = true;
            hio_set = true;
            l_HIO.field_0x4 = -1;
        }
        
        fopAcM_OnStatus(_this, 0x100);
        _this->attention_info.flags = 0;
        _this->mAction = sq_class::ACT_NORMAL;
        fopAcM_SetMtx(_this, _this->mpMorf->getModel()->getBaseTRMtx());
        _this->mAcch.Set(fopAcM_GetPosition_p(_this), fopAcM_GetOldPosition_p(_this), _this, 1,
                         &_this->mAcchCir, fopAcM_GetSpeed_p(_this), NULL, NULL);
        _this->mAcchCir.SetWall(30.0f, 40.0f);
        _this->health = 1;
        _this->field_0x560 = 1;
        _this->mCcStts.Init(100, 0, _this);

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0x0, 0x0}, 0x75}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 20.0f} // mSph
            } // mSphAttr
        };
        _this->mCcSph.Set(cc_sph_src);
        _this->mCcSph.SetStts(&_this->mCcStts);
        
        static dCcD_SrcSph at_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0xd}, {0x0, 0x0}, 0x0}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 15.0f} // mSph
            } // mSphAttr
        };
        _this->mAtSph.Set(at_sph_src);
        _this->mAtSph.SetStts(&_this->mCcStts);

        _this->attention_info.distances[fopAc_attn_CARRY_e] = 7;

        if (_this->mParam1 == 0xff) {
            _this->mBtkFrame = cM_rndF(3.999f);
        } else {
            _this->mBtkFrame = _this->mParam1;
            if (_this->mBtkFrame > 3) {
                _this->mBtkFrame = 3;
            }
        }
        
        daSq_Execute(_this);
    }
    return step;
}

static actor_method_class l_daSq_Method = {
    (process_method_func)daSq_Create,
    (process_method_func)daSq_Delete,
    (process_method_func)daSq_Execute,
    (process_method_func)daSq_IsDelete,
    (process_method_func)daSq_Draw,
};

actor_process_profile_definition g_profile_SQ = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_SQ,
    &g_fpcLf_Method.base,
    sizeof(sq_class),
    0,
    0,
    &g_fopAc_Method.base,
    0x2B9,
    &l_daSq_Method,
    0xC0000,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_0_e,
};
