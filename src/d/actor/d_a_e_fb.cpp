/**
 * @file d_a_e_fb.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_fb.h"
#include "Z2AudioLib/Z2Instances.h"
#include "d/actor/d_a_obj_carry.h"
#include "d/d_s_play.h"

#if DEBUG
#include "d/d_debug_viewer.h"
#endif

daE_FB_HIO_c::daE_FB_HIO_c() {
    mId = -1;
    model_size = 1.5f;
    player_detection_range = 1500.0f;
    next_attack_waiting_time = 30;
    maximum_rotation_width = 0x300;
    minimum_turning_range = 0x10;
    rotation_width_stairs = 120;
    color_register_1r = 0x87;
    color_register_1g = 0x96;
    color_register_1b = 0x91;
    color_register_1a = 0xB6;
    maximum_rotation_width_2 = 120;
}

#if DEBUG
void daE_FB_HIO_c::genMessage(JORMContext* ctext) {
    // "Frizzard (large)"
    ctext->genLabel("フリザド（大）", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // "Model size"
    ctext->genSlider("モデルサイズ", &model_size, 0.0f, 10.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // "Player detection range"
    ctext->genSlider("プレイヤー発見距離", &player_detection_range, 0.0f, 10000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // "Next attack wait time"
    ctext->genSlider("次回攻撃待ち時間", &next_attack_waiting_time, 0, 1000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // "Maximum rotation width"
    ctext->genSlider("最大回転幅", &maximum_rotation_width, 0, 10000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // "Minimum turning range"
    ctext->genSlider("最小回転幅", &minimum_turning_range, 0, 10000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // "Stage rotation amplitude (speed)"
    ctext->genSlider("階段用回転幅（速度）", &rotation_width_stairs, 0, 10000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // "Color Regi 1R"
    ctext->genSlider("カラレジ１Ｒ", &color_register_1r, 0, 0xFF, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // "Color Regi 1G"
    ctext->genSlider("カラレジ１Ｇ", &color_register_1g, 0, 0xFF, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // "Color Regi 1B"
    ctext->genSlider("カラレジ１Ｂ", &color_register_1b, 0, 0xFF, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // "Color Regi 1α "
    ctext->genSlider("カラレジ１α　", &color_register_1a, 0, 0xFF, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // "Maximum rotation width"
    ctext->genSlider("最大回転幅", &maximum_rotation_width_2, 0, 180, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
}
#endif


namespace {
    dCcD_SrcSph cc_fb_src = {
        {
            {0, {{AT_TYPE_100, 1, 0x1D}, {0x48FBFDFF, 0x43}, 0x75}},
            {dCcD_SE_METAL, 0, 0, 2, {0}},
            {dCcD_SE_NONE, 2, 0, 0, {0x103}},
            {0},
        },
        {{
            {0.0f, 0.0f, 0.0f},
            40.0f,
        }},
    };
    dCcD_SrcSph cc_fb_at_src = {
        {
            {0, {{AT_TYPE_100, 1, 0x1D}, {0, 0}, 0}},
            {dCcD_SE_METAL, 0, 0xC, 2, {0}},
            {dCcD_SE_NONE, 0, 0, 0, {0x2}},
            {0},
        },
        {{
            {0.0f, 0.0f, 0.0f},
            40.0f,
        }},
    };
}

int daE_FB_c::ctrlJoint(J3DJoint* i_joint, J3DModel* i_model) {
    J3DJoint* my_joint = i_joint;
    int jnt_no = my_joint->getJntNo();
    mDoMtx_stack_c::copy(i_model->getAnmMtx(jnt_no));
    switch (jnt_no) {
    case 2:
        mDoMtx_stack_c::ZrotM(mHeadAngle);
    }

#if DEBUG
    if (TREG_S(0) && jnt_no == TREG_S(0)) {
        mDoMtx_stack_c::XrotM(TREG_S(1));
        mDoMtx_stack_c::YrotM(TREG_S(2));
        mDoMtx_stack_c::ZrotM(TREG_S(3));
    }
#endif

    i_model->setAnmMtx(jnt_no, mDoMtx_stack_c::get());
    cMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    return 1;
}

int daE_FB_c::JointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DModel* model = j3dSys.getModel();
        daE_FB_c* freezard_p = (daE_FB_c*) model->getUserArea();
        if (freezard_p) {
            freezard_p->ctrlJoint(i_joint, model);
        }
    }

    return 1;
}

#if DEBUG
static f32 dummy_117095() {
    // For rodata ordering in the debug version, this must be put here:
    return 100.0f;
}
#endif

bool hio_set;

static daE_FB_HIO_c l_HIO;

int daE_FB_c::draw() {
    if (mType == 10 || mType == 11) {
        return 1;
    }

    J3DModel* model = mpMorf->getModel();
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);

    J3DModelData* modelData = model->getModelData();
    J3DMaterial* material = modelData->getMaterialNodePointer(0);
    material->getTevColor(1)->r = l_HIO.color_register_1r;
    material->getTevColor(1)->g = l_HIO.color_register_1g;
    material->getTevColor(1)->b = l_HIO.color_register_1b;
    material->getTevColor(1)->a = l_HIO.color_register_1a;

    mpBrkAnm->entry(model->getModelData());
    mInvisibleModel.entryDL(NULL);
    cXyz my_vec;
    my_vec.set(current.pos.x, 10.0f + current.pos.y, current.pos.z);

    #if DEBUG
    if (WREG_S(0) != 0) {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        cXyz cStack_44;
        cStack_44.set(player->current.pos);
        cStack_44.y += 100.0f;
        GXColor local_48;
        local_48.r = 0x82;
        local_48.g = 0xff;
        local_48.b = 0x82;
        local_48.a = 0xfa;
        dDbVw_drawLineXlu(attention_info.position, cStack_44, local_48, 1, 0xc);
    }
    #endif

    GXTexObj* tex_obj = dDlst_shadowControl_c::getSimpleTex();
    mShadowKey =
        dComIfGd_setShadow(mShadowKey, 1, model, &my_vec, BREG_F(19) + 1300.0f, 0.0f, current.pos.y,
                           mObjAcch.GetGroundH(), mObjAcch.m_gnd, &tevStr, 0, 1.0f, tex_obj);
    return 1;
}

static int daE_FB_Draw(daE_FB_c* i_this) {
    return static_cast<daE_FB_c*>(i_this)->draw();
}

void daE_FB_c::setBck(int i_index, u8 i_attr, f32 i_morf, f32 i_rate) {
    mpMorf->setAnm((J3DAnmTransform *) dComIfG_getObjectRes("E_FL", i_index), i_attr, i_morf, i_rate, 0.0f, -1.0f);
    field_0x670 = i_index;
}

void daE_FB_c::setActionMode(int i_action, int i_mode) {
    mActionMode = i_action;
    mMoveMode = i_mode;
}

void daE_FB_c::damage_check() {
    if (mType == 10 || mType == 11) {
        return;
    }

    fopAc_ac_c* tg_hit_ac = NULL;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    mStts.Move();
    field_0x560 = health = 200;
    if (mSphere.ChkTgHit()) {
        mAtInfo.mpCollider = mSphere.GetTgHitObj();
        if (mSphere.GetTgHitObj()->ChkAtType(AT_TYPE_IRON_BALL)) {
            cCcD_Obj* hit_obj = mSphere.GetTgHitObj();
            if (!field_0x68c && mActionMode != 2) {
                tg_hit_ac = mSphere.GetTgHitAc();
                ++field_0x68e;
                if (player == tg_hit_ac) {
                    ++field_0x68e;
                } else if (((daObjCarry_c*)tg_hit_ac)->checkCannon()) {
                    field_0x68e = 3;
                }

                if (field_0x68e > 2) {
                    health = 0;
                    dComIfGp_getVibration().StartShock(VIBMODE_S_POWER5, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
                    fopAcM_OffStatus(this, 0);
                    attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
                    mSphere.OffAtSetBit();
                    mSphere.OffTgSetBit();
                    mSphere2.OffAtSetBit();
                    mSphere2.OffTgSetBit();
                    mCreatureSound.startCollisionSE(Z2SE_HIT_HAMMER, 0x20);
                    mCreatureSound.startCreatureSound(Z2SE_EN_FL_DEATH, 0, -1);
                } else {
                    mCreatureSound.startCollisionSE(Z2SE_HIT_HAMMER, 0x1E);
                    mCreatureSound.startCreatureSound(Z2SE_EN_FL_DAMAGE, 0, -1);
                }

                setActionMode(2, 0);
            } else {
                mCreatureSound.startCollisionSE(dCcD_GObjInf::getHitSeID(((dCcD_GObjInf*) hit_obj)->GetAtSe(), 0), 0x2A);
            }

            mSphere.ClrTgHit();
        }
    }

    if (mSphere2.ChkTgHit()) {
        mAtInfo.mpCollider = mSphere2.GetTgHitObj();
        if (mSphere2.GetTgHitObj()->ChkAtType(AT_TYPE_IRON_BALL)) {
            cCcD_Obj* hit_obj = mSphere2.GetTgHitObj();
            if (!field_0x68c && mActionMode != 2) {
                tg_hit_ac = mSphere2.GetTgHitAc();
                ++field_0x68e;
                if (player == tg_hit_ac) {
                    ++field_0x68e;
                } else if (((daObjCarry_c*)tg_hit_ac)->checkCannon()) {
                    field_0x68e = 3;
                }

                if (field_0x68e > 2) {
                    health = 0;
                    dComIfGp_getVibration().StartShock(VIBMODE_S_POWER5, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
                    fopAcM_OffStatus(this, 0);
                    attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
                    mSphere.OffAtSetBit();
                    mSphere.OffTgSetBit();
                    mSphere2.OffAtSetBit();
                    mSphere2.OffTgSetBit();
                    mCreatureSound.startCollisionSE(Z2SE_HIT_HAMMER, 0x20);
                    mCreatureSound.startCreatureSound(Z2SE_EN_FL_DEATH, 0, -1);
                } else {
                    mCreatureSound.startCollisionSE(Z2SE_HIT_HAMMER, 0x1E);
                    mCreatureSound.startCreatureSound(Z2SE_EN_FL_DAMAGE, 0, -1);
                }

                setActionMode(2, 0);
            } else {
                mCreatureSound.startCollisionSE(dCcD_GObjInf::getHitSeID(((dCcD_GObjInf*) hit_obj)->GetAtSe(), 0), 0x2A);
            }

            mSphere2.ClrTgHit();
        }
    }
}

bool daE_FB_c::mBgLineCheck() {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    dBgS_LinChk linchk;
    cXyz vec;
    vec.set(player->current.pos);
    vec.y += 100.0f;
    linchk.Set(&attention_info.position, &vec, this);
    if (dComIfG_Bgsp().LineCross(&linchk) && !dComIfGp_checkPlayerStatus0(0, 0x02000000)) {
        return true;
    }

    return false;
}

bool daE_FB_c::search_check() {
    bool retval = false;
    if (fopAcM_searchPlayerDistance(this) <= l_HIO.player_detection_range) {
        if (!field_0x69c) {
            mRotation = fopAcM_searchPlayerAngleY(this);
            field_0x69c = 0x1E;
            field_0x696 = 0;
        }

        retval = true;
    } else if (mActionMode != 1 && !field_0x69c) {
        mRotation = home.angle.y;
        field_0x69c = 0x1E;
        field_0x696 = 0;
    }

    cLib_addCalcAngleS2(&field_0x696, l_HIO.maximum_rotation_width, 1, l_HIO.minimum_turning_range);
    cLib_addCalcAngleS2(&shape_angle.y, mRotation, 1, field_0x696);
    if (retval == true && abs(s16(shape_angle.y - mRotation)) > 0x200) {
        mCreatureSound.startCreatureSoundLevel(Z2SE_EN_FL_ROTATE, 0, -1);
        retval = false;
    }

    return retval;
}

void daE_FB_c::executeWait() {
    if (field_0x680) {
        --field_0x680;
    }

    switch (mMoveMode) {
    case 0:
        field_0x680 = l_HIO.next_attack_waiting_time;
        /* fallthrough */
    case 2:
        setBck(8, 2, 6.0f, 1.0f);
        mRotation = home.angle.y;
        mMoveMode = 1;
        /* fallthrough */
    case 1:
        if (mType == 1) {
            fopAc_ac_c* player = dComIfGp_getPlayer(0);
            if (fopAcM_searchPlayerDistanceY(this) > 300.0f) {
                break;
            }

            if (fopAcM_searchPlayerDistanceY(this) < -300.0f) {
                break;
            }

            if (current.pos.y <= 300.0f) {
                if (player->current.pos.x > -2800.0f) {
                    break;
                }
            } else if (player->current.pos.x < -3600.0f) {
                break;
            }

            setActionMode(1, 0);
        } else if (search_check() && !field_0x680) {
            if (mType == 0) {
                if (mBgLineCheck()) {
                    break;
                }
            } else if (mType == 2) {
                if (BREG_S(9)) {
                    OS_REPORT("fopAcM_searchPlayerDistanceY(this) %f\n", fopAcM_searchPlayerDistanceY(this));
                }

                if (fopAcM_searchPlayerDistanceY(this) > 300.0f) {
                    break;
                }

                if (fopAcM_searchPlayerDistanceY(this) < -700.0f + JREG_F(1)) {
                    break;
                }
            }

            setActionMode(1, 0);
        }

        break;
    }

    current.angle.y = shape_angle.y;
}

