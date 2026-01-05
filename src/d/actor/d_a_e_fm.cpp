/**
 * @file d_a_e_fm.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_e_fm.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_player.h"
#include "d/d_s_play.h"
#include "d/actor/d_a_obj_hhashi.h"
#include "d/actor/d_a_obj_ystone.h"
#include "c/c_damagereaction.h"
#include "f_op/f_op_camera_mng.h"
#include "f_op/f_op_msg_mng.h"
#include "Z2AudioLib/Z2Instances.h"

class daE_FM_HIO_c : public JORReflexible {
public:
    daE_FM_HIO_c();

    void genMessage(JORMContext*);
    virtual ~daE_FM_HIO_c() {}

    /* 0x04 */ s8 no;
    /* 0x08 */ f32 base_size;
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 field_0x1c;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2c;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ s16 field_0x34;
    /* 0x36 */ s16 field_0x36;
    /* 0x38 */ s16 field_0x38;
    /* 0x3C */ f32 field_0x3c;
    /* 0x40 */ f32 field_0x40;
    /* 0x44 */ f32 field_0x44;
    /* 0x48 */ f32 chain_size;
    /* 0x4C */ f32 field_0x4c;
    /* 0x50 */ f32 field_0x50;
    /* 0x54 */ f32 field_0x54;
    /* 0x58 */ f32 field_0x58;
    /* 0x5C */ f32 field_0x5c;
    /* 0x60 */ u8 field_0x60;
    /* 0x61 */ u8 field_0x61;
    /* 0x64 */ f32 field_0x64;
    /* 0x68 */ f32 field_0x68;
    /* 0x6C */ f32 field_0x6c;
    /* 0x70 */ f32 field_0x70;
    /* 0x74 */ f32 field_0x74;
    /* 0x78 */ f32 field_0x78;
    /* 0x7C */ f32 field_0x7c;
    /* 0x80 */ f32 field_0x80;
    /* 0x84 */ f32 field_0x84;
    /* 0x88 */ f32 field_0x88;
    /* 0x8C */ f32 field_0x8c;
    /* 0x90 */ f32 field_0x90;
    /* 0x94 */ s16 field_0x94;
    /* 0x96 */ s16 field_0x96;
    /* 0x98 */ s16 field_0x98;
    /* 0x9A */ s16 field_0x9a;
    /* 0x9C */ s16 field_0x9c;
    /* 0xA0 */ f32 field_0xa0;
    /* 0xA4 */ f32 field_0xa4;
    /* 0xA8 */ f32 field_0xa8;
    /* 0xAC */ u8 field_0xac;
};

enum E_FM_RES_FILE_ID {
    /* BCK */
    /* 0x07 */ BCK_EF_FMATTACK_A = 7,
    /* 0x08 */ BCK_EF_FMATTACK_B,
    /* 0x09 */ BCK_FM_ANIMAL,
    /* 0x0A */ BCK_FM_ANIMAL02,
    /* 0x0B */ BCK_FM_ATTACK,
    /* 0x0C */ BCK_FM_ATTACK02,
    /* 0x0D */ BCK_FM_BREAKCHAINL,
    /* 0x0E */ BCK_FM_BREAKCHAINR,
    /* 0x0F */ BCK_FM_CHANCE,
    /* 0x10 */ BCK_FM_CHANCEDAMAGE,
    /* 0x11 */ BCK_FM_DAMAGEWALK,
    /* 0x12 */ BCK_FM_DAMAGE_L,
    /* 0x13 */ BCK_FM_DAMAGE_R,
    /* 0x14 */ BCK_FM_DEAD,
    /* 0x15 */ BCK_FM_DEMOEND01,
    /* 0x16 */ BCK_FM_DEMOEND02,
    /* 0x17 */ BCK_FM_DEMOEND03,
    /* 0x18 */ BCK_FM_DOWN,
    /* 0x19 */ BCK_FM_DOWN02,
    /* 0x1A */ BCK_FM_DOWNDAMAGE,
    /* 0x1B */ BCK_FM_DOWNFR,
    /* 0x1C */ BCK_FM_DOWNWAIT,
    /* 0x1D */ BCK_FM_HANGWAIT,
    /* 0x1E */ BCK_FM_KYORO2,
    /* 0x1F */ BCK_FM_OPDEMO,
    /* 0x20 */ BCK_FM_UP,
    /* 0x21 */ BCK_FM_UP02,
    /* 0x22 */ BCK_FM_WAIT01,
    /* 0x23 */ BCK_FM_WALK,
    /* 0x24 */ BCK_FM_WALK02,

    /* BMDE */
    /* 0x27 */ BMDE_FM_CORE = 0x27,

    /* BMDR */
    /* 0x2A */ BMDR_EF_FMATTACK_A = 0x2A,
    /* 0X2B */ BMDR_EF_FMATTACK_B,
    /* 0x2C */ BMDR_FM,
    /* 0x2D */ BMDR_HANDLE,
    /* 0x2E */ BMDR_KUSARI,

    /* BRK */
    /* 0x31 */ BRK_CORE_LIGHTON = 0x31,
    /* 0x32 */ BRK_EF_FMATTACK_B,
    /* 0x33 */ BRK_FM,
    /* 0x34 */ BRK_FM_ANIMAL02,
    /* 0x35 */ BRK_FM_ATTACK,
    /* 0x36 */ BRK_FM_DEMOEND01,
    /* 0x37 */ BRK_FM_DEMOEND02,
    /* 0x38 */ BRK_FM_DEMOEND03,
    /* 0x39 */ BRK_FM_HANGWAIT,
    /* 0x3A */ BRK_FM_OPDEMO,
    /* 0x3B */ BRK_FM_PUTOUT,
    /* 0x3C */ BRK_FM_PUTOUTWAIT,

    /* BTK */
    /* 0x3F */ BTK_CORE_BEAT = 0x3F,
    /* 0x40 */ BTK_EF_FMATTACK_A,
    /* 0x41 */ BTK_EF_FMATTACK_B,
    /* 0x42 */ BTK_FM,
    /* 0x43 */ BTK_FM_ANIMAL02,
    /* 0x44 */ BTK_FM_ATTACK,
    /* 0x45 */ BTK_FM_DEMOEND01,
    /* 0x46 */ BTK_FM_DEMOEND02,
    /* 0x47 */ BTK_FM_HANGWAIT,
    /* 0x48 */ BTK_FM_OPDEMO,
    /* 0x49 */ BTK_FM_PUTOUT,
    /* 0x4A */ BTK_FM_PUTOUTWAIT,
};

enum daE_FM_ACTION {
    ACTION_NORMAL,
    ACTION_FIGHT_RUN,
    ACTION_N_FIGHT,
    ACTION_F_FIGHT,
    ACTION_DAMAGE_RUN,
    ACTION_ANIMAL,
    ACTION_FIRE,
    ACTION_STOP,
    ACTION_DOWN = 9,
    ACTION_A_DOWN,
    ACTION_START,
    ACTION_END,
};

enum daE_FM_TexAnm {
    TEXANM_FM,
    TEXANM_ATTACK,
    TEXANM_PUTOUT,
    TEXANM_PUTOUT_WAIT,
    TEXANM_ANIMAL,
    TEXANM_OP_DEMO,
    TEXANM_HANG_WAIT,
    TEXANM_DEMO_END01,
    TEXANM_DEMO_END02,
};

daE_FM_HIO_c::daE_FM_HIO_c() {
    no = -1;

    base_size = 1.0f;
    field_0xc = 3.0f;
    field_0x10 = 2.5f;
    field_0x14 = 5.0f;
    field_0x18 = 4.0f;
    field_0x1c = 20000.0f;
    field_0x20 = 1500.0f;
    field_0x24 = 600.0f;
    field_0x28 = 3.0f;
    field_0x2c = 2.5f;
    field_0x30 = 2.5f;

    field_0x34 = 0xFF;
    field_0x36 = 0x8A;
    field_0x38 = 0x18;

    field_0x3c = 1.0f;
    field_0x40 = 120.0f;
    field_0x44 = 1000.0f;
    chain_size = 2.0f;
    field_0x4c = 35.0f;
    field_0x50 = 7000.0f;
    field_0x54 = 180.0f;
    field_0x58 = 20.0f;
    field_0x5c = 0.0f;
    field_0x60 = 0;
    field_0x61 = 0;
    field_0x8c = 1.7f;
    field_0x90 = 1.5f;
    field_0x64 = 1000.0f;
    field_0x6c = 1500.0f;
    field_0x74 = 0.0f;
    field_0x7c = 36000.0f;
    field_0x84 = 40.0f;
    field_0x68 = 35.0f;
    field_0x70 = 753.0f;
    field_0x78 = 258.0f;
    field_0x80 = -2100.0f;
    field_0x88 = 40.0f;

    field_0x94 = 140;
    field_0x96 = 270;
    field_0x98 = 28;
    field_0x9a = 100;
    field_0x9c = 400;

    field_0xa0 = 1100.0f;
    field_0xa4 = 300.0f;
    field_0xac = 0;
    field_0xa8 = 1200.0f;
}

static void* s_fmobj_del(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_OBJ_FMOBJ) {
        fopAcM_delete((fopAc_ac_c*)i_actor);
    }

    return NULL;
}

static u8 hio_set;

static daE_FM_HIO_c l_HIO;

static cXyz h_pos;

static f32 h_range;

static f32 h_power;

static s8 h_unk;

static void* s_hasira_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_Obj_HHASHI) {
        ((daObjHHASHI_c*)i_actor)->Rolling(&h_pos, h_range, h_power, h_unk);
    }

    return NULL;
}

static void* s_hasira_poscheck(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_Obj_HHASHI) {
        f32 x = ((fopAc_ac_c*)i_actor)->current.pos.x - ((fopAc_ac_c*)i_data)->current.pos.x;
        f32 z = ((fopAc_ac_c*)i_actor)->current.pos.z - ((fopAc_ac_c*)i_data)->current.pos.z;
        if (JMAFastSqrt((x * x) + (z * z)) < 500.0f) {
            s16 temp_r0 = ((fopAc_ac_c*)i_data)->shape_angle.y - cM_atan2s(x, z);
            if (temp_r0 < 0x3000 && temp_r0 > -0x3000) {
                return i_actor;
            }
        }
    }

    return NULL;
}

static f32 fire_range;

static void* s_hasira_eff_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_Obj_HHASHI) {
        daObjHHASHI_c* a_actor = (daObjHHASHI_c*)i_actor;
        for (int i = 0; i < 10; i++) {
            if (((e_fm_class*)i_data)->mHashiraActors[i] == a_actor) {
                return NULL;
            }
        }

        for (int i = 0; i < 10; i++) {
            if (((e_fm_class*)i_data)->mHashiraActors[i] == NULL) {
                e_fm_class* a_data = (e_fm_class*)i_data;
                if (a_actor->field_0x5a4 == 0) {
                    cXyz sp10 = a_data->current.pos - a_actor->current.pos;
                    if (sp10.abs() < fire_range) {
                        csXyz eff_rot;
                        eff_rot.x = eff_rot.z = 0;
                        eff_rot.y = cM_atan2s(sp10.x, sp10.z);

                        for (int j = 0; j < 3; j++) {
                            static u16 h_id[3] = {0x8151, 0x872A, 0x872B};
                            dComIfGp_particle_set(h_id[j], &a_actor->current.pos, &eff_rot, NULL);
                        }

                        ((e_fm_class*)i_data)->mHashiraActors[i] = a_actor;
                        return NULL;
                    }
                }
            }
        }
    }

    return NULL;
}

static void hasira_hahen_hit(cXyz* i_pos, f32 i_range, f32 i_power, s8 param_3) {
    h_pos = *i_pos;
    h_range = i_range;
    h_power = i_power;
    h_unk = param_3;
    fpcM_Search(s_hasira_sub, NULL);
}

static void carry_off(e_fm_class* i_this, int i_chainNo) {
    if (i_this->mChain[i_chainNo].field_0x617e != 0) {
        i_this->mChain[i_chainNo].field_0x617e = 0;
        mDoAud_seStart(Z2SE_EN_FM_CHAIN_CUTOFF, &i_this->mChain[i_chainNo].sound_pos, 0, 0);
    }

    if (i_this->mChain[i_chainNo].field_0x617c != 0) {
        i_this->mChain[i_chainNo].field_0x617c = 0;
        i_this->mChain[i_chainNo].field_0x617d = 20;
        daPy_getPlayerActorClass()->cancelFmChainGrab();
        dComIfGp_getVibration().StartShock(VIBMODE_S_POWER6, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
    }
}

static void anm_init(e_fm_class* i_this, int i_anm, f32 i_morf, u8 i_mode, f32 i_speed) {
    i_this->mpFmModelMorf->setAnm((J3DAnmTransform *)dComIfG_getObjectRes("E_fm", i_anm), i_mode, i_morf, i_speed, 0.0f, -1.0f, NULL);
    i_this->mAnm = i_anm;
}

static int nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        int jnt_no = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        e_fm_class* a_this = (e_fm_class*)model->getUserArea();

        if (a_this != NULL) {
            MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);
            if (jnt_no == 2) {
                cMtx_YrotM(*calc_mtx, -a_this->mBodyRotX);
                cMtx_ZrotM(*calc_mtx, a_this->mHeadRotZ);
            } else if (jnt_no == 1) {
                cMtx_XrotM(*calc_mtx, a_this->mBodyRotX);
            }
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }

    return 1;
}

static f32 chain_z[25] = {
    2800.0f,
    2000.0f,
    -3000.0f,
    -3300.0f,
    3200.0f,
    2700.0f,
    -1800.0f,
    -2600.0f,
    2000.0f,
    1400.0f,
    -2900.0f,
    -1600.0f,
    2200.0f,
    2500.0f,
    -1300.0f,
    -2100.0f,
    1100.0f,
    2000.0f,
    -3100.0f,
    -2300.0f,
    1800.0f,
    2900.0f,
    -1100.0f,
    -2700.0f,
    2300.0f,
};

static void chain_draw(e_fm_class* i_this, chain_s* i_chain_s, int i_chainNo) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    cXyz sp1C;

    f32 var_f31 = i_chain_s->field_0x6174 * (0.002f + TREG_F(9));
    if (var_f31 > 0.6f + TREG_F(10)) {
        var_f31 = 0.6f + TREG_F(10);
    }

    f32 var_f30 = i_chain_s->field_0x6178;
    if (var_f30 > 0.75f) {
        var_f30 = 0.75f;
    }

    J3DModel* model;
    for (int i = 0; i < i_chain_s->field_0x61d0 - 1; i++) {
        MtxTrans(i_chain_s->pos[i].x, i_chain_s->pos[i].y, i_chain_s->pos[i].z, 0);
        cMtx_YrotM(*calc_mtx, i_chain_s->rot[i].y);
        cMtx_XrotM(*calc_mtx, i_chain_s->rot[i].x);

        if (i != i_chain_s->field_0x61d0 - 2 || i_chain_s->field_0x617c == 0) {
            s16 spC = (1.0f - var_f30) * chain_z[i];
            if ((i + i_chain_s->field_0x61d0) & 1) {
                spC += 0x4000;
            }
            cMtx_ZrotM(*calc_mtx, spC);
        }

        MtxScale(l_HIO.chain_size, l_HIO.chain_size, l_HIO.chain_size + var_f31, 1);
        MtxTrans(0.0f, 0.0f, 8.0f + XREG_F(4), 1);

        if (i_chainNo >= 2) {
            if (i == i_chain_s->field_0x61d0 - 2) {
                model = i_chain_s->model[24];
            } else {
                model = i_chain_s->model[i];
            }
        } else {
            model = i_chain_s->model[i];
        }

        model->setBaseTRMtx(*calc_mtx);
        g_env_light.settingTevStruct(0, &i_chain_s->pos[i], &i_chain_s->tevstr[i]);
        g_env_light.setLightTevColorType_MAJI(model, &i_chain_s->tevstr[i]);

        J3DModelData* modelData = model->getModelData();
        J3DMaterial* material = modelData->getMaterialNodePointer(0);
        if (i < 6) {
            material->getTevColor(0)->r = 0.166667f * ((6 - i) * i_this->mChainColorR);
        } else {
            material->getTevColor(0)->r = 0;
        }

        mDoExt_modelUpdate(model);
    }
}

static int daE_FM_Draw(e_fm_class* i_this) {
    J3DModel* model = i_this->mpFmModelMorf->getModel();
    g_env_light.settingTevStruct(0, &i_this->current.pos, &i_this->tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &i_this->tevStr);
    i_this->mpFmBtk[i_this->mPlayTexAnmNo]->entry(model->getModelData());
    i_this->mpFmBrk[i_this->mPlayTexAnmNo]->entry(model->getModelData());

    J3DModelData* modelData = model->getModelData();
    J3DMaterial* chain_material = modelData->getMaterialNodePointer(4);

    chain_material->getTevColor(0)->r = i_this->mChainColorR;
    chain_material = modelData->getMaterialNodePointer(0);
    
    int color = 255.0f * i_this->field_0x778;
    chain_material->getTevColor(2)->r = color;
    chain_material->getTevColor(2)->g = color;
    chain_material->getTevColor(2)->b = color;

    i_this->mpFmModelMorf->entryDL();

    if (!i_this->mHideCore) {
        g_env_light.setLightTevColorType_MAJI(i_this->mpCoreModel, &i_this->tevStr);
        fopAcM_setEffectMtx(i_this, i_this->mpCoreModel->getModelData());
        i_this->mpCoreBtk->entry(i_this->mpCoreModel->getModelData());
        i_this->mpCoreBrk->entry(i_this->mpCoreModel->getModelData());
        mDoExt_modelUpdateDL(i_this->mpCoreModel);
    }

    if (i_this->field_0x792 == 0 && i_this->mShadowKey != 353535) {
        cXyz sp10;
        sp10.set(i_this->current.pos.x, 100.0f + i_this->current.pos.y, i_this->current.pos.z);

        i_this->mShadowKey = dComIfGd_setShadow(i_this->mShadowKey, 1, model, &sp10, (2500.0f + BREG_F(19)) * l_HIO.base_size, 0.0f, i_this->current.pos.y, i_this->mAcch.GetGroundH(), i_this->mAcch.m_gnd, &i_this->tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    }

    if (i_this->mDrawDemoModel) {
        J3DModel* model = i_this->mpDemoFmModelMorf->getModel();
        g_env_light.setLightTevColorType_MAJI(model, &i_this->tevStr);
        i_this->mpDemoFmBrk->entry(model->getModelData());
        i_this->mpDemoFmModelMorf->entryDL();
    }

    if (i_this->field_0x790 != 0) {
        for (int i = 0; i < 2; i++) {
            J3DModel* model = i_this->mpAttackEfModelMorf[i]->getModel();
            i_this->mpAttackEfBtk[i]->entry(model->getModelData());
            if (i != 0) {
                i_this->mpAttackEfBrk->entry(model->getModelData());
            }
            i_this->mpAttackEfModelMorf[i]->entryDL();
        }
    }

    for (int i = 0; i < 4; i++) {
        chain_draw(i_this, &i_this->mChain[i], i);
    }

    return 1;
}

static BOOL other_bg_check(e_fm_class* i_this, fopAc_ac_c* i_other) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    dBgS_LinChk linchk;
    cXyz sp24;
    cXyz start;
    cXyz end;

    if (i_other != NULL) {
        end = i_other->current.pos;
        end.y += 30.0f;
        start = actor->current.pos;
        start.y = actor->eyePos.y;

        linchk.Set(&start, &end, actor);
        if (dComIfG_Bgsp().LineCross(&linchk)) {
            return TRUE;
        }
        return FALSE;
    }

    return TRUE;
}

static BOOL pl_check(e_fm_class* i_this, f32 i_range, s16 i_sightRange) {
    if (i_sightRange != 0) {
        s16 angle_diff = i_this->shape_angle.y - i_this->mAngleToPlayer;
        if (angle_diff > i_sightRange) {
            return FALSE;
        } else if (angle_diff < (s16)-i_sightRange) {
            return FALSE;
        }
    }

    if (fopAcM_searchPlayerDistance(i_this) < i_range) {
        return TRUE;
    }

    return FALSE;
}

