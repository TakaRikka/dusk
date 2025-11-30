/**
 * @file d_a_obj_dan.cpp
 * Insect - Pillbug
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_dan.h"
#include "SSystem/SComponent/c_math.h"
#include "m_Do/m_Do_lib.h"
#include "d/d_com_inf_game.h"
#include "d/d_menu_insect.h"
#include "d/d_procname.h"
#include "f_op/f_op_camera_mng.h"

static bool hioInit;

static daObj_DanHIO_c l_HIO;

daObj_DanHIO_c::daObj_DanHIO_c() {
    field_0x4 = -1;
    mScaleMale = 1.0f;
    mScaleFemale = 1.0f;
}

void daObjDAN_c::InitCcSph() {
    const static dCcD_SrcSph ccSphSrc = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0x14002, 0x11}, 0x0}}, // mObj
            {dCcD_SE_STONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
        } // mSphAttr
    };

    mCcStts.Init(1, 0, this);
    mCcSph.Set(ccSphSrc);
    mCcSph.SetStts(&mCcStts);
    mCcSph.SetC(current.pos);
    mCcSph.OnTgNoHitMark();
}

void daObjDAN_c::SetCcSph() {
    mCcSph.SetC(current.pos);
    mCcSph.SetR(20.0f);
    dComIfG_Ccsp()->Set(&mCcSph);
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return static_cast<daObjDAN_c*>(i_this)->CreateHeap();
}

int daObjDAN_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes("I_Dan", 11);
    mpMorf = new mDoExt_McaMorfSO(model_data, NULL, NULL,
                                  static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("I_Dan", 8)),
                                  J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1,
                                  &mCreatureSound, 0, 0x11000284);
    if (mpMorf == NULL || mpMorf->getModel() == NULL) {
        return 0;
    }
    J3DModel* model = mpMorf->getModel();

    mpBrkAnm = new mDoExt_brkAnm();
    if (mpBrkAnm == NULL) {
        return 0;
    }
    if (mSex == SEX_FEMALE) {
        J3DAnmTevRegKey* tev_anm = static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes("I_Dan", 14));
        if (!mpBrkAnm->init(model->getModelData(), tev_anm, TRUE,
                            J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1)) {
            return 0;
        }
    } else {
        J3DAnmTevRegKey* tev_anm = static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes("I_Dan", 15));
        if (!mpBrkAnm->init(model->getModelData(), tev_anm, TRUE,
                            J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1)) {
            return 0;
        }
    }

    mpBtkAnm = new mDoExt_btkAnm();
    if (mpBtkAnm == NULL) {
        return 0;
    }
    if (mSex == SEX_FEMALE) {
        J3DAnmTextureSRTKey* tex_anm =
            static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes("I_Dan", 18));
        if (!mpBtkAnm->init(model->getModelData(), tex_anm, TRUE,
                            J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1)) {
            return 0;
        }
    } else {
        mpBtkAnm = new mDoExt_btkAnm();
        J3DAnmTextureSRTKey* tex_anm =
            static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes("I_Dan", 19));
        if (!mpBtkAnm->init(model->getModelData(), tex_anm, TRUE,
                            J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1)) {
            return 0;
        }
    }

    return 1;
}

static cPhs__Step daObjDAN_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjDAN_c*>(i_this)->create();
}

static int daObjDAN_Delete(daObjDAN_c* i_this) {
    i_this->Delete();
    return 1;
}

void daObjDAN_c::SpeedSet() {
    cLib_addCalc2(&speedF, mTargetSpeed, 0.1f, 10.0f);
    cLib_addCalcAngleS2(&current.angle.y, mTargetAngleY, 0xa0, 0x100);
    shape_angle.y = current.angle.y;
}

void daObjDAN_c::LinkChk() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (fopAcM_searchPlayerDistanceXZ(this) < 50.0f && player->speedF > 0.0f) {
        mAction = ACT_DAMAGE;
        mMode = 0;
    }
}

void daObjDAN_c::MoveAction() {
    if (mTimer[0] == 0) {
        cXyz delta = home.pos - current.pos;
        delta.x += cM_rndFX(550.0f);
        delta.z += cM_rndFX(550.0f);
        mTargetAngleY = cM_atan2s(delta.x, delta.z);
        mTimer[0] = cM_rndF(50.0f) + 50.0f;
    }

    SpeedSet();
    LinkChk();
    mCreatureSound.startCreatureSoundLevel(Z2SE_INSCT_KSKS, 0, -1);
}

void daObjDAN_c::DamageAction() {
    daPy_py_c* player = daPy_getPlayerActorClass();

    switch (mMode) {
    case 0:
        if (fopAcM_searchPlayerDistanceXZ(this) < 50.0f && player->speedF > 0.0f) {
            current.angle.y = player->current.angle.y;
            shape_angle.y = current.angle.y;
            mTargetAngleY = current.angle.y;
            speedF = player->speedF * 1.5f;
            mMode++;
            mTargetSpeed = 0.0f;
            mpMorf->setAnm(static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("I_Dan", 6)),
                           J3DFrameCtrl::EMode_LOOP, 5.0f, 1.0f, 0.0f, -1.0f);
            mTimer[0] = 100;
            gravity = -3.0f;
            speed.y = player->speedF * 0.5f;
            if (speed.y > 15.0f) {
                speed.y = 15.0f;
            } else if (speed.y < 10.0f) {
                speed.y = 10.0f;
            }
        }
        break;

    case 1:
        if (mAcch.ChkGroundHit()) {
            speed.y *= -0.5f;
            mAcch.ClrGroundLanding();
            mBounceCount++;
            if (mBounceCount > 5) {
                mBounceCount = 0;
                speed.y = 0.0f;
                field_0x7a0 = 0.0f;
                speedF = 0.0f;
                mTargetSpeed = 0.0f;
            }
            home.pos = current.pos;
        }

        if (fopAcM_searchPlayerDistanceXZ(this) < 50.0f && player->speedF > 0.0f
            && mTimer[1] == 0 && mAcch.ChkGroundHit())
        {
            mTargetAngleY = player->shape_angle.y;
            current.angle.y = mTargetAngleY;
            speedF = player->speedF * 2.0f;
            mTimer[0] = 100;
            mBounceCount = 0;
            speed.y = player->speedF;
            if (speed.y > 15.0f) {
                speed.y = 15.0f;
            } else if (speed.y < 10.0f) {
                speed.y = 10.0f;
            }
        }

        if (mTimer[0] == 0 && fopAcM_searchPlayerDistanceXZ(this) > 50.0f) {
            shape_angle.x = 0;
            mpMorf->setAnm(static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("I_Dan", 7)),
                           J3DFrameCtrl::EMode_NONE, 5.0f, 1.0f, 0.0f, -1.0f);
            mMode++;
            speed.y = 0.0f;
            speedF = 0.0f;
            mTargetSpeed = 0.0f;
            mBounceCount = 0;
        }

        if (speed.y != 0.0f) {
            field_0x78a += 0x500;
        }
        break;

    case 2:
        if (cLib_chaseAngleS(&field_0x78a, 0, 0x500) && mpMorf->isStop()) {
            mAction = ACT_MOVE;
            mMode = 0;
            mTargetSpeed = 0.5f;
            mpMorf->setAnm(static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("I_Dan", 8)),
                           J3DFrameCtrl::EMode_LOOP, 5.0f, 1.0f, 0.0f, -1.0f);
        }
        break;
    }

    SpeedSet();
}

void daObjDAN_c::Action() {
    for (int i = 0; i < 3; i++) {
        mTimer[i]--;
        if (mTimer[i] <= 0) {
            mTimer[i] = 0;
        }
    }

    switch (mAction) {
    case ACT_MOVE:
        MoveAction();
        break;
    case ACT_DAMAGE:
        DamageAction();
        break;
    }

    Insect_GetDemoMain();
}

void daObjDAN_c::checkGroundPos() {
    cXyz pos = current.pos;
    mGndChk.SetPos(&pos);
    mGroundH = dComIfG_Bgsp().GroundCross(&mGndChk);
}

void daObjDAN_c::Insect_Release() {
    field_0x56c = 1;
    mAction = ACT_MOVE;
}

static u8 const l_dan_itemno[2] = {fpcNm_ITEM_M_DANGOMUSHI, fpcNm_ITEM_F_DANGOMUSHI};

void daObjDAN_c::Z_BufferChk() {
    cXyz vec2, vec1;
    vec1 = current.pos;
    vec1.y += 20.0f;
    mDoLib_project(&vec1, &vec2);
    f32 trim_height;
    camera_class* camera = dComIfGp_getCamera(0);
    if (camera != NULL) {
        trim_height = camera->mCamera.TrimHeight();
    } else {
        trim_height = 0.0f;
    }
    if (vec2.x > 0.0f && vec2.x < FB_WIDTH && vec2.y > trim_height && vec2.y < FB_HEIGHT - trim_height) {
        dComIfGd_peekZ(vec2.x, vec2.y, &mBufferZ);
    }

    f32 near = dComIfGd_getView()->near;
    f32 far = dComIfGd_getView()->far;
    mDoLib_pos2camera(&vec1, &vec2);
    vec2.z += 50.0f;
    if (vec2.z > 0.0f) {
        vec2.z = 0.0f;
    }
    mScreenZ = ((near + far * near / vec2.z) / (far - near) + 1.0f) * 0xffffff;
}

void daObjDAN_c::ParticleSet() {
    if (mScreenZ > mBufferZ) {
        cLib_addCalc2(&mParticleScale, 0.0f, 1.0f, 1.0f);
    } else {
        cLib_addCalc2(&mParticleScale, 1.0f, 1.0f, 1.0f);
    }
    mParticleKey1 = dComIfGp_particle_set(mParticleKey1, 0xa1b, &current.pos, &tevStr,
                                          &shape_angle, NULL, 0xff, NULL, -1, NULL, NULL, NULL);
    f32 scale = mParticleScale;
    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mParticleKey1);
    if (emitter != NULL) {
        emitter->setGlobalParticleScale(scale, scale);
    }
}

void daObjDAN_c::ObjHit() {
    if (mCcSph.ChkTgHit()) {
        cCcD_Obj* hit_obj = mCcSph.GetTgHitObj();
        if (hit_obj->ChkAtType(AT_TYPE_BOOMERANG)) {
            if (!mBoomerangHit) {
                mBoomerangHit = true;
                mAcch.ClrMoveBGOnly();
                mAcch.ClrGrndNone();
                mCcSph.GetTgHitGObj();
                mBoomerangMove.initOffset(&current.pos);
            }
        } else if (!hit_obj->ChkAtType(AT_TYPE_NORMAL_SWORD)) {
            cXyz offset(0.0f, 0.0f, 0.0f);
            daPy_getPlayerActorClass()->setHookshotCarryOffset(fopAcM_GetID(this), &offset);
            mAction = ACT_DAMAGE;
            mMode = 1;
            mpMorf->setAnm(static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("I_Dan", 6)),
                           J3DFrameCtrl::EMode_LOOP, 5.0f, 1.0f, 0.0f, -1.0f);
            mTimer[0] = 100;
        }
        mCcSph.ClrTgHit();
    }
}

static f32 dummy() {
    return 100.0f;
}

void daObjDAN_c::BoomChk() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    
    if (mBoomerangHit) {
        if (mBoomerangMove.posMove(&current.pos, &shape_angle.y, NULL, 0x1c00)) {
            speed.y = 0.0f;
            mBoomerangMove.bgCheckAfterOffset(&current.pos);
            current.pos.y += 0.5f;
            home.pos = current.pos;
            gravity = 0.0f;
        } else {
            speed.y = 30.0f;
            mBoomerangMove.bgCheckAfterOffset(&current.pos);
            mBoomerangHit = false;
            gravity = -3.0f;
            mpMorf->setAnm(static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("I_Dan", 6)),
                           J3DFrameCtrl::EMode_LOOP, 5.0f, 1.0f, 0.0f, -1.0f);
            mTimer[0] = 100;
            mAction = ACT_DAMAGE;
            mMode = 1;
            current.pos.y = old.pos.y = player->current.pos.y + 150.0f;
        }
    }
}

int daObjDAN_c::Execute() {
    if (ChkGetDemo()) {
        mScreenZ = mBufferZ + 10000.0f;
        mCreatureSound.startCreatureSoundLevel(Z2SE_INSCT_KIRA, 0, -1);
        Insect_GetDemoMain();
        ParticleSet();
        return 1;
    }

    checkGroundPos();
    attention_info.position = current.pos;
    eyePos = current.pos;
    eyePos.y += 10.0f;

    switch (mLocation) {
    case LOC_OUTSIDE:
        if (!fopAcM_checkHookCarryNow(this)) {
            Action();
        } else {
            speed.y = 0.0f;
            home.pos = current.pos;
        }

        mParticleKey2 = dComIfGp_particle_set(mParticleKey2, 0xa1c, &current.pos, &tevStr,
                                              &shape_angle, NULL, 0xff, NULL, -1, NULL, NULL, NULL);
        ObjHit();
        BoomChk();
        eyePos = current.pos;
        eyePos.y += 10.0f;
        attention_info.position = eyePos;
        attention_info.position.y += 10.0f;
        SetCcSph();
        ParticleSet();
        break;

    case LOC_AGITHA:
        for (int i = 0; i < 3; i++) {
            mTimer[i]--;
            if (mTimer[i] <= 0) {
                mTimer[i] = 0;
            }
        }
        
        switch (mAction) {
        case ACT_MOVE:
            MoveAction();
            break;
        case ACT_DAMAGE:
            DamageAction();
            break;
        }

        break;
    }

    mCreatureSound.startCreatureSoundLevel(Z2SE_INSCT_KIRA, 0, -1);
    fopAcM_posMoveF(this, mCcStts.GetCCMoveP());
    mAcch.CrrPos(dComIfG_Bgsp());
    mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mpBrkAnm->play();
    mpBtkAnm->play();
    setBaseMtx();
    return 1;
}

int daObjDAN_c::Delete() {
    dComIfG_resDelete(&mPhase, "I_Dan");
    if (mHIOInit) {
        hioInit = false;
    }
    if (heap != NULL) {
        mpMorf->stopZelAnime();
    }
    return 1;
}

void daObjDAN_c::setBaseMtx() {
    cXyz vec(0.0f, -4.0f, 0.0f);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(scale);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->modelCalc();
}

int daObjDAN_c::Draw() {
    if (mDraw) {
        Z_BufferChk();
        J3DModel* model = mpMorf->getModel();
        g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(model, &tevStr);
        mpBtkAnm->entry(model->getModelData());
        mpBrkAnm->entry(model->getModelData());
        mpMorf->entryDL();
        if (mLocation == LOC_OUTSIDE) {
            dComIfGd_setSimpleShadow(&current.pos, mAcch.GetGroundH(), 15.0f, mAcch.m_gnd, 0, -0.6f,
                                     dDlst_shadowControl_c::getSimpleTex());
        }
    }
    return 1;
}

static int daObjDAN_Draw(daObjDAN_c* i_this) {
    return i_this->Draw();
}

static int daObjDAN_Execute(daObjDAN_c* i_this) {
    return i_this->Execute();
}

static u16 const l_musiya_num[2] = {
    0x019B, /* dSv_event_flag_c::F_0411 - Misc. - Pill bug (M) */
    0x019C, /* dSv_event_flag_c::F_0412 - Misc. - Pill bug (F) */
};

