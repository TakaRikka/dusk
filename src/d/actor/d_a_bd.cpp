/**
* @file d_a_bd.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_bd.h"
#include "d/actor/d_a_cow.h"
#include "d/actor/d_a_horse.h"
#include "d/actor/d_a_npc_kkri.h"
#include "d/d_cc_d.h"
#include "d/d_s_play.h"
#include "f_op/f_op_camera_mng.h"
#include "Z2AudioLib/Z2Instances.h"



struct land_pos {
    /* 0x00 */ s32 unk;
    /* 0x04 */ f32 x;
    /* 0x08 */ f32 y;
    /* 0x0C */ f32 z;
};

static land_pos land_pos103[20] = {
    {0, -556.0f, 319.0f, 6063.0f},  {0, 264.0f, 501.0f, 5199.0f},   {0, -313.0f, 267.0f, 4460.0f},
    {0, -714.0f, 116.0f, 3704.0f},  {0, -1208.0f, 100.0f, 2491.0f}, {0, -2478.0f, 135.0f, 3929.0f},
    {0, -3181.0f, 155.0f, 4182.0f}, {0, -3902.0f, 579.0f, 3474.0f}, {0, -3645.0f, 619.0f, 2806.0f},
    {0, -3098.0f, 554.0f, 2335.0f}, {0, -891.0f, 316.0f, 844.0f},   {0, -205.0f, 191.0f, 1223.0f},
    {0, 1341.0f, 122.0f, 1230.0f},  {0, 2033.0f, 100.0f, 3449.0f},  {0, 2786.0f, 129.0f, 3795.0f},
    {0, 3806.0f, 265.0f, 2332.0f},  {0, 2841.0f, 185.0f, 1753.0f},  {0, 3847.0f, 114.0f, 36.0f},
    {0, 605.0f, 81.0f, -2006.0f},   {-1, 0.0f, 0.0f, 0.0f},
};

static land_pos land_pos127[20] = {
    {0, 3238.0f, 66.0f, -3141.0f},   {0, 2721.0f, 79.0f, -2194.0f}, {0, 3885.0f, 35.0f, -1399.0f},
    {0, 3862.0f, 35.0f, -220.0f},    {0, 4581.0f, 35.0f, 651.0f},   {0, 5495.0f, 35.0f, 1558.0f},
    {0, 5804.0f, 38.0f, 2547.0f},    {0, 6409.0f, 79.0f, 3954.0f},  {0, 5814.0f, 63.0f, 5086.0f},
    {0, 3267.0f, 75.0f, 6361.0f},    {0, 1767.0f, 79.0f, 5505.0f},  {0, 520.0f, 35.0f, 5728.0f},
    {0, 27.0f, 35.0f, 6956.0f},      {0, -2067.0f, 35.0f, 6083.0f}, {0, -3188.0f, 35.0f, 5761.0f},
    {0, -3697.0f, 35.0f, 4390.0f},   {0, -4466.0f, 35.0f, 4161.0f}, {0, -4527.0f, 87.0f, 6145.0f},
    {0, -2302.0f, 1216.0f, -242.0f}, {-1, 0.0f, 0.0f, 0.0f},
};

static int wait_bck[3] = {ANM_PITA_LEFTUP, ANM_PITA_RIGHT, ANM_PITA_DOWN};

static u8 l_HIOInit;
static daBd_HIO_c l_HIO;

static cXyz land_sp_pos[4] = {
    cXyz(3350.0f, 474.0f, 1760.0f),
    cXyz(553.0f, 395.0f, -599.0f),
    cXyz(-194.0f, 594.0f, 4832.0f),
    cXyz(-3349.0f, 865.0f, 2773.0f),
};

static int rope_pt;

daBd_HIO_c::daBd_HIO_c() {
    id = -1;
    mBasicSize = 1.2f;
    mFlightSpeed = 20.0f;
    mGroundSpeed = 3.0f;
    mFlightTime = 400;
    mLinkDetectRange = 300.0f;
    mChirpDist = 60;
    field_0x1E = 0;
}

