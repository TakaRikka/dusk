/**
 * d_a_e_ym.cpp
 * Enemy - Shadow Insect / 闇虫 (Yami Mushi)
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_ym.h"
#include "c/c_damagereaction.h"
#include "d/actor/d_a_kago.h"
#include "d/actor/d_a_player.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_tag_firewall.h"
#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_enemy.h"
#include "f_op/f_op_camera_mng.h"

class daE_YM_HIO_c {
public:
    daE_YM_HIO_c();

    virtual ~daE_YM_HIO_c() {}

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 mModelSize;
    /* 0x0C */ f32 mElectricInvincibilityTimeExtension;
    /* 0x10 */ f32 mMoveSpeed;
    /* 0x14 */ f32 mFlyMoveSpeed;
    /* 0x18 */ f32 mFlyAttackSpeed;
    /* 0x1C */ f32 mSurpriseDistance;
    /* 0x20 */ f32 mMoveRange;
};

dCcD_SrcSph E_YM_n::cc_sph_src = {
    {
        {0, {{0x400, 1, 13}, {0xD8FBFDFF, 3}, 0x75}},
        {dCcD_SE_METAL, 0, 0, 0, {0}},
        {dCcD_SE_NONE, 0, 0, 0, {2}},
        {0},
    },
    {
        {
            {0.0f, 0.0f, 0.0f},
            40.0f,
        },
    }
};

static u16 w_eff_id[] = {0x01B8, 0x01B9, 0x01BA, 0x01BB}; // unused

daE_YM_HIO_c::daE_YM_HIO_c() {
    field_0x4 = -1;
    mFlyMoveSpeed = 20.0f;
    mFlyAttackSpeed = 30.0f;
    mModelSize = 0.6f;
    mElectricInvincibilityTimeExtension = 3.0f;
    mMoveSpeed = 13.0f;
    mSurpriseDistance = 400.0f;
    mMoveRange = 300.0f;
}


u8 daE_YM_c::checkBck(char const* i_arcName, int i_resNo) {
    return mpMorf->getAnm() == (J3DAnmTransform*)dComIfG_getObjectRes(i_arcName, i_resNo);
}

void daE_YM_c::bckSet(int i_resID, u8 i_attribute, f32 i_morf, f32 i_speed) {
    int tm_res_id;

    if (mFlyType == true) {
        switch (i_resID) {
        case 6:
            tm_res_id = 8;
            break;
        case 15:
            tm_res_id = 9;
            break;
        case 16:
            i_speed = 2.0f;
            tm_res_id = 10;
            break;
        case 14:
            tm_res_id = 10;
            break;
        default:
            tm_res_id = 9;
        }

        mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_TM", tm_res_id), i_attribute,
                            i_morf, i_speed, 0.0f, -1.0f);
    } else {
        mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_YM", i_resID), i_attribute,
                            i_morf, i_speed, 0.0f, -1.0f);
    }
}

void daE_YM_c::bckSetFly(int i_resID, u8 i_attribute, f32 i_morf, f32 i_speed) {
    mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_TM", i_resID), i_attribute,
                        i_morf, i_speed, 0.0f, -1.0f);
}

int daE_YM_c::draw() {
    if (field_0x71d) {
        return 1;
    }

    if (daPy_py_c::checkNowWolfEyeUp()) {
        cLib_addCalc2(&field_0x6d4, 255.0f, 1.0f, 30.0f);
    } else if (mAction == ACT_DOWN) {
        cLib_addCalc2(&field_0x6d4, 255.0f, 1.0f, 30.0f);
    } else {
        cLib_addCalc2(&field_0x6d4, 0.0f, 1.0f, 30.0f);
    }

    bool bvar = true;

    if (field_0x710 != 0) {
        if (!field_0x6d4) {
            return 1;
        }

        if (mIsHide && mAction != ACT_RAIL && mAction != ACT_BACK_RAIL) {
            bvar = false;
        }
    } else {
        field_0x710 = 1;
    }

    if (mFlyType == true) {
        J3DShape* shape_p =
            mpMorf->getModel()->getModelData()->getMaterialNodePointer(1)->getShape();

        if (shape_p != NULL) {
            if (field_0x6cc) {
                shape_p->show();
            } else {
                shape_p->hide();
            }
        }
    }

    J3DModel* model_p = mpMorf->getModel();
    g_env_light.settingTevStruct(6, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model_p, &tevStr);

    J3DModelData* modelData_p = model_p->getModelData();
    for (u16 i = 0; i < modelData_p->getMaterialNum(); i++) {
        modelData_p->getMaterialNodePointer(i)->getTevKColor(3)->a = field_0x6d4;
    }

    if (bvar) {
        if (field_0x6dc != -70.0f) {
            mpBrk->entry(model_p->getModelData());
            dComIfGd_setListDark();
            mpMorf->entryDL();
            dComIfGd_setList();
        }

        cXyz sp40;
        sp40.set(current.pos.x, current.pos.y + 100.0f, current.pos.z);

        field_0x69c = dComIfGd_setShadow(field_0x69c, 1, model_p, &sp40, 400.0f, 0.0f,
                                         current.pos.y, mAcch.GetGroundH(), mAcch.m_gnd,
                                         &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    }

    return 1;
}

static int daE_YM_Draw(daE_YM_c* i_this) {
    return i_this->draw();
}

static u8 lbl_253_bss_8;

static daE_YM_HIO_c l_HIO;

static fopAc_ac_c* m_near_obj;

static f32 m_obj_dist;

static void* s_obj_sub(void* param_0, void* param_1) {
    fopAc_ac_c* e_ym;
    fopAc_ac_c* near_obj;

    near_obj = (fopAc_ac_c*)param_0;
    e_ym = (fopAc_ac_c*)param_1;

    if (fopAcM_IsActor(near_obj) && fopAcM_GetName(near_obj) == PROC_Obj_Carry) {
        if (!fpcM_IsCreating(fopAcM_GetID(near_obj))) {
            f32 obj_dist = fopAcM_searchActorDistanceXZ(near_obj, e_ym);

            if (obj_dist < 100.0f && obj_dist < m_obj_dist &&
                fabsf(fopAcM_searchActorDistanceY(near_obj, e_ym)) < 30.0f)
            {
                m_near_obj = near_obj;
                m_obj_dist = obj_dist;
            }
        }
    }

    return 0;
}

void daE_YM_c::setDigEffect() {
    cXyz sp1C(field_0x68c, field_0x68c, field_0x68c);
    cXyz sp28(current.pos);
    sp28.y += field_0x6dc;

    field_0xad8 = dComIfGp_particle_set(field_0xad8, 0x83A8, &sp28, &shape_angle, &sp1C);
    field_0xadc = dComIfGp_particle_set(field_0xadc, 0x83A9, &sp28, &shape_angle, &sp1C);
}

void daE_YM_c::setElecEffect1() {
    f32 fVar3 = (field_0x68c * 10.0f) / 6.0f;
    cXyz cStack_2c(fVar3, fVar3, fVar3);
    MtxP iVar5;
    if (mFlyType == true) {
        iVar5 = mpMorf->getModel()->getAnmMtx(8);
    } else {
        iVar5 = mpMorf->getModel()->getAnmMtx(0);
    }

    cXyz cStack_38(iVar5[0][3], iVar5[1][3], iVar5[2][3]);
    field_0xad8 = dComIfGp_particle_set(field_0xad8, 0x8393, &cStack_38, &tevStr, &shape_angle, &cStack_2c,
                                        0xff, 0, -1, 0, 0, 0);
    field_0xadc = dComIfGp_particle_set(field_0xadc, 0x8394, &cStack_38, &tevStr, &shape_angle, &cStack_2c,
                                        0xff, 0, -1, 0, 0, 0);
}


void daE_YM_c::setElecEffect2() {
    f32 fVar3 = (field_0x68c * 10.0f) / 6.0f;
    cXyz cStack_2c(fVar3, fVar3, fVar3);
    MtxP iVar5;
    if (mFlyType == true) {
        iVar5 = mpMorf->getModel()->getAnmMtx(8);
    } else {
        iVar5 = mpMorf->getModel()->getAnmMtx(0);
    }
    cXyz cStack_38(iVar5[0][3], iVar5[1][3], iVar5[2][3]);
    setElecEffect1();
    field_0xae0 = dComIfGp_particle_set(field_0xae0, 0x8395, &cStack_38, &tevStr, 
                                        &shape_angle, &cStack_2c,
                                        0xff, 0, -1, 0, 0, 0);
    field_0xae4 = dComIfGp_particle_set(field_0xae4, 0x8396, &cStack_38, &tevStr,
                                        &shape_angle, &cStack_2c,
                                        0xff, 0, -1, 0, 0, 0);
}


void daE_YM_c::setFireEffect() {
    cXyz my_vec_0(1.0f, 1.0f, 1.0f);
    mParticleKeys[0] = dComIfGp_particle_set(mParticleKeys[0], 0x3ad, &current.pos, NULL,
                                             NULL, &my_vec_0, 0xff, NULL, -1,
                                             NULL, NULL, NULL);
    mParticleKeys[1] = dComIfGp_particle_set(mParticleKeys[1], 0x3af, &current.pos, NULL,
                                             NULL, &my_vec_0, 0xff, NULL, -1,
                                             NULL, NULL, NULL);
    mParticleKeys[2] = dComIfGp_particle_set(mParticleKeys[2], 0x3ae, &current.pos, NULL,
                                             NULL, &my_vec_0, 0xff, NULL, -1,
                                             NULL, NULL, NULL);
    if (field_0x6d0 != NULL) {
        for (int idx = 0x2c; idx >= 0; --idx) {
            field_0x6d0[idx+1] = field_0x6d0[idx];
        }
        field_0x6d0[0] = speed;
        for (int idx = 0; idx < 3; ++idx) {
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mParticleKeys[idx]);
            if (emitter != NULL) {
                if (speed.abs() > 1.0f) {
                    emitter->setParticleCallBackPtr((JPAParticleCallBack*)&JPTracePCB4);
                    emitter->setUserWork((uintptr_t)field_0x6d0);
                } else {
                    emitter->setParticleCallBackPtr(NULL);
                }
            }
        }
    }
}

bool daE_YM_c::checkWallCrash() {
    if (mType == 1 && (daPy_getPlayerActorClass()->checkFrontRollCrash() ||
                       daPy_getPlayerActorClass()->checkWolfAttackReverse()))
    {
        cXyz tmp = daPy_getPlayerActorClass()->current.pos - mPrevPos;
        if (tmp.absXZ() < 600.0f) {
            if (mTagPosP != NULL) {
                setActionMode(ACT_FLY);
            } else {
                setActionMode(ACT_FALL);
            }

            return true;
        }
    }

    return false;
}

u8 daE_YM_c::checkWolfBark() {
    if (mType == 2) {
        return 0;
    } else if (daPy_getPlayerActorClass()->checkWolfBark() && mDistToPlayer < 900.0f) {
        if (mType == 0) {
            s16 dist_ang = cLib_distanceAngleS(mAngleToPlayer, shape_angle.y);
            cXyz my_vec_0 = current.pos - mPrevPos;
            if (my_vec_0.abs() < l_HIO.mMoveRange && dist_ang < 0x4000 && cM_rndF(10.0f) > 1.0f) {
                setActionMode(ACT_BACK);
            } else {
                if (mTagPosP != NULL) {
                    setActionMode(ACT_FLY);
                } else {
                    setActionMode(ACT_SURPRISE);
                }
            }
        } else if (mType == 1) {
            return 0;
        }
        return 1;
    } else {
        return 0;
    }
}

u8 daE_YM_c::checkSurpriseLock() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (mType == 2) {
        return 0;
    }
    if (mAction != ACT_WAIT && mAction != ACT_MOVE && mAction != ACT_SURPRISE
        && mAction != ACT_ATTACK && mAction != ACT_ATTACK_WALL) {
        return 0;
    }
    if (field_0x6f6 != 0) {
        return 0;
    }
    if (field_0x6fc) {
        return 0;
    }
    if (mAction != 8) {
        bool truth = dComIfGp_getAttention()->LockonTruth();
        if (truth) {
            fopAc_ac_c* tgt = dComIfGp_getAttention()->LockonTarget(0);
            if (tgt == this && mType == 0) {
                cXyz my_vec_0 = current.pos - mPrevPos;
                cXyz my_vec_1 = player->current.pos - mPrevPos;
                if (mType == 4) {
                    return checkRailSurprise();
                }
                switch (field_0x6a1) {
                    case 0:
                        if (my_vec_0.abs() > l_HIO.mMoveRange || my_vec_1.abs() > (l_HIO.mMoveRange + 150.0f)) {
                            if (mAction != ACT_SURPRISE) {
                                if (mTagPosP != NULL) {
                                    setActionMode(ACT_FLY);
                                } else {
                                    setActionMode(ACT_SURPRISE);
                                }
                                return 1;
                            }
                            return 0;
                        } else {
                            setActionMode(ACT_ATTACK);
                            return 1;
                        }
                    case 1:
                        if (mAction != ACT_SURPRISE) {
                            if (mTagPosP != NULL) {
                                setActionMode(ACT_FLY);
                            } else {
                                setActionMode(ACT_SURPRISE);
                            }
                        }
                        return 1;
                    default:
                        break;
                }
            }
        }
    }
    return 0;
}

u8 daE_YM_c::checkRailSurprise() {
    if (dComIfGp_checkPlayerStatus0(0, 0x8000000)) {
        if (mDistToPlayer > 200.0f || (s16)cLib_distanceAngleS(mAngleToPlayer, shape_angle.y) > 0x2000) {
            field_0x70a = 0x1e;
        }
    }
    if (field_0x70a != 0) {
        return 0;
    }
    if (field_0x704 == 0) {
        setActionMode(ACT_BACK_RAIL);
    } else {
        setActionMode(ACT_SURPRISE);
    }
    return 1;
}

u8 daE_YM_c::checkSurpriseNear() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz my_vec_0;
    if (mType == 1) {
        if (field_0x6fa) {
            return 0;
        }
        if (player->checkClimbMove()) {
            f32 my_val = field_0x6e0 - 50.0f;
            if (my_val < 50.0f) {
                my_val = 50.0f;
            }
            cXyz my_vec_1 = player->current.pos;
            my_vec_1.y += 100.0f;
            my_vec_0 = mPrevPos - current.pos;
            if (my_vec_0.abs() < my_val) {
                my_vec_0 = my_vec_1 - current.pos;
                if (my_vec_0.abs() < 350.0f) {
                    dBgS_GndChk gnd_chk;
                    gnd_chk.SetPos(&my_vec_1);
                    if ((player->current.pos.y - dComIfG_Bgsp().GroundCross(&gnd_chk)) > 50.0f) {
                        field_0x6a5 = 0;
                        setActionMode(ACT_ATTACK_WALL);
                        return 1;
                    }
                }
            } else {
                my_vec_0 = mPrevPos - my_vec_1;
                if (my_vec_0.abs() < 100.0f) {
                    field_0x6a5 = 1;
                    setActionMode(ACT_ATTACK_WALL);
                    return 1;
                }
            }
        }
    } else if (mType == 0 || mType == 4) {
        if (mAcch.ChkGroundHit() == 0) {
            return 0;
        }
        f32 my_val;
        if (mType == 4) {
            my_val = l_HIO.mSurpriseDistance + 200.0f;
        } else {
            if (field_0x6a9 != 0) {
                my_val = l_HIO.mSurpriseDistance;
            } else {
                my_val = l_HIO.mSurpriseDistance + 200.0f;
            }
        }
        if (mDistToPlayer < my_val) {
            if (mSphCc.ChkCoHit()) {
                cCcD_Obj* hit_obj = mSphCc.GetCoHitObj();
                fopAc_ac_c* my_ac = dCc_GetAc(hit_obj->GetAc());
                if (fopAcM_GetName(my_ac) == PROC_ALINK) {
                    if (mType == 4) {
                        return checkRailSurprise();
                    }
                    if (mTagPosP != NULL) {
                        setActionMode(ACT_FLY);
                    } else {
                        setActionMode(ACT_SURPRISE);
                    }
                    return 1;
                }
            }
            if (mAction == ACT_ATTACK) {
                return 0;
            }
            if (player->getSpeedF() >= 16.0f) {
                field_0x6f6 = 0;
            } else {
                if (field_0x6f8) {
                    return 0;
                }
                if (mDistToPlayer > l_HIO.mSurpriseDistance - 100.0f) {
                    field_0x6f8 = 0x3c;
                    return 0;
                }
            }
            if (field_0x6f6 == 0) {
                if (mType == 4) {
                    return checkRailSurprise();
                }
                if (mTagPosP != NULL) {
                    setActionMode(ACT_FLY);
                } else {
                    setActionMode(ACT_SURPRISE);
                }
                return 1;
            }
        } else {
            field_0x6f8 = 0;
        }
    }
    return 0;
}

