/**
 * @file d_a_obj_lp.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lp.h"
#include "d/actor/d_a_canoe.h"
#include "d/actor/d_a_mg_rod.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"
#include "f_op/f_op_camera_mng.h"
#include "Z2AudioLib/Z2Instances.h"

static int daObj_Lp_Draw(obj_lp_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mActor;
    wd_ss* mWdSs = i_this->mWdSs;
    int roomNo = fopAcM_GetRoomNo(a_this);
    static _GXColor l_color = {
        0x14,
        0x0A,
        0x0A,
        0xFF,
    };

    J3DGXColorS10 color;

    for (int i = 0; i < i_this->field_0xad98; i++, mWdSs++) {
        if (mWdSs->field_0x4e && !mWdSs->field_0x4f) {
            if (g_env_light.fishing_hole_season == 1) {
                color.r = 6;
                color.g = 6;
                color.b = 4;
                color.a = 0xff;
                mWdSs->mpModel->getModelData()->getMaterialNodePointer(0)->setTevColor(0, &color);
            }

            dComIfGp_entrySimpleModel(mWdSs->mpModel, roomNo);
        }
    }

    if (i_this->field_0xadb1 && !i_this->field_0xadb2) {
        i_this->field_0xad7c.update(3, l_color, &a_this->tevStr);
        dComIfGd_set3DlineMat(&i_this->field_0xad7c);
    }

    return 1;
}

static int tandem;

static int demo_f;

static int target_info[10];

static int target_info_count;

static void* s_ks_sub(void* param_1, void* param_2) {
    if (fopAcM_IsActor(param_1) && fopAcM_GetName(param_1) == 0x60) {
        if (target_info_count < 10) {
            target_info[target_info_count] = (int)param_1;
            target_info_count++;
        }
        return param_1;
    }

    return NULL;
}

static int hit_check(obj_lp_class* i_this, wd_ss* WdSs) {
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz sp6c, sp60;
    f32 fVar1, fVar2;
    int rv = 0;

    fVar1 = 50.0f;
    for (int i = 0; i < target_info_count; i++) {
        sp6c = WdSs->field_0x10 - *(cXyz *)(target_info[i] + 0x4d0);
        if (sp6c.y >= -3.0f) {
            f32 dist = JMAFastSqrt(sp6c.x * sp6c.x + sp6c.z * sp6c.z);
            if (dist <= fVar1 * WdSs->field_0x3c) {
                fVar2 = fVar1 * WdSs->field_0x3c - dist;
                cLib_addCalc2(&WdSs->field_0x28.x, fVar2, 0.1f, fVar2 * 0.5f);
                cLib_addCalcAngleS2(&WdSs->field_0x34, cM_atan2s(sp6c.x, sp6c.z), 0x20, 0x400);
                cLib_addCalcAngleS2(&WdSs->field_0x36, -4000, 0x20, 0x400);
                rv = 1;
            }
        }
        fVar1 = 70.0f;
    }

    if (daPy_getLinkPlayerActorClass()->checkCanoeRide()) {
        f32 fVar8 = 0.0f;
        if (demo_f != 0) {
            PSMTXCopy(player->getRightItemMatrix(), mDoMtx_stack_c::get());
            mDoMtx_stack_c::multVecZero(&sp60);
            sp60.x -= WdSs->field_0x10.x;
            sp60.z -= WdSs->field_0x10.z;
            if ((sp60.x * sp60.x + sp60.z * sp60.z) < 2500.0f) {
                fVar8 = 30.0f;
            }
        }

        sp6c = WdSs->field_0x10 - player->current.pos;
        cMtx_YrotS(*calc_mtx, -player->shape_angle.y);
        MtxTrans(0.0f, 0.0f, 0.0f, 1);
        MtxPosition(&sp6c, &sp60);

        if (tandem) {
            fVar1 = -275.0f;
            fVar2 = 150.0f;
        } else {
            fVar1 = -125.0f;
            fVar2 = 300.0f;
        }

        if (fabsf(sp60.x) < fVar8 + 70.0f && sp60.z > fVar1 && sp60.z < fVar2) {
            fVar8 = fVar8 + 70.0f - fabsf(sp60.x);
            if (sp60.z > fVar1 && sp60.z < fVar1 + 75.0f) {
                sp60.z -= fVar1;
                fVar8 *= fabsf(sp60.z) * 0.01333f;
            }

            cLib_addCalc2(&WdSs->field_0x10.y, WdSs->field_0x4.y + fVar8 * -0.5f, 0.5f, 3.0f);
            cLib_addCalc2(&WdSs->field_0x28.x, fVar8, 0.1f, fVar8 * 0.5f);
            cLib_addCalcAngleS2(&WdSs->field_0x34, cM_atan2s(sp6c.x, sp6c.z), 0x20, 0x400);
            cLib_addCalcAngleS2(&WdSs->field_0x36, 0xfffff060, 0x20, 0x400);
            rv = 1;
        }
    }

    return rv;
}

static void hasu_action(obj_lp_class* i_this, wd_ss* WdSs) {
    if (!hit_check(i_this, WdSs)) {
        cLib_addCalc0(&WdSs->field_0x28.x, 0.1f, 0.2f);
        cLib_addCalcAngleS2(&WdSs->field_0x34, 0, 0x30, 100);
        cLib_addCalcAngleS2(&WdSs->field_0x36, 0, 0x20, 200);
        cLib_addCalc2(&WdSs->field_0x10.x, WdSs->field_0x4.x, 1.0f, 0.2f);
        cLib_addCalc2(&WdSs->field_0x10.z, WdSs->field_0x4.z, 1.0f, 0.2f);
        cLib_addCalc2(&WdSs->field_0x10.y, WdSs->field_0x4.y, 0.05f, 1.0f);
    }

    cXyz sp2c = i_this->field_0xad9c - WdSs->field_0x10;
    if (fabsf(sp2c.abs() - i_this->field_0xadac) < 30.0f && WdSs->field_0x28.y < 2.0f) {
        WdSs->field_0x28.z = i_this->field_0xada8 * 0.04f;
        WdSs->field_0x40 = 0;
        WdSs->field_0x42 = cM_atan2s(sp2c.x, sp2c.z);
    }

    WdSs->field_0x28.y = WdSs->field_0x28.z - WdSs->field_0x28.z * cM_scos(WdSs->field_0x40);
    WdSs->field_0x44 = WdSs->field_0x28.z * cM_ssin(WdSs->field_0x40) * -200.0f;
    WdSs->field_0x40 += 0x9c4;
    WdSs->field_0x28.z *= 0.985f;

    mDoMtx_stack_c::transS(WdSs->field_0x10.x, WdSs->field_0x10.y + WdSs->field_0x28.y, WdSs->field_0x10.z);
    mDoMtx_stack_c::YrotM(WdSs->field_0x42);
    mDoMtx_stack_c::XrotM(WdSs->field_0x44);
    mDoMtx_stack_c::YrotM(WdSs->field_0x34 - WdSs->field_0x42);
    mDoMtx_stack_c::transM(0.0f, 0.0f, WdSs->field_0x28.x);
    mDoMtx_stack_c::XrotM(WdSs->field_0x36);
    mDoMtx_stack_c::YrotM(WdSs->field_0x38);
    mDoMtx_stack_c::scaleM(WdSs->field_0x3c, WdSs->field_0x3c, WdSs->field_0x3c);
    WdSs->mpModel->setBaseTRMtx(mDoMtx_stack_c::get());

    if (i_this->field_0xadb1 && !i_this->field_0xadb2) {
        cXyz sp50;
        mDoMtx_stack_c::multVecZero(&sp50);

        cXyz* mPos = i_this->field_0xad7c.getPos(WdSs->field_0x50);

        for (int i = 0; i < 3; i++, mPos++) {
            if (i == 0) {
                mPos->x = sp50.x;
                mPos->y = sp50.y;
                mPos->z = sp50.z;
            } else if (i == 2) {
                mPos->x = WdSs->field_0x1c.x;
                mPos->y = WdSs->field_0x1c.y;
                mPos->z = WdSs->field_0x1c.z;
            } else {
                mPos->x = WdSs->field_0x1c.x + (sp50.x - WdSs->field_0x1c.x) * 0.3f;
                mPos->z = WdSs->field_0x1c.z + (sp50.z - WdSs->field_0x1c.z) * 0.3f;
                mPos->y = WdSs->field_0x1c.y + (sp50.y - WdSs->field_0x1c.y) * 0.5f;
            }
        }
    }
}

static int set_out_check(obj_lp_class* i_this, cXyz* param_2) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mActor;
    cXyz sp3c = *param_2 - a_this->current.pos;
    if (fabsf(sp3c.y) > 100.0f || sp3c.abs() > i_this->field_0x571 * 100.0f) {
        return 1;
    }

    camera_class* camera = dComIfGp_getCamera(0);
    sp3c = camera->lookat.center - camera->lookat.eye;
    s16 sVar1 = cM_atan2s(sp3c.x, sp3c.z);
    sp3c = *param_2 - camera->lookat.eye;
    s16 sVar2 = cM_atan2s(sp3c.x, sp3c.z);
    s16 diff = sVar1 - sVar2;

    if (diff > 0x3000 || diff < -0x3000) {
        return 1;
    }

    return 0;
}

static int daObj_Lp_Execute(obj_lp_class* i_this) {
    i_this->field_0x574++;
    if (i_this->field_0xadb0) {
        i_this->field_0xadb0 += -1;
    }

    for (int i = 1; i < 10; i++) {
        target_info[i] = 0;
    }

    target_info[0] = (int)dComIfGp_getPlayer(0);
    target_info_count = 1;

    if (strcmp(dComIfGp_getStartStageName(), "D_MN05") == 0) {
        fpcM_Search(s_ks_sub, i_this);
    }

    if (i_this->field_0xadb1) {
        daCanoe_c* mCanoe = (daCanoe_c*)fopAcM_SearchByName(0xed);
        if (mCanoe) {
            tandem = mCanoe->checkTandem();
            dmg_rod_class* mDmgRod = (dmg_rod_class*)fopAcM_SearchByName(0x2e4);
            if (((mDmgRod && mDmgRod->play_cam_mode == 11) &&
                mDmgRod->play_cam_timer > 20) && mDmgRod->play_cam_timer < 0x3c) {
                    demo_f = 1;
            } else {
                demo_f = 0;
            }
        }
    }

    camera_class* camera = dComIfGp_getCamera(0);
    wd_ss* WdSs = i_this->mWdSs;
    for (int i = 0; i < i_this->field_0xad98; i++, WdSs++) {
        if ((i_this->field_0x574 + i & 0x1f) == 0) {
            f32 fVar1 = WdSs->field_0x10.x - camera->lookat.eye.x;
            f32 fVar2 = WdSs->field_0x10.z - camera->lookat.eye.z;
            fVar1 = JMAFastSqrt(fVar1 * fVar1 + fVar2 * fVar2);
            if (fVar1 < 5000.0f) {
                WdSs->field_0x4f = 0;
            } else {
                WdSs->field_0x4f = 1;
            }

            if (i == 0 && i_this->field_0xadb1) {
                i_this->field_0xadb2 = WdSs->field_0x4f;
            }
        }

        if (!WdSs->field_0x4f || i_this->field_0xadb0) {
            hasu_action(i_this, WdSs);
        }
    }

    f32 fVar7 = 0.0f;
    if ((i_this->field_0x574 & 15) == 0) {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        if (!set_out_check(i_this, &player->current.pos)) {
            fVar7 = fabsf(player->speedF);
            if (fVar7 > 1.0f) {
                i_this->field_0xad9c = player->current.pos;
            }
        }
    }

    SND_INFLUENCE* sound = dKy_Sound_get();
    if ((sound->actor_id != -1 && (i_this->field_0x574 +3 & 1) == 0) && !set_out_check(i_this, &sound->position)) {
        fVar7 = sound->field_0xc;
        i_this->field_0xad9c = sound->position;
        sound->timer = 1;
        sound->actor_id = -1;
    }

    if (fVar7 > 1.0f) {
        f32 fVar2 = fVar7 * 10.0f;
        if (fVar7 * 10.0f > 100.0f) {
            fVar2 = 100.0f;
        }

        if (i_this->field_0xada8 <= fVar2) {
            i_this->field_0xada8 = fVar2;
            i_this->field_0xadac = 0.0f;
        }
    }

    if (i_this->field_0xada8 > 0.1f) {
        cLib_addCalc0(&i_this->field_0xada8, 0.1f, 1.0f);
        i_this->field_0xadac += 10.0f;
    }

    return 1;
}

static int daObj_Lp_IsDelete(obj_lp_class* i_this) {
    return 1;
}

static int daObj_Lp_Delete(obj_lp_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)&i_this->mActor;
    if (i_this->field_0xadb4) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_lp", 3);
        dComIfGp_removeSimpleModel(modelData, fopAcM_GetRoomNo(a_this));
    }
    dComIfG_resDelete(&i_this->mPhase, "Obj_lp");
    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    obj_lp_class* i_this = (obj_lp_class*)a_this;

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_lp", 3);

    for (int i = 0; i < i_this->field_0xad98; i++) {
        i_this->mWdSs[i].mpModel = mDoExt_J3DModel__create(modelData, 0x20000, 0x11000084);
        if (i_this->mWdSs[i].mpModel == NULL) {
            return 0;
        }
    }

    if (i_this->field_0xadb1) {
        if (!i_this->field_0xad7c.init(i_this->field_0xad98, 3, 1)) {
            return 0;
        }

        for (int i = 0; i < i_this->field_0xad98; i++) {
            f32* mSize = i_this->field_0xad7c.getSize(i);
            for (int j = 0; j < 3; j++) {
                *mSize = 1.3f;
                mSize++;
            }
        }
    }
    
    return 1;
}

static int set_pos_check(obj_lp_class* i_this, int param_2) {
    f32 fVar1 = 35.0f;
    if (i_this->field_0x572 == 1) {
        fVar1 *= 1.5f;
    }

    for (int i = 0; i < param_2; i++) {
        cXyz sp48 = i_this->mWdSs[i].field_0x10 - i_this->mWdSs[param_2].field_0x10;
        if (sp48.abs() < fVar1) {
            return 0;
        }
    }

    return 1;
}

static int daObj_Lp_Create(fopAc_ac_c* a_this) {
    fopAcM_ct(a_this, obj_lp_class);
    obj_lp_class* i_this = (obj_lp_class*)a_this;

    int phase_state = dComIfG_resLoad(&i_this->mPhase, "Obj_lp");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("OBJ_LP PARAM %x\n", fopAcM_GetParam(i_this));

        i_this->field_0x570 = fopAcM_GetParam(i_this);
        i_this->field_0x571 = fopAcM_GetParam(i_this) >> 8;
        i_this->field_0x572 = fopAcM_GetParam(i_this) >> 16;

        if (i_this->field_0x572 == 0xff) {
            i_this->field_0x572 = 0;
        }

        i_this->field_0xad98 = i_this->field_0x570 + 1;

        if (dStage_stagInfo_GetSTType(dComIfGp_getStage()->getStagInfo()) == ST_FIELD) {
            i_this->field_0xad98 <<= 1;
            if (strcmp(dComIfGp_getStartStageName(), "F_SP127") == 0) {
                i_this->field_0xadb1 = 1;
            }
        }

        if (i_this->field_0xad98 > 0x200) {
            i_this->field_0xad98 = 0x200;
        }
        OS_REPORT("OBJ_LP//////////////OBJ_LP SET 1 !!\n");

        int iVar3;
        if (i_this->field_0xadb1 != 0) {
            iVar3 = 800;
        } else {
            iVar3 = 0x1e8;
        }

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, i_this->field_0xad98 * iVar3)) {
            OS_REPORT("//////////////OBJ_LP SET NON !!\n");
            return cPhs_ERROR_e;
        }
           
        OS_REPORT("//////////////OBJ_LP NUM : %d !!\n", i_this->field_0xad98);

        fopAcM_SetMtx(a_this, i_this->mWdSs[0].mpModel->getBaseTRMtx());
            
        f32 fVar1 = i_this->field_0x571 * 70.0f + 50.0f;
        fopAcM_SetMin(a_this, -fVar1, -100.0f, -fVar1);
        fopAcM_SetMax(a_this, fVar1, 100.0f, fVar1);
        MtxTrans(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z, 0);

        cXyz sp13c(0.0f, 0.0f, 0.0f);
        dBgS_GndChk dStack_dc;
        dBgS_ObjGndChk_Spl cStack_130;
        int iVar4 = 0;
        f32 fVar13 = 1.0f;
        if (i_this->field_0x572 == 1) {
            fVar13 = 1.5f;
        }

        for (int i = 0; i < i_this->field_0xad98; i++) {
            MtxPush();
            cMtx_YrotM(*calc_mtx, cM_rndF(65536.0f));
            MtxPosition(&sp13c, &i_this->mWdSs[i].field_0x10);
            sp13c.z = cM_rndF(1.0f);
            sp13c.z = 1.0f - sp13c.z * sp13c.z;
            sp13c.z *= (i_this->field_0x571 * 100.0f);
            MtxPull();

            Vec sp148;
            sp148.x = i_this->mWdSs[i].field_0x10.x;
            sp148.y = i_this->mWdSs[i].field_0x10.y + 100.0f;
            sp148.z = i_this->mWdSs[i].field_0x10.z;

            dStack_dc.SetPos(&sp148);
            cStack_130.SetPos(&sp148);
            i_this->mWdSs[i].field_0x10.y = dComIfG_Bgsp().GroundCross(&cStack_130);

            i_this->mWdSs[i].field_0x38 = cM_rndF(65536.0f);

            if (i == 0) {
                a_this->current.pos.y = i_this->mWdSs[i].field_0x10.y;
            }

            i_this->mWdSs[i].field_0x1c.y = dComIfG_Bgsp().GroundCross(&dStack_dc);

            if ((set_pos_check(i_this, i) && i_this->mWdSs[i].field_0x10.y - i_this->mWdSs[i].field_0x1c.y > 10.0f) && 
                (i_this->field_0xadb1 == 0 || i_this->mWdSs[i].field_0x10.y - i_this->mWdSs[i].field_0x1c.y < 200.0f)) {
                i_this->mWdSs[i].field_0x4e = 1;
                i_this->mWdSs[i].field_0x3c = fVar13 * (cM_rndFX(0.2f) + 1.0f);
                i_this->mWdSs[i].field_0x4 = i_this->mWdSs[i].field_0x10;
                i_this->mWdSs[i].field_0x1c.x = i_this->mWdSs[i].field_0x10.x + cM_rndFX(10.0f);
                i_this->mWdSs[i].field_0x1c.z = i_this->mWdSs[i].field_0x10.z + cM_rndFX(10.0f);
                i_this->mWdSs[i].field_0x50 = i;
            } else {
                i--;
                iVar4++;
                if (iVar4 > 10000) {
                    OS_REPORT("      植物の指定範囲が狭すぎて置けません！！！！\n");
                    return cPhs_ERROR_e;
                }
            }
        }

        i_this->field_0x574 = fopAcM_GetID(i_this);
        i_this->field_0xadb0 = 10;

        daObj_Lp_Execute(i_this);

        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Obj_lp", 3);
        JUT_ASSERT(0x3b0, modelData != NULL);

        dComIfGp_addSimpleModel(modelData, fopAcM_GetRoomNo(a_this), 0);
        if (modelData == NULL) {
            OS_REPORT("\x1b[43;30mリリーパッド:シンプルモデル登録失敗しました。\n");
        }
            
        i_this->field_0xadb4 = 1;
    }

    return phase_state;
}

static actor_method_class l_daObj_Lp_Method = {
    (process_method_func)daObj_Lp_Create,
    (process_method_func)daObj_Lp_Delete,
    (process_method_func)daObj_Lp_Execute,
    (process_method_func)daObj_Lp_IsDelete,
    (process_method_func)daObj_Lp_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_LP = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_OBJ_LP,            // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(obj_lp_class),   // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  708,                    // mPriority
  &l_daObj_Lp_Method,     // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};

AUDIO_INSTANCES;
