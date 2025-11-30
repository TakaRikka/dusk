/**
 * @file d_a_obj_lv5IceWall.cpp
 * Snowpeak Ruins Ice Wall
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv5IceWall.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"

daIceWall_HIO_c::daIceWall_HIO_c() {
    hitcount = 3;
    hitcount2 = 2;

    xyz.x = 1.0f;
    xyz.y = 1.0f;
    xyz.z = 1.0f;
}

void daIceWall_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mDoMtx_stack_c::scaleM(mScale.x, mScale.y, mScale.z);

    mpModel[mIsBreaking]->setBaseScale(cXyz(1.0f, 1.0f, 1.0f));
    mpModel[mIsBreaking]->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daIceWall_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("l5IceWall", 4);
    JUT_ASSERT(0, modelData != NULL);

    mpModel[0] = mDoExt_J3DModel__create(modelData, 0, 0x11000084);
    if (mpModel[0] == NULL) {
        return 0;
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes("l5IceWall", 5);
    JUT_ASSERT(0, modelData != NULL);

    mpModel[1] = mDoExt_J3DModel__create(modelData, 0, 0x11000084);
    if (mpModel[1] == NULL) {
        return 0;
    }

    return 1;
}

static daIceWall_HIO_c l_HIO;

const dCcD_SrcGObjInf daIceWall_c::mCcDObjInfo = {
    {0, {{0x100, 1, 0}, {0x400000, 0x11}, {0}}},
    {dCcD_SE_SWORD, 0, 0, 2, 1},
    {dCcD_SE_STONE, 0, 0, 0, 0x2006},
    {0},
};

dCcD_SrcCyl daIceWall_c::mCcDCyl = {
    daIceWall_c::mCcDObjInfo,
    {
        {0.0f, 0.0f, 0.0f},  // mCenter
        0.0f,                // mRadius
        0.0f                 // mHeight
    }  // mCyl
};

static const int l_bmdIdx[] = {4, 5};

int daIceWall_c::create() {
    fopAcM_ct(this, daIceWall_c);

    mIsBreakSwBit = getSwBit1();
    mIsBreakingSwBit = getSwBit1();
    if (mIsBreakSwBit != 0xFF) {
        if (fopAcM_isSwitch(this, mIsBreakSwBit)) {
            return cPhs_ERROR_e;
        }

        mIsBreaking = false;
        if (mIsBreakingSwBit != 0xFF) {
            mIsBreaking = fopAcM_isSwitch(this, mIsBreakingSwBit);
        }
    } else {
        mIsBreaking = false;
    }

    int phase = dComIfG_resLoad(&mPhase, "l5IceWall");
    if (phase == cPhs_COMPLEATE_e) {
        if (MoveBGCreate("l5IceWall", 8, dBgS_MoveBGProc_TypicalRotY, 0x2100, NULL) == cPhs_ERROR_e)
        {
            return cPhs_ERROR_e;
        }

        mScale.x = getScaleX() * 0.1f;
        mScale.y = getScaleY() * 0.1f;
        mScale.z = getScaleZ() * 0.1f;

        fopAcM_SetMtx(this, mpModel[mIsBreaking]->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel[mIsBreaking]->getModelData());
        mCcStts.Init(0xFF, 0xFF, this);

        for (int i = 0; i < 2; i++) {
            mCcCyl[i].Set(mCcDCyl);
            mCcCyl[i].SetStts(&mCcStts);
        }

        init_modeWait();
        setBaseMtx();
    }

    return phase;
}

int daIceWall_c::Execute(Mtx** param_0) {
    procMain();
    *param_0 = &mpModel[mIsBreaking]->getBaseTRMtx();
    setBaseMtx();
    mCcStts.Move();
    return 1;
}

void daIceWall_c::procMain() {
    typedef void (daIceWall_c::*procFunc)();
    static procFunc mode_proc[] = {
        &daIceWall_c::modeWait,
        &daIceWall_c::modeBreak,
    };

    (this->*mode_proc[mMode])();
}

void daIceWall_c::init_modeWait() {
    mHitcount = l_HIO.hitcount;
    mMode = MODE_WAIT;
}

void daIceWall_c::modeWait() {
    for (int i = 0; i < 2; i++) {
        if (mCcCyl[i].ChkTgHit()) {
            dCcD_GObjInf* hit_obj = mCcCyl[i].GetTgHitGObj();

            if (hit_obj->GetAtType() & AT_TYPE_IRON_BALL) {
                if (hit_obj->GetAtAtp() == 100 || hit_obj->GetAtAtp() == 3) {
                    init_modeBreak();
                } else if (hit_obj->GetAtAtp() == 2) {
                    cXyz ptcl_scale(1.0f, 1.0f, 1.0f);
                    mEmitter2ID =
                        dComIfGp_particle_set(mEmitter2ID, 0x8678, &current.pos, &shape_angle,
                                              &ptcl_scale, 0xFF, 0, -1, NULL, NULL, NULL);

                    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mEmitter1ID);
                    if (emitter != NULL) {
                        cXyz scale;
                        scale.x = mScale.x * 0.65f;
                        scale.y = mScale.z;
                        scale.z = mScale.y * 2.5f;

                        emitter->setLocalScale(scale);
                    }

                    mDoAud_seStart(Z2SE_OBJ_ICE_KEZURU, &current.pos, 0,
                                   dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

                    mHitcount--;
                    if (mHitcount == 0) {
                        mIsBreaking++;
                        if (mIsBreaking == 1) {
                            if (mIsBreakingSwBit != 0xFF) {
                                fopAcM_onSwitch(this, mIsBreakingSwBit);
                            }

                            fopAcM_SetMtx(this, mpModel[mIsBreaking]->getBaseTRMtx());
                            fopAcM_setCullSizeBox2(this, mpModel[mIsBreaking]->getModelData());
                            mHitcount = l_HIO.hitcount2;
                        }

                        if (mIsBreaking > 1) {
                            mIsBreaking = 1;
                            init_modeBreak();
                        }
                    }
                }
            }
            break;
        }
    }

    for (int i = 0; i < 2; i++) {
        cXyz pos_offset(0.0f, 0.0f, 0.0f);
        pos_offset.z = (f32)i * (oREG_F(2) + 130.0f) - (oREG_F(3) + 60.0f);

        mDoMtx_stack_c::ZXYrotS(shape_angle.x, shape_angle.y, shape_angle.z);
        mDoMtx_stack_c::multVec(&pos_offset, &pos_offset);

        cXyz center(current.pos);
        center += pos_offset;

        mCcCyl[i].SetR(mScale.z * (oREG_F(0) + 140.0f));
        mCcCyl[i].SetH(mScale.y * (oREG_F(1) + 510.0f));
        mCcCyl[i].SetC(center);

        dComIfG_Ccsp()->Set(&mCcCyl[i]);
    }

    cXyz ptcl_scale(1.0f, 1.0f, 1.0f);
    mEmitter0ID = dComIfGp_particle_set(mEmitter0ID, 0x867C, &current.pos, &shape_angle,
                                        &ptcl_scale, 0xFF, 0, -1, NULL, NULL, NULL);

    static const u16 l_eff_b[] = {0x8676, 0x8677};
    mEmitter1ID = dComIfGp_particle_set(mEmitter1ID, l_eff_b[mIsBreaking], &current.pos,
                                        &shape_angle, &ptcl_scale, 0xFF, 0, -1, NULL, NULL, NULL);

    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mEmitter1ID);
    if (emitter != NULL) {
        cXyz scale;
        scale.x = mScale.x * 0.85f;
        scale.y = mScale.z * 0.9f;
        scale.z = mScale.y * 2.0f;

        emitter->setLocalScale(scale);
    }
}

void daIceWall_c::init_modeBreak() {
    if (mIsBreakSwBit != 0xFF) {
        fopAcM_onSwitch(this, mIsBreakSwBit);
    }

    mMode = MODE_BREAK;
}

void daIceWall_c::modeBreak() {
    cXyz ptcl_scale(1.0f, 1.0f, 1.0f);
    mEmitter0ID = dComIfGp_particle_set(mEmitter0ID, 0x867B, &current.pos, &shape_angle,
                                        &ptcl_scale, 0xFF, 0, -1, NULL, NULL, NULL);

    static const u16 l_eff_break_b[] = {0x8679, 0x867A};
    mEmitter1ID = dComIfGp_particle_set(mEmitter1ID, l_eff_break_b[mIsBreaking], &current.pos,
                                        &shape_angle, &ptcl_scale, 0xFF, 0, -1, NULL, NULL, NULL);

    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mEmitter1ID);
    if (emitter != NULL) {
        cXyz scale;
        scale.x = mScale.x * 0.65f;
        scale.y = mScale.z;
        scale.z = mScale.y * 2.5f;

        emitter->setLocalScale(scale);
    }

    mDoAud_seStart(Z2SE_OBJ_ICE_BREAK, &current.pos, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    fopAcM_delete(this);
}

int daIceWall_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel[mIsBreaking], &tevStr);

    fopAcM_setEffectMtx(this, mpModel[mIsBreaking]->getModelData());

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel[mIsBreaking]);
    dComIfGd_setList();
    return 1;
}

int daIceWall_c::Delete() {
    dComIfG_resDelete(&mPhase, "l5IceWall");
    return 1;
}

static int daIceWall_Draw(daIceWall_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daIceWall_Execute(daIceWall_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daIceWall_Delete(daIceWall_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daIceWall_Create(fopAc_ac_c* i_this) {
    return ((daIceWall_c*)i_this)->create();
}

static actor_method_class l_daIceWall_Method = {
    (process_method_func)daIceWall_Create,  (process_method_func)daIceWall_Delete,
    (process_method_func)daIceWall_Execute, (process_method_func)NULL,
    (process_method_func)daIceWall_Draw,
};

extern actor_process_profile_definition g_profile_Obj_IceWall = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_IceWall,        // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daIceWall_c),     // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    536,                     // mPriority
    &l_daIceWall_Method,     // sub_method
    0x00040100,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