void daE_YM_c::setNormalCc() {
    mSphCc.SetCoSPrm(0x75);
    mSphCc.SetTgType(0xD8FBFDFF);
}

void daE_YM_c::setAppear() {
    mIsHide = false;

    if (field_0x6a6 != 4) {
        mType = 0;
    }

    field_0x714 = 4;
}

void daE_YM_c::setMoveSound(int param_0) {
    if (mFlyType == 1) {
        if (checkBck("E_TM", 10) == 0) {
            return;
        }
    } else if (checkBck("E_YM", 16) == 0) {
        return;
    }
    mpMorf->setPlaySpeed(fabsf(speedF) * 0.15f);
    if (mpMorf->checkFrame(1.0f) || mpMorf->checkFrame(5.0f) || mpMorf->checkFrame(9.0f)) {
        if (param_0) {
            mSound.startCreatureSound(Z2SE_EN_YM_FN_UNDER, 0, -1);
        } else {
            mSound.startCreatureSound(Z2SE_EN_YM_FOOTNOTE, 0, -1);
        }
    }
}

void daE_YM_c::setTurnSound() {
    if (mpMorf->checkFrame(1.0f) || mpMorf->checkFrame(5.0f) || mpMorf->checkFrame(9.0f)) {
        mSound.startCreatureSound(Z2SE_EN_YM_FOOTNOTE, 0, -1);
    }
}

void daE_YM_c::setActionMode(int i_action) {
    mLastAction = mAction;
    mAction = i_action;
    mMode = 0;
    field_0x6f2 = 0;
    field_0x6f0 = 0;
    current.angle.y = shape_angle.y;
}

void daE_YM_c::executeWait() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz my_vec_0;
    if (mType != 2) {
        mSound.startCreatureSoundLevel(Z2SE_EN_YM_WAIT, 0, -1);
    }
    if (mType != 1) {
        cLib_chaseF(&speed.y, -60.0f, 5.0f);
    }
    if (field_0x6a1 == 1 && mMode != 0 && field_0x6f2 == 0) {
        field_0x6c9 = 0;
        field_0x708 = 0;
    }
    switch (mMode) {
        case 0x64:
            field_0x6f0 = 0x1e;
            mMode = 0x65;
        case 0x65: {
            current.pos = home.pos;
            setHideType();
            mPrevPos = current.pos;
            if (field_0x6a6 == 1 || field_0x6f0 == 0) {
                field_0x6f0 = 0;
                mMode = 0;
            }
            break;
        }
        case 0:
            field_0x6f2 = 0x1e;
            checkWall();
            bckSet(0xf, 2, 5.0f, 1.0f);
            if (mType == 2) {
                mMode = 2;
                field_0x6dc = -70.0f;
                field_0x714 = 0;
                field_0x6f0 = 5;
                field_0x6a5 = 0;
            } else if (mType == 1) {
                if (field_0x6a3 != 0xff) {
                    if (dComIfGs_isSwitch(field_0x6a3, fopAcM_GetRoomNo(this)) == 0) {
                        mMode = 4;
                        break;
                    }
                }
                setNormalCc();
                mMode = 1;
                if (mDistToPlayer < 400.0f) {
                    field_0x6f0 = cM_rndFX(10.0f) + 30.0f;
                } else {
                    field_0x6f0 = cM_rndFX(30.0f) + 100.0f;
                }
            } else {
                setNormalCc();
                mMode = 1;
                field_0x6f0 = cM_rndFX(30.0f) + 100.0f;
            }
            break;
        case 1:
            if (checkSurpriseNear()) {
                return;
            }
            if (checkWolfBark()) {
                return;
            }
            if (checkWallCrash()) {
                return;
            }
            if (field_0x6f0 == 0) {
                setActionMode(ACT_MOVE);
                return;
            }
            break;
        case 2:
            checkFrinedSamePos();
            switch (field_0x6a6) {
                case 1:
                    field_0x718 = 1;
                    if (field_0x6d4) {
                        field_0x714 = 4;
                    } else {
                        field_0x714 = 0;
                    }
                    m_near_obj = NULL;
                    m_obj_dist = 100.0f;
                    fpcM_Search(s_obj_sub, this);
                    if (m_near_obj == NULL) {
                        setActionMode(ACT_ESCAPE);
                        field_0x718 = 0;
                    }
                    break;
                case 2:
                    break;
                default:
                    if (player->checkWolfDig()) {
                        my_vec_0 = player->getLeftHandPos() - current.pos;
                        if (my_vec_0.abs() < 200.0f) {
                            field_0x6f0 = 0x14;
                            mMode = 3;
                            field_0x6a6 = 0;
                            fopAcM_OnStatus(this, 0x4000);
                        }
                    }
            }
            break;
        case 3:
            if (field_0x6f0 == 0) {
                setActionMode(ACT_ESCAPE);
            }
            break;
        case 4:
            if (field_0x6d4) {
                field_0x714 = 4;
            } else {
                field_0x714 = 0;
            }
            if (dComIfGs_isSwitch(field_0x6a3, fopAcM_GetRoomNo(this)) && mType == 1) {
                field_0x714 = 4;
                if (mTagPosP != NULL) {
                    setActionMode(ACT_FLY);
                } else {
                    setActionMode(ACT_FALL);
                }
            }
            break;
        default:
            break;
    }
    current.angle.y = shape_angle.y;
}

void daE_YM_c::executeMove() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    (void) player; // acquired, but not used.
    cXyz my_vec_0;
    cXyz my_vec_1;
    f32 my_val = field_0x6e0 - 50.0f;
    if (my_val < 50.0f) {
        my_val = 50.0f;
    }
    if (checkSurpriseNear() == 0 && checkWolfBark() == 0 && checkWallCrash() == 0 && setAttackMotion() == 0) {
        if (mType != 1) {
            cLib_chaseF(&speed.y, -60.0f, 5.0f);
        }
        switch (mMode) {
            case 0:
                bckSet(0xe, 2, 10.0f, 1.5f);
                checkWall();
                if (mType == 1) {
                    mAcch.SetGroundUpY(fabsf(cM_ssin(field_0x668.x) * 30.0f));
                }
                field_0x6e4 = cM_rndFX(32768.0f);
                if ((s16)cLib_distanceAngleS(field_0x6e4, shape_angle.y) < 0x2000) {
                    field_0x6e4 = shape_angle.y + 0x4000;
                }
                my_vec_0 = mPrevPos - current.pos;
                field_0x6a5 = 0;
                if (my_vec_0.abs() > my_val) {
                    if (mType == 1) {
                        mDoMtx_stack_c::ZrotS(-field_0x668.z);
                        mDoMtx_stack_c::XrotM(-field_0x668.x);
                        mDoMtx_stack_c::multVec(&my_vec_0, &my_vec_1);
                    } else {
                        my_vec_1 = my_vec_0;
                    }
                    field_0x6e4 = cM_atan2s(my_vec_1.x, my_vec_1.z);
                    field_0x6a5 = 1;
                }
                mMode = 1;
                break;
            case 1: {
                setTurnSound();
                s16 my_step = 0x200;
                if (mType == 1 && mDistToPlayer < 400.0f) {
                    my_step = 0x300;
                }
                if (cLib_chaseAngleS(&shape_angle.y, field_0x6e4, my_step)) {
                    if ((!field_0x6e0 && field_0x6a5 == 0) || checkBeforeBg(field_0x6e4)) {
                        setActionMode(ACT_WAIT);
                    } else {
                        bckSet(0xf, 2, 5.0f, 1.0f);
                        field_0x6f0 = cM_rndF(10.0f) + 5.0f;
                        mMode = 2;
                    }
                }
                break;
            }
            case 2:
                mSound.startCreatureSoundLevel(Z2SE_EN_YM_WAIT, 0, -1);
                if (field_0x6f0 == 0 && mpMorf->checkFrame(1.0f)) {
                    bckSet(0x10, 2, 5.0f, 1.0f);
                    mMode = 3;
                    speedF = 10.0f;
                    if (field_0x6a5 == 0 && speedF > field_0x6e0) {
                        speedF = field_0x6e0;
                    }
                    field_0x6f0 = cM_rndF(40.0f) + 10.0f;
                }
                break;
            case 3: {
                setMoveSound(0);
                bool my_bool = false;
                my_vec_0 = current.pos - mPrevPos;
                if (field_0x6a5 == 0) {
                    if (my_vec_0.abs() > field_0x6e0) {
                        my_bool = true;
                    }
                } else {
                    if (my_vec_0.abs() < my_val) {
                        field_0x6a5 = 0;
                    }
                }
                if (my_bool || field_0x6f0 == 0 || checkBeforeBg(field_0x6e4)) {
                    speedF = 0.0f;
                    setActionMode(ACT_WAIT);
                    return;
                }
                break;
            }
            default:
                break;
        }
        current.angle.y = shape_angle.y;
    }
}

void daE_YM_c::executeEscape() {
    cLib_chaseF(&speed.y, -60.0f, 5.0f);
    daPy_py_c* player = daPy_getPlayerActorClass();
    (void) player; // acquired, but not used.
    cXyz my_vec_0;
    if (field_0x6dc < 0.0f) {
        field_0x6dc += 8.0f;
        if (field_0x6dc > 0.0f) {
            field_0x6dc = 0.0f;
        }
    }
    if (dComIfGp_event_runCheck() == 0) {
        fopAcM_OffStatus(this, 0x4000);
    }
    switch (mMode) {
        case 0:
            if (mType == 2) {
                setAppear();
            }
            if (mLastAction != 4) {
                s16 adj_angle = mAngleToPlayer + 0x8000;
                if (mFriendFlag & 0x1) {
                    shape_angle.y = adj_angle;
                } else if (mFriendFlag & 0x2) {
                    if (field_0x6a6 == 0) {
                        shape_angle.y = adj_angle + 0x2800;
                    } else {
                        shape_angle.y = adj_angle + 0x1800;
                    }
                } else if (mFriendFlag & 0x4) {
                    if (field_0x6a6 == 0) {
                        shape_angle.y = adj_angle - 0x2800;
                    } else {
                        shape_angle.y = adj_angle - 0x1800;
                    }
                } else {
                    shape_angle.y = adj_angle + cM_rndFX(8192.0f);
                }
                if (field_0x6a6 == 0 || field_0x6a6 == 4) {
                    field_0x6f0 = 0x28;
                    mMode = 3;
                    speedF = 5.0f;
                    speed.y = cM_rndF(5.0f) + 30.0f;
                    field_0x6a5 = 0;
                } else {
                    speedF = 10.0f;
                    speed.y = cM_rndF(5.0f) + 45.0f;
                    field_0x6f0 = 0x28;
                    mMode = 1;
                }
                field_0x6dc = 0.0f;
                bckSet(0xb, 0, 5.0f, 1.0f);
                mSound.startCreatureSound(Z2SE_EN_YM_JUMP, 0, -1);
            } else {
                field_0x6f2 = 10;
                mMode = 2;
            }
            field_0x6a9 = 1;
            field_0x6e4 = shape_angle.y;
            field_0x6e6 = 0x5dc;
            field_0x6e8 = 0;
            break;
        case 1:
            setElecEffect1();
            mSound.startCreatureSoundLevel(Z2SE_EN_YM_ELECTRIC, 0, -1);
            if (mAcch.ChkGroundHit()) {
                mSound.startCreatureSound(Z2SE_EN_YM_LAND, 0, -1);
                bckSet(0xc, 0, 0.0f, 1.0f);
                speedF = 0.0f;
                speed.y = 0.0f;
                mMode = 2;
                field_0x6f2 = 10;
                if (field_0x6f0 >= 2) {
                    field_0x6f0 = 1;
                }
                field_0x70e = 0x5a;
            }
            break;
        case 2:
            if (field_0x70e) {
                setElecEffect1();
                mSound.startCreatureSoundLevel(Z2SE_EN_YM_ELECTRIC, 0, -1);
            }
            mSound.startCreatureSoundLevel(Z2SE_EN_YM_WAIT, 0, -1);
            if (field_0x6f2 == 0) {
                speedF = 10.0f;
                mMode = 4;
                field_0x6e6 = 0x5dc;
                bckSet(0x10, 2, 5.0f, 1.0f);
            }
            break;
        case 3:
            setElecEffect1();
            mSound.startCreatureSoundLevel(Z2SE_EN_YM_ELECTRIC, 0, -1);
            if (field_0x6a5 == 0) {
                cLib_addCalcAngleS(&shape_angle.y, field_0x6e4 + field_0x6e8, 8, 0x1000, 0x100);
                if (mAcch.ChkGroundHit()) {
                    mSound.startCreatureSound(Z2SE_EN_YM_LAND, 0, -1);
                    field_0x6a5 = 1;
                    speedF = 0.0f;
                    bckSet(0xc, 0, 0.0f, 1.0f);
                }
            } else {
                if (mpMorf->isStop()) {
                    mMode = 4;
                    bckSet(0x10, 2, 5.0f, 1.0f);
                }
            }
            break;
        case 4:
            if (field_0x70e) {
                setElecEffect2();
                mSound.startCreatureSoundLevel(Z2SE_EN_YM_ELECTRIC_LOOP, 0, -1);
                field_0x700 = l_HIO.mElectricInvincibilityTimeExtension;
                mSphCc.OnAtSetBit();
            }
            cLib_chaseF(&speedF, 20.0f, 1.0f);
            setMoveSound(0);
            if (mAcch.ChkWallHit()) {
                field_0x6e4 += 0x800;
            }
            if (field_0x6e8 >= 0) {
                field_0x6e6 -= 200;
            } else {
                field_0x6e6 += 200;
            }
            field_0x6e8 += field_0x6e6;
            cLib_addCalcAngleS(&shape_angle.y, field_0x6e4 + field_0x6e8, 8, 0x1000, 0x100);
            my_vec_0 = current.pos - mPrevPos;
            if (my_vec_0.absXZ() > 500.0f) {
                speedF = 0.0f;
                mPrevPos = current.pos;
                field_0x6dc = 0.0f;
                setActionMode(ACT_WAIT);
                setNormalCc();
            }
            break;
        default:
            break;
    }
    if (field_0x6f0 == 1) {
        mSphCc.SetCoSPrm(0x75);
    }
    current.angle.y = shape_angle.y;
}

