/**
 * @file d_a_b_gnd.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_b_gnd.h"
#include "d/d_com_inf_game.h"
#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_horse.h"
#include "d/d_s_play.h"
#include "f_op/f_op_msg_mng.h"
#include "m_Do/m_Do_graphic.h"
#include "d/actor/d_a_mant.h"
#include "d/actor/d_a_mg_rod.h"

#include "Z2AudioLib/Z2Instances.h"

class daB_GND_HIO_c : public JORReflexible {
public:
    daB_GND_HIO_c();
    virtual ~daB_GND_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s8 no;
    /* 0x08 */ f32 model_size;
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
};

enum B_GND_RES_FILE_ID { // IDs and indexes are synced
    /* BCK */
    B_GND_BCK_AL_EGND_TODOME=0x8,
    B_GND_BCK_AL_EGND_TODOMEWAIT=0x9,
    B_GND_BCK_AL_EGND_TUBAZERI_A=0xA,
    B_GND_BCK_AL_EGND_TUBAZERI_B=0xB,
    B_GND_BCK_AL_EGND_TUBAZERI_C=0xC,
    B_GND_BCK_AL_EGND_TUBAZERI_LOSE=0xD,
    B_GND_BCK_AL_EGND_TUBAZERI_WIN=0xE,
    B_GND_BCK_AL_EGND_WH=0xF,
    B_GND_BCK_AL_FEGND_TODOME=0x10,
    B_GND_BCK_AL_FEGND_TODOMEWAIT=0x11,
    B_GND_BCK_AL_FEGND_WH=0x12,
    B_GND_BCK_EGND_ATTACKA=0x13,
    B_GND_BCK_EGND_ATTACKB=0x14,
    B_GND_BCK_EGND_ATTACKC_A=0x15,
    B_GND_BCK_EGND_ATTACKC_B=0x16,
    B_GND_BCK_EGND_ATTACKC_C=0x17,
    B_GND_BCK_EGND_ATTACKD=0x18,
    B_GND_BCK_EGND_ATTACKE_A=0x19,
    B_GND_BCK_EGND_ATTACKE_B=0x1A,
    B_GND_BCK_EGND_ATTACKE_C=0x1B,
    B_GND_BCK_EGND_ATTACKE_C1=0x1C,
    B_GND_BCK_EGND_ATTACKE_C2=0x1D,
    B_GND_BCK_EGND_ATTACKF=0x1E,
    B_GND_BCK_EGND_ATTACKG=0x1F,
    B_GND_BCK_EGND_CHOUHATU=0x20,
    B_GND_BCK_EGND_DAMAGE_A=0x21,
    B_GND_BCK_EGND_DAMAGE_B=0x22,
    B_GND_BCK_EGND_DEFENSE_A=0x23,
    B_GND_BCK_EGND_DEFENSE_B=0x24,
    B_GND_BCK_EGND_DEFENSE_C=0x25,
    B_GND_BCK_EGND_DEFENSE_D=0x26,
    B_GND_BCK_EGND_DEFENSE_E=0x27,
    B_GND_BCK_EGND_DOWN=0x28,
    B_GND_BCK_EGND_DOWNB=0x29,
    B_GND_BCK_EGND_DOWNUP=0x2A,
    B_GND_BCK_EGND_DOWNWAIT=0x2B,
    B_GND_BCK_EGND_JUMP_A=0x2C,
    B_GND_BCK_EGND_JUMP_B=0x2D,
    B_GND_BCK_EGND_JUMP_C=0x2E,
    B_GND_BCK_EGND_KAMAE=0x2F,
    B_GND_BCK_EGND_KURUSIMI=0x30,
    B_GND_BCK_EGND_KUZURE=0x31,
    B_GND_BCK_EGND_LAUGH=0x32,
    B_GND_BCK_EGND_LOOK=0x33,
    B_GND_BCK_EGND_RATTACK01_A=0x34,
    B_GND_BCK_EGND_RATTACK01_B=0x35,
    B_GND_BCK_EGND_RATTACK01_C=0x36,
    B_GND_BCK_EGND_RATTACK01_D=0x37,
    B_GND_BCK_EGND_RATTACK02_A=0x38,
    B_GND_BCK_EGND_RATTACK02_B=0x39,
    B_GND_BCK_EGND_RATTACK02_C=0x3A,
    B_GND_BCK_EGND_RBRAKE=0x3B,
    B_GND_BCK_EGND_RDAMEGE01_A=0x3C,
    B_GND_BCK_EGND_RDAMEGE01_B=0x3D,
    B_GND_BCK_EGND_RDAMEGE02L=0x3E,
    B_GND_BCK_EGND_RDAMEGE02R=0x3F,
    B_GND_BCK_EGND_RDASH=0x40,
    B_GND_BCK_EGND_RDOWN=0x41,
    B_GND_BCK_EGND_RINANAKI01=0x42,
    B_GND_BCK_EGND_RINANAKI02L=0x43,
    B_GND_BCK_EGND_RINANAKI02R=0x44,
    B_GND_BCK_EGND_RJUMP_END=0x45,
    B_GND_BCK_EGND_RJUMP_MIDDLE=0x46,
    B_GND_BCK_EGND_RJUMP_START=0x47,
    B_GND_BCK_EGND_RRETURN=0x48,
    B_GND_BCK_EGND_RRUN=0x49,
    B_GND_BCK_EGND_RSPUR=0x4A,
    B_GND_BCK_EGND_RWAIT=0x4B,
    B_GND_BCK_EGND_RWALK=0x4C,
    B_GND_BCK_EGND_SAYAOPEN=0x4D,
    B_GND_BCK_EGND_SIDEJUMP_LA=0x4E,
    B_GND_BCK_EGND_SIDEJUMP_LB=0x4F,
    B_GND_BCK_EGND_SIDEJUMP_RA=0x50,
    B_GND_BCK_EGND_SIDEJUMP_RB=0x51,
    B_GND_BCK_EGND_STANDUP=0x52,
    B_GND_BCK_EGND_TODOME=0x53,
    B_GND_BCK_EGND_TODOME_02=0x54,
    B_GND_BCK_EGND_TUBAZERI_A=0x55,
    B_GND_BCK_EGND_TUBAZERI_B=0x56,
    B_GND_BCK_EGND_TUBAZERI_C=0x57,
    B_GND_BCK_EGND_TUBAZERI_LOSE=0x58,
    B_GND_BCK_EGND_TUBAZERI_WIN=0x59,
    B_GND_BCK_EGND_WAIT01=0x5A,
    B_GND_BCK_EGND_WAIT02=0x5B,
    B_GND_BCK_EGND_WAIT03=0x5C,
    B_GND_BCK_EGND_WALK=0x5D,
    B_GND_BCK_ZELDA_SURPRISE=0x5E,
    
    /* BMDR */
    B_GND_BMD_EGND_SHEATH=0x61,
    B_GND_BMD_EGND_SWORD=0x62,
    B_GND_BMD_MANT_SHADOW8=0x63,
    B_GND_BMD_SWORD_EFFECTA=0x64,
    B_GND_BMD_SWORD_EFFECTB=0x65,
    B_GND_BMD_SWORD_EFFECTC=0x66,
    
    /* BMDV */
    B_GND_BMD_EGND=0x69,
    B_GND_BMD_ZELDA=0x6A,
    
    /* BRK */
    B_GND_BRK_EGND_CORE_BEAT=0x6D,
    
    /* BTK */
    B_GND_BTK_AL_FEGND_TODOME=0x70,
    B_GND_BTK_AL_FEGND_WH=0x71,
    B_GND_BTK_EGND_KAMAE=0x72,
    B_GND_BTK_EGND_LAUGH=0x73,
    B_GND_BTK_EGND_LOOK=0x74,
    B_GND_BTK_EGND_SAYAOPEN=0x75,
    B_GND_BTK_EGND_STANDUP=0x76,
    B_GND_BTK_EYE_DEFAULT=0x77,
    B_GND_BTK_ZELDA_SUEPRISE=0x78,
    
    /* BTP */
    B_GND_BTP_EGND_MEPACHI=0x7B,
    B_GND_BTP_ZELDA_SUEPRISE=0x7C,
};

enum B_HG_RES_FILE_ID { // IDs and indexes are synced
    /* BCK */
    B_HG_BCK_HG_DOWN=0x5,
    B_HG_BCK_HG_JUMP_END=0x6,
    B_HG_BCK_HG_JUMP_MIDDLE=0x7,
    B_HG_BCK_HG_JUMP_START=0x8,
    B_HG_BCK_HG_RUN=0x9,
    B_HG_BCK_HG_RUN_DASH=0xA,
    B_HG_BCK_HG_STAND=0xB,
    B_HG_BCK_HG_STANDTURNL=0xC,
    B_HG_BCK_HG_STANDTURNR=0xD,
    B_HG_BCK_HG_STOP=0xE,
    B_HG_BCK_HG_WAIT=0xF,
    B_HG_BCK_HG_WALK=0x10,
    
    /* BMDR */
    B_HG_BMD_HG=0x13,
    
    /* TEX */
    B_HG_BTI_TAZUNA=0x16,
};

enum daB_GND_ACTION {
    ACTION_HWAIT_1 = 1,
    ACTION_HWAIT_2 = 2,
    ACTION_HRUN_A = 3,
    ACTION_HRUN_P = 4,
    ACTION_HJUMP = 5,
    ACTION_HEND = 6,
    ACTION_WAIT = 10,
    ACTION_ATTACK = 11,
    ACTION_DEFENCE = 12,
    ACTION_JUMP = 13,
    ACTION_SIDE = 14,
    ACTION_TUBA = 19,
    ACTION_DAMAGE = 20,
    ACTION_DOWN = 21,
    ACTION_END = 22,
};

static u8 l_initHIO;

daB_GND_HIO_c::daB_GND_HIO_c() {
    no = -1;
    model_size = 1.0f;
    field_0xc = 57.0f;
    field_0x10 = 65.0f;
    field_0x14 = 76.0f;
    field_0x18 = 40.0f;
}

static void anm_init(b_gnd_class* i_this, int i_anmID, f32 i_morf, u8 i_attr, f32 i_speed) {
    i_this->mAnmID = i_anmID;
    if (i_anmID == B_GND_BCK_EGND_RRUN) {
        i_anmID = B_GND_BCK_EGND_RDASH;
    }
    i_this->mpModelMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_gnd", i_anmID), i_attr, i_morf, i_speed, 0.0f, -1.0f);
}

static void h_anm_init(b_gnd_class* i_this, int i_anmID, f32 i_morf, u8 i_attr, f32 i_speed) {
    i_this->mHorseAnmID = i_anmID;
    if (i_anmID == B_HG_BCK_HG_RUN) {
        i_anmID = B_HG_BCK_HG_RUN_DASH;
    }
    i_this->mpHorseMorf->setAnm((J3DAnmTransform*)dComIfG_getObjectRes("B_hg", i_anmID), i_attr, i_morf, i_speed, 0.0f, -1.0f);
}

static int nodeCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        int jntNo = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        b_gnd_class* a_this = (b_gnd_class*)model->getUserArea();
        
        if (a_this != NULL) {
            MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);

            if (jntNo == 1) {
                cMtx_XrotM(*calc_mtx, a_this->mGndBodyRotX / 4);
            } else if (jntNo == 4) {
                cMtx_XrotM(*calc_mtx, a_this->mGndBodyRotX * 0.75f);
                cMtx_ZrotM(*calc_mtx, -a_this->mGndHeadRotZ);
            } else if (jntNo == 28) {
                cMtx_XrotM(*calc_mtx, a_this->mGndArmRRotX);
            } else if (jntNo == 17) {
                cMtx_YrotM(*calc_mtx, a_this->mGndShoulderLRotY);
            } else if (jntNo == 37) {
                cMtx_XrotM(*calc_mtx, a_this->mGndLegRotX);
            } else if (jntNo == 41) {
                cMtx_XrotM(*calc_mtx, -a_this->mGndLegRotX);
            }

            model->setAnmMtx(jntNo, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }

    return 1;
}

static int h_nodeCallBack(J3DJoint* i_joint, int param_2) {
    if (param_2 == 0) {
        int jntNo = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        b_gnd_class* a_this = (b_gnd_class*)model->getUserArea();

        if (a_this != NULL) {
            MTXCopy(model->getAnmMtx(jntNo), *calc_mtx);
            
            if (jntNo == 4 || jntNo == 5) {
                cMtx_ZrotM(*calc_mtx, -a_this->mHorseLegRot * 2);
            } else if (jntNo == 8 || jntNo == 9) {
                cMtx_YrotM(*calc_mtx, -a_this->mHorseLegRot * 2);
            } else if (jntNo == 28 || jntNo == 29) {
                cMtx_YrotM(*calc_mtx, a_this->mHorseLegRot << 1);
            } else if (jntNo == 32 || jntNo == 33) {
                cMtx_YrotM(*calc_mtx, a_this->mHorseLegRot << 1);
            }

            model->setAnmMtx(jntNo, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }

    return 1;
}

static int daB_GND_Draw(b_gnd_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    
    if (i_this->mNoDrawTimer != 0) {
        return 1;
    }
    
    J3DModel* model = i_this->mpModelMorf->getModel();
    g_env_light.settingTevStruct(0, &a_this->current.pos, &a_this->tevStr);
    
    i_this->mpGndEyeBtk[i_this->mGndEyeBtkNo]->entry(model->getModelData());
    i_this->mpGndCoreBrk->entry(model->getModelData());
    i_this->mpGndEyeBtp->entry(model->getModelData());

    g_env_light.setLightTevColorType_MAJI(model, &a_this->tevStr);

    i_this->mpModelMorf->entryDL();

    g_env_light.settingTevStruct(5, &a_this->current.pos, &i_this->mSwordTevstr);
    g_env_light.setLightTevColorType_MAJI(i_this->mpSwordModel, &i_this->mSwordTevstr);
    mDoExt_modelUpdateDL(i_this->mpSwordModel);

    if (i_this->mSwordBlurAlpha > 10) {
        g_env_light.setLightTevColorType_MAJI(i_this->mpSwordBlurModel[i_this->mSwordBlurType], &a_this->tevStr);

        J3DMaterial* material = i_this->mpSwordBlurModel[i_this->mSwordBlurType]->getModelData()->getMaterialNodePointer(0);
        material->getTevKColor(3)->a = i_this->mSwordBlurAlpha;
        mDoExt_modelUpdateDL(i_this->mpSwordBlurModel[i_this->mSwordBlurType]);
    }
    
    if (!i_this->mHideSheath) {
        g_env_light.setLightTevColorType_MAJI(i_this->mpSheathModel, &a_this->tevStr);
        mDoExt_modelUpdateDL(i_this->mpSheathModel);
    }
    
    if (i_this->mDrawZelda) {
        g_env_light.setLightTevColorType_MAJI(i_this->mpZeldaModel->getModel(), &a_this->tevStr);
        i_this->mpZeldaBtk->entry(i_this->mpZeldaModel->getModel()->getModelData());
        i_this->mpZeldaBtp->entry(i_this->mpZeldaModel->getModel()->getModelData());
        i_this->mpZeldaModel->entryDL();
    }

    if (i_this->mDemoCamMode >= 60) {
        return 1;
    }

    for (int i = 0; i < 36; i++) {
        g_env_light.setLightTevColorType_MAJI(i_this->mpMantShadowModel[i], &a_this->tevStr);
        mDoExt_modelUpdateDL(i_this->mpMantShadowModel[i]);
    }
    
    J3DModel* horse_model = NULL;
    if (i_this->mDrawHorse) {
        horse_model = i_this->mpHorseMorf->getModel();
        g_env_light.setLightTevColorType_MAJI(horse_model, &a_this->tevStr);
        i_this->mpHorseMorf->entryDL();
    }
    
    cXyz shadowPos;
    shadowPos.set(a_this->current.pos.x, a_this->current.pos.y + 100.0f, a_this->current.pos.z);

    i_this->mShadowID = dComIfGd_setShadow(i_this->mShadowID, 1, model,
                        &shadowPos, 3000.0f, 0.0f, a_this->current.pos.y,
                        i_this->mAcch.GetGroundH(), i_this->mAcch.m_gnd,
                        &a_this->tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());

    dComIfGd_addRealShadow(i_this->mShadowID, i_this->mpSwordModel);

    if (i_this->mDrawHorse) {
        dComIfGd_addRealShadow(i_this->mShadowID, horse_model);
    } else {
        for (int i = 0; i < 36; i++) {
            dComIfGd_addRealShadow(i_this->mShadowID, i_this->mpMantShadowModel[i]);
        }
    }

    if (i_this->mDrawHorse) {
        static GXColor l_color = {20, 15, 0, 255};

        i_this->mHorseReins[0].update(16, l_color, &a_this->tevStr);
        dComIfGd_set3DlineMat(&i_this->mHorseReins[0]);

        i_this->mHorseReins[1].update(16, l_color, &a_this->tevStr);
        dComIfGd_set3DlineMat(&i_this->mHorseReins[1]);

        i_this->field_0x21e8.update(2, l_color, &a_this->tevStr);
        dComIfGd_set3DlineMat(&i_this->field_0x21e8);
    }
    
    return 1;
}

static void* shot_s_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_ARROW &&
        (fopAcM_GetParam(i_actor) == 1 || fopAcM_GetParam(i_actor) == 2))
    {
        cXyz vec_to_arrow = ((fopAc_ac_c*)i_data)->current.pos - ((fopAc_ac_c*)i_actor)->current.pos;
        if (vec_to_arrow.abs() < 1000.0f) {
            return i_actor;
        }
    }

    return NULL;
}

static daB_GND_HIO_c l_HIO;

static u32 gake_check(b_gnd_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    dBgS_GndChk gnd_chk;
    cXyz sp40;
    cXyz sp34;
    
    if (a_this->current.pos.y < -5000.0f) {
        a_this->current.pos = a_this->home.pos;
        a_this->old.pos = a_this->current.pos;
    }

    for (int i = 0; i < 4; i++) {
        // check if ganondorf is near one of the field exits
        // 0: Kakariko, 1: Eldin Bridge, 2: Castle Town, 3: Eldin Gorge
        static f32 chk_x[] = {14167.0f, 34775.0f, -24164.0f, -11627.0f};
        static f32 chk_z[] = {21775.0f, -16467.0f, 9747.0f, 22601.0f};

        sp40.x = chk_x[i] - a_this->current.pos.x;
        sp40.z = chk_z[i] - a_this->current.pos.z;
        s16 sp12 = cM_atan2s(sp40.x, sp40.z) - a_this->shape_angle.y;

        if (JMAFastSqrt(sp40.x * sp40.x + sp40.z * sp40.z) < 6000.0f && sp12 < 0x4000 && -0x4000 < sp12) {
            if (sp12 > 0) {
                return 4;
            } else {
                return 5;
            }
        }
    }

    cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
    BOOL sp14 = FALSE;

    cXyz sp28;
    sp40.x = 200.0f;
    sp40.y = 1000.0f;
    sp40.z = 200.0f;
    MtxPosition(&sp40, &sp34);
    sp34 += i_this->current.pos;
    gnd_chk.SetPos(&sp34);
            
    if (a_this->current.pos.y - dComIfG_Bgsp().GroundCross(&gnd_chk) > 2000.0f) {
        sp28 = sp34;
        sp14 = TRUE;
    } else {
        sp40.x *= -1.0f;
        MtxPosition(&sp40, &sp34);
        sp34 += i_this->current.pos;
        gnd_chk.SetPos(&sp34);
        if (i_this->current.pos.y - dComIfG_Bgsp().GroundCross(&gnd_chk) > 2000.0f) {
            sp28 = sp34;
            sp14 = TRUE;
        } else {
            sp40.x = 0.0f;
            sp40.y = 1000.0f;
            sp40.z = 250.0f;
            MtxPosition(&sp40, &sp34);
            sp34 += i_this->current.pos;
            gnd_chk.SetPos(&sp34);
            if (i_this->current.pos.y - dComIfG_Bgsp().GroundCross(&gnd_chk) > 2000.0f) {
                sp28 = sp34;
                sp14 = TRUE;
            }
        }
    }

    static cXyz k_pos(0.0f, 0.0f, 0.0f);

    if (sp14) {
        if (k_pos.y < -5000.0f) {
            sp28.y = a_this->current.pos.y;
            k_pos = sp28;
        }

        sp40 = a_this->current.pos - k_pos;
        if (sp40.abs() < 500.0f) {
            cMtx_YrotS(*calc_mtx, cM_atan2s(sp40.x, sp40.z));
            sp40.x = 0.0f;
            sp40.y = 0.0f;
            sp40.z = 500.0f;
            MtxPosition(&sp40, &sp34);

            a_this->current.pos.x = sp34.x + k_pos.x;
            a_this->current.pos.z = sp34.z + k_pos.z;
        }
    } else {
        k_pos.set(0.0f, -10000.0f, 0.0f);
    }
            
    sp40.x = 700.0f;
    sp40.y = 1000.0f;
    sp40.z = 2000.0f;
    MtxPosition(&sp40, &sp34);
    sp34 += i_this->current.pos;
    gnd_chk.SetPos(&sp34);

    if (i_this->current.pos.y - dComIfG_Bgsp().GroundCross(&gnd_chk) > 2000.0f) {
        return 2;
    }

    sp40.x = -700.0f;
    MtxPosition(&sp40, &sp34);
    sp34 += i_this->current.pos;
    gnd_chk.SetPos(&sp34);
    
    if (i_this->current.pos.y - dComIfG_Bgsp().GroundCross(&gnd_chk) > 2000.0f) {
        return 3;
    }

    sp40.x = 0.0f;
    sp40.y = 1000.0f;
    sp40.z = 2500.0f;
    MtxPosition(&sp40, &sp34);
    sp34 += i_this->current.pos;
    gnd_chk.SetPos(&sp34);

    if (i_this->current.pos.y - dComIfG_Bgsp().GroundCross(&gnd_chk) > 2000.0f) {
        return 1;
    }

    return 0;
}

static BOOL saku_jump_check(b_gnd_class* i_this) {
    if (i_this->mGakeChkType >= 4) {
        return FALSE;
    }

    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    dBgS_LinChk lineChk;
    cM3dGPla plane;
    cXyz sp54, sp48, sp3C;

    cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
    cMtx_XrotM(*calc_mtx, a_this->shape_angle.x);

    sp54.x = 10.0f;
    sp54.y = 150.0f;
    sp54.z = 900.0f;

    MtxPosition(&sp54, &sp3C);
    
    sp3C += a_this->current.pos;
    sp3C.y += 200.0f;

    sp48 = a_this->current.pos;
    sp48.y += 350.0f;

    lineChk.Set(&sp48, &sp3C, a_this);

    if (dComIfG_Bgsp().LineCross(&lineChk) && dComIfG_Bgsp().GetTriPla(lineChk, &plane) && !cBgW_CheckBGround(plane.mNormal.y)) {
        return FALSE;
    }

    sp48.y -= 250.0f;
    sp3C.y -= 200.0f;
    lineChk.Set(&sp48, &sp3C, a_this);

    if (dComIfG_Bgsp().LineCross(&lineChk)) {
        if (dComIfG_Bgsp().GetTriPla(lineChk, &plane) && !cBgW_CheckBGround(plane.mNormal.y)) {
            cXyz line_cross_pos(lineChk.GetCross());
            cXyz sp24;

            sp54.x *= -1.0f;
            MtxPosition(&sp54, &sp3C);
            sp3C += a_this->current.pos;
            lineChk.Set(&sp48, &sp3C, a_this);

            if (dComIfG_Bgsp().LineCross(&lineChk)) {
                sp24 = lineChk.GetCross();
            } else {
                return FALSE;
            }

            sp54 = sp24 - line_cross_pos;

            s16 var_r28 = cM_atan2s(sp54.x, sp54.z) + 0x4000 - a_this->shape_angle.y;
            if (var_r28 > -0x3000 && var_r28 < 0x3000) {
                return TRUE;
            }
        }
    }

    return FALSE;
}

static void b_gnd_h_wait(b_gnd_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    f32 player_distxz = i_this->mPlayerDistXZ;
    s16 player_angle = i_this->mPlayerAngleY;

    switch (i_this->mMoveMode) {
    case -1:
        Z2GetAudioMgr()->bgmStart(0x200005D, 0, 0);
    case 0:
        anm_init(i_this, B_GND_BCK_EGND_RWAIT, 10.0f, 2, 1.0f);
        h_anm_init(i_this, B_HG_BCK_HG_WAIT, 10.0f, 2, 1.0f);
        i_this->mMoveMode = 1;
    case 1:
        if (i_this->field_0xc44[0] == 0) {
            i_this->mMoveMode = 2;
        }
        break;
    case 2:
        cLib_addCalcAngleS2(&a_this->current.angle.y, player_angle, 8, 0x200);
        
        s16 angle_diff = player_angle - a_this->current.angle.y;
        if (angle_diff < 0x800 && angle_diff > -0x800) {
            i_this->mActionMode = ACTION_HWAIT_2;
            i_this->mMoveMode = 0;
        }
    }

    cLib_addCalc0(&a_this->speedF, 1.0f, 2.0f);
}