#if DEBUG
/* daBd_HIO_c::genMessage (JORMContext *) */
void daBd_HIO_c::genMessage(JORMContext* mctx) {
    mctx->genLabel("小鳥", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    mctx->genSlider("基本大きさ", &mBasicSize, 0.0f, 5.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    mctx->genSlider("飛行速度", &mFlightSpeed, 0.0f, 50.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    mctx->genSlider("地上速度", &mGroundSpeed, 0.0f, 20.0f, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
    mctx->genSlider("飛行時間（およそ）", &mFlightTime, 0, 30000, 0, NULL, 0xFFFF, 0xFFFF, 0x200,
                    24);
    mctx->genSlider("リンク認識距離", &mLinkDetectRange, 0.0f, 2000.0f, 0, NULL, 0xFFFF, 0xFFFF,
                    0x200, 24);
    mctx->genSlider("羽LevelSE鳴る距離", &mChirpDist, 0, 1000, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 24);
}
#endif


static void anm_init(bd_class* i_this, int i_anmID, f32 i_morf, u8 i_attr, f32 i_speed) {
    i_this->mpMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("Bd", i_anmID), i_attr, i_morf,
                           i_speed, 0.0f, -1.0f, NULL);
    i_this->mAnmID = i_anmID;
}

static int daBd_Draw(bd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;

    J3DModel* mpModel = i_this->mpMorf->getModel();
    g_env_light.settingTevStruct(0, &a_this->current.pos, &a_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &a_this->tevStr);
    i_this->mpBtk->entry(mpModel->getModelData());
    i_this->mpMorf->entryDL();
    return 1;
}


static int way_bg_check(bd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;

    cXyz sp24;
    cXyz sp18;
    cXyz spC;
    sp24 = a_this->current.pos;
    sp24.y += 30.0f;
    cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
    spC.x = 0.0f;
    spC.y = 0.0f;
    spC.z = 100.0f;
    MtxPosition(&spC, &sp18);
    sp18 += sp24;
    dBgS_LinChk sp30;
    sp30.Set(&sp24, &sp18, a_this);
    if (dComIfG_Bgsp().LineCross(&sp30) != 0) {
        return 1;
    }
    return 0;
}


static void* s_a_sub(void* i_target, void* i_bird) {
    fopAc_ac_c* a_target = (fopAc_ac_c*)i_target;
    bd_class* b_bird = (bd_class*)i_bird;
    fopEn_enemy_c* a_bird = (fopEn_enemy_c*)i_bird;

    if ((fopAcM_IsActor(i_target) && fopAcM_GetGroup(a_target) == fopAc_ENEMY_e) ||
        fopAcM_GetGroup(a_target) == fopAc_NPC_e || fopAcM_GetName(i_target) == PROC_OBJ_KANBAN2 ||
        fopAcM_GetName(i_target) == PROC_OBJ_FOOD)
    {
        cXyz distance = a_bird->current.pos - a_target->current.pos;
        if (distance.abs() < l_HIO.mLinkDetectRange + 10.0f * fabsf(a_target->speedF)) {
            return i_target;
        }

        distance = b_bird->field_0x5C4 - a_target->current.pos;
        if (distance.abs() < l_HIO.mLinkDetectRange + 10.0f * fabsf(a_target->speedF)) {
            return i_target;
        }
    }
    return NULL;
}


static void pl_check(bd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    f32 var_f30;
    f32 var_f31;

    cXyz sp8 = i_this->field_0x5C4 - player->current.pos;
    u32 var_r28 = 0;
    if (i_this->field_0x658 != 0 || i_this->field_0x5B6 >= 1) {
        return;
    }

    if ((i_this->field_0x618 & 0xF) == 0) {
        var_r28 = (u32)fpcM_Search(s_a_sub, a_this);
    }

    if (daPy_getPlayerActorClass()->checkHorseRide()) {
        var_f30 = dComIfGp_getHorseActor()->speedF;
    } else {
        var_f30 = player->speedF;
    }

    var_f31 = l_HIO.mLinkDetectRange + 10.0f * fabsf(var_f30);
    if (player->getCutType() != daPy_py_c::CUT_TYPE_NONE) {
        var_f31 += 300.0f;
    }
    if (i_this->field_0x5DC < var_f31 || sp8.abs() < var_f31 || var_r28 != 0) {
        if (i_this->mActionID != ACT_FLY) {
            i_this->mActionID = ACT_FLY;
            i_this->field_0x61C = 0;
            i_this->field_0x61E = 1;
            i_this->field_0x624 = 0.0f;
            i_this->field_0x628 = 6.0f + cM_rndF(2.0f);
            anm_init(i_this, ANM_FLY, 1.0f, 2, 1.0f);
            i_this->mChirpDist = l_HIO.mChirpDist;
            return;
        }
        i_this->field_0x64C[0] = ((f32)l_HIO.mFlightTime + cM_rndF(0.5f * (f32)l_HIO.mFlightTime));
    }
    return;
}

static int pointBgCheck(cXyz* param_0, cXyz* param_1) {
    dBgS_LinChk sp20;
    cXyz spC = *param_1;
    spC.y += 2.0f;
    sp20.Set(param_0, &spC, NULL);
    if (dComIfG_Bgsp().LineCross(&sp20) != 0) {
        return 1;
    }
    return 0;
}


static int land_check(bd_class* i_this) {
    int land_pos_len;
    land_pos* land_pos;
    s8 temp_r1[112];

    int sp20 = g_Counter.mCounter0 + fopAcM_GetID(i_this);
    if ((sp20 & 7) != 0) {
        return -1;
    }

    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;
    camera_class* camera = dComIfGp_getCamera(0);
    cXyz sp30;
    sp30 = camera->lookat.center - camera->lookat.eye;
    s16 spA = cM_atan2s(sp30.x, sp30.z);

    if (strcmp(dComIfGp_getStartStageName(), "F_SP103") == 0) {
        land_pos_len = 20;
        land_pos = land_pos103;
    } else if (strcmp(dComIfGp_getStartStageName(), "F_SP127") == 0) {
        land_pos_len = 20;
        land_pos = land_pos127;
    }

    for (int i = 0; i < land_pos_len; i++) {
        temp_r1[i] = 0;
    }
    f32 var_f31 = 1000.0f;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < land_pos_len; j++) {
            if (land_pos[j].unk < 0) {
                break;
            }
            if (temp_r1[j] != 0) {
                continue;
            }
            sp30.x = land_pos[j].x - camera->lookat.eye.x;
            sp30.z = land_pos[j].z - camera->lookat.eye.z;
            s16 sVar2 = cM_atan2s(sp30.x, sp30.z) - spA;
            if (sVar2 >= 0x2000 || sVar2 <= -0x2000) {
                continue;
            }

            sp30.x = camera->lookat.eye.x - land_pos[j].x;
            sp30.z = camera->lookat.eye.z - land_pos[j].z;

            f32 temp_f1 = JMAFastSqrt((sp30.x * sp30.x) + (sp30.z * sp30.z));
            if (!(temp_f1 >= 600.0f) || !(temp_f1 < var_f31)) {
                continue;
            }

            sp30.x = land_pos[j].x;
            sp30.y = land_pos[j].y + 5.0f;
            sp30.z = land_pos[j].z;
            if (pointBgCheck(&a_this->current.pos, &sp30)) {
                temp_r1[j] = 1;
            } else {
                return j;
            }
        }
        var_f31 += 500.0f;
    }
    return -1;
}


static void drop_check(bd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;

    if (a_this->speed.y < -10.0f) {
        i_this->mActionID = ACT_DROP;
        i_this->field_0x61C = 0;
        a_this->speed.y = 0.0f;
    }
}


static void turn_set(bd_class* i_this) {
    s16 angle_table[3] = {0x8000, 0x4000, -0x4000};
    s16 angle = angle_table[(int)cM_rndF(2.99f)];
    i_this->mTargetAngleY = i_this->enemy.current.angle.y + angle;
}


static void bd_ground(bd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    cXyz sp30;
    cXyz sp24;
    f32 speed = 0.0f;
    dBgS_GndChk gnd_chk;
    switch (i_this->field_0x61C) {
    case -1:
        Vec sp18;
        sp18.x = a_this->home.pos.x + cM_rndFX(100.0f);
        sp18.y = a_this->home.pos.y + 100.0f;
        sp18.z = a_this->home.pos.z + cM_rndFX(100.0f);
        if (strcmp(dComIfGp_getStartStageName(), "R_SP127") != 0) {
            gnd_chk.SetPos(&sp18);
            a_this->current.pos = sp18;
            a_this->current.pos.y = dComIfG_Bgsp().GroundCross(&gnd_chk);
        }
        a_this->home.pos = a_this->current.pos;
        i_this->field_0x61C = 0;
        /* fallthrough */

    case 0:
        i_this->field_0x618 = cM_rndF(65536.0f);
        i_this->field_0x64C[3] = 200.0f + cM_rndF(200.0f);
        i_this->field_0x61C = 1;
        /* fallthrough */

    case 1:
        if (i_this->mBgc.ChkGroundHit() || i_this->field_0x5C2 != 0) {
            i_this->field_0x64C[0] = 20.0f + cM_rndF(60.0f);
            i_this->field_0x61C = 2;
            i_this->field_0x64C[1] = 0;
        }
        break;

    case 2:
        if (i_this->field_0x64C[1] == 0) {
            if (cM_rndF(1.0f) < 0.2f) {
                i_this->field_0x64C[1] = 20.0f + cM_rndF(10.0f);
            } else {
                i_this->field_0x64C[1] = 2.0f + cM_rndF(2.0f);
            }
            anm_init(i_this, wait_bck[(int)cM_rndF(2.99f)], 2.0f, 0, 1.0f);
        }
        if (i_this->field_0x64C[0] == 0) {
            if (way_bg_check(i_this)) {
                turn_set(i_this);
            } else {
                f32 temp_f = 300.0f;
                sp30.x = (a_this->home.pos.x + cM_rndFX(temp_f)) - a_this->current.pos.x;
                sp30.z = (a_this->home.pos.z + cM_rndFX(temp_f)) - a_this->current.pos.z;
                i_this->mTargetAngleY = cM_atan2s(sp30.x, sp30.z);
            }
            i_this->field_0x61C = 3;
            i_this->field_0x64C[0] = 10.0f + cM_rndF(30.0f);
        }
        break;

    case 3:
        anm_init(i_this, ANM_PYON, 1.0f, 2, 1.0f);
        i_this->field_0x61C++;
        /* fallthrough */

    case 4:
        speed = l_HIO.mGroundSpeed;
        cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mTargetAngleY, 4, TREG_S(0) + 0x1000);
        if (i_this->field_0x64C[0] == 0 && !((int)i_this->mpMorf->getFrame() > 1)) {
            i_this->field_0x61C = 1;
        } else if ((i_this->field_0x64C[2] == 0) && (i_this->mBgc.ChkWallHit() != 0)) {
            turn_set(i_this);
            i_this->field_0x64C[2] = 20;
        }
    }

    a_this->speedF = speed;
    if ((i_this->field_0x618 & 0xF) == 0 && cM_rndF(1.0f) < 0.35f) {
        i_this->mSound.startSound(Z2SE_BIRD_STAY, 0, -1);
    }
    drop_check(i_this);
    if (i_this->field_0x658 == 0) {
        if (fopAcM_CheckCondition(a_this, fopAcCnd_NODRAW_e)) {
            if (i_this->field_0x64C[3] > 50) {
                i_this->field_0x64C[3] = 30.0f + cM_rndF(20.0f);
            }

            dScnKy_env_light_c* env_light = dKy_getEnvlight();
            int spC = env_light->daytime / 15.0f;
            if (spC <= 6 || spC >= 17) {
                fopAcM_delete(a_this);
                OS_REPORT("BD TIME OFF\n");
            }
        }
        if (i_this->field_0x64C[3] == 0) {
            i_this->mActionID = ACT_FLY;
            i_this->field_0x61C = 0;
            i_this->field_0x61E = 1;
            i_this->field_0x624 = 0.0f;
            i_this->field_0x628 = 6.0f + cM_rndF(2.0f);
            anm_init(i_this, ANM_FLY, 1.0f, 2, 1.0f);
            i_this->mChirpDist = l_HIO.mChirpDist;
        }
    } else {
        i_this->field_0x658--;
    }
}


