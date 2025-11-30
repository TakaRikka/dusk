/**
* @file d_a_e_kr.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_kr.h"
#include "d/actor/d_a_horse.h"
#include "d/d_bomb.h"
#include "d/d_cc_d.h"
#include "d/d_cc_uty.h"
#include "f_op/f_op_actor_enemy.h"

class e_krHIO_c : public fOpAcm_HIO_entry_c {
public:
    e_krHIO_c() {
        field_0x4 = 0;
        field_0x5 = 0;
        field_0x8 = 20.0f;
        field_0xc = 2500.0f;
        field_0x14 = 1.0f;
        field_0x18 = 2000;
        field_0x1a = 2500;
        field_0x1c = 2000;
        field_0x1e = 2500;
        field_0x20 = 0.35f;
        field_0x24 = 0.5f;
        field_0x28 = 1.0f;
        field_0x2c = 1.0f;
        field_0x30 = 1.0f;
        field_0x38 = 1.0f;
        field_0x3c = 1.0f;
        field_0x40 = 30;
        field_0x44 = 1.0f;
        field_0x48 = 1.0f;
        field_0x4c = 1.5f;
        field_0x50 = 1100;
        field_0x54 = 3.5f;
        field_0x34 = 60;
        field_0x58 = 60.0f;
        field_0x5c = 40.0f;
        field_0x60 = 1;
        field_0x64 = 8000.0f;
        field_0x70 = 3000.0f;
        field_0x6c = 20000;
        field_0x68 = 1500.0f;
        field_0x74 = 2000.0f;
        field_0x6e = 20000;
        field_0x78 = -40.0f;
    }
    virtual ~e_krHIO_c() {}
    void genMessage(JORMContext*);

    /* 0x04 */ u8 field_0x4;
    /* 0x05 */ u8 field_0x5;
    /* 0x06 */ u8 field_0x6;
    /* 0x08 */ f32 field_0x8;
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ u8 field_0x10[0x14 - 0x10];
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ s16 field_0x18;
    /* 0x1A */ s16 field_0x1a;
    /* 0x1C */ s16 field_0x1c;
    /* 0x1E */ s16 field_0x1e;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2c;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ s16 field_0x34;
    /* 0x38 */ f32 field_0x38;
    /* 0x3C */ f32 field_0x3c;
    /* 0x40 */ s16 field_0x40;
    /* 0x44 */ f32 field_0x44;
    /* 0x48 */ f32 field_0x48;
    /* 0x4C */ f32 field_0x4c;
    /* 0x50 */ s16 field_0x50;
    /* 0x54 */ f32 field_0x54;
    /* 0x58 */ f32 field_0x58;
    /* 0x5C */ f32 field_0x5c;
    /* 0x60 */ s16 field_0x60;
    /* 0x64 */ f32 field_0x64;
    /* 0x68 */ f32 field_0x68;
    /* 0x6C */ s16 field_0x6c;
    /* 0x6E */ s16 field_0x6e;
    /* 0x70 */ f32 field_0x70;
    /* 0x74 */ f32 field_0x74;
    /* 0x78 */ f32 field_0x78;
};

STATIC_ASSERT(sizeof(e_krHIO_c) == 0x7c);

static e_krHIO_c l_e_krHIO;

static int nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        J3DJoint* joint = i_joint;
        s32 jointNo = joint->getJntNo();
        J3DModel* model = j3dSys.getModel();

        e_kr_class* kr = (e_kr_class*)model->getUserArea();
        e_kr_class* unused = kr;  // debug match
        if (kr != NULL) {
            MTXCopy(model->getAnmMtx(jointNo), *calc_mtx);
            if (jointNo == 3 || jointNo == 4) {
                cMtx_YrotM(*calc_mtx, kr->field_0xe7c / 2);
                cMtx_XrotM(*calc_mtx, kr->field_0xe7c);
                cMtx_ZrotM(*calc_mtx, kr->field_0xe80 / 3);
            } else if (jointNo == 8) {
                cMtx_ZrotM(*calc_mtx, -(kr->field_0xe80 / 3));
            } else if (jointNo == 7) {
                cMtx_ZrotM(*calc_mtx, -kr->field_0xe80);
            }

            if (jointNo >= 10 && jointNo <= 13) {
                cMtx_ZrotM(*calc_mtx, kr->field_0xe8e[9] / 2);
                cMtx_XrotM(*calc_mtx, kr->field_0xe8e[jointNo - 9]);
            } else {
                if (jointNo >= 15 && jointNo <= 18) {
                    cMtx_ZrotM(*calc_mtx, kr->field_0xe8e[10] / 2);
                    cMtx_XrotM(*calc_mtx, kr->field_0xe8e[jointNo - 9]);
                }
            }

            if (jointNo == 3) {
                cMtx_ZrotM(*calc_mtx, kr->field_0xeac);
            } else if (jointNo == 19) {
                cMtx_ZrotM(*calc_mtx, -kr->field_0xeac);
            } else if (jointNo == 20) {
                cMtx_ZrotM(*calc_mtx, -kr->field_0xeae);
            } else if (jointNo == 25) {
                cMtx_ZrotM(*calc_mtx, -kr->field_0xeb0);
            } else if ((jointNo == 30) || (jointNo == 31)) {
                cMtx_YrotM(*calc_mtx, (s16)kr->field_0xeb6);
                cMtx_ZrotM(*calc_mtx, (s16)kr->field_0xeb6);
            }

            if (kr->field_0xebc != 0) {
                if ((jointNo & 1) != 0) {
                    cMtx_YrotM(*calc_mtx, kr->field_0xebc);
                } else if ((jointNo & 2) != 0) {
                    cMtx_XrotM(*calc_mtx, kr->field_0xebc);
                } else if ((jointNo & 4) != 0) {
                    cMtx_ZrotM(*calc_mtx, kr->field_0xebc);
                }
            }

            model->setAnmMtx(jointNo, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }

    return 1;
}

static void anm_init(e_kr_class* i_this, int i_resIndex, f32 i_morf, u8 i_attr, f32 i_rate) {
    i_this->mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_kr", i_resIndex), i_attr,
                           i_morf, i_rate, 0.0f, -1.0f);
    i_this->field_0x5b8 = i_resIndex;
}

static void kuti_open(e_kr_class* i_this, s16 param_1, u32 param_2) {
    if (i_this->field_0xe82 == 0) {
        i_this->field_0xe82 = param_1;
        i_this->field_0xe84 = param_1 - 5;
        i_this->field_0xe88 = param_2;
    }
}

