/**
 * d_a_obj_ari.cpp
 * Object - Golden Ant
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_ari.h"
#include "SSystem/SComponent/c_math.h"
#include "m_Do/m_Do_lib.h"
#include "d/d_com_inf_game.h"
#include "d/d_menu_insect.h"
#include "d/d_procname.h"
#include "f_op/f_op_camera_mng.h"

static bool hioInit;

static daObj_AriHIO_c l_HIO;

daObj_AriHIO_c::daObj_AriHIO_c() {
    field_0x4 = -1;
    mScaleMale = 1.5f;
    mScaleFemale = 2.0f;
}

static u8 const l_ari_itemno[2] = {fpcNm_ITEM_M_ANT, fpcNm_ITEM_F_ANT};

void daObjARI_c::InitCcSph() {
    const static dCcD_SrcSph ccSphSrc = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0x14002, 0x11}, 0x0}}, // mObj
            {dCcD_SE_STONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 20.0f} // mSph
        } // mSphAttr
    };

    mCcStts.Init(1, 0, this);
    mCcSph.Set(ccSphSrc);
    mCcSph.SetStts(&mCcStts);
    mCcSph.SetC(current.pos);
    mCcSph.OnTgNoHitMark();
}

void daObjARI_c::SetCcSph() {
    mCcSph.SetC(current.pos);
    mCcSph.SetR(40.0f);
    dComIfG_Ccsp()->Set(&mCcSph);
}

int daObjARI_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    int jnt_no = i_joint->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jnt_no));
    if (jnt_no == 1) {
        mDoMtx_stack_c::YrotM(mLegAngleY * 0.2f);
    }
    i_model->setAnmMtx(jnt_no, mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    return 1;
}

static int JointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();
        daObjARI_c* _this = (daObjARI_c*)model->getUserArea();
        if (_this != NULL) {
            _this->ctrlJoint(i_joint, model);
        }
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return static_cast<daObjARI_c*>(i_this)->CreateHeap();
}

int daObjARI_c::CreateHeap() {
    J3DModelData* model_data;
    if (mSex == SEX_FEMALE) {
        model_data = static_cast<J3DModelData*>(dComIfG_getObjectRes("I_Ari", 9));
    } else {
        model_data = static_cast<J3DModelData*>(dComIfG_getObjectRes("I_Ari", 10));
    }
    mpMorf = new mDoExt_McaMorfSO(model_data, NULL, NULL,
                                  static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("I_Ari", 6)),
                                  2, 0.5f, 0, -1, &mCreatureSound, 0, 0x11000284);
    if (mpMorf == NULL || mpMorf->getModel() == NULL) {
        return 0;
    }
    J3DModel* model = mpMorf->getModel();

    mpBrkAnm = new mDoExt_brkAnm();
    if (mpBrkAnm == NULL) {
        return 0;
    }
    if (mSex == SEX_FEMALE) {
        J3DAnmTevRegKey* tev_anm = static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes("I_Ari", 13));
        if (!mpBrkAnm->init(model->getModelData(), tev_anm, 1, 2, 1.0f, 0, -1)) {
            return 0;
        }
    } else {
        J3DAnmTevRegKey* tev_anm = static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes("I_Ari", 14));
        if (!mpBrkAnm->init(model->getModelData(), tev_anm, 1, 2, 1.0f, 0, -1)) {
            return 0;
        }
    }

    mpBtkAnm = new mDoExt_btkAnm();
    if (mpBtkAnm == NULL) {
        return 0;
    }
    if (mSex == SEX_FEMALE) {
        J3DAnmTextureSRTKey* tex_anm =
            static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes("I_Ari", 17));
        if (!mpBtkAnm->init(model->getModelData(), tex_anm, 1, 2, 1.0f, 0, -1)) {
            return 0;
        }
    } else {
        J3DAnmTextureSRTKey* tex_anm =
            static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes("I_Ari", 18));
        if (!mpBtkAnm->init(model->getModelData(), tex_anm, 1, 2, 1.0f, 0, -1)) {
            return 0;
        }
    }

    return 1;
}

static cPhs__Step daObjARI_Create(fopAc_ac_c* i_this) {
    fopAcM_GetID(i_this);
    return static_cast<daObjARI_c*>(i_this)->create();
}

static int daObjARI_Delete(daObjARI_c* i_this) {
    fopAcM_GetID(i_this);
    i_this->Delete();
    return 1;
}

static f32 dummy() {
    cXyz vec1(0.8f, 0.0f, 0.4f);
    cXyz vec2(10.0f, 0.02f, 0.0f);
    return vec1.x + vec2.x;
}

void daObjARI_c::WallWalk() {
    cXyz vec1(0.0f, -50.0f, speedF * 2.0f);
    cXyz vec2(0.0f, 0.0f, 0.0f);
    cXyz vec3(0.0f, 50.0f, 0.0f);
    cXyz vec4(0.0f, 0.0f, 0.0f);
    cXyz vec5(0.0f, 50.0f, speedF);
    cXyz vec6(0.0f, 0.0f, 0.0f);
    cXyz vec7(0.0f, -50.0f, 0.0f);
    cXyz vec8(0.0f, 0.0f, 0.0f);

    dBgS_LinChk lin_chk1, lin_chk2;
    lin_chk1.SetObj();
    lin_chk2.SetObj();

    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(mWallAlignAngle);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::multVec(&vec1, &vec2);
    mDoMtx_stack_c::multVec(&vec3, &vec4);
    mDoMtx_stack_c::multVec(&vec5, &vec6);
    mDoMtx_stack_c::multVec(&vec7, &vec8);
    
    lin_chk1.Set(&vec4, &vec2, NULL);
    lin_chk2.Set(&vec6, &vec8, NULL);

    if (dComIfG_Bgsp().LineCross(&lin_chk1)) {
        speedF = 0.3f;
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(lin_chk1, &plane);
        const cXyz* normal = plane.GetNP();
        s16 normal_ang_y = cM_atan2s(normal->x, normal->z);
        cXyz zero(0.0f, 0.0f, 0.0f);
        cXyz normal_yz(0.0f, normal->y, normal->z);
        f32 normal_len_yz = zero.abs(normal_yz);
        mTargetPos = lin_chk1.GetCross();
        mDownAngleY = normal_ang_y;
        mWallAngle.z = -cM_atan2s(normal->x, normal_len_yz);
        mWallAngle.x = cM_atan2s(normal->z, normal->y);
        mDownAngleY = cM_atan2s(normal->x, normal->z);
    } else if (dComIfG_Bgsp().LineCross(&lin_chk2)) {
        speedF = 0.3f;
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(lin_chk2, &plane);
        const cXyz* normal = plane.GetNP();
        s16 normal_ang_y = cM_atan2s(normal->x, normal->z);
        cXyz zero(0.0f, 0.0f, 0.0f);
        cXyz normal_yz(0.0f, normal->y, normal->z);
        f32 normal_len_yz = zero.abs(normal_yz);
        mTargetPos = lin_chk2.GetCross();
        mDownAngleY = normal_ang_y;
        mWallAngle.z = -cM_atan2s(normal->x, normal_len_yz);
        mWallAngle.x = cM_atan2s(normal->z, normal->y);
        mDownAngleY = cM_atan2s(normal->x, normal->z);
    } else {
        mTargetAngleY += 0x100;
    }

    cLib_chasePos(&current.pos, mTargetPos, speedF);
    old.pos = current.pos;
    cLib_addCalcAngleS2(&mWallAlignAngle.x, mWallAngle.x, 5, 0x200);
    cLib_addCalcAngleS2(&mWallAlignAngle.z, mWallAngle.z, 5, 0x200);
    cLib_addCalcAngleS2(&current.angle.y, mTargetAngleY, 0x10, 0x100);
    shape_angle.y = current.angle.y;
}

void daObjARI_c::MoveAction() {
    if (mTimer == 0) {
        cXyz delta = home.pos - current.pos;
        delta.x += cM_rndFX(1050.0f);
        delta.z += cM_rndFX(1050.0f);
        mTargetAngleY = cM_atan2s(delta.x, delta.z);
        mTimer = cM_rndF(400.0f) + 400.0f;
    }

    field_0x5ec = 0.6f;
    cLib_addCalcAngleS2(&current.angle.y, mTargetAngleY, 21, 0x50);
    cLib_addCalcAngleS2(&mLegAngleY, mTargetAngleY - current.angle.y, 8, 0x100);
    shape_angle.y = current.angle.y;

    WallWalk();
}

void daObjARI_c::Action() {
    mTimer--;
    if (mTimer <= 0) {
        mTimer = 0;
    }

    if (fopAcM_searchPlayerDistance(this) < 1500.0f) {
        MoveAction();
    }

    Insect_GetDemoMain();
}

void daObjARI_c::Insect_Release() {
    field_0x56c = 1;
    mAction = 1;
}

void daObjARI_c::checkGround() {
    dBgS_LinChk lin_chk;
    lin_chk.SetObj();
    lin_chk.Set(&old.pos, &current.pos, NULL);
    
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
        current.pos = lin_chk.GetCross();
        const cXyz* normal = plane.GetNP();
        cXyz zero(0.0f, 0.0f, 0.0f);
        cXyz normal_yz(0.0f, normal->y, normal->z);
        mWallAngle.z = mWallAlignAngle.z = -cM_atan2s(normal->x, zero.abs(normal_yz));
        mWallAngle.x = mWallAlignAngle.x = cM_atan2s(normal->z, normal->y);
        mDownAngleY = cM_atan2s(normal->x, normal->z);
        shape_angle.x = 0;
        mMode++;
    }
}

void daObjARI_c::B_MoveAction() {
    switch (mMode) {
    case 0:
        if (!fopAcM_checkHookCarryNow(this)) {
            speed.y += -2.0f;
            current.pos.y += speed.y;
            shape_angle.x = 0x4000;
            checkGround();
        }
        break;

    case 1:
        mAction = 0;
        mMode = 0;
        mLocation &= 0xf0;
        shape_angle.x = 0;
        break;
    }
}

void daObjARI_c::FallAction() {
    switch (mAction) {
    case 0:
        break;

    case 1:
        B_MoveAction();
        break;
    }
}

void daObjARI_c::ParticleSet() {
    if (mScreenZ > mBufferZ) {
        cLib_addCalc2(&mParticleScale, 0.0f, 1.0f, 1.0f);
    } else {
        cLib_addCalc2(&mParticleScale, mParticleTargetScale, 1.0f, 1.0f);
    }
    mParticleKey1 = dComIfGp_particle_set(mParticleKey1, 0xa1b, &current.pos, &tevStr,
                                          &shape_angle, NULL, 0xff, NULL, -1, NULL, NULL, NULL);
    f32 scale = mParticleScale;
    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mParticleKey1);
    if (emitter != NULL) {
        emitter->setGlobalParticleScale(scale, scale);
    }
}

void daObjARI_c::BoomChk() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    fopAcM_searchPlayerDistanceXZ(this);
    
    if (mBoomerangHit) {
        home.pos = current.pos;
        cXyz player_pos = player->current.pos;
        player_pos.y += 100.0f;

        if (mBoomerangMove.posMove(&current.pos, &shape_angle.y, NULL, 0x1c00)) {
            mBoomerangMove.bgCheckAfterOffset(&current.pos);
            current.pos.y += 0.5f;
        } else {
            dBgS_LinChk lin_chk;
            lin_chk.SetObj();
            lin_chk.Set(&player_pos, &current.pos, NULL);
            
            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                cM3dGPla plane;
                dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
                old.pos = lin_chk.GetCross();
                current.pos = old.pos;
                mAction = 0;
                mMode = 0;
                const cXyz* normal = plane.GetNP();
                cXyz zero(0.0f, 0.0f, 0.0f);
                cXyz normalYZ(0.0f, normal->y, normal->z);
                mWallAngle.z = mWallAlignAngle.z = -cM_atan2s(normal->x, zero.abs(normalYZ));
                mWallAngle.x = mWallAlignAngle.x = cM_atan2s(normal->z, normal->y);
                mDownAngleY = cM_atan2s(normal->x, normal->z);
            } else {
                current.pos.y = old.pos.y = player->current.pos.y + 150.0f;
                mLocation = LOC_UNK_3;
                mAction = 1;
                mMode = 0;
            }

            mBoomerangHit = false;
        }
    }
}

void daObjARI_c::ObjHit() {
    if (mCcSph.ChkTgHit()) {
        cCcD_Obj* hit_obj = mCcSph.GetTgHitObj();
        if (hit_obj->ChkAtType(AT_TYPE_BOOMERANG)) {
            if (!mBoomerangHit) {
                mBoomerangHit = true;
                mCcSph.GetTgHitGObj();
                mBoomerangMove.initOffset(&current.pos);
            }
        } else if (!hit_obj->ChkAtType(AT_TYPE_NORMAL_SWORD)
                    && hit_obj->ChkAtType(AT_TYPE_HOOKSHOT)) {
            cXyz offset(0.0f, 0.0f, 0.0f);
            daPy_getPlayerActorClass()->setHookshotCarryOffset(fopAcM_GetID(this), &offset);
            mBoomerangHit = false;
            mLocation = LOC_UNK_3;
            mAction = 1;
            mMode = 0;
        }
        mCcSph.ClrTgHit();
    }
}

int daObjARI_c::Execute() {
    if (ChkGetDemo()) {
        mScreenZ = mBufferZ + 10000.0f;
        mCreatureSound.startCreatureSoundLevel(Z2SE_INSCT_KIRA, 0, -1);
        Insect_GetDemoMain();
        ParticleSet();
        return 1;
    }

    attention_info.position = current.pos;
    eyePos = current.pos;
    eyePos.y += 10.0f;

    switch (mLocation) {
    case LOC_OUTSIDE:
        if (!fopAcM_checkHookCarryNow(this)) {
            Action();
        } else {
            home.pos = current.pos;
        }

        mParticleKey2 = dComIfGp_particle_set(mParticleKey2, 0xa1c, &current.pos, &tevStr,
                                              &shape_angle, NULL, 0xff, NULL, -1, NULL, NULL, NULL);
        SetCcSph();
        ObjHit();
        BoomChk();
        eyePos = current.pos;
        eyePos.y += 10.0f;
        attention_info.position = eyePos;
        attention_info.position.y += 10.0f;
        ParticleSet();
        break;

    case LOC_UNK_3:
        FallAction();
        mParticleKey2 = dComIfGp_particle_set(mParticleKey2, 0xa1c, &current.pos, &tevStr,
                                              &shape_angle, NULL, 0xff, NULL, -1, NULL, NULL, NULL);
        SetCcSph();
        ObjHit();
        BoomChk();
        eyePos = current.pos;
        eyePos.y += 10.0f;
        attention_info.position = eyePos;
        attention_info.position.y += 10.0f;
        ParticleSet();
        break;

    case LOC_AGITHA:
        mTimer--;
        if (mTimer <= 0) {
            mTimer = 0;
        }

        MoveAction();

        if (!field_0x5fc) {
            cXyz offset(0.0f, 0.0f, 200.0f);
            cXyz vec2(0.0f, 0.0f, 0.0f);
            dBgS_LinChk lin_chk;
            lin_chk.SetObj();
            cXyz vec3;
            vec3 = current.pos;
            vec3.y += 100.0f;
            vec2.set(current.pos.x, current.pos.y - 4000.0f, current.pos.z);
            lin_chk.Set(&vec3, &vec2, NULL);
            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                cM3dGPla plane;
                dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
                const cXyz* normal = plane.GetNP();
                current.pos = lin_chk.GetCross();
                mTargetPos = current.pos;
                old.pos = current.pos;
                cXyz zero(0.0f, 0.0f, 0.0f);
                cXyz normalYZ(0.0f, normal->y, normal->z);
                mWallAngle.z = mWallAlignAngle.z = -cM_atan2s(normal->x, zero.abs(normalYZ));
                mWallAngle.x = mWallAlignAngle.x = cM_atan2s(normal->z, normal->y);
                mDownAngleY = cM_atan2s(normal->x, normal->z);
                field_0x5fc = true;
            }

            vec2.set(0.0f, 0.0f, 0.0f);
            for (int i = 0; i < 4; i++) {
                cLib_offsetPos(&vec2, &current.pos, i * 160000.0f, &offset);
                lin_chk.Set(&current.pos, &vec2, NULL);
                if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                    cM3dGPla plane;
                    dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
                    const cXyz* normal = plane.GetNP();
                    current.pos = lin_chk.GetCross();
                    mTargetPos = current.pos;
                    old.pos = current.pos;
                    cXyz zero(0.0f, 0.0f, 0.0f);
                    cXyz normalYZ(0.0f, normal->y, normal->z);
                    mWallAngle.z = mWallAlignAngle.z = -cM_atan2s(normal->x, zero.abs(normalYZ));
                    mWallAngle.x = mWallAlignAngle.x = cM_atan2s(normal->z, normal->y);
                    mDownAngleY = cM_atan2s(normal->x, normal->z);
                    field_0x5fc = true;
                }
            }
        }

        break;
    }

    if (mBoomerangHit) {
        mAcch.CrrPos(dComIfG_Bgsp());
    }

    mpBrkAnm->play();
    mpBtkAnm->play();
    mCreatureSound.startCreatureSoundLevel(Z2SE_INSCT_KSKS, 0, -1);
    mCreatureSound.startCreatureSoundLevel(Z2SE_INSCT_KIRA, 0, -1);
    mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    setBaseMtx();

    return 1;
}

void daObjARI_c::Z_BufferChk() {
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

int daObjARI_c::Delete() {
    dComIfG_resDelete(&mPhase, "I_Ari");
    if (mHIOInit) {
        hioInit = false;
    }
    if (heap != NULL) {
        mpMorf->stopZelAnime();
    }
    return 1;
}

void daObjARI_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(mWallAlignAngle);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(scale);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->modelCalc();
}

int daObjARI_c::Draw() {
    if (mDraw) {
        Z_BufferChk();
        J3DModel* model = mpMorf->getModel();
        g_env_light.settingTevStruct(0, &current.pos, &tevStr);
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

static int daObjARI_Draw(daObjARI_c* i_this) {
    return i_this->Draw();
}

static int daObjARI_Execute(daObjARI_c* i_this) {
    return i_this->Execute();
}

static u16 const l_musiya_num[2] = {
    0x01A5, /* dSv_event_flag_c::F_0421 - Misc. - Ant (M) */
    0x01A6, /* dSv_event_flag_c::F_0422 - Misc. - Ant (F) */
};


