/**
 * @file d_a_e_oc.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_oc.h"
#include "d/d_camera.h"
#include "Z2AudioLib/Z2Instances.h"
#include "d/d_com_inf_game.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_obj_rotBridge.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_enemy.h"
#include "f_op/f_op_camera_mng.h"


enum OC_ACTIONS {
    E_OC_ACTION_WAIT,
    E_OC_ACTION_WALK,
    E_OC_ACTION_TALK,
    E_OC_ACTION_FIND,
    E_OC_ACTION_ATTACK,
    E_OC_ACTION_DAMAGE,
    E_OC_ACTION_BIG_DAMAGE,
    E_OC_ACTION_WATCH,
    E_OC_ACTION_SOUND_WATCH,
    E_OC_ACTION_DEATH,
    E_OC_ACTION_WATER_DEATH,
    E_OC_ACTION_DEMO_MASTER,
    E_OC_ACTION_DEMO_CHILD,
    E_OC_ACTION_FALL_DEAD,
    E_OC_ACTION_FALL,
    E_OC_ACTION_FIND_STAY,
    E_OC_ACTION_MOVE_OUT,
};

class daE_OC_HIO_c : public JORReflexible {
public:
    daE_OC_HIO_c();
    virtual ~daE_OC_HIO_c() {}

    /* 0x04 */ s8 field_0x04;
    /* 0x05 */ u8 radius_representation;
    /* 0x08 */ f32 plyr_srch_max_radius_S;
    /* 0x0c */ f32 plyr_srch_max_radius_M;
    /* 0x10 */ f32 plyr_srch_min_radius;
    /* 0x14 */ f32 battle_participation_radius;
    /* 0x18 */ f32 teammate_attention_radius;
    /* 0x1c */ f32 basic_size;
    /* 0x20 */ f32 standby_distance;
    /* 0x24 */ f32 demo_waiting_time;

#if DEBUG
    void genMessage(JORMContext*);
#endif
};

f32 const E_OC_n::oc_attackb_trans[10] = {
    0.0f, 0.0f, 0.0f, 0.0f, 15.3f, 
    38.4f, 63.3f, 86.2f, 103.3f, 112.0f
};

f32 const E_OC_n::oc_attackc_trans[10] = {
    0.0f, 31.3f, 72.7f, 91.5f, 91.5f,
    91.5f, 91.5f, 91.5f, 91.5f, 91.5f
};

dCcD_SrcSph E_OC_n::cc_sph_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0xD8FBFDFF, 0x3}, 0x75}}, // mObj
        {0x9, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {0x0, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
    } // mSphAttr
};

