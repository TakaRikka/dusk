/**
 * @file d_a_e_cr_egg.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_cr_egg.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"

static int daE_CR_EGG_Draw(e_cr_egg_class* a_this) {
    fopAc_ac_c* actor = &a_this->enemy;

    g_env_light.settingTevStruct(0, &actor->current.pos, &actor->tevStr);
    g_env_light.setLightTevColorType_MAJI(a_this->model, &actor->tevStr);
    mDoExt_modelUpdateDL(a_this->model);

    dComIfGd_setSimpleShadow(&actor->current.pos, a_this->acch.GetGroundH(), 30.0f + TREG_F(10), a_this->acch.m_gnd, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    return 1;
}

static void e_cr_egg_move(e_cr_egg_class* a_this) {
    fopAc_ac_c* actor = &a_this->enemy;

    switch (a_this->mode) {
    case 0:
        a_this->mode = 1;
        a_this->timers[0] = 150;
        actor->speedF = 5.0f + cM_rndF(3.0f);
        actor->current.angle.y += (int)cM_rndFX(10000.0f);
    case 1:
    case 2:
    case 3:
    case 4:
        if (a_this->acch.ChkWallHit() && a_this->timers[1] == 0) {
            a_this->timers[1] = 10;
            actor->speedF *= -0.5f;
        }

        if (a_this->acch.ChkGroundHit()) {
            if (a_this->mode < 4) {
                static f32 spy[] = {17.0f, 8.0f, 5.0f};
                actor->speed.y = spy[a_this->mode - 1];
                actor->current.angle.y += (int)cM_rndFX(8000.0f);

                int sp28[3] = {40, 20, 10};
                Z2GetAudioMgr()->seStart(Z2SE_EN_CR_EGG_BOUND, &actor->current.pos, sp28[a_this->mode], 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
                a_this->mode++;
            }

            cLib_addCalc0(&actor->speedF, 1.0f, 0.5f + TREG_F(4));
        }

        if (a_this->timers[0] == 0 || a_this->ccSph.ChkTgHit() || a_this->ccSph.ChkAtHit()) {
            fopAcM_delete(actor);

            cXyz effscale(0.5f, 0.5f, 0.5f);
            u16 eff_id = 0x109;
            if (a_this->timers[0] == 0) {
                eff_id = 0x108;
            }
            cXyz effpos(actor->current.pos);
            dComIfGp_particle_set(eff_id, &effpos, NULL, &effscale);
            Z2GetAudioMgr()->seStart(Z2SE_EN_CR_EGG_BOMB, &actor->current.pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        }
    }
}

static void action(e_cr_egg_class* a_this) {
    fopAc_ac_c* actor = &a_this->enemy;
    cXyz mae;
    cXyz ato;

    switch (a_this->action) {
    case 0:
        e_cr_egg_move(a_this);
        break;
    }

    actor->current.angle.x += (int)(actor->speedF * (700.0f + TREG_F(9)));

    cMtx_YrotS(*calc_mtx, actor->current.angle.y);
    mae.x = 0.0f;
    mae.y = 0.0f;
    mae.z = actor->speedF;
    MtxPosition(&mae, &ato);
    actor->speed.x = ato.x;
    actor->speed.z = ato.z;
    actor->current.pos += actor->speed;
    actor->speed.y -= 3.0f;

    actor->current.pos.y -= 20.0f;
    actor->old.pos.y -= 20.0f;
    a_this->acch.CrrPos(dComIfG_Bgsp());
    actor->current.pos.y += 20.0f;
    actor->old.pos.y += 20.0f;
}

static int daE_CR_EGG_Execute(e_cr_egg_class* a_this) {
    fopAc_ac_c* actor = &a_this->enemy;
    cXyz sp2C;
    cXyz sp20;

    a_this->lifetime++;

    for (int i = 0; i < 2; i++) {
        if (a_this->timers[i] != 0) {
            a_this->timers[i]--;
        }
    }

    if (a_this->field_0x656 != 0) {
        a_this->field_0x656--;
    }

    action(a_this);

    mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y, actor->current.pos.z);
    mDoMtx_stack_c::YrotM(actor->current.angle.y);
    mDoMtx_stack_c::XrotM(actor->current.angle.x);
    mDoMtx_stack_c::transM(0.0f, TREG_F(12) - 20.0f, 0.0f);
    
    f32 size = 1.0f + TREG_F(17);
    mDoMtx_stack_c::scaleM(size, size, size);
    a_this->model->setBaseTRMtx(mDoMtx_stack_c::get());

    cXyz c_offset(0.0f, 20.0f, 0.0f);
    a_this->ccSph.SetC(actor->current.pos + c_offset);
    a_this->ccSph.SetR(size * (20.0f + BREG_F(0)));
    dComIfG_Ccsp()->Set(&a_this->ccSph);
    return 1;
}

static int daE_CR_EGG_IsDelete(e_cr_egg_class* a_this) {
    return 1;
}

static int daE_CR_EGG_Delete(e_cr_egg_class* a_this) {
    fopAc_ac_c* actor = &a_this->enemy;

    fpc_ProcID id = fopAcM_GetID(actor);
    dComIfG_resDelete(&a_this->phase, "E_CR");
    a_this->sound.stopAnime();
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    e_cr_egg_class* a_this = (e_cr_egg_class*)i_this;

    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("E_CR", 0xC);
    JUT_ASSERT(374, modelData != NULL);

    a_this->model = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (a_this->model == NULL) {
        return 0;
    }

    return 1;
}

static int daE_CR_EGG_Create(fopAc_ac_c* i_this) {
    e_cr_egg_class* a_this = (e_cr_egg_class*)i_this;
    fopAcM_ct(i_this, e_cr_egg_class);

    int phase_state = dComIfG_resLoad(&a_this->phase, "E_CR");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("E_CR_EGG PARAM %x\n", fopAcM_GetParam(i_this));
    
        a_this->field_0x5b4 = fopAcM_GetParam(i_this);
        if (a_this->field_0x5b4 == 0xFF) {
            a_this->field_0x5b4 = 0;
        }
    
        a_this->field_0x5b5 = (fopAcM_GetParam(i_this) & 0xFF00) >> 8;
        if (a_this->field_0x5b5 == 0xFF) {
            a_this->field_0x5b5 = 0;
        }
    
        OS_REPORT("E_CR_EGG//////////////E_CR_EGG SET 1 !!\n");
        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x820)) {
            OS_REPORT("//////////////E_CR_EGG SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////E_CR_EGG SET 2 !!\n");

        fopAcM_SetMtx(i_this, a_this->model->getBaseTRMtx());

        static dCcD_SrcSph cc_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0xd}, {0xd8fbfdff, 0x3}, 0x75}}, // mObj
                {dCcD_SE_NONE, 0x1, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 20.0f} // mSph
            } // mSphAttr
        };

        a_this->ccStts.Init(50, 0, i_this);
        a_this->ccSph.Set(cc_sph_src);
        a_this->ccSph.SetStts(&a_this->ccStts);
    
        a_this->sound.init(&i_this->current.pos, NULL, 3, 1);

        a_this->acch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1, &a_this->acchcir, fopAcM_GetSpeed_p(i_this), NULL, NULL);
        a_this->acchcir.SetWall(20.0f, 20.0f);
    
        daE_CR_EGG_Execute(a_this);
    }

    return phase_state;
}

static actor_method_class l_daE_CR_EGG_Method = {
    (process_method_func)daE_CR_EGG_Create,
    (process_method_func)daE_CR_EGG_Delete,
    (process_method_func)daE_CR_EGG_Execute,
    (process_method_func)daE_CR_EGG_IsDelete,
    (process_method_func)daE_CR_EGG_Draw,
};

extern actor_process_profile_definition g_profile_E_CR_EGG = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_E_CR_EGG,          // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(e_cr_egg_class), // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    114,                    // mPriority
    &l_daE_CR_EGG_Method,   // sub_method
    0x00040100,             // mStatus
    fopAc_ENEMY_e,          // mActorType
    fopAc_CULLBOX_0_e,      // cullType
};