static void bd_fly(bd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    cXyz sp2C;
    cXyz sp20;
    f32 unused = 0.0f;
    switch (i_this->field_0x61E) {
    case 0:
        if (i_this->field_0x620 <= 0.0f) {
            i_this->field_0x61E = 1;
            i_this->field_0x624 = -2.0f + TREG_F(2);
            if (a_this->speed.y > 0.0f) {
                i_this->field_0x628 = 7;
            } else {
                i_this->field_0x628 = 5;
            }
            anm_init(i_this, ANM_FLY, 1.0f, 2, 1.0f);
        }
        break;

    case 1:
        if (i_this->field_0x628 == 0 && i_this->field_0x624 < 3.0f + TREG_F(1)) {
            anm_init(i_this, ANM_KAKKU1, 1.0f, 0, 1.0f);
            i_this->field_0x61E = 0;
        }
        break;
    }

    switch (i_this->field_0x61C) {
    case 0:
        i_this->field_0x64C[0] = 300.0f + cM_rndF(200.0f);
        i_this->field_0x64C[1] = 0;
        a_this->current.angle.x = TREG_S(1) - 10000;
        anm_init(i_this, ANM_FLY, 1.0f, 2, 1.0f);
        i_this->field_0x61C = 1;
        /* fallthrough */

    case 1:
        if (i_this->field_0x64C[0] == 0) {
            if (strcmp(dComIfGp_getStartStageName(), "F_SP103") == 0) {
                int spC = land_check(i_this);
                if (spC >= 0) {
                    if (cM_rndF(1.0f) <= 0.2f && i_this->field_0x62C != NULL) {
                        a_this->home.pos = *i_this->field_0x62C;
                        i_this->field_0x645 = 1;
                    } else {
                        a_this->home.pos.x = land_pos103[spC].x;
                        a_this->home.pos.y = land_pos103[spC].y;
                        a_this->home.pos.z = land_pos103[spC].z;
                        i_this->field_0x645 = land_pos103[spC].unk;
                    }
                    i_this->mActionID = ACT_LANDING;
                    i_this->field_0x61C = 0;
                } else {
                    i_this->field_0x64C[0] = 0;
                }
            } else {
                if (strcmp(dComIfGp_getStartStageName(), "F_SP127") == 0 ||
                    strcmp(dComIfGp_getStartStageName(), "R_SP127") == 0)
                {
                    if (strcmp(dComIfGp_getStartStageName(), "R_SP127") == 0) {
                        a_this->home.pos.x = -450.0f;
                        a_this->home.pos.y = 3.0f;
                        a_this->home.pos.z = -612.0f;
                        i_this->field_0x645 = 0;
                        i_this->mActionID = ACT_LANDING;
                        i_this->field_0x61C = 0;
                    } else {
                        int sp8 = land_check(i_this);
                        if (sp8 >= 0) {
                            a_this->home.pos.x = land_pos127[sp8].x;
                            a_this->home.pos.y = land_pos127[sp8].y;
                            a_this->home.pos.z = land_pos127[sp8].z;
                            i_this->field_0x645 = land_pos127[sp8].unk;
                            i_this->mActionID = ACT_LANDING;
                            i_this->field_0x61C = 0;
                        } else {
                            i_this->field_0x64C[0] = 0;
                        }
                    }
                } else {
                    i_this->mActionID = ACT_LANDING;
                    i_this->field_0x61C = 0;
                    if (i_this->field_0x5B6 >= 1) {
                        i_this->field_0x645 = 1;
                        a_this->home.pos = i_this->field_0x634;
                    } else {
                        i_this->field_0x645 = 0;
                    }
                }
            }
        } else if (i_this->field_0x64C[1] == 0) {
            if (strcmp(dComIfGp_getStartStageName(), "R_SP127") == 0) {
                i_this->field_0x5C4.x = cM_rndFX(1000.0f);
                i_this->field_0x5C4.y = 800.0f + cM_rndF(300.0f);
                i_this->field_0x5C4.z = -3000.0f + cM_rndFX(1000.0f);
                a_this->current.angle.y = 32768.0f + cM_rndFX(10000.0f);
            } else {
                i_this->field_0x5C4.x = a_this->home.pos.x + cM_rndFX(1000.0f);
                i_this->field_0x5C4.y = a_this->home.pos.y + 800.0f + cM_rndF(300.0f);
                i_this->field_0x5C4.z = a_this->home.pos.z + cM_rndFX(1000.0f);
            }
            i_this->field_0x64C[1] = 10.0f + cM_rndF(30.0f);
        }
        break;
    }

    a_this->speedF = l_HIO.mFlightSpeed;
    sp2C = i_this->field_0x5C4 - a_this->current.pos;
    if (i_this->field_0x61E != 0) {
        cLib_addCalcAngleS2(&a_this->current.angle.y, cM_atan2s(sp2C.x, sp2C.z), 8,
                            (TREG_S(2) + 700));
        cLib_addCalcAngleS2(&a_this->current.angle.x,
                            -cM_atan2s(sp2C.y, JMAFastSqrt((sp2C.x * sp2C.x) + (sp2C.z * sp2C.z))),
                            8, (TREG_S(2) + 700));
    }
}


