/**
* @file d_a_e_warpappear.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_warpappear.h"
#include "d/actor/d_a_e_s1.h"
#include "d/actor/d_a_horse.h"
#include "d/actor/d_a_obj_ihasi.h"
#include "f_op/f_op_camera_mng.h"

static int daE_Warpappear_Draw(e_warpappear_class* i_this) {
    int i_no = (fopAcM_GetParam(i_this) >> 8) & 0xFF;
    if (i_no != 0xff) {
        if (!dComIfGs_isSwitch(i_no, fopAcM_GetRoomNo(i_this))) {
            return 1;
        }
    }

    dComIfGd_setListDark();
    i_this->mpBtk->entry(i_this->mpModel->getModelData());
    i_this->mpBrks[i_this->field_0x584]->entry(i_this->mpModel->getModelData());
    mDoExt_modelUpdateDL(i_this->mpModel);
    dComIfGd_setList();
    return 1;
}

static e_s1_class* master_ns[3];

static void* s_s1_sub(void* i_actor, void* i_data) {
    e_s1_class* shadowBeast = (e_s1_class*)i_actor;

    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_S1) {
        shadowBeast->mAction = 20;
        shadowBeast->mMode = 0;

        f32 fVar1 = -15336.0f;
        f32 fVar2 = -671.0f;
        fVar1 -= shadowBeast->home.pos.x;
        fVar2 -= shadowBeast->home.pos.z;
        if (fVar1 * fVar1 + fVar2 * fVar2 < 40000.0f) {
            master_ns[0] = shadowBeast;
        }

        fVar1 = -16552.0f;
        fVar2 = -144.0f;
        fVar1 -= shadowBeast->home.pos.x;
        fVar2 -= shadowBeast->home.pos.z;
        if (fVar1 * fVar1 + fVar2 * fVar2 < 40000.0f) {
            master_ns[1] = shadowBeast;
        }

        fVar1 = -14796.0f;
        fVar2 = 382.0f;
        fVar1 -= shadowBeast->home.pos.x;
        fVar2 -= shadowBeast->home.pos.z;
        if (fVar1 * fVar1 + fVar2 * fVar2 < 40000.0f) {
            master_ns[2] = shadowBeast;
        }
    }

    return NULL;
}

static void* s_s1drop_sub(void* i_actor, void* i_data) {
    e_s1_class* shadowBeast = (e_s1_class*)i_actor;
    fopAc_ac_c* data = (fopAc_ac_c*)i_data;
    if ((fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_S1) &&
        shadowBeast->mMode != 2)
    {
        shadowBeast->current.pos = data->current.pos;
        shadowBeast->current.pos.y += 400.0f;
        shadowBeast->mMode = 2;
        shadowBeast->scale.x = 2.0f;
        return i_actor;
    }

    return NULL;
}

static void* s_s1riv_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_S1) {
        e_s1_class* shadowBeast = (e_s1_class*)i_actor;
        shadowBeast->current.pos = master_ns[0]->home.pos;
        shadowBeast->old = shadowBeast->current;
        shadowBeast->speed.y = 0.0f;
        shadowBeast->mMode = 10;

        if (shadowBeast != master_ns[0]) {
            shadowBeast->current.pos.y = master_ns[0]->home.pos.y + 50000.0f;
            shadowBeast->gravity = 0.0f;
        } else {
            shadowBeast->current.pos.y = master_ns[0]->home.pos.y + 3000.0f;
        }

        shadowBeast->scale.x = 1.0f;
    }

    return NULL;
}

static void* s_s1fight_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_S1) {
        e_s1_class* shadowBeast = (e_s1_class*)i_actor;
        shadowBeast->mMode = 20;
    }

    return NULL;
}

static int entry_no;

static void* s_s1entry_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_S1) {
        e_s1_class* shadowBeast = (e_s1_class*)i_actor;
        e_warpappear_class* data = (e_warpappear_class*)i_data;

        cXyz sp28 = shadowBeast->current.pos - data->current.pos;
        sp28.y = 0.0f;

        if (shadowBeast->mAction != 20) {
            shadowBeast->mAction = 20;
            shadowBeast->mMode = 0;
            shadowBeast->mEntryNo = entry_no;
            entry_no++;

            data->field_0x64c++;
        }
    }

    return NULL;
}

static void* s_s1drop2_sub(void* i_actor, void* i_data) {
    e_s1_class* shadowBeast = (e_s1_class*)i_actor;
    e_warpappear_class* data = (e_warpappear_class*)i_data;

    if ((fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_S1) &&
        data->field_0x64c == shadowBeast->mEntryNo)
    {
        shadowBeast->current.pos = data->current.pos;
        shadowBeast->current.pos.y += 400.0f;

        shadowBeast->mMode = 2;

        shadowBeast->scale.x = 2.0f;

        return i_actor;
    }

    return NULL;
}

static s16 ew_s1angy;

static void* s_s1Yangset_sub(void* i_actor, void* i_data) {
    e_s1_class* shadowBeast = (e_s1_class*)i_actor;
    e_warpappear_class* data = (e_warpappear_class*)i_data;

    if ((fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_S1) &&
        data->field_0x64c == shadowBeast->mEntryNo)
    {
        shadowBeast->shape_angle.y = shadowBeast->current.angle.y = ew_s1angy;

        return i_actor;
    }

    return NULL;
}

static void* s_s1start_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_S1) {
        e_s1_class* shadowBeast = (e_s1_class*)i_actor;
        fopAc_ac_c* data = (fopAc_ac_c*)i_data;

        cXyz sp28 = shadowBeast->current.pos - data->current.pos;
        sp28.y = 0.0f;

        if (sp28.abs() < 2000.0f) {
            shadowBeast->current = shadowBeast->home;
            shadowBeast->old = shadowBeast->home;
            shadowBeast->shape_angle = shadowBeast->current.angle;
            shadowBeast->mMode = 20;
            shadowBeast->scale.x = 1.0f;
        }
    }

    return NULL;
}

static void action(e_warpappear_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    f32 fVar1 = -15200.0f - player->current.pos.x;
    f32 fVar2 = -35.0f - player->current.pos.z;
    fVar1 = JMAFastSqrt(fVar1 * fVar1 + fVar2 * fVar2);
    s8 bVar1 = false;

    switch (i_this->field_0x596) {
    case 0:
        if (i_this->field_0x570 == 53) {
            if (i_this->field_0x64c == 0) {
                entry_no = 0;
                fpcM_Search(s_s1entry_sub, i_this);
            }

                /* dSv_event_flag_c::M_031 - Goron Mines - Goron Mines clear */
            if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x40])
                && dComIfGs_isSwitch(18, fopAcM_GetRoomNo(a_this)))
            {
                i_this->field_0x596 = 1;
                i_this->field_0x59e = 20;
            }

        } else if (i_this->field_0x570 == 255) {
            fpcM_Search(s_s1_sub, i_this);
            if (fVar1 < 950.0f) {
                i_this->field_0x59e = 1;
                i_this->field_0x596 = 1;
            }

        } else {
            entry_no = 0;
            fpcM_Search(s_s1entry_sub, i_this);
            if (i_this->field_0x64c != 0) {
                u32 i_no = fopAcM_GetParamBit(a_this, 8, 8);
                if ((s32)i_no != 0xff && !dComIfGs_isSwitch(i_no, fopAcM_GetRoomNo(a_this))) {
                    return;
                }

                if (i_this->field_0x59a[0] == 0 && fopAcM_searchPlayerDistanceXZ(a_this) < 1000.0f)
                {
                    i_this->field_0x59e = 10;
                    i_this->field_0x596 = 1;
                }
            }
        }
        break;

    case 1:
        break;

    case 10:
        i_this->mpBrks[0]->setPlaySpeed(1.0f);
        bVar1 = true;
        break;

    case 20:
        i_this->field_0x584 = 1;
        break;
    }

    if (bVar1 != 0) {
        cXyz sp68(i_this->scale.x, i_this->scale.x, i_this->scale.x);
        if (i_this->field_0x598 == 0) {
            i_this->field_0x598++;
            dComIfGp_particle_set(0x84a4, &a_this->current.pos, NULL, &sp68);
        }

        i_this->mParticle1 =
            dComIfGp_particle_set(i_this->mParticle1, 0x84a6, &a_this->current.pos, NULL, &sp68);
        i_this->mParticle2 =
            dComIfGp_particle_set(i_this->mParticle2, 0x84a7, &a_this->current.pos, NULL, &sp68);
    }
}

