/**
* @file d_a_e_rdb.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_rdb.h"
#include "Z2AudioLib/Z2Instances.h"
#include "c/c_damagereaction.h"
#include "d/actor/d_a_e_rd.h"
#include "d/actor/d_a_e_wb.h"
#include "d/d_msg_object.h"
#include "m_Do/m_Do_graphic.h"
#include "f_op/f_op_actor_enemy.h"
#include "f_op/f_op_camera_mng.h"

class daE_RDB_HIO_c : public JORReflexible {
public:
    daE_RDB_HIO_c();
    virtual ~daE_RDB_HIO_c() {};

    void genMessage(JORMContext*);
    /* 0x04 */ s8 mID;
    /* 0x08 */ f32 field_0x8;
};

enum Action {
    /* 0x00 */ ACTION_START,
    /* 0x01 */ ACTION_WAIT,
    /* 0x02 */ ACTION_FIGHT,
    /* 0x03 */ ACTION_ATTACK,
    /* 0x04 */ ACTION_SPIN_ATTACK,
    /* 0x05 */ ACTION_DEFENCE,
    /* 0x06 */ ACTION_DAMAGE,
    /* 0x07 */ ACTION_END,
};

daE_RDB_HIO_c::daE_RDB_HIO_c() {
    mID = -1;
    field_0x8 = 1.55f;
}

static void anm_init(e_rdb_class* i_this, int i_index, f32 i_morf, u8 i_attr, f32 i_rate) {
    i_this->mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_rdb", i_index), i_attr,
                                i_morf, i_rate, 0.0f, -1.0f);
    i_this->mAnm = i_index;
}

static int nodeCallBack(J3DJoint* i_joint, int param_2) {
    s16 sVar1;
    if (param_2 == 0) {
        int jntNo = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        e_rdb_class* rdb_p = (e_rdb_class*)model->getUserArea();
        if (rdb_p != NULL) {
            PSMTXCopy(model->getAnmMtx(jntNo), *calc_mtx);
            if (jntNo == 1 || jntNo == 2) {
                cMtx_XrotM(*calc_mtx, rdb_p->field_0x6ca / 8);

                if (jntNo == 1) {
                    cMtx_XrotM(*calc_mtx, rdb_p->field_0x6d0);
                }

                cMtx_ZrotM(*calc_mtx, -(rdb_p->field_0x6cc / 8));
            } else if (jntNo == 5 || jntNo == 6) {
                cMtx_YrotM(*calc_mtx, rdb_p->field_0x6d2 - rdb_p->field_0x6ca / 3);
                cMtx_ZrotM(*calc_mtx, -(rdb_p->field_0x6cc / 8));
            }

            model->setAnmMtx(jntNo, *calc_mtx);
            PSMTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }

    return 1;
}

static int daE_RDB_Draw(e_rdb_class* i_this) {
    J3DMaterial* material;
    if (i_this->field_0xfe6 != 0) {
        return 1;
    }

    J3DModel* model = i_this->mpModelMorf->getModel();
    g_env_light.settingTevStruct(0, &i_this->enemy.current.pos, &i_this->enemy.tevStr);

    if (i_this->field_0xfcf == 0) {
        g_env_light.setLightTevColorType_MAJI(model, &i_this->enemy.tevStr);

        for (u16 i = 3; i <= 4; i++) {
            J3DShape* shape = model->getModelData()->getMaterialNodePointer(i)->getShape();
            if (shape != NULL) {
                shape->hide();
            }
        }

        for (u16 i = 0; i <= 1; i++) {
            material = model->getModelData()->getMaterialNodePointer(i);
            material->getTevColor(0)->r = i_this->field_0x6e0;
            material->getTevColor(0)->g = i_this->field_0x6e0;
            material->getTevColor(0)->b = i_this->field_0x6e0;
        }

        i_this->mpModelMorf->entryDL();
    }

    if (i_this->field_0xfcf != 1 && i_this->field_0x680 != 0) {
        g_env_light.setLightTevColorType_MAJI(i_this->mpAxeModel, &i_this->enemy.tevStr);

        material = i_this->mpAxeModel->getModelData()->getMaterialNodePointer(0);
        material->getTevColor(0)->r = i_this->field_0x6e0;
        material->getTevColor(0)->g = i_this->field_0x6e0;
        material->getTevColor(0)->b = i_this->field_0x6e0;
        mDoExt_modelUpdateDL(i_this->mpAxeModel);
    }

    if (i_this->field_0xfcf == 0) {
        cXyz shadow_pos;
        shadow_pos.set(i_this->enemy.current.pos.x, i_this->enemy.current.pos.y + 100.0f,
                       i_this->enemy.current.pos.z);

        f32 fVar1 = 1700.0f;
        if (i_this->mAction == 7) {
            fVar1 = 2700.0f;
        }

        i_this->mShadowKey = dComIfGd_setShadow(
            i_this->mShadowKey, 1, model, &shadow_pos, fVar1, 0.0f, i_this->enemy.current.pos.y,
            i_this->mAcch.GetGroundH(), i_this->mAcch.m_gnd, &i_this->enemy.tevStr, 0, 1.0f,
            dDlst_shadowControl_c::getSimpleTex());
        if (i_this->mpAxeModel != NULL) {
            dComIfGd_addRealShadow(i_this->mShadowKey, i_this->mpAxeModel);
        }
    }

    return 1;
}

static void mtx_to_posAngle(Mtx param_1, cXyz* param_2, csXyz* param_3) {
    mDoMtx_multVecZero(param_1, param_2);
    mDoMtx_MtxToRot(param_1, param_3);
}

static int player_way_check(e_rdb_class* i_this) {
    s16 sVar1 = i_this->enemy.shape_angle.y - dComIfGp_getPlayer(0)->shape_angle.y;
    if (sVar1 < 0x4000 && sVar1 > -0x4000) {
        return 0;
    }

    return 1;
}

static void e_rdb_wait(e_rdb_class* i_this) {
    s16 sVar1 = i_this->enemy.shape_angle.y - i_this->mAngleToPlayer;
    f32 fVar1 = 0.0f;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, e_rdb_class::BCK_RB_WAIT01, 10.0f, 2, 1.0f);
        i_this->mMode = 1;
        i_this->field_0x6b8[0] = cM_rndF(10.0f) + 20.0f;
        // [[fallthrough]]
    case 1:
        if (i_this->mAnm == e_rdb_class::BCK_RB_STEP) {
            if (sVar1 < 512 && sVar1 > -512) {
                anm_init(i_this, e_rdb_class::BCK_RB_WAIT01, 5.0f, 2, 1.0f);
            }
        } else if (sVar1 > 512 || sVar1 < -512) {
            anm_init(i_this, e_rdb_class::BCK_RB_STEP, 5.0f, 2, 1.0f);
        }

        if (i_this->mDistToPlayer < 350.0f) {
            if (i_this->mAnm != e_rdb_class::BCK_RB_STEP) {
                anm_init(i_this, e_rdb_class::BCK_RB_STEP, 5.0f, 2, 1.0f);
            }
            i_this->mMode = 2;
        }
        break;

    case 2:
        fVar1 = -2.0f;
        if (i_this->mDistToPlayer > 400.0f) {
            i_this->mMode = 1;
        }
    }

    cLib_addCalc2(&i_this->enemy.speedF, fVar1, 1.0f, 3.0f);
    cLib_addCalcAngleS2(&i_this->enemy.current.angle.y, i_this->mAngleToPlayer, 2, 512);
    i_this->field_0x6c8 = 1;
    fVar1 = 420.0f;
    if (i_this->field_0xfcc >= 2 && strcmp(dComIfGp_getStartStageName(), "D_MN09") == 0) {
        fVar1 = 820.0f;
    }

    if (i_this->mDistToPlayer > fVar1) {
        i_this->mAction = 2;
        i_this->mMode = 0;
    } else if (i_this->mDistToPlayer < fVar1 && i_this->field_0x6b8[0] == 0) {
        if (i_this->field_0xfcc >= 2 && strcmp(dComIfGp_getStartStageName(), "D_MN09") == 0) {
            i_this->mAction = 4;
        } else {
            i_this->mAction = 3;
        }

        i_this->mMode = 0;
    }
}

static void e_rdb_fight(e_rdb_class* i_this) {
    f32 fVar1 = 0.0f;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, e_rdb_class::BCK_RB_WALK, 10.0f, 2, 1.0f);
        i_this->mMode = 1;
        i_this->field_0x5cc = 1.0f;
        // [[fallthrough]]
    case 1:
        if (i_this->mAnm == e_rdb_class::BCK_RB_WALK) {
            fVar1 = 3.0f;
            if (i_this->mDistToPlayer > 700.0f) {
                anm_init(i_this, e_rdb_class::BCK_RB_RUN, 10.0f, 2, 1.0f);
            }
        } else {
            fVar1 = 10.0f;
            if (i_this->mDistToPlayer < 600.0f) {
                i_this->field_0x5cc = 3.0f;
                anm_init(i_this, e_rdb_class::BCK_RB_WALK, 10.0f, 2, i_this->field_0x5cc);
                fVar1 = 3.0f;
            }
        }
    }

    i_this->mpModelMorf->setPlaySpeed(i_this->field_0x5cc);
    cLib_addCalc2(&i_this->enemy.speedF, fVar1 * i_this->field_0x5cc, 1.0f,
                  i_this->field_0x5cc * 3.0f);
    cLib_addCalc2(&i_this->field_0x5cc, 1.0f, 1.0f, 0.1f);
    cLib_addCalcAngleS2(&i_this->enemy.current.angle.y, i_this->mAngleToPlayer, 2, 512);
    fVar1 = 0.0f;

    if (i_this->field_0xfcc >= 2 && strcmp(dComIfGp_getStartStageName(), "D_MN09") == 0) {
        fVar1 = 400.0f;
    }

    if (i_this->mDistToPlayer < fVar1 + 400.0f) {
        i_this->mAction = 1;
        i_this->mMode = 0;
    }

    i_this->field_0x6c8 = 1;
}

