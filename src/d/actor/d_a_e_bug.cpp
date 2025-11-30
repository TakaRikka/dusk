/**
* @file d_a_e_bug.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_bug.h"
#include "d/actor/d_a_player.h"
#include "d/d_path.h"
#include "d/actor/d_a_nbomb.h"
#include "Z2AudioLib/Z2Instances.h"

enum E_bug_RES_File_ID {
    /* BMDG */
    /* 0x3 */ BMDG_MU04 = 0x3,
    /* 0x4 */ BMDG_MU05,
};

daE_Bug_HIO_c::daE_Bug_HIO_c() {
    field_0x4 = -1;
    field_0x8 = 1.5f;
    field_0xc = 1.0f;
}

/* 80697E6C 0001+00 data_80697E6C @1009 */
/* 80697E6D 0003+00 data_80697E6D None */
static u8 l_initHIO;

static daE_Bug_HIO_c l_HIO;

/* 80697E8C 0001+00 data_80697E8C None */
static s8 l_roomNo;

/* 80697E8D 0003+00 data_80697E8D None */
static s8 data_80697E8D;

static int daE_Bug_Draw(e_bug_class* i_this) {
    bug_s* bugs = i_this->Bug_s;

    for (int i = 0; i < i_this->bug_num; i++, bugs++) {
        if (bugs->field_0x50 == 4) {
            if ((bugs->field_0x8 & 1) != 0) {
                dComIfGp_entrySimpleModel(bugs->field_0x0, l_roomNo);
            } else {
                dComIfGp_entrySimpleModel(bugs->field_0x4, l_roomNo);
            }
        } else if (bugs->field_0x50 >= 1) {
            if ((bugs->field_0x52 & 2) != 0) {
                dComIfGp_entrySimpleModel(bugs->field_0x0, l_roomNo);
            } else {
                dComIfGp_entrySimpleModel(bugs->field_0x4, l_roomNo);
            }
        }
    }

    return 1;
}

static int simple_bg_check(bug_s* i_this, int param_2) {
    dBgS_LinChk lin_chk;
    cXyz start, end, spc0, spcc;

    spcc.y = 0.0f;
    int iVar1 = 0;
    f32 fVar1 = (i_this->field_0xc - i_this->field_0x18).abs() + 5.0f;
    cMtx_YrotS(*calc_mtx, i_this->field_0x3c.y);

    static f32 c_x[4] = {0.0f, 1.0f, -1.0f, 0.0f};
    static f32 c_z[4] = {2.0f, 0.0f, 0.0f, -2.0f};
    for (int i = 0; i < param_2 + 3; i++) {
        start = i_this->field_0x18;
        start.y += 20.0f;
        spcc.x = fVar1 * c_x[i];
        spcc.z = fVar1 * c_z[i];
        MtxPosition(&spcc, &spc0);
        end = start + spc0;
        lin_chk.Set(&start, &end, NULL);

        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            i_this->field_0x18.x -= spc0.x;
            i_this->field_0x18.z -= spc0.z;
            iVar1 = i + 1;
        }
    }

    return iVar1;
}

static void bug_mtxset(bug_s* i_this) {
    mDoMtx_stack_c::transS(i_this->field_0x18.x, i_this->field_0x18.y, i_this->field_0x18.z);
    mDoMtx_stack_c::XrotM(i_this->field_0x44);
    mDoMtx_stack_c::ZrotM(i_this->field_0x46);
    mDoMtx_stack_c::YrotM(i_this->field_0x3c.y);
    mDoMtx_stack_c::XrotM(i_this->field_0x3c.x);
    mDoMtx_stack_c::scaleM(l_HIO.field_0x8 * i_this->field_0x28, l_HIO.field_0x8 * i_this->field_0x28, l_HIO.field_0x8 * i_this->field_0x28);

    if ((i_this->field_0x52 & 2) != 0) {
        i_this->field_0x0->setBaseTRMtx(mDoMtx_stack_c::get());
    } else {
        i_this->field_0x4->setBaseTRMtx(mDoMtx_stack_c::get());
    }
}