static void e_fm_normal(e_fm_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    if (i_this->mMode >= 0 && !fopAcM_otherBgCheck(i_this, player)) {
        i_this->mAction = ACTION_FIGHT_RUN;
        i_this->mMode = 0;
        i_this->mSound.startCreatureVoice(Z2SE_EN_FM_V_FIND, -1);
        return;
    }

    cXyz sp50;
    cXyz sp44;
    f32 move_speed = 0.0f;
    s16 spA = 0x3000;
    int anm_frame = (int)i_this->mpFmModelMorf->getFrame();

    i_this->field_0x7c0 = 1;

    switch (i_this->mMode) {
    case 2:
        break;
    case -10:
        fpcM_Search(s_fmobj_del, NULL);
        if (i_this->mTimers[0] == 0 && !dComIfGp_event_runCheck()) {
            i_this->mMode = 0;
        }
        break;
    case 0:
        if (i_this->mTimers[0] == 0) {
            sp44.x = i_this->home.pos.x + cM_rndFX(300.0f);
            sp44.y = i_this->home.pos.y;
            sp44.z = i_this->home.pos.z + cM_rndFX(300.0f);
            sp50 = sp44 - i_this->current.pos;

            s16 sp8 = cM_atan2s(sp50.x, sp50.z) - i_this->current.angle.y;
            if (sp8 > 0x4000) {
                sp8 = 0x4000;
            } else if (sp8 < -0x4000) {
                sp8 = -0x4000;
            }

            i_this->field_0x5c4 = i_this->current.angle.y + sp8;
            anm_init(i_this, BCK_FM_WALK, 20.0f, 2, l_HIO.field_0xc);
            i_this->mMode = 1;
            i_this->mTimers[0] = 100.0f + cM_rndF(100.0f);
        } else {
            spA = 0x7000;
        }
        break;
    case 1:
        i_this->mpFmModelMorf->setPlaySpeed(l_HIO.field_0xc);

        for (int i = 0; i < 2; i++) {
            int sp1C = 1 - i;
            if (fabsf(i_this->mFootPos[i].y - i_this->current.pos.y) < 15.0f && fabsf(i_this->mFootPos[sp1C].y - i_this->current.pos.y) >= 15.0f) {
                move_speed = l_HIO.field_0x10;
                i_this->field_0x7e0 = sp1C;
            }
        }

        if (i_this->speedF < 1.0f && move_speed > 1.0f) {
            i_this->mSound.startCreatureSound(Z2SE_EN_FM_CHAIN_FOOTNOTE, 0, -1);
        }

        if (i_this->speedF > 1.0f && move_speed < 1.0f) {
            dComIfGp_getVibration().StartShock(VIBMODE_S_POWER5, 0xF, cXyz(0.0f, 1.0f, 0.0f));
            dComIfGp_particle_set(0x8150, &i_this->home.pos, NULL, NULL);
            mDoAud_seStart(Z2SE_OBJ_FM_DIRTFALL, NULL, 0, 0);
            i_this->mSound.startCreatureSound(Z2SE_EN_FM_FOOTNOTE, 0, -1);
        }

        if (move_speed > 1.0f) {
            cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->field_0x5c4, 8, 0x100);
        }

        if (i_this->mTimers[0] == 0) {
            i_this->mMode = 0;
            i_this->mTimers[0] = 50.0f + cM_rndF(100.0f);
            anm_init(i_this, BCK_FM_KYORO2, 50.0f, 2, 1.0f);
            move_speed = 0.0f;
        } else {
            for (int i = 0; i < 2; i++) {
                if (i_this->mChain[i].field_0x617e != 0) {}
            }
        }
        break;
    }

    i_this->speedF = move_speed;
}

static void e_fm_fight_run(e_fm_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp4C;
    cXyz sp40;

    f32 move_speed = 0.0f;
    int anm_frame = i_this->mpFmModelMorf->getFrame();
    i_this->field_0x7c0 = 1;
    s8 do_f_fight = FALSE;
    s8 do_n_fight = FALSE;

    switch (i_this->mMode) {
    case 0:
        i_this->mTimers[0] = 20.0f + cM_rndF(10.0f);
        anm_init(i_this, BCK_FM_WAIT01, 30.0f, 2, 1.0f);
        i_this->mTimers[2] = l_HIO.field_0x40 + cM_rndF(0.5f * l_HIO.field_0x40);
        i_this->mMode = 1;
        /* fallthrough */
    case 1:
        if (i_this->mTimers[0] == 0) {
            anm_init(i_this, BCK_FM_WALK02, 30.0f, 2, l_HIO.field_0xc);
            i_this->mMode = 2;
        }
        break;
    case 2:
        i_this->mpFmModelMorf->setPlaySpeed(l_HIO.field_0xc);
        for (int i = 0; i <= 1; i++) {
            int sp18 = 1 - i;
            if (fabsf(i_this->mFootPos[i].y - i_this->current.pos.y) < 15.0f && fabsf(i_this->mFootPos[sp18].y - i_this->current.pos.y) >= 15.0f) {
                move_speed = l_HIO.field_0x14;
                i_this->field_0x7e0 = sp18;
            }
        }

        if (i_this->speedF < 1.0f && move_speed > 1.0f) {
            i_this->mSound.startCreatureSound(Z2SE_EN_FM_CHAIN_FOOTNOTE, 0, -1);
        }

        if (i_this->speedF > 1.0f && move_speed < 1.0f) {
            dComIfGp_getVibration().StartShock(VIBMODE_S_POWER5, 0xF, cXyz(0.0f, 1.0f, 0.0f));
            dComIfGp_particle_set(0x8150, &i_this->home.pos, NULL, NULL);
            mDoAud_seStart(Z2SE_OBJ_FM_DIRTFALL, NULL, 0, 0);
            i_this->mSound.startCreatureSound(Z2SE_EN_FM_FOOTNOTE, 0, -1);
            hasira_hahen_hit(&i_this->mFootPos[i_this->field_0x7e0], 100.0f + VREG_F(4), 20.0f + VREG_F(5), -1);
        }

        for (int i = 2; i <= 3; i++) {
            sp4C = i_this->mChain[i].field_0x6180 - i_this->current.pos;
            s16 spC = i_this->shape_angle.y - cM_atan2s(sp4C.x, sp4C.z);

            if ((spC > 0x6000 || spC < -0x6000) && (i_this->mChain[i].field_0x617e != 0 || (i_this->mChain[i].field_0x617c != 0 && daPy_getPlayerActorClass()->checkEquipHeavyBoots()))) {
                f32 var_f30;
                if (i_this->mChain[i].field_0x617c != 0) {
                    var_f30 = 280.0f + VREG_F(10);
                } else {
                    var_f30 = 100.0f + VREG_F(11);
                }

                if (i_this->mChain[i].field_0x6174 > var_f30 && ((i == 2 && (anm_frame & 252) == 12) || (i == 3 && (anm_frame & 252) == 40))) {
                    i_this->mAction = ACTION_STOP;
                    i_this->mMode = 0;
                    i_this->field_0x808 = i;
                    return;
                }
            }
        }

        if (i_this->field_0x809 != 0) {
            s16 spA = i_this->mAngleToPlayer - i_this->current.angle.y;
            if (spA < 0x400 && spA > -0x400) {
                do_f_fight = TRUE;
            }
        }

        if (i_this->mAcch.ChkWallHit() && fpcM_Search(s_hasira_poscheck, i_this)) {
            do_n_fight = TRUE;
        }
    }

    i_this->speedF = move_speed;

    if (move_speed > 1.0f) {
        cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mAngleToPlayer, 2, KREG_S(6) + 0x500);
    }

    if (pl_check(i_this, l_HIO.field_0x24 + TREG_F(11), 0x3000)) {
        do_n_fight = TRUE;
    } else if (pl_check(i_this, l_HIO.field_0x20 + TREG_F(11), 0) && i_this->mTimers[2] == 0) {
        do_f_fight = TRUE;
    }

    if (do_f_fight) {
        i_this->mAction = ACTION_F_FIGHT;
        i_this->mMode = 0;
        anm_init(i_this, BCK_FM_WAIT01, 20.0f, 2, 1.0f);
    }

    if (do_n_fight) {
        i_this->mAction = ACTION_N_FIGHT;
        i_this->mMode = 0;
    }
}

static void e_fm_n_fight(e_fm_class* i_this) {
    int anm_frame = i_this->mpFmModelMorf->getFrame();
    cXyz sp28;
    cXyz sp1C;

    i_this->field_0x7c0 = 1;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_FM_ATTACK02, 10.0f, 0, 1.0f);
        i_this->mSound.startCreatureSound(Z2SE_EN_FM_ATTACK02_CHAIN, 0, -1);
        i_this->mMode = 1;
        break;
    case 1:
        if (anm_frame >= 30 && anm_frame <= 40) {
            i_this->field_0x1829 = 1;
            i_this->field_0x1828 = 1;

            if (anm_frame == AREG_S(6) + 35) {
                i_this->field_0x2399 = 2;
            }

            if (anm_frame == 30) {
                i_this->mSound.startCreatureSound(Z2SE_EN_FM_BUN, 0, -1);
                i_this->mSound.startCreatureVoice(Z2SE_EN_FM_V_UDEHURI, -1);
            }

            if (anm_frame == 35) {
                carry_off(i_this, i_this->field_0x1828);
            }
        } else if (anm_frame >= 50 && anm_frame <= 60) {
            i_this->field_0x1829 = 1;
            i_this->field_0x1828 = 0;

            if (anm_frame == AREG_S(7) + 57) {
                i_this->field_0x2399 = 1;
            }

            if (anm_frame == 52) {
                i_this->mSound.startCreatureSound(Z2SE_EN_FM_BUN, 0, -1);
                i_this->mSound.startCreatureVoice(Z2SE_EN_FM_V_UDEHURI, -1);
            }

            if (anm_frame == 53) {
                carry_off(i_this, i_this->field_0x1828);
            }
        }

        if (i_this->mpFmModelMorf->isStop()) {
            if (cM_rndF(1.0f) < 0.5f && pl_check(i_this, l_HIO.field_0x24 + TREG_F(11), 0x3000)) {
                i_this->mAction = ACTION_FIRE;
                anm_init(i_this, BCK_FM_WAIT01, 20.0f, 2, 1.0f);
            } else {
                i_this->mAction = ACTION_NORMAL;
            }
            i_this->mMode = 0;
        }
        break;
    }

    cLib_addCalc0(&i_this->speedF, 1.0f, 1.0f);

    if (i_this->field_0x1829 != 0) {
        cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->mAngleToPlayer, 2, KREG_S(6) + 0x500);
    }
}

static void tame_eff_set(e_fm_class* i_this) {
    J3DModel* model = i_this->mpFmModelMorf->getModel();
    cXyz mae(0.0f, 0.0f, 0.0f);
    cXyz ato;

    MTXCopy(model->getAnmMtx(1), *calc_mtx);
    MtxPosition(&mae, &ato);

    i_this->field_0x1870 = dComIfGp_particle_set(i_this->field_0x1870, 0x814D, &ato, &i_this->shape_angle, NULL);
    i_this->field_0x1874 = dComIfGp_particle_set(i_this->field_0x1874, 0x814E, &i_this->current.pos, NULL, NULL);
    i_this->field_0x1878 = dComIfGp_particle_set(i_this->field_0x1878, 0x814F, &i_this->current.pos, NULL, NULL);

    if (i_this->field_0x792 == 0) {
        for (int i = 0; i < 2; i++) {
            static u16 tame_name[] = {0x81A3, 0x81A4};
            i_this->field_0x187c[i] = dComIfGp_particle_set(i_this->field_0x187c[i], tame_name[i], &ato, NULL, NULL);

            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->field_0x187c[i]);
            if (emitter != NULL) {
                emitter->setGlobalRTMatrix(model->getAnmMtx(1));
            }
        }
    }
}

static void e_fm_f_fight(e_fm_class* i_this) {
    cXyz sp30;
    cXyz sp24;
    int anm_frame = i_this->mpFmModelMorf->getFrame();

    switch (i_this->mMode) {
    case 0:
        if ((int)i_this->mpFmBtk[0]->getFrame() == 0) {
            anm_init(i_this, BCK_FM_ATTACK, 10.0f, 0, 1.0f);
            i_this->mSound.startCreatureVoice(Z2SE_EN_FM_V_ATTACK_TAME, -1);
            i_this->mSound.startCreatureSound(Z2SE_EN_FM_ATTACK_TAME, 0, -1);

            i_this->mPlayTexAnmNo = TEXANM_ATTACK;
            i_this->mpFmBrk[TEXANM_ATTACK]->setFrame(0.0f);
            i_this->mpFmBtk[TEXANM_ATTACK]->setFrame(0.0f);
            i_this->mMode = 1;
        }
        break;
    case 1:
        tame_eff_set(i_this);        

        if (anm_frame == TREG_S(7) + 88) {
            i_this->field_0x790 = 1;
            dComIfGp_getVibration().StartQuake(VIBMODE_Q_POWER3, 1, cXyz(0.0f, 1.0f, 0.0f));
            i_this->mSound.startCreatureSound(Z2SE_EN_FM_BLAST, 0, -1);
            i_this->mSound.startCreatureVoice(Z2SE_EN_FM_V_BLAST_GAOO, -1);
            
            if (dComIfGs_getArrowNum() <= 5 || dComIfGs_getLife() <= 4) {
                i_this->mDoCreateBa = TRUE;
            }
        }

        if (i_this->mpFmModelMorf->isStop()) {
            if (cM_rndF(1.0f) < 0.3f) {
                i_this->mAction = ACTION_ANIMAL;
            } else {
                i_this->mAction = ACTION_NORMAL;
            }

            i_this->mMode = 0;
            i_this->mPlayTexAnmNo = TEXANM_FM;
            i_this->mpFmBrk[TEXANM_FM]->setFrame(0.0f);
            i_this->mpFmBtk[TEXANM_FM]->setFrame(0.0f);
        }
        break;
    }

    cLib_addCalc0(&i_this->speedF, 1.0f, 1.0f);
}

static void animal_eff_set(e_fm_class* i_this, s16 param_1) {
    J3DModel* model = i_this->mpFmModelMorf->getModel();

    if (param_1 == 1) {
        for (int i = 0; i < 4; i++) {
            static u16 fire_name[] = {0x8181, 0x8182, 0x8183, 0x8184};
            i_this->field_0x1888[i] = dComIfGp_particle_set(i_this->field_0x1888[i], fire_name[i], &i_this->current.pos, NULL, NULL);
            
            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->field_0x1888[i]);
            if (emitter != NULL) {
                MTXCopy(model->getAnmMtx(3), *calc_mtx);
                MtxScale(1.0f, 1.0f, 1.0f, 1);
                emitter->setGlobalSRTMatrix(*calc_mtx);
            }
        }
    } else {
        i_this->field_0x1884 = dComIfGp_particle_set(i_this->field_0x1884, 0x814C, &i_this->current.pos, NULL, NULL);
        
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->field_0x1884);
        if (emitter != NULL) {
            MTXCopy(model->getAnmMtx(3), *calc_mtx);
            MtxScale(1.0f, 1.0f, 1.0f, 1);
            emitter->setGlobalSRTMatrix(*calc_mtx);
        }
    }
}

static void e_fm_animal(e_fm_class* i_this) {
    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_FM_ANIMAL02, 10.0f, 0, 1.0f);
        i_this->mMode = 1;
        break;
    case 1:
        if (i_this->mpFmModelMorf->isStop()) {
            i_this->mAction = ACTION_NORMAL;
            i_this->mMode = 0;
        }
        break;
    }

    animal_eff_set(i_this, 2);
    cLib_addCalc0(&i_this->speedF, 1.0f, 1.0f);
}

static void e_fm_fire(e_fm_class* i_this) {
    i_this->field_0x7c0 = 1;

    switch (i_this->mMode) {
    case 0:
        if ((int)i_this->mpFmBtk[0]->getFrame() == 0) {
            i_this->field_0x1830 = 0.0f;
            anm_init(i_this, BCK_FM_ANIMAL, 10.0f, 0, 1.0f);
            i_this->mMode = 1;
            i_this->mSound.startCreatureVoice(Z2SE_EN_FM_V_GAOO_SHORT, -1);
        }
        break;
    case 1:
        if ((int)i_this->mpFmModelMorf->getFrame() >= l_HIO.field_0x98 && (int)i_this->mpFmModelMorf->getFrame() <= l_HIO.field_0x9a) {
            i_this->mSound.startCreatureVoiceLevel(Z2SE_EN_FM_HOTAIR, -1);
            if ((int)i_this->mpFmModelMorf->getFrame() >= l_HIO.field_0x98 + 15) {
                i_this->field_0x1829 = 1;
                i_this->field_0x1828 = 2;
                i_this->field_0x7b6 = 3;
                i_this->field_0x5c8 = 4;
            }
        }

        if (i_this->mpFmModelMorf->isStop()) {
            i_this->mAction = ACTION_NORMAL;
            i_this->mMode = 0;
            i_this->mTimers[0] = 0;
            i_this->mPlayTexAnmNo = TEXANM_FM;
            i_this->mpFmBrk[TEXANM_FM]->setFrame(0.0f);
            i_this->mpFmBtk[TEXANM_FM]->setFrame(0.0f);
            i_this->field_0x7b6 = 0;
            i_this->field_0x5c8 = 2;
        }
        break;
    }

    if (i_this->mMode >= 1) {
        animal_eff_set(i_this, 1);
    }

    cLib_addCalc0(&i_this->speedF, 1.0f, 1.0f);
}

static void e_fm_stop(e_fm_class* i_this) {
    i_this->mDamageInvulnerabilityTimer = 6;

    switch (i_this->mMode) {
    case 0:
        i_this->mMode = 1;
        i_this->field_0x806 = 0;
        if (i_this->field_0x1b078 == 0) {
            i_this->mDemoCamMode = 1;
            i_this->mTimers[0] = 40;
            dComIfGp_getVibration().StartQuake(VIBMODE_Q_POWER5, 1, cXyz(0.0f, 1.0f, 0.0f));
        } else {
            i_this->mTimers[0] = 20;
        }
        /* fallthrough */
    case 1:
        if (i_this->mTimers[0] == 35) {
            dComIfGp_getVibration().StartQuake(VIBMODE_Q_POWER3, 1, cXyz(0.0f, 1.0f, 0.0f));
        }

        f32 anm_speed = (0.7f + JREG_F(16)) * cM_ssin(i_this->field_0x806);
        i_this->field_0x806 += JREG_S(7) + 3000;
        f32 temp_f31 = 0.5f + TREG_F(13);

        i_this->mChain[i_this->field_0x808].field_0x6178 = temp_f31 + (temp_f31 * cM_ssin((i_this->field_0x806 + TREG_S(0)) - 20000));
        i_this->mpFmModelMorf->setPlaySpeed(anm_speed);

        if (i_this->mTimers[0] <= 10) {
            i_this->field_0x7c0 = 2;
            if (i_this->mTimers[0] == 10) {
                i_this->mSound.startCreatureVoice(Z2SE_EN_FM_V_FIND, -1);
            }
        }

        if (i_this->mTimers[0] == 0) {
            dComIfGp_getVibration().StopQuake(0x1F);
            i_this->mAction = ACTION_DOWN;
            i_this->mMode = 0;
            i_this->health = 50;
            i_this->mDownCnt++;
            i_this->mChain[i_this->field_0x808].field_0x6178 = 0.0f;

            if (i_this->field_0x1b078 == 0) {
                i_this->mDemoCamMode = 3;
                i_this->mDemoCamTimer = 0;
                daPy_getPlayerActorClass()->changeDemoMode(1, 0, 0, 0);
            }
        }
        break;
    }

    cLib_addCalc0(&i_this->speedF, 1.0f, 1.0f);
}