static void bd_landing(bd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    cXyz sp24;
    cXyz sp18;
    f32 unused = 0.0f;
    switch (i_this->field_0x61E) {
    case 0:
        if (i_this->field_0x620 <= 0.0f) {
            i_this->field_0x61E = 1;
            i_this->field_0x624 = -2.0f + TREG_F(2);
            if (a_this->speed.y > 0.0f) {
                i_this->field_0x628 = 7;
            } else {
                i_this->field_0x628 = 5;
            }
            anm_init(i_this, ANM_FLY, 1.0f, 2, 1.0f);
        }
        break;

    case 1:
        if (i_this->field_0x628 == 0 && i_this->field_0x624 < 3.0f + TREG_F(1)) {
            anm_init(i_this, ANM_KAKKU1, 1.0f, 0, 1.0f);
            i_this->field_0x61E = 0;
        }
        break;
    }

    switch (i_this->field_0x61C) {
    case 1:
        break;

    case 0:
        i_this->field_0x5C4.x = a_this->home.pos.x + cM_rndFX(20.0f);
        i_this->field_0x5C4.y = a_this->home.pos.y + 20.0f + TREG_F(0);
        i_this->field_0x5C4.z = a_this->home.pos.z + cM_rndFX(20.0f);
        i_this->field_0x61C = 1;
        break;
    }

    a_this->speedF = l_HIO.mFlightSpeed;
    sp24 = i_this->field_0x5C4 - a_this->current.pos;
    if (sp24.abs() < 100.0f + TREG_F(3)) {
        i_this->field_0x5C4.x = a_this->home.pos.x;
        i_this->field_0x5C4.z = a_this->home.pos.z;
        if (i_this->field_0x645 != 0) {
            i_this->mActionID = ACT_LANDING3;
            i_this->field_0x61C = 0;
            anm_init(i_this, ANM_FLY, 2.0f, 2, 1.0f);
        } else {
            i_this->mActionID = ACT_LANDING2;
            i_this->field_0x61C = 0;
            anm_init(i_this, ANM_KAKKU, 1.0f, 0, 1.0f);
            i_this->field_0x64C[0] = cM_rndF(45.0f + TREG_F(4));
        }
    }
    cLib_addCalcAngleS2(&a_this->current.angle.y, cM_atan2s(sp24.x, sp24.z), 4, (TREG_S(3) + 1000));
    cLib_addCalcAngleS2(&a_this->current.angle.x,
                        -cM_atan2s(sp24.y, JMAFastSqrt((sp24.x * sp24.x) + (sp24.z * sp24.z))), 4,
                        (TREG_S(3) + 1000));
}