static int getPolyColor(cBgS_PolyInfo& i_polyInfo, int param_2, _GXColor* p_effPrim,
                        _GXColor* p_effEnv, u8* p_alpha, f32* p_ratio) {
    if (dComIfG_Bgsp().ChkPolySafe(i_polyInfo) == 0) {
        return 0;
    }

    if (param_2 == 0) {
        dKy_pol_eff_prim_get(&i_polyInfo, p_effPrim);
        dKy_pol_eff_env_get(&i_polyInfo, p_effEnv);
        *p_alpha = dKy_pol_eff_alpha_get(&i_polyInfo);
        *p_ratio = dKy_pol_eff_ratio_get(&i_polyInfo);
    } else {
        dKy_pol_eff2_prim_get(&i_polyInfo, p_effPrim);
        dKy_pol_eff2_env_get(&i_polyInfo, p_effEnv);
        *p_alpha = dKy_pol_eff2_alpha_get(&i_polyInfo);
        *p_ratio = dKy_pol_eff2_ratio_get(&i_polyInfo);
    }

    return 1;
}

static void e_rdb_attack(e_rdb_class* i_this) {
    static u16 ap_name[3] = {
        0x8A5C,
        0x8A5D,
        0x8A5E,
    };

    int frame = i_this->mpModelMorf->getFrame();

    switch (i_this->mMode) {
    case 0:
        if (cM_rndF(1.0f) < 0.5f) {
            anm_init(i_this, e_rdb_class::BCK_RB_ATTACK01, 5.0f, 0, 1.0f);
            i_this->mMode = 1;
        } else {
            anm_init(i_this, e_rdb_class::BCK_RB_ATTACK02, 5.0f, 0, 1.0f);
            i_this->mMode = 2;
        }
        break;

    case 1:
        if (frame > 63 && frame < 73) {
            i_this->field_0x6c4 = 1;
        }

        if (frame == 68) {
            u8 i_alpha;
            f32 i_ratio;
            _GXColor i_effPrim, i_effEnv;
            if (getPolyColor(i_this->mAcch.m_gnd, 0, &i_effPrim, &i_effEnv, &i_alpha, &i_ratio) !=
                0)
            {
                cXyz i_scale(1.55f, 1.55f, 1.55);

                for (int i = 0; i <= 2; i++) {
                    if (i == 0) {
                        dComIfGp_particle_setColor(ap_name[i], &i_this->enemy.current.pos,
                                                   &i_this->enemy.tevStr, &i_effPrim, &i_effEnv,
                                                   i_ratio, i_alpha, &i_this->enemy.shape_angle,
                                                   &i_scale, NULL, -1, NULL);
                    } else {
                        dComIfGp_particle_set(ap_name[i], &i_this->enemy.current.pos,
                                              &i_this->enemy.shape_angle, &i_scale);
                    }
                }
            }

            dComIfGp_getVibration().StartShock(6, 31, cXyz(0.0f, 1.0f, 0.0f));
            i_this->mSound.startCreatureExtraSound(Z2SE_EN_RDB_ATTACK01_HIT, 0, -1);
        }

        if (i_this->mpModelMorf->isStop()) {
            i_this->mAction = 1;
            i_this->mMode = 0;
        }
        break;

    case 2:
        if (frame > 61 && frame < 72) {
            i_this->field_0x6c4 = 1;
        }

        if (i_this->mpModelMorf->isStop()) {
            i_this->mAction = 1;
            i_this->mMode = 0;
        }
    }

    cLib_addCalc0(&i_this->enemy.speedF, 1.0f, 3.0f);
    i_this->field_0x6c8 = 1;
}

static void e_rdb_spin_attack(e_rdb_class* i_this) {
    static u16 ap_name[2] = {
        0x8A5F,
        0x8A60,
    };

    mDoExt_McaMorfSO* p_modelMorf = i_this->mpModelMorf;
    int iVar1 = 0;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, e_rdb_class::BCK_RB_ATTACK03_START, 5.0f, 0, 1.0f);
        i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_V_ATTACK, -1);
        i_this->mMode = 1;
        break;

    case 1:
        if (!p_modelMorf->isStop())
            break;

        anm_init(i_this, e_rdb_class::BCK_RB_ATTACK03_SPIN, 0.0f, 2, 1.0f);
        i_this->mMode = 2;
        i_this->field_0x6b8[0] = 102;
        // [[fallthrough]]
    case 2:
        i_this->field_0x6c4 = 2;
        i_this->mSound.startCreatureExtraSoundLevel(Z2SE_EN_RDB_ATTACK03, 0, -1);
        i_this->enemy.current.angle.y += 7000;
        i_this->enemy.shape_angle.y = i_this->enemy.current.angle.y;
        i_this->field_0x6e4 = 5.0f;
        i_this->field_0x6e8 = i_this->mAngleToPlayer + (u16)-0x8000;
        iVar1 = 1;
        if (i_this->field_0x6b8[0] == 0) {
            anm_init(i_this, e_rdb_class::BCK_RB_ATTACK03_END, 0.0f, 0, 1.0f);
            i_this->mMode = 3;
        }
        break;

    case 3:
        if (!p_modelMorf->isStop())
            break;

        i_this->mAction = 1;
        i_this->mMode = 0;
        break;
    }

    cLib_addCalc0(&i_this->enemy.speedF, 1.0f, 3.0f);
    u8 i_alpha;
    f32 i_ratio;
    _GXColor i_effPrim, i_effEnv;
    if (iVar1 != 0 &&
        getPolyColor(i_this->mAcch.m_gnd, 0, &i_effPrim, &i_effEnv, &i_alpha, &i_ratio) != 0)
    {
        cXyz i_scale(1.55f, 1.55f, 1.55f);
        for (int i = 0; i <= 1; i++) {
            if (i == 0) {
                i_this->field_0xfd0[i] = dComIfGp_particle_setColor(
                    i_this->field_0xfd0[i], ap_name[i], &i_this->enemy.current.pos,
                    &i_this->enemy.tevStr, &i_effPrim, &i_effEnv, i_ratio, i_alpha,
                    &i_this->enemy.shape_angle, &i_scale, NULL, -1, NULL);
            } else {
                i_this->field_0xfd0[i] = dComIfGp_particle_set(
                    i_this->field_0xfd0[i], ap_name[i], &i_this->enemy.current.pos,
                    &i_this->enemy.shape_angle, &i_scale);
            }
        }
    }
}

static int e_rdb_defence(e_rdb_class* i_this) {
    i_this->field_0x6c6 = 1;
    i_this->field_0x6c0 = 3;
    int frame = i_this->mpModelMorf->getFrame();
    switch (i_this->mMode) {
    case 0:
        if (i_this->mAnm != e_rdb_class::BCK_RB_GUARD) {
            anm_init(i_this, e_rdb_class::BCK_RB_GUARD, 3.0f, 0, 1.0f);
        } else {
            if (frame > 15) {
                i_this->mpModelMorf->setFrame(0.0f);
            }
        }

        i_this->mMode = 1;
        break;

    case 1:
        if (i_this->mpModelMorf->isStop()) {
            i_this->mAction = 1;
            i_this->mMode = 0;
        }
        break;
    }

    cLib_addCalc0(&i_this->enemy.speedF, 1.0f, 3.0f);
    i_this->field_0x6c8 = 1;

    return 1;
}

static void e_rdb_damage(e_rdb_class* i_this) {
    int frame = i_this->mpModelMorf->getFrame();
    int iVar1;
    switch (i_this->mMode) {
    case 0:
        iVar1 = cM_rndF(4.99f);
        if (iVar1 == 0) {
            anm_init(i_this, e_rdb_class::BCK_RB_DAMAGEF, 2.0f, 0, 1.0f);
        } else if (iVar1 == 1) {
            anm_init(i_this, e_rdb_class::BCK_RB_DAMAGEFL, 2.0f, 0, 1.0f);
        } else if (iVar1 == 2) {
            anm_init(i_this, e_rdb_class::BCK_RB_DAMAGEFR, 2.0f, 0, 1.0f);
        } else if (iVar1 == 3) {
            anm_init(i_this, e_rdb_class::BCK_RB_DAMAGEBL, 2.0f, 0, 1.0f);
        } else if (iVar1 == 4) {
            anm_init(i_this, e_rdb_class::BCK_RB_DAMAGEBR, 2.0f, 0, 1.0f);
        }

        i_this->mMode = 1;
        break;

    case 1:
        if (i_this->field_0x6b8[3] != 0) {
            i_this->enemy.onHeadLockFlg();
        }

        if (i_this->mpModelMorf->isStop()) {
            i_this->mAction = 1;
            i_this->mMode = 0;
            i_this->field_0xfcd = 0;
        }
        break;

    case 10:
        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, e_rdb_class::BCK_RB_DOWN_WAIT, 5.0f, 2, 1.0f);
            i_this->field_0x6b8[0] = 60;
            i_this->mMode = 11;
        }
        break;

    case 11:
        if (i_this->field_0x6b8[0] == 0) {
            i_this->field_0x6c0 = 50;
            if (i_this->field_0xfcc >= 2 && strcmp(dComIfGp_getStartStageName(), "D_MN09") == 0) {
                i_this->mAction = 4;
                i_this->mMode = 0;
            } else {
                anm_init(i_this, e_rdb_class::BCK_RB_DOWN_RETURN, 5.0f, 0, 1.0f);
                i_this->mMode = 12;
            }
        }
        break;

    case 12:
        if (frame >= 25 && frame <= 35) {
            cLib_addCalcAngleS2(&i_this->enemy.current.angle.y, i_this->mAngleToPlayer, 2, 0x1000);
        }

        if (frame >= 35 && frame <= 42) {
            i_this->field_0x6c4 = 1;
        }

        if (i_this->mpModelMorf->isStop()) {
            i_this->mAction = 1;
            i_this->mMode = 0;
            i_this->field_0x6c0 = 10;
        }
    }

    cLib_addCalc0(&i_this->enemy.speedF, 1.0f, 3.0f);
}

