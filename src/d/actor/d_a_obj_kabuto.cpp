/**
 * d_a_obj_kabuto.cpp
 * Object - Golden Beetle
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_kabuto.h"
#include "SSystem/SComponent/c_math.h"
#include "m_Do/m_Do_lib.h"
#include "d/d_com_inf_game.h"
#include "d/d_menu_insect.h"
#include "d/d_procname.h"
#include "f_op/f_op_camera_mng.h"

static bool hio_set;

static daObj_KabHIO_c l_HIO;

daObj_KabHIO_c::daObj_KabHIO_c() {
    field_0x4 = -1;
    field_0xc = 1.5f;
    field_0x8 = 1.2f;
    field_0x10 = -0.6f;
}

void daObjKABUTO_c::InitCcSph() {
    const static dCcD_SrcSph ccSphSrc = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0x14002, 0x11}, 0x0}}, // mObj
            {dCcD_SE_STONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 100.0f} // mSph
        } // mSphAttr
    };

    mCcStts.Init(1, 0, this);
    mCcSph.Set(ccSphSrc);
    mCcSph.SetStts(&mCcStts);
    mCcSph.SetC(current.pos);
    mCcSph.OnTgNoHitMark();
}

void daObjKABUTO_c::SetCcSph() {
    mCcSph.SetC(current.pos);
    mCcSph.SetR(20.0f);
    dComIfG_Ccsp()->Set(&mCcSph);
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return static_cast<daObjKABUTO_c*>(i_this)->CreateHeap();
}

int daObjKABUTO_c::CreateHeap() {
    J3DModelData* model_data;
    if (mSex == SEX_FEMALE) {
        model_data = static_cast<J3DModelData*>(dComIfG_getObjectRes("Kab_m", 10));
    } else {
        model_data = static_cast<J3DModelData*>(dComIfG_getObjectRes("Kab_m", 11));
    }
    mpMorf = new mDoExt_McaMorfSO(model_data, NULL, NULL,
                                  static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("Kab_m", 7)),
                                  2, 1.5f, 0, -1, &mCreatureSound, 0, 0x11000284);
    if (mpMorf == NULL || mpMorf->getModel() == NULL) {
        return 0;
    }
    J3DModel* model = mpMorf->getModel();

    mpBrkAnm = new mDoExt_brkAnm();
    if (mpBrkAnm == NULL) {
        return 0;
    }
    if (mSex == SEX_FEMALE) {
        J3DAnmTevRegKey* tev_anm = static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes("Kab_m", 14));
        if (!mpBrkAnm->init(model->getModelData(), tev_anm, 1, 2, 1.0f, 0, -1)) {
            return 0;
        }
    } else {
        J3DAnmTevRegKey* tev_anm = static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes("Kab_m", 15));
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
            static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes("Kab_m", 18));
        if (!mpBtkAnm->init(model->getModelData(), tex_anm, 1, 2, 1.0f, 0, -1)) {
            return 0;
        }
    } else {
        J3DAnmTextureSRTKey* tex_anm =
            static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes("Kab_m", 19));
        if (!mpBtkAnm->init(model->getModelData(), tex_anm, 1, 2, 1.0f, 0, -1)) {
            return 0;
        }
    }

    return 1;
}

static cPhs__Step daObjKABUTO_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjKABUTO_c*>(i_this)->create();
}

static int daObjKABUTO_Delete(daObjKABUTO_c* i_this) {
    i_this->Delete();
    return 1;
}

void daObjKABUTO_c::ShopWaitAction() {
    switch (mMode) {
    case 0: {
        speedF = 0.0f;
        J3DAnmTransform* anm = static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("Kab_m", 7));
        mpMorf->setAnm(anm, 2, 5.0f, 0.0f, 0.0f, -1.0f);
        mMode++;
        mTimers[0] = cM_rndF(50.0f) + 50.0f;
        break;
    }

    case 1:
        mTimers[0]--;
        if (mTimers[0] <= 0) {
            mTimers[0] = 0;
        }
        if (mTimers[0] == 0) {
            s32 rnd = cM_rndF(100.0f);
            if (rnd < 40) {
                mAction = ACT_WALK;
                mMode = 0;
            } else if (rnd < 70) {
                mAction = ACT_MOVE;
                mMode = 0;
            } else {
                mTimers[0] = cM_rndF(50.0f) + 50.0f;
            }
        }
        break;
    }
}

void daObjKABUTO_c::WaitAction() {
    switch (mMode) {
    case 0: {
        speedF = 0.0f;
        J3DAnmTransform* anm = static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("Kab_m", 7));
        mpMorf->setAnm(anm, 2, 5.0f, 0.0f, 0.0f, -1.0f);
        mMode++;
        mTimers[0] = cM_rndF(50.0f) + 50.0f;
        break;
    }

    case 1:
        if (fopAcM_searchActorDistance(this, daPy_getPlayerActorClass()) < 1500.0f) {
            mTimers[0]--;
            if (mTimers[0] <= 0) {
                mTimers[0] = 0;
            }
            if (mTimers[0] == 0 && (s32)cM_rndF(100.0f) < 90) {
                mAction = ACT_WALK;
                mMode = 0;
            }
        }
        break;
    }
}

BOOL daObjKABUTO_c::WallCheck() {
    dBgS_LinChk lin_chk;
    lin_chk.SetObj();
    lin_chk.Set(&old.pos, &current.pos, NULL);
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
        const cXyz* normal = plane.GetNP();
        current.pos = lin_chk.GetCross();
        cXyz zero(0.0f, 0.0f, 0.0f);
        cXyz normal_yz(0.0f, normal->y, normal->z);
        mWallAngle.z = -cM_atan2s(normal->x, zero.abs(normal_yz));
        mWallAngle.x = cM_atan2s(normal->z, normal->y);
        mDownAngleY = cM_atan2s(normal->x, normal->z);
        return true;
    }
    return false;
}

void daObjKABUTO_c::SpeedSet() {
    speed.y += gravity;
    cXyz speed_base(0.0f, speed.y, speedF);
    cXyz speed_real(0.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::ZXYrotS(mWallAngle);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::multVec(&speed_base, &speed_real);
    current.pos.x += speed_real.x;
    current.pos.y += speed_real.y;
    current.pos.z += speed_real.z;
    cLib_addCalc2(&speedF, mTargetSpeedXZ, 0.4f, 10.0f);
    cLib_addCalc2(&speed.y, mTargetSpeedY, 0.1f, 0.5f);
    cLib_addCalcAngleS2(&current.angle.y, mTargetAngleY, 0x10, 0x100);
    shape_angle.y = current.angle.y;
}

void daObjKABUTO_c::WallWalk() {
    cXyz vec1(0.0f, -100.0f, speedF * 3.0f);
    cXyz vec2(0.0f, 40.0f, 0.0f);
    cXyz vec3(0.0f, 20.0f, 100.0f);
    dBgS_LinChk lin_chk1, lin_chk2;
    lin_chk1.SetObj();
    lin_chk2.SetObj();
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(mWallAngle);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mDoMtx_stack_c::multVec(&vec1, &vec1);
    mDoMtx_stack_c::multVec(&vec2, &vec2);
    lin_chk1.Set(&vec2, &vec1, NULL);
    if (dComIfG_Bgsp().LineCross(&lin_chk1)) {
        speedF = 0.3f;
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(lin_chk1, &plane);
        const cXyz* normal = plane.GetNP();
        s16 normal_ang_y = cM_atan2s(normal->x, normal->z);
        cXyz cross;
        cXyz zero(0.0f, 0.0f, 0.0f);
        cXyz normal_yz(0.0f, normal->y, normal->z);
        f32 normal_len_yz = zero.abs(normal_yz);
        cross = lin_chk1.GetCross();
        if (normal_ang_y - mDownAngleY < 0x1000 && normal_ang_y - mDownAngleY > -0x1000
            && cross.abs(current.pos) < speedF * 3.0f)
        {
            current.pos = cross;
            mWallAngle.z = -cM_atan2s(normal->x, normal_len_yz);
            mWallAngle.x = cM_atan2s(normal->z, normal->y);
            mDownAngleY = normal_ang_y;
        } else {
            mTargetAngleY += 0x100;
        }
    } else {
        mTargetAngleY += 0x100;
    }
    cLib_addCalcAngleS2(&current.angle.y, mTargetAngleY, 0x10, 0x50);
    shape_angle.y = current.angle.y;
}

void daObjKABUTO_c::WalkAction() {
    for (int i = 0; i < 3; i++) {
        mTimers[i]--;
        if (mTimers[i] <= 0) {
            mTimers[i] = 0;
        }
    }

    J3DAnmTransform* anm;

    switch (mMode) {
    case 0:
        anm = static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("Kab_m", 7));
        mpMorf->setAnm(anm, 2, 5.0f, 1.5f, 0.0f, -1.0f);
        mTargetSpeedXZ = 0.2f;
        speed.y = 0.0f;
        mTargetSpeedY = 0.0f;
        mMode++;
        mTimers[1] = cM_rndF(50.0f) + 50.0f;
        break;

    case 1:
        if (mTimers[0] == 0) {
            cXyz target_pos(cM_rndFX(100.0f), 0.0f, cM_rndFX(100.0f));
            mTargetAngleY = cM_atan2s(target_pos.x, target_pos.z);
            mTimers[0] = cM_rndF(50.0f) + 50.0f;
        }

        if (mTimers[1] == 0) {
            s32 rnd = cM_rndF(100.0f);
            if (rnd < 30) {
                mAction = ACT_MOVE;
                mMode = 0;
            } else if (rnd < 60) {
                mAction = ACT_WAIT;
                mMode = 0;
            } else {
                mTimers[1] = cM_rndF(50.0f) + 50.0f;
            }
        }
        break;
    }

    WallWalk();
}

void daObjKABUTO_c::MoveAction() {
    for (int i = 0; i < 3; i++) {
        mTimers[i]--;
        if (mTimers[i] <= 0) {
            mTimers[i] = 0;
        }
    }

    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz vec(0.0f, 1.0f, 0.0f);
    cXyz vec2, pos;
    dBgS_LinChk lin_chk;
    J3DAnmTransform* anm;

    switch (mMode) {
    case 0:
        anm = static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("Kab_m", 6));
        mpMorf->setAnm(anm, 2, 5.0f, 0.0f, 0.0f, -1.0f);
        mMode++;
        mTimers[0] = 20;
        break;
    
    case 1:
        if (mTimers[0] == 10) {
            mpMorf->setPlaySpeed(1.0f);
        }

        if (mTimers[0] == 0) {
            mMode++;
            mDoMtx_stack_c::ZXYrotS(mWallAngle);
            mDoMtx_stack_c::multVec(&vec, &vec);
            mTimers[0] = cM_rndF(10.0f) + 30.0f;
            if (mLocation == LOC_OUTSIDE) {
                mTimers[1] = cM_rndF(80.0f) + 80.0f;
            } else {
                mTimers[1] = cM_rndF(50.0f) + 50.0f;
            }
            mTargetSpeedXZ = cM_rndF(5.0f) + 8.0f;
            mTargetAngleY = mDownAngleY;
            mTargetSpeedY = 3.0f;
            speed.y = 5.0f;
        }
        break;

    case 2:
        cXyz* flame_pos = player->getKandelaarFlamePos();
        if (flame_pos != NULL) {
            mTargetAngleY = cLib_targetAngleY(&current.pos, flame_pos);
            if (flame_pos->abs(current.pos) < 200.0f && speed.y < 0.0f) {
                cLib_addCalcAngleS2(&current.angle.y, mTargetAngleY, 2, 0x500);
            } else {
                cLib_addCalcAngleS2(&current.angle.y, mTargetAngleY, 0x10, 0x200);
            }
            if (flame_pos->abs(current.pos) < 11.0f && player->speedF < 1.0f) {
                vec2.set(0.0f, 0.0f, 10.0f);
                if (!field_0x735) {
                    mDownAngleY = cLib_targetAngleY(&current.pos, flame_pos);
                }
                cLib_offsetPos(&pos, flame_pos, mDownAngleY, &vec2);
                current.pos = pos;
                shape_angle.x = 0x4000;
                shape_angle.y = mDownAngleY;
                speedF = 0.0f;
                mTargetSpeedXZ = 0.0f;
                speed.y = 0.0f;
                mTargetSpeedY = 0.0f;
                mpMorf->setPlaySpeed(0.0f);
                mTargetAngleY = 0;
                current.angle.y = mDownAngleY;
                field_0x735 = true;
                anm = static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("Kab_m", 7));
                mpMorf->setAnm(anm, 2, 5.0f, 1.0f, 0.0f, -1.0f);
            } else if (field_0x735) {
                field_0x735 = false;
                mTargetSpeedY = 3.0f;
                speed.y = 3.0f;
                speedF = 8.0f;
                mTargetSpeedXZ = 8.0f;
                mpMorf->setPlaySpeed(1.0f);
                mDownAngleY = 0;
                anm = static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("Kab_m", 6));
                mpMorf->setAnm(anm, 2, 5.0f, 1.0f, 0.0f, -1.0f);
            }
        }

        if (mTimers[0] == 0) {
            if (flame_pos == NULL) {
                vec2 = home.pos - current.pos;
                vec2.x += cM_rndFX(1050.0f);
                vec2.z += cM_rndFX(1050.0f);
                mTargetAngleY = cM_atan2s(vec2.x, vec2.z);
            }
            mTimers[0] = cM_rndF(20.0f) + 20.0f;
            mTargetSpeedXZ = cM_rndF(5.0f) + 8.0f;
        }

        if (mTimers[1] == 0) {
            mTargetSpeedY = -cM_rndF(3.0f);
            mTimers[1] = cM_rndF(20.0f) + 20.0f;
        }

        SpeedSet();
        if (mLocation == LOC_OUTSIDE) {
            cLib_addCalcAngleS2(&mWallAngle.x, 0, 0x10, 0x1000);
            cLib_addCalcAngleS2(&mWallAngle.z, 0, 0x10, 0x1000);
        } else {
            cLib_addCalcAngleS2(&mWallAngle.x, 0, 0x10, 0x100);
            cLib_addCalcAngleS2(&mWallAngle.z, 0, 0x10, 0x100);
        }
        if (!field_0x735) {
            cLib_chaseAngleS(&shape_angle.x, 0, 0x400);
        }

        if (WallCheck()) {
            mAction = ACT_WAIT;
            mMode = 0;
            speedF = 0.0f;
            speed.y = 0.0f;
            shape_angle.x = 0;
            shape_angle.z = 0;
        }

        if (mLocation == LOC_AGITHA) {
            cLib_addCalcAngleS2(&current.angle.y, mTargetAngleY, 0x10, 0x100);
        }
        break;
    }

    if (mpMorf->getPlaySpeed() == 1.0f) {
        mCreatureSound.startCreatureSoundLevel(Z2SE_INSCT_BTBT, 0, -1);
    }
}

void daObjKABUTO_c::Z_BufferChk() {
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

void daObjKABUTO_c::Action() {
    switch (mAction) {
    case ACT_WAIT:
        WaitAction();
        break;
    case ACT_WALK:
        WalkAction();
        mCreatureSound.startCreatureSoundLevel(Z2SE_INSCT_KSKS, 0, -1);
        break;
    case ACT_MOVE:
        MoveAction();
        break;
    }
    Insect_GetDemoMain();
    mCreatureSound.startCreatureSoundLevel(Z2SE_INSCT_KIRA, 0, -1);
}

void daObjKABUTO_c::ShopAction() {
    switch (mAction) {
    case ACT_WAIT:
        ShopWaitAction();
        break;
    case ACT_WALK:
        WalkAction();
        mCreatureSound.startCreatureSoundLevel(Z2SE_INSCT_KSKS, 0, -1);
        break;
    case ACT_MOVE:
        MoveAction();
        break;
    }
    mCreatureSound.startCreatureSoundLevel(Z2SE_INSCT_KIRA, 0, -1);
}

void daObjKABUTO_c::Insect_Release() {
    field_0x56c = 1;
    mAction = ACT_MOVE;
    mMode = 0;
}

static u8 const l_kab_itemno[2] = {fpcNm_ITEM_M_BEETLE, fpcNm_ITEM_F_BEETLE};

// Some unused function went here.
// This fake function is here in its place to make the literals match
static f32 fake(f32 param_0) {
    return param_0 * -9.0f * -0.2f * 0.05f;
}

void daObjKABUTO_c::ParticleSet() {
    if (mScreenZ > mBufferZ) {
        cLib_chaseF(&mParticleScale, 0.0f, 1.0f);
    } else {
        cLib_chaseF(&mParticleScale, mParticleTargetScale, 1.0f);
    }
    mParticleKey1 = dComIfGp_particle_set(mParticleKey1, 0xa1b, &current.pos, &tevStr,
                                          &shape_angle, NULL, 0xff, NULL, -1, NULL, NULL, NULL);
    f32 scale = mParticleScale;
    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mParticleKey1);
    if (emitter != NULL) {
        emitter->setGlobalParticleScale(scale, scale);
    }
    emitter = dComIfGp_particle_getEmitter(mParticleKey2);
    if (emitter != NULL) {
        emitter->setGlobalParticleScale(scale, scale);
    }
}

int daObjKABUTO_c::Execute() {
    if (ChkGetDemo()) {
        mScreenZ = mBufferZ + 10000.0f;
        mCreatureSound.startCreatureSoundLevel(Z2SE_INSCT_KIRA, 0, -1);
        Insect_GetDemoMain();
        ParticleSet();
        return 1;
    }

    daPy_py_c* player = daPy_getPlayerActorClass();
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

        if (mBoomerangHit) {
            home.pos = current.pos;
            cXyz vec = player->current.pos;
            vec.y += 100.0f;
            if (mBoomerangMove.posMove(&current.pos, &shape_angle.y, NULL, 0x1c00)) {
                mBoomerangMove.bgCheckAfterOffset(&current.pos);
                current.pos.y += 0.5f;
            } else {
                dBgS_LinChk lin_chk;
                lin_chk.SetObj();
                lin_chk.Set(&vec, &current.pos, NULL);
                if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                    cM3dGPla plane;
                    dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
                    current.pos = old.pos = lin_chk.GetCross();
                    mAction = ACT_WAIT;
                    mMode = 0;
                    const cXyz* normal = plane.GetNP();
                    current.pos = lin_chk.GetCross();
                    cXyz zero(0.0f, 0.0f, 0.0f);
                    cXyz vec(0.0f, normal->y, normal->z);
                    mWallAngle.z = -cM_atan2s(normal->x, zero.abs(vec));
                    mWallAngle.x = cM_atan2s(normal->z, normal->y);
                    mDownAngleY = cM_atan2s(normal->x, normal->z);
                } else {
                    mAction = ACT_MOVE;
                    mMode = 2;
                    J3DAnmTransform* anm =
                        static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("Kab_m", 6));
                    mpMorf->setAnm(anm, 2, 5.0f, 1.0f, 0.0f, -1.0f);
                    speedF = 8.0f;
                    mTargetSpeedXZ = cM_rndF(5.0f) + 8.0f;
                    home.pos = current.pos;
                    mTargetSpeedY = 5.0f;
                    speed.y = 5.0f;
                    mTimers[0] = 50;
                    current.pos.y += 50.0f;
                    mTimers[1] = cM_rndF(5.0f) + 5.0f;
                    current.pos.y = old.pos.y = player->current.pos.y + 100.0f;
                }
                mBoomerangHit = 0;
            }
        }

        eyePos = current.pos;
        eyePos.y += 10.0f;
        attention_info.position = eyePos;
        attention_info.position.y += 10.0f;
        ParticleSet();
        break;

    case LOC_AGITHA:
        ShopAction();
        break;
    }

    if (mAction == ACT_MOVE && mMode == 2 && speed.y < 0.0f) {
        mAcch.CrrPos(dComIfG_Bgsp());
    }
    mpBtkAnm->play();
    mpBrkAnm->play();
    mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    setBaseMtx();
    return 1;
}

void daObjKABUTO_c::ObjHit() {
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
            mAction = ACT_MOVE;
            mMode = 2;
            mBoomerangHit = false;
            J3DAnmTransform* anm = static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("Kab_m", 6));
            mpMorf->setAnm(anm, 2, 5.0f, 1.0f, 0.0f, -1.0f);
            speedF = 8.0f;
            mTargetSpeedXZ = cM_rndF(5.0f) + 8.0f;
            mTimers[1] = cM_rndF(5.0f) + 5.0f;
        }
        mCcSph.ClrTgHit();
    }
}

int daObjKABUTO_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, "Kab_m");
    if (mHIOInit) {
        hio_set = false;
    }
    if (heap != NULL) {
        mpMorf->stopZelAnime();
    }
    return 1;
}

void daObjKABUTO_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(mWallAngle);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(scale);
    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->modelCalc();
}

int daObjKABUTO_c::Draw() {
    if (mDraw) {
        Z_BufferChk();
        J3DModel* model = mpMorf->getModel();
        g_env_light.settingTevStruct(0, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(model, &tevStr);
        mpBtkAnm->entry(model->getModelData());
        mpBrkAnm->entry(model->getModelData());
        mpMorf->entryDL();
        if (mLocation == LOC_OUTSIDE) {
            dComIfGd_setSimpleShadow(&current.pos, mAcch.GetGroundH(), 15.0f, mAcch.m_gnd, 0,
                                     l_HIO.field_0x10, dDlst_shadowControl_c::getSimpleTex());
        }
    }
    return 1;
}

static int daObjKABUTO_Draw(daObjKABUTO_c* i_this) {
    return i_this->Draw();
}

static int daObjKABUTO_Execute(daObjKABUTO_c* i_this) {
    return i_this->Execute();
}

static u16 const l_musiya_num[2] = {
    0x0191, /* dSv_event_flag_c::F_0401 - Misc. - Beetle (M) */
    0x0192, /* dSv_event_flag_c::F_0402 - Misc. - Beetle (F) */
};