static void bd_landing2(bd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;

    cXyz sp20;
    cXyz sp14;
    if (i_this->field_0x64C[0] == 0x14) {
        anm_init(i_this, ANM_FLY, 1.0f, 2, 1.0f);
    }
    if (i_this->field_0x64C[0] == 0) {
        anm_init(i_this, ANM_FLY, 2.0f, 2, 1.0f);
        i_this->mActionID = ACT_GROUND;
        i_this->field_0x61C = 0;
        a_this->speed.y = 0.0f;
    }
    cLib_addCalc2(&a_this->speedF, 10.0f + TREG_F(8), 0.5f, 1.0f + TREG_F(9));
    sp20 = i_this->field_0x5C4 - a_this->current.pos;
    s16 temp_r28 = a_this->current.angle.y;
    cLib_addCalcAngleS2(&a_this->current.angle.y, cM_atan2s(sp20.x, sp20.z), 4, TREG_S(4) + 2000);
    cLib_addCalcAngleS2(&a_this->current.angle.x,
                        -cM_atan2s(sp20.y, JMAFastSqrt((sp20.x * sp20.x) + (sp20.z * sp20.z))), 4,
                        TREG_S(4) + 2000);
    cLib_addCalcAngleS2(&a_this->current.angle.z,
                        (TREG_S(6) + 4) * (temp_r28 - a_this->current.angle.y), 4,
                        TREG_S(7) + 1000);
}


static void bd_landing3(bd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    cXyz sp20;
    cXyz sp14;
    f32 unused = 0.0f;
    switch (i_this->field_0x61C) {
    case 1:
        break;
    case 0:
        i_this->field_0x61C = 1;
        break;
    }

    if (i_this->field_0x62C != NULL) {
        i_this->field_0x5C4 = *i_this->field_0x62C;
    } else {
        i_this->field_0x5C4 = a_this->home.pos;
    }
    a_this->speedF = 0.0f;
    a_this->gravity = 0.0f;
    cLib_addCalc2(&a_this->current.pos.x, i_this->field_0x5C4.x, 0.1f + TREG_F(5),
                  fabsf(a_this->speed.x));
    cLib_addCalc2(&a_this->current.pos.y, i_this->field_0x5C4.y, 0.1f + TREG_F(5),
                  fabsf(a_this->speed.y));
    cLib_addCalc2(&a_this->current.pos.z, i_this->field_0x5C4.z, 0.1f + TREG_F(5),
                  fabsf(a_this->speed.z));
    sp20 = i_this->field_0x5C4 - a_this->current.pos;
    cLib_addCalcAngleS2(&a_this->current.angle.y, cM_atan2s(sp20.x, sp20.z), 4, TREG_S(4) + 1000);
    if (sp20.abs() < 10.0f) {
        if (i_this->field_0x5B6 >= 1) {
            i_this->mActionID = ACT_KKRI;
            i_this->field_0x64C[3] = 400.0f + cM_rndF(100.0f);
        } else {
            i_this->mActionID = ACT_ROPE;
            i_this->field_0x64C[3] = 200.0f + cM_rndF(200.0f);
        }
        i_this->field_0x61C = 0;
    }
}