static void demo_camera(e_warpappear_class* i_this) {
    static u16 w_id[20] = {
        0x850D, 0x850E, 0x850F, 0x8510, 0x8511, 0x8512, 0x8513, 0x8514, 0x8515, 0x8516,
        0x8517, 0x8518, 0x8519, 0x851A, 0x851B, 0x851C, 0x851D, 0x851E, 0x851F, 0x8520,
    };

    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    daHorse_c* horseActor = (daHorse_c*)dComIfGp_getHorseActor();

    cXyz sp48, sp54;
    dComIfG_play_c* play;
    switch (i_this->field_0x59e) {
    case 1:
        if (!i_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(i_this, 2, 0xffff, 0);
            i_this->eventInfo.onCondition(2);
            return;
        }

        camera->mCamera.Stop();
        i_this->field_0x59e = 2;
        i_this->field_0x5a0 = 0;
        i_this->field_0x5f4 = 55.0f;
        i_this->field_0x5a0 = 0;
        i_this->field_0x5ec = 300.0f;
        i_this->field_0x5f0 = 1000.0f;
        i_this->field_0x5f8 = 0.0f;

        camera->mCamera.SetTrimSize(3);
        daPy_getPlayerActorClass()->changeOriginalDemo();
        daPy_getPlayerActorClass()->changeDemoMode(23, 0, 0, 0);
        // [[fallthrough]]

    case 2:
        sp54.set(-15600.0f, 0.0f, -200.0f);
        player->setPlayerPosAndAngle(&sp54, 0x4ef8, 0);
        i_this->current.pos.x = sp54.x;
        i_this->current.pos.z = sp54.z;
        i_this->field_0x5ec = 200.0f;
        i_this->field_0x5f0 = -500.0f;

        mDoMtx_stack_c::YrotS(player->shape_angle.y);
        sp48.x = -100.0f;
        sp48.y = i_this->field_0x5ec;
        sp48.z = i_this->field_0x5f0;

        mDoMtx_stack_c::multVec(&sp48, &i_this->field_0x5a4);
        i_this->field_0x5a4 += player->current.pos;
        i_this->field_0x5b0 = player->current.pos;
        i_this->field_0x5b0.y += 100.0f;

        if (i_this->field_0x5a0 > 20) {
            i_this->field_0x59e = 3;
            i_this->field_0x5a0 = 0;
        }
        break;

    case 3:
        cLib_addCalc2(&i_this->field_0x5b0.y, i_this->current.pos.y, 0.1f, i_this->field_0x5f8);
        cLib_addCalc2(&i_this->field_0x5f8, 200.0f, 1.0f, 10.0f);
        if (i_this->field_0x5a0 == 40) {
            i_this->field_0x596 = 10;
            Z2GetAudioMgr()->seStart(Z2SE_DEMO_NS_WARPHOLE, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                     0);
        }

        if (i_this->field_0x5a0 > 150) {
            i_this->field_0x59e = 4;
            i_this->field_0x5a0 = 0;
        }
        break;

    case 4:
        sp54.set(-15600.0f, 0.0f, 200.0f);
        player->setPlayerPosAndAngle(&sp54, 0x6e38, 0);
        i_this->current.pos.x = sp54.x;
        i_this->current.pos.z = sp54.z;
        cLib_addCalc2(&i_this->field_0x5f4, 70.0f, 0.05f, 0.08f);
        mDoMtx_stack_c::YrotS(player->shape_angle.y);
        sp48.x = -100.0f;
        sp48.y = -1000.0f;
        sp48.z = -1500.0f;
        mDoMtx_stack_c::multVec(&sp48, &i_this->field_0x5a4);
        i_this->field_0x5a4 += i_this->current.pos;
        i_this->field_0x5b0 = i_this->current.pos;
        i_this->field_0x5b0.y += -300.0f;

        if (i_this->field_0x5a0 == 50 || i_this->field_0x5a0 == 70 || i_this->field_0x5a0 == 90) {
            fpcM_Search(s_s1drop_sub, i_this);
        }

        if (i_this->field_0x5a0 == 110) {
            i_this->field_0x596 = 20;
        }

        if (i_this->field_0x5a0 == 200) {
            i_this->field_0x59e = 5;
            i_this->field_0x5a0 = 0;
            fpcM_Search(s_s1riv_sub, i_this);

            i_this->field_0x5b0.set(-14654.0f, 13.0f, -1518.0f);
            i_this->field_0x5a4.set(-15595.0f, 140.0f, -335.0f);
            i_this->field_0x5c8.set(-14932.0f, -19.0f, -847.0f);
            i_this->field_0x5bc.set(-15806.0f, 92.0f, 385.0f);

            i_this->field_0x5d4 = fabsf(i_this->field_0x5bc.x - i_this->field_0x5a4.x);
            i_this->field_0x5d8 = fabsf(i_this->field_0x5bc.y - i_this->field_0x5a4.y);
            i_this->field_0x5dc = fabsf(i_this->field_0x5bc.z - i_this->field_0x5a4.z);
            i_this->field_0x5e0 = fabsf(i_this->field_0x5c8.x - i_this->field_0x5b0.x);
            i_this->field_0x5e4 = fabsf(i_this->field_0x5c8.y - i_this->field_0x5b0.y);
            i_this->field_0x5e8 = fabsf(i_this->field_0x5c8.z - i_this->field_0x5b0.z);
            i_this->field_0x5b0.y += 4200.0f;
            i_this->field_0x5f8 = 0.0f;
            i_this->field_0x5f4 = 55.0f;
        }
        break;

    case 5:
    case 6:
        sp54.set(-15600.0f, 0.0f, 200.0f);
        player->setPlayerPosAndAngle(&sp54, 0x6e38, 0);
        if (i_this->field_0x5a0 == 38) {
            dComIfGp_getVibration().StartShock(5, 1, cXyz(0.0f, 1.0f, 0.0f));
            i_this->field_0x5fc = 30.0f;
        }

        if (i_this->field_0x5a0 <= 80) {
            cLib_addCalc2(&i_this->field_0x5b0.y, 13.0f, 0.4f, 100.0f);
        } else {
            f32 fVar1 = 0.1f;
            if (i_this->field_0x59e == 6) {
                fVar1 = 0.2f;
            }

            cLib_addCalc2(&i_this->field_0x5b0.x, i_this->field_0x5c8.x, fVar1,
                          i_this->field_0x5e0 * i_this->field_0x5f8);
            cLib_addCalc2(&i_this->field_0x5b0.y, i_this->field_0x5c8.y, fVar1,
                          i_this->field_0x5e4 * i_this->field_0x5f8);
            cLib_addCalc2(&i_this->field_0x5b0.z, i_this->field_0x5c8.z, fVar1,
                          i_this->field_0x5e8 * i_this->field_0x5f8);
            cLib_addCalc2(&i_this->field_0x5a4.x, i_this->field_0x5bc.x, fVar1,
                          i_this->field_0x5d4 * i_this->field_0x5f8);
            cLib_addCalc2(&i_this->field_0x5a4.y, i_this->field_0x5bc.y, fVar1,
                          i_this->field_0x5d8 * i_this->field_0x5f8);
            cLib_addCalc2(&i_this->field_0x5a4.z, i_this->field_0x5bc.z, fVar1,
                          i_this->field_0x5dc * i_this->field_0x5f8);
            cLib_addCalc2(&i_this->field_0x5f8, 0.05f, 1.0f, 0.002f);
        }

        if (i_this->field_0x5a0 == 150) {
            i_this->field_0x5b0.set(-16437.0f, 251.0f, -605.0f);
            i_this->field_0x5a4.set(-15451.0f, 45.0f, 526.0f);
            i_this->field_0x5c8.set(-16531.0f, 251.0f, -511.0f);
            i_this->field_0x5bc.set(-15454.0f, 45.0f, 501.0f);
            i_this->field_0x5d4 = fabsf(i_this->field_0x5bc.x - i_this->field_0x5a4.x) * 0.2f;
            i_this->field_0x5d8 = fabsf(i_this->field_0x5bc.y - i_this->field_0x5a4.y) * 0.2f;
            i_this->field_0x5dc = fabsf(i_this->field_0x5bc.z - i_this->field_0x5a4.z) * 0.2f;
            i_this->field_0x5e0 = fabsf(i_this->field_0x5c8.x - i_this->field_0x5b0.x) * 0.2f;
            i_this->field_0x5e4 = fabsf(i_this->field_0x5c8.y - i_this->field_0x5b0.y) * 0.2f;
            i_this->field_0x5e8 = fabsf(i_this->field_0x5c8.z - i_this->field_0x5b0.z) * 0.2f;
            i_this->field_0x5f8 = 0.02f;

            master_ns[1]->current.pos.x = master_ns[1]->home.pos.x;
            master_ns[1]->current.pos.z = master_ns[1]->home.pos.z;
            master_ns[1]->current.pos.y = master_ns[1]->home.pos.y + 500.0f;
            master_ns[1]->speed.y = -120.0f;
            master_ns[1]->gravity = -7.0f;
        }

        if (i_this->field_0x5a0 == 210) {
            i_this->field_0x5b0.set(-14474.0f, 87.0f, -154.0f);
            i_this->field_0x5a4.set(-15888.0f, 40.0f, 390.0f);
            i_this->field_0x5c8.set(-14447.0f, -131.0f, 15.0f);
            i_this->field_0x5bc.set(-15895.0f, 145.0f, 365.0f);
            i_this->field_0x5d4 = fabsf(i_this->field_0x5bc.x - i_this->field_0x5a4.x) * 0.2f;
            i_this->field_0x5d8 = fabsf(i_this->field_0x5bc.y - i_this->field_0x5a4.y) * 0.2f;
            i_this->field_0x5dc = fabsf(i_this->field_0x5bc.z - i_this->field_0x5a4.z) * 0.2f;
            i_this->field_0x5e0 = fabsf(i_this->field_0x5c8.x - i_this->field_0x5b0.x) * 0.2f;
            i_this->field_0x5e4 = fabsf(i_this->field_0x5c8.y - i_this->field_0x5b0.y) * 0.2f;
            i_this->field_0x5e8 = fabsf(i_this->field_0x5c8.z - i_this->field_0x5b0.z) * 0.2f;
            i_this->field_0x5f8 = 0.02f;

            master_ns[2]->current.pos.x = master_ns[2]->home.pos.x;
            master_ns[2]->current.pos.z = master_ns[2]->home.pos.z;
            master_ns[2]->current.pos.y = master_ns[2]->home.pos.y + 500.0f;
            master_ns[2]->speed.y = -120.0f;
            master_ns[2]->gravity = -7.0f;
        }

        if (i_this->field_0x5a0 == 290) {
            i_this->field_0x5b0.set(-15312.0f, 361.0f, 1425.0f);
            i_this->field_0x5a4.set(-15630.0f, 16.0f, -15.0f);
            i_this->field_0x5c8.set(-15312.0f, 361.0f, 1425.0f);
            i_this->field_0x5bc.set(-15630.0f, 16.0f, -15.0f);
            i_this->field_0x5f8 = 0.0f;
        }

        // TODO: gameInfo fake match to force reuse of pointer
        play = &g_dComIfG_gameInfo.play;
        play->getEvent().onHindFlag(15);

        if (i_this->field_0x59e == 5) {
            if (i_this->field_0x5a0 >= 310) {
                if (i_this->field_0x5a0 == 310) {
                    i_this->mMsgFlow.init(i_this, 127, 0, NULL);
                }
                i_this->mMsgFlow.doFlow(i_this, NULL, 0);
            }

            if (i_this->mMsgFlow.getNowMsgNo() == 0x13a5 ||
                i_this->mMsgFlow.getNowMsgNo() == 0x13e3)
            {
                i_this->field_0x59e = 6;
                i_this->field_0x5b0.set(-15605.0f, 144.0f, -1017.0f);
                i_this->field_0x5a4.set(-15487.0f, 69.0f, 491.0f);
                i_this->field_0x5c8.set(-15560.0f, 85.0f, 108.0f);
                i_this->field_0x5bc.set(-15778.0f, 534.0f, 1538.0f);
                i_this->field_0x5d4 = fabsf(i_this->field_0x5bc.x - i_this->field_0x5a4.x) * 0.9f;
                i_this->field_0x5d8 = fabsf(i_this->field_0x5bc.y - i_this->field_0x5a4.y) * 0.9f;
                i_this->field_0x5dc = fabsf(i_this->field_0x5bc.z - i_this->field_0x5a4.z) * 0.9f;
                i_this->field_0x5e0 = fabsf(i_this->field_0x5c8.x - i_this->field_0x5b0.x) * 0.9f;
                i_this->field_0x5e4 = fabsf(i_this->field_0x5c8.y - i_this->field_0x5b0.y) * 0.9f;
                i_this->field_0x5e8 = fabsf(i_this->field_0x5c8.z - i_this->field_0x5b0.z) * 0.9f;
                i_this->field_0x5f8 = 0.02f;
            }
        } else if (i_this->field_0x59e == 6 && i_this->mMsgFlow.doFlow(i_this, NULL, 0) != 0) {
            camera->mCamera.Reset(i_this->field_0x5b0, i_this->field_0x5a4, i_this->field_0x5f4, 0);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            play->getEvent().reset();
            daPy_getPlayerActorClass()->cancelOriginalDemo();
            fopAcM_delete(i_this);
            fpcM_Search(s_s1fight_sub, i_this);
            /* dSv_event_flag_c::F_0053 - Faron Woods - Saw night stalker appearance cutscene */
            dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[151]);
        }
        break;

    case 10:
        if (!i_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(i_this, 2, 0xffff, 0);
            i_this->eventInfo.onCondition(2);
            return;
        }

        camera->mCamera.Stop();
        i_this->field_0x59e = 11;
        i_this->field_0x5a0 = 0;
        i_this->field_0x5f4 = 55.0f;
        i_this->field_0x5a0 = 0;
        i_this->field_0x5ec = 300.0f;
        i_this->field_0x5f0 = 1000.0f;
        i_this->field_0x5f8 = 0.0f;
        camera->mCamera.SetTrimSize(3);
        daPy_getPlayerActorClass()->changeOriginalDemo();
        daPy_getPlayerActorClass()->changeDemoMode(23, 0, 0, 0);
        // [[fallthrough]]

    case 11:
        if (strcmp(dComIfGp_getStartStageName(), "F_SP104") == 0) {
            if (i_this->field_0x5a0 >= 40) {
                if (i_this->field_0x5a0 == 40) {
                    i_this->mMsgFlow.init(i_this, 0xbba, 0, NULL);
                }

                if (i_this->mMsgFlow.doFlow(i_this, NULL, 0)) {
                    i_this->field_0x59e = 12;
                    i_this->field_0x5a0 = 0;
                }
            }
        } else if (i_this->field_0x5a0 == 40) {
            i_this->field_0x59e = 12;
            i_this->field_0x5a0 = 0;
        }

        sp54 = i_this->home.pos;
        sp54.y = player->current.pos.y + 200.0f;

        if (fopAcM_gc_c::gndCheck(&sp54)) {
            sp54.y = fopAcM_gc_c::getGroundY();
        } else {
            sp54.y = player->current.pos.y;
        }
        player->setPlayerPosAndAngle(&sp54, i_this->home.angle.y, 0);

        i_this->field_0x5ec = 200.0f;
        i_this->field_0x5f0 = -500.0f;
        mDoMtx_stack_c::YrotS(i_this->home.angle.y);
        sp48.x = -100.0f;
        sp48.y = i_this->field_0x5ec;
        sp48.z = i_this->field_0x5f0;
        mDoMtx_stack_c::multVec(&sp48, &i_this->field_0x5a4);
        i_this->field_0x5a4 += player->current.pos;
        i_this->field_0x5b0 = player->current.pos;
        i_this->field_0x5b0.y += 100.0f;
        break;

    case 12:
        if (i_this->field_0x5a0 >= 29) {
            cLib_addCalc2(&i_this->field_0x5b0.y, i_this->current.pos.y, 0.2f, i_this->field_0x5f8);
            cLib_addCalc2(&i_this->field_0x5f8, 200.0f, 1.0f, 10.0f);

            if (i_this->field_0x5a0 == 69) {
                i_this->field_0x596 = 10;
                Z2GetAudioMgr()->seStart(Z2SE_DEMO_NS_WARPHOLE, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                         -1.0f, 0);
            }

            if (i_this->field_0x5a0 > 159) {
                i_this->field_0x59e = 13;
                i_this->field_0x5a0 = 0;
            }
        }
        break;

    case 13:
        cLib_addCalc2(&i_this->field_0x5f4, 70.0f, 0.05f, 0.08f);
        mDoMtx_stack_c::YrotS(i_this->home.angle.y);
        sp48.x = -100.0f;
        sp48.y = -1000.0f;
        sp48.z = -1500.0f;
        mDoMtx_stack_c::multVec(&sp48, &i_this->field_0x5a4);
        i_this->field_0x5a4 += i_this->current.pos;
        i_this->field_0x5b0 = i_this->current.pos;
        i_this->field_0x5b0.y += -300.0f;

        if (i_this->field_0x5a0 == 50 && i_this->field_0x64c != 0) {
            i_this->field_0x64c--;
            fpcM_Search(s_s1drop2_sub, i_this);
            if (i_this->field_0x64c != 0) {
                i_this->field_0x5a0 += -20;
            }
        }

        if (i_this->field_0x5a0 == 70) {
            i_this->field_0x596 = 20;
        }

        if (i_this->field_0x5a0 == 140) {
            fpcM_Search(s_s1start_sub, i_this);
            camera->mCamera.Reset(i_this->field_0x5b0, i_this->field_0x5a4, i_this->field_0x5f4, 0);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            daPy_getPlayerActorClass()->cancelOriginalDemo();
            fopAcM_delete(i_this);
        }
        break;

    case 20:
        if (!i_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(i_this, 2, 0xffff, 0);
            i_this->eventInfo.onCondition(2);
            return;
        }

        camera->mCamera.Stop();
        i_this->field_0x59e = 21;
        i_this->field_0x5a0 = 0;
        i_this->field_0x5f4 = 55.0f;
        i_this->field_0x5a0 = 0;
        camera->mCamera.SetTrimSize(3);
        i_this->field_0x5b0.set(34800.0f, 5700.0f, -26735.0f);
        i_this->field_0x5a4.set(37592.0f, 1256.0f, -24152.0f);
        // [[fallthrough]]

    case 21:
        if ((daPy_getPlayerActorClass()->checkHorseRide() == 0 && horseActor != NULL) &&
            horseActor->current.pos.z > -28000.0f)
        {
            horseActor->current.pos.set(34785.0f, -300.0f, -28500.0f);
            horseActor->old.pos.set(34785.0f, -300.0f, -28500.0f);
        }

        if (i_this->field_0x5a0 == 40) {
            i_this->field_0x596 = 10;
            Z2GetAudioMgr()->seStart(Z2SE_DEMO_NS_WARPHOLE, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                     0);
        }

        if (i_this->field_0x5a0 >= 180) {
            obj_ihasi_class* ihasiActor = (obj_ihasi_class*)fopAcM_SearchByName(PROC_OBJ_IHASI);
            sp54.set(34800.0f, i_this->field_0x5ec + -300.0f, -26735.0f);

            ihasiActor->mParticleKey =
                dComIfGp_particle_set(ihasiActor->mParticleKey, 0x8521, &sp54, 0, 0);
            if (ihasiActor->mpBtkAnm->getFrame() >= 140.0f) {
                cLib_addCalc2(&i_this->field_0x5ec, -1000.0f, 1.0f, 16.166668f);
            }

            if (i_this->field_0x5a0 == 180) {
                sp54 = i_this->field_0x5b0;
                cXyz scale(i_this->scale.x, i_this->scale.x, i_this->scale.x);
                dComIfGp_particle_set(0x850c, &sp54, NULL, &scale);
                sp54.set(34800.0f, -300.0f, -26735.0f);

                for (int i = 0; i < 20; i++) {
                    dComIfGp_particle_set(w_id[i], &sp54, NULL, NULL);
                }

                i_this->field_0x5ec = -30.0f;
                ihasiActor->mFlag2 = 1;
            }
        }

        if (i_this->field_0x5a0 >= 182) {
            cLib_addCalc2(&i_this->field_0x5b0.y, -300.0f, 0.5f, 800.0f);
        }

        if (i_this->field_0x5a0 == 190) {
            dComIfGp_getVibration().StartQuake(3, 1, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (i_this->field_0x5a0 >= 190 && i_this->field_0x5a0 < 222) {
            i_this->field_0x5fc = 10.0f;
        }

        if (i_this->field_0x5a0 == 222) {
            dComIfGp_getVibration().StopQuake(31);
            dComIfGp_getVibration().StartShock(8, 1, cXyz(0.0f, 1.0f, 0.0f));
            i_this->field_0x5fc = 25.0f;
        }

        if (i_this->field_0x5a0 >= 460) {
            i_this->field_0x59e = 22;
            i_this->field_0x5a0 = 0;
            i_this->field_0x5b0.set(34800.0f, 5700.0f, -26735.0f);
            i_this->field_0x5a4.set(37592.0f, 1256.0f, -24152.0f);
            sp54.set(34800.0f, 5700.0f, -26735.0f);
            cXyz sp84(i_this->scale.x, i_this->scale.x, i_this->scale.x);
            dComIfGp_particle_set(0x8522, &sp54, NULL, &sp84);
            dComIfGp_particle_set(0x8523, &sp54, NULL, &sp84);
            i_this->field_0x5f8 = 0.0f;
            ew_s1angy = 0;
        }
        break;

    case 22:
        if (i_this->field_0x5a0 >= 30) {
            cLib_addCalc2(&i_this->field_0x5f4, 30.0f, 0.05f, i_this->field_0x5f8 * 0.2f);
            cLib_addCalc2(&i_this->field_0x5f8, 1.0f, 1.0f, 0.03f);
        }

        if (i_this->field_0x5a0 == 140 && i_this->field_0x64c != 0) {
            i_this->field_0x64c--;
            fpcM_Search(s_s1drop2_sub, i_this);
            fpcM_Search(s_s1Yangset_sub, i_this);
            ew_s1angy += 0x5555;
            if (i_this->field_0x64c != 0) {
                i_this->field_0x5a0 += -20;
            }
        }

        if (i_this->field_0x5a0 == 150) {
            i_this->field_0x596 = 20;
        }

        if (i_this->field_0x5a0 >= 230) {
            fpcM_Search(s_s1start_sub, i_this);
            i_this->field_0x59e = 23;
            i_this->field_0x5a0 = 0;
            i_this->field_0x5b0.set(34397.0f, -180.0f, -31312.0f);
            i_this->field_0x5a4.set(34218.0f, -166.0f, -31724.0f);
            i_this->field_0x5f4 = 55.0f;
        }
        break;

    case 23:
        if (i_this->field_0x5a0 >= 80) {
            camera->mCamera.Reset(i_this->field_0x5b0, i_this->field_0x5a4, i_this->field_0x5f4, 0);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            daPy_getPlayerActorClass()->cancelOriginalDemo();
            if (horseActor != NULL) {
                horseActor->cancelOriginalDemo();
            }
            fopAcM_delete(i_this);
        }
        break;

    case 0x7:
    case 0x8:
    case 0x9:
    case 0xe:
    case 0xf:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x0:
        break;
    }

    if (i_this->field_0x59e != 0) {
        s16 sVar1 = i_this->field_0x5fc * cM_scos(i_this->field_0x594 * 0x3500) * 10.0f;
        cXyz sp90 = i_this->field_0x5b0;
        cXyz sp9c = i_this->field_0x5a4;
        sp90.y += i_this->field_0x5fc * cM_ssin(i_this->field_0x594 * 0x3200);
        sp9c.y += i_this->field_0x5fc * cM_ssin(i_this->field_0x594 * 0x3200);
        camera->mCamera.Set(sp90, sp9c, sVar1, i_this->field_0x5f4);
        cLib_addCalc0(&i_this->field_0x5fc, 1.0f, 2.0f);
        i_this->field_0x5a0++;

        if (i_this->field_0x5a0 > 10000) {
            i_this->field_0x5a0 = 10000;
        }
    }
}

static void mtx_set(e_warpappear_class* i_this) {
    mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z);
    mDoMtx_stack_c::scaleM(i_this->scale.x, i_this->scale.x, i_this->scale.x);
    i_this->mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

static int daE_Warpappear_Execute(e_warpappear_class* i_this) {
    i_this->field_0x594++;
    i_this->scale.x = 20.0f;
    for (int i = 0; i < 2; i++) {
        if (i_this->field_0x59a[i] != 0) {
            i_this->field_0x59a[i]--;
        }
    }

    action(i_this);
    demo_camera(i_this);
    mtx_set(i_this);
    i_this->mpBtk->play();
    i_this->mpBrks[i_this->field_0x584]->play();
    return 1;
}

static int daE_Warpappear_IsDelete(e_warpappear_class* i_this) {
    return 1;
}

static int daE_Warpappear_Delete(e_warpappear_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "Kytag04");
    return 1;
}