static void bug_mtxset_stick(bug_s* i_this) {
    cXyz sp2c;
    mDoMtx_stack_c::transS(i_this->field_0x18.x, i_this->field_0x18.y, i_this->field_0x18.z);
    mDoMtx_stack_c::YrotM(i_this->field_0x3c.y);
    sp2c = i_this->field_0x18 - i_this->field_0xc;

    s16 sVar1 = cM_atan2s(JMAFastSqrt(sp2c.x * sp2c.x + sp2c.z * sp2c.z), sp2c.y);
    if (i_this->field_0x4a < 0) {
        sVar1 *= -1;
    }

    cLib_addCalcAngleS2(&i_this->field_0x3c.z, sVar1, 2, 0x1000);
    mDoMtx_stack_c::ZrotM(i_this->field_0x3c.z);
    mDoMtx_stack_c::XrotM(-0x4000);
    mDoMtx_stack_c::scaleM(l_HIO.field_0xc * i_this->field_0x28, l_HIO.field_0xc * i_this->field_0x28, l_HIO.field_0xc * i_this->field_0x28);
    mDoMtx_stack_c::transM(0.0f, 2.0f, 0.0f);

    if ((i_this->field_0x52 & 2) != 0) {
        i_this->field_0x0->setBaseTRMtx(mDoMtx_stack_c::get());
    } else {
        i_this->field_0x4->setBaseTRMtx(mDoMtx_stack_c::get());
    }
}

static void bug_mtxset_stickW(bug_s* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp38, sp44, sp50;

    sp38 = i_this->field_0x18 - i_this->field_0xc;
    s16 sVar1 = cM_atan2s(JMAFastSqrt(sp38.x * sp38.x + sp38.z * sp38.z), sp38.y);
    if (i_this->field_0x4a < 0) {
        sVar1 *= -1;
    }

    MTXCopy(daPy_getLinkPlayerActorClass()->getModelJointMtx(1), *calc_mtx);
    sp38.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp38, &sp44);
    sp50 = i_this->field_0x18 - sp44;
    mDoMtx_stack_c::transS(sp44.x, sp44.y, sp44.z);
    mDoMtx_stack_c::YrotM(player->shape_angle.y);
    mDoMtx_stack_c::XrotM(-14000);
    mDoMtx_stack_c::transM(sp50.x, sp50.y, sp50.z);
    mDoMtx_stack_c::YrotM(i_this->field_0x3c.y);
    cLib_addCalcAngleS2(&i_this->field_0x3c.z, sVar1, 2, 0x1000);
    mDoMtx_stack_c::ZrotM(i_this->field_0x3c.z);
    mDoMtx_stack_c::XrotM(-0x4000);
    mDoMtx_stack_c::scaleM(l_HIO.field_0xc * i_this->field_0x28, l_HIO.field_0xc * i_this->field_0x28, l_HIO.field_0xc * i_this->field_0x28);
    mDoMtx_stack_c::transM(0.0f, 2.0f, 0.0f);

    if ((i_this->field_0x52 & 2) != 0) {
        i_this->field_0x0->setBaseTRMtx(mDoMtx_stack_c::get());
    } else {
        i_this->field_0x4->setBaseTRMtx(mDoMtx_stack_c::get());
    }
}

static void bug_mtxset_fail(bug_s* i_this) {
    mDoMtx_stack_c::transS(i_this->field_0x18.x, i_this->field_0x18.y, i_this->field_0x18.z);
    mDoMtx_stack_c::XrotM(i_this->field_0x44);
    mDoMtx_stack_c::ZrotM(i_this->field_0x46);
    mDoMtx_stack_c::YrotM(i_this->field_0x3c.y);
    mDoMtx_stack_c::XrotM(i_this->field_0x3c.x);
    mDoMtx_stack_c::scaleM(l_HIO.field_0x8 * i_this->field_0x28, l_HIO.field_0x8 * i_this->field_0x28, l_HIO.field_0x8 * i_this->field_0x28);
    mDoMtx_stack_c::transM(0.0f, -4.0f, 0.0f);

    if ((i_this->field_0x8 & 1) != 0) {
        i_this->field_0x0->setBaseTRMtx(mDoMtx_stack_c::get());
    } else {
        i_this->field_0x4->setBaseTRMtx(mDoMtx_stack_c::get());
    }
}