static int mFireTimer;

void daE_FB_c::executeAttack() {
    static u16 a_eff_id[3] = {
        dPa_RM(ID_ZI_S_FL_BREATH_A),
        dPa_RM(ID_ZI_S_FL_BREATH_B),
        dPa_RM(ID_ZI_S_FL_BREATH_C),
    };
    cXyz sp_0x48;
    cXyz child_pos; // dbg sp 0x3C
    cXyz sp_0x30(mModelSize, mModelSize, mModelSize);

    switch (mMoveMode) {
    case 0:
        field_0x68f = 0;
        mFireTimer = 0;
        field_0x690 = 0;
        if (mType == 2 && dComIfGp_checkPlayerStatus0(0, 0x02000000)) {
            setBck(6, 2, 6.0f, 1.0f);
            mMoveMode = 2;
        } else if (mType == 1) {
            setBck(6, 2, 6.0f, 1.0f);
            mMoveMode = 3;
            mRotation = home.angle.y - ((l_HIO.maximum_rotation_width_2 >> 1) * 0xB6);
            if (BREG_S(6)) {
                OS_REPORT("\n\n");
                // "First rotation %d\n"
                OS_REPORT("最初の回転 %d\n", mRotation);
                // "First rotation %x\n"
                OS_REPORT("最初の回転 %x\n", mRotation);
                OS_REPORT("\n\n");
            }
        } else {
            setBck(5, 0, 6.0f, 1.0f);
            mCreatureSound.startCreatureSound(Z2SE_EN_FL_BLIZZARD, 0, -1);
            mMoveMode = 1;
        }

        break;
    case 1:
    case 3:
        if (field_0x670 == 5) {
            if (int(mpMorf->getFrame()) == 160) {
                mCreatureSound.startCreatureSound(Z2SE_EN_FL_BLIZZARD_END, 0, -1);
            }
        }
        /* fallthrough */
    case 2:
        if (mMoveMode != 3 && search_check()) {
            // empty
        }

        if (mType == 0 && mBgLineCheck()) {
            field_0x69c = 0;
            field_0x680 = l_HIO.next_attack_waiting_time;
            setActionMode(0, 0);
            break;
        }

        if ((mpMorf->getFrame() >= 36.0f && mpMorf->getFrame() <= 160.0f) || mMoveMode == 2 || mMoveMode == 3) {
            for (int idx = 0; idx < 3; ++idx) {
                mKeys[idx + 5] = dComIfGp_particle_set(mKeys[idx + 5], a_eff_id[idx],
                                                       &current.pos, &shape_angle, &sp_0x30);
                JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(mKeys[idx + 5]);
                if (emitter != NULL) {
                    emitter->setGlobalSRTMatrix(mpMorf->getModel()->getAnmMtx(2));
                }
            }

            mCreatureSound.startCreatureVoiceLevel(Z2SE_EN_FL_BREATH, -1);
            ++field_0x68f;
            field_0x68f &= (u8) 1;
            if (field_0x68f == 0) {
                csXyz sp_0x28;
                u32 child_type = 10;
                sp_0x28 = shape_angle;
                sp_0x28.x = mHeadAngle + 700 + JREG_S(1);
                if (mType == 1) {
                    if (current.pos.y <= 300.0f) {
                        sp_0x28.x = mHeadAngle + 2000 + BREG_S(2);
                    } else {
                        sp_0x28.x = mHeadAngle + 2000 + BREG_S(3);
                    }
                }

                if (!dComIfGp_event_runCheck()) {
                    cMtx_YrotS(*calc_mtx, current.angle.y);
                    cMtx_XrotM(*calc_mtx, sp_0x28.x);
                    sp_0x48.x = 0.0f;
                    sp_0x48.y = 250.0f + BREG_F(2);
                    sp_0x48.z = 200.0f + BREG_F(3);
                    MtxPosition(&sp_0x48, &child_pos);
                    child_pos += current.pos;
                    if (mType == 1) {
                        child_type = 11;
                    }

                    fopAcM_createChild(PROC_E_FB, fopAcM_GetID(this), child_type, &child_pos,
                                       fopAcM_GetRoomNo(this), &sp_0x28, NULL, -1, NULL);
                }
            }
        }

        if (mMoveMode == 3) {
            fopAc_ac_c* player = (fopAc_ac_c*) dComIfGp_getPlayer(0);
            cLib_addCalcAngleS2(&shape_angle.y, mRotation, 1, l_HIO.rotation_width_stairs);
            if (current.pos.y <= 300.0f) {
                mHeadAngle = f32(NREG_S(1) + 14000 - abs(s16(shape_angle.y))) / (6.0f + NREG_F(1));
                if (player->current.pos.x > -2800.0f) {
                    field_0x69c = 0;
                    current.angle.y = shape_angle.y;
                    setActionMode(0, 0);
                    return;
                }
            } else {
                mHeadAngle = NREG_S(2) - 2500;
                mHeadAngle -= abs(s16(shape_angle.y)) * (-0.2f + NREG_F(2));
                if (player->current.pos.x < -3600.0f) {
                    field_0x69c = 0;
                    current.angle.y = shape_angle.y;
                    setActionMode(0, 0);
                    return;
                }
            }

            if (fopAcM_searchPlayerDistanceY(this) > 300.0f
                || fopAcM_searchPlayerDistanceY(this) < -300.0f) {
                field_0x69c = 0;
                current.angle.y = shape_angle.y;
                setActionMode(0, 0);
                return;
            }

            if (BREG_S(9)) {
                OS_REPORT("shape_angle.sy %x\n", abs(shape_angle.y));
                OS_REPORT("mHeadAngle %x\n\n", mHeadAngle);
            }

            mCreatureSound.startCreatureSoundLevel(Z2SE_EN_FL_ROTATE, 0, -1);
            if (abs(s16(shape_angle.y - mRotation)) < 0x200) {
                if (field_0x690 == 0) {
                    mRotation = home.angle.y + (l_HIO.maximum_rotation_width_2 >> 1) * 0xB6;
                } else {
                    mRotation = home.angle.y - (l_HIO.maximum_rotation_width_2 >> 1) * 0xB6;
                }

                ++field_0x690;
                field_0x690 &= (u8) 1;
                if (BREG_S(6)) {
                    OS_REPORT("\n\n");
                    // "Next rotation %d\n"
                    OS_REPORT("次回転 %d\n", mRotation);
                    // "Next rotation %x\n"
                    OS_REPORT("次回転 %x\n", mRotation);
                    OS_REPORT("\n\n");
                }
            }
        }

        if (mMoveMode == 1 && mpMorf->isStop()) {
            field_0x69c = 0;
            field_0x680 = l_HIO.next_attack_waiting_time;
            setActionMode(0, 0);
        } else if (mMoveMode == 2 && dComIfGp_checkPlayerStatus0(0, 0x02000000) == FALSE
                    && fopAcM_searchPlayerDistanceY(this) < -900.0f) {
            if (NREG_S(6)) {
                OS_REPORT("\n\n");
                OS_REPORT("fopAcM_searchPlayerDistanceY %f\n", fopAcM_searchPlayerDistanceY(this));
                OS_REPORT("\n\n");
            }

            field_0x69c = 0;
            field_0x680 = l_HIO.next_attack_waiting_time;
            setActionMode(0, 0);
        }
    }

    current.angle.y = shape_angle.y;
}