static void e_fm_damage_run(e_fm_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp78;
    cXyz sp6C;
    f32 move_speed = 0.0f;
    int anm_frame = (int)i_this->mpFmModelMorf->getFrame();

    i_this->mDamageInvulnerabilityTimer = 10;

    switch (i_this->mMode) {
    case 0:
        if (i_this->mpFmModelMorf->isStop()) {
            i_this->mTimers[0] = l_HIO.field_0xa4 + cM_rndF(50.0f);
            anm_init(i_this, BCK_FM_DAMAGEWALK, 5.0f, 2, 2.0f);
            i_this->mMode = 1;
            Z2GetAudioMgr()->changeBgmStatus(3);
        }
        break;
    case 1:
        for (int i = 0; i <= 1; i++) {
            int sp20 = 1 - i;
            if (fabsf(i_this->mFootPos[i].y - i_this->current.pos.y) < 15.0f && fabsf(i_this->mFootPos[sp20].y - i_this->current.pos.y) >= 15.0f) {
                move_speed = l_HIO.field_0x18;
                i_this->field_0x7e0 = sp20;
            }
        }

        if (i_this->speedF < 1.0f && move_speed > 1.0f) {
            i_this->mSound.startCreatureSound(Z2SE_EN_FM_CHAIN_FOOTNOTE, 0, -1);

            if (i_this->mTimers[1] == 0) {
                i_this->mTimers[1] = 30.0f + cM_rndF(30.0f);

                s8 sp8 = 0;
                for (int i = 2; i < 4; i++) {
                    if (i_this->mChain[i].field_0x617e != 0 || (i_this->mChain[i].field_0x617c != 0 && daPy_getPlayerActorClass()->checkEquipHeavyBoots())) {
                        sp78 = i_this->mChain[i].field_0x6180;
                        sp78.y = 0.0f;

                        if (sp78.abs() < 1000.0f + KREG_F(12)) {
                            sp78 = i_this->current.pos - i_this->mChain[i].field_0x6180;
                        } else {
                            sp78 = i_this->home.pos - i_this->mChain[i].field_0x6180;
                        }

                        i_this->field_0x5c4 = cM_atan2s(sp78.x, sp78.z) + (s16)cM_rndFX(10000.0f);
                        sp8 = 1;
                        break;
                    }
                }

                if (sp8 == 0) {
                    sp6C.x = i_this->home.pos.x + cM_rndFX(300.0f);
                    sp6C.y = i_this->home.pos.y;
                    sp6C.z = i_this->home.pos.z + cM_rndFX(300.0f);
                    sp78 = sp6C - i_this->current.pos;

                    s16 spC = cM_atan2s(sp78.x, sp78.z) - i_this->current.angle.y;
                    if (spC > 0x4000) {
                        spC = 0x4000;
                    } else if (spC < -0x4000) {
                        spC = -0x4000;
                    }
                    i_this->field_0x5c4 = i_this->current.angle.y + spC;
                }
            }
        }

        if (i_this->speedF > 1.0f && move_speed < 1.0f) {
            dComIfGp_getVibration().StartShock(YREG_S(3) + (int)VIBMODE_S_POWER7, 0xF, cXyz(0.0f, 1.0f, 0.0f));
            dComIfGp_particle_set(0x8150, &i_this->home.pos, NULL, NULL);
            mDoAud_seStart(Z2SE_OBJ_FM_DIRTFALL, NULL, 0, 0);
            i_this->mSound.startCreatureSound(Z2SE_EN_FM_FOOTNOTE, 0, -1);
            hasira_hahen_hit(&i_this->mFootPos[i_this->field_0x7e0], 100.0f + VREG_F(4), 20.0f + VREG_F(5), -1);
        }

        for (int i = 2; i <= 3; i++) {
            sp78 = i_this->mChain[i].field_0x6180 - i_this->current.pos;

            s16 spA = i_this->shape_angle.y - cM_atan2s(sp78.x, sp78.z);
            if ((spA > 0x6000 || spA < -0x6000) && (i_this->mChain[i].field_0x617e != 0 || (i_this->mChain[i].field_0x617c != 0 && daPy_getPlayerActorClass()->checkEquipHeavyBoots()))) {
                f32 var_f30;
                if (i_this->mChain[i].field_0x617c != 0) {
                    var_f30 = 300.0f + VREG_F(10);
                } else {
                    var_f30 = 100.0f + VREG_F(11);
                }

                if (i_this->mChain[i].field_0x6174 > var_f30 && ((i == 2 && (anm_frame & 252) == 12) || (i == 3 && (anm_frame & 252) == 40))) {
                    i_this->mAction = ACTION_STOP;
                    i_this->mMode = 0;
                    i_this->field_0x808 = i;
                    return;
                }
            }
        }

        if (((i_this->mTimers[0] < l_HIO.field_0xa4 - 250.0f) && i_this->field_0x809 != 0) || i_this->mTimers[0] == 0) {
            anm_init(i_this, BCK_FM_ANIMAL02, 10.0f, 0, 1.0f);
            i_this->mMode = 2;
            i_this->mPlayTexAnmNo = TEXANM_ANIMAL;
            i_this->mpFmBrk[TEXANM_ANIMAL]->setFrame(0.0f);
            i_this->mpFmBtk[TEXANM_ANIMAL]->setFrame(0.0f);
        }
        break;
    case 2:
        if (i_this->mpFmModelMorf->isStop()) {
            i_this->mAction = ACTION_NORMAL;
            i_this->mMode = 0;
            i_this->mPlayTexAnmNo = TEXANM_FM;
            i_this->mpFmBrk[TEXANM_FM]->setFrame(0.0f);
            i_this->mpFmBtk[TEXANM_FM]->setFrame(0.0f);
            Z2GetAudioMgr()->changeBgmStatus(4);
        }
    }

    i_this->speedF = move_speed;
    if (move_speed > 1.0f) {
        cLib_addCalcAngleS2(&i_this->current.angle.y, i_this->field_0x5c4, 2, KREG_S(2) + 0x200);
    }
}

static void cam_3d_morf(e_fm_class* i_this, f32 param_1) {
    cLib_addCalc2(&i_this->mDemoCamCenter.x, i_this->mDemoCamCenterTarget.x, param_1, i_this->mDemoCamCenterSpd.x * i_this->field_0x1b050);
    cLib_addCalc2(&i_this->mDemoCamCenter.y, i_this->mDemoCamCenterTarget.y, param_1, i_this->mDemoCamCenterSpd.y * i_this->field_0x1b050);
    cLib_addCalc2(&i_this->mDemoCamCenter.z, i_this->mDemoCamCenterTarget.z, param_1, i_this->mDemoCamCenterSpd.z * i_this->field_0x1b050);
    cLib_addCalc2(&i_this->mDemoCamEye.x, i_this->mDemoCamEyeTarget.x, param_1, i_this->mDemoCamEyeSpd.x * i_this->field_0x1b050);
    cLib_addCalc2(&i_this->mDemoCamEye.y, i_this->mDemoCamEyeTarget.y, param_1, i_this->mDemoCamEyeSpd.y * i_this->field_0x1b050);
    cLib_addCalc2(&i_this->mDemoCamEye.z, i_this->mDemoCamEyeTarget.z, param_1, i_this->mDemoCamEyeSpd.z * i_this->field_0x1b050);
}

static int demo_stop;

static void demo_camera(e_fm_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* camera0 = dComIfGp_getCamera(0);

    cXyz spEC;
    cXyz spE0;
    cXyz spD4;
    cXyz spC8;

    switch (i_this->mDemoCamMode) {
    case 0:
        break;
    case 1:
        if (!actor->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
            actor->eventInfo.onCondition(2);
            return;
        }

        camera->mCamera.Stop();
        i_this->mDemoCamMode = 2;
        i_this->mDemoCamTimer = 0;
        daPy_getPlayerActorClass()->changeOriginalDemo();

        if ((i_this->mChain[2].field_0x617c != 0 || i_this->mChain[3].field_0x617c != 0) && daPy_getPlayerActorClass()->checkEquipHeavyBoots()) {
            daPy_getPlayerActorClass()->changeDemoMode(0x43, 0, 0, 0);
        } else {
            daPy_getPlayerActorClass()->changeDemoMode(1, 0, 0, 0);
        }

        i_this->mDemoCamFovy = 55.0f + NREG_F(10);
        camera->mCamera.SetTrimSize(3);
    case 2:
        if (i_this->mDemoCamTimer == 10) {
            mDoAud_seStart(Z2SE_EN_FM_CHAIN_TENSION, &i_this->mChain[i_this->field_0x808].sound_pos, 0, 0);
        }

        spEC = i_this->mChain[i_this->field_0x808].field_0x68 - i_this->mChain[i_this->field_0x808].field_0x6180;
        cMtx_YrotS(*calc_mtx, cM_atan2s(spEC.x, spEC.z));

        if (i_this->field_0x808 == 2) {
            spEC.x = -(100.0f + NREG_F(11));
        } else {
            spEC.x = (100.0f + NREG_F(11));
        }
        spEC.y = 50.0f + NREG_F(12);
        spEC.z = NREG_F(13) - 200.0f;
        MtxPosition(&spEC, &i_this->mDemoCamEye);
        i_this->mDemoCamEye += i_this->mChain[i_this->field_0x808].field_0x6180;

        if (i_this->field_0x808 == 2) {
            spEC.x = 200.0f + NREG_F(14);
        } else {
            spEC.x = -(200.0f + NREG_F(14));
        }
        spEC.y = -100.0f + NREG_F(15);
        spEC.z = NREG_F(16);
        MtxPosition(&spEC, &i_this->mDemoCamCenter);
        i_this->mDemoCamCenter += i_this->mChain[i_this->field_0x808].field_0x68;
        break;
    case 3:
        if (actor->current.pos.abs() > 700.0f + BREG_F(18)) {
            spEC = actor->current.pos;
            cMtx_YrotS(*calc_mtx, cM_atan2s(spEC.x, spEC.z));
            spEC.x = 0.0f;
            spEC.y = 0.0f;
            spEC.z = 700.0f + BREG_F(18);
            MtxPosition(&spEC, &spE0);
            actor->current.pos.x = spE0.x;
            actor->current.pos.z = spE0.z;
            OS_REPORT(" FM DOWN POS REVISE \n");
        }

        cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
        if (i_this->field_0x808 == 2) {
            spEC.x = 400.0f + BREG_F(1);
        } else {
            spEC.x = -(400.0f + BREG_F(0));
        }
        spEC.y = 400.0f + BREG_F(2);
        spEC.z = 300.0f + BREG_F(3);
        MtxPosition(&spEC, &i_this->mDemoCamEye);
        i_this->mDemoCamEye += actor->current.pos;

        spEC.x = BREG_F(4);
        spEC.y = 500.0f + BREG_F(5);
        spEC.z = BREG_F(6);
        MtxPosition(&spEC, &i_this->mDemoCamCenter);
        i_this->mDemoCamCenter += actor->current.pos;
        break;
    case 4:
        cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
        spEC.x = NREG_F(0);
        spEC.y = 300.0f + NREG_F(1);
        spEC.z = 200.0f + NREG_F(2);
        MtxPosition(&spEC, &i_this->mDemoCamCenter);
        i_this->mDemoCamCenter += actor->current.pos;
        i_this->mDemoCamMode++;
        /* fallthrough */
    case 5:
        cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
        if (i_this->field_0x808 == 2) {
            spEC.x = 100.0f + NREG_F(5);
            spEC.z = 100.0f + (1000.0f + NREG_F(6));
        } else {
            spEC.x = -(550.0f + NREG_F(3));
            spEC.z = 800.0f + NREG_F(4);
        }
        spEC.y = 100.0f + NREG_F(7);
        MtxPosition(&spEC, &i_this->mDemoCamEye);
        i_this->mDemoCamEye += actor->current.pos;

        if (i_this->mDemoCamTimer > (s16)(NREG_S(0) + 34)) {
            cLib_addCalc2(&i_this->mDemoCamCenter.y, 100.0f + NREG_F(8), 0.5f, 20.0f + NREG_F(9));
        }
        break;
    case 10:
        if (!actor->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
            actor->eventInfo.onCondition(2);
            return;
        }

        camera->mCamera.Stop();
        i_this->mDemoCamMode = 11;
        i_this->mDemoCamTimer = 0;
        daPy_getPlayerActorClass()->changeOriginalDemo();

        i_this->mDemoCamFovy = 50.0f;
        i_this->mDemoCamEye = camera0->lookat.eye;
        i_this->mDemoCamCenter = camera0->lookat.center;
        i_this->field_0x778 = 0.0f;
        camera->mCamera.SetTrimSize(3);
        dComIfGp_getEvent()->startCheckSkipEdge(actor);
    case 11:
        if (i_this->mDemoCamTimer == (s16)(VREG_S(0) + 8)) {
            daPy_getPlayerActorClass()->changeDemoMode(0x19, 0, 0, 0);
        }

        if (i_this->mDemoCamTimer >= 60) {
            if (i_this->mDemoCamTimer == 60) {
                i_this->mDemoCamCenter.set(0.0f, 176.0f, 1782.0f);
                i_this->mDemoCamEye.set(0.0f, 217.0f, 1968.0f);
                i_this->mDemoCamCenterTarget.set(0.0f, 671.0f, 1664.0f);
                i_this->mDemoCamEyeTarget.set(0.0f, 752.0f, 1836.0f);

                i_this->mDemoCamEyeSpd.x = fabsf(i_this->mDemoCamEyeTarget.x - i_this->mDemoCamEye.x);
                i_this->mDemoCamEyeSpd.y = fabsf(i_this->mDemoCamEyeTarget.y - i_this->mDemoCamEye.y);
                i_this->mDemoCamEyeSpd.z = fabsf(i_this->mDemoCamEyeTarget.z - i_this->mDemoCamEye.z);

                i_this->mDemoCamCenterSpd.x = fabsf(i_this->mDemoCamCenterTarget.x - i_this->mDemoCamCenter.x);
                i_this->mDemoCamCenterSpd.y = fabsf(i_this->mDemoCamCenterTarget.y - i_this->mDemoCamCenter.y);
                i_this->mDemoCamCenterSpd.z = fabsf(i_this->mDemoCamCenterTarget.z - i_this->mDemoCamCenter.z);

                i_this->field_0x1b050 = 0.0f;
                i_this->mDemoCamFovy = 55.0f + NREG_F(10);
            }

            if (i_this->mDemoCamTimer == 120) {
                player->changeDemoMode(2, 1, 0, 0);
                spE0.set(0.0f, 0.0f, 600.0f);
                player->changeDemoPos0(&spE0);
            }

            if (i_this->mDemoCamTimer >= 160) {
                cam_3d_morf(i_this, 0.1f);
                cLib_addCalc2(&i_this->field_0x1b050, 0.008f, 1.0f, 0.0002f);
            }

            if (i_this->mDemoCamTimer == 340) {
                i_this->mDemoCamMode = 12;
                i_this->mDemoCamTimer = 0;

                i_this->mDemoCamCenter.set(0.0f, 140.0f, 669.0f);
                i_this->mDemoCamEye.set(0.0f, 163.0f, 857.0f);
                i_this->mDemoCamCenterTarget.set(0.0f, 211.0f, 673.0f);
                i_this->mDemoCamEyeTarget.set(0.0f, 163.0f, 857.0f);

                i_this->mDemoCamEyeSpd.x = fabsf(i_this->mDemoCamEyeTarget.x - i_this->mDemoCamEye.x);
                i_this->mDemoCamEyeSpd.y = fabsf(i_this->mDemoCamEyeTarget.y - i_this->mDemoCamEye.y);
                i_this->mDemoCamEyeSpd.z = fabsf(i_this->mDemoCamEyeTarget.z - i_this->mDemoCamEye.z);

                i_this->mDemoCamCenterSpd.x = fabsf(i_this->mDemoCamCenterTarget.x - i_this->mDemoCamCenter.x);
                i_this->mDemoCamCenterSpd.y = fabsf(i_this->mDemoCamCenterTarget.y - i_this->mDemoCamCenter.y);
                i_this->mDemoCamCenterSpd.z = fabsf(i_this->mDemoCamCenterTarget.z - i_this->mDemoCamCenter.z);

                i_this->field_0x1b050 = 0.0f;
            }
        }
        break;
    case 12:
        if (i_this->mDemoCamTimer >= 50) {
            cam_3d_morf(i_this, 0.1f);
            cLib_addCalc2(&i_this->field_0x1b050, 0.01f, 1.0f, 0.0005f);
        }

        if (i_this->mDemoCamTimer == 250) {
            i_this->mDemoCamMode = 13;
            i_this->mDemoCamTimer = 0;
            i_this->mDemoCamCenter.set(2.8f, 496.0f, 162.0f);
            i_this->mDemoCamEye.set(9.7f, 362.0f, 296.0f);
            Z2GetAudioMgr()->subBgmStart(Z2BGM_BOSSFIREMAN_1);
        }
        break;
    case 13:
        if (i_this->mDemoCamTimer == 90) {
            i_this->mpCoreBtk->setPlaySpeed(1.0f);
            i_this->field_0x770 = 10000;
        }

        if (i_this->mDemoCamTimer == 100) {
            i_this->mSound.startCreatureSound(Z2SE_EN_FM_CORE_EYEOPEN, 0, -1);
        }

        if (i_this->mDemoCamTimer >= 190) {
            cLib_addCalc2(&i_this->field_0x778, 1.0f, 1.0f, 0.05f);
        }

        if (i_this->mDemoCamTimer != 260) {
            break;
        }

        i_this->mDemoCamMode = 14;
        i_this->mDemoCamTimer = 0;
    case 14:
        cMtx_YrotS(*calc_mtx, actor->current.angle.y);
        spEC.x = -200.0f + ZREG_F(1);
        spEC.y = 100.0f + ZREG_F(2);
        spEC.z = 900.0f + ZREG_F(3);
        MtxPosition(&spEC, &i_this->mDemoCamEye);
        i_this->mDemoCamEye += actor->current.pos;

        spEC.x = ZREG_F(5);
        spEC.y = -100.0f + ZREG_F(6);
        spEC.z = ZREG_F(7);
        MtxPosition(&spEC, &spC8);
        spC8 += actor->eyePos;

        if (i_this->mDemoCamTimer == 0) {
            i_this->mDemoCamCenter = spC8;
        } else {
            cLib_addCalc2(&i_this->mDemoCamCenter.x, spC8.x, 0.1f, 50.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.y, spC8.y, 0.1f, 50.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.z, spC8.z, 0.1f, 50.0f);
        }

        if (i_this->mDemoCamTimer == 60) {
            anm_init(i_this, BCK_FM_OPDEMO, 10.0f, 0, 1.0f);
            i_this->mSound.startCreatureVoice(Z2SE_EN_FM_V_OPDEMO, -1);
            i_this->mSound.startCreatureSound(Z2SE_EN_FM_OPDEMO, 0, -1);

            i_this->mMode = 2;
            i_this->mPlayTexAnmNo = TEXANM_OP_DEMO;
            i_this->mpFmBrk[TEXANM_OP_DEMO]->setFrame(0.0f);
            i_this->mpFmBtk[TEXANM_OP_DEMO]->setFrame(0.0f);
        }

        if (i_this->mDemoCamTimer == 420) {
            daPy_getPlayerActorClass()->changeDemoMode(0x17, 1, 2, 0);
        }

        if (i_this->mDemoCamTimer == 163 || i_this->mDemoCamTimer == 203 || i_this->mDemoCamTimer == 793) {
            dComIfGp_getVibration().StartQuake(VIBMODE_Q_POWER3, 1, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (i_this->mDemoCamTimer == 195 || i_this->mDemoCamTimer == 257 || i_this->mDemoCamTimer == 882) {
            dComIfGp_getVibration().StopQuake(0x1F);
        }

        if (i_this->mDemoCamTimer == 804) {
            daPy_getPlayerActorClass()->changeDemoMode(0x22, 0, 0, 0);
        }

        if (i_this->mDemoCamTimer == 404) {
            dComIfGp_getVibration().StartShock(VIBMODE_S_POWER5, 1, cXyz(0.0f, 1.0f, 0.0f));
        }
        break;
    case 20:
        if (!actor->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
            actor->eventInfo.onCondition(2);
            return;
        }

        camera->mCamera.Stop();
        i_this->mDemoCamMode = 21;
        i_this->mDemoCamTimer = 0;
        daPy_getPlayerActorClass()->changeOriginalDemo();
        daPy_getPlayerActorClass()->changeDemoMode(1, 0, 0, 0);
        i_this->mDemoCamFovy = 55.0f + NREG_F(10);
        i_this->field_0x1b050 = 0.0f;
        camera->mCamera.SetTrimSize(3);
    case 21:
        if (actor->current.pos.abs() > 700.0f + BREG_F(18)) {
            spEC = actor->current.pos;
            cMtx_YrotS(*calc_mtx, cM_atan2s(spEC.x, spEC.z));
            spEC.x = 0.0f;
            spEC.y = 0.0f;
            spEC.z = 700.0f + BREG_F(18);
            MtxPosition(&spEC, &spE0);
            actor->current.pos.x = spE0.x;
            actor->current.pos.z = spE0.z;
            OS_REPORT(" FM DOWN POS REVISE \n");
        }
        
        cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
        spEC.x = 100.0f + NREG_F(15);
        spEC.y = 50.0f + NREG_F(11);
        spEC.z = 1000.0f + NREG_F(12);
        MtxPosition(&spEC, &i_this->mDemoCamEye);
        i_this->mDemoCamEye += actor->current.pos;
        spC8 = actor->eyePos;

        if (i_this->mDemoCamTimer < 110) {
            if (i_this->mDemoCamTimer == 0) {
                i_this->mDemoCamCenter = spC8;
                i_this->mDemoCamCenter.y -= 300.0f + NREG_F(16);
            } else {
                cLib_addCalc2(&i_this->mDemoCamCenter.y, (spC8.y - 300.0f) + NREG_F(16), 0.1f, 5.0f);
                cLib_addCalc2(&i_this->mDemoCamCenter.x, spC8.x, 0.1f, 5.0f);
                cLib_addCalc2(&i_this->mDemoCamCenter.z, spC8.z, 0.1f, 5.0f);
            }
        } else {
            cLib_addCalc2(&i_this->mDemoCamCenter.x, spC8.x, 0.2f, 100.0f * i_this->field_0x1b050);
            cLib_addCalc2(&i_this->mDemoCamCenter.y, spC8.y, 0.2f, 200.0f * i_this->field_0x1b050);
            cLib_addCalc2(&i_this->mDemoCamCenter.z, spC8.z, 0.2f, 100.0f * i_this->field_0x1b050);
            cLib_addCalc2(&i_this->field_0x1b050, 1.0f, 1.0f, 0.1f);
        }
        break;
    case 50:
        if (!actor->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
            actor->eventInfo.onCondition(2);
            return;
        }

        camera->mCamera.Stop();
        i_this->mDemoCamMode = 51;
        i_this->mDemoCamTimer = 0;
        daPy_getPlayerActorClass()->changeOriginalDemo();
        i_this->mDemoCamFovy = 55.0f + NREG_F(10);
        i_this->field_0x770 = 0;
        i_this->field_0x1aff6 = 0;
        i_this->field_0x1b050 = i_this->field_0x1b054 = i_this->field_0x1b068 = i_this->field_0x1b06c = i_this->field_0x1b074 = i_this->field_0x1b070 = 0.0f;

        Z2GetAudioMgr()->bgmStreamPrepare(0x200001E);
        Z2GetAudioMgr()->bgmStreamPlay();
        daPy_getPlayerActorClass()->changeDemoMode(0x17, 1, 2, 0);
        camera->mCamera.SetTrimSize(3);
    case 51:
        actor->shape_angle.y = actor->current.angle.y = actor->home.angle.y;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < i_this->mChain[i].field_0x61d0 - 1; j++) {
                i_this->mChain[i].pos[j].x -= actor->current.pos.x;
                i_this->mChain[i].pos[j].z -= actor->current.pos.z;
                i_this->mChain[i].field_0x1ac[j] = i_this->mChain[i].pos[j];
            }
        }

        actor->current.pos.x = 0.0f;
        actor->current.pos.z = 0.0f;

        cMtx_YrotS(*calc_mtx, actor->current.angle.y);
        spEC.x = 0.0f;
        spEC.y = 0.0f;
        spEC.z = (700.0f + ZREG_F(0));
        MtxPosition(&spEC, &spE0);
        spE0 += actor->current.pos;
        player->setPlayerPosAndAngle(&spE0, actor->home.angle.y + 0x8000, 0);

        if (i_this->mDemoCamTimer < XREG_S(0) + 420) {
            cMtx_YrotS(*calc_mtx, actor->current.angle.y);
            spEC.x = (200.0f + ZREG_F(1));
            spEC.y = (100.0f + ZREG_F(2));
            spEC.z = (900.0f + ZREG_F(3));
            MtxPosition(&spEC, &i_this->mDemoCamEye);
            i_this->mDemoCamEye += actor->current.pos;

            spEC.x = ZREG_F(5);
            spEC.y = i_this->field_0x1b074;
            spEC.z = ZREG_F(7);
            MtxPosition(&spEC, &i_this->field_0x1b040);

            if (i_this->mDemoCamTimer == 0) {
                i_this->field_0x1b040 += actor->current.pos;
                i_this->field_0x1b040.z += 600.0f;
            } else {
                i_this->field_0x1b040 += actor->eyePos;
            }
            spC8 = i_this->field_0x1b040;

            if (i_this->mDemoCamTimer > XREG_S(1) + 220) {
                cLib_addCalc2(&i_this->field_0x1b074, -100.0f, 1.0f, 5.0f);
            }
        } else {
            cMtx_YrotS(*calc_mtx, (actor->current.angle.y + i_this->field_0x1aff6));
            spEC.x = (200.0f + ZREG_F(1));
            spEC.y = (100.0f + ZREG_F(2) + i_this->field_0x1b068);
            spEC.z = (900.0f + ZREG_F(3) + i_this->field_0x1b06c);
            MtxPosition(&spEC, &i_this->mDemoCamEye);
            i_this->mDemoCamEye += actor->current.pos;

            spEC.x = ZREG_F(5) + i_this->field_0x1b070;
            spEC.y = -100.0f + ZREG_F(6);
            spEC.z = ZREG_F(7);
            MtxPosition(&spEC, &spC8);
            spC8 += i_this->field_0x1b040;

            cLib_addCalcAngleS2(&i_this->field_0x1aff6, 20000, 8, 200.0f * i_this->field_0x1b054);
            cLib_addCalc2(&i_this->field_0x1b054, 1.0f, 1.0f, 0.05f);

            if (i_this->mDemoCamTimer > (s16)(XREG_S(2) + 470)) {
                cLib_addCalc2(&i_this->field_0x1b068, 800.0f, 0.125f, 20.0f * i_this->field_0x1b050);
                cLib_addCalc2(&i_this->field_0x1b070, -100.0f, 0.125f, 12.5f * i_this->field_0x1b050);
                cLib_addCalc2(&i_this->field_0x1b06c, -400.0f, 0.125f, 10.0f * i_this->field_0x1b050);
                cLib_addCalc2(&i_this->field_0x1b050, 1.0f, 1.0f, 0.02f);
            }
        }

        spC8.x *= 0.5f;
        if (i_this->mDemoCamTimer == 0) {
            i_this->mDemoCamCenter = spC8;
        } else {
            cLib_addCalc2(&i_this->mDemoCamCenter.x, spC8.x, 0.1f, 50.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.y, spC8.y, 0.1f, 50.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.z, spC8.z, 0.1f, 50.0f);
        }

        if (i_this->mDemoCamTimer == XREG_S(3) + 640) {
            i_this->field_0x770 = 0;
        }

        if (i_this->mDemoCamTimer == XREG_S(3) + 668) {
            i_this->field_0x182c = 2;
            i_this->mHideCore = TRUE;
        }

        if (i_this->mDemoCamTimer == XREG_S(5) + 537) {
            i_this->field_0x5c8 = 3;
            i_this->mKankyoBlend = 1.0f;
        }

        if (i_this->mDemoCamTimer == XREG_S(3) + XREG_S(4) + 678) {
            demo_stop = 1;
            i_this->mDemoCamTimer = 0;
            i_this->mDemoCamMode = 52;
        }
        break;
    case 52:
        cMtx_YrotS(*calc_mtx, actor->current.angle.y);
        spEC.x = 100.0f + ZREG_F(1);
        spEC.y = 100.0f + ZREG_F(2);
        spEC.z = 900.0f + ZREG_F(3);
        MtxPosition(&spEC, &i_this->mDemoCamEye);
        i_this->mDemoCamEye += actor->current.pos;

        spEC.x = ZREG_F(5);
        spEC.y = 370.0f + ZREG_F(6);
        spEC.z = ZREG_F(7);
        MtxPosition(&spEC, &i_this->mDemoCamCenter);
        i_this->mDemoCamCenter += actor->current.pos;

        if (i_this->mDemoCamTimer == XREG_S(6) + 85) {
            anm_init(i_this, BCK_FM_DEMOEND02, 1.0f, 0, 1.0f);
            i_this->mPlayTexAnmNo = TEXANM_DEMO_END02;
            i_this->mpFmBrk[TEXANM_DEMO_END02]->setFrame(0.0f);
            i_this->mpFmBtk[TEXANM_DEMO_END02]->setFrame(0.0f);
            i_this->mDemoCamMode = 53;
            i_this->mDemoCamTimer = 0;
        }
        break;
    case 53: {
        if (i_this->mDemoCamTimer == AREG_S(9) + 97) {
            player->changeDemoMode(0x1E, 0, 0, 0);
        }

        if (i_this->mDemoCamTimer == XREG_S(7) + 86) {
            i_this->mSound.startCreatureSound(Z2SE_EN_BOSS_EXPLODE, 0, 0);
            i_this->mDrawDemoModel = TRUE;
            i_this->mDemoModelSize = 1.0f;

            for (int i = 0; i < 16; i++) {
                static u16 g_c_i[] = {
                    0x8481, 0x8482, 0x8483, 0x8484, 0x8485, 0x8486, 0x8487, 0x8488,
                    0x8489, 0x848A, 0x848B, 0x848C, 0x848D, 0x848E, 0x848F, 0x8490,
                };

                JPABaseEmitter* emitter = dComIfGp_particle_set(g_c_i[i], &actor->current.pos, NULL, NULL);
                if (emitter != NULL) {
                    static int g_c_ji[] = {6, 13, 8, 5, 3, 21, 24, 19, 6, 7, 13, 14, 3, 21, 24, 19};
                    emitter->setGlobalRTMatrix(i_this->mpFmModelMorf->getModel()->getAnmMtx(g_c_ji[i]));
                }
            }

            i_this->mShadowKey = 353535;
        }

        if (i_this->mDemoCamTimer > XREG_S(8) + 14) {
            cLib_addCalc2(&i_this->mDemoCamFovy, 85.0f, 0.05f, 0.05f + JREG_F(16));
        }

        if (i_this->mDemoCamTimer != XREG_S(7) + 206) {
            break;
        }

        i_this->mDemoCamCenter.set(207.0f, 615.0f, -343.0f);
        i_this->mDemoCamEye.set(813.0f, 942.0f, -1001.0f);
        i_this->mDemoCamCenterTarget.set(223.0f, 474.0f, 162.0f);
        i_this->mDemoCamEyeTarget.set(636.0f, 802.0f, -630.0f);

        i_this->mDemoCamEyeSpd.x = fabsf(i_this->mDemoCamEyeTarget.x - i_this->mDemoCamEye.x);
        i_this->mDemoCamEyeSpd.y = fabsf(i_this->mDemoCamEyeTarget.y - i_this->mDemoCamEye.y);
        i_this->mDemoCamEyeSpd.z = fabsf(i_this->mDemoCamEyeTarget.z - i_this->mDemoCamEye.z);

        i_this->mDemoCamCenterSpd.x = fabsf(i_this->mDemoCamCenterTarget.x - i_this->mDemoCamCenter.x);
        i_this->mDemoCamCenterSpd.y = fabsf(i_this->mDemoCamCenterTarget.y - i_this->mDemoCamCenter.y);
        i_this->mDemoCamCenterSpd.z = fabsf(i_this->mDemoCamCenterTarget.z - i_this->mDemoCamCenter.z);

        i_this->field_0x1b050 = 0.0f;
        i_this->mDemoCamMode = 54;
        i_this->mDemoCamTimer = 0;

        cXyz spBC(0.0f, 0.0f, 0.0f);
        for (int i = 0; i < 4; i++) {
            static u16 g_e_i[] = {0x847B, 0x847C, 0x847D, 0x847E};
            dComIfGp_particle_set(g_e_i[i], &spBC, NULL, NULL);
        }

        i_this->mDemoCamFovy = 55.0f + NREG_F(10);
        i_this->mSound.startCreatureSound(Z2SE_EN_BOSS_CONVERGE, 0, 0);
        daPy_getPlayerActorClass()->changeDemoMode(1, 0, 0, 0);
    }
    case 54: {
        if (i_this->mDemoCamTimer >= VREG_S(0) + 100) {
            cam_3d_morf(i_this, 0.2f);
            cLib_addCalc2(&i_this->field_0x1b050, 0.15f, 1.0f, 0.002f);
        }

        if (i_this->mDemoCamTimer == VREG_S(1) + 100) {
            cMtx_YrotS(*calc_mtx, player->shape_angle.y);
            spEC.set(0.0f, 300.0f + JREG_F(8), 100.0f + JREG_F(9));
            MtxPosition(&spEC, &spE0);
            spE0 += player->current.pos;

            OS_REPORT("///YSTONE POS  %d,%d,%d\n", (int)spE0.x, (int)spE0.y, (int)spE0.z);
            fopAcM_create(PROC_OBJ_YSTONE, 0, &spE0, fopAcM_GetRoomNo(actor), NULL, NULL, -1);
        }

        if (i_this->mDemoCamTimer == VREG_S(1) + 120) {
            obj_ystone_class* ystone = (obj_ystone_class*)fopAcM_SearchByName(PROC_OBJ_YSTONE);
            if (ystone != NULL) {
                ystone->field_0x59b = 0;
            }
        }

        if (i_this->mDemoCamTimer == VREG_S(2) + 180) {
            i_this->mDemoCamMode = 55;
            i_this->mDemoCamTimer = 0;
            i_this->field_0x1b05c = 300.0f + JREG_F(8);
            i_this->field_0x79c = 70.0f + DREG_F(7);
            i_this->mTimers[3] = 100;
            actor->current.pos.z -= 300.0f;
        }
        break;
    }
    case 55: {
        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        spEC.x = ZREG_F(1);
        spEC.y = 100.0f + ZREG_F(2);
        spEC.z = -200.0f + ZREG_F(3);
        MtxPosition(&spEC, &i_this->mDemoCamEye);
        i_this->mDemoCamEye += player->current.pos;
        i_this->mDemoCamCenter = player->current.pos;
        i_this->mDemoCamCenter.y += 200.0f + ZREG_F(4);

        spEC.set(0.0f, i_this->field_0x1b05c, 100.0f + JREG_F(9));
        MtxPosition(&spEC, &spE0);
        spE0 += player->current.pos;

        cLib_addCalc2(&i_this->field_0x1b05c, 210.0f, 0.05f, 0.5f);

        obj_ystone_class* ystone = (obj_ystone_class*)fopAcM_SearchByName(PROC_OBJ_YSTONE);
        if (ystone != NULL) {
            ystone->setCurrentPos(spE0);
        }
    
        if (i_this->mDemoCamTimer == VREG_S(3) + 104) {
            cXyz warp_pos(-363.0f, 0.0f,291.0f);
            csXyz warp_rot(0, 0, 0);
            fopAcM_createWarpHole(&warp_pos, &warp_rot, fopAcM_GetRoomNo(actor), 0, 1, 0xFF);

            spEC = actor->current.pos;
            fopAcM_create(PROC_NPC_GRZ, 0xFFFFFF01, &spEC, fopAcM_GetRoomNo(actor), &actor->shape_angle, NULL, -1);
            i_this->mDrawDemoModel = FALSE;
        }

        if (i_this->mDemoCamTimer == VREG_S(3) + 110) {
            i_this->mDemoCamMode = 100;
            dComIfGs_onStageBossEnemy();
            /* dSv_event_flag_c::M_031 - Goron Mines - Goron Mines clear */
            dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[64]);
        }
        break;
    }
    case 100:
        camera->mCamera.Reset(i_this->mDemoCamCenter, i_this->mDemoCamEye);
        camera->mCamera.Start();
        camera->mCamera.SetTrimSize(0);
        dComIfGp_event_reset();
        i_this->mDemoCamMode = 0;
        break;
    }

    if (i_this->mDemoCamMode != 0) {
        s16 sp8 = 7.5f * (i_this->field_0x1b064 * cM_scos(i_this->mCounter * 0x2C00));
        spC8 = i_this->mDemoCamCenter;
        spD4 = i_this->mDemoCamEye;
        spC8.y += (1.0f + KREG_F(8)) * (i_this->field_0x1b064 * cM_ssin(i_this->mCounter * 0x2800));
        spD4.y += (1.0f + KREG_F(8)) * (i_this->field_0x1b064 * cM_ssin(i_this->mCounter * 0x2800));

        camera->mCamera.Set(spC8, spD4, sp8, i_this->mDemoCamFovy);
        cLib_addCalc0(&i_this->field_0x1b064, 1.0f, 2.0f + TREG_F(16));
        i_this->mDemoCamTimer++;
    }
}

