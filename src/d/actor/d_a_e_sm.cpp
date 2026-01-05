/**
 * @file d_a_e_sm.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_sm.h"
#include "d/d_cc_d.h"
#include "d/d_camera.h"
#include "d/actor/d_a_arrow.h"
#include "Z2AudioLib/Z2Instances.h"
#include "f_op/f_op_camera_mng.h"

enum E_SM_RES_File_ID {
    /* BCK */
    /* 0x05 */ BCK_SC_ATTACK = 0x5,
    /* 0x06 */ BCK_SC_BUTTOBI,
    /* 0x07 */ BCK_SC_DAMAGE,
    /* 0x08 */ BCK_SC_DAMAGE02,
    /* 0x09 */ BCK_SC_DAMAGEB,
    /* 0x0A */ BCK_SC_DAMAGEL,
    /* 0x0B */ BCK_SC_DAMAGER,
    /* 0x0C */ BCK_SC_DEAD,
    /* 0x0D */ BCK_SC_DEAD02,
    /* 0x0E */ BCK_SC_DRAW,
    /* 0x0F */ BCK_SC_ESCAPE,
    /* 0x10 */ BCK_SC_F_SHOCK,
    /* 0x11 */ BCK_SC_KYORO,
    /* 0x12 */ BCK_SC_LIBERATION,
    /* 0x13 */ BCK_SC_MOVE,
    /* 0x14 */ BCK_SC_MOVE02,
    /* 0x15 */ BCK_SC_PROVOKE,
    /* 0x16 */ BCK_SC_REFILL,
    /* 0x17 */ BCK_SC_REFILL02,
    /* 0x18 */ BCK_SC_S_JUMP,
    /* 0x19 */ BCK_SC_S_JUMP_A,
    /* 0x1A */ BCK_SC_S_JUMP_B,
    /* 0x1B */ BCK_SC_S_JUMP_C,
    /* 0x1C */ BCK_SC_WAIT,
    /* 0x1D */ BCK_SC_WAIT02,
    /* 0x1E */ BCK_SC_WHISTLE,

    /* BMDE */
    /* 0x21 */ BMDE_SM = 0x21,

    /* BMDR */
    /* 0x24 */ BMDR_SC = 0x24,
};

enum Action {
    /* 0x0 */ ACTION_NORMAL,
    /* 0x1 */ ACTION_MOVE,
    /* 0x2 */ ACTION_DAMAGE,
    /* 0x3 */ ACTION_UNK,
    /* 0x4 */ ACTION_DELETE,
    /* 0x5 */ ACTION_ATTACK,
};

enum Core_Action {
    /* 0x0 */ CORE_ACTION_NORMAL,
    /* 0x1 */ CORE_ACTION_FIGHT,
    /* 0x3 */ CORE_ACTION_FREE = 0x3,
    /* 0x4 */ CORE_ACTION_DEATH,
    /* 0x5 */ CORE_ACTION_HOOK,
    /* 0x7 */ CORE_ACTION_DEMO = 0x7,
};

class daE_Sm_HIO_c : public JORReflexible {
public:
    daE_Sm_HIO_c();
    virtual ~daE_Sm_HIO_c() {}

    void genMessage(JORMContext*);

    /* スライム - Slime */
    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 basic_size;      // 基本大きさ - Basic Size
    /* 0x0C */ f32 pl_notice_dist;  // PL認識距離 - PL Notice Distance   
    /* 0x10 */ f32 core_size;       // コア大きさ - Core Size
};

daE_Sm_HIO_c::daE_Sm_HIO_c() {
    field_0x4 = -1;
    basic_size = 1.0f;
    pl_notice_dist = 1000.0f;
    core_size = 1.0f;
}

void daE_SM_c::SetAnm(int i_index, int i_attr, f32 i_rate, f32 i_morf) {
    mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_SM", i_index), i_attr, i_morf, i_rate, 0.0f, -1.0f);
    mAnm = i_index;
}

int daE_SM_c::Draw() {
    J3DModel* model = mpModelMorf->getModel();

    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);

    if (field_0x698) {
        J3DModelData* modelData = model->getModelData();
        J3DMaterial* material1 = modelData->getMaterialNodePointer(1);
        material1->getTevColor(0)->r = field_0x690 * -255.0f;
        material1->getTevColor(0)->g = field_0x690 * -255.0f;
        material1->getTevColor(0)->b = field_0x690 * -255.0f;

        J3DMaterial* material2 = modelData->getMaterialNodePointer(2);
        material2->getTevColor(0)->r = field_0x690 * -255.0f;
        material2->getTevColor(0)->g = field_0x690 * -255.0f;
        material2->getTevColor(0)->b = field_0x690 * -255.0f;
    }

    if (!field_0x683) {
        mpModelMorf->entryDL();
    }

    if (field_0x9c4 != 0) {
        cXyz sp68;
        sp68.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);
        mShadowKey = dComIfGd_setShadow(mShadowKey, 1, model, &sp68, 400.0f, 100.0f, current.pos.y,
                                        field_0xa60.GetGroundH(), field_0xa60.m_gnd, &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    }

    if (!field_0x684) {
        g_env_light.setLightTevColorType_MAJI(mpBubbleModel, &tevStr);
        fopAcM_setEffectMtx(this, mpBubbleModel->getModelData());
        mpBubbleModel->getModelData()->getMaterialNodePointer(0)->getTevKColor(1)->a = field_0x694 * 167.0f;
        mDoExt_modelUpdateDL(mpBubbleModel);
        dComIfGd_setSimpleShadow(&field_0x990, field_0xc78.GetGroundH(), (TREG_F(10) + 195.0f) * (field_0x6f0 / field_0x6e0),
                                 field_0xc78.m_gnd, 0, -field_0x6b8, dDlst_shadowControl_c::getSimpleTex());
    }

    return 1;
}

static BOOL way_bg_check(daE_SM_c* i_this, f32 param_2, cXyz param_3, s16 param_4) {
    dBgS_LinChk lin_chk;
    cXyz sp94, i_start, i_end;

    i_start = param_3;
    i_start.y += 80.0f;
    cMtx_YrotS(*calc_mtx, param_4);
    sp94.x = 0.0f;
    sp94.y = 80.0f;
    sp94.z = param_2;
    MtxPosition(&sp94, &i_end);
    i_end += param_3;
    lin_chk.Set(&i_start, &i_end, i_this);

    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        return TRUE;
    }

    return FALSE;
}

static BOOL other_bg_check(daE_SM_c* i_this, fopAc_ac_c* param_2) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    dBgS_LinChk lin_chk;
    cXyz sp98, i_start, i_end;

    i_end = param_2->current.pos;
    i_end.y += 80.0f;
    i_start = a_this->current.pos;
    i_start.y += 80.0f;
    lin_chk.Set(&i_start, &i_end, a_this);

    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        return TRUE;
    }

    return FALSE;
}

static BOOL pl_check(daE_SM_c* i_this, f32 param_2) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    if (fabsf(i_this->current.pos.y - player->current.pos.y) < 500.0f && !other_bg_check(i_this, player)) {
        return TRUE;
    }

    return FALSE;
}

static void move_check(daE_SM_c* i_this, int* param_2) {
    *param_2 = -1;
    
    if (pl_check(i_this, i_this->field_0x970)) {
        *param_2 = 0;
    }
}

void daE_SM_c::Particle_Set(u16 i_resID, cXyz i_scale, cXyz i_pos, csXyz i_rotation) {
    dComIfGp_particle_set(i_resID, &i_pos, &tevStr, &i_rotation, &i_scale);
}

void daE_SM_c::ArrowCheck() {
    cXyz* tgHitPosP = mCoSm.GetTgHitPosP();
    field_0x6d4 = 10;
    
    if (((daArrow_c*)mAtInfo.mpActor)->changeActorControll()) {
        field_0x6f9[field_0x6f8] = 1;
        daArrow_c* arrow_p;
        fopAcM_SearchByID(mArrowProcIDs[field_0x6f8], (fopAc_ac_c**)&arrow_p);

        if (arrow_p != NULL) {
            arrow_p->deleteArrow();
        }

        if (tgHitPosP != NULL) {
            mArrowProcIDs[field_0x6f8] = fopAcM_GetID(mAtInfo.mpActor);
            cXyz sp64(field_0x990 - *tgHitPosP);
            sp64.y += field_0x6f0 * (KREG_F(1) + 70.0f);
            field_0x808[field_0x6f8].y = cM_rndFX(1000.0f) + cM_atan2s(sp64.x, sp64.z);
            field_0x808[field_0x6f8].x = cM_rndFX(1000.0f) + -(cM_atan2s(sp64.y, JMAFastSqrt(sp64.x * sp64.x + sp64.z * sp64.z)));

            f32 fVar1 = 230.0f;
            if (field_0x808[field_0x6f8].x < 0) {
                fVar1 = HREG_F(10) + 130.0f;
            }

            field_0x790[field_0x6f8] = field_0x6f0 * -fVar1;
            field_0x8bc[field_0x6f8].y = mAtInfo.mpActor->shape_angle.y - field_0x808[field_0x6f8].y;
            field_0x8bc[field_0x6f8].x = mAtInfo.mpActor->shape_angle.x - field_0x808[field_0x6f8].x;
            field_0x6f8++;

            if (field_0x6f8 == 30) {
                field_0x6f8 = 0;
            }
        }
    }

    if (mAction != ACTION_ATTACK && mAction != ACTION_MOVE) {
        mAction = ACTION_DAMAGE;
    }

    field_0x6dc = XREG_F(12) + 0.05f + cM_rndFX(XREG_F(13) + 0.01f);
    field_0x978 = cM_rndFX(0.1f) + -0.3f;
    field_0x6c0[0] = 50;
    field_0x980 = XREG_F(14) + 2500.0f;
}