static void b_gnd_h_wait2(b_gnd_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);

    cXyz sp18;
    cXyz spC;
    f32 player_distxz = i_this->mPlayerDistXZ;
    s16 player_angle = i_this->mPlayerAngleY;
    int h_anm_frame = i_this->mpHorseMorf->getFrame();
    
    switch (i_this->mMoveMode) {
    case 0:
        if (i_this->mGakeChkType != 0) {
            i_this->field_0x5cc = a_this->current.angle.y + 0x8000;
        } else if (cM_rndF(1.0f) < 0.5f) {
            i_this->field_0x5cc = player_angle + 0x2000;
        } else {
            i_this->field_0x5cc = player_angle + -0x2000;
        }

        if ((s16)(i_this->field_0x5cc - a_this->current.angle.y) > 0) {
            anm_init(i_this, B_GND_BCK_EGND_RINANAKI02L, 3.0f, 0, 1.0f);
            h_anm_init(i_this, B_HG_BCK_HG_STANDTURNL, 3.0f, 0, 1.0f);
            OS_REPORT(" GND TURN L\n");
        } else {
            anm_init(i_this, B_GND_BCK_EGND_RINANAKI02R, 3.0f, 0, 1.0f);
            h_anm_init(i_this, B_HG_BCK_HG_STANDTURNR, 3.0f, 0, 1.0f);
            OS_REPORT(" GND TURN R\n");
        }

        i_this->mMoveMode = 1;
        i_this->field_0xc68 = 0x100;
    case 1:
        if (h_anm_frame >= 12) {
            cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x5cc, 4, i_this->field_0xc68);
            cLib_addCalcAngleS2(&i_this->field_0xc68, 0x400, 1, 0x80);
        }

        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, B_GND_BCK_EGND_RWAIT, 10.0f, 2, 1.0f);
            h_anm_init(i_this, B_HG_BCK_HG_WAIT, 10.0f, 2, 1.0f);
            i_this->mMoveMode = 2;
        }
        break;
    case 2:
        if (player_distxz < 4000.0f || player_distxz > 7500.0f) {
            i_this->mActionMode = ACTION_HRUN_P;
            i_this->mMoveMode = 0;
            i_this->field_0xc44[3] = 100;
        }
    }

    cLib_addCalc0(&a_this->speedF, 1.0f, 1.0f);
}

static cXyz b_path[8];

static void b_gnd_h_run_a(b_gnd_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp8C, sp80;

    f32 var_f30;
    f32 player_distxz = i_this->mPlayerDistXZ;
    f32 speed_target = 0.0f;
    f32 speed_step = 1.0f;

    s8 sp9 = 0;
    if (daPy_getPlayerActorClass()->checkHorseRide() && dComIfGp_getHorseActor()->speedF >= 30.0f) {
        sp9 = 1;
    }

    s16 sp12 = a_this->current.angle.y;
    f32 anm_speed_target = 1.0f;

    BOOL sp2C = FALSE;
    s16 sp10 = (i_this->mPlayerAngleY + 0x7FFF) - player->shape_angle.y;
    if (sp10 < 0x4000 && sp10 > -0x4000) {
        sp2C = TRUE;
    }

    s8 sp8 = false;
    s16 spE = 0x80;
    cXyz sp74, sp68;

    switch (i_this->mMoveMode) {
    case 0:
    case 1: {
        if (i_this->mAnmID != B_GND_BCK_EGND_RRUN) {
            anm_init(i_this, B_GND_BCK_EGND_RRUN, 5.0f, 2, 1.0f);
            h_anm_init(i_this, B_HG_BCK_HG_RUN, 5.0f, 2, 1.0f);
            i_this->mPlaySpeed = 1.0f;
        }

        i_this->mMoveMode = 2;
        i_this->field_0xc68 = 0;
        
        int sp28 = 0;
        
        for (int i = 1; i < 8; i++) {
            sp74 = b_path[sp28] - a_this->current.pos;
            sp68 = b_path[i] - a_this->current.pos;

            if (sp74.abs() > sp68.abs()) {
                sp28 = i;
            }
        }

        i_this->field_0xc60 = sp28;
        i_this->field_0xc44[2] = cM_rndF(100.0f) + 100.0f;
    }
    case 2:
        if (i_this->field_0xc44[2] == 0 && sp9) {
            i_this->mMoveMode = 3;
        } else if (player_distxz > 5000.0f && i_this->field_0xc44[3] == 0) {
            if (sp2C) {
                i_this->mMoveMode = 20;
                i_this->field_0xc44[0] = 10;
            } else {
                i_this->mActionMode = ACTION_HRUN_P;
                i_this->mMoveMode = 0;
                i_this->field_0xc44[3] = 100;
            }
        }

        i_this->field_0xc6a = 1;
        break;
    case 3:
        anm_init(i_this, B_GND_BCK_EGND_RATTACK02_A, 3.0f, 0, 1.0f);
        i_this->mMoveMode = 4;
        i_this->field_0x1e0f = 1;
        i_this->field_0x1e10 = 0.0f;
    case 4:
        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, B_GND_BCK_EGND_RATTACK02_B, 3.0f, 2, 1.0f);
            i_this->field_0xc44[0] = 20;
            i_this->mMoveMode = 5;
        }
        break;
    case 5:
        if (i_this->field_0xc44[0] == 0) {
            anm_init(i_this, B_GND_BCK_EGND_RATTACK02_C, 3.0f, 0, 1.0f);
            i_this->mMoveMode = 6;
            i_this->field_0xc44[0] = 15;
            i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_ATTACK02C, -1);
        }
        break;
    case 6:
        if (i_this->field_0xc44[0] == 1) {
            i_this->field_0x1e50.y = a_this->shape_angle.y + 0x4000;
            i_this->field_0x1e50.x = 0;
            i_this->field_0x1e0f = 2;
            i_this->field_0x1e4c = 40.0f;
            i_this->field_0x1e0e = 0;
        }

        if (i_this->mpModelMorf->isStop()) {
            i_this->mMoveMode = 2;
            i_this->field_0xc44[2] = cM_rndF(100.0f) + 100.0f;
        }
        break;
    case 7:
        if (i_this->mHorseAnmID != B_HG_BCK_HG_RUN) {
            h_anm_init(i_this, B_HG_BCK_HG_RUN, 5.0f, 2, 1.0f);
        }
        anm_init(i_this, B_GND_BCK_EGND_RDAMEGE01_A, 1.0f, 0, 1.0f);
        i_this->mMoveMode = 8;
        i_this->field_0xc44[0] = 300;
    case 8:
        i_this->field_0x1e08 = 10;

        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, B_GND_BCK_EGND_RDAMEGE01_B, 2.0f, 2, 1.0f);
            i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_DAMAGE01, -1);
            i_this->mMoveMode = 9;
        }
        break;
    case 9:
        i_this->field_0x1e08 = 10;
        if (i_this->field_0xc44[0] == 0) {
            anm_init(i_this, B_GND_BCK_EGND_RRETURN, 3.0f, 0, 1.0f);
            i_this->mMoveMode = 10;
        }
        break;
    case 10:
        i_this->mDamageInvulnerabilityTimer = 10;

        if (i_this->mpModelMorf->isStop()) {
            i_this->mMoveMode = 0;
        }
        break;
    case 11:
        if (sp10 > 0) {
            anm_init(i_this, B_GND_BCK_EGND_RDAMEGE02L, 2.0f, 0, 1.0f);
        } else {
            anm_init(i_this, B_GND_BCK_EGND_RDAMEGE02R, 2.0f, 0, 1.0f);
        }
        i_this->mMoveMode = 12;
    case 12:
        i_this->mDamageInvulnerabilityTimer = 10;

        if (i_this->mpModelMorf->isStop()) {
            i_this->mMoveMode = 0;
        }
        break;
    case 20:
        speed_target = l_HIO.field_0xc;
        speed_step = 2.0f;
        if (i_this->field_0xc44[0] == 0) {
            i_this->mMoveMode = 21;
            anm_init(i_this, B_GND_BCK_EGND_RBRAKE, 3.0f, 0, 1.0f);
            h_anm_init(i_this, B_HG_BCK_HG_STOP, 3.0f, 0, 1.0f);
        }

        i_this->field_0xc6a = 1;
        break;
    case 21:
        speed_step = 2.0f;
        if (a_this->speedF > 15.0f) {
            i_this->field_0xc6a = 2;
            i_this->mGndSound.startCreatureSoundLevel(Z2SE_EN_HG_SLIP, 0, -1);
        }

        if (a_this->speedF < 2.0f) {
            i_this->mActionMode = ACTION_HWAIT_2;
            i_this->mMoveMode = 0;
        }
    }

    if (i_this->mMoveMode < 20) {
        sp8C = b_path[i_this->field_0xc60] - a_this->current.pos;

        var_f30 = JMAFastSqrt(sp8C.x * sp8C.x + sp8C.z * sp8C.z);
        if (var_f30 < 800.0f) {
            sp8C = b_path[(i_this->field_0xc60 + 1) & 7] - a_this->current.pos;

            sp10 = a_this->current.angle.y - cM_atan2s(sp8C.x, sp8C.z);
            if (sp10 < 0x4000 && sp10 > -0x4000) {
                i_this->field_0xc64 = 1;
            } else {
                i_this->field_0xc64 = -1;
            }

            i_this->field_0xc60 = i_this->field_0xc60 + i_this->field_0xc64;
            i_this->field_0xc60 = i_this->field_0xc60 & 7;
            sp8 = true;
            i_this->field_0xc68 = 0;
        }

        i_this->field_0x5cc = cM_atan2s(sp8C.x, sp8C.z);
        if (i_this->field_0x1e0a >= 1 && i_this->mMoveMode <= 2) {
            var_f30 = (var_f30 - 800.0f) * (AREG_F(8) + 2.0f);
            if (var_f30 > 7000.0f) {
                var_f30 = 7000.0f;
            } else if (var_f30 < 0.0f) {
                var_f30 = 0.0f;
            }

            i_this->field_0xc90 = i_this->field_0xc90 + i_this->field_0xc92;
            if ((i_this->mCounter & 0x3F) == 0) {
                i_this->field_0xc92 = cM_rndF(600.0f) + 300.0f;
            }

            i_this->field_0x5cc += (s16)(var_f30 * cM_ssin(i_this->field_0xc90));
        }

        if (i_this->field_0xc72 != 0) {
            i_this->field_0x5cc = i_this->field_0xc74;
        }

        if (i_this->field_0x1e08 != 0) {
            i_this->field_0xc68 = 0x80;
        }

        cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x5cc, 8, i_this->field_0xc68);

        if (i_this->mGakeChkType != 0) {
            spE = 0x100;
        }

        cLib_addCalcAngleS2(&i_this->field_0xc68, 0x400, 1, spE);

        if (i_this->field_0x1e08 != 0) {
            speed_target = l_HIO.field_0x18;
            speed_step = 0.5f;
        } else if (sp9) {
            speed_target = dComIfGp_getHorseActor()->speedF;
            if (speed_target > l_HIO.field_0x10) {
                speed_target = l_HIO.field_0x10;
            } else if (speed_target < l_HIO.field_0xc) {
                speed_target = l_HIO.field_0xc;
            }

            if (fopAcM_searchPlayerDistanceXZ(a_this) < KREG_F(0) + 2000.0f) {
                speed_target *= 1.2f;
            }

            if (a_this->speedF < l_HIO.field_0xc) {
                speed_step = 2.5f;
            } else {
                speed_step = 2.0f;
            }
            
            anm_speed_target = a_this->speedF / 40.0f;
        } else {
            speed_step = 2.0f;
            speed_target = l_HIO.field_0xc;
        }
    }

    if (anm_speed_target < 1.0f) {
        anm_speed_target = 1.0f;
    }

    if (anm_speed_target > 1.3f) {
        anm_speed_target = 1.3f;
    }

    cLib_addCalc2(&a_this->speedF, speed_target, 1.0f, speed_step);
    if (speed_target > 10.0f) {
        sp12 = (AREG_S(7) + -8) * (a_this->current.angle.y - sp12);
        if (sp12 > 2000) {
            sp12 = 2000;
        } else if (sp12 < -2000) {
            sp12 = -2000;
        }

        cLib_addCalcAngleS2(&i_this->mHorseLegRot, sp12, 4, 0x300);
    }

    if (i_this->mAnmID == B_GND_BCK_EGND_RRUN) {
        i_this->mpModelMorf->setPlaySpeed(i_this->mPlaySpeed * (JREG_F(11) + 1.0f));
        i_this->mpHorseMorf->setPlaySpeed(i_this->mPlaySpeed * (JREG_F(11) + 1.0f));
    }

    cLib_addCalc2(&i_this->mPlaySpeed, anm_speed_target, 1.0f, 0.1f);

    if (sp8) {
        sp8C.x = 0.0f;
        sp8C.y = 1000.0f;
        for (int i = 0; i < 8; i++) {
            cMtx_YrotS(*calc_mtx, (int)(i << 13));
            sp8C.z = cM_rndF(3000.0f) + 12000.0f;
            MtxPosition(&sp8C, &sp80);
            sp80.x += -2989.0f;
            sp80.z += 1364.0f;

            b_path[i] = sp80;
        }
    }
}

static void b_gnd_h_run_p(b_gnd_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz sp38;
    cXyz sp2C;

    f32 player_distxz = i_this->mPlayerDistXZ;
    f32 speed_target = 0.0f;
    f32 speed_step = 2.5f;

    s8 sp9 = FALSE;
    if (player->checkHorseRide() && dComIfGp_getHorseActor()->speedF >= 30.0f) {
        sp9 = TRUE;
    }

    s16 spE = a_this->current.angle.y;
    f32 anm_speed_target = 1.0f;
    cXyz sp20;
    cXyz sp14;

    BOOL var_r27 = FALSE;
    s16 angle_diff = i_this->mPlayerAngleY - a_this->shape_angle.y;
    if (angle_diff < 0x4000 && angle_diff > -0x4000) {
        var_r27 = TRUE;
    }

    f32 var_f28;
    if (sp9) {
        var_f28 = 0.0f;
    } else {
        var_f28 = 400.0f;
    }

    switch (i_this->mMoveMode) {
    case 0:
    case 1:
        if (i_this->mAnmID != B_GND_BCK_EGND_RRUN) {
            i_this->mPlaySpeed = 1.0f;
            anm_init(i_this, B_GND_BCK_EGND_RRUN, 5.0f, 2, 1.0f);
            h_anm_init(i_this, B_HG_BCK_HG_RUN, 5.0f, 2, 1.0f);
        }

        i_this->field_0xc68 = 0;
        i_this->mMoveMode = 2;
        break;
    case 2:
        var_r27 = TRUE;
        if (player_distxz < var_f28 + 1700.0f) {
            i_this->mMoveMode = 3;
        }
        break;
    case 3:
        anm_init(i_this, B_GND_BCK_EGND_RATTACK01_A, 3.0f, 0, 1.0f);
        i_this->mMoveMode = 4;
    case 4:
        var_r27 = TRUE;
        if (player_distxz < var_f28 + 550.0f || i_this->mpModelMorf->isStop()) {
            anm_init(i_this, B_GND_BCK_EGND_RATTACK01_D, 2.0f, 2, 1.0f);
            i_this->mMoveMode = 5;
        }
        break;
    case 5:
        var_r27 = TRUE;
        if (player_distxz < var_f28 + 550.0f) {
            if (angle_diff < 0 || !sp9) {
                anm_init(i_this, B_GND_BCK_EGND_RATTACK01_C, 2.0f, 0, 1.0f);
            } else {
                anm_init(i_this, B_GND_BCK_EGND_RATTACK01_B, 2.0f, 0, 1.0f);
            }

            i_this->mMoveMode = 6;
        }
        break;
    case 6:
        var_r27 = TRUE;
        i_this->field_0xc77 = 1;

        if (i_this->mpModelMorf->isStop()) {
            var_r27 = FALSE;
        }
        break;
    default:
        break;
    }

    cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);

    if (sp9) {
        if (angle_diff < 0) {
            sp38.x = 200.0f;
        } else {
            sp38.x = -200.0f;
        }
    } else {
        sp38.x = 0.0f;
    }
    sp38.y = 0.0f;
    sp38.z = 0.0f;
    MtxPosition(&sp38, &sp2C);

    sp2C += player->current.pos;
    sp2C -= a_this->current.pos;

    i_this->field_0x5cc = (s16)cM_atan2s(sp2C.x, sp2C.z);

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->field_0x5cc, 8, i_this->field_0xc68);
    cLib_addCalcAngleS2(&i_this->field_0xc68, 0x400, 1, 0x40);

    if (sp9) {
        speed_target = l_HIO.field_0x14;
        if (player_distxz > 2000.0f) {
            speed_target *= 1.4f;
        } else if (player_distxz > 1000.0f) {
            speed_target *= 1.2f;
        }

        anm_speed_target = a_this->speedF / 40.0f;
        if (anm_speed_target < 1.0f) {
            anm_speed_target = 1.0f;
        }
        if (anm_speed_target > 1.3f) {
            anm_speed_target = 1.3f;
        }
    } else {
        speed_step = 3.0f;
        speed_target = l_HIO.field_0xc;
    }

    if ((!var_r27 && i_this->field_0xc44[3] == 0) || i_this->mGakeChkType != 0) {
        i_this->mActionMode = ACTION_HRUN_A;

        if (i_this->mGakeChkType == 2) {
            i_this->field_0xc72 = 40;
            i_this->field_0xc74 = a_this->shape_angle.y + -0x4000;
        } else if (i_this->mGakeChkType == 3) {
            i_this->field_0xc72 = 40;
            i_this->field_0xc74 = a_this->shape_angle.y + 0x4000;
        } else if (i_this->mGakeChkType >= 4) {
            i_this->field_0xc74 = a_this->shape_angle.y + 0x8000;
        }

        i_this->mMoveMode = 0;
        i_this->field_0xc44[3] = 100;
    }

    i_this->field_0xc6a = 1;
    cLib_addCalc2(&a_this->speedF, speed_target, 1.0f, speed_step);

    if (speed_target > 10.0f) {
        spE = (a_this->current.angle.y - spE) * -8;
        if (spE > 2000) {
            spE = 2000;
        } else if (spE < -2000) {
            spE = -2000;
        }

        cLib_addCalcAngleS2(&i_this->mHorseLegRot, spE, 4, 0x300);
    }

    i_this->mpHorseMorf->setPlaySpeed(i_this->mPlaySpeed);

    if (i_this->mAnmID == B_GND_BCK_EGND_RRUN) {
        i_this->mpModelMorf->setPlaySpeed(i_this->mPlaySpeed);
    }

    cLib_addCalc2(&i_this->mPlaySpeed, anm_speed_target, 1.0f, 0.1f);
}

static void b_gnd_h_jump(b_gnd_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    switch (i_this->mMoveMode) {
    case 0:
        i_this->mMoveMode = 1;
        anm_init(i_this, B_GND_BCK_EGND_RJUMP_START, 2.0f, 0, 1.0f);
        h_anm_init(i_this, B_HG_BCK_HG_JUMP_START, 2.0f, 0, 1.0f);
        a_this->speed.y = 55.0f;
        break;
    case 1:
        if (i_this->mpModelMorf->isStop()) {
            i_this->mMoveMode = 2;
            anm_init(i_this, B_GND_BCK_EGND_RJUMP_MIDDLE, 2.0f, 0, 1.0f);
            h_anm_init(i_this, B_HG_BCK_HG_JUMP_MIDDLE, 2.0f, 0, 1.0f);
        }
        break;
    case 2:
        if (i_this->mAcch.ChkGroundHit()) {
            i_this->mMoveMode = 3;
            anm_init(i_this, B_GND_BCK_EGND_RJUMP_END, 2.0f, 0, 1.0f);
            h_anm_init(i_this, B_HG_BCK_HG_JUMP_END, 2.0f, 0, 1.0f);
            i_this->field_0x2699 = 1;
        }
        break;
    case 3:
        if (i_this->mpModelMorf->isStop()) {
            i_this->mActionMode = i_this->field_0xb00;
            i_this->mMoveMode = 0;
            i_this->mPlaySpeed = 1.5f;
            anm_init(i_this, B_GND_BCK_EGND_RRUN, 2.0f, 2, 1.5f);
            h_anm_init(i_this, B_HG_BCK_HG_RUN, 2.0f, 2, 1.5f);
        }
    }
}

static void b_gnd_h_end(b_gnd_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    mant_class* mant_p = (mant_class*)fopAcM_SearchByID(i_this->mMantChildID);
    int h_anm_frame = i_this->mpHorseMorf->getFrame();
    i_this->mDamageInvulnerabilityTimer = 10;

    switch (i_this->mMoveMode) {
    case 0:
        Z2GetAudioMgr()->bgmStop(30, 0);
        i_this->mMoveMode = 1;
        h_anm_init(i_this, B_HG_BCK_HG_DOWN, 3.0f, 0, 1.0f);
        anm_init(i_this, B_GND_BCK_EGND_RDOWN, 2.0f, 0, 1.0f);
        i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_DOWN, -1);

        a_this->speedF = 50.0f;
        a_this->current.pos.set(2.4f, 701.0f, -5920.0f);
        a_this->old.pos = a_this->current.pos;
        a_this->shape_angle.y = 0;
        a_this->current.angle.y = 0;
        i_this->field_0x1e0f = 0;
        i_this->field_0x1e10 = 0.0f;
        i_this->mDemoCamMode = 30;
        a_this->health = 100;
        i_this->field_0x1e0a = 0;
    case 1:
        if (i_this->mDemoCamMode < 33 && h_anm_frame == 30) {
            i_this->mpModelMorf->setFrame(0.0f);
            i_this->mpHorseMorf->setFrame(0.0f);

            if (i_this->mDemoCamMode < 32) {
                i_this->mDemoCamMode = 32;
            } else {
                i_this->mDemoCamMode = 34;
                i_this->mDemoCamTimer = 0;
                i_this->mMoveMode = 2;
            }
        }
        break;
    case 2:
        if (i_this->mDemoCamMode < 34) {
            if (h_anm_frame == 57) {
                cXyz eff_size(4.0f, 4.0f, 4.0f);
                dComIfGp_particle_set(0x8C21, &a_this->current.pos, &a_this->shape_angle, &eff_size);   
            }
                
            if (h_anm_frame >= 62) {
                cLib_addCalc0(&a_this->speedF, 1.0f, 1.2f);
                cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y + 0x1000, 8, 130);
                i_this->field_0x2688 = dComIfGp_particle_set(i_this->field_0x2688, 0x8b9a, 
                                                        &i_this->current.pos, 0, 0);
                if (a_this->speedF > 14.0f) {
                    i_this->mGndSound.startCreatureSoundLevel(Z2SE_EN_HG_SLIP, 0, -1);
                }
            }
        
            if (a_this->speedF < 1.0f) {
                mant_p->field_0x3965 = 1;
            }
        } else {
            i_this->mpModelMorf->setFrame(0.0f);
            i_this->mpHorseMorf->setFrame(0.0f);
            a_this->speedF = 0.0f;
            a_this->current.pos.x = 1000.0f;
        }
    case 3:
    default:
        break;
    }
}

static void b_gnd_g_wait(b_gnd_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    f32 speed_target = 0.0f;
    f32 speed_step = 2.0f;
    s16 angle_step = 0x400;

    mant_class* mant_p = (mant_class*)fopAcM_SearchByID(i_this->mMantChildID);
    dmg_rod_class* mgrod_p = (dmg_rod_class*)fopAcM_SearchByName(PROC_MG_ROD);

    if (mgrod_p != NULL && mgrod_p->action != 0) {
        if (i_this->mMoveMode < 5 && i_this->mPlayerDistXZ < 600.0f) {
            i_this->mMoveMode = 5;
            i_this->field_0xc44[0] = 10;
        }
    } else if (i_this->mMoveMode == 5) {
        i_this->mMoveMode = 6;
        i_this->field_0xc44[0] = 40;
    }

    switch (i_this->mMoveMode) {
    case 0:
        i_this->mMoveMode = 1;
        anm_init(i_this, B_GND_BCK_EGND_WALK, 15.0f, 2, 1.0f);
        i_this->field_0xc44[0] = cM_rndF(100.0f) + 200.0f;
    case 1:
        speed_target = 4.3f;

        if (i_this->mPlayerDistXZ < 500.0f) {
            i_this->mMoveMode = 2;
            anm_init(i_this, B_GND_BCK_EGND_WAIT02, 10.0f, 2, 1.0f);
            i_this->field_0xc44[0] = cM_rndF(50.0f) + 30.0f;
        } else if (i_this->field_0xc44[0] == 1) {
            if (i_this->mPlayerDistXZ < 800.0f && cM_rndF(1.0f) < 0.3f) {
                i_this->mActionMode = ACTION_ATTACK;
                i_this->mMoveMode = 10;
            } else {
                i_this->mActionMode = ACTION_JUMP;
                i_this->mMoveMode = 0;
            }
        }
        break;
    case 2:
        if (i_this->field_0xc44[8] != 0) {
            if (i_this->mPlayerDistXZ < 300.0f) {
                i_this->field_0xc44[8] = 0;
            }

            if (i_this->field_0xc44[8] == 1) {
                i_this->mActionMode = ACTION_ATTACK;
                i_this->mMoveMode = 10;
            }
        } else {
            if (i_this->mPlayerDistXZ > 650.0f) {
                i_this->mMoveMode = 0;
            }

            if (i_this->mPlayerDistXZ > 550.0f) {
                i_this->mActionMode = ACTION_ATTACK;
                if (cM_rndF(1.0f) < 0.5f) {
                    i_this->mMoveMode = 10;
                } else {
                    i_this->mMoveMode = 15;
                }
            } else if (i_this->mPlayerDistXZ < 150.0f) {
                i_this->mActionMode = ACTION_ATTACK;
                i_this->mMoveMode = 4;
            } else if (i_this->field_0xc44[0] == 0) {
                i_this->mActionMode = ACTION_ATTACK;
                if (i_this->mPlayerDistXZ < 300.0f) {
                    if (cM_rndF(1.0f) < 0.5f) {
                        i_this->mMoveMode = 0;
                    } else {
                        i_this->mMoveMode = 8;
                    }
                } else {
                    i_this->mMoveMode = 2;
                }
            }
        }
        break;
    case 3:
        if (i_this->mpModelMorf->isStop()) {
            i_this->mMoveMode = 0;
        }
        break;
    case 5:
        if (i_this->field_0xc44[0] == 1) {
            anm_init(i_this, B_GND_BCK_EGND_WAIT02, 10.0f, 2, 1.0f);
        }

        if (i_this->field_0xc44[0] == 0) {
            angle_step = 0;
        }

        i_this->field_0xc7d = 2;
        i_this->field_0xc80 = mgrod_p->hook_pos;
        break;
    case 6:
        angle_step = 0;
        i_this->field_0xc5a = 10;
        if (i_this->field_0xc44[0] == 0) {
            i_this->mMoveMode = 0;
        }
    }

    cLib_addCalc2(&a_this->speedF, speed_target, 1.0f, speed_step);
    if ((i_this->mCounter & 12) == 0) {
        angle_step /= 2;
    }

    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mPlayerAngleY, 2, angle_step);

    s16 angle_diff = a_this->current.angle.y - i_this->mPlayerAngleY;
    if (angle_diff > 0x100 || angle_diff < -0x100) {
        i_this->field_0x26c2 = 2500.0f * cM_ssin(i_this->mCounter * 5000);
    }

    if (i_this->mActionMode != ACTION_WAIT && mant_p->field_0x3969 == 2) {
        mant_p->field_0x3969 = 1;
    }
}