bool daObjDAN_c::CreateChk() {
    u8 param = (fopAcM_GetParam(this) >> 8) & 0xf;
    if (param == 0xf) {
        param = 0;
    }

    if (mLocation != LOC_UNK_2) {
        if (param != 0) {
            if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[l_musiya_num[SEX_MALE]])
                || !dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[l_musiya_num[SEX_FEMALE]]))
            {
                return false;
            }
        } else {
            if (!strcmp("R_SP160", dComIfGp_getStartStageName())
                && dComIfGp_getStartStageRoomNo() == 3)
            {
                if (dMenu_Insect_c::isCatchNotGiveInsect(l_dan_itemno[mSex])) {
                    return false;
                }
                if ((mSex == SEX_MALE && !dComIfGs_isEventBit(
                        dSv_event_flag_c::saveBitLabels[l_musiya_num[mSex]]))
                    || (mSex == SEX_FEMALE && !dComIfGs_isEventBit(
                        dSv_event_flag_c::saveBitLabels[l_musiya_num[mSex]])))
                {
                    return false;
                }
            }
        }
    }

    return true;
}

cPhs__Step daObjDAN_c::create() {
    fopAcM_ct(this, daObjDAN_c);
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhase, "I_Dan");

    if (step == cPhs_COMPLEATE_e) {
        mLocation = fopAcM_GetParam(this) & 0xf;
        if (mLocation == LOC_UNK_2) {
            field_0x56c = 0;
            shape_angle.x -= 0x2000;
            fopAcM_OnStatus(this, 0x4000);
        } else {
            mDraw = true;
        }

        mSex = (fopAcM_GetParam(this) >> 4) & 1;
        attention_info.distances[fopAc_attn_CARRY_e] = 0x5D;
        setItemNo(l_dan_itemno[mSex]);
        setSaveBitNo(l_musiya_num[mSex]);

        if (mSex == SEX_FEMALE) {
            scale.set(l_HIO.mScaleFemale, l_HIO.mScaleFemale, l_HIO.mScaleFemale);
        } else if (mSex == SEX_MALE) {
            scale.set(l_HIO.mScaleMale, l_HIO.mScaleMale, l_HIO.mScaleMale);
        }

        if (!CreateChk()) {
            return cPhs_ERROR_e;
        }

        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x1200)) {
            return cPhs_ERROR_e;
        }

        if (!hioInit) {
            hioInit = true;
            mHIOInit = true;
            l_HIO.field_0x4 = -1;
        }

        mAcchCir.SetWall(30.0f, 50.0f);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), NULL, NULL);
        mTargetSpeed = 0.5f;
        gravity = -3.0f;
        mAction = ACT_MOVE;

        if (mLocation == LOC_OUTSIDE) {
            InitCcSph();
        }
        if (mLocation == LOC_UNK_2 && !strcmp("R_SP160", dComIfGp_getStartStageName())
            && dComIfGp_getStartStageRoomNo() == 3)
        {
            mLocation &= 0xf0;
            gravity = -3.0f;
            mAction = ACT_DAMAGE;
            mMode = 1;
            mpMorf->setAnm(static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("I_Dan", 6)),
                           J3DFrameCtrl::EMode_LOOP, 5.0f, 1.0f, 0.0f, -1.0f);
            mTimer[0] = 100;
        }

        mAcch.OffClrSpeedY();

        dBgS_ObjGndChk gnd_chk;
        cXyz pos = current.pos;
        pos.y += 100.0f;
        gnd_chk.SetPos(&pos);
        f32 cross = dComIfG_Bgsp().GroundCross(&gnd_chk);
        if (cross != -G_CM3D_F_INF) {
            current.pos.y = cross;
        }

        mBounceCount = 0;
        fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -50.0f, -50.0f, -50.0f);
        fopAcM_SetMax(this, 50.0f, 50.0f, 50.0f);

        mCreatureSound.init(&current.pos, &eyePos, 3, 1);
        daObjDAN_Execute(this);
    }

    return step;
}

static int daObjDAN_IsDelete(daObjDAN_c* i_this) {
    return 1;
}

static actor_method_class l_daObjDAN_Method = {
    (process_method_func)daObjDAN_Create,
    (process_method_func)daObjDAN_Delete,
    (process_method_func)daObjDAN_Execute,
    (process_method_func)daObjDAN_IsDelete,
    (process_method_func)daObjDAN_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Dan = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Obj_Dan,
    &g_fpcLf_Method.base,
    sizeof(daObjDAN_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x1E1,
    &l_daObjDAN_Method,
    0xC0120,
    fopAc_ENV_e,
    fopAc_CULLBOX_CUSTOM_e,
};