static BOOL e_kr_player_bg_check(e_kr_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    dBgS_LinChk linChk;
    cXyz unkXyz1;
    cXyz unkXyz2;
    cXyz unkXyz3;

    unkXyz3 = player->current.pos;
    unkXyz3.y = player->eyePos.y;
    unkXyz2 = actor->current.pos;
    unkXyz2.y += 100.0f;
    linChk.Set(&unkXyz2, &unkXyz3, actor);
    if (dComIfG_Bgsp().LineCross(&linChk)) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static BOOL e_kr_setpos_bg_check(e_kr_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    dBgS_LinChk linChk;
    cXyz unkXyz1;
    cXyz unkXyz2;
    cXyz unkXyz3;

    unkXyz3 = actor->home.pos;
    unkXyz3.y += 100.0f;
    unkXyz2 = actor->eyePos;
    linChk.Set(&unkXyz2, &unkXyz3, actor);
    if (dComIfG_Bgsp().LineCross(&linChk)) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static BOOL e_kr_player_view_check(e_kr_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    if (l_e_krHIO.field_0x6 != 0) {
        return TRUE;
    }
    if (i_this->field_0x665 != 0xff) {
        if (i_this->field_0x6c4 > i_this->field_0x665 * 100.0f ||
            e_kr_player_bg_check(i_this) != 0)
        {
            return FALSE;
        }
    } else {
        if (i_this->mCurAction == 4 || i_this->mCurAction == 7) {
            if (i_this->field_0x6c4 > l_e_krHIO.field_0x68 || e_kr_player_bg_check(i_this) != 0) {
                return FALSE;
            }
        } else if (i_this->field_0x6c4 > l_e_krHIO.field_0x64 ||
                   e_kr_player_bg_check(i_this) != 0)
        {
            return FALSE;
        }
    }

    f32 unkFloat1;
    s16 unkInt1;
    if (i_this->mCurAction == 4 || i_this->mCurAction == 7) {
        unkInt1 = l_e_krHIO.field_0x6e;
        unkFloat1 = l_e_krHIO.field_0x74;
    } else {
        unkInt1 = l_e_krHIO.field_0x6c;
        unkFloat1 = l_e_krHIO.field_0x70;
    }

    f32 unkFloat2 = fabsf(player->eyePos.y + 100.0f - actor->eyePos.y);
    if (unkFloat2 < unkFloat1) {
        s16 unkShort1 = actor->current.angle.y - i_this->field_0xe7c - i_this->field_0x6c0;
        if (unkShort1 < 0) {
            unkShort1 = unkShort1 * -1;
        }
        if ((u16)unkShort1 < unkInt1) {
            return true;
        }
    }
    return false;
}

static BOOL pl_horse_check(e_kr_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    if (daPy_getPlayerActorClass()->checkHorseRide() && dComIfGp_getHorseActor()->speedF > 20.0f) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static void daE_Kr_shadowDraw(e_kr_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    J3DModel* model = i_this->mpMorf->getModel();
    cXyz unkXyz1;
    unkXyz1.set(actor->current.pos.x, actor->current.pos.y + 150.0f + BREG_F(0x12),
                  actor->current.pos.z);
    i_this->field_0x5bc = dComIfGd_setShadow(
        i_this->field_0x5bc, 0x01, model, &unkXyz1, BREG_F(0x13) + 1200.0f, BREG_F(0x11),
        actor->current.pos.y, i_this->mAcch.GetGroundH(), i_this->mAcch.m_gnd, &actor->tevStr, 0,
        1.0f, dDlst_shadowControl_c::getSimpleTex());
}

static int daE_Kr_Draw(e_kr_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;

    if (i_this->field_0x674 != 0) {
        return 1;
    }

    J3DModel* model = i_this->mpMorf->getModel();

    g_env_light.settingTevStruct(0, &actor->current.pos, &actor->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &actor->tevStr);

    if (i_this->field_0xefc != 0) {
        J3DModelData* modelData = model->getModelData();
        for (u16 i = 0; i < modelData->getMaterialNum(); i++) {
            J3DMaterial* material = modelData->getMaterialNodePointer(i);
            material->getTevColor(0)->r = i_this->field_0xef8;
            material->getTevColor(0)->g = i_this->field_0xef8;
            material->getTevColor(0)->b = i_this->field_0xef8;
        }
    }

    i_this->mpMorf->entryDL();

    daE_Kr_shadowDraw(i_this);

    return 1;
}

static void e_kr_pos_move(e_kr_class* i_this) {
    cXyz unkXyz1;

    fopAc_ac_c* actor = &i_this->enemy;

    f32 xDiff = i_this->field_0x678.x - actor->current.pos.x;
    f32 yDiff = i_this->field_0x678.y - actor->current.pos.y;
    f32 zDiff = i_this->field_0x678.z - actor->current.pos.z;

    s16 unkYaw = cM_atan2s(xDiff, zDiff);
    s16 unkPitch = -cM_atan2s(yDiff, JMAFastSqrt(xDiff * xDiff + zDiff * zDiff));

    s16 curYaw = actor->current.angle.y;

    cLib_addCalcAngleS2(&actor->current.angle.y, unkYaw, 10 + TREG_S(3),
                        i_this->field_0x694 * i_this->field_0x68c);

    curYaw = (curYaw - actor->current.angle.y) * 32;
    s16 unkShort1 = 5500 + TREG_S(1);
    if (curYaw > unkShort1) {
        curYaw = unkShort1;
    } else if ((s32)curYaw < -(s32)unkShort1) {
        curYaw = -unkShort1;
    }

    cLib_addCalcAngleS2(&actor->current.angle.z, curYaw, 10 + TREG_S(3),
                        i_this->field_0x694 * i_this->field_0x68c * 0.5f);
    cLib_addCalcAngleS2(&actor->current.angle.x, unkPitch, TREG_S(3) + 10,
                        i_this->field_0x694 * i_this->field_0x68c);
    cLib_addCalc2(&i_this->field_0x68c, 1.0f, 1.0f, 0.04f);
    cLib_addCalc2(&actor->speedF, i_this->field_0x684, 1.0f, i_this->field_0x688);

    unkXyz1.x = 0.0f;
    unkXyz1.y = 0.0f;
    unkXyz1.z = actor->speedF;

    cMtx_YrotS(*calc_mtx, actor->current.angle.y);
    cMtx_XrotM(*calc_mtx, actor->current.angle.x);
    MtxPosition(&unkXyz1, &actor->speed);

    actor->current.pos.x = actor->current.pos.x + actor->speed.x;
    actor->current.pos.y = actor->current.pos.y + actor->speed.y;
    actor->current.pos.z = actor->current.pos.z + actor->speed.z;

    if (i_this->field_0x6a8 != 0) {
        i_this->field_0x6a8--;
        actor->current.pos.y += 5.0f;
    }
}

static void e_kr_ground_pos_move(e_kr_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;

    cXyz unkXyz1;
    cXyz unkXyz2;

    f32 xDiff = i_this->field_0x678.x - actor->current.pos.x;
    f32 zDiff = i_this->field_0x678.z - actor->current.pos.z;
    s16 latAngle = cM_atan2s(xDiff, zDiff);
    cLib_addCalcAngleS2(&actor->current.angle.y, latAngle, 2 + TREG_S(3),
                        i_this->field_0x694 * i_this->field_0x6c4);
    cLib_addCalc2(&i_this->field_0x6c4, 1.0f, 1.0f, 0.1f);
    cLib_addCalc2(&actor->speedF, i_this->field_0x684, 1.0f, i_this->field_0x688);

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

static BOOL coach_path_check(e_kr_class* i_this) {
    fopAc_ac_c* coach = fopAcM_SearchByName(PROC_NPC_COACH);
    if (coach == NULL) {
        return FALSE;
    }

    Vec* pointPos = &i_this->field_0x6e4->m_points[0].m_position;
    f32 x = pointPos->x - coach->current.pos.x;
    f32 z = pointPos->z - coach->current.pos.z;
    if (JMAFastSqrt(x * x + z * z) < 500.0f + KREG_F(9)) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static s8 e_kr_path_move(e_kr_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* playerActor = dComIfGp_getPlayer(0);
    daPy_py_c* player = (daPy_py_c*)playerActor;

    cXyz unkXyz1;
    s8 unkFlag1 = TRUE;
    s8 retVal = TRUE;
    i_this->field_0xe8c = 2;
    s32 frame = i_this->mpMorf->getFrame();

    switch (i_this->field_0x670) {
    case 0: {
        if (actor->current.pos.y > i_this->field_0x678.y) {
            if (frame == 9 + TREG_S(0)) {
                i_this->field_0x670 = 1;
                anm_init(i_this, 14, 12.0f + TREG_F(0), 2, 1.0f);
            }
        }
        break;
    }
    case 1: {
        if (actor->current.pos.y <= i_this->field_0x678.y) {
            i_this->field_0x670 = 0;
            anm_init(i_this, 12, 5.0f, 2, 2.0f);
        }
        break;
    }
    case 2: {
        i_this->field_0x670 = 3;
        anm_init(i_this, 8, 5.0f, 2, 2.0f);
        break;
    }
    case 3:
        break;
    default:
        break;
    }

    dPnt* point;
    dBomb_c* bomb;
    switch (i_this->field_0x672) {
    case -10: {
        unkFlag1 = FALSE;
        retVal = FALSE;
        i_this->field_0x6c8 = 10;
        if (coach_path_check(i_this)) {
            i_this->field_0x672 = -9;
        }
        break;
    }
    case -9: {
        unkFlag1 = FALSE;
        retVal = FALSE;
        bomb = (dBomb_c*)dBomb_c::createBombCargoCarry(&actor->current.pos, &actor->shape_angle,
                                                       fopAcM_GetRoomNo(actor));
        if (bomb != NULL) {
            i_this->field_0x66b = 1;
            i_this->field_0x674 = 0;
            i_this->field_0x672 = 0;
            i_this->field_0xf00 = fopAcM_GetParam(actor) >> 24;
            i_this->mBombId = fopAcM_GetID(bomb);
            fopAcM_setCarryNow(bomb, 0);
        }
        break;
    }
    case 0: {
        i_this->field_0x672 = 1;
        i_this->field_0x684 = (50.0f + NREG_F(0)) * 1.2f;
        i_this->field_0x688 = 2.0f;
        i_this->field_0x68c = 1.0f + TREG_F(7);
        i_this->field_0x6e2++;

        point = &i_this->field_0x6e4->m_points[i_this->field_0x6e2];
        i_this->field_0x678.x = point->m_position.x;
        i_this->field_0x678.y = point->m_position.y;
        i_this->field_0x678.z = point->m_position.z;
    }
    case 1: {
        f32 xDiff = i_this->field_0x678.x - actor->current.pos.x;
        f32 yDiff = i_this->field_0x678.y - actor->current.pos.y;
        f32 zDiff = i_this->field_0x678.z - actor->current.pos.z;
        f32 totalDiff = JMAFastSqrt(xDiff * xDiff + yDiff * yDiff + zDiff * zDiff);
        i_this->field_0x694 = 5000.0f + NREG_F(6);

        if (totalDiff < 10.0f * TREG_F(10) + 300.0f) {
            point = &i_this->field_0x6e4->m_points[i_this->field_0x6e2];
            u8 pointArg1 = point->mArg1;
            if (pointArg1 == -1) {
                i_this->field_0x684 = 50.0f * 1.2f;
            } else {
                i_this->field_0x684 = pointArg1;
            }

            if (i_this->field_0x6e2 >= i_this->field_0x6e4[0].m_num - 1) {
                i_this->field_0x672 = 2;
                i_this->field_0x69c[2] = 200;
                cMtx_YrotS(*calc_mtx, actor->current.angle.y);
                unkXyz1.x = 0.0f;
                unkXyz1.y = 2000.0f;
                unkXyz1.z = -2000.0f;
                MtxPosition(&unkXyz1, &i_this->field_0x678);
                i_this->field_0x678 += actor->current.pos;
                i_this->field_0x670 = 2;
                i_this->field_0x66b = 0;
                bomb = (dBomb_c*)fopAcM_SearchByID(i_this->mBombId);
                if (bomb != NULL) {
                    fopAcM_cancelCarryNow(bomb);
                    bomb->speedF = KREG_F(8) + 20.0f;
                }
                i_this->field_0x6c8 = 0xdc;
            } else {
                i_this->field_0x672 = 0;
            }
        }
        break;
    }
    case 2: {
        i_this->field_0x6c8 = 10;
        retVal = FALSE;
        if (i_this->field_0x69c[2] == 180) {
            daPy_getPlayerActorClass()->cancelBoomerangLockActor(actor);
        }
        if (i_this->field_0x69c[2] == 150) {
            bomb = (dBomb_c*)fopAcM_SearchByID(i_this->mBombId);
            if (bomb != NULL) {
                bomb->setCargoBombExplode();
            }
            i_this->field_0xf04 = 1;
        }
        if (i_this->field_0x69c[2] == 50) {
            i_this->field_0xf00 = -1;
        }
        if (i_this->field_0x69c[2] == 0) {
            actor->current = actor->home;
            i_this->field_0x672 = -10;
            i_this->field_0x674 = 1;
            i_this->field_0x6e2 = 0;
            actor->speedF = 0.0f;
            i_this->field_0x684 = 0;
            i_this->field_0xf04 = 0;
            unkFlag1 = FALSE;
        }
        break;
    }
    }

    if (unkFlag1) {
        e_kr_pos_move(i_this);
    }

    return retVal;
}

static void e_kr_auto_move(e_kr_class* i_this) {
    fopAc_ac_c* playerActor = dComIfGp_getPlayer(0);
    fopAc_ac_c* player = (daPy_py_c*)playerActor;
    fopAc_ac_c* actor = &i_this->enemy;
    cXyz unkXyz1;
    cXyz unkXyz2;
    s8 unkByte1 = 0;

    i_this->field_0xe8c = 2;
    switch (i_this->field_0x670) {
    case 0: {
        s32 frame = i_this->mpMorf->getFrame();
        if (i_this->field_0x69c[0] == 0 && frame == 9 + TREG_S(0)) {
            i_this->field_0x670 = 1;
            i_this->field_0x69c[0] = cM_rndF(200.0f) + 50.0f;
            anm_init(i_this, 13, 12.0f + TREG_F(0), 2, 1.0f);
        }

        break;
    }
    case 1: {
        if (i_this->field_0x69c[0] == 0 && actor->current.pos.y < i_this->field_0x678.y) {
            i_this->field_0x670 = 0;
            i_this->field_0x69c[0] = cM_rndF(60.0f) + 20.0f;
            anm_init(i_this, 8, 5.0f, 2, l_e_krHIO.field_0x28);
        }

        break;
    }
    case 10:
        break;
    case 20: {
        s32 frame = i_this->mpMorf->getFrame();
        if (frame == 34) {
            i_this->field_0x670 = 0;
            i_this->field_0x69c[0] = cM_rndF(60.0f) + 20.0f;
            anm_init(i_this, 8, 0.0f, 2, l_e_krHIO.field_0x28);
        }
    }
    default:
        break;
    }

    switch (i_this->field_0x672) {
    case 0: {
        if (i_this->field_0x69c[1] == 0) {
            f32 xDiff = actor->home.pos.x + cM_rndFX(l_e_krHIO.field_0xc) - actor->current.pos.x;
            f32 zDiff = actor->home.pos.z + cM_rndFX(l_e_krHIO.field_0xc) - actor->current.pos.z;
            if (JMAFastSqrt(xDiff * xDiff + zDiff * zDiff) > 200.0f) {
                i_this->field_0x69c[1] = cM_rndF(150.0f) + 50.0f;
                i_this->field_0x678.x = xDiff + actor->current.pos.x;
                i_this->field_0x678.y = actor->home.pos.y + cM_rndF(500.0f);
                i_this->field_0x678.z = zDiff + actor->current.pos.z;
                i_this->field_0x68c = 0;
                i_this->field_0x684 = 25.0f + TREG_F(10);
                i_this->field_0x688 = 1.0f + TREG_F(11);
                i_this->field_0x694 = cM_rndF(300.0f) + 200.0f;
                if ((l_e_krHIO.field_0x6 != 0 ||
                     (i_this->field_0x69c[3] == 0 && i_this->field_0x66b == 0)) &&
                    e_kr_player_view_check(i_this))
                {
                    i_this->mCurAction = 3;
                    i_this->field_0x672 = 0;
                    i_this->mSound.startCreatureVoice(Z2SE_EN_KR_V_FIND, -1);
                }
            }
        }
    }
    }

    switch (unkByte1) {
    case 0:
        e_kr_pos_move(i_this);
        break;
    case 1:
        e_kr_ground_pos_move(i_this);
        if (i_this->field_0x672 != 0x19) {
            f32 xDiff = playerActor->current.pos.x - actor->current.pos.x;
            f32 zDiff = playerActor->current.pos.z - actor->current.pos.z;
            if (JMAFastSqrt(xDiff * xDiff + zDiff * zDiff) < 400.0f) {
                i_this->field_0x672 = 0;
                i_this->field_0x6a8 = 0;
                i_this->field_0x69c[0] = cM_rndF(60.0f) + 20.0f;
                anm_init(i_this, 8, 1.0f, 2, l_e_krHIO.field_0x68);
                i_this->field_0x69c[1] = 50;
                i_this->field_0x694 = 10.0f * TREG_F(4) + 5000.0f;
                i_this->field_0x684 = 1.0f;
                i_this->field_0x688 = 1.0f;
                actor->speedF = 0.0f;
                MtxTrans(actor->current.pos.x, actor->current.pos.y, actor->current.pos.z, 0);
                unkXyz1.x = 0.0f;
                unkXyz1.y = 1000.0f;
                unkXyz1.z = 2000.0f;
                cMtx_YrotM(*calc_mtx, actor->current.angle.y);
                MtxPosition(&unkXyz1, &i_this->field_0x678);
                i_this->field_0x66a = cM_rndF(250.0f) + 250.0f;
            }
        }
        break;
    }
}

static void e_kr_atack_move(e_kr_class* i_this) {
    fopAc_ac_c* playerActor = dComIfGp_getPlayer(0);
    daPy_py_c* player = (daPy_py_c*)playerActor;
    fopAc_ac_c* actor = &i_this->enemy;
    f32 playerDist = fopAcM_searchPlayerDistance(actor);
    cXyz unkXyz1;

    s8 unkFlag1 = FALSE;
    s8 unkFlag2 = FALSE;

    i_this->field_0xe8c = 1;

    if (i_this->field_0xec0 != 0) {
        i_this->field_0xec0 = 0;
        i_this->field_0x69c[5] = (s32)(l_e_krHIO.field_0x24 * (cM_rndF(30.0f) + 20.0f));

        anm_init(i_this, 17, 10.0f, 2, l_e_krHIO.field_0x2c);

        i_this->field_0x690 = 0;
        i_this->field_0x68c = 0;
        i_this->field_0x672 = 4;

        kuti_open(i_this, 25, Z2SE_EN_KR_V_NAKU);
    }

    s32 frame = i_this->mpMorf->getFrame();

    f32 unkFloat1;
    switch (i_this->field_0x672) {
    case 0:
        i_this->field_0x672 = 1;
        i_this->field_0x68c = i_this->field_0x690 = 0.0f;
        i_this->field_0x684 = 30.0f;
        i_this->field_0x688 = 1.0f;

        anm_init(i_this, 13, 10.0f, 2, 1.0f);
    case 1:
    case 2:
        i_this->field_0x678 = playerActor->current.pos;
        i_this->field_0x678.y = playerActor->eyePos.y + 50.0f;

        e_kr_pos_move(i_this);

        unkXyz1 = i_this->field_0x678 - actor->current.pos;

        unkFloat1 = unkXyz1.abs();
        if (unkFloat1 < 100.0f * TREG_F(9) + 500.0f) {
            i_this->field_0x672 = 3;
        } else {
            if (i_this->field_0x672 == 1) {
                i_this->field_0x694 = 400.0f;
                if (unkFloat1 < l_e_krHIO.field_0x50) {
                    i_this->field_0x672 = 2;

                    anm_init(i_this, 11, 10.0f, 0, 1.0f);

                    kuti_open(i_this, 25, Z2SE_EN_KR_V_NAKU);
                }
            } else {
                i_this->field_0xea8 = KREG_F(11) + 1000.0f;
                i_this->field_0x694 = 2000.0f;
            }
        }
        break;
    case 3:
    case 35:
        i_this->field_0x684 = 0;
        i_this->field_0x688 = 3.0f;

        e_kr_pos_move(i_this);

        if (actor->speedF <= 0.1f || i_this->mAcch.ChkWallHit()) {
            i_this->field_0x672 = 4;
            i_this->field_0x69c[0] = 0;

            if (i_this->field_0x664 == 4 || i_this->field_0x664 == 7) {
                i_this->field_0x69c[1] =
                    l_e_krHIO.field_0x1c + cM_rndF(l_e_krHIO.field_0x1e - l_e_krHIO.field_0x1c);
            } else {
                i_this->field_0x69c[1] = (f32)l_e_krHIO.field_0x18 +
                                      cM_rndF(l_e_krHIO.field_0x1a - l_e_krHIO.field_0x18);
            }

            i_this->field_0x68c = i_this->field_0x690 = 0.0f;

            kuti_open(i_this, 25, Z2SE_EN_KR_V_NAKU);
        }
        if (i_this->field_0x672 == 3 && actor->speedF <= 10.0f) {
            i_this->field_0x672 = 0x23;

            anm_init(i_this, 17, 5.0f, 2, l_e_krHIO.field_0x2c);
        }
        break;
    case 4:
        i_this->field_0x6d8++;
        unkXyz1.x = cM_ssin(i_this->field_0x6d8 * (TREG_S(2) + 1000)) * 200.0f;
        unkXyz1.y = cM_ssin(i_this->field_0x6d8 * (TREG_S(3) + 0x4b0)) * 100.0f;
        unkXyz1.z = cM_scos(i_this->field_0x6d8 * (TREG_S(4) + 0x5dc)) * 200.0f;

        cLib_addCalc2(&actor->current.pos.x, i_this->field_0x678.x + unkXyz1.x, 0.1f,
                      i_this->field_0x68c * 30.0f);
        cLib_addCalc2(&actor->current.pos.y, i_this->field_0x678.y + unkXyz1.y, 0.1f,
                      i_this->field_0x68c * 30.0f);
        cLib_addCalc2(&actor->current.pos.z, i_this->field_0x678.z + unkXyz1.z, 0.1f,
                      i_this->field_0x68c * 30.0f);
        cLib_addCalc2(&i_this->field_0x68c, l_e_krHIO.field_0x20, 1.0f,
                      l_e_krHIO.field_0x20 * 0.1f);

        cLib_addCalcAngleS2(&actor->current.angle.y, i_this->field_0x6c0, 5,
                            i_this->field_0x690 * 1280.0f);
        cLib_addCalcAngleS2(&actor->current.angle.x, i_this->field_0x6c2, 5,
                            i_this->field_0x690 * 768.0f);
        cLib_addCalcAngleS2(&actor->current.angle.z, 0, 5, i_this->field_0x690 * 768.0f);

        cLib_addCalc2(&i_this->field_0x690, 1.0f, 1.0f, 0.05f);

        if (i_this->field_0x69c[1] == 0) {
            unkFlag1 = TRUE;
        } else {
            if ((i_this->field_0x69c[0] & 0x1f) == 0 && cM_rndF(1.0f) < 0.5f) {
                kuti_open(i_this, 25, Z2SE_EN_KR_V_NAKU);
            }

            if (i_this->field_0x69c[0] == 0) {
                cMtx_YrotS(*calc_mtx, (playerActor->shape_angle).y + (s16)cM_rndFX(7000.0f));
                unkXyz1.x = 0.0f;
                unkXyz1.y = cM_rndF(100.0f) + 100.0f + TREG_F(15);
                unkXyz1.z = cM_rndF(100.0f) + 300.0f + TREG_F(16);

                MtxPosition(&unkXyz1, &i_this->field_0x678);

                i_this->field_0x678.x = i_this->field_0x678.x + playerActor->current.pos.x;
                i_this->field_0x678.y = i_this->field_0x678.y + playerActor->eyePos.y;
                i_this->field_0x678.z = i_this->field_0x678.z + playerActor->current.pos.z;

                i_this->field_0x69c[0] = cM_rndF(100.0f) + 20.0f;
                i_this->field_0x68c = i_this->field_0x690 = 0.0f;
            }

            if (i_this->field_0x69c[5] == 0 && i_this->field_0x6c4 > TREG_F(5) * 10.0f + 300.0f &&
                i_this->field_0x6c4 < TREG_F(6) * 10.0f + 400.0f && frame >= 14 + BREG_S(7))
            {
                i_this->field_0x672 = 7;
                i_this->field_0x69c[5] = l_e_krHIO.field_0x24 * (cM_rndF(30.0f) + 20.0f);

                anm_init(i_this, 4, 2.0f, 0, l_e_krHIO.field_0x30);

                i_this->mSound.startCreatureVoice(Z2SE_EN_KR_V_ATTACK, -1);

                i_this->field_0x68c = i_this->field_0x690 = 0.0f;
                i_this->field_0x684 = 0.0f;

                i_this->field_0x688 = 5.0f;
                actor->speedF = 0.0f;
            }
        }

        if (playerDist > 600.0f) {
            i_this->field_0x672 = 5;
            i_this->field_0x69c[0] = cM_rndF(35.0f) + 35.0f;
        }
        break;
    case 5:
        if ((i_this->field_0x6d6 & 0x3f) == 0 && cM_rndF(1.0f) < 0.5f) {
            kuti_open(i_this, 25, Z2SE_EN_KR_V_NAKU);
        }

        i_this->field_0x684 = 25.0f;
        i_this->field_0x688 = 2.0f;
        i_this->field_0x694 = 2000.0f;
        i_this->field_0x678.x = playerActor->current.pos.x;
        i_this->field_0x678.y = playerActor->eyePos.y + 50.0f;
        i_this->field_0x678.z = playerActor->current.pos.z;

        e_kr_pos_move(i_this);

        if (playerDist < 500.0f) {
            i_this->field_0x672 = 4;
            i_this->field_0x690 = 0;
            i_this->field_0x68c = 0;
        }

        if (i_this->field_0x69c[0] == 0 && playerDist > 1350.0f) {
            i_this->field_0x672 = 0;
        }

        if (i_this->field_0x69c[1] == 0) {
            unkFlag1 = TRUE;
        }
        break;
    case 7:
        i_this->field_0xe8c = 0;

        if (frame == 3) {
            kuti_open(i_this, 25, Z2SE_SY_DUMMY);
        }

        if (frame == 15 + TREG_S(5)) {
            i_this->field_0x684 = 30.0f + TREG_F(7);
            i_this->field_0x688 = 10.0f + TREG_F(8);
        }

        if (frame == 25 + TREG_S(6)) {
            i_this->field_0x684 = -10.0f + TREG_F(9);
            i_this->field_0x688 = 5.0f + TREG_F(10);
        }

        if (frame <= 23) {
            i_this->field_0x694 = 2000.0f;
            i_this->field_0x678.x = playerActor->current.pos.x;
            i_this->field_0x678.y = playerActor->eyePos.y;
            i_this->field_0x678.z = playerActor->current.pos.z;
        } else {
            i_this->field_0x694 = 0;
        }

        e_kr_pos_move(i_this);

        if (i_this->mpMorf->isStop()) {
            i_this->field_0x672 = 4;

            anm_init(i_this, 17, 3.0f, 2, l_e_krHIO.field_0x2c);

            i_this->field_0x69c[5] = l_e_krHIO.field_0x24 * (cM_rndF(30.0f) + 20.0f);

            cMtx_YrotS(*calc_mtx, playerActor->shape_angle.y + (s16)cM_rndFX(3000.0f));

            unkXyz1.x = 0.0f;
            unkXyz1.y = cM_rndF(100.0f) + 100.0f + TREG_F(15);
            unkXyz1.z = cM_rndF(100.0f) + 300.0f + TREG_F(16);
            MtxPosition(&unkXyz1, &i_this->field_0x678);

            i_this->field_0x678.x = i_this->field_0x678.x + playerActor->current.pos.x;
            i_this->field_0x678.y = i_this->field_0x678.y + playerActor->eyePos.y;
            i_this->field_0x678.z = i_this->field_0x678.z + playerActor->current.pos.z;

            i_this->field_0x69c[0] = cM_rndF(100.0f) + 20.0f;
            i_this->field_0x68c = i_this->field_0x690 = 1.0f;
        } else {
            if (frame >= 15 && frame <= 27) {
                unkFlag2 = true;
                if (player->checkPlayerGuard() && i_this->mSphere1.ChkAtHit()) {
                    i_this->field_0x6aa = 10;
                    i_this->field_0x672 = 10;
                    i_this->field_0x69c[0] = l_e_krHIO.field_0x34;

                    anm_init(i_this, 16, 0.0f, 0, l_e_krHIO.field_0x38);

                    i_this->field_0x684 = TREG_F(7);
                    i_this->field_0x688 = 1.0f;
                    actor->speedF = -20.0f;

                    unkFlag2 = false;

                    i_this->mSound.startCreatureVoice(Z2SE_EN_KR_V_DAMAGE_S, -1);
                }
            }
        }
        break;
    case 10:
    case 11:
        i_this->field_0xe8c = 0;

        e_kr_pos_move(i_this);

        if ((s32)i_this->mpMorf->getFrame() == 24) {
            i_this->mSound.startCreatureVoice(Z2SE_EN_KR_V_FURA, -1);
        }

        if (i_this->mpMorf->isStop() && i_this->field_0x672 == 10) {
            i_this->field_0x672 = 11;
            anm_init(i_this, 5, 0.0f, 2, l_e_krHIO.field_0x3c);
        }

        if (i_this->field_0x69c[0] == 0) {
            i_this->field_0x672 = 4;
            i_this->field_0x69c[5] = l_e_krHIO.field_0x24 * (cM_rndF(30.0f) + 20.0f);
            anm_init(i_this, 17, 10.0f, 2, l_e_krHIO.field_0x2c);
            i_this->field_0x68c = i_this->field_0x690 = 0.0f;
        }

        cLib_addCalc2(&actor->current.pos.y, i_this->mAcch.GetGroundH() + 100.0f + TREG_F(0x11),
                      0.05f, 5.0f);

        break;
    default:
        break;
    }

    if (i_this->field_0x664 != 0x03) {
        unkXyz1 = i_this->field_0x678 - actor->current.pos;

        unkFloat1 = unkXyz1.abs();
        if (e_kr_player_bg_check(i_this) || e_kr_setpos_bg_check(i_this) || unkFlag1) {
            i_this->mCurAction = i_this->field_0x664;
            if (i_this->mCurAction == 4 || i_this->mCurAction == 7) {
                i_this->field_0x672 = 10;

                anm_init(i_this, 8, 5.0f, 2, l_e_krHIO.field_0x28);
            } else {
                i_this->field_0x672 = 0;
                i_this->field_0x69c[0] = 0;
                i_this->field_0x69c[1] = 0;
                i_this->field_0x670 = 1;

                anm_init(i_this, 13, TREG_F(0) + 12.0f, 2, 1.0f);

                actor->speedF = 0.0f;

                i_this->field_0x678.y = playerActor->current.pos.y + 500.0f;
                i_this->field_0x69c[3] = cM_rndF(200.0f) + 300.0f;
            }
            i_this->field_0x68c = 0;
        }
    }

    if (unkFlag2) {
        i_this->mSphere1.SetC(actor->eyePos);
        dComIfG_Ccsp()->Set(&i_this->mSphere1);
    } else {
        i_this->mSphere1.ClrAtHit();
    }

    if ((i_this->field_0x6d6 & 0x3f) == 0 && pl_horse_check(i_this)) {
        i_this->mCurAction = 8;
        i_this->field_0x672 = 0;
    }
}

static void e_kr_horse_move(e_kr_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz unkXyz1;
    cXyz unkXyz2;

    i_this->field_0xe8c = 1;

    s32 frame = i_this->mpMorf->getFrame();
    f32 speed = dComIfGp_getHorseActor()->speedF;
    if (speed > 60.0f) {
        speed = 60.0f;
    } else if (speed < 30.0f) {
        speed = 30.0f;
    }

    s8 bVar2 = false;

    switch (i_this->field_0x672) {
    case 0: {
        i_this->field_0x672 = 2;
        i_this->field_0x68c = i_this->field_0x690 = 0.0f;
        anm_init(i_this, 8, 5.0f, 2, cM_rndF(0.2f) + 1.2f);
        i_this->field_0x69c[1] = cM_rndF(100.0f) + 60.0f;
        i_this->field_0x6bc = player->shape_angle.y;
        i_this->field_0x69c[0] = 0;
        i_this->field_0x69c[2] = 0;
        i_this->field_0x69c[3] = 0;
    }
    case 1: {
        if (i_this->field_0x69c[1] == 0) {
            anm_init(i_this, 8, 5.0f, 2, cM_rndF(0.2f) + 1.0f);

            i_this->field_0x672 = 2;
            i_this->field_0x69c[1] = cM_rndF(150.0f) + 70.0f;
        }

        goto FROM_CASE_1;
    }
    case 2: {
        if (i_this->field_0x69c[1] == 0) {
            anm_init(i_this, 13, 10.0f, 2, 1.0f);

            i_this->field_0x672 = 1;
            i_this->field_0x69c[1] = cM_rndF(150.0f) + 70.0f;
        }

    FROM_CASE_1:
        cLib_addCalcAngleS2(&i_this->field_0x6bc, player->shape_angle.y, 4, 0x200);

        if (i_this->field_0x69c[0] == 0) {
            i_this->field_0x6b4 = cM_rndFX(400.0f);
            i_this->field_0x6b8 = cM_rndF(180.0f) + 100.0f;
            i_this->field_0x69c[0] = cM_rndF(70.0f) + 20.0f;
        }

        f32 unkFloat1 = cM_ssin(i_this->field_0x6d6 * (KREG_S(7) + 200)) * 20.0f;
        cMtx_YrotS(*calc_mtx, i_this->field_0x6bc);
        unkXyz1.x = i_this->field_0x6b4;
        unkXyz1.y = i_this->field_0x6b8;
        unkXyz1.z = (speed - 30.0f) * (unkFloat1 + 30.0f);
        MtxPosition(&unkXyz1, &unkXyz2);
        i_this->field_0x678 = player->current.pos + unkXyz2;
        unkXyz2 = i_this->field_0x678 - actor->current.pos;
        unkFloat1 = unkXyz2.abs();
        if (i_this->field_0x69c[2] == 0) {
            i_this->field_0x69c[2] = cM_rndF(30.0f) + 20.0f;

            if (unkFloat1 < 300.0f) {
                speed *= 0.95f;
                i_this->field_0x694 = 400.0f + NREG_F(3);
            } else {
                speed *= 1.2f;
                i_this->field_0x694 = 600.0f + NREG_F(4);
            }

            i_this->field_0x684 = speed;
            i_this->field_0x688 = NREG_F(2) + 0.4f;

            if (i_this->field_0x684 - actor->speedF > 10.0f) {
                if (i_this->field_0x672 == 1) {
                    i_this->field_0x672 = 2;

                    anm_init(i_this, 8, 5.0f, 2, cM_rndF(0.2f) + 1.2f);

                    i_this->field_0x69c[1] = cM_rndF(100.0f) + 60.0f;
                } else {
                    i_this->mpMorf->setPlaySpeed(cM_rndF(0.2f) + 1.2f);
                }
            }
        }

        if ((i_this->field_0x6d6 & 0x3f) == 0 && cM_rndF(1.0f) < 0.5f) {
            kuti_open(i_this, 25, Z2SE_EN_KR_V_NAKU);
        }

        if (i_this->field_0x69c[3] == 0) {
            unkXyz1 = player->eyePos - actor->current.pos;
            if (unkXyz1.abs() < JREG_F(0) + 250.0f) {
                i_this->field_0x672 = 3;

                anm_init(i_this, 10, 5.0f, 2, cM_rndF(0.1f) + 1.0f);

                i_this->mSound.startCreatureVoice(Z2SE_EN_KR_V_ATTACK2, -1);

                i_this->field_0x69c[0] = JREG_S(1) + 0x14;
            }
        }

        break;
    }
    case 3: {
        bVar2 = true;

        if (i_this->field_0x69c[0] == 0) {
            anm_init(i_this, 9, 3.0f, 0, 1.0f);

            i_this->field_0x672 = 4;
        }

        break;
    }
    case 4: {
        bVar2 = true;

        speed = JREG_F(1) + 30.0f;

        cMtx_YrotS(*calc_mtx, (player->shape_angle).y);

        unkXyz1.x = 0.0f;
        unkXyz1.y = 0.0f;
        unkXyz1.z = JREG_F(7) + -100.0f;

        MtxPosition(&unkXyz1, &unkXyz2);
        cLib_addCalc2(&actor->current.pos.x, player->eyePos.x + unkXyz2.x, 0.5f, speed);
        cLib_addCalc2(&actor->current.pos.y, player->eyePos.y, 0.5f, speed);
        cLib_addCalc2(&actor->current.pos.z, player->eyePos.z + unkXyz2.z, 0.5f, speed);

        if (frame >= 10) {
            i_this->mSphere1.SetC(i_this->field_0xe64[0]);
            dComIfG_Ccsp()->Set(&i_this->mSphere1);
        } else {
            i_this->mSphere1.ClrAtHit();
        }

        if (i_this->mpMorf->isStop() && JREG_S(3) == 0) {
            i_this->field_0x672 = 2;
            i_this->field_0x69c[1] = 0;
            i_this->field_0x69c[3] = cM_rndF(100.0f) + 60.0f;
        }

        break;
    }
    }

    if (bVar2) {
        cLib_addCalcAngleS2(&i_this->field_0x6bc, player->shape_angle.y, 2, 0x2000);
        cMtx_YrotS(*calc_mtx, i_this->field_0x6bc);

        unkXyz1.x = 0.0f;
        unkXyz1.y = 200.0f;
        unkXyz1.z = 100.0f;
        MtxPosition(&unkXyz1, &unkXyz2);

        i_this->field_0x678 = player->current.pos + unkXyz2;

        i_this->field_0x694 = 1000.0f + NREG_F(4);
    }
    e_kr_pos_move(i_this);
    if ((i_this->field_0x6d6 & 0x3f) == 0 && !pl_horse_check(i_this)) {
        i_this->mCurAction = 3;
        i_this->field_0x672 = 0;
    }
}

static void e_kr_wait_move(e_kr_class* i_this) {
    fopAc_ac_c* playerActor = dComIfGp_getPlayer(0);
    daPy_py_c* player = (daPy_py_c*)playerActor;
    fopAc_ac_c* actor = &i_this->enemy;

    cXyz unkXyz1;
    cXyz unkXyz2;

    s8 unkByte1 = -1;
    f32 speedX = fabsf(actor->speed.x);
    if (speedX < 5.0f) {
        speedX = 5.0f;
    }
    f32 speedZ = fabsf(actor->speed.z);
    if (speedZ < 5.0f) {
        speedZ = 5.0f;
    }

    i_this->field_0xe8c = 2;

    switch (i_this->field_0x672) {
    case -1:
        i_this->field_0x91c = true;

        if (!i_this->mpMorf->isStop()) {
            break;
        }

        i_this->field_0x672 = 0;
    case 0:
        anm_init(i_this, 21, 1.0f, 2, 1.0f);

        i_this->field_0x672 = 1;
        i_this->field_0x69c[0] = 100;
    case 1:
        i_this->field_0x91c = true;

        cLib_addCalcAngleS2(&actor->current.angle.y, actor->home.angle.y, 5, 0x300);
        cLib_addCalcAngleS2(&actor->current.angle.x, 0, 5, 0x800);
        cLib_addCalcAngleS2(&actor->current.angle.z, 0, 5, 0x800);

        cLib_addCalc2(&actor->current.pos.x, actor->home.pos.x, 0.2f, speedX);
        cLib_addCalc2(&actor->current.pos.z, actor->home.pos.z, 0.2f, speedZ);
        actor->current.pos.y = actor->home.pos.y;

        if (i_this->field_0x69c[0] == 0 && e_kr_player_view_check(i_this)) {
            i_this->field_0x672 = 2;
            i_this->field_0x69c[0] = 0x1e;

            anm_init(i_this, 7, 5.0f, 0, 1.0f);

            i_this->mSound.startCreatureVoice(Z2SE_EN_KR_V_FIND, -1);
        }

        break;
    case 2:
        i_this->field_0xe8c = 1;

        if (i_this->field_0x69c[0] != 0) {
            break;
        }

        i_this->field_0x672 = 3;

        anm_init(i_this, 8, 5.0f, 2, l_e_krHIO.field_0x28 * 1.5f);

        i_this->field_0x69c[0] = TREG_S(0) + 0x46;
    case 3:
        i_this->field_0xe8c = 1;
        cLib_addCalcAngleS2(&actor->current.angle.y, i_this->field_0x6c0, 10, 0x200);
        unkXyz1.x = 0.0f;
        unkXyz1.y = TREG_F(0) * 10.0f + 300.0f;
        unkXyz1.z = TREG_F(1) * 10.0f + 300.0f;
        cMtx_YrotS(*calc_mtx, actor->current.angle.y);
        MtxPosition(&unkXyz1, &unkXyz2);
        cLib_addCalc2(&actor->current.pos.x, actor->home.pos.x + unkXyz2.x, 0.1f, 10.0f);
        cLib_addCalc2(&actor->current.pos.y, actor->home.pos.y + unkXyz2.y, 0.1f, 10.0f);
        cLib_addCalc2(&actor->current.pos.z, actor->home.pos.z + unkXyz2.z, 0.1f, 10.0f);
        if (i_this->field_0x69c[0] == 0) {
            i_this->mCurAction = 3;
            i_this->field_0x672 = 0;
        }
        break;
    case 10:
    case 11:
    case 12: {
        unkByte1 = 0;
        i_this->field_0xe8c = 0;

        unkXyz1.x = 0.0f;
        unkXyz1.y = 0.0f;
        unkXyz1.z = 0.0f;

        cMtx_YrotS(*calc_mtx, actor->current.angle.y);
        MtxPosition(&unkXyz1, &unkXyz2);

        speedX = 80.0f;
        if (i_this->field_0x672 == 10) {
            speedX = 300.0f;
        }

        i_this->field_0x678.x = actor->home.pos.x + unkXyz2.x;
        i_this->field_0x678.y = actor->home.pos.y + speedX;
        i_this->field_0x678.z = actor->home.pos.z + unkXyz2.z;

        i_this->field_0x684 = 20.0f;
        i_this->field_0x688 = 5.0f;
        i_this->field_0x694 = 2000.0f;

        f32 diffX = i_this->field_0x678.x - actor->current.pos.x;
        f32 diffY = i_this->field_0x678.y - actor->current.pos.y;
        f32 diffZ = i_this->field_0x678.z - actor->current.pos.z;
        f32 diff = JMAFastSqrt(diffX * diffX + diffY * diffY + diffZ * diffZ);

        if (i_this->field_0x672 == 10) {
            if (diff < 2000.0f) {
                i_this->field_0x672 = 0xb;
            } else {
                if (e_kr_player_view_check(i_this)) {
                    i_this->mCurAction = 3;
                    i_this->field_0x672 = 0;
                    i_this->mSound.startCreatureVoice(Z2SE_EN_KR_V_FIND, -1);
                }
            }
        } else if (i_this->field_0x672 == 0xb) {
            if (diff < l_e_krHIO.field_0x50) {
                i_this->field_0x672 = 12;

                anm_init(i_this, 11, 10.0f, 0, 1.0f);
            }
        } else if (i_this->field_0x672 == 12) {
            i_this->field_0xea8 = KREG_F(0xb) + 1000.0f;
            if (diff < 10.0f * TREG_F(9) + 300.0f) {
                i_this->field_0x672 = 0x14;

                anm_init(i_this, 15, 5.0f, 2, l_e_krHIO.field_0x44);

                i_this->field_0x670 = 10;
                actor->speed.y = 0.0f;
                i_this->field_0x6d8 = 0;
                unkByte1 = -1;
            }
        }
        break;
    }
    case 20: {
        i_this->field_0x91c = true;
        i_this->field_0xe8c = 0;
        s32 unkInt1 = i_this->field_0x6d8;
        i_this->field_0x6d8++;

        cLib_addCalc2(&actor->current.pos.x, actor->home.pos.x, 0.1f, speedX);
        cLib_addCalc2(&actor->current.pos.z, actor->home.pos.z, 0.1f, speedZ);
        actor->current.pos.y = actor->current.pos.y + actor->speed.y;

        if (unkInt1 > l_e_krHIO.field_0x40) {
            actor->speed.y = actor->speed.y - (TREG_F(7) + 0.8f);
        }

        cLib_addCalcAngleS2(&actor->current.angle.x, 0, 5, 0x800);
        cLib_addCalcAngleS2(&actor->current.angle.y, actor->home.angle.y, 5, 0x300);
        cLib_addCalcAngleS2(&actor->current.angle.z, 0, 5, 0x800);

        if (actor->current.pos.y <= actor->home.pos.y) {
            actor->current.pos.y = actor->home.pos.y;
            actor->speed.y = -0.5f;
            if (unkInt1 > l_e_krHIO.field_0x40) {
                anm_init(i_this, 19, 5.0f, 0, l_e_krHIO.field_0x48);
                i_this->field_0x672 = -1;
            }
        }

        break;
    }
    }

    switch (unkByte1) {
    case 0:
        e_kr_pos_move(i_this);
        break;
    }
}

static void e_kr_su_wait_move(e_kr_class* i_this) {
    fopAc_ac_c* playerActor = dComIfGp_getPlayer(0);
    daPy_py_c* player = (daPy_py_c*)playerActor;
    fopAc_ac_c* actor = &i_this->enemy;

    cXyz unkXyz1;
    cXyz unkXyz2;

    s8 unkByte1 = -1;

    f32 unkFloat1 = fabsf(actor->speed.x);
    if (unkFloat1 < 5.0f) {
        unkFloat1 = 5.0f;
    }

    f32 unkFloat2 = fabsf(actor->speed.z);
    if (unkFloat2 < 5.0f) {
        unkFloat2 = 5.0f;
    }

    i_this->field_0xe8c = 2;

    switch (i_this->field_0x672) {
    case -1:
        i_this->field_0x91c = true;

        if (!i_this->mpMorf->isStop()) {
            break;
        }

        i_this->field_0x672 = 0;
    case 0:
        anm_init(i_this, 21, 1.0f, 2, 1.0f);

        i_this->field_0x672 = 1;
        i_this->field_0x69c[0] = 100;
    case 1:
        i_this->field_0x91c = true;

        cLib_addCalcAngleS2(&actor->current.angle.y, actor->home.angle.y, 5, 0x300);
        cLib_addCalcAngleS2(&actor->current.angle.x, 0, 5, 0x800);
        cLib_addCalcAngleS2(&actor->current.angle.z, 0, 5, 0x800);

        cLib_addCalc2(&actor->current.pos.x, actor->home.pos.x, 0.2f, unkFloat1);
        cLib_addCalc2(&actor->current.pos.z, actor->home.pos.z, 0.2f, unkFloat2);

        actor->current.pos.y = actor->home.pos.y;

        if (i_this->field_0x69c[0] == 0 && e_kr_player_view_check(i_this)) {
            i_this->field_0x672 = 2;
            i_this->field_0x69c[0] = 30;

            anm_init(i_this, 7, 5.0f, 0, 1.0f);

            i_this->mSound.startCreatureVoice(Z2SE_EN_KR_V_FIND, -1);
        }

        break;
    case 2:
        i_this->field_0xe8c = 1;
        if (i_this->field_0x69c[0] != 0) {
            break;
        }

        i_this->field_0x672 = 3;

        anm_init(i_this, 8, 5.0f, 2, l_e_krHIO.field_0x28 * 1.5f);

        i_this->field_0x69c[0] = 70 + TREG_S(0);
    case 3:
        i_this->field_0xe8c = 1;

        cLib_addCalcAngleS2(&actor->current.angle.y, i_this->field_0x6c0, 10, 0x200);

        unkXyz1.x = 0.0f;
        unkXyz1.y = 10.0f * TREG_F(0) + 300.0f;
        unkXyz1.z = 10.0f * TREG_F(1) + 300.0f;

        cMtx_YrotS(*calc_mtx, actor->current.angle.y);

        MtxPosition(&unkXyz1, &unkXyz2);

        cLib_addCalc2(&actor->current.pos.x, actor->home.pos.x + unkXyz2.x, 0.1f, 10.0f);
        cLib_addCalc2(&actor->current.pos.y, actor->home.pos.y + unkXyz2.y, 0.1f, 10.0f);
        cLib_addCalc2(&actor->current.pos.z, actor->home.pos.z + unkXyz2.z, 0.1f, 10.0f);

        if (i_this->field_0x69c[0] == 0) {
            i_this->mCurAction = 3;
            i_this->field_0x672 = 0;
        }

        break;
    case 10:
    case 11:
    case 12: {
        unkByte1 = 0;
        i_this->field_0xe8c = 0;

        unkXyz1.x = 0.0f;
        unkXyz1.y = 0.0f;
        unkXyz1.z = 0.0f;

        cMtx_YrotS(*calc_mtx, actor->current.angle.y);
        MtxPosition(&unkXyz1, &unkXyz2);

        unkFloat1 = 80.0f;
        if (i_this->field_0x672 == 10) {
            unkFloat1 = 300.0f;
        }
        i_this->field_0x678.x = actor->home.pos.x + unkXyz2.x;
        i_this->field_0x678.y = actor->home.pos.y + unkFloat1;
        i_this->field_0x678.z = actor->home.pos.z + unkXyz2.z;

        i_this->field_0x684 = 20.0f;
        i_this->field_0x688 = 5.0f;
        i_this->field_0x694 = 2000.0f;

        f32 diffX = i_this->field_0x678.x - actor->current.pos.x;
        f32 diffY = i_this->field_0x678.y - actor->current.pos.y;
        f32 diffZ = i_this->field_0x678.z - actor->current.pos.z;

        f32 diff = JMAFastSqrt(diffX * diffX + diffY * diffY + diffZ * diffZ);
        if (i_this->field_0x672 == 10) {
            if (diff < 2000.0f) {
                i_this->field_0x672 = 11;
            }
        } else if (i_this->field_0x672 == 0xb) {
            if (diff < l_e_krHIO.field_0x50) {
                i_this->field_0x672 = 12;

                anm_init(i_this, 11, 10.0f, 0, 1.0f);
            }
        } else if (i_this->field_0x672 == 12) {
            i_this->field_0xea8 = KREG_F(0xb) + 1000.0f;

            if (diff < TREG_F(9) * 10.0f + 300.0f) {
                i_this->field_0x672 = 0x14;

                anm_init(i_this, 15, 5.0f, 2, l_e_krHIO.field_0x44);

                i_this->field_0x670 = 10;
                actor->speed.y = 0.0f;
                i_this->field_0x6d8 = 0;
                unkByte1 = -1;
            }
        }
        break;
    }
    case 20: {
        i_this->field_0x91c = 1;
        i_this->field_0xe8c = 0;

        s32 unkInt1 = (s32)i_this->field_0x6d8;
        i_this->field_0x6d8++;

        cLib_addCalc2(&actor->current.pos.x, actor->home.pos.x, 0.1f, unkFloat1);
        cLib_addCalc2(&actor->current.pos.z, actor->home.pos.z, 0.1f, unkFloat2);

        actor->current.pos.y = actor->current.pos.y + actor->speed.y;

        if (unkInt1 > l_e_krHIO.field_0x40) {
            actor->speed.y = actor->speed.y - (TREG_F(7) + 0.8f);
        }

        cLib_addCalcAngleS2(&actor->current.angle.x, 0, 5, 0x800);
        cLib_addCalcAngleS2(&actor->current.angle.y, actor->home.angle.y, 5, 0x300);
        cLib_addCalcAngleS2(&actor->current.angle.z, 0, 5, 0x800);

        if (actor->current.pos.y <= actor->home.pos.y) {
            actor->current.pos.y = actor->home.pos.y;
            actor->speed.y = -0.5f;

            if (unkInt1 > l_e_krHIO.field_0x40) {
                anm_init(i_this, 19, 5.0f, 0, l_e_krHIO.field_0x48);

                i_this->field_0x672 = -1;
            }
        }
        break;
    }
    }

    switch (unkByte1) {
    case 0:
        e_kr_pos_move(i_this);
        break;
    }
}

static void body_smoke_set(e_kr_class* i_this) {
    cXyz unkXyz1;
    cXyz unkXyz2;

    MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(1), *calc_mtx);
    unkXyz1.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&unkXyz1, &unkXyz2);

    fopAcM_effSmokeSet1(&i_this->field_0xec4[0], &i_this->field_0xed8[0], &unkXyz2, NULL,
                        1.2f + XREG_F(4), &i_this->enemy.tevStr, 1);
}

static void wing_smoke_set(e_kr_class* i_this) {
    static int wing_j[4] = {0x0C, 0x0D, 0x11, 0x12};

    cXyz unkXyz1;
    cXyz unkXyz2;

    for (s32 i = 0; i <= 3; i++) {
        MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(wing_j[i]), *calc_mtx);
        if (i <= 1) {
            unkXyz1.set(0.0f, 0.0f, 50.0f);
        } else {
            unkXyz1.set(0.0f, 0.0f, -50.0f);
        }
        MtxPosition(&unkXyz1, &unkXyz2);

        fopAcM_effSmokeSet2(&i_this->field_0xec4[i + 1], &i_this->field_0xed8[i + 1], &unkXyz2, 0,
                            1.5f + XREG_F(3), &i_this->enemy.tevStr);
    }
}

static s8 e_kr_damage(e_kr_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)actor;
    cXyz unkXyz1;
    s32 unkInt1 = 0;

    i_this->field_0xe8c = 0;
    i_this->field_0x6c8 = 10;
    actor->speedF = 0.0f;
    cLib_addCalcAngleS2(&actor->current.angle.x, 0, 1, 0x800);

    if (enemy->checkCutDownHitFlg()) {
        enemy->offCutDownHitFlg();
        i_this->field_0x672 = 3;
        i_this->field_0xefc = 1;
        i_this->field_0x69c[0] = 0x46;

        i_this->mSound.startCreatureVoice(Z2SE_EN_KR_V_DEATH2, -1);

        enemy->offDownFlg();

        i_this->field_0xea8 = 5000.0f + KREG_F(17);
        i_this->field_0xeae = i_this->field_0xeb0 = 0;
        i_this->field_0xeb2 = cM_rndF(400.0f);
        i_this->field_0xeb4 = cM_rndF(400.0f);
        i_this->field_0xebe = 15 + TREG_S(7);
        actor->health = 0;
    }

    s8 retVal = 1;

    switch (i_this->field_0x672) {
    case 0: {
        anm_init(i_this, 6, 0.0f, 0, 1.0f);
        i_this->field_0x672 = 1;
        unkXyz1.x = 0.0f;
        unkXyz1.y = 30.0f + BREG_F(4) + cM_rndF(10.0f);
        unkXyz1.z = -i_this->field_0x6d0;
        cMtx_YrotS(*calc_mtx, i_this->field_0x6ca);
        MtxPosition(&unkXyz1, &actor->speed);
        i_this->field_0xeb6 = 8000;
        i_this->field_0x69c[1] = 90;
    }
    case 1: {
        actor->current.pos += actor->speed;
        actor->speed.y += -3.0f;

        if (actor->speed.y < 0.0f) {
            actor->speed.x *= 0.96f;
            actor->speed.z *= 0.96f;
            cLib_addCalcAngleS2(&i_this->field_0x6ea.x, -0x4000, 4, (BREG_S(7) + 0x180));
        }

        i_this->field_0xea8 = KREG_F(14) + 3000.0f;
        actor->current.angle.y = actor->current.angle.y + i_this->field_0x6d4;

        if (i_this->mAcch.ChkGroundHit()) {
            i_this->field_0x672 = 2;
            i_this->field_0xea8 = 5000.0f + KREG_F(17);
            i_this->field_0xeb8 = 8000.0f + KREG_F(19);
            actor->speed.y = 15.0f + BREG_F(8);
            actor->speed.x *= 0.4f;
            actor->speed.z *= 0.4f;
            actor->current.pos += actor->speed;

            body_smoke_set(i_this);

            if (actor->health > 0) {
                enemy->onDownFlg();
            }
        }

        break;
    }
    case 2: {
        actor->current.pos += actor->speed;
        actor->speed.y += -3.0f;

        if (i_this->mAcch.ChkGroundHit()) {
            actor->speed.x = 0.0f;
            actor->speed.z = 0.0f;
            i_this->field_0x6d0 = 0;
            i_this->field_0x672 = 3;
            i_this->field_0x69c[0] = 0x50;
            i_this->field_0xea8 = KREG_F(0x11) + 5000.0f;
            i_this->field_0xeb2 = cM_rndF(200.0f);
            i_this->field_0xeb4 = cM_rndF(200.0f);

            body_smoke_set(i_this);
        }

        cLib_addCalcAngleS2(&i_this->field_0x6ea.x, -0x4000, 1, 0x800);

        break;
    }
    case 3: {
        if (actor->health > 0) {
            i_this->mSound.startCreatureSoundLevel(Z2SE_EN_KR_V_DOWN, 0, -1);
        } else {
            retVal = 0;
        }

        if (i_this->field_0x69c[0] == (s16)(70 + XREG_S(0))) {
            wing_smoke_set(i_this);
        }

        cLib_addCalcAngleS2(&i_this->field_0x6ea.x, -0x4000, 1, 0x800);
        cLib_addCalcAngleS2(&i_this->field_0xeb6, 0, 8, 200);
        cLib_addCalcAngleS2(&i_this->field_0xeae, 8000, 0x10, i_this->field_0xeb2);
        cLib_addCalcAngleS2(&i_this->field_0xeb0, 8000, 0x10, i_this->field_0xeb4);

        if (actor->health <= 0 && i_this->field_0x69c[0] == 0x14) {
            unkInt1 = 1;
        }

        if (i_this->field_0x69c[0] == 0) {
            i_this->field_0xeae = i_this->field_0xeb0 = 0;
            i_this->mCurAction = 3;
            i_this->field_0x672 = 0;
            i_this->field_0x69c[1] = 0;

            i_this->field_0x69c[1] =
                l_e_krHIO.field_0x1c + cM_rndF(l_e_krHIO.field_0x1e - l_e_krHIO.field_0x1c);
            i_this->field_0x6e1 = 0;
            i_this->field_0xec0 = 1;
            actor->current.angle.x = -0x3000;

            i_this->mSound.startCreatureVoice(Z2SE_EN_KR_V_AWAKE, -1);

            enemy->offDownFlg();
        }
    }
    }

    if (actor->speed.y < -30.0f) {
        actor->speed.y = -30.0f;
    }

    if (enemy->checkDownFlg()) {
        enemy->setDownPos(&actor->current.pos);
    }

    if (i_this->field_0xebe != 0) {
        i_this->field_0xebe--;
        i_this->field_0xebc =
            (100.0f + TREG_F(5)) *
            (i_this->field_0xebe * cM_ssin(i_this->field_0xebe * (0x3800 + TREG_S(9))));
    }

    if (actor->health <= 0 && i_this->field_0x69c[1] == 0 || unkInt1 != 0) {
        fopAcM_createDisappear(actor, &actor->current.pos, 0xf, 0, 10);
        fopAcM_delete(actor);

        if (i_this->field_0x667 != 0xff) {
            dComIfGs_onSwitch(i_this->field_0x667, fopAcM_GetRoomNo(actor));
        }
    }

    return retVal;
}

static void damage_check(e_kr_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    fopAc_ac_c* actor = &i_this->enemy;

    i_this->mStts.Move();

    if ((i_this->mSphere3.ChkTgHit() || i_this->mSphere2.ChkTgHit()) && i_this->field_0x6c8 == 0) {
        if (i_this->mSphere3.ChkTgHit()) {
            i_this->mAtInfo.mpCollider = i_this->mSphere3.GetTgHitObj();
        } else if (i_this->mSphere2.ChkTgHit()) {
            i_this->mAtInfo.mpCollider = i_this->mSphere2.GetTgHitObj();
            s16 health = actor->health; // debug match
        }

        if (i_this->mAtInfo.mpCollider->ChkAtType(0xd8000000)) {
            i_this->field_0x6c8 = 20;
        } else {
            i_this->field_0x6c8 = 10;
        }

        i_this->mAtInfo.mpActor = cc_at_check(actor, &i_this->mAtInfo);
        if (i_this->field_0x66b != 0) {
            dBomb_c* bomb = (dBomb_c*)fopAcM_SearchByID(i_this->mBombId);

            if (bomb != NULL) {
                bomb->setCargoBombExplode();
            }

            i_this->field_0x66b = 0;
            i_this->field_0xf00 = -1;
            i_this->field_0xf04 = 0;
            i_this->field_0x6ca = actor->shape_angle.y;
            i_this->field_0x6cc = 0;
            i_this->mCurAction = 9;
            i_this->field_0x672 = 0;
            i_this->field_0x6d0 = l_e_krHIO.field_0x58;

            if (cM_rndF(1.0f) < 0.5f) {
                i_this->field_0x6d4 = cM_rndF(1000.0f) + 1000.0f;
            } else {
                i_this->field_0x6d4 = -(cM_rndF(1000.0f) + 1000.0f);
            }
            i_this->mSound.startCreatureVoice(Z2SE_EN_KR_V_DEATH, -1);
            i_this->field_0xefc = 1;
            actor->health = 0;
        } else {
            dKy_Sound_set(actor->current.pos, 100, fopAcM_GetID(i_this), 5);
            if (i_this->mAtInfo.mAttackPower == 0) {
                i_this->mCurAction = 3;
                i_this->field_0x672 = 10;
                i_this->field_0x69c[0] = l_e_krHIO.field_0x34;

                anm_init(i_this, 16, 0.0f, 0, l_e_krHIO.field_0x38);

                i_this->field_0x684 = TREG_F(7);
                i_this->field_0x688 = 1.0f;
                actor->speedF = -20.0f;
                i_this->mSound.startCreatureVoice(Z2SE_EN_KR_V_DAMAGE_S, -1);
            } else {
                if (i_this->mAtInfo.mHitType == 1) {
                    i_this->field_0x6ca = i_this->field_0x6c0;
                    i_this->field_0x6cc = i_this->field_0x6c2;
                } else {
                    i_this->field_0x6ca = i_this->mAtInfo.mHitDirection.y;
                    i_this->field_0x6cc = 0;
                }

                i_this->mCurAction = 9;
                i_this->field_0x672 = 0;

                if (i_this->mAtInfo.mHitStatus != 0) {
                    i_this->field_0x6d0 = l_e_krHIO.field_0x58;
                } else {
                    i_this->field_0x6d0 = l_e_krHIO.field_0x5c;
                }

                if (cM_rndF(1.0f) < 0.5f) {
                    i_this->field_0x6d4 = cM_rndF(1000.0f) + 1000.0f;
                } else {
                    i_this->field_0x6d4 = -(cM_rndF(1000.0f) + 1000.0f);
                }

                if (actor->health <= 0) {
                    i_this->mSound.startCreatureVoice(Z2SE_EN_KR_V_DEATH, -1);
                    i_this->field_0xefc = 1;
                } else {
                    i_this->mSound.startCreatureVoice(Z2SE_EN_KR_V_DAMAGE, -1);
                }
            }
        }
    }
}

static void ground_angle_set(e_kr_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;

    cXyz unkXyz1;
    cXyz unkXyz2;

    Vec unkVec1;

    s16 unkShort1 = 0;
    s16 unkShort2 = 0;

    f32 unkFloat1;
    f32 diffY;
    f32 diffZ;

    if (i_this->field_0x6ea.x <= -0x1000) {
        J3DModel* model;
        cXyz unkXyz3;
        cXyz unkXyz4;
        dBgS_GndChk gndChk;

        unkFloat1 = 75.0f;

        MTXCopy(i_this->mpMorf->getModel()->getAnmMtx(0), *calc_mtx);
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
                diffY = unkVec1.y - unkXyz3.y;
                diffZ = unkVec1.z - unkXyz3.z;
                unkShort2 = -cM_atan2s(diffY, diffZ);
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
                diffY = unkVec1.y - unkXyz3.y;
                diffZ = unkVec1.x - unkXyz3.x;
                unkShort1 = (s16)cM_atan2s(diffY, diffZ);
                if (unkShort1 > 0x3000 || unkShort1 < -0x3000) {
                    unkShort1 = 0;
                }
            }
        }

        dBgS_LinChk linChk;
        cXyz unkXyz5;
        cXyz unkXyz6;
        cXyz unkXyz7;

        unkXyz6 = actor->current.pos;
        unkXyz6.y += JREG_F(5) + 30.0f;

        model = i_this->mpMorf->getModel();

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
            unkXyz5.z = 50.0f + TREG_F(11);
            MtxPosition(&unkXyz5, &unkXyz2);
            actor->current.pos += unkXyz2;
        }
    }

    cLib_addCalcAngleS2(&i_this->field_0x6f0.x, unkShort2, 1, 0x400);
    cLib_addCalcAngleS2(&i_this->field_0x6f0.z, unkShort1, 1, 0x400);
}

