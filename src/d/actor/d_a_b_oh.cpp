/**
 * d_a_b_oh.cpp
 * Morpheel Tentacle
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_b_ob.h"
#include "d/actor/d_a_b_oh.h"
#include "d/actor/d_a_player.h"
#include "c/c_damagereaction.h"
#include "Z2AudioLib/Z2Instances.h"

enum B_oh_RES_File_ID {
    /* BCK */
    /* 0x08 */ BCK_AL_FOIFINISH = 0x8,
    /* 0x09 */ BCK_AL_OICUT,
    /* 0x0A */ BCK_AL_OICUTB,
    /* 0x0B */ BCK_AL_OIFINISH,
    /* 0x0C */ BCK_AL_OIHANG,
    /* 0x0D */ BCK_AL_OIHANGMISS,
    /* 0x0E */ BCK_AL_OIHANGWAIT,
    /* 0x0F */ BCK_AL_OIHANGWAITB,
    /* 0x10 */ BCK_AL_OISPIT,
    /* 0x11 */ BCK_OH_CORE_OPEN,
    /* 0x12 */ BCK_OI_APPEAR,
    /* 0x13 */ BCK_OI_APPEAR02,
    /* 0x14 */ BCK_OI_DEAD,
    /* 0x15 */ BCK_OI_EAT,
    /* 0x16 */ BCK_OI_EAT02,
    /* 0x17 */ BCK_OI_LASTDAMAGE,
    /* 0x18 */ BCK_OI_OPENMOUTH,
    /* 0x19 */ BCK_OI_OPENMOUTHWAIT,
    /* 0x1A */ BCK_OI_SWALLOW,
    /* 0x1B */ BCK_OI_SWIMWAIT,
    /* 0x1C */ BCK_OI_TENTACLE_END,
    /* 0x1D */ BCK_OI_THROWUP,
    /* 0x1E */ BCK_OI_WAIT,

    /* BMDR */
    /* 0x21 */ BMDR_EF_OISUI = 0x21,
    /* 0x22 */ BMDR_OI_BODY,
    /* 0x23 */ BMDR_OI_FINA,
    /* 0x24 */ BMDR_OI_FINB,
    /* 0x25 */ BMDR_OI_FINC,
    /* 0x26 */ BMDR_OI_TAIL,

    /* BMDV */
    /* 0x29 */ BMDV_OH = 0x29,
    /* 0x2A */ BMDV_OH_CORE,
    /* 0x2B */ BMDV_OI_HEAD,

    /* BRK */
    /* 0x2E */ BRK_EF_OISUI = 0x2E,
    /* 0x2F */ BRK_OH_LOOP,

    /* BTK */
    /* 0x32 */ BTK_AL_FOICUT = 0x32,
    /* 0x33 */ BTK_AL_FOICUTB,
    /* 0x34 */ BTK_AL_FOIFINISH,
    /* 0x35 */ BTK_EF_OISUI,
    /* 0x36 */ BTK_OH_LOOP,

    /* BTP */
    /* 0x39 */ BTP_AL_FOIFINISH = 0x39,
};

class daB_OH_HIO_c {
public:
    daB_OH_HIO_c();
    virtual ~daB_OH_HIO_c() {}

    /* 0x4 */ s8 field_0x4;
    /* 0x8 */ f32 mModelSize;
    /* 0xC */ f32 mLength;
};

daB_OH_HIO_c::daB_OH_HIO_c() {
    field_0x4 = -1;
    mModelSize = 1.0f;
    mLength = 70.0f;
}