void daE_YM_c::executeDown() {
    field_0x6f4 = 10;
    field_0x6dc = 0.0f;
    maxFallSpeed = -60.0f;
    Vec wtr_pos = current.pos;
    wtr_pos.y += 500.0f;
    dBgS_ObjGndChk_Wtr gnd_chk_wtr;
    gnd_chk_wtr.SetPos(&wtr_pos);
    f32 gnd_cross = dComIfG_Bgsp().GroundCross(&gnd_chk_wtr);
    if (current.pos.y > gnd_cross) {
        gravity = -5.0f;
    } else {
        gravity = 0.0f;
    }
    OS_REPORT("YM executeDown %d %f %f \n", gnd_cross, current.pos.z, mMode);
    field_0x6cf = 0;
    switch (mMode) {
        case 0:
            if (mFlyType == 1) {
                bckSet(6, 0, 5.0f, 0.0f);
            } else {
                bckSet(0x10, 2, 5.0f, 1.0f);
            }
            mMode = 1;
            field_0x668.set(0, 0, 0);
            speedF = cM_rndF(5.0f) + 15.0f;
            speed.y = cM_rndF(5.0f) + 40.0f;
            shape_angle.y = mAtInfo.mHitDirection.y + 0x8000;
            shape_angle.x = 0;
            mAcch.ClrGroundHit();
            mAcchCir.SetWall(40.0f, 60.0f);
            if (mType == 1) {
                cXyz my_vec_0 = daPy_getPlayerActorClass()->current.pos;
                my_vec_0.y += 50.0f;
                cXyz my_vec_1(speedF * cM_ssin(shape_angle.y), speed.y, speedF * cM_scos(shape_angle.y));
                my_vec_1 += current.pos;
                dBgS_LinChk lin_chk;
                lin_chk.Set(&my_vec_0, &my_vec_1, NULL);
                if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                    cM3dGPla plane;
                    dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
                    current.pos = lin_chk.GetCross() + (plane.mNormal * 60.0f);
                    speedF = 0.0f;
                }
            }
            setAppear();
            field_0x714 = 0;
            if (0 == strcmp("R_SP108", dComIfGp_getStartStageName())) {
                if (dComIfGp_getStartStageRoomNo() == 0 && dComIfG_play_c::getLayerNo(0) == 0xe && mSwitchBit != 0xff) {
                    if (dComIfGs_isSwitch(mSwitchBit, fopAcM_GetRoomNo(this)) == 0) {
                        dComIfGs_onSwitch(mSwitchBit, fopAcM_GetRoomNo(this));
                    }
                }
            }
            break;
        case 1:
        case 2:
            if (mMode == 1) {
                if (speed.y < 0.0f) {
                    mAcch.SetGroundUpY(30.0f);
                }
                cLib_addCalcAngleS(&shape_angle.x, 0x7800, 8, 0x1000, 0x10);
            } else {
                cLib_addCalcAngleS(&shape_angle.x, -0x8000, 8, 0x1000, 0x10);
            }
            if (mAcch.ChkGroundHit()) {
                if (mFlyType != 1) {
                    bckSet(6, 0, 0.0f, 1.0f);
                }
                if (mMode == 1) {
                    mSound.startCreatureSound(Z2SE_EN_YM_LAND, 0, -1);
                    mMode = 2;
                    speedF = cM_rndF(5.0f) + 5.0f;
                    speed.y = 15.0f;
                } else {
                    mSound.startCreatureSound(Z2SE_EN_YM_MOGAKU, 0, -1);
                    mMode = 3;
                    speedF = 0.0f;
                    shape_angle.x = -0x8000;
                    field_0x6f0 = 0;
                }
                break;
            }
            if (current.pos.y < gnd_cross) {
                mSound.startCreatureSound(Z2SE_CM_BODYFALL_WATER_M, 0, -1);
                mSound.startCreatureSound(Z2SE_EN_YM_MOGAKU, 0, -1);
                bckSet(6, 0, 0.0f, 1.0f);
                speedF = 0.0f;
                gravity = 0.0f;
                speed.y = 0.0f;
                current.pos.y = gnd_cross;
                shape_angle.x = -0x8000;
                field_0x6f0 = 0;
                mMode = 3;
                break;
            }
            if (speed.y < 0.0f && fabsf(gnd_cross - current.pos.y) > 1000.f) {
                dBgS_GndChk gnd_chk;
                gnd_chk.SetPos(&current.pos);
                gnd_cross = dComIfG_Bgsp().GroundCross(&gnd_chk);
                if (gnd_cross == -G_CM3D_F_INF || fabsf(gnd_cross - current.pos.y) > 1000.0f
                    || dComIfG_Bgsp().GetGroundCode(gnd_chk) == 4 || dComIfG_Bgsp().GetGroundCode(gnd_chk) == 10
                    || dComIfG_Bgsp().GetGroundCode(gnd_chk) == 5) {
                    bckSet(6, 0, 0.0f, 1.0f);
                    mMode = 3;
                    speedF = 0.0f;
                    shape_angle.x = -0x8000;
                    field_0x6f0 = 0;
                }
            }
            break;
        case 3:
            if (mpMorf->isStop()) {
                if (mSwitchBit != 0xff && dComIfGs_isSwitch(mSwitchBit, fopAcM_GetRoomNo(this)) == 0) {
                    dComIfGs_onSwitch(mSwitchBit, fopAcM_GetRoomNo(this));
                }
                fopAcM_delete(this);
                fopAcM_createDisappear(this, &current.pos, 10, 1, 0xff);
            }
            break;
        default:
            break;
    }
    current.angle.y = shape_angle.y;
}

void daE_YM_c::damage_check() {
    if (field_0x6f4 != 0 || mAction == ACT_RIVER || mAction == ACT_DOWN) {
        return;
    }
    mStts.Move();
    if (mSphCc.ChkTgHit()) {
        mAtInfo.mpCollider = mSphCc.GetTgHitObj();
        if (field_0x700 == 0 || !mAtInfo.mpCollider->ChkAtType(0xD8000000)) {
            cc_at_check(this, &mAtInfo);
            if (mAtInfo.mpCollider->ChkAtType(0xD8000000)) {
                field_0x6f4 = 20;
            } else {
                field_0x6f4 = 10;
            }
            if (mAtInfo.mAttackPower <= 1) {
                field_0x6f4 = 10;
            }
            if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG)) {
                if (mIsHide) {
                    setActionMode(ACT_WIND);
                }
            } else if (mType != 2) {
                mSound.startCreatureSound(Z2SE_EN_YM_DEATH, 0, -1);
                if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_10000000)) {
                    dComIfGp_setHitMark(3, this, &eyePos, NULL, NULL, 0);
                }
                setActionMode(ACT_DOWN);
                executeDown();
            }
        }
    }
}

void daE_YM_c::executeWind() {
    cLib_chaseF(&speed.y, -60.0f, 5.0f);
    switch (mMode) {
        case 0:
            setAppear();
            setNormalCc();
            field_0x6f0 = cM_rndFX(5.0f) + 20.0f;
            field_0x6f2 = 0x28;
            mMode = 1;
            field_0x6dc = 0.0f;
        case 1:
            if (speed.y < 0.0f) {
                mAcch.SetGroundUpY(50.0f);
            }
            cLib_chaseAngleS(&shape_angle.z, -0x8000, 0x400);
            if (field_0x6f0) {
                cLib_chaseAngleS(&field_0x6e6, 4000, 200);
                speed.y = 25.0f;
            } else {
                if (mAcch.ChkGroundHit()) {
                    mMode = 2;
                    speedF = 0.0f;
                    speed.y = 15.0f;
                }
            }
            break;
        case 2:
            if (mAcch.ChkGroundHit()) {
                speed.y = 0.0f;
                speedF = 0.0f;
                if (cLib_chaseAngleS(&field_0x6e6, 0, 200)) {
                    mMode = 3;
                    field_0x6f0 = 0x32;
                    field_0x6f2 = (u8)(cM_rndF(5.0f) + 5.0f);
                    field_0x6e8 = shape_angle.y;
                    field_0x6a5 = 0;
                }
            }
            break;
        case 3:
            if (field_0x6f0 < 0xf) {
                shape_angle.z = cM_rndF(4096.0f) + 32768.0f;
                shape_angle.y = field_0x6e8 + cM_rndF(2048.0f);
            } else {
                if (field_0x6f2 == 0) {
                    field_0x6f2 = (u8)(cM_rndF(5.0f) + 5.0f);
                    field_0x6a5 = field_0x6a5 + 1 & 1;
                }
                if (field_0x6a5 == 0) {
                    shape_angle.z = cM_rndF(2048.0f) + 32768.0f;
                    shape_angle.y = field_0x6e8 + cM_rndF(2048.0f);
                }
            }
            if (field_0x6f0 == 0) {
                speedF = cM_rndF(5.0f) + 5.0f;
                speed.y = 15.0f;
                mMode = 4;
                mAcch.SetGroundUpY(0.0f);
                current.angle.y = shape_angle.y - 0x4000;
            }
            break;
        case 4:
            if (cLib_chaseAngleS(&shape_angle.z, 0, 0x1000) && mAcch.ChkGroundHit()) {
                speed.y = 0.0f;
                speedF = 0.0f;
                setActionMode(ACT_ESCAPE);
            }
            break;
        default:
            break;
    }
    shape_angle.y += field_0x6e6;
}

int daE_YM_c::getSurpriseType() {
    cXyz my_vec_0 = mPrevPos - current.pos;
    int ret;
    if (my_vec_0.abs() > l_HIO.mMoveRange) {
        ret = 2;
    } else {
        s16 dist_ang = cLib_distanceAngleS(mAngleToPlayer, shape_angle.y);
        ret = 1;
        if (dist_ang < 0x3000 && cM_rndF(10.0f) > 8.0f) {
            ret = 0;
        }
    }
    return ret;
}

void daE_YM_c::setSurpriseAway() {
    field_0x6f0 = 0x1e;
    mMode = 1;
    bckSet(0x10, 2, 3.0f, speedF / 12.0f);
}

void daE_YM_c::setGoHomeType() {
    s16 tgt_ang = cLib_targetAngleY(&current.pos, &mPrevPos);
    f32 rand_val = cM_rnd();
    field_0x6a8 = 1;
    if (field_0x6a8 == 0 && rand_val < 0.1f && mAcch.ChkGroundHit() && mAcch.ChkWallHit() == 0) {
        field_0x6a8 = 1;
        field_0x6a5 = 2;
        shape_angle.y = tgt_ang + cM_rndFX(4000.0f);
        mMode = 3;
    } else {
        if ((s16)(mAngleToPlayer - tgt_ang) < 0) {
            field_0x6a5 = 2;
        } else {
            field_0x6a5 = 3;
        }
        if (field_0x6cb) {
            speedF = 25.0f;
        } else {
            speedF = l_HIO.mMoveSpeed;
        }
        setSurpriseAway();
    }
}

void daE_YM_c::executeSurprise() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    if (mType != 1) {
        cLib_chaseF(&speed.y, -90.0f, 15.0f);
    }
    cXyz my_vec_0 = mPrevPos - current.pos;
    if (daPy_getPlayerActorClass()->checkWolfLock(this) && mAcch.ChkGroundHit()) {
        cLib_chaseF(&speedF, 0.0f, 5.0f);
        cLib_chaseAngleS(&shape_angle.y,mAngleToPlayer, 0x400);
        return;
    }
    switch (mMode) {
        case 0:
            if (field_0x6a1 == 1 && field_0x708 == 0 && field_0x6c9 == 0) {
                field_0x708 = 0x78;
            }
            if (getSurpriseType() == 2 && field_0x6a9) {
                setGoHomeType();
            } else {
                field_0x6a5 = 0;
                field_0x6e4 = mAngleToPlayer + 0x8000;
                s16 sh_ang = player->shape_angle.y;
                s16 dist_ang = cLib_distanceAngleS(field_0x6e4, player->shape_angle.y);
                if (sh_ang + dist_ang == field_0x6e4) {
                    field_0x6a5 = 0;
                } else {
                    field_0x6a5 = 1;
                }
                if (field_0x6cb) {
                    speedF = 25.0f;
                    if (field_0x6a9 == 0) {
                        speedF = 28.0f;
                        field_0x6a9 = 1;
                    }
                } else {
                    speedF = 17.0f;
                    if (field_0x6a9 == 0) {
                        speedF = 20.0f;
                        field_0x6a9 = 1;
                    }
                }
                setSurpriseAway();
            }
            break;
        case 1:
            if (mAcch.ChkWallHit() && field_0x6a5 < 2) {
                setGoHomeType();
                return;
            }
            setMoveSound(0);
            if (field_0x6a5 == 0) {
                field_0x6e4 = mAngleToPlayer - 0x6000;
            } else if (field_0x6a5 == 1) {
                field_0x6e4 = mAngleToPlayer + 0x6000;
            } else {
                s16 sh_val = -0x2000;
                if (field_0x6a5 == 2) {
                    sh_val = 0x2000;
                }
                if (current.pos.absXZ(mPrevPos) < 100.0f) {
                    field_0x6e4 = sh_val + cLib_targetAngleY(&current.pos, &home.pos);
                } else {
                    s16 next_0x6e4 = sh_val + cLib_targetAngleY(&current.pos, &mPrevPos);
                    if (mAcch.ChkWallHit()) {
                        s16 wall_angle = mAcchCir.GetWallAngleY();
                        if (abs((s16)(next_0x6e4 - wall_angle)) > 0x6000) {
                            if ((s16)(cLib_targetAngleY(&current.pos, &mPrevPos) - wall_angle) > 0) {
                                next_0x6e4 = wall_angle + 0x4000;
                            } else {
                                next_0x6e4 = wall_angle - 0x4000;
                            }
                        }
                    }
                    field_0x6e4 = next_0x6e4;
                }
            }
            if (field_0x6a5 <= 1 && my_vec_0.abs() >= l_HIO.mMoveRange + 200.0f) {
                field_0x6f0 = 0;
            }
            cLib_addCalcAngleS(&shape_angle.y, field_0x6e4, 8, 0x1000, 0x100);
            if (field_0x6f0 == 0) {
                if (!field_0x6d4 && cM_rnd() < 0.3f) {
                    if (field_0x6a1 == 1) {
                        setActionMode(ACT_ELECTRIC);
                        return;
                    }
                    if (field_0x6a1) {
                        return;
                    }
                    if (daPy_getPlayerActorClass()->current.pos.abs(mPrevPos) <= l_HIO.mMoveRange + 200.0f) {
                        setActionMode(ACT_ATTACK);
                    }
                    return;
                }
                if (field_0x6c9 && mDistToPlayer < 300.0f) {
                    setActionMode(ACT_ELECTRIC);
                } else {
                    setActionMode(ACT_WAIT);
                    speedF = 0.0f;
                    field_0x6f6 = 5;
                }
            }
            break;
        case 2:
            field_0x6a5 = getSurpriseType();
            if (mAcch.ChkWallHit()) {
                field_0x6a5 = 2;
            }
            if (field_0x6a5 == 0) {
                shape_angle.y = mAngleToPlayer + cM_rndFX(4000.0f);
            } else if (field_0x6a5 == 1) {
                shape_angle.y = mAngleToPlayer + 0x8000 + cM_rndFX(4000.0f);
            } else {
                shape_angle.y = cLib_targetAngleY(&current.pos, &mPrevPos) + cM_rndFX(4000.0f);
            }
        case 3:
            mMode = 5;
            speedF = 0.0f;
            bckSet(10, 0, 10.0f, 2.0f);
            break;
        case 5:
            if (mpMorf->isStop()) {
                bckSet(0xb, 0, 5.0f, 1.0f);
                mSound.startCreatureSound(Z2SE_EN_YM_JUMP, 0, -1);
                mMode = 6;
                switch (field_0x6a5 & 3) {
                    case 0:
                        speedF = (mDistToPlayer + 650.0f) / 12.0f;
                        speed.y = 90.0f;
                        break;
                    case 1:
                        speedF = (650.0f - mDistToPlayer) / 10.0f;
                        speed.y = 75.0f;
                        break;
                    case 2:
                        speedF = my_vec_0.abs() / 10.0f;
                        speed.y = 75.0f;
                        break;
                    default:
                        break;
                }
                if (speedF < 35.0f) {
                    speedF = 35.0f;
                } else if (speedF >= 50.0f) {
                    speedF = 50.0f;
                }
                if (speed.y < 60.0f) {
                    speed.y = 60.0f;
                } else if (speed.y >= 90.0f) {
                    speed.y = 90.0f;
                }
            }
            break;
        case 6:
            if (mAcch.ChkGroundHit()) {
                mSound.startCreatureSound(Z2SE_EN_YM_LAND, 0, -1);
                bckSet(0xc, 0, 0.0f, 1.0f);
                mMode = 7;
                speedF = 0.0f;
            }
            break;
        case 7:
            if (mpMorf->isStop() && field_0x6f0 == 0) {
                setActionMode(ACT_WAIT);
                field_0x6f6 = 0xf;
                field_0x6a7 = 0;
            }
            break;
        default:
            break;
    }
    current.angle.y = shape_angle.y;
}