static void bug_ground_ang_set(bug_s* i_this) {
    dBgS_LinChk lin_chk;
    cXyz sp8c, end, cross;

    cXyz start(i_this->field_0x18);
    start.y += 30.0f;

    mDoMtx_stack_c::transS(i_this->field_0x18.x, i_this->field_0x18.y, i_this->field_0x18.z);
    mDoMtx_stack_c::transM(5.0f, -30.0f, 0.0f);
    mDoMtx_stack_c::multVecZero(&end);
    mDoMtx_stack_c::transM(-10.0f, 0.0f, 0.0f);
    mDoMtx_stack_c::multVecZero(&cross);

    s8 sVar1 = 0;
    lin_chk.Set(&start, &end, NULL);

    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        end = lin_chk.GetCross();
        lin_chk.Set(&start, &cross, NULL);

        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            cross = lin_chk.GetCross();
            sVar1 = 1;
        }
    }

    if (sVar1) {
        sp8c = end - cross;
        i_this->field_0x46 = cM_atan2s(sp8c.y, JMAFastSqrt(sp8c.x * sp8c.x + sp8c.z * sp8c.z));
    }

    mDoMtx_stack_c::transM(5.0f, 0.0f, 5.0f);
    mDoMtx_stack_c::multVecZero(&end);
    mDoMtx_stack_c::transM(0.0f, 0.0f, -10.0f);
    mDoMtx_stack_c::multVecZero(&cross);
    sVar1 = 0;
    lin_chk.Set(&start, &end, NULL);

    if (dComIfG_Bgsp().LineCross(&lin_chk)) {
        end = lin_chk.GetCross();
        lin_chk.Set(&start, &cross, NULL);

        if (dComIfG_Bgsp().LineCross(&lin_chk)) {
            cross = lin_chk.GetCross();
            sVar1 = 1;
        }
    }

    if (sVar1) {
        sp8c = end - cross;
        i_this->field_0x44 = -cM_atan2s(sp8c.y, JMAFastSqrt(sp8c.x * sp8c.x + sp8c.z * sp8c.z));
    }
}

static int bug_action(e_bug_class* a_this, bug_s* i_this) {
    int rv = 0;
    i_this->field_0x18.x += i_this->field_0x30.x;
    i_this->field_0x18.y += i_this->field_0x30.y;
    i_this->field_0x18.z += i_this->field_0x30.z;
    
    i_this->field_0x30.y -= 3.0f;
    if (i_this->field_0x30.y < -60.0f) {
        i_this->field_0x30.y = -60.0f;
    }

    cXyz sp24(i_this->field_0x18.x, i_this->field_0x18.y + 70.0f, i_this->field_0x18.z);

    if (fopAcM_gc_c::gndCheck(&sp24)) {
        if (i_this->field_0x18.y <= fopAcM_gc_c::getGroundY()) {
            i_this->field_0x18.y = fopAcM_gc_c::getGroundY();
            i_this->field_0x30.y = -i_this->field_0x24 - 0.5f;

            if ((i_this->field_0x52 & 7) == 0) {
                bug_ground_ang_set(i_this);
            }

            rv = 1;
        }
    }

    return rv;
}

static cXyz at_pos;

static f32 at_size;

static s8 data_80697EAC;

static void bug_stick(bug_s* i_this) {
    cXyz sp30, sp3c, sp48;
    daPy_py_c* player = daPy_getLinkPlayerActorClass();
    
    MTXCopy(player->getModelJointMtx(1), *calc_mtx);
    sp30.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp30, &sp48);

    if ((i_this->field_0x52 & 15) == 0) {
        if (cM_rndF(1.0f) < 0.5f) {
            i_this->field_0x4a = cM_rndF(1000.0f) + 1000.0f;
        } else {
            i_this->field_0x4a = -(cM_rndF(1000.0f) + 1000.0f);
        }

        if (cM_rndF(1.0f) < 0.5f) {
            i_this->field_0x4e = cM_rndF(200.0f) + 600.0f;
        }
    }

    i_this->field_0x48 += i_this->field_0x4a;
    i_this->field_0x4c += i_this->field_0x4e;
    f32 fVar1 = cM_ssin(i_this->field_0x4c);
    cMtx_YrotS(*calc_mtx, i_this->field_0x48);
    sp30.x = 0.0f;
    sp30.y = fVar1 * 40.0f;
    sp30.z = 7.0f * fabsf(fVar1) + 13.0f;

    if (fabsf(sp30.y) > 30.0f) {
        sp30.z *= 0.6f;
    }

    MtxPosition(&sp30, &sp3c);
    i_this->field_0x18 = sp48 + sp3c;
    i_this->field_0x3c.y = i_this->field_0x48 + 0x8000;

    if ((i_this->field_0x52 & 15) == 0 && (daPy_getLinkPlayerActorClass()->checkFrontRoll() || daPy_getLinkPlayerActorClass()->checkMetamorphose() || 
        daPy_getPlayerActorClass()->eventInfo.checkCommandDoor() || data_80697EAC != 0)) {
        i_this->field_0x50 = 1;
        i_this->field_0x3c.y = i_this->field_0x48;
        i_this->field_0x30.y = cM_rndF(5.0f) + 30.0f;
        i_this->field_0x24 = cM_rndF(3.0f) + 9.0f;

        i_this->mSound.startSound(Z2SE_EN_BUG_JUMP, 0, -1);
        i_this->field_0x54[2] = cM_rndF(20.0f) + 20.0f;
    }
}