dCcD_SrcSph E_OC_n::at_sph_src = {
    {
        {0x0, {{0x1000, 0x1, 0xD}, {0x0, 0x0}, 0x0}}, // mObj
        {0x9, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {0x0, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
    } // mSphAttr
};

daE_OC_HIO_c::daE_OC_HIO_c() {
    field_0x04 = -1;
    radius_representation = 0;
    basic_size = 1.5f;
    plyr_srch_max_radius_S = 1200.0f;
    plyr_srch_max_radius_M = 5000.0f;
    plyr_srch_min_radius = 400.0f;
    battle_participation_radius = 1200.0f;
    teammate_attention_radius = 2000.0f;
    standby_distance = 1400.0f;
    demo_waiting_time = 70.0f;
}

#if DEBUG
void daE_OC_HIO_c::genMessage(JORMContext* ctext) {
    // enemy sample
    ctext->genLabel("敵サンプル", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // basic size
    ctext->genSlider("基本サイズ", &basic_size, 0.0f, 5.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // player search max radius S
    ctext->genSlider("プレイヤーサーチ最大半径S", &plyr_srch_max_radius_S, 0.0f, 10000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // player search max radius M
    ctext->genSlider("プレイヤーサーチ最大半径M", &plyr_srch_max_radius_M, 0.0f, 10000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // player search min radius
    ctext->genSlider("プレイヤーサーチ最小半径", &plyr_srch_min_radius, 0.0f, 10000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // battle participation radius
    ctext->genSlider("戦闘参加半径", &battle_participation_radius, 0.0f, 3000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // attention radius of slain teammate
    ctext->genSlider("やられ仲間注目半径", &teammate_attention_radius, 0.0f, 3000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // demo waiting time
    ctext->genSlider("デモ待機時間", &demo_waiting_time, 0.0f, 3000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // out of range standby distance
    ctext->genSlider("範囲外待機距離", &standby_distance, 0.0f, 10000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // radius representation
    ctext->genCheckBox("半径表示", &radius_representation, 1, 0, NULL, -1, -1, 0x200, 0x18);
}
#endif

int daE_OC_c::ctrlJoint(J3DJoint* i_joint, J3DModel* param_1) {
    J3DJoint* my_joint = i_joint;
    int jnt_no = my_joint->getJntNo();
    mDoMtx_stack_c::copy(param_1->getAnmMtx(jnt_no));
    if (jnt_no == 0x11) {
        mDoMtx_stack_c::YrotM(field_0x6d2);
    } else if (jnt_no == 0xc) {
        mDoMtx_stack_c::YrotM(field_0x6d0);
    }
    param_1->setAnmMtx(jnt_no, mDoMtx_stack_c::get());
    mDoMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    return 1;
}

int daE_OC_c::JointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DModel* model_p = j3dSys.getModel();
        daE_OC_c* bokoblin_ptr = (daE_OC_c*) model_p->getUserArea();
        if (bokoblin_ptr) {
            bokoblin_ptr->ctrlJoint(i_joint, model_p);
        }
    }
    return 1;
}

int daE_OC_c::draw() {
    J3DModel* model = mpMorf->getModel();
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);
    if (mHide) {
        return 1;
    }

    if (field_0x6e8 != 0) {
        J3DModelData* model_data = model->getModelData();
        for (u16 idx = 0; idx < model_data->getMaterialNum(); ++idx) {
            J3DMaterial* material = model_data->getMaterialNodePointer(idx);
            material->getTevColor(0)->r = field_0x6e4;
            material->getTevColor(0)->g = field_0x6e4;
            material->getTevColor(0)->b = field_0x6e4;
        }
    }

    mpMorf->entryDL();
    cXyz shadow_pos;
    shadow_pos.set(current.pos.x, 100.0f + current.pos.y, current.pos.z);
    mShadowKey = dComIfGd_setShadow(mShadowKey, 1, model, &shadow_pos, 1000.0f, 0.0f,
                                    current.pos.y, mAcch.GetGroundH(), mAcch.m_gnd,
                                    &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());

    if (mpModel) {
        g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
        mDoExt_modelUpdateDL(mpModel);
        dComIfGd_addRealShadow(mShadowKey, mpModel);
    }

    return 1;
}

static void daE_OC_Draw(daE_OC_c* i_this) {
    i_this->draw();
}

daE_OC_c* E_OC_n::m_battle_oc;

daE_OC_c* E_OC_n::m_damage_oc;

daE_OC_c* E_OC_n::m_death_oc;

daE_OC_c* E_OC_n::m_talk_oc;

static u8 lbl_216_bss_58;

static daE_OC_HIO_c l_HIO;

static void* s_other_oc(void* arg_lhs, void* arg_rhs) {
    f32 dist;
    if (arg_lhs != arg_rhs && fopAcM_IsActor(arg_lhs)) {
        if (fpcM_IsCreating(fopAcM_GetID(arg_lhs)) == 0 && fopAcM_GetName(arg_lhs) == PROC_E_OC) {
            if (((daE_OC_c*) arg_lhs)->isBattleOn()) {
                dist = fopAcM_searchActorDistance((fopAc_ac_c*) arg_lhs, (fopAc_ac_c*) arg_rhs);
                if (dist < l_HIO.battle_participation_radius) {
                    dist = ((fopAc_ac_c*) arg_lhs)->current.pos.absXZ(((fopAc_ac_c*) arg_rhs)->home.pos);
                    if (dist < ((daE_OC_c*) arg_rhs)->getMoveRange()) {
                        if (fopAcM_searchPlayerDistance((fopAc_ac_c*) arg_lhs) < ((daE_OC_c*) arg_lhs)->getPlayerRange()) {
                            E_OC_n::m_battle_oc = (daE_OC_c*) arg_lhs;
                        }
                    }
                }
            }

            switch (((daE_OC_c*) arg_lhs)->getActionMode()) {
                case E_OC_ACTION_BIG_DAMAGE:
                    dist = fopAcM_searchActorDistance((fopAc_ac_c*) arg_lhs, (fopAc_ac_c*) arg_rhs);
                    if (dist < l_HIO.teammate_attention_radius) {
                        E_OC_n::m_damage_oc = (daE_OC_c*) arg_lhs;
                    }
                    break;
                case E_OC_ACTION_DEATH:
                    dist = fopAcM_searchActorDistance((fopAc_ac_c*) arg_lhs, (fopAc_ac_c*) arg_rhs);
                    if (dist < l_HIO.teammate_attention_radius) {
                        E_OC_n::m_death_oc = (daE_OC_c*) arg_lhs;
                    }
                    break;
                case E_OC_ACTION_WAIT:
                case E_OC_ACTION_WALK:
                    dist = fopAcM_searchActorDistance((fopAc_ac_c*) arg_lhs, (fopAc_ac_c*) arg_rhs);
                    if (dist < 400.0f && dist > 200.0f) {
                        E_OC_n::m_talk_oc = (daE_OC_c*) arg_lhs;
                    }
                    break;
            }
        }
    }

    return NULL;
}

s16 daE_OC_c::getVisionAngle(fopAc_ac_c* other_oc) {
    return cLib_distanceAngleS(shape_angle.y, fopAcM_searchActorAngleY(this, other_oc));
}

void daE_OC_c::searchOtherOc() {
    if (field_0x6e3 == 0) {
        field_0x6dd = 0;
        E_OC_n::m_battle_oc = NULL;
        E_OC_n::m_damage_oc = NULL;
        E_OC_n::m_death_oc = NULL;
        E_OC_n::m_talk_oc = NULL;
        fpcM_Search(s_other_oc, this);
        mpBattle = E_OC_n::m_battle_oc;
        if (field_0x6c6 == 0 && (mActionMode == E_OC_ACTION_WAIT || mActionMode == E_OC_ACTION_WALK)) {
            mpTalk = E_OC_n::m_talk_oc;
            if (mpTalk && mpTalk->getTalkOc() == this) {
                s16 sh_val = 0x2000;
                if (isHomeWait() && mpTalk->isHomeWait()) {
                    sh_val = 0x4800;
                }

                if (getVisionAngle(mpTalk) <= sh_val && mpTalk->getVisionAngle(this) <= sh_val) {
                    setActionMode(E_OC_ACTION_TALK, 0);
                    mpTalk->setActionMode(E_OC_ACTION_TALK, 0);
                }
            }
        }

        if (mpDamage == NULL || mpDamage != E_OC_n::m_damage_oc) {
            mpDamage = E_OC_n::m_damage_oc;
            if (mpDamage) {
                field_0x6dd = 1;
            }
        }

        if (E_OC_n::m_death_oc && E_OC_n::m_death_oc->checkBeforeDeath()) {
            field_0x6dd = 2;
            mWatchPos = E_OC_n::m_death_oc->current.pos;
        }
    }
}

bool daE_OC_c::setWatchMode() {
    if (field_0x6dd != 0) {
        if (field_0x6dd == 1) {
            setActionMode(E_OC_ACTION_WATCH, 0);
        } else {
            setActionMode(E_OC_ACTION_WATCH, 2);
        }

        return true;
    }

    return false;
}

bool daE_OC_c::searchPlayer() {
    if (fopAcM_searchPlayerDistance(this) < mPlayerRange) {
        s16 diff = shape_angle.y - fopAcM_searchPlayerAngleY(this);
        if (fopAcM_searchPlayerDistance(this) < l_HIO.plyr_srch_min_radius) {
            if (daPy_getPlayerActorClass()->speedF > 12.0f) {
                return true;
            }

            if (abs(diff) < 0x5000) {
                if (fopAcM_otherBgCheck(this, dComIfGp_getPlayer(0)) == 0) {
                    return true;
                }
            }
        } else {
            if (abs(diff) < 0x4000) {
                if (fopAcM_otherBgCheck(this, dComIfGp_getPlayer(0)) == 0) {
                    return true;
                }
            }
        }
    }

    return false;
}

static void* s_obj_sub(void* arg_lhs, void* arg_rhs) {
    if (fopAcM_IsActor(arg_lhs)) {
        if (fpcM_IsCreating(fopAcM_GetID(arg_lhs)) == 0 && fopAcM_GetName(arg_lhs) == PROC_Obj_RotBridge) {
            if (fopAcM_GetRoomNo((fopAc_ac_c*) arg_lhs) == fopAcM_GetRoomNo((fopAc_ac_c*) arg_rhs)) {
                return arg_lhs;
            }
        }
    }

    return NULL;
}

bool daE_OC_c::searchPlayer2() {
    field_0x6e2 = 0;
    if (field_0x6b4 == 2) {
        mpBridge = (daRotBridge_c *) fpcM_Search(s_obj_sub, this);
        if (mpBridge && mpBridge->getPlayerRide()) {
            if (mpBridge->getBridgeAngle()) {
                if (field_0x6e1 != 0xFF) {
                    if (!dComIfGs_isSwitch(field_0x6e1, fopAcM_GetRoomNo(this))) {
                        dComIfGs_onSwitch(field_0x6e1, fopAcM_GetRoomNo(this));
                    }
                }

                field_0x6e2 = 1;
                return true;
            }

            return false;
        }
    }

    if (searchPlayer()) {
        if (field_0x6b4 == 2 && field_0x6e1 != 0xFF) {
            if (!dComIfGs_isSwitch(field_0x6e1, fopAcM_GetRoomNo(this))) {
                dComIfGs_onSwitch(field_0x6e1, fopAcM_GetRoomNo(this));
                field_0x6e2 = 1;
            }
        }

        return true;
    }

    if (mpBattle) {
        if (mpBattle->getActionMode() != E_OC_ACTION_FIND) {
            return true;
        }

        if (mpBattle->searchPlayer()) {
            return true;
        }
    }

    return false;
}

bool daE_OC_c::searchPlayerShakeHead() {
    if (field_0x6e3) {
        return false;
    }

    if (field_0x6b4 == 2 && mpBridge && mpBridge->getPlayerRide()) {
        return false;
    }

    if (fopAcM_searchPlayerDistance(this) < mPlayerRange) {
        s16 diff = getHeadAngle() - fopAcM_searchPlayerAngleY(this);
        if (abs(diff) < 0x2000) {
            if (fopAcM_otherBgCheck(this, dComIfGp_getPlayer(0)) == FALSE) {
                return true;
            }
        }
    }

    return false;
}

bool daE_OC_c::searchSound() {
    if (field_0x6b4 == 2 && mpBridge && mpBridge->getPlayerRide()) {
        return false;
    }

    if (daPy_getPlayerActorClass()->getCutType() != daPy_py_c::CUT_TYPE_NONE && fopAcM_searchPlayerDistance(this) < mPlayerRange) {
        mWatchPos = dComIfGp_getPlayer(0)->current.pos;
        setActionMode(E_OC_ACTION_SOUND_WATCH, 0);
        return true;
    }

    if (fopAcM_otoCheck(this, 1000.0f)) {
        mWatchPos = dKy_Sound_get()->position;
        setActionMode(E_OC_ACTION_SOUND_WATCH, 0);
        return true;
    }

    return false;
}

static void* s_demo_oc(void* arg_lhs, void* arg_rhs) {
    (void) arg_rhs;
    if (fopAcM_IsActor(arg_lhs)) {
        if (fpcM_IsCreating(fopAcM_GetID(arg_lhs)) == 0 && fopAcM_GetName(arg_lhs) == PROC_E_OC
            && (fopAcM_GetParam(arg_lhs) & 0xFF) == 4) {
            return arg_lhs;
        }
    }

    return NULL;
}

int daE_OC_c::checkBeforeBg() {
    dBgS_LinChk link_chk;
    cM3dGPla plane;
    bool tri_pla = false;
    cXyz my_vec_0;
    cXyz my_vec_1;
    cXyz my_vec_2;
    my_vec_0 = current.pos;
    my_vec_0.y += 100.0f;
    my_vec_1 = my_vec_0;
    my_vec_1.x += cM_ssin(shape_angle.y) * 300.0f;
    my_vec_1.z += cM_scos(shape_angle.y) * 300.0f;
    link_chk.Set(&my_vec_0, &my_vec_1, this);
    s16 tan_val;
    if (dComIfG_Bgsp().LineCross(&link_chk)) {
        tri_pla = dComIfG_Bgsp().GetTriPla(link_chk, &plane);
        if (my_vec_0.abs(link_chk.GetCross()) < 100.0f) {
            return 1;
        }

        tan_val = cM_atan2s(plane.mNormal.x, plane.mNormal.z);
        if (s16(tan_val - shape_angle.y) > 0) {
            return 2;
        } else {
            return 3;
        }
    }

    return 0;
}

bool daE_OC_c::checkBeforeBgFind() {
    dBgS_LinChk line_chk;
    cXyz oc_pos;
    cXyz plyr_pos;
    cXyz my_vec_2;
    s16 pl_ang = fopAcM_searchPlayerAngleY(this);
    oc_pos = current.pos;
    oc_pos.y += 100.0f;
    plyr_pos = daPy_getPlayerActorClass()->current.pos;
    plyr_pos.y += 100.0f;
    line_chk.Set(&oc_pos, &plyr_pos, NULL);
    if (!dComIfG_Bgsp().LineCross(&line_chk)) {
        return false;
    }

    my_vec_2 = plyr_pos;
    plyr_pos.x += cM_ssin(pl_ang + 0x4000) * 300.0f;
    plyr_pos.z += cM_scos(pl_ang + 0x4000) * 300.0f;
    line_chk.Set(&oc_pos, &plyr_pos, NULL);
    if (!dComIfG_Bgsp().LineCross(&line_chk)) {
        field_0x6da = (s16) cLib_targetAngleY(&oc_pos, &plyr_pos);
        return true;
    }

    my_vec_2.x += cM_ssin(pl_ang - 0x4000) * 300.0f;
    my_vec_2.z += cM_scos(pl_ang - 0x4000) * 300.0f;
    line_chk.Set(&oc_pos, &my_vec_2, NULL);
    if (!dComIfG_Bgsp().LineCross(&line_chk)) {
        field_0x6da = (s16) cLib_targetAngleY(&oc_pos, &my_vec_2);
        return true;
    } else {
        field_0x6da = (s16) cLib_targetAngleY(&oc_pos, &plyr_pos);
        return true;
    }
}

bool daE_OC_c::checkBeforeFloorBg(f32 arg) {
    cXyz my_vec_0;
    dBgS_ObjGndChk_Spl gnd_chk_spl;
    dBgS_GndChk gnd_chk;
    cXyz my_vec_1(0.0f, 100.0f, arg);
    cLib_offsetPos(&my_vec_0, &current.pos, shape_angle.y, &my_vec_1);
    gnd_chk_spl.SetPos(&my_vec_0);
    f32 gnd_cross_0 = dComIfG_Bgsp().GroundCross(&gnd_chk_spl);
    if (gnd_cross_0 != -G_CM3D_F_INF) {
        gnd_chk.SetPos(&my_vec_0);
        f32 gnd_cross_1 = dComIfG_Bgsp().GroundCross(&gnd_chk);
        if (gnd_cross_0 >= gnd_cross_1) {
            return true;
        }
    }

    return false;
}

void daE_OC_c::checkDamageBg() {
    cXyz my_vec(current.pos);
    my_vec.y += 50.0f;
    cXyz o_vec;
    mDoMtx_stack_c::transS(my_vec);
    mDoMtx_stack_c::ZXYrotM(field_0x688);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::YrotM(field_0x6d8);
    mDoMtx_stack_c::transM(0.0f, 0.0f, -200.0f);
    mDoMtx_stack_c::multVecZero(&o_vec);
    dBgS_LinChk link_chk;
    link_chk.Set(&my_vec, &o_vec, this);
    if (dComIfG_Bgsp().LineCross(&link_chk)) {
        if (field_0x6be == 0) {
            cM3dGPla plane;
            dComIfG_Bgsp().GetTriPla(link_chk, &plane);
            s16 val = cM_atan2s(plane.mNormal.x, plane.mNormal.z);
            s16 adj_angle = shape_angle.y + 0x8000;
            if (s16(val - adj_angle) > 0) {
                field_0x6be = 0x400;
            } else {
                field_0x6be = -0x400;
            }
        }

        shape_angle.y += field_0x6be;
    }
}

void daE_OC_c::setGroundAngle() {
    cXyz my_vec_0;
    cXyz my_vec_1;
    cXyz my_vec_2;
    cXyz my_vec_3;
    dBgS_GndChk gnd_chk;
    my_vec_2 = current.pos;
    my_vec_2.y = mAcch.GetGroundH();
    my_vec_0.x = my_vec_2.x;
    my_vec_0.y = my_vec_2.y + 50.0f;
    my_vec_0.z = my_vec_2.z + 80.0f;
    gnd_chk.SetPos(&my_vec_0);
    my_vec_0.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
    if (my_vec_0.y != -G_CM3D_F_INF && std::abs(my_vec_0.y - my_vec_2.y) < 50.0f) {
        my_vec_3 = my_vec_0 - my_vec_2;
        field_0x688.x = -cM_atan2s(my_vec_3.y, my_vec_3.z);
    }
    my_vec_1.x = my_vec_2.x + 50.0f;
    my_vec_1.y = my_vec_2.y + 50.0f;
    my_vec_1.z = my_vec_2.z;
    gnd_chk.SetPos(&my_vec_1);
    my_vec_1.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
    if (my_vec_1.y != -G_CM3D_F_INF && std::abs(my_vec_1.y - my_vec_2.y) < 50.0f) {
        my_vec_3 = my_vec_1 - my_vec_2;
        field_0x688.z = (s16) cM_atan2s(my_vec_3.y, my_vec_3.x);
    }
}

void daE_OC_c::setActionMode(int i_action, int i_state) {
    mOldActionMode = mActionMode;
    mActionMode = i_action;
    mOcState = i_state;
    field_0x6c0 = field_0x6c2 = 0;
    mSphs_cc[0].OnCoSetBit();
    mSphs_cc[0].OnTgSetBit();
    mSphs_cc[1].OnCoSetBit();
    mSphs_cc[1].OnTgSetBit();
    offHeadLockFlg();
}

int daE_OC_c::getCutType() {
    if (daPy_getPlayerActorClass()->getCutCount() >= 4) {
        return 5;
    }

    switch (daPy_getPlayerActorClass()->getCutType()) {
        case daPy_py_c::CUT_TYPE_TURN_RIGHT:
        case daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT:
        case daPy_py_c::CUT_TYPE_HEAD_JUMP:
            return 6;
        case daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT:
        case daPy_py_c::CUT_TYPE_TURN_LEFT:
            return 7;
        case daPy_py_c::CUT_TYPE_MORTAL_DRAW_A:
        case daPy_py_c::CUT_TYPE_TWIRL:
        case daPy_py_c::CUT_TYPE_MORTAL_DRAW_B:
        case daPy_py_c::CUT_TYPE_JUMP:
            return 5;
        case daPy_py_c::CUT_TYPE_NM_STAB:
        case daPy_py_c::CUT_TYPE_NM_RIGHT:
        case daPy_py_c::CUT_TYPE_FINISH_STAB:
        case daPy_py_c::CUT_TYPE_DASH_UNK_26:
        case daPy_py_c::CUT_TYPE_NM_VERTICAL:
            return 0;
        case daPy_py_c::CUT_TYPE_COMBO_STAB:
        case daPy_py_c::CUT_TYPE_HORSE_UNK_22:
        case daPy_py_c::CUT_TYPE_HORSE_UNK_23:
        case daPy_py_c::CUT_TYPE_DASH_UNK_25:
        case daPy_py_c::CUT_TYPE_NM_LEFT:
            return 1;
        default:
            return 2;
    }
}

void daE_OC_c::offTgSph() {
    mSphs_cc[0].OffTgSetBit();
    mSphs_cc[0].ClrTgHit();
    mSphs_cc[1].OffTgSetBit();
    mSphs_cc[1].ClrTgHit();
}

void daE_OC_c::damage_check() {
    if (checkCutDownHitFlg()) {
        // Following needed to use r31 for debug asm:
        int unused;
        offCutDownHitFlg();
        health = 0;
        setActionMode(E_OC_ACTION_DEATH, 1);
        offTgSph();
        return;
    }

    if (field_0x6cc) {
        return;
    }

    mStts.Move();
    int had_hit = 0;
    cXyz my_vec_0;
    if (mSphs_cc[0].ChkTgHit()) {
        mAtInfo.mpCollider = mSphs_cc[0].GetTgHitObj();
        had_hit = 1;
        my_vec_0 = *mSphs_cc[0].GetTgHitPosP();
    }

    if (mSphs_cc[1].ChkTgHit()) {
        mAtInfo.mpCollider = mSphs_cc[1].GetTgHitObj();
        had_hit = 1;
        my_vec_0 = *mSphs_cc[1].GetTgHitPosP();
    }

    if (had_hit == 0) {
        return;
    }

    s16 old_health = health;    // unused.
    if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_UNK)) {
        field_0x6cc = 0x14;
    } else {
        field_0x6cc = 10;
    }

    if (mAtInfo.mAttackPower <= 1) {
        field_0x6cc = 10 + KREG_S(8);
    }

    u8 my_val = 2;
    if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_MASTER_SWORD | AT_TYPE_NORMAL_SWORD)) {
        my_val = getCutType();
        if (daPy_py_c::checkWoodSwordEquip()) {
            my_val = 2;
        }

        if (my_val <= 2) {
            if (field_0x6d6) {
                field_0x6dc ^= (u8) 1;
                my_val = field_0x6dc;
            } else {
                field_0x6dc = my_val;
            }

            field_0x6d6 = 0x1e;
        }

        if (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_HEAD_JUMP) {
            health = 0;
        }
    } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_SHIELD_ATTACK)) {
        my_val = 3;
    } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_ARROW)) {
        if (cM_rnd() < 0.5f) {
            my_val = 0;
        } else {
            my_val = 1;
        }
    } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOMB)) {
        my_val = 5;
        health = 0;
    } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_IRON_BALL)) {
        my_val = 5;
        if (dComIfGp_checkPlayerStatus0(0,0x400)) {
            health += (s16) 140;
        } else {
            health += (s16) 80;
        }
    } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG)) {
        my_val = 4;
    } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_40)) {
        health += (s16) 10;
    } else if (mAtInfo.mpCollider->ChkAtType(AT_TYPE_SLINGSHOT)) {
        if (mName == "E_OC") {
            health -= (s16) 5;
            if (health < 0) {
                health = 0;
                mSound.startCollisionSE(0x40007,0x20);
                dComIfGp_setHitMark(3, this, &my_vec_0, NULL, NULL, 0);
                setActionMode(E_OC_ACTION_BIG_DAMAGE, 0);
                offTgSph();
                mAtInfo.mHitDirection.y = fopAcM_searchPlayerAngleY(this);
                return;
            }
        } else {
            mSound.startCollisionSE(0x40018,0x2d);
            if (mActionMode == E_OC_ACTION_WAIT
             || mActionMode == E_OC_ACTION_WALK
             || mActionMode == E_OC_ACTION_TALK) {
                setActionMode(E_OC_ACTION_FIND, 1);
            }

            return;
        }
    }

    cc_at_check(this, &mAtInfo);
    if ( ((dCcD_GObjInf*)mAtInfo.mpCollider)->GetAtSpl() == 1
        && (!((dCcD_GObjInf*)mAtInfo.mpCollider)->ChkAtType(AT_TYPE_NORMAL_SWORD) || !daPy_py_c::checkWoodSwordEquip())
        && my_val < 5
    ) {
        my_val = 5;
    }

    if (health <= 1) {
        attention_info.flags = 0;
        if (my_val < 5) {
            my_val = 5;
        }
    }

    if (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_JUMP) {
        if (daPy_getPlayerActorClass()->checkCutJumpCancelTurn()) {
            setActionMode(E_OC_ACTION_DAMAGE, my_val);
            field_0x6cc = 3 + NREG_S(7);
            return;
        }
    }

    if (my_val >= 5) {
        setActionMode(E_OC_ACTION_BIG_DAMAGE, my_val - 5);
        offTgSph();
    } else {
        setActionMode(E_OC_ACTION_DAMAGE, my_val);
    }
}