void daE_SM_c::E_SM_Damage() {
    field_0x6bc += (s16)((field_0x980 + 1000.0f) / field_0x6f0);
    mCoSm.OffAtSetBit();

    if (field_0x6c0[0] != 0) {
        f32 fVar1;

        if (field_0x685 == true) {
            fVar1 = field_0x978 * cM_ssin(field_0x6be);
        } else {
            fVar1 = field_0x978;
        }

        field_0x6be += (s16)(3000.0f / field_0x6f0);
        cLib_addCalc2(&field_0x6e4, field_0x974, 0.3f, 1.0f);
        cLib_addCalc2(&field_0x6e0, fVar1 + 1.0f, 0.3f, 1.0f);
        cLib_addCalc2(&field_0x6dc, field_0x97c + 0.005f, 0.05f, 0.5f);
        cLib_addCalc2(&field_0x980, 0.0f, 0.01f, 1000.0f);
        cLib_chaseF(&field_0x974, 0.0f, 0.01f);
        cLib_chaseF(&field_0x978, 0.0f, 0.105f);
        cLib_chaseF(&field_0x97c, 0.0f, 0.03f);
    } else {
        field_0x685 = false;

        if (mCoreAction != CORE_ACTION_DEATH && health > 0 && mCoreAction != CORE_ACTION_DEMO && mAction != ACTION_ATTACK && mAction != ACTION_MOVE) {
            if (mCoreAction != CORE_ACTION_FREE && mCoreAction != CORE_ACTION_HOOK) {
                if (mAnm == BCK_SC_WHISTLE) {
                    if (mpModelMorf->isStop()) {
                        SetAnm(BCK_SC_MOVE02, J3DFrameCtrl::EMode_LOOP, 1.0f, 3.0f);
                        mAction = ACTION_NORMAL;
                        mMode = 0;
                    }
                } else {
                    SetAnm(BCK_SC_MOVE02, J3DFrameCtrl::EMode_LOOP, 1.0f, 3.0f);
                    mAction = ACTION_NORMAL;
                    mMode = 0;
                }
            } else {
                mAction = ACTION_UNK;
                mMode = 0;
            }
        }
    }
}

void daE_SM_c::SmDamageCheck() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz& pos = fopAcM_GetPosition(player);

    if (field_0x6d4 != 0) {
        mCoSm.ClrTgHit();
        mCoSm.ClrAtHit();
        return;
    }

    field_0xe50.Move();
    field_0xe8c.Move();

    if (mCoSm.ChkAtHit()) {
        field_0x6d4 = 10;
        s16 sVar1 = cLib_targetAngleY(&field_0x990, &pos);

        if (!player->checkPlayerGuard()) {
            player->setThrowDamage(sVar1, 15.0f, 20.0f, 1, 0, 0);
        }

        mCoSm.ClrAtHit();
        return;
    }

    if (mCoSm.ChkTgHit()) {
        field_0xa10 = true;
        mAtInfo.mpCollider = mCoSm.GetTgHitObj();
        at_power_check(&mAtInfo);
        field_0x6d4 = 10;

        if (mCoreAction == CORE_ACTION_FREE && (field_0x6b6 == 7 || field_0x6b6 == 6)) {
            field_0x6d4 = 0;
            mCoSm.ClrTgHit();
        }

        if (mAction != ACTION_ATTACK && mAction != ACTION_MOVE && mCoreAction != CORE_ACTION_FREE && mCoreAction != CORE_ACTION_DEATH) {
            if (mAnm == BCK_SC_WHISTLE || mAnm == BCK_SC_PROVOKE) {
                if (mpModelMorf->isStop()) {
                    int iVar1;
                    move_check(this, &iVar1);
                    if (iVar1 >= 0) {
                        mAction = ACTION_ATTACK;
                        mMode = 0;
                    } else if (cM_rndF(1.0f) < 0.5f) {
                        SetAnm(BCK_SC_PROVOKE, J3DFrameCtrl::EMode_NONE, 1.0f, 5.0f);
                        mSound.startCreatureVoice(Z2SE_EN_SC_V_PROVOKE, -1);
                    } else {
                        SetAnm(BCK_SC_WHISTLE, J3DFrameCtrl::EMode_NONE, 1.0f, 5.0f);
                        mSound.startCreatureVoice(Z2SE_EN_SC_V_WHISTLE, -1);
                    }
                }
            } else if (cM_rndF(1.0f) < 0.5f) {
                SetAnm(BCK_SC_PROVOKE, J3DFrameCtrl::EMode_NONE, 1.0f, 5.0f);
                mSound.startCreatureVoice(Z2SE_EN_SC_V_PROVOKE, -1);
            } else {
                SetAnm(BCK_SC_WHISTLE, J3DFrameCtrl::EMode_NONE, 1.0f, 5.0f);
                mSound.startCreatureVoice(Z2SE_EN_SC_V_WHISTLE, -1);
            }
        }
        
        field_0x6be = 0;
        field_0x6c0[0] = 50;
        field_0x685 = false;
        field_0x9a8 = 0.0f;
        speedF = 0.0f;
        field_0x6dc = 0.06f;
        field_0x97c = 0.0f;
        field_0x980 = 2500.0f;
        mCoSm.OffAtSetBit();

        s16 sVar2;
        if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_ARROW)) {
            mSoundSimple.startSound(Z2SE_EN_SC_SL_REBOUND, 0, -1);
            ArrowCheck();

            if (mAction == ACTION_ATTACK) {
                return;
            }

            if (mAction == ACTION_MOVE) {
                return;
            }

            sVar2 = mAction;
        } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_NORMAL_SWORD)) {
            field_0x6dc = 0.06f;
            field_0x978 += -0.35;
            field_0x97c = 0.0f;
            field_0x6c0[0] = 50;
            field_0x980 = 2500.0f;
            mSoundSimple.startSound(Z2SE_EN_SC_SL_REBOUND, 0, -1);

            if (mAction == ACTION_ATTACK) {
                return;
            }

            if (mAction == ACTION_MOVE) {
                return;
            }

            sVar2 = mAction;
        } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOMB)) {
            mCoCore.OnTgSetBit();
            mCoCore.OffTgNoHitMark();
            field_0x684 = true;
            mCoSm.ClrTgHit();
            field_0x6d4 = 10;
            field_0x682 = true;
            cXyz i_scale(1.5f, 1.5f, 1.5f);
            dComIfGp_particle_set(0x85BF, &current.pos, &tevStr, &current.angle, &i_scale);

            for (int i = 0; i < 30; i++) {
                daArrow_c* arrow_p;
                if (fopAcM_SearchByID(mArrowProcIDs[i], (fopAc_ac_c**)&arrow_p) != 0 && arrow_p != NULL) {
                    arrow_p->deleteArrow();
                }
            }

            mCoreAction = CORE_ACTION_FREE;
            field_0x6b4 = 0;
            field_0x6b6 = 0;
            SetAnm(BCK_SC_LIBERATION, J3DFrameCtrl::EMode_NONE, 1.0f, 3.0f);
        } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_IRON_BALL)) {
            field_0x6dc = cM_rndF(0.01f) + 0.05f;
            field_0x978 = cM_rndFX(0.05f) + -0.3f;
            field_0x97c = 0.05f;
            field_0x6c0[0] = 50;
            field_0x980 = 2500.0f;
            mSoundSimple.startSound(Z2SE_EN_SC_SL_REBOUND, 0, -1);
        } else {
            mSoundSimple.startSound(Z2SE_EN_SC_SL_REBOUND, 0, -1);
        }

        mAction = ACTION_DAMAGE;
        cLib_addCalc2(&field_0x69c, 0.0f, 0.05f, 100.0f);
        return;
    }

    cXyz* hookshotTopPos = player->getHookshotTopPos();
    if (hookshotTopPos == NULL) {
        return;
    }

    cXyz sp34(field_0x990);
    sp34.y += 100.0f;
    
    if (!(sp34.abs(*hookshotTopPos) < field_0x6f0 * 100.0f)) {
        return;
    }

    if (!daPy_getPlayerActorClass()->checkHookshotShootReturnMode()) {
        return;
    }

    field_0x6be = 0;
    field_0x6c0[0] = 50;
    field_0x685 = false;
    field_0x9a8 = 0.0f;
    field_0x6dc = cM_rndF(0.01f) + 0.05f;
    field_0x978 = cM_rndFX(0.05f) + -0.3f;
    field_0x97c = 0.0f;
    field_0x6c0[0] = 50;
    field_0x980 = 2500.0f;

    mSoundSimple.startSound(Z2SE_EN_SC_SL_REBOUND, 0, -1);
    mAction = ACTION_DAMAGE;
    cLib_addCalc2(&field_0x69c, 0.0f, 0.05f, 100.0f);
}

static u8 hio_set;

static daE_Sm_HIO_c l_HIO;

namespace {
    static f32 s_Dis;

    static s16 s_TargetAngle;
};