void daE_FB_c::executeDamage() {
    static u16 d_eff_id[2] = {
        dPa_RM(ID_ZI_S_FL_HIT_A),
        dPa_RM(ID_ZI_S_FL_HIT_B)
    };
    cXyz sp_0x34(mModelSize, mModelSize, mModelSize);
    int keys[2];
    switch (mMoveMode) {
    case 0:
        for (int idx = 0; idx < 2; ++idx) {
            keys[idx] = dComIfGp_particle_set(keys[idx], d_eff_id[idx],
                                              &current.pos, &shape_angle, &sp_0x34);
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(keys[idx]);
            if (emitter != NULL) {
                emitter->setGlobalSRTMatrix(mpMorf->getModel()->getAnmMtx(1));
            }
        }

        if (field_0x68e <= 2) {
            setBck(7, 0, 3.0f, 1.0f);
            mMoveMode = 1;
        } else {
            csXyz mini_angle;
            cXyz mini_pos;
            // Spawn the mini freezard babies!
            int num_babies = 4;
            for (int idx = 0; idx < num_babies; ++idx) {
                mini_angle = shape_angle;
                mini_angle.y = fopAcM_searchPlayerAngleY(this) + 0x8000;
                mini_angle.y += s16(cM_rndFX(4000.0f));
                mini_pos = current.pos;
                mini_pos.x += cM_rndF(50.0f);
                mini_pos.y += cM_rndF(100.0f) + 100.0f;
                mini_pos.z += cM_rndF(50.0f);
                fopAcM_create(PROC_E_FZ, 1, &mini_pos, fopAcM_GetRoomNo(this), &mini_angle, NULL, -1);
            }

            dead_eff_set();
            if (swBit0 != 0xFF) {
                fopAcM_onSwitch(this, swBit0);
            }

            fopAcM_delete(this);
        }

        break;

    case 1:
        if (mpMorf->isStop()) {
            setActionMode(0, 0);
            mMoveMode = 2;
            field_0x680 = 0;
        }

        break;
    }

    current.angle.y = shape_angle.y;
}

