/**
* @file d_a_obj_sw.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

// #define VIRTUAL_3DLINEMAT
#include "d/actor/d_a_obj_sw.h"
#include "d/actor/d_a_player.h"
#include "d/d_camera.h"
#include "d/d_com_inf_game.h"
#include "d/d_path.h"
#include "d/d_s_play.h"
#include "f_op/f_op_camera_mng.h"
#include "m_Do/m_Do_ext.h"

static int daObj_Sw_Draw(obj_sw_class* i_this) {
    static _GXColor l_color = {
        0x14,
        0x0F,
        0x00,
        0xFF,
    };

    fopAc_ac_c* a_this = &i_this->actor;
    if (i_this->field_0x570 == 8) {
        g_env_light.settingTevStruct(0, &a_this->current.pos, &a_this->tevStr);
        obj_sc_s* sc_p = i_this->field_0x5a8;
        for (int i = 0; i < 8; i++) {
            if (sc_p->field_0x0 != 0) {
                g_env_light.setLightTevColorType_MAJI(sc_p->mpModelMorf->getModel(),
                                                      &a_this->tevStr);
                sc_p->mpModelMorf->entryDL();

                if (i == 2 && i_this->field_0x8c8 != NULL) {
                    g_env_light.setLightTevColorType_MAJI(i_this->field_0x8c8, &a_this->tevStr);
                    mDoExt_modelUpdateDL(i_this->field_0x8c8);
                }
            }
            sc_p = (obj_sc_s*)((s8*)sc_p + 100);
        }
    } else {
        g_env_light.settingTevStruct(16, &a_this->current.pos, &a_this->tevStr);
        i_this->field_0xd50.update(63, l_color, &a_this->tevStr);
        dComIfGd_set3DlineMat(&i_this->field_0xd50);
    }

    return 1;
}

static int saru_ct;

static void* s_ks_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_KS) {
        int swBit = fopAcM_GetParam(i_actor) >> 24;
        if (swBit != 0xFF) {
            if (dComIfGs_isSwitch(swBit, fopAcM_GetRoomNo((fopAc_ac_c*)i_actor))) {
                saru_ct++;
            }
        }
    }
    return NULL;
}

struct path {
    int field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
};

static path sc_path[17] = {
    {1, 8172.0f, 3380.0f, -10887.0f},
    {1, 8305.0f, 3524.0f, -11275.0f},
    {1, 8774.0f, 3677.0f, -10728.0f},
    {1, 8794.0f, 3677.0f, -11424.0f},
    {1, 9267.0f, 3887.0f, -10586.0f},
    {1, 9013.0f, 3902.0f, -11735.0f},
    {1, 9184.0f, 3997.0f, -11230.0f},
    {1, 9238.0f, 4127.0f, -11886.0f},
    {1, 9624.0f, 4602.0f, -11648.0f},
    {0, 9513.0f, 4854.0f, -12209.0f},
    {0, 9049.0f, 5035.0f, -12050.0f},
    {0, 8715.0f, 5083.0f, -11785.0f},
    {0, 8428.0f, 5206.0f, -11388.0f},
    {0, 8223.0f, 5353.0f, -11250.0f},
    {0, 7851.0f, 5460.0f, -11269.0f},
    {0, 7599.0f, 5537.0f, -11186.0f},
    {0, 0.0f, 0.0f, 0.0f},
};

static void* s_ksdel_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_NPC_KS) {
        fopAcM_delete((fopAc_ac_c*)i_actor);

        ((obj_sw_class*)i_data)->field_0x5a8[saru_ct].field_0x4.x = sc_path[7 - saru_ct].field_0x4;
        ((obj_sw_class*)i_data)->field_0x5a8[saru_ct].field_0x4.y = sc_path[7 - saru_ct].field_0x8;
        ((obj_sw_class*)i_data)->field_0x5a8[saru_ct].field_0x4.z = sc_path[7 - saru_ct].field_0xc;
        ((obj_sw_class*)i_data)->field_0x5a8[saru_ct].field_0x34 = (saru_ct * 0x14) + 0x3C;
        ((obj_sw_class*)i_data)->field_0x5a8[saru_ct].field_0x38 = 8 - saru_ct;

        saru_ct++;
    }

    return NULL;
}

static void anm_init(obj_sc_s* i_this, int param_2, f32 i_morf, u8 i_mode, f32 i_speed) {
    i_this->mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("Npc_ksw", param_2), i_mode,
                                i_morf, i_speed, 0.0f, -1.0f, NULL);
    i_this->field_0x3c = param_2;
}

static void sc_build(obj_sw_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    obj_sc_s* sc_p = i_this->field_0x5a8;
    cXyz sp24, sp30;

    for (int i = 0; i < 8; i++) {
        if (sc_p->field_0x34 != 0) {
            sc_p->field_0x34--;
        }

        s16 sVar1;
        switch (sc_p->field_0x0) {
        case 0:
            anm_init(sc_p, 14, 5.0f, 2, 1.0f);
            sc_p->field_0x0 = 1;
            sc_p->field_0x10 = sc_p->field_0x4;
            sc_p->field_0x1c.x = sc_path[sc_p->field_0x38].field_0x4;
            sc_p->field_0x1c.y = sc_path[sc_p->field_0x38].field_0x8;
            sc_p->field_0x1c.z = sc_path[sc_p->field_0x38].field_0xc;
            break;

        case 1:
            sp24 = sc_p->field_0x1c - sc_p->field_0x4;
            cLib_addCalcAngleS2(&sc_p->field_0x28.y, cM_atan2s(sp24.x, sp24.z), 1, 0x2000);
            if (sc_p->field_0x34 == 0) {
                sc_p->field_0x0 = 2;
                anm_init(sc_p, 11, 2.0f, 0, 1.0f);
            }
            break;

        case 2:
            if (sc_path[sc_p->field_0x38].field_0x0 == 1) {
                if (sc_p->field_0x3c == 11) {
                    if (sc_p->mpModelMorf->isStop()) {
                        anm_init(sc_p, 12, 1.0f, 0, 1.0f);
                        sc_p->mSound.startSound(Z2SE_KOSARU_V_JUMP, 0, -1);
                        sc_p->mSound.startSound(Z2SE_KOSARU_JUMP_START, 0, -1);
                        sc_p->mSound.startSound(Z2SE_KOSARU_JUMP_WIND, 0, -1);
                        sc_p->field_0x30 = 40.0f;
                    }
                }
            } else {
                sc_p->field_0x30 = 25.0f;
            }

            sp24 = sc_p->field_0x1c - sc_p->field_0x4;
            sVar1 = cM_atan2s(sp24.x, sp24.z);
            cLib_addCalcAngleS2(&sc_p->field_0x28.y, sVar1, 1, 0x2000);
            cMtx_YrotS(*calc_mtx, sVar1);
            cMtx_XrotM(*calc_mtx,
                       -cM_atan2s(sp24.y, JMAFastSqrt(sp24.x * sp24.x + sp24.z * sp24.z)));
            sp24.x = 0.0f;
            sp24.y = 0.0f;
            sp24.z = sc_p->field_0x30;
            MtxPosition(&sp24, &sp30);
            sc_p->field_0x4 += sp30;
            sp24 = sc_p->field_0x1c - sc_p->field_0x4;

            if ((sp24.x * sp24.x + sp24.z * sp24.z) <= 1700.0f) {
                sc_p->field_0x4 = sc_p->field_0x1c;
                if (sc_p->field_0x38 == 16) {
                    sc_p->field_0x0 = 3;
                    anm_init(sc_p, 4, 2.0f, 2, 2.0f);
                    sc_p->field_0x30 = 0.0f;
                    sc_p->mSound.startSound(Z2SE_KOSARU_V_WAIT, 0, -1);
                } else {
                    sc_p->field_0x38++;
                    sc_p->field_0x10 = sc_p->field_0x4;
                    sc_p->field_0x1c.x = sc_path[sc_p->field_0x38].field_0x4;
                    sc_p->field_0x1c.y = sc_path[sc_p->field_0x38].field_0x8;
                    sc_p->field_0x1c.z = sc_path[sc_p->field_0x38].field_0xc;

                    if (sc_path[sc_p->field_0x38].field_0x0 == 1) {
                        anm_init(sc_p, 11, 2.0f, 0, 1.0f);
                        sc_p->field_0x30 = 0.0f;
                    } else if (sc_p->field_0x3c != 10) {
                        anm_init(sc_p, 10, 2.0f, 2, 1.0f);
                    }
                }
            }
            break;

        case 3:
            cLib_addCalcAngleS2(&sc_p->field_0x28.y, a_this->current.angle.y + 0x8000, 2, 0x1000);
            cMtx_YrotS(*calc_mtx, sc_p->field_0x28.y);
            sp24.x = 0.0f;
            sp24.y = 0.0f;
            sp24.z = -35.0f;
            MtxPosition(&sp24, &sp30);
            cLib_addCalc2(&sc_p->field_0x4.x, a_this->eyePos.x + sp30.x, 1.0f, 30.0f);
            cLib_addCalc2(&sc_p->field_0x4.y, a_this->eyePos.y, 1.0f, 30.0f);
            cLib_addCalc2(&sc_p->field_0x4.z, a_this->eyePos.z + sp30.z, 1.0f, 30.0f);

            if (sc_p->field_0x4.y <= a_this->eyePos.y + 1.0f) {
                sc_p->field_0x4 - a_this->eyePos;  // ?
                anm_init(sc_p, 13, 5.0f, 0, 1.0f);
                sc_p->field_0x0 = 10;
                sc_p->mSound.startSound(Z2SE_KOSARU_V_WAIT, 0, -1);

                if (i == 7) {
                    i_this->field_0x576 = 2;
                    i_this->field_0x57a[1] = 120;
                    int swBit = fopAcM_GetParam(a_this) >> 24;
                    if (swBit != 0xFF) {
                        dComIfGs_onSwitch(swBit, fopAcM_GetRoomNo(a_this));
                    }
                }
            }
        }

        if (sc_p->field_0x30 >= 30.0f) {
            sc_p->field_0x35 = 1;
        }

        sc_p = (obj_sc_s*)((s8*)sc_p + 100);
    }
}

static void sc_move(obj_sw_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp24, sp30;

    if (i_this->mDemoMode == 0) {
        if (i_this->field_0x57a[0] == 0) {
            if (cM_rndF(1.0f) < 0.2f) {
                anm_init(&i_this->field_0x5a8[7], 5, 5.0f, 2, 1.0f);
            } else if (cM_rndF(1.0f) < 0.33f) {
                anm_init(&i_this->field_0x5a8[7], 6, 5.0f, 2, 1.0f);
            } else if (cM_rndF(1.0f) < 0.5f) {
                anm_init(&i_this->field_0x5a8[7], 7, 5.0f, 2, 1.0f);
            } else {
                anm_init(&i_this->field_0x5a8[7], 8, 5.0f, 2, 1.0f);
            }

            i_this->field_0x57a[0] = cM_rndF(60.0f) + 40.0f;
            i_this->field_0x5a8[7].mSound.startSound(Z2SE_KOSARU_V_WAIT, 0, -1);
        }

        if (i_this->field_0x57a[2] == 0 && i_this->field_0x584 > 500.0f) {
            sp24 = a_this->eyePos - daPy_getPlayerActorClass()->getLeftHandPos();
            sp24.y *= 0.4f;
            if (sp24.abs() < 250.0f) {
                i_this->mDemoMode = 1;
                anm_init(&i_this->field_0x5a8[7], 13, 2.0f, 0, 1.0f);
                i_this->field_0x580 = -0x1E9B;
                i_this->field_0x588 = 1400.0f;
                i_this->field_0x594 = 0;
                i_this->field_0x590 = 0.0f;
                dComIfGp_getVibration().StartShock(3, 31, cXyz(0.0f, 1.0f, 0.0f));
            }
        }
    }

    if (i_this->field_0x57a[1] == 1) {
        i_this->field_0x588 = 0.0f;
        i_this->field_0x584 = 1400.0f;
        i_this->field_0x590 = 4.0f;
        i_this->field_0x594 = 5000;
        i_this->field_0x580 = 0x4000;
    }
}

static void demo_camera(obj_sw_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* zero_camera_p = dComIfGp_getCamera(0);
    cXyz sp24, sp30;

    switch (i_this->mDemoMode) {
    case 0:
        break;
    case 1:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xFFFF, 0);
            a_this->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }

        i_this->mDemoMode = 2;
        camera->mCamera.Stop();
        i_this->field_0x8ce = 0;
        player->changeOriginalDemo();
        player->changeDemoMode(19, 0, 0, 0);
        player->changeDemoParam0(0);

        cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
        sp24.x = 0.0f;
        sp24.y = 0.0f;
        sp24.z = 400.0f;
        MtxPosition(&sp24, &sp30);
        i_this->field_0x598 = a_this->eyePos + sp30;
        dComIfGp_event_offHindFlag(15);
        // [[fallthrough]]

    case 2:
        dComIfGp_setDoStatusForce(1, 0);
        if (i_this->field_0x8ce < 10) {
            cLib_addCalc2(&i_this->field_0x598.x, a_this->eyePos.x, 1.0f, 40.0f);
            cLib_addCalc2(&i_this->field_0x598.y, a_this->eyePos.y, 1.0f, 40.0f);
            cLib_addCalc2(&i_this->field_0x598.z, a_this->eyePos.z, 1.0f, 40.0f);
        } else {
            i_this->field_0x598 = a_this->eyePos;
        }

        player->setPlayerPosAndAngle(&i_this->field_0x598, a_this->current.angle.y + 0x8000, 0);
        i_this->mDemoCamFovy = 55.0f;
        i_this->field_0x8e8.z = -800.0f;
        i_this->field_0x8e8.x = 300.0f;
        i_this->field_0x8e8.y = -200.0f;
        cLib_addCalc2(&i_this->mDemoCamCenter.x, player->current.pos.x, 0.4f, 200.0f);
        cLib_addCalc2(&i_this->mDemoCamCenter.y, player->current.pos.y + 100.0f, 0.4f, 200.0f);
        cLib_addCalc2(&i_this->mDemoCamCenter.z, player->current.pos.z, 0.4f, 200.0f);

        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        sp24.x = i_this->field_0x8e8.x;
        sp24.y = i_this->field_0x8e8.y;
        sp24.z = i_this->field_0x8e8.z;
        MtxPosition(&sp24, &sp30);
        sp30.x += player->current.pos.x;
        sp30.y += player->current.pos.y;
        sp30.z += player->current.pos.z;

        if (i_this->field_0x8ce == 0) {
            i_this->mDemoCamEye = sp30;
            i_this->mDemoCamCenter = player->current.pos;
            i_this->mDemoCamCenter.y += 100.0f;
        } else {
            cLib_addCalc2(&i_this->mDemoCamEye.x, sp30.x, 0.1f, 200.0f);
            cLib_addCalc2(&i_this->mDemoCamEye.y, sp30.y, 0.1f, 200.0f);
            cLib_addCalc2(&i_this->mDemoCamEye.z, sp30.z, 0.1f, 200.0f);
        }

        player->changeDemoParam1(s16(i_this->field_0x596 * 10));
        if (mDoCPd_c::getTrigA(0) != 0) {
            i_this->field_0x5a8[7].mSound.startSound(Z2SE_KOSARU_V_THROW, 0, -1);
            player->changeDemoMode(24, 0, 0, 0);
            anm_init(&i_this->field_0x5a8[7], 9, 2.0f, 0, 1.0f);
            i_this->mDemoMode = 3;
            i_this->field_0x8ce = 0;
            i_this->field_0x57a[2] = 50;
        }
        break;

    case 3:
        cLib_addCalc2(&i_this->mDemoCamCenter.x, player->current.pos.x, 0.4f, 100.0f);
        cLib_addCalc2(&i_this->mDemoCamCenter.y, player->current.pos.y, 0.4f, 100.0f);
        cLib_addCalc2(&i_this->mDemoCamCenter.z, player->current.pos.z, 0.4f, 100.0f);

        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        sp24.x = 0.0f;
        sp24.y = 0.0f;
        sp24.z = -400.0f;
        MtxPosition(&sp24, &sp30);
        sp30.x += player->current.pos.x;
        sp30.y += player->current.pos.y;
        sp30.z += player->current.pos.z;

        cLib_addCalc2(&i_this->mDemoCamEye.x, sp30.x, 0.1f, 50.0f);
        cLib_addCalc2(&i_this->mDemoCamEye.y, sp30.y, 0.1f, 50.0f);
        cLib_addCalc2(&i_this->mDemoCamEye.z, sp30.z, 0.1f, 50.0f);

        if (i_this->field_0x8ce == 15) {
            i_this->mDemoMode = 100;
            a_this->current.angle.y += -0x8000;
        }
        break;

    case 100:
        camera->mCamera.Reset(i_this->mDemoCamCenter, i_this->mDemoCamEye, i_this->mDemoCamFovy, 0);
        camera->mCamera.Start();
        camera->mCamera.SetTrimSize(0);
        dComIfGp_event_reset();
        daPy_getPlayerActorClass()->cancelOriginalDemo();
        i_this->mDemoMode = 0;
    }

    if (i_this->mDemoMode != 0) {
        camera->mCamera.Set(i_this->mDemoCamCenter, i_this->mDemoCamEye, i_this->mDemoCamFovy, 0);
        i_this->field_0x8ce++;
        if (i_this->field_0x8ce > 10000) {
            i_this->field_0x8ce = 10000;
        }
    }
}

static void sc_action(obj_sw_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    cXyz sp90;
    int swBit = fopAcM_GetParam(a_this) >> 24;

    switch (i_this->field_0x576) {
    case 0:
        saru_ct = 0;
        fpcM_Search(s_ks_sub, i_this);
        if (saru_ct == 8 &&
            ((swBit != 0xFF && dComIfGs_isSwitch(swBit, fopAcM_GetRoomNo(a_this))) ||
             !dComIfGp_event_runCheck()) &&
            dComIfGs_isSwitch(83, fopAcM_GetRoomNo(a_this)))
        {
            saru_ct = 0;
            fpcM_Search(s_ksdel_sub, i_this);
            a_this->eyePos = a_this->home.pos;
            if (swBit != 0xFF) {
                if (dComIfGs_isSwitch(swBit, fopAcM_GetRoomNo(a_this))) {
                    i_this->field_0x576 = 2;
                    i_this->field_0x57a[1] = 120;
                    for (int i = 0; i < 8; i++) {
                        i_this->field_0x5a8[i].field_0x0 = 10;
                        i_this->field_0x5a8[i].field_0x28.x = -0x8000;
                    }
                    break;
                }
            }

            dComIfGs_onSwitch(121, fopAcM_GetRoomNo(a_this));
            i_this->field_0x576 = 1;
            sc_path[16].field_0x4 = a_this->home.pos.x;
            sc_path[16].field_0x8 = a_this->home.pos.y;
            sc_path[16].field_0xc = a_this->home.pos.z;
        }
        break;

    case 1:
        sc_build(i_this);
        break;

    case 2:
        sc_move(i_this);
        break;
    }

    cLib_addCalc2(&i_this->field_0x588, i_this->field_0x584, 0.1f, 10.0f);
    i_this->field_0x58c = (1400.0f - i_this->field_0x588) / 5.0f;
    cLib_addCalc0(&i_this->field_0x590, 0.1f, 0.05f);
    cLib_addCalcAngleS2(&i_this->field_0x594, 00, 1, 5);

    obj_sc_s* sc_p = i_this->field_0x5a8;
    mDoMtx_stack_c::transS(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z);
    mDoMtx_stack_c::YrotM(a_this->current.angle.y + 0x8000);

    f32 fVar1 = 1.0f;
    for (int i = 0; i < 8;) {
        s16 sVar1 = i_this->field_0x588 *
                    (fVar1 * cM_ssin((i_this->field_0x580 * 800) - i * i_this->field_0x594));
        s16 sVar2 = i_this->field_0x58c * cM_ssin((i_this->field_0x582 * 1000) - i * -10000);
        sc_p->field_0x28.z = sVar2;
        if (i == 7) {
            i_this->field_0x596 = sVar1;
        }

        mDoMtx_stack_c::XrotM(sVar1);
        mDoMtx_stack_c::ZrotM(sc_p->field_0x28.z);

        if (sc_p->field_0x0 == 10) {
            cLib_addCalcAngleS2(&sc_p->field_0x28.y, a_this->current.angle.y + 0x8000, 2, 0x1000);
            cLib_addCalcAngleS2(&sc_p->field_0x28.x, -0x8000, 2, 0x1000);
            mDoMtx_stack_c::push();
            mDoMtx_stack_c::XrotM(sc_p->field_0x28.x);

            sc_p->mpModelMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
            sc_p->mpModelMorf->play(NULL, 0, 0);
            sc_p->mpModelMorf->modelCalc();

            if (i == 2 && i_this->field_0x8c8 != NULL) {
                i_this->field_0x8c8->setBaseTRMtx(sc_p->mpModelMorf->getModel()->getAnmMtx(4));
            }

            sc_p->mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
            mDoMtx_stack_c::pop();
        }

        mDoMtx_stack_c::transM(0.0f, -230.0f, 0.0f);
        fVar1 += i_this->field_0x590;
        if (sc_p->field_0x0 == 10) {
            sp90.set(0.0f, 0.0f, 0.0f);
            mDoMtx_stack_c::multVec(&sp90, &a_this->eyePos);
            a_this->attention_info.position = a_this->eyePos;
            a_this->attention_info.position.y += 40.0f;
        }

        i++;
        sc_p++;
    }

    i_this->field_0x580--;
    i_this->field_0x582++;
    sc_p = i_this->field_0x5a8;
    for (int i = 0; i < 8; i++) {
        if (sc_p->field_0x0 < 10) {
            f32 fVar2 = 0.0f;
            f32 fVar3;
            if (sc_p->field_0x35 != 0) {
                sp90 = sc_p->field_0x1c - sc_p->field_0x10;
                fVar3 = sp90.abs();
                sp90 = sc_p->field_0x1c - sc_p->field_0x4;
                fVar2 = sp90.abs();
                f32 fVar4 = fVar3 * 0.3f;
                if (fVar4 > 1000.0f) {
                    fVar4 = 1000.0f;
                }
                fVar2 = fVar4 * cM_ssin((fVar2 / fVar3) * 32768.0f);
                sc_p->field_0x35 = 0;
            }

            mDoMtx_stack_c::transS(sc_p->field_0x4.x, sc_p->field_0x4.y + fVar2, sc_p->field_0x4.z);
            mDoMtx_stack_c::YrotM(sc_p->field_0x28.y);
            mDoMtx_stack_c::XrotM(sc_p->field_0x28.x);
            mDoMtx_stack_c::ZrotM(sc_p->field_0x28.z);
            sc_p->mpModelMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
            sc_p->mpModelMorf->play(NULL, 0, 0);
            sc_p->mpModelMorf->modelCalc();
            sc_p->mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
        }

        sc_p++;
    }

    demo_camera(i_this);
}

static void sw_action(obj_sw_class* i_this) {
    f32 tmp;
    f32 fVar1;
    f32 fVar2;
    f32 fVar3;
    f32 fVar4;
    cXyz sp98;
    cXyz spa4;
    cXyz spb0 = i_this->field_0x904[1] - i_this->field_0x904[0];
    cXyz i_position = i_this->field_0x904[0] + (spb0 * 0.5f);
    cXyz spc8;

    fVar1 = -(i_this->field_0x91c * 4.0f + 50.0f);

    cXyz i_direction;
    f32 i_power;
    dKyw_get_AllWind_vec(&i_position, &i_direction, &i_power);
    i_power *= i_power;

    cLib_addCalc2(&i_this->field_0x5a4, i_power, 0.1f, 0.03f);
    cMtx_YrotS(*calc_mtx, cM_atan2s(i_direction.x, i_direction.z));

    for (int i = 0; i < i_this->field_0xd8c; i++) {
        spc8 = (spb0 / (i_this->field_0xd8c - 1)) * i;

        fVar2 = cM_ssin(((f32)i / (i_this->field_0xd8c - 1)) * 32768.0f);
        tmp = i_this->field_0x8fc *
              cM_ssin(i_this->field_0x574 * (XREG_S(0) + 0x9C4) + i * (XREG_S(1) + 2000));

        spc8.y += (fVar2 * fVar1) + (fVar2 * tmp);

        sp98.x = 0.0f;
        sp98.y = 0.0f;
        sp98.z = fVar2 * (70.0f + JREG_F(15)) * i_this->field_0x5a4;
        MtxPosition(&sp98, &spa4);

        spc8.x += spa4.x;
        spc8.z += spa4.z;
        i_this->field_0x920[i] = i_this->field_0x904[0] + spc8;
    }

    for (int i = 0; i < i_this->field_0xd8c; i++) {
        if (i_this->field_0xd10[i] != 0) {
            i_this->field_0xd10[i]--;
            fVar3 = -(i_this->field_0x91c * 8.0f + 30.0f);
            fVar4 = 10.0f;
        } else {
            fVar3 = 0.0f;
            fVar4 = 5.0f;
        }

        cLib_addCalc2(&i_this->field_0xc14[i], fVar3, 0.5f, fVar4);
        f32 divres;
        if (i_this->field_0xc14[i]) {
            for (int j = i; j < i_this->field_0xd8c; j++) {
                divres = (f32)(i_this->field_0xd8c - 1 - j) / (i_this->field_0xd8c - 1 - i);
                i_this->field_0x920[j].y += i_this->field_0xc14[i] * divres;
            }

            for (int k = i - 1; k >= 0; k--) {
                divres = (f32)k / i;
                i_this->field_0x920[k].y += i_this->field_0xc14[i] * divres;
            }
        }
    }

    if (i_this->field_0x900 > 5.0f) {
        i_this->field_0x900 = 5.0f;
    }

    cLib_addCalc2(&i_this->field_0x8fc, i_this->field_0x900, 0.1f, 0.075f + XREG_F(2));
    i_this->field_0x900 = 0.0f;
}

static int daObj_Sw_Execute(obj_sw_class* i_this) {
    for (int i = 0; i < 3; i++) {
        if (i_this->field_0x57a[i] != 0) {
            i_this->field_0x57a[i]--;
        }
    }

    i_this->field_0x574++;
    if (i_this->field_0x570 == 8) {
        sc_action(i_this);
    } else {
        sw_action(i_this);
        cXyz* other = i_this->field_0x920;
        cXyz* c_this = i_this->field_0xd50.getPos(0);
        for (int i = 0; i < 63; i++, c_this++, other++) {
            if (i < i_this->field_0xd8c) {
                *c_this = *other;
            } else {
                *c_this = i_this->field_0x904[1];
            }
        }
    }

    return 1;
}

static int daObj_Sw_IsDelete(obj_sw_class* i_this) {
    return 1;
}

static int daObj_Sw_Delete(obj_sw_class* i_this) {
    fopAc_ac_c* a_this = &i_this->actor;
    fopAcM_GetID(a_this);
    if (i_this->field_0x570 == 8) {
        dComIfG_resDelete(&i_this->mPhase, "Npc_ksw");
    } else {
        dComIfG_resDelete(&i_this->mPhase, "Obj_sw");
    }

    if (a_this->heap != NULL) {
        for (int i = 0; i < 8; i++) {
            i_this->field_0x5a8[i].mSound.deleteObject();
        }
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    obj_sw_class* i_this = (obj_sw_class*)a_this;

    if (i_this->field_0x570 == 8) {
        for (int i = 0; i < 8; i++) {
            i_this->field_0x5a8[i].mpModelMorf =
                new mDoExt_McaMorf((J3DModelData*)dComIfG_getObjectRes("Npc_ksw", 17), NULL, NULL,
                                   (J3DAnmTransform*)dComIfG_getObjectRes("Npc_ksw", 13), 2, 1.0f,
                                   0, -1, 1, NULL, 0x80000, 0x11000084);

            if (i_this->field_0x5a8[i].mpModelMorf == NULL ||
                i_this->field_0x5a8[i].mpModelMorf->getModel() == NULL)
            {
                return 0;
            }

            mDoMtx_stack_c::scaleS(0.0f, 0.0f, 0.0f);
            i_this->field_0x5a8[i].mpModelMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());

            if (i == 2) {
                J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Npc_ksw", 18);
                JUT_ASSERT(1220, modelData != NULL);
                i_this->field_0x8c8 = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
                if (i_this->field_0x8c8 == NULL) {
                    return 0;
                }
                i_this->field_0x8c8->setBaseTRMtx(mDoMtx_stack_c::get());
            }

            i_this->field_0x5a8[i].mSound.init(&i_this->field_0x5a8[i].field_0x4, 1);
        }
    } else {
        if (i_this->field_0xd50.init(1, 63, (ResTIMG*)dComIfG_getObjectRes("Obj_sw", 3), 1) == 0) {
            return 0;
        }

        f32* size_p = i_this->field_0xd50.getSize(0);
        for (int i = 0; i < 63; i++) {
            *size_p = 6.0f;
            size_p++;
        }
    }

    return 1;
}

static int daObj_Sw_Create(fopAc_ac_c* a_this) {
    obj_sw_class* i_this = (obj_sw_class*)a_this;
    int phase;
    u32 i_size;
    fopAcM_ct(a_this, obj_sw_class);

    i_this->field_0x570 = fopAcM_GetParam(a_this);
    if (i_this->field_0x570 == 0xFF) {
        i_this->field_0x570 = 0;
    }
    OS_REPORT("OBJ_SW//////////////OBJ_SW SET 0 !!\n");

    if (i_this->field_0x570 == 8) {
        phase = dComIfG_resLoad(&i_this->mPhase, "Npc_ksw");
        i_size = 0x10140;
    } else {
        phase = dComIfG_resLoad(&i_this->mPhase, "Obj_sw");
        i_size = 0x1B00;
    }

    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("OBJ_SW PARAM %x\n", fopAcM_GetParam(a_this));
        OS_REPORT("OBJ_SW//////////////OBJ_SW SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, i_size)) {
            OS_REPORT("//////////////OBJ_SW SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////OBJ_SW SET 2 !!\n");
        if (i_this->field_0x570 == 8) {
            a_this->attention_info.distances[2] = 22;
            a_this->home.pos.y += -70.0f;
            a_this->current.pos.y = a_this->home.pos.y;

            fopAc_ac_c* player = dComIfGp_getPlayer(0);
            if (player != NULL && player->current.pos.z < -14000.0f) {
                a_this->current.angle.y += -0x8000;
            }
            return phase;
        }

        i_this->field_0x571 = fopAcM_GetParam(a_this) >> 8;
        u8 path_index = fopAcM_GetParam(a_this) >> 16;
        int swBit = fopAcM_GetParam(a_this) >> 24;
        if (path_index == 0xFF) {
            if (fopAcM_GetRoomNo(a_this) == 0) {
                if (i_this->field_0x570 == 0) {
                    i_this->field_0x904[0].set(1000.0f, 3450.0f, 12670.0f);
                    i_this->field_0x904[1].set(200.0f, 3450.0f, 12670.0f);
                } else {
                    i_this->field_0x904[0].set(-200.0f, 3450.0f, 12670.0f);
                    i_this->field_0x904[1].set(-1000.0f, 3450.0f, 12670.0f);
                }
            } else {
                i_this->field_0x904[0].set(-100.0f, 3380.0f, 6280.0f);
                i_this->field_0x904[1].set(-100.0f, 3380.0f, 3780.0f);
            }
        } else {
            dPath* path_p = dPath_GetRoomPath(path_index, fopAcM_GetRoomNo(a_this));
            if (path_p != NULL) {
                dPnt* pnt_p = path_p->m_points;
                i_this->field_0x904[0] = pnt_p->m_position;
                i_this->field_0x904[1] = (pnt_p + 1)->m_position;
            } else {
                OS_REPORT("OJBSW NON PATH !!!!!!!!! %d!\n");
                return cPhs_ERROR_e;
            }
        }

        daObj_Sw_Execute(i_this);
        cXyz sp34 = i_this->field_0x904[1] - i_this->field_0x904[0];
        a_this->current.pos = i_this->field_0x904[0];
        a_this->current.angle.y = cM_atan2s(sp34.x, sp34.z);

        OS_REPORT("OBJ_SW DIS  %f\n");
        if (sp34.abs() > 5600.0f) {
            OS_REPORT("OBJ_SW TOO LONG !!!!!!!\n");
            return cPhs_ERROR_e;
        }

        if (fopAcM_GetRoomNo(a_this) == 0) {
            i_this->field_0x91c = 1;
        } else if (fopAcM_GetRoomNo(a_this) == 2) {
            i_this->field_0x91c = 2;
        } else if (fopAcM_GetRoomNo(a_this) == 4) {
            i_this->field_0x91c = 4;
        } else {
            return cPhs_ERROR_e;
        }

        i_this->field_0xd8c = (i_this->field_0x91c * 63) / 8;
        if (fopAcM_GetRoomNo(a_this) == 2 || fopAcM_GetRoomNo(a_this) == 4) {
            fopAcM_createChild(PROC_OBJ_BRG, fopAcM_GetID(a_this), swBit << 24 | 0xFFFFFF,
                               &a_this->current.pos, fopAcM_GetRoomNo(a_this),
                               &a_this->current.angle, NULL, -1, NULL);
        }

        if (fopAcM_GetRoomNo(a_this) == 4 && swBit != 0xFF) {
            if (!dComIfGs_isSwitch(swBit, fopAcM_GetRoomNo(a_this))) {
                fopAcM_createChild(PROC_E_MK, fopAcM_GetID(a_this), swBit << 16 | 1,
                                   &a_this->current.pos, fopAcM_GetRoomNo(a_this),
                                   &a_this->current.angle, NULL, -1, NULL);
                return cPhs_ERROR_e;
            }
        }
    }

    return phase;
}

static actor_method_class l_daObj_Sw_Method = {
    (process_method_func)daObj_Sw_Create,  (process_method_func)daObj_Sw_Delete,
    (process_method_func)daObj_Sw_Execute, (process_method_func)daObj_Sw_IsDelete,
    (process_method_func)daObj_Sw_Draw,
};

actor_process_profile_definition g_profile_OBJ_SW = {
    fpcLy_CURRENT_e,       // mLayerID
    4,                     // mListID
    fpcPi_CURRENT_e,       // mListPrio
    PROC_OBJ_SW,           // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(obj_sw_class),  // mSize
    0,                     // mSizeOther
    0,                     // mParameters
    &g_fopAc_Method.base,  // sub_method
    60,                    // mPriority
    &l_daObj_Sw_Method,    // sub_method
    0x00044000,            // mStatus
    fopAc_ACTOR_e,         // mActorType
    fopAc_CULLBOX_0_e,     // cullType
};