void daE_SM_c::E_SM_Normal() {
    f32 fVar1 = 0.0f;
    f32 fVar2 = 0.0f;
    field_0x6bc += (int)((field_0x980 + 1000.0f) / field_0x6f0);
    f32 fVar3 = cM_ssin(field_0x6bc);

    switch (mMode) {
        case 0:
            if (field_0xc78.ChkGroundHit()) {
                mMode += 1;
                field_0x9a8 = 0.0f;
                field_0x6c0[0] = 0;
            }
            break;

        case 1:
            if (field_0x6c0[0] == 0 && fVar3 <= -0.9f) {
                mMode++;
                field_0x6c0[0] = cM_rndF(100.0f) + 100.0f;

                if (mAnm != BCK_SC_MOVE02) {
                    SetAnm(BCK_SC_MOVE02, J3DFrameCtrl::EMode_LOOP, 1.0f, 5.0f);
                }

                if (field_0xa10) {
                    field_0x9b0.y = s_TargetAngle;
                    
                    if (way_bg_check(this, scale.x * 100.0f, field_0x990, field_0x9b0.y)) {
                        field_0x9b0.y += 0x100;
                    }
                } else {
                    for (int i = 0; i < 100; i++) {
                        field_0x9b0.y = cM_rndFX(32767.0f);

                        if (!way_bg_check(this, scale.x * 100.0f, field_0x990, field_0x9b0.y)) break;
                    }
                }
            }

            cLib_addCalc0(&field_0x9a8, 1.0f, 0.5f);
            break;

        case 2: {
            if (field_0xa10) {
                field_0x9b0.y = s_TargetAngle;

                if (way_bg_check(this, scale.x * 100.0f, field_0x990, field_0x9b0.y)) {
                    field_0x9b0.y += 0x100;
                }
            }

            if (field_0x6c0[0] == 0 && fVar3 <= -0.9f) {
                mMode = 1;
                field_0x6c0[0] = cM_rndF(50.0f) + 50.0f;
                SetAnm(BCK_SC_WAIT, J3DFrameCtrl::EMode_LOOP, 1.0f, 5.0f);
            }

            s32 tmp = (s32)field_0x6bc;
            tmp += (int)((field_0x980 + 1000.0f) / field_0x6f0);
            fVar1 = cM_ssin(tmp);
            if (fVar3 < -0.0f && fVar1 > 0.0f) {
                mSoundSimple.startSound(Z2SE_EN_SC_SL_MOVE, 0, -1);
                mSound.startCreatureVoice(Z2SE_EN_SC_V_WAIT, -1);
            }

            f32 reg1 = (TREG_F(15) + 0.1f);
            fVar1 = fVar3 * reg1 + (TREG_F(15) + 0.1f);
            f32 reg2 = (TREG_F(13) + 2.0f);
            field_0x9a8 = fVar3 * reg2 + (TREG_F(13) + 2.0f);
            fVar2 = 50.0f;
            break;
        }

        case 3:
            break;
    }

    cLib_addCalc2(&field_0x6e4, fVar1, 0.2f, 0.1f);
    cLib_addCalc2(&field_0x6e0, 1.0f, 0.2f, 0.05f);
    cLib_addCalc2(&field_0x6dc, 0.005f, 0.01f, 0.01f);
    cLib_addCalc2(&field_0x69c, fVar2, 0.05f, 100.0f);

    if (field_0xc78.ChkGroundHit() && fVar3 <= -0.9f && mMode != 3) {
        int iVar1 = -1;

        if (s_Dis < 700.0f) {
            move_check(this, &iVar1);
        }

        if (iVar1 >= 0) {
            mAction = ACTION_ATTACK;
            mMode = 0;
            mSound.setLinkSearch(true);
            field_0x6c0[0] = 0;
        } else {
            mCoreAction = CORE_ACTION_NORMAL;
            mSound.setLinkSearch(false);
        }
    }
}

void daE_SM_c::E_SM_Move() {
    field_0x6bc += (int)((field_0x980 + 1500.0f) / field_0x6f0);

    switch (mMode) {
        case 0:
            if (field_0xc78.ChkGroundHit()) {
                mSoundSimple.startSound(Z2SE_EN_SC_SL_ATTACK, 0, -1);
                mMode = 1;
                field_0x9b0.y = s_TargetAngle + cM_rndFX(3000.0f);
                field_0x9b8.y = 45.0f;
                field_0x9a8 = field_0x9c8.x * (cM_rndF(8.0f) + 20.0f);
            }
            break;

        case 1:
            cLib_addCalc2(&field_0x6dc, 0.02f, 0.1f, 0.005f);
            cLib_addCalc0(&field_0x6e4, 0.5f, 0.1f);

            if (field_0xc78.ChkGroundHit()) {
                mSoundSimple.startSound(Z2SE_EN_SC_SL_LAND, 0, -1);

                if (field_0xc78.ChkWaterHit()) {
                    mSoundSimple.startSound(Z2SE_CM_BODYFALL_ASASE_M, 0, -1);
                } else {
                    mSoundSimple.startSound(Z2SE_CM_BODYFALL_M, 0, -1);
                }

                mCoSm.OffAtSetBit();
                field_0x9a8 = 0.0f;
                field_0x6bc = 0;
                mAction = ACTION_DAMAGE;
                field_0x978 = -0.4f;
                field_0x97c = 0.15f;
                field_0x974 = 0.05f;
                field_0x980 = 2500.0f;
                field_0x6c0[0] = 30;
                field_0x6be = 0;

                for (int i = 0; i < 2; i++) {
                    E_SM_Damage();
                }
            } else if (field_0x9b8.y < 0.0f) {
                mCoSm.OnAtSetBit();
                cLib_addCalc(&field_0x6e0, 1.0f, 0.2f, 0.5f, 0.0f);
            } else {
                cLib_addCalc(&field_0x6e0, 1.4f, 0.5f, 0.5f, 0.0f);
            }

            cLib_addCalc2(&field_0x69c, 0.0f, 0.05f, 100.0f);
            break;
    }
}

void daE_SM_c::E_SM_Attack() {
    if (field_0x6c0[0] == 0) {
        field_0x6bc += (int)((field_0x980 + 2000.0f) / field_0x6f0);
    } else {
        field_0x6bc += (int)((field_0x980 + 1000.0f) / field_0x6f0);
    }

    switch (mMode) {
        case 0:
            SetAnm(BCK_SC_ATTACK, J3DFrameCtrl::EMode_NONE, 1.0f, 5.0f);
            mSound.startCreatureVoice(Z2SE_EN_SC_V_ATTACK, -1);
            mCoreAction = CORE_ACTION_FIGHT;
            mMode++;
            field_0x97c = 0.0f;
            field_0x6dc += 0.01f;

            if (field_0x6d4 != 0) {
                field_0x980 = 2600.0f;
            } else {
                field_0x980 = 1500.0f;
            }

            if (field_0x6dc > 0.041f) {
                field_0x6dc = 0.041f;
            }
            break;
        
        case 1:
            if (field_0x6dc > 0.041f) {
                field_0x6dc = 0.041f;
            }

            if (mpModelMorf->isStop()) {
                SetAnm(BCK_SC_S_JUMP_C, J3DFrameCtrl::EMode_NONE, 1.0f, 5.0f);
                field_0x6c0[2] = 10;
                mMode++;
            }
            break;

        case 2:
            if (field_0x6dc > 0.041f) {
                field_0x6dc = 0.041f;
            }

            if (field_0x6c0[2] == 0) {
                SetAnm(BCK_SC_MOVE02, 2, 1.0f, 5.0f);
                mAction = ACTION_MOVE;
                mMode = 0;
            } else {
                cLib_addCalc2(&current.pos.y, field_0x990.y + 20.0f, 5.0f, 0.05f);
                cLib_addCalc2(&field_0x6e0, 0.85f, 0.4f, 0.05f);
                cLib_addCalc2(&field_0x6dc, 0.0f, 0.03f, 0.01f);
            }
            break;
    }

    f32 fVar1;
    if (field_0x685 == true) {
        fVar1 = field_0x978 * cM_ssin(field_0x6be);
    } else {
        fVar1 = field_0x978;
    }

    cLib_addCalc2(&field_0x69c, 0.0f, 0.05f, 100.0f);
    cLib_chaseF(&field_0x978, 0.0f, 0.105f);
    cLib_addCalc2(&field_0x6e0, fVar1 + 1.0f, 0.3f, 1.0f);
}

void daE_SM_c::Action() {
    cXyz sp2c, sp38;

    SmDamageCheck();

    switch (mAction) {
        case ACTION_NORMAL:
            E_SM_Normal();
            break;
        
        case ACTION_MOVE:
            E_SM_Move();
            break;

        case ACTION_ATTACK:
            E_SM_Attack();
            break;

        case ACTION_DAMAGE:
            E_SM_Damage();
            break;

        case ACTION_UNK:
            field_0x9a8 = 0.0f;
            field_0x6bc += (int)((field_0x980 + 1000.0f) / field_0x6f0);
            cLib_addCalc2(&field_0x6e4, 0.0f, 0.2f, 0.1);
            cLib_addCalc2(&field_0x6e0, 1.0f, 0.2f, 0.05f);
            cLib_addCalc2(&field_0x6dc, 0.02f, 0.5f, 0.01f);
            cLib_chaseF(&field_0x69c, 0.0f, 1.0f);
            break;

        case ACTION_DELETE:
            if (mpModelMorf->isStop()) {
                if (mSwBit != 0xFF && !dComIfGs_isSwitch(mSwBit, fopAcM_GetRoomNo(this))) {
                    dComIfGs_onSwitch(mSwBit, fopAcM_GetRoomNo(this));
                }

                fopAcM_delete(this);
            }
            break;
    }

    cLib_chaseF(&field_0x980, 0.0f, 10.0f);
    cMtx_YrotS(*calc_mtx, field_0x9b0.y);
    sp2c.x = 0.0f;
    sp2c.y = 0.0f;
    sp2c.z = field_0x9a8;
    MtxPosition(&sp2c, &sp38);
    field_0x9b8.x = sp38.x;
    field_0x9b8.z = sp38.z;
    field_0x990 += field_0x9b8;
    field_0x9b8.y -= 5.0f;
    
    cXyz* ccMoveP = field_0xe8c.GetCCMoveP();
    if (ccMoveP != NULL) {
        field_0x990 += *ccMoveP;
    }

    field_0xc78.CrrPos(dComIfG_Bgsp());
}

