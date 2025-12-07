/**
 * @file d_a_npc_df.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_df.h"
#include "d/actor/d_a_mg_rod.h"
#include "d/d_s_play.h"
#include "d/actor/d_a_npc_henna.h"
#include "Z2AudioLib/Z2Instances.h"

#if DEBUG
#define NPC_DF_HEAP_SIZE 0x4B000
#else
#define NPC_DF_HEAP_SIZE 0x3B90
#endif

static int daNpc_Df_Draw(npc_df_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    df_s* df_p = i_this->mDfs;
    int roomNo = fopAcM_GetRoomNo(actor);
    
    for (int i = 0; i < i_this->mDragonflyCount; i++, df_p++) {
        if (df_p->field_0x74 != 0) {
            dComIfGp_entrySimpleModel(df_p->mpModels[0], roomNo);
            dComIfGp_entrySimpleModel(df_p->mpModels[1], roomNo);
        }
    }

    return 1;
}

static dmg_rod_class* lrl;

static dBgS_LinChk line_check;

static dBgS_ObjGndChk gc_work;

static void df_fly(npc_df_class* i_this, df_s* df_p) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp40, sp4c;

    if ((df_p->field_0x6c + df_p->field_0x77 & 15) == 0) {
        sp40.set(df_p->field_0x8.x, df_p->field_0x8.y + 100.0f, df_p->field_0x8.z);
        gc_work.SetPos(&sp40);
        sp4c.y = dComIfG_Bgsp().GroundCross(&gc_work);
        if (sp4c.y > 10.0f) {
            df_p->field_0x68 = sp4c.y + 40.0f;
        } else {
            df_p->field_0x68 = 0.0f;
        }

        if (df_p->field_0x78[2] == 0) {
            cMtx_YrotS(*calc_mtx, df_p->field_0x44.y);
            sp40.x = 0.0f;
            sp40.y = 0.0f;
            sp40.z = 100.0f;
            MtxPosition(&sp40, &sp4c);
            sp4c += df_p->field_0x8;
            line_check.Set(&df_p->field_0x8, &sp4c, actor);

            if (dComIfG_Bgsp().LineCross(&line_check)) {
                df_p->field_0x78[2] = 50;
                df_p->field_0x4a.y += (int)(cM_rndFX(8000.0f) + 32768.0f);
                df_p->field_0x75 = 1;
                df_p->field_0x78[0] = cM_rndF(40.0f) + 80.0f;
                df_p->field_0x50 = 0.0f;
                df_p->field_0x54 = 4.0f;
            }
        }
    }

    if (df_p->field_0x75 == 0) {
        if (df_p->field_0x78[0] == 0) {
            if (df_p->field_0x77 == 0) {
                sp40.x = player->current.pos.x + cM_rndFX(300.0f) - df_p->field_0x8.x;
                sp40.y = player->current.pos.y + 1000.0f - df_p->field_0x8.y;
                sp40.z = player->current.pos.z + cM_rndFX(300.0f) - df_p->field_0x8.z;
            } else {
                sp40.x = player->current.pos.x + cM_rndFX(1500.0f) - df_p->field_0x8.x;
                sp40.y = player->current.pos.y + 50.0f + cM_rndF(700.0f) - df_p->field_0x8.y;
                sp40.z = player->current.pos.z + cM_rndFX(1500.0f) - df_p->field_0x8.z;
            }

            df_p->field_0x4a.x = -cM_atan2s(sp40.y, JMAFastSqrt(sp40.x * sp40.x + sp40.z * sp40.z));
            s16 sVar1 = cM_atan2s(sp40.x, sp40.z) - df_p->field_0x44.y;
            if (sVar1 > 0x3000) {
                sVar1 = 0x3000;
            } else if (sVar1 < -0x3000) {
                sVar1 = -0x3000;
            }

            df_p->field_0x72 = cM_rndF(10.0f) + 10.0f;
            df_p->field_0x4a.y += sVar1;
            df_p->field_0x75 = 1;
            df_p->field_0x78[0] = cM_rndF(60.0f) + 10.0f;
            df_p->field_0x54 = cM_rndF(3.0f) + 4.0f;
        }
    } else if (df_p->field_0x78[0] == 0) {
        df_p->field_0x75 = 0;
        df_p->field_0x78[0] = cM_rndF(70.0f) + 10.0f;
        df_p->field_0x54 = cM_rndF(1.5f) + 0.5f;
    }

    cLib_addCalcAngleS2(&df_p->field_0x44.y, df_p->field_0x4a.y, 2, 0x800);
    cLib_addCalcAngleS2(&df_p->field_0x44.x, df_p->field_0x4a.x, 2, 0x800);
    cLib_addCalc2(&df_p->field_0x2c.x, df_p->field_0x38.x, 0.1f, YREG_F(9) + 2.0f);
    cLib_addCalc2(&df_p->field_0x2c.y, df_p->field_0x38.y, 0.1f, YREG_F(9) + 2.0f);
    cLib_addCalc2(&df_p->field_0x2c.z, df_p->field_0x38.z, 0.1f, YREG_F(9) + 2.0f);
    cLib_addCalc2(&df_p->field_0x50, df_p->field_0x54, 0.2f, 0.3f);

    sp40.x = 0.0f;
    sp40.y = 0.0f;
    sp40.z = df_p->field_0x50;

    cMtx_YrotS(*calc_mtx, df_p->field_0x44.y);
    cMtx_XrotM(*calc_mtx, df_p->field_0x44.x);
    MtxPosition(&sp40, &sp4c);
    df_p->field_0x8 += sp4c;

    if (df_p->field_0x8.y < df_p->field_0x68 + 20.0f) {
        df_p->field_0x8.y = df_p->field_0x68 + 20.0f;
    }

    df_p->field_0x70 = 0;

    if (df_p->field_0x78[1] == 0) {
        df_p->field_0x38.x = cM_rndFX(YREG_F(8) + 30.0f);
        df_p->field_0x38.y = cM_rndF(YREG_F(8) + 40.0f);
        df_p->field_0x38.z = cM_rndFX(YREG_F(8) + 30.0f);
        df_p->field_0x78[1] = cM_rndF(30.0f) + 5.0f;
        df_p->field_0x72 = cM_rndF(5.0f) + 5.0f;

        if (df_p->field_0x77 == 0 && df_p->field_0x6c > 2000 && lrl != NULL) {
            if (lrl->field_0x6ac.y < player->eyePos.y + 80.0f) {
                if (lrl->kind == 0 || (lrl->kind != 0 && lrl->play_cam_mode == 7)) {
                    df_p->field_0x74 = 2;
                    df_p->field_0x75 = 0;
                    df_p->field_0x6c = 0;
                }
            }
        }
    }

    df_p->field_0x5c = YREG_F(6) + 0.2f + ((df_p->field_0x6c & 1) - 0.5f) * (YREG_F(7) + 0.2f);
}

static void df_rod(npc_df_class* i_this, df_s* df_p) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    if (lrl == NULL) {
        df_p->field_0x74 = 1;
        df_p->field_0x4a.y += (int)cM_rndFX(8192.0f);
        s16 sVar1 = -8192.0f - cM_rndF(4096.0f);
        df_p->field_0x4a.x = sVar1;
        df_p->field_0x44.x = sVar1;
        df_p->field_0x75 = 1;
        df_p->field_0x78[0] = cM_rndF(30.0f) + 20.0f;
        df_p->field_0x50 = df_p->field_0x54 = cM_rndF(3.0f) + 4.0f;
        df_p->field_0x78[1] = cM_rndF(60.0f) + 30.0f;
    } else {
        f32 fVar1 = 1.0f;

        if (df_p->field_0x75 == 0) {
            df_p->field_0x75 = 1;

            if (lrl->kind == 0) {
                df_p->field_0x7b = cM_rndF(4.99f) + 7.0f;
            } else {
                df_p->field_0x7b = cM_rndF(2.99f) + 9.0f;
            }

            df_p->field_0x64 = cM_rndF(1.0f);

            if (cM_rndF(1.0f) < 0.5f) {
                df_p->field_0x6e = -0x8000;
            } else {
                df_p->field_0x6e = 0;
            }

            df_p->field_0x2c.x = cM_rndFX(400.0f);
            df_p->field_0x2c.z = cM_rndFX(400.0f);
            df_p->field_0x2c.y = 500.0f;
            df_p->field_0x38.y = 5.0f;
            df_p->field_0x8 = lrl->mg_rod.field_0x0[df_p->field_0x7b];
            df_p->field_0x60 = -cM_rndF(0.25f);
            df_p->field_0x78[0] = 0;
        } else if (df_p->field_0x75 == 1) {
            fVar1 = 0.1f;
            df_p->field_0x38.y = 10.0f;

            if (df_p->field_0x78[1] == 0) {
                df_p->field_0x38.x = cM_rndFX(YREG_F(8) + 5.0f);
                df_p->field_0x38.z = cM_rndFX(YREG_F(8) + 5.0f);
                df_p->field_0x78[1] = cM_rndF(10.0f) + 5.0f;
            }

            if (df_p->field_0x2c.y < 50.0f) {
                cLib_addCalcAngleS2(&df_p->field_0x44.y, lrl->field_0x6c4 + df_p->field_0x6e, 4, 0x400);
            } else {
                if (fabsf(df_p->field_0x2c.x) > 10.0f && fabsf(df_p->field_0x2c.z) > 10.0f) {
                    cLib_addCalcAngleS2(&df_p->field_0x44.y, cM_atan2s(-df_p->field_0x2c.x, -df_p->field_0x2c.z), 4, 0x400);
                }
            }

            if (df_p->field_0x2c.y < 10.1f) {
                df_p->field_0x75 = 2;
            }
        } else {
            df_p->field_0x38.z = 0.0f;
            df_p->field_0x38.x = 0.0f;
            df_p->field_0x38.y = 5.0f;
            cLib_addCalcAngleS2(&df_p->field_0x44.y, lrl->field_0x6c4 + df_p->field_0x6e, 2, 0x800);
        }

        cXyz sp54(lrl->mg_rod.field_0x0[df_p->field_0x7b] + ((lrl->mg_rod.field_0x0[df_p->field_0x7b + 1] - lrl->mg_rod.field_0x0[df_p->field_0x7b]) * df_p->field_0x64));
        
        if ((i_this->field_0xd34 - lrl->mg_rod.field_0x0[8]).abs() > 5.0f || lrl->field_0x6ac.y > player->eyePos.y + 80.0f) {
            df_p->field_0x74 = 1;
            df_p->field_0x4a.y += (int)cM_rndFX(8192.0f);
            df_p->field_0x44.x = df_p->field_0x4a.x = -8192.0f - cM_rndF(4096.0f);
            df_p->field_0x75 = 1;
            df_p->field_0x78[0] = cM_rndF(30.0f) + 20.0f;
            df_p->field_0x50 = df_p->field_0x54 = cM_rndF(3.0f) + 4.0f;
            df_p->field_0x78[1] = cM_rndF(60.0f) + 30.0f;
        } else {
            df_p->field_0x8 = sp54;
            cLib_addCalc2(&df_p->field_0x2c.x, df_p->field_0x38.x, fVar1, 1.0f);
            cLib_addCalc2(&df_p->field_0x2c.y, df_p->field_0x38.y, fVar1, 1.0f);
            cLib_addCalc2(&df_p->field_0x2c.z, df_p->field_0x38.z, fVar1, 1.0f);

            if (df_p->field_0x2c.y < 5.1f) {
                if (df_p->field_0x78[0] == 0) {
                    df_p->field_0x78[0] = cM_rndF(15.0f) + 35.0f;

                    if (df_p->field_0x60 > -0.13f) {
                        df_p->field_0x60 = -0.13f;
                    } else if (df_p->field_0x60 > -0.2f) {
                        df_p->field_0x60 = -0.2f;
                    } else if (df_p->field_0x60 > -0.33f) {
                        df_p->field_0x60 = -0.33f;
                    } else {
                        df_p->field_0x60 = -0.42f;
                    }
                }

                cLib_addCalc2(&df_p->field_0x5c, df_p->field_0x60, 0.8f, 0.1f);

                npc_henna_class* henna_p = (npc_henna_class*)fopAcM_SearchByName(PROC_NPC_HENNA);
                if (henna_p != NULL) {
                    henna_p->field_0x7b8 = 10;
                }
            } else {
                df_p->field_0x5c = YREG_F(6) + 0.2f + ((df_p->field_0x6c & 1) - 0.5f) * (YREG_F(7) + 0.2f);
            }
        }
    }
}

static void df_control(npc_df_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    cXyz sp34;
    df_s* df_p = i_this->mDfs;

    for (int i = 0; i < i_this->mDragonflyCount; i++, df_p++) {
        if (df_p->field_0x74 != 0) {
            for (int j = 0; j < 3; j++) {
                if (df_p->field_0x78[j] != 0) {
                    df_p->field_0x78[j]--;
                }
            }

            if (df_p->field_0x74 == 1) {
                df_fly(i_this, df_p);
                df_p->field_0x70 = 0;
            } else {
                df_rod(i_this, df_p);
            }

            df_p->field_0x6c++;
            mDoMtx_stack_c::transS(df_p->field_0x8.x + df_p->field_0x2c.x, df_p->field_0x8.y + df_p->field_0x2c.y, df_p->field_0x8.z + df_p->field_0x2c.z);
            mDoMtx_stack_c::YrotM(df_p->field_0x44.y);
            mDoMtx_stack_c::XrotM(df_p->field_0x70);
            mDoMtx_stack_c::scaleM(df_p->field_0x58, df_p->field_0x58, df_p->field_0x58);

            if (df_p->field_0x72 != 0) {
                mDoMtx_stack_c::ZrotM((nREG_F(1) + 400.0f) * (df_p->field_0x72 * cM_ssin(df_p->field_0x72 * 0x2300)));
                df_p->field_0x72--;
            }

            df_p->mpModels[0]->setBaseTRMtx(mDoMtx_stack_c::get());
            mDoMtx_stack_c::transM(JREG_F(0), JREG_F(1) + 1.0f, JREG_F(2) + 3.0f);
            mDoMtx_stack_c::scaleM(1.0f, df_p->field_0x5c, 1.0f);
            df_p->mpModels[1]->setBaseTRMtx(mDoMtx_stack_c::get());
        }

        if (i == 0 && lrl != NULL) {
            i_this->field_0xd34 = lrl->mg_rod.field_0x0[8];
        }
    }
}

static void* s_rod_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_MG_ROD) {
        return i_actor;
    }

    return NULL;
}

static int daNpc_Df_Execute(npc_df_class* i_this) {
    lrl = (dmg_rod_class*)fpcM_Search(s_rod_sub, i_this);
    df_control(i_this);
    return 1;
}

static int daNpc_Df_IsDelete(npc_df_class* i_this) {
    return 1;
}

static int daNpc_Df_Delete(npc_df_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    fpc_ProcID id = fopAcM_GetID(actor);

    if (i_this->field_0x573) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Npc_df", 3);
        dComIfGp_removeSimpleModel(modelData, fopAcM_GetRoomNo(actor));
        modelData = (J3DModelData*)dComIfG_getObjectRes("Npc_df", 4);
        dComIfGp_removeSimpleModel(modelData, fopAcM_GetRoomNo(actor));
    }

    dComIfG_resDelete(&i_this->mPhase, "Npc_df");
    
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    npc_df_class* actor = (npc_df_class*)i_this;

    J3DModelData* modelData[2];
    modelData[0] = (J3DModelData*)dComIfG_getObjectRes("Npc_df", 3);
    JUT_ASSERT(605, modelData[0] != NULL);
    modelData[1] = (J3DModelData*)dComIfG_getObjectRes("Npc_df", 4);
    JUT_ASSERT(608, modelData[1] != NULL)

    for (int i = 0; i < actor->mDragonflyCount; i++) {
        for (int j = 0; j < 2; j++) {
            actor->mDfs[i].mpModels[j] = mDoExt_J3DModel__create(modelData[j], J3DMdlFlag_UseSharedDL, 0x11000084);
            if (actor->mDfs[i].mpModels[j] == NULL) {
                return 0;
            }
        }
    }

    return 1;
}

static cPhs__Step daNpc_Df_Create(fopAc_ac_c* i_this) {
    fopAcM_ct(i_this, npc_df_class);
    npc_df_class* actor = (npc_df_class*)i_this;

    cPhs__Step phase = (cPhs__Step)dComIfG_resLoad(&actor->mPhase, "Npc_df");
    if (phase == cPhs_COMPLEATE_e) {
        OS_REPORT("NPC_DF PARAM %x\n", fopAcM_GetParam(i_this));
        actor->field_0x570 = fopAcM_GetParam(i_this);
        actor->mDragonflyCount = actor->field_0x570 + 1;

        if (actor->mDragonflyCount > 16) {
            actor->mDragonflyCount = 16;
        }

        OS_REPORT("NPC_DF//////////////NPC_DF SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, NPC_DF_HEAP_SIZE)) {
            OS_REPORT("//////////////NPC_DF SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////NPC_DF SET 2 !!\n");

        for (int i = 0; i < actor->mDragonflyCount; i++) {
            actor->mDfs[i].field_0x77 = i;
            actor->mDfs[i].field_0x74 = 1;
            actor->mDfs[i].field_0x8 = i_this->home.pos;
            actor->mDfs[i].field_0x58 = 1.0f;
        }

        daNpc_Df_Execute(actor);

        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("Npc_df", 3);
        JUT_ASSERT(683, modelData != NULL);

        if (dComIfGp_addSimpleModel(modelData, fopAcM_GetRoomNo(i_this), 0) == -1) {
            OS_REPORT("1Bh,[43;30m赤トンボ：シンプルモデル登録失敗しました。\n,1Bh,[m");
        }

        modelData = (J3DModelData*)dComIfG_getObjectRes("Npc_df", 4);
        JUT_ASSERT(691, modelData != NULL);

        if (dComIfGp_addSimpleModel(modelData, fopAcM_GetRoomNo(i_this), 0) == -1) {
            OS_REPORT("1Bh,[43;30m赤トンボ：シンプルモデル登録失敗しました。\n,1Bh,[m");
        }

        actor->field_0x573 = 1;
    }

    return phase;
}

AUDIO_INSTANCES;

static actor_method_class l_daNpc_Df_Method = {
    (process_method_func)daNpc_Df_Create,
    (process_method_func)daNpc_Df_Delete,
    (process_method_func)daNpc_Df_Execute,
    (process_method_func)daNpc_Df_IsDelete,
    (process_method_func)daNpc_Df_Draw,
};

extern actor_process_profile_definition g_profile_NPC_DF = {
  fpcLy_CURRENT_e,        // mLayerID
  9,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_DF,            // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(npc_df_class),   // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  439,                    // mPriority
  &l_daNpc_Df_Method,     // sub_method
  0x00040000,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
