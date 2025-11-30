/**
 * @file d_a_e_yr.cpp
 *
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_yr.h"
#include "d/actor/d_a_horse.h"
#include "d/d_cc_d.h"
#include "d/d_path.h"
#include "f_op/f_op_actor_enemy.h"

class e_yrHIO_c : public fOpAcm_HIO_entry_c {
public:
    e_yrHIO_c() {
        mInvulnerable = 0;
        mStationary = 0;
        field_0x8 = 20.0f;
        field_0xc = 2500.0f;
        mModelScale = 1.0f;
        mAttackAMinTime = 2000;
        mAttackAMaxTime = 2500;
        mAttackBMinTime = 2000;
        mAttackBMaxTime = 2500;
        mAttackMoveSpeed = 0.35f;
        mAttackFrequency = 0.5f;
        mFlyPlaybackSpeed = 1.0f;
        mHoveringPlaybackSpeed = 1.0f;
        mAttackPlaybackSpeed = 1.0f;
        field_0x38 = 1.0f;
        mChancePlaybackSpeed = 1.0f;
        field_0x40 = 30;
        field_0x44 = 1.0f;
        field_0x48 = 1.0f;
        field_0x4c = 1.5f;
        mBrakeStartDist = 1100;
        field_0x54 = 3.5f;
        mChanceTime = 60;
        mLargeDamage = 60.0f;
        mSmallDamage = 40.0f;
        mHitOccurrenceFrame = 1;
        mDetectDistSky = 8000.0f;
        mDetectHeightSky = 3000.0f;
        mDetectAngleSky = 20000;
        mDetectDistGround = 1500.0f;
        mDetectHeightGround = 2000.0f;
        mDetectAngleGround = 20000;
    }

    virtual ~e_yrHIO_c() {}

#if DEBUG
    void genMessage(JORMContext*);
#endif

    /* 0x04 */ u8 mInvulnerable;
    /* 0x05 */ u8 mStationary;
    /* 0x06 */ u8 mSuddenAttack;
    /* 0x08 */ f32 field_0x8;
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ f32 mTurningRadius;
    /* 0x14 */ f32 mModelScale;
    /* 0x18 */ s16 mAttackAMinTime;
    /* 0x1A */ s16 mAttackAMaxTime;
    /* 0x1C */ s16 mAttackBMinTime;
    /* 0x1E */ s16 mAttackBMaxTime;
    /* 0x20 */ f32 mAttackMoveSpeed;
    /* 0x24 */ f32 mAttackFrequency;
    /* 0x28 */ f32 mFlyPlaybackSpeed;
    /* 0x2C */ f32 mHoveringPlaybackSpeed;
    /* 0x30 */ f32 mAttackPlaybackSpeed;
    /* 0x34 */ s16 mChanceTime;
    /* 0x38 */ f32 field_0x38;
    /* 0x3C */ f32 mChancePlaybackSpeed;
    /* 0x40 */ s16 field_0x40;
    /* 0x44 */ f32 field_0x44;
    /* 0x48 */ f32 field_0x48;
    /* 0x4C */ f32 field_0x4c;
    /* 0x50 */ s16 mBrakeStartDist;
    /* 0x54 */ f32 field_0x54;
    /* 0x58 */ f32 mLargeDamage;
    /* 0x5C */ f32 mSmallDamage;
    /* 0x60 */ s16 mHitOccurrenceFrame;
    /* 0x64 */ f32 mDetectDistSky;
    /* 0x68 */ f32 mDetectDistGround;
    /* 0x6C */ s16 mDetectAngleSky;
    /* 0x6E */ s16 mDetectAngleGround;
    /* 0x70 */ f32 mDetectHeightSky;
    /* 0x74 */ f32 mDetectHeightGround;
};