static s8 e_fm_down(e_fm_class* i_this) {
    fopEn_enemy_c* enemy = (fopEn_enemy_c*)i_this;
    s8 sp8 = 1;
    int anm_frame = i_this->mpFmModelMorf->getFrame();

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_FM_DOWN, 10.0f, 0, 1.0f);
        i_this->mSound.startCreatureVoice(Z2SE_EN_FM_V_DOWN, -1);
        i_this->mMode = 1;
        i_this->field_0x7c0 = 2;
        i_this->mDamageInvulnerabilityTimer = 5;
        break;
    case 1:
        i_this->mDamageInvulnerabilityTimer = 5;
        i_this->field_0x182b = 1;

        if (anm_frame == (BREG_S(0) + 54) && i_this->field_0x1b078 == 0) {
            i_this->mDemoCamMode++;
            i_this->mDemoCamTimer = 0;
        }

        if (anm_frame < 72) {
            i_this->field_0x7c0 = 2;
        }

        if (anm_frame == 90) {
            dComIfGp_getVibration().StartShock(VIBMODE_S_POWER8, 0xF, cXyz(0.0f, 1.0f, 0.0f));
            dComIfGp_particle_set(0x8150, &i_this->home.pos, NULL, NULL);
            mDoAud_seStart(Z2SE_OBJ_FM_DIRTFALL, NULL, 0, 0);

            i_this->mSound.startCreatureSound(Z2SE_EN_FM_DOWN, 0, -1);
            i_this->field_0x1b064 = 30.0f;
            hasira_hahen_hit(&i_this->eyePos, 500.0f + VREG_F(4), 50.0f + VREG_F(5), -1);

            if (i_this->field_0x792 != 0) {
                i_this->field_0x792 = 0;
                i_this->mPlayTexAnmNo = TEXANM_PUTOUT_WAIT;
                i_this->mpFmBrk[TEXANM_PUTOUT_WAIT]->setFrame(0.0f);
                i_this->mpFmBtk[TEXANM_PUTOUT_WAIT]->setFrame(0.0f);
            }

            if (i_this->mDownCnt >= 3) {
                enemy->onDownFlg();
            }

            Z2GetAudioMgr()->changeBgmStatus(1);
        }

        if (i_this->mpFmModelMorf->isStop()) {
            anm_init(i_this, BCK_FM_DOWNWAIT, 0.0f, 2, 1.0f);
            i_this->mMode = 2;
            i_this->mTimers[0] = 33;
            i_this->mTimers[1] = l_HIO.field_0x9c;
            /* dSv_event_flag_c::F_0256 - For E3 2006 - Knocked down boss at leased once */
            dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[256]);
        }
        break;
    case 2:
        /* dSv_event_flag_c::F_0257 - For E3 2006 - Only ON when boss is in hollow state (normally off, changes in real time) */
        dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[257]);

        if (i_this->mAnm == BCK_FM_DOWNDAMAGE && i_this->mpFmModelMorf->isStop()) {
            anm_init(i_this, BCK_FM_DOWNWAIT, 5.0f, 2, 1.0f);
        }

        if (i_this->mTimers[0] == 1) {
            cXyz sp40;
            cXyz sp34;
            fopAc_ac_c* player = dComIfGp_getPlayer(0);
            i_this->mDemoCamMode = 100;

            cMtx_YrotS(*calc_mtx, player->shape_angle.y);
            sp40.x = 0.0f;
            sp40.y = 200.0f;
            sp40.z = -400.0f;
            MtxPosition(&sp40, &sp34);
            i_this->mDemoCamEye = player->current.pos + sp34;
            i_this->mDemoCamCenter = player->eyePos;
            daPy_getPlayerActorClass()->cancelOriginalDemo();
        }

        if (i_this->mTimers[1] == 0) {
            anm_init(i_this, BCK_FM_UP, 10.0f, 0, 1.0f);
            i_this->mSound.startCreatureVoice(Z2SE_EN_FM_V_UP, -1);
            i_this->mMode = 3;
            enemy->offDownFlg();
            Z2GetAudioMgr()->changeBgmStatus(2);
            OS_REPORT(" CHANGE BGM STATUS 2 \n");
        }
        break;
    case 3:
        i_this->field_0x770 = 0;
        i_this->mDamageInvulnerabilityTimer = 1000;
        sp8 = 0;

        if (i_this->mpFmModelMorf->isStop()) {
            anm_init(i_this, BCK_FM_ANIMAL02, 10.0f, 0, 1.0f);
            i_this->mMode = 4;
            i_this->mPlayTexAnmNo = TEXANM_ANIMAL;
            i_this->mpFmBrk[TEXANM_ANIMAL]->setFrame(0.0f);
            i_this->mpFmBtk[TEXANM_ANIMAL]->setFrame(0.0f);
        }
        break;
    case 4:
        sp8 = 0;

        if (anm_frame == 60) {
            i_this->field_0x792 = 1;
            Z2GetAudioMgr()->changeBgmStatus(4);
        }

        if (anm_frame == 65) {
            for (int i = 0; i < 4; i++) {
                carry_off(i_this, i);
            }

            i_this->field_0x79c = 100.0f;
            i_this->field_0x182c = 3;
        }

        if (i_this->mpFmModelMorf->isStop()) {
            i_this->mAction = ACTION_NORMAL;
            i_this->mMode = 0;
            i_this->mPlayTexAnmNo = TEXANM_FM;
            i_this->mpFmBrk[TEXANM_FM]->setFrame(0.0f);
            i_this->mpFmBtk[TEXANM_FM]->setFrame(0.0f);
            i_this->mDamageInvulnerabilityTimer = 2;
            i_this->field_0x770 = 1;
        }
        break;
    }

    i_this->speedF = 0.0f;
    return sp8;
}

