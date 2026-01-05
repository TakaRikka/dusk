/**
 * @file d_a_e_zs.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_zs.h"
#include "f_op/f_op_actor_enemy.h"
#include "d/actor/d_a_b_ds.h"

class daE_ZS_HIO_c {
public:
    daE_ZS_HIO_c();
    virtual ~daE_ZS_HIO_c() {}

    /* 0x4 */ s8 field_0x4;
    /* 0x8 */ f32 field_0x8;
    /* 0xc */ f32 field_0xc;
};

namespace {
static dCcD_SrcCyl cc_zs_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x43}, 0x75}},  // mObj
        {dCcD_SE_13, 0x0, 0x0, 0x0, 0x0},                    // mGObjAt
        {dCcD_SE_NONE, 0x2, 0x0, 0x0, 0x0206},               // mGObjTg
        {0x0},                                               // mGObjCo
    },                                                       // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f},  // mCenter
            30.0f,               // mRadius
            130.0f               // mHeight
        }  // mCyl
    }
};
}

daE_ZS_HIO_c::daE_ZS_HIO_c() {
    field_0x4 = -1;
    field_0x8 = 1.4;
    field_0xc = 1200.0f;
}

int daE_ZS_c::draw() {
    if (field_0x673 == 0) {
        return 1;
    }

    J3DModel * model = mpMorf->getModel();
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);
    mpMorf->entryDL();
    cXyz adj_pos;
    adj_pos.set(current.pos.x, current.pos.y + 10.0f, current.pos.z);
    mShadowKey = dComIfGd_setShadow(mShadowKey, 1, model, &adj_pos, 700.0f, 0.0f,
                                    current.pos.y, mAcch.GetGroundH(), mAcch.m_gnd, &tevStr, 0,
                                    1.0f, dDlst_shadowControl_c::getSimpleTex());
    return 1;
}

static int daE_ZS_Draw(daE_ZS_c* i_this) {
    return i_this->draw();
}

void daE_ZS_c::setBck(int i_res_index, u8 i_attr, f32 i_morf, f32 i_rate) {
    J3DAnmTransform* anmTransform = (J3DAnmTransform*)dComIfG_getObjectRes("E_ZS", i_res_index);
    mpMorf->setAnm(anmTransform, i_attr, i_morf, i_rate, 0.0f, -1.0f);
    mResIndex = i_res_index;
}

void daE_ZS_c::setActionMode(int i_action, int i_mode) {
    mAction = i_action;
    mMode = i_mode;
}

static u8 lbl_259_bss_8;

static daE_ZS_HIO_c l_HIO;

void daE_ZS_c::damage_check() {
    static u16 eff_Damage_id[] = {0x8BE9, 0x8BEA, 0x8BEB};

    if (health > 1 && field_0x670 == 0 && field_0x673 && mAction != 3 && mCyl.ChkTgSet()) {
        cXyz eff_pos;
        mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(1));
        mDoMtx_stack_c::multVecZero(&eff_pos);
        scale.set(l_HIO.field_0x8, l_HIO.field_0x8, l_HIO.field_0x8);
        setMidnaBindEffect(this, &mSound, &eff_pos, &scale);
        mStts.Move();
        if (mCyl.ChkTgHit()) {
            cXyz my_vec_1;
            mpCollider = mCyl.GetTgHitObj();
            my_vec_1 = *mCyl.GetTgHitPosP();
            if (mCyl.GetTgHitObj()->ChkAtType(AT_TYPE_SPINNER)) {
                mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(1));
                mDoMtx_stack_c::multVecZero(&eff_pos);
                for (int idx = 0; idx < 3; ++idx) {
                    dComIfGp_particle_set(eff_Damage_id[idx], &eff_pos, NULL, &shape_angle,
                                          NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
                }
            }

            if ((mCyl.GetTgHitObj()->ChkAtType(AT_TYPE_SPINNER) && daPy_getPlayerActorClass()->checkSpinnerTriggerAttack())
                || mCyl.GetTgHitObj()->ChkAtType(AT_TYPE_NORMAL_SWORD)
                || mCyl.GetTgHitObj()->ChkAtType(AT_TYPE_CSTATUE_BOSS_SWING)
                || mCyl.GetTgHitObj()->ChkAtType(AT_TYPE_MIDNA_LOCK)
                || mCyl.GetTgHitObj()->ChkAtType(AT_TYPE_IRON_BALL)
                || mCyl.GetTgHitObj()->ChkAtType(AT_TYPE_BOMB)) {
                health = 0;
                field_0x560 = 0;
                def_se_set(&mSound, mpCollider, 0x1F, NULL);
                dComIfGp_setHitMark(3, this, &my_vec_1, &shape_angle, NULL, 0);
                if (mCyl.GetTgHitObj()->ChkAtType(AT_TYPE_CSTATUE_BOSS_SWING) == 0
                   && mCyl.GetTgHitObj()->ChkAtType(AT_TYPE_BOMB) == 0) {
                    dScnPly_c::setPauseTimer(3);
                }

                mSound.startCollisionSE(0x40000, 0x20);
                mCyl.OffTgSetBit();
                mCyl.OffCoSetBit();
                setActionMode(2, 2);
                mCyl.ClrTgHit();
            } else {
                def_se_set(&mSound, mpCollider, 0x28, 0);
                dComIfGp_setHitMark(2, this, &my_vec_1, &shape_angle, NULL, 0);
                setActionMode(2, 0);
                field_0x670 = 8;
                mCyl.ClrTgHit();
            }

        }

    }
}