static void bug_stickW(bug_s* i_this) {
    cXyz sp34, sp40, sp4c;

    MTXCopy(daPy_getLinkPlayerActorClass()->getModelJointMtx(1), *calc_mtx);
    sp34.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp34, &sp4c);

    if ((i_this->field_0x52 & 15) == 0) {
        if (cM_rndF(1.0f) < 0.5f) {
            i_this->field_0x4a = cM_rndF(1000.0f) + 1000.0f;
        } else {
            i_this->field_0x4a = -(cM_rndF(1000.0f) + 1000.0f);
        }

        if (cM_rndF(1.0f) < 0.5f) {
            i_this->field_0x4e = cM_rndF(200.0f) + 600.0f;
        }
    }

    i_this->field_0x48 += i_this->field_0x4a;
    i_this->field_0x4c += i_this->field_0x4e;
    f32 fVar1 = cM_ssin(i_this->field_0x4c);
    cMtx_YrotS(*calc_mtx, i_this->field_0x48);
    sp34.x = 0.0f;
    sp34.y = (fVar1 * 41.0f - 5.0f) - 30.0f;
    sp34.z = -13.0f * fabsf(fVar1) + 13.0f + 10.0f;
    MtxPosition(&sp34, &sp40);
    i_this->field_0x18 = sp4c + sp40;
    i_this->field_0x3c.y = i_this->field_0x48 + 0x8000;
}

static void wind_move(bug_s* i_this) {
    s16 sVar1 = i_this->field_0x52 * -0x1700;
    i_this->field_0x3c.x -= 0xD00;
    i_this->field_0x3c.y += 0x700;
    i_this->field_0x30.x = cM_ssin(sVar1) * 25.0f;
    i_this->field_0x30.z = cM_scos(sVar1) * 25.0f;
    i_this->field_0x30.y = 20.0f;
    i_this->field_0x18 += i_this->field_0x30;

    if (i_this->field_0x54[0] == 0 || simple_bg_check(i_this, 1) != 0) {
        i_this->field_0x50 = 1;
        i_this->field_0x3c.y = sVar1;
        i_this->field_0x24 = cM_rndF(5.0f) + 8.0f;
    }
}

static void bug_fail(e_bug_class* a_this, bug_s* i_this) {
    i_this->field_0x53 = bug_action(a_this, i_this);

    if (i_this->field_0x51 == 0 && simple_bg_check(i_this, 1) != 0) {
        i_this->field_0x51 = 1;
        i_this->field_0x18 = i_this->field_0xc;
        i_this->field_0x30.x = 0.0f;
        i_this->field_0x30.z = 0.0f;
    }

    if (i_this->field_0x53 == 0) {
        i_this->field_0x3c.x += 0x2000;
        i_this->field_0x3c.y += 0x1300;
    } else {
        i_this->field_0x30.set(0.0f, 0.0f, 0.0f);
        i_this->field_0x3c.x = -0x8000;
    }

    if (i_this->field_0x54[0] == 0) {
        cLib_addCalc0(&i_this->field_0x28, 1.0f, 0.1f);
        if (i_this->field_0x28 < 0.01f) {
            i_this->field_0x50 = 0;
        }
    }
}

