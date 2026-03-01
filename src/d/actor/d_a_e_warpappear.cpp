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
#include <cstring>

static int daE_Warpappear_Draw(e_warpappear_class* i_this) {
    int sw = (fopAcM_GetParam(i_this) >> 8) & 0xFF;
    if (sw != 0xff) {
        if (!dComIfGs_isSwitch(sw, fopAcM_GetRoomNo(i_this))) {
            return 1;
        }
    }

    dComIfGd_setListDark();
    i_this->btk->entry(i_this->model->getModelData());
    i_this->anm_p[i_this->field_0x584]->entry(i_this->model->getModelData());
    mDoExt_modelUpdateDL(i_this->model);
    dComIfGd_setList();
    return 1;
}

static e_s1_class* master_ns[3];

static void* s_s1_sub(void* i_actor, void* i_data) {
    e_s1_class* ns = (e_s1_class*)i_actor;

    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_S1) {
        ns->mAction = 20;
        ns->mMode = 0;

        f32 x = -15336.0f;
        f32 z = -671.0f;
        x -= ns->home.pos.x;
        z -= ns->home.pos.z;
        if (SQUARE(x) + SQUARE(z) < SQUARE(200.0f)) {
            master_ns[0] = ns;
        }

        x = -16552.0f;
        z = -144.0f;
        x -= ns->home.pos.x;
        z -= ns->home.pos.z;
        if (SQUARE(x) + SQUARE(z) < SQUARE(200.0f)) {
            master_ns[1] = ns;
        }

        x = -14796.0f;
        z = 382.0f;
        x -= ns->home.pos.x;
        z -= ns->home.pos.z;
        if (SQUARE(x) + SQUARE(z) < SQUARE(200.0f)) {
            master_ns[2] = ns;
        }
    }

    return NULL;
}

static void* s_s1drop_sub(void* i_actor, void* i_data) {
    e_s1_class* ns = (e_s1_class*)i_actor;
    fopAc_ac_c* actor = (fopAc_ac_c*)i_data;
    if ((fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_S1) &&
        ns->mMode != 2)
    {
        ns->current.pos = actor->current.pos;
        ns->current.pos.y += 400.0f;
        ns->mMode = 2;
        ns->scale.x = 2.0f;
        return i_actor;
    }

    return NULL;
}

static void* s_s1riv_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_S1) {
        e_s1_class* ns = (e_s1_class*)i_actor;
        ns->current.pos = master_ns[0]->home.pos;
        ns->old = ns->current;
        ns->speed.y = 0.0f;
        ns->mMode = 10;

        if (ns != master_ns[0]) {
            ns->current.pos.y = master_ns[0]->home.pos.y + 50000.0f;
            ns->gravity = 0.0f;
        } else {
            ns->current.pos.y = master_ns[0]->home.pos.y + 3000.0f;
        }

        ns->scale.x = 1.0f;
    }

    return NULL;
}

static void* s_s1fight_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_S1) {
        e_s1_class* ns = (e_s1_class*)i_actor;
        ns->mMode = 20;
    }

    return NULL;
}

static int entry_no;

static void* s_s1entry_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_S1) {
        e_s1_class* ns = (e_s1_class*)i_actor;
        e_warpappear_class* i_this = (e_warpappear_class*)i_data;

        cXyz sp28 = ns->current.pos - i_this->current.pos;
        sp28.y = 0.0f;

        if (ns->mAction != 20) {
            ns->mAction = 20;
            ns->mMode = 0;
            ns->mEntryNo = entry_no;
            entry_no++;

            i_this->target_info_count++;
        }
    }

    return NULL;
}

static void* s_s1drop2_sub(void* i_actor, void* i_data) {
    e_s1_class* ns = (e_s1_class*)i_actor;
    e_warpappear_class* i_this = (e_warpappear_class*)i_data;

    if ((fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_S1) &&
        i_this->target_info_count == ns->mEntryNo)
    {
        ns->current.pos = i_this->current.pos;
        ns->current.pos.y += 400.0f;

        ns->mMode = 2;

        ns->scale.x = 2.0f;

        return i_actor;
    }

    return NULL;
}

static s16 ew_s1angy;