static void e_rdb_end(e_rdb_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    e_wb_class* actor = (e_wb_class*)fopAcM_SearchByName(PROC_E_WB);
    int frame = i_this->mpModelMorf->getFrame();
    cXyz sp40;
    f32 fVar1 = 0.0f;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, e_rdb_class::BCK_RB_ED_DEMO_DOWN, 2.0f, 0, 1.0f);
        i_this->mMode = 1;
        if (actor != NULL) {
            actor->field_0x169e = 60;
        }
        break;

    case 1:
        if (frame == 3) {
            PSMTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(15), *calc_mtx);
            mtx_to_posAngle(*calc_mtx, &i_this->field_0x684, &i_this->field_0x69c);
            i_this->field_0x680 = 2;
            cMtx_YrotS(*calc_mtx, i_this->enemy.shape_angle.y);
            sp40.x = 10.0f;
            sp40.y = 25.0f;
            sp40.z = -10.0f;
            MtxPosition(&sp40, &i_this->field_0x690);
            i_this->field_0x6a4 = 200;
            i_this->field_0x6a2 = 700;
        }

        if (frame == 141) {
            fopAcM_effSmokeSet1(&i_this->field_0xfd0[0], &i_this->field_0xfd0[1],
                                &i_this->enemy.eyePos, &i_this->enemy.shape_angle, 2.5f,
                                &i_this->enemy.tevStr, 1);
            dComIfGp_getVibration().StartShock(2, 31, cXyz(0.0f, 1.0f, 0.0f));
            i_this->mSound.startCreatureSound(Z2SE_CM_BODYFALL_L, 0, -1);
        }

        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, e_rdb_class::BCK_RB_ED_DEMO_DOWNWAIT, 2.0f, 2, 1.0f);
            i_this->mMode = 2;
        }
        break;

    case 3:
        anm_init(i_this, e_rdb_class::BCK_RB_ED_DEMO_DOWNUP, 5.0f, 0, 1.0f);
        i_this->mMode = 4;
        break;

    case 4:
        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, e_rdb_class::BCK_RB_ED_DEMO_DOWNWALK, 3.0f, 2, 1.0f);
            i_this->mMode = 5;
        }
        break;

    case 5:
        fVar1 = 5.0f;
        break;

    case 10:
        if ((((actor->mActionID == 102 && actor->mEnemy.speedF > 40.0f) &&
              player->current.pos.x > 1470.0f && player->current.pos.x < 2130.0f)) &&
            ((player->current.pos.z < 200.0f) &&
             (player->shape_angle.y > 0x7000 || player->shape_angle.y < -0x7000)))
        {
            actor->field_0x169e = 80;
            i_this->mMode = 11;
        }
        // [[fallthrough]]
    case 11:
        i_this->field_0x6c0 = 10;
        i_this->enemy.current.pos.y = i_this->enemy.home.pos.y + 5000.0f;
        i_this->enemy.speed.y = 0.0f;
        i_this->field_0xfcf = 2;
        fopAcM_OffStatus(&i_this->enemy, 0x100);
        break;

    case 20:
        anm_init(i_this, e_rdb_class::BCK_RB_LV9_END01, 2.0f, 0, 1.0f);
        i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_V_LV9_END01, -1);
        i_this->mMode = 21;
        i_this->mDemoMode = 10;
        break;

    case 21:
        if (frame == 12) {
            i_this->field_0x680 = 0;
        }

        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, e_rdb_class::BCK_RB_LV9_END02, 3.0f, 0, 1.0f);
            i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_V_LV9_END02, -1);
            i_this->mMode = 22;
        }
        break;

    case 22:
        if (frame == 50) {
            i_this->field_0x1104 = 1;
        }
    }

    cLib_addCalc2(&i_this->enemy.speedF, fVar1, 1.0f, 3.0f);
}

static void e_rdb_start(e_rdb_class* i_this) {
    int frame = i_this->mpModelMorf->getFrame();
    f32 fVar1 = 0.0f;
    i_this->field_0x6c0 = 3;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, e_rdb_class::BCK_RB_WAIT01, 10.0f, 2, 1.0f);
        i_this->mMode = 1;
        break;

    case 2:
        i_this->field_0xfcf = 0;
        if (i_this->field_0x6b8[0] == 0) {
            anm_init(i_this, e_rdb_class::BCK_RB_WALK, 10.0f, 2, 1.0f);
            i_this->mMode = 3;
        }
        break;

    case 3:
        fVar1 = 3.0f;
        break;

    case 4:
        anm_init(i_this, e_rdb_class::BCK_RB_OP_DEMO_SWING, 10.0f, 0, 1.0f);
        i_this->mMode = 5;
        i_this->field_0x6d8 = 3;
        break;

    case 5:
        if ((frame >= 128 && frame <= 142) || (frame >= 166 && frame <= 175)) {
            fVar1 = 10.0f;
        }

        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, e_rdb_class::BCK_RB_WAIT01, 10.0f, 2, 1.0f);
            i_this->mMode = 6;
        }
        break;

    case 7:
        i_this->mAction = 2;
        i_this->mMode = 0;
        break;

    case 10:
        i_this->mMode = 11;
        break;

    case 11:
        i_this->enemy.current.pos.y = i_this->enemy.home.pos.y + 20000.0f;
        i_this->enemy.speed.y = 0.0f;

        if (fopAcM_searchPlayerDistanceXZ(&i_this->enemy) < 500.0f) {
            i_this->mMode = 12;
            anm_init(i_this, e_rdb_class::BCK_RB_LV9_OP01, fVar1, 0, 1.0f);
            i_this->mSound.startCreatureSound(Z2SE_EN_RDB_LV9_OP, 0, -1);
            i_this->mDemoMode = 1;
            i_this->enemy.current.pos.y = i_this->enemy.home.pos.y + 2000.0f;
        }
        break;

    case 12:
        if (i_this->mAcch.ChkGroundHit()) {
            anm_init(i_this, e_rdb_class::BCK_RB_LV9_OP02, 0.0f, 0, 1.0f);
            i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_V_LV9_OP, -1);
            i_this->mMode = 13;
            dComIfGp_getVibration().StartShock(8, 79, cXyz(0.0f, 1.0f, 0.0f));

            mDoAud_seStart(Z2SE_OBJ_FM_DIRTFALL, 0, 0, 0);
            i_this->mSound.startCreatureSound(Z2SE_EN_FM_DOWN, 0, -1);
            fopAcM_effSmokeSet1(&i_this->field_0xfd0[0], &i_this->field_0xfd0[1],
                                &i_this->enemy.current.pos, &i_this->enemy.shape_angle, 3.5f,
                                &i_this->enemy.tevStr, 1);
        }
        break;

    case 13:
        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, e_rdb_class::BCK_RB_WAIT01, 5.0f, 2, 1.0f);
        }
    }

    i_this->enemy.speedF = fVar1;
}