static int nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        int jnt_no = i_joint->getJntNo();
        J3DModel* model_p = j3dSys.getModel();
        b_oh_class* this_ = (b_oh_class*)model_p->getUserArea();

        if (this_ != NULL && jnt_no >= this_->field_0xca8 && jnt_no <= 29) {
            MTXCopy(model_p->getAnmMtx(jnt_no), *calc_mtx);
            mDoMtx_YrotM(*calc_mtx, this_->field_0x61c[jnt_no].y + this_->field_0x784[jnt_no].y);
            mDoMtx_ZrotM(*calc_mtx, this_->field_0x61c[jnt_no].x + this_->field_0x784[jnt_no].x);
            MtxTrans(this_->mTentacleLength + -100.0f, 1.0f, 1.0f, 1);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            MtxScale(1.0f, this_->field_0x8ec[jnt_no] + this_->field_0x9dc[jnt_no],
                     this_->field_0x8ec[jnt_no] + this_->field_0x9dc[jnt_no], 1);
            model_p->setAnmMtx(jnt_no, *calc_mtx);
        }
    }

    return 1;
}

static int daB_OH_Draw(b_oh_class* i_this) {
    if (i_this->mAction == OH_ACTION_NON) {
        return 1;
    }

    J3DModel* model_p = i_this->mpMorf->getModel();

    g_env_light.settingTevStruct(0, &i_this->current.pos, &i_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(model_p, &i_this->tevStr);

    i_this->mpBtk->entry(model_p->getModelData());
    i_this->mpBrk->entry(model_p->getModelData());
    i_this->mInvisModel.entryDL(NULL);
    return 1;
}

static u8 data_8061DBB5;

static daB_OH_HIO_c l_HIO;

static b_ob_class* boss;

static int Cinit;

static void start(b_oh_class* i_this) {
    cXyz sp28;

    switch (i_this->mActionPhase) {
    case 0:
        if (boss->mCoreHandNo != i_this->field_0x5c8) {
            i_this->field_0xcac = -1500.0f;
        } else {
            i_this->field_0x608 = 0.3f;
            i_this->mActionPhase = 1;
        }
        break;
    case 1:
        if (i_this->field_0xcac < -100.0f) {
            for (int i = 0; i < 28; i++) {
                MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(i), mDoMtx_stack_c::get());
                mDoMtx_stack_c::multVecZero(&sp28);

                if (sp28.y > boss->field_0x47a0) {
                    sp28.y = boss->field_0x47a0;
                    i_this->field_0x1f80 =
                        dComIfGp_particle_set(i_this->field_0x1f80, 0x8808, &sp28, NULL, NULL);
                    break;
                }
            }
        }

        cLib_addCalc0(&i_this->field_0xcac, 0.1f, 30.0f);
        cLib_addCalc2(&i_this->field_0x608, 0.3f, 0.1f, 0.005f);
        break;
    case 2:
        cLib_addCalc2(&i_this->field_0x608, 1.0f, 0.1f, 0.05f);
        break;
    }
}

static void wait(b_oh_class* i_this) {
    daPy_py_c* player_p = (daPy_py_c*)dComIfGp_getPlayer(0);

    switch (i_this->mActionPhase) {
    case 0:
        i_this->mActionPhase = 1;
        i_this->mTimers[0] = (s16)(cM_rndF(30.0f) + 60.0f);
        break;
    case 1:
        cLib_addCalc2(&i_this->field_0x608, 1.0f, 0.1f, 0.005f);

        if (i_this->field_0xca8 == 0 && player_p->current.pos.y < -23000.0f &&
            i_this->mDistToPlayer < 1300.0f && i_this->mTimers[0] == 0 && boss->field_0x4744 == 0 &&
            boss->field_0x4794 == 0)
        {
            i_this->mAction = OH_ACTION_ATTACK;
            i_this->mActionPhase = 0;
            i_this->field_0xc98 = 0;
            boss->field_0x4794 = 3;
        }
        break;
    }
}