static void* s_s1Yangset_sub(void* i_actor, void* i_data) {
    e_s1_class* ns = (e_s1_class*)i_actor;
    e_warpappear_class* i_this = (e_warpappear_class*)i_data;

    if ((fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_S1) &&
        i_this->target_info_count == ns->mEntryNo)
    {
        ns->shape_angle.y = ns->current.angle.y = ew_s1angy;

        return i_actor;
    }

    return NULL;
}

static void* s_s1start_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_S1) {
        e_s1_class* ns = (e_s1_class*)i_actor;
        fopAc_ac_c* actor = (fopAc_ac_c*)i_data;

        cXyz sp28 = ns->current.pos - actor->current.pos;
        sp28.y = 0.0f;

        if (sp28.abs() < 2000.0f) {
            ns->current = ns->home;
            ns->old = ns->home;
            ns->shape_angle = ns->current.angle;
            ns->mMode = 20;
            ns->scale.x = 1.0f;
        }
    }

    return NULL;
}

static void action(e_warpappear_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    fopAc_ac_c* pla = dComIfGp_getPlayer(0);
    f32 dx = -15200.0f - pla->current.pos.x;
    f32 dz = -35.0f - pla->current.pos.z;
    dx = JMAFastSqrt(SQUARE(dx) + SQUARE(dz));
    s8 bVar1 = false;

    switch (i_this->action) {
    case 0:
        if (i_this->arg0 == 0x35) {
            if (i_this->target_info_count == 0) {
                entry_no = 0;
                fpcM_Search(s_s1entry_sub, i_this);
            }

                /* dSv_event_flag_c::M_031 - Goron Mines - Goron Mines clear */
            if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x40])
                && dComIfGs_isSwitch(18, fopAcM_GetRoomNo(actor)))
            {
                i_this->action = 1;
                i_this->demo_mode = 20;
            }

        } else if (i_this->arg0 == 0xFF) {
            fpcM_Search(s_s1_sub, i_this);
            if (dx < 950.0f) {
                i_this->demo_mode = 1;
                i_this->action = 1;
            }

        } else {
            entry_no = 0;
            fpcM_Search(s_s1entry_sub, i_this);
            if (i_this->target_info_count != 0) {
                u32 sw = fopAcM_GetParamBit(actor, 8, 8);
                if ((s32)sw != 0xff && !dComIfGs_isSwitch(sw, fopAcM_GetRoomNo(actor))) {
                    return;
                }

                if (i_this->timer[0] == 0 && fopAcM_searchPlayerDistanceXZ(actor) < 1000.0f)
                {
                    i_this->demo_mode = 10;
                    i_this->action = 1;
                }
            }
        }
        break;

    case 1:
        break;

    case 10:
        i_this->anm_p[0]->setPlaySpeed(1.0f);
        bVar1 = TRUE;
        break;

    case 20:
        i_this->field_0x584 = 1;
        break;
    }

    if (bVar1 != 0) {
        cXyz sc(i_this->scale.x, i_this->scale.x, i_this->scale.x);
        if (i_this->field_0x598 == 0) {
            i_this->field_0x598++;
            dComIfGp_particle_set(dPa_RM(ID_ZI_S_PORTAL_APP_A01), &actor->current.pos, NULL, &sc);
        }

        i_this->portal_app_b =
            dComIfGp_particle_set(i_this->portal_app_b, dPa_RM(ID_ZI_S_PORTAL_APP_B), &actor->current.pos, NULL, &sc);
        i_this->portal_app_c =
            dComIfGp_particle_set(i_this->portal_app_c, dPa_RM(ID_ZI_S_PORTAL_APP_C), &actor->current.pos, NULL, &sc);
    }
}