static int useHeapInit(fopAc_ac_c* actor) {
    e_warpappear_class* i_this = (e_warpappear_class*)actor;

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Kytag04", 5);
    JUT_ASSERT(1535, modelData != NULL);

    i_this->mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);
    if (i_this->mpModel == NULL) {
        return 0;
    }

    i_this->mpBtk = new mDoExt_btkAnm();
    if (i_this->mpBtk == NULL) {
        return 0;
    }
    J3DAnmTextureSRTKey* key = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Kytag04", 14);
    if (i_this->mpBtk->init(i_this->mpModel->getModelData(), key, 1, 2, 1.0f, 0, -1) == 0) {
        return 0;
    }

    i_this->mpBrks[0] = new mDoExt_brkAnm();
    if (i_this->mpBrks[0] == NULL) {
        return 0;
    }
    J3DAnmTevRegKey* pJVar8 = (J3DAnmTevRegKey*)dComIfG_getObjectRes("Kytag04", 10);
    if (i_this->mpBrks[0]->init(i_this->mpModel->getModelData(), pJVar8, 1, 0, 1.0f, 0, -1) == 0) {
        return 0;
    }
    i_this->mpBrks[0]->setPlaySpeed(0.0f);

    i_this->mpBrks[1] = new mDoExt_brkAnm();
    if (i_this->mpBrks[1] == NULL) {
        return 0;
    }
    pJVar8 = (J3DAnmTevRegKey*)dComIfG_getObjectRes("Kytag04", 11);
    if (i_this->mpBrks[1]->init(i_this->mpModel->getModelData(), pJVar8, 1, 0, 1.0f, 0, -1) == 0) {
        return 0;
    }

    return 1;
}