static s8 e_fm_a_down(e_fm_class* i_this) {
    int anm_frame = i_this->mpFmModelMorf->getFrame();
    i_this->mDamageInvulnerabilityTimer = 10;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_FM_DOWN02, 15.0f, 0, 1.0f);
        i_this->mSound.startCreatureVoice(Z2SE_EN_FM_V_DOWN, -1);
        i_this->mMode = 1;
        break;
    case 1:
        i_this->field_0x182b = 1;

        if (anm_frame == 112) {
            dComIfGp_getVibration().StartShock(YREG_S(2) + 5, 0xF, cXyz(0.0f, 1.0f, 0.0f));
            dComIfGp_particle_set(0x8150, &i_this->home.pos, NULL, NULL);
            mDoAud_seStart(Z2SE_OBJ_FM_DIRTFALL, NULL, 0, 0);
            i_this->mSound.startCreatureSound(Z2SE_EN_FM_DOWN, 0, -1);

            i_this->field_0x1b064 = 30.0f;
            hasira_hahen_hit(&i_this->eyePos, 500.0f + VREG_F(4), 50.0f + VREG_F(5), -1);

            if (i_this->field_0x792 != 0) {
                i_this->field_0x792 = 0;
                i_this->mPlayTexAnmNo = TEXANM_PUTOUT_WAIT;
                i_this->mpFmBrk[TEXANM_PUTOUT_WAIT]->setFrame(0.0f);
                i_this->mpFmBtk[TEXANM_PUTOUT_WAIT]->setFrame(0.0f);
            }

            if (i_this->mDownCnt >= 3) {
                i_this->onDownFlg();
            }

            Z2GetAudioMgr()->changeBgmStatus(1);
        }

        if (i_this->mpFmModelMorf->isStop()) {
            anm_init(i_this, BCK_FM_DOWNWAIT, 0.0f, 2, 1.0f);
            i_this->mAction = ACTION_DOWN;
            i_this->mMode = 2;
            i_this->mTimers[0] = 33;
            i_this->mTimers[1] = l_HIO.field_0x9c;
        }
        break;
    }

    i_this->speedF = 0.0f;
    return 1;
}

static s8 e_fm_start(e_fm_class* i_this) {
    cXyz sp50;
    cXyz sp44;

    s8 var_r25 = 0;
    int anm_frame = i_this->mpFmModelMorf->getFrame();
    i_this->mDamageInvulnerabilityTimer = 5;
    int var_r26 = 0;

    if (i_this->mDemoCamMode >= 11 && dComIfGp_getEvent()->checkSkipEdge()) {
        cDmr_SkipInfo = 1;
        i_this->mDemoCamMode = 1000;
        dComIfGp_event_reset();
        dComIfGp_getVibration().StopQuake(0x1F);
        dStage_changeScene(1, 0.0f, 0, fopAcM_GetRoomNo(i_this), 0, -1);
        return 0;
    }

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_FM_HANGWAIT, 0.0f, 2, 1.0f);
        i_this->mPlayTexAnmNo = TEXANM_HANG_WAIT;
        i_this->mMode = 1;
        i_this->mChain[3].field_0x617e = 1;
        i_this->mChain[2].field_0x617e = 1;
        i_this->mChain[1].field_0x617e = 1;
        i_this->mChain[0].field_0x617e = 1;
        i_this->mDemoCamMode = 10;
        break;
    case 1:
        cMtx_YrotS(*calc_mtx, i_this->shape_angle.y);
        sp50.x = 800.0f + NREG_F(10);
        sp50.y = 1000.0f + NREG_F(11);
        sp50.z = -500.0f + NREG_F(12);
        MtxPosition(&sp50, &sp44);
        i_this->mChain[0].field_0x6180 = i_this->current.pos + sp44;

        sp50.x = -(800.0f + NREG_F(10));
        MtxPosition(&sp50, &sp44);
        i_this->mChain[1].field_0x6180 = i_this->current.pos + sp44;
        i_this->mChain[2].field_0x6180.set(677.0f, 40.0f, -226.0f);
        i_this->mChain[3].field_0x6180.set(-677.0f, 40.0f, -226.0f);
        break;
    case 2:
        if (anm_frame == 339) {
            i_this->field_0x792 = 1;
            i_this->field_0x182c = 3;
            carry_off(i_this, 2);
            carry_off(i_this, 3);
            i_this->field_0x5c8 = 1;
        }

        if (anm_frame >= 33) {
            var_r25 = 1;
        }

        if (anm_frame >= 576 && anm_frame <= 596) {
            i_this->field_0x1829 = 1;
            i_this->field_0x1828 = 1;

            if (anm_frame == 576) {
                carry_off(i_this, 1);
                var_r26 = 2;
                dComIfGp_getVibration().StartShock(VIBMODE_S_POWER3, 1, cXyz(0.0f, 1.0f, 0.0f));
            }
        }

        if (anm_frame >= 651 && anm_frame <= 671) {
            i_this->field_0x1829 = 1;
            i_this->field_0x1828 = 0;

            if (anm_frame == 651) {
                carry_off(i_this, 0);
                var_r26 = 1;
                dComIfGp_getVibration().StartShock(VIBMODE_S_POWER3, 1, cXyz(0.0f, 1.0f, 0.0f));
            }
        }

        if (anm_frame == 730) {
            i_this->field_0x1b080 = 1;
            fpcM_Search(s_fmobj_del, NULL);
        }

        if (anm_frame == (int)((i_this->mpFmModelMorf->getEndFrame() - 80.0f) - 20.0f)) {
            fopMsgM_messageSetDemo(0x480);
        }

        if (i_this->mpFmModelMorf->isStop()) {
            i_this->mAction = ACTION_NORMAL;
            i_this->mMode = 0;
            i_this->mPlayTexAnmNo = TEXANM_FM;
            i_this->mpFmBrk[TEXANM_FM]->setFrame(0.0f);
            i_this->mpFmBtk[TEXANM_FM]->setFrame(0.0f);
            Z2GetAudioMgr()->bgmStart(Z2BGM_BOSSFIREMAN_0, 0, 0);
            i_this->mDemoCamMode = 100;
        }
        break;
    }

    if (var_r26 != 0) {
        cXyz ef_size(1.5f, 1.5f, 1.5f);
        dComIfGp_particle_set(0x8455, &i_this->mChain[var_r26 - 1].field_0x6180, NULL, &ef_size);
    }

    return var_r25;
}

static s8 e_fm_end(e_fm_class* i_this) {
    int anm_frame = i_this->mpFmModelMorf->getFrame();
    i_this->mDamageInvulnerabilityTimer = 5;
    s8 var_r30 = 0;

    switch (i_this->mMode) {
    case 0:
        anm_init(i_this, BCK_FM_DEMOEND01, 1.0f, 0, 1.0f);
        i_this->mMode = 1;
        Z2GetAudioMgr()->bgmStop(0x1E, 0);
        i_this->mPlayTexAnmNo = TEXANM_DEMO_END01;
        i_this->mpFmBrk[TEXANM_DEMO_END01]->setFrame(0.0f);
        i_this->mpFmBtk[TEXANM_DEMO_END01]->setFrame(0.0f);
        i_this->mDemoCamMode = 50;
        break;
    case 1:
        break;
    case 10:
        anm_init(i_this, BCK_FM_DEMOEND01, 0.0f, 0, 0.0f);
        i_this->mMode = 11;
        i_this->mPlayTexAnmNo = TEXANM_DEMO_END01;
        i_this->mpFmBrk[TEXANM_DEMO_END01]->setFrame(0.0f);
        i_this->mpFmBtk[TEXANM_DEMO_END01]->setFrame(0.0f);
        i_this->mpFmBrk[TEXANM_DEMO_END01]->setPlaySpeed(0.0f);
        i_this->mpFmBtk[TEXANM_DEMO_END01]->setPlaySpeed(0.0f);
        i_this->mTimers[0] = 40;
        /* fallthrough */
    case 11:
        var_r30 = 1;
        if (i_this->mTimers[0] == 0) {
            i_this->mMode = 1;
            i_this->mpFmModelMorf->setPlaySpeed(1.0f);
            i_this->mpFmBrk[TEXANM_DEMO_END01]->setPlaySpeed(1.0f);
            i_this->mpFmBtk[TEXANM_DEMO_END01]->setPlaySpeed(1.0f);
            Z2GetAudioMgr()->bgmStop(0x1E, 0);
            i_this->mDemoCamMode = 50;
        }
        break;
    }

    return var_r30;
}

static void damage_check(e_fm_class* i_this) {
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp30;
    cXyz sp24;

    i_this->field_0xa24.Move();
    i_this->field_0xa60.Move();

    if (i_this->mDamageInvulnerabilityTimer == 0) {
        if (i_this->mCoreSph.ChkTgHit()) {
            i_this->mDamageInvulnerabilityTimer = 6;
            i_this->mSound.startCreatureSound(Z2SE_EN_FM_COREHIT, 0, -1);

            if (i_this->mAction == ACTION_DOWN) {
                i_this->mAtInfo.mpCollider = i_this->mCoreSph.GetTgHitObj();
                cc_at_check(i_this, &i_this->mAtInfo);
                OS_REPORT("//////////////FM AP %d\n", i_this->mAtInfo.mAttackPower);
                OS_REPORT("//////////////FM HP %d\n", i_this->health);

                if (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_JUMP && daPy_getPlayerActorClass()->checkCutJumpCancelTurn()) {
                    i_this->mDamageInvulnerabilityTimer = NREG_S(7) + 3;
                } else {
                    i_this->mDamageInvulnerabilityTimer = NREG_S(6) + 6;
                }

                if (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_MORTAL_DRAW_B) {
                    i_this->health = 0;
                }

                if (i_this->health <= 0 || daPy_getPlayerActorClass()->getCutCount() >= 4) {
                    if (i_this->health <= 0 && i_this->mDownCnt >= 3) {
                        i_this->mAction = ACTION_END;
                        i_this->mMode = 0;
                        carry_off(i_this, 2);
                        carry_off(i_this, 3);
                    } else {
                        anm_init(i_this, BCK_FM_UP02, 5.0f, 0, 1.0f);
                        i_this->mSound.startCreatureVoice(Z2SE_EN_FM_V_UP02, -1);
                        i_this->mMode = 3;
                        Z2GetAudioMgr()->changeBgmStatus(2);
                    }
                } else {
                    anm_init(i_this, BCK_FM_DOWNDAMAGE, 3.0f, 0, 1.0f);
                    i_this->mSound.startCreatureSound(Z2SE_EN_FM_FOOTNOTE, 0, -1);
                }

                i_this->mSound.startCreatureVoice(Z2SE_EN_FM_V_DAMAGE, -1);
                /* dSv_event_flag_c::F_0670 - Goron Mines - Hitting knocked-down Fyrus */
                dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[670]);
            } else {
                i_this->field_0x804++;
                /* dSv_event_flag_c::F_0254 - For E3 2006 - Hit boss's weak spot at least once */
                dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[254]);

                if (i_this->field_0x804 >= 10) {
                    i_this->mDownCnt++;
                    i_this->mAction = ACTION_A_DOWN;
                    i_this->health = 50;
                    i_this->mDemoCamMode = 20;
                } else {
                    i_this->mAction = ACTION_DAMAGE_RUN;
                    anm_init(i_this, BCK_FM_CHANCEDAMAGE, 3.0f, 0, 1.0f);
                    i_this->mSound.startCreatureVoice(Z2SE_EN_FM_V_CHANCEDAMAGE, -1);
                }
                i_this->mMode = 0;
            }

            i_this->field_0x2398 = 30;
        }

        sp24 = i_this->eyePos;
        i_this->setDownPos(&i_this->eyePos);

        if (i_this->checkCutDownHitFlg()) {
            i_this->mAction = ACTION_END;
            i_this->mMode = 10;
            carry_off(i_this, 2);
            carry_off(i_this, 3);
            i_this->offCutDownHitFlg();
            i_this->offDownFlg();
            i_this->mSound.startCreatureVoice(Z2SE_EN_FM_V_TODOME, -1);
            i_this->mDamageInvulnerabilityTimer = 300;
            i_this->field_0x182c = 1;
        }
    }
}

static void chain_control1(e_fm_class* i_this, chain_s* i_chain_s, int param_2) {
    fopAc_ac_c* actor = i_this;
    cXyz sp80;
    cXyz sp74;

    i_chain_s->pos[0] = i_chain_s->field_0x68;

    int i;
    s16 spE, spC;
    cXyz* ppos = &i_chain_s->pos[1];
    cXyz* var_r27 = &i_chain_s->field_0x2e4[1];
    csXyz* prot = &i_chain_s->rot[1];

    cXyz sp68 = i_chain_s->sound_pos - i_chain_s->field_0x6094;

    f32 temp_f28;
    f32 temp_f27;
    f32 var_f26;
    f32 var_f29 = sp68.abs();

    cXyz sp5C(0.0f, 0.0f, 0.0f);
    f32 sp24 = 0.0f;

    if ((i_chain_s->field_0x617e != 0 || (i_chain_s->field_0x617c != 0 && daPy_getPlayerActorClass()->checkEquipHeavyBoots())) && i_chain_s->field_0x6174 > 100.0f) {
        f32 var_f25 = i_chain_s->field_0x6174 - 100.0f;
        if (var_f25 > 50.0f) {
            var_f25 = 50.0f;
        }

        sp80 = i_chain_s->field_0x6180 - i_chain_s->field_0x68;

        cMtx_YrotS(*calc_mtx, cM_atan2s(sp80.x, sp80.z));
        cMtx_XrotM(*calc_mtx, -cM_atan2s(sp80.y, JMAFastSqrt((sp80.x * sp80.x) + (sp80.z * sp80.z))));
        sp80.x = 0.0f;
        sp80.y = 0.0f;
        sp80.z = var_f25 * (10.0f + YREG_F(10));
        MtxPosition(&sp80, &sp5C);
    } else if (i_this->field_0x79c > 1.0f) {
        if (i_this->mAction == ACTION_END) {
            cMtx_YrotS(*calc_mtx, actor->shape_angle.y + 0x8000);
        } else {
            sp80 = i_chain_s->pos[i_chain_s->field_0x61d0 - 1] - actor->current.pos;
            cMtx_YrotS(*calc_mtx, cM_atan2s(sp80.x, sp80.z));
        }

        sp80.x = 0.0f;
        sp80.y = 0.0f;
        sp80.z = i_this->field_0x79c;
        MtxPosition(&sp80, &sp5C);
    }

    sp80.x = 0.0f;
    sp80.y = 0.0f;
    sp80.z = l_HIO.field_0x4c;

    f32 var_f31;
    f32 sp20 = -20.0f + XREG_F(0);

    if (i_chain_s->field_0x617c != 0) {
        var_f31 = 0.73f + XREG_F(3);
        if (i_this->field_0x1829 != 0) {
            var_f31 += 0.12f;
        }
        sp20 = (-20.0f + XREG_F(4));
    } else {
        var_f31 = 0.66f + XREG_F(2);
        if (i_this->field_0x1829 != 0) {
            var_f31 += 0.17f;
        }
    }

    f32 sp1C = 15.0f + i_this->mAcch.GetGroundH();
    cXyz sp50(0.75f, 0.75f, 0.75f);

    for (i = 1; i < i_chain_s->field_0x61d0; i++, ppos++, prot++, var_r27++) {
        temp_f28 = sp5C.x + (var_r27[0].x + (ppos[0].x - ppos[-1].x));
        temp_f27 = sp5C.z + (var_r27[0].z + (ppos[0].z - ppos[-1].z));

        if (i_this->field_0x79c > 1.0f) {
            sp24 = (i_this->field_0x79c * (0.3f + KREG_F(15))) +
                   ((0.3f + KREG_F(15)) * (i_this->field_0x79c * cM_ssin(((i_this->mCounter * 6000) + (i * (KREG_S(4) - 3000))))));
        }

        var_f26 = sp24 + (sp5C.y + (sp20 + (ppos->y + var_r27->y)));
        if (var_f26 <= sp1C) {
            var_f26 = sp1C;
            if (var_f29 >= (10.0f + DREG_F(0)) && i_this->mTimers[3] == 0) {
                if (i == i_chain_s->field_0x61d0 / 2) {
                    if (var_f29 > 110.0f) {
                        var_f29 = 110.0f;
                    }

                    i_this->mSound.startChainSoundLevel(Z2SE_EN_FM_CHAIN_HIKIZURI, param_2, var_f29 - 10.0f, 0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));
                }
            }
        }

        f32 sp2C = var_f26 - ppos[-1].y;

        spC = (s16)cM_atan2s(temp_f28, temp_f27);
        spE = -cM_atan2s(sp2C, JMAFastSqrt((temp_f28 * temp_f28) + (temp_f27 * temp_f27)));

        cMtx_YrotS(*calc_mtx, spC);
        cMtx_XrotM(*calc_mtx, spE);
        MtxPosition(&sp80, &sp74);
        *var_r27 = *ppos;

        ppos[0].x = ppos[-1].x + sp74.x;
        ppos[0].y = ppos[-1].y + sp74.y;
        ppos[0].z = ppos[-1].z + sp74.z;
        var_r27[0].x = var_f31 * (ppos[0].x - var_r27[0].x);
        var_r27[0].y = var_f31 * (ppos[0].y - var_r27[0].y);
        var_r27[0].z = var_f31 * (ppos[0].z - var_r27[0].z);

        if (i_chain_s->field_0x617c == 0) {
            prot[-1].x = spE;

            f32 var_f30 = 1024.0f * var_r27->abs();
            if (var_f30 > 16384.0f) {
                prot[-1].y = spC;
            } else {
                if (var_f30 < 2048.0f) {
                    if (demo_stop != 0) {
                        var_f30 = 0.0f;
                    } else {
                        var_f30 = 2048.0f;
                    }
                    var_r27->set(0.0f, 0.0f, 0.0f);
                }

                cLib_addCalcAngleS2(&prot[-1].y, spC, 2, var_f30);
            }
        }
    }

    for (int i = 0; i < i_chain_s->field_0x61d0; i++) {
        i_chain_s->field_0x1ac[i] = i_chain_s->pos[i];
    }
}

static void chain_control2(e_fm_class* i_this, chain_s* i_chain_s, int param_2) {
    if ((i_chain_s->field_0x617c != 0) || (i_chain_s->field_0x619c != 0) || (i_chain_s->field_0x617e != 0)) {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        cXyz sp64;
        cXyz sp58;
        cXyz sp4C;
        fopAc_ac_c* actor = i_this;
        dBgS_GndChk gndchk;

        f32 temp_f31;
        f32 temp_f27;
        f32 temp_f30;

        if (i_chain_s->field_0x617c != 0) {
            daPy_py_c* sp18 = (daPy_py_c*)dComIfGp_getPlayer(0);
            MTXCopy(sp18->getRightItemMatrix(), *calc_mtx);
            sp64.set(0.0f, 0.0f, 0.0f);
            MtxPosition(&sp64, &sp4C);

            s16 spC = -10000.0f + (30.0f * i_chain_s->field_0x6174);
            if (spC > -4000) {
                spC = -4000;
            }

            f32 var_f29 = -100.0f + (0.1f * i_chain_s->field_0x6174);
            if (var_f29 > -80.0f) {
                var_f29 = -80.0f;
            }

            f32 var_f28 = 30.0f + (0.025f * i_chain_s->field_0x6174);
            if (var_f28 > 35.0f) {
                var_f28 = 35.0f;
            }

            cLib_addCalcAngleS2(&i_chain_s->field_0x619e, spC, 2, 1000);
            cLib_addCalc2(&i_chain_s->field_0x61a0, var_f29, 0.5f, 5.0f);
            cLib_addCalc2(&i_chain_s->field_0x61a4, var_f28, 0.5f, 5.0f);

            cMtx_YrotS(*calc_mtx, player->shape_angle.y);
            sp64.x = (25.0f + NREG_F(0)) - 7.0f;
            sp64.y = (48.0f + (i_chain_s->field_0x61a0 + NREG_F(1))) - 8.0f;
            sp64.z = (20.0f + (i_chain_s->field_0x61a4 + NREG_F(2))) - 7.0f;
            MtxPosition(&sp64, &sp58);
            i_chain_s->pos[i_chain_s->field_0x61d0 - 1] = sp4C + sp58;
            i_chain_s->field_0x618c = i_chain_s->pos[i_chain_s->field_0x61d0 - 1];
            i_chain_s->field_0x6198 = 0.0f;
        } else if (i_chain_s->field_0x617e != 0) {
            cMtx_YrotS(*calc_mtx, i_chain_s->rot[i_chain_s->field_0x61d0 - 2].y);
            sp64.x = 0.0f;
            sp64.y = -5.0f + AREG_F(11);
            sp64.z = (20.0f + AREG_F(12)) - 30.0f;
            MtxPosition(&sp64, &sp58);
            i_chain_s->pos[i_chain_s->field_0x61d0 - 1] = i_chain_s->field_0x6180 + sp58;
        } else if (i_chain_s->field_0x619c != 0) {
            i_chain_s->field_0x619c--;
            if (i_chain_s->field_0x619c == 0) {
                daPy_py_c::setPlayerSe(Z2SE_AL_GRAB_CHAIN);
            }

            i_chain_s->field_0x618c.y += i_chain_s->field_0x6198;
            i_chain_s->field_0x6198 -= 7.0f;

            if (i_chain_s->field_0x618c.y <= 15.0f + i_this->mAcch.GetGroundH()) {
                i_chain_s->field_0x618c.y = 15.0f + i_this->mAcch.GetGroundH();
            }

            i_chain_s->pos[i_chain_s->field_0x61d0 - 1] = i_chain_s->field_0x618c;
        }

        int i;
        s16 sp10, spE;
        cXyz* ppos = &i_chain_s->pos[i_chain_s->field_0x61d0 - 2];
        csXyz* prot = &i_chain_s->rot[i_chain_s->field_0x61d0 - 2];
        sp64.x = 0.0f;
        sp64.y = 0.0f;
        sp64.z = l_HIO.field_0x4c;

        for (i = i_chain_s->field_0x61d0 - 2; i >= i_chain_s->field_0x6170; i--, ppos--, prot--) {
            temp_f31 = ppos[0].x - ppos[1].x;
            temp_f27 = ppos[0].y - ppos[1].y;
            temp_f30 = ppos[0].z - ppos[1].z;

            spE = (s16)cM_atan2s(temp_f31, temp_f30);
            sp10 = -cM_atan2s(temp_f27, JMAFastSqrt((temp_f31 * temp_f31) + (temp_f30 * temp_f30)));

            cMtx_YrotS(*calc_mtx, spE);
            cMtx_XrotM(*calc_mtx, sp10);
            MtxPosition(&sp64, &sp58);

            if (i == (i_chain_s->field_0x61d0 - 2) && (i_chain_s->field_0x617c != 0 || i_chain_s->field_0x619c != 0)) {
                *ppos = *(ppos + 1);
            } else {
                *ppos = *(ppos + 1) + sp58;
            }

            if (i_chain_s->field_0x617c != 0 && i == ((i_chain_s->field_0x61d0 + KREG_S(2)) - 2)) {
                cLib_addCalcAngleS2(&prot->y, player->shape_angle.y + 0x8000, 2, 0x2000);
                cLib_addCalcAngleS2(&prot->x, i_chain_s->field_0x619e, 2, 0x2000);
            } else {
                prot->x = -sp10;
                prot->y = (spE + 0x8000);
            }
        }
    }
}