void daE_SM_c::E_SM_C_Normal() {
    cLib_addCalcAngleS2(&current.angle.y, field_0x9b0.y, 8, 0x400);

    if (mAnm == BCK_SC_WHISTLE && mpModelMorf->checkFrame(63.0f)) {
        mSound.startCreatureVoice(Z2SE_EN_SC_V_WHISTLE2, -1);
    }
}

void daE_SM_c::E_SM_C_Fight() {
    cLib_addCalcAngleS2(&current.angle.y, s_TargetAngle, 8, 0x800);

    if (mAnm == BCK_SC_WHISTLE && mpModelMorf->checkFrame(63.0f)) {
        mSound.startCreatureVoice(Z2SE_EN_SC_V_WHISTLE2, -1);
    }
}

void daE_SM_c::E_SM_C_Near_Escape() {
    fopAcM_GetPosition(daPy_getPlayerActorClass());

    switch (field_0x6b6) {
        case 4:
            speedF = 0.0f;
            field_0x6d8 = s_TargetAngle;
            cLib_chaseAngleS(&current.angle.y, field_0x6d8, 0x1000);

            if (mpModelMorf->isStop()) {
                field_0x6b6 = 0;
                field_0x6ca[3] = 10;
                field_0x6d8 = s_TargetAngle + 0x8000;
            }
            break;
        
        case 0:
            field_0x6b6 = 3;
            cLib_chaseAngleS(&current.angle.y, field_0x6d8, 0x2000);
            field_0x9b0.y = current.angle.y;

            if (!way_bg_check(this, 400.0f, current.pos, current.angle.y)) {
                current.angle.y += cM_rndFX(2000.0f);
            } else {
                field_0x6d8 += 0x2000;
            }

            if (s_Dis > 600.0f) {
                field_0x6b4 = 1;
                field_0x6b6 = 0;
            }

            if (field_0x6ca[0] == 0.0f && !field_0x684) {
                field_0x6b4 = 2;
                field_0x6b6 = 4;
            }
            break;

        case 3:
            if (mAnm != BCK_SC_ESCAPE) {
                mSound.startCreatureVoice(Z2SE_EN_SC_V_ESCAPE, -1);
                mSound.startCreatureSound(Z2SE_EN_SC_JUMP, 0, -1);
                SetAnm(BCK_SC_ESCAPE, J3DFrameCtrl::EMode_NONE, 1.0f, 3.0f);
            }

            if (mpModelMorf->getFrame() >= 2.0f) {
                speedF = TREG_F(6) + 20.0f;
                field_0x6b6 = 1;
                mAnm = 0;
            }
            break;

        case 1:
            if (mpModelMorf->checkFrame(11.0f)) {
                speedF = 0.0f;
                field_0x6b6 = 0;
                SetAnm(BCK_SC_MOVE, J3DFrameCtrl::EMode_NONE, 1.0f, 3.0f);
            }
            break;
    }
}

void daE_SM_c::E_SM_C_Far_Escape() {
    fopAcM_GetPosition(daPy_getPlayerActorClass());

    switch (field_0x6b6) {
        case 4:
            speedF = 0.0f;

            if (mpModelMorf->isStop()) {
                field_0x6b6 = 3;
            }
            break;

        case 0:
            field_0x6b6 = 3;

            for (int i = 0; i < 100; i++) {
                field_0x6d8 = current.angle.y + (int)cM_rndFX(12000.0f);
                field_0x9b0.y = field_0x6d8;

                if (!way_bg_check(this, 400.0f, current.pos, current.angle.y)) break;
            }

            if (s_Dis <= 600.0f) {
                field_0x6b4 = 0;
                field_0x6b6 = 4;
                SetAnm(BCK_SC_KYORO, J3DFrameCtrl::EMode_NONE, 1.0f, 3.0f);
                mSound.startCreatureVoice(Z2SE_EN_SC_V_KYORO, -1);
            }

            if (field_0x6ca[0] == 0 && !field_0x684) {
                field_0x6b4 = 2;
                field_0x6b6 = 4;
            }
            break;

        case 3:
            if (mAnm != BCK_SC_ESCAPE) {
                SetAnm(BCK_SC_ESCAPE, J3DFrameCtrl::EMode_NONE, 1.0f, 3.0f);
                mSound.startCreatureVoice(Z2SE_EN_SC_V_ESCAPE, -1);
                mSound.startCreatureSound(Z2SE_EN_SC_JUMP, 0, -1);
            }

            if (mpModelMorf->getFrame() >= 2.0f) {
                speedF = 20.0f;
                field_0x6b6 = 1;
                mAnm = BCK_SC_S_JUMP_A;
            }
            break;

        case 1:
            if (mpModelMorf->checkFrame(11.0f)) {
                speedF = 0.0f;
                field_0x6b6 = 0;
            }
            break;
    }

    cLib_chaseAngleS(&current.angle.y, field_0x6d8, 0x400);
}

void daE_SM_c::E_SM_C_Home_Escape() {
    switch (field_0x6b6) {
        case 4:
            speedF = 0.0f;

            if (mpModelMorf->isStop()) {
                field_0x6b6 = 5;
            }

            mAnm = BCK_SC_S_JUMP_A;
            break;

        case 0:
            if (mpModelMorf->checkFrame(11.0f)) {
                speedF = 0.0f;
                field_0x6b6 = 5;

                if (field_0xa60.ChkWaterHit()) {
                    mSound.startCreatureSound(Z2SE_EN_SC_LAND_WATER, 0, -1);
                } else {
                    mSound.startCreatureSound(Z2SE_EN_SC_LAND, 0, -1);
                }
            }
            break;

        case 5:
            mCoCore.OnCoSetBit();
            field_0x6d8 = cLib_targetAngleY(&current.pos, &field_0x990);
            cLib_chaseAngleS(&current.angle.y, field_0x6d8, 0x1000);

            if (current.pos.absXZ(field_0x990) < oREG_F(0) + 220.0f) {
                field_0x6b6 = 7;
                SetAnm(BCK_SC_S_JUMP_A, J3DFrameCtrl::EMode_NONE, 1.0f, 3.0f);
                mSound.startCreatureSound(Z2SE_EN_SC_JUMP, 0, -1);
                mCoCore.OffTgNoHitMark();
                mCoCore.OffTgSetBit();
            } else {
                if (mAnm != BCK_SC_ESCAPE) {
                    SetAnm(BCK_SC_ESCAPE, J3DFrameCtrl::EMode_NONE, 1.0f, 3.0f);
                    mSound.startCreatureVoice(Z2SE_EN_SC_V_ESCAPE, -1);
                    mSound.startCreatureSound(Z2SE_EN_SC_JUMP, 0, -1);
                }

                if (mpModelMorf->checkFrame(2.0f)) {
                    speedF = 20.0f;
                    field_0x6b6 = 0;
                    mAnm = BCK_SC_S_JUMP_A;
                }
            }
            break;

        case 7:
            if (mAnm == BCK_SC_S_JUMP_A && mpModelMorf->isStop()) {
                speed.y = oREG_F(1) + 73.0f;
                speedF = oREG_F(2) + 10.0f;
                current.angle.y = cLib_targetAngleY(&current.pos, &field_0x990);
                field_0x6ca[3] = 15;
                SetAnm(BCK_SC_S_JUMP_B, J3DFrameCtrl::EMode_LOOP, 1.0f, 3.0f);
            }

            if (mAnm == BCK_SC_S_JUMP_B) {
                speedF = oREG_F(2) + 10.0f;
            }

            if (speed.y <= 0.0f && mAnm == BCK_SC_S_JUMP_B && mCoCore.ChkCoHit()) {
                mCoCore.OffCoSameActorHit();
                mCoCore.OffCoSetBit();
                mSoundSimple.startSound(Z2SE_EN_SC_SL_JUMP_IN, 0, -1);
                field_0x6b6 = 6;

                cXyz sp20(0.5f, 0.5f, 0.5f);
                sp20 *= 1.0f;
                dComIfGp_particle_set(0x8781, &current.pos, &tevStr, &current.angle, &scale);
                dComIfGp_particle_set(0x8782, &current.pos, &tevStr, &current.angle, &scale);
                dComIfGp_particle_set(0x8783, &current.pos, &tevStr, &current.angle, &scale);

                SetAnm(BCK_SC_S_JUMP_C, J3DFrameCtrl::EMode_NONE, 1.0f, 5.0f);
                mAction = ACTION_DAMAGE;
                field_0x978 = XREG_F(4) + -0.6f;
                field_0x6dc = XREG_F(2) + 0.04f;
                field_0x97c = XREG_F(3) + 0.05f;
                field_0x974 = 0.05f;
                field_0x980 = XREG_F(5) + 3000.0f;
                field_0x6c0[0] = 40;
                field_0x6be = 0;

                for (int i = 0; i < 2; i++) {
                    E_SM_Damage();
                }

                field_0x6e8 = 0.36f;
                field_0x6c0[0] = 30;
                field_0xe50.ClrCcMove();
                field_0xe8c.SetWeight(150);
            }
            break;

        case 20:
            if (current.pos.y < field_0x990.y + 80.0f) {
                speed.y = 0.0f;
                speedF = 0.0f;

                if (mAnm != BCK_SC_S_JUMP_C) {
                    SetAnm(BCK_SC_S_JUMP_C, J3DFrameCtrl::EMode_NONE, 1.0f, 3.0f);
                }

                cXyz sp2c;
                sp2c = field_0x990;
                sp2c.y += 80.0f;

                if (cLib_chasePos(&current.pos, sp2c, 6.0f) != 0) {
                    mCoreAction = CORE_ACTION_NORMAL;
                    field_0x6b2 = 0;
                    mAction = ACTION_NORMAL;
                    mMode = 0;
                }
            }
            break;

        case 6:
            field_0xe50.ClrCcMove();
            speedF = 0.0f;
            cLib_chaseF(&speed.y, 0.0f, yREG_F(3) + 17.0f);
            cLib_chaseF(&speedF, 0.0f, yREG_F(7) + 3.5f);
            cLib_chaseF(&current.pos.x, field_0x990.x, 8.0f);
            cLib_chaseF(&current.pos.z, field_0x990.z, 8.0f);

            if (mpModelMorf->isStop()) {
                SetAnm(BCK_SC_MOVE02, J3DFrameCtrl::EMode_LOOP, 1.0f, 5.0f);
            }

            if (speed.y == 0.0f && speedF == 0.0f) {
                cXyz sp38;
                sp38 = field_0x990;
                sp38.y += 80.0f;

                if (cLib_chasePos(&current.pos, sp38, oREG_F(7) + 5.0f) != 0) {
                    mCoCore.SetTgType(0);
                    mCoCore.SetTgType(0x4000);
                    field_0x6be = 0;
                    field_0x9c4 = 0;

                    if (!field_0xa08) {
                        mCoreAction = CORE_ACTION_NORMAL;
                        field_0x6b2 = 0;
                        field_0x6b4 = 0;
                        field_0x6b6 = 0;
                        mAction = ACTION_NORMAL;
                        mMode = 0;
                    }

                    field_0x68c = true;
                    mCoCore.OnTgSetBit();
                }
            }
            break;
    }

    cLib_chaseAngleS(&current.angle.y, field_0x6d8, 0x400);
}