static void bd_rope(bd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;

    a_this->speedF = 0.0f;
    a_this->gravity = 0.0f;
    a_this->speed.y = 0.0f;
    if (i_this->field_0x64C[1] == 0) {
        if (cM_rndF(1.0f) < 0.2f) {
            i_this->field_0x64C[1] = (20.0f + cM_rndF(10.0f));
        } else {
            i_this->field_0x64C[1] = (2.0f + cM_rndF(2.0f));
        }
        anm_init(i_this, wait_bck[(int)cM_rndF(1.99f)], 2.0f, 0, 1.0f);
    }
    if (i_this->field_0x62C != NULL) {
        i_this->field_0x5C4 = *i_this->field_0x62C;
    } else {
        i_this->field_0x5C4 = a_this->home.pos;
    }
    cLib_addCalc2(&a_this->current.pos.x, i_this->field_0x5C4.x, 1.0f,
                  3.0f + fabsf(a_this->speed.x));
    cLib_addCalc2(&a_this->current.pos.y, i_this->field_0x5C4.y, 1.0f,
                  3.0f + fabsf(a_this->speed.y));
    cLib_addCalc2(&a_this->current.pos.z, i_this->field_0x5C4.z, 1.0f,
                  3.0f + fabsf(a_this->speed.z));
    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x640, 1, 0x800);
    if (i_this->field_0x64C[3] == 0) {
        i_this->mActionID = ACT_FLY;
        i_this->field_0x61C = 0;
        i_this->field_0x61E = 1;
        i_this->field_0x624 = 0.0f;
        i_this->field_0x628 = (6.0f + cM_rndF(2.0f));
        anm_init(i_this, ANM_FLY, 1.0f, 2, 1.0f);
        i_this->mChirpDist = l_HIO.mChirpDist;
    }
}

static void bd_kkri(bd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;

    a_this->speedF = 0.0f;
    a_this->gravity = 0.0f;
    a_this->speed.y = 0.0f;
    if (i_this->field_0x64C[1] == 0) {
        if (cM_rndF(1.0f) < 0.2f) {
            i_this->field_0x64C[1] = (20.0f + cM_rndF(10.0f));
        } else {
            i_this->field_0x64C[1] = (2.0f + cM_rndF(2.0f));
        }
        anm_init(i_this, wait_bck[(int)cM_rndF(1.99f)], 2.0f, 0, 1.0f);
    }
    i_this->field_0x5C4 = i_this->field_0x634;
    cLib_addCalc2(&a_this->current.pos.x, i_this->field_0x5C4.x, 1.0f,
                  3.0f + fabsf(a_this->speed.x));
    cLib_addCalc2(&a_this->current.pos.y, i_this->field_0x5C4.y, 1.0f,
                  3.0f + fabsf(a_this->speed.y));
    cLib_addCalc2(&a_this->current.pos.z, i_this->field_0x5C4.z, 1.0f,
                  3.0f + fabsf(a_this->speed.z));
    if (i_this->field_0x64C[3] == 0) {
        i_this->mActionID = ACT_FLY;
        i_this->field_0x61C = 0;
        i_this->field_0x61E = 1;
        i_this->field_0x624 = 0.0f;
        i_this->field_0x628 = (6.0f + cM_rndF(2.0f));
        anm_init(i_this, ANM_FLY, 1.0f, 2, 1.0f);
        i_this->mChirpDist = l_HIO.mChirpDist;
    }
}

static void bd_drop(bd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;

    switch (i_this->field_0x61C) { /* irregular */
    case 1:
        break;
    case 0:
        anm_init(i_this, ANM_FLY, 1.0f, 2, 1.0f);
        i_this->field_0x61C++;
        break;
    }
    a_this->gravity = -1.0f;
    if (i_this->mBgc.ChkGroundHit() || i_this->field_0x5C2 != 0) {
        i_this->mActionID = ACT_GROUND;
        i_this->field_0x61C = 0;
    }
}


static void action(bd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;

    cXyz sp24;
    cXyz sp18;
    a_this->gravity = -7.0f;
    i_this->mSphere.OnCoSetBit();
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    i_this->field_0x5D4 = &player->current;
    i_this->field_0x5DC = fopAcM_searchPlayerDistance(a_this);
    i_this->field_0x5E0 = fopAcM_searchPlayerDistanceXZ(a_this);
    i_this->field_0x5D8 = fopAcM_searchPlayerAngleY(a_this);

    int sp10 = 0;
    int var_r28 = 0;
    int unused = 0;
    s16 target_angle_x = 0;
    pl_check(i_this);

    switch (i_this->mActionID) {
    case ACT_GROUND:
        bd_ground(i_this);
        var_r28 = 1;
        sp10 = 1;
        break;

    case ACT_DROP:
        bd_drop(i_this);
        var_r28 = 1;
        sp10 = 1;
        break;

    case ACT_FLY:
        bd_fly(i_this);
        break;

    case ACT_LANDING:
        bd_landing(i_this);
        break;

    case ACT_LANDING2:
        bd_landing2(i_this);
        sp10 = true;
        break;

    case ACT_LANDING3:
        bd_landing3(i_this);
        var_r28 = -1;
        target_angle_x = TREG_S(5) - 0x2000;
        break;

    case ACT_ROPE:
        bd_rope(i_this);
        var_r28 = -1;
        break;

    case ACT_KKRI:
        bd_kkri(i_this);
        var_r28 = -1;
        break;
    }

    cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 4, 0x2000);
    cLib_addCalcAngleS2(&a_this->shape_angle.x, target_angle_x, 1, 0x1000);
    a_this->shape_angle.z = a_this->current.angle.z;
    cLib_addCalcAngleS2(&a_this->current.angle.z, 0, 1, 500);
    if (var_r28 != 0) {
        if (var_r28 > 0) {
            cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
            sp24.x = 0.0f;
            sp24.y = 0.0f;
            sp24.z = a_this->speedF;
            MtxPosition(&sp24, &sp18);
            a_this->speed.x = sp18.x;
            a_this->speed.z = sp18.z;
            a_this->current.pos += a_this->speed;
            a_this->speed.y += a_this->gravity;
            if (a_this->speed.y < -80.0f) {
                a_this->speed.y = -80.0f;
            }
        }
    } else {
        cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
        cMtx_XrotM(*calc_mtx, a_this->current.angle.x);
        sp24.x = 0.0f;
        sp24.y = 0.0f;
        sp24.z = a_this->speedF;
        MtxPosition(&sp24, &a_this->speed);
        a_this->current.pos += a_this->speed;
    }
    i_this->field_0x5C2 = 0;
    if (sp10) {
        i_this->mBgc.CrrPos(dComIfG_Bgsp());
        if (strcmp(dComIfGp_getStartStageName(), "R_SP127") == 0) {
            if (a_this->speed.y <= 0.0f) {
                if (a_this->current.pos.y <= 0.0f) {
                    a_this->current.pos.y = 0.0f;
                    a_this->speed.y = 0.0f;
                    i_this->field_0x5C2 = 1;
                }
            }
        }
    }
    i_this->field_0x620 += i_this->field_0x624;
    if (i_this->field_0x628 != 0) {
        i_this->field_0x628--;
        i_this->field_0x624 += 1.5f + TREG_F(6);
    } else {
        i_this->field_0x624 -= 0.5f + TREG_F(7);
    }
    if (i_this->field_0x620 <= 0.0f) {
        i_this->field_0x620 = 0.0f;
    }
}