static void damage_check(e_rdb_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->enemy;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    if (i_this->field_0x6c2 == 0 && i_this->field_0xe64.ChkTgHit() != 0 &&
        i_this->field_0xe64.ChkTgShield() != 0)
    {
        def_se_set(&i_this->mSound, i_this->field_0xe64.GetTgHitObj(), 40, NULL);
        i_this->field_0x6c2 = 8;
        i_this->field_0x6c0 = 8;
        i_this->field_0x6e4 = 12.0f + TREG_F(7);
        i_this->field_0x6e8 = a_this->shape_angle.y;
    }

    i_this->mStts.Move();

    u32 i_soundID = 0;
    s8 bVar1 = 0;
    s8 bVar2 = 0;
    if (i_this->field_0x6c0 == 0) {
        for (int i = 0; i <= 2; i = i + 1) {
            if (i_this->field_0x944[i].ChkTgHit() != 0) {
                i_this->mAtInfo.mpCollider = i_this->field_0x944[i].GetTgHitObj();
                cc_at_check(a_this, &i_this->mAtInfo);
                OS_REPORT("E_rdb HP %d\n", a_this->health);
                if (i_this->mAtInfo.mAttackPower != 0) {
                    u16 uVar1;
                    cXyz sp2c(1.0f, 1.0f, 1.0f);
                    if (i_this->mAtInfo.mHitStatus != 0) {
                        uVar1 = 3;
                    } else {
                        uVar1 = 1;
                    }

                    cXyz sp38, sp44;
                    cMtx_YrotS(*calc_mtx, i_this->mAngleToPlayer);
                    if (i_this->mAnm == e_rdb_class::BCK_RB_DOWN || i_this->mAnm == e_rdb_class::BCK_RB_DOWN_WAIT) {
                        sp44.x = 0.0f + YREG_F(7);
                        sp44.y = 120.0f + YREG_F(8);
                        sp44.z = 190.0f + YREG_F(9);
                        MtxPosition(&sp44, &sp38);
                        sp38 += a_this->current.pos;
                    } else if (daPy_getPlayerActorClass()->getCutType() ==
                               daPy_py_c::CUT_TYPE_HEAD_JUMP)
                    {
                        sp38 = a_this->eyePos;
                        sp38.y += 100.0f;
                    } else {
                        sp44.x = 0.0f + YREG_F(7);
                        sp44.y = 150.0f + YREG_F(8);
                        sp44.z = 100.0f + YREG_F(9);
                        MtxPosition(&sp44, &sp38);
                        sp38 += a_this->current.pos;
                    }

                    dComIfGp_setHitMark(uVar1, a_this, &sp38, NULL, &sp2c, 0);
                    if (a_this->health <= 0) {
                        bVar2 = 1;
                        break;
                    }
                }

                i_this->field_0x6ec |= i_this->mAtInfo.mHitBit;
                if (i_this->field_0x6c7 != 0) {
                    i_this->field_0x6c7 = 0;
                    bVar1 = 1;
                }

                if (player->getCutType() == daPy_py_c::CUT_TYPE_JUMP &&
                    player->checkCutJumpCancelTurn())
                {
                    i_this->field_0x6c0 = 3;
                    i_this->field_0x6c7 = 1;
                } else {
                    i_this->field_0x6c0 = 6;
                }

                if ((i_this->mAction != 6 || i_this->mMode < 10) &&
                    (daPy_getPlayerActorClass()->getCutCount() >= 4 || bVar1))
                {
                    int iVar1;
                    if (strcmp(dComIfGp_getStartStageName(), "D_MN09") == 0) {
                        iVar1 = 6;
                    } else {
                        iVar1 = 4;
                    }

                    if (i_this->field_0xfcc >= iVar1) {
                        bVar2 = 1;
                    } else {
                        anm_init(i_this, e_rdb_class::BCK_RB_DOWN, 5.0f, 0, 1.0f);
                        i_this->mAction = 6;
                        i_this->mMode = 10;
                        i_this->field_0x6c0 = 20;
                        i_this->field_0xfcc++;
                        i_soundID = Z2SE_EN_RDB_V_DAMAGE;
                    }
                } else if (i_this->mAction == 6 && i_this->mMode >= 10) {
                    i_this->field_0x6ce = 15;
                    s16 angle_diff = a_this->shape_angle.y - i_this->mAngleToPlayer;
                    if (angle_diff > 0) {
                        i_this->field_0x6d4 = 1.0f;
                    } else {
                        i_this->field_0x6d4 = -1.0f;
                    }
                    i_soundID = Z2SE_EN_RDB_V_DAMAGE;
                } else {
                    i_this->mAction = 6;
                    i_this->mMode = 0;
                    if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_SHIELD_ATTACK) != 0) {
                        i_this->field_0x6b8[3] = 30;
                        i_this->field_0x6c0 = 30;
                        i_soundID = Z2SE_EN_RDB_V_AWAKE;
                    } else {
                        i_this->field_0x6b8[3] = 0;
                        i_soundID = Z2SE_EN_RDB_V_DAMAGE;
                    }

                    i_this->field_0xfcd++;
                    if (i_this->field_0xfcd >= 4) {
                        i_this->field_0x6c0 = 60;
                    }
                }

                if (a_this->health <= 1) {
                    i_this->field_0x944[i].SetTgHitMark((CcG_Tg_HitMark)3);
                }

                break;
            }
        }
    }

    if (bVar2 != 0) {
        i_this->mAction = 7;
        i_this->field_0xfcd = 0;
        if (strcmp(dComIfGp_getStartStageName(), "D_MN09") == 0) {
            i_this->mMode = 20;
        } else {
            i_this->mMode = 0;
        }

        i_this->field_0x6c0 = 1000;
        i_soundID = Z2SE_EN_RDB_V_ED_DEMO_DOWN;

        Z2GetAudioMgr()->subBgmStop();
    }

    if (i_soundID != 0) {
        i_this->mSound.startCreatureVoice(i_soundID, -1);
    }
}

static void* shot_s_sub(void* i_actor, void* i_data) {
    if ((fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_ARROW) &&
        (fopAcM_GetParam(i_actor) == 1 || fopAcM_GetParam(i_actor) == 2))
    {
        cXyz sp28 = static_cast<fopAc_ac_c*>(i_data)->current.pos -
                    static_cast<fopAc_ac_c*>(i_actor)->current.pos;
        if (sp28.abs() < 1000.0f) {
            return i_actor;
        }
    }

    return NULL;
}

static u8 s_HIOinit;

static daE_RDB_HIO_c l_HIO;

static void action(e_rdb_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->enemy;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz sp44, sp50;
    i_this->field_0x6c8 = 0;
    i_this->mDistToPlayer = fopAcM_searchPlayerDistance(a_this);
    i_this->mAngleToPlayer = fopAcM_searchPlayerAngleY(a_this);

    damage_check(i_this);
    s8 sVar1 = 0;
    s8 sVar2 = 1;
    int sVar3 = 1;

    switch (i_this->mAction) {
    case ACTION_START:
        e_rdb_start(i_this);
        sVar2 = 0;
        sVar3 = 0;
        break;

    case ACTION_WAIT:
        e_rdb_wait(i_this);
        sVar1 = 1;
        break;

    case ACTION_FIGHT:
        e_rdb_fight(i_this);
        sVar1 = 1;
        break;

    case ACTION_ATTACK:
        e_rdb_attack(i_this);
        sVar1 = 1;
        sVar3 = 0;
        break;

    case ACTION_SPIN_ATTACK:
        e_rdb_spin_attack(i_this);
        sVar1 = 1;
        sVar3 = 0;
        break;

    case ACTION_DEFENCE:
        sVar3 = e_rdb_defence(i_this);
        sVar1 = 1;
        break;

    case ACTION_DAMAGE:
        e_rdb_damage(i_this);
        sVar3 = 0;
        break;

    case ACTION_END:
        e_rdb_end(i_this);
        sVar3 = 0;
    }

    if (sVar1) {
        i_this->mSound.setLinkSearch(true);
    } else {
        i_this->mSound.setLinkSearch(false);
    }

    if (sVar2) {
        fopAcM_OnStatus(a_this, 0);
        a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(a_this, 0);
        a_this->attention_info.flags = 0;
    }

    if ((s8)sVar3) {
        int iVar1 = 0;
        if (daPy_getPlayerActorClass()->checkHookshotShootReturnMode() &&
            !daPy_getPlayerActorClass()->checkHookshotReturnMode())
        {
            iVar1 = 1;
        } else if (strcmp(dComIfGp_getStartStageName(), "D_MN09") == 0 &&
                   fpcM_Search(shot_s_sub, i_this) != NULL)
        {
            iVar1 = 1;
        }

        if (strcmp(dComIfGp_getStartStageName(), "D_MN09") == 0) {
            cXyz* ironBallCenterPos = player->getIronBallCenterPos();
            if (ironBallCenterPos && (player->current.pos - *ironBallCenterPos).abs() > 200.0f &&
                (a_this->current.pos - *ironBallCenterPos).abs() < 500.0f &&
                (dComIfGp_checkPlayerStatus0(0, 0x400) == 0) &&
                (!daPy_getPlayerActorClass()->checkIronBallReturn()) &&
                (!daPy_getPlayerActorClass()->checkIronBallGroundStop()))
            {
                iVar1 = 2;
            }
        }

        int bVar1 = 0;
        if ((strcmp(dComIfGp_getStartStageName(), "D_MN09") == 0 &&
             player->getCutType() == daPy_py_c::CUT_TYPE_GUARD_ATTACK) ||
            (daPy_getPlayerActorClass()->getCutAtFlg() != 0 &&
             player->getCutType() != daPy_py_c::CUT_TYPE_HEAD_JUMP))
        {
            bVar1 = 1;
        }

        if ((iVar1 != 0 && player_way_check(i_this) != 0) ||
            ((i_this->mDistToPlayer < 500.0f && player_way_check(i_this) != 0 && bVar1) &&
             (player->checkNowWolf() || (strcmp(dComIfGp_getStartStageName(), "D_MN09") == 0) ||
              (i_this->field_0x6ec & cc_pl_cut_bit_get()) != 0)))
        {
            i_this->mAction = ACTION_DEFENCE;
            i_this->mMode = 0;
            i_this->field_0x6c0 = 3;
        }
    }

    cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 2, 0x1000);
    cLib_addCalcAngleS2(&a_this->shape_angle.x, a_this->current.angle.x, 2, 0x1000);
    cLib_addCalcAngleS2(&a_this->shape_angle.z, a_this->current.angle.z, 2, 0x1000);
    a_this->gravity = -5.0f;

    cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
    sp44.x = 0.0f;
    sp44.y = 0.0f;
    sp44.z = a_this->speedF * (l_HIO.field_0x8 / 1.44f);
    MtxPosition(&sp44, &sp50);
    a_this->speed.x = sp50.x;
    a_this->speed.z = sp50.z;
    a_this->current.pos += a_this->speed;
    a_this->speed.y += a_this->gravity;
    if (a_this->speed.y < -80.0f) {
        a_this->speed.y = -80.0f;
    }

    if (i_this->field_0x6e4 > 0.1f) {
        sp44.x = 0.0f;
        sp44.y = 0.0f;
        sp44.z = -i_this->field_0x6e4;
        cMtx_YrotS(*calc_mtx, i_this->field_0x6e8);
        MtxPosition(&sp44, &sp50);
        a_this->current.pos += sp50;
        cLib_addCalc0(&i_this->field_0x6e4, 1.0f, 3.0f);
    }

    s16 sVar5 = 0;
    s16 sVar6 = 0;
    cXyz sp5c = player->eyePos;
    if (i_this->field_0x6c8 != 0) {
        sp44 = sp5c - a_this->current.pos;
        sp44.y += -150.0f;
        sVar5 = cM_atan2s(sp44.x, sp44.z) - a_this->shape_angle.y;
        sVar6 = a_this->shape_angle.x +
                cM_atan2s(sp44.y, JMAFastSqrt(sp44.x * sp44.x + sp44.z * sp44.z));

        if (sVar5 > 0x3E80) {
            sVar5 = 16000;
        } else if (sVar5 < -16000) {
            sVar5 = -16000;
        }

        if (sVar6 > 0x3E80) {
            sVar6 = 16000;
        } else if (sVar6 < -16000) {
            sVar6 = -16000;
        }
    }

    cLib_addCalcAngleS2(&i_this->field_0x6ca, sVar5, 2, 0x1000);
    cLib_addCalcAngleS2(&i_this->field_0x6cc, sVar6, 2, 0x1000);
    if (i_this->field_0x6ce != 0) {
        i_this->field_0x6ce--;
        i_this->field_0x6d0 = i_this->field_0x6ce * cM_ssin(i_this->field_0x6ce * 12000) * 50.0f;
        i_this->field_0x6d2 = i_this->field_0x6d4 *
                              (i_this->field_0x6ce * cM_ssin(i_this->field_0x6ce * 5000)) * 600.0f;
    }

    cXyz sp68(1.5f, 1.5f, 1.5f);
    setMidnaBindEffect(a_this, &i_this->mSound, &a_this->eyePos, &sp68);
}