bool daObjARI_c::CreateChk() {
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
                if (dMenu_Insect_c::isCatchNotGiveInsect(l_ari_itemno[mSex])) {
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

cPhs__Step daObjARI_c::create() {
    fopAcM_ct(this, daObjARI_c);
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhase, "I_Ari");

    if (step == cPhs_COMPLEATE_e) {
        mLocation = fopAcM_GetParam(this) & 0xf;
        if (mLocation == LOC_UNK_2) {
            field_0x56c = 0;
            shape_angle.x -= 0x2000;
            fopAcM_OnStatus(this, 0x4000);
        } else {
            mDraw = true;
        }

        attention_info.distances[fopAc_attn_CARRY_e] = 0x5D;
        mSex = (fopAcM_GetParam(this) >> 4) & 1;
        setItemNo(l_ari_itemno[mSex]);
        setSaveBitNo(l_musiya_num[mSex]);

        if (mSex == SEX_FEMALE) {
            scale.set(l_HIO.mScaleFemale, l_HIO.mScaleFemale, l_HIO.mScaleFemale);
            mParticleTargetScale = 1.0f;
        } else if (mSex == SEX_MALE) {
            scale.set(l_HIO.mScaleMale, l_HIO.mScaleMale, l_HIO.mScaleMale);
            mParticleTargetScale = 1.1f;
        }

        if (!CreateChk()) {
            return cPhs_ERROR_e;
        }

        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0xaa0)) {
            return cPhs_ERROR_e;
        }

        if (!hioInit) {
            hioInit = true;
            mHIOInit = true;
            l_HIO.field_0x4 = -1;
        }

        gravity = 0.0f;
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), NULL, NULL);

        bool cross = false;
        cXyz vec1(0.0f, 0.0f, 500.0f);
        cXyz vec2(0.0f, 0.0f, -200.0f);
        cXyz vec3, vec4;
        dBgS_LinChk lin_chk;
        lin_chk.SetObj();
        for (int i = 0; i < 4; i++) {
            cLib_offsetPos(&vec3, &current.pos, shape_angle.y + 0x4000 * i, &vec1);
            cLib_offsetPos(&vec4, &current.pos, shape_angle.y + 0x4000 * i, &vec2);
            lin_chk.Set(&vec3, &vec4, NULL);
            lin_chk.SetObj();
            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                cM3dGPla plane;
                dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
                const cXyz* normal = plane.GetNP();
                current.pos = lin_chk.GetCross();
                cXyz zero(0.0f, 0.0f, 0.0f);
                cXyz normalYZ(0.0f, normal->y, normal->z);
                mWallAngle.z = mWallAlignAngle.z = -cM_atan2s(normal->x, zero.abs(normalYZ));
                mWallAngle.x = mWallAlignAngle.x = cM_atan2s(normal->z, normal->y);
                mDownAngleY = cM_atan2s(normal->x, normal->z);
                cross = true;
                old.pos = current.pos;
                break;
            }
        }
        
        if (!cross) {
            vec1.set(0.0f, 500.0f, 0.0f);
            vec3.set(0.0f, -1000.0f, 0.0f);
            lin_chk.Set(&vec1, &vec3, NULL);
            lin_chk.SetObj();
            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                cM3dGPla plane;
                dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
                const cXyz* normal = plane.GetNP();
                current.pos = lin_chk.GetCross();
                cXyz zero(0.0f, 0.0f, 0.0f);
                cXyz normalYZ(0.0f, normal->y, normal->z);
                mWallAngle.z = mWallAlignAngle.z = -cM_atan2s(normal->x, zero.abs(normalYZ));
                mWallAngle.x = mWallAlignAngle.x = cM_atan2s(normal->z, normal->y);
                mDownAngleY = cM_atan2s(normal->x, normal->z);
                old.pos = current.pos;
            }
        }

        J3DModel* model = mpMorf->getModel();
        model->setUserArea((uintptr_t)this);
        for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
            if (i != 0) {
                model->getModelData()->getJointNodePointer(i)->setCallBack(JointCallBack);
            }
        }

        if (mLocation == LOC_OUTSIDE) {
            InitCcSph();
        }
        if (mLocation == LOC_UNK_2 && !(!strcmp("R_SP160", dComIfGp_getStartStageName())
            && dComIfGp_getStartStageRoomNo() == 3))
        {
            InitCcSph();
        }

        mTargetPos = current.pos;

        fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -50.0f, -50.0f, -50.0f);
        fopAcM_SetMax(this, 50.0f, 50.0f, 50.0f);

        mCreatureSound.init(&current.pos, &eyePos, 3, 1);
        daObjARI_Execute(this);
    }

    return step;
}

static int daObjARI_IsDelete(daObjARI_c* i_this) {
    return 1;
}

static actor_method_class l_daObjARI_Method = {
    (process_method_func)daObjARI_Create,
    (process_method_func)daObjARI_Delete,
    (process_method_func)daObjARI_Execute,
    (process_method_func)daObjARI_IsDelete,
    (process_method_func)daObjARI_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Ari = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Obj_Ari,
    &g_fpcLf_Method.base,
    sizeof(daObjARI_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x1E4,
    &l_daObjARI_Method,
    0xC0100,
    fopAc_ENV_e,
    fopAc_CULLBOX_CUSTOM_e,
};