static void demo_camera(e_warpappear_class* i_this) {
    static u16 w_id[20] = {
    dPa_RM(ID_ZI_S_BAJYOHASHI_WARPDEMO_B001),
    dPa_RM(ID_ZI_S_BAJYOHASHI_WARPDEMO_B002),
    dPa_RM(ID_ZI_S_BAJYOHASHI_WARPDEMO_B003),
    dPa_RM(ID_ZI_S_BAJYOHASHI_WARPDEMO_B004),
    dPa_RM(ID_ZI_S_BAJYOHASHI_WARPDEMO_B005),
    dPa_RM(ID_ZI_S_BAJYOHASHI_WARPDEMO_B006),
    dPa_RM(ID_ZI_S_BAJYOHASHI_WARPDEMO_B007),
    dPa_RM(ID_ZI_S_BAJYOHASHI_WARPDEMO_B008),
    dPa_RM(ID_ZI_S_BAJYOHASHI_WARPDEMO_C001),
    dPa_RM(ID_ZI_S_BAJYOHASHI_WARPDEMO_C002),
    dPa_RM(ID_ZI_S_BAJYOHASHI_WARPDEMO_C003),
    dPa_RM(ID_ZI_S_BAJYOHASHI_WARPDEMO_C004),
    dPa_RM(ID_ZI_S_BAJYOHASHI_WARPDEMO_C005),
    dPa_RM(ID_ZI_S_BAJYOHASHI_WARPDEMO_C006),
    dPa_RM(ID_ZI_S_BAJYOHASHI_WARPDEMO_C007),
    dPa_RM(ID_ZI_S_BAJYOHASHI_WARPDEMO_C008),
    dPa_RM(ID_ZI_S_BAJYOHASHI_WARPDEMO_C009),
    dPa_RM(ID_ZI_S_BAJYOHASHI_WARPDEMO_C010),
    dPa_RM(ID_ZI_S_BAJYOHASHI_WARPDEMO_C011),
    dPa_RM(ID_ZI_S_BAJYOHASHI_WARPDEMO_C012),
};

    daPy_py_c* pla = (daPy_py_c*)dComIfGp_getPlayer(0);
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* camera0 = dComIfGp_getCamera(0);
    daHorse_c* horse = (daHorse_c*)dComIfGp_getHorseActor();

    cXyz sp48, sp54;
    cXyz temp1, temp2;
    switch (i_this->demo_mode) {
    case 1:
        if (!i_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(i_this, 2, 0xffff, 0);
            i_this->eventInfo.onCondition(2);
            return;
        }

        camera->mCamera.Stop();
        i_this->demo_mode = 2;
        i_this->demo_timer = 0;
        i_this->field_0x5f4 = 55.0f;
        i_this->demo_timer = 0;
        i_this->field_0x5ec = 300.0f;
        i_this->field_0x5f0 = 1000.0f;
        i_this->field_0x5f8 = 0.0f;

        camera->mCamera.SetTrimSize(3);
        daPy_getPlayerActorClass()->changeOriginalDemo();
        daPy_getPlayerActorClass()->changeDemoMode(23, 0, 0, 0);
        // [[fallthrough]]

    case 2:
        sp54.set(-15600.0f, 0.0f, -200.0f);
        pla->setPlayerPosAndAngle(&sp54, 0x4ef8, 0);
        i_this->current.pos.x = sp54.x;
        i_this->current.pos.z = sp54.z;
        i_this->field_0x5ec = 200.0f;
        i_this->field_0x5f0 = -500.0f;

        mDoMtx_stack_c::YrotS(pla->shape_angle.y);
        sp48.x = -100.0f;
        sp48.y = i_this->field_0x5ec;
        sp48.z = i_this->field_0x5f0;

        mDoMtx_stack_c::multVec(&sp48, &i_this->field_0x5a4);
        i_this->field_0x5a4 += pla->current.pos;
        i_this->field_0x5b0 = pla->current.pos;
        i_this->field_0x5b0.y += 100.0f;

        if (i_this->demo_timer > 20) {
            i_this->demo_mode = 3;
            i_this->demo_timer = 0;
        }
        break;

    case 3:
        cLib_addCalc2(&i_this->field_0x5b0.y, i_this->current.pos.y, 0.1f, i_this->field_0x5f8);
        cLib_addCalc2(&i_this->field_0x5f8, 200.0f, 1.0f, 10.0f);
        if (i_this->demo_timer == 40) {
            i_this->action = 10;
            Z2GetAudioMgr()->seStart(Z2SE_DEMO_NS_WARPHOLE, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                     0);
        }

        if (i_this->demo_timer > 150) {
            i_this->demo_mode = 4;
            i_this->demo_timer = 0;
        }
        break;

    case 4:
        sp54.set(-15600.0f, 0.0f, 200.0f);
        pla->setPlayerPosAndAngle(&sp54, 0x6e38, 0);
        i_this->current.pos.x = sp54.x;
        i_this->current.pos.z = sp54.z;
        cLib_addCalc2(&i_this->field_0x5f4, 70.0f, 0.05f, 0.08f);
        mDoMtx_stack_c::YrotS(pla->shape_angle.y);
        sp48.x = -100.0f;
        sp48.y = -1000.0f;
        sp48.z = -1500.0f;
        mDoMtx_stack_c::multVec(&sp48, &i_this->field_0x5a4);
        i_this->field_0x5a4 += i_this->current.pos;
        i_this->field_0x5b0 = i_this->current.pos;
        i_this->field_0x5b0.y += -300.0f;

        if (i_this->demo_timer == 50 || i_this->demo_timer == 70 || i_this->demo_timer == 90) {
            fpcM_Search(s_s1drop_sub, i_this);
        }

        if (i_this->demo_timer == 110) {
            i_this->action = 20;
        }

        if (i_this->demo_timer == 200) {
            i_this->demo_mode = 5;
            i_this->demo_timer = 0;
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
        pla->setPlayerPosAndAngle(&sp54, 0x6e38, 0);
        if (i_this->demo_timer == 38) {
            dComIfGp_getVibration().StartShock(5, 1, cXyz(0.0f, 1.0f, 0.0f));
            i_this->field_0x5fc = 30.0f;
        }

        if (i_this->demo_timer <= 80) {
            cLib_addCalc2(&i_this->field_0x5b0.y, 13.0f, 0.4f, 100.0f);
        } else {
            f32 fVar1 = 0.1f;
            if (i_this->demo_mode == 6) {
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

        if (i_this->demo_timer == 150) {
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

        if (i_this->demo_timer == 210) {
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

        if (i_this->demo_timer == 290) {
            i_this->field_0x5b0.set(-15312.0f, 361.0f, 1425.0f);
            i_this->field_0x5a4.set(-15630.0f, 16.0f, -15.0f);
            i_this->field_0x5c8.set(-15312.0f, 361.0f, 1425.0f);
            i_this->field_0x5bc.set(-15630.0f, 16.0f, -15.0f);
            i_this->field_0x5f8 = 0.0f;
        }

        dComIfGp_event_onHindFlag(15);

        if (i_this->demo_mode == 5) {
            if (i_this->demo_timer >= 310) {
                if (i_this->demo_timer == 310) {
                    i_this->MsgFlow.init(i_this, 127, 0, NULL);
                }
                i_this->MsgFlow.doFlow(i_this, NULL, 0);
            }

            if (i_this->MsgFlow.getNowMsgNo() == 0x13a5 ||
                i_this->MsgFlow.getNowMsgNo() == 0x13e3)
            {
                i_this->demo_mode = 6;
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
        } else if (i_this->demo_mode == 6 && i_this->MsgFlow.doFlow(i_this, NULL, 0) != 0) {
            camera->mCamera.Reset(i_this->field_0x5b0, i_this->field_0x5a4, i_this->field_0x5f4, 0);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
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
        i_this->demo_mode = 11;
        i_this->demo_timer = 0;
        i_this->field_0x5f4 = 55.0f;
        i_this->demo_timer = 0;
        i_this->field_0x5ec = 300.0f;
        i_this->field_0x5f0 = 1000.0f;
        i_this->field_0x5f8 = 0.0f;
        camera->mCamera.SetTrimSize(3);
        daPy_getPlayerActorClass()->changeOriginalDemo();
        daPy_getPlayerActorClass()->changeDemoMode(23, 0, 0, 0);
        // [[fallthrough]]

    case 11:
        if (strcmp(dComIfGp_getStartStageName(), "F_SP104") == 0) {
            if (i_this->demo_timer >= 40) {
                if (i_this->demo_timer == 40) {
                    i_this->MsgFlow.init(i_this, 0xbba, 0, NULL);
                }

                if (i_this->MsgFlow.doFlow(i_this, NULL, 0)) {
                    i_this->demo_mode = 12;
                    i_this->demo_timer = 0;
                }
            }
        } else if (i_this->demo_timer == 40) {
            i_this->demo_mode = 12;
            i_this->demo_timer = 0;
        }

        sp54 = i_this->home.pos;
        sp54.y = pla->current.pos.y + 200.0f;

        if (fopAcM_gc_c::gndCheck(&sp54)) {
            sp54.y = fopAcM_gc_c::getGroundY();
        } else {
            sp54.y = pla->current.pos.y;
        }
        pla->setPlayerPosAndAngle(&sp54, i_this->home.angle.y, 0);

        i_this->field_0x5ec = 200.0f;
        i_this->field_0x5f0 = -500.0f;
        mDoMtx_stack_c::YrotS(i_this->home.angle.y);
        sp48.x = -100.0f;
        sp48.y = i_this->field_0x5ec;
        sp48.z = i_this->field_0x5f0;
        mDoMtx_stack_c::multVec(&sp48, &i_this->field_0x5a4);
        i_this->field_0x5a4 += pla->current.pos;
        i_this->field_0x5b0 = pla->current.pos;
        i_this->field_0x5b0.y += 100.0f;
        break;

    case 12:
        if (i_this->demo_timer >= 29) {
            cLib_addCalc2(&i_this->field_0x5b0.y, i_this->current.pos.y, 0.2f, i_this->field_0x5f8);
            cLib_addCalc2(&i_this->field_0x5f8, 200.0f, 1.0f, 10.0f);

            if (i_this->demo_timer == 69) {
                i_this->action = 10;
                Z2GetAudioMgr()->seStart(Z2SE_DEMO_NS_WARPHOLE, NULL, 0, 0, 1.0f, 1.0f, -1.0f,
                                         -1.0f, 0);
            }

            if (i_this->demo_timer > 159) {
                i_this->demo_mode = 13;
                i_this->demo_timer = 0;
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

        if (i_this->demo_timer == 50 && i_this->target_info_count != 0) {
            i_this->target_info_count--;
            fpcM_Search(s_s1drop2_sub, i_this);
            if (i_this->target_info_count != 0) {
                i_this->demo_timer += -20;
            }
        }

        if (i_this->demo_timer == 70) {
            i_this->action = 20;
        }

        if (i_this->demo_timer == 140) {
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
        i_this->demo_mode = 21;
        i_this->demo_timer = 0;
        i_this->field_0x5f4 = 55.0f;
        i_this->demo_timer = 0;
        camera->mCamera.SetTrimSize(3);
        i_this->field_0x5b0.set(34800.0f, 5700.0f, -26735.0f);
        i_this->field_0x5a4.set(37592.0f, 1256.0f, -24152.0f);
        // [[fallthrough]]

    case 21:
        if ((daPy_getPlayerActorClass()->checkHorseRide() == 0 && horse != NULL) &&
            horse->current.pos.z > -28000.0f)
        {
            horse->current.pos.set(34785.0f, -300.0f, -28500.0f);
            horse->old.pos.set(34785.0f, -300.0f, -28500.0f);
        }

        if (i_this->demo_timer == 40) {
            i_this->action = 10;
            Z2GetAudioMgr()->seStart(Z2SE_DEMO_NS_WARPHOLE, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                                     0);
        }

        if (i_this->demo_timer >= 180) {
            obj_ihasi_class* ihasi = (obj_ihasi_class*)fopAcM_SearchByName(PROC_OBJ_IHASI);
            sp54.set(34800.0f, i_this->field_0x5ec + -300.0f, -26735.0f);

            ihasi->mParticleKey =
                dComIfGp_particle_set(ihasi->mParticleKey, dPa_RM(ID_ZI_S_BAJYOHASHI_WARPDEMO_D001), &sp54, 0, 0);
            if (ihasi->mpBtkAnm->getFrame() >= 140.0f) {
                cLib_addCalc2(&i_this->field_0x5ec, -1000.0f, 1.0f, 16.166668f);
            }

            if (i_this->demo_timer == 180) {
                sp54 = i_this->field_0x5b0;
                cXyz sc(i_this->scale.x, i_this->scale.x, i_this->scale.x);
                dComIfGp_particle_set(dPa_RM(ID_ZI_S_BAJYOHASHI_WARPDEMO_A001), &sp54, NULL, &sc);
                sp54.set(34800.0f, -300.0f, -26735.0f);

                for (int i = 0; i < 20; i++) {
                    dComIfGp_particle_set(w_id[i], &sp54, NULL, NULL);
                }

                i_this->field_0x5ec = -30.0f;
                ihasi->mFlag2 = 1;
            }
        }

        if (i_this->demo_timer >= 182) {
            cLib_addCalc2(&i_this->field_0x5b0.y, -300.0f, 0.5f, 800.0f);
        }

        if (i_this->demo_timer == 190) {
            dComIfGp_getVibration().StartQuake(3, 1, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (i_this->demo_timer >= 190 && i_this->demo_timer < 222) {
            i_this->field_0x5fc = 10.0f;
        }

        if (i_this->demo_timer == 222) {
            dComIfGp_getVibration().StopQuake(31);
            dComIfGp_getVibration().StartShock(8, 1, cXyz(0.0f, 1.0f, 0.0f));
            i_this->field_0x5fc = 25.0f;
        }

        if (i_this->demo_timer >= 460) {
            i_this->demo_mode = 22;
            i_this->demo_timer = 0;
            i_this->field_0x5b0.set(34800.0f, 5700.0f, -26735.0f);
            i_this->field_0x5a4.set(37592.0f, 1256.0f, -24152.0f);
            sp54.set(34800.0f, 5700.0f, -26735.0f);
            cXyz sc(i_this->scale.x, i_this->scale.x, i_this->scale.x);
            dComIfGp_particle_set(dPa_RM(ID_ZI_S_BAJYOHASHI_WARPDEMO_E001), &sp54, NULL, &sc);
            dComIfGp_particle_set(dPa_RM(ID_ZI_S_BAJYOHASHI_WARPDEMO_E002), &sp54, NULL, &sc);
            i_this->field_0x5f8 = 0.0f;
            ew_s1angy = 0;
        }
        break;

    case 22:
        if (i_this->demo_timer >= 30) {
            cLib_addCalc2(&i_this->field_0x5f4, 30.0f, 0.05f, i_this->field_0x5f8 * 0.2f);
            cLib_addCalc2(&i_this->field_0x5f8, 1.0f, 1.0f, 0.03f);
        }

        if (i_this->demo_timer == 140 && i_this->target_info_count != 0) {
            i_this->target_info_count--;
            fpcM_Search(s_s1drop2_sub, i_this);
            fpcM_Search(s_s1Yangset_sub, i_this);
            ew_s1angy += 0x5555;
            if (i_this->target_info_count != 0) {
                i_this->demo_timer += -20;
            }
        }

        if (i_this->demo_timer == 150) {
            i_this->action = 20;
        }

        if (i_this->demo_timer >= 230) {
            fpcM_Search(s_s1start_sub, i_this);
            i_this->demo_mode = 23;
            i_this->demo_timer = 0;
            i_this->field_0x5b0.set(34397.0f, -180.0f, -31312.0f);
            i_this->field_0x5a4.set(34218.0f, -166.0f, -31724.0f);
            i_this->field_0x5f4 = 55.0f;
        }
        break;

    case 23:
        if (i_this->demo_timer >= 80) {
            camera->mCamera.Reset(i_this->field_0x5b0, i_this->field_0x5a4, i_this->field_0x5f4, 0);
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            dComIfGp_event_reset();
            daPy_getPlayerActorClass()->cancelOriginalDemo();
            if (horse != NULL) {
                horse->cancelOriginalDemo();
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

    if (i_this->demo_mode != 0) {
        s16 zoom = i_this->field_0x5fc * cM_scos(i_this->counter * 0x3500) * 10.0f;
        cXyz center = i_this->field_0x5b0;
        cXyz eye = i_this->field_0x5a4;
        center.y += i_this->field_0x5fc * cM_ssin(i_this->counter * 0x3200);
        eye.y += i_this->field_0x5fc * cM_ssin(i_this->counter * 0x3200);
        camera->mCamera.Set(center, eye, zoom, i_this->field_0x5f4);
        cLib_addCalc0(&i_this->field_0x5fc, 1.0f, 2.0f);
        i_this->demo_timer++;

        if (i_this->demo_timer > 10000) {
            i_this->demo_timer = 10000;
        }
    }
}

static void mtx_set(e_warpappear_class* i_this) {
    mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z);
    mDoMtx_stack_c::scaleM(i_this->scale.x, i_this->scale.x, i_this->scale.x);
    i_this->model->setBaseTRMtx(mDoMtx_stack_c::get());
}

static int daE_Warpappear_Execute(e_warpappear_class* i_this) {
    i_this->counter++;
    i_this->scale.x = 20.0f;
    for (int i = 0; i < 2; i++) {
        if (i_this->timer[i] != 0) {
            i_this->timer[i]--;
        }
    }

    action(i_this);
    demo_camera(i_this);
    mtx_set(i_this);
    i_this->btk->play();
    i_this->anm_p[i_this->field_0x584]->play();
    return 1;
}

static int daE_Warpappear_IsDelete(e_warpappear_class* i_this) {
    return 1;
}

static int daE_Warpappear_Delete(e_warpappear_class* i_this) {
    dComIfG_resDelete(&i_this->phase, "Kytag04");
    return 1;
}

static int useHeapInit(fopAc_ac_c* actor) {
    e_warpappear_class* i_this = (e_warpappear_class*)actor;

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Kytag04", 5);
    JUT_ASSERT(1535, modelData != NULL);

    i_this->model = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000284);
    if (i_this->model == NULL) {
        return 0;
    }

    i_this->btk = new mDoExt_btkAnm();
    if (i_this->btk == NULL) {
        return 0;
    }
    J3DAnmTextureSRTKey* key = (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("Kytag04", 14);
    if (i_this->btk->init(i_this->model->getModelData(), key, 1, 2, 1.0f, 0, -1) == 0) {
        return 0;
    }

    i_this->anm_p[0] = new mDoExt_brkAnm();
    if (i_this->anm_p[0] == NULL) {
        return 0;
    }
    J3DAnmTevRegKey* brk = (J3DAnmTevRegKey*)dComIfG_getObjectRes("Kytag04", 10);
    if (i_this->anm_p[0]->init(i_this->model->getModelData(), brk, 1, 0, 1.0f, 0, -1) == 0) {
        return 0;
    }
    i_this->anm_p[0]->setPlaySpeed(0.0f);

    i_this->anm_p[1] = new mDoExt_brkAnm();
    if (i_this->anm_p[1] == NULL) {
        return 0;
    }
    brk = (J3DAnmTevRegKey*)dComIfG_getObjectRes("Kytag04", 11);
    if (i_this->anm_p[1]->init(i_this->model->getModelData(), brk, 1, 0, 1.0f, 0, -1) == 0) {
        return 0;
    }

    return 1;
}

static int daE_Warpappear_Create(fopAc_ac_c* actor) {
    fopAcM_ct(actor, e_warpappear_class);
    e_warpappear_class* i_this = (e_warpappear_class*)actor;

    int phase_state = dComIfG_resLoad(&i_this->phase, "Kytag04");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("E_Warpappear PARAM %x\n", fopAcM_GetParam(i_this));
        OS_REPORT("E_Warpappear YP %d\n", i_this->home.pos.y);

        i_this->arg0 = fopAcM_GetParam(i_this);
        if (i_this->arg0 != 0x35) {
            if (i_this->arg0 == 0xFF) {
                    /* dSv_event_flag_c::F_0053 - Faron Woods - Saw night stalker appearance cutscene */
                if (dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[0x97])) {
                    return cPhs_ERROR_e;
                }
            } else {
                i_this->timer[0] = 120;
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

    return phase_state;
}

static actor_method_class l_daE_Warpappear_Method = {
    (process_method_func)daE_Warpappear_Create,  (process_method_func)daE_Warpappear_Delete,
    (process_method_func)daE_Warpappear_Execute, (process_method_func)daE_Warpappear_IsDelete,
    (process_method_func)daE_Warpappear_Draw,
};

actor_process_profile_definition g_profile_E_WAP = {
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