static void cam_3d_morf(e_rdb_class* i_this, f32 i_scale) {
    cLib_addCalc2(&i_this->mDemoCamCenter.x, i_this->field_0x10d0.x, i_scale,
                  i_this->field_0x10e8.x * i_this->field_0x10f8);
    cLib_addCalc2(&i_this->mDemoCamCenter.y, i_this->field_0x10d0.y, i_scale,
                  i_this->field_0x10e8.y * i_this->field_0x10f8);
    cLib_addCalc2(&i_this->mDemoCamCenter.z, i_this->field_0x10d0.z, i_scale,
                  i_this->field_0x10e8.z * i_this->field_0x10f8);
    cLib_addCalc2(&i_this->mDemoCamEye.x, i_this->field_0x10c4.x, i_scale,
                  i_this->field_0x10dc.x * i_this->field_0x10f8);
    cLib_addCalc2(&i_this->mDemoCamEye.y, i_this->field_0x10c4.y, i_scale,
                  i_this->field_0x10dc.y * i_this->field_0x10f8);
    cLib_addCalc2(&i_this->mDemoCamEye.z, i_this->field_0x10c4.z, i_scale,
                  i_this->field_0x10dc.z * i_this->field_0x10f8);
}

static void cam_spd_set(e_rdb_class* i_this) {
    i_this->field_0x10dc.x = fabsf(i_this->field_0x10c4.x - i_this->mDemoCamEye.x);
    i_this->field_0x10dc.y = fabsf(i_this->field_0x10c4.y - i_this->mDemoCamEye.y);
    i_this->field_0x10dc.z = fabsf(i_this->field_0x10c4.z - i_this->mDemoCamEye.z);
    i_this->field_0x10e8.x = fabsf(i_this->field_0x10d0.x - i_this->mDemoCamCenter.x);
    i_this->field_0x10e8.y = fabsf(i_this->field_0x10d0.y - i_this->mDemoCamCenter.y);
    i_this->field_0x10e8.z = fabsf(i_this->field_0x10d0.z - i_this->mDemoCamCenter.z);
    i_this->field_0x10f8 = 0.0f;
}

static void demo_camera(e_rdb_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->enemy;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    cXyz sp28, sp34, sp40, sp4c;
    s8 sVar1 = 0;

    switch (i_this->mDemoMode) {
    case 1:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xffff, 0);
            a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }

        camera->mCamera.Stop();
        i_this->mDemoMode = 2;
        i_this->field_0x10aa = 0;
        i_this->mDemoCamFovy = 55.0f;
        camera->mCamera.SetTrimSize(3);
        player->changeOriginalDemo();
        dComIfGp_getEvent().startCheckSkipEdge(i_this);
        Z2GetAudioMgr()->setDemoName("force_start");
        // [[fallthrough]]
    case 2:
        cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
        sp28.x = 0.0f;
        sp28.y = 150.0f;
        sp28.z = 330.0f;
        MtxPosition(&sp28, &sp40);
        sp40.x += a_this->current.pos.x;
        sp40.z += a_this->current.pos.z;
        sp40.y = 140.0f;
        sp4c.x = a_this->current.pos.x;
        sp4c.z = a_this->current.pos.z;
        sp4c.y = a_this->eyePos.y - 20.0f;
        if (i_this->field_0x10aa == 0) {
            i_this->mDemoCamCenter = sp4c;
            i_this->mDemoCamEye = sp40;
        } else {
            i_this->mDemoCamEye = sp40;
            cLib_addCalc2(&i_this->mDemoCamCenter.x, sp4c.x, 0.2f, 100.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.y, sp4c.y, 0.2f, 100.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.z, sp4c.z, 0.2f, 100.0f);
        }

        cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
        sp28.x = 0.0f;
        sp28.y = 50.0f;
        sp28.z = 600.0f;
        MtxPosition(&sp28, &sp34);
        sp34.x += a_this->current.pos.x;
        sp34.z += a_this->current.pos.z;
        sp34.y = 50.0f;
        player->setPlayerPosAndAngle(&sp34, i_this->mAngleToPlayer + (u16)-0x8000, 0);
        if (i_this->field_0x10aa >= 170) {
            if (i_this->field_0x10aa == 170) {
                i_this->mMsgFlow.init(a_this, 0x389, 0, NULL);
            }

            if (i_this->mMsgFlow.doFlow(a_this, NULL, 0) != 0 && i_this->field_0x10aa >= 0x168) {
                i_this->mDemoMode = 3;
                i_this->field_0x10aa = 0;
                i_this->field_0x10d0.set(-7627.0f, 153.0f, -5371.0f);
                i_this->field_0x10c4.set(-7470.0f, 144.0f, -5071.0f);
                cam_spd_set(i_this);
            }
        }
        break;

    case 3:
        if (i_this->field_0x10aa >= 20) {
            cam_3d_morf(i_this, 0.2f);
            cLib_addCalc2(&i_this->field_0x10f8, 0.03f, 1.0f, 0.003f);
            if (i_this->field_0x10aa == 60) {
                player->changeDemoMode(23, 1, 2, 0);
            }

            if (i_this->field_0x10aa == 130) {
                sVar1 = 1;
                i_this->mAction = ACTION_WAIT;
                i_this->mMode = 10;
                Z2GetAudioMgr()->subBgmStart(Z2BGM_FACE_OFF_BATTLE3);
            }
        }
        break;

    case 10:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xffff, 0);
            a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }

        camera->mCamera.Stop();
        i_this->mDemoMode = 11;
        i_this->field_0x10aa = 0;
        i_this->mDemoCamFovy = 55.0f;
        camera->mCamera.SetTrimSize(3);

        player->changeOriginalDemo();
        a_this->current = a_this->home;
        a_this->shape_angle = a_this->current.angle;
        Z2GetAudioMgr()->setDemoName("force_start");
        // [[fallthrough]]
    case 11:
        cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
        sp28.x = 50.0f;
        sp28.y = 0.0f;
        sp28.z = 230.0f;
        MtxPosition(&sp28, &sp40);
        sp40.x += a_this->current.pos.x;
        sp40.z += a_this->current.pos.z;
        sp40.y = 220.0f;
        if (i_this->field_0x10aa >= 328) {
            PSMTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(21), *calc_mtx);
            sp28.x = 30.0f;
            sp28.y = 20.0f;
            sp28.z = 0.0f;
            MtxPosition(&sp28, &sp4c);
        } else {
            sp4c.x = a_this->current.pos.x;
            sp4c.z = a_this->current.pos.z;
            sp4c.y = a_this->eyePos.y;
            if (sp4c.y < 190.0f) {
                sp4c.y = 190.0f;
            }
        }

        if (i_this->field_0x10aa == 0) {
            i_this->mDemoCamCenter = sp4c;
            i_this->mDemoCamEye = sp40;
        } else {
            i_this->mDemoCamEye = sp40;
            cLib_addCalc2(&i_this->mDemoCamCenter.x, sp4c.x, 0.2f, 10.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.y, sp4c.y, 0.2f, 10.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.z, sp4c.z, 0.2f, 10.0f);
        }

        cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
        sp28.x = 0.0f;
        sp28.y = 50.0f;
        sp28.z = 300.0f;
        MtxPosition(&sp28, &sp34);
        sp34.x += a_this->current.pos.x;
        sp34.z += a_this->current.pos.z;
        sp34.y = 50.0f;
        player->setPlayerPosAndAngle(&sp34, i_this->mAngleToPlayer + (u16)-0x8000, 0);

        if (i_this->field_0x10aa >= 360) {
            if (i_this->field_0x10aa == 360) {
                i_this->mMsgFlow.init(a_this, 0x388, 0, NULL);
            }

            int iVar1 = 0;
            if (i_this->field_0x10aa >= 9000) {
                i_this->mMsgFlow.remove();
                iVar1 = 1;
            }

            if (i_this->mMsgFlow.doFlow(a_this, NULL, 0) != 0 && i_this->field_0x10aa >= 400) {
                iVar1 = 1;
            }

            if (iVar1 != 0) {
                daPy_getPlayerActorClass()->changeDemoMode(11, 32, 0, 0);
                i_this->mDemoMode = 12;
                i_this->field_0x10aa = 0;
                i_this->field_0xfe5 = 1;
                dComIfGs_onTbox(3);
            }
        }
        break;

    case 12:
        sp4c.x = player->current.pos.x;
        sp4c.y = player->current.pos.y + 145.0f;
        sp4c.z = player->current.pos.z;
        cLib_addCalc2(&i_this->mDemoCamCenter.x, sp4c.x, 0.2f, 10.0f);
        cLib_addCalc2(&i_this->mDemoCamCenter.y, sp4c.y, 0.2f, 10.0f);
        cLib_addCalc2(&i_this->mDemoCamCenter.z, sp4c.z, 0.2f, 10.0f);
        cLib_addCalc2(&i_this->mDemoCamEye.y, 170.0f, 0.2f, 10.0f);
        cLib_addCalc2(&i_this->mDemoCamFovy, 70.0f, 0.05f, 0.25f);
        if (i_this->field_0x10aa >= 70 && !dMsgObject_isTalkNowCheck()) {
            i_this->mDemoMode = 13;
            i_this->field_0x10aa = 0;
            mDoGph_gInf_c::fadeOut(0.05f, g_blackColor);
            i_this->field_0xfe6 = 1;
            dComIfGs_offOneZoneSwitch(15, -1);
        }
        break;

    case 13:
        if (i_this->field_0x10aa == 40) {
            cXyz sp58(-7080.0f, 50.0f, -6634.0f);
            sp28.x = -10008.0f - sp58.x;
            sp28.z = -2729.0f - sp58.z;
            csXyz i_angle(0, cM_atan2s(sp28.x, sp28.z), 0);
            sp58 = a_this->current.pos;
            sp58.y += 2000.0f;
            fopAcM_create(PROC_E_RD, 12, &sp58, fopAcM_GetRoomNo(a_this), &i_angle, NULL, -1);
            sp34.set(-7530.0f, 50.0f, -5451.0f);
            player->setPlayerPosAndAngle(&sp34, -0x5F3B, 0);
            i_this->mDemoCamCenter.set(-7661.0f, 323.0f, -5684.0f);
            i_this->mDemoCamEye.set(-7382.0f, 185.0f, -5360.0f);
            i_this->mDemoCamFovy = 55.0f;
            daPy_getPlayerActorClass()->changeDemoMode(1, 1, 0, 0);
        }

        if (i_this->field_0x10aa >= 40) {
            e_rd_class* bulblin_p = (e_rd_class*)fopAcM_SearchByName(PROC_E_RD);
            if (bulblin_p != NULL) {
                a_this->current.pos = bulblin_p->actor.current.pos;
                a_this->eyePos = bulblin_p->actor.eyePos;
            }
        }

        if (i_this->field_0x10aa == 50) {
            mDoGph_gInf_c::fadeOut(-0.05f, g_blackColor);
        }

        if (i_this->field_0x10aa >= 220) {
            if (i_this->field_0x10aa == 220) {
                i_this->mMsgFlow.init(a_this, 0x388, 0, NULL);
            }

            if (i_this->mMsgFlow.doFlow(a_this, NULL, 0) != 0 && i_this->field_0x10aa >= 300) {
                i_this->mDemoMode = 14;
                i_this->field_0x10aa = 0;
            }
        }
        break;

    case 14:
        if (i_this->field_0x10aa >= 65) {
            i_this->field_0x10d0.set(-7738.0f, 185.0f, -5086.0f);
            i_this->field_0x10c4.set(-7382.0f, 185.0f, -5360.0f);
            cam_spd_set(i_this);
            i_this->mDemoMode = 15;
            i_this->field_0x10aa = 0;
        }
        break;

    case 15:
        cam_3d_morf(i_this, 0.1f);
        cLib_addCalc2(&i_this->field_0x10f8, 0.05f, 1.0f, 0.005f);
        if (i_this->field_0x10aa >= 40) {
            cLib_addCalc2(&i_this->mDemoCamFovy, 25.0f, 0.05f, 0.5f);
        }

        if (i_this->field_0x10aa == 179) {
            player->setPlayerPosAndAngle(&player->current.pos, -0x26AE, 0);
        }

        if (i_this->field_0x10aa == 180) {
            sVar1 = 1;
            fopAcM_delete(a_this);
            fopAcM_delete(fopAcM_SearchByName(PROC_E_RD));
            fopAcM_delete(fopAcM_SearchByName(PROC_E_WB));
            dComIfGs_onStageMiddleBoss();
            int swBit = fopAcM_GetParam(a_this) >> 24;
            if (swBit != 0xFF) {
                dComIfGs_onSwitch(swBit, fopAcM_GetRoomNo(a_this));
            }
        }
    }

    if (i_this->mDemoMode > 0 && i_this->mDemoMode < 10) {
        if (dComIfGp_getEvent().checkSkipEdge()) {
            sVar1 = 1;
            i_this->mAction = ACTION_WAIT;
            i_this->mMode = 10;
            Z2GetAudioMgr()->subBgmStart(Z2BGM_FACE_OFF_BATTLE3);
        }
    }

    if (sVar1) {
        Z2GetAudioMgr()->setDemoName("force_end");
        camera->mCamera.Reset(i_this->mDemoCamCenter, i_this->mDemoCamEye);
        camera->mCamera.Start();
        camera->mCamera.SetTrimSize(0);
        dComIfGp_event_reset();
        daPy_getPlayerActorClass()->cancelOriginalDemo();
        i_this->mDemoMode = 0;
    }

    if (i_this->mDemoMode > 0) {
        cXyz i_center = i_this->mDemoCamCenter;
        cXyz i_eye = i_this->mDemoCamEye;
        camera->mCamera.Set(i_center, i_eye, i_this->mDemoCamFovy, 0);
        i_this->field_0x10aa++;
        if (i_this->field_0x10aa > 10000) {
            i_this->field_0x10aa = 10000;
        }
    }
}

