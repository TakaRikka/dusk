/**
 * d_a_b_go.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_b_go.h"
#include "SSystem/SComponent/c_math.h"
#include "f_op/f_op_actor_mng.h"
#include "d/d_com_inf_game.h"

daB_GO_HIO_c::daB_GO_HIO_c() {
    field_0x4 = -1;
    mSmallSize = 1.5f;
    mNormalSpeed = 15.0f;
    mAttackInitRange = 300;
    mDisplayModelImage = false;
}

static bool data_80604140;

static daB_GO_HIO_c l_HIO;

static int daB_GO_Draw(b_go_class* i_this) {
    if (l_HIO.mDisplayModelImage) {
        J3DModel* model = i_this->mpMorf->getModel();

        g_env_light.settingTevStruct(0, &i_this->current.pos, &i_this->tevStr);
        g_env_light.setLightTevColorType_MAJI(model, &i_this->tevStr);
        i_this->mpMorf->entryDL();
    }

    return 1;
}

static void anm_init(b_go_class* i_this, int i_anmID, f32 i_morf, u8 i_attribute, f32 i_speed) {
    J3DAnmTransform* bck = (J3DAnmTransform*)dComIfG_getObjectRes("B_go", i_anmID);
    i_this->mpMorf->setAnm(bck, i_attribute, i_morf, i_speed, 0.0f, -1.0f);
    i_this->mAnmID = i_anmID;
}

static void damage_check(b_go_class* i_this) {}

static void h_wait(b_go_class* i_this) {
    i_this->speedF = 0.0f;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_WAIT_03, 10.0f, 2, 1.0f);
        i_this->mMode = 1;
        i_this->mTimers[0] = cM_rndF(60.0f) + 100.0f;
        break;
    case 1:
        if (i_this->mTimers[0] == 0 || i_this->mDistToPlayer < l_HIO.mAttackInitRange) {
            i_this->mActionID = ACT_WALK;
            i_this->mMode = 0;
        }
        break;
    }
}

static void h_walk(b_go_class* i_this) {
    f32 speed = 0.0f;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_WALK, 10.0f, 2, 1.0f);
        i_this->mMode = 1;
        i_this->mTimers[0] = cM_rndF(60.0f) + 150.0f;
        break;
    case 1:
        speed = l_HIO.mNormalSpeed;
        if (i_this->mTimers[0] == 0) {
            i_this->mActionID = ACT_WAIT;
            i_this->mMode = 0;
        } else if (i_this->mDistToPlayer < l_HIO.mAttackInitRange) {
            i_this->mActionID = ACT_ATTACK;
            i_this->mMode = 0;
        }
        break;
    }

    cLib_addCalc2(&i_this->speedF, speed, 1.0f, 1.0f);
    cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mAngleToPlayer, 1, 0x200);
}

static void h_attack(b_go_class* i_this) {
    int anm_frame = i_this->mpMorf->getFrame();
    cLib_addCalc0(&i_this->speedF, 1.0f, 1.0f);

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, ANM_ATTACK, 10.0f, 0, 1.0f);
        i_this->mMode = 1;
        break;
    case 1:
        if (anm_frame >= 25 && anm_frame <= 33) {
            if (anm_frame == 25) {
                i_this->mSound.startCreatureSound(Z2SE_CM_KAZAKIRI_S, 0, -1);
                i_this->unk_0x660 = 1;
            } else {
                i_this->unk_0x660 = 2;
            }
        }

        if (i_this->mpMorf->isStop()) {
            i_this->mActionID = ACT_WAIT;
            i_this->mMode = 0;
        }
        break;
    }
}

static void action(b_go_class* i_this) {
    cXyz speed_offset;
    cXyz move_speed;

    i_this->mAngleToPlayer = fopAcM_searchPlayerAngleY(i_this);
    i_this->mDistToPlayer = fopAcM_searchPlayerDistance(i_this);

    fopAcM_OffStatus(i_this, 0);
    i_this->attention_info.flags = 0;

    if (i_this->mTimers[1] == 0) {
        if (i_this->field_0x692 != 2) {
            i_this->field_0x692 = 2;
            i_this->mTimers[1] = cM_rndF(200.0f) + 1000.0f;
        } else {
            i_this->field_0x692 = 1;
            i_this->mTimers[1] = cM_rndF(100.0f) + 300.0f;
        }
    }

    switch (i_this->mActionID) {
    case ACT_WAIT:
        h_wait(i_this);
        break;
    case ACT_WALK:
        h_walk(i_this);
        break;
    case ACT_ATTACK:
        h_attack(i_this);
        break;
    }

    cLib_addCalcAngleS2(&i_this->shape_angle.y, i_this->current.angle.y, 4, 0x2000);
    cMtx_YrotS(*calc_mtx, i_this->current.angle.y);

    speed_offset.x = 0.0f;
    speed_offset.y = 0.0f;
    speed_offset.z = i_this->speedF;

    MtxPosition(&speed_offset, &move_speed);
    i_this->speed.x = move_speed.x;
    i_this->speed.z = move_speed.z;

    i_this->current.pos += i_this->speed;
    i_this->speed.y += i_this->gravity;

    i_this->mAcch.CrrPos(dComIfG_Bgsp());
}

static int daB_GO_Execute(b_go_class* i_this) {
    i_this->unk_0x668++;

    for (int i = 0; i < 4; i++) {
        if (i_this->mTimers[i] != 0) {
            i_this->mTimers[i]--;
        }
    }

    if (i_this->unk_0x690 != 0) {
        i_this->unk_0x690--;
    }

    action(i_this);
    damage_check(i_this);

    mDoMtx_stack_c::transS(i_this->current.pos.x, i_this->current.pos.y, i_this->current.pos.z);
    mDoMtx_stack_c::YrotM(i_this->shape_angle.y);
    mDoMtx_stack_c::scaleM(l_HIO.mSmallSize, l_HIO.mSmallSize, l_HIO.mSmallSize);
    i_this->mpMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());

    i_this->mpMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(i_this)));
    i_this->mpMorf->modelCalc();

    return 1;
}

static int daB_GO_IsDelete(b_go_class* i_this) {
    return 1;
}

static int daB_GO_Delete(b_go_class* i_this) {
    dComIfG_resDelete(&i_this->mPhase, "B_go");

    if (i_this->field_0xd34) {
        data_80604140 = 0;
    }

    if (i_this->heap != NULL) {
        i_this->mpMorf->stopZelAnime();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    b_go_class* a_this = (b_go_class*)i_this;

    a_this->mpMorf = new mDoExt_McaMorfSO(
        (J3DModelData*)dComIfG_getObjectRes("B_go", RES_IS_MODEL), NULL, NULL,
        (J3DAnmTransform*)dComIfG_getObjectRes("B_go", ANM_WAIT_03), J3DFrameCtrl::EMode_LOOP,
        1.0f, 0, -1, &a_this->mSound, 0x80000, 0x11000084);

    if (a_this->mpMorf == NULL || a_this->mpMorf->getModel() == NULL) {
        return 0;
    }

    return 1;
}

static int daB_GO_Create(fopAc_ac_c* i_this) {
    fopAcM_ct(i_this, b_go_class);
    b_go_class* a_this = (b_go_class*)i_this;

    OS_REPORT("B_GO//////////////B_GO SET 0 !!\n");

    int phase_state = dComIfG_resLoad(&a_this->mPhase, "B_go");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("B_GO PARAM %x\n", fopAcM_GetParam(a_this));
        OS_REPORT("B_GO//////////////B_GO SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x4B000)) {
            OS_REPORT("//////////////B_GO SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////B_GO SET 2 !!\n");

        if (data_80604140 == 0) {
            a_this->field_0xd34 = 1;
            data_80604140 = 1;
            l_HIO.field_0x4 = -1;
        }

        a_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
        fopAcM_SetMtx(a_this, a_this->mpMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(a_this, -500.0f, -2000.0f, -500.0f);
        fopAcM_SetMax(a_this, 500.0f, 2000.0f, 500.0f);
        a_this->health = 1000;
        a_this->field_0x560 = 1000;

        a_this->mAcch.Set(fopAcM_GetPosition_p(a_this), fopAcM_GetOldPosition_p(a_this), a_this, 1,
                          &a_this->mAcchCir, fopAcM_GetSpeed_p(a_this), NULL, NULL);
        a_this->mAcchCir.SetWall(100.0f, 300.0f);

        a_this->mSound.init(&a_this->current.pos, &a_this->eyePos, 3, 1);
        a_this->mAtInfo.mpSound = &a_this->mSound;

        a_this->gravity = -7.0f;
        a_this->mActionID = ACT_WAIT;

        daB_GO_Execute(a_this);

        cXyz child_pos;
        child_pos.y = a_this->current.pos.y;

        csXyz child_angle(0, 0, 0);
        for (int i = 0; i < GORON_CHILD_MAX; i++) {
            child_pos.x = cM_rndFX(500.0f) + a_this->current.pos.x;
            child_pos.z = cM_rndFX(500.0f) + a_this->current.pos.z;
            child_angle.y = cM_rndF(0x10000);

            a_this->mGoronChildIDs[i] =
                fopAcM_createChild(PROC_B_GOS, fopAcM_GetID(a_this), i, &child_pos,
                                   fopAcM_GetRoomNo(a_this), &child_angle, NULL, -1, NULL);
        }
    }

    return phase_state;
}

b_go_class::b_go_class() {}

static actor_method_class l_daB_GO_Method = {
    (process_method_func)daB_GO_Create,  (process_method_func)daB_GO_Delete,
    (process_method_func)daB_GO_Execute, (process_method_func)daB_GO_IsDelete,
    (process_method_func)daB_GO_Draw,
};

extern actor_process_profile_definition g_profile_B_GO = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_B_GO,
    &g_fpcLf_Method.base,
    sizeof(b_go_class),
    0,
    0,
    &g_fopAc_Method.base,
    215,
    &l_daB_GO_Method,
    0x40100,
    fopAc_ENEMY_e,
    fopAc_CULLBOX_CUSTOM_e,
};