void daE_OC_c::setBck(int i_index, u8 i_attr, f32 i_morf, f32 i_rate) {
    mpMorf->setAnm((J3DAnmTransform *) dComIfG_getObjectRes("E_ocb", i_index),
                    i_attr, i_morf, i_rate, 0.0f, -1.0f);
}

bool daE_OC_c::checkBck(int i_index) {
    if (mpMorf->getAnm() != dComIfG_getObjectRes("E_ocb", i_index)) {
        return false;
    } else {
        return true;
    }
}

void daE_OC_c::setSpitEffect() {
    MtxP my_mtx_p = mpMorf->getModel()->getAnmMtx(0x11);
    cXyz my_vec(my_mtx_p[0][3], my_mtx_p[1][3], my_mtx_p[2][3]);
    mParticleKeys[0] = dComIfGp_particle_set(mParticleKeys[0], dPa_RM(ID_ZM_S_ORCSPIT00),
                                             &my_vec, &tevStr, &shape_angle, NULL,
                                             -1, NULL, -1, NULL, NULL, NULL);
}

s16 daE_OC_c::getHeadAngle() {
    cXyz my_vec_0;
    cXyz my_vec_1;
    MtxP my_mtx_p = mpMorf->getModel()->getAnmMtx(0x11);
    my_vec_0.set(my_mtx_p[0][3], my_mtx_p[1][3], my_mtx_p[2][3]);
    my_mtx_p = mpMorf->getModel()->getAnmMtx(0x13);
    my_vec_1.set(my_mtx_p[0][3], my_mtx_p[1][3], my_mtx_p[2][3]);
    return (s16)cLib_targetAngleY(&my_vec_0, &my_vec_1);
}

void daE_OC_c::setStabPos() {
    cXyz my_vec_0;
    cXyz my_vec_1;
    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(0x1));
    my_vec_1.set(-10.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::multVec(&my_vec_1, &my_vec_0);
    setDownPos(&my_vec_0);
}

void daE_OC_c::setWaitSound() {
    if (checkBck(0x1b) && mpMorf->checkFrame(0.0f)) {
        mSound.startCreatureVoice(Z2SE_EN_OC_V_WAIT, -1);
    }
}

void daE_OC_c::setWalkSound() {
    if (checkBck(0x1d)) {
        if (mpMorf->checkFrame(14.0f)) {
            mSound.startCreatureSound(Z2SE_EN_OC_FOOTNOTE_L, 0, -1);
        } else if (mpMorf->checkFrame(30.0f)) {
            mSound.startCreatureSound(Z2SE_EN_OC_FOOTNOTE_R, 0, -1);
        }
    }
}

void daE_OC_c::setWalkStSound() {
    if (checkBck(0x1e)) {
        if (mpMorf->checkFrame(0.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_OC_V_WALK_ST, -1);
        } else if (mpMorf->checkFrame(1.0f)) {
            mSound.startCreatureSound(Z2SE_EN_OC_FOOTNOTE_L, 0, -1);
        } else if (mpMorf->checkFrame(9.0f)) {
            mSound.startCreatureSound(Z2SE_EN_OC_FOOTNOTE_R, 0, -1);
        }
    }
}

void daE_OC_c::setDashSound() {
    if (checkBck(0xb)) {
        if (mpMorf->checkFrame(0.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_OC_V_DASH, -1);
        } else if (mpMorf->checkFrame(1.0f)) {
            mSound.startCreatureSound(Z2SE_EN_OC_FOOTNOTE_L, 0, -1);
        } else if (mpMorf->checkFrame(8.5f)) {
            mSound.startCreatureSound(Z2SE_EN_OC_FOOTNOTE_R, 0, -1);
        }
        if (mName == "E_OC") {
            mSound.startCreatureSoundLevel(Z2SE_EN_OC_CLUB_DAGGLE, 0, -1);
        } else {
            mSound.startCreatureSoundLevel(Z2SE_EN_OC_NATA_DAGGLE, 0, -1);
        }
    }
}

void daE_OC_c::setWaitStSound() {
    if (checkBck(0x1c)) {
        if (mpMorf->checkFrame(0.0f)) {
            mSound.startCreatureVoice(Z2SE_EN_OC_V_WAIT_ST, -1);
        } else if (mpMorf->checkFrame(6.5f) || mpMorf->checkFrame(20.0f)) {
            mSound.startCreatureSound(Z2SE_EN_OC_FOOTNOTE_L, 0, -1);
        } else if (mpMorf->checkFrame(13.0f) || mpMorf->checkFrame(28.5f)) {
            mSound.startCreatureSound(Z2SE_EN_OC_FOOTNOTE_R, 0, -1);
        }
    }
}

void daE_OC_c::setFootNoteSound() {
    setWaitSound();
    setWalkSound();
    setWalkStSound();
    setDashSound();
    setWaitStSound();
}

void daE_OC_c::executeWait() {
    mPrevShapeAngle = shape_angle.y;
    field_0x6de = 0;
    if (field_0x6e3 == 0) {
        if (setWatchMode()) {
            return;
        }

        if (searchSound()) {
            return;
        }

        if (searchPlayer2()) {
            setActionMode(E_OC_ACTION_FIND, 1);
            return;
        }
    }

    if (field_0x6b4 == 1 && home.pos.abs(current.pos) < 100.0f
        && (s16)cLib_distanceAngleS(shape_angle.y, home.angle.y) < 0x100) {
        mIsHomeWait = 1;
    }

    switch (mOcState) {
        case 0:
            if (field_0x6b4 == 0) {
                setBck(0x1b, 2, 5.0f, 1.0f);
                mSound.startCreatureVoice(Z2SE_EN_OC_V_WAIT, -1);
            } else {
                setBck(0x18, 2, 5.0f, 1.0f);
                mSound.startCreatureVoice(Z2SE_EN_OC_V_STAND_WAIT, -1);
            }

            speedF = 0.0f;
            field_0x6bc = shape_angle.y;
            if (field_0x6b4 == 0) {
                if (cM_rnd() < 0.1f) {
                    field_0x6c0 = cM_rndF(20.0f) + 10.0f;
                    mOcState = 1;
                } else {
                    field_0x6c0 = cM_rndF(30.0f) + 50.0f;
                    mOcState = 3;
                }
            } else if (home.pos.abs(current.pos) > 100.0f) {
                field_0x6c0 = cM_rndF(30.0f) + 50.0f;
                mOcState = 3;
            } else {
                field_0x6c0 = cM_rndF(20.0f) + 10.0f;
                mOcState = 1;
            }
            break;
        case 1:
            cLib_addCalcAngleS(&shape_angle.y, field_0x6bc, 4, 0x800, 0x100);
            if (field_0x6c0 == 0) {
                setBck(0x16, 0, 5.0f, 1.0f);
                mSound.startCreatureVoice(Z2SE_EN_OC_V_SEARCH, -1);
                mOcState = 2;
            }
            break;
        case 2:
            if (searchPlayerShakeHead()) {
                setActionMode(E_OC_ACTION_FIND, 1);
            } else {
                if (mpMorf->isStop()) {
                    setBck(0x1b, 2, 5.0f, 1.0f);
                    mSound.startCreatureVoice(Z2SE_EN_OC_V_WAIT, -1);
                    if (field_0x6b4 == 0) {
                        field_0x6c0 = cM_rndF(10.0f) + 10.0f;
                        mOcState = 3;
                    } else if (home.pos.abs(current.pos) > 100.0f) {
                        field_0x6c0 = cM_rndF(10.0f) + 10.0f;
                        mOcState = 3;
                    } else {
                        field_0x6c0 = cM_rndFX(30.0f) + 300.0f;
                        mOcState = 1;
                        field_0x6bc = home.angle.y;
                    }
                }
            }
            break;
        case 3:
            if (field_0x6c0 == 0) {
                setActionMode(E_OC_ACTION_WALK, 0);
            }
            break;
        default:
            break;
    }
}

void daE_OC_c::executeWalk() {
    mPrevShapeAngle = shape_angle.y;
    field_0x6de = 0;
    if (field_0x6e3 == 0) {
        if (setWatchMode()) {
            return;
        }

        if (searchSound()) {
            return;
        }

        if (searchPlayer2()) {
            setActionMode(E_OC_ACTION_FIND, 1);
            return;
        }
    }

    switch (mOcState) {
        case 0:
            setBck(0x1d, 2, 5.0f, 1.0f);
            field_0x6c0 = cM_rndFX(50.0f) + 150.0f;
            mOcState = 1;
            if (field_0x6b4 == 0) {
                if (home.pos.abs(current.pos) >= 500.0f) {
                    field_0x6bc = cLib_targetAngleY(&current.pos, &home.pos);
                } else {
                    field_0x6bc = cM_rndFX(32768.0f);
                }
            } else {
                field_0x6bc = cLib_targetAngleY(&current.pos, &home.pos);
            }

            field_0x6be = 0;
            break;
        case 1:
            if ((field_0x6c0 & 0x7) == 0) {
                int before_bg_val = checkBeforeBg();
                switch (before_bg_val) {
                    case 1:
                        field_0x6c0 = 0;
                        field_0x6be = 0;
                        break;
                    case 2:
                        if (field_0x6be == 0) {
                            field_0x6be = 0x1000;
                        }
                        break;
                    case 3:
                        if (field_0x6be == 0) {
                            field_0x6be = -0x1000;
                        }
                        break;
                    default:
                        break;
                }
                field_0x6bc += field_0x6be;
            }

            if (field_0x6c0 == 0) {
                setActionMode(E_OC_ACTION_WAIT, 0);
            } else if (field_0x6b4 == 1 && home.pos.abs(current.pos) < 100.0f) {
                setActionMode(E_OC_ACTION_WAIT, 0);
            } else {
                cLib_chaseF(&speedF, 4.0f, 1.0f);
                cLib_addCalcAngleS(&shape_angle.y, field_0x6bc, 4, 0x800, 0x100);
                current.angle.y = shape_angle.y;
            }

            break;
        default:
            break;
    }
}