static void anm_se_set(e_rdb_class* i_this) {
    if (i_this->mAnm == e_rdb_class::BCK_RB_WALK) {
        if (i_this->mpModelMorf->checkFrame(29.0f) || i_this->mpModelMorf->checkFrame(59.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_RDB_FOOTNOTE, 0, -1);
        }
    } else if (i_this->mAnm == e_rdb_class::BCK_RB_RUN) {
        if (i_this->mpModelMorf->checkFrame(2.0f) || i_this->mpModelMorf->checkFrame(17.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_RDB_FOOTNOTE, 0, -1);
        }
    } else if (i_this->mAnm == e_rdb_class::BCK_RB_STEP) {
        if (i_this->mpModelMorf->checkFrame(15.0f) || i_this->mpModelMorf->checkFrame(35.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_RDB_FOOTNOTE, 0, -1);
        }
    } else if (i_this->mAnm == e_rdb_class::BCK_RB_ATTACK01) {
        if (i_this->mpModelMorf->checkFrame(0.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_V_ATTACK, -1);
        } else if (i_this->mpModelMorf->checkFrame(58.0f)) {
            i_this->mSound.startCreatureExtraSound(Z2SE_EN_RDB_ATTACK01, 0, -1);
        }
    } else if (i_this->mAnm == e_rdb_class::BCK_RB_ATTACK02) {
        if (i_this->mpModelMorf->checkFrame(0.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_V_ATTACK, -1);
        } else if (i_this->mpModelMorf->checkFrame(61.0f)) {
            i_this->mSound.startCreatureExtraSound(Z2SE_EN_RDB_ATTACK02, 0, -1);
        } else if (i_this->mpModelMorf->checkFrame(110.0f)) {
            i_this->mSound.startCreatureExtraSound(Z2SE_EN_RDB_ATTACK02B, 0, -1);
        }
    } else if (i_this->mAnm == e_rdb_class::BCK_RB_ATTACK03_END) {
        if (i_this->mpModelMorf->checkFrame(4.0f) || i_this->mpModelMorf->checkFrame(50.5f)) {
            i_this->mSound.startCreatureSound(Z2SE_EN_RDB_FOOTNOTE, 0, -1);
        }
    } else if (i_this->mAnm == e_rdb_class::BCK_RB_DOWN) {
        if (i_this->mpModelMorf->checkFrame(0.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_V_DAMAGE, -1);
        } else if (i_this->mpModelMorf->checkFrame(31.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_CM_BODYFALL_M, 0, -1);
        }
    } else if (i_this->mAnm == e_rdb_class::BCK_RB_DOWN_RETURN) {
        if (i_this->mpModelMorf->checkFrame(0.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_V_ED_DEMO_DOWNUP, -1);
        } else if (i_this->mpModelMorf->checkFrame(35.0f)) {
            i_this->mSound.startCreatureExtraSound(Z2SE_EN_RDB_ATTACK02, 0, -1);
        } else if (i_this->mpModelMorf->checkFrame(52.0f)) {
            i_this->mSound.startCreatureExtraSound(Z2SE_EN_RDB_ATTACK02B, 0, -1);
        } else if (i_this->mpModelMorf->checkFrame(77.0f)) {
            i_this->mSound.startCreatureSound(Z2SE_CM_BODYFALL_M, 0, -1);
        }
    } else if (i_this->mAnm == e_rdb_class::BCK_RB_PUSH) {
        if (i_this->mpModelMorf->checkFrame(11.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_V_PUSH, -1);
        }
    } else if (i_this->mAnm == e_rdb_class::BCK_RB_OP_DEMO_SWING) {
        if (i_this->mpModelMorf->checkFrame(0.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_V_OP_DEMO_SWING, -1);
            i_this->mSound.startCreatureExtraSound(Z2SE_EN_RDB_OP_DEMO_SWING, 0, -1);
        } else if ((i_this->mpModelMorf->checkFrame(83.0f) ||
                    i_this->mpModelMorf->checkFrame(124.0f)) ||
                   ((i_this->mpModelMorf->checkFrame(142.0f) ||
                     i_this->mpModelMorf->checkFrame(176.0f))))
        {
            i_this->mSound.startCreatureSound(Z2SE_EN_RDB_FOOTNOTE, 0, -1);
        }
    } else if (i_this->mAnm == e_rdb_class::BCK_RB_ED_DEMO_DOWNUP) {
        if (i_this->mpModelMorf->checkFrame(11.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_V_ED_DEMO_DOWNUP, -1);
        }
    } else if (i_this->mAnm == e_rdb_class::BCK_RB_ED_DEMO_DOWNWALK) {
        if (i_this->mpModelMorf->checkFrame(13.0f) || i_this->mpModelMorf->checkFrame(39.0f)) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_RDB_V_ED_DEMO_WALK, -1);
        } else if (i_this->mpModelMorf->checkFrame(12.0f) ||
                   ((i_this->mpModelMorf->checkFrame(18.0f) ||
                     i_this->mpModelMorf->checkFrame(36.0f)) ||
                    i_this->mpModelMorf->checkFrame(43.0f)))
        {
            i_this->mSound.startCreatureSound(Z2SE_EN_RDB_ED_FOOTNOTE, 0, -1);
        }
    }
}