static void action(e_kr_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)&i_this->enemy;

    fopAc_ac_c* playerActor = dComIfGp_getPlayer(0);
    daPy_py_c* player = (daPy_py_c*)playerActor;

    cXyz unkXyz1;
    cXyz unkXyz2;

    enemy->attention_info.distances[fopAc_attn_BATTLE_e] = 3;

    s8 unkByte1 = 0;
    s8 unkByte2 = 1;
    s8 unkByte3 = 1;

    switch (i_this->mCurAction) {
    case 0:
        e_kr_auto_move(i_this);
        unkByte1 = 1;
        break;
    case 9:
        unkByte2 = e_kr_damage(i_this);
        unkByte3 = 0;
        break;
    case 8:
        enemy->attention_info.distances[fopAc_attn_BATTLE_e] = 22;
        e_kr_horse_move(i_this);
        unkByte1 = 1;
        break;
    case 3:
        e_kr_atack_move(i_this);
        unkByte1 = 1;
        break;
    case 4:
        e_kr_wait_move(i_this);
        break;
    case 7:
        e_kr_su_wait_move(i_this);
        break;
    case 10:
        enemy->attention_info.distances[fopAc_attn_BATTLE_e] = 22;
        unkByte2 = e_kr_path_move(i_this);
        unkByte1 = 1;
    }

    if (unkByte1) {
        i_this->mSound.setLinkSearch(true);
    } else {
        i_this->mSound.setLinkSearch(false);
    }

    if (unkByte2) {
        fopAcM_OnStatus(enemy, 0);
        enemy->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(enemy, 0);
        enemy->attention_info.flags = 0;
    }

    if (unkByte3) {
        cLib_addCalcAngleS2(&i_this->field_0x6ea.x, 0, 4, 0x800);
    }

    cXyz unkXyz3(1.3f, 1.3f, 1.3f);
    setMidnaBindEffect(enemy, &i_this->mSound, &enemy->eyePos, &unkXyz3);
}