void daE_YM_c::executeBack() {
    if (mType != 1) {
        cLib_chaseF(&speed.y, -60.0f, 5.0f);
    }
    switch (mMode) {
        case 0:
            field_0x6f0 = 0x32;
            bckSet(0x10, 2, 5.0f, 1.0f);
            mMode = 1;
            speedF = -5.0f;
            break;
        case 1:
            cLib_chaseAngleS(&shape_angle.y, mAngleToPlayer, 0x400);
            current.angle.y = shape_angle.y;
            setMoveSound(0);
            if (field_0x6f0 == 0) {
                speedF = 0.0f;
                setActionMode(ACT_WAIT);
            }
            break;
        default:
            break;
    }
}

void daE_YM_c::executeFall() {
    cLib_chaseF(&speed.y, -90.0f, 10.0f);
    switch (mMode) {
        case 0:
            mSound.startCreatureSound(Z2SE_EN_YM_FALL, 0, -1);
            field_0x6f0 = 0xc;
            mMode = 1;
        case 1:
            if (field_0x6f0 == 0) {
                mType = 0;
                bckSet(0xb, 0, 10.0f, 2.0f);
                mSound.startCreatureSound(Z2SE_EN_YM_JUMP, 0, -1);
                speedF = 30.0f;
                speed.y = 50.0f;
                mMode = 2;
                mAcchCir.SetWall(40.0f, 60.0f);
                mAcch.SetGroundUpY(0.0f);
                shape_angle.x = field_0x668.x;
                shape_angle.y = field_0x668.z;
                current.angle.y = shape_angle.y + 0x8000;
                if ((s16)cLib_distanceAngleS(current.angle.y, mAngleToPlayer) < 0x1000) {
                    if ((s16)cLib_distanceAngleS(current.angle.y, mAngleToPlayer + 0x1000) < 0x1000) {
                        current.angle.y = shape_angle.y - 0x7000;
                    } else {
                        current.angle.y = shape_angle.y + 0x7000;
                    }
                }
                field_0x668.z = 0;
                field_0x668.x = 0;
                setElecEffect1();
                mSound.startCreatureSoundLevel(Z2SE_EN_YM_ELECTRIC, 0, -1);
            }
            break;
        case 2:
            field_0x6fe = 0x1e;
            cLib_chaseAngleS(&shape_angle.y, current.angle.y, 0x1000);
            cLib_chaseAngleS(&shape_angle.x, 0, 0x800);
            setElecEffect2();
            mSound.startCreatureSoundLevel(Z2SE_EN_YM_ELECTRIC_LOOP, 0, -1);
            field_0x700 = l_HIO.mElectricInvincibilityTimeExtension;
            mSphCc.OnAtSetBit();
            if (mAcch.ChkGroundHit() == 0) {
                break;
            }
            mSound.startCreatureSound(Z2SE_EN_YM_LAND, 0, -1);
            speedF = 0.0f;
            setActionMode(ACT_WAIT);
            bckSet(0xf, 2, 2.0f, 1.0f);
            shape_angle.y = current.angle.y;
            shape_angle.x = 0;
            mPrevPos = current.pos;
            break;
        default:
            break;
    }
}

void daE_YM_c::executeAttack() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz my_vec_0 = player->current.pos - mPrevPos;
    if (mType != 1) {
        cLib_chaseF(&speed.y, -100.0f, 4.0f);
    }
    if (my_vec_0.abs() <= l_HIO.mMoveRange + 200.0f) {
        field_0x688 = mDistToPlayer;
        field_0x6e4 = mAngleToPlayer;
    }
    switch (mMode) {
        case 0:
            bckSet(0xe, 2, 10.0f, 1.5f);
            speedF = 0.0f;
            field_0x6f0 = 0x50;
            mMode = 1;
            field_0x6a5 = 0;
            break;
        case 1:
            if (field_0x6d4 && (dComIfGp_getAttention()->LockonTruth() == 0 || dComIfGp_getAttention()->LockonTarget(0) != this)) {
                if (checkSurpriseNear()) {
                    return;
                }
            }
            setTurnSound();
            if (cLib_distanceAngleS(shape_angle.y, field_0x6e4) < 0x200) {
                if (checkBck("E_YM", 0xf) == 0) {
                    bckSet(0xf, 2, 5.0f, 1.0f);
                }
                field_0x6a5 += 1;
            } else {
                if (checkBck("E_YM", 0xe) == 0) {
                    bckSet(0xe, 2, 10.0f, 1.5f);
                }
                cLib_chaseAngleS(&shape_angle.y, field_0x6e4, 0x200);
                field_0x6a5 = 0;
            }
            if (field_0x6f0 == 0 || field_0x6a5 >= 0x1e || my_vec_0.abs() > l_HIO.mMoveRange + 200.0f) {
                bckSet(0xd, 0, 10.0f, 2.0f);
                mMode = 2;
            }
            break;
        case 2:
            setElecEffect1();
            mSound.startCreatureSoundLevel(Z2SE_EN_YM_ELECTRIC, 0, -1);
            if (mpMorf->isStop()) {
                bckSet(10, 0, 10.0f, 2.0f);
                mMode = 3;
            }
            break;
        case 3:
            setElecEffect2();
            mSound.startCreatureSoundLevel(Z2SE_EN_YM_ELECTRIC_LOOP, 0, -1);
            if (mpMorf->isStop()) {
                bckSet(0xb, 0, 5.0f, 1.0f);
                mSound.startCreatureSound(Z2SE_EN_YM_JUMP, 0, -1);
                speed.y = 35.0f;
                speedF = (field_0x688 * 4.0f) / 90.0f;
                mMode = 4;
            }
            break;
        case 4:
            setElecEffect2();
            mSound.startCreatureSoundLevel(Z2SE_EN_YM_ELECTRIC_LOOP, 0, -1);
            field_0x700 = l_HIO.mElectricInvincibilityTimeExtension;
            mSphCc.OnAtSetBit();
            if (mAcch.ChkGroundHit()) {
                mSound.startCreatureSound(Z2SE_EN_YM_LAND, 0, -1);
                bckSet(0xc, 0, 0.0f, 1.0f);
                speedF = 0.0f;
                mMode = 5;
            }
            break;
        case 5:
            if (mpMorf->isStop()) {
                setActionMode(ACT_WAIT);
                field_0x6f6 = 0xf;
                field_0x6fc = 0x32;
            }
            break;
        default:
            break;
    }
    current.angle.y = shape_angle.y;
}

u8 daE_YM_c::checkAttackEnd() {
    if (daPy_getPlayerActorClass()->checkClimbMove() == 0) {
        setActionMode(ACT_MOVE);
        speedF = 0.0f;
        return 1;
    } else {
        cXyz my_vec_0 = mPrevPos - current.pos;
        if (my_vec_0.abs() > l_HIO.mMoveRange + 50.0f) {
            setActionMode(ACT_WAIT);
            speedF = 0.0f;
            return 1;
        } else {
            return 0;
        }
    }
}

u8 daE_YM_c::setAttackMotion() {
    if (mType != 1) {
        return 0;
    }
    if (mSphCc.ChkAtHit()) {
        fopAc_ac_c* hit_actor = dCc_GetAc(mSphCc.GetAtHitObj()->GetAc());
        if (fopAcM_GetName(hit_actor) == PROC_ALINK) {
            if (mAction != ACT_ATTACK_WALL) {
                setActionMode(ACT_ATTACK_WALL);
            } else if (mMode == 4) {
                return 0;
            }
            speedF = 0.0f;
            mMode = 4;
            bckSet(5, 0, 3.0f, 1.0f);
            mSound.startCreatureSound(Z2SE_EN_YM_ATTACK, 0, -1);
            return 1;
        }
    }
    return 0;
}

void daE_YM_c::executeAttackWall() {
    field_0x6fa = 0x1e;
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz my_vec_0;
    cXyz my_vec_1;
    cXyz my_vec_2;
    if (checkWolfBark() == 0 && checkWallCrash() == 0 && setAttackMotion() == 0) {
        switch (mMode) {
            case 0:
                checkWall();
                if (field_0x6a5 == 0) {
                    my_vec_2 = player->current.pos;
                    my_vec_2.y += 100.0f;
                    my_vec_0 = my_vec_2 - current.pos;
                } else {
                    my_vec_0 = mPrevPos - current.pos;
                }
                mDoMtx_stack_c::ZrotS(-field_0x668.z);
                mDoMtx_stack_c::XrotM(-field_0x668.x);
                mDoMtx_stack_c::multVec(&my_vec_0, &my_vec_1);
                field_0x6e4 = cM_atan2s(my_vec_1.x, my_vec_1.z);
                if (checkBeforeBg(field_0x6e4)) {
                    setActionMode(ACT_MOVE);
                    speedF = 0.0f;
                    return;
                }
                bckSet(0xe, 2, 10.0f, 1.5f);
                speedF = 0.0f;
                mMode = 1;
                break;
            case 1:
                if (checkAttackEnd()) {
                    return;
                }
                setTurnSound();
                if (cLib_chaseAngleS(&shape_angle.y, field_0x6e4, 0x400)) {
                    bckSet(0xf, 2, 5.0f, 1.0f);
                    mMode = 2;
                }
                break;
            case 2:
                if (checkAttackEnd()) {
                    return;
                }
                mSound.startCreatureSoundLevel(Z2SE_EN_YM_WAIT, 0, -1);
                if (field_0x6f0 == 0) {
                    bckSet(0x10, 2, 5.0f, 1.0f);
                    mMode = 3;
                    speedF = 20.0f;
                    field_0x6f0 = cM_rndF(20.0f) + 30.0f;
                    mSound.startCreatureSound(Z2SE_EN_YM_FIND, 0, -1);
                }
                break;
            case 3: {
                setMoveSound(0);
                bool do_stuff = false;
                if (field_0x6f0 == 0 || checkBeforeBg(field_0x6e4)) {
                    do_stuff = true;
                } else {
                    my_vec_0 = mPrevPos - current.pos;
                    if (field_0x6a5 == 0) {
                        if (my_vec_0.abs() > l_HIO.mMoveRange + 100.0f) {
                            do_stuff = true;
                        }
                    } else if (my_vec_0.abs() < 30.0f) {
                        do_stuff = true;
                    }
                }
                if (do_stuff) {
                    speedF = 0.0f;
                    mMode = 4;
                    bckSet(5, 0, 3.0f, 1.0f);
                    mSound.startCreatureSound(Z2SE_EN_YM_ATTACK, 0, -1);
                }
                break;
            }
            case 4:
                if (mpMorf->isStop()) {
                    setActionMode(ACT_WAIT);
                    return;
                }
                break;
            default:
                break;
        }
        current.angle.y = shape_angle.y;
    }
}

void daE_YM_c::executeDefense() {
    switch (mMode) {
        case 0:
            mSound.startCreatureSound(Z2SE_EN_YM_DEFENCE, 0, -1);
            if (mType != 1) {
                bckSet(7, 2, 3.0f, 1.0f);
                mMode = 1;
                shape_angle.y = mAngleToPlayer;
                current.angle.y = mAngleToPlayer + 0x8000;
                if (mAcch.ChkGroundHit()) {
                    speedF = 15.0f;
                    field_0x6a5 = 1;
                } else {
                    speedF = 15.0f;
                    speed.y = 25.0f;
                    field_0x6a5 = 0;
                }
            } else {
                bckSet(7, 0, 3.0f, 1.0f);
                mMode = 2;
                speed.y = 0.0f;
                speedF = 0.0f;
            }
            break;
        case 1:
            cLib_chaseF(&speed.y, -60.0f, 5.0f);
            if (mAcch.ChkGroundHit()) {
                if (field_0x6a5 == 0) {
                    mSound.startCreatureSound(Z2SE_EN_YM_LAND, 0, -1);
                    field_0x6a5 = 1;
                }
                if (speedF >= 3.0f) {
                    fopAcM_effSmokeSet2(&field_0xad0, &field_0xad4, &current.pos, NULL, 1.5f, &tevStr);
                }
                speed.y = 0.0f;
                if (cLib_chaseF(&speedF, 0.0f, 1.0f)) {
                    setActionMode(ACT_WAIT);
                }
            }
            break;
        case 2:
            if (mpMorf->isStop()) {
                setActionMode(ACT_WAIT);
            }
            break;
        default:
            break;
    }
}

u8 daE_YM_c::checkFlyTerritory() {
    if (mPrevPos.absXZ(current.pos) > 300.0f) {
        return 0;
    } else {
        if (fabsf(mPrevPos.y - current.pos.y) > 150.0f) {
            return 0;
        }
        return 1;
    }
}

void daE_YM_c::initFly() {
    attention_info.distances[fopAc_attn_BATTLE_e] = 46;
    gravity = 0.0f;
    field_0x6e4 = 0;
    if (mTagPosP != NULL) {
        mPrevPos = mTagPos;
    } else {
        mPrevPos = home.pos;
    }
    mType = 3;
    shape_angle.x = 0;
}