static void attack(b_oh_class* i_this) {
    daPy_py_c* player_p = (daPy_py_c*)dComIfGp_getPlayer(0);
    i_this->field_0xca0++;

    switch (i_this->mActionPhase) {
    case 0:
        i_this->mActionPhase = 1;
        i_this->field_0xc9c = 0.0f;
        i_this->field_0xca2 = 0;
        i_this->field_0xca4 = 0;

        if ((s16)(i_this->mAngleToPlayer - i_this->current.angle.y) > 0) {
            i_this->field_0xc88 = 1;
        } else {
            i_this->field_0xc88 = -1;
        }

        i_this->mActionPhase = 2;
        i_this->mTimers[0] = 100;
        i_this->field_0xca0 = 0;
        i_this->field_0xc98 = 0;
    case 1:
    case 2:
        if (i_this->mTimers[0] == 0) {
            i_this->mAction = OH_ACTION_WAIT;
            i_this->mActionPhase = 0;
            i_this->field_0xc98 = 0;
            i_this->field_0xca2 = 0;
            return;
        }
    case 3:
        cLib_addCalcAngleS2(&i_this->field_0xc98, 0x200, 1, 32);

        if (i_this->field_0xca0 > 8) {
            cLib_addCalc2(&i_this->field_0xc90, -0.12f, 0.8f, 0.02f);
        }

        if (i_this->field_0xca0 < 70) {
            cLib_addCalcAngleS2(&i_this->field_0xca4, 0, 1, 500);
            cLib_addCalcAngleS2(&i_this->current.angle.y,
                                i_this->mAngleToPlayer + i_this->field_0xc88 * -0x600, 2, 0x400);
            cLib_addCalc2(&i_this->field_0xc9c, 200.0f, 0.5f, 50.0f);

            if (i_this->field_0xca0 < 30) {
                f32 fvar = player_p->current.pos.y - -24000.0f;
                if (fvar > 1000.0f) {
                    fvar = 1000.0f;
                }

                i_this->field_0xc96 = fvar + 3700.0f;

                cLib_addCalcAngleS2(&i_this->current.angle.x,
                                    (s16)(-fvar * 2.0f + 11700.0f) - 0x4000, 1, 1000);
            }
        } else {
            s16 var7;
            s16 var3;

            if (i_this->mTimers[1] != 0) {
                var7 = 6;
                var3 = i_this->field_0xc88 * 9000;
            } else {
                var7 = 10;
                var3 = i_this->field_0xc88 * 5000;
            }

            cLib_addCalcAngleS2(&i_this->field_0xca2, var7, 1, 1);
            cLib_addCalcAngleS2(&i_this->field_0xca4, var3, 1, 500);

            if (i_this->mTimers[1] == 0 || i_this->mTimers[1] > 10) {
                i_this->current.angle.y += (s16)(i_this->field_0xc88 * 300);
            }

            if (i_this->mActionPhase == 2) {
                for (int i = 7; i < 15; i++) {
                    if (i_this->mColliders[i].ChkCoHit()) {
                        cCcD_Obj* obj_p = i_this->mColliders[i].GetCoHitObj();
                        if (fopAcM_GetName(obj_p->GetAc()) == PROC_ALINK &&
                            !dComIfGp_event_runCheck())
                        {
                            if (!player_p->checkHookshotShootReturnMode() &&
                                boss->field_0x4744 == 0)
                            {
                                i_this->mActionPhase = 3;
                                boss->mDemoAction = 1;
                                boss->field_0x5ce8 = i_this->field_0x5c8;
                                i_this->mTimers[1] = 20;

                                if (boss->mAction != OB_ACTION_CORE_HAND_MOVE) {
                                    boss->mAction = OB_ACTION_CORE_HAND_MOVE;
                                    boss->mMode = 0;
                                    boss->mCoreAnm = BCK_OI_WAIT;
                                    boss->mCoreAnmMode = J3DFrameCtrl::EMode_LOOP;
                                }

                                boss->mOISound.startTentacleSound(
                                    Z2SE_EN_OI_TENT_SWING, i_this->field_0x5c8, 0,
                                    dComIfGp_getReverb(fopAcM_GetRoomNo(i_this)));
                                break;
                            }
                        }
                    }
                }
            }
        }

        cLib_addCalcAngleS2(&i_this->field_0xc94, 22500, 1, 2000);
        cLib_addCalc2(&i_this->field_0xc8c, 4000.0f, 1.0f, 400.0f);
        if (i_this->mTimers[1] == 1) {
            i_this->mAction = OH_ACTION_CAUGHT;
            i_this->mActionPhase = 0;
            i_this->field_0xc98 = 0;
        }
        break;
    }
}