static int daE_RDB_Execute(e_rdb_class* i_this) {
    if (i_this->field_0xfe6 != 0) {
        demo_camera(i_this);
        return 1;
    } else {
        if (cDmrNowMidnaTalk()) {
            return 1;
        }
        fopEn_enemy_c* a_this = (fopEn_enemy_c*)&i_this->enemy;

        cXyz sp64, sp70, sp7c;

        i_this->field_0x67c++;

        for (int i = 0; i < 4; i++) {
            if (i_this->field_0x6b8[i] != 0) {
                i_this->field_0x6b8[i]--;
            }
        }

        if (i_this->field_0x6c0 != 0) {
            i_this->field_0x6c0--;
        }

        if (i_this->field_0x6c2 != 0) {
            i_this->field_0x6c2--;
        }

        i_this->enemy.offHeadLockFlg();
        action(i_this);

        i_this->mAcch.CrrPos(dComIfG_Bgsp());
        mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z);
        mDoMtx_stack_c::YrotM((s16)a_this->shape_angle.y);
        mDoMtx_stack_c::XrotM((s16)a_this->shape_angle.x);
        mDoMtx_stack_c::ZrotM(a_this->shape_angle.z);
        f32 fVar1 = l_HIO.field_0x8 * a_this->scale.x;
        mDoMtx_stack_c::scaleM(fVar1, fVar1, fVar1);

        J3DModel* model = i_this->mpModelMorf->getModel();
        model->setBaseTRMtx(mDoMtx_stack_c::get());
        anm_se_set(i_this);
        i_this->mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
        i_this->mpModelMorf->modelCalc();
        MTXCopy(model->getAnmMtx(6 + AREG_S(9)), *calc_mtx);
        sp64.set(20.0f, 0.0f, 0.0f);
        MtxPosition(&sp64, &a_this->eyePos);
        a_this->attention_info.position = a_this->eyePos;
        a_this->attention_info.position.y += 30.0f;

        cXyz sp88(0.0f, 0.0f, 0.0f);
        if (i_this->field_0x6c0 != 0) {
            sp88.set(-20000.0f, 12000.0f, 30000.0f);
        }
        i_this->field_0x944[0].SetC(a_this->eyePos + sp88);
        i_this->field_0x944[0].SetR(35.0f);

        sp64.set(0.0f + ZREG_F(0), 0.0f + ZREG_F(1), 0.0f + ZREG_F(2));
        MTXCopy(model->getAnmMtx(2), *calc_mtx);
        MtxPosition(&sp64, &sp70);
        i_this->field_0x944[1].SetC(sp70 + sp88);
        i_this->field_0x944[1].SetR((40.0f + ZREG_F(3)) * l_HIO.field_0x8);

        sp64.set(0.0f + ZREG_F(4), 0.0f + ZREG_F(5), 0.0f + ZREG_F(6));
        MTXCopy(model->getAnmMtx(22), *calc_mtx);
        MtxPosition(&sp64, &sp70);
        i_this->field_0x944[2].SetC(sp70 + sp88);
        i_this->field_0x944[2].SetR((50.0f + ZREG_F(3)) * l_HIO.field_0x8);

        for (int i = 0; i <= 2; i++) {
            dComIfG_Ccsp()->Set(&i_this->field_0x944[i]);
        }

        cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
        sp64.x = 0.0f;
        sp64.y = 0.0f;
        sp64.z = 40.0f + KREG_F(12);
        MtxPosition(&sp64, &sp70);
        sp70 += a_this->current.pos;

        i_this->field_0xcec.SetC(sp70);
        i_this->field_0xcec.SetR(100.0f + NREG_F(7));
        i_this->field_0xcec.SetH(200.0f + NREG_F(8));
        dComIfG_Ccsp()->Set(&i_this->field_0xcec);

        if (i_this->field_0x680 == 1) {
            MTXCopy(model->getAnmMtx(15), *calc_mtx);
            i_this->mpAxeModel->setBaseTRMtx(*calc_mtx);
        } else if (i_this->field_0x680 >= 2) {
            i_this->field_0x684 += i_this->field_0x690;
            i_this->field_0x690.y -= 5.0f;
            if (i_this->field_0x684.y <= 5.0f + KREG_F(11)) {
                i_this->field_0x684.y = 5.0f + KREG_F(11);
                if (i_this->field_0x680 == 2) {
                    i_this->field_0x690.x *= 0.2f;
                    i_this->field_0x690.y = 20.0f;
                    i_this->field_0x690.z *= 0.2f;
                    i_this->field_0x680 = 3;
                    i_this->field_0x6a4 = 0;
                    i_this->field_0x6a2 = 0;
                } else {
                    i_this->field_0x690.x = 0.0f;
                    i_this->field_0x690.y = 0.0f;
                    i_this->field_0x690.z = 0.0f;
                }

                i_this->field_0x69c.x += i_this->field_0x6a2;
                if (i_this->field_0x69c.x >= 0x4000) {
                    if (i_this->field_0x680 == 3) {
                        i_this->field_0x680 = 4;
                        i_this->field_0x69c.x = 0x4000;
                        i_this->field_0x6a2 = -0x600;
                        i_this->field_0x6a8 = 30;
                        i_this->mSound.startCreatureSound(Z2SE_OBJ_KEYLOCK_BOUND, 0, -1);
                        sp70.set(2834.0f, 0.0f, 993.0f);
                        fopAcM_effSmokeSet1(&i_this->field_0xfd0[0], &i_this->field_0xfd0[1], &sp70,
                                            NULL, 1.5f + TREG_F(12), &a_this->tevStr, 1);
                        i_this->field_0xfc0 = i_this->field_0x684;
                        i_this->mSound.startCreatureExtraSound(Z2SE_EN_RDB_DEMO_ONO_LAND, 0, -1);
                    } else if (i_this->field_0x680 == 4) {
                        i_this->field_0x680 = 5;
                        i_this->field_0x69c.x = 0x4000;
                        i_this->field_0x6a2 = -0x300;
                    } else {
                        i_this->field_0x69c.x = 0x4000;
                    }
                }

                cLib_addCalcAngleS2(&i_this->field_0x6a2, 0x800, 1, 0x100);
            } else {
                i_this->field_0x69c.y += i_this->field_0x6a4;
                i_this->field_0x69c.x += i_this->field_0x6a2;
            }

            mDoMtx_stack_c::transS(i_this->field_0x684.x, i_this->field_0x684.y,
                                   i_this->field_0x684.z);
            mDoMtx_stack_c::YrotM(i_this->field_0x69c.y);
            mDoMtx_stack_c::XrotM(i_this->field_0x69c.x);
            mDoMtx_stack_c::ZrotM(i_this->field_0x69c.z);
            mDoMtx_stack_c::transM(0.0f, -80.0f + nREG_F(7), 0.0f);

            s16 sVar1 = 0;
            if (i_this->field_0x6a8 != 0) {
                sVar1 = i_this->field_0x6a8 * cM_ssin(i_this->field_0x6a8 * (TREG_S(9) + 7000)) *
                        (200.0f + TREG_F(5));
                i_this->field_0x6a8--;
            }

            mDoMtx_stack_c::YrotM(sVar1);
            mDoMtx_stack_c::scaleM(fVar1, fVar1, fVar1);
            i_this->mpAxeModel->setBaseTRMtx(mDoMtx_stack_c::get());
        }

        if (i_this->field_0x6c4 != 0) {
            sp64.set(0.0f + AREG_F(0), -150.0f + AREG_F(1), 0.0f + AREG_F(2));
            MtxPosition(&sp64, &sp70);
            i_this->field_0xfc0 = sp70;

            if (i_this->field_0x6c5 == 0) {
                i_this->field_0x6c5 = 1;
                i_this->field_0xe64.StartCAt(sp70);
            } else {
                i_this->field_0xe64.MoveCAt(sp70);
            }

            i_this->field_0xe64.SetR((100.0f + AREG_F(3)) * l_HIO.field_0x8);
            i_this->field_0x6c4 = 0;
            i_this->field_0xe64.OnAtSetBit();
            i_this->field_0x6c6 = 2;
        } else {
            i_this->field_0x6c5 = 0;
            i_this->field_0xe64.OffAtSetBit();
        }

        if (i_this->field_0x6c6 != 0) {
            if (i_this->field_0x6c6 == 1) {
                sp64.set(0.0f + AREG_F(5), -60.0f + AREG_F(6), 10.0f + AREG_F(7));
                MtxPosition(&sp64, &sp70);
                i_this->field_0xe64.SetC(sp70);
                i_this->field_0xe64.SetR((50.0f + AREG_F(8)) * l_HIO.field_0x8);
            }

            i_this->field_0x6c6 = 0;
            i_this->field_0xe64.OnTgSetBit();
        } else {
            sp70.x += -10000.0f;
            i_this->field_0xe64.SetC(sp70);
        }
        dComIfG_Ccsp()->Set(&i_this->field_0xe64);

        demo_camera(i_this);

        if (strcmp(dComIfGp_getStartStageName(), "T_DESER") == 0 ||
            strcmp(dComIfGp_getStartStageName(), "F_SP118") == 0)
        {
            f32 fVar2 = 0.0f;
            f32 fVar3 = 0.05f;
            int sVar2 = i_this->field_0x6d8;
            if (sVar2 == 0) {
                dKy_custom_colset(0, 8, i_this->mBlend);
            } else if (sVar2 == 1) {
                dKy_custom_colset(0, 8, i_this->mBlend);
                fVar2 = 1.0f;
                fVar3 = 0.1f + JREG_F(8);
            } else if (sVar2 == 2) {
                i_this->mBlend = 0.0f;
                dKy_custom_colset(9, 8, i_this->mBlend);
            } else if (sVar2 == 3) {
                dKy_custom_colset(9, 10, i_this->mBlend);
                fVar2 = 1.0f;
                fVar3 = 0.02f + JREG_F(10);
            } else if (sVar2 == 4) {
                dKy_custom_colset(11, 10, i_this->mBlend);
                fVar2 = 0.0f;
                fVar3 = 0.019999999552965164f;
            } else {
                fVar2 = 0.0f;
            }

            cLib_addCalc2(&i_this->mBlend, fVar2, 1.0f, fVar3);
        }

        static const cXyz fire_eff_pos[24] = {
            cXyz(2573.0f, 370.0f, 2200.0f), cXyz(2901.0f, 600.0f, 2200.0f),
            cXyz(3100.0f, 222.0f, 1888.0f), cXyz(1900.0f, 555.0f, 2200.0f),
            cXyz(3100.0f, 600.0f, 1600.0f), cXyz(1783.0f, 355.0f, 2200.0f),
            cXyz(1555.0f, 200.0f, 2200.0f), cXyz(3100.0f, 350.0f, 1332.0f),
            cXyz(3100.0f, 550.0f, 1070.0f), cXyz(1375.0f, 600.0f, 2200.0f),
            cXyz(3100.0f, 250.0f, 850.0f),  cXyz(1165.0f, 430.0f, 2200.0f),
            cXyz(3100.0f, 450.0f, 450.0f),  cXyz(3100.0f, 600.0f, 200.0f),
            cXyz(700.0f, 260.0f, 2200.0f),  cXyz(3100.0f, 300.0f, -150.0f),
            cXyz(500.0f, 522.0f, 1888.0f),  cXyz(500.0f, 300.0f, 1632.0f),
            cXyz(500.0f, 400.0f, 1332.0f),  cXyz(500.0f, 550.0f, 1070.0f),
            cXyz(500.0f, 350.0f, 850.0f),   cXyz(500.0f, 550.0f, 450.0f),
            cXyz(500.0f, 300.0f, 200.0f),   cXyz(500.0f, 600.0f, -150.0f),
        };

        if (i_this->field_0xfe4 != 0) {
            csXyz cStack_c0(0, 0, 0);
            for (int i = 0; i < i_this->field_0xfe4; i++) {
                if (fire_eff_pos[i].x == 3100.0f) {
                    cStack_c0.y = -0x4000;
                } else if (fire_eff_pos[i].x == 500.0f) {
                    cStack_c0.y = 0x4000;
                } else {
                    cStack_c0.y = -0x8000;
                }

                i_this->field_0xfe8[i] = dComIfGp_particle_set(i_this->field_0xfe8[i], 0x85C2,
                                                               &fire_eff_pos[i], &cStack_c0, NULL);
                i_this->field_0x1048[i] = dComIfGp_particle_set(i_this->field_0x1048[i], 0x85C3,
                                                                &fire_eff_pos[i], &cStack_c0, NULL);

                Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_CRVN_BURNING, &fire_eff_pos[i], 0, 0, 1.0f,
                                              1.0f, -1.0f, -1.0f, 0);
            }

            if ((i_this->field_0x67c & 15) == 0 && i_this->field_0xfe4 < 24) {
                i_this->field_0xfe4++;
            }
        }

        cXyz i_pos(a_this->eyePos);
        i_pos.y += 180.0f + NREG_F(7);
        i_this->enemy.setHeadLockPos(&i_pos);
    }
    return 1;
}