static void* s_BossSearch(void* i_lhs, void* i_rhs) {
    if (fopAcM_IsActor(i_lhs) && fopAcM_GetName(i_lhs) == 0xf6) {
        return i_lhs;
    }
    return NULL;
}

u8 daE_ZS_c::mBossHandCheck() {
    daB_DS_c* stallord_boss = (daB_DS_c*) fpcM_Search(s_BossSearch, this);
    if (stallord_boss == NULL) {
        return 0;
    }

    cXyz my_vec_0;
    my_vec_0 = stallord_boss->getHandPosL() - current.pos;
    my_vec_0.y = 0.0f;
    if (my_vec_0.abs() < 400.0f) {
        return 1;
    }

    my_vec_0 = stallord_boss->getHandPosR() - current.pos;
    my_vec_0.y = 0.0f;
    if (my_vec_0.abs() < 400.0f) {
        return 1;
    }

    return 0;
}

static u8 lbl_259_bss_28;
static u8 lbl_259_bss_29;

void daE_ZS_c::executeAppear() {
    static u16 eff_Appear_id[] = {0x8BE7, 0x8BE8};

    switch (mMode) {
        case 0: {
            daB_DS_c* stallord_boss = (daB_DS_c*) fpcM_Search(s_BossSearch, this);
            if (stallord_boss == NULL) {
                fopAcM_delete(this);
            } else {
                if (lbl_259_bss_28 == 0) {
                    if (stallord_boss->isOpening()) {
                        break;
                    }

                    lbl_259_bss_28 = 1;
                }

                if (mBossHandCheck() == 0) {
                    mMode = 1;
                }
            }

            break;
        }
        case 1:
            if (field_0x672 || fopAcM_searchPlayerDistance(this) <= l_HIO.field_0xc) {
                if (field_0x672 == 2) {
                    if (mResIndex != 9) {
                        field_0x65c = -240.0f;
                        setBck(9, 2, 3.0f, 1.0f);
                    }
                } else if (mResIndex != 4) {
                    setBck(4, 0, 3.0f, 1.0f);
                }

                mSound.startCreatureSound(Z2SE_EN_ZS_APPEAR, 0, -1);
                for (int idx = 0; idx < 2; ++idx) {
                    dComIfGp_particle_set(eff_Appear_id[idx], &current.pos, NULL, &shape_angle,
                                          NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
                }
                field_0x673 = 1;
                mMode = 2;
                mCyl.OnTgSetBit();
                mCyl.OnCoSetBit();
            }
            break;
        case 2:
            if (field_0x672 == 2) {
                cLib_addCalcAngleS2(&current.angle.y, fopAcM_searchPlayerAngleY(this), 8, 0x1000);
                shape_angle.y = current.angle.y;
                cLib_addCalc0(&field_0x65c, 0.7f, 10.0f);
                if (field_0x65c > -2.0f) {
                    field_0x65c = 0.0f;
                    offWolfNoLock();
                    fopAcM_OnStatus(this, 0x200000);
                    setActionMode(1, 0);
                }
            } else {
                if (mpMorf->isStop()) {
                    offWolfNoLock();
                    fopAcM_OnStatus(this, 0x200000);
                    setActionMode(1, 0);
                }
            }

            break;
        case 10: {
            daB_DS_c* stallord_boss = (daB_DS_c*) fpcM_Search(s_BossSearch, this);
            if (stallord_boss == NULL) {
                fopAcM_delete(this);
            } else if (!stallord_boss->isDemo() && ((field_0x672 == 1 && stallord_boss->isAppear())
                                                || (field_0x672 == 2 && stallord_boss->isAppearG()))) {
                if (field_0x672 == 2) {
                    field_0x65c = -240.0f;
                    setBck(9, 2, 3.0f, 1.0f);
                } else {
                    setBck(4, 0, 3.0f, 1.0f);
                }

                field_0x673 = 1;
                current.pos.set(home.pos);
                mMode = 1;
            }

            break;
        }
        default:
            break;
    }
}

void daE_ZS_c::executeWait() {
    switch (mMode) {
        case 0:
            if (field_0x672 == 2) {
                setBck(9, 2, 3.0f, 1.0f);
            } else if (cM_rnd() < 0.5f) {
                setBck(7, 2, 5.0f, 1.0f);
            } else {
                setBck(8, 2, 5.0f, 1.0f);
            }

            mMode = 1;
        case 1: {
            if ((int)mpMorf->getFrame() == 0) {
                mSound.startCreatureVoice(Z2SE_EN_ZS_V_WAIT, -1);
            }
            daB_DS_c* stallord_boss = (daB_DS_c*) fpcM_Search(s_BossSearch, this);
            if (stallord_boss == NULL) {
                fopAcM_delete(this);
                return;
            }
            if (   (field_0x672 != 0 || mBossHandCheck())
                && (field_0x672 != 1 || !stallord_boss->isAppear())
                && (field_0x672 != 2 || !stallord_boss->isAppearG())) {
                setActionMode(3, 10);
            }

            break;
        }
        default:
            break;
    }

    if (field_0x672 || fopAcM_searchPlayerDistance(this) <= l_HIO.field_0xc) {
        cLib_addCalcAngleS2(&current.angle.y, fopAcM_searchPlayerAngleY(this), 8, 0x1000);
        shape_angle.y = current.angle.y;
    }
}

void daE_ZS_c::executeDamage() {
    static u16 w_eff_id[] = {0x854C, 0x854D, 0x854E, 0x854F};

    switch (mMode) {
        case 0:
            setBck(5, 0, 3.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_ZS_V_DMG, -1);
            mMode = 1;
            break;
        case 1:
            if (mpMorf->isStop()) {
                setActionMode(1, 0);
            }

            break;
        case 2:
            setBck(6, 0, 3.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_ZS_V_DEAD, -1);
            mMode = 3;
            break;
        case 3:
            if (mpMorf->isStop()) {
                cXyz my_vec_0(5.0f, 5.0f, 5.0f);
                for (int idx = 0; idx < 4; ++idx) {
                    dComIfGp_particle_set(w_eff_id[idx], &current.pos, &tevStr, NULL,
                                          NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
                }
                fopAcM_delete(this);
            }

            break;
        default:
            break;
    }
}

void daE_ZS_c::executeDrive() {
    switch (mMode) {
        case 0:
        case 10:
            if (mMode == 0) {
                setBck(4, 0, 3.0f, -1.0f);
            } else {
                setBck(4, 0, 0.0f, -2.0f);
            }

            if (daPy_getPlayerActorClass()->checkWolfLock(this)) {
                daPy_getPlayerActorClass()->cancelWolfLock(this);
            }

            onWolfNoLock();
            fopAcM_OffStatus(this, 0x200000);
            mSound.startCreatureSound(Z2SE_EN_ZS_DISAPPEAR, 0, -1);
            field_0x673 = 1;
            mCyl.OffTgSetBit();
            mCyl.OffCoSetBit();
            ++mMode;
            break;
        case 1:
        case 11:
            if ((int)mpMorf->getFrame() < 1) {
                if (lbl_259_bss_29 || mMode == 1) {
                    fopAcM_delete(this);
                } else {
                    field_0x673 = 0;
                    setActionMode(0, 10);
                    offWolfNoLock();
                }
            }
    }
}

void daE_ZS_c::action() {
    damage_check();
    switch (mAction) {
        case ACT_APPEAR:
            executeAppear();
            break;
        case ACT_WAIT:
            executeWait();
            break;
        case ACT_DAMAGE:
            executeDamage();
            break;
        case ACT_DRIVE:
            executeDrive();
            break;
        default:
            break;
    }

    if (field_0x671 == 0 && mAction != ACT_DRIVE) {
        daB_DS_c* stallord_boss = (daB_DS_c*) fpcM_Search(s_BossSearch, this);
        if (stallord_boss == NULL || (field_0x672 && stallord_boss->isDemo())
            || stallord_boss->isDead() || stallord_boss->isArg0() || lbl_259_bss_29) {
            if (field_0x672 == 0) {
                lbl_259_bss_29 = 1;
            }

            if (mAction == ACT_DRIVE || (mAction == ACT_APPEAR && mMode == 10)) {
                fopAcM_delete(this);
            } else if (field_0x672) {
                setActionMode(ACT_DRIVE, 10);
            } else {
                setActionMode(ACT_DRIVE, 0);
            }
        }
    }

    fopAcM_posMoveF(this, mStts.GetCCMoveP());
    mAcch.CrrPos(dComIfG_Bgsp());
    if (field_0x673) {
        mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    }
}

void daE_ZS_c::mtx_set() {
    if (field_0x673) {
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
        mDoMtx_stack_c::transM(0.0f, field_0x65c, 0.0f);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        mDoMtx_stack_c::scaleM(l_HIO.field_0x8, l_HIO.field_0x8, l_HIO.field_0x8);
        mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
        mpMorf->modelCalc();
    }
}

void daE_ZS_c::cc_set() {
    if (field_0x673) {
        cXyz my_vec_0;
        cXyz my_vec_1;
        J3DModel* model = mpMorf->getModel();
        mDoMtx_stack_c::copy(model->getAnmMtx(4));
        my_vec_0.set(0.0f, 25.0f, 0.0f);
        mDoMtx_stack_c::multVec(&my_vec_0, &my_vec_1);
        eyePos.set(my_vec_1);
        attention_info.position = eyePos;
        attention_info.position.y += 25.0f;
        mDoMtx_stack_c::copy(model->getAnmMtx(0));
        mDoMtx_stack_c::multVecZero(&my_vec_1);
        mCyl.SetC(my_vec_1);
        mCyl.SetH(280.0f);
        mCyl.SetR(50.0f);
        dComIfG_Ccsp()->Set(&mCyl);
    }
}

int daE_ZS_c::execute() {
    if (field_0x670 != 0) {
        --field_0x670;
    }

    if (field_0x671 != 0) {
        --field_0x671;
    }

    action();
    mtx_set();
    cc_set();
    return 1;
}

static int daE_ZS_Execute(daE_ZS_c* i_this) {
    return i_this->execute();
}

static int daE_ZS_IsDelete(daE_ZS_c* i_this) {
    return true;
}

int daE_ZS_c::_delete() {
    dComIfG_resDelete(&mPhase, "E_ZS");
    if (field_0xa28) {
        lbl_259_bss_8 = 0;
    }

    if (heap) {
        mSound.deleteObject();
    }

    return 1;
}

static int daE_ZS_Delete(daE_ZS_c* i_this) {
    return i_this->_delete();
}

int daE_ZS_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*) dComIfG_getObjectRes("E_ZS", 0xC);
    JUT_ASSERT(0x3e9, modelData != NULL);
    mpMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_ZS", 4),
                                  1, 1.0f, 0, -1, &mSound, 0,0x11000084);
    if (mpMorf == NULL) {
        return 0;
    }
    return 1;
}