static void caught(b_oh_class* i_this) {
    i_this->field_0x5f4 = 6;

    switch (i_this->mActionPhase) {
    case 0:
        i_this->mActionPhase = 1;
        i_this->mTimers[0] = 10;
        i_this->mTimers[1] = 50;
    case 1:
        i_this->field_0x60c = 500.0f;
        cLib_addCalcAngleS2(&i_this->field_0xca2, 6, 1, 1);
        cLib_addCalcAngleS2(&i_this->field_0xca4, i_this->field_0xc88 * 9000, 1, 500);

        if (i_this->mTimers[0] == 1) {
            boss->mCoreAnm = BCK_OI_EAT;
            boss->mCoreAnmMode = J3DFrameCtrl::EMode_NONE;
            boss->mOISound.startCreatureVoice(Z2SE_EN_OI_V_EAT, -1);
        }

        if (!(i_this->field_0x5cc & 0xF)) {
            dComIfGp_getVibration().StartShock(3, 31, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (i_this->mTimers[1] == 0) {
            if (boss->mDemoAction != 0) {
                boss->mDemoAction = 3;
                boss->mDemoActionTimer = 0;
            }

            i_this->mAction = OH_ACTION_WAIT;
            i_this->mActionPhase = 0;
            i_this->field_0xc98 = 0;
        }
        break;
    }
}

static void end(b_oh_class* i_this) {
    i_this->field_0x5f4 = 6;

    switch (i_this->mActionPhase) {
    case 0:
        i_this->mActionPhase = 1;
        i_this->mTimers[0] = 70;
        break;
    case 1:
        cLib_addCalc2(&i_this->field_0x60c, 1000.0f, 1.0f, 150.0f);

        if (i_this->mTimers[0] == 0) {
            cLib_addCalc0(&i_this->mTentacleLength, 1.0f, 1.0f);
        }
        break;
    }
}

static void non(b_oh_class* i_this) {
    i_this->field_0x5f4 = 6;

    switch (i_this->mActionPhase) {
    case 0:
        i_this->mActionPhase = 1;
        break;
    case 1:
        break;
    }
}

static void action(b_oh_class* i_this) {
    b_oh_class* a_this = (b_oh_class*)i_this;
    int var_r28;
    BOOL var_r27;

    cXyz sp90;
    cXyz sp9C;

    a_this->mAngleToPlayer = fopAcM_searchPlayerAngleY(a_this);
    a_this->mDistToPlayer = fopAcM_searchPlayerDistance(a_this);

    fopAcM_OffStatus(a_this, 0);
    a_this->attention_info.flags = 0;

    var_r28 = 0;
    var_r27 = true;

    cLib_addCalcAngleS2(&a_this->field_0xc98, 0x800, 1, 0x10);

    switch (a_this->mAction) {
    case OH_ACTION_START:
        start(a_this);
        var_r28 = 1;
        break;
    case OH_ACTION_WAIT:
        wait(a_this);
        var_r28 = 1;
        break;
    case OH_ACTION_ATTACK:
        attack(a_this);
        var_r28 = 2;
        boss->field_0x4794 = 180;
        break;
    case OH_ACTION_CAUGHT:
        caught(a_this);
        var_r27 = false;
        var_r28 = 3;
        boss->field_0x4794 = 180;
        break;
    case OH_ACTION_END:
        end(a_this);
        var_r28 = 1;
        break;
    case OH_ACTION_NON:
        non(a_this);
        return;
    }

    if (boss->mAction == OB_ACTION_BOMBFISH_SET) {
        cLib_addCalc2(&a_this->field_0x60c, 500.0f, 1.0f, 80.0f);
    }

    if (var_r28 == 1) {
        f32 var_f29 = a_this->field_0x608 * 500.0f;
        f32 var_f28 = a_this->field_0x608 * 1500.0f;
        s16 var_r5 = 2000;

        for (int i = i_this->field_0xca8; i < 30; i++) {
            f32 var_f31 = 1.0f;
            if (i < 5) {
                var_f31 = i * 0.2f;
            } else if (i >= 20) {
                var_f31 = (i - 20) * 0.3f + 1.0f;
            }

            a_this->field_0x6d0[i].x =
                var_f31 * (var_f29 * cM_ssin(i_this->field_0x5f8 + (i * 1800)));
            a_this->field_0x6d0[i].y =
                var_f31 * (var_f28 * cM_ssin(i_this->field_0x5fa + (i * 1800)));

            a_this->field_0x838[i].x =
                var_r5 + var_f31 * (var_f29 * cM_ssin(a_this->field_0x5fa + (i * 7000)) * 0.5f);
            a_this->field_0x838[i].y =
                var_f31 * (var_f28 * cM_ssin(a_this->field_0x5f8 + (i * 7000)) * 0.5f);

            var_r5 -= 200;
            if (var_r5 < 0) {
                var_r5 = 0;
            }

            a_this->field_0x964[i] =
                a_this->field_0x610 + 1.0f +
                a_this->field_0x610 * cM_ssin(a_this->field_0x5f6 + i * -10000);
        }

        cLib_addCalcAngleS2(&a_this->current.angle.x, -0xF2C, 4, 100);
        cLib_addCalcAngleS2(&a_this->current.angle.y, a_this->home.angle.y, 4, 0x100);
    } else if (var_r28 == 2) {
        f32 var_f4 = a_this->field_0xc8c;
        for (int i = a_this->field_0xca8; i < 30; i++) {
            if (i >= 30 - a_this->field_0xca2) {
                a_this->field_0x6d0[i].y = a_this->field_0xca4;
            } else {
                a_this->field_0x6d0[i].y = 0;
            }

            if (i >= 13) {
                var_f4 *= a_this->field_0xc90 + 1.0f;
            }

            a_this->field_0x6d0[i].x =
                var_f4 * cM_ssin(a_this->field_0xc94 + i * a_this->field_0xc96);

            if (i >= 18) {
                a_this->field_0x838[i].x =
                    a_this->field_0xc9c * cM_ssin(a_this->field_0x5cc * 1000 + i * -4000);
            } else {
                a_this->field_0x838[i].x = 0;
            }

            a_this->field_0x838[i].y = 0;
            a_this->field_0x964[i] =
                a_this->field_0x610 + 1.0f +
                a_this->field_0x610 * cM_ssin(a_this->field_0x5f6 + i * -10000);
        }
    } else if (var_r28 == 3) {
        for (int i = a_this->field_0xca8; i < 30; i++) {
            a_this->field_0x838[i].y = 0;
            a_this->field_0x838[i].x = 0;

            if (i >= 30 - a_this->field_0xca2) {
                a_this->field_0x6d0[i].y = a_this->field_0xca4;
                a_this->field_0x6d0[i].x = 0;
                a_this->field_0x964[i] = 1.0f;
            } else {
                a_this->field_0x6d0[i].y = 0;
                a_this->field_0x6d0[i].x = 2250;
                a_this->field_0x964[i] =
                    a_this->field_0x610 + 1.0f +
                    a_this->field_0x610 * cM_ssin(a_this->field_0x5f6 + i * -10000);
            }
        }

        cLib_addCalcAngleS2(&a_this->current.angle.x, 0xA92, 4, 0x200);
        cLib_addCalcAngleS2(&a_this->current.angle.y, a_this->home.angle.y, 4, 0x800);
    }

    a_this->field_0x600 =
        (-a_this->field_0x614 - a_this->field_0x60c) + cM_ssin(a_this->field_0x5cc * 200) * 100.0f;
    a_this->field_0x604 = ((100.0f - a_this->field_0x614) - a_this->field_0x60c) +
                          cM_ssin(a_this->field_0x5cc * 200) * 100.0f;
    a_this->field_0x5f8 += (s16)a_this->field_0x600;
    a_this->field_0x5fa += (s16)a_this->field_0x604;
    a_this->field_0x5fc = a_this->field_0x60c + 2000.0f;
    a_this->field_0x5f6 += (s16)a_this->field_0x5fc;

    cLib_addCalc0(&a_this->field_0x60c, 0.1f, 50.0f);
    cLib_addCalc2(&a_this->field_0x610, 0.2f, 0.1f, 0.01f);

    if (var_r28 <= 3) {
        cLib_addCalc2(&a_this->mTentacleLength, l_HIO.mLength, 0.1f, 0.5f);
    }

    MTXCopy(boss->mBodyParts[0].mpMorf->getModel()->getAnmMtx(a_this->field_0x5c8 + 8),
            mDoMtx_stack_c::get());
    mDoMtx_stack_c::multVecZero(&a_this->current.pos);

    sp90.x = a_this->current.pos.x - boss->home.pos.x;
    sp90.z = a_this->current.pos.z - boss->home.pos.z;
    a_this->home.angle.y = cM_atan2s(sp90.x, sp90.z);
    cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 2, 0x2000);
    cLib_addCalcAngleS2(&a_this->shape_angle.x, a_this->current.angle.x, 2, 0x2000);

    for (int i = a_this->field_0xca8; i < 30; i++) {
        if (Cinit) {
            a_this->field_0x61c[i] = a_this->field_0x6d0[i];
            a_this->field_0x784[i] = a_this->field_0x838[i];
            a_this->field_0x8ec[i] = a_this->field_0x964[i];
        } else {
            cLib_addCalcAngleS2(&a_this->field_0x61c[i].x, a_this->field_0x6d0[i].x, 2,
                                a_this->field_0xc98);
            cLib_addCalcAngleS2(&a_this->field_0x61c[i].y, a_this->field_0x6d0[i].y, 2,
                                a_this->field_0xc98);
            cLib_addCalcAngleS2(&a_this->field_0x784[i].x, a_this->field_0x838[i].x, 2,
                                a_this->field_0xc98);
            cLib_addCalcAngleS2(&a_this->field_0x784[i].y, a_this->field_0x838[i].y, 2,
                                a_this->field_0xc98);
            cLib_addCalc2(&a_this->field_0x8ec[i], a_this->field_0x964[i], 0.5f, 0.2f);
            cLib_addCalc0(&a_this->field_0x9dc[i], 0.1f, 0.2f);
        }
    }

    for (int i = 0; i < 15; i++) {
        if (var_r27 && boss->mDemoAction == 0) {
            a_this->mColliders[i].OnCoSetBit();
        } else {
            a_this->mColliders[i].OffCoSetBit();
        }
    }

    Cinit = 0;
}

static void damage_check(b_oh_class* i_this) {
    i_this->mCcStts.Move();

    if (i_this->field_0x5f4 == 0) {
        BOOL bvar = false;
        for (int i = 7; i < 15; i++) {
            if (i_this->mColliders[i].ChkTgHit()) {
                i_this->mAtInfo.mpCollider = i_this->mColliders[i].GetTgHitObj();
                i_this->health = 1000;
                cc_at_check(i_this, &i_this->mAtInfo);

                MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(i * 2 + 1), mDoMtx_stack_c::get());
                mDoMtx_stack_c::multVecZero(&i_this->eyePos);
                dComIfGp_setHitMark(1, i_this, &i_this->eyePos, NULL, NULL, 0);
                mDoAud_seStart(Z2SE_EN_OI_HIT_TENTACLE, &i_this->eyePos, 0, 0);

                bvar = true;
                i_this->field_0x5f4 = 20;
                break;
            }
        }

        if (i_this->mAction == OH_ACTION_ATTACK && boss->field_0x5d10) {
            boss->field_0x5d10 = 0;
            bvar = true;
        }

        if (bvar) {
            i_this->field_0x60c = 2000.0f;
            i_this->field_0x610 = 0.5f;

            if (i_this->mAction != OH_ACTION_WAIT) {
                i_this->mAction = OH_ACTION_WAIT;
                i_this->mActionPhase = 0;
                i_this->field_0xc98 = 0;
                i_this->field_0xca2 = 0;
            }

            if (boss->mDemoAction != 0) {
                boss->mDemoAction = 100;
                boss->mCoreAnm = BCK_OI_WAIT;
                boss->mCoreAnmMode = J3DFrameCtrl::EMode_LOOP;
                boss->field_0x4794 = 180;
            }

            i_this->field_0x5f4 = 6;
            boss->mOISound.startCreatureVoice(Z2SE_EN_OI_V_TENT_DAMAGE, -1);
        }
    }
}