static BOOL b_gnd_g_attack(b_gnd_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    daPy_py_c* player = daPy_getPlayerActorClass();

    f32 speed_target = 0.0f;
    f32 speed_step = 2.0f;
    int anm_frame = i_this->mpModelMorf->getFrame();
    s16 angle_step = 0;

    mant_class* mant_p = (mant_class*)fopAcM_SearchByID(i_this->mMantChildID);

    BOOL spC = true;
    switch (i_this->mMoveMode) {
    case 0:
        i_this->mMoveMode = 1;
        anm_init(i_this, B_GND_BCK_EGND_ATTACKA, 3.0f, 0, 1.0f);
        break;
    case 1:
        if (20 <= anm_frame && anm_frame <= 28) {
            i_this->field_0xc77 = 1;
            mant_p->field_0x395c = 1.0f;
            spC = false;
        }

        if (44 <= anm_frame && anm_frame <= 50) {
            mant_p->field_0x395c = 3.0f / 5.0f;
        }

        if (anm_frame == 25) {
            i_this->field_0x2698 = 1;
        }

        if (20 <= anm_frame && anm_frame <= 26) {
            i_this->mSwordBlurAlpha = 80;
            i_this->field_0xc28 = -3000;
        }

        if (i_this->mpModelMorf->isStop()) {
            i_this->mActionMode = ACTION_WAIT;
            i_this->mMoveMode = 0;
        }
        break;
    case 2:
        i_this->mMoveMode = 3;
        anm_init(i_this, B_GND_BCK_EGND_ATTACKB, 3.0f, 0, 1.0f);
        break;
    case 3:
        if (anm_frame < 25) {
            spC = false;
        }

        if (17 <= anm_frame && anm_frame <= 23) {
            i_this->field_0xc77 = 1;
        }

        if (17 <= anm_frame && anm_frame <= 19) {
            mant_p->field_0x395c = 0.7f;
        }

        if (37 <= anm_frame && anm_frame <= 47) {
            mant_p->field_0x395c = 0.6f;
        }

        if (anm_frame == 20) {
            i_this->field_0x2698 = 1;
        }

        if (i_this->mpModelMorf->isStop()) {
            i_this->mActionMode = ACTION_WAIT;
            i_this->mMoveMode = 0;
        }
        break;
    case 4:
        i_this->mMoveMode = 5;
        anm_init(i_this, B_GND_BCK_EGND_ATTACKD, 3.0f, 0, 1.0f);
        break;
    case 5:
        if (3 <= anm_frame && anm_frame <= 10) {
            mant_p->field_0x395c = 0.6f;
        }

        if (3 <= anm_frame && anm_frame <= 10) {
            i_this->field_0xc77 = 1;
            i_this->field_0xc78 = 1;
            spC = false;
        }

        if (anm_frame == 6) {
            i_this->field_0x2698 = 1;
        }

        if (i_this->mpModelMorf->isStop()) {
            i_this->mActionMode = ACTION_WAIT;
            i_this->mMoveMode = 0;
        }
        break;
    case 6:
        i_this->mMoveMode = 7;
        anm_init(i_this, B_GND_BCK_EGND_ATTACKF, 5.0f, 0, 1.0f);
        break;
    case 7:
        i_this->mDamageInvulnerabilityTimer = 10;
        if (anm_frame < 35) {
            spC = false;
        }
        speed_step = 3.3299999f;
        
        if (13 <= anm_frame && anm_frame <= 33) {
            speed_target = 10.0f;
        }

        if (15 <= anm_frame && anm_frame <= 38) {
            i_this->field_0xc77 = 1;
            mant_p->field_0x395c = 0.7f;
        }

        if (15 <= anm_frame && anm_frame <= 30) {
            i_this->mSwordBlurAlpha = 80;
            i_this->field_0xc28 = -3000;
        }

        if (anm_frame == 20 || anm_frame == 35) {
            i_this->field_0x2698 = 1;
        }

        if (i_this->mpModelMorf->isStop()) {
            i_this->mActionMode = ACTION_WAIT;
            i_this->mMoveMode = 0;
        }
        break;
    case 8:
        i_this->mMoveMode = 9;
        anm_init(i_this, B_GND_BCK_EGND_ATTACKG, 3.0f, 0, 1.0f);
        break;
    case 9:
        if (anm_frame < 25) {
            spC = false;
        }

        if ((18 <= anm_frame && anm_frame <= 24) || (33 <= anm_frame && anm_frame <= 40)) {
            i_this->field_0xc77 = 1;
            mant_p->field_0x395c = 0.7f;
            i_this->mSwordBlurAlpha = 80;
        }

        if (anm_frame == 20 || anm_frame == 38) {
            i_this->field_0x2698 = 1;
        }

        if (i_this->mpModelMorf->isStop()) {
            i_this->mActionMode = ACTION_WAIT;
            i_this->mMoveMode = 0;
        }
        break;
    case 10:
        anm_init(i_this, B_GND_BCK_EGND_ATTACKC_A, 3.0f, 0, 1.0f);
        i_this->mMoveMode = 11;
        i_this->mDamageInvulnerabilityTimer = 10;
        break;
    case 11:
        i_this->mDamageInvulnerabilityTimer = 10;
        angle_step = 0x800;

        if (anm_frame >= 15) {
            speed_target = 30.0f;
            speed_step = 10.0f;
            spC = false;
        }

        if (!i_this->mpModelMorf->isStop()) {
            break;
        }

        anm_init(i_this, B_GND_BCK_EGND_ATTACKC_B, 0.0f, 2, 1.0f);
        i_this->mMoveMode = 12;
        i_this->field_0xc44[0] = 50;
    case 12:
        spC = false;
        i_this->mDamageInvulnerabilityTimer = 10;
        i_this->field_0xc79 = 1;
        mant_p->field_0x395c = 1.0f;
        speed_target = 30.0f;
        speed_step = 15.0f;
        angle_step = 0x800;
        i_this->field_0x2698 = 2;

        if (player->checkMasterSwordEquip() && i_this->mPlayerDistXZ < 600.0f) {
            dComIfGp_setDoStatusForce(107, 0);
            i_this->field_0x2740 = 1;
        }

        if (i_this->field_0xc44[0] == 0 || i_this->mPlayerDistXZ < 450.0f) {
            anm_init(i_this, B_GND_BCK_EGND_ATTACKC_C, 1.0f, 0, 1.0f);
            i_this->mMoveMode = 13;
        }
        break;
    case 13:
        i_this->mDamageInvulnerabilityTimer = 5;
        i_this->field_0xc79 = 1;
        speed_step = 10.0f;

        if (anm_frame < 30) {
            spC = false;
        }

        if (18 <= anm_frame && anm_frame <= 26) {
            mant_p->field_0x395c = 1.0f;
        }

        if (43 <= anm_frame && anm_frame <= 53) {
            mant_p->field_0x395c = 0.6f;
        }

        if (20 <= anm_frame && anm_frame <= 32) {
            i_this->field_0xc77 = 2;
        }

        if (18 <= anm_frame && anm_frame <= 23) {
            i_this->mSwordBlurAlpha = 80;
        }

        if (anm_frame == 25) {
            i_this->field_0x2698 = 1;
        }

        if (i_this->mpModelMorf->isStop()) {
            i_this->mActionMode = ACTION_WAIT;
            i_this->mMoveMode = 0;
        }

        if (anm_frame < 28 && player->checkMasterSwordEquip()) {
            dComIfGp_setDoStatusForce(107, 0);
            i_this->field_0x2740 = 1;
            if (cc_pl_cut_bit_get() == 0x100 && player->speed.y < 0.0f) {
                i_this->mActionMode = ACTION_TUBA;
                i_this->mMoveMode = 0;
            }
        }
        break;
    case 15:
        anm_init(i_this, B_GND_BCK_EGND_ATTACKE_A, speed_step, 0, 1.0f);
        i_this->mMoveMode = 16;
        i_this->mDamageInvulnerabilityTimer = 10;
        i_this->field_0xc44[0] = 11;
        break;
    case 16:
        i_this->mDamageInvulnerabilityTimer = 10;
        spC = false;
        angle_step = 0x800;
        
        if (anm_frame >= 5) {
            speed_target = 60.0f;
            speed_step = 30.0f;
            if (anm_frame == 5) {
                a_this->speed.y = 20.0f;
            }
        }

        if (i_this->field_0xc44[0] != 0)
            break;

        anm_init(i_this, B_GND_BCK_EGND_ATTACKE_B, 1.0f, 0, 1.0f);
        i_this->mMoveMode = 17;
    case 17:
        spC = false;
        i_this->mDamageInvulnerabilityTimer = 10;
        speed_step = 15.0f;
        angle_step = 0x800;

        if (2 <= anm_frame && anm_frame <= 12) {
            i_this->field_0xc78 = 2;
            i_this->field_0xc77 = 1;
        }

        if (anm_frame >= 5) {
            mant_p->field_0x395c = 0.7f;
        }
        
        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, B_GND_BCK_EGND_ATTACKE_C2, 1.0f, 0, 1.0f);
            i_this->mMoveMode = 18;
        }
        break;
    case 18:
        speed_step = 15.0f;
        if (10 <= anm_frame && anm_frame <= 20) {
            i_this->field_0xc77 = 1;
            mant_p->field_0x395c = 0.7f;
        }

        if (i_this->mpModelMorf->isStop()) {
            i_this->mActionMode = ACTION_WAIT;
            i_this->mMoveMode = 0;
        }
    }

    cLib_addCalc2(&a_this->speedF, speed_target, 1.0f, speed_step);
    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mPlayerAngleY, 4, angle_step);
    return spC;
}

static void b_gnd_g_defence(b_gnd_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    int anm_frame = i_this->mpModelMorf->getFrame();
    s16 angle_step = 0;
    mant_class* mant_p = (mant_class *)fopAcM_SearchByID(i_this->mMantChildID);
    i_this->mDamageInvulnerabilityTimer = 10;

    cc_pl_cut_bit_get();

    switch (i_this->mMoveMode) {
    case 0:
        i_this->mMoveMode = 1;

        if (cM_rndF(1.0) < 0.5f) {
            if (i_this->field_0xc7b & 1) {
                anm_init(i_this, B_GND_BCK_EGND_DEFENSE_A, 3.0f, 0, 1.0f);
            } else {
                anm_init(i_this, B_GND_BCK_EGND_DEFENSE_B, 3.0f, 0, 1.0f);
            }
        } else if (i_this->field_0xc7b & 1) {
            anm_init(i_this, B_GND_BCK_EGND_DEFENSE_C, 3.0f, 0, 1.0f);
        } else {
            anm_init(i_this, B_GND_BCK_EGND_DEFENSE_D, 3.0f, 0, 1.0f);
        }

        i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_DEFENCE, -1);
        i_this->field_0xc44[0] = 20;
        i_this->field_0xc7a = 0;
        break;
    case 1:
    case 2:
        if (i_this->mMoveMode == 1) {
            if (i_this->field_0xc44[0] == 0 && daPy_getPlayerActorClass()->getCutAtFlg()) {
                i_this->mMoveMode = 2;
                i_this->field_0xc7b++;
    
                if (cM_rndF(1.0) < 0.5f) {
                    if (i_this->field_0xc7b & 1) {
                        anm_init(i_this, B_GND_BCK_EGND_DEFENSE_A, 3.0f, 0, 1.0f);
                    } else {
                        anm_init(i_this, B_GND_BCK_EGND_DEFENSE_B, 3.0f, 0, 1.0f);
                    }
                } else if (i_this->field_0xc7b & 1) {
                    anm_init(i_this, B_GND_BCK_EGND_DEFENSE_C, 3.0f, 0, 1.0f);
                } else {
                    anm_init(i_this, B_GND_BCK_EGND_DEFENSE_D, 3.0f, 0, 1.0f);
                }
    
                i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_DEFENCE, -1);
    
                if (i_this->mAcch.ChkWallHit()) {
                    i_this->mActionMode = ACTION_JUMP;
                    i_this->mMoveMode = 0;
                } else if ((i_this->field_0xc7a >= 3 || cM_rndF(0.1f)) && cM_rndF(1.0f) < 0.35f) {
                    i_this->mActionMode = ACTION_JUMP;
                    i_this->mMoveMode = 0;
                }
            }
        } else if (!daPy_getPlayerActorClass()->getCutAtFlg()) {
            i_this->mMoveMode = 1;
        }

        if (anm_frame < 10) {
            angle_step = 0x1000;
        }

        if (cc_pl_cut_bit_get() && 5 < anm_frame && anm_frame < 10) {
            i_this->mpModelMorf->setFrame(anm_frame - 1);
        }

        if (anm_frame < 10) {
            i_this->field_0xc79 = 1;
        }

        if (i_this->mpModelMorf->isStop()) {
            i_this->mActionMode = ACTION_WAIT;
            i_this->mMoveMode = 0;
        }
        break;
    case 5:
        if (i_this->field_0xc44[0] != 0) {
            a_this->current.angle.y += 0x1800;
            i_this->field_0xeb0 = 30.0f;
            mant_p->field_0x395c = 1.2f;
        } else {
            i_this->field_0x2698 = 1;
            i_this->mActionMode = ACTION_WAIT;
            i_this->mMoveMode = 3;
            anm_init(i_this, B_GND_BCK_EGND_CHOUHATU, 10.0f, 0, 1.0f);
        }

        a_this->shape_angle.y = a_this->current.angle.y;
        break;
    }

    cLib_addCalc2(&a_this->speedF, 0.0f, 1.0f, 10.0f);
    cLib_addCalcAngleS2(&a_this->current.angle.y, i_this->mPlayerAngleY, 2, angle_step);

    if (i_this->mDefSph.ChkTgHit()) {
        i_this->field_0xeb0 = 25.0f;
        i_this->field_0xeac = a_this->shape_angle.y;
        mant_p->field_0x395c = 0.3f;
        i_this->field_0xc7a++;
        i_this->field_0x2698 = 1;
    }
}

static int b_gnd_g_jump(b_gnd_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    f32 var_f31 = 0.0f;
    f32 var_f30 = 10.0f;
    int sp10 = 1;
    mant_class* mant_p = (mant_class*)fopAcM_SearchByID(i_this->mMantChildID);

    switch (i_this->mMoveMode) {
    case 0:
        anm_init(i_this, B_GND_BCK_EGND_JUMP_A, 2.0f, 0, 1.0f);
        i_this->mMoveMode = 1;
        i_this->mDamageInvulnerabilityTimer = 100;
        break;
    case 1:
        if (!i_this->mpModelMorf->isStop()) {
            break;
        }

        anm_init(i_this, B_GND_BCK_EGND_JUMP_B, 0.0f, 2, 1.0f);
        i_this->mMoveMode = 2;

        if (i_this->mPlayerDistXZ > 500.0f) {
            a_this->speedF = cM_rndF(10.0f) + 30.0f;
            a_this->current.angle.y += (int)cM_rndFX(3000.0f);
        } else {
            a_this->speedF = cM_rndF(10.0f) + 10.0f;
            a_this->current.angle.y += (int)cM_rndFX(6000.0f);
        }
        
        a_this->speed.y = 70.0f;
        i_this->field_0x2698 = 1;
        break;
    case 2:
        sp10 = 0;
        cLib_addCalcAngleS2(&a_this->shape_angle.y, i_this->mPlayerAngleY, 2, 0x600);
        mant_p->field_0x3964 = 1;

        if (i_this->mAcch.ChkGroundHit()) {
            if (i_this->mPlayerDistXZ < 400.0f) {
                i_this->mActionMode = ACTION_ATTACK;
                i_this->mMoveMode = 6;
            } else {
                i_this->mActionMode = ACTION_ATTACK;
                i_this->mMoveMode = 3;
                anm_init(i_this, B_GND_BCK_EGND_ATTACKB, 3.0f, 0, 1.0f);
                i_this->mpModelMorf->setFrame(10.0f);
            }

            a_this->current.angle.y = a_this->shape_angle.y;
            a_this->speedF = 0.0f;

            mant_p->field_0x395c = 1.0f;
            i_this->mDamageInvulnerabilityTimer = 0;
            i_this->field_0x2698 = 1;
            i_this->mGndSound.startCreatureSound(Z2SE_EN_GND_LAND, 0, -1);
        }
        break;
    case 4:
        cLib_addCalcAngleS2(&a_this->shape_angle.y, i_this->mPlayerAngleY, 2, 0x600);
        a_this->current.angle.y = a_this->shape_angle.y;

        if (i_this->field_0xc44[0] == 0) {
            i_this->mActionMode = ACTION_ATTACK;
            if (i_this->mPlayerDistXZ < 150.0f) {
                i_this->mMoveMode = 4;
            } else if (i_this->mPlayerDistXZ < 300.0f) {
                i_this->mMoveMode = 0;
            } else if (cM_rndF(1.0f) < 0.5f) {
                i_this->mMoveMode = 2;
            } else {
                i_this->mMoveMode = 10;
            }
        }
        break;
    case 5:
        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, B_GND_BCK_EGND_WAIT02, 10.0f, 2, 1.0f);
        }

        if (i_this->field_0xc44[0] == 0) {
            i_this->mMoveMode = 4;
        }
    }

    return sp10;
}

static void b_gnd_g_side(b_gnd_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    mant_class* mant_p = (mant_class*)fopAcM_SearchByID(i_this->mMantChildID);
    i_this->mDamageInvulnerabilityTimer = 10;

    switch (i_this->mMoveMode) {
    case 0:
        if (cM_rndF(1.0f) < 0.5f) {
            anm_init(i_this, B_GND_BCK_EGND_SIDEJUMP_LA, 2.0f, 0, 1.0f);
            a_this->current.angle.y = i_this->mPlayerAngleY + 0x3000;
        } else {
            anm_init(i_this, B_GND_BCK_EGND_SIDEJUMP_RA, 2.0f, 0, 1.0f);
            a_this->current.angle.y = i_this->mPlayerAngleY + -0x3000;
        }

        a_this->speedF = 30.0f;
        a_this->speed.y = 30.0f;
        i_this->mMoveMode = 1;
        mant_p->field_0x395c = 1.0f;
        break;
    case 1:
        cLib_addCalcAngleS2(&a_this->shape_angle.y, i_this->mPlayerAngleY, 2, 0x600);

        if (i_this->mAcch.ChkGroundHit()) {
            if (i_this->mAnmID == B_GND_BCK_EGND_SIDEJUMP_LA) {
                anm_init(i_this, B_GND_BCK_EGND_SIDEJUMP_LB, 1.0f, 0, 1.0f);
            } else {
                anm_init(i_this, B_GND_BCK_EGND_SIDEJUMP_RB, 1.0f, 0, 1.0f);
            }

            i_this->mMoveMode = 2;
            i_this->field_0x2698 = 1;
            i_this->mGndSound.startCreatureSound(Z2SE_EN_GND_LAND, 0, -1);
        }
        break;
    case 2:
        cLib_addCalc0(&a_this->speedF, 1.0f, 6.0f);

        if (i_this->mpModelMorf->isStop()) {
            if (i_this->mPlayerDistXZ < 500.0f) {
                i_this->mActionMode = ACTION_ATTACK;
                if (i_this->mPlayerDistXZ < 300.0f) {
                    i_this->mMoveMode = 6;
                } else {
                    i_this->mMoveMode = 3;
                    anm_init(i_this, B_GND_BCK_EGND_ATTACKB, 3.0f, 0, 1.0f);
                    i_this->mpModelMorf->setFrame(10.0f);
                }
            } else {
                i_this->mActionMode = ACTION_WAIT;
                if (cM_rndF(1.0f) < 0.5f) {
                    i_this->mMoveMode = 0;
                } else {
                    i_this->mMoveMode = 3;
                    anm_init(i_this, B_GND_BCK_EGND_CHOUHATU, 10.0f, 0, 1.0f);
                }
            }

            a_this->current.angle.y = a_this->shape_angle.y;
        }
    }
}

static void b_gnd_g_tuba(b_gnd_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    mant_class* mant_p = (mant_class*)fopAcM_SearchByID(i_this->mMantChildID);
    i_this->mDamageInvulnerabilityTimer = 10;

    switch (i_this->mMoveMode) {
    case 0:
        anm_init(i_this, B_GND_BCK_EGND_TUBAZERI_A, 5.0f, 2, 1.0f);
        i_this->mMoveMode = 1;
        mant_p->field_0x395c = 1.5f;
        i_this->mDemoCamMode = 50;
        a_this->speedF = 0.0f;
        dComIfGp_getVibration().StartShock(VIBMODE_S_POWER6, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
        break;
    case 1:
        break;
    }
}

static void b_gnd_g_damage(b_gnd_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    daPy_py_c* player = daPy_getPlayerActorClass();
    int anm_frame = i_this->mpModelMorf->getFrame();
    mant_class* mant_p = (mant_class*)fopAcM_SearchByID(i_this->mMantChildID);

    switch (i_this->mMoveMode) {
    case 0:
        i_this->mMoveMode = 1;
        if (cM_rndF(1.0f) < 0.5f) {
            anm_init(i_this, B_GND_BCK_EGND_DAMAGE_A, 3.0f, 0, 1.0f);
        } else {
            anm_init(i_this, B_GND_BCK_EGND_DAMAGE_B, 3.0f, 0, 1.0f);
        }

        i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_DAMAGE, -1);
        a_this->current.angle.y = player->shape_angle.y + 0x8000;
        i_this->field_0xeb0 = 25.0f;
        i_this->field_0xeac = a_this->current.angle.y;
        mant_p->field_0x395c = 0.5f;
        break;
    case 1:
        if ((i_this->mDamageInvulnerabilityTimer <= 50 || anm_frame <= 10) && !i_this->mpModelMorf->isStop()) {
            break;
        }

        i_this->mDamageInvulnerabilityTimer = 15;
        i_this->mActionMode = ACTION_WAIT;
        i_this->mMoveMode = 0;
        i_this->field_0x1e0a = 0;
        i_this->field_0xc7c = 0;
    }
    
    cLib_addCalc2(&a_this->speedF, 0.0f, 1.0f, 10.0f);
}