static int daE_Kr_Execute(e_kr_class* i_this) {
    fopAc_ac_c* actor = &i_this->enemy;

    s16 unkShort1;
    s16 unkShort2;

    fopAc_ac_c* playerActor = dComIfGp_getPlayer(0);
    daPy_py_c* player = (daPy_py_c*)playerActor;
    
    cXyz unkXyz1;
    cXyz unkXyz2;
    f32 diffX = player->current.pos.x - actor->current.pos.x;
    f32 diffY = player->eyePos.y - actor->current.pos.y;
    f32 diffZ = player->current.pos.z - actor->current.pos.z;

    i_this->field_0x6c4 = JMAFastSqrt(diffX * diffX + diffZ * diffZ);
    i_this->field_0x6c0 = cM_atan2s(diffX, diffZ);
    i_this->field_0x6c2 = -cM_atan2s(diffY, i_this->field_0x6c4);
    i_this->field_0x6d6++;
    
    for (s32 i = 0; i < 6; i++) {
        if ((&i_this->field_0x69c[0])[i] != 0) {
            i_this->field_0x69c[i]--;
        }
    }
    
    if (i_this->field_0x6aa != 0) {
        i_this->field_0x6aa--;
    }
    if (i_this->field_0x6c8 != 0) {
        i_this->field_0x6c8--;
    }

    action(i_this);

    i_this->mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));

    if (i_this->field_0x5b8 == 0xd) {
        i_this->mSound.startCreatureSoundLevel(0x7002f, 0, -1);
    } else {
        if (i_this->mpMorf->checkFrame(27.0f) && i_this->field_0x5b8 == 8) {
            i_this->mSound.startCreatureSound(Z2SE_EN_KR_WING, 0, -1);
        } else if (i_this->mpMorf->checkFrame(11.0f) && i_this->field_0x5b8 == 15) {
            i_this->mSound.startCreatureSound(Z2SE_EN_KR_WING, 0, -1);
        } else if (i_this->mpMorf->checkFrame(12.0f) && i_this->field_0x5b8 == 17) {
            i_this->mSound.startCreatureSound(Z2SE_EN_KR_WING, 0, -1);
        }
    }

    actor->shape_angle = actor->current.angle + i_this->field_0x6ea;

    ground_angle_set(i_this);

    MtxTrans(actor->current.pos.x,
             actor->current.pos.y,
             actor->current.pos.z, 0);
    cMtx_XrotM(*calc_mtx, i_this->field_0x6f0.x);
    cMtx_ZrotM(*calc_mtx, i_this->field_0x6f0.z);
    cMtx_YrotM(*calc_mtx, actor->shape_angle.y);
    cMtx_XrotM(*calc_mtx, actor->shape_angle.x);
    cMtx_ZrotM(*calc_mtx, actor->shape_angle.z);
    MtxScale(l_e_krHIO.field_0x14, l_e_krHIO.field_0x14,
             l_e_krHIO.field_0x14, 1);

    J3DModel* model = i_this->mpMorf->getModel();
    model->setBaseScale(actor->scale);
    model->setBaseTRMtx(*calc_mtx);

    i_this->mpMorf->modelCalc();
    MTXCopy(model->getAnmMtx(6), *calc_mtx);
    unkXyz1.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&unkXyz1, &actor->eyePos);

    actor->attention_info.position = actor->eyePos;
    actor->attention_info.position.y += 10.0f * TREG_F(8) + 50.0f;

    MTXCopy(model->getAnmMtx(22), *calc_mtx);
    MtxPosition(&unkXyz1, i_this->field_0xe64);

    MTXCopy(model->getAnmMtx(27), *calc_mtx);
    MtxPosition(&unkXyz1, &i_this->field_0xe64[1]);

    if ((i_this->field_0x66b != 0)) {
        fopAc_ac_c* bomb = fopAcM_SearchByID(i_this->mBombId);
        if (bomb != NULL && i_this->field_0x66b != 0)
        {
            bomb->current.pos = i_this->field_0xe64[0] + (i_this->field_0xe64[1] - i_this->field_0xe64[0]) / 2.0f;
            bomb->current.pos.y += l_e_krHIO.field_0x78;
            bomb->current.angle = actor->current.angle;
            bomb->speedF = 0.0f;
        }
    }
    if (i_this->field_0x6c8 != 0) {
        cXyz unkXyz3(actor->eyePos.x - 10000.0f, 10000.0f, 200000.0f);
        i_this->mSphere2.SetC(unkXyz3);
        i_this->mSphere3.SetC(unkXyz3);
        i_this->mSphere4.SetC(unkXyz3);
    } else {
        i_this->mSphere2.SetC(actor->eyePos);
        i_this->mSphere3.SetC(actor->current.pos);
        i_this->mSphere4.SetC(actor->current.pos);
    }

    dComIfG_Ccsp()->Set(&i_this->mSphere2);
    dComIfG_Ccsp()->Set(&i_this->mSphere3);
    dComIfG_Ccsp()->Set(&i_this->mSphere4);

    f32 unkFloat1;
    cXyz* unkXyzP1 = i_this->mStts.GetCCMoveP();
    if (unkXyzP1 != NULL) {
        unkFloat1 = 0.3f + TREG_F(0x13);
        actor->current.pos.x += unkXyzP1->x * unkFloat1;
        actor->current.pos.y += unkXyzP1->y * unkFloat1;
        actor->current.pos.z += unkXyzP1->z * unkFloat1;
    }

    if (i_this->field_0x91c == 0) {
        unkFloat1 = 70.0f + TREG_F(15);
        if (i_this->mCurAction == 9) {
            unkFloat1 = 5.0f + TREG_F(14);
        }

#if VERSION == VERSION_SHIELD_DEBUG
        actor->current.pos.y = actor->current.pos.y - unkFloat1;
        actor->old.pos.y = actor->old.pos.y - unkFloat1;
        i_this->mAcch.CrrPos(dComIfG_Bgsp());
        actor->current.pos.y = actor->current.pos.y + unkFloat1;
        actor->old.pos.y = actor->old.pos.y + unkFloat1;
#else

        cLib_addCalc2(&i_this->field_0x920, unkFloat1, 1.0f, 2.0f);

        actor->current.pos.y = actor->current.pos.y - i_this->field_0x920;
        actor->old.pos.y = actor->old.pos.y - i_this->field_0x920;
        i_this->mAcch.CrrPos(dComIfG_Bgsp());
        actor->current.pos.y = actor->current.pos.y + i_this->field_0x920;
        actor->old.pos.y = actor->old.pos.y + i_this->field_0x920;
#endif
    }

    i_this->field_0x91c = false;

    damage_check(i_this);

    s16 unkShort3 = 0x800;
    if (i_this->field_0xe8c != 0) {
        if (i_this->field_0xe8c == 1) {
            diffX = player->current.pos.x - actor->current.pos.x;
            diffZ = player->current.pos.z - actor->current.pos.z;
            s16 diff = cM_atan2s(diffX, diffZ);
            i_this->field_0xe7e = actor->current.angle.y - diff;
            if (i_this->field_0xe7e > 0x2710) {
                i_this->field_0xe7e = 10000;
            } else {
                if (i_this->field_0xe7e < -10000) {
                    i_this->field_0xe7e = 0xd8f0;
                }
            }
        } else if (i_this->field_0xe8c == 2) {
            unkShort3 = 0x200;
            if ((i_this->field_0x6d6 & 0x1f) == 0 && cM_rndF(1.0f) < 0.5f) {
                i_this->field_0xe7e = cM_rndFX(10000.0f);
            }
            if ((i_this->field_0x6d6 & 0x3f) == 0 && cM_rndF(1.0f) < 0.5f) {
                kuti_open(i_this, 0x19, 0x7002b);
            }
        }
        i_this->field_0xe8c = 0;
    } else {
        i_this->field_0xe7e = 0;
    }

    cLib_addCalcAngleS2(&i_this->field_0xe7c, -(i_this->field_0xe7e / 2), 2, unkShort3);
    s16 unkShort4 = 0;
    if (i_this->field_0xe82 != 0) {
        i_this->field_0xe82--;
        unkShort4 = 10000;
        if (i_this->field_0xe82 == i_this->field_0xe84 && i_this->field_0xe88 != 0)
        {
            i_this->mSound.startCreatureVoice(i_this->field_0xe88, -1);
        }
    }

    cLib_addCalcAngleS2(&i_this->field_0xe80, unkShort4, 2, 2000);
    for (s32 i = 0; i <= 8; i++) {
        i_this->field_0xe8e[i] = (i_this->field_0xea8 * cM_ssin(i_this->field_0x6d6 * (7000 + KREG_S(2)) + i * (11000 + KREG_S(3))));
    }

    cLib_addCalc0(&i_this->field_0xea8, 1.0f, TREG_F(0x12) + 200.0f);
    if (i_this->field_0x6d0 > 0.01f) {
        s16 unkShort5 = (s16)i_this->field_0x6ca - actor->shape_angle.y;
        if (i_this->field_0x6ea.x <= -0x1000) {
            unkShort5 = 0;
        }
        unkShort2 = -unkShort5 + 0x4000;
        if (unkShort2 > 0x3a98) {
            unkShort2 = 15000;
        } else if (unkShort2 < -15000) {
            unkShort2 = -15000;
        }
        unkShort1 = unkShort5 + 0x4000;
        if (unkShort1 > 0x3a98) {
            unkShort1 = 15000;
        } else if (unkShort1 < -15000) {
            unkShort1 = -15000;
        }
    } else {
        unkShort2 = unkShort1 = 0;
    }

    cLib_addCalcAngleS2(&i_this->field_0xe8e[9], unkShort2, 4, 0x800);
    cLib_addCalcAngleS2(&i_this->field_0xe8e[10], unkShort1, 4, 0x800);
    i_this->field_0xeac = i_this->field_0xeb8 * cM_ssin(i_this->field_0x6d6 * (NREG_S(2) + 7000));
    cLib_addCalc0(&i_this->field_0xeb8, 1.0f, NREG_F(0x12) + 300.0f);
    if (i_this->field_0xefc != 0) {
        cLib_addCalc2(&i_this->field_0xef8, -20.0f, 1.0f, 0.4f);
    }

    return 1;
}