static int daB_OH_Execute(b_oh_class* i_this) {
    if (cDmrNowMidnaTalk()) {
        return 1;
    }

    if (i_this->field_0x5c8 == 0) {
        boss = (b_ob_class*)fopAcM_SearchByID(i_this->parentActorID);
    }

    if (boss == NULL) {
        return 1;
    }

    if (boss->mAction == OB_ACTION_CORE_END && i_this->mAction != OH_ACTION_END) {
        i_this->mAction = OH_ACTION_END;
        i_this->mActionPhase = 0;
    }

    cXyz collider_center;

    i_this->field_0x5cc++;

    for (int i = 0; i < 4; i++) {
        if (i_this->mTimers[i] != 0) {
            i_this->mTimers[i]--;
        }
    }

    if (i_this->field_0x5f4 != 0) {
        i_this->field_0x5f4--;
    }

    damage_check(i_this);
    action(i_this);

    mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y + i_this->field_0xcac,
                           i_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(i_this->shape_angle.x);
    mDoMtx_stack_c::scaleM(l_HIO.mModelSize, l_HIO.mModelSize, l_HIO.mModelSize);

    J3DModel* model_p = i_this->mpMorf->getModel();
    model_p->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->mpMorf->play(NULL, dComIfGp_getReverb(fopAcM_GetRoomNo(i_this)), 0);
    i_this->mpBtk->play();
    i_this->mpBrk->play();
    i_this->mpMorf->modelCalc();

    int tmp = 1;
    if (i_this->mDistToPlayer > 150.0f && i_this->mAction == OH_ACTION_WAIT) {
        tmp = i_this->field_0x5cc & 1;
    }

    for (int i = 0; i < 15; i++) {
        MTXCopy(model_p->getAnmMtx(tmp + i * 2), mDoMtx_stack_c::get());
        mDoMtx_stack_c::multVecZero(&collider_center);

        if (i_this->mAction >= OH_ACTION_END) {
            collider_center.z -= 20000.0f;
        }

        i_this->mColliders[i].SetC(collider_center);

        if (i_this->mAction == OH_ACTION_ATTACK) {
            i_this->mColliders[i].SetR(l_HIO.mModelSize * 70.0f);
        } else {
            i_this->mColliders[i].SetR(l_HIO.mModelSize * 50.0f);
        }

        dComIfG_Ccsp()->Set(&i_this->mColliders[i]);
    }

    return 1;
}