static void b_gnd_g_down(b_gnd_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    fopEn_enemy_c* e_this = (fopEn_enemy_c*)a_this;

    f32 var_f31 = 0.0f;
    f32 var_f30 = 10.0f;
    int anm_frame = (int)i_this->mpModelMorf->getFrame();

    mant_class* mant_p = (mant_class*)fopAcM_SearchByID(i_this->mMantChildID);
    i_this->mDamageInvulnerabilityTimer = 10;

    switch(i_this->mMoveMode) {
    case 0:
        i_this->field_0x1e0c++;
        if (i_this->field_0x1e0c >= 3) {
            anm_init(i_this, B_GND_BCK_EGND_DOWN, 3.0f, 0, 1.0f);
        } else {
            anm_init(i_this, B_GND_BCK_EGND_DOWNB, 3.0f, 0, 1.0f);
            i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_DAMAGE, -1);
        }

        a_this->current.angle.y = i_this->mPlayerAngleY + 0x8000;
        i_this->mMoveMode = 1;
        
        mant_p->field_0x395c = 1.0f;

        a_this->speedF = 30.0f;
        a_this->speed.y = 35.0f;
        return;
    case 1:
        if (anm_frame >= 16) {
            cLib_addCalc0(&a_this->speedF, 1.0f, 3.0f);

            if (anm_frame == 16) {
                cXyz eff_pos;
                MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(1), mDoMtx_stack_c::get());
                mDoMtx_stack_c::multVecZero(&eff_pos);

                fopAcM_effSmokeSet1(&i_this->field_0x25b4, &i_this->field_0x25b8, &eff_pos, &a_this->shape_angle, 2.5f, &i_this->tevStr, 1);
                dComIfGp_getVibration().StartShock(VIBMODE_S_POWER6, 0x1F, cXyz(0.0f, 1.0f, 0.0f));
                mant_p->field_0x3960 = 9.0f;

                if (i_this->mAnmID == B_GND_BCK_EGND_DOWNB) {
                    mant_p->field_0x395c = 1.0f;
                }
            }

            if (i_this->mAnmID == B_GND_BCK_EGND_DOWN && anm_frame >= 22) {
                mant_p->field_0x3965 = 1;
            }
        } else {
            mant_p->field_0x3964 = 1;
        }

        if (i_this->mAnmID == B_GND_BCK_EGND_DOWNB && anm_frame == 48) {
            mant_p->field_0x395c = 1.0f;
        }

        if (!i_this->mpModelMorf->isStop()) {
            return;
        }

        a_this->speedF = 0.0f;

        if (i_this->mAnmID == B_GND_BCK_EGND_DOWN) {
            anm_init(i_this, B_GND_BCK_EGND_DOWNWAIT, 3.0f, 2, 1.0f);
            i_this->mMoveMode = 2;
            i_this->field_0xc44[0] = 200;

            if (daPy_getPlayerActorClass()->checkMasterSwordEquip()) {
                e_this->onDownFlg();
            }
            return;
        }

        i_this->mActionMode = ACTION_WAIT;
        i_this->mMoveMode = 0;
        a_this->current.angle.y = a_this->shape_angle.y;
        return;
    case 2:
        mant_p->field_0x3965 = 1;
        if (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_DOWN) {
            i_this->mDemoCamMode = 60;
            i_this->mActionMode = ACTION_END;
            i_this->mMoveMode = 0;
            Z2GetAudioMgr()->bgmStop(30, 0);
        }

        if (i_this->field_0xc44[0] != 0 && daPy_getPlayerActorClass()->checkMasterSwordEquip()) {
            return;
        }

        anm_init(i_this, B_GND_BCK_EGND_DOWNUP, 3.0f, 0, 1.0f);
        a_this->current.angle.y = a_this->shape_angle.y;
        i_this->mMoveMode = 3;
        e_this->offDownFlg();
    case 3:
        if (anm_frame <= 10) {
            mant_p->field_0x3965 = 1;
            if (anm_frame == 10) {
                mant_p->field_0x395c = 1.0f;
            }
        }

        if (i_this->mpModelMorf->isStop()) {
            i_this->mActionMode = ACTION_WAIT;
            i_this->mMoveMode = 0;
        }
    }
}

static void b_gnd_g_end(b_gnd_class* i_this) {
    i_this->mDamageInvulnerabilityTimer = 10;
    mant_class* mant_p = (mant_class*)fopAcM_SearchByID(i_this->mMantChildID);
    mant_p->field_0x3965 = 1;

    if (i_this->mMoveMode == 0) {
        return;
    }
}

static void damage_check(b_gnd_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    i_this->mGndCcStts.Move();

    if (i_this->field_0x1e09 != 0) {
        i_this->field_0x1e09--;
    }

    if (i_this->mDamageInvulnerabilityTimer == 0) {
        mant_class* mant_p = (mant_class*)fopAcM_SearchByID(i_this->mMantChildID);
        BOOL hit_down = FALSE;

        for (int i = 0; i < 3; i++) {
            if (i_this->mGndSph[i].ChkTgHit()) {
                i_this->mAtInfo.mpCollider = i_this->mGndSph[i].GetTgHitObj();
                cc_at_check(a_this, &i_this->mAtInfo);

                if (i_this->mAtInfo.mpCollider->ChkAtType(AT_TYPE_BOOMERANG)) {
                    mant_p->field_0x395c = 1.3f;
                    i_this->mDamageInvulnerabilityTimer = 6;
                    i_this->mGndSound.startCreatureSound(Z2SE_EN_GND_MANTEAU, 0, -1);
                    return;
                }

                if (i_this->field_0x1e09 != 0) {
                    hit_down = TRUE;
                }

                if (i_this->health <= 0 || (i_this->field_0x1e0a >= 3 && daPy_getPlayerActorClass()->getCutCount() >= 4)) {
                    hit_down = TRUE;
                } else {
                    i_this->mActionMode = ACTION_DAMAGE;
                    i_this->mMoveMode = 0;
                    i_this->field_0x2698 = 1;

                    if (i_this->mAtInfo.mHitStatus != 0) {
                        mant_p->field_0x3967 = 3;
                    } else {
                        mant_p->field_0x3967 = 2;
                    }

                    if (player->getCutType() == daPy_py_c::CUT_TYPE_JUMP && player->checkCutJumpCancelTurn()) {
                        i_this->mDamageInvulnerabilityTimer = 3;
                        i_this->field_0x1e09 = 10;
                    } else {
                        i_this->mDamageInvulnerabilityTimer = 10;
                        i_this->field_0x1e0a++;

                        if (i_this->field_0xc7c == 0 && i_this->field_0x1e0a >= 3) {
                            i_this->mDamageInvulnerabilityTimer = 100;
                        }
                    }
                }

                cXyz hitmark_size(1.0f, 1.0f, 1.0f);
                u16 hitmark;
                if (hit_down) {
                    hitmark = 3;
                } else {
                    hitmark = 1;
                }
                
                cXyz ato;
                cXyz mae;
                MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(2), *calc_mtx);
                mae.x = cM_rndFX(10.0f);
                mae.y = cM_rndFX(10.0f) + 50.0f;
                mae.z = cM_rndFX(10.0f);
                MtxPosition(&mae, &ato);
                dComIfGp_setHitMark(hitmark, a_this, &ato, NULL, &hitmark_size, 0);

                if (hit_down) {
                    i_this->mActionMode = ACTION_DOWN;
                    i_this->mMoveMode = 0;
                    i_this->mDamageInvulnerabilityTimer = 10;
                    i_this->field_0xc7c = 0;
                    dScnPly_c::setPauseTimer(7);
                    a_this->health = 100;
                }
                break;
            }
        }
    }
}

static void eff_set_h(b_gnd_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz sp44;
    cXyz sp38;

    if (i_this->mHorseAnmID == B_HG_BCK_HG_RUN) {
        if (i_this->mpHorseMorf->checkFrame(5.0f)) {
            i_this->field_0x2698 |= 3;
        } else if (i_this->mpHorseMorf->checkFrame(20.0f)) {
            i_this->field_0x2698 |= 12;
        }

        i_this->field_0xc6a = 1;
    }

    dBgS_GndChk spA4;
    dBgS_ObjGndChk_Spl sp50;

    sp44 = i_this->current.pos;
    sp44.y += 100.0f;

    spA4.SetPos(&sp44);
    sp50.SetPos(&sp44);

    s8 var_r27 = FALSE;
    s8 var_r26 = FALSE;

    if (dComIfG_Bgsp().GroundCross(&sp50) - dComIfG_Bgsp().GroundCross(&spA4) > 0.0f) {
        var_r27 = TRUE;
    }

    if (i_this->field_0x2698 != 0) {
        J3DModel* model = i_this->mpHorseMorf->getModel();
        sp44.set(0.0f, 0.0f, 0.0f);

        if (i_this->field_0x2698 & 1) {
            MTXCopy(model->getAnmMtx(10), *calc_mtx);
            MtxPosition(&sp44, &sp38);

            if (var_r27) {
                var_r26 = TRUE;
            } else {
                fopAcM_effSmokeSet2(&i_this->field_0x25ac[1], &i_this->field_0x25d0, &sp38, &i_this->shape_angle, i_this->scale.z * 2.0f, &i_this->tevStr);
                if (i_this->field_0xc6a != 0) {
                    i_this->field_0x25c0 = dComIfGp_particle_set(i_this->field_0x25c0, 0x80E3, &sp38, &i_this->shape_angle, 0);
                }
            }
        }

        if (i_this->field_0x2698 & 2) {
            MTXCopy(model->getAnmMtx(6), *calc_mtx);
            MtxPosition(&sp44, &sp38);

            if (var_r27) {
                var_r26 = TRUE;
            } else {
                fopAcM_effSmokeSet2(&i_this->field_0x25ac[0], &i_this->field_0x25cc, &sp38, &i_this->shape_angle, i_this->scale.z * 2.0f, &i_this->tevStr);
                if (i_this->field_0xc6a != 0) {
                    i_this->field_0x25bc = dComIfGp_particle_set(i_this->field_0x25bc, 0x80E3, &sp38, &i_this->shape_angle, 0);
                }
            }
        }

        if (i_this->field_0x2698 & 4) {
            MTXCopy(model->getAnmMtx(34), *calc_mtx);
            MtxPosition(&sp44, &sp38);

            if (var_r27) {
                var_r26 = TRUE;
            } else {
                fopAcM_effSmokeSet2(&i_this->field_0x25b4, &i_this->field_0x25d4, &sp38, &i_this->shape_angle, i_this->scale.z * 2.0f, &i_this->tevStr);
                if (i_this->field_0xc6a != 0) {
                    i_this->field_0x25c4 = dComIfGp_particle_set(i_this->field_0x25c4, 0x80E3, &sp38, &i_this->shape_angle, 0);
                }
            }
        }

        if (i_this->field_0x2698 & 8) {
            MTXCopy(model->getAnmMtx(30), *calc_mtx);
            MtxPosition(&sp44, &sp38);

            if (var_r27) {
                var_r26 = TRUE;
            } else {
                fopAcM_effSmokeSet2(&i_this->field_0x25b8, &i_this->field_0x25d8, &sp38, &i_this->shape_angle, i_this->scale.z * 2.0f, &i_this->tevStr);
                if (i_this->field_0xc6a != 0) {
                    i_this->field_0x25c8 = dComIfGp_particle_set(i_this->field_0x25c8, 0x80E3, &sp38, &i_this->shape_angle, 0);
                }
            }
        }
    }

    if (i_this->field_0x2699 != 0) {
        sp44.set(0.0f, 0.0f, 0.0f);
        MTXCopy(i_this->mpHorseMorf->getModel()->getAnmMtx(2), *calc_mtx);
        MtxPosition(&sp44, &sp38);

        f32 var_f30 = 2.5f;
        if (i_this->field_0x2699 == 2) {
            var_f30 = 4.0f;
        }

        if (var_r27) {
            var_r26 = TRUE;
        } else {
            fopAcM_effSmokeSet1(&i_this->field_0x25dc, &i_this->field_0x25e0, &sp38, NULL, var_f30, &i_this->tevStr, 1);
        }
    }

    if (var_r26) {
        cXyz sp2C(2.0f, 2.0f, 2.0f);
        csXyz sp24(0, 0, 0);

        sp38.y = dComIfG_Bgsp().GroundCross(&spA4);
        for (int i = 0; i < 3; i++) {
            static u16 w_eff_name[3] = {0x825B, 0x825C, 0x825D};
            static u16 w_eff_name2[3] = {0x8258, 0x8259, 0x825A};

            i_this->field_0x25e4[i] = dComIfGp_particle_set(i_this->field_0x25e4[i],
                w_eff_name[i], &sp38, &i_this->tevStr,
                &sp24, &sp2C, -1, NULL,
                -1, NULL, NULL, NULL);
        }
    }

    i_this->field_0x2698 = 0;
    i_this->field_0xc6a = 0;
    i_this->field_0x2699 = 0;

    if (i_this->field_0x1e08 != 0) {
        MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(1), mDoMtx_stack_c::get());
        mDoMtx_stack_c::multVecZero(&sp38);

        for (int i = 0; i < 3; i++) {
            static u16 e_name[3] = {0x8B95, 0x8B96, 0x8B97};
            i_this->field_0x268c[i] = dComIfGp_particle_set(i_this->field_0x268c[i], e_name[i], &sp38, NULL, 0);
        }

        i_this->mGndSound.startCreatureSoundLevel(Z2SE_EN_GND_DENKIBIRI, 0, -1);
    }
}

static void eff_set(b_gnd_class* i_this) {
    if (i_this->field_0x2698 == 2) {
        if (i_this->mCounter & 1) {
            fopAcM_effSmokeSet2(&i_this->field_0x25ac[0], &i_this->field_0x25ac[1],
                &i_this->field_0x26a8[(i_this->mCounter & 2) >> 1], &i_this->shape_angle,
                2.0f, &i_this->tevStr);
        }
    } else if (i_this->field_0x2698 == 1) {
        for (int i = 0; i < 2; i++) {
            fopAcM_effSmokeSet2((&i_this->field_0x25ac[i]), (&i_this->field_0x25ac[i + 2]),
            &i_this->field_0x26a8[i], &i_this->shape_angle, 2.5f, &i_this->tevStr);
        }
    }

    i_this->field_0x2698 = 0;
}

static void* s_fkdel_sub(void* i_actor, void* i_data) {
    if (fopAc_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_FK) {
        fopAcM_delete((fopAc_ac_c*)i_actor);
    }

    return NULL;
}

static void* s_objgbdel_sub(void* i_actor, void* i_data) {
    if (fopAc_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_OBJ_GB) {
        fopAcM_delete((fopAc_ac_c*)i_actor);
    }

    return NULL;
}

static void h_damage_check(b_gnd_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);

    i_this->mGndCcStts.Move();

    if (i_this->mDamageInvulnerabilityTimer == 0) {
        mant_class* mant_p = (mant_class*)fopAcM_SearchByID(i_this->mMantChildID);
        BOOL var_r27 = FALSE;

        for (int i = 0; i < 3; i++) {
            if (i_this->mGndSph[i].ChkTgHit()) {
                i_this->mAtInfo.mpCollider = i_this->mGndSph[i].GetTgHitObj();
                cc_at_check(a_this, &i_this->mAtInfo);
                i_this->mDamageInvulnerabilityTimer = 10;

                OS_REPORT(" GND AP %d\n", i_this->mAtInfo.mAttackPower);
                OS_REPORT(" GND HP2 %d\n", i_this->health);

                if (i_this->field_0x1e08 != 0) {
                    i_this->field_0x1e0a++;
                    mant_p->field_0x3967 = 1;
                    i_this->mDamageInvulnerabilityTimer = 30;

                    if (i_this->health <= 0) {
                        i_this->mActionMode = ACTION_HEND;
                        i_this->mMoveMode = 0;
                        fpcM_Search(s_fkdel_sub, i_this);
                        i_this->field_0x1e0a = 0;
                    } else {
                        i_this->mMoveMode = 11;
                        OS_REPORT(" GND PIYO DAM %d\n", i_this->field_0x1e0a);
                    }

                    var_r27 = true;
                    break;
                }

                if (((dCcD_GObjInf*)i_this->mAtInfo.mpCollider)->GetAtMtrl() == dCcD_MTRL_LIGHT) {
                    OS_REPORT(" GND LIGHT HITL\n");
                    if (i_this->field_0x1e0f == 1) {
                        i_this->field_0x1e0f = 0;
                        i_this->field_0x1e10 = 0.0f;
                    }

                    i_this->mActionMode = ACTION_HRUN_A;
                    i_this->mMoveMode = 7;
                    var_r27 = true;
                    break;
                }
            }
        }

        if (!var_r27) {
            for (int i = 0; i < 2; i++) {
                if (i_this->mHorseBodySph[i].ChkTgHit()) {
                    i_this->mAtInfo.mpCollider = i_this->mHorseBodySph[i].GetTgHitObj();
                    i_this->mHorseBodySph[i].ClrTgHit();

                    if (i_this->mDrawHorse && i_this->field_0x1e08 == 0 && ((dCcD_GObjInf*)i_this->mAtInfo.mpCollider)->GetAtMtrl() == dCcD_MTRL_LIGHT) {
                        OS_REPORT(" HGND LIGHT HITL 2\n");
                        i_this->mDamageInvulnerabilityTimer = 10;
                        if (i_this->field_0x1e0f == 1) {
                            i_this->field_0x1e0f = 0;
                            i_this->field_0x1e10 = 0.0f;
                        }

                        i_this->mActionMode = ACTION_HRUN_A;
                        i_this->mMoveMode = 7;
                        var_r27 = true;
                        break;
                    }
                }
            }
        }

        if (var_r27) {
            cXyz sp34(1.0f, 1.0f, 1.0f);
            u16 sp8;
            if (i_this->mAtInfo.mHitStatus != 0) {
                sp8 = 3;
            } else {
                sp8 = 1;
            }

            cXyz sp28;
            cXyz sp1C;
            MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(2), *calc_mtx);
            sp1C.x = cM_rndFX(10.0f);
            sp1C.y = cM_rndFX(10.0f) + 50.0f;
            sp1C.z = cM_rndFX(10.0f);
            MtxPosition(&sp1C, &sp28);
            dComIfGp_setHitMark(sp8, a_this, &sp28, NULL, &sp34, 0);
        }
    }
}

static void action(b_gnd_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz sp88;
    cXyz sp7C;

    i_this->mPlayerDistXZ = fopAcM_searchPlayerDistanceXZ(a_this);
    i_this->mPlayerAngleY = fopAcM_searchPlayerAngleY(a_this);

    if (i_this->field_0x1e08 != 0) {
        i_this->field_0x1e08--;
    }

    BOOL attn_on = TRUE;
    BOOL snd_link_search = TRUE;
    BOOL can_hjump = FALSE;
    BOOL sp38 = FALSE;
    BOOL sp34 = TRUE;

    i_this->field_0xc7d = 1;
    i_this->field_0x2740 = 0;

    switch (i_this->mActionMode) {
    case ACTION_HWAIT_1:
        b_gnd_h_wait(i_this);
        break;
    case ACTION_HWAIT_2:
        b_gnd_h_wait2(i_this);
        break;
    case ACTION_HRUN_A:
        b_gnd_h_run_a(i_this);
        can_hjump = true;
        break;
    case ACTION_HRUN_P:
        b_gnd_h_run_p(i_this);
        can_hjump = true;
        break;
    case ACTION_HJUMP:
        b_gnd_h_jump(i_this);
        break;
    case ACTION_HEND:
        b_gnd_h_end(i_this);
        sp34 = FALSE;
        attn_on = FALSE;
        i_this->field_0xc7d = 0;
        break;
    case ACTION_WAIT:
        b_gnd_g_wait(i_this);
        sp38 = TRUE;
        break;
    case ACTION_ATTACK:
        sp38 = b_gnd_g_attack(i_this);
        break;
    case ACTION_DEFENCE:
        b_gnd_g_defence(i_this);
        break;
    case ACTION_JUMP:
        attn_on = b_gnd_g_jump(i_this);
        sp34 = FALSE;
        break;
    case ACTION_SIDE:
        b_gnd_g_side(i_this);
        sp34 = FALSE;
        break;
    case ACTION_TUBA:
        b_gnd_g_tuba(i_this);
        break;
    case ACTION_DAMAGE:
        b_gnd_g_damage(i_this);
        i_this->field_0xc7d = 0;
        break;
    case ACTION_DOWN:
        b_gnd_g_down(i_this);
        sp34 = FALSE;
        i_this->field_0xc7d = 0;
        break;
    case ACTION_END:
        b_gnd_g_end(i_this);
        sp34 = FALSE;
        i_this->field_0xc7d = 0;
    }

#if DEBUG
    if (mDoCPd_c::getTrigR(PAD_2)) {
        dScnKy_env_light_c* kankyo = dKy_getEnvlight();
        kankyo->wether = 1;

        i_this->mActionMode = ACTION_WAIT;
        i_this->mMoveMode = 0;
        i_this->mDrawHorse = FALSE;
        i_this->speedF = 0.0f;

        mant_class* mant_p = (mant_class*)fopAcM_SearchByID(i_this->mMantChildID);
        mant_p->field_0x3969 = 1;

        h_anm_init(i_this, B_HG_BCK_HG_DOWN, 3.0f, 0, 1.0f);
        if (daPy_getPlayerActorClass()->checkHorseRide()) {
            i_this->mDemoCamMode = 90;
        }

        fpcM_Search(s_fkdel_sub, i_this);
        i_this->health = 100;
        i_this->field_0x1e0a = 0;
        i_this->shape_angle.z = 0;
        i_this->shape_angle.x = 0;
        i_this->current.angle.z = 0;
        i_this->current.angle.x = 0;

        Z2GetAudioMgr()->bgmStart(Z2BGM_VS_GANON_04, 0, 0);
        dComIfGs_onOneZoneSwitch(15, -1);
        dComIfGs_onSaveDunSwitch(1);
    }

    if (mDoCPd_c::getTrigZ(PAD_2)) {
        i_this->mActionMode = ACTION_HEND;
        i_this->mMoveMode = 0;
        i_this->mDrawHorse = TRUE;
        fpcM_Search(s_fkdel_sub, i_this);
    }
#endif

    if (snd_link_search) {
        i_this->mGndSound.setLinkSearch(true);
    } else {
        i_this->mGndSound.setLinkSearch(false);
    }

    if (attn_on) {
        fopAcM_OnStatus(i_this, 0);
        i_this->attention_info.flags = fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OffStatus(i_this, 0);
        i_this->attention_info.flags = 0;
    }

    if (i_this->mDrawHorse) {
        if (can_hjump && saku_jump_check(i_this)) {
            i_this->field_0xb00 = i_this->mActionMode;
            i_this->mActionMode = ACTION_HJUMP;
            i_this->mMoveMode = 0;
        }
    } else {
        if (!daPy_py_c::checkMasterSwordEquip()) {
            i_this->mDamageInvulnerabilityTimer = 10;
        }

        if (i_this->field_0xc5a != 0) {
            sp38 = FALSE;
        }

        if (sp38) {
            int sp28 = fpcM_Search(shot_s_sub, i_this) != NULL;
            if (daPy_getPlayerActorClass()->checkHookshotShootReturnMode() && !daPy_getPlayerActorClass()->checkHookshotReturnMode()) {
                sp28 = 2;
            }

            cXyz* ironball_pos = player->getIronBallCenterPos();
            if (ironball_pos != NULL && (player->current.pos - *ironball_pos).abs() > 300.0f && (a_this->current.pos - *ironball_pos).abs() < 400.0f && !dComIfGp_checkPlayerStatus0(0, 0x400) && !daPy_getPlayerActorClass()->checkIronBallReturn() && !daPy_getPlayerActorClass()->checkIronBallGroundStop()) {
                sp28 = 2;
            }

            f32 pl_at_check_range = 0.0f;
            if (daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_LARGE_TURN_RIGHT || daPy_getPlayerActorClass()->getCutType() == daPy_py_c::CUT_TYPE_LARGE_TURN_LEFT) {
                pl_at_check_range = 150.0f;
            }

            int cut_type = cc_pl_cut_bit_get();
            if (cut_type == 0) {
                cut_type = daPy_getPlayerActorClass()->getCutType();
            }

            if (cut_type == 0x400 || (cut_type == 0x100 && player->speed.y > 5.0f)) {
                cut_type = 0;
            }

            if (!fopAcM_plAngleCheck(a_this, 0x4000)) {
                cut_type = 0;
            }

            if (!daPy_py_c::checkMasterSwordEquip()) {
                cut_type = 0;
            }

            if ((cut_type != 0 || sp28 != 0) && (sp28 != 0 || i_this->mPlayerDistXZ < pl_at_check_range + 380.0f)) {
                if (sp28 == 2 || (sp28 == 1 && cM_rndF(1.0f) < 0.5f) || cut_type == 0x100) {
                    if (sp28 == 2 && i_this->mPlayerDistXZ > 700.0f && cM_rndF(1.0f) < 0.5f) {
                        i_this->mActionMode = ACTION_JUMP;
                        i_this->mMoveMode = 0;
                    } else {
                        i_this->mActionMode = ACTION_SIDE;
                        i_this->mMoveMode = 0;
                        i_this->mDamageInvulnerabilityTimer = 15;
                    }
                } else {
                    i_this->mActionMode = ACTION_DEFENCE;
                    i_this->mMoveMode = 0;
                    i_this->mDamageInvulnerabilityTimer = 15;
                    i_this->field_0xc79 = 1;
                }
            }
        }
    }

    s16 sp14 = 0;
    s16 sp12 = 0;
    s16 sp10 = 0x1000;
    if (i_this->field_0xc7d != 0) {
        if (i_this->field_0xc7d == 1) {
            sp88 = player->eyePos - a_this->current.pos;
        } else {
            sp88 = i_this->field_0xc80 - a_this->current.pos;
        }

        if (i_this->mDrawHorse) {
            sp88.y += -450.0f;
        } else {
            sp88.y += -200.0f;
        }

        sp14 = cM_atan2s(sp88.x, sp88.z) - a_this->shape_angle.y;
        sp12 = a_this->shape_angle.x + cM_atan2s(sp88.y, JMAFastSqrt(sp88.x * sp88.x + sp88.z * sp88.z));
        if (sp14 > 11000) {
            sp14 = 11000;
        } else if (sp14 < -11000) {
            sp14 = -11000;
        }

        if (sp12 > 3000) {
            sp12 = 3000;
        } else if (sp12 < -3000) {
            sp12 = -3000;
        }
    }

    cLib_addCalcAngleS2(&i_this->mGndBodyRotX, sp14, 2, sp10);
    cLib_addCalcAngleS2(&i_this->mGndHeadRotZ, sp12, 2, sp10);

    if (sp34) {
        cLib_addCalcAngleS2(&a_this->shape_angle.y, a_this->current.angle.y, 2, 0x1000);
    }

    if (i_this->field_0xc70 != 0) {
        i_this->field_0xc70--;
        i_this->field_0xc6c = i_this->field_0xc70;
        if (i_this->field_0xc6c > 7.0f) {
            i_this->field_0xc6c = 0.0f;
        }
    } else {
        i_this->field_0xc6c = 0.0f;
        i_this->field_0xc70 = cM_rndF(200.0f) + 200.0f;
    }

    cLib_addCalcAngleS2(&i_this->mGndLegRotX, i_this->field_0x26c2, 2, 0x600);
    i_this->field_0x26c2 = 0;
}