static void damage_check(e_bug_class* a_this, bug_s* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&a_this->actor;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz sp4c, sp58;
    f32 fVar1 = 70.0f;

    if (daPy_py_c::checkNowWolf()) {
        sp58 = player->current.pos;
    } else {
        if (daPy_getLinkPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_TURN_RIGHT || daPy_getLinkPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT ||
            daPy_getLinkPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT || daPy_getLinkPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_TURN_LEFT) {
            sp58 = player->current.pos;
            fVar1 = 200.0f;
        } else {
            MTXCopy(player->getLeftItemMatrix(), mDoMtx_stack_c::get());
            mDoMtx_stack_c::multVecZero(&sp58);
            sp58.y -= 50.0f;
        }
    }

    sp4c = i_this->field_0x18 - sp58;
    s8 sVar1 = 0;

    if (daPy_getPlayerActorClass()->getCutAtFlg() != 0 && sp4c.abs() < fVar1) {
        if (daPy_py_c::checkNowWolf()) {
            if (i_this->field_0x50 == 2) {
                sVar1 = 2;
            } else {
                sVar1 = 1;
            }
        } else if (i_this->field_0x50 == 1 || fVar1 > 100.0f) {
            sVar1 = 1;
        }
    }

    if (data_80697E8D != 0) {
        sp4c = i_this->field_0x18 - at_pos;

        if (data_80697E8D == 1) {
            if (sp4c.y < 50.0f && sp4c.y > -400.0f && JMAFastSqrt(sp4c.x * sp4c.x + sp4c.z * sp4c.z) < 50.0f) {
                sVar1 = 2;
            }
        } else if (data_80697E8D == 2) {
            if (sp4c.abs() < at_size) {
                sVar1 = 1;
            }
        }
    }
    
    if (sVar1 != 0){
        if (sVar1 == 1) {
            i_this->field_0x50 = 4;
            i_this->field_0x51 = 0;
            i_this->field_0x54[0] = cM_rndF(15.0f) + 15.0f + 25.0f;
            i_this->mSound.startSound(Z2SE_EN_BUG_DIE, 0, -1);
            cMtx_YrotS(*calc_mtx, cM_atan2s(sp4c.x, sp4c.z));
            sp4c.x = 0.0f;

            if (data_80697E8D == 2) {
                sp4c.y = cM_rndF(10.0f) + 35.0f;
                sp4c.z = cM_rndF(10.0f) + 35.0f;
            } else {
                sp4c.y = cM_rndF(10.0f) + 20.0f;
                sp4c.z = cM_rndF(10.0f) + 20.0f;
            }

            MtxPosition(&sp4c, &i_this->field_0x30);
            i_this->field_0x53 = 0;
            i_this->field_0x3c.x = cM_rndF(65536.0f);
            i_this->field_0x3c.z = cM_rndF(65536.0f);
            cXyz sp64(0.35f, 0.35f, 0.35f);
            dComIfGp_setHitMark(1, actor, &i_this->field_0x18, NULL, &sp64, 0);
        } else {
            i_this->field_0x50 = 3;
            i_this->field_0x54[0] = cM_rndF(10.0f) + 15.0f;
            i_this->field_0x3c.y = cM_rndF(65536.0f);
        }
    }
}

static void set_wait(e_bug_class* a_this, bug_s* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp40;
    s8 sVar1 = 0;

    switch (i_this->field_0x51) {
        case 0:
            if (a_this->bitSw != 0xFF) {
                if (dComIfGs_isSwitch(a_this->bitSw, l_roomNo)) {
                    sVar1 = 1;
                }
            } else {
                sp40 = i_this->field_0x18 - player->current.pos;
                if (sp40.abs() < a_this->field_0x57c) {
                    sVar1 = 1;
                }
            }
            break;

        case 1:
            if (i_this->field_0x54[0] == 0) {
                i_this->field_0x50 = 1;
                i_this->field_0x51 = 0;
            }
            break;
    }

    if (sVar1) {
        i_this->field_0x54[0] = i_this->field_0x8 * 3.0f + cM_rndF(2.0f);
        i_this->field_0x51 = 1;
        sp40 = i_this->field_0x18 - player->current.pos;
        i_this->field_0x3c.y = cM_rndFX(65536.0f);
    }
}