static u8 lbl_188_bss_7C;

void daE_FB_c::executeBullet() {
    cXyz sp_0x20;
    cXyz sp_0x14;
    switch (mMoveMode) {
    case 0:
        field_0x69c = mFireTimer;
        mFireTimer += 3;
        if (mFireTimer > 36 + JREG_S(0)) {
            mFireTimer = 36 + JREG_S(0);
        }

        ++mMoveMode;
        /* fallthrough */
    case 1:
        speedF = 35.0f + JREG_F(2);
        if (parentActorID) {
            fopAc_ac_c* actor_p = fopAcM_SearchByID(fopAcM_GetLinkId(this));
            if (actor_p != NULL) {
                s16 ang_diff = actor_p->shape_angle.y - home.angle.y;
                current.angle.y -= s16(ang_diff * (0.1f + JREG_F(3)));
            }
        }

        cMtx_YrotS(*calc_mtx, current.angle.y);
        cMtx_XrotM(*calc_mtx, current.angle.x);
        sp_0x20.x = 0.0f;
        sp_0x20.y = 0.0f;
        sp_0x20.z = speedF;
        MtxPosition(&sp_0x20, &sp_0x14);
        speed.y = sp_0x14.y;

        break;
    }

    cXyz sp_0x8(speedF * cM_ssin(current.angle.y), 0.0f, speedF * cM_scos(current.angle.y));
    mAtSph.SetC(current.pos);
    mAtSph.SetR(40.0f + BREG_F(1));
    mAtSph.SetAtVec(sp_0x8);
    dComIfG_Ccsp()->Set(&mAtSph);
    if (mAtSph.ChkAtHit()) {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        fopAc_ac_c* at_hit_ac = mAtSph.GetAtHitAc();
        if (player == at_hit_ac && lbl_188_bss_7C < 2) {
            ++lbl_188_bss_7C;
        }

        mAtSph.ClrAtHit();
    }

    if (mObjAcch.ChkGroundHit() || field_0x69c == 0) {
        fopAcM_delete(this);
    }
}