static int daB_OH_IsDelete(b_oh_class* i_this) {
    return 1;
}

static int daB_OH_Delete(b_oh_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "B_oh");
    if (i_this->field_0x1f86) {
        data_8061DBB5 = 0;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    b_oh_class* this_ = (b_oh_class*)i_this;

    this_->mpMorf = new mDoExt_McaMorf((J3DModelData*)dComIfG_getObjectRes("B_oh", BMDV_OH), NULL,
                                       NULL, NULL, 2, 1.0f, 0, -1, 1, NULL, 0, 0x11000284);
    if (this_->mpMorf == NULL || this_->mpMorf->getModel() == NULL) {
        return 0;
    }

    if (!this_->mInvisModel.create(this_->mpMorf->getModel(), 1)) {
        return 0;
    }

    this_->mpMorf->getModel()->setUserArea((uintptr_t)this_);

    for (u16 i = 0; i < this_->mpMorf->getModel()->getModelData()->getJointNum(); i++) {
        this_->mpMorf->getModel()->getModelData()->getJointNodePointer(i)->setCallBack(
            nodeCallBack);
    }

    this_->mpBtk = new mDoExt_btkAnm();
    if (this_->mpBtk == NULL) {
        return 0;
    }

    J3DAnmTextureSRTKey* btk = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("B_oh", BTK_OH_LOOP);
    if (!this_->mpBtk->init(this_->mpMorf->getModel()->getModelData(), btk, TRUE, 2, 1.0f, 0, -1)) {
        return 0;
    }

    this_->mpBtk->setFrame(cM_rndF(39.0f));
    this_->mpBtk->setPlaySpeed(cM_rndFX(0.1f) + 1.0f);

    this_->mpBrk = new mDoExt_brkAnm();
    if (this_->mpBrk == NULL) {
        return 0;
    }

    J3DAnmTevRegKey* brk = (J3DAnmTevRegKey*)dComIfG_getObjectRes("B_oh", BRK_OH_LOOP);
    if (!this_->mpBrk->init(this_->mpMorf->getModel()->getModelData(), brk, TRUE, 2, 1.0f, 0, -1)) {
        return 0;
    }

    this_->mpBrk->setFrame(cM_rndF(39.0f));
    this_->mpBrk->setPlaySpeed(cM_rndFX(0.1f) + 1.0f);

    return 1;
}