static void normal_move(e_bug_class* a_this, bug_s* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp68, sp74;

    if (i_this->field_0x53 != 0) {
        if ((i_this->field_0x52 & 15) == 0) {
            i_this->field_0x4a = cM_rndF(1000.0f) + 700.0f;
            i_this->field_0x24 = cM_rndFX(0.5f) + 5.0f;
        }

        s16 sVar1 = cM_ssin(i_this->field_0x48) * 3000.0f;
        i_this->field_0x48 += i_this->field_0x4a;
        sp74.x = player->current.pos.x - i_this->field_0x18.x;
        sp74.z = player->current.pos.z - i_this->field_0x18.z;

        if (data_80697EAC != 0 && JMAFastSqrt(sp74.x * sp74.x + sp74.z * sp74.z) < 200.0f) {
            sVar1 -= 0x8000;
        }

        cLib_addCalcAngleS2(&i_this->field_0x3c.y, i_this->field_0x42 + (sVar1 + cM_atan2s(sp74.x, sp74.z)), 2, 0x800);
        i_this->field_0x3c.x = 0;
    } else {
        i_this->field_0x3c.x += (s16)((i_this->field_0x8 << 1) + 0xE00);
    }

    i_this->field_0x30.x = i_this->field_0x24 * cM_ssin(i_this->field_0x3c.y);
    i_this->field_0x30.z = i_this->field_0x24 * cM_scos(i_this->field_0x3c.y);
    i_this->field_0x53 = bug_action(a_this, i_this);

    if (i_this->field_0x54[1] == 1) {
        i_this->field_0x42 = 0;
    }

    sp74 = player->current.pos - i_this->field_0x18;
    f32 fVar1 = JMAFastSqrt(sp74.x * sp74.x + sp74.z * sp74.z);
    f32 fVar2 = sp74.y;

    if ((i_this->field_0x52 & 1) != 0) {
        f32 fVar3 = sp74.abs();

        if (i_this->field_0x53 != 0 && data_80697EAC == 0 && i_this->field_0x54[2] == 0 && fVar3 < 140.0f) {
            s16 sVar2 = cM_atan2s(sp74.x, sp74.z);
            s16 sVar3 = i_this->field_0x3c.y - sVar2;
            
            if (sVar3 < 0x1000 && sVar3 > -0x1000) {
                i_this->field_0x30.y = cM_rndF(5.0f) + 25.0f;
                i_this->field_0x24 = 9.0f;
                i_this->field_0x53 = 0;
                i_this->field_0x3c.y = sVar2;
                i_this->mSound.startSound(Z2SE_EN_BUG_JUMP, 0, -1);
            }
        }

        f32 fVar4 = fVar3 * 0.10000000149011612f;
        if (fVar4 > 100.0f) {
            fVar4 = 100.0f;
        } else if (fVar4 < 20.0f) {
            fVar4 = 20.0f;
        }

        cLib_addCalc2(&i_this->field_0x2c, fVar4, 0.2f, 2.0f);
        int iVar1 = i_this->field_0x8 + 1;
        for (int i = iVar1; i < a_this->bug_num; i++) {
            sp68.x = a_this->Bug_s[i].field_0x18.x - i_this->field_0x18.x;
            sp68.z = a_this->Bug_s[i].field_0x18.z - i_this->field_0x18.z;
            fVar4 = JMAFastSqrt(sp68.x * sp68.x + sp68.z * sp68.z);
            iVar1 = i;

            if (fVar4 < i_this->field_0x2c) {
                cMtx_YrotS(*calc_mtx, cM_atan2s(-sp68.x, -sp68.z));
                sp68.x = 0.0f;
                sp68.y = 0.0f;
                sp68.z = 0.4f;
                MtxPosition(&sp68, &sp74);
                i_this->field_0x18.x += sp74.x;
                i_this->field_0x18.z += sp74.z;
                iVar1 = i;
            }
        }
    }

    if (i_this->field_0x30.y <= 0.0f && data_80697EAC == 0 && !dComIfGp_checkPlayerStatus0(0, 0x100) && fVar1 < 40.0f && fVar2 <= 0.0f && fVar2 >= -150.0f) {
        i_this->field_0x50 = 2;

        if (cM_rndF(1.0f) < 0.5f) {
            i_this->field_0x4a = cM_rndF(1000.0f) + 1000.0f;
        } else {
            i_this->field_0x4a = -(cM_rndF(1000.0f) + 1000.0f);
        }

        i_this->field_0x4e = cM_rndF(300.0f) + 600.0f;
    }

    int iVar2 = simple_bg_check(i_this, 0);

    if (i_this->field_0x54[1] == 0 && iVar2 >= 2) {
        if (iVar2 == 3) {
            i_this->field_0x42 = 0x1000;
        } else {
            i_this->field_0x42 = -0x1000;
        }

        i_this->field_0x54[1] = cM_rndF(30.0f) + 30.0f;
    }
}

static void bug_control(e_bug_class* a_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&a_this->actor;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz sp1c, sp28;
    bug_s* i_this = a_this->Bug_s;
    u8 sVar1 = 0;
    u8 sVar2 = 0;
    u8 uVar1 = 0;

    for (int i = 0; i < a_this->bug_num; i++, i_this++) {
        if (i_this->field_0x50 != 0) {
            if (i_this->field_0x18.y < actor->home.pos.y - 2000.0f) {
                i_this->field_0x50 = 0;
            }

            sVar1++;
            i_this->field_0xc = i_this->field_0x18;
            actor->current.pos = i_this->field_0x18;

            for (int j = 0; j < 3; j++) {
                if (i_this->field_0x54[j] != 0) {
                    i_this->field_0x54[j]--;
                }
            }

            i_this->field_0x52++;

            if (i_this->field_0x50 == -1) {
                set_wait(a_this, i_this);
                bug_mtxset(i_this);
            } else if (i_this->field_0x50 == 1) {
                normal_move(a_this, i_this);

                if (i_this->field_0x53 != 0) {
                    sVar2++;
                }

                bug_mtxset(i_this);
                damage_check(a_this, i_this);
            } else if (i_this->field_0x50 == 2) {
                if (daPy_py_c::checkNowWolf()) {
                    bug_stickW(i_this);
                    bug_mtxset_stickW(i_this);
                } else {
                    bug_stick(i_this);
                    bug_mtxset_stick(i_this);
                }

                uVar1++;
                damage_check(a_this, i_this);
            } else if (i_this->field_0x50 == 3) {
                wind_move(i_this);
                bug_mtxset(i_this);
                damage_check(a_this, i_this);
            } else if (i_this->field_0x50 == 4) {
                bug_fail(a_this, i_this);
                bug_mtxset_fail(i_this);
            }

            i_this->mSound.framework(0, dComIfGp_getReverb(l_roomNo));
        }
    }

    if (sVar1 == 0) {
        fopAcM_delete(actor);
        OS_REPORT("E_BUG DELETED \n");
    } else {
        if (sVar2 != 0) {
            a_this->mSound.playBeeGroupSound(Z2SE_EN_BUG_WALK_GRD, sVar2);
        }

        if (uVar1 != 0) {
            a_this->mSound.playBeeGroupSound(Z2SE_EN_BUG_WALK_BODY, uVar1);
        }

        a_this->mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));
    }

    if (uVar1 > 10) {
        player->onHeavyStateMidnaPanic();
    }
}