void daE_FB_c::action() {
    mModelSize = l_HIO.model_size;
    if (mType == 1) {
        mModelSize = 1.3f;
    }

    if (mType == 2 && lbl_188_bss_7C == 2) {
        lbl_188_bss_7C = 3;
        if (swBit1 != 0xFF) {
            fopAcM_onSwitch(this, swBit1);
        }
    }

    if (mType == 0 || mType == 2) {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        cXyz sp_0x14;
        cXyz sp_0x8;
        mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(2));
        mDoMtx_stack_c::multVecZero(&sp_0x14);
        sp_0x8.set(player->current.pos);
        sp_0x8.y += 200.0f;
        sp_0x14 -= sp_0x8;
        s16 var_r29 = sp_0x14.atan2sY_XZ() * -1.0f;
        if (var_r29 < 0) {
            var_r29 = 0;
        } else if (var_r29 > 3400) {
            var_r29 = 3400;
        }

        cLib_addCalcAngleS2(&mHeadAngle, s16(var_r29), 2 + NREG_S(4), 0x200 + NREG_S(5));
    }

    if (!mBgLineCheck() || mType == 1) {
        fopAcM_OnStatus(this, 0);
        attention_info.flags |= fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(this, 0);
        attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
    }

    int var_r28 = 0;
    damage_check();
    switch (mActionMode) {
    case 0:
        executeWait();
        break;
    case 1:
        executeAttack();
        var_r28 = 1;
        break;
    case 2:
        executeDamage();
        break;
    case 3:
        executeBullet();
        break;
    }

    if (mType == 10 || mType == 11) {
        fopAcM_posMoveF(this, mStts.GetCCMoveP());
        return;
    }

    mCreatureSound.setLinkSearch(var_r28);
    mpBrkAnm->setFrame(field_0x68e);
    mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
}