void daE_OC_c::executeTalk() {
    field_0x6de = 0;
    if (setWatchMode()) {
        return;
    }

    if (searchSound()) {
        return;
    }

    if (searchPlayer2()) {
        setActionMode(E_OC_ACTION_FIND, 1);
        return;
    }

    switch (mOcState) {
        case 0:
            speedF = 0.0f;
            field_0x6c6 = cM_rndFX(10.0f) + 300.0f;
            field_0x6c0 = cM_rndF(30.0f);
            mOcState = 1;
            // fallthrough intentional.
        case 1: {
            if (field_0x6c0) {
                mPrevShapeAngle = shape_angle.y;
                return;
            }
            f32 rand_val = cM_rndF(1.0f);
            if (rand_val < 0.2f) {
                setBck(0x1a, 0, 5.0f, 1.0f);
                mOcState = 3;
                mSound.startCreatureVoice(Z2SE_EN_OC_V_TALK_B, -1);
            } else if (rand_val < 0.6f) {
                setBck(0x19, 2, 5.0f, 1.0f);
                mSound.startCreatureVoice(Z2SE_EN_OC_V_TALK, -1);
                mOcState = 2;
                field_0x6c0 = cM_rndF(30.0f) + 30.0f;
            } else {
                setBck(0x18, 2, 5.0f, 1.0f);
                mSound.startCreatureVoice(Z2SE_EN_OC_V_STAND_WAIT, -1);
                mOcState = 2;
                field_0x6c0 = cM_rndF(30.0f) + 30.0f;
            }
            break;
        }
        case 2:
            if (field_0x6c0 == 0) {
                mOcState = 1;
            }
            break;
        case 3:
            if (mpMorf->isStop()) {
                mOcState = 1;
            }
            break;
        case 4:
            mPrevShapeAngle = field_0x6bc;
            if (field_0x6c0 == 0) {
                setActionMode(E_OC_ACTION_WAIT, 0);
                field_0x6c6 = 300;
            }

            return;
        default:
            break;
    }
    if (mpTalk == NULL || field_0x6c6 == 0) {
        mPrevShapeAngle = field_0x6bc;
        mOcState = 4;
        field_0x6c0 = cM_rndF(30.0f);
    } else {
        field_0x6bc = mPrevShapeAngle = fopAcM_searchActorAngleY(this, mpTalk);
        field_0x6df = 1;
    }
}

void daE_OC_c::executeFind() {
    s16 pl_ang = fopAcM_searchPlayerAngleY(this);
    f32 pl_dist = fopAcM_searchPlayerDistance(this);
    if (mOcState < 3 || !setWatchMode()) {
        if (field_0x6b4 == 2 && !dComIfGp_event_runCheck()) {
            fopAcM_OffStatus(this, 0x4000);
            field_0x6b4 = 0;
        }

        mPrevShapeAngle = pl_ang;
        switch (mOcState) {
            case 0:
            case 1:
            case 2:
                field_0x6de = 0;
                if (mOcState == 0) {
                    field_0x6c0 = 0;
                    field_0x6c2 = field_0x6c0 + 0x14;
                    if (checkBck(0x1c) == 0) {
                        setBck(0x1c, 2, 5.0f, 1.0f);
                        mSound.startCreatureVoice(Z2SE_EN_OC_V_WAIT_ST, -1);
                    }

                    field_0x6de = 1;
                } else if (mOcState == 1) {
                    if (field_0x6e2) {
                        field_0x6c0 = l_HIO.demo_waiting_time;
                    } else {
                        field_0x6c0 = cM_rndF(10.0f) + 20.0f;
                    }

                    field_0x6e2 = 0;
                    field_0x6ca = 0x96;
                    setBck(0xd, 0, 5.0f, 1.0f);
                    mSound.startCreatureVoice(Z2SE_EN_OC_V_FIND, -1);
                } else {
                    field_0x6c0 = cM_rndF(5.0f) + 5.0f;
                    setBck(0xd, 0, 5.0f, 1.0f);
                    mSound.startCreatureVoice(Z2SE_EN_OC_V_FIND, -1);
                }

                mOcState = 3;
                speedF = 0.0f;
            /* fallthrough intentional */
            case 3:
                if (field_0x6c0 == 0) {
                    if (field_0x6de || pl_dist < 400.0f) {
                        setBck(0x1e, 2, 5.0f, 1.3f);
                        field_0x6de = 1;
                        mSound.startCreatureVoice(Z2SE_EN_OC_V_WALK_ST, -1);
                    } else {
                        setBck(0xb, 2, 5.0f, 1.2f);
                        mSound.startCreatureVoice(Z2SE_EN_OC_V_DASH, -1);
                    }

                    mOcState = 4;
                    field_0x6c0 = 0x1e;
                }

                break;
            case 4:
                if (checkBeforeFloorBg(200.0f)) {
                    if (field_0x6e3) {
                        setActionMode(E_OC_ACTION_MOVE_OUT, 0);
                    } else {
                        setActionMode(E_OC_ACTION_FIND_STAY, 0);
                    }

                    return;
                }

                mBattleOn = true;
                if ((s16)cLib_distanceAngleS(shape_angle.y, pl_ang) >= 0x400) {
                    cLib_addCalcAngleS(&shape_angle.y, pl_ang, 4, 0x800, 0x100);
                }

                current.angle.y = shape_angle.y;
                if (field_0x6ce) {
                    current.angle.y = field_0x6da;
                } else {
                    if (checkBeforeBgFind()) {
                        field_0x6ce = 20;
                    }
                }

                if (!dComIfGp_event_runCheck()) {
                    if (field_0x6c2 == 0) {
                        if (pl_dist < 300.0f) {
                            cLib_chaseF(&speedF, -3.0f, 1.0f);
                        } else {
                            cLib_chaseF(&speedF, 20.0f + nREG_F(0), 1.0f);
                        }

                        if (pl_dist < 400.0f && pl_dist > 200.0f) {
                            if (abs(shape_angle.y - fopAcM_searchPlayerAngleY(this)) < 0x1000) {
                                if (!dComIfGp_event_runCheck()) {
                                    setActionMode(E_OC_ACTION_ATTACK, 0);
                                }
                            }

                            return;
                        }
                    } else {
                        if (pl_dist < 400.0f && pl_dist > 200.0f) {
                            cLib_chaseF(&speedF, 0.0f, 1.0f);
                        } else if (pl_dist < 300.0f) {
                            cLib_chaseF(&speedF, -3.0f, 1.0f);
                        } else {
                            cLib_chaseF(&speedF, 20.0f + nREG_F(0), 1.0f);
                        }

                        if (speedF < 0.0f) {
                            if (checkBck(0x1c) == 0 || mpMorf->getPlaySpeed() != 1.0f) {
                                setBck(0x1c, 2, 5.0f, 1.0f);
                                mSound.startCreatureVoice(Z2SE_EN_OC_V_WAIT_ST, -1);
                            }
                        } else if (!speedF && (s16)cLib_distanceAngleS(shape_angle.y, pl_ang) < 0x1000) {
                            if (checkBck(0x1c) == 0) {
                                setBck(0x1c, 2, 5.0f, 1.0f);
                                mSound.startCreatureVoice(Z2SE_EN_OC_V_WAIT_ST, -1);
                            }
                        } else {
                            if (checkBck(0x1e) == 0 || mpMorf->getPlaySpeed() != 1.3f) {
                                setBck(0x1e, 2, 5.0f, 1.3f);
                                mSound.startCreatureVoice(Z2SE_EN_OC_V_WALK_ST, -1);
                            }
                        }
                    }
                } else {
                    field_0x6c2 = 0x14;
                    if (mAcch.ChkWallHit()) {
                        field_0x6c4 = 5;
                    }

                    if (pl_dist < 400.0f && field_0x6c4 == 0) {
                        cLib_chaseF(&speedF, -15.0f, 1.0f);
                    } else {
                        cLib_chaseF(&speedF, 0.0f, 1.0f);
                    }

                    if (speedF < 0.0f) {
                        if (checkBck(0x1e) == 0 || mpMorf->getPlaySpeed() != -1.0f) {
                            setBck(0x1e, 2, 5.0f, -1.0f);
                            mSound.startCreatureVoice(Z2SE_EN_OC_V_WALK_ST, -1);
                        }
                    } else if (!speedF && (s16)cLib_distanceAngleS(shape_angle.y, pl_ang) < 0x1000) {
                        if (checkBck(0x1c) == 0) {
                            setBck(0x1c, 2, 5.0f, 1.0f);
                            mSound.startCreatureVoice(Z2SE_EN_OC_V_WAIT_ST, -1);
                        }
                    } else {
                        if (checkBck(0x1e) == 0 || mpMorf->getPlaySpeed() != 1.3f) {
                            setBck(0x1e, 2, 5.0f, 1.3f);
                            mSound.startCreatureVoice(Z2SE_EN_OC_V_WALK_ST, -1);
                        }
                    }

                    if (0 == strcmp("D_MN05", dComIfGp_getStartStageName())
                        && dComIfGp_getStartStageRoomNo() == 0x16) {
                        // Forest Temple - Entrance
                        fopAc_ac_c* ks_actor;
                        // Caged girl monkey in 1st room:
                        fopAcM_SearchByName(PROC_NPC_KS, &ks_actor);
                        if (ks_actor) {
                            mWatchPos = ks_actor->current.pos;
                            mWatchPos.y += 100.0f;
                            mPrevShapeAngle = cLib_targetAngleY(&current.pos, &mWatchPos);
                        }
                    }
                }

                if (field_0x6c0 == 0) {
                    if (!searchPlayer2()) {
                        setActionMode(E_OC_ACTION_WAIT, 0);
                    } else if (field_0x6e3) {
                        setActionMode(E_OC_ACTION_MOVE_OUT, 0);
                    }
                }

                break;
            case 5:
                setBck(0x16, 0, 5.0f, 1.0f);
                mSound.startCreatureVoice(Z2SE_EN_OC_V_SEARCH, -1);
                mOcState = 6;
                field_0x6de = 0;
                // fallthrough intentional.
            case 6:
                mBattleOn = true;
                mPrevShapeAngle = shape_angle.y;
                if (mpMorf->isStop()) {
                    mOcState = 0;
                }

                if (pl_dist < 500.0f) {
                    mOcState = 0;
                }

                if (mpMorf->getFrame() >= 20.0f && searchPlayerShakeHead()) {
                    mOcState = 0;
                }

                break;
            default:
                break;
        }
    }
}

void daE_OC_c::setWeaponGroundAngle() {
    s16 my_val = 0;
    if (checkBck(5) && mpMorf->getFrame() >= 30.0f && mpMorf->getFrame() <= 45.0f) {
        dBgS_GndChk gnd_chk;
        cXyz my_vec_0;
        cXyz my_vec_1;
        mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(0xc));
        mDoMtx_stack_c::multVecZero(&my_vec_0);
        my_vec_0.y += 100.0f;
        mDoMtx_stack_c::transM(0.0f,0.0f,100.0f);
        mDoMtx_stack_c::multVecZero(&my_vec_1);
        my_vec_1.y += 100.0f;
        gnd_chk.SetPos(&my_vec_0);
        my_vec_0.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
        if (my_vec_0.y != -G_CM3D_F_INF) {
            gnd_chk.SetPos(&my_vec_1);
            my_vec_1.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
            if (my_vec_1.y != -G_CM3D_F_INF) {
                my_val = (s16) -cM_atan2s(my_vec_1.y - my_vec_0.y, my_vec_0.absXZ(my_vec_1));
                if (my_val < -0x2000) {
                    my_val = -0x2000;
                }

                if (my_val > 0x2000) {
                    my_val = 0x2000;
                }
            }
        }
    }

    cLib_addCalcAngleS(&field_0x6d0, my_val, 4, 0x1000, 0x100);
}