static void chain_control3(e_fm_class* i_this, chain_s* i_chain_s, int param_2) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz sp68;
    cXyz sp5C;

    sp68 = i_chain_s->field_0x1ac[i_chain_s->field_0x6170] - i_chain_s->pos[i_chain_s->field_0x6170];
    i_chain_s->field_0x6174 = sp68.abs();
    
    cXyz* ppos = &i_chain_s->pos[i_chain_s->field_0x6170];
    for (int i = i_chain_s->field_0x6170; i < i_chain_s->field_0x61d0; i++, ppos++) {
        *ppos += sp68 * ((f32)(i_chain_s->field_0x61d0 - (i - i_chain_s->field_0x6170)) / (f32)i_chain_s->field_0x61d0);
    }

    if (i_chain_s->field_0x617c != 0) {
        i_chain_s->field_0x6180 = player->current.pos;
        sp68 = i_chain_s->pos[i_chain_s->field_0x61d0 - 2] - i_chain_s->pos[i_chain_s->field_0x61d0 - 1];

        if (daPy_getPlayerActorClass()->checkEquipHeavyBoots()) {
            if (i_chain_s->field_0x6174 > 300.0f) {
                cMtx_YrotS(*calc_mtx, cM_atan2s(sp68.x, sp68.z));
                sp68.y = 0.0f;
                sp68.x = 0.0f;
                sp68.z = (i_chain_s->field_0x6174 - 300.0f) * (0.05f + XREG_F(14));
                MtxPosition(&sp68, &sp5C);
                actor->current.pos -= sp5C;
            }
        } else if (i_chain_s->field_0x6174 > 50.0f && i_this->mDemoCamMode == 0) {
            daPy_py_c* spC = (daPy_py_c*)dComIfGp_getPlayer(0);
            f32 temp_f31 = (i_chain_s->field_0x6174 - 50.0f);
            temp_f31 *= 0.5f + TREG_F(12);
            
            spC->setOutPower(temp_f31, cM_atan2s(sp68.x, sp68.z), 0);
        }
    } else if (i_chain_s->field_0x617e == 0) {
        i_chain_s->field_0x6174 = 0.0f;
    } else if (i_this->mAction != ACTION_START) {
        f32 var_f30;
        if (i_this->mAction == ACTION_STOP) {
            var_f30 = 200.0f;
        } else {
            var_f30 = 50.0f;
        }

        if (i_chain_s->field_0x6174 > var_f30) {
            sp68 = i_chain_s->pos[i_chain_s->field_0x61d0 - 2] - i_chain_s->pos[i_chain_s->field_0x61d0 - 1];
            cMtx_YrotS(*calc_mtx, cM_atan2s(sp68.x, sp68.z));
            sp68.y = 0.0f;
            sp68.x = 0.0f;
            sp68.z = (0.05f + XREG_F(14)) * (i_chain_s->field_0x6174 - var_f30);
            MtxPosition(&sp68, &sp5C);
            actor->current.pos -= sp5C;
        }
    }

    if (param_2 >= 2 && i_this->mAction != ACTION_END) {
        if (i_chain_s->field_0x617d != 0) {
            i_chain_s->field_0x617d--;
        }

        fopAc_ac_c* var_r24 = NULL;
        if (i_chain_s->field_0x617c == 0 || i_chain_s->field_0x617e != 0) {
            if (i_chain_s->pos[i_chain_s->field_0x61d0 - 1].y < 0.0f) {
                daPy_py_c::setLookPos(&i_chain_s->pos[i_chain_s->field_0x61d0 - 1]);
            }

            if (i_chain_s->field_0x617d == 0) {
                if (daPy_py_c::setFmChainPos(actor, &i_chain_s->pos[i_chain_s->field_0x61d0 - 1], param_2)) {
                    i_chain_s->field_0x617c = 1;

                    daPy_py_c::setPlayerSe(Z2SE_AL_GRAB_CHAIN);
                    dComIfGp_getVibration().StartShock(VIBMODE_S_POWER3, 1, cXyz(0.0f, 1.0f, 0.0f));

                    if (i_chain_s->field_0x617e != 0 && i_this->mAction == ACTION_STOP) {
                        i_this->mAction = ACTION_NORMAL;
                        i_this->mMode = 0;
                    }

                    i_chain_s->field_0x617e = 0;
                    /* dSv_event_flag_c::F_0669 - Goron Mines - Pulled on Fyrus chains at least once */
                    dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[669]);
                }
            }
        } else {
            if (i_chain_s->field_0x617d == 0 && i_chain_s->field_0x617c != 0 &&
                !daPy_py_c::setFmChainPos(actor, &i_chain_s->pos[i_chain_s->field_0x61d0 - 1], param_2))
            {
                if (var_r24 != NULL) {
                    i_chain_s->field_0x617d = 10;
                } else {
                    i_chain_s->field_0x617d = 1;
                }
            }
            
            if (i_chain_s->field_0x617d == 1) {
                i_chain_s->field_0x617c = 0;
                i_chain_s->field_0x6170 = 0;

                if (var_r24 != NULL) {
                    i_chain_s->field_0x617e = 1;
                    i_chain_s->field_0x6180 = var_r24->current.pos;
                    i_chain_s->field_0x6180.y += 40.0f + YREG_F(15);

                    daPy_py_c::setPlayerSe(Z2SE_AL_SET_CHAIN);
                    cXyz sp14;
                    dComIfGp_getVibration().StartShock(VIBMODE_S_POWER2, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
                } else {
                    i_chain_s->field_0x619c = 4;
                }
            }
        }
    }
}

static int ba_count;

static void* s_ba_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_BA) {
        ba_count++;
    }

    return NULL;
}

static void action(e_fm_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp48;
    cXyz sp3C;

    i_this->mAngleToPlayer = fopAcM_searchPlayerAngleY(actor);

#if DEBUG
    if (mDoCPd_c::getTrigStart(PAD_2) && i_this->mAction != ACTION_DOWN) {
        i_this->mAction = ACTION_DOWN;
        i_this->mMode = 0;
        actor->health = 50;
        i_this->mDownCnt = 3;
    }
#endif

    s8 attn_on = FALSE;
    s8 set_look_pos = TRUE;
    damage_check(i_this);
    s8 bossroom_wait_on = TRUE;

    i_this->field_0x1b07c = 1;

    /* dSv_event_flag_c::F_0255 - For E3 2006 - Boss exhausted (grabbing chains) only on during state (normally off) */
    dComIfGs_offEventBit(dSv_event_flag_c::saveBitLabels[255]);
    /* dSv_event_flag_c::F_0257 - For E3 2006 - Only ON when boss is in hollow state (normally off, changes in real time) */
    dComIfGs_offEventBit(dSv_event_flag_c::saveBitLabels[257]);

    switch (i_this->mAction) {
    case ACTION_NORMAL:
        e_fm_normal(i_this);
        i_this->field_0x1b07c = 0;
        break;
    case ACTION_FIGHT_RUN:
        e_fm_fight_run(i_this);
        i_this->field_0x1b07c = 0;
        break;
    case ACTION_N_FIGHT:
        e_fm_n_fight(i_this);
        i_this->field_0x1b07c = 0;
        break;
    case ACTION_F_FIGHT:
        e_fm_f_fight(i_this);
        i_this->field_0x1b07c = 0;
        break;
    case ACTION_DAMAGE_RUN:
        e_fm_damage_run(i_this);
        /* dSv_event_flag_c::F_0255 - For E3 2006 - Boss exhausted (grabbing chains) only on during state (normally off) */
        dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[255]);
        break;
    case ACTION_ANIMAL:
        e_fm_animal(i_this);
        i_this->field_0x1b07c = 0;
        break;
    case ACTION_FIRE:
        e_fm_fire(i_this);
        i_this->field_0x1b07c = 0;
        break;
    case ACTION_STOP:
        e_fm_stop(i_this);
        i_this->field_0x1b07c = 0;
        break;
    case ACTION_DOWN:
        attn_on = e_fm_down(i_this);
        break;
    case ACTION_A_DOWN:
        attn_on = e_fm_a_down(i_this);
        break;
    case ACTION_START:
        bossroom_wait_on = e_fm_start(i_this);
        break;
    case ACTION_END:
        attn_on = e_fm_end(i_this);
        bossroom_wait_on = FALSE;
        set_look_pos = FALSE;
        break;
    }

    cLib_addCalcAngleS2(&actor->shape_angle.y, actor->current.angle.y, 2, 0x1000);
    cLib_addCalcAngleS2(&actor->shape_angle.x, actor->current.angle.x, 2, 0x1000);
    cLib_addCalcAngleS2(&actor->shape_angle.z, actor->current.angle.z, 2, 0x1000);

    cMtx_YrotS(*calc_mtx, actor->current.angle.y);
    sp48.z = l_HIO.field_0xc * (actor->speedF * l_HIO.base_size);
    actor->gravity = -5.0f;
    sp48.x = 0.0f;
    sp48.y = 0.0f;
    MtxPosition(&sp48, &sp3C);
    actor->speed.x = sp3C.x;
    actor->speed.z = sp3C.z;
    actor->current.pos += actor->speed;

    if (actor->current.pos.abs() > l_HIO.field_0xa0) {
        sp48 = actor->current.pos;
        cMtx_YrotS(*calc_mtx, cM_atan2s(-sp48.x, -sp48.z));
        sp48.x = 0.0f;
        sp48.y = 0.0f;
        sp48.z = 10.0f + VREG_F(7);
        MtxPosition(&sp48, &sp3C);
        actor->current.pos += sp3C;
        i_this->field_0x809 = 1;
    } else {
        i_this->field_0x809 = 0;
    }

    cXyz* cc_move_p = i_this->field_0xa24.GetCCMoveP();
    if (cc_move_p != NULL) {
        actor->current.pos.x += cc_move_p->x;
        actor->current.pos.z += cc_move_p->z;
    }

    if (i_this->field_0x7fc > 0.1f) {
        sp48.x = 0.0f;
        sp48.y = 0.0f;
        sp48.z = -i_this->field_0x7fc;
        cMtx_YrotS(*calc_mtx, i_this->field_0x800);
        MtxPosition(&sp48, &sp3C);
        actor->current.pos += sp3C;

        cLib_addCalc0(&i_this->field_0x7fc, 1.0f, 7.0f + TREG_F(12));
    }

    s16 sp16 = 0;
    s16 sp14 = 0;
    if (i_this->field_0x7c0 != 0) {
        if (i_this->field_0x7c0 == 1) {
            sp48 = player->eyePos - actor->current.pos;
            sp48.y += -300.0f + TREG_F(2);

            sp16 = cM_atan2s(sp48.x, sp48.z) - actor->shape_angle.y;
            sp14 = actor->shape_angle.x + -cM_atan2s(sp48.y, JMAFastSqrt((sp48.x * sp48.x) + (sp48.z * sp48.z)));

            if (sp16 > 7000) {
                sp16 = 7000;
            } else if (sp16 < -7000) {
                sp16 = -7000;
            }

            if (sp14 > 1000) {
                sp14 = 1000;
            } else if (sp14 < -1000) {
                sp14 = -1000;
            }
        } else {
            if (i_this->field_0x808 & 1) {
                sp16 = -14000;
            } else {
                sp16 = 14000;
            }
            sp14 = 2000;
        }

        i_this->field_0x7c0 = 0;
    }

    cLib_addCalcAngleS2(&i_this->mBodyRotX, sp16 / 2, 0x10, 0x200);
    cLib_addCalcAngleS2(&i_this->mHeadRotZ, sp14, 0x10, 0x200);

    if (attn_on) {
        fopAcM_OnStatus(actor, 0);
        actor->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(actor, 0);
        actor->attention_info.flags = 0;
    }

    if (set_look_pos) {
        daPy_py_c::setLookPos(&actor->eyePos);
    }

    if (i_this->mDoCreateBa) {
        i_this->mDoCreateBa = FALSE;
        ba_count = 0;
        fpcM_Search(s_ba_sub, i_this);
        
        const int MAX_BA = 4;
        int create_num = MAX_BA - ba_count;

        csXyz rot(0, 0, 0);
        rot.x = JREG_S(8);

        for (int i = 0; i < create_num; i++) {
            fopAcM_createChild(PROC_E_BA, fopAcM_GetID(actor), 0xFFFF1F02, &actor->eyePos, fopAcM_GetRoomNo(actor), &rot, NULL, -1, NULL);
            rot.y += (s16)(0x10000 / create_num);
        }
    }

    demo_camera(i_this);

    if (bossroom_wait_on) {
        daPy_getPlayerActorClass()->onBossRoomWait();
    }
}

static void effect_set(e_fm_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    cXyz sp4C(l_HIO.field_0x2c, l_HIO.field_0x2c, l_HIO.field_0x2c);

    J3DModel* model = i_this->mpFmModelMorf->getModel();
    if (i_this->field_0x7b6 != 3) {
        i_this->field_0x7b6 = 0;
    }

    dComIfGs_BossLife_public_Set(0);

    f32 var_f28;
    f32 var_f27;
    if (i_this->field_0x792 != 0) {
        dComIfGs_BossLife_public_Set(100);
        var_f28 = 40.0f + KREG_F(9);
        var_f27 = 0.65f;

        if (i_this->field_0x7b6 != 3) {
            i_this->field_0x7b6 = 1;
        }

        f32 temp_f29 = l_HIO.field_0x28 * i_this->field_0x794;

        for (int i = 0; i < 14; i++) {
            static int eff_joint_d[] = {2, 6, 13, 8, 15, 1, 1, 6, 13, 1, 1, 1, 19, 1};

            static u16 eff_id[] = {
                0x80FD, 0x80F9, 0x80F9, 0x80FC, 0x80FC, 0x80FA, 0x80FB,
                0x80F7, 0x80F7, 0x80F8, 0x80FE, 0x80FF, 0x8723, 0x8722,
            };

            i_this->field_0x1838[i] = dComIfGp_particle_set(i_this->field_0x1838[i], (u16)eff_id[i], &i_this->current.pos, NULL, NULL);

            JPABaseEmitter* player = dComIfGp_particle_getEmitter(i_this->field_0x1838[i]);
            if (player != NULL) {
                MTXCopy(model->getAnmMtx(eff_joint_d[i]), *calc_mtx);
                MtxScale(temp_f29, temp_f29, temp_f29, 1);
                player->setGlobalSRTMatrix(*calc_mtx);
            }
        }

        i_this->mSound.startCreatureSoundLevel(Z2SE_EN_FM_BURNING, 0, -1);
    } else {
        var_f28 = 0.0f;
        var_f27 = 0.15f;
    }

    cLib_addCalc2(&i_this->mChainColorR, var_f28, 1.0f, var_f27);

    if (i_this->field_0x790 != 0) {
        i_this->field_0x790++;
        if (i_this->field_0x790 == 10) {
            for (int i = 0; i < 4; i++) {
                carry_off(i_this, i);
            }

            i_this->field_0x79c = 100.0f;
            hasira_hahen_hit(&actor->current.pos, 1000.0f + VREG_F(14), 100.0f + VREG_F(15), -1);
        }

        if (i_this->field_0x790 == 2) {
            for (int i = 0; i < 2; i++) {
                static int ef_bck[] = {7, 8};
                i_this->mpAttackEfModelMorf[i]->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("E_fm", ef_bck[i]), 0, 1.0f, 1.0f, 0.0f, -1.0f, NULL);
                i_this->mpAttackEfModelMorf[i]->setFrame(0.0f);
                i_this->mpAttackEfBtk[i]->setFrame(0.0f);

                if (i == 1) {
                    i_this->mpAttackEfBrk->setFrame(0.0f);
                    i_this->field_0x798 = 2.5f + KREG_F(7);
                }
            }

            dComIfGp_particle_set(0x811B, &actor->current.pos, &actor->shape_angle, &sp4C);

            for (int i = 0; i < 10; i++) {
                i_this->mHashiraActors[i] = NULL;
            }
        }

        mDoMtx_stack_c::transS(actor->current.pos.x, 3.0f + actor->current.pos.y, actor->current.pos.z);
        mDoMtx_stack_c::scaleM(l_HIO.field_0x30, l_HIO.field_0x30, l_HIO.field_0x30);

        for (int i = 0; i < 2; i++) {
            model = i_this->mpAttackEfModelMorf[i]->getModel();
            model->setBaseTRMtx(mDoMtx_stack_c::get());

            if (i == 1) {
                i_this->mpAttackEfModelMorf[i]->setPlaySpeed(i_this->field_0x798);
                cLib_addCalc2(&i_this->field_0x798, 0.5f + KREG_F(8), 1.0f, 0.08f + KREG_F(9));
            }

            i_this->mpAttackEfModelMorf[i]->play(NULL, 0, 0);
            i_this->mpAttackEfBtk[i]->play();

            if (i == 1) {
                i_this->mpAttackEfBrk->play();
            }

            i_this->mpAttackEfModelMorf[i]->modelCalc();
        }

        fire_range = (10.5f + KREG_F(6)) * (l_HIO.field_0x28 * i_this->mpAttackEfModelMorf[1]->getFrame());

        if (i_this->field_0x790 < (TREG_S(6) + 43)) {
            if (i_this->field_0x790 == (TREG_S(6) + 41)) {
                dComIfGp_getVibration().StopQuake(0x1F);
            }

            fopAc_ac_c* sp10 = (fopAc_ac_c*)dComIfGp_getPlayer(0);
            if (other_bg_check(i_this, sp10)) {
                i_this->mEffAtSph.OffAtVsPlayerBit();
            } else {
                i_this->mEffAtSph.OnAtVsPlayerBit();
            }

            i_this->mEffAtSph.SetC(actor->current.pos);
            i_this->mEffAtSph.SetR(fire_range);
            dComIfG_Ccsp()->Set(&i_this->mEffAtSph);

            i_this->field_0x7b6 = 2;
            fpcM_Search(s_hasira_eff_sub, i_this);
        }

        if (i_this->field_0x790 >= (TREG_S(7) + 70)) {
            i_this->field_0x790 = 0;
        }
    }

    if (i_this->mShadowKey != 353535 && i_this->field_0x7b8 > 0.02f) {
        GXColor light_col;
        light_col.r = l_HIO.field_0x34;
        light_col.g = l_HIO.field_0x36;
        light_col.b = l_HIO.field_0x38;
        light_col.a = 0xFF;

        cXyz light_pos(actor->current.pos);
        light_pos.y += 300.0f + TREG_F(12);

        f32 var_f26 = l_HIO.field_0x3c * i_this->field_0x7b8;
        dKy_BossLight_set(&light_pos, &light_col, var_f26, 0);
    }

    f32 var_f31;
    f32 var_f30;
    if (i_this->field_0x7b6 == 0) {
        var_f31 = 0.0f;
        var_f30 = 2.0f;
    } else if (i_this->field_0x7b6 == 1) {
        var_f31 = 1.7f + (2.0f + JREG_F(17)) + (0.5f * cM_ssin((i_this->mCounter * 6656)));
        var_f30 = 2.0f;
    } else if (i_this->field_0x7b6 == 2) {
        var_f31 = 30.0f;
        var_f30 = 2.0f;
    } else if (i_this->field_0x7b6 == 3) {
        var_f31 = 0.0f;
        var_f30 = 0.025f;
    }

    cLib_addCalc2(&i_this->field_0x7b8, var_f31, 1.0f, var_f30);
    cLib_addCalc0(&i_this->field_0x79c, 1.0f, 4.0f);

    cXyz sp34;
    model = i_this->mpFmModelMorf->getModel();

    MTXCopy(model->getAnmMtx(0x16), *calc_mtx);
    sp34.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp34, &i_this->mFootPos[0]);

    MTXCopy(model->getAnmMtx(0x19), *calc_mtx);
    MtxPosition(&sp34, &i_this->mFootPos[1]);
}