void daE_YM_c::executeFly() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz player_pos(player->current.pos);
    cXyz my_vec_1 = current.pos - player_pos;
    cXyz my_vec_2 = current.pos - mPrevPos;
    cXyz my_vec_3 = player_pos - mPrevPos;
    mSound.startCreatureSoundLevel(Z2SE_EN_YM_FLY, 0, -1);
    switch (mMode) {
        case 0:
            field_0x6f2 = 100;
            bckSetFly(0xd, 0, 0.0f, 1.0f);
            field_0x6a5 = 0;
            mMode = 1;
        case 1:
            if (mType == 1) {
                setElecEffect1();
            }
            if (mpMorf->checkFrame(6.0f)) {
                field_0x6cc = 1;
            }
            if (mpMorf->checkFrame(14.0f)) {
                if (mType != 1) {
                    mMode = 7;
                    field_0x6f0 = 0x1e;
                    initFly();
                } else {
                    field_0x70e = 0x28;
                    mMode = 10;
                    field_0x6f0 = 0x5a;
                    speed.y = -10.0f;
                    initFly();
                    current.angle.y = cLib_targetAngleY(&current.pos, &mPrevPos);
                }
            }
            break;
        case 2:
            field_0x6f2 = 100;
            bckSetFly(0xe, 2, 5.0f, 1.0f);
            initFly();
            field_0x6a5 = 0;
            mMode = 3;
            break;
        case 3: {
            mAcch.SetGroundUpY(70.0f);
            if (field_0x6a5) {
                if (field_0x70c == 0 && cM_rnd() < 0.4f) {
                    field_0x70c = 0x5a;
                    field_0x67c = mPrevPos;
                    if (my_vec_3.absXZ() >= 400.0f) {
                        if (checkFlyTerritory()) {
                            field_0x67c.x += cM_rndFX(300.0f);
                            field_0x67c.z += cM_rndFX(300.0f);
                        }
                    } else {
                        s16 tgt_ang_y;
                        if (checkFlyTerritory()) {
                            tgt_ang_y = cLib_targetAngleY(&current.pos, &player_pos) + 0x8000;
                        } else {
                            tgt_ang_y = cLib_targetAngleY(&current.pos, &mPrevPos);
                        }
                        field_0x67c.x += cM_ssin(tgt_ang_y) * 300.0f;
                        field_0x67c.z += cM_scos(tgt_ang_y) * 300.0f;
                    }
                    field_0x67c.y = player_pos.y + cM_rndF(50.0f);
                    if (fabsf(field_0x67c.y - mPrevPos.y) > 300.0f) {
                        field_0x67c.y = mPrevPos.y + cM_rndF(75.0f);
                    }
                    current.angle.y = cLib_targetAngleY(&current.pos, &field_0x67c);
                    current.angle.x = cLib_targetAngleX(&current.pos, &field_0x67c);
                    speedF = l_HIO.mFlyMoveSpeed * cM_scos(current.angle.x);
                    speed.y = l_HIO.mFlyMoveSpeed * cM_ssin(current.angle.x);
                    field_0x6f0 = 0x14;
                    mMode = 6;
                    break;
                }
            } else {
                field_0x6a5 += 1;
            }
            if (my_vec_3.absXZ() >= 400.0f) {
                field_0x67c = mPrevPos;
                if (checkFlyTerritory()) {
                    field_0x67c.x += cM_rndFX(300.0f);
                    if (mPrevPos.y < current.pos.y) {
                        field_0x67c.y -= cM_rndF(75.0f);
                    } else {
                        field_0x67c.y += cM_rndF(75.0f);
                    }
                    field_0x67c.z += cM_rndFX(300.0f);
                }
                current.angle.y = cLib_targetAngleY(&current.pos, &field_0x67c);
                current.angle.x = cLib_targetAngleX(&current.pos, &field_0x67c);
            } else {
                if (checkFlyTerritory()) {
                    current.angle.y = cLib_targetAngleY(&current.pos, &player_pos) + 0x8000;
                    current.angle.x = cM_rndF(4096.0f);
                } else {
                    current.angle.y = cLib_targetAngleY(&current.pos, &mPrevPos);
                    current.angle.x = cLib_targetAngleX(&current.pos, &mPrevPos);
                    if ((s16)(cLib_targetAngleY(&current.pos, &player_pos) - current.angle.y) < 0) {
                        current.angle.y += 0x3000;
                    } else {
                        current.angle.y -= 0x3000;
                    }
                }
            }
            speed.y = l_HIO.mFlyMoveSpeed * cM_ssin(current.angle.x);
            field_0x6f0 = 0x14;
            mMode = 4;
        }
        case 4:
        case 6: {
            cLib_chaseF(&speedF, l_HIO.mFlyMoveSpeed * fabsf(cM_scos(current.angle.x)), 1.0f);
            if (mpMorf->isStop() && checkBck("E_TM", 0xd)) {
                bckSetFly(0xc, 2, 0.0f, 1.0f);
            }
            cLib_chaseAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &player_pos), 0x400);
            s16 ang_diff = current.angle.y - shape_angle.y;
            cLib_chaseAngleS(&shape_angle.z, cM_ssin(ang_diff) * -3072.0f * fabsf(cM_scos(current.angle.x)), 0x100);
            if (fabsf(cM_scos(current.angle.x)) >= fabsf(cM_ssin(current.angle.x))) {
                cLib_chaseAngleS(&shape_angle.x, cM_scos(ang_diff) * 5120.0f * fabsf(cM_scos(current.angle.x)), 0x400);
            } else {
                cLib_chaseAngleS(&shape_angle.x, fabsf(cM_ssin(current.angle.x)) * 5120.0f, 0x400);
            }
            int do_stuff = false;
            if (mMode == 4) {
                if (field_0x6f0 == 0) {
                    do_stuff = true;
                }
            } else if (fabsf(field_0x67c.y - current.pos.y) < 100.0f || mAcch.ChkGroundHit()) {
                do_stuff = true;
                speed.y = 0.0f;
            }
            if (do_stuff) {
                mMode = 5;
                field_0x6f0 = 0x1e;
                bckSetFly(0xe, 2, 5.0f, 1.0f);
            }
            break;
        }
        case 7: {
            if (mpMorf->isStop() && checkBck("E_TM", 0xd)) {
                bckSetFly(0xc, 2, 0.0f, 1.0f);
            }
            cLib_chaseF(&speedF, 20.0f, 3.0f);
            cLib_chaseF(&speed.y, 15.0f, 3.0f);
            cLib_addCalcAngleS(&shape_angle.y, mAngleToPlayer + 0x8000, 8, 0x1000, 0x100);
            current.angle.y = shape_angle.y;
            if (field_0x6f0 == 0) {
                mMode = 5;
                field_0x6f0 = 0x1e;
                speed.y = 0.0f;
                speedF = 0.0f;
                bckSetFly(0xe, 2, 5.0f, 1.0f);
            }
            break;
        }
        case 5: {
            cLib_chaseAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &player_pos), 0x800);
            cLib_chaseAngleS(&shape_angle.x, 0, 0x400);
            cLib_chaseAngleS(&shape_angle.z, 0, 0x400);
            cLib_chaseF(&speedF, 0.0f, 1.0f);
            cLib_chaseF(&speed.y, 0.0f, 3.0f);
            if (field_0x6f0 == 0 || my_vec_1.absXZ() < 50.0f
                || (daPy_getPlayerActorClass()->checkWolfBark() && my_vec_1.absXZ() < 100.0f)) {
                speed.y = 0.0f;
                speedF = 0.0f;
                mMode = 3;
                return;
            }
            if (field_0x6f2 == 0 && my_vec_1.absXZ() < 500.0f
                && my_vec_3.absXZ() < l_HIO.mMoveRange + 150.0f && fabsf(my_vec_2.y) < 150.0f) {
                speed.y = 0.0f;
                speedF = 0.0f;
                setActionMode(ACT_FLY_ATTACK);
                return;
            }
            break;
        }
        case 10: {
            if (field_0x70e) {
                setElecEffect2();
                mSound.startCreatureSoundLevel(Z2SE_EN_YM_ELECTRIC_LOOP, 0, -1);
                field_0x700 = l_HIO.mElectricInvincibilityTimeExtension;
                mSphCc.OnAtSetBit();
            }
            if (field_0x70e) {
                if (mpMorf->isStop() && checkBck("E_TM", 0xd)) {
                    bckSetFly(0xc, 2, 0.0f, 1.0f);
                }
                if (player_pos.y + 100.0f > current.pos.y) {
                    speed.y -= 1.0f;
                    if (speed.y < -15.0f) {
                        speed.y = -15.0f;
                    }
                } else {
                    cLib_chaseF(&speed.y, 0.0f, 1.0f);
                }
                cLib_chaseF(&speedF, 30.0f - fabsf(speed.y), 1.0f);
            } else {
                s16 tgt_ang = cLib_targetAngleY(&current.pos, &mPrevPos);
                if ((s16)(cLib_targetAngleY(&current.pos, &player_pos) - tgt_ang) < 0) {
                    tgt_ang += 0x3000;
                } else {
                    tgt_ang -= 0x3000;
                }
                cLib_chaseAngleS(&current.angle.y, tgt_ang, 0x400);
                current.angle.x = cLib_targetAngleX(&current.pos, &mPrevPos);
                speed.y = l_HIO.mFlyMoveSpeed * cM_ssin(current.angle.x);
                cLib_chaseF(&speedF, l_HIO.mFlyMoveSpeed * cM_scos(current.angle.x), 1.0f);
            }
            cLib_chaseAngleS(&shape_angle.y, current.angle.y, 0x400);
            if (field_0x6f0 == 0) {
                mMode = 3;
            }
        }
    }
    field_0x6e4 += 0x800;
    current.pos.y += cM_ssin(field_0x6e4) * 3.0f;
}

void daE_YM_c::setInclination() {
    s16 tan = cM_atan2s(speedF, speed.y) - 0x4000;
    if (tan < -6000) {
        tan = -6000;
    }
    if (tan > 6000) {
        tan = 6000;
    }
    cLib_chaseAngleS(&shape_angle.x, tan, 0x0180);
}

void daE_YM_c::executeFlyAttack() {
    cXyz my_vec_0;
    mSound.startCreatureSoundLevel(Z2SE_EN_YM_FLY, 0, -1);
    switch (mMode) {
        case 0:
            field_0x6e4 = 0;
            bckSetFly(5, 0, 0.0f, 1.0f);
            mMode = 1;
            field_0x67c = dComIfGp_getPlayer(0)->current.pos;
            field_0x67c.y += 50.0f;
            break;
        case 1:
            setElecEffect1();
            mSound.startCreatureSoundLevel(Z2SE_EN_YM_ELECTRIC, 0, -1);
            cLib_chaseAngleS(&shape_angle.x, 0, 0x400);
            cLib_chaseAngleS(&shape_angle.z, 0, 0x400);
            if (mpMorf->isStop()) {
                bckSetFly(6, 2, 0.0f, 1.0f);
                mMode = 2;
                field_0x6f0 = 300;
            }
            break;
        case 2: {
            setElecEffect2();
            mSound.startCreatureSoundLevel(Z2SE_EN_YM_ELECTRIC_LOOP, 0, -1);
            mSound.startCreatureSoundLevel(Z2SE_EN_YM_FLY_ATTACK, 0, -1);
            field_0x700 = l_HIO.mElectricInvincibilityTimeExtension;
            mSphCc.OnAtSetBit();
            my_vec_0 = field_0x67c - current.pos;
            cLib_chaseAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &field_0x67c), 0x800);
            current.angle.y = shape_angle.y;
            int tan_val = cM_atan2s(my_vec_0.absXZ(), my_vec_0.y);
            cLib_chaseF(&speed.y, l_HIO.mFlyAttackSpeed * cM_scos(tan_val), 5.0f);
            cLib_chaseF(&speedF, l_HIO.mFlyAttackSpeed * cM_ssin(tan_val), 3.0f);
            if (mSphCc.ChkAtHit()) {
                fopAc_ac_c* hit_actor = dCc_GetAc(mSphCc.GetAtHitObj()->GetAc());
                if (fopAcM_GetName(hit_actor) == PROC_ALINK) {
                    bckSetFly(7, 0, 0.0f, 1.0f);
                    speed.y = 0.0f;
                    speedF = 0.0f;
                    mMode = 4;
                    current.angle.y = cLib_targetAngleY(&current.pos, &mPrevPos);
                    shape_angle.x = 0;
                    return;
                }
            }
            if (my_vec_0.abs() < 100.0f || field_0x6f0 == 0) {
                mMode = 5;
                field_0x700 = l_HIO.mElectricInvincibilityTimeExtension;
                bckSetFly(0xe, 2, 5.0f, 1.0f);
                field_0x6f0 = 0x1e;
            }
            setInclination();
            break;
        }
        case 4:
        case 5: {
            field_0x6e4 += 0x800;
            current.pos.y += cM_ssin(field_0x6e4) * 3.0f;
            cLib_chaseF(&speed.y, 0.0f, 3.0f);
            cLib_chaseF(&speedF, 0.0f, 3.0f);
            int do_stuff = 0;
            if (mMode == 4) {
                if (mpMorf->isStop()) {
                    do_stuff = true;
                }
            } else if (field_0x6f0 == 0) {
                do_stuff = true;
            }
            if (do_stuff) {
                mMode = 3;
                bckSetFly(0xe, 2, 5.0f, 1.0f);
                speedF = 0.0f;
                speed.y = 0.0f;
                field_0x6f0 = cM_rndF(10.0f) + 30.0f;
            }
            setInclination();
            break;
        }
        case 3: {
            s16 mang = mAngleToPlayer;
            cLib_addCalcAngleS(&current.angle.y, mang + 0x8000, 4, 0x1000, 0x100);
            cLib_chaseAngleS(&shape_angle.y, mang, 0x400);
            cLib_chaseF(&speed.y, 10.0f, 3.0f);
            cLib_chaseF(&speedF, 10.0f, 3.0f);
            if (field_0x6f0 == 0) {
                setActionMode(ACT_FLY);
                mMode = 2;
            }
            s16 tan = cM_atan2s(speedF, speed.y) - 0x4000;
            if (tan < -6000) {
                tan = -6000;
            }
            if (tan > 6000) {
                tan = 6000;
            }
            cLib_chaseAngleS(&shape_angle.x, tan, 0x180);
            setInclination();
            break;
        }
        default:
            break;
    }
}

void daE_YM_c::setNextPathPoint() {
    cXyz my_vec_0;
    ++mCurrentPntNo;
    if (mCurrentPntNo >= mpPath->m_num) {
        mCurrentPntNo = 0;
    }
    my_vec_0 = dPath_GetPnt(mpPath, mCurrentPntNo)->m_position;
    shape_angle.y = current.angle.y = cLib_targetAngleY(&current.pos, &my_vec_0);
    mMode = 2;
    bckSet(0x10, 2, 5.0f, 1.0f);
}

u8 daE_YM_c::checkRailDig() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz my_vec_0;
    if (player->checkWolfDig()) {
        my_vec_0 = player->getLeftHandPos() - current.pos;
        if (my_vec_0.abs() < 200.0f) {
            field_0x714 &= ~0x80;
            field_0x6f0 = 0x14;
            mMode = 3;
            mAcchCir.SetWall(40.0f, 60.0f);
            fopAcM_OnStatus(this, 0x4000);
            return 1;
        }
    }
    return 0;
}

void daE_YM_c::executeRail() {
    daPy_py_c* player = daPy_getPlayerActorClass();;
    cXyz my_vec_0;
    switch (mMode) {
        case 0:
            mIsHide = 1;
            field_0x6a6 = 4;
            field_0x714 |= 4;
            field_0x6dc = -70.0f;
            old.pos = dPath_GetPnt(mpPath, mCurrentPntNo)->m_position;
            current.pos = old.pos;
            field_0x6f0 = 0x3c;
            field_0x714 |= 0x80;
            mSphCc.SetCoSPrm(0x145);
            mSphCc.SetTgType(0x10000);
            setNextPathPoint();
        case 1:
            if (checkRailDig() == 0) {
                mSound.startCreatureSoundLevel(Z2SE_EN_YM_WAIT, 0, -1);
                if (player->checkWolfDig() == 0 && field_0x6f0 == 0) {
                    setNextPathPoint();
                }
            }
            break;
        case 2:
            if (checkRailDig() == 0) {
                speedF = 9.0f;
                setMoveSound(1);
                my_vec_0 = dPath_GetPnt(mpPath, mCurrentPntNo)->m_position;
                if (cLib_chasePos(&current.pos, my_vec_0, speedF) || current.pos.absXZ(my_vec_0) < 10.0f) {
                    mMode = 1;
                    field_0x6f0 = 0x3c;
                    bckSet(0xf, 2, 5.0f, 1.0f);
                }
                speedF = 0.0f;
            }
            break;
        case 3:
            if (field_0x6f0 == 0) {
                setAppear();
                mPrevPos = current.pos;
                setActionMode(ACT_ESCAPE);
                field_0x704 = 0x1c2;
                field_0x70e = 0x5a;
                if (mTagPosP != NULL) {
                    mType = 0;
                }
            }
            break;
        default:
            break;
    }
}

static f32 YM_DIG_POS_Y[] = {-15.0f, -30.0f, -70.0f};