void daE_OC_c::executeAttack() {
    f32 my_float = 0.0f;
    int frame_ctrl = (mpMorf->getFrame() - 9.0f);
    if (frame_ctrl >= 0) {
        if (frame_ctrl >= 9) {
            frame_ctrl = 9;
        }

        if (checkBck(5)) {
            my_float = E_OC_n::oc_attackb_trans[frame_ctrl];
        } else {
            my_float = E_OC_n::oc_attackc_trans[frame_ctrl];
        }
    }

    mBattleOn = true;
    mPrevShapeAngle = shape_angle.y;
    switch (mOcState) {
        case 0: {
            if (cLib_chaseF(&speedF, 0.0f, 1.0f)) {
                if (cM_rndF(1.0f) < 0.5f) {
                    setBck(5, 0, 5.0f, 1.0f);
                    mSound.startCreatureVoice(Z2SE_EN_OC_V_ATTACK_B, -1);
                    mOcState = 1;
                } else {
                    setBck(6, 0, 5.0f, 1.0f);
                    mSound.startCreatureVoice(Z2SE_EN_OC_V_ATTACK_C, -1);
                    mOcState = 2;
                }

                field_0x6a0 = 0.0f;
            }

            break;
        }

        case 2: {
            MtxP my_mtx_p = mpMorf->getModel()->getAnmMtx(0);
            cXyz my_vec_0(my_mtx_p[0][3], my_mtx_p[1][3], my_mtx_p[2][3]);
            mParticleKeys[0] = dComIfGp_particle_set(mParticleKeys[0], dPa_RM(ID_ZM_S_ORCATTACKBLUR00),
                                                     &my_vec_0, &tevStr,
                                                     &shape_angle, NULL, -1, NULL, -1,
                                                     NULL, NULL, NULL);
            mParticleKeys[1] = dComIfGp_particle_set(mParticleKeys[1], dPa_RM(ID_ZM_S_ORCATTACKBLUR01),
                                                     &my_vec_0, &tevStr,
                                                     &shape_angle, NULL, -1, NULL, -1,
                                                     NULL, NULL, NULL);
        }
        // fallthrough intentional
        case 1: {
            if (mOcState == 1) {
                if (mpMorf->checkFrame(10.0f)) {
                    mSound.startCreatureVoice(Z2SE_EN_OC_V_ATTACK_B2, -1);
                } else if (mpMorf->checkFrame(12.5f)) {
                    mSound.startCreatureSound(Z2SE_EN_OC_ATTACK_B, 0, -1);
                } else if (mpMorf->checkFrame(19.0f)) {
                    if (mName == "E_OC") {
                        mSound.startCreatureSound(Z2SE_EN_OC_CLUB_HIT, 0, -1);
                    } else {
                        mSound.startCreatureSound(Z2SE_EN_OC_NATA_HIT, 0, -1);
                    }
                }
            } else {
                if (mpMorf->checkFrame(7.0f)) {
                    mSound.startCreatureVoice(Z2SE_EN_OC_V_ATTACK_C2, -1);
                } else if (mpMorf->checkFrame(11.5f)) {
                    mSound.startCreatureSound(Z2SE_EN_OC_ATTACK_C, 0, -1);
                }
            }

            if (mpMorf->getFrame() >= 14.0f && mpMorf->getFrame() <= 22.0f) {
                mSphs_at[0].OnAtSetBit();
                mSphs_at[1].OnAtSetBit();
            }

            if (mpMorf->getFrame() >= 22.0f) {
                mPrevShapeAngle = fopAcM_searchPlayerAngleY(this);
            }

            u8 my_bool = 0;
            fopAc_ac_c* hit_actor;
            if (mSphs_at[0].ChkAtHit()) {
                hit_actor = dCc_GetAc(mSphs_at[0].GetAtHitObj()->GetAc());
                if (fopAcM_GetName(hit_actor) == PROC_ALINK) {
                    my_bool = 1;
                }
            }

            if (mSphs_at[1].ChkAtHit()) {
                hit_actor = dCc_GetAc(mSphs_at[1].GetAtHitObj()->GetAc());
                if (fopAcM_GetName(hit_actor) == PROC_ALINK) {
                    my_bool = 1;
                }
            }

            if (my_bool && daPy_getPlayerActorClass()->checkPlayerGuard()) {
                mpMorf->setPlaySpeed(-1.0);
                mOcState = 3;
                dComIfGp_getVibration().StartShock(3, 0x1f, cXyz(0.0f, 1.0f, 0.0f));
            }

            current.pos.x += (my_float - field_0x6a0) * cM_ssin(shape_angle.y);
            current.pos.z += (my_float - field_0x6a0) * cM_scos(shape_angle.y);
            field_0x6a0 = my_float;
            if (mpMorf->isStop()) {
                setBck(0x1c, 2, 0.0f, 1.0f);
                mSound.startCreatureVoice(Z2SE_EN_OC_V_WAIT_ST, -1);
                if (field_0x6e3) {
                    setActionMode(E_OC_ACTION_MOVE_OUT, 0);
                } else if (field_0x6ca && fopAcM_searchPlayerDistance(this) < 500.0f) {
                    mOcState = 0;
                } else {
                    setActionMode(E_OC_ACTION_FIND, 0);
                }
            }

            break;
        }

        case 3: {
            current.pos.x += (my_float - field_0x6a0) * cM_ssin(shape_angle.y);
            current.pos.z += (my_float - field_0x6a0) * cM_scos(shape_angle.y);
            field_0x6a0 = my_float;
            if (mpMorf->isStop()) {
                setBck(0x1c, 2, 5.0f, 1.0f);
                mSound.startCreatureVoice(Z2SE_EN_OC_V_WAIT_ST, -1);
                if (field_0x6e3) {
                    setActionMode(E_OC_ACTION_MOVE_OUT, 0);
                } else {
                    if (field_0x6ca && fopAcM_searchPlayerDistance(this) < 500.0f) {
                        if (abs(shape_angle.y - fopAcM_searchPlayerAngleY(this)) < 0x1000) {
                            mOcState = 0;
                            break;
                        }
                    }

                    setActionMode(E_OC_ACTION_FIND, 0);
                }
            }
            break;
        }
    }

    current.angle.y = shape_angle.y;
}

void daE_OC_c::executeDamage() {
    mBattleOn = true;
    mPrevShapeAngle = shape_angle.y;
    switch (mOcState) {
        case 0:
            speedF = 0.0f;
            setBck(0x9, 0, 0.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_OC_V_DAMAGE_LR, -1);
            mOcState = 5;
            break;
        case 1:
            speedF = 0.0f;
            setBck(0xa, 0, 0.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_OC_V_DAMAGE_LR, -1);
            mOcState = 5;
            break;
        case 2:
            speedF = 0.0f;
            setBck(0x8, 0, 0.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_OC_V_DAMAGE_LR, -1);
            mOcState = 5;
            break;
        case 3:
            setBck(0x8, 0, 0.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_OC_V_DAMAGE, -1);
            mOcState = 5;
            if (s16(cLib_distanceAngleS(shape_angle.y, fopAcM_searchPlayerAngleY(this))) < 0x4000) {
                speedF = -20.0f;
            } else {
                speedF = 20.0f;
            }
            onHeadLockFlg();
            break;
        case 4:
            setBck(0xf, 0, 0.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_OC_V_GALE, -1);
            mOcState = 6;
            speedF = 0.0f;
            break;
        case 5:
            setSpitEffect();
            cLib_chaseF(&speedF, 0.0f, 3.0f);
            if (speedF) {
                fopAcM_effSmokeSet2(&field_0xe60, &field_0xe64, &current.pos, NULL, 2.0f, &tevStr);
            }
            if (mpMorf->isStop()) {
                if (daPy_getPlayerActorClass()->getCutType() != daPy_py_c::CUT_TYPE_HEAD_JUMP) {
                    offHeadLockFlg();
                    if (field_0x6e3) {
                        setActionMode(E_OC_ACTION_MOVE_OUT, 0);
                    } else {
                        setActionMode(E_OC_ACTION_FIND, 0);
                    }
                }
            }
            break;
        case 6:
            setSpitEffect();
            if (mpMorf->isStop()) {
                if (field_0x6e3) {
                    setActionMode(E_OC_ACTION_MOVE_OUT, 0);
                } else {
                    setActionMode(E_OC_ACTION_FIND, 0);
                }
            }
            break;
    }
}

void daE_OC_c::executeBigDamage() {
    mBattleOn = true;
    mPrevShapeAngle = shape_angle.y;
    switch (mOcState) {
        case 0:
        case 1:
        case 2:
            if (mOcState) {
                field_0x6dc = 1;
                setBck(0x15, 0, 0.0f, 1.0f);
                if (mOcState == 1) {
                    field_0x6bc = 0x1f60;
                } else {
                    field_0x6bc = -0x1f60;
                }
                if (health < 1) {
                    mSound.startCreatureVoice(Z2SE_EN_OC_V_DEATH, -1);
                } else {
                    mSound.startCreatureVoice(Z2SE_EN_OC_V_LEAP_SPIN, -1);
                }
            } else {
                field_0x6dc = 0;
                setBck(0x13, 0, 0.0f, 1.0f);
                field_0x6bc = 0;
                if (health < 1) {
                    mSound.startCreatureVoice(Z2SE_EN_OC_V_DEATH, -1);
                } else {
                    mSound.startCreatureVoice(Z2SE_EN_OC_V_LEAP_A, -1);
                }
            }
            offTgSph();
            mOcState = 3;
            speed.y = 40.0f;
            speedF = 20.0f;
            current.angle.y = mAtInfo.mHitDirection.y + 0x8000;
            shape_angle.y = mAtInfo.mHitDirection.y;
            field_0x6be = 0;
            break;
        case 3:
            if (field_0x6dc == 0) {
                setSpitEffect();
            } else if (field_0x6c2) {
                setSpitEffect();
            } else {
                field_0x6c2 = 5;
            }
            checkDamageBg();
            if (field_0x6bc) {
                field_0x6d8 += field_0x6bc;
                cLib_chaseAngleS(&field_0x6bc, 0, 0x100);
                cLib_chaseAngleS(&shape_angle.x, -0x4000, 0x400);
            }
            if (mAcch.ChkWallHit()) {
                speedF = 0.0f;
            }
            if (mAcch.ChkGroundHit()) {
                shape_angle.x = field_0x6d8 = 0;
                setBck(0x14, 0, 0.0f, 1.0f);
                mSound.startCreatureSound(Z2SE_CM_BODYFALL_M, 0, -1);
                if (field_0x6dc) {
                    mpMorf->setFrame(5.0f);
                    mSound.startCreatureSound(Z2SE_EN_OC_LEAP_B, 0, -1);
                }
                mOcState = 4;
                fopAcM_effSmokeSet1(&field_0xe60, &field_0xe64, &current.pos, NULL, 2.0f, &tevStr, 1);
            }
            break;
        case 4:
            setGroundAngle();
            if (mpMorf->checkFrame(15.0f)) {
                mSound.startCreatureSound(Z2SE_CM_BODYFALL_M, 0, -1);
            }
            if (field_0x6dc == 0 && mpMorf->checkFrame(5.0f)) {
                mSound.startCreatureSound(Z2SE_EN_OC_LEAP_B, 0, -1);
            }
            checkDamageBg();
            if (mAcch.ChkWallHit()) {
                speedF = 0.0f;
            }
            cLib_chaseF(&speedF, 0.0f, 1.0f);
            if (mpMorf->isStop()) {
                if (health < 1) {
                    setActionMode(E_OC_ACTION_DEATH, 0);
                    offTgSph();
                } else {
                    mOcState = 5;
                    onDownFlg();
                    setStabPos();
                    field_0x6c0 = cM_rndFX(10.0f) + 45.0f;
                }
            }
            break;
        case 5:
            setGroundAngle();
            checkDamageBg();
            setStabPos();
            cLib_chaseF(&speedF, 0.0f, 1.0f);
            mSphs_cc[0].OnCoSetBit();
            mSphs_cc[1].OnCoSetBit();
            if (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_DOWN) {
                mSphs_cc[0].OffCoSetBit();
                mSphs_cc[1].OffCoSetBit();
                mStts.ClrCcMove();
                field_0x6c0 = 0x1e;
            }
            if (field_0x6c0 == 0) {
                setBck(0x17, 0, 5.0f, 1.0f);
                mSound.startCreatureVoice(Z2SE_EN_OC_V_STAND, -1);
                mOcState = 6;
                offDownFlg();
            }
            break;
        case 6:
            cLib_chaseAngleS(&field_0x688.x, 0, 0x400);
            cLib_chaseAngleS(&field_0x688.z, 0, 0x400);
            if (mpMorf->isStop()) {
                setActionMode(E_OC_ACTION_FIND, 5);
            }
        default:
            break;
    }
}

void daE_OC_c::executeWatch() {
    mPrevShapeAngle = shape_angle.y;
    switch (mOcState) {
        case 0:
            speedF = 0.0f;
            if (field_0x6de) {
                setBck(0x1c, 2, 5.0f, 1.0f);
                mSound.startCreatureVoice(Z2SE_EN_OC_V_WAIT_ST, -1);
            } else {
                setBck(0x1b, 2, 5.0f, 1.0f);
                mSound.startCreatureVoice(Z2SE_EN_OC_V_WAIT, -1);
            }
            field_0x6c0 = (s8) (cM_rndFX(15.0f) + 35.0f);
            mOcState = 1;
        case 1:
            if (mpDamage && fopAcM_IsActor(mpDamage)) {
                mPrevShapeAngle = fopAcM_searchActorAngleY(this, mpDamage);
            }
            if (field_0x6c0 == 0) {
                setActionMode(E_OC_ACTION_FIND, 2);
            }
            break;
        case 2:
            speedF = 0.0f;
            if (field_0x6de) {
                setBck(0x1c, 2, 5.0f, 1.0f);
                mSound.startCreatureVoice(Z2SE_EN_OC_V_WAIT_ST, -1);
            } else {
                setBck(0x1b, 2, 5.0f, 1.0f);
                mSound.startCreatureVoice(Z2SE_EN_OC_V_WAIT, -1);
            }
            field_0x6c0 = (s8) (cM_rndFX(15.0f) + 35.0f);
            mOcState = 3;
        case 3:
            mPrevShapeAngle = cLib_targetAngleY(&current.pos, &mWatchPos);
            if (field_0x6c0 == 0) {
                mOcState = 4;
                setBck(7, 0, 5.0f, 1.0f);
                mSound.startCreatureVoice(Z2SE_EN_OC_V_CRY, -1);
            }
            break;
        case 4:
            mPrevShapeAngle = fopAcM_searchPlayerAngleY(this);
            if (mpMorf->isStop()) {
                setActionMode(E_OC_ACTION_FIND, 2);
            }
            break;
        default:
            break;
    }
}