static void useHeapInit(fopAc_ac_c* i_this) {
    static_cast<daE_ZS_c*>(i_this)->CreateHeap();
}

int daE_ZS_c::create() {
    fopAcM_ct(this, daE_ZS_c);
    int phase = dComIfG_resLoad(&mPhase, "E_ZS");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("E_ZS PARAM %x\n", fopAcM_GetParam(this));
        if (!fopAcM_entrySolidHeap(this, (heapCallbackFunc)useHeapInit, 0xFC0)) {
            return cPhs_ERROR_e;
        }

        if (lbl_259_bss_8 == 0) {
            lbl_259_bss_8 = 1;
            field_0xa28 = 1;
            l_HIO.field_0x4 = -1;
        }

        field_0x672 = fopAcM_GetParam(this);
        if (field_0x672 == 0xFF) {
            field_0x672 = 0;
        }

        OS_REPORT("arg0 %d\n", field_0x672);
        attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(this, 200.0f, 200.0f, 200.0f);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), NULL, NULL);
        mAcchCir.SetWall(10.0f, 60.0f);
        mStts.Init(0xFF, 0, this);
        health = 0x14;
        field_0x560 = 0x14;
        field_0x671 = 5;
        mCyl.Set(cc_zs_src);
        mCyl.SetStts(&mStts);
        mSound.init(&current.pos, &eyePos, 3, 1);
        mpSound = &mSound;
        field_0xa22 = 10;
        gravity = -1.0f;
        mpMorf->setStartFrame(0.0f);
        onWolfNoLock();
        if (field_0x672 == 0) {
            setActionMode(0, 0);
        } else {
            field_0x673 = 0;
            setActionMode(0, 10);
        }

        shape_angle.z = 0;
        shape_angle.x = 0;
        attention_info.distances[2] = 0;
        daE_ZS_Execute(this);
        mCyl.OffTgSetBit();
        mCyl.OffCoSetBit();
    }

    return phase;
}

static int daE_ZS_Create(daE_ZS_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daE_ZS_Method = {
    (process_method_func)daE_ZS_Create,
    (process_method_func)daE_ZS_Delete,
    (process_method_func)daE_ZS_Execute,
    (process_method_func)daE_ZS_IsDelete,
    (process_method_func)daE_ZS_Draw,
};

actor_process_profile_definition g_profile_E_ZS = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_ZS,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daE_ZS_c),       // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  170,                    // mPriority
  &l_daE_ZS_Method,       // sub_method
  0x00040100,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
