/**
 * @file d_a_e_sb.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_sb.h"
#include "d/actor/d_a_e_bg.h"
#include "d/actor/d_a_player.h"
#include "d/d_bomb.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "Z2AudioLib/Z2Instances.h"

daE_SB_HIO_c::daE_SB_HIO_c() {
    field_0x04 = -1;
    search_area = 1100.0f;
    distance_home = 600.0f;
    jump_xz_axis_speed = 40.0f;
    jump_y_axis_speed = 35.0f;
    atk_jump_xz_axis_speed = 75.0f;
    atk_y_axis_speed = 40.0f;
    magne_on_atk_y_axis_speed = 30.0f;
    grav_reduct_rate = 0.83f;
    other_anm_speed = 1.0f;
    shield_atk_anm_speed = 1.0f;
    death_anm_speed = 0.7f;
    field_0x3c = 0x1000;
    field_0x3e = 0x1000;
    size = 1.4f;
    atk_start_range = 1000.0f;
}

#if DEBUG
void daE_SB_HIO_c::genMessage(JORMContext* ctext) {
    // "Shell Blade"
    ctext->genLabel("シェルブレイド", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // "Search Area"
    ctext->genSlider("サーチエリア", &search_area, 0.0f, 3000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // "Distance home"
    ctext->genSlider("お家に帰る距離", &distance_home, 0.0f, 1000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // "Jump XZ axis speed"
    ctext->genSlider("ジャンプXZ軸スピード", &jump_xz_axis_speed, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // "Jump Y-axis speed"
    ctext->genSlider("ジャンプY軸スピード", &jump_y_axis_speed, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // "Attack jump XZ axis speed"
    ctext->genSlider("攻撃ジャンプXZ軸スピード", &atk_jump_xz_axis_speed, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // "Attack Y-axis speed"
    ctext->genSlider("攻撃Y軸スピード", &atk_y_axis_speed, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // "Magne On Attack Y-axis Speed"
    ctext->genSlider("マグネOn攻撃Y軸スピード", &magne_on_atk_y_axis_speed, 0.0f, 100.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // "Gravity reduction rate"
    ctext->genSlider("重力の減る比率", &grav_reduct_rate, 0.0f, 1.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // "Other animation speed"
    ctext->genSlider("その他アニメーションスピード", &other_anm_speed, 0.0f, 10.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // "Shield attack animation speed"
    ctext->genSlider("盾攻撃アニメーションスピード", &shield_atk_anm_speed, 0.0f, 10.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // "DEATH animation speed"
    ctext->genSlider("DEATHアニメーションスピード", &death_anm_speed, 0.0f, 10.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // "Size"
    ctext->genSlider("大きさ", &size, 0.0f, 10.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    // "Attack start range"
    ctext->genSlider("攻撃開始範囲", &atk_start_range, 0.0f, 3000.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
}
#endif

int daE_SB_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*) dComIfG_getObjectRes("E_SB", 0xF);
    JUT_ASSERT(271, modelData != NULL);
    mpMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL,
                                  (J3DAnmTransform*)dComIfG_getObjectRes("E_SB", 0xC),
                                  2, 1.0f, 0, -1,
                                  &mSound, 0x80000, 0x11000084);
    if (mpMorf == NULL || mpMorf->getModel() == NULL) {
        return 0;
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    return static_cast<daE_SB_c*>(i_this)->CreateHeap();
}

void daE_SB_c::initCcCylinder() {
    const static dCcD_SrcCyl ccCylSrc = {
        {
            {0x0, {{AT_TYPE_CSTATUE_SWING, 0x2, 0xd}, {0x6032, 0x11}, 0x75}}, // mObj
            {dCcD_SE_STONE, 0x0, 0x0, 0x0, 0x1}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x1}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {
                {0.0f, 0.0f, 0.0f}, // mCenter
                80.0f, // mRadius
                130.0f // mHeight
            } // mCyl
        }
    };

    mStts.Init(0xFE, 0xFF, this);
    mCyl.Set(ccCylSrc);
    mCyl.SetStts(&mStts);
    mCyl.SetC(current.pos);
}

void daE_SB_c::setCcCylinder() {
    mCyl.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mCyl);
}

int daE_SB_c::ctrlJoint(J3DJoint* i_joint, J3DModel* param_1) {
    int jnt_no = i_joint->getJntNo();
    MtxP anmMtx = param_1->getAnmMtx(jnt_no);
    mDoMtx_stack_c::copy(anmMtx);
    switch (jnt_no) {
        case 3:
        case 1: {
            break;
        }

        case 2: {
            mDoMtx_stack_c::ZrotM(field_0x5d8.x);
            mDoMtx_stack_c::YrotM(field_0x5d8.y);
            break;
        }

        case 4: {
            mDoMtx_stack_c::ZrotM(-field_0x5de.x);
            mDoMtx_stack_c::YrotM(field_0x5de.y);
            break;
        }
    }

    param_1->setAnmMtx(jnt_no, mDoMtx_stack_c::get());
    mDoMtx_copy(mDoMtx_stack_c::get(), J3DSys::mCurrentMtx);
    return 1;
}

static int JointCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        daE_SB_c* sb_ptr = (daE_SB_c*) j3dSys.getModel()->getUserArea();
        if (sb_ptr) {
            sb_ptr->ctrlJoint(i_joint, j3dSys.getModel());
        }
    }

    return 1;
}

void daE_SB_c::Particle_Set(u16 param_0) {
    dComIfGp_particle_set(param_0, &current.pos, &tevStr, &shape_angle, NULL);
}

void daE_SB_c::SetAnm(int param_0, int param_1, f32 param_2, f32 param_3) {
    mpMorf->setAnm((J3DAnmTransform*) dComIfG_getObjectRes("E_SB", param_0), param_1, param_2, param_3, 0.0f, -1.0f);
    field_0x608 = param_0;
}

void daE_SB_c::MemberClear() {
    gravity = -9.0f;
    speedF = 0.0f;
    speed.y = 0.0f;
    field_0x5b0 = 1;
    field_0x5ac = 0;
    field_0x5b4 = 0;
    field_0x5b8 = 0;
    field_0x5bc = 0;
    field_0x5c0 = 0;
    field_0x5c4 = 0;
    field_0x5c8 = 0;
    mCyl.OnTgShield();
    mCyl.SetTgHitMark(CcG_Tg_UNK_MARK_2);
}

static BOOL other_bg_check(daE_SB_c* a_this, fopAc_ac_c* i_other) {
    fopAc_ac_c* i_this = (fopAc_ac_c*)a_this;
    dBgS_LinChk line_chk;
    cXyz sp28;
    cXyz start;
    cXyz end;

    end = i_other->current.pos;
    end.y += 100.0f;

    start = i_this->current.pos;
    start.y += 100.0f;

    line_chk.Set(&start, &end, i_this);

    if (dComIfG_Bgsp().LineCross(&line_chk)) {
        return TRUE;
    }

    return FALSE;
}

static void* s_obj_sub(void* arg0, void* arg1) {
    if (fopAcM_IsActor(arg0) && dBomb_c::checkBombActor((fopAc_ac_c*) arg0)) {
        if (!fopAcM_checkCarryNow((fopAc_ac_c*) arg0) && !((dBomb_c*)arg0)->checkStateExplode()) {
            if (arg1 != NULL) {
                ((daE_SB_c*) arg1)->field_0x5f0 = fopAcM_searchActorDistanceXZ((fopAc_ac_c*) arg0, (fopAc_ac_c*) arg1);
                ((daE_SB_c*) arg1)->field_0x5ec = &((fopAc_ac_c*) arg0)->current.pos;
            }

            return arg0;
        }
    } else if (fopAcM_GetName(arg0) == PROC_E_BG) {
        // bomb fish:
        if (arg0 != NULL && ((daE_BG_c*) arg0)->isBomb()) {
            if (arg1 != NULL) {
                ((daE_SB_c*) arg1)->field_0x5f0 = fopAcM_searchActorDistanceXZ((fopAc_ac_c*) arg0, (fopAc_ac_c*) arg1);
                ((daE_SB_c*) arg1)->field_0x5ec = &((fopAc_ac_c*) arg0)->current.pos;
            }

            return arg0;
        }
    } else if (fopAcM_GetName(arg0) == PROC_NBOMB) {
        // generic bomb:
        if (arg1 != NULL) {
            ((daE_SB_c*) arg1)->field_0x5f0 = fopAcM_searchActorDistanceXZ((fopAc_ac_c*) arg0, (fopAc_ac_c*) arg1);
            ((daE_SB_c*) arg1)->field_0x5ec = &((fopAc_ac_c*) arg0)->current.pos;
        }

        return arg0;
    }

    return NULL;
}

static void* s_bomb_sub(void* arg0, void* arg1) {
    if (fopAcM_GetName(arg0) == PROC_E_BG) {
        // bomb fish:
        if (arg0 != NULL && ((daE_BG_c*) arg0)->isBomb()) {
            if (arg1 != NULL) {
                ((daE_SB_c*) arg1)->field_0x5f0 = fopAcM_searchActorDistanceXZ((fopAc_ac_c*) arg0, (fopAc_ac_c*) arg1);
                ((daE_SB_c*) arg1)->field_0x5ec = &((fopAc_ac_c*) arg0)->current.pos;
            }

            return arg0;
        }
    } else if (fopAcM_GetName(arg0) == PROC_NBOMB) {
        // generic bomb:
        if (arg1 != NULL) {
            ((daE_SB_c*) arg1)->field_0x5f0 = fopAcM_searchActorDistanceXZ((fopAc_ac_c*) arg0, (fopAc_ac_c*) arg1);
            ((daE_SB_c*) arg1)->field_0x5ec = &((fopAc_ac_c*) arg0)->current.pos;
        }

        return arg0;
    }

    return NULL;
}

static u8 hio_set;

static daE_SB_HIO_c l_HIO;

namespace {
    static fopAc_ac_c* s_Bomb;
    static s16 s_TargetAngle;
    static cXyz* s_LinkPos;
}

void daE_SB_c::Shield_Motion() {
    mpMorf->setAnm((J3DAnmTransform*) dComIfG_getObjectRes("E_SB", 0xB), 0, 5.0f, l_HIO.shield_atk_anm_speed, 0.0f, -1.0f);
    field_0x602 = 1;
    current.angle.y = cLib_targetAngleY(&current.pos, s_LinkPos);
    speedF = -5.0f;
    field_0x5b0 = 5;
    field_0x5b8 = 0;
    mCyl.OffAtSetBit();
    dComIfGp_particle_set(0x849E, &current.pos, &tevStr, &shape_angle, NULL);
    dComIfGp_particle_set(0x8478, &current.pos, &tevStr, &shape_angle, NULL);
}

void daE_SB_c::AttackStop() {
    s_Bomb = (fopAc_ac_c*) fpcM_Search(s_obj_sub, this);
    if (s_Bomb == NULL) {
        MemberClear();
    }
}

void daE_SB_c::AttackSetSP() {
    daPy_py_c* player = daPy_getPlayerActorClass();
    dBgS_LinChk line_chk;
    cXyz pi_start(0.0f, 200.0f, 100.0f);
    cXyz pi_end(0.0f, -200.0f, 100.0f);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::multVec(&pi_start, &pi_start);
    mDoMtx_stack_c::multVec(&pi_end, &pi_end);
    line_chk.Set(&pi_start, &pi_end, NULL);
    if (dComIfG_Bgsp().LineCross(&line_chk)) {
        cXyz cross = line_chk.GetCross();
        if (cross.y - current.pos.y > 0.0f) {
            if (field_0x5c0 != 2) {
                if (!player->checkEquipHeavyBoots()) {
                    speed.y = (0.8f * l_HIO.atk_y_axis_speed);
                } else {
                    speed.y = (0.8f * l_HIO.magne_on_atk_y_axis_speed);
                }
            } else {
                speed.y = (0.5f * l_HIO.atk_y_axis_speed);
            }
        } else if (field_0x5c0 != 2) {
            if (!player->checkEquipHeavyBoots()) {
                speed.y = (0.7f * l_HIO.atk_y_axis_speed);
            } else {
                speed.y = (0.7f * l_HIO.magne_on_atk_y_axis_speed);
            }
        } else {
            speed.y = (0.5f * l_HIO.atk_y_axis_speed);
        }
    }

    speedF = l_HIO.atk_jump_xz_axis_speed;
}

void daE_SB_c::AttackAngle() {
    s16 var_r29 = 0x300;
    s16 var_r30 = (field_0x5c0 == 2) ? cLib_targetAngleY(&current.pos, field_0x5ec)
                                     : cLib_targetAngleY(&current.pos, s_LinkPos);
    field_0x610 += (0x13a0);
    shape_angle.x = (1792.0f * cM_ssin(field_0x610));
    if (shape_angle.x < 0) {
        var_r29 = 0;
    }

    cLib_chaseAngleS(&field_0x612, var_r29, 0x100);
    cLib_chaseAngleS(&shape_angle.y, var_r30, field_0x612);
    cLib_chaseAngleS(&current.angle.y, var_r30, field_0x612);
}

void daE_SB_c::AttackSpeedMotion() {
    cLib_addCalc2(&speedF, 5.0f, 0.2f, 30.0f);
    gravity = -1.0f;
}

void daE_SB_c::Attack_Motion() {
    switch (field_0x5ac) {
        case 0: {
            SetAnm(4, 0, 5.0f, 1.0f);
            field_0x5ac = 1;
            mSound.startCreatureSound(Z2SE_EN_SB_ATTACK01, 0, -1);
            Particle_Set(0x8478);
            break;
        }

        case 1: {
            if (mpMorf->getFrame() < 53.0f && field_0x5c0 == 2) {
                AttackStop();
            }

            if (mpMorf->getFrame() > 53.0f) {
                mCyl.OnTgShield();
                mCyl.SetTgHitMark(CcG_Tg_UNK_MARK_2);
                mCyl.OnAtSetBit();
                gravity = -9.0f;
                field_0x5ac = 2;
            }

            AttackAngle();
            break;
        }

        case 2: {
            mSound.startCreatureSound(Z2SE_EN_SB_JUMP, 0, -1);
            AttackSetSP();
            field_0x5ac = 3;
            mCyl.SetTgHitMark(CcG_Tg_UNK_MARK_2);
            mCyl.OnTgShield();
            break;
        }

        case 3: {
            if (64.0f == mpMorf->getFrame()) {
                mSound.startCreatureSound(Z2SE_EN_SB_ATTACK02, 0, -1);
            }

            cLib_chaseAngleS(&shape_angle.x, 0, 0x100);
            AttackSpeedMotion();
            fopAc_ac_c* actor_p = (fopAc_ac_c*) fpcM_Search(s_bomb_sub, this);
            if (field_0x5f0 < 200.0f && actor_p != NULL) {
                fopAcM_delete(actor_p);
                if (field_0x61a != 0xFF && !dComIfGs_isSwitch(field_0x61a, fopAcM_GetRoomNo(this))) {
                    dComIfGs_onSwitch(field_0x61a, fopAcM_GetRoomNo(this));
                }

                field_0x603 = 1;
            }

            if (mAcch.ChkGroundHit() && mpMorf->isStop()) {
                mSound.startCreatureSound(Z2SE_EN_SB_LAND, 0, -1);
                Particle_Set(0x8479);
                speedF = 0.0f;
                field_0x5ac = 4;
                field_0x5c8 = 10;
                field_0x5d0 = 1;
                mCyl.OffAtSetBit();
                if (field_0x603 == 1) {
                    MemberClear();
                    field_0x5b0 = 3;
                    field_0x5c8 = 40;
                    SetAnm(12, 2, 5.0f, 2.0f);
                    mSound.startCreatureSound(Z2SE_EN_SB_WAIT, 0, -1);
                    field_0x5ac = 0;
                }
            }

            break;
        }

        case 4: {
            if (field_0x5c8 == 0) {
                MemberClear();
            }

            break;
        }
    }

    if (field_0x5ac != 3) {
        ActionCheck();
        Obj_Damage();
    }
}

void daE_SB_c::Bomb_Death_Motion() {
    if (field_0x5c8 == 0) {
        dBomb_c::createNormalBombExplode(&current.pos);
        mCyl.OffAtSetBit();
        mCyl.OnTgNoHitMark();
        SetAnm(6, 0, 5.0f, l_HIO.other_anm_speed);
        Particle_Set(0x8475);
        Particle_Set(0x8476);
        field_0x5b0 = 2;
    }
}

void daE_SB_c::BombJumpSet() {
    speed.y = 0.5f * l_HIO.jump_y_axis_speed;
    speedF = l_HIO.jump_xz_axis_speed;
}

void daE_SB_c::GetHomeSet() {
    if (field_0x5f4.absXZ(current.pos) < 300.0f) {
        speedF = 0.0f;
        speed.y = 0.5f * l_HIO.jump_y_axis_speed;
        field_0x604 = 1;
        if (shape_angle.y == field_0x600) {
            field_0x604 = 0;
            MemberClear();
        }
    } else {
        s16 var_r30 = cLib_targetAngleY(&current.pos, &field_0x5f4);
        if (mAcch.ChkWallHit()) {
            var_r30 += 0x1000;
        }

        cLib_chaseAngleS(&shape_angle.y, var_r30, 0x1000);
        cLib_chaseAngleS(&current.angle.y, var_r30, 0x1000);
        speed.y = 0.5f * l_HIO.jump_y_axis_speed;
        speedF = l_HIO.jump_xz_axis_speed;
    }
}

void daE_SB_c::HomeJumpMotion() {
    if (field_0x604 == 1) {
        speedF = 0.0f;
        cLib_chaseAngleS(&shape_angle.y, field_0x600, 0x200);
        cLib_chaseAngleS(&current.angle.y, field_0x600, 0x200);
        return;
    }

    s16 temp_r3 = cLib_targetAngleY((Vec *) &current.pos, (Vec *) &field_0x5f4);
    cLib_chaseAngleS(&shape_angle.y, temp_r3, 0x200);
    cLib_chaseAngleS(&current.angle.y, temp_r3, 0x200);
}

void daE_SB_c::Jump_Normal_Set() {
    f32 my_speed = l_HIO.jump_xz_axis_speed;
    cLib_chaseAngleS(&shape_angle.y, s_TargetAngle, 0x1000);
    cLib_chaseAngleS(&current.angle.y, s_TargetAngle, 0x1000);
    dBgS_LinChk line_chk;
    cXyz pi_start(0.0f, 200.0f, 100.0f);
    cXyz pi_end(0.0f, -200.0f, 100.0f);
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mDoMtx_stack_c::multVec(&pi_start, &pi_start);
    mDoMtx_stack_c::multVec(&pi_end, &pi_end);
    line_chk.Set(&pi_start, &pi_end, NULL);
    if (dComIfG_Bgsp().LineCross(&line_chk)) {
        cXyz cross = line_chk.GetCross();
        if (cross.y - current.pos.y > 0.0f) {
            speed.y = 0.6f * l_HIO.jump_y_axis_speed;
        } else {
            speed.y = 0.5f * l_HIO.jump_y_axis_speed;
        }
    }

    speedF = my_speed;
}

void daE_SB_c::Jump_Set() {
    switch (field_0x5c0) {
        case 0:
            Jump_Normal_Set();
            return;
        case 1:
            GetHomeSet();
            return;
        case 2:
            BombJumpSet();
            return;
    }
}

void daE_SB_c::JumpMiddleMotion() {
    cLib_addCalc2(&speedF, 5.0f, 0.2f, 30.0f);
    gravity = -1.0f;
    switch (field_0x5c0) {
        case 0:
            cLib_chaseAngleS(&shape_angle.y, s_TargetAngle, 0x400);
            cLib_chaseAngleS(&current.angle.y, s_TargetAngle, 0x400);
            return;
        case 1:
            HomeJumpMotion();
            return;
        case 2:
            cLib_chaseAngleS(&shape_angle.y, cLib_targetAngleY((Vec *) &current.pos, field_0x5ec), 0x400);
            cLib_chaseAngleS(&current.angle.y, cLib_targetAngleY((Vec *) &current.pos, field_0x5ec), 0x400);
            return;
    }
}

void daE_SB_c::Jump_Motion() {
    switch (field_0x5bc) {
        case 0: {
            mSound.startCreatureSound(Z2SE_EN_SB_JUMP, 0, -1);
            ActionCheck();
            SetAnm(8, 0, 5.0f, l_HIO.other_anm_speed);
            field_0x5bc = 1;
            Jump_Set();
            Obj_Damage();
            return;
        }

        case 1: {
            SetAnm(9, 0, 5.0f, l_HIO.other_anm_speed);
            JumpMiddleMotion();
            if (mAcch.ChkGroundHit()) {
                if (speed.y <= 0.0f) {
                    speed.y = 0.0f;
                    speedF = 0.0f;
                    gravity = -9.0f;
                    SetAnm(0xA, 0, 5.0f, l_HIO.other_anm_speed);
                    mSound.startCreatureSound(Z2SE_EN_SB_LAND, 0, -1);
                    Particle_Set(0x8479);
                    field_0x5bc = 2;
                    return;
                }
            }

            break;
        }

        case 2: {
            ActionCheck();
            if (mpMorf->isStop()) {
                MemberClear();
            }

            Obj_Damage();
            break;
        }
    }
}

void daE_SB_c::Death_Motion() {
    fopAcM_createDisappear(this, &current.pos, 10, 0, 15);
    fopAcM_delete(this);
}

void daE_SB_c::Hanekaeri() {
    // Japanese for "Bounce"
    mCyl.OnTgShield();
    mCyl.SetTgHitMark(CcG_Tg_UNK_MARK_2);
    field_0x5d0 = 1;
}

void daE_SB_c::Hit() {
    mCyl.OffTgShield();
    mCyl.SetTgHitMark(CcG_Tg_UNK_MARK_0);
    field_0x5d0 = 0;
}

void daE_SB_c::ActionCheck() {
    if (field_0x5b0 == 0 || field_0x602 == 1) {
        u8 temp_r27 = daPy_getPlayerActorClass()->getCutCount();
        u8 temp_r29 = daPy_getPlayerActorClass()->getCutType();
        daPy_py_c* player = daPy_getPlayerActorClass();
        cXyz& unused_pos = fopAcM_GetPosition(player);
        s16 tgt_ang = cLib_targetAngleY(&current.pos, s_LinkPos);
        if ( (s16) (shape_angle.y - tgt_ang) > -0x3000 && (s16) (shape_angle.y - tgt_ang) < 0x3000) {
            if (temp_r27 > 1U) {
                Hit();
            } else if (temp_r29 == 1 || temp_r29 == 10 || temp_r29 == 0x29 || dComIfGp_checkPlayerStatus0(0, 0x4000U)) {
                Hanekaeri();
            } else {
                Hit();
            }
        } else {
            Hanekaeri();
        }

        return;
    }

    Hanekaeri();
}

void daE_SB_c::Search() {
    f32 temp_f31 = current.pos.absXZ(*s_LinkPos);
    f32 temp_f30 = field_0x5f4.absXZ(current.pos);
    daPy_py_c* player = daPy_getPlayerActorClass();
    mSound.setLinkSearch(0);
    attention_info.flags = 0;
    if (temp_f31 < l_HIO.search_area) {
        if (!other_bg_check(this, player)) {
            attention_info.flags = fopAc_AttnFlag_BATTLE_e;
            mSound.setLinkSearch(1);
            field_0x5b0 = 4;
            if (temp_f31 < l_HIO.atk_start_range && (s16) (shape_angle.y - s_TargetAngle) > -0x3000
                && (s16) (shape_angle.y - s_TargetAngle) < 0x3000) {
                field_0x5b0 = 0;
            }
        } else {
            field_0x5b0 = 4;
            field_0x5c0 = 1;
        }
    } else if (temp_f31 < (400.0f + l_HIO.search_area)) {
        if (!other_bg_check(this, player)) {
            attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        }
    } else if (temp_f30 > l_HIO.distance_home) {
        field_0x5b0 = 4;
        field_0x5c0 = 1;
    } else if (field_0x608 != 12) {
        field_0x5b0 = 1;
        field_0x5b4 = 0;
    }

    Search_Bomb();
    if (current.pos.y - home.pos.y > 200.0f) {
        field_0x5b0 = 4;
        field_0x5c0 = 1;
    }
}

void daE_SB_c::Wait_Motion() {
    switch (field_0x5b4) {
        case 0:
            SetAnm(0xC, 2, 5.0f, l_HIO.other_anm_speed);
            mSound.startCreatureSound(Z2SE_EN_SB_WAIT, 0, -1);
            field_0x5b4 = 1;
            break;
        case 1:
            if (mpMorf->getFrame() == 0.0f) {
                mSound.startCreatureSound(Z2SE_EN_SB_WAIT, 0, -1);
            }

            Search();
            break;
    }

    ActionCheck();
    Obj_Damage();
}

void daE_SB_c::Attack() {
    field_0x5cc = dComIfGp_particle_set(field_0x5cc, 0x8473, &current.pos, &tevStr, &shape_angle,
                                        NULL, 0xFF, NULL, -1, NULL, NULL, NULL);
    mCyl.SetTgHitMark(CcG_Tg_UNK_MARK_2);
    mCyl.OnTgShield();
    Attack_Motion();
}

void daE_SB_c::Shield() {
    switch (field_0x5b8) {
        case 0:
            cLib_addCalc2(&speedF, 0.0f, 0.2f, 30.0f);
            if (speedF > -1.0f) {
                speedF = 0.0f;
                field_0x5b8 = 1;
            }

            break;
        case 1:
            ActionCheck();
            if (mpMorf->isStop()) {
                field_0x602 = 0;
                MemberClear();
            }
            break;
    }

    Obj_Damage();
}

void daE_SB_c::Anm_Motion() {
    switch (field_0x5b0) {
        case 1:
            Wait_Motion();
            break;
        case 0:
            Attack();
            break;
        case 4:
            Jump_Motion();
            break;
        case 5:
            Shield();
            break;

        case 2: {
            if (mpMorf->checkFrame(49.5f)) {
                mSound.startCreatureSound(Z2SE_EN_SB_LAND, 0, -1);
            }

            if (mpMorf->isStop()) {
                Death_Motion();
            }

            break;
        }

        case 3:
            Bomb_Death_Motion();
            break;

        case 6: {
            ActionCheck();
            if (2.0f == mpMorf->getFrame()) {
                mSound.startCreatureSound(Z2SE_EN_SB_DEFENCE, 0, -1);
            }

            if (mpMorf->isStop()) {
                MemberClear();
                field_0x5b0 = 1;
                SetAnm(0xC, 2, 5.0f, l_HIO.other_anm_speed);
                mSound.startCreatureSound(Z2SE_EN_SB_WAIT, 0, -1);
            }

            Obj_Damage();
            break;
        }
    }
}

void daE_SB_c::Search_Bomb() {
    s_Bomb = (fopAc_ac_c*) fpcM_Search(s_obj_sub, this);
    if (s_Bomb != NULL) {
        if (!other_bg_check(this, s_Bomb) && !(field_0x5f0 < 100.0f)) {
            if (field_0x5f0 < 400.0f) {
                field_0x5b0 = 0;
                field_0x5c0 = 2;
                return;
            }

            if (field_0x5f0 < 1000.0f) {
                field_0x5b0 = 4;
                field_0x5c0 = 2;
            }
        }
    } else {
        field_0x5c0 = 0;
    }
}

void daE_SB_c::ChanceMotion() {
    if (field_0x5e6 <= 0) {
        mSound.startCreatureSound(Z2SE_EN_SB_DEATH, 0, -1);
        mCyl.OffAtSetBit();
        mCyl.OnTgNoHitMark();
        mSound.startCreatureSound(Z2SE_EN_SB_BUBBLE, 0, -1);
        SetAnm(6, 0, 5.0f, l_HIO.death_anm_speed);
        Particle_Set(0x84A3);
        Particle_Set(0x8475);
        Particle_Set(0x8476);
        attention_info.flags = 0;
        if (field_0x61a != 0xFF && !dComIfGs_isSwitch(field_0x61a, fopAcM_GetRoomNo(this))) {
            dComIfGs_onSwitch(field_0x61a, fopAcM_GetRoomNo(this));
        }

        field_0x5b0 = 2;
        return;
    }

    field_0x5d2 = 0x2000;
    field_0x5e4 = 1;
    field_0x5d8.set(0, 0, 0);
    field_0x5de.set(0, 0, 0);
    field_0x5d4 = 0.0f;
    Shield_Motion();
}

void daE_SB_c::NoChanceMotion() {
    if (mAcch.ChkGroundHit()) {
        if (speed.y <= 0.0f) {
            mCyl.OnTgShield();
            mCyl.OffAtSetBit();
            mCyl.GetTgHitObj();
            MemberClear();
            SetAnm(7, 0, 5.0f, l_HIO.other_anm_speed);
            Particle_Set(0x8478);
            field_0x5b0 = 6;
        }
    }
}

void daE_SB_c::Obj_Damage() {
    if (field_0x60c > 0) {
        mCyl.OffTgSetBit();
        mCyl.ClrTgHit();
        return;
    }

    mCyl.OnTgSetBit();
    mStts.Move();
    if (mCyl.ChkTgHit()) {
        mAtInfo.mpCollider = mCyl.GetTgHitObj();
        if (field_0x5d0) {
            mAtInfo.field_0x18 = 42;
        } else {
            mAtInfo.field_0x18 = 30;
        }

        cCcD_Obj* tg_hit_obj = mCyl.GetTgHitObj();
        cc_at_check(this, &mAtInfo);
        field_0x60c = 0xF;
        if (tg_hit_obj->ChkAtType(AT_TYPE_SHIELD_ATTACK)) {
            field_0x60c = 10;
        }

        if (tg_hit_obj->ChkAtType(AT_TYPE_HOOKSHOT)) {
            field_0x60c = 5;
        }

        u8 cut_cnt = daPy_getPlayerActorClass()->getCutCount();
        if (tg_hit_obj->ChkAtType(AT_TYPE_SHIELD_ATTACK)
          || tg_hit_obj->ChkAtType(AT_TYPE_HOOKSHOT)
          || tg_hit_obj->ChkAtType(AT_TYPE_BOMB)) {
            s16 tgt_ang = cLib_targetAngleY(&current.pos, s_LinkPos);
            if ((s16) (shape_angle.y - tgt_ang) < 0x6000 && (s16) (shape_angle.y - tgt_ang) > -0x6000) {
                if (mAcch.ChkGroundHit()) {
                    if (speed.y <= 0.0f) {
                        if (tg_hit_obj->ChkAtType(AT_TYPE_HOOKSHOT)) {
                            SetAnm(5, 0, 5.0f, l_HIO.shield_atk_anm_speed);
                            mSound.startCreatureSound(Z2SE_EN_SB_BIND, 0, -1);
                        } else {
                            SetAnm(0xB, 0, 5.0f, l_HIO.shield_atk_anm_speed);
                            mSound.startCreatureSound(Z2SE_EN_SB_SHIELD, 0, -1);
                        }

                        field_0x602 = 1;
                        mCyl.OnTgShield();
                        mCyl.SetTgHitMark(CcG_Tg_UNK_MARK_2);
                        current.angle.y = s_TargetAngle;
                        speedF = -5.0f;
                        MemberClear();
                        field_0x5b0 = 5;
                        mCyl.OffAtSetBit();
                        Particle_Set(0x849E);
                        Particle_Set(0x8478);
                    }
                }
            } else {
                NoChanceMotion();
            }

            return;
        }

        if (field_0x5b0 == 0 || field_0x602 == 1) {
            if (tg_hit_obj->ChkAtType(AT_TYPE_NORMAL_SWORD)) {
                u32 cut_type = daPy_getPlayerActorClass()->getCutType();
                if (field_0x5e8 == 1 && cut_cnt > 1
                  && (s16) (shape_angle.y - s_TargetAngle) > -0x3000 && (s16) (shape_angle.y - s_TargetAngle) < 0x3000) {
                    ChanceMotion();
                    --field_0x5e6;
                    if (daPy_getPlayerActorClass()->checkMasterSwordEquip()) {
                        field_0x5e6 -= 1;
                    }

                    if (cut_type == daPy_py_c::CUT_TYPE_TURN_RIGHT
                      || cut_type == daPy_py_c::CUT_TYPE_TURN_LEFT
                      || cut_type == daPy_py_c::CUT_TYPE_LARGE_JUMP_INIT
                      || cut_type == daPy_py_c::CUT_TYPE_LARGE_JUMP
                      || cut_type == daPy_py_c::CUT_TYPE_LARGE_JUMP_FINISH
                      || cut_type == daPy_py_c::CUT_TYPE_MORTAL_DRAW_B
                      || cut_type == daPy_py_c::CUT_TYPE_MORTAL_DRAW_A
                      || cut_type == daPy_py_c::CUT_TYPE_TWIRL
                      || cut_type == daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT
                      || cut_type == daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT) {
                        field_0x5e6 -= 5;
                        ChanceMotion();
                        speedF = 0.0f;
                    }
                } else if (cut_type != daPy_py_c::CUT_TYPE_NM_VERTICAL && cut_type != daPy_py_c::CUT_TYPE_JUMP
                  && (s16) (shape_angle.y - s_TargetAngle) > -0x3000 && (s16) (shape_angle.y - s_TargetAngle) < 0x3000) {
                    if (cut_type == daPy_py_c::CUT_TYPE_TURN_RIGHT
                      || cut_type == daPy_py_c::CUT_TYPE_TURN_LEFT
                      || cut_type == daPy_py_c::CUT_TYPE_LARGE_JUMP_INIT
                      || cut_type == daPy_py_c::CUT_TYPE_LARGE_JUMP
                      || cut_type == daPy_py_c::CUT_TYPE_LARGE_JUMP_FINISH
                      || cut_type == daPy_py_c::CUT_TYPE_MORTAL_DRAW_B
                      || cut_type == daPy_py_c::CUT_TYPE_MORTAL_DRAW_A
                      || cut_type == daPy_py_c::CUT_TYPE_TWIRL
                      || cut_type == daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT
                      || cut_type == daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT) {
                        field_0x5e6 -= 5;
                        ChanceMotion();
                        speedF = 0.0f;
                    }

                    if (field_0x5ac == 3) {
                        field_0x603 = 1;
                    } else {
                        ChanceMotion();
                        --field_0x5e6;
                        if (daPy_getPlayerActorClass()->checkMasterSwordEquip()) {
                            field_0x5e6 -= 1;
                        }

                        field_0x5e8 = 1;
                    }
                } else {
                    NoChanceMotion();
                    field_0x5e8 = 0;
                }
            }
        } else {
            NoChanceMotion();
        }
    }
}

void daE_SB_c::HashiraVib() {
    // "Hashira" means pillar.
    field_0x5d4 += 6000.0f;
    field_0x5d2 *= 0.9f;
    field_0x5d8.x = field_0x5d2 * cM_ssin(field_0x5d4);
    field_0x5d8.y = field_0x5d2 * cM_ssin(8192.0f + field_0x5d4);
    field_0x5de.x = field_0x5d2 * cM_ssin(field_0x5d4);
    field_0x5de.y = field_0x5d2 * cM_ssin(field_0x5d4 - 8192.0f);
}

void daE_SB_c::Yazirushi() {
    eyePos.x = current.pos.x;
    eyePos.z = current.pos.z;
    eyePos.y = current.pos.y;
    eyePos.y += 80.0f;
    attention_info.position.y = eyePos.y;
    attention_info.position.y += 80.0f;
    attention_info.position.x = current.pos.x;
    attention_info.position.z = current.pos.z;
    s_TargetAngle = cLib_targetAngleY(&current.pos, s_LinkPos);
}

void daE_SB_c::setGroundAngle() {
    cXyz vec_0x44;
    cXyz vec_0x38;
    cXyz vec_0x2c;
    cXyz vec_0x20;
    dBgS_GndChk gnd_chk;
    vec_0x2c = current.pos;
    vec_0x2c.y = mAcch.GetGroundH();
    vec_0x44.x = vec_0x2c.x;
    vec_0x44.y = 50.0f + vec_0x2c.y;
    vec_0x44.z = 80.0f + vec_0x2c.z;
    gnd_chk.SetPos(&vec_0x44);
    vec_0x44.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
    if (-G_CM3D_F_INF != vec_0x44.y && fabsf(vec_0x44.y - vec_0x2c.y) < 50.0f) {
        vec_0x20 = vec_0x44 - vec_0x2c;
        cLib_chaseAngleS(&field_0x614.x, (s16) -cM_atan2s(vec_0x20.y, vec_0x20.z), 0x100);
    }

    vec_0x38.x = 50.0f + vec_0x2c.x;
    vec_0x38.y = 50.0f + vec_0x2c.y;
    vec_0x38.z = vec_0x2c.z;
    gnd_chk.SetPos(&vec_0x38);
    vec_0x38.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
    if (-G_CM3D_F_INF != vec_0x38.y && fabsf(vec_0x38.y - vec_0x2c.y) < 50.0f) {
        vec_0x20 = vec_0x38 - vec_0x2c;
        cLib_chaseAngleS(&field_0x614.z, (s16) cM_atan2s(vec_0x20.y, vec_0x20.x), 0x100);
    }
}

int daE_SB_c::Execute() {
    s_LinkPos = &fopAcM_GetPosition(daPy_getPlayerActorClass());
    field_0x5c8 -= 1;
    if (field_0x5c8 < 0) {
        field_0x5c8 = 0;
    }

    Yazirushi();
    if (field_0x5e4 == 1) {
        HashiraVib();
    }

    Anm_Motion();
    setGroundAngle();
    if (speed.y > 0.0f) {
        mAcch.OffClrSpeedY();
    } else {
        mAcch.OnClrSpeedY();
    }

    mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    field_0x60c -= 1;
    if (field_0x60c < 0) {
        field_0x60c = 0;
    }

    setCcCylinder();
    fopAcM_posMoveF(this, mStts.GetCCMoveP());
    mAcch.CrrPos(dComIfG_Bgsp());
    setBaseMtx();
    return 1;
}

int daE_SB_c::Draw() {
    J3DModel* model = mpMorf->getModel();
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);
    J3DShape* shape = mpMorf->getModel()->getModelData()->getMaterialNodePointer(1)->getShape();
    if (shape != NULL) {
        if (field_0x5b0 == 2) {
            shape->hide();
        } else {
            shape->show();
        }
    }

    mpMorf->entryDL();
    cXyz my_vec;
    my_vec.set(current.pos.x, 100.0f + current.pos.y, current.pos.z);
    _GXTexObj* tex_obj = dDlst_shadowControl_c::getSimpleTex();
    mShadowKey = dComIfGd_setShadow(mShadowKey, 1, model, &my_vec, 1000.0f, 300.0f,
                                    current.pos.y, mAcch.GetGroundH(), mAcch.m_gnd,
                                    &tevStr, 0, 1.0f, tex_obj);
    return 1;
}

int daE_SB_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, "E_SB");
    if (field_0xbbc) {
        hio_set = 0;
    }

    if (heap != NULL) {
        mpMorf->stopZelAnime();
    }

    return 1;
}

void daE_SB_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(field_0x614);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(l_HIO.size, l_HIO.size, l_HIO.size);
    J3DModel* model = mpMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->modelCalc();
}

static int daE_SB_Draw(daE_SB_c* i_this) {
    return i_this->Draw();
}

static int daE_SB_Execute(daE_SB_c* i_this) {
    return i_this->Execute();
}

static int daE_SB_IsDelete(daE_SB_c*) {
    return 1;
}

static int daE_SB_Delete(daE_SB_c* i_this) {
    return i_this->Delete();
}

cPhs__Step daE_SB_c::Create() {
    fopAcM_ct(this, daE_SB_c);
    cPhs__Step phase = (cPhs__Step) dComIfG_resLoad(&mPhaseReq, "E_SB");
    if (phase == cPhs_COMPLEATE_e) {
        field_0x61a = fopAcM_GetParam(this);
        if (field_0x61a != 0xFF && dComIfGs_isSwitch(field_0x61a, fopAcM_GetRoomNo(this))) {
            return cPhs_ERROR_e;
        }

        OS_REPORT("E_SB PARAM %x\n", fopAcM_GetParam(this));
        if (!fopAcM_entrySolidHeap(this, useHeapInit, 0x1100)) {
            return cPhs_ERROR_e;
        }

        if (hio_set == 0) {
            hio_set = 1;
            field_0xbbc = 1;
            l_HIO.field_0x04 = -1;
        }

        attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        attention_info.distances[2] = 0x24;
        fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(this, -200.0f, 0.0f, -200.0f);
        fopAcM_SetMax(this, 200.0f, 200.0f, 200.0f);
        mAcchCir.SetWall(30.0f, 100.0f);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                  &mAcchCir, fopAcM_GetSpeed_p(this), NULL, 0);
        mSound.init(&current.pos, &eyePos, 3, 1);
        mAtInfo.mpSound = &mSound;
        mAtInfo.mPowerType = 1;
        mAtInfo.mpSound = &mSound;
        if (fopAcM_gc_c::gndCheck(&current.pos)) {
            current.pos.y = fopAcM_gc_c::getGroundY();
        }

        initCcCylinder();
        mpMorf->setAnm((J3DAnmTransform *) dComIfG_getObjectRes("E_SB", 0xC), 2, 5.0f, l_HIO.other_anm_speed, 0.0f, -1.0f);
        mSound.startCreatureSound(Z2SE_EN_SB_WAIT, 0, -1);
        field_0x5b0 = 1;
        gravity = -9.0f;
        field_0x5f4 = current.pos;
        field_0x600 = shape_angle.y;
        mCyl.OnTgShield();
        mCyl.SetTgHitMark(CcG_Tg_UNK_MARK_2);
        mCyl.OffAtSetBit();
        mSound.setEnemyName("E_SB");
        J3DModel* model = mpMorf->getModel();
        model->setUserArea((uintptr_t) this);
        for (u16 idx = 0; idx < model->getModelData()->getJointNum(); ++idx) {
            if (idx != 0) {
                model->getModelData()->getJointNodePointer(idx)->setCallBack(JointCallBack);
            }
        }

        field_0x5e6 = 3;
        daE_SB_Execute(this);
    }

    return phase;
}

static int daE_SB_Create(fopAc_ac_c* i_this) {
    return static_cast<daE_SB_c*>(i_this)->Create();
}

static actor_method_class l_daE_SB_Method = {
    (process_method_func)daE_SB_Create,
    (process_method_func)daE_SB_Delete,
    (process_method_func)daE_SB_Execute,
    (process_method_func)daE_SB_IsDelete,
    (process_method_func)daE_SB_Draw,
};

actor_process_profile_definition g_profile_E_SB = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_SB,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daE_SB_c),       // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  125,                    // mPriority
  &l_daE_SB_Method,       // sub_method
  0x00040100,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};

AUDIO_INSTANCES;