static int daE_Kr_IsDelete(e_kr_class* i_this) {
    return 1;
}

static int daE_Kr_Delete(e_kr_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "E_kr");
    fopEn_enemy_c* enemy = &i_this->enemy;
#if DEBUG
    //l_e_krHIO.removeHIO(*enemy); //TODO: several missing inline functions
#endif
    if (enemy->heap != NULL) {
        i_this->mpMorf->stopZelAnime();
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_kr_class* kr = (e_kr_class*)i_this;

    kr->mpMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("E_kr", 24), NULL, NULL,
                                      (J3DAnmTransform*)dComIfG_getObjectRes("E_kr", 13), 2, 1.0f,
                                      0, -1, &((e_kr_class*)i_this)->mSound, 0x80000, 0x11000084);
    if (kr->mpMorf == NULL || kr->mpMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = kr->mpMorf->getModel();
    model->setUserArea((int)kr);

    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        if (i != 0) {
            model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
        }
    }

    return 1;
}

static int daE_Kr_Create(fopAc_ac_c* i_this) {
    static dCcD_SrcSph at_sph_src = {
        {
            {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0xd}, {0x0, 0x0}, 0x0}},  // mObj
            {dCcD_SE_METAL, 0x0, 0x0, 0x0, 0x1},                          // mGObjAt
            {dCcD_SE_HARD_BODY, 0x0, 0x0, 0x0, 0x0},                      // mGObjTg
            {0x0},                                                        // mGObjCo
        },                                                                // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 25.0f}  // mSph
        }  // mSphAttr
    };
    static dCcD_SrcSph head_tg_sph_src = {
        {
            {0x0, {{0x0, 0x1, 0x0}, {(s32)0xd8fbfdff, 0x3}, 0x0}},  // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                     // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},                     // mGObjTg
            {0x0},                                                  // mGObjCo
        },                                                          // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 62.5f}  // mSph
        }  // mSphAttr
    };

    static dCcD_SrcSph body_tg_sph_src = {
        {
            {0x0, {{0x0, 0x1, 0x0}, {(s32)0xd8fbfdff, 0x3}, 0x0}},  // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},                // mGObjTg
            {0x0},                                             // mGObjCo
        },                                                     // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 65.0f}  // mSph
        }  // mSphAttr
    };
    static dCcD_SrcSph body_co_sph_src = {
        {
            {0x0, {{0x0, 0x1, 0x0}, {0x0, 0x0}, 0x75}},  // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},          // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},          // mGObjTg
            {0x0},                                       // mGObjCo
        },                                               // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 80.0f}  // mSph
        }  // mSphAttr
    };

    e_kr_class* kr = (e_kr_class*)i_this;

    s32 loadResult = dComIfG_resLoad(&kr->mPhase, "E_kr");
    fopAcM_ct(i_this, e_kr_class);

    if (loadResult == cPhs_COMPLEATE_e) {
        OS_REPORT("PARAM %x\n", fopAcM_GetParam(i_this));

        kr->field_0x664 = (u8)fopAcM_GetParam(i_this);
        if (kr->field_0x664 == 0xff) {
            kr->field_0x664 = 0;
        }

        kr->mCurAction = kr->field_0x664;

        kr->field_0x665 = (u8)(fopAcM_GetParam(i_this) >> 8);
        kr->field_0x666 = (u8)(fopAcM_GetParam(i_this) >> 16);
        kr->field_0x667 = kr->enemy.home.angle.z;
        i_this->current.angle.z = 0;

        if (kr->field_0x667 != 0xff) {
            if (dComIfGs_isSwitch(kr->field_0x667, fopAcM_GetRoomNo(&kr->enemy))) {
                // "I'm not going to reset it because I've been defeated"
                OS_REPORT("E_KR やられ後なので再セットしません\n");

                return cPhs_ERROR_e;
            }
        }

        if (fopAcM_entrySolidHeap(i_this, useHeapInit, 0x21c0) == 0) {
            return cPhs_ERROR_e;
        }

        i_this->current.pos.y += 70.0f + TREG_F(5);
        i_this->home.pos = i_this->current.pos;
        if (kr->field_0x666 != 0xff) {
            kr->field_0x6e4 = dPath_GetRoomPath(kr->field_0x666, fopAcM_GetRoomNo(i_this));
            if (kr->field_0x6e4 == NULL) {
                return 5;
            }

            kr->field_0x6e1 = kr->field_0x666 + 1;
            kr->field_0x6e3 = 1;
            kr->mCurAction = 10;
            kr->field_0x672 = -10;
            kr->field_0x674 = 0x01;
            i_this->health = 0x28;
        } else {
            i_this->field_0x560 = i_this->health = 0x28;
        }
        kr->field_0xf00 = -1;

        OS_REPORT("");

        fopAcM_SetMtx(i_this, kr->mpMorf->getModel()->getBaseTRMtx());

        fopAcM_SetMin(i_this, -300.0f, -200.0f, -300.0f);
        fopAcM_SetMax(i_this, 300.0f, 200.0f, 300.0f);

        kr->mAcch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1,
                      &kr->mAcchCir, fopAcM_GetSpeed_p(i_this), NULL, NULL);

        kr->mAcchCir.SetWall(100.0f, 100.0f);

        kr->mStts.Init(100, 0xff, i_this);

        kr->mSphere1.Set(at_sph_src);
        kr->mSphere2.Set(head_tg_sph_src);
        kr->mSphere3.Set(body_tg_sph_src);
        kr->mSphere4.Set(body_co_sph_src);

        kr->mSphere1.SetStts(&kr->mStts);
        kr->mSphere2.SetStts(&kr->mStts);
        kr->mSphere3.SetStts(&kr->mStts);
        kr->mSphere4.SetStts(&kr->mStts);

        kr->field_0x69c[3] = 300.0f + cM_rndF(200.0f);
        kr->field_0x6d6 = cM_rndF(10000.0f);
        i_this->attention_info.distances[fopAc_attn_BATTLE_e] = 22;

        kr->mSound.init(&i_this->current.pos, &i_this->eyePos, 3, 1);
        kr->mSound.setEnemyName("E_kr");
        kr->mAtInfo.mpSound = &kr->mSound;
        kr->mAtInfo.mPowerType = 1;
        fopAcM_OnStatus(i_this, 0);
        daE_Kr_Execute(kr);
    }
    return loadResult;
}

e_kr_class::e_kr_class() {
}

static actor_method_class l_daE_Kr_Method = {
    (process_method_func)daE_Kr_Create,  (process_method_func)daE_Kr_Delete,
    (process_method_func)daE_Kr_Execute, (process_method_func)daE_Kr_IsDelete,
    (process_method_func)daE_Kr_Draw,
};

extern actor_process_profile_definition g_profile_E_KR = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_E_KR,               // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(e_kr_class),      // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    186,                     // mPriority
    &l_daE_Kr_Method,        // sub_method
    0x10040120,              // mStatus
    fopAc_ENEMY_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