static int daE_FM_Execute(e_fm_class* i_this) {
    if (cDmrNowMidnaTalk()) {
        return 1;
    }

    if (i_this->mDemoCamMode == 1000) {
        return 1;
    }

    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    cXyz sp130;
    cXyz sp124;
    cXyz sp118;

    i_this->mCounter++;

    for (int i = 0; i < 4; i++) {
        if (i_this->mTimers[i] != 0) {
            i_this->mTimers[i]--;
        }
    }

    if (i_this->mDamageInvulnerabilityTimer != 0) {
        i_this->mDamageInvulnerabilityTimer--;
    }

    action(i_this);

    i_this->mAcch.CrrPos(dComIfG_Bgsp());
    actor->current.pos.y = actor->home.pos.y;

    mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y, actor->current.pos.z + ZREG_F(7));
    mDoMtx_stack_c::YrotM(actor->shape_angle.y);
    mDoMtx_stack_c::XrotM(actor->shape_angle.x);
    mDoMtx_stack_c::ZrotM(actor->shape_angle.z);
    mDoMtx_stack_c::scaleM(l_HIO.base_size, l_HIO.base_size, l_HIO.base_size);

    J3DModel* model = i_this->mpFmModelMorf->getModel();
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    i_this->mpFmModelMorf->play(&actor->eyePos, 0, 0);

    if (i_this->mAnm == BCK_FM_DEMOEND01 && i_this->mpFmModelMorf->checkFrame(1.0f)) {
        i_this->mSound.startCreatureSound(Z2SE_EN_FM_DEMOEND, 0, -1);
        i_this->mSound.startCreatureSound(Z2SE_EN_FM_DEMOEND_CORE, 0, -1);
        i_this->mSound.startCreatureVoice(Z2SE_EN_FM_V_DEMOEND, -1);
    } else if (i_this->mAnm == BCK_FM_CHANCE && i_this->mpFmModelMorf->checkFrame(20.0f)) {
        i_this->mSound.startCreatureVoice(Z2SE_EN_FM_V_CHANCE, -1);
    } else if (i_this->mAnm == BCK_FM_DAMAGEWALK && (i_this->mpFmModelMorf->checkFrame(1.0f) || i_this->mpFmModelMorf->checkFrame(30.0f))) {
        i_this->mSound.startCreatureVoice(Z2SE_EN_FM_V_DAMAGEWALK, -1);
    } else if (i_this->mAnm == BCK_FM_DOWNWAIT && i_this->mpFmModelMorf->checkFrame(5.0f)) {
        i_this->mSound.startCreatureVoice(Z2SE_EN_FM_V_DOWNWAIT, -1);
    } else if (i_this->mAnm == BCK_FM_KYORO2 && (i_this->mpFmModelMorf->checkFrame(1.0f) || i_this->mpFmModelMorf->checkFrame(30.0f))) {
        i_this->mSound.startCreatureVoice(Z2SE_EN_FM_V_KYORO, -1);
    } else if (i_this->mAnm == BCK_FM_UP02 && i_this->mpFmModelMorf->checkFrame(145.0f)) {
        i_this->mSound.startCreatureVoice(Z2SE_EN_FM_V_UP02_2, -1);
    } else if (i_this->mAnm == BCK_FM_WAIT01 && i_this->mpFmModelMorf->checkFrame(13.0f)) {
        i_this->mSound.startCreatureVoice(Z2SE_EN_FM_V_WAIT, -1);
    } else if (i_this->mAnm == BCK_FM_ANIMAL02 && i_this->mpFmModelMorf->checkFrame(1.0f)) {
        i_this->mSound.startCreatureVoice(Z2SE_EN_FM_V_GAOO_LONG, -1);
    } else if (i_this->mAnm == BCK_FM_HANGWAIT && i_this->mpFmModelMorf->checkFrame(1.0f)) {
        i_this->mSound.startCreatureVoice(Z2SE_EN_FM_V_HANGWAIT, -1);
    }

    if (i_this->mDrawDemoModel) {
        mDoMtx_stack_c::scaleM(i_this->mDemoModelSize, i_this->mDemoModelSize, i_this->mDemoModelSize);
        cLib_addCalc2(&i_this->mDemoModelSize, 0.65f + KREG_F(11), 0.2f, i_this->mDemoModelSizeStep);
        cLib_addCalc2(&i_this->mDemoModelSizeStep, 0.02f + KREG_F(13), 1.0f, 0.001f + KREG_F(14));

        i_this->mpDemoFmModelMorf->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());
        i_this->mpDemoFmBrk->play();
        i_this->mpDemoFmModelMorf->play(&actor->eyePos, 0, 0);
        i_this->mpDemoFmModelMorf->modelCalc();

        dComIfGs_offOneZoneSwitch(0, -1);
    }

    i_this->mpFmBtk[i_this->mPlayTexAnmNo]->play();
    i_this->mpFmBrk[i_this->mPlayTexAnmNo]->play();

    if (i_this->mPlayTexAnmNo != TEXANM_FM && i_this->mPlayTexAnmNo != TEXANM_PUTOUT_WAIT) {
        if (i_this->mpFmBrk[i_this->mPlayTexAnmNo]->getFrame() > i_this->mpFmBrk[i_this->mPlayTexAnmNo]->getEndFrame() - 2.0f) {
            i_this->mpFmBrk[i_this->mPlayTexAnmNo]->setFrame(i_this->mpFmBrk[i_this->mPlayTexAnmNo]->getEndFrame() - 2.0f);
        }

        if (i_this->mpFmBtk[i_this->mPlayTexAnmNo]->getFrame() > i_this->mpFmBtk[i_this->mPlayTexAnmNo]->getEndFrame() - 2.0f) {
            i_this->mpFmBtk[i_this->mPlayTexAnmNo]->setFrame(i_this->mpFmBtk[i_this->mPlayTexAnmNo]->getEndFrame() - 2.0f);
        }
    }

    i_this->mpFmModelMorf->modelCalc();

    cXyz sp10C(-20000.0f, 20000.0f, 20000.0f);
    i_this->field_0x1aff0 = 0;

    MTXCopy(model->getAnmMtx(3), *calc_mtx);
    sp130.set(20.0f, 0.0f, 0.0f);
    MtxPosition(&sp130, &sp124);
    i_this->mCcSph[0].SetC(sp124);
    i_this->mCcSph[0].SetR(120.0f + KREG_F(17));
    dComIfG_Ccsp()->Set(&i_this->mCcSph[0]);

    sp130.set(0.0f, 0.0f, 0.0f);
    MTXCopy(model->getAnmMtx(1), *calc_mtx);
    sp130.set(100.0f + VREG_F(0), VREG_F(1), VREG_F(2));
    MtxPosition(&sp130, &sp124);
    i_this->mCcSph[1].SetC(sp124);
    i_this->mCcSph[1].SetR(170.0f + VREG_F(3));
    dComIfG_Ccsp()->Set(&i_this->mCcSph[1]);

    MTXCopy(model->getAnmMtx(3), *calc_mtx);
    sp130.set(0.0f, 0.0f, 0.0f);
    i_this->mpCoreModel->setBaseTRMtx(*calc_mtx);

    f32 brk_frame = 0.0f;
    if (i_this->field_0x770 != 0) {
        brk_frame = 29.0f;
        i_this->mSound.startCreatureSoundLevel(Z2SE_EN_FM_CORE_GLOW, 0, -1);
        i_this->field_0x1834 = 1.0f;
    } else {
        cLib_addCalc0(&i_this->field_0x1834, 1.0f, 0.05f);
    }

    cLib_addCalc2(&i_this->mCoreBrkFrame, brk_frame, 1.0f, 1.0f);

    if (!(i_this->mCoreBrkFrame > 0.0f) || i_this->field_0x770 >= 9970) {
        i_this->field_0x1834 = 0.0f;
    }

    if (i_this->field_0x1834 > 0.0001f) {
        if (i_this->field_0x1834 <= 0.05f) {
            i_this->field_0x1834 = 0.0f;
        }

        cXyz sp100;
        sp130.set(10.0f + (135.0f + KREG_F(7)), -25.0f + KREG_F(8), KREG_F(9));
        MtxPosition(&sp130, &sp100);

        if (i_this->mAction == ACTION_END || i_this->mCoreBrkFrame < 2.0f) {
            sp100.y += 20000.0f;
        }

        JGeometry::TVec3<f32> spF4;
        spF4.x = spF4.y = spF4.z = i_this->field_0x1834;

        if (i_this->mAction == ACTION_START) {
            for (int i = 0; i < 2; i++) {
                static u16 cg_name[] = {0x842C, 0x8496};
                i_this->field_0x2374[i] = dComIfGp_particle_set(i_this->field_0x2374[i], cg_name[i], &sp100, NULL, NULL);

                JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->field_0x2374[i]);
                if (emitter != NULL) {
                    JGeometry::TVec3<f32> spE8;
                    spE8.x = spE8.y = spE8.z = i_this->field_0x1834;
                    emitter->setGlobalScale(spE8);
                }
            }
        } else {
            for (int i = 0; i < 6; i++) {
                static u16 cg_name[] = {0x840F, 0x847F, 0x8726, 0x8727, 0x8728, 0x8729};
                i_this->field_0x2374[i] = dComIfGp_particle_set(i_this->field_0x2374[i], cg_name[i], &sp100, NULL, NULL);

                JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->field_0x2374[i]);
                if (emitter != NULL) {
                    emitter->setGlobalScale(spF4);
                    model = i_this->mpFmModelMorf->getModel();
                    emitter->setGlobalRTMatrix(model->getAnmMtx(3));
                }
            }
        }
    }

    i_this->mpCoreBrk->setFrame(i_this->mCoreBrkFrame);
    i_this->mpCoreBtk->play();

    sp130.set((10.0f + (135.0f + KREG_F(7))) - 20.0f, -25.0f + KREG_F(8), KREG_F(9));
    MtxPosition(&sp130, &actor->eyePos);
    actor->attention_info.position = actor->eyePos;
    actor->attention_info.position.y += 30.0f;
    sp124 = actor->eyePos;

    if (i_this->field_0x770 != 0) {
        if (i_this->field_0x770 > 1) {
            i_this->field_0x770--;
        }
    } else {
        sp124.x += 20000.0f;
    }

    i_this->mCoreSph.SetC(sp124);
    i_this->mCoreSph.SetR(10.0f + (40.0f + YREG_F(3)));
    dComIfG_Ccsp()->Set(&i_this->mCoreSph);

    sp130.set(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < 6; i++) {
        static int hl_d[] = {21, 24, 7, 8, 14, 15};
        static f32 hl_s[] = {50.0f, 50.0f, 50.0f, 50.0f, 50.0f, 50.0f};

        MTXCopy(model->getAnmMtx(hl_d[i]), *calc_mtx);
        MtxPosition(&sp130, &sp124);
        i_this->mCcSph[i + 2].SetC(sp124);

        if (i_this->field_0x182b != 0) {
            i_this->mCcSph[i + 2].SetR(2.2f * hl_s[i]);
        } else if (i_this->mAction == ACTION_DOWN) {
            i_this->mCcSph[i + 2].SetR((1.8f + KREG_F(11)) * hl_s[i]);
        } else {
            i_this->mCcSph[i + 2].SetR(hl_s[i]);
        }

        dComIfG_Ccsp()->Set(&i_this->mCcSph[i + 2]);
    }

    for (int i = 0; i < 8; i++) {
        if (i_this->field_0x792 == 0) {
            i_this->mCcSph[i].OffAtVsPlayerBit();
        } else {
            i_this->mCcSph[i].OnAtVsPlayerBit();
        }

        if (i_this->field_0x182b != 0) {
            i_this->mCcSph[i].SetAtType(0x100000);
        } else {
            i_this->mCcSph[i].SetAtType(0x400);
        }
    }

    i_this->field_0x182b = 0;

    if (i_this->field_0x1829 != 0) {
        i_this->mAtSph.SetR(60.0f);

        if (i_this->field_0x1828 == 0) {
            MTXCopy(model->getAnmMtx(8), *calc_mtx);
        } else if (i_this->field_0x1828 == 1) {
            MTXCopy(model->getAnmMtx(0xF), *calc_mtx);
        } else if (i_this->field_0x1828 == 2) {
            MTXCopy(model->getAnmMtx(3), *calc_mtx);
            MtxTrans((550.0f + KREG_F(13)) * i_this->field_0x1830, (100.0f + KREG_F(14)) * i_this->field_0x1830, KREG_F(15), 1);
            i_this->mAtSph.SetR(80.0f);

            i_this->field_0x1830 += 0.18f;
            if (i_this->field_0x1830 > 1.0f) {
                i_this->field_0x1830 -= 1.0f;
            }
        }

        MtxPosition(&sp130, &sp124);

        if (i_this->field_0x182a == 0) {
            i_this->field_0x182a = 1;
            i_this->mAtSph.StartCAt(sp124);
        } else {
            i_this->mAtSph.MoveCAt(sp124);
        }

        dComIfG_Ccsp()->Set(&i_this->mAtSph);
    } else {
        i_this->field_0x182a = 0;
    }

    effect_set(i_this);

    if (i_this->mShadowKey != 353535) {
        i_this->mPlight.mPosition = actor->current.pos;
        i_this->mPlight.mPosition.y += 200.0f;
        i_this->mPlight.mColor.r = 175;
        i_this->mPlight.mColor.g = 93;
        i_this->mPlight.mColor.b = 0;
        i_this->mPlight.mPow = l_HIO.field_0x44 * l_HIO.base_size;
        i_this->mPlight.mFluctuation = 1.0f;
    } else {
        i_this->mPlight.mPow = 0.0f;
    }

    for (int i = 0; i < 4; i++) {
        if (i_this->field_0x1b080 != 0) {
            if (i <= 1) {
                i_this->mChain[i].field_0x61d0 = 21;
            } else {
                i_this->mChain[i].field_0x61d0 = 18;
            }
        }

        static int sisi_joint[] = {8, 15, 22, 25};
        MTXCopy(model->getAnmMtx(sisi_joint[i]), *calc_mtx);

        if (i == 0) {
            sp130.set(-25.0f, 20.0f, 50.0f);
        } else if (i == 1) {
            sp130.set(-25.0f, 20.0f, -50.0f);
        } else if (i == 2) {
            sp130.set(-5.0f, 30.0f, -35.0f);
        } else if (i == 3) {
            sp130.set(-5.0f, 30.0f, 35.0f);
        }

        MtxPosition(&sp130, &i_this->mChain[i].field_0x68);

        if ((i_this->field_0x2399 != 1 || i != 0) && (i_this->field_0x2399 != 2 || i != 1)) {
            chain_control1(i_this, &i_this->mChain[i], i);
            chain_control2(i_this, &i_this->mChain[i], i);
            chain_control3(i_this, &i_this->mChain[i], i);
        }

        static int chain_hit_d[] = {21, 15, 9, 3};

        if (i < 2 && i == i_this->field_0x1828 && i_this->field_0x1829 != 0) {
            if (i_this->field_0x1ab0c[i] == 0) {
                i_this->field_0x1ab0c[i] = 1;

                for (int j = 0; j < 4; j++) {
                    if (chain_hit_d[j] < i_this->mChain[i].field_0x61d0) {
                        i_this->mChainAtSph[j].StartCAt(i_this->mChain[i].pos[chain_hit_d[j]]);
                    } else {
                        i_this->mChainAtSph[j].StartCAt(sp10C);
                    }
                }
            } else {
                for (int j = 0; j < 4; j++) {
                    if (chain_hit_d[j] < i_this->mChain[i].field_0x61d0) {
                        i_this->mChainAtSph[j].MoveCAt(i_this->mChain[i].pos[chain_hit_d[j]]);

                        if (j == 2) {
                            sp130 = i_this->mChain[i].pos[chain_hit_d[j]];
                            if (sp130.y < 100.0f) {
                                hasira_hahen_hit(&sp130, 500.0f + VREG_F(17), 100.0f + VREG_F(18), i_this->field_0x1828);
                            }
                        }
                    } else {
                        i_this->mChainAtSph[j].MoveCAt(sp10C);
                    }

                    dComIfG_Ccsp()->Set(&i_this->mChainAtSph[j]);

                    if (daPy_getPlayerActorClass()->checkEquipHeavyBoots()) {
                        i_this->mChainAtSph[j].SetAtSpl((dCcG_At_Spl)2);
                    } else {
                        i_this->mChainAtSph[j].SetAtSpl((dCcG_At_Spl)14);
                    }
                }
            }
        }

        i_this->mChain[i].field_0x6094 = i_this->mChain[i].sound_pos;
        i_this->mChain[i].sound_pos = i_this->mChain[i].pos[(i_this->mChain[i].field_0x61d0 / 2) + 4];
    }

    if (i_this->field_0x2399 != 0) {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 3; j++) {
                static u16 chain_at_id[] = {0x8724, 0x8725};
                static int c_id[] = {10, 12, 14};

                dComIfGp_particle_set(chain_at_id[i], &i_this->mChain[i_this->field_0x2399 - 1].pos[c_id[j]], NULL, NULL);
            }
        }

        i_this->field_0x2399 = 0;
    }

    i_this->field_0x1829 = 0;
    i_this->mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));

    if (!i_this->checkCameraFree()) {
        if (fopAcM_searchPlayerDistance(actor) < l_HIO.field_0xa8) {
            dComIfGs_onOneZoneSwitch(0, -1);
        } else {
            dComIfGs_offOneZoneSwitch(0, -1);
        }
    }

    if (i_this->field_0x2398 != 0) {
        i_this->field_0x2398--;
        i_this->field_0x182c = 1;
    }

    if (i_this->field_0x182c != 0) {
        if (i_this->field_0x182c == 1) {
            for (int i = 0; i < 3; i++) {
                static u16 core_id[] = {0x8410, 0x8411, 0x8480};
                i_this->field_0x238c[i] = dComIfGp_particle_set(i_this->field_0x238c[i], core_id[i], &actor->eyePos, NULL, NULL);

                if (i == 2) {
                    JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->field_0x238c[i]);
                    if (emitter != NULL) {
                        model = i_this->mpFmModelMorf->getModel();
                        emitter->setGlobalRTMatrix(model->getAnmMtx(3));
                    }
                }
            }
        }

        if (i_this->field_0x182c == 2) {
            dComIfGp_particle_set(0x8491, &actor->eyePos, NULL, NULL);
            fopAcM_createDisappear(actor, &actor->eyePos, 5, 0, 0xFF);
            mDoAud_seStart(Z2SE_EN_BQ_EYE_EXPLODE, NULL, 0, 0);
            dComIfGp_getVibration().StartShock(VIBMODE_S_POWER5, 1, cXyz(0.0f, 1.0f, 0.0f));

            cXyz spDC(1.0f, 1.0f, 1.0f);
            csXyz rot(actor->shape_angle);
            rot.y += 0x800;

            fopAcM_createItemForBoss(&actor->eyePos, fpcNm_ITEM_UTAWA_HEART, fopAcM_GetRoomNo(actor), &rot, NULL, 10.0f + KREG_F(19), 30.0f, -1);
        }

        if (i_this->field_0x182c == 3) {
            for (int i = 0; i < 3; i++) {
                static u16 f_id[] = {0x8412, 0x8413, 0x8414};
                JPABaseEmitter* emitter = dComIfGp_particle_set(f_id[i], &actor->current.pos, NULL, NULL);
            }

            i_this->mSound.startCreatureSound(Z2SE_EN_FM_SAITENKA, 0, -1);
        }

        i_this->field_0x182c = 0;
    }

    if (i_this->mAnm == BCK_FM_DEMOEND01) {
        int anm_frame = i_this->mpFmModelMorf->getFrame();
        if (anm_frame == 90 || anm_frame == 120) {
            dComIfGp_getVibration().StartShock(VIBMODE_S_POWER3, 1, cXyz(0.0f, 1.0f, 0.0f));
        } else if (anm_frame == 108 || anm_frame == 196 || anm_frame == 300 || anm_frame == 340) {
            dComIfGp_getVibration().StartShock(VIBMODE_S_POWER5, 1, cXyz(0.0f, 1.0f, 0.0f));
        }

        for (int i = 0; i < 8; i++) {
            static u16 end_id[] = {0x8492, 0x8493, 0x8494, 0x8495, 0x86D6, 0x86D7, 0x86D8, 0x86D9};
            i_this->field_0x2354[i] = dComIfGp_particle_set(i_this->field_0x2354[i], end_id[i], &actor->eyePos, NULL, NULL);

            JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->field_0x2354[i]);
            if (emitter != NULL) {
                emitter->setGlobalRTMatrix(model->getAnmMtx(3));
            }
        }
    } else if (i_this->mAnm == BCK_FM_OPDEMO) {
        i_this->field_0x2350 = dComIfGp_particle_set(i_this->field_0x2350, 0x872C, &actor->current.pos, NULL, NULL);
        
        JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->field_0x2350);
        if (emitter != NULL) {
            model = i_this->mpFmModelMorf->getModel();
            emitter->setGlobalRTMatrix(model->getAnmMtx(3));
        }
    }

    if (i_this->mAction == ACTION_END) {
        cXyz spD0(20000.0f, -23000.0f, 40000.0f);
        for (int i = 0; i <= 7; i++) {
            i_this->mCcSph[i].SetC(spD0);
        }

        i_this->mAtSph.SetC(spD0);
        i_this->mCoreSph.SetC(spD0);
        i_this->mEffAtSph.SetC(spD0);

        for (int i = 0; i < 8; i++) {
            i_this->mFEffAtSph[i].SetC(spD0);
        }

        for (int i = 0; i < 4; i++) {
            i_this->mChainAtSph[i].SetC(spD0);
        }
    }

    f32 blend_target = 0.0f;
    f32 blend_step = 0.05f;

    int sp3C = i_this->field_0x5c8;
    if (sp3C == 0) {
        dKy_custom_colset(0, 1, i_this->mKankyoBlend);
    } else if (sp3C == 1) {
        dKy_custom_colset(0, 1, i_this->mKankyoBlend);
        blend_target = 1.0f;
        blend_step = 0.1f;

        if (i_this->mKankyoBlend == 1.0f) {
            i_this->field_0x5c8 = 2;
            i_this->mKankyoBlend = 0.0f;
        }
    } else if (sp3C == 2) {
        dKy_custom_colset(1, 3, i_this->mKankyoBlend);
        if (i_this->field_0x7b6 == 2) {
            blend_target = 1.0f;
            blend_step = 0.2f;
        } else {
            blend_target = 0.0f;
        }
    } else if (sp3C == 3) {
        dKy_custom_colset(2, 1, i_this->mKankyoBlend);
        blend_target = 0.0f;
        blend_step = 0.02f;
    } else if (sp3C == 4) {
        dKy_custom_colset(1, 3, i_this->mKankyoBlend);
        blend_target = 1.0f;
        blend_step = 0.2f;
    }

    cLib_addCalc2(&i_this->mKankyoBlend, blend_target, 1.0f, blend_step);

    if (l_HIO.field_0xac != 0) {
        i_this->field_0x1b07c = 1;
    }

    return 1;
}