static void* s_boom_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_BOOMERANG && daPy_py_c::checkBoomerangCharge() && fopAcM_GetParam(i_actor) == 1) {
        data_80697E8D = 1;
        at_pos = ((fopAc_ac_c*)i_actor)->current.pos;
        return i_actor;
    }

    return NULL;
}

static void* s_bomb_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && dBomb_c::checkBombActor((fopAc_ac_c*)i_actor) && ((daNbomb_c*)i_actor)->checkExplodeNow()) {
        data_80697E8D = 2;
        at_pos = ((fopAc_ac_c*)i_actor)->current.pos;
        at_size = 300.0f;
        return i_actor;
    }

    return NULL;
}

static int daE_Bug_Execute(e_bug_class* i_this) {
    daPy_py_c* player = daPy_getPlayerActorClass();

    if (!player->checkMetamorphose()) {
        if (!daPy_getPlayerActorClass()->eventInfo.checkCommandDoor() && dComIfGp_event_runCheck()) {
            return 1;
        }
    }

    i_this->field_0x580++;
    data_80697E8D = 0;
    fpcM_Search(s_boom_sub, i_this);
    fpcM_Search(s_bomb_sub, i_this);

    if (daPy_getPlayerActorClass()->checkSpinnerRide()) {
        if (daPy_getPlayerActorClass()->checkSpinnerTriggerAttack()) {
            data_80697E8D = 2;
            at_pos = player->current.pos;
            at_size = 120.0f;
        }
    }

    if ((i_this->field_0x580 & 1) != 0) {
        cXyz* pos = player->getIronBallCenterPos();
        if (pos != NULL && (player->current.pos - *pos).abs() > 200.0f && !daPy_getPlayerActorClass()->checkIronBallReturn() && !daPy_getPlayerActorClass()->checkIronBallGroundStop()) {
            data_80697E8D = 2;
            at_pos = *pos;
            at_size = 130.0f;
        }
    }

    if (daPy_getPlayerActorClass()->getKandelaarFlamePos() != NULL || daPy_getPlayerActorClass()->eventInfo.checkCommandDoor()) {
        data_80697EAC = 1;
    } else {
        data_80697EAC = 0;
    }

    bug_control(i_this);
    return 1;
}

static int daE_Bug_IsDelete(e_bug_class* i_this) {
    return 1;
}

static int daE_Bug_Delete(e_bug_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;

    static u32 const l_bmdidx[2] = {BMDG_MU04, BMDG_MU05};
    if (i_this->field_0x7dad != 0) {
        for (u32 i = 0; i < 2; i++) {
            J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_bug", l_bmdidx[i]);
            dComIfGp_removeSimpleModel(modelData, fopAcM_GetRoomNo(a_this));
        }
    }

    dComIfG_resDelete(&i_this->mPhase, "E_bug");

    if (i_this->field_0x7dac != 0) {
        l_initHIO = 0;
    }

    if (a_this->heap != NULL) {
        i_this->mSound.deleteObject();
        for (int i = 0; i < i_this->bug_num; i++) {
            i_this->Bug_s[i].mSound.deleteObject();
        }
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    e_bug_class* i_this = (e_bug_class*)a_this;
    
    J3DModelData* modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes("E_bug", BMDG_MU04));
    JUT_ASSERT(1322, modelData != NULL);

    J3DModelData* modelData2 = static_cast<J3DModelData*>(dComIfG_getObjectRes("E_bug", BMDG_MU05));
    JUT_ASSERT(1327, modelData2 != NULL);

    for (int i = 0; i < i_this->bug_num; i++) {
        i_this->Bug_s[i].field_0x0 = mDoExt_J3DModel__create(modelData, 0x20000, 0x11000084);
        if (i_this->Bug_s[i].field_0x0 == NULL) {
            return 0;
        }
        
        i_this->Bug_s[i].field_0x4 = mDoExt_J3DModel__create(modelData2, 0x20000, 0x11000084);
        if (i_this->Bug_s[i].field_0x4 == NULL) {
            return 0;
        }
    }

    return 1;
}