static BOOL ball_bg_check(b_gnd_class* i_this, int i_no) {
    cXyz start;
    cXyz end;
    cXyz sp2C;
    dBgS_LinChk linChk;

    start = i_this->mLightBallPos[i_no];
    end = i_this->mLightBallPos[i_no] + ((i_this->mLightBallPos[i_no] - i_this->field_0x1e98[i_no]) * 2.0f);
    linChk.Set(&start, &end, i_this);

    if (dComIfG_Bgsp().LineCross(&linChk)) {
        return TRUE;
    }

    return FALSE;
}

static void himo_control1(b_gnd_class* i_this, cXyz* param_2, int param_3, s8 param_4) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz sp60;
    cXyz sp54;

    int i;
    s16 sp10;
    s16 spE;

    cXyz* var_r30 = i_this->field_0x1ff0[param_3].field_0x0;
    var_r30[0] = *param_2;

    cXyz sp48;
    cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
    cMtx_XrotM(*calc_mtx, a_this->shape_angle.x);

    sp60.x = 20.0f;
    if (param_3 == 0) {
        sp60.x *= -1.0f;
    }
    sp60.y = 0.0f;
    sp60.z = 0.0f;
    MtxPosition(&sp60, &sp48);

    cXyz sp3C;
    cXyz sp30;
    cXyz sp24;

    sp60.x = 0.0f;
    sp60.y = i_this->field_0x1fd0;
    sp60.z = sp60.y;
    MtxPosition(&sp60, &sp30);

    if (a_this->speedF > 20.0f) {
        sp60.x = i_this->field_0x1fd0 * 0.4f;
    } else {
        sp60.x = i_this->field_0x1fd0 * 0.1f;
    }

    if (param_3 == 0) {
        sp60.x *= -1.0f;
    }
    sp60.y = 0.0f;
    sp60.z = 0.0f;
    MtxPosition(&sp60, &sp24);

    sp60.x = 0.0f;
    sp60.y = 0.0f;
    if (param_4 != 0) {
        sp60.z = 15.0f;
    } else {
        sp60.z = 20.0f;
    }

    i = 1;
    cXyz* var_r30_2 = var_r30 + 1;
    for (; i < 16; i++, var_r30_2++) {
        sp3C.x = sp30.x * cM_ssin(i_this->field_0x1fd4 + i * 6000);  
        sp3C.y = sp30.y * cM_ssin(i_this->field_0x1fd4 + i * 6000);
        sp3C.z = sp30.z * cM_ssin(i_this->field_0x1fd4 + i * 6000);

        f32 var_f29 = 1.0f;
        f32 var_f31 = sp24.x + (sp3C.x + ((var_r30_2[0].x - var_r30_2[-1].x) + (sp48.x * var_f29)));
        f32 var_f28 = sp3C.y + ((var_r30_2[0].y - var_r30_2[-1].y) - 20.0f);
        f32 var_f30 = sp24.z + (sp3C.z + ((var_r30_2[0].z - var_r30_2[-1].z) + (sp48.z * var_f29)));

        spE = (s16)cM_atan2s(var_f31, var_f30);
        sp10 = -cM_atan2s(var_f28, JMAFastSqrt(var_f31 * var_f31 + var_f30 * var_f30));

        cMtx_YrotS(*calc_mtx, spE);
        cMtx_XrotM(*calc_mtx, sp10);
        MtxPosition(&sp60, &sp54);

        var_r30_2[0].x = var_r30_2[-1].x + sp54.x;
        var_r30_2[0].y = var_r30_2[-1].y + sp54.y;
        var_r30_2[0].z = var_r30_2[-1].z + sp54.z;
    }
}

static void himo_control2(b_gnd_class* i_this, cXyz* param_2, int param_3, s8 param_4) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    cXyz sp30;
    cXyz sp24;

    int i;
    s16 sp10;
    s16 spE;
    g_himo_s* sp1C = &i_this->field_0x1ff0[param_3];
    cXyz* temp_r31 = &sp1C->field_0x0[15];
    *temp_r31 = *param_2;

    sp30.x = 0.0f;
    sp30.y = 0.0f;

    if (param_4 != 0) {
        sp30.z = 15.0f;
    } else {
        sp30.z = 20.0f;
    }

    temp_r31--;

    for (i = 14; i >= 1; i--, temp_r31--) {
        f32 temp_f31 = temp_r31[0].x - temp_r31[1].x;
        f32 temp_f29 = temp_r31[0].y - temp_r31[1].y;
        f32 temp_f30 = temp_r31[0].z - temp_r31[1].z;
        spE = (s16)cM_atan2s(temp_f31, temp_f30);
        sp10 = -cM_atan2s(temp_f29, JMAFastSqrt((temp_f31 * temp_f31) + (temp_f30 * temp_f30)));

        cMtx_YrotS(*calc_mtx, spE);
        cMtx_XrotM(*calc_mtx, sp10);
        MtxPosition(&sp30, &sp24);

        temp_r31[0].x = temp_r31[1].x + sp24.x;
        temp_r31[0].y = temp_r31[1].y + sp24.y;
        temp_r31[0].z = temp_r31[1].z + sp24.z;
    }

    temp_r31 = sp1C->field_0x0;
    cXyz* var_r29 = i_this->mHorseReins[param_3].getPos(0);
    for (int i = 0; i < 16; i++, var_r29++, temp_r31++) {
        *var_r29 = *temp_r31;
    }

    var_r29 = i_this->field_0x21e8.getPos(0);
    var_r29[param_3] = i_this->field_0x1ff0[param_3].field_0x0[15];
}

static void cam_3d_morf(b_gnd_class* i_this, f32 i_scale) {
    cLib_addCalc2(&i_this->mDemoCamCenter.x, i_this->mDemoCamCenterTarget.x, i_scale, i_this->mDemoCamCenterSpd.x * i_this->field_0x2710.y);
    cLib_addCalc2(&i_this->mDemoCamCenter.y, i_this->mDemoCamCenterTarget.y, i_scale, i_this->mDemoCamCenterSpd.y * i_this->field_0x2710.y);
    cLib_addCalc2(&i_this->mDemoCamCenter.z, i_this->mDemoCamCenterTarget.z, i_scale, i_this->mDemoCamCenterSpd.z * i_this->field_0x2710.y);

    cLib_addCalc2(&i_this->mDemoCamEye.x, i_this->mDemoCamEyeTarget.x, i_scale, i_this->mDemoCamEyeSpd.x * i_this->field_0x2710.y);
    cLib_addCalc2(&i_this->mDemoCamEye.y, i_this->mDemoCamEyeTarget.y, i_scale, i_this->mDemoCamEyeSpd.y * i_this->field_0x2710.y);
    cLib_addCalc2(&i_this->mDemoCamEye.z, i_this->mDemoCamEyeTarget.z, i_scale, i_this->mDemoCamEyeSpd.z * i_this->field_0x2710.y);
}

static void cam_spd_set(b_gnd_class* i_this) {
    i_this->mDemoCamEyeSpd.x = fabsf(i_this->mDemoCamEyeTarget.x - i_this->mDemoCamEye.x);
    i_this->mDemoCamEyeSpd.y = fabsf(i_this->mDemoCamEyeTarget.y - i_this->mDemoCamEye.y);
    i_this->mDemoCamEyeSpd.z = fabsf(i_this->mDemoCamEyeTarget.z - i_this->mDemoCamEye.z);

    i_this->mDemoCamCenterSpd.x = fabsf(i_this->mDemoCamCenterTarget.x - i_this->mDemoCamCenter.x);
    i_this->mDemoCamCenterSpd.y = fabsf(i_this->mDemoCamCenterTarget.y - i_this->mDemoCamCenter.y);
    i_this->mDemoCamCenterSpd.z = fabsf(i_this->mDemoCamCenterTarget.z - i_this->mDemoCamCenter.z);

    i_this->field_0x2710.y = 0.0f;
}