void daE_OC_c::executeSoundWatch() {
    field_0x6bc = mPrevShapeAngle = cLib_targetAngleY(&current.pos, &mWatchPos);
    if (mOcState == 2) {
        field_0x6de = 0;
        if (setWatchMode()) {
            return;
        }

        if (searchPlayer2()) {
            setActionMode(E_OC_ACTION_FIND, 3);
            return;
        }
    }

    switch (mOcState) {
        case 0:
            speedF = 0.0f;
            setBck(0xd, 0, 5.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_OC_V_FIND, -1);
            field_0x6c0 = cM_rndF(30.0f) + 60.0f;
            mOcState = 1;
        case 1:
            if (field_0x6c0 == 0) {
                setBck(0xb, 2, 5.0f, 1.2f);
                mSound.startCreatureVoice(Z2SE_EN_OC_V_DASH, -1);
                mOcState = 2;
                field_0x6c0 = 0x3c;
            }
            break;
        case 2:
            cLib_chaseF(&speedF, 20.0f + nREG_F(0), 1.0f);
            cLib_addCalcAngleS(&shape_angle.y, field_0x6bc, 4, 0x800, 0x100);
            current.angle.y = shape_angle.y;
            if (current.pos.abs(mWatchPos) < 300.0f || field_0x6c0 == 0) {
                setBck(0x16, 0, 5.0f, 1.0f);
                mOcState = 3;
                speedF = 0.0f;
            }
            break;
        case 3:
            if (mpMorf->isStop()) {
                setActionMode(E_OC_ACTION_WAIT, 0);
            }
            break;
        default:
            break;
    }
}

bool daE_OC_c::checkBeforeDeath() {
    if (mActionMode == E_OC_ACTION_DEATH && mOcState >= 2 && field_0x6c0 < 2) {
        return true;
    }

    return false;
}

void daE_OC_c::executeDeath() {
    mBattleOn = true;
    mPrevShapeAngle = shape_angle.y;
    switch (mOcState) {
        case 0:
        case 1:
            field_0x6e8 = 1;
            offTgSph();
            if (mOcState == 0) {
                mOcState = 2;
                field_0x6c0 = 0x1e;
            } else {
                field_0x6c0 = 0x2d;
                setBck(0xe, 0, 0.0f, 1.0f);
                mSound.startCreatureVoice(Z2SE_EN_OC_V_FINISH, -1);
                mOcState = 3;
                offDownFlg();
            }
            break;
        case 3:
            setSpitEffect();
            if (mpMorf->checkFrame(28.5f)) {
                mSound.startCreatureSound(Z2SE_CM_BODYFALL_S, 0, -1);
            }
        case 2:
            cLib_chaseF(&speedF, 0.0f, 1.0f);
            if (field_0x6c0 == 0) {
                if (mName == "E_OC") {
                    fopAcM_createDisappear(this, &current.pos, 10, 0, 4);
                } else {
                    fopAcM_createDisappear(this, &current.pos, 10, 0, 0x34);
                }

                fopAcM_delete(this);
                if (mDefeatedSwitch != 0xFF && dComIfGs_isSwitch(mDefeatedSwitch, fopAcM_GetRoomNo(this)) == 0) {
                    dComIfGs_onSwitch(mDefeatedSwitch, fopAcM_GetRoomNo(this));
                }
            }
            break;
        default:
            break;
    }
    if (field_0x6e8) {
        cLib_addCalc2(&field_0x6e4, -20.0f, 1.0f, 0.4f);
    }
}

void daE_OC_c::setWaterEffect() {
    cXyz my_vec_0(current.pos.x, mWaterLvl, current.pos.z);
    static cXyz sc(1.0f, 1.0f, 1.0f);
    static u16 w_eff_id[4] = {
        ID_ZI_J_DOWNWTRA_A,
        ID_ZI_J_DOWNWTRA_B,
        ID_ZI_J_DOWNWTRA_C,
        ID_ZI_J_DOWNWTRA_D,
    };

    for (int idx = 0; idx < 4; ++idx) {
        mWaterEffects[idx] = dComIfGp_particle_set(mWaterEffects[idx], w_eff_id[idx], &my_vec_0,
                                                 &tevStr, NULL, &sc, -1, NULL,
                                                 -1, NULL, NULL, NULL);
    }
}

void daE_OC_c::executeWaterDeath() {
    switch (mOcState) {
        case 0:
            setBck(0xC, 0, 0.0f, 1.0f);
            mSound.startCreatureVoice(Z2SE_EN_OC_V_DROWNED, -1);
            mSound.startCreatureSound(Z2SE_EN_OC_DROWNED, 0, -1);
            current.pos.y += 140.0f;
            mOcState = 1;
            gravity = speedF = 0.0f;
            setWaterEffect();
            mSound.startCreatureSound(Z2SE_CM_BODYFALL_WATER_M, 0, -1);
            offTgSph();
            break;
        case 1:
            cLib_chaseF(&speed.y, 5.0f, 3.0f);
            cLib_chaseAngleS(&shape_angle.x, 0, 0x800);

            if (mpMorf->checkFrame(24.0f) ||
                mpMorf->checkFrame(37.0f) ||
                mpMorf->checkFrame(49.0f)) {
                setWaterEffect();
            }

            if (speed.y > 0.0f && current.pos.y > mWaterLvl) {
                current.pos.y = mWaterLvl;
                mOcState = 2;
                speed.y = 0.0f;
            }
            break;
        case 2:
            if (mpMorf->checkFrame(24.0f) ||
                mpMorf->checkFrame(37.0f) ||
                mpMorf->checkFrame(49.0f)) {
                setWaterEffect();
            }

            if (mpMorf->isStop()) {
                if (mName == "E_OC") {
                    fopAcM_createDisappear(this, &current.pos, 10, 0, 4);
                } else {
                    fopAcM_createDisappear(this, &current.pos, 10, 0, 0x34);
                }
                fopAcM_delete(this);

                if (mDefeatedSwitch != 0xFF && !dComIfGs_isSwitch(mDefeatedSwitch, fopAcM_GetRoomNo(this))) {
                    dComIfGs_onSwitch(mDefeatedSwitch, fopAcM_GetRoomNo(this));
                }
            }
            break;
        default:
            break;
    }
    
    if (current.pos.y - 140.0f < mWaterLvl && mWaterLvl < current.pos.y + 50.0f) {
        cXyz hamon_pos(current.pos.x, mWaterLvl, current.pos.z);
        fopAcM_effHamonSet(&mRippleKey, &hamon_pos, 1.0f, 1.0f);
    }
}

void daE_OC_c::executeDemoMaster() {
    cXyz my_vec_0(-4264.0f, 302.0f, -2330.0f);
    cXyz my_vec_1(16200.0f, 2850.0f, 7000.0f);
    my_vec_1 += my_vec_0;
    mPrevShapeAngle = shape_angle.y;
    camera_class* p_camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    switch (mOcState) {
        case 0:
            mHide = false;
            mOcState = 1;
            field_0x6f8.set(16300.0f, 3700.0f, 8400.0f);
            field_0x6f8 += my_vec_0;
            mPlayerPos.set(16325.0f, 2858.0f, 6597.0f);
            mPlayerPos += my_vec_0;
            field_0x704 = 55.0f;
            field_0x6bc = 0x5000;
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&my_vec_1, field_0x6bc, 0);
            p_camera->mCamera.SetTrimSize(3);
            shape_angle.y = current.angle.y = fopAcM_searchPlayerAngleY(this);
            return;
        case 1:
            mSound.startCreatureVoice(Z2SE_EN_OC_V_SAKEBU, -1);
            setBck(7, 0, 5.0f, 1.0f);
            mOcState = 2;
            field_0x6c0 = 15;
            break;
        case 2:
            if (field_0x6c0 == 0) {
                daPy_getPlayerActorClass()->changeOriginalDemo();
                daPy_getPlayerActorClass()->changeDemoMode(0x19, 0, 0, 0);
                mOcState = 4;
            }
            break;
        case 4:
        case 5:
            cLib_addCalc2(&field_0x704, 13.0f, 0.2f, 2.0f);
            if (mOcState == 4) {
                if (mpMorf->isStop()) {
                    setBck(4, 0, 5.0f, 1.0f);
                    mSound.startCreatureVoice(Z2SE_EN_OC_V_ANGRY_B, -1);
                    mOcState = 5;
                }
            } else {
                if (mpMorf->checkFrame(9.0f)) {
                    mSound.startCreatureSound(Z2SE_EN_OC_ATTACK_C, 0, -1);
                }

                cLib_chaseAngleS(&field_0x6bc, fopAcM_searchPlayerAngleY(this) + 0x8000, 0x200);
                if (mpMorf->isStop()) {
                    mOcState = 6;
                    setBck(0x10, 0, 5.0f, 1.0f);
                    mSound.startCreatureVoice(Z2SE_EN_OC_V_JUMP, -1);
                    speedF = 10.0f;
                    speed.y = 40.0f;
                    field_0x6c0 = 0x28;
                    daPy_getPlayerActorClass()->changeDemoMode(1, 0, 0, 0);
                    daPy_getPlayerActorClass()->cancelOriginalDemo();
                }
            }
            break;
        case 6:
            if (field_0x6c0 > 20) {
                cLib_addCalc2(&field_0x6f8.y, current.pos.y + 100.0f, 0.2f, 10.0f);
            }

            if (mpMorf->isStop()) {
                setBck(0x11, 0, 5.0f, 1.0f);
            }

            if (field_0x6c0 == 0) {
                mOcState = 7;
                field_0x6f8.set(16450.0f, 3050.0f, 8250.0f);
                field_0x6f8 += my_vec_0;
                mPlayerPos.set(16310.0f, 2970.0f, 7500.0f);
                mPlayerPos += my_vec_0;
                field_0x704 = 37.0f;
                current.pos.set(16449.0f, 3300.0f, 7879.0f);
                current.pos += my_vec_0;
                shape_angle.y = current.angle.y =  fopAcM_searchPlayerAngleY(this);
                speedF = 0.0f;
                field_0x6c0 = 10;
            }
            break;
        case 7:
            if (field_0x6c0 == 0) {
                cLib_addCalc2(&field_0x6f8.y, my_vec_0.y + 2950.0f, 0.1f, 10.0f);
            }

            if (mAcch.ChkGroundHit()) {
                mSound.startCreatureSound(Z2SE_CM_BODYFALL_S, 0, -1);
                fopAcM_effSmokeSet1(&field_0xe60, &field_0xe64, &current.pos, NULL, 2.0f, &tevStr, 1);
                setBck(0x12, 0, 5.0f, 1.0f);
                mOcState = 8;
            }

            break;
        case 8:
            field_0x704 = 47.0f;
            cLib_addCalc2(&field_0x6f8.y, my_vec_0.y + 2950.0f, 0.1f, 10.0f);
            if (mpMorf->checkFrame(34.0f)) {
                mSound.startCreatureVoice(Z2SE_EN_OC_V_FIND, -1);
            }

            if (mpMorf->isStop()) {
                field_0x6c8 = 30;
                setActionMode(E_OC_ACTION_FIND, 0);
                Z2GetAudioMgr()->onEnemyDamage();
                Z2GetAudioMgr()->unMuteSceneBgm(1);
                return;
            }

            break;
    }

    if (mOcState) {
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&my_vec_1, field_0x6bc, 0);
        p_camera->mCamera.Set(field_0x6f8, mPlayerPos, field_0x704, 0);
    }
}