void daE_SM_c::E_SM_C_Escape() {
    switch (field_0x6b4) {
        case 0:
            E_SM_C_Near_Escape();
            break;
        
        case 1:
            E_SM_C_Far_Escape();
            break;

        case 2:
            E_SM_C_Home_Escape();
            break;
    }
}

void daE_SM_c::E_SM_C_Free() {
    fopAcM_GetPosition(daPy_getPlayerActorClass());

    switch (field_0x6b2) {
        case 0:
            if (field_0x68c) {
                field_0xa18 = dComIfGp_particle_set(field_0xa18, 0x8784, &current.pos, &tevStr, &current.angle, &scale, 0xFF, NULL, -1, NULL, NULL, NULL);
                field_0xa1c = dComIfGp_particle_set(field_0xa1c, 0x8788, &current.pos, &tevStr, &current.angle, &scale, 0xFF, NULL, -1, NULL, NULL, NULL);
            }

            field_0x9c4 = 1;

            if (mpModelMorf->checkFrame(7.0f)) {
                if (field_0xa60.ChkWaterHit()) {
                    mSound.startCreatureSound(Z2SE_CM_BODYFALL_ASASE_S, 0, -1);
                } else {
                    mSound.startCreatureSound(Z2SE_CM_BODYFALL_S, 0, -1);
                }
            }

            if (mpModelMorf->checkFrame(53.0f)) {
                if (field_0xa60.ChkWaterHit()) {
                    mSound.startCreatureSound(Z2SE_EN_SC_LAND_WATER, 0, -1);
                } else {
                    mSound.startCreatureSound(Z2SE_EN_SC_LAND, 0, -1);
                }
            }

            if (mpModelMorf->checkFrame(32.0f)) {
                mSound.startCreatureVoice(Z2SE_EN_SC_V_LIBERATION, -1);
            }

            if (mpModelMorf->isStop()) {
                mCoCore.SetTgType(0xD8FBFDFF);
                mCoCore.OnCoSetBit();
                mCoCore.OnCoSameActorHit();
                mCoCore.OnCoSameActorHit();
                field_0x6b2 = 1;
                field_0x6b4 = 0;
                field_0x6b6 = 0;
                field_0x6d8 = s_TargetAngle + 0x8000;
            }
            break;
        
        case 1:
            E_SM_C_Escape();
            break;
    }
}

void daE_SM_c::E_SM_C_Death() {
    field_0xe50.ClrCcMove();

    switch (field_0x6b4) {
        case 0: {
            attention_info.flags = 0;
            s16 sVar1 = current.angle.y - s_TargetAngle;

            if (sVar1 > -0x4000 && sVar1 < 0x4000) {
                speed.y = 35.0f;
                speedF = -15.0f;
                field_0x6b4 = 1;
            } else {
                speed.y = 35.0f;
                speedF = 10.0f;
                field_0x6b4 = 1;
            }
            break;
        }

        case 1:
            if (field_0xa60.ChkGroundHit()) {
                SetAnm(BCK_SC_DEAD, J3DFrameCtrl::EMode_NONE, 1.0f, 3.0f);
                field_0x6b4 = 2;
                speedF = 0.0f;
                speed.set(0.0f, 0.0f, 0.0f);
            }
            break;
        
        case 2:
            speedF = 0.0f;

            if (mpModelMorf->checkFrame(4.0f)) {
                if (field_0xa60.ChkWaterHit()) {
                    mSound.startCreatureSound(Z2SE_CM_BODYFALL_ASASE_S, 0, -1);
                } else {
                    mSound.startCreatureSound(Z2SE_CM_BODYFALL_S, 0, -1);
                }
            }

            if (mpModelMorf->isStop()) {
                field_0x698 = true;

                if (cLib_chaseF(&field_0x690, 0.1f, 0.005f) != 0) {
                    fopAcM_createDisappear(this, &current.pos, 10, 0, 23);

                    if (field_0x684) {
                        fopAcM_delete(this);
                        if (mSwBit != 0xFF) {
                            if (!dComIfGs_isSwitch(mSwBit, fopAcM_GetRoomNo(this))) {
                                dComIfGs_onSwitch(mSwBit, fopAcM_GetRoomNo(this));
                            }
                        }
                    }

                    field_0x683 = true;
                    field_0x6b4 = 3;
                    field_0x6ca[2] = 30;
                }
            }
            break;

        case 3:
            cLib_chaseF(&field_0x6e0, XREG_F(6) + 0.7f, 0.06f);
            cLib_chaseF(&field_0x694, 0.0f, 0.03f);
            cLib_chaseF(&field_0x6b8, 0.0f, 0.03f);

            if (field_0x6ca[2] == 0) {
                if (!field_0x682) {
                    field_0x682 = true;

                    for (int i = 0; i < 30; i++) {
                        daArrow_c* arrow_p;
                        if (fopAcM_SearchByID(mArrowProcIDs[i], (fopAc_ac_c**)&arrow_p) != 0 && arrow_p != NULL) {
                            arrow_p->deleteArrow();
                        }
                    }
                }

                field_0x6b4 = 4;
                field_0x6ca[2] = 30;
                field_0x684 = true;
            }
            break;

        case 4:
            if (mSwBit != 0xFF) {
                if (!dComIfGs_isSwitch(mSwBit, fopAcM_GetRoomNo(this))) {
                    dComIfGs_onSwitch(mSwBit, fopAcM_GetRoomNo(this));
                }
            }

            fopAcM_delete(this);
            break;
    }
}