static void demo_camera(b_gnd_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c *)dComIfGp_getPlayer(0);
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* camera0 = dComIfGp_getCamera(0);
    mant_class* mant_p = (mant_class*)fopAcM_SearchByID(i_this->mMantChildID);
    dScnKy_env_light_c* kankyo = dKy_getEnvlight();

    daHorse_c* horse;
    if (i_this->mDemoCamMode != 0) {
        // fakematch - dComIfGp_getHorseActor() isn't getting inlined for some reason
        horse = (daHorse_c*)g_dComIfG_gameInfo.play.getPlayerPtr(1);
    }

    cXyz sp114;
    cXyz sp108;
    cXyz spFC;
    cXyz spF0;

    s8 sp8 = false;
    switch (i_this->mDemoCamMode) {
    case 0:
        break;
    case 30:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xffff, 0);
            a_this->eventInfo.onCondition(2);
            return;
        }

        camera->mCamera.Stop();
        i_this->mDemoCamMode = 31;
        i_this->mDemoCamTimer = 0;
        i_this->field_0x2710.x = 55.0f;

        horse->changeOriginalDemo();
        player->changeOriginalDemo();
        camera->mCamera.SetTrimSize(3);

        sp108 = a_this->current.pos;
        sp108.x -= 5000.0f;

        horse->setHorsePosAndAngle(&sp108, 1);

        i_this->mDemoCamCenterTarget.set(0.0f, 0.0f, 0.0f);
        i_this->mDemoCamCenterSpd.set(0.0f, 0.0f, 0.0f);
        i_this->field_0xc44[9] = 0;
        Z2GetAudioMgr()->setDemoName("force_start");
        break;
    case 31:
    case 32:
    case 33:
        if (i_this->mDemoCamMode < 33) {
            if (i_this->field_0xc44[9] == 0) {
                i_this->field_0xc44[9] = (s16)(cM_rndF(10.0f) + 5.0f);
                i_this->mDemoCamCenterSpd.x = cM_rndFX(40.0f);
                i_this->mDemoCamCenterSpd.y = cM_rndFX(40.0f);
                i_this->mDemoCamCenterSpd.z = cM_rndFX(40.0f);
            }

            cLib_addCalc2(&i_this->mDemoCamCenterTarget.x, i_this->mDemoCamCenterSpd.x, 0.3f, 30.0f);
            cLib_addCalc2(&i_this->mDemoCamCenterTarget.y, i_this->mDemoCamCenterSpd.y, 0.3f, 30.0f);
            cLib_addCalc2(&i_this->mDemoCamCenterTarget.z, i_this->mDemoCamCenterSpd.z, 0.3f, 30.0f);

            i_this->mDemoCamCenter.x = a_this->current.pos.x + i_this->mDemoCamCenterTarget.x;
            i_this->mDemoCamCenter.y = a_this->current.pos.y + 300.0f + i_this->mDemoCamCenterTarget.y;
            i_this->mDemoCamCenter.z = a_this->current.pos.z + i_this->mDemoCamCenterTarget.z;
        } else {
            i_this->mDemoCamCenter = a_this->eyePos;
        }

        cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
        if (i_this->mDemoCamMode == 31) {
            sp114.x = 300.0f;
            sp114.y = 300.0f;
            sp114.z = -500.0f;
        } else if (i_this->mDemoCamMode == 32) {
            sp114.x = -300.0f;
            sp114.y = 300.0f;
            sp114.z = 400.0f;
        } else if (i_this->mDemoCamMode == 33) {
            sp114.x = i_this->field_0x2710.z;
            sp114.y = 300.0f;
            sp114.z = -1000.0f;
            cLib_addCalc2(&i_this->field_0x2710.z, 100.0f, 0.1f, 5.0f);
        }

        MtxPosition(&sp114, &i_this->mDemoCamEye);
        i_this->mDemoCamEye += a_this->current.pos;

        if (i_this->mDemoCamMode == 33) {
            if (i_this->mDemoCamTimer == 140) {
                sp108.set(0.0f, 1100.0f, -2000.0f);
                horse->setHorsePosAndAngle(&sp108, 0);
                horse->changeDemoMode(1, 0);
                player->changeDemoMode(93, 0, 0, 0);
            }

            if (i_this->mDemoCamTimer == 160) {
                i_this->mDemoCamMode = 35;
                i_this->mDemoCamTimer = 0;
                i_this->mMoveMode = 3;
                Z2GetAudioMgr()->bgmStreamPrepare(0x2000063);
                Z2GetAudioMgr()->bgmStreamPlay();
            }
        }
        break;
    case 34:
        if (i_this->mDemoCamTimer == 0) {
            i_this->field_0x2710.z = -400.0f;
            horse->changeDemoMode(8, 0);
        } else {
            if (i_this->mDemoCamTimer == 1) {
                i_this->mDemoCamCenter = player->current.pos;
                i_this->mDemoCamEye.set(200.0f, 1318.0f, -1355.0f);
            }

            cLib_addCalc2(&i_this->mDemoCamCenter.x, player->current.pos.x, 0.2f, 50.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.y, player->current.pos.y, 0.2f, 50.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.z, player->current.pos.z, 0.2f, 50.0f);
        }

        sp108.set(0.0f, 1100.0f, i_this->field_0x2710.z + -1500.0f);
        horse->setHorsePosAndAngle(&sp108, 0);
        cLib_addCalc0(&i_this->field_0x2710.z, 0.5f, 20.0f);

        if (i_this->mDemoCamTimer == 55) {
            i_this->mDemoCamMode = 33;
            i_this->mDemoCamTimer = 0;
            a_this->speedF = 50.0f;
            i_this->field_0x2710.z = -300.0f;
            a_this->current.pos.x = 0.0f;
            sp108.set(0.0f, 1100.0f, -4000.0f);
            horse->setHorsePosAndAngle(&sp108, 0);
        }
        break;
    case 35:
        i_this->mDemoCamCenter.set(-286.0f, 1364.0f, -1780.0f);
        i_this->mDemoCamEye.set(389.0f, 1326.0f, -2356.0f);

        if (i_this->mDemoCamTimer <= 1) {
            i_this->field_0x2710.x = 25.0f;
        } else {
            cLib_addCalc2(&i_this->field_0x2710.x, 20.0f, 0.03f, 0.1f);
        }

        if (i_this->mDemoCamTimer == 70) {
            i_this->mDrawHorse = FALSE;
            anm_init(i_this, B_GND_BCK_EGND_WAIT03, 0.0f, 2, 1.0f);
            a_this->current.angle.y = a_this->shape_angle.y = i_this->mPlayerAngleY;
            i_this->mHideSheath = FALSE;
            i_this->field_0x770 = 1;

            for (int i = 0; i < 2; i++) {
                static u16 e_name[] = {0x8B9B, 0x8B9C};
                dComIfGp_particle_set(e_name[i], &a_this->current.pos, &a_this->shape_angle, NULL);
            }
        }

        if (i_this->mDemoCamTimer == 80) {
            i_this->mDemoCamMode = 40;
            i_this->mDemoCamTimer = 0;
            i_this->mDemoCamCenter.set(79.0f, 1305.0f, -1029.0f);
            i_this->mDemoCamEye.set(86.0f, 1321.0f, -1277.0f);
            i_this->field_0x2710.x = 55.0f;
            sp108.set(0.0f, 1100.0f, -2000.0f);
            horse->setHorsePosAndAngle(&sp108, -0x2000);
        }
        break;
    case 40:
        if (i_this->mDemoCamTimer != 110)
            break;

        i_this->mDemoCamMode = 41;
        i_this->mDemoCamTimer = 0;
        anm_init(i_this, B_GND_BCK_EGND_STANDUP, 1.0f, 0, 1.0f);
        i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_STANDUP, -1);
        i_this->mGndEyeBtkNo = 1;
        horse->changeDemoMode(1, 0);
        player->changeDemoMode(1, 0, 0, 0);
    case 41:
        cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
        sp114.x = -50.0f;
        sp114.y = 200.0f;
        sp114.z = 300.0f;
        MtxPosition(&sp114, &sp108);
        i_this->mDemoCamEye.x = sp108.x + a_this->current.pos.x;
        i_this->mDemoCamEye.z = sp108.z + a_this->current.pos.z;
        spFC.y = a_this->current.pos.y + ((a_this->eyePos.y - a_this->current.pos.y) * 0.55f);

        i_this->mDemoCamCenter.x = a_this->current.pos.x;
        i_this->mDemoCamCenter.z = a_this->current.pos.z;
        spF0.y = a_this->eyePos.y - 50.0f;

        if (i_this->mDemoCamTimer == 0) {
            i_this->mDemoCamCenter.y = spF0.y;
            i_this->mDemoCamEye.y = spFC.y;
        } else {
            cLib_addCalc2(&i_this->mDemoCamCenter.y, spF0.y, 0.1f, 10.0f);
            cLib_addCalc2(&i_this->mDemoCamEye.y, spFC.y, 0.1f, 10.0f);
        }

        if (i_this->mAnmID == B_GND_BCK_EGND_STANDUP && i_this->mpModelMorf->isStop()) {
            anm_init(i_this, B_GND_BCK_EGND_LAUGH, 0.0f, 2, 1.0f);
            i_this->mGndEyeBtkNo = 2;
        }

        if (i_this->mDemoCamTimer != 250) {
            break;
        }
        
        i_this->mDemoCamMode = 42;
        i_this->mDemoCamTimer = 0;
        anm_init(i_this, B_GND_BCK_EGND_LOOK, 0.0f, 0, 1.0f);
        i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_LOOK, -1);
        i_this->mGndEyeBtkNo = 3;
        player->onForceHorseGetOff();
    case 42:
        cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
        sp114.x = 80.0f;
        sp114.y = 280.0f;
        sp114.z = 40.0f;
        MtxPosition(&sp114, &i_this->mDemoCamEye);

        i_this->mDemoCamEye += a_this->current.pos;
        i_this->mDemoCamCenter.x = a_this->current.pos.x;
        i_this->mDemoCamCenter.z = a_this->current.pos.z - 20.0f;
        i_this->mDemoCamCenter.y = a_this->eyePos.y + 10.0f;

        if (i_this->mDemoCamTimer < 70) {
            i_this->field_0xc70 = 8;
        }

        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, B_GND_BCK_EGND_SAYAOPEN, 0.0f, 0, 1.0f);
            i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_SAYAOPEN, -1);
            i_this->mGndEyeBtkNo = 4;
            i_this->mDemoCamMode = 43;
            i_this->mDemoCamTimer = 0;
            sp108.set(0.0f, 1100.0f, -1000.0f);
            player->setPlayerPosAndAngle(&sp108, 1, 0);

            mant_p->field_0x3969 = 1;
        } else {
            break;
        }
    case 43:
        if (i_this->mpModelMorf->checkFrame(212.0f)) {
            i_this->mGndSound.startCreatureSound(Z2SE_EN_GND_SAYAOPEN, 0, -1);
        }
        
        if (i_this->mDemoCamTimer >= 10) {
            if (i_this->mDemoCamTimer == 10) {
                i_this->mMsgFlow.init(a_this, 3018, 0, NULL);
            }
            i_this->mMsgFlow.doFlow(a_this, NULL, 0);  
        }

        if (i_this->mpModelMorf->getFrame() > 340.0f) {
            cLib_addCalc2(&a_this->speedF, 4.3f, 1.0f, 0.7f);
            mant_p->field_0x3969 = 3;
        } else {
            cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
            sp114.x = 20.0f;
            sp114.y = 180.0f;
            sp114.z = 260.0f;
            MtxPosition(&sp114, &i_this->mDemoCamEye);

            i_this->mDemoCamEye += a_this->current.pos;
            i_this->mDemoCamCenter.x = a_this->current.pos.x;
            i_this->mDemoCamCenter.z = a_this->current.pos.z - 20.0f;
            i_this->mDemoCamCenter.y = a_this->eyePos.y - 50.0f;
        }

        if (i_this->mDemoCamTimer == 211) {
            i_this->field_0x770 = 0;
            mant_p->field_0x395c = 0.9f;
        }

        if (i_this->mDemoCamTimer == 215) {
            i_this->field_0x2739 = 1;
            i_this->mKankyoBlend = 0.0f;
            kankyo->mThunderEff.field_0x2 = 1;
        }

        if (i_this->mDemoCamTimer == 278) {
            i_this->field_0x772 = 1;
        }

        if (i_this->mDemoCamTimer == 390) {
            anm_init(i_this, B_GND_BCK_EGND_WALK, 0.0f, 2, 1.0f);
            i_this->mpModelMorf->setFrame(20.0f);
            i_this->mDemoCamMode = 44;
            i_this->mDemoCamTimer = 0;
            i_this->mGndEyeBtkNo = 0;
            i_this->mDemoCamCenter.set(792.0f, 1595.0f, 1160.0f);
            i_this->mDemoCamEye.set(938.0f, 1675.0f, 1337.0f);
            sp108.set(0.0f, 1100.0f, -500.0f);
            player->setPlayerPosAndAngle(&sp108, 1, 0);
        }
        break;
    case 44:
        i_this->mMsgFlow.doFlow(a_this, NULL, 0);

        if (i_this->mDemoCamTimer == 125) {
            kankyo->mThunderEff.field_0x2 = 1;
        }

        if (i_this->mDemoCamTimer >= 100) {
            if (i_this->mDemoCamTimer == 100) {
                anm_init(i_this, B_GND_BCK_EGND_WAIT01, 12.0f, 2, 1.0f);
            }

            if (i_this->mDemoCamTimer == 135) {
                i_this->mDemoCamMode = 45;
                i_this->mDemoCamTimer = 0;
                sp108.set(0.0f, 1100.0f, 0.0f);
                fopAcM_create(PROC_OBJ_GB, 0xF0069600, &sp108, fopAcM_GetRoomNo(a_this), 0, 0, -1);
            }

            cLib_addCalc0(&a_this->speedF, 1.0f, 0.7f);
            break;
        }
        
        cLib_addCalc2(&a_this->speedF, 4.3f, 1.0f, 0.7f);
        break;
    case 45:
        if (i_this->mDemoCamTimer == 1) {
            horse->changeDemoMode(9, 0);
            i_this->mDemoCamCenter.set(-81.0f, 1391.0f, -2144.0f);
            i_this->mDemoCamEye.set(37.0f, 1366.0f, -1865.0f);
            i_this->mDemoCamCenterTarget.set(76.0f, 1352.0f, -1866.0f);
            i_this->mDemoCamEyeTarget.set(129.0f, 1358.0f, -2165.0f);
            cam_spd_set(i_this);
        }

        if (i_this->mDemoCamTimer == 15) {
            kankyo->mThunderEff.field_0x2 = 1;
        }

        if (i_this->mDemoCamTimer >= 25) {
            cam_3d_morf(i_this, 0.2f);
            cLib_addCalc2(&i_this->field_0x2710.y, 0.07f, 1.0f, 0.002f);
        }

        if (i_this->mDemoCamTimer == 120) {
            i_this->mDemoCamMode = 46;
            i_this->mDemoCamTimer = 200;
            i_this->mDemoCamCenter.set(101.0f, 1206.0f, -582.0f);
            i_this->mDemoCamEye.set(154.0f, 1199.0f, -819.0f);
        }
        break;
    case 46:
        cLib_addCalc2(&i_this->field_0x2710.x, 30.0f, 0.1f, 0.2f);

        if (i_this->mDemoCamTimer >= 305) {
            mant_p->field_0x3969 = 2;
        }

        if (i_this->mDemoCamTimer == 255) {
            daPy_getPlayerActorClass()->changeDemoMode(23, 1, 2, 0);
        }

        if (i_this->mDemoCamTimer == 290) {
            anm_init(i_this, B_GND_BCK_EGND_KAMAE, 3.0f, 0, 1.0f);
            i_this->mGndEyeBtkNo = 5;
            Z2GetAudioMgr()->bgmStart(Z2BGM_VS_GANON_04, 0, 0);
            kankyo->wether = 3;
        }

        if (i_this->mDemoCamTimer == 320) {
            fopMsgM_messageSetDemo(1159);  // Dark Lord GANONDORF
        }

        if (i_this->mAnmID == B_GND_BCK_EGND_KAMAE && i_this->mpModelMorf->isStop()) {
            anm_init(i_this, B_GND_BCK_EGND_WAIT02, 3.0f, 2, 1.0f);
            kankyo->mThunderEff.field_0x2 = 1;
        }

        if (i_this->mDemoCamTimer == 410) {
            i_this->mActionMode = ACTION_WAIT;
            i_this->mMoveMode = 2;
            i_this->field_0xc44[0] = cM_rndF(50.0f) + 30.0f;
            i_this->field_0xc44[8] = 100;

            sp8 = true;
            dComIfGs_onOneZoneSwitch(15, -1);
            dComIfGs_onSaveDunSwitch(1);
            Z2GetAudioMgr()->setDemoName("force_end");
            kankyo->wether = 1;
        }
        break;
    case 50:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xFFEF, 0);
            a_this->eventInfo.onCondition(2);
            return;
        }

        camera->mCamera.Stop();
        i_this->mDemoCamMode = 51;
        i_this->mDemoCamTimer = 0;
        i_this->field_0x2710.x = 55.0f;
        i_this->field_0x2720 = player->current.pos;
        i_this->field_0x272c = player->shape_angle.y;
        i_this->mTubazeriPushAmount = 0.0f;
        i_this->mTubazeriPushValue = 0.0f;

        daPy_getPlayerActorClass()->changeOriginalDemo();
        daPy_getPlayerActorClass()->changeDemoMode(89, 0, 0, 0);
        Z2GetAudioMgr()->changeBgmStatus(1);
    case 51:
        mant_p->field_0x3966 = 1;
        i_this->field_0xc70 = 20;

        if (i_this->mDemoCamTimer >= 5) {
            dComIfGp_getVibration().StartQuake(VIBMODE_Q_POWER2, 1, cXyz(0.0f, 1.0f, 0.0f));
        }

        dComIfGp_setDoStatusForce(100, 4);
        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        sp114.x = 0.0f;
        sp114.y = 0.0f;
        sp114.z = i_this->mTubazeriPushAmount * 1.3f;
        MtxPosition(&sp114, &sp108);
        sp108 += i_this->field_0x2720;
        sp108.y = player->current.pos.y;
        player->setPlayerPosAndAngle(&sp108, i_this->field_0x272c, 0);

        sp114.x = 0.0f;
        sp114.y = 0.0f;
        sp114.z = 135.0f;
        MtxPosition(&sp114, &sp108);
        a_this->current.pos.x = sp108.x + player->field_0x5f8.x;
        a_this->current.pos.z = sp108.z + player->field_0x5f8.z;
        a_this->current.angle.y = player->shape_angle.y + 0x8000;

        spF0 = a_this->current.pos + ((*player->getViewerCurrentPosP() - a_this->current.pos) * 0.5f);
        spF0.y += 150.0f;

        sp114.x = 200.0f;
        sp114.y = 0.0f;
        sp114.z = 0.0f;
        MtxPosition(&sp114, &spFC);
        spFC += spF0;

        if (i_this->mDemoCamTimer == 0) {
            i_this->mDemoCamCenter = spF0;
            i_this->mDemoCamEye = spFC;
            
            cXyz spE4(2.0f, 2.0f, 2.0f);
            dComIfGp_setHitMark(2, a_this, &spF0, NULL, &spE4, 0);
            i_this->mGndSound.startCollisionSE(Z2SE_HIT_SWORD, 40);
        } else {
            MTXCopy(i_this->mpModelMorf->getModel()->getAnmMtx(33), *calc_mtx);
            sp114.set(10.0f, 20.0f, 0.0f);
            MtxPosition(&sp114, &sp108);

            for (int i = 0; i < 2; i++) {
                static u16 e_name[] = {0x8B98, 0x8B99};
                i_this->field_0x2680[i] = dComIfGp_particle_set(i_this->field_0x2680[i], e_name[i], &sp108, NULL, NULL);
            }

            i_this->mGndSound.startCreatureExtraSoundLevel(Z2SE_EN_GND_TUBAZERI_EFF, 0 , -1);

            if (i_this->field_0xc44[3] == 0) {
                i_this->field_0xc44[3] = (s16)(cM_rndF(10.0f) + 3.0f);
                i_this->mDemoCamCenterTarget.x = cM_rndFX(20.0f);
                i_this->mDemoCamCenterTarget.y = cM_rndFX(20.0f);
                i_this->mDemoCamCenterTarget.z = cM_rndFX(20.0f);
            }

            cLib_addCalc2(&i_this->mDemoCamCenter.x, spF0.x + i_this->mDemoCamCenterTarget.x, 0.1f, 5.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.y, spF0.y + i_this->mDemoCamCenterTarget.y, 0.1f, 5.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.z, spF0.z + i_this->mDemoCamCenterTarget.z, 0.1f, 5.0f);
        }

        if ((int)mDoCPd_c::getTrigA(PAD_1) != 0) {
            i_this->mTubazeriPushValue += 0.1f;
            if (i_this->mTubazeriPushValue > 1.0f) {
                i_this->mTubazeriPushValue = 1.0f;
            }
        } else {
            i_this->mTubazeriPushValue -= 0.02f;
            if (i_this->mTubazeriPushValue < -1.0f) {
                i_this->mTubazeriPushValue = -1.0f;
            }
        }

        cLib_addCalc2(&i_this->mTubazeriPushAmount, i_this->mTubazeriPushValue * 50.0f, 0.1f, 2.0f);
        if (i_this->mTubazeriPushAmount <= -49.9f) {
            i_this->mDemoCamMode = 52;
            i_this->mDemoCamTimer = 0;

            anm_init(i_this, B_GND_BCK_EGND_TUBAZERI_WIN, 3.0f, 0, 1.0f);
            daPy_getPlayerActorClass()->changeDemoMode(89, 4, 0, 0);
            dComIfGp_getVibration().StopQuake(31);
            Z2GetAudioMgr()->changeBgmStatus(2);
        } else if (i_this->mTubazeriPushAmount >= 48.0f) {
            i_this->mDemoCamMode = 55;
            i_this->mDemoCamTimer = 0;

            anm_init(i_this, B_GND_BCK_EGND_TUBAZERI_LOSE, 3.0f, 0, 1.0f);
            daPy_getPlayerActorClass()->changeDemoMode(89, 3, 0, 0);
            dComIfGp_getVibration().StopQuake(31);
            Z2GetAudioMgr()->changeBgmStatus(3);
        } else if (i_this->mTubazeriPushValue < -0.4f) {
            i_this->mGndSound.startCreatureVoiceLevel(Z2SE_EN_GND_V_TUBAZERI_B, -1);
            if (i_this->mAnmID != B_GND_BCK_EGND_TUBAZERI_B && i_this->field_0xc44[2] == 0) {
                anm_init(i_this, B_GND_BCK_EGND_TUBAZERI_B, 3.0f, 2, 1.0f);
                daPy_getPlayerActorClass()->changeDemoMode(89, 1, 0, 0);
                i_this->field_0xc44[2] = 30;
                Z2GetAudioMgr()->changeBgmStatus(4);
            }
        } else if (i_this->mTubazeriPushValue > 0.4f) {
            i_this->mGndSound.startCreatureVoiceLevel(Z2SE_EN_GND_V_TUBAZERI_C, -1);
            if (i_this->mAnmID != B_GND_BCK_EGND_TUBAZERI_C && i_this->field_0xc44[2] == 0) {
                anm_init(i_this, B_GND_BCK_EGND_TUBAZERI_C, 3.0f, 2, 1.0f);
                daPy_getPlayerActorClass()->changeDemoMode(89, 2, 0, 0);
                i_this->field_0xc44[2] = 30;
                Z2GetAudioMgr()->changeBgmStatus(5);
            }
        } else {
            i_this->mGndSound.startCreatureVoiceLevel(Z2SE_EN_GND_V_TUBAZERI_A, -1);
            if (i_this->mAnmID != B_GND_BCK_EGND_TUBAZERI_A && i_this->field_0xc44[2] == 0) {
                anm_init(i_this, B_GND_BCK_EGND_TUBAZERI_A, 3.0f, 2, 1.0f);
                daPy_getPlayerActorClass()->changeDemoMode(89, 0, 0, 0);
                i_this->field_0xc44[2] = 30;
                Z2GetAudioMgr()->changeBgmStatus(6);
            }
        }
        break;
    case 52:
        i_this->field_0xc70 = 20;
        if (i_this->mpModelMorf->isStop()) {
            i_this->mActionMode = ACTION_ATTACK;
            i_this->mMoveMode = 2;
            i_this->mDemoCamMode = 53;
            i_this->mDemoCamTimer = 0;
        }

        if (i_this->mDemoCamTimer == 20) {
            cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
            sp114.x = -200.0f;
            sp114.y = 50.0f;
            sp114.z = 500.0f;
            MtxPosition(&sp114, &i_this->mDemoCamEye);
            i_this->mDemoCamEye += a_this->current.pos;
            i_this->mDemoCamCenter = player->current.pos;
            i_this->mDemoCamCenter.y += 80.0f;
            mant_p->field_0x395c = 1.2f;
        }
        break;
    case 53:
        i_this->field_0xc70 = 20;
        if (i_this->mDemoCamTimer >= 20) {
            spF0 = player->current.pos;
            spF0.y += 80.0f;
            cLib_addCalc2(&i_this->mDemoCamCenter.x, spF0.x, 0.15f, 200.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.y, spF0.y, 0.15f, 200.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.z, spF0.z, 0.15f, 200.0f);
        }

        if (i_this->mDemoCamTimer == 18) {
            daPy_getPlayerActorClass()->setThrowDamage(a_this->shape_angle.y, 50.0f, 30.0f, 8, 1, 0);
            daPy_getPlayerActorClass()->changeDemoMode(1, 0, 0, 0);

            cXyz spD8(player->current.pos);
            spD8.y += 150.0f;
            dComIfGp_setHitMark(3, NULL, &spD8, &player->shape_angle, NULL, 0);
        }

        if (i_this->mDemoCamTimer == 92) {
            sp8 = TRUE;
        }
        break;
    case 55:
        i_this->field_0xc70 = 20;
        if (i_this->mDemoCamTimer == 20) { 
            cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
            sp114.x = 200.0f;
            sp114.y = 50.0f;
            sp114.z = 500.0f;
            MtxPosition(&sp114, &i_this->mDemoCamEye);
            i_this->mDemoCamEye += a_this->current.pos;
            i_this->mDemoCamCenter = player->current.pos;
            i_this->mDemoCamCenter.y += 80.0f;
            mant_p->field_0x395c = 1.2f;
        }

        if (i_this->mDemoCamTimer >= 43) {
            sp8 = true;
            i_this->mActionMode = ACTION_JUMP;
            i_this->mMoveMode = 5;
            i_this->field_0xc44[0] = 50;
            i_this->field_0xc7c = 1;
        }
        break;
    case 60:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xffff, 0);
            a_this->eventInfo.onCondition(2);
            return;
        }
        
        camera->mCamera.Stop();
        i_this->mDemoCamMode = 61;
        i_this->mDemoCamTimer = 0;
        i_this->field_0x2710.x = 55.0f;

        daPy_getPlayerActorClass()->changeOriginalDemo();
        daPy_getPlayerActorClass()->changeDemoMode(90, 0, 0, 0);
        sp108.set(0.0f, 1100.0f, 0.0f);
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&sp108, -0x2000, 0);

        a_this->current.pos.set(0.0f, 1100.0f, 0.0f);
        a_this->old.pos = a_this->current.pos;
        a_this->shape_angle.y = -0x2000;
        a_this->current.angle.y = -0x2000;

        cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
        sp114.x = -300.0f;
        sp114.y = 150.0f;
        sp114.z = 200.0f;
        MtxPosition(&sp114, &i_this->mDemoCamEye);
        i_this->mDemoCamEye += a_this->current.pos;
        i_this->mDemoCamCenter = a_this->current.pos;
    case 61:
        i_this->field_0xc70 = 20;
        if (i_this->mDemoCamTimer >= 1) {
            cLib_addCalc2(&i_this->mDemoCamCenter.x, player->eyePos.x, 0.3f, 50.0f);
            spF0.y = player->eyePos.y;
            if (spF0.y > 1400.0f) {
                spF0.y = 1400.0f;
            }

            cLib_addCalc2(&i_this->mDemoCamCenter.y, spF0.y, 0.3f, 50.0f);
            cLib_addCalc2(&i_this->mDemoCamCenter.z, player->eyePos.z, 0.3f, 50.0f);
        }

        if (i_this->mDemoCamTimer == 31) {
            i_this->mDemoCamMode = 62;
            i_this->mDemoCamTimer = 0;
            anm_init(i_this, B_GND_BCK_EGND_TODOME_02, 0.0f, 0, 1.0f);
            dComIfGp_getVibration().StartShock(VIBMODE_S_POWER7, 1, cXyz(0.0f, 1.0f, 0.0f));
            fpcM_Search(s_objgbdel_sub, i_this);
            i_this->field_0x2739 = 2;
            Z2GetAudioMgr()->seMoveVolumeAll(0.0f, 0);
            i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_END, -1);
        }
        break;
    case 62:
        i_this->field_0xc70 = 20;
        i_this->field_0x2710.x = 25.0f;

        cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
        sp114.set(55.7f, 1146.0f, -151.0f);
        MtxPosition(&sp114, &i_this->mDemoCamCenter);
        sp114.set(-131.0f, 1146.0f, -156.0f);
        MtxPosition(&sp114, &i_this->mDemoCamEye);

        if (i_this->mpModelMorf->isStop()) {
            anm_init(i_this, B_GND_BCK_EGND_KURUSIMI, 10.0f, 2, 1.0f);
        }

        if (i_this->mDemoCamTimer == 80) {
            i_this->mDemoCamMode = 63;
            i_this->mDemoCamTimer = 0;
            Z2GetAudioMgr()->seMoveVolumeAll(1.0f, 37);
            i_this->field_0x2739 = 3;
        }

        sp108.set(0.0f, 1100.0f, -2000.0f);
        horse->setHorsePosAndAngle(&sp108, -0x2000);
        break;
    case 63:
        cMtx_YrotS(*calc_mtx, i_this->shape_angle.y);
        sp114.set(90.0f, 1250.0f, -57.0f);
        MtxPosition(&sp114, &i_this->mDemoCamCenter);
        sp114.set(-80.0f, 1188.0f, -106.0f);
        MtxPosition(&sp114, &i_this->mDemoCamEye);

        if (i_this->mDemoCamTimer == 80) {
            i_this->mDemoCamMode = 64;
            i_this->mDemoCamTimer = 0;
            i_this->mDrawZelda = FALSE;
            i_this->field_0x2739 = 4;
        }
        break;
    case 64:
        i_this->field_0xc70 = 20;
        i_this->mDrawZelda = TRUE;

        mDoMtx_stack_c::transS(0.0f, 1430.0f, -2000.0f);
        mDoMtx_stack_c::YrotM(-0x2000);
        i_this->mpZeldaModel->getModel()->setBaseTRMtx(mDoMtx_stack_c::get());

        i_this->mpZeldaBtk->play();
        i_this->mpZeldaBtp->play();
        i_this->mpZeldaModel->play(NULL, 0, 0);
        i_this->mpZeldaModel->modelCalc();

        i_this->mDemoCamCenter.set(25.0f, 1603.0f, -2033.0f);
        i_this->mDemoCamEye.set(-70.0f, 1564.0f, -1884.0f);

        if (i_this->mDemoCamTimer == 88) {
            i_this->mDemoCamMode = 65;
            i_this->mDemoCamTimer = 0;
            i_this->field_0x271c = 3000;
            i_this->field_0x2710.z = 1.0f;
            i_this->field_0x2710.y = 0.0f;
            i_this->field_0x2739 = 5;
        }
        break;
    case 65:
        i_this->field_0xc70 = 20;
        i_this->mDemoCamCenter = a_this->eyePos;
        i_this->mDemoCamCenter.z += -5.0f;

        cMtx_YrotS(*calc_mtx, a_this->shape_angle.y + i_this->field_0x271c);
        sp114.x = 0.0f;
        sp114.y = i_this->field_0x2710.z * 150.0f;
        sp114.z = i_this->field_0x2710.z * 50.0f;
        MtxPosition(&sp114, &i_this->mDemoCamEye);
        i_this->mDemoCamEye += i_this->mDemoCamCenter;

        if (i_this->mDemoCamTimer > 80) {
            cLib_addCalc2(&i_this->field_0x2710.y, 1.0f, 1.0f, 0.05f);
            i_this->field_0x271c += (int)(i_this->field_0x2710.y * 200.0f);
            cLib_addCalc2(&i_this->field_0x2710.z, 500.0f, 1.0f, i_this->field_0x2710.y * 0.5f);

            if (i_this->mDemoCamTimer == 150) {
                daPy_getPlayerActorClass()->changeDemoMode(16, 0, 0, 0);
            }

            if (i_this->mDemoCamTimer == 230) {
                mDoGph_gInf_c::fadeOut(0.01f, g_blackColor);
            }

            if (i_this->mDemoCamTimer == 330) {
                dStage_changeScene(0, 0.0f, 0, fopAcM_GetRoomNo(a_this), 0, -1);
            }
        }
        break;
    case 90:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xffff, 0);
            a_this->eventInfo.onCondition(2);
            return;
        }

        camera->mCamera.Stop();
        i_this->mDemoCamMode = 91;
        i_this->mDemoCamTimer = 0;
        i_this->field_0x2710.x = 55.0f;

        daPy_getPlayerActorClass()->changeOriginalDemo();
        horse->changeOriginalDemo();

        i_this->mDemoCamCenter = player->current.pos;
        i_this->mDemoCamEye = player->current.pos;
        i_this->mDemoCamEye.x += 100.0f;
        break;
    case 91:
        sp108.set(0.0f, 1100.0f, -2000.0f);
        horse->setHorsePosAndAngle(&sp108, -0x2000);

        if (i_this->mDemoCamTimer == 2) {
            player->onForceHorseGetOff();
        }

        if (i_this->mDemoCamTimer == 5) {
            i_this->mDemoCamMode = 92;
            i_this->mDemoCamTimer = 0;
        }
        break;
    case 92:
        if (i_this->mDemoCamTimer == 1) {
            sp108.set(0.0f, 1100.0f, 0.0f);
            fopAcM_create(PROC_OBJ_GB, 0xF0069600, &sp108, fopAcM_GetRoomNo(a_this), NULL, NULL, -1);
        }

        if (i_this->mDemoCamTimer == 5) {
            sp108.set(600.0f, 1100.0f, 0.0f);
            player->setPlayerPosAndAngle(&sp108, -0x4802, 0);
            a_this->current.pos.set(-600.0f, 1100.0f, 0.0f);
            a_this->old.pos = a_this->current.pos;
            a_this->current.angle.y = 0x37FE;
            a_this->shape_angle.y = 0x37FE;
            sp8 = TRUE;
        }
        break;
    case 95:
        if (!a_this->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(a_this, 2, 0xffff, 0);
            a_this->eventInfo.onCondition(2);
            return;
        }

        i_this->mDemoCamMode = 96;
        i_this->mDemoCamTimer = 0;

        daPy_getPlayerActorClass()->changeOriginalDemo();
        horse->changeOriginalDemo();

        sp108.set(0.0f, 1100.0f, -2000.0f);
        horse->setHorsePosAndAngle(&sp108, -0x2000);

        sp108.set(600.0f, 1100.0f, 0.0f);
        player->setPlayerPosAndAngle(&sp108, -0x4802, 0);
        a_this->current.pos.set(-600.0f, 1100.0f, 0.0f);

        a_this->old.pos = a_this->current.pos;
        a_this->current.angle.y = 0x37FE;
        a_this->shape_angle.y = 0x37FE;

        sp108.set(0.0f, 1100.0f, 0.0f);
        fopAcM_create(PROC_OBJ_GB, 0xF0069600, &sp108, fopAcM_GetRoomNo(a_this), NULL, NULL, -1);
        break;
    case 96:
        i_this->mDemoCamTimer++;
        if (i_this->mDemoCamTimer == 2) {
            dComIfGp_event_reset();
            sp8 = TRUE;
            break;
        }
        return;
    }

    if (sp8) {
        if (sp8 == 1) {
            cMtx_YrotS(*calc_mtx, player->shape_angle.y);
            sp114.x = 0.0f;
            sp114.y = 100.0f;
            sp114.z = -250.0f;
            MtxPosition(&sp114, &i_this->mDemoCamEye);
            i_this->mDemoCamEye += player->current.pos;
            i_this->mDemoCamCenter = player->current.pos;
            i_this->mDemoCamCenter.y += 120.0f;
        }

        camera->mCamera.Reset(i_this->mDemoCamCenter, i_this->mDemoCamEye);
        camera->mCamera.Start();
        camera->mCamera.SetTrimSize(0);
        dComIfGp_event_reset();
        daPy_getPlayerActorClass()->cancelOriginalDemo();
        i_this->mDemoCamMode = 0;
    }
    
    if (i_this->mDemoCamMode > 0) {
        cXyz center;
        cXyz eye;
        center = i_this->mDemoCamCenter;
        eye = i_this->mDemoCamEye;
        camera->mCamera.Set(center, eye, i_this->field_0x2710.x, 0);

        i_this->mDemoCamTimer++;
        if (i_this->mDemoCamTimer > 10000) {
            i_this->mDemoCamTimer = 10000;
        }
    }
}