static int daE_Warpappear_Create(fopAc_ac_c* actor) {
    fopAcM_ct(actor, e_warpappear_class);
    e_warpappear_class* i_this = (e_warpappear_class*)actor;

    int phase = dComIfG_resLoad(&i_this->mPhase, "Kytag04");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("E_Warpappear PARAM %x\n", fopAcM_GetParam(i_this));
        OS_REPORT("E_Warpappear YP %d\n", i_this->home.pos.y);

        i_this->field_0x570 = fopAcM_GetParam(i_this);
        if (i_this->field_0x570 != 53) {
            if (i_this->field_0x570 == 0xff) {
                    /* dSv_event_flag_c::F_0053 - Faron Woods - Saw night stalker appearance cutscene */
                if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x97])) {
                    return cPhs_ERROR_e;
                }
            } else {
                i_this->field_0x59a[0] = 120;
            }

            if (dComIfGs_isSwitch(fopAcM_GetParam(i_this) >> 0x18, fopAcM_GetRoomNo(i_this))) {
                return cPhs_ERROR_e;
            }
        }

        OS_REPORT("E_Warpappear//////////////E_Warpappear SET 1 !!\n");
        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x2080)) {
            OS_REPORT("//////////////E_Warpappear SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////E_Warpappear SET 2 !!\n");
        mtx_set(i_this);
    }

    return phase;
}

static actor_method_class l_daE_Warpappear_Method = {
    (process_method_func)daE_Warpappear_Create,  (process_method_func)daE_Warpappear_Delete,
    (process_method_func)daE_Warpappear_Execute, (process_method_func)daE_Warpappear_IsDelete,
    (process_method_func)daE_Warpappear_Draw,
};

extern actor_process_profile_definition g_profile_E_WAP = {
    fpcLy_CURRENT_e,             // mLayerID
    7,                           // mListID
    fpcPi_CURRENT_e,             // mListPrio
    PROC_E_WAP,                  // mProcName
    &g_fpcLf_Method.base,        // sub_method
    sizeof(e_warpappear_class),  // mSize
    0,                           // mSizeOther
    0,                           // mParameters
    &g_fopAc_Method.base,        // sub_method
    233,                         // mPriority
    &l_daE_Warpappear_Method,    // sub_method
    0x00044000,                  // mStatus
    fopAc_ACTOR_e,               // mActorType
    fopAc_CULLBOX_CUSTOM_e,      // cullType
};