static int daBd_Execute(bd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;
    daNpc_Kkri_c* kkri;
    daCow_c* cow;

    if (i_this->field_0x5B6 >= 1) {
        if (i_this->field_0x642 != 0) {
            i_this->field_0x642--;
        }

        fopAcM_SearchByName(PROC_NPC_KKRI, (fopAc_ac_c**)&kkri);
        if (kkri != NULL && i_this->field_0x642 == 0) {
            if (i_this->field_0x5B6 == 1) {
                MTXCopy(kkri->getBd1Mtx(), *calc_mtx);
            } else if (i_this->field_0x5B6 == 2) {
                MTXCopy(kkri->getBd2Mtx(), *calc_mtx);
            } else {
                MTXCopy(kkri->getBd3Mtx(), *calc_mtx);
            }
            cXyz sp18(0.0f, 0.0f, 0.0f);
            MtxPosition(&sp18, &i_this->field_0x634);
            i_this->field_0x62C = &i_this->field_0x634;
            i_this->field_0x640 = kkri->shape_angle.y;
        }
    }
    i_this->field_0x618++;
    for (int i = 0; i < 4; i++) {
        if (i_this->field_0x64C[i] != 0) {
            i_this->field_0x64C[i]--;
        }
    }
    if (i_this->field_0x654 != 0) {
        i_this->field_0x654--;
    }
    action(i_this);
    mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y + i_this->field_0x620,
                           a_this->current.pos.z);
    mDoMtx_stack_c::YrotM(a_this->shape_angle.y);
    mDoMtx_stack_c::XrotM(a_this->shape_angle.x);
    mDoMtx_stack_c::ZrotM(a_this->shape_angle.z);
    mDoMtx_stack_c::scaleM(l_HIO.mBasicSize, l_HIO.mBasicSize, l_HIO.mBasicSize);
    J3DModel* model_p = i_this->mpMorf->getModel();
    model_p->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->mpMorf->play(&a_this->eyePos, 0, 0);
    if (i_this->mAnmID == ANM_FLY && i_this->mChirpDist != 0) {
        i_this->mChirpDist--;
        i_this->mSound.startLevelSound(Z2SE_BIRD_FLYING, 0, -1);
    }
    i_this->mpBtk->setFrame((f32)(int)i_this->field_0x5EC);
    i_this->mpMorf->modelCalc();
    a_this->eyePos = a_this->current.pos;
    a_this->eyePos.y += 20.0f;
    a_this->attention_info.position = a_this->eyePos;
    i_this->mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));

    if (i_this->field_0x656 == 0 && dComIfGp_event_runCheck()) {
        cow = (daCow_c*)fopAcM_SearchByName(PROC_COW);
        if (cow != NULL && cow->speedF > 1.0f) {
            i_this->field_0x656 = 1;
            i_this->mActionID = ACT_GROUND;
            i_this->field_0x61C = 0;
            i_this->field_0x658 = TREG_S(6) + 160;
            a_this->current.pos.x = -914.0f + cM_rndFX(200.0f);
            a_this->current.pos.y = 160.0f;
            a_this->current.pos.z = 3931.0f + cM_rndFX(200.0f);
        }
    }
    return 1;
}

static int daBd_IsDelete(bd_class* i_this) {
    return 1;
}

