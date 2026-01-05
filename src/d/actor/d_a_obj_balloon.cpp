/**
 * d_a_obj_balloon.cpp
 * Plumm Minigame Balloon
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_balloon.h"
#include "JSystem/JAudio2/JASAudioThread.h"
#include "JSystem/JAudio2/JAUSoundTable.h"
#include "SSystem/SComponent/c_math.h"
#include "Z2AudioLib/Z2Instances.h"
#include "Z2AudioLib/Z2WolfHowlMgr.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "d/d_com_inf_game.h"
#include "d/d_drawlist.h"
#include "f_op/f_op_actor_mng.h"
#include "d/actor/d_a_balloon_2D.h"

daObj_Balloon_HIO_c::daObj_Balloon_HIO_c() {
    field_0x04 = -1;

    mScale[daObj_Balloon_c::SIZE_LARGE_e] = 1.0f;
    mScale[daObj_Balloon_c::SIZE_MEDIUM_e] = 1.0f;
    mScale[daObj_Balloon_c::SIZE_SMALL_e] = 1.5f;

    mScore[daObj_Balloon_c::SIZE_LARGE_e] = 1.0f;
    mScore[daObj_Balloon_c::SIZE_MEDIUM_e] = 3.0f;
    mScore[daObj_Balloon_c::SIZE_SMALL_e] = 10.0f;
}

int daObj_Balloon_c::draw() {
    g_env_light.settingTevStruct(16, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mModel, &tevStr);
    fopAcM_setEffectMtx(this, mModel->getModelData());
    mDoExt_modelUpdateDL(mModel);

    cXyz pos;
    pos.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);
    mShadowKey = dComIfGd_setShadow(mShadowKey, 1, mModel, &pos, 400.0f, 0.0f, current.pos.y,
                                    mAcch.GetGroundH(), mAcch.m_gnd, &tevStr, 0, 1.0f,
                                    dDlst_shadowControl_c::getSimpleTex());
    return 1;
}

static int daObj_Balloon_Draw(daObj_Balloon_c* i_this) {
    return i_this->draw();
}

namespace {
static int m_combo_type = 0xFFFFFFFF;

static int m_combo_count;

static int m_combo_next_score;

static u32 m_balloon_score;
}  // namespace

void daObj_Balloon_c::saveBestScore() {
    dComIfGp_setMessageCountNumber(m_balloon_score);
}

static u8 hio_set;

static daObj_Balloon_HIO_c l_HIO;

void daObj_Balloon_c::cc_set() {
    cXyz cc_center;
    mDoMtx_stack_c::copy(mModel->getBaseTRMtx());
    mDoMtx_stack_c::multVecZero(&cc_center);

    static f32 balloon_radius[3] = {1070.0f, 390.0f, 110.0f};

    mSph.SetC(cc_center);
    mSph.SetR(balloon_radius[mType] * l_HIO.mScale[mType]);
    dComIfG_Ccsp()->Set(&mSph);
}

void daObj_Balloon_c::action() {
    static u16 kago_wall_hit_id[2] = {0x8A12, 0x8A13};
    static f32 kago_effect_scale[3] = {4.0f, 1.5f, 0.8f};
    static u32 break_sound_id[3] = {Z2SE_OBJ_BLN_BREAK_L, Z2SE_OBJ_BLN_BREAK_M,
                                    Z2SE_OBJ_BLN_BREAK_S};

    switch (mRotateMode) {
    case 0:
        gravity = 0.0f;
        speedF = 0.0f;
        speed.y = 0.0f;
        mRotateMode = 1;
        break;
    case 1:
        cLib_addCalcAngleS2(&mRotateZ, mRotZTarget, 16, 8);
        cLib_addCalcAngleS2(&mRotateY, mRotYTarget, 16, 8);
        cLib_chaseF(&speed.y, -3.0f, 0.1f);
        if (current.pos.y < home.pos.y) {
            mRotateMode = 2;
            mRotateTimer = (s16)(30.0f + cM_rndF(60.0f));
            mRotZTarget = 512.0f + cM_rndF(512.0f);
            mRotYTarget = 512.0f + cM_rndF(512.0f);
        }
        break;
    case 2:
        cLib_addCalcAngleS2(&mRotateZ, mRotZTarget, 16, 32);
        cLib_addCalcAngleS2(&mRotateY, mRotYTarget, 16, 32);
        cLib_chaseF(&speed.y, 5.0f, 0.1f);
        if (mRotateTimer == 0) {
            mRotateMode = 1;
            mRotZTarget = cM_rndF(128.0f);
            mRotYTarget = cM_rndF(128.0f);
        }
        break;
    }

    shape_angle.z += mRotateZ;
    shape_angle.y += mRotateY;
    if (mRotateTimer != 0) {
        mRotateTimer--;
    }

    mStts.Move();

    if (mSph.ChkTgHit()) {
        cCcD_Obj* hit_obj = mSph.GetTgHitObj();
        if (fopAcM_GetName(dCc_GetAc(hit_obj->GetAc())) == PROC_KAGO) {
            cXyz eff_scale(kago_effect_scale[mType], kago_effect_scale[mType],
                           kago_effect_scale[mType]);
            for (int i = 0; i < 2; i++) {
                dComIfGp_particle_set(kago_wall_hit_id[i], &current.pos, &tevStr, &shape_angle,
                                      &eff_scale);
            }

            u32 add_score;
            if (m_combo_type == mType) {
                m_combo_count++;
                add_score = m_combo_next_score;
                if (m_combo_count >= 10) {
                    m_combo_next_score = l_HIO.mScore[mType] * 512.0f;
                } else {
                    m_combo_next_score *= 2;
                }
            } else {
                m_combo_type = mType;
                m_combo_count = 1;
                m_combo_next_score = l_HIO.mScore[mType] * 2.0f;
                add_score = l_HIO.mScore[mType];
            }

            Z2GetAudioMgr()->seStart(break_sound_id[mType], &current.pos, m_combo_count, 0, 1.0f,
                                     1.0f, -1.0f, -1.0f, 0);
            m_balloon_score += add_score;

            daBalloon2D_c* balloon_2d = (daBalloon2D_c*)fopAcM_SearchByName(PROC_BALLOON2D);
            if (balloon_2d != NULL) {
                u8 balloon_size;
                if (mType == SIZE_SMALL_e) {
                    balloon_size = 0;
                } else if (mType == SIZE_MEDIUM_e) {
                    balloon_size = 1;
                } else if (mType == SIZE_LARGE_e) {
                    balloon_size = 2;
                }

                cXyz pos_2d(dCc_GetAc(hit_obj->GetAc())->current.pos);
                pos_2d = (pos_2d + current.pos) / 2.0f;
                balloon_2d->setComboCount(balloon_size, m_combo_count);
                balloon_2d->setScoreCount(m_balloon_score);
                balloon_2d->addScoreCount(&pos_2d, add_score, balloon_size);
            }

            fopAcM_delete(this);
            return;
        }
    }

    fopAcM_posMoveF(this, mStts.GetCCMoveP());
}

void daObj_Balloon_c::mtx_set() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(l_HIO.mScale[mType], l_HIO.mScale[mType], l_HIO.mScale[mType]);
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

int daObj_Balloon_c::execute() {
    action();
    cc_set();
    mtx_set();
    return 1;
}

static int daObj_Balloon_Execute(daObj_Balloon_c* i_this) {
    return i_this->execute();
}

static int daObj_Balloon_IsDelete(daObj_Balloon_c* i_this) {
    return 1;
}

int daObj_Balloon_c::_delete() {
    dComIfG_resDelete(&mPhase, "Obj_bal");
    Z2GetAudioMgr()->seStop(Z2SE_OBJ_WATERMILL_ROUND, 0);
    if (mHIOInit) {
        hio_set = false;
    }
    return 1;
}

static int daObj_Balloon_Delete(daObj_Balloon_c* i_this) {
    return i_this->_delete();
}

int daObj_Balloon_c::CreateHeap() {
    J3DModelData* modelData;
    if (mType == SIZE_LARGE_e) {
        modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_bal", 3);
    } else if (mType == SIZE_MEDIUM_e) {
        modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_bal", 4);
    } else if (mType == SIZE_SMALL_e) {
        modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_bal", 5);
    }
    JUT_ASSERT(501, modelData != NULL);
    mModel = mDoExt_J3DModel__create(modelData, 0, 0x11000084);
    return mModel != NULL ? TRUE : FALSE;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return static_cast<daObj_Balloon_c*>(i_this)->CreateHeap();
}

int daObj_Balloon_c::create() {
    fopAcM_ct(this, daObj_Balloon_c);
    mType = fopAcM_GetParam(this);
    if (mType == 0xFF) {
        mType = SIZE_LARGE_e;
    }

    int phase_state = dComIfG_resLoad(&mPhase, "Obj_bal");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("Obj_Balloon PARAM %x\n", fopAcM_GetParam(this));
        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0xD60)) {
            return cPhs_ERROR_e;
        }

        if (!hio_set) {
            mHIOInit = true;
            hio_set = true;
            l_HIO.field_0x04 = -1;
        }

        attention_info.flags = 0;
        fopAcM_SetMtx(this, mModel->getBaseTRMtx());
        fopAcM_SetMin(this, -1000.0f, -1000.0f, -1000.0f);
        fopAcM_SetMax(this, 1000.0f, 1000.0f, 1000.0f);
        fopAcM_setCullSizeFar(this, 2.0f);

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), NULL, NULL);
        mAcchCir.SetWall(0.0f, 0.0f);

        health = 60;
        field_0x560 = 60;

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0x8, 0x11}, 0x0}},  // mObj
                {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x0},         // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},          // mGObjTg
                {0x0},                                       // mGObjCo
            },                                               // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f}  // mSph
            }  // mSphAttr
        };

        mStts.Init(0xFF, 0, this);
        mSph.Set(cc_sph_src);
        mSph.SetStts(&mStts);
        daObj_Balloon_Execute(this);
    }

    return phase_state;
}

static int daObj_Balloon_Create(daObj_Balloon_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daObj_Balloon_Method = {
    (process_method_func)daObj_Balloon_Create,  (process_method_func)daObj_Balloon_Delete,
    (process_method_func)daObj_Balloon_Execute, (process_method_func)daObj_Balloon_IsDelete,
    (process_method_func)daObj_Balloon_Draw,
};

actor_process_profile_definition g_profile_OBJ_BALLOON = {
    fpcLy_CURRENT_e,          // mLayerID
    7,                        // mListID
    fpcPi_CURRENT_e,          // mListPrio
    PROC_OBJ_BALLOON,         // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daObj_Balloon_c),  // mSize
    0,                        // mSizeOther
    0,                        // mParameters
    &g_fopAc_Method.base,     // sub_method
    64,                       // mPriority
    &l_daObj_Balloon_Method,  // sub_method
    0x40100,                  // mStatus
    fopAc_ACTOR_e,            // mActorType
    fopAc_CULLBOX_CUSTOM_e,   // cullType
};

AUDIO_INSTANCES;