void daE_OC_c::executeDemoChild() {
    cXyz local_18(-4264.0f, 302.0f, -2330.0f);
    mPrevShapeAngle = shape_angle.y;
    fopAcM_SearchByID(fopAcM_GetLinkId(this), (fopAc_ac_c**)&mpParent);
    if (mpParent) {
        switch (mOcState) {
            case 0:
                if (mpParent->mOcState == 1) {
                    mHide = false;
                    setBck(0x1C, 2, 5.0f, 1.0f);
                    mOcState = 1;
                }
                break;
            case 1:
                if (mpParent->mOcState == 5) {
                    setBck(3, 0, 5.0f, 1.0f);
                    mSound.startCreatureVoice(Z2SE_EN_OC_V_ANGRY_A, -1);
                    mOcState = 5;
                }
                break;
            case 5:
                if (mpMorf->isStop()) {
                    setBck(0x10, 0, 5.0f, 1.0f);
                    mSound.startCreatureVoice(Z2SE_EN_OC_V_JUMP, -1);
                    mOcState = 6;
                    speedF = 10.0f;
                    speed.y = 40.0f;
                }
                break;
            case 6:
                if (mpMorf->isStop()) {
                    setBck(0x11, 0, 5.0f, 1.0f);
                }
                if (mpParent->mOcState == 7) {
                    current.pos.set(16249.0f, 4000.0f, 8036.0f);
                    current.pos += local_18;
                    shape_angle.y = current.angle.y = fopAcM_searchPlayerAngleY(this);
                    speedF = speed.y = 0.0f;
                    mOcState = 7;
                }
                break;
            case 7:
                if (mAcch.ChkGroundHit()) {
                    mSound.startCreatureSound(Z2SE_CM_BODYFALL_S, 0, -1);
                    fopAcM_effSmokeSet1(&field_0xe60, &field_0xe64, &current.pos, NULL, 2.0f, &tevStr, 1);
                    setBck(0x12, 0, 5.0f, 1.0f);
                    mOcState = 8;
                }
                break;
            case 8:
                if (mpMorf->checkFrame(34.0f)) {
                    mSound.startCreatureVoice(Z2SE_EN_OC_V_FIND, -1);
                }
                if (mpMorf->isStop()) {
                    setActionMode(E_OC_ACTION_FIND, 0);
                }
                break;
            default:
                break;
        }
    }
}

void daE_OC_c::checkFall() {
    dBgS_GndChk gnd_chk;
    cXyz gnd_pos = current.pos;
    gnd_pos.y += 100.0f;
    gnd_chk.SetPos(&gnd_pos);
    f32 groundY = dComIfG_Bgsp().GroundCross(&gnd_chk);
    if (mAcch.ChkGroundHit()) {
        field_0x69c = groundY;
        return;
    }

    if (mActionMode != E_OC_ACTION_FALL_DEAD
     && mActionMode != E_OC_ACTION_FALL
     && mActionMode != E_OC_ACTION_WATER_DEATH
     && mActionMode != E_OC_ACTION_DEMO_MASTER
     && mActionMode != E_OC_ACTION_DEMO_CHILD
     && speed.y < 0.0f) {
        if (groundY == -G_CM3D_F_INF || dComIfG_Bgsp().GetGroundCode(gnd_chk) == 4
            || dComIfG_Bgsp().GetGroundCode(gnd_chk) == 10
            || dComIfG_Bgsp().GetGroundCode(gnd_chk) == 5) {
            if (field_0x69c > current.pos.y) {
                setActionMode(E_OC_ACTION_FALL_DEAD, 0);
            }
        } else if (mActionMode != E_OC_ACTION_BIG_DAMAGE && (field_0x69c - 100.0f) > current.pos.y
                   && (field_0x69c - groundY) > 500.0f) {
            setActionMode(E_OC_ACTION_FALL, 0);
        }
    }
}

void daE_OC_c::executeFallDead() {
    mPrevShapeAngle = shape_angle.y;
    switch (mOcState) {
        case 0:
            mSound.startCreatureVoice(Z2SE_EN_OC_V_FALL, -1);
            if (checkBck(0x15) || checkBck(0x13)) {
                mOcState = 1;
            } else {
                if (!checkBck(0xC)) {
                    setBck(0xC, 0, 0.0f, 1.0f);
                    current.pos.y += 140.0f;
                }
                mOcState = 2;
            }
            field_0x6c0 = 100;
            return;
        case 1:
            if (field_0x6bc != 0) {
                field_0x6d8 += field_0x6bc;
                cLib_chaseAngleS(&field_0x6bc, 0, 100);
                cLib_chaseAngleS(&shape_angle.x, -0x4000, 0x400);
            }
        case 2:
            cLib_chaseF(&speedF, 0.0f, 0.2f);
            if (field_0x6c0 == 0) {
                fopAcM_delete(this);
                if (mDefeatedSwitch != 0xFF && !dComIfGs_isSwitch(mDefeatedSwitch, fopAcM_GetRoomNo(this))) {
                    dComIfGs_onSwitch(mDefeatedSwitch, fopAcM_GetRoomNo(this));
                }
            }
            break;
        default:
            break;
    }
}

void daE_OC_c::executeFall() {
    mPrevShapeAngle = shape_angle.y;
    cLib_chaseF(&speedF, 0.0f, 0.1f);
    switch (mOcState) {
        case 0:
            setBck(0x11, 0, 5.0f, 1.0f);
            mOcState = 1;
            break;
        case 1:
            if (mAcch.ChkGroundHit()) {
                speedF = 0.0f;
                mSound.startCreatureSound(Z2SE_CM_BODYFALL_S, 0, -1);
                fopAcM_effSmokeSet1(&field_0xe60, &field_0xe64, &current.pos, NULL, 2.0f, &tevStr, 1);
                setBck(0x12, 0, 5.0f, 1.0f);
                mOcState = 2;
            }
            break;
        case 2:
            if (mpMorf->isStop()) {
                setActionMode(E_OC_ACTION_FIND, 0);
            }
            break;
        default:
            break;
    }
}

void daE_OC_c::executeFindStay() {
    s16 target_angle = fopAcM_searchPlayerAngleY(this);
    f32 target_dist = fopAcM_searchPlayerDistance(this);
    mPrevShapeAngle = target_angle;
    mBattleOn = true;

    if (!checkBeforeFloorBg(200.0f)) {
        setActionMode(E_OC_ACTION_FIND, 0);
        return;
    }

    switch (mOcState) {
        case 0:
            if (!speedF) {
                if (!checkBck(0x1C)) {
                    setBck(0x1C, 2, 5.0f, 1.0f);
                }
                mOcState = 2;
            } else {
                if (!checkBck(0x1E)) {
                    setBck(0x1E, 2, 5.0f, 1.0f);
                }
                mOcState = 1;
            }
            break;
        case 1:
            cLib_chaseF(&speedF, 0.0f, 2.0f);
            if (!speedF) {
                setBck(0x1C, 2, 5.0f, 1.0f);
                mOcState = 2;
            }
        case 2: {
            if (target_dist < 400.0f) {
                cLib_chaseF(&speedF, -15.0f, 1.0f);
            } else {
                cLib_chaseF(&speedF, 0.0f, 1.0f);
            }

            if (s16(cLib_distanceAngleS(shape_angle.y, target_angle)) >= 0x400) {
                cLib_addCalcAngleS(&shape_angle.y, target_angle, 4, 0x800, 0x100);
            }

            current.angle.y = shape_angle.y;
            if (target_dist < 400.0f && target_dist > 200.0f) {
                if (abs(shape_angle.y - fopAcM_searchPlayerAngleY(this)) < 0x1000 && checkBeforeFloorBg(100.0f)
                    && !dComIfGp_event_runCheck()) {
                    setActionMode(E_OC_ACTION_ATTACK, 0);
                }

                return;
            }

            if (!searchPlayer2()) {
                setActionMode(E_OC_ACTION_WAIT, 0);
                return;
            }
        }
    }
}

void daE_OC_c::executeMoveOut() {
    f32 player_distance = fopAcM_searchPlayerDistance(this);
    s16 target_angle = fopAcM_searchPlayerAngleY(this);
    s16 home_angle = cLib_targetAngleY(&home.pos, &current.pos);
    mBattleOn = true;
    mPrevShapeAngle = shape_angle.y;

    BOOL my_cond;
    switch (mOcState) {
        case 0:
            if (field_0x6e3 == 2) {
                if (!checkBck(0x1E)) {
                    setBck(0x1E, 2, 5.0f, 1.0f);
                }

                mOcState = 1;
            } else {
                if (!checkBck(0x1C)) {
                    setBck(0x1C, 2, 5.0f, 1.0f);
                }

                field_0x6c0 = 30;
                mOcState = 2;
            }
        // fallthrough intentional
        case 1:
            cLib_addCalcAngleS(&shape_angle.y, home_angle, 4, 0x800, 0x100);
            current.angle.y = shape_angle.y;
            cLib_chaseF(&speedF, -15.0f, 1.0f);
            if (home.pos.absXZ(current.pos) < mMoveRange) {
                setBck(0x1C, 2, 5.0f, 1.0f);
                field_0x6c0 = 30;
                mOcState = 2;
            }

            break;

        case 2:
            my_cond = FALSE;
            if (player_distance < 200.0f || home.pos.abs(current.pos) > mMoveRange + 200.0f) {
                if (abs(s16(shape_angle.y - home_angle)) < 0x3000) {
                    my_cond = TRUE;
                    cLib_chaseF(&speedF, -15.0f, 1.0f);
                }
            }

            if (my_cond == FALSE) {
                cLib_chaseF(&speedF, 0.0f, 1.0f);
                if (!speedF && !checkBck(0x1C)) {
                    setBck(0x1C, 2, 5.0f, 1.0f);
                }
            } else {
                if (!checkBck(0x1E)) {
                    setBck(0x1E, 2, 5.0f, 1.0f);
                }

                cLib_chaseF(&speedF, -15.0f, 1.0f);
                if (home.pos.abs(current.pos) < mMoveRange - 200.0f) {
                    setActionMode(E_OC_ACTION_FIND, 0);
                    return;
                }
            }

            if (s16(cLib_distanceAngleS(shape_angle.y, target_angle)) >= 0x400) {
                cLib_addCalcAngleS(&shape_angle.y, target_angle, 4, 0x800, 0x100);
            }

            current.angle.y = shape_angle.y;
            if (field_0x6c0 == 0) {
                if (home.pos.abs(daPy_getPlayerActorClass()->current.pos) < (mMoveRange - 200.0f)) {
                    setActionMode(E_OC_ACTION_FIND, 0);
                    return;
                }

                if (player_distance > l_HIO.standby_distance) {
                    if (home.pos.abs(daPy_getPlayerActorClass()->current.pos) > mMoveRange + 200.0f) {
                        setActionMode(E_OC_ACTION_WAIT, 0);
                        return;
                    }
                }

                if (player_distance < 400.0f && player_distance > 200.0f) {
                    if (abs(shape_angle.y - fopAcM_searchPlayerAngleY(this)) < 0x1000
                        && !dComIfGp_event_runCheck()) {
                        setActionMode(E_OC_ACTION_ATTACK, 0);
                    }

                    return;
                }
            }
    }
}

bool daE_OC_c::checkWaterSurface() {
    dBgS_ObjGndChk_Spl gnd_chk_spl;
    cXyz my_vec_0 = current.pos;
    my_vec_0.y += 500.0f;
    gnd_chk_spl.SetPos((Vec*)&my_vec_0);
    mWaterLvl = dComIfG_Bgsp().GroundCross(&gnd_chk_spl);
    if (mAcch.ChkGroundHit()) {
        mWaterLvl = -G_CM3D_F_INF;
    }

    if (mWaterLvl > (current.pos.y - 140.0f)) {
        return 1;
    } else {
        return 0;
    }
}

void daE_OC_c::action() {
    mSphs_at[0].OffAtSetBit();
    mSphs_at[1].OffAtSetBit();
    damage_check();

    if (mMoveRange) {
        if (field_0x6e3 == 0) {
            if (home.pos.absXZ(current.pos) > mMoveRange) {
                if (home.pos.absXZ(current.pos) > mMoveRange + 300.0f) {
                    field_0x6e3 = 2;
                } else {
                    field_0x6e3 = 1;
                }
            }
        } else if (home.pos.absXZ(current.pos) < mMoveRange - 100.0f) {
            field_0x6e3 = 0;
        }
    }

    searchOtherOc();
    mIsHomeWait = false;
    mBattleOn = false;
    field_0x6df = 0;

    if (mActionMode != E_OC_ACTION_WATER_DEATH && checkWaterSurface()) {
        setActionMode(E_OC_ACTION_WATER_DEATH, 0);
    }

    if (mActionMode == E_OC_ACTION_FIND || mActionMode == E_OC_ACTION_WALK) {
        mAcchCir.SetWallR(60.0f);
    } else {
        mAcchCir.SetWallR(100.0f);
    }

    checkFall();
    field_0x566 = 0;
    switch (mActionMode) {
    case E_OC_ACTION_WAIT:
        executeWait();
        break;
    case E_OC_ACTION_WALK:
        executeWalk();
        break;
    case E_OC_ACTION_TALK:
        executeTalk();
        break;
    case E_OC_ACTION_FIND:
        field_0x566 = 1;
        executeFind();
        break;
    case E_OC_ACTION_ATTACK:
        field_0x566 = 1;
        executeAttack();
        break;
    case E_OC_ACTION_DAMAGE:
        executeDamage();
        break;
    case E_OC_ACTION_BIG_DAMAGE:
        executeBigDamage();
        break;
    case E_OC_ACTION_WATCH:
        executeWatch();
        break;
    case E_OC_ACTION_SOUND_WATCH:
        executeSoundWatch();
        break;
    case E_OC_ACTION_DEATH:
        executeDeath();
        break;
    case E_OC_ACTION_WATER_DEATH:
        executeWaterDeath();
        break;
    case E_OC_ACTION_DEMO_MASTER:
        field_0x566 = 1;
        executeDemoMaster();
        break;
    case E_OC_ACTION_DEMO_CHILD:
        field_0x566 = 1;
        executeDemoChild();
        break;
    case E_OC_ACTION_FALL_DEAD:
        executeFallDead();
        break;
    case E_OC_ACTION_FALL:
        executeFall();
        break;
    case E_OC_ACTION_FIND_STAY:
        field_0x566 = 1;
        executeFindStay();
        break;
    case E_OC_ACTION_MOVE_OUT:
        field_0x566 = 1;
        executeMoveOut();
        break;
    }

    mSound.setLinkSearch(field_0x566);
    setFootNoteSound();
    s16 angle_diff = shape_angle.y - mPrevShapeAngle;
    if (field_0x6df == 0) {
        cLib_addCalcAngleS(&field_0x6d2, angle_diff, 4, 0x1000, 0x400);
    } else {
        cLib_addCalcAngleS(&field_0x6d2, angle_diff, 8, 0x800, 0x400);
    }

    if (field_0x6d2 >= 0x3000) {
        field_0x6d2 = 0x3000;
    }

    if (field_0x6d2 < -0x3000) {
        field_0x6d2 = -0x3000;
    }

    fopAcM_posMoveF(this, mStts.GetCCMoveP());
    mAcch.CrrPos(dComIfG_Bgsp());
    cXyz head_lock_pos(eyePos);
    head_lock_pos.y += 300.0f + NREG_F(7);
    setHeadLockPos(&head_lock_pos);
    setWeaponGroundAngle();
    mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
}