static int daBd_Delete(bd_class* i_this) {
    fopEn_enemy_c* a_this = (fopEn_enemy_c*)i_this;

    fopAcM_RegisterDeleteID(i_this, "Bd");
    dComIfG_resDelete(&i_this->mPhase, "Bd");
    if (i_this->field_0x9E8 != 0) {
        l_HIOInit = 0;
        mDoHIO_DELETE_CHILD(l_HIO.id);
    }
    if (a_this->heap != 0) {
        i_this->mSound.deleteObject();
    }
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    bd_class* a_this = (bd_class*)i_this;

    a_this->mpMorf = new mDoExt_McaMorf((J3DModelData*)dComIfG_getObjectRes("Bd", 0xE), NULL, NULL,
                                        (J3DAnmTransform*)dComIfG_getObjectRes("Bd", 8), 0, 1.0f, 0,
                                        -1, 1, NULL, 0x80000, 0x11000284);
    if (a_this->mpMorf == NULL || a_this->mpMorf->getModel() == NULL) {
        return 0;
    }

    a_this->mpBtk = new mDoExt_btkAnm();
    if (a_this->mpBtk == NULL) {
        return 0;
    }
    if (a_this->mpBtk->init(a_this->mpMorf->getModel()->getModelData(),
                            (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Bd", 0x11), 1, 0, 1.0f, 0,
                            -1) == 0)
    {
        return 0;
    }
    a_this->mSound.init(&i_this->current.pos, 1);
    return 1;
}


static int daBd_Create(fopAc_ac_c* i_act_this) {
    bd_class* i_this = (bd_class*)i_act_this;
    fopAcM_ct(i_act_this, bd_class);

    int phase_state = dComIfG_resLoad(&i_this->mPhase, "Bd");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("BD PARAM %x\n", fopAcM_GetParam(i_act_this));
        i_this->field_0x5B4 = fopAcM_GetParam(i_act_this);
        i_this->field_0x5B5 = (fopAcM_GetParam(i_act_this) & 0xFF00) >> 8;
        i_this->field_0x5B6 = (fopAcM_GetParam(i_act_this) & 0xFF0000) >> 0x10;
        i_this->field_0x5B7 = (fopAcM_GetParam(i_act_this) & 0xFF000000) >> 0x18;
        if (i_this->field_0x5B6 == 0xFF) {
            i_this->field_0x5B6 = 0;
        }
        i_this->field_0x5B9 = (fopAcM_GetParam(i_act_this) & 0xFF0000) >> 0x10;
        i_this->field_0x5B8 = (fopAcM_GetParam(i_act_this) & 0xFF000000) >> 0x18;
        OS_REPORT("BD//////////////BD SET 1 !!\n");
        if (fopAcM_entrySolidHeap(i_act_this, useHeapInit, 0xBE0) == 0) {
            OS_REPORT("//////////////BD SET NON !!\n");
            return cPhs_ERROR_e;
        }
        OS_REPORT("//////////////BD SET 2 !!\n");
        if (l_HIOInit == 0) {
            i_this->field_0x9E8 = 1;
            l_HIOInit = 1;
            l_HIO.id = mDoHIO_CREATE_CHILD("小鳥", (JORReflexible*)&l_HIO);
        }
        fopAcM_SetMtx(i_act_this, i_this->mpMorf->getModel()->getBaseTRMtx());
        i_this->mBgc.Set(fopAcM_GetPosition_p(i_act_this), fopAcM_GetOldPosition_p(i_act_this),
                         i_act_this, 1, &i_this->mAcchCir, fopAcM_GetSpeed_p(i_act_this), NULL,
                         NULL);
        i_this->mAcchCir.SetWall(10.0f, 10.0f);
        i_act_this->health = 1;
        i_act_this->field_0x560 = 1;
        i_this->mStts.Init(100, 0, i_act_this);

        /* 804DA234-804DA274 0002AC 0040+00 1/1 0/0 0/0 .data            cc_sph_src$4956 */
        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x75}},  // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                 // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2},                 // mGObjTg
                {0x0},                                              // mGObjCo
            },                                                      // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 10.0f}  // mSph
            }  // mSphAttr
        };

        i_this->mSphere.Set(cc_sph_src);
        i_this->mSphere.SetStts(&i_this->mStts);
        if (i_this->field_0x5B5 == 0xFF) {
            i_this->field_0x5EC = cM_rndF(7.999f);
        } else {
            i_this->field_0x5EC = i_this->field_0x5B5;
            if (i_this->field_0x5EC > 7) {
                i_this->field_0x5EC = 7;
            }
        }
        i_this->mActionID = ACT_GROUND;
        i_this->field_0x61C = -1;
        i_this->field_0x630 = rope_pt;
        rope_pt++;
        if (rope_pt > 13) {
            rope_pt = 0;
        }
        i_this->field_0x642 = 20;
        daBd_Execute(i_this);
        csXyz sp1C = i_act_this->current.angle;
        sp1C.y = cM_rndF(65536.0f);
        if (i_this->field_0x5B4 != 0xFF && i_this->field_0x5B4 != 0 && i_this->field_0x5B4 != 1) {
            for (int i = 0; i < i_this->field_0x5B4 - 1; i++) {
                fopAcM_createChild(
                    PROC_BD, fopAcM_GetID(i_act_this), fopAcM_GetParam(i_act_this) & 0xFFFFFF00,
                    &i_act_this->current.pos, fopAcM_GetRoomNo(i_act_this), &sp1C, NULL, -1, NULL);
            }
        }
    }
    return phase_state;
}

static actor_method_class l_daBd_Method = {
    (process_method_func)daBd_Create,  (process_method_func)daBd_Delete,
    (process_method_func)daBd_Execute, (process_method_func)daBd_IsDelete,
    (process_method_func)daBd_Draw,
};

extern actor_process_profile_definition g_profile_BD = {
    fpcLy_CURRENT_e,       // mLayerID
    8,                     // mListID
    fpcPi_CURRENT_e,       // mListPrio
    PROC_BD,               // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(bd_class),      // mSize
    0,                     // mSizeOther
    0,                     // mParameters
    &g_fopAc_Method.base,  // sub_method
    718,                   // mPriority
    &l_daBd_Method,        // sub_method
    0x000C4100,            // mStatus
    fopAc_ACTOR_e,         // mActorType
    fopAc_CULLBOX_0_e,     // cullType
};

AUDIO_INSTANCES