void daE_FB_c::mtx_set() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(mModelSize, mModelSize, mModelSize);
    J3DModel* model = mpMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->modelCalc();
}

void daE_FB_c::cc_set() {
    cXyz sp8;
    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(2));
    mDoMtx_stack_c::transS(mDoMtx_stack_c::get()[0][3],
                           mDoMtx_stack_c::get()[1][3],
                           mDoMtx_stack_c::get()[2][3]);
    mDoMtx_stack_c::multVecZero(&attention_info.position);
    attention_info.position.y += 100.0f;
    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(2));
    mDoMtx_stack_c::transM(-20.0f, 30.0f, 0.0f);
    mDoMtx_stack_c::multVecZero(&sp8);
    mSphere.SetC(sp8);
    mSphere.SetR(150.0f);
    dComIfG_Ccsp()->Set(&mSphere);
    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(1));
    mDoMtx_stack_c::transM(-40.0f, 30.0f, -20.0f);
    mDoMtx_stack_c::multVecZero(&sp8);
    mSphere2.SetC(sp8);
    mSphere2.SetR(170.0f);
    dComIfG_Ccsp()->Set(&mSphere2);
    eyePos = sp8;
}

void daE_FB_c::normal_eff_set() {
    static u16 n_eff_id[5] = {
        dPa_RM(ID_ZI_S_FL_REIKI_A),
        dPa_RM(ID_ZI_S_FL_REIKI_B),
        dPa_RM(ID_ZI_S_FL_REIKI_C),
        dPa_RM(ID_ZI_S_FL_REIKI_D),
        dPa_RM(ID_ZI_S_FL_REIKI_E),
    };
    static int n_joint_id[5] = {0, 0, 1, 2, 0};
    cXyz sp_0x14(mModelSize, mModelSize, mModelSize);
    cXyz sp8;
    for (int idx = 0; idx < 5; ++idx) {
        sp8.set(current.pos);
        if (n_joint_id[idx]) {
            mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(n_joint_id[idx]));
            mDoMtx_stack_c::multVecZero(&sp8);
        }

        mKeys[idx] = dComIfGp_particle_set(mKeys[idx], n_eff_id[idx],
                                           &sp8, &shape_angle, &sp_0x14);
    }
}