bool daObjKABUTO_c::CreateChk() {
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
                if (dMenu_Insect_c::isCatchNotGiveInsect(l_kab_itemno[mSex])) {
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

cPhs__Step daObjKABUTO_c::create() {
    fopAcM_ct(this, daObjKABUTO_c);
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhaseReq, "Kab_m");

    if (step == cPhs_COMPLEATE_e) {
        mLocation = fopAcM_GetParam(this) & 0xf;
        if (mLocation == LOC_UNK_2) {
            field_0x56c = 0;
            shape_angle.x -= 0x2000;
            fopAcM_OnStatus(this, 0x4000);
        } else {
            mDraw = true;
            attention_info.distances[fopAc_attn_CARRY_e] = 0x5D;
        }

        mSex = (fopAcM_GetParam(this) >> 4) & 1;
        setItemNo(l_kab_itemno[mSex]);
        setSaveBitNo(l_musiya_num[mSex]);

        if (!CreateChk()) {
            return cPhs_ERROR_e;
        }

        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0xb60)) {
            return cPhs_ERROR_e;
        }

        if (!hio_set) {
            hio_set = true;
            mHIOInit = true;
            l_HIO.field_0x4 = -1;
        }

        mAction = ACT_WAIT;
        mMode = 0;
        gravity = 0.0f;
        bool cross = false;

        if (mLocation != LOC_UNK_2) {
            cXyz vec1(0.0f, 0.0f, 500.0f);
            cXyz vec2(0.0f, 0.0f, -200.0f);
            cXyz vec3, vec4;
            dBgS_LinChk lin_chk;
            lin_chk.SetObj();
            for (int i = 0; i < 4; i++) {
                cLib_offsetPos(&vec3, &current.pos, shape_angle.y + 0x4000 * i, &vec1);
                cLib_offsetPos(&vec4, &current.pos, shape_angle.y + 0x4000 * i, &vec2);
                lin_chk.Set(&vec4, &vec3, NULL);
                lin_chk.SetObj();
                if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                    cM3dGPla plane;
                    dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
                    const cXyz* normal = plane.GetNP();
                    current.pos = lin_chk.GetCross();
                    cXyz zero(0.0f, 0.0f, 0.0f);
                    cXyz normalYZ(0.0f, normal->y, normal->z);
                    mWallAngle.z = -cM_atan2s(normal->x, zero.abs(normalYZ));
                    mWallAngle.x = cM_atan2s(normal->z, normal->y);
                    mDownAngleY = cM_atan2s(normal->x, normal->z);
                    cross = true;
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
                    mWallAngle.z = -cM_atan2s(normal->x, zero.abs(normalYZ));
                    mWallAngle.x = cM_atan2s(normal->z, normal->y);
                    mDownAngleY = cM_atan2s(normal->x, normal->z);
                }
            }
        } else {
            mAction = ACT_WAIT;
            mTimers[0] = 10;
            if (!strcmp("R_SP160", dComIfGp_getStartStageName())
                && dComIfGp_getStartStageRoomNo() == 3)
            {
                mLocation |= LOC_UNK_3;
            }
        }

        attention_info.flags = 0;
        if (mLocation == LOC_OUTSIDE) {
            InitCcSph();
        }
        if (mLocation == LOC_UNK_2 && !(!strcmp("R_SP160", dComIfGp_getStartStageName())
            && dComIfGp_getStartStageRoomNo() == 3))
        {
            InitCcSph();
        }
        mAcch.Set(&current.pos, &old.pos, this, 1, &mAcchCir, &speed, NULL, NULL);
        fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -50.0f, -50.0f, -50.0f);
        fopAcM_SetMax(this, 50.0f, 50.0f, 50.0f);

        if (mSex == SEX_FEMALE) {
            scale.x = 1.2f;
            scale.y = 1.2f;
            scale.z = 1.2f;
            mParticleScale = 1.0f;
            mParticleTargetScale = 1.0f;
        } else if (mSex == SEX_MALE) {
            scale.x = 1.5f;
            scale.y = 1.5f;
            scale.z = 1.5f;
            mParticleScale = 1.1f;
            mParticleTargetScale = 1.1f;
        }

        mCreatureSound.init(&current.pos, &eyePos, 3, 1);
        mTimers[2] = 100;
        daObjKABUTO_Execute(this);
    }

    return step;
}

static int daObjKABUTO_IsDelete(daObjKABUTO_c* i_this) {
    return 1;
}

static actor_method_class l_daObjKABUTO_Method = {
    (process_method_func)daObjKABUTO_Create,
    (process_method_func)daObjKABUTO_Delete,
    (process_method_func)daObjKABUTO_Execute,
    (process_method_func)daObjKABUTO_IsDelete,
    (process_method_func)daObjKABUTO_Draw,
};

actor_process_profile_definition g_profile_Obj_Kabuto = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_Obj_Kabuto,
    &g_fpcLf_Method.base,
    sizeof(daObjKABUTO_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x1DD,
    &l_daObjKABUTO_Method,
    0xC0120,
    fopAc_ENV_e,
    fopAc_CULLBOX_CUSTOM_e,
};