void daE_SM_c::C_DamageCheck() {
    cXyz& pos = fopAcM_GetPosition(daPy_getPlayerActorClass());

    if (field_0x6d6 > 0) {
        mCoCore.ClrTgHit();
    } else {
        field_0xe8c.Move();
        field_0xe50.Move();

        if (mCoCore.ChkTgHit()) {
            field_0xe50.ClrCcMove();
            speedF = 0.0f;
            field_0x6d6 = 10;
            field_0x6b6 = 4;
            s16 sVar1 = current.angle.y - s_TargetAngle;

            if (sVar1 > 0x4000 || sVar1 < -0x4000) {
                SetAnm(BCK_SC_DAMAGEB, J3DFrameCtrl::EMode_NONE, 1.0f, 3.0f);
            } else if (sVar1 <= 0x4000 && sVar1 >= 0) {
                SetAnm(BCK_SC_DAMAGEL, J3DFrameCtrl::EMode_NONE, 1.0f, 3.0f);
            } else {
                SetAnm(BCK_SC_DAMAGER, J3DFrameCtrl::EMode_NONE, 1.0f, 3.0f);
            }

            mAtInfo.mpCollider = mCoCore.GetTgHitObj();
            mAtInfo.field_0x18 = 30;
            cc_at_check(this, &mAtInfo);
            speedF = 0.0f;

            if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_HOOKSHOT)) {
                mCoCore.SetTgType(0xD8FBFDFF);

                if (mCoreAction == CORE_ACTION_NORMAL) {
                    field_0x68c = true;
                } else {
                    field_0x68c = false;
                }

                csXyz i_rotation(0, fopAcM_searchPlayerAngleY(this), 0);

                if (mCoreAction != CORE_ACTION_FREE) {
                    dComIfGp_particle_set(0x877E, &current.pos, &tevStr, &i_rotation, &scale);
                    dComIfGp_particle_set(0x877F, &current.pos, &tevStr, &i_rotation, &scale);
                    dComIfGp_particle_set(0x8780, &current.pos, &tevStr, &i_rotation, &scale);
                    field_0x6ac = true;
                    field_0x6dc = 0.05f;
                    field_0x978 = -0.1f;
                    field_0x6dc = 0.1f;
                    field_0x6c0[0] = 50;
                    field_0x980 = 2000.0f;
                    field_0x6e8 = -0.33f;
                    mAction = ACTION_DAMAGE;
                } else {
                    field_0x6ac = false;
                }

                mSound.startCreatureVoice(Z2SE_EN_SC_V_F_SHOCK, -1);
                field_0x6c0[0] = 10;
                mCoCore.OffTgNoHitMark();
                mCoreAction = CORE_ACTION_HOOK;
                SetAnm(BCK_SC_F_SHOCK, J3DFrameCtrl::EMode_LOOP, 1.0f, 3.0f);
                speedF = 0.0f;
                field_0xe8c.SetWeight(254);
            } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_IRON_BALL)) {
                if (mCoreAction != CORE_ACTION_FREE) {
                    mCoreAction = CORE_ACTION_DEATH;
                    mSound.startCreatureVoice(Z2SE_EN_SC_V_DEAD, -1);
                    field_0x6b2 = 1;
                    field_0x6b4 = 0;
                    mAction = ACTION_UNK;
                    cXyz i_scale(1.5f, 1.5f, 1.5f);
                    Particle_Set(0x85BF, i_scale, current.pos, current.angle);
                    field_0x684 = true;

                    for (int i = 0; i < 30; i++) {
                        field_0x682 = true;
                        daArrow_c* arrow_p;
                        if (fopAcM_SearchByID(mArrowProcIDs[i], (fopAc_ac_c**)&arrow_p) != 0 && arrow_p != NULL) {
                            arrow_p->deleteArrow();
                        }
                    }
                } else {
                    mCoreAction = CORE_ACTION_DEATH;
                    mSound.startCreatureVoice(Z2SE_EN_SC_V_DEAD, -1);
                    field_0x6b2 = 0;
                    field_0x6b4 = 0;
                }
            } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_SHIELD_ATTACK)) {
                mSound.startCreatureVoice(Z2SE_EN_SC_V_DAMAGE, -1);
            } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG)) {
                mSound.startCreatureVoice(Z2SE_EN_SC_V_DAMAGE, -1);
            } else {
                field_0x984--;

                if (field_0x984 > 0) {
                    mSound.startCreatureVoice(Z2SE_EN_SC_V_DAMAGE, -1);
                }
            }

            field_0xe50.Move();
            mCoCore.ClrTgHit();
        }

        if (field_0x984 <= 0 || health <= 0) {
            health = 0;
            mCoreAction = CORE_ACTION_DEATH;
            mSound.startCreatureVoice(Z2SE_EN_SC_V_DEAD, -1);
            field_0x6b2 = 0;
            field_0x6b4 = 0;
        }
    }
}

void daE_SM_c::E_SM_C_Hook() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    fopAcM_GetPosition(player);

    if (!player->checkHookshotReturnMode() || field_0xa60.ChkWallHit()) {
        fopAcM_cancelHookCarryNow(this);
        speedF = IREG_F(18);
        speed.set(0.0f, 0.0f, 0.0f);
        current.pos.y -= 40.0f;
        mCoreAction = CORE_ACTION_FREE;
        field_0x6b2 = 0;
        SetAnm(BCK_SC_LIBERATION, J3DFrameCtrl::EMode_NONE, 1.0f, 3.0f);
        field_0x6ac = false;
    } else {
        if (strcmp("D_SB01", dComIfGp_getStartStageName()) == 0 && current.pos.y > field_0x990.y + 800.0f) {
            fopAcM_cancelHookCarryNow(this);
            speedF = IREG_F(18);
            speed.set(0.0f, 0.0f, 0.0f);
            current.pos.y -= 40.0f;
            mCoreAction = CORE_ACTION_FREE;
            field_0x6b2 = 0;
            SetAnm(BCK_SC_LIBERATION, J3DFrameCtrl::EMode_NONE, 1.0f, 3.0f);
            field_0x6ac = false;
        }

        cXyz sp28(0.0f, yREG_F(4) + -60.0f, 0.0f);
        daPy_getPlayerActorClass()->setHookshotCarryOffset(fopAcM_GetID(this), &sp28);

        if (mAnm == BCK_SC_F_SHOCK && mpModelMorf->isStop()) {
            SetAnm(BCK_SC_DRAW, J3DFrameCtrl::EMode_LOOP, 1.0f, 3.0f);
        }
    }

}

bool daE_SM_c::CheckViewArea() {
    Vec vec;
    mDoLib_project(&current.pos, &vec);
    bool rv = false;

    if (vec.x >= 0.0f && vec.x <= FB_WIDTH && vec.y >= 0.0f && vec.y <= FB_HEIGHT) {
        rv = true;
    }

    return rv;
}

bool daE_SM_c::CameraSet() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    dCamera_c* camBody = dCam_getBody();
    bool rv;

    if (!eventInfo.checkCommandDemoAccrpt()) {
        fopAcM_orderPotentialEvent(this, 2, 0xFFFF, 0);
        eventInfo.onCondition(dEvtCnd_NONE_e);
        rv = false;
    } else {
        camera->mCamera.Stop();
        mFovy = camBody->Fovy();
        mDemoCamCenter = camBody->Center();
        field_0x9e0 = mDemoCamCenter;
        mDemoCamEye = camBody->Eye();
        field_0x9f8 = mDemoCamEye;
        camera->mCamera.SetTrimSize(3);
        rv = true;
    }

    return rv;
}

void daE_SM_c::SetStopCam(cXyz param_1, f32 param_2, f32 param_3, s16 param_4) {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz sp28(0.0f, param_3, param_2);
    cXyz eye(0.0f, 0.0f, 0.0f);

    mDemoCamCenter.set(param_1.x, param_1.y, param_1.z);
    field_0x9e0 = mDemoCamCenter;
    cLib_offsetPos(&eye, &param_1, param_4, &sp28);
    mDemoCamEye = eye;
    field_0x9f8 = eye;
    camera->mCamera.Set(mDemoCamCenter, mDemoCamEye, mFovy, 0);
}

void daE_SM_c::SetCMoveCam(cXyz param_1, f32 i_scale, f32 i_maxStep) {
    field_0x9e0 = param_1;
    cLib_addCalcPos2(&mDemoCamCenter, field_0x9e0, i_scale, i_maxStep);
}

void daE_SM_c::SetStopingCam() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera->mCamera.Set(mDemoCamCenter, mDemoCamEye, mFovy, 0);
}

void daE_SM_c::SetReleaseCam() {
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));

    camera->mCamera.Reset(mDemoCamCenter, mDemoCamEye);
    camera->mCamera.Start();
    camera->mCamera.SetTrimSize(0);
    dComIfGp_event_reset();
}

void daE_SM_c::DemoStart() {
    cXyz sp18;

    switch (mDemoMode) {
        case 0:
            if (CameraSet()) {
                current.angle.y = s_TargetAngle;
                shape_angle.y = current.angle.y;
                SetAnm(BCK_SC_WAIT02, J3DFrameCtrl::EMode_NONE, 1.0f, 5.0f);
                current.pos.y = field_0xa60.GetGroundH();
                sp18 = current.pos;
                sp18.y += 80.0f;
                SetStopCam(sp18, 300.0f, 0.0f, s_TargetAngle);
                field_0x6c0[0] = 10;
                mDemoMode++;
                field_0xe8c.SetWeight(254);
            }
            break;
        
        case 1:
            if (field_0x6c0[0] == 0) {
                mDemoMode++;
                SetAnm(BCK_SC_KYORO, J3DFrameCtrl::EMode_NONE, 1.0f, 5.0f);
                mSound.startCreatureVoice(Z2SE_EN_SC_V_KYORO, -1);
                field_0x6b2 = 0;
                field_0x6b4 = 2;
            }
            break;

        case 2:
            if (!CheckViewArea()) {
                mCDemoMode = 1;
                mDemoMode = 0;
            }

            E_SM_C_Free();
            break;
    }
}

void daE_SM_c::DemoMid() {
    cXyz sp1c;
    bool bVar1 = true;

    switch (mDemoMode) {
        case 0:
            SetStopCam(field_0x990, 500.0f, 100.0f, s_TargetAngle);
            mDemoCamCenter = current.pos;
            mDemoCamCenter.y += 80.0f;
            field_0x9e0 = mDemoCamCenter;
            SetStopingCam();
            mDemoMode++;
            break;
        
        case 1:
            sp1c = current.pos;
            sp1c.y += 80.0f;
            SetCMoveCam(sp1c, yREG_F(6) + 0.1f, yREG_F(5) + 25.0f);
            SetStopingCam();

            if (field_0x9c4 == 0) {
                if (mAnm != BCK_SC_PROVOKE && mAnm != BCK_SC_MOVE02) {
                    mMode = 3;
                    SetAnm(BCK_SC_PROVOKE, J3DFrameCtrl::EMode_NONE, 1.0f, 5.0f);
                    mSound.startCreatureVoice(Z2SE_EN_SC_V_PROVOKE, -1);
                } else {
                    if (mpModelMorf->isStop()) {
                        mAction = ACTION_NORMAL;
                        mMode = 0;
                        field_0x98c = 0;
                        mCoreAction = CORE_ACTION_NORMAL;
                        field_0x6b2 = 0;
                        field_0x6b4 = 0;
                        field_0x6b6 = 0;
                        SetReleaseCam();
                        field_0xa08 = false;

                        if (mSwBit2 != 0xFF && !dComIfGs_isSwitch(mSwBit2, fopAcM_GetRoomNo(this))) {
                            dComIfGs_onSwitch(mSwBit2, fopAcM_GetRoomNo(this));
                        }
                    }
                }

                bVar1 = false;
                field_0x6d8 = cLib_targetAngleY(&current.pos, &mDemoCamEye);
                current.angle.y = field_0x6d8;
                shape_angle.y = current.angle.y;
            }
            break;
    }

    if (bVar1) {
        E_SM_C_Free();
    }

    field_0x9b0.y = s_TargetAngle;
}