void daE_FB_c::dead_eff_set() {
    static u16 d_eff_id[5] = {
        dPa_RM(ID_ZI_S_FL_DEAD_A),
        dPa_RM(ID_ZI_S_FL_DEAD_B),
        dPa_RM(ID_ZI_S_FL_DEAD_C),
        dPa_RM(ID_ZI_S_FL_DEAD_D),
        dPa_RM(ID_ZI_S_FL_DEAD_E),
    };
    static int d_joint_id[5] = {2, 1, 2, 2, 1};
    cXyz sp_0x14(mModelSize, mModelSize, mModelSize);
    cXyz sp8;
    int keys[5];
    for (int idx = 0; idx < 5; ++idx) {
        keys[idx] = dComIfGp_particle_set(keys[idx], d_eff_id[idx],
                                          &current.pos, &shape_angle, &sp_0x14);
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(keys[idx]);
        if (emitter != NULL) {
            emitter->setGlobalSRTMatrix(mpMorf->getModel()->getAnmMtx(d_joint_id[idx]));
        }
    }
}

#if DEBUG
static char* dummy_117771() {
    return "Delete -> E_FB(id=%d)\n";
}
#endif

int daE_FB_c::execute() {
    if (field_0x69c != 0) {
        --field_0x69c;
    }

    if (field_0x68c != 0) {
        --field_0x68c;
    }

    action();
    if (mType != 10 && mType != 11) {
        mtx_set();
        cc_set();
        normal_eff_set();
    }

    return 1;
}

static int daE_FB_Execute(daE_FB_c* i_this) {
    return i_this->execute();
}

static int daE_FB_IsDelete(daE_FB_c*) {
    return 1;
}

int daE_FB_c::_delete() {
    dComIfG_resDelete(&mPhaseReq, "E_FL");
    if (mType == 10 || mType == 11) {
        return 1;
    }

    if (mHIOInit) {
        hio_set = 0;
        mDoHIO_DELETE_CHILD(l_HIO.mId);
    }

    if (heap != NULL) {
        mCreatureSound.deleteObject();
    }

    return 1;
}

static int daE_FB_Delete(daE_FB_c* i_this) {
    int id = fopAcM_GetID(i_this);
    return static_cast<daE_FB_c*>(i_this)->_delete();
}