void daE_YM_c::executeBackRail() {
    cXyz my_vec_0 = dPath_GetPnt(mpPath, mCurrentPntNo)->m_position;
    switch (mMode) {
        case 0:
            if (current.pos.abs(my_vec_0) < 150.0f) {
                if ((s16)cLib_distanceAngleS(shape_angle.y, cLib_targetAngleY(&current.pos, &my_vec_0)) >= 0x2000) {
                    ++mCurrentPntNo;
                    if (mCurrentPntNo >= mpPath->m_num) {
                        mCurrentPntNo = 0;
                    }
                    my_vec_0 = dPath_GetPnt(mpPath, mCurrentPntNo)->m_position;
                }
            }
            bckSet(8, 2, 5.0f, 1.0f);
            mIsHide = 1;
            field_0x6a5 = 0;
            field_0x6f0 = 7;
            mMode = 1;
            mAcchCir.SetWall(40.0f, 60.0f);
        case 1:
            setDigEffect();
            mSound.startCreatureSoundLevel(Z2SE_EN_YM_DIG, 0, -1);
            cLib_chaseF(&field_0x6dc, YM_DIG_POS_Y[field_0x6a5], 3.0f);
            cLib_addCalcAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &my_vec_0), 8, 0x2000, 0x100);
            current.angle.y = shape_angle.y;
            cLib_chaseF(&speedF, 5.0f, 1.0f);
            if (field_0x6f0 == 0) {
                field_0x6f0 = 10;
                mMode = 2;
                speedF = 0.0f;
            }
            break;
        case 2:
            setDigEffect();
            mSound.startCreatureSoundLevel(Z2SE_EN_YM_DIG, 0, -1);
            if (field_0x6f0 == 0) {
                ++field_0x6a5;
                if (field_0x6a5 >= 3) {
                    mMode = 3;
                    field_0x6a5 = 0;
                    mSphCc.SetCoSPrm(0x145);
                    mSphCc.SetTgType(0x10000);
                    field_0x714 &= ~0x4;
                    bckSet(0x10, 2, 3.0f, 0.0f);
                } else {
                    if (field_0x6a5 == 2) {
                        field_0x6f0 = 0xf;
                        mSphCc.SetCoSPrm(0x145);
                        mSphCc.SetTgType(0x10000);
                        field_0x714 &= ~0x4;
                    } else {
                        field_0x6f0 = 7;
                    }
                    mMode = 1;
                }
            }
            break;
        case 3:
            cLib_chaseF(&field_0x6dc, -70.0f, 2.0f);
            setMoveSound(1);
            if (current.pos.absXZ(my_vec_0) < 100.0f) {
                cLib_chaseAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &my_vec_0), 0x1000);
            } else {
                cLib_addCalcAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &my_vec_0), 8, 0x2000, 0x100);
            }
            current.angle.y = shape_angle.y;
            cLib_chaseF(&speedF, 20.0f, 1.0f);
            if (mAcch.ChkWallHit()) {
                field_0x6a5 += 2;
                if (field_0x6a5 >= 0x14 && checkBeforeGround()) {
                    field_0x6a5 = 0;
                }
            } else if (field_0x6a5) {
                --field_0x6a5;
            }
            if (current.pos.absXZ(my_vec_0) < 30.0f && field_0x6dc == -70.0f) {
                speedF = 0.0f;
                mMode = 4;
                field_0x6f0 = 100;
                bckSet(0xf, 2, 5.0f, 1.0f);
            }
            break;
        case 4:
            if (field_0x6f0 == 0) {
                field_0x714 |= 4;
                setActionMode(ACT_RAIL);
            }
            break;
        default:
            break;
    }
}

void daE_YM_c::checkElectricStart() {
    if (field_0x6a1 == 1) {
        if (mAction != ACT_WAIT && mAction != ACT_SURPRISE) {
            field_0x6c9 = 0;
            field_0x708 = 0;
        } else if (field_0x708 != 0) {
            field_0x708--;

            if (field_0x708 == 0) {
                field_0x6c9 = 1;
            }
        }
    } else {
        field_0x6c9 = 0;
        field_0x708 = 0;
    }
}

void daE_YM_c::executeElectric() {
    switch (mMode) {
        case 0:
            field_0x6c9 = 0;
            bckSet(9, 0, 3.0f, 1.0f);
            speedF = 0.0f;
            mMode = 1;
            break;
        case 1:
            if (mpMorf->getFrame() < 36.0f) {
                setElecEffect1();
                mSound.startCreatureSoundLevel(Z2SE_EN_YM_ELECTRIC, 0, -1);
            } else {
                setElecEffect2();
                mSound.startCreatureSoundLevel(Z2SE_EN_YM_ELECTRIC_LOOP, 0, -1);
            }
            if (mpMorf->checkFrame(40.0f)) {
                mMode = 2;
            }
            break;
        case 2:
            setElecEffect2();
            mSound.startCreatureSoundLevel(Z2SE_EN_YM_ELECTRIC_LOOP, 0, -1);
            field_0x700 = l_HIO.mElectricInvincibilityTimeExtension;
            mSphCc.OnAtSetBit();
            if (mpMorf->isStop()) {
                mMode = 3;
                field_0x6f0 = 0x1e;
                bckSet(0xf, 2, 5.0f, 1.0f);
            }
            break;
        case 3:
            if (field_0x6f0 == 0) {
                setActionMode(ACT_WAIT);
            }
        default:
            break;
    }
}

void daE_YM_c::executeSwitch() {
    cXyz my_vec_0;
    switch (mMode) {
        case 0:
            mSphCc.SetCoSPrm(0x145);
            mSphCc.SetTgType(0);
            field_0x6dc = -70.0f;
            mIsHide = 1;
            if (mpPath != NULL) {
                if (mpPath->field_0x4 == 1) {
                    field_0x714 = 4;
                } else {
                    field_0x714 = 0;
                    field_0x6cf = 0;
                }
            } else {
                field_0x714 = 0;
            }
            if (dComIfGs_isSwitch(field_0x6a3, fopAcM_GetRoomNo(this))) {
                if (mpPath == NULL) {
                    mPrevPos = current.pos;
                    mMode = 1;
                    field_0x6f0 = 10;
                    field_0x6e4 = shape_angle.y;
                    field_0x6e6 = 0x5dc;
                    field_0x6e8 = 0;
                } else {
                    dPnt* pnt = dPath_GetPnt(mpPath, mCurrentPntNo);
                    mCurrentPntNo = 0;
                    ++mCurrentPntNo;
                    old.pos = pnt->m_position;
                    current.pos = old.pos;
                    mMode = 2;
                }
                if (field_0x6cb) {
                    speedF = 25.0f;
                } else {
                    speedF = 17.0f;
                }
                setAppear();
                setNormalCc();
                field_0x70e = 0x5a;
                bckSet(0x10, 2, 5.0f, 1.0f);
            }
            break;
        case 1:
            cLib_chaseF(&field_0x6dc, 0.0f, 15.0f);
            setMoveSound(0);
            if (mAcch.ChkWallHit()) {
                field_0x6e4 += 0x800;
            }
            if (field_0x6e8 >= 0) {
                field_0x6e6 -= 200;
            } else {
                field_0x6e6 += 200;
            }
            field_0x6e8 += field_0x6e6;
            cLib_addCalcAngleS(&shape_angle.y, field_0x6e4 + field_0x6e8, 4, 0x1000, 0x100);
            if (current.pos.absXZ(mPrevPos) > 500.0f) {
                speedF = 0.0f;
                mPrevPos = current.pos;
                field_0x6dc = 0.0f;
                setActionMode(ACT_WAIT);
            }
            break;
        case 2:
            mAcchCir.SetWall(0.0f, 0.0f);
            if (field_0x70e) {
                setElecEffect2();
                mSound.startCreatureSoundLevel(Z2SE_EN_YM_ELECTRIC_LOOP, 0, -1);
                field_0x700 = l_HIO.mElectricInvincibilityTimeExtension;
                mSphCc.OnAtSetBit();
            }
            cLib_chaseF(&field_0x6dc, 0.0f, 3.0f);
            setMoveSound(0);
            my_vec_0 = dPath_GetPnt(mpPath, mCurrentPntNo)->m_position;
            cLib_addCalcAngleS(&shape_angle.y, cLib_targetAngleY(&current.pos, &my_vec_0),
                               4, 0x2000, 0x100);
            if (current.pos.absXZ(my_vec_0) < 50.0f) {
                ++mCurrentPntNo;
                if (mCurrentPntNo >= mpPath->m_num) {
                    if (mpPath->field_0x4 == 2) {
                        field_0x71d = 1;
                        field_0x714 = 0;
                    } else {
                        speedF = 0.0f;
                        mPrevPos = current.pos;
                        field_0x6dc = 0.0f;
                        mAcchCir.SetWall(40.0f, 60.0f);
                        setActionMode(ACT_WAIT);
                    }
                }
            }
            break;
        case 10:
            setAppear();
            setNormalCc();
            old.pos = dPath_GetPnt(mpPath, mpPath->m_num-1)->m_position;
            current.pos = old.pos;
            mPrevPos = current.pos;
            field_0x6dc = 0.0f;
            speedF = 0.0f;
            mAcchCir.SetWall(40.0f, 60.0f);
            setActionMode(ACT_WAIT);
            break;
        default:
            break;
    }
    current.angle.y = shape_angle.y;
}

void daE_YM_c::initFireFly(int param_0) {
    field_0x6e6 = cM_rndFX(512.0f);
    if (param_0 && mAcch.ChkWallHit()) {
        s16 wall_ang = mAcchCir.GetWallAngleY();
        s16 res = (wall_ang << 1) - (current.angle.y + 0x8000);
        current.angle.y = res + cM_rndFX(4096.0);
        speed.y += cM_rndFX(15.0f);
        field_0x70e = cM_rndFX(5.0f) + 10.0f;
    }
    field_0x6f0 = 10;
}

void daE_YM_c::executeFire() {
    if (current.pos.y >= 550.0f) {
        current.pos.y = 550.0f;
        mAcch.SetRoofHit();
    }
    switch (mMode) {
        case 0:
            if (field_0x6f0) {
                setElecEffect2();
                mSound.startCreatureSoundLevel(Z2SE_EN_YM_ELECTRIC_LOOP, 0, -1);
            } else if (daPy_getPlayerActorClass()->checkFrontRollCrash() || daPy_getPlayerActorClass()->checkWolfAttackReverse()) {
                cXyz pos_diff = daPy_getPlayerActorClass()->current.pos - current.pos;
                if (pos_diff.abs() < 250.0f) {
                    field_0x6f0 = 10;
                }
            }
            if (field_0x6d4) {
                field_0x714 = 4;
            } else {
                field_0x714 = 0;
            }
            mSphCc.SetCoSPrm(0x145);
            mSphCc.SetTgType(0);
            field_0x6dc = -70.0f;
            mIsHide = 1;
            field_0x71c = 1;
            current.pos = home.pos;
            if (field_0x6a3 == 0xff || dComIfGs_isSwitch(field_0x6a3, fopAcM_GetRoomNo(this))) {
                field_0x71c = 0;
                mPrevPos = current.pos;
                field_0x6e4 = 0;
                setAppear();
                setNormalCc();
                mSphCc.SetTgType(0);
                field_0x6dc = 0.0f;
                mMode = 1;
                bckSetFly(0xd, 0, 5.0f, 1.0f);
                field_0x6a5 = 0;
            }
            break;
        case 1:
            if (mpMorf->checkFrame(6.0f)) {
                field_0x6cc = 1;
            }
            if (mpMorf->checkFrame(14.0f)) {
                attention_info.distances[2] = 0x2e;
                gravity = 0.0f;
                fopAcM_SearchByName(0x1f7, &field_0x660);
                if (field_0x660 != NULL) {
                    for (int idx = 0; idx < 20; ++idx) {
                        if (((daTag_FWall_c*)field_0x660)->getTagNo(idx) == getTagNo()) {
                            field_0x67c = ((daTag_FWall_c*)field_0x660)->getPos(idx);
                            break;
                        }
                    }
                    mMode = 2;
                } else {
                    initFireFly(0);
                    mMode = 3;
                }
            }
        case 2:
            if (fopAcM_CheckCondition(this, 4) == 0) {
                setFireEffect();
                setElecEffect2();
            }
            mSound.startCreatureSoundLevel(Z2SE_EN_YM_ELECTRIC_LOOP, 0, -1);
            field_0x700 = l_HIO.mElectricInvincibilityTimeExtension;
            mSphCc.OnAtSetBit();
            current.angle.y = cLib_targetAngleY(&current.pos, &field_0x67c);
            current.angle.x = cLib_targetAngleX(&current.pos, &field_0x67c);
            speedF = cM_scos(current.angle.x) * 40.0f;
            speed.y = cM_ssin(current.angle.x) * 40.0f;
            if (field_0x67c.abs(current.pos) < 200.0f && mAcch.ChkWallHit()) {
                mMode = 3;
                initFireFly(1);
                for (int idx = 0; idx < 20; ++idx) {
                    if (((daTag_FWall_c*)field_0x660)->getTagNo(idx) == getTagNo()) {
                        ((daTag_FWall_c*)field_0x660)->setFire(idx);
                    }
                }
            }
            shape_angle.y = current.angle.y;
            break;
        case 3:
            if (dComIfGp_event_runCheck() == 0) {
                mSphCc.OnCoSetBit();
            } else {
                mSphCc.OffCoSetBit();
            }
            if (field_0x6a5 == 0 && mpMorf->isStop()) {
                bckSetFly(0xc, 2, 0.0f, 1.0f);
                ++field_0x6a5;
            }
            if (fopAcM_CheckCondition(this, 4) == 0) {
                setFireEffect();
            }
            if (field_0x70e) {
                if (fopAcM_CheckCondition(this, 4) == 0) {
                    setElecEffect2();
                }
                mSound.startCreatureSoundLevel(Z2SE_EN_YM_ELECTRIC_LOOP, 0, -1);
                field_0x700 = l_HIO.mElectricInvincibilityTimeExtension;
                mSphCc.OnAtSetBit();
            }
            if (current.pos.y > 400.0f) {
                speed.y -= 1.0f;
                if (speed.y < -15.0f) {
                    speed.y = -15.0f;
                }
            } else if (current.pos.y < 200.0f) {
                if (speed.y < 0.0f) {
                    speed.y = 0.0f;
                }
            } else {
                speed.y += 1.0f;
                if (speed.y > 15.0f) {
                    speed.y = 15.0f;
                }
            }
            speedF = 40.0f - fabsf(speed.y);
            current.angle.y += field_0x6e6;
            if (mAcch.ChkWallHit()) {
                initFireFly(1);
            } else if (field_0x6f0 == 0) {
                initFireFly(0);
            }
            shape_angle.y = current.angle.y;
            break;
        default:
            break;
    }
    field_0x6e4 += 0x2000;
    current.pos.y += cM_ssin(field_0x6e4) * 3.0f;
    if (mMode) {
        mSound.startCreatureSoundLevel(Z2SE_EN_YM_FLY, 0, -1);
    }
}

void daE_YM_c::setRiverAttention() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz player_pos = player->current.pos;
    attention_info.distances[2] = 60;
    field_0x714 = 0;
    if (current.pos.abs(mpKago->current.pos) > 2000.0f) {
        return;
    }
    cXyz my_vec_0;
    cXyz my_vec_1;
    mDoMtx_stack_c::YrotS(-shape_angle.y);
    mDoMtx_stack_c::XrotM(-shape_angle.x);
    mDoMtx_stack_c::transM(current.pos - mpKago->current.pos);
    mDoMtx_stack_c::multVecZero(&my_vec_0);
    my_vec_1.set(my_vec_0.x, my_vec_0.z, my_vec_0.y);
    if (abs(cM_atan2s(my_vec_1.y, my_vec_1.absXZ()) - 0x4000) < 0x2000) {
        field_0x714 = 4;
    }
}