static void anm_se_set(b_gnd_class* i_this) {
    if (i_this->mAnmID == B_GND_BCK_EGND_RSPUR && i_this->mpModelMorf->checkFrame(15.0f) ||
        i_this->mAnmID == B_GND_BCK_EGND_RRETURN && i_this->mpModelMorf->checkFrame(86.0f)) {
        i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_SPUR, -1);
    } else if (i_this->mAnmID == B_GND_BCK_EGND_RBRAKE && i_this->mpModelMorf->checkFrame(1.0f)) {
        i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_BRAKE, -1);
    } else if (i_this->mAnmID == B_GND_BCK_EGND_RATTACK01_A && i_this->mpModelMorf->checkFrame(1.0f)) {
        i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_ATTACK01A, -1);
    } else if ((i_this->mAnmID == B_GND_BCK_EGND_RATTACK01_B || i_this->mAnmID == B_GND_BCK_EGND_RATTACK01_C) && i_this->mpModelMorf->checkFrame(1.0f)) {
        i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_ATTACK01B, -1);
    } else if (i_this->mAnmID == B_GND_BCK_EGND_RATTACK02_A && i_this->mpModelMorf->checkFrame(21.0f)) {
        i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_ATTACK02A, -1);
    } else if (i_this->mAnmID == B_GND_BCK_EGND_RJUMP_START && i_this->mpModelMorf->checkFrame(1.0f)) {
        i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_JUMP_START, -1);
    } else if ((i_this->mAnmID == B_GND_BCK_EGND_RDAMEGE02L || i_this->mAnmID == B_GND_BCK_EGND_RDAMEGE02R) && i_this->mpModelMorf->checkFrame(1.0f)) {
        i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_DAMAGE02, -1);
    } else if (i_this->mAnmID == B_GND_BCK_EGND_RRETURN && i_this->mpModelMorf->checkFrame(1.0f)) {
        i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_RETURN, -1);
    }
    
    if (i_this->mAnmID == B_GND_BCK_EGND_WALK && (i_this->mpModelMorf->checkFrame(1.0f) || i_this->mpModelMorf->checkFrame(26.0f))) {
        i_this->mGndSound.startCreatureSound(Z2SE_EN_GND_FOOTNOTE, 0, -1);
    } else if (i_this->mAnmID == B_GND_BCK_EGND_CHOUHATU && i_this->mpModelMorf->checkFrame(25.0f)) {
        i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_CHOUHATU, -1);
    } else if (i_this->mAnmID == B_GND_BCK_EGND_ATTACKA) {
        if (i_this->mpModelMorf->checkFrame(15.0f)) {
            i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_ATTACKA, -1);
        } else if (i_this->mpModelMorf->checkFrame(17.0f)) {
            i_this->mGndSound.startCreatureExtraSound(Z2SE_EN_GND_ATTACKA, 0, -1);
        } else if (i_this->mpModelMorf->checkFrame(21.0f)) {
            i_this->mGndSound.startCreatureSound(Z2SE_EN_GND_MANTEAU, 0, -1);
        }
    } else if (i_this->mAnmID == B_GND_BCK_EGND_ATTACKB) {
        if (i_this->mpModelMorf->checkFrame(14.0f)) {
            i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_ATTACKB, -1);
        } else if (i_this->mpModelMorf->checkFrame(16.5f)) {
            i_this->mGndSound.startCreatureExtraSound(Z2SE_EN_GND_ATTACKB, 0, -1);
        } else if (i_this->mpModelMorf->checkFrame(21.0f)) {
            i_this->mGndSound.startCreatureSound(Z2SE_EN_GND_MANTEAU, 0, -1);
        }
    } else if (i_this->mAnmID == B_GND_BCK_EGND_ATTACKC_A) {
        if (i_this->mpModelMorf->checkFrame(1.0f)) {
            i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_ATTACKC_A, -1);
            i_this->mGndSound.startCreatureExtraSound(Z2SE_EN_GND_ATTACKC_A, 0, -1);
        }
    } else if (i_this->mAnmID == B_GND_BCK_EGND_ATTACKC_B && (i_this->mpModelMorf->checkFrame(2.0f) || i_this->mpModelMorf->checkFrame(9.5f))) {
            i_this->mGndSound.startCreatureSound(Z2SE_EN_GND_FOOTNOTE, 0, -1);
    } else if (i_this->mAnmID == B_GND_BCK_EGND_ATTACKC_C) {
        if (i_this->mpModelMorf->checkFrame(1.0f) || i_this->mpModelMorf->checkFrame(5.0f) ||
            i_this->mpModelMorf->checkFrame(7.0f)) {
            i_this->mGndSound.startCreatureSound(Z2SE_EN_GND_FOOTNOTE, 0, -1);
        } else if (i_this->mpModelMorf->checkFrame(13.0f)) {
            i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_ATTACKC_C, -1);
        } else if (i_this->mpModelMorf->checkFrame(19.0f)) {
            i_this->mGndSound.startCreatureExtraSound(Z2SE_EN_GND_ATTACKC_C, 0, -1);
        } else if (i_this->mpModelMorf->checkFrame(21.0f)) {
            i_this->mGndSound.startCreatureSound(Z2SE_EN_GND_MANTEAU, 0, -1);
        }
    } else if (i_this->mAnmID == B_GND_BCK_EGND_ATTACKD && i_this->mpModelMorf->checkFrame(3.0f)) {
        i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_ATTACKD, -1);
        i_this->mGndSound.startCreatureExtraSound(Z2SE_EN_GND_ATTACKD, 0, -1);
    } else if (i_this->mAnmID == B_GND_BCK_EGND_ATTACKE_A && i_this->mpModelMorf->checkFrame(1.0f)) {
        i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_ATTACKE_A, -1);
        i_this->mGndSound.startCreatureExtraSound(Z2SE_EN_GND_ATTACKE_A, 0, -1);
    } else if (i_this->mAnmID == B_GND_BCK_EGND_ATTACKE_B) {
        if (i_this->mpModelMorf->checkFrame(2.0f)) {
            i_this->mGndSound.startCreatureSound(Z2SE_EN_GND_FN_SLIDE, 0, -1);
        } else if (i_this->mpModelMorf->checkFrame(8.0f)) {
            i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_ATTACKE_B, -1);
            i_this->mGndSound.startCreatureExtraSound(Z2SE_EN_GND_ATTACKE_B, 0, -1);
        } else if (i_this->mpModelMorf->checkFrame(21.0f)) {
            i_this->mGndSound.startCreatureSound(Z2SE_EN_GND_MANTEAU, 0, -1);
        }
    } else if (i_this->mAnmID == B_GND_BCK_EGND_ATTACKE_C2) {
        if (i_this->mpModelMorf->checkFrame(8.0f)) {
            i_this->mGndSound.startCreatureSound(Z2SE_EN_GND_FN_SLIDE, 0, -1);
            i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_ATTACKE_C2, -1);
            i_this->mGndSound.startCreatureExtraSound(Z2SE_EN_GND_ATTACKE_C2, 0, -1);
        } else if (i_this->mpModelMorf->checkFrame(17.0f)) {
            i_this->mGndSound.startCreatureSound(Z2SE_EN_GND_MANTEAU, 0, -1);
        }
    } else if (i_this->mAnmID == B_GND_BCK_EGND_ATTACKF) {
        if (i_this->mpModelMorf->checkFrame(11.0f)) {
            i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_ATTACK_F, -1);
        } else if (i_this->mpModelMorf->checkFrame(13.0f)) {
            i_this->mGndSound.startCreatureExtraSound(Z2SE_EN_GND_ATTACK_F, 0, -1);
        } else if (i_this->mpModelMorf->checkFrame(19.0f) || i_this->mpModelMorf->checkFrame(28.0f)) {
            i_this->mGndSound.startCreatureSound(Z2SE_EN_GND_FN_SLIDE, 0, -1);
        } else if (i_this->mpModelMorf->checkFrame(31.0f)) {
            i_this->mGndSound.startCreatureSound(Z2SE_EN_GND_MANTEAU, 0, -1);
        }
    } else if (i_this->mAnmID == B_GND_BCK_EGND_ATTACKG) {
        if (i_this->mpModelMorf->checkFrame(17.0f)) {
            i_this->mGndSound.startCreatureExtraSound(Z2SE_EN_GND_ATTACK_G1, 0, -1);
            i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_ATTACK_G1, -1);
        } else if (i_this->mpModelMorf->checkFrame(32.0f)) {
            i_this->mGndSound.startCreatureExtraSound(Z2SE_EN_GND_ATTACK_G2, 0, -1);
            i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_ATTACK_G2, -1);
        } else if (i_this->mpModelMorf->checkFrame(26.0f)) {
            i_this->mGndSound.startCreatureSound(Z2SE_EN_GND_MANTEAU, 0, -1);
        }
    } else if (i_this->mAnmID == B_GND_BCK_EGND_JUMP_A) {
        if (i_this->mpModelMorf->checkFrame(8.0f)) {
            i_this->mGndSound.startCreatureSound(Z2SE_EN_GND_JUMP, 0, -1);
            i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_JUMP, -1);
        }
    } else if (i_this->mAnmID == B_GND_BCK_EGND_SIDEJUMP_RA || i_this->mAnmID == B_GND_BCK_EGND_SIDEJUMP_LA) {
        if (i_this->mpModelMorf->checkFrame(3.0f)) {
            i_this->mGndSound.startCreatureSound(Z2SE_EN_GND_JUMP, 0, -1);
            i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_JUMP, -1);
        }
    } else if (i_this->mAnmID == B_GND_BCK_EGND_TUBAZERI_WIN && i_this->mpModelMorf->checkFrame(14.0f)) {
        i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_TUBAZERI_WIN, -1);
        i_this->mGndSound.startCreatureExtraSound(Z2SE_EN_GND_TUBAZERI_PUSH, 0, -1);
    } else if (i_this->mAnmID == B_GND_BCK_EGND_TUBAZERI_LOSE) {
        if (i_this->mpModelMorf->checkFrame(12.0f)) {
            i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_TUBAZERI_LOSE, -1);
            i_this->mGndSound.startCreatureExtraSound(Z2SE_EN_GND_TUBAZERI_PUSH, 0, -1);
        } else if (i_this->mpModelMorf->checkFrame(18.0f) ||
            i_this->mpModelMorf->checkFrame(24.0f) ||
            i_this->mpModelMorf->checkFrame(31.0f) ||
            i_this->mpModelMorf->checkFrame(55.0f)) {
            i_this->mGndSound.startCreatureSound(Z2SE_EN_GND_FOOTNOTE, 0, -1);  
        }
    } else if (i_this->mAnmID == B_GND_BCK_EGND_DOWN && i_this->mpModelMorf->checkFrame(1.0f)) {
        i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_DAMAGE_DOWN, -1);
    } else if (i_this->mAnmID == B_GND_BCK_EGND_DOWNWAIT && i_this->mpModelMorf->checkFrame(1.0f)) {
        i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_DOWNWAIT, -1);
    } else if (i_this->mAnmID == B_GND_BCK_EGND_DOWNUP) {
        if (i_this->mpModelMorf->checkFrame(12.0f)) {
            i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_DOWNUP, -1);
        } else if (i_this->mpModelMorf->checkFrame(27.0f)) {
            i_this->mGndSound.startCreatureSound(Z2SE_EN_GND_LAND, 0, -1);
        }
    } else if (i_this->mAnmID == B_GND_BCK_EGND_DOWNB) {
        if (i_this->mpModelMorf->checkFrame(1.0f)) {
            i_this->mGndSound.startCreatureVoice(Z2SE_EN_GND_V_DOWNB, -1);
        } else if (i_this->mpModelMorf->checkFrame(17.0f)) {
            i_this->mGndSound.startCreatureSound(Z2SE_EN_GND_LAND, 0, -1);
        } else if (i_this->mpModelMorf->checkFrame(55.0f)) {
            i_this->mGndSound.startCreatureSound(Z2SE_EN_GND_MANTEAU, 0, -1);
        }
    } 
}

static int daB_GND_Execute(b_gnd_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    cXyz sp118;
    cXyz sp10C;
    cXyz sp100;

    if (i_this->mDemoCamMode == 0 && !player->checkElecDamage() && dComIfGp_event_runCheck()) {
        return 1;
    } 
    
    if (i_this->mNoDrawTimer != 0) {
        i_this->mNoDrawTimer--;
        if (i_this->mNoDrawTimer == 0) {
            dScnKy_env_light_c* kankyo = dKy_getEnvlight();
            kankyo->wether = 1;

            i_this->mActionMode = ACTION_WAIT;
            i_this->mMoveMode = 0;
            i_this->mDrawHorse = FALSE;
            a_this->speedF = 0.0f;

            mant_class* mant_p = (mant_class*)fopAcM_SearchByID(i_this->mMantChildID);
            mant_p->field_0x3969 = 1;

            h_anm_init(i_this, B_HG_BCK_HG_DOWN, 3.0f, 0, 1.0f);

            i_this->mDemoCamMode = 95;

            fpcM_Search(s_fkdel_sub, i_this);
            a_this->health = 100;
            i_this->field_0x1e0a = 0;

            Z2GetAudioMgr()->bgmStart(Z2BGM_VS_GANON_04, 0, 0);
            dComIfGs_onOneZoneSwitch(15, -1);
        } else {
            return 1;
        }
    }

    if (dComIfGp_event_runCheck()) {
        if (fopAcM_getTalkEventPartner(daPy_getLinkPlayerActorClass()) == (fopAc_ac_c*)daPy_py_c::getMidnaActor()) {
            return 1;
        }
    }

    i_this->mCounter++;

    for (int i = 0; i < 10; i++) {
        if (i_this->field_0xc44[i] != 0) {
            i_this->field_0xc44[i]--;
        }
    }

    if (i_this->mDamageInvulnerabilityTimer != 0) {
        i_this->mDamageInvulnerabilityTimer--;
    }

    if (i_this->field_0xc5a != 0) {
        i_this->field_0xc5a--;
    }

    if (i_this->field_0xc72 != 0) {
        i_this->field_0xc72--;
    }

    action(i_this);
    demo_camera(i_this);

    if (i_this->mDrawHorse) {
        h_damage_check(i_this);
        i_this->mGakeChkType = gake_check(i_this);

        if (a_this->speed.y <= 0.0f && i_this->mAcch.ChkGroundHit()) {
            dBgS_GndChk sp124;
            Vec spF4;
            cXyz spE8;

            cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
            sp118.x = 0.0f;
            sp118.y = 200.0f;
            sp118.z = 200.0f;
            MtxPosition(&sp118, &sp10C);
            sp10C += a_this->current.pos;

            sp118.z = 0.0f;
            MtxPosition(&sp118, &spE8);
            spE8 += a_this->current.pos;

            spF4.x = sp10C.x;
            spF4.y = sp10C.y;
            spF4.z = sp10C.z;
            sp124.SetPos(&spF4);
            sp10C.y = dComIfG_Bgsp().GroundCross(&sp124);

            spF4.x = spE8.x;
            spF4.y = spE8.y;
            spF4.z = spE8.z;
            sp124.SetPos(&spF4);
            spE8.y = dComIfG_Bgsp().GroundCross(&sp124);

            if (fabsf(sp10C.y - spE8.y) <= 200.0f) {
                a_this->current.angle.x = -cM_atan2s(sp10C.y - spE8.y, 200.0f);
            }

            a_this->speed.y = -20.0f;
        }
    } else {
        damage_check(i_this);
    }

    cLib_addCalcAngleS2(&a_this->shape_angle.x, a_this->current.angle.x, 2, 0x1000);
    cLib_addCalcAngleS2(&a_this->shape_angle.z, a_this->current.angle.z, 2, 0x1000);
    a_this->gravity = -5.0f;

    cMtx_YrotS(*calc_mtx, a_this->current.angle.y);
    sp118.x = 0.0f;
    sp118.y = 0.0f;
    sp118.z = a_this->speedF;
    MtxPosition(&sp118, &sp10C);

    a_this->speed.x = sp10C.x;
    a_this->speed.z = sp10C.z;

    a_this->current.pos += a_this->speed;

    a_this->speed.y += a_this->gravity;
    if (a_this->speed.y < -100.0f) {
        a_this->speed.y = -100.0f;
    }

    if (i_this->field_0xeb0 > 0.1f) {
        sp118.x = 0.0f;
        sp118.y = 0.0f;
        sp118.z = -i_this->field_0xeb0;
        cMtx_YrotS(*calc_mtx, i_this->field_0xeac);
        MtxPosition(&sp118, &sp10C);
        a_this->current.pos += sp10C;
        cLib_addCalc0(&i_this->field_0xeb0, 1.0f, 7.5f);
    }

    if (i_this->mDrawHorse) {
        cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
        sp118.set(0.0f, 0.0f, 150.0f);
        MtxPosition(&sp118, &sp10C);
        
        a_this->current.pos += sp10C;
        a_this->old.pos += sp10C;
        i_this->mAcch.CrrPos(dComIfG_Bgsp());

        a_this->current.pos -= sp10C;
        a_this->old.pos -= sp10C;

        i_this->field_0xeb4 = a_this->current.pos;
        i_this->field_0xec0 = a_this->shape_angle;
    } else {
        i_this->mAcch.CrrPos(dComIfG_Bgsp());
    }

    J3DModel* sp6C;
    if (i_this->mDrawHorse) {
        mDoMtx_stack_c::transS(i_this->field_0xeb4.x, i_this->field_0xeb4.y, i_this->field_0xeb4.z);
        mDoMtx_stack_c::YrotM(i_this->field_0xec0.y);
        mDoMtx_stack_c::XrotM(i_this->field_0xec0.x);
        mDoMtx_stack_c::ZrotM(i_this->field_0xec0.z + i_this->mHorseLegRot);

        cLib_addCalcAngleS2(&i_this->mHorseLegRot, 0, 8, 0x100);

        mDoMtx_stack_c::scaleM(l_HIO.model_size * a_this->scale.x, l_HIO.model_size * a_this->scale.x, l_HIO.model_size * a_this->scale.x);
        sp6C = i_this->mpHorseMorf->getModel();
        sp6C->setBaseTRMtx(mDoMtx_stack_c::get());

        u32 sp68;
        if (i_this->mAcch.GetGroundH() != -G_CM3D_F_INF) {
            if (i_this->mAcch.ChkWaterHit() && i_this->mAcch.m_wtr.GetHeight() > a_this->current.pos.y) {
                sp68 = dKy_pol_sound_get(&i_this->mAcch.m_wtr);
            } else if (i_this->mAcch.ChkGroundHit()) {
                sp68 = dKy_pol_sound_get(&i_this->mAcch.m_gnd);
            } else {
                sp68 = 0;
            }
        } else {
            sp68 = 0;
        }

        i_this->mpHorseMorf->play(sp68, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
        i_this->mpHorseMorf->modelCalc();
        MTXCopy(sp6C->getAnmMtx(2), *calc_mtx);

        sp118.set(70.0f, 60.0f, 0.0f);
        MtxPosition(&sp118, &sp10C);
        i_this->mHorseBodySph[0].SetC(sp10C);
        i_this->mHorseBodySph[0].SetR(100.0f);

        MTXCopy(sp6C->getAnmMtx(26), *calc_mtx);

        sp118.set(10.0f, -30.0f, 0.0f);
        MtxPosition(&sp118, &i_this->field_0x1fb8);

        i_this->mHorseBodySph[1].SetC(i_this->field_0x1fb8);
        i_this->mHorseBodySph[1].SetR(90.0f);

        i_this->mHorseBodySph[0].OffAtSetBit();
        i_this->mHorseBodySph[1].OffAtSetBit();

        if (i_this->field_0x1e08 != 0) {
            i_this->mHorseBodySph[0].OffTgSetBit();
            i_this->mHorseBodySph[1].OffTgSetBit();
        } else {
            i_this->mHorseBodySph[0].OnTgSetBit();
            i_this->mHorseBodySph[1].OnTgSetBit();
        }

        dComIfG_Ccsp()->Set(&i_this->mHorseBodySph[0]);
        dComIfG_Ccsp()->Set(&i_this->mHorseBodySph[1]);

        for (int i = 0; i < 4; i++) {
            static int footJ[4] = {6, 10, 30, 34};
            MTXCopy(sp6C->getAnmMtx(footJ[i]), *calc_mtx);
            sp118.set(0.0f, 0.0f, 0.0f);
            MtxPosition(&sp118, &sp10C);

            i_this->mHorseFootSph[i].SetC(sp10C);
            i_this->mHorseFootSph[i].SetR(40.0f);

            if (i_this->field_0x1e08 == 0 && i < 2 && a_this->speedF > 10.0f) {
                i_this->mHorseFootSph[i].OnAtSetBit();
            } else {
                i_this->mHorseFootSph[i].OffAtSetBit();
            }

            if (i_this->field_0x1e08 != 0) {
                i_this->mHorseFootSph[i].OffTgSetBit();
            } else {
                i_this->mHorseFootSph[i].OnTgSetBit();
            }

            dComIfG_Ccsp()->Set(&i_this->mHorseFootSph[i]);
        }
    }

    if (!i_this->mDrawHorse) {
        MtxTrans(a_this->current.pos.x, a_this->current.pos.y, a_this->current.pos.z, 0);
        cMtx_YrotM(*calc_mtx, a_this->shape_angle.y);
        cMtx_XrotM(*calc_mtx, a_this->shape_angle.x);
        cMtx_ZrotM(*calc_mtx, a_this->shape_angle.z);
    } else {
        MTXCopy(sp6C->getAnmMtx(21), *calc_mtx);
        cMtx_ZrotM(*calc_mtx, -0x4000);
        MtxTrans(0.0f, 55.0f, -30.0f, 1);
    }

    sp6C = i_this->mpModelMorf->getModel();
    sp6C->setBaseTRMtx(*calc_mtx);
    i_this->mpModelMorf->play(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));

    if (i_this->mAnmID == B_GND_BCK_EGND_DEFENSE_C || i_this->mAnmID == B_GND_BCK_EGND_DEFENSE_D) {
        cLib_addCalcAngleS2(&i_this->mGndShoulderLRotY, cM_ssin(i_this->mCounter * 4000) * 600.0f + -1200.0f, 1, 500);

        if (i_this->field_0x1fc8 != 0) {
            i_this->field_0x1fc8--;
            i_this->mGndArmRRotX = i_this->field_0x1fc8 * (cM_ssin(i_this->mCounter * 0x5500)) * 90.0f;
        }
    } else {
        cLib_addCalcAngleS2(&i_this->mGndShoulderLRotY, 0, 1, 100);
        i_this->mGndArmRRotX = 0;
        i_this->field_0x1fc8 = 0;
    }

    i_this->mpGndCoreBrk->play();

    if (i_this->mGndEyeBtkNo == 0) {
        i_this->mpGndEyeBtk[i_this->mGndEyeBtkNo]->setFrame(0.0f);
    } else {
        i_this->mpGndEyeBtk[i_this->mGndEyeBtkNo]->play();
    }

    i_this->mpGndEyeBtp->setFrame(i_this->field_0xc6c);
    i_this->mpModelMorf->modelCalc();

    if (!i_this->mHideSheath) {
        if (i_this->field_0x772 != 0) {
            i_this->field_0x772++;
            MTXCopy(i_this->mpSheathModel->getBaseTRMtx(), *calc_mtx);
            MtxTrans(10.0f, 0.0f, -5.0f, 1);
            cMtx_YrotM(*calc_mtx, 800);
            cMtx_XrotM(*calc_mtx, 0);
            cMtx_ZrotM(*calc_mtx, 0);

            if (i_this->field_0x772 > 20) {
                i_this->mHideSheath = TRUE;
            }
        } else {
            MTXCopy(sp6C->getAnmMtx(24), *calc_mtx);
        }

        i_this->mpSheathModel->setBaseTRMtx(*calc_mtx);
    }

    if (i_this->field_0x770 == 1) {
        MtxTrans(81.81f, 2.54f, 2.9f, 1);
        cMtx_XrotM(*calc_mtx, 10800);
        cMtx_YrotM(*calc_mtx, 550);
        cMtx_ZrotM(*calc_mtx, 20030);
        i_this->mpSwordModel->setBaseTRMtx(*calc_mtx);
    } else {
        MTXCopy(sp6C->getAnmMtx(33), *calc_mtx);
        i_this->mpSwordModel->setBaseTRMtx(*calc_mtx);
    }

    if (i_this->mSwordBlurAlpha >= 80) {
        i_this->mSwordBlurType++;
        if (i_this->mSwordBlurType > 1) {
            i_this->mSwordBlurType = 1;
        }

        cMtx_YrotM(*calc_mtx, -0x4048);
        cMtx_XrotM(*calc_mtx, 0x32DD);
        cMtx_ZrotM(*calc_mtx, i_this->field_0xc28 + 0x4ECF);
        MtxTrans(0.0f, 0.0f, -130.0f, 1);
        i_this->mpSwordBlurModel[i_this->mSwordBlurType]->setBaseTRMtx(*calc_mtx);
        MTXCopy(sp6C->getAnmMtx(33), *calc_mtx);

        i_this->field_0xc28 = 0;
    } else if (i_this->mSwordBlurAlpha > 10) {
        i_this->mSwordBlurType = 1;
    } else {
        i_this->mSwordBlurType = -1;
        i_this->mSwordBlurAlpha = 0;
    }

    cLib_addCalcAngleS2(&i_this->mSwordBlurAlpha, 0, 1, 20);

    if (i_this->field_0xc78 == 0) {
        sp118.set(40.0f, 80.0f, 0.0f);
        MtxPosition(&sp118, &sp10C);
        i_this->field_0x269c = sp10C;

        if (i_this->mDrawHorse) {
            i_this->mAtSph.SetR(120.0f);
            if (player->checkHorseRide()) {
                i_this->mAtSph.SetAtSpl((dCcG_At_Spl)13);
            } else {
                i_this->mAtSph.SetAtSpl((dCcG_At_Spl)0);
            }
        } else {
            i_this->mAtSph.SetR(100.0f);
            i_this->mAtSph.SetAtSpl((dCcG_At_Spl)10);
        }
    } else if (i_this->field_0xc78 == 1) {
        MTXCopy(sp6C->getAnmMtx(41), *calc_mtx);
        sp118.set(0.0f, 0.0f, 0.0f);
        MtxPosition(&sp118, &sp10C);
        i_this->mAtSph.SetR(70.0f);
        i_this->mAtSph.SetAtSpl((dCcG_At_Spl)10);
    } else if (i_this->field_0xc78 == 2) {
        MTXCopy(sp6C->getAnmMtx(19), *calc_mtx);
        sp118.set(0.0f, 0.0f, 0.0f);
        MtxPosition(&sp118, &sp10C);
        i_this->mAtSph.SetR(70.0f);
        i_this->mAtSph.SetAtSpl((dCcG_At_Spl)10);
    } else {
        sp118.set(40.0f, 130.0f, 0.0f);
        MtxPosition(&sp118, &sp10C);
    }

    if (i_this->field_0xc77 == 0) {
        sp10C.y -= 20000.0f;
    } else {
        if (i_this->field_0xc77 == 2) {
            i_this->mAtSph.SetAtSpl((dCcG_At_Spl)11);
            i_this->mAtSph.SetAtAtp(4);
        } else {
            i_this->mAtSph.SetAtAtp(2);
        }

        i_this->field_0xc78 = 0;
        i_this->field_0xc77 = 0;
    }

    i_this->mAtSph.SetC(sp10C);
    dComIfG_Ccsp()->Set(&i_this->mAtSph);
    cMtx_YrotS(*calc_mtx, a_this->shape_angle.y);
    cMtx_XrotM(*calc_mtx, a_this->shape_angle.x);

    sp118.set(0.0f, 130.0f, 0.0f);
    MtxPosition(&sp118, &sp10C);
    sp10C += i_this->current.pos;

    if (i_this->field_0xc79 != 0) {
        i_this->mDefSph.SetR(50.0f);
        i_this->mDefSph.SetC(sp10C);
        dComIfG_Ccsp()->Set(&i_this->mDefSph);
        i_this->field_0xc79 = 0;
    } else {
        sp10C.y -= 13000.0f;
    }

    i_this->mDefSph.SetC(sp10C);
    dComIfG_Ccsp()->Set(&i_this->mDefSph);

    if (i_this->mDefSph.ChkTgHit()) {
        def_se_set(&i_this->mGndSound, i_this->mDefSph.GetTgHitObj(), 40, NULL);
        i_this->field_0x1fc8 = 7;
    }

    MTXCopy(sp6C->getAnmMtx(4), *calc_mtx);
    sp118.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp118, &a_this->eyePos);
    a_this->attention_info.position = a_this->eyePos;

    a_this->attention_info.position.y += 50.0f;

    f32 var_f29 = 50.0f;
    if (i_this->mDrawHorse) {
        var_f29 = 100.0f;
    }

    cXyz spDC;
    if (i_this->mDamageInvulnerabilityTimer != 0) {
        spDC.set(-20000.0f, 100000.0f, -13456.0f);
    } else {
        spDC.set(0.0f, 0.0f, 0.0f);
    }

    i_this->mGndSph[0].SetC(a_this->eyePos + spDC);
    i_this->mGndSph[0].SetR(40.0f);

    MTXCopy(sp6C->getAnmMtx(2), *calc_mtx);
    MtxPosition(&sp118, &sp10C);
    i_this->setDownPos(&sp10C);

    i_this->mGndSph[1].SetC(sp10C + spDC);
    i_this->mGndSph[1].SetR(var_f29);

    MTXCopy(sp6C->getAnmMtx(1), *calc_mtx);
    sp118.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp118, &sp10C);

    i_this->mGndSph[2].SetC(sp10C + spDC);
    i_this->mGndSph[2].SetR(var_f29);

    for (int i = 0; i < 3; i++) {
        if (i_this->mDrawHorse) {
            if (i_this->field_0x1e08 != 0) {
                i_this->mGndSph[i].SetTgType(2);
            } else {
                i_this->mGndSph[i].SetTgType(0x2000);
            }
        } else {
            i_this->mGndSph[i].SetTgType(0x12002);
        }

        dComIfG_Ccsp()->Set(&i_this->mGndSph[i]);
    }

    MTXCopy(sp6C->getAnmMtx(37), mDoMtx_stack_c::get());
    mDoMtx_stack_c::multVecZero(&i_this->field_0x26a8[0]);

    MTXCopy(sp6C->getAnmMtx(42), mDoMtx_stack_c::get());
    mDoMtx_stack_c::multVecZero(&i_this->field_0x26a8[1]);

    sp118.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp118, &sp10C);
    sp10C.y = a_this->current.pos.y;

    i_this->mCoCyl.SetC(sp10C);
    i_this->mCoCyl.SetR(50.0f);
    i_this->mCoCyl.SetH(200.0f);
    dComIfG_Ccsp()->Set(&i_this->mCoCyl);

    static u16 b_id[6] = {0x8A37, 0x8A38, 0x8A39, 0x8A3A, 0x8A3B, 0x8C24};
    static u16 b_id2[5] = {0x8A3D, 0x8A3E, 0x8A3F, 0x8A40, 0x8A41};

    if (i_this->field_0x1e0f > 0) {
        i_this->field_0x1e2c.startLevelSound(Z2SE_EN_GND_LIGHTBALL, 0, -1);

        if (i_this->field_0x1e0f == 1) {
            MTXCopy(sp6C->getAnmMtx(24), *calc_mtx);
            sp118.set(0.0f, 0.0f, 0.0f);
            MtxPosition(&sp118, &i_this->field_0x1e20);
            cLib_addCalc2(&i_this->field_0x1e10, 1.0f, 1.0f, 0.02f);
        } else {
            i_this->field_0x1e0e++;
            i_this->field_0x1e14 = i_this->field_0x1e20;
            sp118 = a_this->current.pos;
            sp118.y += 800.0f;
            sp118 -= i_this->field_0x1e20;

            if (i_this->field_0x1e0e > 10) {
                cLib_addCalcAngleS2(&i_this->field_0x1e50.y,cM_atan2s(sp118.x, sp118.z), 4, 0x800);
                cLib_addCalcAngleS2(&i_this->field_0x1e50.x, (s16)-cM_atan2s(sp118.y, JMAFastSqrt(sp118.x * sp118.x + sp118.z * sp118.z)), 4, 0x800);
            }

            cLib_addCalc2(&i_this->field_0x1e4c, 100.0f, 1.0f, 2.0f);
            cMtx_YrotS(*calc_mtx, i_this->field_0x1e50.y);
            cMtx_XrotM(*calc_mtx, i_this->field_0x1e50.x);
            sp118.x = 0.0f;
            sp118.y = 0.0f;
            sp118.z = i_this->field_0x1e4c;
            MtxPosition(&sp118, &sp10C);

            i_this->field_0x1e20 += sp10C;
            i_this->field_0x1e20 += a_this->speed;

            if (i_this->field_0x1e0e == 50) {
                i_this->field_0x1e0f = -1;
                i_this->field_0x1e2c.startSound(Z2SE_EN_GND_LIGHTBALL_SPLIT, 0, -1);

                cMtx_YrotS(*calc_mtx, i_this->mPlayerAngleY);
                sp118.y = -50.0f - cM_rndF(10.0f);
                sp118.z = cM_rndF(20.0f);

                for (int i = 0; i < 5; i++) {
                    i_this->field_0x1e56[i] = 1;

                    static f32 bun_xs[] = {-150.0f, -75.0f, 0.0f, 75.0f, 150.0f};
                    sp118.x = bun_xs[i];
                    MtxPosition(&sp118, &i_this->field_0x1ed4[i]);
                    i_this->mLightBallPos[i] = i_this->field_0x1e20;
                    i_this->field_0x1e98[i] = i_this->mLightBallPos[i];
                }
            }
        }

        for (int i = 0; i < 6; i++) {
            i_this->field_0x25f0[i] = dComIfGp_particle_set(i_this->field_0x25f0[i], b_id[i], &i_this->field_0x1e20, NULL, NULL);
        }
    } else {
        if (i_this->field_0x1e0f < 0) {
            for (int i = 0; i < 6; i++) {
                JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->field_0x25f0[i]);
                if (emitter != NULL) {
                    emitter->deleteAllParticle();
                    dComIfGp_particle_levelEmitterOnEventMove(i_this->field_0x25f0[i]);
                }
            }

            for (int i = 0; i < 5; i++) {
                dComIfGp_particle_set(b_id2[i], &i_this->field_0x1e20, NULL, NULL);
            }

            i_this->field_0x1e0f = 0;
        }
    }

    cXyz ball_eff_size(1.0f, 1.0f, 1.0f);
    csXyz fk_angle(0, 0, 0);
    for (int i = 0; i < 5; i++) {
        if (i_this->field_0x1e56[i] == 1) {
            i_this->mLightBallSound[i].startLevelSound(Z2SE_EN_GND_LIGHTBALL_SUB, 0, -1);
            i_this->field_0x1e98[i] = i_this->mLightBallPos[i];
            i_this->mLightBallPos[i] += i_this->field_0x1ed4[i];

            for (int j = 0; j < 6; j++) {
                i_this->field_0x2608[i][j] = dComIfGp_particle_set(i_this->field_0x2608[i][j], b_id[j], &i_this->mLightBallPos[i], NULL, &ball_eff_size);
            }

            if (ball_bg_check(i_this, i)) {
                i_this->field_0x1e56[i] = 2;

                for (int k = 0; k < 5; k++) {
                    dComIfGp_particle_set(b_id2[k], &i_this->mLightBallPos[i], NULL, &ball_eff_size);
                }

                sp118 = (player->current.pos - i_this->mLightBallPos[i]);
                fk_angle.y = (s16)cM_atan2s(sp118.x, sp118.z);

                fopAcM_create(PROC_E_FK, i | 0xFFFFFF00, &i_this->mLightBallPos[i], fopAcM_GetRoomNo(a_this), &fk_angle, NULL, -1);
                i_this->mLightBallSound[i].startSound(Z2SE_EN_GND_LIGHTBALL_SUB_HIT, 0, -1);
            }
        } else if (i_this->field_0x1e56[i] == 2) {
            i_this->field_0x1e56[i] = 0;

            for (int j = 0; j < 6; j++) {
                JPABaseEmitter* emitter = dComIfGp_particle_getEmitter(i_this->field_0x2608[i][j]);
                if (emitter != NULL) {
                    emitter->deleteAllParticle();
                    dComIfGp_particle_levelEmitterOnEventMove(i_this->field_0x2608[i][j]);
                }
            }
        }
    }

    mant_class* mant_p = (mant_class*)fopAcM_SearchByID(i_this->mMantChildID);
    if (mant_p != NULL) {
        mant_p->current.pos = a_this->current.pos;

        MTXCopy(sp6C->getAnmMtx(34), *calc_mtx);
        sp118.set(10.0f, 5.0f, -17.0f);
        MtxPosition(&sp118, &mant_p->field_0x3928[0]);

        MTXCopy(sp6C->getAnmMtx(25), *calc_mtx);
        sp118.set(10.0f, 5.0f, 17.0f);
        MtxPosition(&sp118, &mant_p->field_0x3928[1]);

        f32 var_f26 = 2.5f;
        for (int i = 0; i < 6; i++) {
            for (int j = 0; j < 6; j++) {
                cXyz* sp30 = &mant_p->field_0x25a8[i * 2 + 1].field_0x0[j * 2];
                cXyz* sp2C = &mant_p->field_0x25a8[i * 2 + 1].field_0x0[j * 2 + 1];
                mDoMtx_stack_c::transS(sp30->x, sp30->y, sp30->z);
                sp118 = *sp2C - *sp30;

                f32 var_f28 = var_f26 + (f32)(j * 0.25f);

                mDoMtx_stack_c::YrotM(cM_atan2s(sp118.x, sp118.z));
                mDoMtx_stack_c::XrotM(-cM_atan2s(sp118.y, JMAFastSqrt(sp118.x * sp118.x + sp118.z * sp118.z)));
                mDoMtx_stack_c::scaleM(var_f28, var_f28, var_f28);
                i_this->mpMantShadowModel[i + j * 6]->setBaseTRMtx(mDoMtx_stack_c::get());
            }
        }
    }

    if (i_this->mDrawHorse) {
        i_this->field_0x1fd4 += i_this->field_0x1fd6;

        cLib_addCalcAngleS2(&i_this->field_0x1fd6, (s16)(i_this->speedF * 50.0f + 1500.0f), 1, 60);
        cLib_addCalc2(&i_this->field_0x1fd0, i_this->speedF * 1.5f, 1.0f, 0.9f);

        sp6C = i_this->mpModelMorf->getModel();
        J3DModel* horse_model = i_this->mpHorseMorf->getModel();

        MTXCopy(horse_model->getAnmMtx(19), *calc_mtx);
        MtxPush();
        sp118.set(0.0f, 7.0f, -15.0f);
        MtxPosition(&sp118, &sp10C);
        himo_control1(i_this, &sp10C, 0, 0);
        MtxPull();

        sp118.set(0.0f, 7.0f, 15.0f);
        MtxPosition(&sp118, &sp10C);
        himo_control1(i_this, &sp10C, 1, 0);

        MTXCopy(horse_model->getAnmMtx(21), *calc_mtx);
        sp118.set(60.0f, 0.0f, 50.0f);
        MtxPosition(&sp118, i_this->field_0x1fd8);

        i_this->field_0x1fd8[1] = i_this->field_0x1fd8[0];
        himo_control2(i_this, &i_this->field_0x1fd8[0], 0, 0);
        himo_control2(i_this, &i_this->field_0x1fd8[1], 1, 0);
    }

    if (i_this->mDrawHorse) {
        eff_set_h(i_this);

        i_this->field_0x1e2c.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));

        for (int i = 0; i < 5; i++) {
            i_this->mLightBallSound[i].framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(a_this)));
        }
    } else {
        eff_set(i_this);
    }

    anm_se_set(i_this);

    f32 target_blend = 1.0f;
    f32 blend_step = 0.1f;

    if (i_this->field_0x2739 == 0) {
        target_blend = 0.0f;
        dKy_custom_colset(0, 0, i_this->mKankyoBlend);
    } else if (i_this->field_0x2739 == 1) {
        blend_step = 0.05f;
        dKy_custom_colset(0, 5, i_this->mKankyoBlend);
    } else if (i_this->field_0x2739 == 2) {
        target_blend = 0.0f;
        dKy_custom_colset(1, 1, i_this->mKankyoBlend);
    } else if(i_this->field_0x2739 == 3) {
        target_blend = 0.0f;
        dKy_custom_colset(2, 2, i_this->mKankyoBlend);
    } else if (i_this->field_0x2739 == 4) {
        target_blend = 0.0f;
        dKy_custom_colset(3, 3, i_this->mKankyoBlend);
    }else if (i_this->field_0x2739 == 5) {
        target_blend = 0.0f;
        dKy_custom_colset(4, 4, i_this->mKankyoBlend);
    }

    if (target_blend >= 0.0f) {
        cLib_addCalc2(&i_this->mKankyoBlend, target_blend, 1.0f, blend_step);
    }

    return 1;
}