void daE_SM_c::E_SM_C_Demo() {
    switch (mCDemoMode) {
        case 0:
            DemoStart();
            break;
        
        case 1:
            DemoMid();
            break;
    }
}

void daE_SM_c::CoreAction() {
    cXyz sp1c, sp28;
    s8 sVar1 = 0;

    switch (mCoreAction) {
        case CORE_ACTION_NORMAL:
            C_DamageCheck();
            E_SM_C_Normal();
            mCoCore.OnTgNoHitMark();
            break;

        case CORE_ACTION_FIGHT:
            C_DamageCheck();
            E_SM_C_Fight();
            break;

        case CORE_ACTION_FREE:
            if (field_0x6b6 != 7 && field_0x6b6 != 6) {
                C_DamageCheck();
            } else {
                field_0x6d6 = 0;
                field_0x6d4 = 0;
                mCoCore.ClrTgHit();
                mCoSm.ClrTgHit();
            }

            E_SM_C_Free();
            sVar1 = 1;
            break;

        case CORE_ACTION_DEATH:
            E_SM_C_Death();
            sVar1 = 1;
            break;

        case CORE_ACTION_HOOK:
            mCoCore.OffTgNoHitMark();
            E_SM_C_Hook();
            sVar1 = 1;
            break;

        case CORE_ACTION_DEMO:
            E_SM_C_Demo();
            sVar1 = 1;
            break;
    }

    if (sVar1 == 0) {
        current.pos.set(field_0x990.x, field_0x990.y + 80.0f, field_0x990.z);
        current.pos.y += std::abs(cM_ssin(field_0x6be)) * -25.0f;
    } else {
        if (sVar1 == 1) {
            fopAcM_posMoveF(this, field_0xe50.GetCCMoveP());

            if (mCoreAction != CORE_ACTION_NORMAL) {
                if (field_0x6b6 != 6) {
                    gravity = -9.0f;
                } else {
                    gravity = 0.0f;
                    current.pos.y += speed.y;
                }
            } else {
                gravity = 0.0f;
            }

            field_0xa60.CrrPos(dComIfG_Bgsp());
            fopAcM_OffStatus(this, fopAcM_STATUS_UNK_0x100);
        }
    }

    cLib_addCalcAngleS2(&shape_angle.y, current.angle.y, 5, 0x1000);
}

void daE_SM_c::ArrowOn() {
    cXyz sp68, sp74;
    f32 fVar1 = 1.0f - field_0x6e4 * 0.2f;
    f32 fVar2 = field_0x6f0 / field_0x6e0;
    f32 fVar3 = field_0x6f0 * field_0x6e0;
    f32 fVar4 = 1.0f / field_0x6f0;

    for (int i = 0; i < 30; i++) {
        if (field_0x6f9[i] != 0) {
            mDoMtx_stack_c::transS(field_0x990.x, field_0x990.y, field_0x990.z);
            mDoMtx_stack_c::YrotM(field_0x9b0.y);
            mDoMtx_stack_c::scaleM(fVar1, fVar1, field_0x6e4 + 1.0f);
            mDoMtx_stack_c::YrotM(-field_0x9b0.y);
            mDoMtx_stack_c::scaleM(fVar2, fVar3, fVar2);
            mDoMtx_stack_c::transM(20.0f, 0.0f, 20.0f);
            mDoMtx_stack_c::ZrotM(field_0x9b0.z);
            mDoMtx_stack_c::XrotM(field_0x9b0.z);
            mDoMtx_stack_c::scaleM(1.0f - field_0x6dc, field_0x6dc + 1.0f, 1.0f - field_0x6dc);
            mDoMtx_stack_c::XrotM(-field_0x9b0.z);
            mDoMtx_stack_c::ZrotM(-field_0x9b0.z);
            mDoMtx_stack_c::transM(-20.0f, -0.0f, -20.0f);
            mDoMtx_stack_c::transM(0.0f, 50.0f, 0.0f);
            mDoMtx_stack_c::YrotM(field_0x808[i].y);
            mDoMtx_stack_c::XrotM(field_0x808[i].x);
            mDoMtx_stack_c::transM(0.0f, 0.0f, field_0x790[i]);
            mDoMtx_stack_c::YrotM(field_0x8bc[i].y);
            mDoMtx_stack_c::XrotM(field_0x8bc[i].x);

            cLib_addCalcAngleS2(&field_0x8bc[i].y, 0, 8, 0x800);
            cLib_addCalcAngleS2(&field_0x8bc[i].x, 0, 8, 0x800);
            mDoMtx_stack_c::scaleM(fVar4, fVar4, fVar4);

            daArrow_c* arrow_p;
            if (fopAcM_SearchByID(mArrowProcIDs[i], (fopAc_ac_c**)&arrow_p) != 0 && arrow_p != NULL) {
                arrow_p->setModelMatrix(mDoMtx_stack_c::get());
            }
        }
    }
}

int daE_SM_c::Execute() {
    field_0x99c = field_0x990;
    s_TargetAngle = fopAcM_searchPlayerAngleY(this);
    s_Dis = fopAcM_searchPlayerDistanceXZ(this);
    cLib_addCalc2(&field_0x6f0, scale.x * l_HIO.basic_size, 0.1f, 0.1f);

    for (int i = 0; i < 5; i++) {
        if (field_0x6c0[i] != 0) {
            field_0x6c0[i]--;
        }

        if (field_0x6ca[i] != 0) {
            field_0x6ca[i]--;
        }
    }

    if (field_0x6d4 != 0) {
        field_0x6d4--;
    }

    if (field_0x6d6 != 0) {
        field_0x6d6--;
    }

    fopAcM_OnStatus(this, fopAcM_STATUS_UNK_0x100);
    Action();
    CoreAction();
    cXyz i_effSize(1.0f, 1.0f, 1.0f);
    setMidnaBindEffect(this, &mSound, &current.pos, &i_effSize);
    setSmBaseMtx();
    setBaseMtx();
    CoSmSet();
    CoCoreSet();

    if (mCoreAction == CORE_ACTION_FREE) {
        eyePos = current.pos;
        eyePos.y += 80.0f;
        attention_info.position = eyePos;
        attention_info.position.y += 80.0f;
    } else {
        eyePos = current.pos;
        eyePos.y += 10.0f;
        attention_info.position = eyePos;
        attention_info.position.y += 50.0f;
    }

    mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));

    if (!field_0x682) {
        ArrowOn();
    }

    return 1;
}

void daE_SM_c::setSmBaseMtx() {
    mDoMtx_stack_c::transS(field_0x990);
    mDoMtx_stack_c::YrotM(field_0x9b0.y);

    f32 fVar1 = 1.0f - field_0x6e4 * 0.2f;
    mDoMtx_stack_c::scaleM(fVar1, fVar1, field_0x6e4 + 1.0f);
    mDoMtx_stack_c::transM(0.0f, 0.0f, field_0x69c);

    fVar1 = field_0x6f0 / field_0x6e0;
    mDoMtx_stack_c::scaleM(fVar1, field_0x6f0 * field_0x6e0, fVar1);
    mDoMtx_stack_c::transM(0.0f, 0.0f, -field_0x69c);
    mDoMtx_stack_c::YrotM(-field_0x9b0.y);

    field_0x9b0.z = field_0x6bc;
    mDoMtx_stack_c::transM(20.0f, 0.0f, 20.0f);
    mDoMtx_stack_c::ZrotM(field_0x9b0.z);
    mDoMtx_stack_c::XrotM(field_0x9b0.z);
    mDoMtx_stack_c::scaleM(1.0f - field_0x6dc, field_0x6dc + 1.0f, 1.0f - field_0x6dc);
    mDoMtx_stack_c::XrotM(-field_0x9b0.z);
    mDoMtx_stack_c::ZrotM(-field_0x9b0.z);
    mDoMtx_stack_c::transM(-20.0f, -0.0f, -20.0f);
    mDoMtx_stack_c::transM(20.0f, 0.0f, -20.0f);
    mDoMtx_stack_c::ZrotM(field_0x9b0.z);
    mDoMtx_stack_c::YrotM(field_0x9b0.z);
    mDoMtx_stack_c::scaleM(1.0f - field_0x6dc, field_0x6dc + 1.0f, 1.0f - field_0x6dc);
    mDoMtx_stack_c::YrotM(-field_0x9b0.z);
    mDoMtx_stack_c::ZrotM(-field_0x9b0.z);
    mDoMtx_stack_c::transM(-20.0f, -0.0f, 20.0f);
    mDoMtx_stack_c::transM(-20.0f, 0.0f, 20.0f);
    mDoMtx_stack_c::ZrotM(field_0x9b0.z);
    mDoMtx_stack_c::scaleM(1.0f - field_0x6dc, field_0x6dc + 1.0f, 1.0f - field_0x6dc);
    mDoMtx_stack_c::ZrotM(-field_0x9b0.z);
    mDoMtx_stack_c::transM(20.0f, -0.0f, -20.0f);
    mDoMtx_stack_c::transM(-20.0f, 0.0f, -20.0f);
    mDoMtx_stack_c::XrotM(field_0x9b0.z);
    mDoMtx_stack_c::scaleM(1.0f - field_0x6dc, field_0x6dc + 1.0f, 1.0f - field_0x6dc);
    mDoMtx_stack_c::XrotM(-field_0x9b0.z);
    mDoMtx_stack_c::transM(20.0f, -0.0f, 20.0f);
    mpBubbleModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

void daE_SM_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::XrotM(shape_angle.x);
    mDoMtx_stack_c::ZrotM(shape_angle.z);
    mDoMtx_stack_c::scaleM(l_HIO.core_size, l_HIO.core_size, l_HIO.core_size);
    mpModelMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpModelMorf->modelCalc();
}