void daE_YM_c::setLockByCargo() {
    field_0x6f2 = 90;
}

void daE_YM_c::executeRiver() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    cXyz player_pos = player->current.pos;
    cXyz pnt_pos = dPath_GetPnt(mpPath, mCurrentPntNo)->m_position;
    cXyz my_vec_0;
    switch (mMode) {
        case 0:
            field_0x714 = 0;
            field_0x6d4 = 0.0f;
            field_0x6dc = 0.0f;
            gravity = 0.0f;
            speedF = 0.0f;
            speed.y = 0.0f;
            field_0x6ec = 0.0f;
            field_0x6f2 = 0;
            if (mpKago == NULL) {
                fopAcM_SearchByName(0xf4, (fopAc_ac_c**)&mpKago);
            } else if ( mpKago->getPathDir() ) {
                if (dComIfGs_isLightDropGetFlag(dComIfGp_getStartStageDarkArea()) == 0) {
                    field_0x71d = 1;
                } else {
                    if (mpKago->getPathDir() > 0) {
                        if (mpPath != NULL) {
                            mMode = 1;
                        } else {
                            field_0x71d = 1;
                            break;
                        }
                    } else {
                        if (field_0x6b0 != NULL) {
                            mMode = 1;
                            mpPath = field_0x6b0;
                        } else {
                            field_0x71d = 1;
                            break;
                        }
                    }
                    if (mMode == 1) {
                        bckSetFly(0xe, 2, 5.0f, 1.0f);
                        field_0x6cc = 1;
                        mCurrentPntNo = 0;
                        pnt_pos = dPath_GetPnt(mpPath, mCurrentPntNo)->m_position;
                        old.pos = pnt_pos;
                        current.pos = old.pos;
                        ++mCurrentPntNo;
                        pnt_pos = dPath_GetPnt(mpPath, mCurrentPntNo)->m_position;
                        s16 tgt_ang = cLib_targetAngleY(&current.pos, &pnt_pos);
                        current.angle.y = tgt_ang;
                        shape_angle.y = tgt_ang;
                        setAppear();
                        setNormalCc();
                        field_0x6f0 = 0;
                    }
                }
            }
            break;
        case 1:
            setElecEffect1();
            mSound.startCreatureSoundLevel(Z2SE_EN_YM_FLY, 0, -1);
            if (mpKago->isFlying()) {
                if (field_0x6a3 != 0xff) {
                    if (dComIfGs_isSwitch(field_0x6a3, fopAcM_GetRoomNo(this))) {
                        speedF = 0.0f;
                        speed.y = 0.0f;
                        mMode = 2;
                        bckSetFly(0xb, 2, 5.0f, 1.0f);
                    }
                } else {
                    f32 next_path = mpKago->checkNextPath(current.pos);
                    if (next_path < 2000.0f) {
                        speedF = 0.0f;
                        speed.y = 0.0f;
                        mMode = 2;
                        bckSetFly(0xb, 2, 5.0f, 1.0f);
                    }
                }
            }
            break;
        case 2: {
            setRiverAttention();
            field_0x6e8 += 0x200;
            current.pos.y += cM_ssin(field_0x6e8 << 1) * 15.0f;
            f32 my_float_val = cM_scos(field_0x6e8) * 15.0f;
            current.pos.x += my_float_val * cM_ssin(shape_angle.y);
            current.pos.z += my_float_val * cM_scos(shape_angle.y);
            s16 tgt_ang = cLib_targetAngleY(&current.pos, &pnt_pos);
            current.angle.y = tgt_ang;
            shape_angle.y = tgt_ang;
            tgt_ang = cLib_targetAngleX(&current.pos, &pnt_pos);
            current.angle.x = tgt_ang;
            shape_angle.x = tgt_ang;
            // The following is necessary for matching, but unused?!
            abs(mAngleToPlayer - shape_angle.y);
            f32 next_path = mpKago->checkNextPath(current.pos);
            if (mpKago->isFlying()) {
                if (field_0x6f2) {
                    field_0x714 = 4;
                    if (next_path < 200.0f) {
                        cLib_chaseF(&field_0x6ec, 70.0f, 1.0f);
                    } else if (next_path > 1000.0f) {
                        cLib_chaseF(&field_0x6ec, 10.0f, 1.0f);
                    } else {
                        cLib_chaseF(&field_0x6ec, 30.0f, 1.0f);
                    }
                } else {
                    if (next_path < 0.0f) {
                        field_0x6f0 = 0x96;
                        cLib_chaseF(&field_0x6ec, 70.0f, 1.0f);
                    } else if (field_0x6f0) {
                        cLib_chaseF(&field_0x6ec, 70.0f, 1.0f);
                    } else {
                        if (next_path < 4000.0f) {
                            if (next_path < 1500.0f) {
                                f32 abs_val = mpKago->speed.abs();
                                abs_val -= 10.0f;
                                if (abs_val < 30.0f) {
                                    abs_val = 30.0f;
                                }
                                cLib_chaseF(&field_0x6ec, abs_val, 1.0f);
                            } else {
                                cLib_chaseF(&field_0x6ec, 30.0f, 1.0f);
                            }
                        } else {
                            cLib_chaseF(&field_0x6ec, 0.0f, 1.0f);
                        }
                    }
                }
            } else {
                cLib_chaseF(&field_0x6ec, 0.0f, 1.0f);
                field_0x6d4 = 0.0f;
            }
            speed.y = field_0x6ec * cM_ssin(current.angle.x);
            speedF = field_0x6ec * cM_scos(current.angle.x);
            if (pnt_pos.abs(current.pos) < 500.0f && ((++mCurrentPntNo), mCurrentPntNo >= mpPath->m_num)) {
                speedF = 0.0f;
                mPrevPos = pnt_pos;
                field_0x6dc = 0.0f;
                mAcchCir.SetWall(40.0f, 60.0f);
                current.angle.x = 0;
                shape_angle.x = 0;
                setActionMode(ACT_WAIT);
            } else {
                if (current.pos.abs(mpKago->current.pos) > 2000.0f) {
                    field_0x68c = 2.4f;
                    setElecEffect2();
                    field_0x68c = 1.5f;
                }
                mSound.startCreatureSoundLevel(Z2SE_EN_YM_FLY, 0, -1);
                if (mSphCc.ChkTgHit()) {
                    cCcD_Obj * tg_hit_obj = mSphCc.GetTgHitObj();
                    fopAc_ac_c* hit_actor = dCc_GetAc(tg_hit_obj->GetAc());
                    if (fopAcM_GetName(hit_actor) == PROC_KAGO) {
                        if (mpKago == tg_hit_obj->GetAc()) {
                            if (mpKago->getLockActor() == this) {
                                mpKago->setLockActor(NULL);
                            }
                            setAppear();
                            speedF = 0.0f;
                            speed.y = 0.0f;
                            field_0x714 = 0;
                            bckSet(0x10, 2, 5.0f, 1.0f);
                            mpKago->setEatYm();
                            mMode = 3;
                            field_0x6ce = 1;
                        }
                    }
                }
            }
            break;
        }
        case 3:
            setElecEffect1();
            if (mpKago != NULL) {
                mDoMtx_stack_c::copy(mpKago->getMouthMtx());
                mDoMtx_stack_c::multVecZero(&my_vec_0);
                current.pos += mpKago->speed;
                current.pos = my_vec_0;
                if (cLib_chasePos(&current.pos, my_vec_0, 50.0f)) {
                    if (mSwitchBit != 0xff && dComIfGs_isSwitch(mSwitchBit, fopAcM_GetRoomNo(this)) == 0) {
                        dComIfGs_onSwitch(mSwitchBit, fopAcM_GetRoomNo(this));
                    }
                    dComIfGp_setHitMark(3, this, &current.pos, NULL, NULL, 0);
                    fopAcM_delete(this);
                    fopAcM_createDisappear(this, &current.pos, 0x14, 1, 0xff);
                }
            }
            break;
        default:
            break;
    }
}

static void* s_ym_sub(void* param_0, void* param_1) {
    daE_YM_c* actor_a = (daE_YM_c*)param_0;
    daE_YM_c* actor_b = (daE_YM_c*)param_1;

    if (fopAcM_IsActor(actor_a) && fopAcM_GetName(actor_a) == PROC_E_YM) {
        if (!fpcM_IsCreating(fopAcM_GetID(actor_a)) && actor_a != actor_b &&
            actor_a->current.pos == actor_b->current.pos)
        {
            if (actor_b->getFriendFlag() & 1) {
                actor_a->setFriendFlag(0x84);
            } else {
                actor_a->setFriendFlag(0x82);
            }

            actor_b->setFriendFlag(0x81);
        }
    }

    return 0;
}

void daE_YM_c::checkFrinedSamePos() {
    if (mFriendFlag == 0) {
        setFriendFlag(0x80);
        fpcM_Search(s_ym_sub, this);
    }
}

void daE_YM_c::action() {
    if (daPy_getPlayerActorClass()->getCutType() == 0x36) {
        field_0x700 = 0;
    }
    mDistToPlayer = fopAcM_searchPlayerDistance(this);
    mAngleToPlayer = fopAcM_searchPlayerAngleY(this);
    cXyz my_vec_0;
    cXyz my_vec_1;
    mSphCc.OnTgSetBit();
    if (field_0x6fe) {
        mSphCc.OffTgSetBit();
    }
    if (!field_0x6d4) {
        mSphCc.OffTgSetBit();
    }
    mSphCc.SetTgSpl(0);
    mSphCc.SetAtMtrl(0);
    if (field_0x700) {
        mSphCc.SetAtMtrl(7);
    }
    if (mType == 1) {
        mSphCc.OnAtSetBit();
    } else {
        mSphCc.OffAtSetBit();
    }
    damage_check();
    checkSurpriseLock();
    field_0x6cf = 1;
    int do_stuff = false;
    switch (mAction) {
        case ACT_WAIT:
            executeWait();
            break;
        case ACT_MOVE:
            executeMove();
            break;
        case ACT_ESCAPE:
            executeEscape();
            break;
        case ACT_DOWN:
            executeDown();
            break;
        case ACT_WIND:
            executeWind();
            break;
        case ACT_SURPRISE:
            executeSurprise();
            break;
        case ACT_BACK:
            executeBack();
            break;
        case ACT_FALL:
            executeFall();
            break;
        case ACT_ATTACK:
            do_stuff = true;
            executeAttack();
            break;
        case ACT_ATTACK_WALL:
            do_stuff = true;
            executeAttackWall();
            break;
        case ACT_DEFENSE:
            executeDefense();
            break;
        case ACT_FLY:
            executeFly();
            break;
        case ACT_FLY_ATTACK:
            do_stuff = true;
            executeFlyAttack();
            break;
        case ACT_RAIL:
            executeRail();
            break;
        case ACT_BACK_RAIL:
            executeBackRail();
            break;
        case ACT_ELECTRIC:
            do_stuff = true;
            executeElectric();
            break;
        case ACT_SWITCH:
            executeSwitch();
            break;
        case ACT_FIRE:
            executeFire();
            break;
        case ACT_RIVER:
            executeRiver();
            break;
        default:
            break;
    }
    mSound.setLinkSearch(do_stuff);
    if (field_0x6cf) {
        mSound.startCreatureSoundLevel(Z2SE_EN_YM_NEAR, 0, -1);
    }
    if (mType != 1) {
        if (mAcch.ChkGroundHit()) {
            cM3dGPla plane;
            dComIfG_Bgsp().GetTriPla(mAcch.m_gnd, &plane);
            cXyz* p_vec = plane.GetNP();
            cLib_addCalcAngleS(&field_0x668.x, (s16)cM_atan2s(p_vec->z, p_vec->y), 8, 0x800, 0x100);
            cLib_addCalcAngleS(&field_0x668.y, (s16)cM_atan2s(p_vec->x, p_vec->y), 8, 0x800, 0x100);
        } else {
            field_0x668.set(0, 0, 0);
        }
        fopAcM_posMoveF(this, mStts.GetCCMoveP());
        if (field_0x71c == 0) {
            mAcch.CrrPos(dComIfG_Bgsp());
        }
    } else {
        mDoMtx_stack_c::XrotS(field_0x668.x);
        mDoMtx_stack_c::ZrotM(field_0x668.z);
        mDoMtx_stack_c::YrotM(current.angle.y);
        my_vec_0.set(0.0f, 0.0f, speedF);
        mDoMtx_stack_c::multVec(&my_vec_0, &my_vec_1);
        speed = my_vec_1;
        current.pos += speed;
    }
    if (!field_0x6d4) {
        attention_info.flags = 0;
        if (daPy_getPlayerActorClass()->checkWolfLock(this)) {
            daPy_getPlayerActorClass()->cancelWolfLock(this);
        }
        onWolfNoLock();
    } else {
        attention_info.flags = field_0x714;
        offWolfNoLock();
        if (mType == 1 && fopAcM_CheckCondition(this, 4) == 0) {
            dBgS_LinChk lin_chk;
            lin_chk.Set(&dComIfGp_getCamera(0)->lookat.eye, &attention_info.position, this);
            if (dComIfG_Bgsp().LineCross(&lin_chk)) {
                onWolfNoLock();
            }
        }
    }
    cXyz unused_vec(field_0x68c, field_0x68c, field_0x68c);
    cXyz my_pos = current.pos;
    setMidnaBindEffect(this, &mSound, &my_pos, &scale);
    mpMorf->play(NULL, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    mpBrk->play();
}

void daE_YM_c::mtx_set() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y + field_0x6dc, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(field_0x668);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(field_0x68c, field_0x68c, field_0x68c);

    mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->modelCalc();
}

void daE_YM_c::cc_set() {
    cXyz my_vec_0;
    cXyz my_vec_1;
    J3DModel* model = mpMorf->getModel();
    cXyz my_vec_2(0.0f, 0.0f, 0.0f);
    if (mIsHide) {
        my_vec_2.y = 40.0f;
    }
    if (field_0x718 == 0) {
        if (mType != 1) {
            MTXCopy(model->getAnmMtx(0), mDoMtx_stack_c::get());
            my_vec_0.set(30.0f, 0.0f, 0.0f);
            mDoMtx_stack_c::multVec(&my_vec_0, &eyePos);
            attention_info.position = eyePos;
            attention_info.position.y += field_0x68c * 50.0f;
        } else {
            mDoMtx_stack_c::XrotS(field_0x668.x);
            mDoMtx_stack_c::ZrotM(field_0x668.z);
            my_vec_0.set(0.0f, -30.0f, 50.0f);
            mDoMtx_stack_c::multVec(&my_vec_0, &my_vec_1);
            eyePos = current.pos + my_vec_1;
            mDoMtx_stack_c::XrotS(field_0x668.x);
            mDoMtx_stack_c::ZrotM(field_0x668.z);
            my_vec_0.set(0.0f, 30.0f, 70.0f);
            mDoMtx_stack_c::multVec(&my_vec_0, &my_vec_1);
            attention_info.position = current.pos + my_vec_1;
        }
    } else if (field_0x718 == 1) {
        eyePos = current.pos;
        attention_info.position = eyePos;
        attention_info.position.y += 70.0f;
    }
    MTXCopy(model->getAnmMtx(0), mDoMtx_stack_c::get());
    if (mType != 1) {
        my_vec_0.set(10.0f, -20.0f, 0.0f);
    } else {
        my_vec_0.set(10.0f, 40.0f, 0.0f);
    }
    mDoMtx_stack_c::multVec(&my_vec_0, &my_vec_1);
    mSphCc.SetC(my_vec_1 + my_vec_2);
    if (mAction == ACT_RIVER) {
        mSphCc.SetR(100.0f);
    } else if (field_0x700) {
        mSphCc.SetR(40.0f);
    } else {
        mSphCc.SetR(30.0f);
    }
    dComIfG_Ccsp()->Set(&mSphCc);
}