static int daB_GND_IsDelete(b_gnd_class* i_this) {
    return 1;
}

static int daB_GND_Delete(b_gnd_class* i_this) {
    fopAc_ac_c* a_this = (fopAc_ac_c*)i_this;

    dComIfG_resDelete(&i_this->mPhaseReq, "B_gnd");
    dComIfG_resDelete(&i_this->mHorsePhaseReq, "B_hg");

    if (i_this->mInitHIO) {
        l_initHIO = false;
    }

    if (a_this->heap != NULL) {
        i_this->mpModelMorf->stopZelAnime();
        i_this->mpHorseMorf->stopZelAnime();
        i_this->field_0x1e2c.deleteObject();

        for (int i = 0; i < 5; i++) {
            i_this->mLightBallSound[i].deleteObject();
        }
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* a_this) {
    b_gnd_class* i_this = (b_gnd_class*)a_this;

    static int const blur_model[2] = {0x66, 0x65};
    static int btk_d[6] = {0x77, 0x76, 0x73, 0x74, 0x75, 0x72};

    i_this->mpModelMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("B_gnd", 0x69), 
        NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("B_gnd", 0x4b),
        2, 1.0f, 0, -1, &i_this->mGndSound, 0, 0x11020284);

    if (i_this->mpModelMorf == NULL || i_this->mpModelMorf->getModel() == NULL) {
        return 0;
    }

    J3DModel* model = i_this->mpModelMorf->getModel();
    model->setUserArea((uintptr_t)i_this);

    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(nodeCallBack);
    }

    mDoMtx_stack_c::scaleS(0.0f, 0.0f, 0.0f);
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    i_this->mpGndCoreBrk = new mDoExt_brkAnm();
    if (i_this->mpGndCoreBrk == NULL) {
        return 0;
    }

    if (!i_this->mpGndCoreBrk->init(i_this->mpModelMorf->getModel()->getModelData(),
        (J3DAnmTevRegKey*)dComIfG_getObjectRes("B_gnd", 0x6d), 
        1, 2, 1.0f, 0, -1)) {
        return 0;
    }

    for (int i = 0; i < 6; i++) {
        i_this->mpGndEyeBtk[i] = new mDoExt_btkAnm();
        if (i_this->mpGndEyeBtk[i] == NULL) {
            return 0;
        }

        if (!i_this->mpGndEyeBtk[i]->init(i_this->mpModelMorf->getModel()->getModelData(), 
            (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("B_gnd", btk_d[i]), 
            1, 0, 1.0f, 0, -1)) {
            return 0;
        }
    }

    i_this->mpGndEyeBtp = new mDoExt_btpAnm();
    if (i_this->mpGndEyeBtp == NULL) {
        return 0;
    }

    if (!i_this->mpGndEyeBtp->init(i_this->mpModelMorf->getModel()->getModelData(),
        (J3DAnmTexPattern*)dComIfG_getObjectRes("B_gnd", 0x7b), 1, 2, 1.0f, 0, -1)) {
        return 0;
    }
        
    J3DModelData* modelData;
    modelData = (J3DModelData*)dComIfG_getObjectRes("B_gnd", 0x62);
    JUT_ASSERT(6612, modelData != NULL);
    i_this->mpSwordModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (i_this->mpSwordModel == NULL) {
        return 0;
    }
        
    mDoMtx_stack_c::scaleS(0.0f, 0.0f, 0.0f);
    i_this->mpSwordModel->setBaseTRMtx(mDoMtx_stack_c::get());

    modelData = (J3DModelData*)dComIfG_getObjectRes("B_gnd", 0x61);
    JUT_ASSERT(6626, modelData != NULL);
    i_this->mpSheathModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (i_this->mpSheathModel == NULL) {
        return 0;
    }
        
    mDoMtx_stack_c::scaleS(0.0f, 0.0f, 0.0f);
    i_this->mpSheathModel->setBaseTRMtx(mDoMtx_stack_c::get());

    for (int i = 0; i < 2; i++) {
        modelData = (J3DModelData*)dComIfG_getObjectRes("B_gnd", blur_model[i]);
        JUT_ASSERT(6647, modelData != NULL);
        i_this->mpSwordBlurModel[i] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (i_this->mpSwordBlurModel[i] == NULL) {
            return 0;
        }

        mDoMtx_stack_c::scaleS(0.0f, 0.0f, 0.0f);
        i_this->mpSwordBlurModel[i]->setBaseTRMtx(mDoMtx_stack_c::get());
    }

    modelData = (J3DModelData*)dComIfG_getObjectRes("B_gnd", 99);
    JUT_ASSERT(6670, modelData != NULL);

    for (int i = 0; i < 36; i++) {
        i_this->mpMantShadowModel[i] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (i_this->mpMantShadowModel[i] == NULL) {
            return 0;
        }

        mDoMtx_stack_c::scaleS(0.0f, 0.0f, 0.0f);
        i_this->mpMantShadowModel[i]->setBaseTRMtx(mDoMtx_stack_c::get());
    }

    i_this->mpHorseMorf = new mDoExt_McaMorfSO((J3DModelData*)dComIfG_getObjectRes("B_hg", 19),
         NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("B_hg", 15), 
         2, 1.0f, 0, -1, &i_this->mHorseSound, 0x80000, 0x11000084);
    if (i_this->mpHorseMorf == NULL || i_this->mpHorseMorf->getModel() == NULL) {
        return 0;
    }
    
    model = i_this->mpHorseMorf->getModel();
    model->setUserArea((uintptr_t)a_this);
    for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
        model->getModelData()->getJointNodePointer(i)->setCallBack(h_nodeCallBack);
    }

    mDoMtx_stack_c::scaleS(0.0f, 0.0f, 0.0f);
    model->setBaseTRMtx(mDoMtx_stack_c::get());

    for (int i = 0; i < 2; i++) {
        if (!i_this->mHorseReins[i].init(1, 16, (ResTIMG *)dComIfG_getObjectRes("B_hg", 22), 1)) {
            return 0;
        }

        f32* size = i_this->mHorseReins[i].getSize(0);
        for (int j = 0; j < 16; j++) {
            *size = 4.0f;
            size++;
        }
    }

    if (!i_this->field_0x21e8.init(1, 2, (ResTIMG *)dComIfG_getObjectRes("B_hg", 22), 1)) {
        return 0;
    }

    f32* size = i_this->field_0x21e8.getSize(0);
    for (int i = 0; i < 2; i++) {
        *size = 4.0f;
        size++;
    }

    i_this->mpZeldaModel = new mDoExt_McaMorf((J3DModelData*)dComIfG_getObjectRes("B_gnd", 0x6a),
        NULL, NULL, (J3DAnmTransform*)dComIfG_getObjectRes("B_gnd", 0x5e), 
        0, 1.0f, 0, -1, 1, NULL, 0, 0x11020284);
    if (i_this->mpZeldaModel == NULL || i_this->mpZeldaModel->getModel() == NULL) {
        return 0;
    }

    i_this->mpZeldaBtk = new mDoExt_btkAnm();
    if (i_this->mpZeldaBtk == NULL) {
        return 0;
    }
    
    if (!i_this->mpZeldaBtk->init(i_this->mpZeldaModel->getModel()->getModelData(),
        (J3DAnmTextureSRTKey*)dComIfG_getObjectRes("B_gnd", 0x78), 
        1, 0, 1.0f, 0, -1)) {
            return 0;
    }

    i_this->mpZeldaBtp = new mDoExt_btpAnm();
    if (i_this->mpZeldaBtp == NULL) {
        return 0;
    }

    J3DAnmTexPattern* texPattern = (J3DAnmTexPattern*)dComIfG_getObjectRes("B_gnd", 0x7c);
    if (!i_this->mpZeldaBtp->init(i_this->mpZeldaModel->getModel()->getModelData(),
        texPattern, 
        1, 0, 1.0f, 0, -1)) {
        return 0;
    }
    
    return 1;
}

static int daB_GND_Create(fopAc_ac_c* a_this) {
    b_gnd_class* i_this = (b_gnd_class*)a_this;
    fopAcM_ct(i_this, b_gnd_class);

    int phase_state = dComIfG_resLoad(&i_this->mPhaseReq, "B_gnd");
    int h_phase_state = dComIfG_resLoad(&i_this->mHorsePhaseReq, "B_hg");
    i_this->field_0x5be = fopAcM_GetParam(a_this);

    if (h_phase_state != cPhs_COMPLEATE_e) {
        phase_state = h_phase_state;
    }

    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("b_gnd PARAM %x\n", fopAcM_GetParam(a_this));

        if (!fopAcM_entrySolidHeap(a_this, useHeapInit, 0x2B1A0)) {
            OS_REPORT("//////////////B_GND SET NON !!\n");
            return cPhs_ERROR_e;
        }
        
        if (!l_initHIO) {
            i_this->mInitHIO = true;
            l_initHIO = true;
            l_HIO.no = mDoHIO_CREATE_CHILD("ガノンドロフ", &l_HIO);
        }

        a_this->attention_info.distances[2] = 36;

        fopAcM_SetMtx(a_this, i_this->mpModelMorf->getModel()->getBaseTRMtx());
        fopAcM_SetMin(a_this, -400.0f, -200.0f, -400.0f);
        fopAcM_SetMax(a_this, 400.0f, 200.0f, 400.0f);

        i_this->mAcch.Set(fopAcM_GetPosition_p(a_this), fopAcM_GetOldPosition_p(a_this), a_this, 1, &i_this->mAcchCir, fopAcM_GetSpeed_p(a_this), NULL, NULL);
        i_this->mAcchCir.SetWall(50.0f, 150.0f);
        i_this->mAcch.ClrWaterNone();

        cXyz sp3C;
        cXyz sp30;
        sp3C.y = 0.0f;
        sp3C.x = 0.0f;

        for (int i = 0; i < 8; i++) {
            cMtx_YrotS(*calc_mtx, i << 13);
            sp3C.z = cM_rndF(3000.0f) + 12000.0f;
            MtxPosition(&sp3C, &sp30);

            b_path[i].x = sp30.x;
            b_path[i].z = sp30.z;
        }

        i_this->mMantChildID = fopAcM_createChild(PROC_MANT, fopAcM_GetID(a_this),
                0, &a_this->current.pos, fopAcM_GetRoomNo(a_this), NULL, NULL, -1, 0);
        a_this->health = 24;
        i_this->field_0x560 = 24;

        i_this->mGndCcStts.Init(0xFE, 0, a_this);
        i_this->mHorseCcStts.Init(150, 0, a_this);

        static dCcD_SrcSph h_cc_sph_src = {
            {
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x2, 0x1f}, {0x4006020, 0x3}, 0x75}}, // mObj
                {dCcD_SE_HARD_BODY, 0x0, 0x7, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 40.0f} // mSph
            } // mSphAttr
        };
    
        static dCcD_SrcSph cc_sph_src = {
            {
            {0x00, {{AT_TYPE_0, 0x0, 0x00}, {0x4012000, 0x3}, 0x75}}, // mObj
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
                {0x0, {{AT_TYPE_CSTATUE_SWING, 0x2, 0x1f}, {0x0, 0x0}, 0x0}}, // mObj
                {dCcD_SE_HARD_BODY, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 35.0f} // mSph
            } // mSphAttr
        };
    
        static dCcD_SrcSph def_sph_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0xdcfbfdfd, 0x3}, 0x0}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_METAL, 0x5, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {{0.0f, 0.0f, 0.0f}, 100.0f} // mSph
            } // mSphAttr
        };
    
        static dCcD_SrcCyl co_cyl_src = {
            {
                {0x0, {{0x0, 0x0, 0x0}, {0x0, 0x0}, 0x75}}, // mObj
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                {0x0}, // mGObjCo
            }, // mObjInf
            {
                {0.0f, 0.0f, 0.0f}, // mCenter
                100.0f, // mRadius
                200.0f // mHeight
            } // mCyl
        };

        for (int i = 0; i < 2; i++) {
            i_this->mHorseBodySph[i].Set(h_cc_sph_src);
            i_this->mHorseBodySph[i].SetStts(&i_this->mHorseCcStts);
            i_this->mHorseBodySph[i].OnTgShield();
            i_this->mHorseBodySph[i].SetTgHitMark(CcG_Tg_UNK_MARK_2);
        }

        for (int i = 0; i < 4; i++) {
            i_this->mHorseFootSph[i].Set(h_cc_sph_src);
            i_this->mHorseFootSph[i].SetStts(&i_this->mHorseCcStts);
            i_this->mHorseFootSph[i].OnTgShield();
            i_this->mHorseFootSph[i].SetTgHitMark(CcG_Tg_UNK_MARK_2);
        }

        for (int i = 0; i < 3; i++) {
            i_this->mGndSph[i].Set(cc_sph_src);
            i_this->mGndSph[i].SetStts(&i_this->mGndCcStts);
            i_this->mGndSph[i].OnTgNoHitMark();
        }

        i_this->mAtSph.Set(at_sph_src);
        i_this->mAtSph.SetStts(&i_this->mGndCcStts);

        i_this->mAtInfo.mpSound = &i_this->mGndSound;
        i_this->mAtInfo.mPowerType = 5;

        i_this->mGndSound.init(&a_this->current.pos, &a_this->eyePos, &i_this->field_0x269c, 3, 1, 1);
        i_this->mGndSound.setEnemyName("B_gnd");

        i_this->mHorseSound.init(&a_this->current.pos, &a_this->eyePos, 3, 1);
        i_this->mGndSound.setEnemyName("B_gnd");

        i_this->mDefSph.Set(def_sph_src);
        i_this->mDefSph.SetStts(&i_this->mGndCcStts);

        i_this->mCoCyl.Set(co_cyl_src);
        i_this->mCoCyl.SetStts(&i_this->mGndCcStts);

        i_this->mActionMode = ACTION_HWAIT_1;
        i_this->mMoveMode = -1;
        i_this->mDrawHorse = TRUE;

        dKy_tevstr_init(&i_this->mSwordTevstr, fopAcM_GetRoomNo(a_this), -1);

        i_this->field_0x770 = 0;
        i_this->mHideSheath = TRUE;

        if (dComIfGs_isSaveDunSwitch(1)) {
            i_this->mNoDrawTimer = 2;
        }

        i_this->field_0x1e2c.init(&i_this->field_0x1e20, 1);

        for (int i = 0; i < 5; i++) {
            i_this->mLightBallSound[i].init(&i_this->mLightBallPos[i], 1);
        }

        daB_GND_Execute(i_this);
    }
    
    return phase_state;
}

b_gnd_class::b_gnd_class() {}

static actor_method_class l_daB_GND_Method = {
    (process_method_func)daB_GND_Create,
    (process_method_func)daB_GND_Delete,
    (process_method_func)daB_GND_Execute,
    (process_method_func)daB_GND_IsDelete,
    (process_method_func)daB_GND_Draw,
};

extern actor_process_profile_definition g_profile_B_GND = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_B_GND,             // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(b_gnd_class),    // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  214,                    // mPriority
  &l_daB_GND_Method,      // sub_method
  0x00044000,             // mStatus
  fopAc_ENEMY_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};

static int unk_bss_0xF0;

AUDIO_INSTANCES;