static int daB_OH_Create(fopAc_ac_c* i_this) {
    fopAcM_ct(i_this, b_oh_class);
    b_oh_class* this_ = (b_oh_class*)i_this;

    int phase = dComIfG_resLoad(&this_->mPhase, "B_oh");
    if (phase == cPhs_COMPLEATE_e) {
        this_->field_0x5c8 = fopAcM_GetParam(this_) & 0xFF;

        if (!fopAcM_entrySolidHeap(this_, useHeapInit, 0x23E0)) {
            return cPhs_ERROR_e;
        }

        if (!data_8061DBB5) {
            this_->field_0x1f86 = 1;
            data_8061DBB5 = 1;
            l_HIO.field_0x4 = -1;
        }

        this_->health = 1000;
        this_->field_0x560 = 1000;

        this_->field_0x5cc = cM_rndF(65536.0f);
        this_->field_0x5f6 = cM_rndF(65536.0f);
        this_->field_0x5f8 = cM_rndF(65536.0f);
        this_->field_0x5fa = cM_rndF(65536.0f);
        this_->field_0x614 = cM_rndF(100.0f) + 400.0f;

        static dCcD_SrcSph cc_sph_src = {
            {
                {0, {{0, 0, 12}, {0xD8FBFDFF, 3}, 0x15}},
                {dCcD_SE_NONE, 0, 0, 0, {0}},
                {dCcD_SE_NONE, 0, 0, 0, {0x80}},
                {0},
            },
            {
                {
                    {0.0f, 0.0f, 0.0f},
                    60.0f,
                },
            },
        };

        this_->mCcStts.Init(0xFF, 0, this_);
        for (int i = 0; i < 15; i++) {
            this_->mColliders[i].Set(cc_sph_src);
            this_->mColliders[i].SetStts(&this_->mCcStts);
            this_->mColliders[i].OnTgNoHitMark();
        }

        this_->current.angle.x = -0x3448;

        if (cDmr_SkipInfo) {
            this_->mAction = OH_ACTION_WAIT;
            this_->mActionPhase = 1;
            this_->mTimers[0] = cM_rndF(100.0f) + 200.0f;
            this_->mTentacleLength = l_HIO.mLength;
            this_->field_0x608 = 1.0f;
            Cinit = 1;
        } else {
            this_->mAction = OH_ACTION_START;
            Cinit = 0;
        }

        daB_OH_Execute(this_);
    }

    return phase;
}

AUDIO_INSTANCES;

static actor_method_class l_daB_OH_Method = {
    (process_method_func)daB_OH_Create,
    (process_method_func)daB_OH_Delete,
    (process_method_func)daB_OH_Execute,
    (process_method_func)daB_OH_IsDelete,
    (process_method_func)daB_OH_Draw,
};

actor_process_profile_definition g_profile_B_OH = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_B_OH,
    &g_fpcLf_Method.base,
    sizeof(b_oh_class),
    0,
    0,
    &g_fopAc_Method.base,
    218,
    &l_daB_OH_Method,
    0x00044000,
    fopAc_ENEMY_e,
    fopAc_CULLBOX_CUSTOM_e,
};