void daE_OC_c::mtx_set() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(field_0x688);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::YrotM(field_0x6d8);
    mDoMtx_stack_c::scaleM(l_HIO.basic_size, l_HIO.basic_size, l_HIO.basic_size);
    J3DModel* model = mpMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->modelCalc();
    if (mpModel) {
        mpModel->setBaseTRMtx(model->getAnmMtx(0xc));
    }
}

void daE_OC_c::cc_set() {
    cXyz my_vec_lhs;
    cXyz my_vec_rhs;
    J3DModel *model = mpMorf->getModel();
    mDoMtx_stack_c::copy(model->getAnmMtx(0x11));
    my_vec_lhs.set(10.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::multVec(&my_vec_lhs, &eyePos);
    attention_info.position = eyePos;
    attention_info.position.y += 30.0f;
    mDoMtx_stack_c::copy(model->getAnmMtx(1));
    my_vec_lhs.set(10.0f, 10.0f, 0.0f);
    mDoMtx_stack_c::multVec(&my_vec_lhs, &my_vec_rhs);
    mSphs_cc[0].SetC(my_vec_rhs);
    mSphs_cc[0].SetR(50.0f);
    dComIfG_Ccsp()->Set(&mSphs_cc[0]);
    mDoMtx_stack_c::copy(model->getAnmMtx(0x11));
    my_vec_lhs.set(10.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::multVec(&my_vec_lhs, &my_vec_rhs);
    mSphs_cc[1].SetC(my_vec_rhs);
    mSphs_cc[1].SetR(45.0f);
    dComIfG_Ccsp()->Set(&mSphs_cc[1]);
    mDoMtx_stack_c::copy(model->getAnmMtx(0xc));
    my_vec_lhs.set(0.0f, 0.0f, 70.0f);
    mDoMtx_stack_c::multVec(&my_vec_lhs, &my_vec_rhs);
    mSphs_at[0].SetC(my_vec_rhs);
    mSphs_at[0].SetR(45.0f);
    mDoMtx_stack_c::copy(model->getAnmMtx(0xc));
    my_vec_lhs.set(0.0f, 0.0f, 20.0f);
    mDoMtx_stack_c::multVec(&my_vec_lhs, &my_vec_rhs);
    mSphs_at[1].SetC(my_vec_rhs);
    mSphs_at[1].SetR(30.0f);
    dComIfG_Ccsp()->Set(&mSphs_at[0]);
    dComIfG_Ccsp()->Set(&mSphs_at[1]);
}

int daE_OC_c::execute() {
    if (field_0x6c0) {
        --field_0x6c0;
    }
    if (field_0x6c2) {
        --field_0x6c2;
    }
    if (field_0x6c4) {
        --field_0x6c4;
    }
    if (field_0x6c6) {
        --field_0x6c6;
    }
    if (field_0x6cc) {
        --field_0x6cc;
    }
    if (field_0x6ca) {
        --field_0x6ca;
    }
    if (field_0x6d6) {
        --field_0x6d6;
    }
    if (field_0x6ce) {
        --field_0x6ce;
    }

    if (field_0x6c8) {
        --field_0x6c8;
        if (field_0x6c8 == 0) {
            camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
            mPlayerPos = daPy_getPlayerActorClass()->current.pos;
            mPlayerPos.y += 500.0f;
            field_0x704 = 55.0f;
            camera->mCamera.Reset(field_0x6f8, mPlayerPos, field_0x704, 0);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            fopAcM_OffStatus(this, 0x4000);
            fopAc_ac_c* search_actor = (fopAc_ac_c*)fpcM_Search(s_demo_oc, this);
            if (search_actor) {
                fopAcM_OffStatus(search_actor, 0x4000);
            }
        }
    }

    action();
    mtx_set();
    cc_set();
    cXyz my_vec_0;
    mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(0x1));
    mDoMtx_stack_c::multVecZero(&my_vec_0);
    cXyz my_vec_1(1.0f + nREG_F(3), 2.0f + nREG_F(4), 1.0f + nREG_F(5));
    setMidnaBindEffect(this, &mSound, &my_vec_0, &my_vec_1);
    return 1;
}

static void daE_OC_Execute(daE_OC_c* i_this) {
    i_this->execute();
}

static bool daE_OC_IsDelete(daE_OC_c* param_0) {
    return true;
}

int daE_OC_c::_delete() {
    dComIfG_resDelete(&mPhaseReqs[0], mName);
    dComIfG_resDelete(&mPhaseReqs[1], "E_ocb");
    if (field_0xe84) {
        lbl_216_bss_58 = 0;
        mDoHIO_DELETE_CHILD(l_HIO.field_0x04);
    }

    if (heap) {
        mSound.deleteObject();
    }

    return 1;
}

static void daE_OC_Delete(daE_OC_c* i_this) {
    fopAcM_RegisterDeleteID(i_this, "E_OC");
    i_this->_delete();
}

int daE_OC_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*) dComIfG_getObjectRes(mName, 0x3);
    JUT_ASSERT(3443, modelData != NULL);
    mpMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL,
                                  (J3DAnmTransform*)dComIfG_getObjectRes("E_ocb", 0x1b),
                                  0, 1.0f, 0, -1,
                                  &mSound, 0x80000, 0x11000084);
    if (mpMorf == NULL || mpMorf->getModel() == 0) {
        return 0;
    }

    J3DModel* my_model = mpMorf->getModel();
    my_model->setUserArea((uintptr_t)this);
    for (u16 idx = 1; idx < my_model->getModelData()->getJointNum(); ++idx) {
        if (idx == 17 || idx == 12) {
            my_model->getModelData()->getJointNodePointer(idx)->setCallBack(&daE_OC_c::JointCallBack);
        }
    }

    modelData = (J3DModelData*) dComIfG_getObjectRes(mName, 0x4);
    JUT_ASSERT(3478, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    daE_OC_c* a_this = static_cast<daE_OC_c*>(i_this);
    return a_this->CreateHeap();
}

cPhs__Step daE_OC_c::create() {
    fopAcM_ct(this, daE_OC_c);
    field_0x6b4 = fopAcM_GetParam(this) & 0xFF;
    if (field_0x6b4 == 0xFF) {
        field_0x6b4 = 0;
    }

    u8 u_var = ((fopAcM_GetParam(this) & 0xFF00) >> 8);
    if (u_var == 0x1) {
        mPlayerRange = l_HIO.plyr_srch_max_radius_M;
    } else {
        mPlayerRange = l_HIO.plyr_srch_max_radius_S;
    }

    mDefeatedSwitch = (fopAcM_GetParam(this) & 0xFF0000) >> 0x10;
    if (mDefeatedSwitch != 0xFF) {
        if (dComIfGs_isSwitch(mDefeatedSwitch, fopAcM_GetRoomNo(this))) {
            // "E_OC I've been defeated so I won't reset it\n"
            OS_REPORT("E_OC やられ後なので再セットしません\n");
            return cPhs_ERROR_e;
        }
    }

    field_0x6e1 = (fopAcM_GetParam(this) & 0xFF000000) >> 0x18;
    u_var = current.angle.x & 0xFF;
    if (u_var == 0xFF) {
        u_var = 0;
    }

    mMoveRange = 100.0f * u_var;
    u_var = current.angle.x >> 8 & 0xFF;
    if (u_var == 0xFF || u_var == 0) {
        mName = "E_OC";
    } else {
        mName = "E_OC2";
    }

    cPhs__Step phase = (cPhs__Step) dComIfG_resLoad(&mPhaseReqs[0], mName);
    if (phase == cPhs_COMPLEATE_e) {
        phase = (cPhs__Step) dComIfG_resLoad(&mPhaseReqs[1], "E_ocb");
        if (phase == cPhs_COMPLEATE_e) {
            OS_REPORT("E_OC PARAM %x\n", fopAcM_GetParam(this));
            if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x2e40)) {
                return cPhs_ERROR_e;
            }

            if (!lbl_216_bss_58) {
                field_0xe84 = 1;
                lbl_216_bss_58 = 1;
                // "oku"
                l_HIO.field_0x04 = mDoHIO_CREATE_CHILD("オーク", &l_HIO);
            }

            attention_info.flags = fopAc_AttnFlag_BATTLE_e;
            fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
            fopAcM_SetMin(this, -200.0f, -200.0f, -200.0f);
            fopAcM_SetMax(this, 200.0f, 200.0f, 200.0f);
            mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
                      fopAcM_GetSpeed_p(this), NULL, NULL);
            if (0 == strcmp("D_MN05", dComIfGp_getStartStageName())
                && dComIfGp_getStartStageRoomNo() == 0xc) {
                mAcchCir.SetWallH(95.0f);
            } else {
                mAcchCir.SetWallH(70.0f);
            }
            mAcchCir.SetWallR(100.0f);
            if (mName == "E_OC") {
                field_0x560 = health = 40;
            } else {
                field_0x560 = health = 220;
            }

            mStts.Init(100.0f, 0, this);
            mSphs_cc[0].Set(E_OC_n::cc_sph_src);
            mSphs_cc[0].SetStts(&mStts);
            mSphs_cc[1].Set(E_OC_n::cc_sph_src);
            mSphs_cc[1].SetStts(&mStts);
            mSphs_at[0].Set(E_OC_n::at_sph_src);
            mSphs_at[0].SetStts(&mStts);
            mSphs_at[1].Set(E_OC_n::at_sph_src);
            mSphs_at[1].SetStts(&mStts);
            if (mName == "E_OC") {
                mSphs_at[0].SetAtAtp(1);
                mSphs_at[1].SetAtAtp(1);
                fopAcM_OnStatus(this, 0x10000);
            } else {
                mSphs_at[0].SetAtAtp(2);
                mSphs_at[1].SetAtAtp(2);
            }

            mSound.init(&current.pos, &eyePos, 3, 1);
            mSound.setEnemyName("E_oc");
            mAtInfo.mpSound = &mSound;
            mAtInfo.mPowerType = 1;
            current.angle.x = shape_angle.x = 0;
            gravity = -5.0f;
            switch (field_0x6b4) {
                case 2:
                    break;
                case 3:
                    // When you break the monkey cage in Forest Temple - Underground Monkey Room,
                    //  two bokoblins spawn. One with lowest bit == 3 and the other == 4.
                    mHide = true;
                    setActionMode(E_OC_ACTION_DEMO_MASTER, 0);
                    break;
                case 4:
                    // Ditto to above.
                    mHide = true;
                    setActionMode(E_OC_ACTION_DEMO_CHILD, 0);
                    break;
                default:
                    fopAcM_OffStatus(this, 0x4000);
                    break;
            }

            daE_OC_Execute(this);
        }
    }

    return phase;
}

static void daE_OC_Create(daE_OC_c* i_this) {
    i_this->create();
}

static actor_method_class l_daE_OC_Method = {
    (process_method_func)daE_OC_Create,
    (process_method_func)daE_OC_Delete,
    (process_method_func)daE_OC_Execute,
    (process_method_func)daE_OC_IsDelete,
    (process_method_func)daE_OC_Draw,
};

actor_process_profile_definition g_profile_E_OC = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_OC,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daE_OC_c),       // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  201,                    // mPriority
  &l_daE_OC_Method,       // sub_method
  0x00044100,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};

AUDIO_INSTANCES;