static cPhs__Step daE_Bug_Create(fopAc_ac_c* a_this) {
    e_bug_class* i_this = (e_bug_class*)a_this;

    fopAcM_ct(a_this, e_bug_class);

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&i_this->mPhase, "E_bug");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("E_BUG PARAM %x\n", fopAcM_GetParam(a_this));
        i_this->field_0x570 = fopAcM_GetParam(a_this);
        i_this->bug_num = i_this->field_0x570 + 1;

        if (i_this->bug_num > 0x100) {
            i_this->bug_num = 0x100;
        }

        if (strcmp(dComIfGp_getStartStageName(), "T_ENEMY") == 0) {
            i_this->bug_num = 0x100;
        }

        OS_REPORT("E_BUG//////////////E_BUG SET 1 !!\n");
        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0x4b000)) {
            OS_REPORT("//////////////E_BUG SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////E_BUG SET 2 !!\n");
        if (l_initHIO == 0) {
            i_this->field_0x7dac = 1;
            l_initHIO = 1;
            l_HIO.field_0x4 = -1;
        }

        i_this->bitSw = (fopAcM_GetParam(a_this) & 0xFF0000) >> 16;
        i_this->field_0x578 = fopAcM_GetParam(a_this) >> 24;
        i_this->field_0x57c = ((fopAcM_GetParam(a_this) & 0xFF00) >> 8) * 100.0f;

        u8 uVar1 = a_this->home.angle.z;
        a_this->home.angle.z = 0;
        a_this->current.angle.z = 0;

        for (int i = 0; i < i_this->bug_num; i++) {
            i_this->Bug_s[i].field_0x8 = i;
            i_this->Bug_s[i].field_0x50 = -1;
            i_this->Bug_s[i].field_0x18 = a_this->home.pos;
            i_this->Bug_s[i].field_0x52 = i;
            i_this->Bug_s[i].field_0x28 = cM_rndFX(0.1f) + 1.0f;
            i_this->Bug_s[i].field_0x48 = cM_rndF(65536.0f);
            i_this->Bug_s[i].field_0x4c = cM_rndF(65536.0f);
            i_this->Bug_s[i].mSound.init(&i_this->Bug_s[i].field_0x18, 1);
        }

        if (uVar1 != 0) {
            dPath* roomPath = dPath_GetRoomPath(uVar1, fopAcM_GetRoomNo(a_this));
            if (roomPath != NULL) {
                dPnt* mPnts = roomPath->m_points;
                if (roomPath->m_num >= 1 && roomPath->m_num <= 4) {
                    int iVar1 = i_this->bug_num / (roomPath->m_num + 1);
                    for (int i = 0; i < roomPath->m_num; i++, mPnts++) {
                        for (int j = iVar1 * (i + 1); j < iVar1 * (i + 2); j++) {
                            i_this->Bug_s[j].field_0x18.x = mPnts->m_position.x;
                            i_this->Bug_s[j].field_0x18.y = mPnts->m_position.y;
                            i_this->Bug_s[j].field_0x18.z = mPnts->m_position.z;
                        }
                    }
                }
            }
        }

        daE_Bug_Execute(i_this);

        static u32 const l_bmdidx[2] = {BMDG_MU04, BMDG_MU05};
        for (u32 i = 0; i < 2; i++) {
            J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_bug", l_bmdidx[i]);
            JUT_ASSERT(1476, modelData != NULL);

            if (dComIfGp_addSimpleModel(modelData, fopAcM_GetRoomNo(a_this), 0) == -1) {
                OS_REPORT("1Bh[43;30m虫の集団：シンプルモデル登録失敗しました。\n1Bh[m"); // 1Bh, group of insects: Simple model registration failed.
            }

            i_this->field_0x7dad = 1;
        }

        l_roomNo = fopAcM_GetRoomNo(a_this);
        i_this->mSound.init(&a_this->current.pos, 2);
    }

    return phase;
}

AUDIO_INSTANCES;

static actor_method_class l_daE_Bug_Method = {
    (process_method_func)daE_Bug_Create,
    (process_method_func)daE_Bug_Delete,
    (process_method_func)daE_Bug_Execute,
    (process_method_func)daE_Bug_IsDelete,
    (process_method_func)daE_Bug_Draw,
};

extern actor_process_profile_definition g_profile_E_BUG = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_E_BUG,              // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(e_bug_class),     // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    182,                     // mPriority
    &l_daE_Bug_Method,       // sub_method
    0x00044000,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