static int daE_RDB_IsDelete(e_rdb_class* i_this) {
    return 1;
}

static int daE_RDB_Delete(e_rdb_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fopAcM_GetID(i_this);
    dComIfG_resDelete(&i_this->mPhase, "E_rdb");
    if (i_this->field_0xfce != 0) {
        s_HIOinit = 0;
        mDoHIO_DELETE_CHILD(l_HIO.mID);
    }

    if (a_this->heap != NULL) {
        i_this->mpModelMorf->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* actor) {
    e_rdb_class* i_this = (e_rdb_class*)actor;
    i_this->mpModelMorf =
        new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_rdb", e_rdb_class::BMDR_RB), NULL, NULL,
                             (J3DAnmTransform*)dComIfG_getObjectRes("E_rdb", e_rdb_class::BCK_RB_WAIT01), 2, 1.0f, 0, -1,
                             &i_this->mSound, 0x80000, 0x11000084);
    if (i_this->mpModelMorf == NULL || i_this->mpModelMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = i_this->mpModelMorf->getModel();
    model->setUserArea((uintptr_t)i_this);
    for (u16 i_idx = 0; i_idx < model->getModelData()->getJointNum(); i_idx++) {
        model->getModelData()->getJointNodePointer(i_idx)->setCallBack(nodeCallBack);
    }

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_rdb", e_rdb_class::BMDR_RB_ONO);
    JUT_ASSERT(2827, modelData != NULL);
    i_this->mpAxeModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (i_this->mpAxeModel != NULL) {
        return 1;
    }

    return 0;
}

static int daE_RDB_Create(fopAc_ac_c* actor) {
    static dCcD_SrcSph cc_sph_src = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0xd8fafdbf, 0x3}, 0x0}},  // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x102},              // mGObjTg
            {0x0},                                             // mGObjCo
        },                                                     // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 40.0f}  // mSph
        }  // mSphAttr
    };

    static dCcD_SrcSph at_sph_src = {
        {
            {0x0, {{AT_TYPE_CSTATUE_SWING, 0xc, 0xd}, {0xd8fafdbf, 0x3}, 0x0}},  // mObj
            {dCcD_SE_METAL, 0x0, 0x2, 0x0, 0x0},                                 // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x102},                                // mGObjTg
            {0x0},                                                               // mGObjCo
        },                                                                       // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 35.0f}  // mSph
        }  // mSphAttr
    };

    static dCcD_SrcCyl co_cyl_src = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0x0, 0x0}, 0x75}},  // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},          // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},          // mGObjTg
            {0x0},                                       // mGObjCo
        },                                               // mObjInf
        {
            {0.0f, 0.0f, 0.0f},  // mCenter
            100.0f,              // mRadius
            200.0f               // mHeight
        }  // mCyl
    };

    e_rdb_class* i_this = (e_rdb_class*)actor;
    fopAcM_ct(actor, e_rdb_class);

    int phase = dComIfG_resLoad(&i_this->mPhase, "E_rdb");
    i_this->field_0x5b6 = fopAcM_GetParam(actor);
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("E_rdb PARAM %x\n", fopAcM_GetParam(actor));
        if (strcmp(dComIfGp_getStartStageName(), "D_MN09") == 0 && dComIfGs_isStageMiddleBoss()) {
            return cPhs_ERROR_e;
        }

        int i_no = fopAcM_GetParam(actor) >> 24;
        if (i_no != 0xFF) {
            if (dComIfGs_isSwitch(i_no, fopAcM_GetRoomNo(actor))) {
                return cPhs_ERROR_e;
            }
        }

        if (!fopAcM_entrySolidHeap(actor, useHeapInit, 0x3C00)) {
            OS_REPORT("//////////////E_RDB SET NON !!\n");
            return cPhs_ERROR_e;
        }

        i_this->mAction = ACTION_WAIT;
        i_this->mMode = 0;

        if (s_HIOinit == 0) {
            i_this->field_0xfce = 1;
            s_HIOinit = 1;
            l_HIO.mID = -1;
        }

        fopAcM_SetMtx(actor, i_this->mpModelMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(actor, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(actor, 200.0f, 200.0f, 200.0f);
        i_this->mAcch.Set(fopAcM_GetPosition_p(actor), fopAcM_GetOldPosition_p(actor), actor, 1,
                          &i_this->mAcchCir, fopAcM_GetSpeed_p(actor), NULL, NULL);
        i_this->mAcchCir.SetWall(50.0f, 150.0f);

        if (strcmp(dComIfGp_getStartStageName(), "D_MN09") == 0) {
            actor->health = 900;
            actor->field_0x560 = 900;
        } else {
            actor->health = 600;
            actor->field_0x560 = 600;
        }

        i_this->mStts.Init(200, 0, actor);

        for (int i = 0; i <= 2; i++) {
            i_this->field_0x944[i].Set(cc_sph_src);
            i_this->field_0x944[i].SetStts(&i_this->mStts);
            i_this->field_0x944[i].OnTgNoHitMark();
        }

        i_this->mStts2.Init(0xFF, 0, actor);
        i_this->field_0xe64.Set(at_sph_src);
        i_this->field_0xe64.SetStts(&i_this->mStts2);
        i_this->field_0xe64.OnTgShield();
        i_this->field_0xe64.SetTgHitMark((CcG_Tg_HitMark)2);
        i_this->field_0xe64.SetAtSpl((dCcG_At_Spl)11);

        i_this->field_0xcec.Set(co_cyl_src);
        i_this->field_0xcec.SetStts(&i_this->mStts2);

        i_this->mSound.init(&actor->current.pos, &actor->eyePos, &i_this->field_0xfc0, 3, 1, 1);
        i_this->mSound.setEnemyName("E_rdb");
        i_this->mAtInfo.mpSound = &i_this->mSound;
        i_this->mAtInfo.mPowerType = 6;

        if (strcmp(dComIfGp_getStartStageName(), "D_MN09") == 0) {
            i_this->mAction = ACTION_START;
            i_this->mMode = 10;
            actor->current.pos.set(-7625.0f, 50.0f, -5825.0f);
            actor->current.angle.y = 0;
            actor->old = actor->current;
            actor->home = actor->current;
        } else if (i_this->field_0x5b6 == 0xFF) {
            i_this->field_0xfcf = 1;
            i_this->mAction = ACTION_START;
            actor->current.angle.y = 26000;
            i_this->field_0x6e0 = -50.0f;
        } else {
            i_this->mAction = ACTION_WAIT;
        }

        i_this->field_0x680 = 1;
        daE_RDB_Execute(i_this);
    }

    return phase;
}

static s32 unused_bss_29c = 0;

AUDIO_INSTANCES

static actor_method_class l_daE_RDB_Method = {
    (process_method_func)daE_RDB_Create,  (process_method_func)daE_RDB_Delete,
    (process_method_func)daE_RDB_Execute, (process_method_func)daE_RDB_IsDelete,
    (process_method_func)daE_RDB_Draw,
};

extern actor_process_profile_definition g_profile_E_RDB = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_E_RDB,              // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(e_rdb_class),     // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    156,                     // mPriority
    &l_daE_RDB_Method,       // sub_method
    0x00044100,              // mStatus
    fopAc_ENEMY_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