int daE_YM_c::execute() {
    if (field_0x71d) {
        return 1;
    }

    if (field_0x6f0 != 0) {
        field_0x6f0--;
    }

    if (field_0x6f2 != 0) {
        field_0x6f2--;
    }

    if (field_0x6f4 != 0) {
        field_0x6f4--;
    }

    if (field_0x6f6 != 0) {
        field_0x6f6--;
    }

    if (field_0x6f8 != 0) {
        field_0x6f8--;
    }

    if (field_0x6fc != 0) {
        field_0x6fc--;
    }

    if (field_0x6fe != 0) {
        field_0x6fe--;
    }

    if (field_0x700 != 0) {
        field_0x700--;
    }

    if (field_0x6fa != 0) {
        field_0x6fa--;
    }

    if (field_0x704 != 0) {
        field_0x704--;
    }

    if (field_0x70a != 0) {
        field_0x70a--;
    }

    if (field_0x70c != 0) {
        field_0x70c--;
    }

    if (field_0x70e != 0) {
        field_0x70e--;
    }

    checkElectricStart();
    action();
    mtx_set();
    cc_set();
    return 1;
}

static int daE_YM_Execute(daE_YM_c* i_this) {
    return i_this->execute();
}

static int daE_YM_IsDelete(daE_YM_c* i_this) {
    return 1;
}

int daE_YM_c::_delete() {
    if (mFlyType == true) {
        dComIfG_resDelete(&mPhase, "E_TM");
    } else {
        dComIfG_resDelete(&mPhase, "E_YM");
    }

    if (field_0xaf4) {
        lbl_253_bss_8 = 0;
    }

    if (heap != NULL) {
        mSound.deleteObject();
    }

    return 1;
}

static int daE_YM_Delete(daE_YM_c* i_this) {
    return i_this->_delete();
}

int daE_YM_c::CreateHeap() {
    if (mFlyType == 1) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_TM", 0x11);
        JUT_ASSERT(0x1094, modelData != NULL);
        mpMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_TM", 10),
                                      0, 1.0f, 0, -1, &mSound, 0x80000,0x11000084);
        if (mpMorf == NULL || mpMorf->getModel() == NULL) {
            return 0;
        }
        mpBrk = new mDoExt_brkAnm();
        if (mpBrk == NULL) {
            return 0;
        }
        if (!mpBrk->init(mpMorf->getModel()->getModelData(),
                         (J3DAnmTevRegKey*)dComIfG_getObjectRes("E_TM", 0x14),
                         TRUE, 2, 1.0f, 0, -1)) {
            return 0;
        }
    } else {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_YM", 0x13);
        JUT_ASSERT(0x10bb, modelData != NULL);
        mpMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("E_YM", 0x10),
                                      0, 1.0f, 0, -1, &mSound, 0x80000,0x11000084);
        if (mpMorf == NULL || mpMorf->getModel() == NULL) {
            return 0;
        }
        mpBrk = new mDoExt_brkAnm();
        if (mpBrk == NULL) {
            return 0;
        }
        if (!mpBrk->init(mpMorf->getModel()->getModelData(),
                         (J3DAnmTevRegKey*)dComIfG_getObjectRes("E_YM", 0x16),
                         TRUE, 2, 1.0f, 0, -1)) {
            return 0;
        }
    }
    if (mType == 6) {
        field_0x6d0 = new cXyz[45];
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return static_cast<daE_YM_c*>(i_this)->CreateHeap();
}

u8 daE_YM_c::checkBeforeBg(s16 i_rot_val) {
    dBgS_LinChk lin_chk;
    cXyz my_vec_0;
    cXyz my_vec_1;
    cXyz my_vec_2;
    cXyz my_vec_3;
    if (!speedF) {
        return 0;
    }
    if (mType != 1) {
        if (mAcch.ChkWallHit()) {
            return 1;
        }
    } else {
        mDoMtx_stack_c::XrotS(field_0x668.x);
        mDoMtx_stack_c::ZrotM(field_0x668.z);
        mDoMtx_stack_c::YrotM(i_rot_val);
        my_vec_0.set(0.0f, 30.0f, 0.0f);
        mDoMtx_stack_c::multVec(&my_vec_0, &my_vec_3);
        my_vec_3 += current.pos;
        mDoMtx_stack_c::XrotS(field_0x668.x);
        mDoMtx_stack_c::ZrotM(field_0x668.z);
        mDoMtx_stack_c::YrotM(i_rot_val);
        my_vec_0.set(0.0f, 0.0f, 60.0f);
        mDoMtx_stack_c::multVec(&my_vec_0, &my_vec_1);
        my_vec_0.set(my_vec_1.x, 0.0f, my_vec_1.z);
        my_vec_2 = my_vec_3 + my_vec_0;
        lin_chk.Set(&my_vec_3, &my_vec_2, NULL);
        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            return 1;
        }
        if (my_vec_1.y > 0.0f) {
            my_vec_0.set(0.0f, 60.0f, 0.0f);
        } else {
            my_vec_0.set(0.0f, -200.0f, 0.0f);
        }
        my_vec_2 = my_vec_3 + my_vec_0;
        lin_chk.Set(&my_vec_3, &my_vec_2, NULL);
        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            return 1;
        }
        mDoMtx_stack_c::XrotS(field_0x668.x);
        mDoMtx_stack_c::ZrotM(field_0x668.z);
        mDoMtx_stack_c::YrotM(i_rot_val);
        my_vec_0.set(0.0f, 30.0f, 80.0f);
        mDoMtx_stack_c::multVec(&my_vec_0, &my_vec_1);
        my_vec_3 = current.pos + my_vec_1;
        cXyz inside_vec(0.0f, 0.0f, 80.0f);
        cLib_offsetPos(&my_vec_2, &my_vec_3, (s16)field_0x668.z, &inside_vec);
        lin_chk.Set(&my_vec_3, &my_vec_2, NULL);
        if (dComIfG_Bgsp().LineCross(&lin_chk) == 0) {
            return 1;
        }
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
        cXyz* p_vec = plane.GetNP();
        s16 tan_val = cM_atan2s(p_vec->absXZ(), p_vec->y) + -0x8000;
        if (abs(tan_val + 0x4000) <= 0x1555) {
            if (field_0x668.z != (s16)(cM_atan2s(p_vec->x, p_vec->z) + 0x8000)) {
                return 1;
            }
        }
    }
    return 0;
}

u8 daE_YM_c::checkBeforeGround() {
    cXyz my_vec_0;
    cXyz my_vec_1;
    dBgS_GndChk gnd_chk;
    cXyz adj_pos = current.pos;
    adj_pos.y += 300.0f;
    mDoMtx_stack_c::YrotS(current.angle.y);
    my_vec_0.set(0.0f, 00.0f, 60.0f);
    mDoMtx_stack_c::multVec(&my_vec_0, &my_vec_1);
    adj_pos.x += my_vec_1.x;
    adj_pos.z += my_vec_1.z;
    gnd_chk.SetPos(&adj_pos);
    f32 gnd_cross = dComIfG_Bgsp().GroundCross(&gnd_chk);
    if (gnd_cross > current.pos.y) {
        current.pos.set(adj_pos.x, gnd_cross, adj_pos.z);
        old.pos = current.pos;
        return 1;
    } else {
        return 0;
    }
}

void daE_YM_c::checkInitialWall() {
    cXyz my_vec_0;
    cXyz my_vec_1(0.0f, 0.0f, 200.0f);
    dBgS_LinChk lin_chk;
    for (int idx = 0; idx < 4; ++idx) {
        cLib_offsetPos(&my_vec_0, &current.pos, shape_angle.y + (idx << 0xe), &my_vec_1);
        lin_chk.Set(&current.pos, &my_vec_0, NULL);
        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            cM3dGPla plane;
            dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
            cXyz* p_vec = plane.GetNP();
            f32 absxz_val = p_vec->absXZ();
            s16 tan_val = cM_atan2s(absxz_val, p_vec->y) + -0x8000;
            if (abs(tan_val + 0x4000) <= 0x1555) {
                current.pos = lin_chk.GetCross();
                field_0x668.z = cM_atan2s(p_vec->x, p_vec->z) + 0x8000;
                field_0x668.x = tan_val;
                mAcchCir.SetWall(20.0f, 10.0f);
                return;
            }
        }
    }
    mType = 0;
}

u8 daE_YM_c::checkWall() {
    cXyz my_vec_0;
    cXyz my_vec_1;
    cXyz my_vec_2(0.0f, 0.0f, 100.0f);
    dBgS_LinChk lin_chk;
    if (mType != 1) {
        return 0;
    }
    cLib_offsetPos(&my_vec_1, &current.pos, field_0x668.z, &my_vec_2);
    my_vec_2.z = -50.0;
    cLib_offsetPos(&my_vec_0, &current.pos, field_0x668.z, &my_vec_2);
    lin_chk.Set(&my_vec_0, &my_vec_1, NULL);
    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        cM3dGPla plane;
        dComIfG_Bgsp().GetTriPla(lin_chk, &plane);
        cXyz* p_vec = plane.GetNP();
        s16 tan_val = cM_atan2s(p_vec->absXZ(), p_vec->y) + -0x8000;
        if (abs(tan_val + 0x4000) <= 0x1555) {
            current.pos = lin_chk.GetCross();
            field_0x668.z = cM_atan2s(p_vec->x, p_vec->z) + 0x8000;
            field_0x668.x = cM_atan2s(p_vec->absXZ(), p_vec->y) + -0x8000;
            return 1;
        }
    }
    return 0;
}

void daE_YM_c::setHideType() {
    mIsHide = true;
    mSphCc.SetCoSPrm(0x145);
    mSphCc.SetTgType(0x10000);

    m_near_obj = NULL;
    m_obj_dist = 100.0f;

    fpcM_Search(s_obj_sub, this);

    if (m_near_obj != NULL) {
        cXyz obj_pos(m_near_obj->current.pos);

        current.pos.x = obj_pos.x;
        current.pos.z = obj_pos.z;
        field_0x6a6 = 1;
    } else {
        field_0x6a6 = 3;
    }
}

int daE_YM_c::create() {
    fopAcM_ct(this, daE_YM_c);

    mType = fopAcM_GetParam(this);
    if (mType == 0xFF) {
        mType = 0;
    }

    mSwitchBit = (fopAcM_GetParam(this) & 0xff000000) >> 24;

    u8 prm2 = (fopAcM_GetParam(this) & 0xff0000) >> 16;
    if (prm2 == 0xFF) {
        prm2 = 0;
    }

    field_0x6e0 = prm2 * 100.0f;

    u8 prm1 = (fopAcM_GetParam(this) & 0xff00) >> 8;
    u8 tmp0 = (current.angle.z & 0xff00) >> 8;
    mTagNo = current.angle.z;
    field_0x6a1 = 1;
    if ((current.angle.x & 3) != 0) {
        field_0x6a1 = 0;
    }

    if ((current.angle.x & 12) != 0) {
        field_0x6cb = 1;
    }

    field_0x6a3 = (current.angle.x & 0xff00) >> 8;

    if (mSwitchBit != 0xFF && dComIfGs_isSwitch(mSwitchBit, fopAcM_GetRoomNo(this))) {
        // E_YM won't reset because it's already been defeated.
        OS_REPORT("E_YM やられ後なので再セットしません\n");
        return cPhs_ERROR_e;
    }

    if ((mTagNo != 0 && mTagNo != 0xFF) || (mType == 6 || mType == 3 || mType == 7)) {
        mFlyType = true;
    } else {
        mFlyType = false;
    }

    int phase;
    if (mFlyType == true) {
        phase = dComIfG_resLoad(&mPhase, "E_TM");
    } else {
        phase = dComIfG_resLoad(&mPhase, "E_YM");
    }

    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("E_YM PARAM %x %x %x \n", fopAcM_GetParam(this), current.angle.z, current.angle.x);
        current.angle.x = current.angle.z = 0;
        shape_angle.x = shape_angle.z = 0;

        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x28B0)) {
            return cPhs_ERROR_e;
        }

        if (!lbl_253_bss_8) {
            field_0xaf4 = 1;
            lbl_253_bss_8 = 1;
            l_HIO.field_0x4 = -1;
        }

        field_0x714 = 4;
        attention_info.distances[fopAc_attn_BATTLE_e] = 58;
        fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -200.0f, -100.0f, -200.0f);
        fopAcM_SetMax(this, 200.0f, 100.0f, 200.0f);

        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                  fopAcM_GetSpeed_p(this), NULL, NULL);
        mAcch.OnLineCheck();
        mAcchCir.SetWall(30.0f, 60.0f);
        field_0x560 = health = 10;

        mStts.Init(100, 0, this);
        mSphCc.Set(E_YM_n::cc_sph_src);
        mSphCc.SetStts(&mStts);

        mSound.init(&current.pos, &eyePos, 3, 1);
        mSound.setEnemyName("E_ym");
        mAtInfo.mpSound = &mSound;
        mAtInfo.mPowerType = 1;
        field_0x68c = 0.6f;

        switch (mType) {
        case 0:
            mPrevPos = current.pos;
            setActionMode(ACT_WAIT);
            break;
        case 1:
            checkInitialWall();
            mPrevPos = current.pos;
            setActionMode(ACT_WAIT);
            break;
        case 2:
            setActionMode(ACT_WAIT);
            mMode = 100;
            break;
        case 3:
            setActionMode(ACT_FLY);
            mMode = 2;
            break;
        case 4:
            if (tmp0 != 0xFF) {
                mpPath = dPath_GetRoomPath(tmp0, fopAcM_GetRoomNo(this));

                if (mpPath != NULL) {
                    attention_info.distances[fopAc_attn_ETC_e] = 31;
                    field_0x6a6 = 4;
                    mPrevPos = current.pos;
                    setActionMode(ACT_WAIT);
                    break;
                }
            }

            mType = 0;
            mPrevPos = current.pos;
            setActionMode(ACT_WAIT);
            break;
        case 5:
            if (field_0x6a3 == 0xFF) {
                mType = 0;
                mPrevPos = current.pos;
                setActionMode(ACT_WAIT);
            } else {
                if (tmp0 != 0xFF) {
                    mpPath = dPath_GetRoomPath(tmp0, fopAcM_GetRoomNo(this));
                }

                setActionMode(ACT_SWITCH);
                if (dComIfGs_isSwitch(field_0x6a3, fopAcM_GetRoomNo(this))) {
                    mMode = 10;
                }
            }
            break;
        case 6:
            setActionMode(ACT_FIRE);
            break;
        case 7:
            if (tmp0 != 0xFF) {
                mpPath = dPath_GetRoomPath(tmp0, fopAcM_GetRoomNo(this));
            }

            if (prm1 != 0xFF) {
                field_0x6b0 = dPath_GetRoomPath(prm1, fopAcM_GetRoomNo(this));
            }

            if (mpPath != NULL || field_0x6b0 != NULL) {
                field_0x68c = 1.5f;
                setActionMode(ACT_RIVER);
                break;
            }

            return cPhs_ERROR_e;
        }

        field_0x6cc = 1;
        if (mFlyType == true && mType != 3) {
            field_0x6cc = 0;
        }

        daE_YM_Execute(this);
    }

    return phase;
}

static int daE_YM_Create(daE_YM_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daE_YM_Method = {
    (process_method_func)daE_YM_Create,
    (process_method_func)daE_YM_Delete,
    (process_method_func)daE_YM_Execute,
    (process_method_func)daE_YM_IsDelete,
    (process_method_func)daE_YM_Draw,
};

extern actor_process_profile_definition g_profile_E_YM = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_YM,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daE_YM_c),       // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  194,                    // mPriority
  &l_daE_YM_Method,       // sub_method
  0x00040100,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
