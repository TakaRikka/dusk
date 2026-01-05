/**
 * @file d_a_npc_inko.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_npc_inko.h"
#include "d/actor/d_a_obj_kage.h"
#include "d/actor/d_a_npc_henna.h"
#include "d/actor/d_a_player.h"
#include "d/d_s_play.h"
#include "c/c_damagereaction.h"

static int nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        int jnt_no = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        npc_inko_class* i_this = (npc_inko_class*)model->getUserArea();

        if (i_this != NULL) {
            MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);
            if (jnt_no == 1) {
                cMtx_YrotM(*calc_mtx, i_this->field_0x5c8.y);
                cMtx_XrotM(*calc_mtx, i_this->field_0x5c8.x);
                cMtx_ZrotM(*calc_mtx, i_this->field_0x5c8.z);
            } else if (jnt_no == 2) {
                cMtx_ZrotM(*calc_mtx, i_this->field_0x5e6);
            } else if (jnt_no == 3) {
                cMtx_ZrotM(*calc_mtx, i_this->field_0x5e0);
            } else if (jnt_no == 4) {
                cMtx_ZrotM(*calc_mtx, -i_this->field_0x5e0);
            } else if (jnt_no == 5 || jnt_no == 6) {
                cMtx_XrotM(*calc_mtx, i_this->field_0x5d4[jnt_no - 5]);
            }

            model->setAnmMtx(jnt_no, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);

            if (jnt_no == TREG_S(0)) {
                MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);
                cMtx_YrotM(*calc_mtx, TREG_S(1));
                cMtx_XrotM(*calc_mtx, TREG_S(2));
                cMtx_ZrotM(*calc_mtx, TREG_S(3));
                model->setAnmMtx(jnt_no, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }
        }
    }

    return 1;
}

static int daNpc_Inko_Draw(npc_inko_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    g_env_light.settingTevStruct(0, &actor->current.pos, &actor->tevStr);
    g_env_light.setLightTevColorType_MAJI(i_this->anm_p->getModel(), &actor->tevStr);
    i_this->anm_p->entryDL();
    return 1;
}

static void anm_init(npc_inko_class* i_this, int i_anmId, f32 i_morf, u8 i_mode, f32 i_speed) {
    i_this->anm_p->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("Npc_inko", i_anmId), i_mode, i_morf, i_speed, 0.0f, -1.0f, NULL);
    i_this->anmId = i_anmId;
}

static int daNpc_Inko_Execute(npc_inko_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    cXyz sp78;
    cXyz sp6C;

    i_this->field_0x596++;

    for (int i = 0; i < 4; i++) {
        if (i_this->field_0x59c[i] != 0) {
            i_this->field_0x59c[i]--;
        }
    }

    obj_kage_class* kage = (obj_kage_class*)fopAcM_SearchByID(actor->parentActorID);
    if (kage == NULL) {
        return 1;
    }

    s16 sp1A = 0;
    s16 sp18 = 0;
    MTXCopy(kage->model->getBaseTRMtx(), mDoMtx_stack_c::get());
    i_this->field_0x5c0 = kage->actor.shape_angle;

    if (i_this->field_0x598 <= 2) {
        mDoMtx_stack_c::transM(i_this->field_0x5a8, -78.0f + TREG_F(1), -5.0f + TREG_F(2));
        mDoMtx_stack_c::multVecZero(&i_this->field_0x5b4);
        cLib_addCalcAngleS2(&actor->shape_angle.y, i_this->field_0x59a, 1, 0xC00);
        cLib_addCalcAngleS2(&actor->shape_angle.x, 0, 1, 0xA00);
    } else {
        mDoMtx_stack_c::YrotM(i_this->field_0x59a);
        mDoMtx_stack_c::transM(0.0f, i_this->field_0x5e8, 40.0f + JREG_F(2));
        mDoMtx_stack_c::multVecZero(&i_this->field_0x5b4);
        cLib_addCalcAngleS2(&actor->shape_angle.y, (i_this->field_0x5c0.y + i_this->field_0x59a), 1, 0xA00);
        cLib_addCalcAngleS2(&actor->shape_angle.x, i_this->field_0x5ec, 1, 0xA00);
    }

    f32 var_f31;
    if (i_this->field_0x598 == 0) {
        if (i_this->field_0x59c[1] == 0) {
            i_this->field_0x59c[1] = 30.0f + cM_rndF(70.0f);
            i_this->field_0x5ac = cM_rndFX(30.0f);

            for (int i = 0; i < 10; i++) {
                i_this->field_0x5ac = cM_rndFX(30.0f);
                if (fabsf(i_this->field_0x5a8 - i_this->field_0x5ac) >= 10.0f) {
                    break;
                }
            }
        }

        cLib_addCalc2(&i_this->field_0x5a8, i_this->field_0x5ac, 1.0f, 1.5f + TREG_F(3));
        i_this->field_0x5e0 = 0;

        if (fabsf(i_this->field_0x5a8 - i_this->field_0x5ac) > 1.0f) {
            s16 sp16 = i_this->field_0x596 * (TREG_S(1) + 8000);
            var_f31 = (2.0f + TREG_F(4)) * cM_ssin(sp16);
            if (var_f31 < 0.0f) {
                var_f31 *= -1.0f;
            }

            if ((u16)sp16 < 0x4000) {
                i_this->field_0x5e0 = 10000;
            } else if ((u16)sp16 > 0x8000 && (u16)sp16 < 0xC000) {
                i_this->field_0x5e0 = 10000;
            }

            sp1A = (1300.0f + TREG_F(8)) * cM_ssin(sp16);
            if (sp1A < 0) {
                sp1A *= -1;
            }

            sp18 = (1000.0f + TREG_F(9)) * cM_ssin(sp16);
        } else {
            var_f31 = 0.0f;
        }

        actor->current.pos = i_this->field_0x5b4;

        u8 spA = 0;
        s16 sp14 = fopAcM_searchPlayerAngleY(actor);
        s16 sp12 = sp14 - actor->shape_angle.y;

        if (i_this->field_0x59c[2] == 0) {
            if (cM_rndF(1.0f) < 0.3f && sp12 < 0x3000 && sp12 > -0x3000) {
                i_this->field_0x598 = 3;
                anm_init(i_this, 4, 1.0f, 2, 1.2f + cM_rndF(3.0f));
                i_this->sound.startSound(Z2SE_LINKBIRD_WING, 0, -1);

                i_this->field_0x59c[0] = 40.0f + cM_rndF(100.0f);
                i_this->field_0x59a = (sp14 - i_this->field_0x5c0.y) + (s16)cM_rndFX(7000.0f);

                u16 sp10 = i_this->field_0x59a;
                if (cM_rndF(1.0f) < 0.5f && ((sp10 < 0x5000 && sp10 > 0xA000) || sp10 < 0x3000 || sp10 > 0xD000)) {
                    i_this->field_0x5e8 = -(70.0f + cM_rndF(8.0f));
                    i_this->field_0x5ec = -8000;
                } else {
                    i_this->field_0x5e8 = -(41.0f + cM_rndF(14.0f));
                    i_this->field_0x5ec = -10000;
                }
            } else {
                i_this->field_0x598 = 2;
                anm_init(i_this, 4, 1.0f, 2, 1.2f + cM_rndF(3.0f));
                i_this->sound.startSound(Z2SE_LINKBIRD_WING, 0, -1);
                i_this->field_0x5b0 = i_this->field_0x5a8;

                for (int i = 0; i < 10; i++) {
                    i_this->field_0x5ac = cM_rndFX(30.0f);
                    if (fabsf(i_this->field_0x5a8 - i_this->field_0x5ac) >= 10.0f) {
                        break;
                    }
                }

                if (cM_rndF(1.0f) < 0.5f) {
                    i_this->field_0x59a = i_this->field_0x5c0.y + 0x8000;
                } else {
                    i_this->field_0x59a = (s16)i_this->field_0x5c0.y;
                }
            }
        }
    } else if (i_this->field_0x598 == 2) {
        i_this->field_0x5e0 = 0;
        cLib_addCalc2(&i_this->field_0x5a8, i_this->field_0x5ac, 1.0f, 2.0f + TREG_F(7));

        if (fabsf(i_this->field_0x5a8 - i_this->field_0x5ac) <= 0.01f) {
            i_this->field_0x598 = 0;
            i_this->field_0x59c[2] = 30.0f + cM_rndF(100.0f);
            i_this->field_0x59c[1] = 20.0f + cM_rndF(20.0f);
            anm_init(i_this, 5, 3.0f, 2, 1.0f);
            kage->field_0x5a8 = 200.0f + KREG_F(11);
        }

        f32 temp_f30 = fabsf(i_this->field_0x5ac - i_this->field_0x5b0);
        f32 temp_f26 = fabsf(i_this->field_0x5ac - i_this->field_0x5a8);
        f32 temp_f25 = temp_f30 * (1.5f + BREG_F(19));
        var_f31 = temp_f25 * cM_ssin(32768.0f * (temp_f26 / temp_f30));
        if (var_f31 > 30.0f + BREG_F(18)) {
            var_f31 = 30.0f + BREG_F(18);
        }

        actor->current.pos = i_this->field_0x5b4;
    } else if (i_this->field_0x598 == 3) {
        i_this->field_0x5e0 = 0;
        cLib_addCalc2(&actor->current.pos.x, i_this->field_0x5b4.x, 0.8f, 5.0f);
        cLib_addCalc2(&actor->current.pos.y, i_this->field_0x5b4.y, 0.8f, 5.0f);
        cLib_addCalc2(&actor->current.pos.z, i_this->field_0x5b4.z, 0.8f, 5.0f);

        bool sp9 = (actor->current.pos - i_this->field_0x5b4).abs() < 0.1f;
        if (sp9 && i_this->anmId == 4) {
            anm_init(i_this, 5, 3.0f, 2, 1.0f);
            kage->field_0x5a8 = 300.0f + KREG_F(12);
        }

        i_this->field_0x5ce.z = 13000;

        if (i_this->field_0x59c[0] == 0) {
            anm_init(i_this, 4, 1.0f, 2, 1.2f + cM_rndF(3.0f));
            i_this->sound.startSound(Z2SE_LINKBIRD_WING, 0, -1);
            i_this->field_0x598 = 1;

            if (cM_rndF(1.0f) < 0.5f) {
                i_this->field_0x59a = i_this->field_0x5c0.y + 0x8000;
            } else {
                i_this->field_0x59a = (s16)i_this->field_0x5c0.y;
            }
        }
    } else if (i_this->field_0x598 == 1) {
        cLib_addCalc2(&actor->current.pos.x, i_this->field_0x5b4.x, 0.9f, 5.0f);
        cLib_addCalc2(&actor->current.pos.y, i_this->field_0x5b4.y, 0.9f, 2.5f);
        cLib_addCalc2(&actor->current.pos.z, i_this->field_0x5b4.z, 0.9f, 5.0f);

        bool sp8 = (actor->current.pos - i_this->field_0x5b4).abs() < 0.1f;
        if (sp8) {
            i_this->field_0x598 = 0;
            i_this->field_0x59c[2] = 30.0f + cM_rndF(100.0f);
            i_this->field_0x59c[1] = 20.0f + cM_rndF(20.0f);
            anm_init(i_this, 5, 3.0f, 2, 1.0f);
            i_this->field_0x5ce.z = 0;
            kage->field_0x5a8 = 200.0f + KREG_F(11);
        }
    }

    if (i_this->field_0x5c6 != 0) {
        i_this->field_0x5c6--;
    }

    cLib_addCalcAngleS2(&i_this->field_0x5c8.x, i_this->field_0x5ce.x, 2, 3000);
    cLib_addCalcAngleS2(&i_this->field_0x5c8.y, i_this->field_0x5ce.y, 2, 3000);
    cLib_addCalcAngleS2(&i_this->field_0x5c8.z, i_this->field_0x5ce.z, 2, 3000);

    if (i_this->field_0x5c6 == 0) {
        i_this->field_0x5c6 = 10.0f + cM_rndF(30.0f);
        i_this->field_0x5ce.x = cM_rndFX(3000.0f + BREG_F(1));
        i_this->field_0x5ce.y = cM_rndFX(3000.0f + BREG_F(1));
        i_this->field_0x5ce.z = cM_rndFX(3000.0f + BREG_F(1));
    }

    if (i_this->field_0x5e2 != 0) {
        i_this->field_0x5e2--;
    }

    if (i_this->field_0x5e2 == 5) {
        i_this->field_0x5e4 = 10.0f + cM_rndF(10.0f);
    }

    if (i_this->field_0x5e2 == 0) {
        i_this->field_0x5e2 = (s8)(30.0f + cM_rndF(90.0f));
        i_this->sound.startSound(Z2SE_LINKBIRD_CRY, 0, -1);
    }

    s16 spE = 0;
    if (i_this->field_0x5e4 != 0) {
        i_this->field_0x5e4--;
        spE = 10000;
    }

    cLib_addCalcAngleS2(&i_this->field_0x5e6, spE, 1, 4000);

    for (int i = 0; i < 2; i++) {
        if (i_this->field_0x5dc[i] != 0) {
            i_this->field_0x5dc[i]--;
        }

        if (i_this->field_0x5dc[i] == 0) {
            i_this->field_0x5d8[i] = cM_rndFX(500.0f + BREG_F(0));
            i_this->field_0x5dc[i] = 5.0f + cM_rndF(10.0f);
        }

        cLib_addCalcAngleS2(&i_this->field_0x5d4[i], i_this->field_0x5d8[i], 2, 0x1F4);
    }

    mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y + var_f31, actor->current.pos.z);
    mDoMtx_stack_c::YrotM(BREG_S(0) + (actor->shape_angle.y + sp18));
    mDoMtx_stack_c::XrotM(BREG_S(1) + (actor->shape_angle.x + sp1A));
    mDoMtx_stack_c::ZrotM(actor->shape_angle.z);
    mDoMtx_stack_c::transM(0.0f, 0.0f, BREG_F(4) + -2.0f);

    i_this->anm_p->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
    i_this->anm_p->play(NULL, 0, 0);
    i_this->anm_p->modelCalc();

    i_this->sound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));

    f32 temp_f1 = fopAcM_searchPlayerDistanceXZ(actor);
    if (temp_f1 < (100.0f + KREG_F(15))) {
        daPy_py_c::setLookPos(&actor->current.pos);
    }

    if (daPy_getPlayerActorClass()->checkFrontRollCrash() && temp_f1 < 200.0f) {
        i_this->field_0x59c[2] = 0;
        
        npc_henna_class* henna = (npc_henna_class*)fopAcM_SearchByName(PROC_NPC_HENNA);
        if (henna != NULL) {
            henna->field_0x7b5 = 40;
            data_80450C9D |= 0x40;
        }
    }

    if (i_this->field_0x5f4 != 0) {
        if (i_this->msgflow.doFlow(actor, NULL, 0)) {
            dComIfGp_event_reset();
            i_this->field_0x5f4 = 0;
        }
        return 1;
    }

    if (dComIfGp_event_runCheck() && actor->eventInfo.checkCommandTalk() && i_this->field_0x5f4 == 0) {
        int flowId;
        if (cM_rndF(1.0f) < 0.1f) {
            flowId = 0x361;
            npc_henna_class* henna = (npc_henna_class*)fopAcM_SearchByName(PROC_NPC_HENNA);
            if (henna != NULL) {
                henna->field_0x7ba = 2;
            }
        } else {
            static u32 mes_0[] = {
                0x357,
                0x358,
                0x359,
                0x35A,
            };

            static u32 mes_1[] = {
                0x35B,
                0x35C,
                0x35D,
                0x35E,
            };

            int sp30 = (data_80450C9D & 3);
            if (data_80450C9D & 0x40) {
                flowId = mes_1[sp30];
            } else {
                flowId = mes_0[sp30];
            }
        }

        i_this->msgflow.init(actor, flowId, 0, NULL);
        i_this->field_0x5f4 = 1;

        data_80450C9D++;
        data_80450C9D &= 0xC3;
    }

    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    f32 temp_f28 = kage->actor.current.pos.x - player->current.pos.x;
    f32 temp_f27 = kage->actor.current.pos.z - player->current.pos.z;

    if ((data_80450C9D & 0x80) && i_this->field_0x59c[3] < 15 && (SQUARE(temp_f28) + SQUARE(temp_f27)) < 23000.0f) {
        s16 spC = fopAcM_searchPlayerAngleY(actor);
        spC = (spC - player->shape_angle.y) + 0x8000;
        if (spC < 0x1800 && spC > -0x1800) {
            i_this->field_0x5f5 = 5;
        }
    }

    if (i_this->field_0x59c[3] == 0 || mDoCPd_c::getTrigA(PAD_1)) {
        i_this->field_0x59c[3] = 150.0f + cM_rndF(200.0f);
    }

    if (i_this->field_0x5f5 != 0) {
        i_this->field_0x5f5--;

        actor->eyePos = actor->current.pos;
        actor->eyePos.y += 20.0f;
        actor->attention_info.position = actor->eyePos;
        actor->attention_info.position.y += 20.0f;
        fopAcM_OnStatus(actor, 0);
        cLib_onBit<u32>(actor->attention_info.flags, fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
        actor->eventInfo.onCondition(1);
    } else {
        fopAcM_OffStatus(actor, 0);
        cLib_offBit<u32>(actor->attention_info.flags, fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
    }

    return 1;
}

static int daNpc_Inko_IsDelete(npc_inko_class* i_this) {
    return 1;
}

static int daNpc_Inko_Delete(npc_inko_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    fpc_ProcID id = fopAcM_GetID(i_this);
    dComIfG_resDelete(&i_this->phase, "Npc_inko");

    if (actor->heap != NULL) {
        i_this->sound.deleteObject();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* actor) {
    npc_inko_class* i_this = (npc_inko_class*)actor;
    i_this->anm_p = new mDoExt_McaMorf((J3DModelData*)dComIfG_getObjectRes("Npc_inko", 8), NULL, NULL, NULL, 0, 1.0f, 0, -1, 1, 0, 0x80000, 0x11000084);
    if (i_this->anm_p == NULL || i_this->anm_p->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = i_this->anm_p->getModel();
    model->setUserArea((uintptr_t)i_this);

    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
    }

    fopAcM_SetMtx(actor, model->getBaseTRMtx());
    i_this->sound.init(&actor->current.pos, 2);
    return 1;
}

static int daNpc_Inko_Create(fopAc_ac_c* actor) {
    npc_inko_class* i_this = (npc_inko_class*)actor;
    fopAcM_ct(actor, npc_inko_class);

    int phase_state = dComIfG_resLoad(&i_this->phase, "Npc_inko");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("NPC_INKO PARAM %x\n", fopAcM_GetParam(actor));
        i_this->field_0x570 = fopAcM_GetParam(actor);

        OS_REPORT("NPC_INKO//////////////NPC_INKO SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(actor, useHeapInit, 0xC00)) {
            OS_REPORT("//////////////NPC_INKO SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////NPC_INKO SET 2 !!\n");

        i_this->field_0x596 = cM_rndF(65536.0f);
        fopAcM_SetMin(actor, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(actor, 200.0f, 200.0f, 200.0f);
        daNpc_Inko_Execute(i_this);
    }

    return phase_state;
}

static actor_method_class l_daNpc_Inko_Method = {
    (process_method_func)daNpc_Inko_Create,
    (process_method_func)daNpc_Inko_Delete,
    (process_method_func)daNpc_Inko_Execute,
    (process_method_func)daNpc_Inko_IsDelete,
    (process_method_func)daNpc_Inko_Draw,
};

actor_process_profile_definition g_profile_NPC_INKO = {
  fpcLy_CURRENT_e,        // mLayerID
  8,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_NPC_INKO,          // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(npc_inko_class), // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  704,                    // mPriority
  &l_daNpc_Inko_Method,   // sub_method
  0x00040100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