#if DEBUG
void e_yrHIO_c::genMessage(JORMContext* ctx) {
    // "Kagarok"
    ctx->genLabel("\u3000闇カーゴロック", 0x80000001, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genCheckBox("不死身", &mInvulnerable, 1, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genCheckBox("静止", &mStationary, 1, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genCheckBox("いきなり攻撃", &mSuddenAttack, 1, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("旋回半径", &mTurningRadius, 0.0, 10000.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("モデルスケール", &mModelScale, 0.0, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("Ａ攻時間（最短）", &mAttackAMinTime, 0, 15000, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    ctx->genSlider("Ａ攻時間（最長）", &mAttackAMaxTime, 0, 15000, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    ctx->genSlider("Ｂ攻時間（最短）", &mAttackBMinTime, 0, 15000, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    ctx->genSlider("Ｂ攻時間（最長）", &mAttackBMaxTime, 0, 15000, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    ctx->genSlider("攻撃移動速度", &mAttackMoveSpeed, 0.0, 2.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    ctx->genSlider("攻撃頻度", &mAttackFrequency, 0.0, 10.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("fly再生速度", &mFlyPlaybackSpeed, 0.0, 3.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    ctx->genSlider("hovering再生速度", &mHoveringPlaybackSpeed, 0.0, 3.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("atack再生速度", &mAttackPlaybackSpeed, 0.0, 3.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    ctx->genSlider("chance再生速度", &mChancePlaybackSpeed, 0.0, 3.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("chance時間", &mChanceTime, 0, 200, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("brake開始距離", &mBrakeStartDist, 0, 3000, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    ctx->genSlider("大ダメージ力", &mLargeDamage, 0.0, 150.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("小ダメージ力", &mSmallDamage, 0.0, 150.0f, 0, NULL, 0xffff, 0xffff, 0x200, 0x18);
    ctx->genSlider("ＨＩＴ発生フレーム", &mHitOccurrenceFrame, 0, 5, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    ctx->genSlider("プ認距離（空）", &mDetectDistSky, 0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    ctx->genSlider("プ認高さ（空）", &mDetectHeightSky, 0.0f, 5000.0f, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    ctx->genSlider("プ認角度（空）", &mDetectAngleSky, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
    ctx->genSlider("プ認距離（地）", &mDetectDistGround, 0.0f, 10000.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("プ認高さ（地）", &mDetectHeightGround, 0.0f, 5000.0f, 0, NULL, 0xffff, 0xffff,
                   0x200, 0x18);
    ctx->genSlider("プ認角度（地）", &mDetectAngleGround, 0, 0x7fff, 0, NULL, 0xffff, 0xffff, 0x200,
                   0x18);
}
#endif

static int nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DJoint* joint = i_joint;
        s32 jointNo = joint->getJntNo();
        J3DModel* model = j3dSys.getModel();

        e_yr_class* yr = (e_yr_class*)model->getUserArea();
        e_yr_class* yr_2 = yr;
        if (yr != NULL) {
            MTXCopy(model->getAnmMtx(jointNo), *calc_mtx);
            if (jointNo == 3 || jointNo == 4) {
                cMtx_YrotM(*calc_mtx, yr->field_0xe84 / 2);
                cMtx_XrotM(*calc_mtx, yr->field_0xe84);
                cMtx_ZrotM(*calc_mtx, yr->field_0xe88 / 3);
            } else if (jointNo == 8) {
                cMtx_ZrotM(*calc_mtx, -(yr->field_0xe88 / 3));
            } else if (jointNo == 7) {
                cMtx_ZrotM(*calc_mtx, -yr->field_0xe88);
            }

            if (jointNo >= 10 && jointNo <= 13) {
                cMtx_ZrotM(*calc_mtx, yr->field_0xea8 / 2);
                cMtx_XrotM(*calc_mtx, yr->field_0xe96[jointNo - 9]);
            } else {
                if (jointNo >= 15 && jointNo <= 18) {
                    cMtx_ZrotM(*calc_mtx, yr->field_0xeaa / 2);
                    cMtx_XrotM(*calc_mtx, yr->field_0xe96[jointNo - 9]);
                }
            }

            if (jointNo == 3) {
                cMtx_ZrotM(*calc_mtx, yr->field_0xeb4);
            } else if (jointNo == 19) {
                cMtx_ZrotM(*calc_mtx, -yr->field_0xeb4);
            } else if (jointNo == 20) {
                cMtx_ZrotM(*calc_mtx, -yr->field_0xeb6);
            } else if (jointNo == 25) {
                cMtx_ZrotM(*calc_mtx, -yr->field_0xeb8);
            } else if (jointNo == 30 || jointNo == 31) {
                cMtx_YrotM(*calc_mtx, (s16)yr->field_0xebe);
                cMtx_ZrotM(*calc_mtx, (s16)yr->field_0xebe);
            }

            if (jointNo == TREG_S(0)) {
                cMtx_YrotM(*calc_mtx, TREG_S(1));
                cMtx_XrotM(*calc_mtx, TREG_S(2));
                cMtx_ZrotM(*calc_mtx, TREG_S(3));
            } else if (jointNo == TREG_S(4)) {
                cMtx_YrotM(*calc_mtx, TREG_S(5));
                cMtx_XrotM(*calc_mtx, TREG_S(6));
                cMtx_ZrotM(*calc_mtx, TREG_S(7));
            }

            if (yr->field_0xec4 != 0) {
                if ((jointNo & 1) != 0) {
                    cMtx_YrotM(*calc_mtx, yr->field_0xec4);
                } else if ((jointNo & 2) != 0) {
                    cMtx_XrotM(*calc_mtx, yr->field_0xec4);
                } else if ((jointNo & 4) != 0) {
                    cMtx_ZrotM(*calc_mtx, yr->field_0xec4);
                }
            }

            model->setAnmMtx(jointNo, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }

    return 1;
}

static void yr_disappear(e_yr_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->mEnemy;

    cXyz unkXyz1;
    cXyz unkXyz2(0.0f, 0.0f, 0.0f);
    cXyz unkXyz3(1.0f, 1.0f, 1.0f);

    dComIfGp_particle_set(0x826c, &actor->current.pos, NULL, &unkXyz3);

    if (i_this->field_0x5b8 == 6) {
        cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
        unkXyz1.x = 28.0f;
        unkXyz1.y = 88.0f;
        unkXyz1.z = 4.0f;
        MtxPosition(&unkXyz1, &unkXyz2);
    }

    unkXyz2 += actor->current.pos;

    dComIfGp_particle_set(0x826d, &unkXyz2, NULL, &unkXyz3);

    fopAcM_seStart(actor, Z2SE_DARK_VANISH, 0);
    fopAcM_createItemFromEnemyID(Z2_ENEMY_YK, &actor->current.pos, -1, -1, 0, 0, 0,
                                 0);
    if (i_this->field_0x668 != 0xff) {
        dComIfGs_onSwitch(i_this->field_0x668, fopAcM_GetRoomNo(actor));
    }
}

static void anm_init(e_yr_class* i_this, int param_1, f32 param_2, u8 param_3, f32 param_4) {
    if (i_this->field_0x66b < 3) {
        i_this->mpMorfSO->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_Yr", param_1), param_3,
                                 param_2, param_4, 0.0f, -1.0f);
        i_this->field_0x5b8 = param_1;
    }
}

static void kuti_open(e_yr_class* i_this, s16 param_1, u32 param_2) {
    if (i_this->field_0xe8a == 0) {
        i_this->field_0xe8a = param_1;
        i_this->field_0xe8c = param_1 - 5;
        i_this->field_0xe90 = param_2;
    }
}

static int e_yr_player_bg_check(e_yr_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->mEnemy;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    dBgS_LinChk linChk;
    cXyz unkXyz1;
    cXyz unkXyz2;
    cXyz unkXyz3;

    unkXyz3 = player->current.pos;
    unkXyz3.y += 100.0f;

    unkXyz2 = actor->current.pos;
    unkXyz2.y += 100.0f;

    linChk.Set(&unkXyz2, &unkXyz3, actor);

    if (dComIfG_Bgsp().LineCross(&linChk)) {
        return 1;
    } else {
        return 0;
    }
}

static e_yrHIO_c l_e_yrHIO;

static int e_yr_player_view_check(e_yr_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->mEnemy;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    if (l_e_yrHIO.mSuddenAttack != 0) {
        return 1;
    }

    if (i_this->field_0x665 != 0xff) {
        if (i_this->mPlayerLatDist > i_this->field_0x665 * 100.0f || e_yr_player_bg_check(i_this))
        {
            return 0;
        }
    } else if (i_this->field_0x669 == 4 || i_this->field_0x669 == 7) {
        if (i_this->mPlayerLatDist > l_e_yrHIO.mDetectDistGround || e_yr_player_bg_check(i_this)) {
            return 0;
        }
    } else if (i_this->mPlayerLatDist > l_e_yrHIO.mDetectDistSky || e_yr_player_bg_check(i_this)) {
        return 0;
    }

    s16 detectAngle;
    f32 detectHeight;
    if (i_this->field_0x669 == 4 || i_this->field_0x669 == 7) {
        detectAngle = l_e_yrHIO.mDetectAngleGround;
        detectHeight = l_e_yrHIO.mDetectHeightGround;
    } else {
        detectAngle = l_e_yrHIO.mDetectAngleSky;
        detectHeight = l_e_yrHIO.mDetectHeightSky;
    }

    f32 vDistToPlayer = fabsf(player->current.pos.y - actor->eyePos.y);
    if (vDistToPlayer < detectHeight) {
        s16 r28 = actor->current.angle.y - i_this->field_0xe84 - i_this->mYawToPlayer;
        if (r28 < 0) {
            r28 = -r28;
        }
        if ((u16)r28 < detectAngle) {
            return 1;
        }
    }
    return 0;
}

static void path_check(e_yr_class* i_this) {
    static u8 check_index[255];

    if (i_this->field_0x6ec != 0) {
        dPnt* point;
        fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->mEnemy;

        dBgS_LinChk linChk;
        cXyz unkXyz1;
        cXyz unkXyz2;

        unkXyz1 = actor->current.pos;
        unkXyz1.y += 100.0f;

        point = i_this->field_0x6ec->m_points;
        for (s32 i = 0; i < i_this->field_0x6ec->m_num; i++, point++) {
            unkXyz2.x = point->m_position.x;
            unkXyz2.y = point->m_position.y + 100.0f;
            unkXyz2.z = point->m_position.z;

            linChk.Set(&unkXyz1, &unkXyz2, actor);

            if (!dComIfG_Bgsp().LineCross(&linChk)) {
                check_index[i] = 1;
            } else {
                check_index[i] = 0;
            }
        }

        f32 xDist;
        f32 yDist;
        f32 zDist;
        f32 unkFloat1 = 0.0f;
        bool unkFlag1 = false;
        for (s32 i = 0; i < 100; i++, unkFloat1 += 50.0f) {
            point = i_this->field_0x6ec->m_points;
            for (s32 j = 0; j < i_this->field_0x6ec->m_num; j++, point++) {
                if (check_index[j] != 0) {
                    xDist = actor->current.pos.x - point->m_position.x;
                    yDist = actor->current.pos.y - point->m_position.y;
                    zDist = actor->current.pos.z - point->m_position.z;

                    if (JMAFastSqrt(xDist * xDist + yDist * yDist + zDist * zDist) < unkFloat1) {
                        i_this->field_0x6ea = j - (s32)i_this->field_0x6eb;
                        if (i_this->field_0x6ea >= (s8)i_this->field_0x6ec->m_num) {
                            i_this->field_0x6ea = i_this->field_0x6ec->m_num;
                        } else {
                            if (i_this->field_0x6ea < 0) {
                                i_this->field_0x6ea = 0;
                            }
                        }

                        unkFlag1 = true;

                        break;
                    }
                }
            }

            if (unkFlag1) {
                break;
            }
        }

        if (!unkFlag1) {
            i_this->field_0x6e9 = 0;
        } else {
            i_this->field_0x6e9 = i_this->field_0x666 + 1;
        }
    }
}

static int pl_horse_check(e_yr_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->mEnemy;
    if (daPy_getPlayerActorClass()->checkHorseRide() && dComIfGp_getHorseActor()->speedF > 20.0f) {
        return 1;
    } else {
        return 0;
    }
}

static void daE_Yr_shadowDraw(e_yr_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->mEnemy;

    J3DModel* model = i_this->mpMorfSO->getModel();
    cXyz unkXyz1;
    unkXyz1.set(actor->current.pos.x, actor->current.pos.y + 150.0f + BREG_F(18),
                  actor->current.pos.z);
    i_this->mShadowKey = dComIfGd_setShadow(i_this->mShadowKey, 1, model, &unkXyz1,
                                   800.0f + BREG_F(19), 40.0f + BREG_F(17), actor->current.pos.y,
                                   i_this->mAcch.GetGroundH(), i_this->mAcch.m_gnd, &actor->tevStr,
                                   0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
}

static int daE_Yr_Draw(e_yr_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->mEnemy;

    if (i_this->field_0x67e != 0) {
        return 1;
    }

    J3DModel* model = i_this->mpMorfSO->getModel();

    g_env_light.settingTevStruct(2, &actor->current.pos, &actor->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &actor->tevStr);

    dComIfGd_setListDark();

    i_this->mpMorfSO->entryDL();

    dComIfGd_setList();

    daE_Yr_shadowDraw(i_this);

    return 1;
}

static void e_yr_pos_move(e_yr_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->mEnemy;

    cXyz unkXyz1;

    f32 xDiff = i_this->field_0x680.x - actor->current.pos.x;
    f32 yDiff = i_this->field_0x680.y - actor->current.pos.y;
    f32 zDiff = i_this->field_0x680.z - actor->current.pos.z;
    s16 yaw = cM_atan2s(xDiff, zDiff);
    s16 pitch = -cM_atan2s(yDiff, JMAFastSqrt(xDiff * xDiff + zDiff * zDiff));
    s16 angleY = actor->current.angle.y;

    cLib_addCalcAngleS2(&actor->current.angle.y, yaw, 10 + TREG_S(3), i_this->field_0x69c * i_this->field_0x694);
    angleY = (angleY - actor->current.angle.y) * 0x20;
    s16 limit = 5500 + TREG_S(1);
    if (angleY > limit) {
        angleY = limit;
    } else if (angleY < -limit) {
        angleY = -limit;
    }

    cLib_addCalcAngleS2(&actor->current.angle.z, angleY, 10 + TREG_S(3),
                        i_this->field_0x69c * i_this->field_0x694 * 0.5f);

    cLib_addCalcAngleS2(&actor->current.angle.x, pitch, 10 + TREG_S(3),
                        i_this->field_0x69c * i_this->field_0x694);
    cLib_addCalc2(&i_this->field_0x694, 1.0f, 1.0f, 0.04f);
    cLib_addCalc2(&actor->speedF, i_this->field_0x68c, 1.0f, i_this->field_0x690);

    unkXyz1.x = 0.0f;
    unkXyz1.y = 0.0f;
    unkXyz1.z = actor->speedF;
    cMtx_YrotS(*calc_mtx, actor->current.angle.y);
    cMtx_XrotM(*calc_mtx, actor->current.angle.x);
    MtxPosition(&unkXyz1, &actor->speed);
    actor->current.pos.x += actor->speed.x;
    actor->current.pos.y += actor->speed.y;
    actor->current.pos.z += actor->speed.z;

    if (i_this->field_0x6b0 != 0) {
        i_this->field_0x6b0--;
        actor->current.pos.y += 5.0f;
    }
}

static void e_yr_ground_pos_move(e_yr_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->mEnemy;

    cXyz unkXyz1;
    cXyz unkXyz2;
    f32 xDiff = i_this->field_0x680.x - actor->current.pos.x;
    f32 zDiff = i_this->field_0x680.z - actor->current.pos.z;
    s16 yaw = cM_atan2s(xDiff, zDiff);
    cLib_addCalcAngleS2(&actor->current.angle.y, yaw, TREG_S(3) + 2,
                        i_this->field_0x69c * i_this->field_0x694);
    cLib_addCalc2(&i_this->field_0x694, 1.0f, 1.0f, 0.1f);
    cLib_addCalc2(&actor->speedF, i_this->field_0x68c, 1.0f, i_this->field_0x690);
    unkXyz1.x = 0.0f;
    unkXyz1.y = 0.0f;
    unkXyz1.z = actor->speedF;
    cMtx_YrotS(*calc_mtx, actor->current.angle.y);
    MtxPosition(&unkXyz1, &unkXyz2);
    actor->speed.x = unkXyz2.x;
    actor->speed.z = unkXyz2.z;
    actor->current.pos.x += actor->speed.x;
    actor->current.pos.y += actor->speed.y;
    actor->current.pos.z += actor->speed.z;
    actor->speed.y = actor->speed.y - 3.0f;

    if (i_this->mAcch.ChkGroundHit()) {
        actor->speed.y = -0.5f;
    }
}

static void e_yr_path_move(e_yr_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->mEnemy;

    fopAc_ac_c* playerActor = dComIfGp_getPlayer(0);
    daPy_py_c* player = (daPy_py_c*)playerActor;
    cXyz unkXyz1;

    s8 unkChar1 = 0;

    i_this->field_0xe94 = 2;

    s32 frame;
    switch (i_this->field_0x67c) {
    case 0:
        frame = i_this->mpMorfSO->getFrame();
        if (actor->current.pos.y > i_this->field_0x680.y &&
            frame == 9 + TREG_S(0))
        {
            i_this->field_0x67c = 1;
            anm_init(i_this, 12, 12.0f + TREG_F(0), 2, 1.0f);
        }

        break;
    case 1:
        if (actor->current.pos.y <= (i_this->field_0x680).y) {
            i_this->field_0x67c = 0;
            anm_init(i_this, 8, 5.0f, 2, l_e_yrHIO.mFlyPlaybackSpeed);
        }

        break;
    case 10:
    default:
        break;
    }

    f32 xDiff;
    f32 yDiff;
    f32 zDiff;
    f32 dist;
    dPnt* point;
    switch (i_this->field_0x67d) {
    case 0: {
        i_this->field_0x6ea += i_this->field_0x6eb;
        if (i_this->field_0x6ea >= (s8)i_this->field_0x6ec->m_num) {
            if (dPath_ChkClose(i_this->field_0x6ec)) {
                i_this->field_0x6ea = 0;
            } else {
                i_this->field_0x6eb = -1;
                i_this->field_0x6ea = i_this->field_0x6ec->m_num - 2;
            }

            s32 nextId = i_this->field_0x6ec->m_nextID;
            if (nextId != 0xffff) {
                i_this->field_0x6ec = dPath_GetRoomPath(nextId, fopAcM_GetRoomNo(actor));
                JUT_ASSERT(0x451, i_this->field_0x6ec != NULL);
            }
        } else {
            if (i_this->field_0x6ea < 0) {
                i_this->field_0x6eb = 1;
                i_this->field_0x6ea = 1;
            }
        }
    }
    case -1: {
        i_this->field_0x67d = 1;

        point = i_this->field_0x6ec->m_points;
        point = &point[i_this->field_0x6ea];

        if (i_this->field_0x6f0 != 0) {
            i_this->field_0x68c = 25.0f;
            i_this->field_0x690 = 2.0f;
            i_this->field_0x6f0 = 0;
        } else {
            if (actor->current.pos.y < i_this->field_0x680.y) {
                i_this->field_0x68c = 20.0f;
                i_this->field_0x690 = 1.0f + TREG_F(11);
            } else {
                i_this->field_0x68c = 30.0f;
                i_this->field_0x690 = 1.0f + TREG_F(13);
            }
        }

        i_this->field_0x694 = TREG_F(7);

        i_this->field_0x680.x = point->m_position.x;
        i_this->field_0x680.y = point->m_position.y;
        i_this->field_0x680.z = point->m_position.z;

        if (point->mArg0 == 1) {
            i_this->field_0x67d = 10;
        } else if (i_this->field_0x66b == 0) {
            i_this->field_0x680.x = point->m_position.x + cM_rndFX(150.0f);
            i_this->field_0x680.y = point->m_position.y + cM_rndFX(150.0f);
            i_this->field_0x680.z = point->m_position.z + cM_rndFX(150.0f);
        }

        if ((l_e_yrHIO.mSuddenAttack != 0 ||
             i_this->field_0x6a4[3] == 0 && i_this->field_0x66b == 0) &&
            e_yr_player_view_check(i_this) != 0)
        {
            i_this->field_0x669 = 3;
            i_this->field_0x67d = 0;
            i_this->field_0x6e9 = 0;
            i_this->mSound.startCreatureVoice(Z2SE_EN_YR_V_FIND, -1);
        }

        break;
    }
    case 1: {
        cLib_addCalc2(&i_this->field_0x694, 1.0f, 1.0f, 0.04f);

        xDiff = i_this->field_0x680.x - actor->current.pos.x;
        yDiff = i_this->field_0x680.y - actor->current.pos.y;
        zDiff = i_this->field_0x680.z - actor->current.pos.z;
        dist = JMAFastSqrt(xDiff * xDiff + yDiff * yDiff + zDiff * zDiff);
        i_this->field_0x69c =
            10.0f * TREG_F(5) + 100.0f + (1000.0f * TREG_F(6) + 500000.0f) / dist;

        if (i_this->field_0x69c > 10.0f * TREG_F(4) + 5000.0f) {
            i_this->field_0x69c = 10.0f * TREG_F(4) + 5000.0f;
        }
        if (dist < 10.0f * TREG_F(10) + 300.0f) {
            i_this->field_0x67d = 0;
            point = i_this->field_0x6ec->m_points;
            point = &point[i_this->field_0x6ea];
            if (point->mArg0 == 5) {
                i_this->field_0x66c = 1;
            } else if (point->mArg0 == 6) {
                fopAcM_delete(actor);
            }
        }
        break;
    }
    case 10: {
        i_this->field_0x68c = 20.0f;

        xDiff = i_this->field_0x680.x - actor->current.pos.x;
        yDiff = i_this->field_0x680.y + 50.0f + TREG_F(8) * 10.0f -
                actor->current.pos.y;
        zDiff = i_this->field_0x680.z - actor->current.pos.z;
        dist = JMAFastSqrt(xDiff * xDiff + yDiff * yDiff + zDiff * zDiff);
        if (dist < TREG_F(9) * 10.0f + 300.0f) {
            i_this->field_0x67d = 20;
            i_this->field_0x67c = 10;
            actor->speed.y = 0.0f;
            unkChar1 = -1;
        }

        break;
    }
    case 20: {
        unkChar1 = -1;
        frame = i_this->mpMorfSO->getFrame();

        cLib_addCalc2(&actor->current.pos.x, i_this->field_0x680.x, 0.1f, fabsf(actor->speed.x));
        cLib_addCalc2(&actor->current.pos.z, i_this->field_0x680.z, 0.1f, fabsf(actor->speed.z));

        actor->current.pos.y += actor->speed.y;

        if (frame > 15 + TREG_S(2)) {
            actor->speed.y -= 0.8f + TREG_F(7);
        }

        cLib_addCalcAngleS2(&actor->current.angle.x, 0, 5, 0x800);
        cLib_addCalcAngleS2(&actor->current.angle.z, 0, 5, 0x800);

        if (i_this->mAcch.ChkGroundHit()) {
            actor->speed.y = -0.5f;
        }
        if (frame == 44) {
        }

        if (i_this->field_0x6a4[2] == 0) {
            i_this->field_0x67d = 23;
        }

        break;
    }
    case 22: {
        unkChar1 = 1;
        if (i_this->field_0x6a4[2] == 0) {
            i_this->field_0x6ea++;
            if (i_this->field_0x6ea >= i_this->field_0x6ec->m_num) {
                i_this->field_0x6ea = 0;
            }

            point = i_this->field_0x6ec->m_points;
            point = &point[i_this->field_0x6ea];
            i_this->field_0x680.x = point->m_position.x;
            i_this->field_0x680.y = point->m_position.y;
            i_this->field_0x680.z = point->m_position.z;

            if (point->mArg0 == 2) {
                i_this->field_0x67d = 25;
            } else {
                i_this->field_0x67d = 23;
                i_this->field_0x69c = 1000.0f;
                i_this->field_0x694 = 1.0f;
            }
        }

        i_this->field_0x68c = 0.0f;
        actor->speedF = 0.0f;

        break;
    }
    case 23: {
        unkChar1 = 1;
        frame = i_this->mpMorfSO->getFrame();
        if (frame >= TREG_S(3) && frame <= 9 + TREG_S(4)) {
            i_this->field_0x68c = i_this->field_0x6b4 * 5.0f;
            actor->speedF = i_this->field_0x6b4 * 5.0f;
        } else {
            i_this->field_0x68c = 0.0f;
            actor->speedF = 0.0f;
            xDiff = i_this->field_0x680.x - actor->current.pos.x;
            zDiff = i_this->field_0x680.z - actor->current.pos.z;
            if (JMAFastSqrt(xDiff * xDiff + zDiff * zDiff) < 50.0f) {
                i_this->field_0x67d = 22;
                point = i_this->field_0x6ec->m_points;
                point = &point[i_this->field_0x6ea];
                if (point->mArg0 == 3) {
                    i_this->field_0x6a4[2] = cM_rndF(50.0f) + 50.0f;
                }
            }
        }

        break;
    }
    case 25: {
        unkChar1 = 1;
        anm_init(i_this, 8, 5.0f, 2, l_e_yrHIO.mFlyPlaybackSpeed);
        i_this->field_0x67d = 0xff;
        i_this->field_0x69c = 10.0f * TREG_F(4) + 5000.0f;
        i_this->field_0x68c = 25.0f;
        i_this->field_0x690 = 2.0f;
        actor->speedF = 0.0f;
        i_this->field_0x6b0 = 10 + TREG_S(4);

        break;
    }
    default:
        break;
    }

    switch (unkChar1) {
    case 0:
        e_yr_pos_move(i_this);
        break;
    case 1:
        e_yr_ground_pos_move(i_this);
        break;
    }
}

static void e_yr_auto_move(e_yr_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->mEnemy;
    fopAc_ac_c* playerActor = dComIfGp_getPlayer(0);
    daPy_py_c* player = (daPy_py_c*)playerActor;

    cXyz unkXyz1;
    cXyz unkXyz2;

    i_this->field_0xe94 = 2;

    s32 frame;
    switch (i_this->field_0x67c) {
    case 0:
        frame = i_this->mpMorfSO->getFrame();
        if (i_this->field_0x6a4[0] == 0 && frame == 9 + TREG_S(0)) {
            i_this->field_0x67c = 1;
            i_this->field_0x6a4[0] = cM_rndF(200.0f) + 50.0f;

            anm_init(i_this, 12, 12.0f + TREG_F(0), 2, 1.0f);
        }

        break;
    case 1:
        if (i_this->field_0x6a4[0] == 0 && actor->current.pos.y < i_this->field_0x680.y) {
            i_this->field_0x67c = 0;
            i_this->field_0x6a4[0] = cM_rndF(60.0f) + 20.0f;

            anm_init(i_this, 8, 5.0f, 2, l_e_yrHIO.mFlyPlaybackSpeed);
        }

        break;
    case 10:
        break;
    case 20:
        frame = i_this->mpMorfSO->getFrame();
        if (frame == 34) {
            i_this->field_0x67c = 0;
            i_this->field_0x6a4[0] = cM_rndF(60.0f) + 20.0f;

            anm_init(i_this, 8, 0.0f, 2, l_e_yrHIO.mFlyPlaybackSpeed);
        }

        break;
    }

    f32 xDiff;
    f32 zDiff;
    switch (i_this->field_0x67d) {
    case 0:
        if (i_this->field_0x6a4[1] == 0) {
            xDiff =
                actor->home.pos.x + cM_rndFX(l_e_yrHIO.field_0xc) - actor->current.pos.x;
            zDiff =
                actor->home.pos.z + cM_rndFX(l_e_yrHIO.field_0xc) - actor->current.pos.z;
            if (JMAFastSqrt(xDiff * xDiff + zDiff * zDiff) > 200.0f) {
                i_this->field_0x6a4[1] = cM_rndF(150.0f) + 50.0f;

                i_this->field_0x680.x = xDiff + actor->current.pos.x;
                i_this->field_0x680.y = actor->home.pos.y + cM_rndF(500.0f);
                i_this->field_0x680.z = zDiff + actor->current.pos.z;

                i_this->field_0x694 = 0.0f;
                i_this->field_0x68c = 25.0f + TREG_F(10);
                i_this->field_0x690 = 1.0f + TREG_F(11);
                i_this->field_0x69c = cM_rndF(300.0f) + 200.0f;

                if ((l_e_yrHIO.mSuddenAttack != 0 ||
                     i_this->field_0x6a4[3] == 0 && i_this->field_0x66b == 0) &&
                    e_yr_player_view_check(i_this))
                {
                    i_this->field_0x669 = 3;
                    i_this->field_0x67d = 0;

                    i_this->mSound.startCreatureVoice(Z2SE_EN_YR_V_FIND, -1);
                }
            }
        }
    }

    e_yr_pos_move(i_this);
}

static void e_yr_atack_move(e_yr_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->mEnemy;

    fopAc_ac_c* playerActor = dComIfGp_getPlayer(0);
    daPy_py_c* player2 = (daPy_py_c*)playerActor;

    f32 playerDist = fopAcM_searchPlayerDistance(actor);

    cXyz unkXyz1;
    s8 unkFlag1 = 0;
    s8 unkFlag2 = 0;

    i_this->field_0xe94 = 1;

    if ((s8)i_this->field_0xec8 != 0) {
        i_this->field_0xec8 = 0;
        i_this->field_0x6a4[5] = l_e_yrHIO.mAttackFrequency * (cM_rndF(30.0f) + 20.0f);

        anm_init(i_this, 15, 10.0f, 2, l_e_yrHIO.mHoveringPlaybackSpeed);

        i_this->field_0x694 = i_this->field_0x698 = 0.0f;
        i_this->field_0x67d = 4;

        kuti_open(i_this, 25, Z2SE_EN_YR_V_NAKU);
    }

    s32 frame = i_this->mpMorfSO->getFrame();

    switch (i_this->field_0x67d) {
    case 0: {
        i_this->field_0x67d = 1;
        i_this->field_0x694 = i_this->field_0x698 = 0.0f;
        i_this->field_0x68c = 30.0f;
        i_this->field_0x690 = 1.0f;
        anm_init(i_this, 12, 10.0f, 2, 1.0f);
    }
    case 1:
    case 2: {
        i_this->field_0x680 = playerActor->current.pos;
        i_this->field_0x680.y += 200.0f;

        e_yr_pos_move(i_this);

        unkXyz1 = i_this->field_0x680 - actor->current.pos;
        f32 dist = unkXyz1.abs();
        if (dist < 100.0f * TREG_F(9) + 500.0f) {
            i_this->field_0x67d = 3;
        } else {
            if (i_this->field_0x67d == 1) {
                i_this->field_0x69c = 400.0f;
                if (dist < l_e_yrHIO.mBrakeStartDist) {
                    i_this->field_0x67d = 2;
                    anm_init(i_this, 11, 10.0f, 0, 1.0f);
                    kuti_open(i_this, 25, Z2SE_EN_YR_V_NAKU);
                }
            } else {
                i_this->field_0xeb0 = KREG_F(0xb) + 1000.0f;
                i_this->field_0x69c = 2000.0f;
            }
        }

        break;
    }
    case 3:
    case 35: {
        i_this->field_0x68c = 0.0f;
        i_this->field_0x690 = 3.0f;
        e_yr_pos_move(i_this);
        if (actor->speedF <= 0.1f || i_this->mAcch.ChkWallHit())
        {
            i_this->field_0x67d = 4;
            i_this->field_0x6a4[0] = 0;

            if (i_this->field_0x664 == 4 || i_this->field_0x664 == 7) {
                i_this->field_0x6a4[1] = l_e_yrHIO.mAttackBMinTime + cM_rndF(l_e_yrHIO.mAttackBMaxTime - l_e_yrHIO.mAttackBMinTime);
            } else {
                i_this->field_0x6a4[1] = l_e_yrHIO.mAttackAMinTime + cM_rndF(l_e_yrHIO.mAttackAMaxTime - l_e_yrHIO.mAttackAMinTime);
            }

            i_this->field_0x694 = i_this->field_0x698 = 0.0f;

            kuti_open(i_this, 25, Z2SE_EN_YR_V_NAKU);
        }
        if (i_this->field_0x67d == 3 && actor->speedF <= 10.0f) {
            i_this->field_0x67d = 35;

            anm_init(i_this, 15, 5.0f, 2, l_e_yrHIO.mHoveringPlaybackSpeed);
        }

        break;
    }
    case 4: {
        i_this->field_0x6e0++;

        unkXyz1.x = cM_ssin(i_this->field_0x6e0 * (TREG_S(2) + 1000)) * 200.0f;
        unkXyz1.y = cM_ssin(i_this->field_0x6e0 * (TREG_S(3) + 0x4b0)) * 100.0f;
        unkXyz1.z = cM_scos(i_this->field_0x6e0 * (TREG_S(4) + 0x5dc)) * 200.0f;

        cLib_addCalc2(&actor->current.pos.x, i_this->field_0x680.x + unkXyz1.x, 0.1f,
                      i_this->field_0x694 * 30.0f);
        cLib_addCalc2(&actor->current.pos.y, i_this->field_0x680.y + unkXyz1.y, 0.1f,
                      i_this->field_0x694 * 30.0f);
        cLib_addCalc2(&actor->current.pos.z, i_this->field_0x680.z + unkXyz1.z, 0.1f,
                      i_this->field_0x694 * 30.0f);
        cLib_addCalc2(&i_this->field_0x694, l_e_yrHIO.mAttackMoveSpeed, 1.0f,
                      l_e_yrHIO.mAttackMoveSpeed * 0.1f);
        cLib_addCalcAngleS2(&actor->current.angle.y, i_this->mYawToPlayer, 5,
                            i_this->field_0x698 * 1280.0f);
        cLib_addCalcAngleS2(&actor->current.angle.x, i_this->mPitchToPlayer, 5,
                            i_this->field_0x698 * 768.0f);
        cLib_addCalcAngleS2(&actor->current.angle.z, 0, 5, i_this->field_0x698 * 768.0f);
        cLib_addCalc2(&i_this->field_0x698, 1.0f, 1.0f, 0.05f);

        if (i_this->field_0x6a4[1] == 0) {
            unkFlag1 = 1;
        } else {
            if ((i_this->field_0x6a4[0] & 0x1f) == 0 && cM_rndF(1.0f) < 0.5f) {
                kuti_open(i_this, 25, Z2SE_EN_YR_V_NAKU);
            }
            if (i_this->field_0x6a4[0] == 0) {
                cMtx_YrotS(*calc_mtx, playerActor->shape_angle.y + (s16)cM_rndFX(7000.0f));
                unkXyz1.x = 0.0f;
                unkXyz1.y = cM_rndF(100.0f) + 250.0f + TREG_F(15);
                unkXyz1.z = cM_rndF(100.0f) + 300.0f + TREG_F(16);

                MtxPosition(&unkXyz1, &i_this->field_0x680);

                i_this->field_0x680 += playerActor->current.pos;
                i_this->field_0x6a4[0] = cM_rndF(100.0f) + 20.0f;
                i_this->field_0x694 = i_this->field_0x698 = 0.0f;
            }
            if (i_this->field_0x6a4[5] == 0 &&
                i_this->mPlayerLatDist > 10.0f * TREG_F(5) + 300.0f &&
                i_this->mPlayerLatDist < 10.0f * TREG_F(6) + 400.0f && frame >= 14 + BREG_S(7))
            {
                i_this->field_0x67d = 7;
                i_this->field_0x6a4[5] = l_e_yrHIO.mAttackFrequency * (cM_rndF(30.0f) + 20.0f);

                anm_init(i_this, 4, 2.0f, 0, l_e_yrHIO.mAttackPlaybackSpeed);

                i_this->mSound.startCreatureVoice(Z2SE_EN_YR_V_ATTACK, -1);

                i_this->field_0x694 = i_this->field_0x698 = 0.0f;
                i_this->field_0x68c = 0.0f;
                i_this->field_0x690 = 5.0f;
                actor->speedF = 0.0f;
            }
        }
        if (playerDist > 600.0f) {
            i_this->field_0x67d = 5;
            i_this->field_0x6a4[0] = cM_rndF(35.0f) + 35.0f;
        }
        break;
    }
    case 5: {
        if (((i_this->field_0x6de & 0x3f) == 0) && cM_rndF(1.0f) < 0.5f) {
            kuti_open(i_this, 25, Z2SE_EN_YR_V_NAKU);
        }
        i_this->field_0x68c = 25.0f;
        i_this->field_0x690 = 2.0f;
        i_this->field_0x69c = 2000.0f;
        i_this->field_0x680.x = playerActor->current.pos.x;
        i_this->field_0x680.y = playerActor->current.pos.y + 200.0f + TREG_F(19);
        i_this->field_0x680.z = playerActor->current.pos.z;
        e_yr_pos_move(i_this);
        if (playerDist < 500.0f) {
            i_this->field_0x67d = 4;
            i_this->field_0x694 = i_this->field_0x698 = 0.0f;
        }
        if (i_this->field_0x6a4[0] == 0 && playerDist > 1350.0f) {
            i_this->field_0x67d = 0;
        }
        if (i_this->field_0x6a4[1] == 0) {
            unkFlag1 = 1;
        }
        break;
    }
    case 7: {
        i_this->field_0xe94 = 0;
        if (frame == 3) {
            kuti_open(i_this, 25, Z2SE_SY_DUMMY);
        }
        if (frame == 15 + TREG_S(5)) {
            i_this->field_0x68c = 30.0f + TREG_F(7);
            i_this->field_0x690 = 10.0f + TREG_F(8);
        }
        if (frame == 25 + TREG_S(6)) {
            i_this->field_0x68c = -10.0f + TREG_F(9);
            i_this->field_0x690 = 5.0f + TREG_F(10);
        }
        if (frame <= 23) {
            i_this->field_0x69c = 2000.0f;
            i_this->field_0x680.x = playerActor->current.pos.x;
            i_this->field_0x680.y = playerActor->current.pos.y + 100.0f;
            i_this->field_0x680.z = playerActor->current.pos.z;
        } else {
            i_this->field_0x69c = 0.0f;
        }

        e_yr_pos_move(i_this);

        if (i_this->mpMorfSO->isStop()) {
            i_this->field_0x67d = 4;

            anm_init(i_this, 15, 3.0f, 2, l_e_yrHIO.mHoveringPlaybackSpeed);

            i_this->field_0x6a4[5] = l_e_yrHIO.mAttackFrequency * (cM_rndF(30.0f) + 20.0f);

            cMtx_YrotS(*calc_mtx, playerActor->shape_angle.y + (s16)cM_rndFX(3000.0f));

            unkXyz1.x = 0.0f;
            unkXyz1.y = cM_rndF(100.0f) + 250.0f + TREG_F(15);
            unkXyz1.z = cM_rndF(100.0f) + 300.0f + TREG_F(16);

            MtxPosition(&unkXyz1, &i_this->field_0x680);

            i_this->field_0x680 += playerActor->current.pos;
            i_this->field_0x6a4[0] = cM_rndF(100.0f) + 20.0f;
            i_this->field_0x694 = i_this->field_0x698 = 1.0f;
        } else {
            if (frame >= 15 && frame <= 27) {
                unkFlag2 = 1;
                if (player2->checkPlayerGuard() && i_this->mSph1.ChkAtHit()) {
                    i_this->field_0x6b2 = 10;
                    i_this->field_0x67d = 10;
                    i_this->field_0x6a4[0] = l_e_yrHIO.mChanceTime;

                    anm_init(i_this, 14, 0.0f, 0, l_e_yrHIO.field_0x38);

                    i_this->field_0x68c = TREG_F(7);
                    i_this->field_0x690 = 1.0f;
                    actor->speedF = -20.0f;
                    unkFlag2 = 0;

                    i_this->mSound.startCreatureVoice(Z2SE_EN_YR_V_DAMAGE_S, -1);
                }
            }
        }
        break;
    }
    case 10:
    case 11: {
        i_this->field_0xe94 = 0;
        e_yr_pos_move(i_this);
        if ((s32)i_this->mpMorfSO->getFrame() == 24) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_YR_V_FURA, -1);
        }

        if (i_this->mpMorfSO->isStop() && i_this->field_0x67d == 10) {
            i_this->field_0x67d = 11;
            anm_init(i_this, 5, 0.0f, 2, l_e_yrHIO.mChancePlaybackSpeed);
        }

        if (i_this->field_0x6a4[0] == 0) {
            i_this->field_0x67d = 4;
            i_this->field_0x6a4[5] = l_e_yrHIO.mAttackFrequency * (cM_rndF(30.0f) + 20.0f);

            anm_init(i_this, 15, 10.0f, 2, l_e_yrHIO.mHoveringPlaybackSpeed);

            i_this->field_0x694 = i_this->field_0x698 = 0.0f;
        }

        cLib_addCalc2(&actor->current.pos.y, i_this->mAcch.GetGroundH() + 100.0f + TREG_F(17), 0.05f, 5.0f);

        break;
    }
    }

    if (i_this->field_0x664 != 3) {
        unkXyz1 = i_this->field_0x680 - actor->current.pos;
        f32 unusedDist = unkXyz1.abs();

        if (unkFlag1) {
            i_this->field_0x669 = i_this->field_0x664;
            if (i_this->field_0x666 != 0xff) {
                i_this->field_0x6e9 = i_this->field_0x666 + 1;

                path_check(i_this);
            }
            if (i_this->field_0x669 == 4 || i_this->field_0x669 == 7) {
                i_this->field_0x67d = 10;

                anm_init(i_this, 8, 5.0f, 2, l_e_yrHIO.mFlyPlaybackSpeed);
            } else {
                i_this->field_0x67d = 0;
                i_this->field_0x6a4[0] = 0;
                i_this->field_0x6a4[1] = 0;
                i_this->field_0x67c = 1;

                anm_init(i_this, 12, 12.0f + TREG_F(0), 2, 1.0f);

                actor->speedF = 0.0f;

                i_this->field_0x680.y = playerActor->current.pos.y + 500.0f;
                i_this->field_0x6a4[3] = cM_rndF(200.0f) + 300.0f;
            }
            i_this->field_0x694 = 0.0f;
        }
    }

    if (unkFlag2) {
        i_this->mSph1.SetC(actor->eyePos);
        dComIfG_Ccsp()->Set(&i_this->mSph1);
    } else {
        i_this->mSph1.ClrAtHit();
    }

    if ((i_this->field_0x6de & 0x3f) == 0 && pl_horse_check(i_this)) {
        i_this->field_0x669 = 9;
        i_this->field_0x67d = 0;
    }
}

static void e_yr_horse_move(e_yr_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->mEnemy;
    fopAc_ac_c* playerActor = dComIfGp_getPlayer(0);
    daPy_py_c* player = (daPy_py_c*)playerActor;

    cXyz unkXyz1;
    cXyz unkXyz2;

    i_this->field_0xe94 = 1;

    s32 frame = i_this->mpMorfSO->getFrame();

    f32 horseSpeed = dComIfGp_getHorseActor()->speedF;
    if (horseSpeed > 60.0f) {
        horseSpeed = 60.0f;
    } else {
        if (horseSpeed < 30.0f) {
            horseSpeed = 30.0f;
        }
    }

    s8 unkFlag1 = 0;

    f32 unkFloat0;
    f32 unkFloat1;
    f32 unkFloat2;
    switch (i_this->field_0x67d) {
    case 0:
        i_this->field_0x67d = 2;
        i_this->field_0x694 = i_this->field_0x698 = 0.0f;

        anm_init(i_this, 8, 5.0f, 2, cM_rndF(0.2f) + 1.2f);

        i_this->field_0x6a4[1] = cM_rndF(100.0f) + 60.0f;
        i_this->field_0x6c4 = playerActor->shape_angle.y;
        i_this->field_0x6a4[0] = 0;
        i_this->field_0x6a4[2] = 0;
        i_this->field_0x6a4[3] = 0;
    case 1:
        if (i_this->field_0x6a4[1] == 0) {
            anm_init(i_this, 8, 5.0f, 2, cM_rndF(0.2f) + 1.0f);

            i_this->field_0x67d = 2;
            i_this->field_0x6a4[1] = cM_rndF(150.0f) + 70.0f;
        }

        goto lbl_case2;
    case 2:
        if (i_this->field_0x6a4[1] == 0) {
            anm_init(i_this, 12, 10.0f, 2, 1.0f);

            i_this->field_0x67d = 1;
            i_this->field_0x6a4[1] = cM_rndF(150.0f) + 70.0f;
        }

    lbl_case2:
        cLib_addCalcAngleS2(&i_this->field_0x6c4, playerActor->shape_angle.y, 4, 0x200);
        if (i_this->field_0x6a4[0] == 0) {
            i_this->field_0x6bc = cM_rndFX(400.0f);
            i_this->field_0x6c0 = cM_rndF(180.0f) + 100.0f;
            i_this->field_0x6a4[0] = cM_rndF(70.0f) + 20.0f;
        }

        unkFloat1 = cM_ssin(i_this->field_0x6de * (KREG_S(7) + 200)) * 20.0f;
        cMtx_YrotS(*calc_mtx, i_this->field_0x6c4);
        unkXyz1.x = i_this->field_0x6bc;
        unkXyz1.y = i_this->field_0x6c0;
        unkXyz1.z = (horseSpeed - 30.0f) * (unkFloat1 + 30.0f);
        MtxPosition(&unkXyz1, &unkXyz2);
        i_this->field_0x680 = playerActor->current.pos + unkXyz2;

        unkXyz2 = i_this->field_0x680 - actor->current.pos;
        unkFloat2 = unkXyz2.abs();

        if (i_this->field_0x6a4[2] == 0) {
            i_this->field_0x6a4[2] = cM_rndF(30.0f) + 20.0f;

            if (unkFloat2 < 300.0f) {
                horseSpeed *= 0.95f;
                i_this->field_0x69c = 400.0f + NREG_F(3);
            } else {
                horseSpeed *= 1.2f;
                i_this->field_0x69c = 600.0f + NREG_F(4);
            }

            i_this->field_0x68c = horseSpeed;
            i_this->field_0x690 = 0.4f + NREG_F(2);

            if (i_this->field_0x68c - actor->speedF > 10.0f) {
                if (i_this->field_0x67d == 1) {
                    i_this->field_0x67d = 2;

                    anm_init(i_this, 8, 5.0f, 2, cM_rndF(0.2f) + 1.2f);

                    i_this->field_0x6a4[1] = cM_rndF(100.0f) + 60.0f;
                } else {
                    i_this->mpMorfSO->setPlaySpeed(cM_rndF(0.2f) + 1.2f);
                }
            }
        }

        if ((i_this->field_0x6de & 0x3f) == 0 && cM_rndF(1.0f) < 0.5f) {
            kuti_open(i_this, 25, Z2SE_EN_YR_V_NAKU);
        }

        if (i_this->field_0x6a4[3] == 0) {
            unkXyz1 = playerActor->eyePos - actor->current.pos;
            if (unkXyz1.abs() < 250.0f + JREG_F(0)) {
                i_this->field_0x67d = 3;

                anm_init(i_this, 10, 5.0f, 2, cM_rndF(0.1f) + 1.0f);

                i_this->mSound.startCreatureVoice(Z2SE_EN_YR_V_ATTACK2, -1);
                i_this->field_0x6a4[0] = 20 + JREG_S(1);
            }
        }

        break;
    case 3:
        unkFlag1 = 1;
        if (i_this->field_0x6a4[0] == 0) {
            anm_init(i_this, 9, 3.0f, 0, 1.0f);

            i_this->field_0x67d = 4;
        }

        break;
    case 4:
        unkFlag1 = 1;
        unkFloat0 = 30.0f + JREG_F(1);
        cMtx_YrotS(*calc_mtx, playerActor->shape_angle.y);
        unkXyz1.x = 0.0f;
        unkXyz1.y = 0.0f;
        unkXyz1.z = -100.0f + JREG_F(7);
        MtxPosition(&unkXyz1, &unkXyz2);
        cLib_addCalc2(&actor->current.pos.x, playerActor->eyePos.x + unkXyz2.x, 0.5f, unkFloat0);
        cLib_addCalc2(&actor->current.pos.y, playerActor->eyePos.y, 0.5f, unkFloat0);
        cLib_addCalc2(&actor->current.pos.z, playerActor->eyePos.z + unkXyz2.z, 0.5f, unkFloat0);

        if (frame >= 10) {
            i_this->mSph1.SetC(i_this->field_0xe6c[0]);
            dComIfG_Ccsp()->Set(&i_this->mSph1);
        } else {
            i_this->mSph1.ClrAtHit();
        }

        if (i_this->mpMorfSO->isStop() && JREG_S(3) == 0) {
            i_this->field_0x67d = 2;
            i_this->field_0x6a4[1] = 0;
            i_this->field_0x6a4[3] = cM_rndF(100.0f) + 60.0f;
        }

        break;
    }

    if (unkFlag1) {
        cLib_addCalcAngleS2(&i_this->field_0x6c4, playerActor->shape_angle.y, 2, 0x2000);
        cMtx_YrotS(*calc_mtx, i_this->field_0x6c4);

        unkXyz1.x = 0.0f;
        unkXyz1.y = 200.0f;
        unkXyz1.z = 100.0f;
        MtxPosition(&unkXyz1, &unkXyz2);
        i_this->field_0x680 = playerActor->current.pos + unkXyz2;

        i_this->field_0x69c = NREG_F(4) + 1000.0f;
    }

    e_yr_pos_move(i_this);

    if ((i_this->field_0x6de & 0x3f) == 0 && !pl_horse_check(i_this)) {
        i_this->field_0x669 = 3;
        i_this->field_0x67d = 0;
    }
}

static void e_yr_wait_move(e_yr_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->mEnemy;

    fopAc_ac_c* playerActor = dComIfGp_getPlayer(0);
    daPy_py_c* player = (daPy_py_c*)playerActor;

    cXyz unkXyz1;
    cXyz unkXyz2;

    s8 unkChar1 = -1;

    f32 absSpeedX = fabsf(actor->speed.x);
    if (absSpeedX < 5.0f) {
        absSpeedX = 5.0f;
    }

    f32 absSpeedZ = fabsf(actor->speed.z);
    if (absSpeedZ < 5.0f) {
        absSpeedZ = 5.0f;
    }

    i_this->field_0xe94 = 2;

    f32 xDiff;
    f32 yDiff;
    f32 zDiff;
    f32 dist;
    f32 yOff;
    s32 old_0x6e0;
    switch (i_this->field_0x67d) {
    case -1:
        i_this->field_0x924 = 1;
        if (!i_this->mpMorfSO->isStop()) {
            break;
        }
        i_this->field_0x67d = 0;
    case 0:
        anm_init(i_this, 18, 1.0f, 2, 1.0f);

        i_this->field_0x67d = 1;
        i_this->field_0x6a4[0] = 100;
    case 1:
        i_this->field_0x924 = 1;

        cLib_addCalcAngleS2(&actor->current.angle.y, actor->home.angle.y, 5, 0x300);
        cLib_addCalcAngleS2(&actor->current.angle.x, 0, 5, 0x800);
        cLib_addCalcAngleS2(&actor->current.angle.z, 0, 5, 0x800);
        cLib_addCalc2(&actor->current.pos.x, actor->home.pos.x, 0.2f, absSpeedX);
        cLib_addCalc2(&actor->current.pos.z, actor->home.pos.z, 0.2f, absSpeedZ);

        actor->current.pos.y = actor->home.pos.y;

        if (i_this->field_0x6a4[0] == 0 && e_yr_player_view_check(i_this)) {
            i_this->field_0x67d = 2;
            i_this->field_0x6a4[0] = 30;

            anm_init(i_this, 7, 5.0f, 0, 1.0f);

            i_this->mSound.startCreatureVoice(Z2SE_EN_YR_V_FIND, -1);
        }

        break;
    case 2:
        i_this->field_0xe94 = 1;

        if (i_this->field_0x6a4[0] != 0) {
            break;
        }

        i_this->field_0x67d = 3;

        anm_init(i_this, 8, 5.0f, 2, 1.5f * l_e_yrHIO.mFlyPlaybackSpeed);

        i_this->field_0x6a4[0] = 70 + TREG_S(0);
    case 3:
        i_this->field_0xe94 = 1;

        cLib_addCalcAngleS2(&actor->current.angle.y, i_this->mYawToPlayer, 10, 0x200);

        unkXyz1.x = 0.0f;
        unkXyz1.y = TREG_F(0) * 10.0f + 300.0f;
        unkXyz1.z = TREG_F(1) * 10.0f + 300.0f;
        cMtx_YrotS(*calc_mtx, actor->current.angle.y);
        MtxPosition(&unkXyz1, &unkXyz2);
        cLib_addCalc2(&actor->current.pos.x, actor->home.pos.x + unkXyz2.x, 0.1f, 10.0f);
        cLib_addCalc2(&actor->current.pos.y, actor->home.pos.y + unkXyz2.y, 0.1f, 10.0f);
        cLib_addCalc2(&actor->current.pos.z, actor->home.pos.z + unkXyz2.z, 0.1f, 10.0f);

        if (i_this->field_0x6a4[0] == 0) {
            i_this->field_0x669 = 3;
            i_this->field_0x67d = 0;
        }

        break;
    case 10:
    case 11:
    case 12:
        unkChar1 = 0;

        i_this->field_0xe94 = 0;

        unkXyz1.x = 0.0f;
        unkXyz1.y = 0.0f;
        unkXyz1.z = 0.0f;
        cMtx_YrotS(*calc_mtx, actor->current.angle.y);
        MtxPosition(&unkXyz1, &unkXyz2);

        yOff = 80.0f;

        if (i_this->field_0x67d == 10) {
            yOff = 300.0f;
        }

        i_this->field_0x680.x = actor->home.pos.x + unkXyz2.x;
        i_this->field_0x680.y = actor->home.pos.y + yOff;
        i_this->field_0x680.z = actor->home.pos.z + unkXyz2.z;
        i_this->field_0x68c = 20.0f;
        i_this->field_0x690 = 5.0f;
        i_this->field_0x69c = 2000.0f;

        xDiff = i_this->field_0x680.x - actor->current.pos.x;
        yDiff = i_this->field_0x680.y - actor->current.pos.y;
        zDiff = i_this->field_0x680.z - actor->current.pos.z;
        dist = JMAFastSqrt(xDiff * xDiff + yDiff * yDiff + zDiff * zDiff);

        if (i_this->field_0x67d == 10) {
            if (dist < 2000.0f) {
                i_this->field_0x67d = 11;
            } else {
                if (e_yr_player_view_check(i_this)) {
                    i_this->field_0x669 = 3;
                    i_this->field_0x67d = 0;

                    i_this->mSound.startCreatureVoice(Z2SE_EN_YR_V_FIND, -1);
                }
            }
        } else if (i_this->field_0x67d == 11) {
            if (dist < (f32)l_e_yrHIO.mBrakeStartDist) {
                i_this->field_0x67d = 12;

                anm_init(i_this, 11, 10.0f, 0, 1.0f);
            }
        } else if (i_this->field_0x67d == 12) {
            i_this->field_0xeb0 = 1000.0f + KREG_F(11);

            if (dist < 10.0f * TREG_F(9) + 300.0f) {
                i_this->field_0x67d = 20;

                anm_init(i_this, 13, 5.0f, 2, l_e_yrHIO.field_0x44);

                i_this->field_0x67c = 10;
                actor->speed.y = 0.0f;
                i_this->field_0x6e0 = 0;
                unkChar1 = -1;
            }
        }

        break;
    case 20:
        i_this->field_0x924 = 1;
        i_this->field_0xe94 = 0;

        old_0x6e0 = i_this->field_0x6e0;
        i_this->field_0x6e0++;

        cLib_addCalc2(&actor->current.pos.x, actor->home.pos.x, 0.1f, absSpeedX);
        cLib_addCalc2(&actor->current.pos.z, actor->home.pos.z, 0.1f, absSpeedZ);

        actor->current.pos.y += actor->speed.y;

        if (old_0x6e0 > l_e_yrHIO.field_0x40) {
            actor->speed.y -= 0.8f + TREG_F(7);
        }

        cLib_addCalcAngleS2(&actor->current.angle.x, 0, 5, 0x800);
        cLib_addCalcAngleS2(&actor->current.angle.y, actor->home.angle.y, 5, 0x300);
        cLib_addCalcAngleS2(&actor->current.angle.z, 0, 5, 0x800);

        if (actor->current.pos.y <= actor->home.pos.y) {
            actor->current.pos.y = actor->home.pos.y;
            actor->speed.y = -0.5f;

            if (old_0x6e0 > l_e_yrHIO.field_0x40) {
                anm_init(i_this, 16, 5.0f, 0, l_e_yrHIO.field_0x48);

                i_this->field_0x67d = 0xff;
            }
        }

        break;
    }

    switch (unkChar1) {
    case 0:
        e_yr_pos_move(i_this);
        break;
    }
}

static void e_yr_su_wait_move(e_yr_class* i_this) {
    s32 old_0x6e0;

    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->mEnemy;
    fopAc_ac_c* playerActor = dComIfGp_getPlayer(0);
    daPy_py_c* player = (daPy_py_c*)playerActor;

    cXyz unkXyz1;
    cXyz unkXyz2;

    s8 unkChar1 = -1;

    f32 absSpeedX = fabsf(actor->speed.x);
    if (absSpeedX < 5.0f) {
        absSpeedX = 5.0f;
    }

    f32 absSpeedZ = fabsf(actor->speed.z);
    if (absSpeedZ < 5.0f) {
        absSpeedZ = 5.0f;
    }

    i_this->field_0xe94 = 2;

    f32 xDiff;
    f32 yDiff;
    f32 zDiff;
    f32 dist;
    f32 yOff;
    switch (i_this->field_0x67d) {
    case -1:
        i_this->field_0x924 = 1;

        if (!i_this->mpMorfSO->isStop()) {
            break;
        }

        i_this->field_0x67d = 0;
    case 0:
        anm_init(i_this, 18, 1.0f, 2, 1.0f);

        i_this->field_0x67d = 1;
        i_this->field_0x6a4[0] = 100;
    case 1:
        i_this->field_0x924 = 1;

        cLib_addCalcAngleS2(&actor->current.angle.y, actor->home.angle.y, 5, 0x300);
        cLib_addCalcAngleS2(&actor->current.angle.x, 0, 5, 0x800);
        cLib_addCalcAngleS2(&actor->current.angle.z, 0, 5, 0x800);
        cLib_addCalc2(&actor->current.pos.x, actor->home.pos.x, 0.2f, absSpeedX);
        cLib_addCalc2(&actor->current.pos.z, actor->home.pos.z, 0.2f, absSpeedZ);

        actor->current.pos.y = actor->home.pos.y;

        if (i_this->field_0x6a4[0] == 0 && e_yr_player_view_check(i_this)) {
            i_this->field_0x67d = 2;
            i_this->field_0x6a4[0] = 30;

            anm_init(i_this, 7, 5.0f, 0, 1.0f);

            i_this->mSound.startCreatureVoice(Z2SE_EN_YR_V_FIND, -1);
        }

        break;
    case 2:
        i_this->field_0xe94 = 1;

        if (i_this->field_0x6a4[0] != 0) {
            break;
        }

        i_this->field_0x67d = 3;

        anm_init(i_this, 8, 5.0f, 2, l_e_yrHIO.mFlyPlaybackSpeed * 1.5f);

        i_this->field_0x6a4[0] = TREG_S(0) + 0x46;
    case 3:
        i_this->field_0xe94 = 1;
        cLib_addCalcAngleS2(&actor->current.angle.y, i_this->mYawToPlayer, 10, 0x200);
        unkXyz1.x = 0.0f;
        unkXyz1.y = 10.0f * TREG_F(0) + 300.0f;
        unkXyz1.z = 10.0f * TREG_F(1) + 300.0f;
        cMtx_YrotS(*calc_mtx, actor->current.angle.y);
        MtxPosition(&unkXyz1, &unkXyz2);

        cLib_addCalc2(&actor->current.pos.x, actor->home.pos.x + unkXyz2.x, 0.1f, 10.0f);
        cLib_addCalc2(&actor->current.pos.y, actor->home.pos.y + unkXyz2.y, 0.1f, 10.0f);
        cLib_addCalc2(&actor->current.pos.z, actor->home.pos.z + unkXyz2.z, 0.1f, 10.0f);

        if (i_this->field_0x6a4[0] == 0) {
            i_this->field_0x669 = 3;
            i_this->field_0x67d = 0;
        }

        break;
    case 10:
    case 11:
    case 12:
        unkChar1 = 0;
        i_this->field_0xe94 = 0;
        unkXyz1.x = 0.0f;
        unkXyz1.y = 0.0f;
        unkXyz1.z = 0.0f;

        cMtx_YrotS(*calc_mtx, actor->current.angle.y);
        MtxPosition(&unkXyz1, &unkXyz2);

        yOff = 80.0f;
        if (i_this->field_0x67d == 10) {
            yOff = 300.0f;
        }

        i_this->field_0x680.x = actor->home.pos.x + unkXyz2.x;
        i_this->field_0x680.y = actor->home.pos.y + yOff;
        i_this->field_0x680.z = actor->home.pos.z + unkXyz2.z;

        i_this->field_0x68c = 20.0f;
        i_this->field_0x690 = 5.0f;
        i_this->field_0x69c = 2000.0f;

        xDiff = i_this->field_0x680.x - actor->current.pos.x;
        yDiff = i_this->field_0x680.y - actor->current.pos.y;
        zDiff = i_this->field_0x680.z - actor->current.pos.z;

        dist = JMAFastSqrt(xDiff * xDiff + yDiff * yDiff + zDiff * zDiff);

        if (i_this->field_0x67d == 10) {
            if (dist < 2000.0f) {
                i_this->field_0x67d = 11;
            }
        } else if (i_this->field_0x67d == 11) {
            if (dist < (f32)l_e_yrHIO.mBrakeStartDist) {
                i_this->field_0x67d = 12;

                anm_init(i_this, 11, 10.0f, 0, 1.0f);
            }
        } else if (i_this->field_0x67d == 12) {
            i_this->field_0xeb0 = 1000.0f + KREG_F(11);

            if (dist < 10.0f * TREG_F(9) + 300.0f) {
                i_this->field_0x67d = 20;

                anm_init(i_this, 13, 5.0f, 2, l_e_yrHIO.field_0x44);

                i_this->field_0x67c = 10;
                actor->speed.y = 0.0f;
                i_this->field_0x6e0 = 0;
                unkChar1 = -1;
            }
        }

        break;
    case 20:
        i_this->field_0x924 = 1;
        i_this->field_0xe94 = 0;

        old_0x6e0 = i_this->field_0x6e0;
        i_this->field_0x6e0++;

        cLib_addCalc2(&actor->current.pos.x, actor->home.pos.x, 0.1f, absSpeedX);
        cLib_addCalc2(&actor->current.pos.z, actor->home.pos.z, 0.1f, absSpeedZ);

        actor->current.pos.y += actor->speed.y;

        if (old_0x6e0 > l_e_yrHIO.field_0x40) {
            actor->speed.y -= 0.8f + TREG_F(7);
        }

        cLib_addCalcAngleS2(&actor->current.angle.x, 0, 5, 0x800);
        cLib_addCalcAngleS2(&actor->current.angle.y, actor->home.angle.y, 5, 0x300);
        cLib_addCalcAngleS2(&actor->current.angle.z, 0, 5, 0x800);

        if (actor->current.pos.y <= actor->home.pos.y) {
            actor->current.pos.y = actor->home.pos.y;
            actor->speed.y = -0.5f;

            if (old_0x6e0 > l_e_yrHIO.field_0x40) {
                anm_init(i_this, 16, 5.0f, 0, l_e_yrHIO.field_0x48);

                i_this->field_0x67d = 0xff;
            }
        }
        break;
    }
    switch (unkChar1) {
    case 0:
        e_yr_pos_move(i_this);
        break;
    }
}

f32 dummy() {
    return -50.0f;
}

static s8 e_yr_damage(e_yr_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->mEnemy;
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)actor;

    cXyz unkXyz1;

    i_this->field_0xe94 = 0;
    i_this->field_0x6d0 = 10;
    actor->speedF = 0.0f;

    cLib_addCalcAngleS2(&actor->current.angle.x, 0, 1, 0x800);

    if (enemy->checkCutDownHitFlg()) {
        enemy->offCutDownHitFlg();

        i_this->field_0x67d = 3;
        i_this->field_0x6a4[0] = 0x46;

        i_this->mSound.startCreatureVoice(Z2SE_EN_YR_V_DEATH, -1);

        enemy->offDownFlg();

        i_this->field_0xeb0 = 5000.0f + KREG_F(17);
        i_this->field_0xeb6 = i_this->field_0xeb8 = 0;
        i_this->field_0xeba = cM_rndF(400.0f);
        i_this->field_0xebc = cM_rndF(400.0f);
        i_this->field_0xec6 = 15 + TREG_S(7);
        actor->health = 0;
    }

    s8 retVal = 0;
    s8 unkFlag1 = 1;

    switch (i_this->field_0x67d) {
    case 0:
        anm_init(i_this, 6, 0.0f, 0, 1.0f);

        i_this->field_0x67d = 1;

        unkXyz1.x = 0.0f;
        unkXyz1.y = BREG_F(4) + 30.0f + cM_rndF(10.0f);
        unkXyz1.z = -i_this->field_0x6d8;
        cMtx_YrotS(*calc_mtx, i_this->field_0x6d2);
        MtxPosition(&unkXyz1, &actor->speed);

        i_this->field_0xebe = 8000;
        i_this->field_0x6a4[0] = cM_rndF(10.0f) + 30.0f;
        i_this->field_0x6a4[1] = 60;
    case 1:
        actor->current.pos += actor->speed;
        actor->speed.y += -3.0f;
        if (actor->speed.y < 0.0f) {
            actor->speed.x *= 0.96f;
            actor->speed.z *= 0.96f;

            cLib_addCalcAngleS2(&i_this->field_0x6f2.x, -0x4000, 4, 0x180 + BREG_S(7));
        }

        i_this->field_0xeb0 = 3000.0f + KREG_F(14);

        actor->current.angle.y = actor->current.angle.y + i_this->field_0x6dc;

        if (actor->health > 0) {
            if (i_this->field_0x6a4[0] == 0 || i_this->mAcch.ChkGroundHit()) {
                retVal = 1;
            }
        } else {
            unkFlag1 = 0;

            if (i_this->mAcch.ChkGroundHit() || i_this->field_0x6a4[1] == 0) {
                yr_disappear(i_this);
                fopAcM_delete(actor);

                return unkFlag1;
            }
        }
    }

    if (actor->speed.y < -30.0f) {
        actor->speed.y = -30.0f;
    }

    if (enemy->checkDownFlg()) {
        enemy->setDownPos(&actor->current.pos);
    }

    if (i_this->field_0xec6 != 0) {
        i_this->field_0xec6--;
        i_this->field_0xec4 = i_this->field_0xec6 *
                              cM_ssin(i_this->field_0xec6 * (TREG_S(9) + 0x3800)) *
                                  (100.0f + TREG_F(5));
    }

    if (retVal) {
        i_this->field_0x6d8 = 0.0f;
        i_this->field_0xebe = i_this->field_0xeb6 = i_this->field_0xeb8 = 0;
        i_this->field_0x669 = 3;
        i_this->field_0x67d = 0;

        i_this->field_0x6a4[1] = l_e_yrHIO.mAttackBMinTime +
                              cM_rndF(l_e_yrHIO.mAttackBMaxTime - l_e_yrHIO.mAttackBMinTime);
        i_this->field_0x6e9 = 0;
        i_this->field_0xec8 = 1;
        actor->current.angle.x = -0x3000;

        enemy->offDownFlg();
    }

    return unkFlag1;
}

static void damage_check(e_yr_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->mEnemy;

    fopAc_ac_c* unusedPlayer = dComIfGp_getPlayer(0);

    i_this->mStts.Move();

    if ((i_this->mSph3.ChkTgHit() || i_this->mSph2.ChkTgHit()) && i_this->field_0x6d0 == 0) {
        if (i_this->mSph3.ChkTgHit()) {
            i_this->mAtInfo.mpCollider = i_this->mSph3.GetTgHitObj();
        } else {
            if (i_this->mSph2.ChkTgHit()) {
                i_this->mAtInfo.mpCollider = i_this->mSph2.GetTgHitObj();
            }
        }

        s16 health = actor->health;

        i_this->mAtInfo.mpActor = cc_at_check(actor, &i_this->mAtInfo);

        if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_MIDNA_LOCK | AT_TYPE_10000000 |
                                                  AT_TYPE_WOLF_CUT_TURN | AT_TYPE_WOLF_ATTACK))
        {
            i_this->field_0x6d0 = 20;
        } else {
            i_this->field_0x6d0 = 10;
        }

        dKy_Sound_set(actor->current.pos, 100, fopAcM_GetID(i_this), 5);

#if DEBUG
        if (l_e_yrHIO.mInvulnerable) {
            actor->health = 40;
        }
#endif

        if (i_this->mAtInfo.mAttackPower == 0) {
            i_this->field_0x669 = 3;
            i_this->field_0x67d = 10;
            i_this->field_0x6a4[0] = l_e_yrHIO.mChanceTime;

            anm_init(i_this, 14, 0.0f, 0, l_e_yrHIO.field_0x38);

            i_this->field_0x68c = TREG_F(7);
            i_this->field_0x690 = 1.0f;
            actor->speedF = -20.0f;

            i_this->mSound.startCreatureVoice(Z2SE_EN_YR_V_DAMAGE_S, -1);
        } else {
            if (i_this->mAtInfo.mHitType == 1) {
                i_this->field_0x6d2 = i_this->mYawToPlayer;
                i_this->field_0x6d4 = i_this->mPitchToPlayer;
            } else {
                i_this->field_0x6d2 = i_this->mAtInfo.mHitDirection.y;
                i_this->field_0x6d4 = 0;
            }

            i_this->field_0x669 = 10;
            i_this->field_0x67d = 0;

            if (i_this->mAtInfo.mHitStatus) {
                i_this->field_0x6d8 = l_e_yrHIO.mLargeDamage;
            } else {
                i_this->field_0x6d8 = l_e_yrHIO.mSmallDamage;
            }

            if (cM_rndF(1.0f) < 0.5f) {
                i_this->field_0x6dc = cM_rndF(1000.0f) + 1000.0f;
            } else {
                i_this->field_0x6dc = -(cM_rndF(1000.0f) + 1000.0f);
            }

            if (actor->health <= 0) {
                i_this->mSound.startCreatureVoice(Z2SE_EN_YR_V_DEATH, -1);
            } else {
                i_this->mSound.startCreatureVoice(Z2SE_EN_YR_V_DAMAGE, -1);
            }
        }
    }
}

static void ground_angle_set(e_yr_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->mEnemy;

    J3DModel* model;

    cXyz unkXyz1;
    cXyz unkXyz2;
    Vec unkVec1;

    s16 unkShort1 = 0;
    s16 unkShort2 = 0;

    f32 xDiff;
    f32 yDiff;
    f32 zDiff;

    if (i_this->field_0x6f2.x <= -0x1000) {
        cXyz unkXyz3;
        cXyz unkXyz4;
        dBgS_GndChk gndChk;
        f32 unkFloat1 = 75.0f;

        model = i_this->mpMorfSO->getModel();
        MTXCopy(model->getAnmMtx(0), *calc_mtx);
        unkXyz1.set(0.0f, 0.0f, 0.0f);
        MtxPosition(&unkXyz1, &unkXyz3);

        unkXyz3.y += 100.0f;
        unkXyz3.y += 100.0f;
        gndChk.SetPos(&unkXyz3);

        unkXyz3.y = dComIfG_Bgsp().GroundCross(&gndChk);
        if (unkXyz3.y != -G_CM3D_F_INF) {
            unkVec1.x = unkXyz3.x;
            unkVec1.y = unkXyz3.y + 100.0f;
            unkVec1.z = unkXyz3.z + unkFloat1;
            gndChk.SetPos(&unkVec1);

            unkVec1.y = dComIfG_Bgsp().GroundCross(&gndChk);
            if (unkVec1.y != -G_CM3D_F_INF) {
                yDiff = unkVec1.y - unkXyz3.y;
                zDiff = unkVec1.z - unkXyz3.z;
                unkShort2 = -cM_atan2s(yDiff, zDiff);
                if (unkShort2 > 0x3000 || unkShort2 < -0x3000) {
                    unkShort2 = 0;
                }
            }
            unkVec1.x = unkXyz3.x + unkFloat1;
            unkVec1.y = unkXyz3.y + 100.0f;
            unkVec1.z = unkXyz3.z;
            gndChk.SetPos(&unkVec1);
            unkVec1.y = dComIfG_Bgsp().GroundCross(&gndChk);
            if (unkVec1.y != -G_CM3D_F_INF) {
                yDiff = unkVec1.y - unkXyz3.y;
                xDiff = unkVec1.x - unkXyz3.x;
                unkShort1 = (s16)cM_atan2s(yDiff, xDiff);
                if (unkShort1 > 0x3000 || unkShort1 < -0x3000)
                {
                    unkShort1 = 0;
                }
            }
        }

        dBgS_LinChk linChk;
        cXyz unkXyz5;
        cXyz unkXyz6;
        cXyz unkXyz7;

        unkXyz6 = actor->current.pos;
        unkXyz6.y += 30.0f + JREG_F(5);

        model = i_this->mpMorfSO->getModel();
        unkXyz5.set(0.0f, 0.0f, 0.0f);
        MTXCopy(model->getAnmMtx(5), *calc_mtx);

        MtxPosition(&unkXyz5, &unkXyz7);
        unkXyz7.y += JREG_F(6) + 30.0f;

        linChk.Set(&unkXyz6, &unkXyz7, actor);

        if (dComIfG_Bgsp().LineCross(&linChk)) {
            unkXyz5 = unkXyz6 - unkXyz7;
            cMtx_YrotS(*calc_mtx, cM_atan2s(unkXyz5.x, unkXyz5.z));

            unkXyz5.x = 0.0f;
            unkXyz5.y = 0.0f;
            unkXyz5.z = TREG_F(11) + 50.0f;
            MtxPosition(&unkXyz5, &unkXyz2);

            actor->current.pos += unkXyz2;
        }
    }

    cLib_addCalcAngleS2(&i_this->field_0x6f8.x, unkShort2, 1, 0x400);
    cLib_addCalcAngleS2(&i_this->field_0x6f8.z, unkShort1, 1, 0x400);
}

static int daE_Yr_Execute(e_yr_class* i_this) {
    static s32 wing_j[4] = {12, 13, 17, 18};

    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->mEnemy;

    fopAc_ac_c* playerActor = dComIfGp_getPlayer(0);
    daPy_py_c* player = (daPy_py_c*)playerActor;

    cXyz unkXyz1;
    cXyz unkXyz2;

    f32 xDiff = playerActor->current.pos.x - actor->current.pos.x;
    f32 yDiff = (100.0f + playerActor->current.pos.y) - actor->current.pos.y;
    f32 zDiff = playerActor->current.pos.z - actor->current.pos.z;

    i_this->mPlayerLatDist = JMAFastSqrt(xDiff * xDiff + zDiff * zDiff);
    i_this->mYawToPlayer = cM_atan2s(xDiff, zDiff);
    i_this->mPitchToPlayer = -cM_atan2s(yDiff, i_this->mPlayerLatDist);

    i_this->field_0x6de++;

    if (i_this->field_0x67e != 0) {
        if (dComIfGs_isSwitch(i_this->field_0x67e - 1, fopAcM_GetRoomNo(actor))) {
            i_this->field_0x67e = 0;
        } else {
            return 1;
        }
    }

    actor->attention_info.flags = fopAc_AttnFlag_BATTLE_e;

    fopAcM_OnStatus(actor, 0);

    s8 unkFlag1 = 1;

    if (l_e_yrHIO.mStationary == 0) {

        for (s32 i = 0; i < 6; i++) {
            if (i_this->field_0x6a4[i] != 0) {
                i_this->field_0x6a4[i]--;
            }
        }

        if (i_this->field_0x6b2 != 0) {
            i_this->field_0x6b2--;
        }

        if (i_this->field_0x6d0 != 0) {
            i_this->field_0x6d0--;
        }

        i_this->mpMorfSO->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));

        if (i_this->field_0x5b8 == 12) {
            i_this->mSound.startCreatureSoundLevel(Z2SE_EN_YR_GLIDE, 0, -1);
        } else if (i_this->field_0x5b8 == 15) {
            i_this->mSound.startCreatureSoundLevel(Z2SE_EN_YR_V_HOVERING, 0, -1);
            if (i_this->mpMorfSO->checkFrame(12.0f)) {
                i_this->mSound.startCreatureSound(Z2SE_EN_YR_WING, 0, -1);
            }
        } else if (i_this->mpMorfSO->checkFrame(27.0f) && i_this->field_0x5b8 == 8) {
            i_this->mSound.startCreatureSound(Z2SE_EN_YR_WING, 0, -1);
        } else if (i_this->mpMorfSO->checkFrame(11.0f) && i_this->field_0x5b8 == 13) {
            i_this->mSound.startCreatureSound(Z2SE_EN_YR_WING, 0, -1);
        } else if (i_this->mpMorfSO->checkFrame(12.0f) && i_this->field_0x5b8 == 15) {
            i_this->mSound.startCreatureSound(Z2SE_EN_YR_WING, 0, -1);
        }

        if (i_this->field_0x669 == 10) {
            unkFlag1 = e_yr_damage(i_this);
        } else {
            actor->attention_info.distances[fopAc_attn_BATTLE_e] = 3;
            s8 unkFlag2 = 0;
            if (i_this->field_0x6e9 != 0) {
                e_yr_path_move(i_this);
            } else {
                if (i_this->field_0x669 == 9) {
                    actor->attention_info.distances[fopAc_attn_BATTLE_e] = 22;

                    e_yr_horse_move(i_this);

                    unkFlag2 = 1;
                } else if (i_this->field_0x669 == 3) {
                    e_yr_atack_move(i_this);

                    unkFlag2 = 1;
                } else if (i_this->field_0x669 == 4) {
                    e_yr_wait_move(i_this);
                } else if (i_this->field_0x669 == 7) {
                    e_yr_su_wait_move(i_this);
                } else {
                    e_yr_auto_move(i_this);
                }
            }

            if (unkFlag2 != 0) {
                i_this->mSound.setLinkSearch(true);
            } else {
                i_this->mSound.setLinkSearch(false);
            }

            cLib_addCalcAngleS2(&i_this->field_0x6f2.x, 0, 4, 0x800);
        }
    }
    if (unkFlag1 == 0) {
        fopAcM_OffStatus(actor, 0);
        actor->attention_info.flags = 0;
    }

    actor->shape_angle = actor->current.angle + i_this->field_0x6f2;

    ground_angle_set(i_this);

    MtxTrans(actor->current.pos.x, actor->current.pos.y, actor->current.pos.z, 0);
    cMtx_XrotM(*calc_mtx, i_this->field_0x6f8.x);
    cMtx_ZrotM(*calc_mtx, i_this->field_0x6f8.z);
    cMtx_YrotM(*calc_mtx, actor->shape_angle.y);
    cMtx_XrotM(*calc_mtx, actor->shape_angle.x);
    cMtx_ZrotM(*calc_mtx, actor->shape_angle.z);
    MtxScale(l_e_yrHIO.mModelScale, l_e_yrHIO.mModelScale, l_e_yrHIO.mModelScale, 1);

    J3DModel* model = i_this->mpMorfSO->getModel();
    model->setBaseScale(actor->scale);
    model->setBaseTRMtx(*calc_mtx);
    i_this->mpMorfSO->modelCalc();

    if (i_this->field_0xf1c == 0) {
        static u16 e_name[5] = {0x843A, 0x843B, 0x843C, 0x843D, 0x843E};
        static s32 wing_j[4] = {11, 13, 16, 18};

        for (s32 i = 0; i < 5; i++) {
            i_this->mEmitterIds[i] = dComIfGp_particle_set(i_this->mEmitterIds[i], e_name[i], &actor->current.pos, NULL, NULL);
            if (i != 4) {
                JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->mEmitterIds[i]);
                if (emitter != NULL) {
                    emitter->setGlobalRTMatrix(model->getAnmMtx(wing_j[i]));
                }
            }
        }
    }

    if (i_this->field_0x5b8 == 4 && i_this->mpMorfSO->checkFrame(1.0f)) {
        dComIfGp_particle_set(0x8435, &actor->current.pos, NULL, NULL);
    }

    if (i_this->field_0x5b8 == 6) {
        static u16 e_name[2] = {0x8444, 0x8445};
        static s32 wing_j[2] = {12, 17};

        i_this->field_0xf1c = 1;
        for (s32 i = 0; i < 2; i++) {
            i_this->mEmitterIds[i + 5] = dComIfGp_particle_set(i_this->mEmitterIds[i + 5], e_name[i], &actor->current.pos, NULL, NULL);
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->mEmitterIds[i + 5]);
            if (emitter != NULL) {
                emitter->setGlobalRTMatrix(model->getAnmMtx(wing_j[i]));
            }
        }
    } else {
        i_this->field_0xf1c = 0;
    }

    MTXCopy(model->getAnmMtx(6), *calc_mtx);
    unkXyz1.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&unkXyz1, &actor->eyePos);
    actor->attention_info.position = actor->eyePos;
    actor->attention_info.position.y += 50.0f + 10.0f * TREG_F(8);
    MTXCopy(model->getAnmMtx(22), *calc_mtx);
    MtxPosition(&unkXyz1, &i_this->field_0xe6c[0]);
    MTXCopy(model->getAnmMtx(27), *calc_mtx);
    MtxPosition(&unkXyz1, &i_this->field_0xe6c[1]);

    if (i_this->field_0x66b != 0) {
        fopAc_ac_c* unkActor1;
        switch (i_this->field_0x66b) {
        case 1: {
            i_this->field_0x66b = 2;

            break;
        }
        case 2: {
            if ((unkActor1 = fopAcM_SearchByID(i_this->field_0x674)) != 0) {
                i_this->field_0x670 = unkActor1;
                i_this->field_0x66b = 3;
            }

            break;
        }
        case 3: {
            unkActor1 = i_this->field_0x670;

            cXyz unkXyz3;
            unkXyz3.x = i_this->field_0xe6c[0].x +
                     (i_this->field_0xe6c[1].x - i_this->field_0xe6c[0].x) / 2;
            unkXyz3.y = i_this->field_0xe6c[0].y +
                     (i_this->field_0xe6c[1].y - i_this->field_0xe6c[0].y) / 2;
            unkXyz3.z = i_this->field_0xe6c[0].z +
                     (i_this->field_0xe6c[1].z - i_this->field_0xe6c[0].z) / 2;
            unkActor1->current.pos = unkXyz3;

            unkActor1->current.angle = actor->current.angle;
            unkActor1->shape_angle = actor->current.angle;

            break;
        }
        }
    }

    if (i_this->field_0x6d0 != 0) {
        cXyz unkXyz4(actor->eyePos.x - 10000.0f, 10000.0f, 200000.0f);
        i_this->mSph2.SetC(unkXyz4);
        i_this->mSph3.SetC(unkXyz4);
        i_this->mSph4.SetC(unkXyz4);
    } else {
        i_this->mSph2.SetC(actor->eyePos);
        i_this->mSph3.SetC(actor->current.pos);
        i_this->mSph4.SetC(actor->current.pos);
    }

    dComIfG_Ccsp()->Set(&i_this->mSph2);
    dComIfG_Ccsp()->Set(&i_this->mSph3);
    dComIfG_Ccsp()->Set(&i_this->mSph4);

    cXyz* unkXyzP = i_this->mStts.GetCCMoveP();
    if (unkXyzP != NULL) {
        f32 temp_f28 = 0.3f + TREG_F(19);
        actor->current.pos.x += unkXyzP->x * temp_f28;
#if DEBUG
        actor->current.pos.y += unkXyzP->y * temp_f28;
#endif
        actor->current.pos.z += unkXyzP->z * temp_f28;
    }

    if (i_this->field_0x924 == 0 || fabsf(i_this->field_0x6d8) > 0.01f) {

        f32 unkFloat1 = 70.0f + TREG_F(15);
        if (i_this->field_0x669 == 10) {
            unkFloat1 = 5.0f + TREG_F(14);
        }

#if DEBUG
        actor->current.pos.y -= unkFloat1;
        actor->old.pos.y -= unkFloat1;
#else
        cLib_addCalc2(&i_this->field_0x928, unkFloat1, 1.0f, 2.0f);
        actor->current.pos.y -= i_this->field_0x928;
        actor->old.pos.y -= i_this->field_0x928;
#endif
        i_this->mAcch.CrrPos(dComIfG_Bgsp());
#if DEBUG
        actor->current.pos.y += unkFloat1;
        actor->old.pos.y += unkFloat1;
#else
        actor->current.pos.y += i_this->field_0x928;
        actor->old.pos.y += i_this->field_0x928;
#endif
    }

    i_this->field_0x924 = 0;

    damage_check(i_this);

    s16 unkShort1 = 0x800;
    if (i_this->field_0xe94 != 0) {
        if (i_this->field_0xe94 == 1) {
            xDiff = playerActor->current.pos.x - actor->current.pos.x;
            zDiff = playerActor->current.pos.z - actor->current.pos.z;
            s16 yaw = cM_atan2s(xDiff, zDiff);
            i_this->field_0xe86 = actor->current.angle.y - yaw;
            if (i_this->field_0xe86 > 10000) {
                i_this->field_0xe86 = 10000;
            } else if (i_this->field_0xe86 < -10000) {
                i_this->field_0xe86 = -10000;
            }
        } else if (i_this->field_0xe94 == 2) {
            unkShort1 = 0x200;

            if ((i_this->field_0x6de & 0x1f) == 0 && cM_rndF(1.0f) < 0.5f) {
                i_this->field_0xe86 = cM_rndFX(10000.0f);
            }
            if ((i_this->field_0x6de & 0x3f) == 0 && cM_rndF(1.0f) < 0.5f) {
                kuti_open(i_this, 25, Z2SE_EN_YR_V_NAKU);
            }
        }

        i_this->field_0xe94 = 0;
    } else {
        i_this->field_0xe86 = 0;
    }

    cLib_addCalcAngleS2(&i_this->field_0xe84, -(i_this->field_0xe86 / 2), 2, unkShort1);

    s16 unkShort2 = 0;
    if (i_this->field_0xe8a != 0) {
        i_this->field_0xe8a--;
        unkShort2 = 10000;
        if (i_this->field_0xe8a == i_this->field_0xe8c && i_this->field_0xe90 != Z2SE_SY_DUMMY) {
            i_this->mSound.startCreatureVoice(i_this->field_0xe90, -1);
        }
    }

    cLib_addCalcAngleS2(&i_this->field_0xe88, unkShort2, 2, 2000);

    for (s32 i = 0; i <= 8; i++) {
        i_this->field_0xe96[i] = i_this->field_0xeb0 *
                     cM_ssin(i_this->field_0x6de * (7000 + KREG_S(2)) + i * (11000 + KREG_S(3)));
    }

    cLib_addCalc0(&i_this->field_0xeb0, 1.0f, 200.0f + TREG_F(18));

    s16 unkShort3;
    s16 unkShort4;
    if (i_this->field_0x6d8 > 0.01f) {
        s16 unkShort5 = (s16)i_this->field_0x6d2;
        s16 unkShort6 = unkShort5 - actor->shape_angle.y;
        if (i_this->field_0x6f2.x <= -0x1000) {
            unkShort6 = 0;
        }

        unkShort3 = -unkShort6 + 0x4000;
        if (unkShort3 > 15000) {
            unkShort3 = 15000;
        } else if (unkShort3 < -15000) {
            unkShort3 = -15000;
        }

        unkShort4 = unkShort6 + 0x4000;
        if (unkShort4 > 15000) {
            unkShort4 = 15000;
        } else if (unkShort4 < -15000) {
            unkShort4 = -15000;
        }
    } else {
        unkShort3 = unkShort4 = 0;
    }

    cLib_addCalcAngleS2(&i_this->field_0xea8, unkShort3, 4, 0x800);
    cLib_addCalcAngleS2(&i_this->field_0xeaa, unkShort4, 4, 0x800);

    i_this->field_0xeb4 = i_this->field_0xec0 * cM_ssin(i_this->field_0x6de * (7000 + NREG_S(2)));

    cLib_addCalc0(&i_this->field_0xec0, 1.0f, 300.0f + NREG_F(18));

    cXyz midnaBindEffSize(1.3f, 1.3f, 1.3f);
    setMidnaBindEffect((fopEn_enemy_c*)actor, &i_this->mSound, &actor->eyePos, &midnaBindEffSize);

    return 1;
}

static int daE_Yr_IsDelete(e_yr_class* i_this) {
    return 1;
}

static int daE_Yr_Delete(e_yr_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->mEnemy;

    dComIfG_resDelete(&i_this->mPhaseReq, "E_Yr");

#if DEBUG
    l_e_yrHIO.removeHIO(&i_this->mEnemy);
#endif

    if (actor->heap != NULL) {
        i_this->mpMorfSO->stopZelAnime();
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_yr_class* yr = (e_yr_class*)i_this;

    yr->mpMorfSO = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_Yr", 21), NULL, NULL,
                                        (J3DAnmTransform*)dComIfG_getObjectRes("E_Yr", 12), 2, 1.0f,
                                        0, -1, &yr->mSound, 0x80000, 0x11000084);

    if (yr->mpMorfSO == NULL ||
        yr->mpMorfSO->getModel() == NULL)
    {
        return 0;
    }

    J3DModel* model = yr->mpMorfSO->getModel();
    model->setUserArea((int)yr);

    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        if (i != 0) {
            model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
        }
    }

    return 1;
}

static int daE_Yr_Create(fopAc_ac_c* i_this) {
    static dCcD_SrcSph at_sph_src = {
        {
            {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0xd}, {0x0, 0x0}, 0x0}}, // mObj
            {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x1}, // mGObjAt
            {dCcD_SE_HARD_BODY, 0x0, 0x0, 0x0, 0x0}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 25.0f} // mSph
        } // mSphAttr
    };

    static dCcD_SrcSph head_tg_sph_src = {
        {
            {0x0, {{0x0, 0x1, 0x0}, {(s32)0xd8fbfdff, 0x3}, 0x0}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 62.5f} // mSph
        } // mSphAttr
    };

    static dCcD_SrcSph body_tg_sph_src = {
        {
            {0x0, {{0x0, 0x1, 0x0}, {(s32)0xd8fbfdff, 0x3}, 0x0}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 65.0f} // mSph
        } // mSphAttr
    };

    static dCcD_SrcSph body_co_sph_src = {
        {
            {0x0, {{0x0, 0x1, 0x0}, {0x0, 0x0}, 0x75}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 80.0f} // mSph
        } // mSphAttr
    };

    e_yr_class* yr = (e_yr_class*)i_this;

    s32 loadResult = dComIfG_resLoad(&yr->mPhaseReq, "E_Yr");
    fopAcM_ct(i_this, e_yr_class);

    if (loadResult == cPhs_COMPLEATE_e) {
        // "It won't reset because it's already been defeated"
        OS_REPORT("PARAM %x\n", fopAcM_GetParam(i_this));

        yr->field_0x664 = fopAcM_GetParam(i_this);
        yr->field_0x669 = yr->field_0x664;
        yr->field_0x665 = (fopAcM_GetParam(i_this) & 0xff00) >> 8;
        yr->field_0x666 = (fopAcM_GetParam(i_this) & 0xff0000) >> 16;
        yr->field_0x667 = (fopAcM_GetParam(i_this) & 0xff000000) >> 24;

        yr->field_0x668 = i_this->home.angle.z;
        i_this->current.angle.z = 0;
        i_this->current.angle.x = 0;

        if (yr->field_0x668 != 0) {
            if (dComIfGs_isSwitch(yr->field_0x668, fopAcM_GetRoomNo(i_this))) {
                // "It won't reset because it's already been defeated"
                OS_REPORT("E_YR やられ後なので再セットしません\n");
                return cPhs_ERROR_e;
            }
        }

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x2340)) {
            return cPhs_ERROR_e;
        }

        i_this->current.pos.y += 70.0f + TREG_F(5);
        i_this->home.pos = i_this->current.pos;

        if (yr->field_0x666 != 0xff) {
            yr->field_0x6ec = dPath_GetRoomPath(yr->field_0x666, fopAcM_GetRoomNo(i_this));
            if (yr->field_0x6ec == 0) {
                return cPhs_ERROR_e;
            }

            yr->field_0x6e9 = yr->field_0x666 + 1;
            yr->field_0x6eb = 1;
        }

        if (yr->field_0x667 != 0xff) {
            yr->field_0x67e = yr->field_0x667 + 1;
        }

        if (yr->field_0x669 == 5 || yr->field_0x669 == 6) {
            yr->field_0x66b = 1;
        } else if (yr->field_0x664 == 3) {
            yr->field_0x669 = 3;
        }

        fopAcM_SetMtx(i_this, yr->mpMorfSO->getModel()->getBaseTRMtx());
        fopAcM_SetMin(i_this, -300.0f, -200.0f, -300.0f);
        fopAcM_SetMax(i_this, 300.0f, 200.0f, 300.0f);

        yr->mAcch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1,
                      &yr->mAcchCir, fopAcM_GetSpeed_p(i_this), NULL, NULL);
        yr->mAcchCir.SetWall(100.0f, 100.0f);

        i_this->field_0x560 = i_this->health = 0x50;

        yr->mStts.Init(100, 0xff, i_this);
        yr->mSph1.Set(at_sph_src);
        yr->mSph2.Set(head_tg_sph_src);
        yr->mSph3.Set(body_tg_sph_src);
        yr->mSph4.Set(body_co_sph_src);
        yr->mSph1.SetStts(&yr->mStts);
        yr->mSph2.SetStts(&yr->mStts);
        yr->mSph3.SetStts(&yr->mStts);
        yr->mSph4.SetStts(&yr->mStts);

        yr->field_0x6a4[3] = cM_rndF(200.0f) + 300.0f;
        yr->field_0x6de = cM_rndF(10000.0f);
        i_this->attention_info.distances[fopAc_attn_BATTLE_e] = 22;

#if DEBUG
        // "Dark Kargarok"
        l_e_yrHIO.entryHIO("闇カーゴロック");
#endif

        yr->mSound.init(&i_this->current.pos, &i_this->eyePos, 3, 1);
        yr->mSound.setEnemyName("E_yr");
        yr->mAtInfo.mPowerType = 3;
        yr->mAtInfo.mpSound = &yr->mSound;

        daE_Yr_Execute(yr);
    }

    return loadResult;
}

static actor_method_class l_daE_Yr_Method = {
    (process_method_func)daE_Yr_Create,
    (process_method_func)daE_Yr_Delete,
    (process_method_func)daE_Yr_Execute,
    (process_method_func)daE_Yr_IsDelete,
    (process_method_func)daE_Yr_Draw,
};

extern actor_process_profile_definition g_profile_E_YR = {
    (u32)fpcLy_CURRENT_e,   // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_E_YR,              // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(e_yr_class),     // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    189,                    // mPriority
    &l_daE_Yr_Method,       // sub_method
    0x10040100,             // mStatus
    fopAc_ENEMY_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};

e_yr_class::e_yr_class() {
}