void daE_SM_c::CoCoreSet() {
    cXyz sp18;
    sp18 = current.pos;

    if (mCoreAction == CORE_ACTION_DEATH) {
        if (field_0x6b4 == 0 || field_0x6b4 == 1) {
            sp18.y = current.pos.y + 80.0f;
            mCoCore.SetC(sp18);
            mCoCore.SetR(40.0f);
            dComIfG_Ccsp()->Set(&mCoCore);
        }
    } else if (mCoreAction == CORE_ACTION_NORMAL || mCoreAction == CORE_ACTION_FIGHT) {
        sp18.y = current.pos.y;
        mCoCore.SetC(sp18);
        mCoCore.SetR(KREG_F(6) + 40.0f);
        dComIfG_Ccsp()->Set(&mCoCore);
    } else {
        sp18.y = current.pos.y + 80.0f;
        mCoCore.SetC(sp18);
        mCoCore.SetR(KREG_F(6) + 40.0f);
        dComIfG_Ccsp()->Set(&mCoCore);
    }
}

void daE_SM_c::CoSmSet() {
    cXyz sp18(0.0f, field_0x6f0 * 80.0f, 0.0f);
    sp18 += field_0x990;

    if (!field_0x684) {
        mCoSm.SetC(sp18);
        mCoSm.SetR(field_0x6f0 * 150.0f);
        dComIfG_Ccsp()->Set(&mCoSm);
    }
}

static int daE_SM_IsDelete(daE_SM_c* i_this) {
    return 1;
}

static int daE_SM_Delete(daE_SM_c* i_this) {
    return i_this->Delete();
}

static int daE_SM_Draw(daE_SM_c* i_this) {
    return i_this->Draw();
}

static int daE_SM_Execute(daE_SM_c* i_this) {
    return i_this->Execute();
}

static int daE_SM_Create(fopAc_ac_c* a_this) {
    return static_cast<daE_SM_c*>(a_this)->Create();
}

int daE_SM_c::Delete() {
    dComIfG_resDelete(&mPhase, "E_SM");

    for (int i = 0; i < 30; i++) {
        daArrow_c* arrow_p;
        if (fopAcM_SearchByID(mArrowProcIDs[i], (fopAc_ac_c**)&arrow_p) != 0 && arrow_p != NULL) {
            arrow_p->deleteArrow();
        }
    }

    if (field_0x115c != 0) {
        hio_set = 0;
    }

    if (heap != NULL) {
        mpModelMorf->stopZelAnime();
    }

    mSoundSimple.deleteObject();

    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    return static_cast<daE_SM_c*>(a_this)->CreateHeap();
}

int daE_SM_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_SM", BMDE_SM);

    JUT_ASSERT(3246, modelData != NULL);

    mpBubbleModel = mDoExt_J3DModel__create(modelData, 0, 0x11020203);
    if (mpBubbleModel == NULL) {
        return 0;
    }

    mpModelMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_SM", BMDR_SC), NULL, NULL,
                                       (J3DAnmTransform*)dComIfG_getObjectRes("E_SM", BCK_SC_WAIT), 2, 1.0f, 0, -1, &mSound, 0x80000, 0x11000084);
    if (mpModelMorf == NULL || mpModelMorf->getModel() == NULL) {
        return 0;
    }

    return 1;
}

void daE_SM_c::initCoSph() {
    static dCcD_SrcSph cc_sph_src = {
        {
            {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0xd}, {0xd8fbbdff, 0x3}, 0x75}}, // mObj
            {dCcD_SE_SOFT_BODY, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x1}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 20.0f} // mSph
        } // mSphAttr
    };

    static dCcD_SrcSph core_sph_src = {
        {
            {0x0, {{0x0, 0x0, 0xd}, {0x4000, 0x43}, 0x75}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
        } // mSphAttr
    };

    field_0xe50.Init(150, 0, this);
    field_0xe8c.Init(150, 0, this);
    field_0x6f0 = scale.x * l_HIO.basic_size;
    mCoSm.Set(cc_sph_src);
    mCoSm.SetStts(&field_0xe8c);
    mCoSm.OnTgNoHitMark();
    mCoSm.OffAtSetBit();
    mCoSm.OnTgIronBallRebound();
    mCoCore.OffCoSetBit();
    mCoCore.Set(core_sph_src);
    mCoCore.SetStts(&field_0xe50);
    mCoCore.OnTgNoHitMark();
}

cPhs__Step daE_SM_c::Create() {
    fopAcM_ct(this, daE_SM_c);

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&mPhase, "E_SM");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("E_SM PARAM %x\n", fopAcM_GetParam(this));

        mSwBit = fopAcM_GetParam(this) >> 8;
        if (mSwBit != 0xFF) {
            if (dComIfGs_isSwitch(mSwBit, fopAcM_GetRoomNo(this))) {
                return cPhs_ERROR_e;
            }
        }

        scale.x = 1.0f;

        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x3280)) {
            OS_REPORT("//////////////E_SM SET NON !!\n");
            return cPhs_ERROR_e;
        }

        if (hio_set == 0) {
            hio_set = 1;
            field_0x115c = 1;
            l_HIO.field_0x4 = -1;
        }

        field_0x9c8.set(1.0f, 1.0f, 1.0f);
        mAction = ACTION_NORMAL;
        mMode = 0;
        mCoreAction = CORE_ACTION_NORMAL;
        field_0x6b2 = 0;
        field_0x6b4 = 0;
        field_0x6b6 = 0;
        field_0x990 = current.pos;
        field_0x99c = field_0x990;

        if (fopAcM_GetRoomNo(this) == 10) {
            mSwBit2 = fopAcM_GetParam(this) >> 16;
            if (mSwBit2 == 0xFF || !dComIfGs_isSwitch(mSwBit2, fopAcM_GetRoomNo(this))) {
                field_0xa08 = true;
                mAction = ACTION_UNK;
                mMode = 0;
                mCoreAction = CORE_ACTION_DEMO;
                mCDemoMode = 0;

                cXyz sp40(0.0f, 0.0f, 0.0f);
                cXyz sp4c(700.0f, XREG_F(0) + 50.0f, -400.0f);
                cLib_offsetPos(&sp40, &current.pos, current.angle.y, &sp4c);
                current.pos = sp40;
                SetAnm(BCK_SC_WAIT02, J3DFrameCtrl::EMode_NONE, 0.0f, 5.0f);
                shape_angle.y = fopAcM_searchPlayerAngleY(this);
                current.angle.y = shape_angle.y;
            }
        }

        if ((u8)fopAcM_GetParam(this) != 0xFF) {
            field_0x970 = (fopAcM_GetParam(this) & 0xFF) * 100.0f;
        } else {
            field_0x970 = 1000.0f;
        }

        initCoSph();
        field_0x6be = 0;
        field_0xa10 = false;
        fopAcM_SetMtx(this, mpBubbleModel->getBaseTRMtx());
        fopAcM_SetMtx(this, mpModelMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -800.0f, -200.0f, -800.0f);
        fopAcM_SetMax(this, 800.0f, 500.0f, 800.0f);
        attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        attention_info.distances[fopAc_attn_BATTLE_e] = 58;

        mSound.init(&current.pos, NULL, 3, 1);
        mSoundSimple.init(&field_0x990, 1);
        field_0x682 = false;
        mAtInfo.mpSound = &mSound;
        mSound.setEnemyName("E_SM");

        field_0xc38.SetWall(yREG_F(0) + 40.0f, yREG_F(1) + 120.0f + (TREG_F(11) + 30.0f) * field_0x6f0);
        field_0xc78.Set(&field_0x990, &field_0x99c, this, 1, &field_0xc38, &field_0x9b8, NULL, NULL);
        field_0xa20.SetWall(100.0f, 50.0f);

        field_0xa60.Set(&current.pos, &old.pos, this, 1, &field_0xa20, &speed, NULL, NULL);
        field_0xa60.OnLineCheck();
        field_0xa60.SetGndThinCellingOff();
        field_0xc78.SetGndThinCellingOff();

        field_0x6bc = cM_rndF(65535.0f);
        health = 80;
        field_0x984 = 4;
        field_0x6e0 = 1.0f;
        field_0x694 = 1.0f;

        mSound.init(&current.pos, &eyePos, 3, 1);

        field_0x690 = 0.0f;
        mCoCore.OnTgSetBit();
        field_0x6b8 = 0.5f;

        for (int i = 0; i < 30; i++) {
            mArrowProcIDs[i] = fpcM_ERROR_PROCESS_ID_e;
        }

        daE_SM_Execute(this);
    }

    return phase;
}

daE_SM_c::daE_SM_c() {}

AUDIO_INSTANCES;

static actor_method_class l_daE_SM_Method = {
    (process_method_func)daE_SM_Create,
    (process_method_func)daE_SM_Delete,
    (process_method_func)daE_SM_Execute,
    (process_method_func)daE_SM_IsDelete,
    (process_method_func)daE_SM_Draw,
};

actor_process_profile_definition g_profile_E_SM = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_SM,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daE_SM_c),       // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  127,                    // mPriority
  &l_daE_SM_Method,       // sub_method
  0x000C0100,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