int daE_FB_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*) dComIfG_getObjectRes("E_FL", 0xB);
    JUT_ASSERT(1516, modelData != NULL);
    mpMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL,
                                  (J3DAnmTransform*)dComIfG_getObjectRes("E_FL", 0x8),
                                  0, 1.0f, 0, -1, &mCreatureSound, 0, 0x31000084);
    if (mpMorf == NULL || mpMorf->getModel() == NULL) {
        return 0;
    }

    if (mInvisibleModel.create(mpMorf->getModel(), 1) == FALSE) {
        return 0;
    }

    model = mpMorf->getModel();
    model->setUserArea((uintptr_t)this);
    for (u16 idx = 0; idx < model->getModelData()->getJointNum(); ++idx) {
        if (idx != 0) {
            model->getModelData()->getJointNodePointer(idx)->setCallBack(&daE_FB_c::JointCallBack);
        }
    }

    mpBrkAnm = new mDoExt_brkAnm();
    if (mpBrkAnm == NULL) {
        return 0;
    }

    if (mpBrkAnm->init(mpMorf->getModel()->getModelData(), (J3DAnmTevRegKey*) dComIfG_getObjectRes("E_FL", 0xE),
                       1, 2, 1.0f, 0, -1) == FALSE) {
        return 0;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    daE_FB_c* a_this = static_cast<daE_FB_c*>(i_this);
    return a_this->CreateHeap();
}

cPhs__Step daE_FB_c::create() {
    fopAcM_ct(this, daE_FB_c);
    mType = fopAcM_GetParam(this);
    swBit0 = (fopAcM_GetParam(this) & 0xFF00) >> 8;
    swBit1 = (fopAcM_GetParam(this) & 0xFF0000) >> 16;
    if (mType == 0xFF) {
        mType = 0;
    }

    cPhs__Step phase = (cPhs__Step) dComIfG_resLoad(&mPhaseReq, "E_FL");
    if (phase == cPhs_COMPLEATE_e) {
        if (mType == 10 || mType == 11) {
            fopAcM_OffStatus(this, 0);
            attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
            attention_info.distances[fopAc_attn_BATTLE_e] = 0;
            fopAcM_SetGroup(this, 0);
            mStts.Init(0xFF, 0, this);
            mAtSph.Set(cc_fb_at_src);
            mAtSph.SetStts(&mStts);
            setActionMode(3, 0);
        } else {
            if (swBit0 != 0xFF && fopAcM_isSwitch(this, swBit0)) {
                return cPhs_ERROR_e;
            }

            if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x2830)) {
                return cPhs_ERROR_e;
            }

            if (hio_set == false) {
                hio_set = true;
                mHIOInit = 1;
                l_HIO.mId = mDoHIO_CREATE_CHILD("フリザド（大）", &l_HIO);
            }

            attention_info.flags = fopAc_AttnFlag_BATTLE_e;
            attention_info.distances[fopAc_attn_BATTLE_e] = 0x45;
            if (mType == 1) {
                // Pretty sure these are the caged freezards
                //  in Snowpeak Ruins - Spiral Room.
                attention_info.distances[fopAc_attn_BATTLE_e] = 0x1D;
            }

            fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
            fopAcM_SetMin(this, -200.0f, -200.0f, -200.0f);
            fopAcM_SetMax(this, 200.0f, 200.0f, 200.0f);
            mObjAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                         fopAcM_GetSpeed_p(this), NULL, NULL);
            onWolfNoLock();
            mAcchCir.SetWall(40.0f, 80.0f);
            mObjAcch.CrrPos(dComIfG_Bgsp());
            mStts.Init(0xFA, 0, this);
            field_0x560 = health = 200;
            lbl_188_bss_7C = 0;
            mSphere.Set(cc_fb_src);
            mSphere.SetStts(&mStts);
            mSphere2.Set(cc_fb_src);
            mSphere2.SetStts(&mStts);
            mAtSph.Set(cc_fb_at_src);
            mAtSph.SetStts(&mStts);
            mCreatureSound.init(&current.pos, &eyePos, 3, 1);
            mCreatureSound.setEnemyName("E_fb");
            mAtInfo.mpSound = &mCreatureSound;
            mAtInfo.mPowerType = 6;
#if VERSION != VERSION_SHIELD_DEBUG
            attention_info.position.x = current.pos.x;
            attention_info.position.y = current.pos.y;
            attention_info.position.z = current.pos.z;
#endif
            setActionMode(0, 0);
        }

        daE_FB_Execute(this);
    }

    return phase;
}

daE_FB_c::daE_FB_c() {
    // Empty function.
}

static int daE_FB_Create(daE_FB_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daE_FB_Method = {
    (process_method_func)daE_FB_Create,
    (process_method_func)daE_FB_Delete,
    (process_method_func)daE_FB_Execute,
    (process_method_func)daE_FB_IsDelete,
    (process_method_func)daE_FB_Draw,
};

actor_process_profile_definition g_profile_E_FB = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_FB,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daE_FB_c),       // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  176,                    // mPriority
  &l_daE_FB_Method,       // sub_method
  0x00040100,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};

AUDIO_INSTANCES;