static int daE_FM_IsDelete(e_fm_class* i_this) {
    return 1;
}

static int daE_FM_Delete(e_fm_class* i_this) {
    fopAcM_GetID(i_this);
    dComIfG_resDelete(&i_this->mPhase, "E_fm");

    if (i_this->mInitHIO != 0) {
        hio_set = 0;
        mDoHIO_DELETE_CHILD(l_HIO.no);
    }

    dKy_plight_cut(&i_this->mPlight);

    if (i_this->heap != NULL) {
        i_this->mSound.deleteObject();
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    J3DModelData* modelData;
    e_fm_class* a_this = (e_fm_class*)i_this;

    a_this->mpFmModelMorf = new mDoExt_McaMorf(static_cast<J3DModelData*>(dComIfG_getObjectRes("E_fm", BMDR_FM)), NULL, NULL,
                                               static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("E_fm", BCK_FM_WAIT01)), 0, 1.0f, 0, -1, 1, NULL, 0x80000, 0x11000284);
    if (a_this->mpFmModelMorf == NULL || a_this->mpFmModelMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* fm_model = a_this->mpFmModelMorf->getModel();
    fm_model->setUserArea((uintptr_t)a_this);

    for (u16 i = 0; i < fm_model->getModelData()->getJointNum(); i++) {
        if (i == 1 || i == 2) {
            fm_model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
        }
    }

    int anm_mode;
    for (int i = 0; i < 9; i++) {
        a_this->mpFmBrk[i] = new mDoExt_brkAnm();
        if (a_this->mpFmBrk[i] == NULL) {
            return 0;
        }

        if (i >= 7) {
            anm_mode = 0;
        } else {
            anm_mode = 2;
        }

        static int brk[] = {
            BRK_FM, BRK_FM_ATTACK, BRK_FM_PUTOUT, BRK_FM_PUTOUTWAIT, BRK_FM_ANIMAL02, 
            BRK_FM_OPDEMO, BRK_FM_HANGWAIT, BRK_FM_DEMOEND01, BRK_FM_DEMOEND02
        };
        if (a_this->mpFmBrk[i]->init(a_this->mpFmModelMorf->getModel()->getModelData(),
                                     static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes("E_fm", brk[i])),
                                     1, anm_mode, 1.0f, 0, -1) == FALSE) {
            return 0;
        }

        a_this->mpFmBtk[i] = new mDoExt_btkAnm();
        if (a_this->mpFmBtk[i] == NULL) {
            return 0;
        }

        static int btk[] = {
            BTK_FM, BTK_FM_ATTACK, BTK_FM_PUTOUT, BTK_FM_PUTOUTWAIT, BTK_FM_ANIMAL02, 
            BTK_FM_OPDEMO, BTK_FM_HANGWAIT, BTK_FM_DEMOEND01, BTK_FM_DEMOEND02
        };
        if (a_this->mpFmBtk[i]->init(a_this->mpFmModelMorf->getModel()->getModelData(),
                                     static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes("E_fm", btk[i])),
                                     1, anm_mode, 1.0f, 0, -1) == FALSE) {
            return 0;
        }
    }

    a_this->mpDemoFmModelMorf = new mDoExt_McaMorf(static_cast<J3DModelData*>(dComIfG_getObjectRes("E_fm", BMDR_FM)), NULL, NULL,
                                                   static_cast<J3DAnmTransform*>(dComIfG_getObjectRes("E_fm", BCK_FM_DEMOEND03)), 0, 1.0f, 0, -1, 1, NULL, 0x80000, 0x11000284);
    if (a_this->mpDemoFmModelMorf == NULL || a_this->mpDemoFmModelMorf->getModel() == NULL) {
        return 0;
    }

    a_this->mpDemoFmBrk = new mDoExt_brkAnm();
    if (a_this->mpDemoFmBrk == NULL) {
        return 0;
    }

    if (a_this->mpDemoFmBrk->init(a_this->mpFmModelMorf->getModel()->getModelData(),
                                  static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes("E_fm", BRK_FM_DEMOEND03)),
                                  1, 0, 1.0f, 0, -1) == FALSE) {
        return 0;
    }

    modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes("E_fm", BMDE_FM_CORE));
    JUT_ASSERT(7257, modelData != NULL);
    a_this->mpCoreModel = mDoExt_J3DModel__create(modelData, 0, 0x11020203);
    if (a_this->mpCoreModel == NULL) {
        return 0;
    }

    a_this->mpCoreBrk = new mDoExt_brkAnm();
    if (a_this->mpCoreBrk == NULL) {
        return 0;
    }

    if (a_this->mpCoreBrk->init(a_this->mpCoreModel->getModelData(),
                                static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes("E_fm", BRK_CORE_LIGHTON)),
                                1, 2, 1.0f, 0, -1) == FALSE) {
        return 0;
    }

    a_this->mpCoreBtk = new mDoExt_btkAnm();
    if (a_this->mpCoreBtk == NULL) {
        return 0;
    }

    if (a_this->mpCoreBtk->init(a_this->mpCoreModel->getModelData(),
                                static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes("E_fm", BTK_CORE_BEAT)),
                                1, 0, 1.0f, 0, -1) == FALSE) {
        return 0;
    }

    a_this->mpCoreBtk->setPlaySpeed(0.0f);

    for (int i = 0; i < 2; i++) {
        static int eff_bmd[] = {BMDR_EF_FMATTACK_A, BMDR_EF_FMATTACK_B};
        a_this->mpAttackEfModelMorf[i] = new mDoExt_McaMorf(static_cast<J3DModelData*>(dComIfG_getObjectRes("E_fm", eff_bmd[i])), NULL, NULL, NULL, 0, 1.0f, 0, -1, 1, NULL, 0x80000, 0x11000284);
        if (a_this->mpAttackEfModelMorf[i] == NULL || a_this->mpAttackEfModelMorf[i]->getModel() == NULL) {
            return 0;
        }

        if (i != 0) {
            a_this->mpAttackEfBrk = new mDoExt_brkAnm();
            if (a_this->mpAttackEfBrk == NULL) {
                return 0;
            }

            if (a_this->mpAttackEfBrk->init(a_this->mpAttackEfModelMorf[i]->getModel()->getModelData(),
                                            static_cast<J3DAnmTevRegKey*>(dComIfG_getObjectRes("E_fm", BRK_EF_FMATTACK_B)),
                                            1, 2, 1.0f, 0, -1) == FALSE) {
                return 0;
            }
        }

        a_this->mpAttackEfBtk[i] = new mDoExt_btkAnm();
        if (a_this->mpAttackEfBtk[i] == NULL) {
            return 0;
        }

        static int eff_btk[] = {BTK_EF_FMATTACK_A, BTK_EF_FMATTACK_B};
        if (a_this->mpAttackEfBtk[i]->init(a_this->mpAttackEfModelMorf[i]->getModel()->getModelData(),
                                           static_cast<J3DAnmTextureSRTKey*>(dComIfG_getObjectRes("E_fm", eff_btk[i])),
                                           1, 2, 1.0f, 0, -1) == FALSE) {
            return 0;
        }
    }

    modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes("E_fm", BMDR_KUSARI));
    JUT_ASSERT(7404, modelData != NULL);
    
    J3DModelData* modelData2 = static_cast<J3DModelData*>(dComIfG_getObjectRes("E_fm", BMDR_HANDLE));
    JUT_ASSERT(7408, modelData2 != NULL);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 26; j++) {
            if (j == 24) {
                a_this->mChain[i].model[j] = mDoExt_J3DModel__create(modelData2, 0x80000, 0x11000084);
            } else {
                a_this->mChain[i].model[j] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
            }

            if (a_this->mChain[i].model[j] == NULL) {
                return 0;
            }

            dKy_tevstr_init(&a_this->mChain[i].tevstr[j], (int) fopAcM_GetRoomNo(i_this), 0xFF);
        }

        a_this->mChain[i].field_0x61d0 = 26;
        a_this->mChain[i].field_0x619e = -10000;
        a_this->mChain[i].field_0x61a0 = -100.0f;
        a_this->mChain[i].field_0x61a4 = 30.0f;
    }

    return 1;
}

static int daE_FM_Create(fopAc_ac_c* i_this) {
    e_fm_class* a_this = (e_fm_class*)i_this;
    fopAcM_ct(a_this, e_fm_class);

    static dCcD_SrcSph cc_sph_src = {
        {
            {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0x1d}, {0x0, 0x0}, 0x79}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
        } // mSphAttr
    };

    static dCcD_SrcSph at_sph_src = {
        {
            {0x0, {{AT_TYPE_CSTATUE_SWING, 0x2, 0x1d}, {0x0, 0x0}, 0x0}}, // mObj
            {dCcD_SE_NONE, 0x0, 0xe, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 35.0f} // mSph
        } // mSphAttr
    };

    static dCcD_SrcSph core_sph_src = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0x2002, 0x3}, 0x0}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
        } // mSphAttr
    };

    static dCcD_SrcSph eff_at_sph_src = {
        {
            {0x0, {{AT_TYPE_CSTATUE_SWING, 0x4, 0x1f}, {0x0, 0x0}, 0x0}}, // mObj
            {dCcD_SE_NONE, 0x0, 0xe, dCcD_MTRL_FIRE, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 35.0f} // mSph
        } // mSphAttr
    };

    static dCcD_SrcSph f_eff_at_sph_src = {
        {
            {0x0, {{AT_TYPE_CSTATUE_SWING, 0x2, 0xd}, {0x0, 0x0}, 0x0}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x0, dCcD_MTRL_FIRE, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x1, 0x0, 0x2}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 35.0f} // mSph
        } // mSphAttr
    };

    static dCcD_SrcSph chain_at_sph_src = {
        {
            {0x0, {{AT_TYPE_CSTATUE_BOSS_SWING, 0x2, 0x1f}, {0x0, 0x0}, 0x0}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x2, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 90.0f} // mSph
        } // mSphAttr
    };

    static dCcD_SrcSph chain_co_sph_src = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x3}, 0x75}}, // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
            {dCcD_SE_METAL, 0x2, 0x0, 0x0, 0x3}, // mGObjTg
            {0x0}, // mGObjCo
        }, // mObjInf
        {
            {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
        } // mSphAttr
    };

    int phase_state = dComIfG_resLoad(&a_this->mPhase, "E_fm");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("E_FM PARAM %x\n", fopAcM_GetParam(i_this));

        if (dComIfGs_isStageBossEnemy()) {
            OS_REPORT("E_FM やられ後なので再セットしません\n");
            cXyz pos(-363.0f, 0.0f, 291.0f);
            csXyz rot(0, 0, 0);
            fopAcM_createWarpHole(&pos, &rot, fopAcM_GetRoomNo(i_this), 0, 0, 0xFF);

            if (!dComIfGs_isStageLife()) {
                cXyz size(1.0f, 1.0f, 1.0f);
                pos.set(209.0f, 0.0f, 861.0f);
                fopAcM_createItemForBoss(&pos, fpcNm_ITEM_UTAWA_HEART, fopAcM_GetRoomNo(i_this), &i_this->shape_angle, &size, 0.0f, 0.0f, -1);
            }

            g_env_light.mColpatWeather = 2;
            g_env_light.wether_pat0 = 2;
            g_env_light.wether_pat1 = 2;
            dComIfGs_BossLife_public_Set(0);
            return cPhs_ERROR_e;
        }

        a_this->arg0 = fopAcM_GetParam(i_this);
        if (a_this->arg0 == 0xFF) {
            a_this->arg0 = 0;
        }

        a_this->arg1 = (fopAcM_GetParam(i_this) & 0xFF00) >> 8;
        if (a_this->arg1 == 0xFF) {
            a_this->arg1 = 1;
        }

        int arg2 = (fopAcM_GetParam(i_this) & 0xFF0000) >> 0x10;
        a_this->arg3 = (fopAcM_GetParam(i_this) & 0xFF000000) >> 0x18;

        OS_REPORT("E_FM//////////////E_FM SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, 0x4B000)) {
            OS_REPORT("//////////////E_FM SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////E_FM SET 2 !!\n");

        if (!hio_set) {
            a_this->mInitHIO = TRUE;
            hio_set = TRUE;
            l_HIO.no = mDoHIO_CREATE_CHILD("火魔人", &l_HIO);
        }

        i_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;

        fopAcM_SetMtx(i_this, a_this->mpFmModelMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(i_this, -200.0f, -200.0f, -200.0f);
        fopAcM_SetMax(i_this, 200.0f, 200.0f, 200.0f);

        a_this->mAcch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1, &a_this->mAcchCir, fopAcM_GetSpeed_p(i_this), NULL, NULL);
        a_this->mAcchCir.SetWall(100.0f, 400.0f);

        i_this->health = 50;
        i_this->field_0x560 = 50;

        a_this->field_0xa24.Init(0xFA, 0, i_this);

        for (int i = 0; i <= 7; i++) {
            a_this->mCcSph[i].Set(cc_sph_src);
            a_this->mCcSph[i].SetStts(&a_this->field_0xa24);
            a_this->mCcSph[i].SetAtType(0x100);
            a_this->mCcSph[i].SetAtMtrl(dCcD_MTRL_FIRE);
        }

        a_this->mAtSph.Set(at_sph_src);
        a_this->mAtSph.SetStts(&a_this->field_0xa24);
        a_this->mAtSph.SetAtType(0x100);
        a_this->mAtSph.SetAtMtrl(dCcD_MTRL_FIRE);

        a_this->field_0xa60.Init(0xA, 0, i_this);

        a_this->mCoreSph.Set(core_sph_src);
        a_this->mCoreSph.SetStts(&a_this->field_0xa60);

        a_this->mEffAtSph.Set(eff_at_sph_src);
        a_this->mEffAtSph.SetStts(&a_this->field_0xa24);
        a_this->mEffAtSph.SetAtType(0x100);
        a_this->mEffAtSph.SetAtMtrl(dCcD_MTRL_FIRE);

        for (int i = 0; i < 8; i++) {
            a_this->mFEffAtSph[i].Set(f_eff_at_sph_src);
            a_this->mFEffAtSph[i].SetStts(&a_this->field_0xa24);
            a_this->mFEffAtSph[i].SetAtType(0x100);
            a_this->mFEffAtSph[i].SetAtMtrl(dCcD_MTRL_FIRE);
        }

        for (int i = 0; i < 4; i++) {
            a_this->mChainAtSph[i].Set(chain_at_sph_src);
            a_this->mChainAtSph[i].SetStts(&a_this->field_0xa24);
        }

        a_this->mCounter = cM_rndF(65535.0f);
        dKy_plight_set(&a_this->mPlight);
    
        i_this->attention_info.distances[fopAc_attn_BATTLE_e] = 75;

        a_this->mSound.init(&i_this->current.pos, &i_this->eyePos, &a_this->mChain[0].sound_pos, &a_this->mChain[1].sound_pos, &a_this->mChain[2].sound_pos, &a_this->mChain[3].sound_pos, 6, 1, 1, 1, 1, 1);
        a_this->mAtInfo.mpSound = &a_this->mSound;

        if (cDmr_SkipInfo != 0) {
            cDmr_SkipInfo = 0;
            a_this->mAction = ACTION_NORMAL;
            a_this->mMode = -10;
            a_this->mTimers[0] = 20;
            Z2GetAudioMgr()->bgmStart(Z2BGM_BOSSFIREMAN_0, 0, 0);
            a_this->field_0x5c8 = 2;
            a_this->mKankyoBlend = 0.0f;
            a_this->field_0x792 = 1;
            a_this->mpCoreBtk->setPlaySpeed(1.0f);
            a_this->field_0x770 = 1;
            a_this->field_0x1b080 = 1;
        } else {
            a_this->mAction = ACTION_START;
        }

        a_this->field_0x794 = 1.0f;
        a_this->field_0x778 = 1.0f;

        /* dSv_event_flag_c::F_0254 - For E3 2006 - Hit boss's weak spot at least once */
        dComIfGs_offEventBit(dSv_event_flag_c::saveBitLabels[254]);
        /* dSv_event_flag_c::F_0256 - For E3 2006 - Knocked down boss at leased once */
        dComIfGs_offEventBit(dSv_event_flag_c::saveBitLabels[256]);
        /* dSv_event_flag_c::F_0669 - Goron Mines - Pulled on Fyrus chains at least once */
        dComIfGs_offEventBit(dSv_event_flag_c::saveBitLabels[669]);
        /* dSv_event_flag_c::F_0670 - Goron Mines - Hitting knocked-down Fyrus */
        dComIfGs_offEventBit(dSv_event_flag_c::saveBitLabels[670]);
        demo_stop = 0;

        daE_FM_Execute(a_this);
    }

    return phase_state;
}

e_fm_class::e_fm_class() {}

static actor_method_class l_daE_FM_Method = {
    (process_method_func)daE_FM_Create,
    (process_method_func)daE_FM_Delete,
    (process_method_func)daE_FM_Execute,
    (process_method_func)daE_FM_IsDelete,
    (process_method_func)daE_FM_Draw,
};

actor_process_profile_definition g_profile_E_FM = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_E_FM,              // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(e_fm_class),     // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  158,                    // mPriority
  &l_daE_FM_Method,       // sub_method
  0x00044000,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};

AUDIO_INSTANCES;
