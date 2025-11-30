/**
 * @file d_a_mg_rod.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_mg_rod.h"
#include "d/d_com_inf_game.h"
#include "d/d_s_play.h"
#include "d/actor/d_a_alink.h"
#include "d/actor/d_a_canoe.h"
#include "d/actor/d_a_npc_henna.h"
#include "d/actor/d_a_obj_lp.h"
#include "d/actor/d_a_e_sg.h"
#include "d/actor/d_a_obj_life_container.h"
#include "d/d_menu_window.h"
#include "d/d_meter2_info.h"
#include "d/d_msg_object.h"
#include "d/d_item.h"
#include "f_op/f_op_kankyo_mng.h"
#include "c/c_damagereaction.h"
#include "SSystem/SComponent/c_counter.h"
#include "Z2AudioLib/Z2Instances.h"
#include <cmath.h>

class dmg_rod_HIO_c : public JORReflexible {
public:
    dmg_rod_HIO_c();
    virtual ~dmg_rod_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x04 */ s8 id;
    /* 0x08 */ f32 field_0x8;
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 field_0x1c;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ u8 force_fish_msg_output;
};

static u8 const lit_3879[12] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static u16 check_kind[] = {
    0xF57F, 0xF47F, 0xF37F, 0xF27F,
};

dmg_rod_HIO_c::dmg_rod_HIO_c() {
    id = -1;
    field_0x8 = 1.0f;
    field_0x20 = 0.3f;
    field_0xc = 0.7f;
    field_0x10 = -7.0f;
    field_0x14 = 35.0f;
    field_0x18 = 0.8f;
    field_0x1c = 1.5f;
    force_fish_msg_output = 0;
}

#if DEBUG
void dmg_rod_HIO_c::genMessage(JORMContext* ctx) {
    // Fishing System
    ctx->genLabel("　釣りシステム　", 0x80000001, 0, NULL, 0xFFFF, 0xFFFF, 0x200, 0x18);
    ctx->startComboBox("強制魚メッセージ出力", &force_fish_msg_output, 0, NULL, 0xFFFF, 0xFFFF, 0x100, 0x1A);
    ctx->genComboBoxItem("通常", 0); // Normal
    ctx->genComboBoxItem("バス", 1); // Bass
    ctx->genComboBoxItem("ドジョウ", 2); //Loach
    ctx->genComboBoxItem("パイク", 3); // Pike
    ctx->genComboBoxItem("ナマズ", 4); // Catfish
    ctx->genComboBoxItem("マス", 5); // Trout
    ctx->genComboBoxItem("ギル", 6); // Gill
    ctx->genComboBoxItem("小バス", 7); // Small Bass
    ctx->genComboBoxItem("小ドジョウ", 8); // Small Loach
    ctx->genComboBoxItem("小パイク", 9); // Small Pike
    ctx->genComboBoxItem("小ナマズ", 10); // Small Catfish
    ctx->genComboBoxItem("小マス", 11); // Small Trout
    ctx->endComboBox();
}
#endif

static int frog_nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        int jnt_no = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        dmg_rod_class* a_this = (dmg_rod_class*)model->getUserArea();

        if (a_this != NULL) {
            if (jnt_no == 1) {
                MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);
                cMtx_YrotM(*calc_mtx, -a_this->field_0xff2);
                MtxScale(1.5f + JREG_F(10), 1.0f + JREG_F(11), 1.3f + JREG_F(12), 1);
                model->setAnmMtx(jnt_no, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }
        }
    }

    return 1;
}

static int ws_nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        int jnt_no = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        dmg_rod_class* a_this = (dmg_rod_class*)model->getUserArea();

        if (a_this != NULL) {
            MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);
            cMtx_XrotM(*calc_mtx, a_this->field_0x101c[jnt_no - 1]);  // technically an OoB access, but this callback is only ever set for joints 1 and 2
            model->setAnmMtx(jnt_no, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }

    return 1;
}

static int Reel_CallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        int jnt_no = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        dmg_rod_class* a_this = (dmg_rod_class*)model->getUserArea();

        if (a_this != NULL) {
            if (jnt_no == 1 || jnt_no == 3) {
                MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);

                if (jnt_no == 1) {
                    cMtx_XrotM(*calc_mtx, -a_this->field_0x75c);
                } else {
                    cMtx_XrotM(*calc_mtx, (a_this->field_0x75c * 2));
                }

                model->setAnmMtx(jnt_no, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }
        }
    }

    return 1;
}

static int Worm_nodeCallBack(J3DJoint* i_joint, int param_1) {
    if (param_1 == 0) {
        int jnt_no = i_joint->getJntNo();
        J3DModel* model = j3dSys.getModel();
        dmg_rod_class* a_this = (dmg_rod_class*)model->getUserArea();

        if (a_this != NULL) {
            if (jnt_no >= 1 && jnt_no <= 9) {
                MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);
                cMtx_YrotM(*calc_mtx, a_this->field_0x103c[jnt_no - 1]);
                cMtx_ZrotM(*calc_mtx, a_this->field_0x104e[jnt_no - 1]);
                model->setAnmMtx(jnt_no, *calc_mtx);
                MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
            }
        }
    }

    return 1;
}

static int dmg_rod_Draw(dmg_rod_class* i_this) {
    int unused;
    fopAc_ac_c* actor = &i_this->actor;
    g_env_light.settingTevStruct(0, &actor->current.pos, &actor->tevStr);

    if (i_this->kind == MG_ROD_KIND_LURE) {
        J3DModel* model = i_this->lure_model[i_this->lure_type];

        g_env_light.setLightTevColorType_MAJI(model, &actor->tevStr);
        mDoExt_modelUpdateDL(model);
        g_env_light.setLightTevColorType_MAJI(i_this->ring_model, &actor->tevStr);
        mDoExt_modelUpdateDL(i_this->ring_model);

        if (i_this->lure_type != MG_LURE_FR) {
            for (int i = 0; i < 2; i++) {
                if (i == 1 || i_this->lure_type != MG_LURE_SP) {
                    g_env_light.setLightTevColorType_MAJI(i_this->hook_model[i], &actor->tevStr);
                    mDoExt_modelUpdateDL(i_this->hook_model[i]);
                }
            }
        }

        if (i_this->field_0x1148 != 0) {
            i_this->field_0x1148--;
        } else {
            for (int i = 0; i < 15; i++) {
                g_env_light.setLightTevColorType_MAJI(i_this->rod_uki_model[i], &actor->tevStr);
                mDoExt_modelUpdateDL(i_this->rod_uki_model[i]);
            }

            for (int i = 0; i < 6; i++) {
                g_env_light.setLightTevColorType_MAJI(i_this->unk_ring_model[i], &actor->tevStr);
                mDoExt_modelUpdateDL(i_this->unk_ring_model[i]);
            }
        }

        static GXColor l_color = {0xFF, 0xFF, 0x96, 0xFF};
        i_this->linemat.update(MG_ROD_LURE_LINE_LEN, l_color, &actor->tevStr);
        dComIfGd_set3DlineMat(&i_this->linemat);

        model = i_this->rod_modelMorf->getModel();
        g_env_light.setLightTevColorType_MAJI(model, &actor->tevStr);
        i_this->rod_modelMorf->entryDL();
    } else {
        if (dComIfGp_checkPlayerStatus0(0, 0x2000)) {
            fopAc_ac_c* player = dComIfGp_getPlayer(0);
            camera_class* camera = dComIfGp_getCamera(0);
            f32 dx = player->current.pos.x - camera->lookat.eye.x;
            f32 dz = player->current.pos.z - camera->lookat.eye.z;

            if ((SQUARE(dx) + SQUARE(dz)) < 5000.0f) {
                return 1;
            }
        }

        g_env_light.setLightTevColorType_MAJI(i_this->uki_model, &actor->tevStr);
        mDoExt_modelUpdateDL(i_this->uki_model);
        g_env_light.setLightTevColorType_MAJI(i_this->uki_saki_model, &actor->tevStr);
        mDoExt_modelUpdateDL(i_this->uki_saki_model);

        static GXColor l_color = {0xFF, 0xFF, 0x96, 0xFF};
        i_this->linemat.update(MG_ROD_UKI_LINE_LEN, l_color, &actor->tevStr);
        dComIfGd_set3DlineMat(&i_this->linemat);

        for (int i = 0; i < 15; i++) {
            g_env_light.setLightTevColorType_MAJI(i_this->rod_uki_model[i], &actor->tevStr);
            mDoExt_modelUpdateDL(i_this->rod_uki_model[i]);
        }

        if (i_this->action != ACTION_UKI_HIT && i_this->action != ACTION_UKI_CATCH) {
            g_env_light.setLightTevColorType_MAJI(i_this->hook_model[i_this->hook_kind], &actor->tevStr);
            mDoExt_modelUpdateDL(i_this->hook_model[i_this->hook_kind]);

            if (i_this->esa_kind != 0) {
                g_env_light.setLightTevColorType_MAJI(i_this->esa_model[i_this->esa_kind - 1], &actor->tevStr);

                if (i_this->esa_kind == 2) {
                    i_this->esa_model[i_this->esa_kind - 1]->setUserArea((uintptr_t)i_this);
                    for (u16 i = 0; i < i_this->esa_model[i_this->esa_kind - 1]->getModelData()->getJointNum(); i++) {
                        if (i >= 1) {
                            i_this->esa_model[i_this->esa_kind - 1]->getModelData()->getJointNodePointer(i)->setCallBack(Worm_nodeCallBack);
                        }
                    }
                }

                mDoExt_modelUpdateDL(i_this->esa_model[i_this->esa_kind - 1]);
            }
        } else if (i_this->action == ACTION_UKI_CATCH) {
            fopAc_ac_c* mgfish_a = fopAcM_SearchByID(i_this->mg_fish_id);
            mg_fish_class* mgfish = (mg_fish_class*)mgfish_a;

            if (mgfish->mCaughtType == MG_CATCH_SP || mgfish->mCaughtType >= MG_CATCH_ED) {
                g_env_light.setLightTevColorType_MAJI(i_this->hook_model[i_this->hook_kind], &actor->tevStr);
                mDoExt_modelUpdateDL(i_this->hook_model[i_this->hook_kind]);
            }
        }
    }

    return 1;
}

static void rod_control(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    cXyz* temp_r28;
    int i;

    cXyz sp98;
    cXyz sp8C;
    cXyz sp80;
    cXyz sp74;
    cXyz sp68;
    csXyz sp30;
    s16 spE, spC, spA, sp8;

    temp_r28 = i_this->mg_rod.field_0x0;

    if (i_this->kind == MG_ROD_KIND_LURE && (i_this->action == ACTION_LURE_ONBOAT || (i_this->action == ACTION_LURE_STANDBY && i_this->timers[4] != 0))) {
        daCanoe_c* boat = (daCanoe_c*)fopAcM_SearchByID(i_this->boat_actor_id);
        if (i_this->play_cam_mode >= 950 && i_this->play_cam_mode < 1000) {
            cMtx_YrotS(*calc_mtx, player->shape_angle.y);
            sp98.x = -50.0f + AREG_F(0);
            sp98.y = -20.0f + AREG_F(1);
            sp98.z = -70.0f + AREG_F(2);
            MtxPosition(&sp98, &sp8C);
            sp8C += player->current.pos;

            MtxTrans(sp8C.x, sp8C.y, sp8C.z, 0);
            cMtx_YrotM(*calc_mtx, (player->shape_angle.y + AREG_S(0)) - 30200);
            cMtx_XrotM(*calc_mtx, AREG_S(1) - 13000);
        } else {
            MTXCopy(boat->getModelMtx(), *calc_mtx);
            if (boat->checkTandem()) {
                MtxTrans(-33.0f + NREG_F(0), 30.0f + NREG_F(1), 115.0f + NREG_F(2), 1);
                cMtx_YrotM(*calc_mtx, (NREG_S(0) - 24000));
            } else {
                MtxTrans(-30.0f + NREG_F(0), 30.0f + NREG_F(1), 35.0f + NREG_F(2), 1);
                cMtx_YrotM(*calc_mtx, NREG_S(0) - 25000);
            }

            cMtx_XrotM(*calc_mtx, NREG_S(1) + 0x76C);
            cMtx_YrotM(*calc_mtx, JREG_S(3) + 0x20B6);
            cMtx_ZrotM(*calc_mtx, JREG_S(5) - 0x4000);
        }

        i_this->field_0x6c4 = boat->shape_angle.y + NREG_S(3);
    } else {
        i_this->field_0x6c4 = daAlink_getAlinkActorClass()->getFishingRodAngleY();
        if (i_this->kind == MG_ROD_KIND_LURE && daAlink_getAlinkActorClass()->checkFishingRodGrabLeft()) {
            MTXCopy(daAlink_getAlinkActorClass()->getLeftItemMatrix(), *calc_mtx);
            cMtx_YrotM(*calc_mtx, (s16)(JREG_S(3) + 0x20B6));
            cMtx_XrotM(*calc_mtx, (s16)(JREG_S(4) + 0x8000));
            cMtx_ZrotM(*calc_mtx, (s16)(JREG_S(5) - 0x4000));
        } else {
            MTXCopy(daAlink_getAlinkActorClass()->getRightItemMatrix(), *calc_mtx);
            if (i_this->kind == MG_ROD_KIND_LURE) {
                sp8 = 0;
                spA = i_this->field_0x6c4 - player->shape_angle.y;
                spA = 1000.0f + (spA * (0.2f + NREG_F(10)));

                if (i_this->field_0x14f8 < 0) {
                    sp8 = i_this->field_0x14f8 * (1.2f + NREG_F(11));
                }

                cMtx_XrotM(*calc_mtx, (JREG_S(4) + spA) - 2000);
                cMtx_ZrotM(*calc_mtx, JREG_S(5) + sp8);
            }
        }
    }

    if (i_this->kind == MG_ROD_KIND_LURE) {
        MtxTrans(JREG_F(4), 3.0f + JREG_F(5), JREG_F(6), 1);
        
        J3DModel* model = i_this->rod_modelMorf->getModel();
        model->setBaseTRMtx(*calc_mtx);

        MtxPush();
        i_this->rod_modelMorf->modelCalc();
        MtxPull();

        sp98.set(0.0f, 0.0f, 0.0f);
        MtxPosition(&sp98, &i_this->field_0x750);
        sp98.set(i_this->field_0x760, 5.0f + DREG_F(8), (-4.5f + DREG_F(9)) - 1.0f);
        MtxPosition(&sp98, &i_this->field_0x744);

        MTXCopy(model->getAnmMtx(2), *calc_mtx);
        cMtx_XrotM(*calc_mtx, -0x4000);
    } else {
        cMtx_YrotM(*calc_mtx, (s16)(YREG_S(0) - 17000));
        cMtx_XrotM(*calc_mtx, (s16)(YREG_S(1) + 0xA134));
        MtxTrans(KREG_F(3), -1.0f + KREG_F(4), -10.0f + KREG_F(5), 1);
    }

    sp98.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp98, temp_r28);
    MtxTrans(JREG_F(0), JREG_F(1), 16.0f * i_this->field_0x6a0, 1);

    sp98.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp98, &sp68);
    sp98 = sp68 - *temp_r28;

    sp30.y = (s16)cM_atan2s(sp98.x, sp98.z);
    sp30.x = -cM_atan2s(sp98.y, JMAFastSqrt(SQUARE(sp98.x) + SQUARE(sp98.z)));
    cMtx_YrotS(*calc_mtx, sp30.y);
    cMtx_XrotM(*calc_mtx, sp30.x);
    sp98.x = 0.0f;
    sp98.y = 0.0f;
    sp98.z = 300.0f + YREG_F(0);
    MtxPosition(&sp98, &sp80);
    sp98 = i_this->mg_line.pos[70] - sp68;

    if (i_this->kind == MG_ROD_KIND_UKI && i_this->action == ACTION_UKI_STANDBY && i_this->field_0x1508 < 0.1f) {
        cMtx_YrotS(*calc_mtx, i_this->field_0x6c4);
        cMtx_XrotM(*calc_mtx, 6000);
    } else {
        cMtx_YrotS(*calc_mtx, (s16)cM_atan2s(sp98.x, sp98.z));
        cMtx_XrotM(*calc_mtx, -cM_atan2s(sp98.y, JMAFastSqrt(SQUARE(sp98.x) + SQUARE(sp98.z))));
    }

    sp98.x = 0.0f;
    sp98.y = 0.0f;
    sp98.z = i_this->field_0x6f8 * (3.0f + YREG_F(1));
    MtxPosition(&sp98, &sp74);

    sp98.x = 0.0f;
    sp98.y = 0.0f;
    sp98.z = i_this->field_0x6a0 * i_this->field_0x6a4;

    temp_r28++;
    for (i = 1; i < 16; i++, temp_r28++) {
        static f32 rod_p[] = {
            0.0f,
            0.002915448f,
            0.023323584f,
            0.045553874f,
            0.078717098f,
            0.124999836f,
            0.15374433f,
            0.18658867f,
            0.22380619f,
            0.2656702f,
            0.31245404f,
            0.36443099f,
            0.48505768f,
            0.62973678f,
            0.80065495f,
            0.99999869f,
        };

        f32 temp_f29 = rod_p[i];
        f32 temp_f31 = sp80.x + ((temp_r28[0].x - temp_r28[-1].x) + (sp74.x * temp_f29));
        f32 temp_f26 = sp80.y + ((temp_r28[0].y - temp_r28[-1].y) + (sp74.y * temp_f29));
        f32 temp_f30 = sp80.z + ((temp_r28[0].z - temp_r28[-1].z) + (sp74.z * temp_f29));

        spC = (s16)cM_atan2s(temp_f31, temp_f30);
        spE = -cM_atan2s(temp_f26, JMAFastSqrt(SQUARE(temp_f31) + SQUARE(temp_f30)));
        cMtx_YrotS(*calc_mtx, spC);
        cMtx_XrotM(*calc_mtx, spE);
        MtxPosition(&sp98, &sp8C);

        temp_r28[0].x = temp_r28[-1].x + sp8C.x;
        temp_r28[0].y = temp_r28[-1].y + sp8C.y;
        temp_r28[0].z = temp_r28[-1].z + sp8C.z;

        if (i == 15) {
            i_this->field_0x6b8 = i_this->field_0x6ac;
            i_this->field_0x6ac = *temp_r28;
        } else if (i == 1) {
            i_this->field_0x6d4 = i_this->field_0x6c8;
            i_this->field_0x6c8 = *temp_r28;
        }
    }

    if (i_this->kind == MG_ROD_KIND_UKI) {
        temp_r28 = i_this->mg_rod.field_0x0;
        for (i = 0; i < 15; i++, temp_r28++) {
            sp98 = temp_r28[1] - temp_r28[0];

            mDoMtx_stack_c::transS(temp_r28->x, temp_r28->y, temp_r28->z);
            mDoMtx_stack_c::YrotM(cM_atan2s(sp98.x, sp98.z));
            mDoMtx_stack_c::XrotM(NREG_S(6) + -cM_atan2s(sp98.y, JMAFastSqrt(SQUARE(sp98.x) + SQUARE(sp98.z))));

            static u8 rod_wd[] = {
                15,
                15,
                15,
                13,
                11,
                10,
                9,
                8,
                7,
                6,
                5,
                4,
                3,
                2,
                2,
            };

            f32 temp_f28 = rod_wd[i] * (0.05166f + NREG_F(3));
            f32 temp_f24 = (0.073f + NREG_F(1)) * sp98.abs();
            mDoMtx_stack_c::scaleM(temp_f28, temp_f28, temp_f24);

            if (i == 0) {
                mDoMtx_stack_c::scaleM(1.0f, 1.0f, 0.5f + NREG_F(4));
                mDoMtx_stack_c::transM(0.0f, 0.0f, -5.5f + NREG_F(5));
            }

            i_this->rod_uki_model[i]->setBaseTRMtx(mDoMtx_stack_c::get());
        }
    } else {
        temp_r28 = i_this->mg_rod.field_0x0;
        for (i = 0; i < 15; i++, temp_r28++) {
            sp98 = temp_r28[1] - temp_r28[0];

            mDoMtx_stack_c::transS(temp_r28->x, temp_r28->y, temp_r28->z);
            mDoMtx_stack_c::YrotM(cM_atan2s(sp98.x, sp98.z));
            mDoMtx_stack_c::XrotM(NREG_S(6) + -cM_atan2s(sp98.y, JMAFastSqrt(SQUARE(sp98.x) + SQUARE(sp98.z))));

            static u8 rod_wd[] = {
                10,
                9,
                9,
                8,
                8,
                7,
                7,
                6,
                6,
                5,
                5,
                4,
                4,
                3,
                3,
            };

            f32 temp_f28 = rod_wd[i] * (0.05166f + NREG_F(3));
            f32 temp_f27 = (0.073f + NREG_F(1)) * sp98.abs();
            mDoMtx_stack_c::scaleM(temp_f28, temp_f28, temp_f27);

            i_this->rod_uki_model[i]->setBaseTRMtx(mDoMtx_stack_c::get());
        }
    }
}

static void rod_main(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    rod_control(i_this);
}

static npc_henna_class* henna;

/* 804BBBD4 0001+00 data_804BBBD4 None */
static u8 data_804BBBD4;
static u8 l_HIOInit;

static dmg_rod_HIO_c l_HIO;

static cXyz old_line_pos[100];

static void line_control1(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    int i;

    cXyz work;
    cXyz sp40;
    Vec sp34;
    dBgS_GndChk sp6C;
    cM3dGPla sp58;
    cXyz sp28;

    if (i_this->field_0x6e0 > 0.1f) {
        work = i_this->mg_rod.field_0x0[14] - i_this->mg_rod.field_0x0[12];
        cMtx_YrotS(*calc_mtx, cM_atan2s(work.x, work.z));
        cMtx_XrotM(*calc_mtx, -cM_atan2s(work.y, JMAFastSqrt(SQUARE(work.x) + SQUARE(work.z))));

        work.x = 0.0f;
        work.y = 0.0f;
        work.z = i_this->field_0x6e0 * (500.0f + NREG_F(7));
        MtxPosition(&work, &sp28);
        cLib_addCalc0(&i_this->field_0x6e0, 1.0f, 0.2f + NREG_F(8));
    } else {
        sp28.set(0.0f, 0.0f, 0.0f);
    }

    if (i_this->action == ACTION_LURE_STANDBY && i_this->timers[0] != 0) {
        cMtx_YrotS(*calc_mtx, i_this->field_0x6c4);
        cMtx_XrotM(*calc_mtx, (NREG_S(7) - 21500));

        work.x = 0.0f;
        work.y = 0.0f;
        work.z = 1000.0f + NREG_F(17);
        MtxPosition(&work, &sp28);
    }

    cXyz* temp_r30 = i_this->mg_line.pos;
    *temp_r30 = i_this->field_0x6ac;

    work.x = 0.0f;
    work.y = 0.0f;
    work.z = i_this->field_0xf5c;

    temp_r30++;

    BOOL sp10 = 1;
    if (
        i_this->kind == MG_ROD_KIND_LURE &&
        (i_this->action == ACTION_LURE_ACTION || i_this->action == ACTION_LURE_CAST || i_this->action == ACTION_LURE_HIT ||
        i_this->action == ACTION_LURE_ONBOAT || i_this->action == ACTION_LURE_STANDBY)
    ) {
        sp10 = 0;
    }

    f32 var_f30, var_f27, var_f26, temp_f29, temp_f28, var_f31;
    var_f31 = i_this->field_0xf64;

    for (i = 1; i < 100; i++, temp_r30++) {
        temp_f29 = sp28.x + (temp_r30[0].x - temp_r30[-1].x);
        temp_f28 = sp28.z + (temp_r30[0].z - temp_r30[-1].z);

        if (sp10 != 0) {
            sp34.x = temp_r30->x;
            sp34.y = 50.0f + temp_r30->y;
            sp34.z = temp_r30->z;
            sp6C.SetPos(&sp34);

            var_f30 = KREG_F(7) + (2.0f + dComIfG_Bgsp().GroundCross(&sp6C));
            if (var_f30 < i_this->field_0x590) {
                var_f30 = i_this->field_0x590;
            }

            var_f27 = sp28.y + (temp_r30[0].y + var_f31);
            if (var_f27 < var_f30) {
                var_f27 = var_f30;
            }

            var_f26 = var_f27 - temp_r30[-1].y;
        } else {
            var_f31 = (0.5f + TREG_F(5)) * ((2.0f + i_this->field_0x590) - temp_r30[0].y);
            if (var_f31 < i_this->field_0xf64) {
                var_f31 = i_this->field_0xf64;
            } else if (var_f31 > 0.25f) {
                var_f31 = 0.25f;
            }

            if (i_this->lure_type == MG_LURE_SP && var_f31 > 0.0f) {
                var_f31 = JREG_F(11);
            }

            var_f26 = sp28.y + (var_f31 + (temp_r30[0].y - temp_r30[-1].y));
        }

        s16 spA, sp8;
        sp8 = (s16)cM_atan2s(temp_f29, temp_f28);
        spA = -cM_atan2s(var_f26, JMAFastSqrt(SQUARE(temp_f29) + SQUARE(temp_f28)));

        cMtx_YrotS(*calc_mtx, sp8);
        cMtx_XrotM(*calc_mtx, spA);
        MtxPosition(&work, &sp40);

        old_line_pos[i] = *temp_r30;

        temp_r30[0].x = temp_r30[-1].x + sp40.x;
        temp_r30[0].y = temp_r30[-1].y + sp40.y;
        temp_r30[0].z = temp_r30[-1].z + sp40.z;

        if (i == 99) {
            i_this->field_0x764 = *temp_r30;
        }
    }
}

static void line_control2(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    int i;

    cXyz sp34;
    cXyz sp28;
    Vec sp1C;
    dBgS_GndChk spD8;
    cM3dGPla sp54;

    i_this->mg_line.pos[99] = i_this->field_0x1010;

    cXyz* var_r28 = &i_this->mg_line.pos[98];
    sp34.x = 0.0f;
    sp34.y = 0.0f;
    sp34.z = i_this->field_0xf5c;

    for (i = 98; i >= 1; i--, var_r28--) {
        f32 temp_f30, sp14, temp_f29, var_f31, var_f28;
        temp_f30 = (var_r28[0].x - var_r28[1].x) + i_this->field_0xc20[i + TREG_S(9)];
        temp_f29 = (var_r28[0].z - var_r28[1].z) + i_this->field_0xdb0[i + TREG_S(9)];

        sp1C.x = var_r28->x;
        sp1C.y = 50.0f + var_r28->y;
        sp1C.z = var_r28->z;
        spD8.SetPos(&sp1C);

        if (dComIfG_Bgsp().GetTriPla(spD8, &sp54) && cBgW_CheckBGround(sp54.mNormal.y)) {
            var_f31 = KREG_F(7) + (2.0f + dComIfG_Bgsp().GroundCross(&spD8));
        } else if (i_this->lure_type == MG_LURE_SP) {
            var_f31 = i_this->field_0x590 - 1000.0f;
        } else {
            var_f31 = i_this->field_0x590;
        }

        var_f28 = var_r28[0].y;
        if (var_f28 < var_f31) {
            var_f28 = var_f31;
        }

        sp14 = var_f28 - var_r28[1].y;

        s16 spA, sp8;
        sp8 = (s16)cM_atan2s(temp_f30, temp_f29);
        spA = -cM_atan2s(sp14, JMAFastSqrt(SQUARE(temp_f30) + SQUARE(temp_f29)));

        cMtx_YrotS(*calc_mtx, sp8);
        cMtx_XrotM(*calc_mtx, spA);
        MtxPosition(&sp34, &sp28);
        var_r28[0].x = var_r28[1].x + sp28.x;
        var_r28[0].y = var_r28[1].y + sp28.y;
        var_r28[0].z = var_r28[1].z + sp28.z;
    }

    if (i_this->lure_type != MG_LURE_SP && i_this->action == ACTION_LURE_ACTION && i_this->field_0x100d != 0) {
        dBgS_LinChk sp68;
        cM3dGPla sp40;
        sp68.SetRope();

        for (i = 1; i < 99; i++) {
            sp68.Set(&old_line_pos[i], &i_this->mg_line.pos[i], actor);
            if (dComIfG_Bgsp().LineCross(&sp68)) {
                dComIfG_Bgsp().GetTriPla(sp68, &sp40);

                sp34.x = sp40.GetNP()->x;
                sp34.z = sp40.GetNP()->z;
                cMtx_YrotS(*calc_mtx, cM_atan2s(sp34.x, sp34.z));

                sp34.x = 0.0f;
                sp34.y = 0.0f;
                sp34.z = 5.0f + YREG_F(18);
                MtxPosition(&sp34, &sp28);

                f32 temp_f27 = (0.1f * sp28.x) + sp68.GetCross().x;
                f32 temp_f26 = (0.1f * sp28.z) + sp68.GetCross().z;
                f32 temp_f25 = i_this->mg_line.pos[i - 1].x - temp_f27;
                f32 temp_f24 = i_this->mg_line.pos[i - 1].z - temp_f26;
                if (JMAFastSqrt(SQUARE(temp_f25) + SQUARE(temp_f24)) < 100.0f + nREG_F(6)) {
                    i_this->mg_line.pos[i].x = temp_f27;
                    i_this->mg_line.pos[i].z = temp_f26;
                }

                i_this->field_0xc20[i] = sp28.x;
                i_this->field_0xdb0[i] = sp28.z;

                for (int sp10 = i - 3; sp10 < i + 3; sp10++) {
                    if (sp10 >= 1 && sp10 < 99) {
                        i_this->field_0xc20[sp10] = sp28.x;
                        i_this->field_0xdb0[sp10] = sp28.z;
                    }
                }
            } else {
                if (fabsf(i_this->field_0xc20[i]) > 0.05f || fabsf(i_this->field_0xdb0[i]) > 0.05f) {
                    i_this->field_0xc20[i] *= 0.95f;
                    i_this->field_0xdb0[i] *= 0.95f;
                    actor->current.pos.x += (0.02f + TREG_F(17)) * i_this->field_0xc20[i];
                    actor->current.pos.z += (0.02f + TREG_F(17)) * i_this->field_0xdb0[i];
                    cLib_addCalcAngleS2(&i_this->field_0xffe, 0, 2, 20);

                    cMtx_YrotS(*calc_mtx, actor->current.angle.y);
                    sp34.x = 0.0f;
                    sp34.y = 0.0f;
                    sp34.z = 0.1f + TREG_F(16);
                    MtxPosition(&sp34, &sp28);
                    actor->current.pos += sp28;
                } else {
                    i_this->field_0xc20[i] = i_this->field_0xdb0[i] = 0.0f;
                }
            }
        }
    }
}

static void line_control1_u(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    int i;

    cXyz work;
    cXyz offset;
    Vec gnd_chk_pos;
    dBgS_GndChk gnd_chk;
    cXyz sp74(0.0f, 0.0f, 0.0f);

    cMtx_YrotS(*calc_mtx, player->shape_angle.y);
    work.x = 0.0f;
    work.y = i_this->field_0x6e0 * (20.0f + BREG_F(8));
    work.z = i_this->field_0x6e0 * (30.0f + BREG_F(9));
    MtxPosition(&work, &sp74);
    sp74.y += i_this->field_0x6e4;

    cLib_addCalc0(&i_this->field_0x6e4, 1.0f, 3.0f + ZREG_F(17));

    cXyz* temp_r28 = i_this->mg_line.pos;
    *temp_r28 = i_this->field_0x6ac;

    f32 fVar1, fVar2, fVar3, fVar4;
    f32 fVar5, fVar6;

    if (i_this->action == ACTION_UKI_CATCH) {
        fopAc_ac_c* mg_fish = fopAcM_SearchByID(i_this->mg_fish_id);
        f32 fVar7 = i_this->play_cam_timer * 2;
        if (fVar7 > 300.0f) {
            fVar7 = 300.0f;
        }
        temp_r28[0].y = 700.0f + mg_fish->current.pos.y + fVar7;
    }

    work.x = 0.0f;
    work.y = 0.0f;
    work.z = i_this->field_0xf5c;

    temp_r28++;

    f32 fVar8;
    f32 fVar9 = i_this->field_0xf64;
    f32 fVar10 = i_this->field_0xf68;
    if (i_this->hook_kind == 1) {
        fVar9 *= 1.5f;
        fVar10 *= 1.5f;
    }

    cXyz sp68;
    f32 fVar11;
    dKyw_get_AllWind_vec(&i_this->hook_pos, &sp68, &fVar11);

    if (fVar11 < 0.4f) {
        fVar11 = 0.0f;
    } else {
        fVar11 -= 0.4f;
        if (fVar11 > 1.0f) {
            fVar11 = 1.0f;
        }
    }

    f32 fVar12, fVar13, fVar14;
    if (fVar11 > 0.01f) {
        fVar11 *= 40.0f;
        fVar12 = sp68.x * fVar11;
        fVar13 = sp68.z * fVar11;

        if ((i_this->field_0x578 & 15) == 0) {
            i_this->field_0x109e = 2000.0f + (60.0f * fVar11) + cM_rndF(60.0f * fVar11);
        }

        i_this->field_0x109c += i_this->field_0x109e;
    } else {
        sp68.x = sp68.y = sp68.z = 0.0f;
    }

    cXyz speed;
    int iVar1 = 0;
    fopAcM_getWaterStream(&actor->current.pos, i_this->acchcir, &speed, &iVar1, 0);

    for (i = 1; i < 100; i++, temp_r28++) {
        if (fVar11 > 0.01f && temp_r28[0].y > (20.0f + i_this->field_0x590)) {
            fVar14 = 0.1f + (0.003f * i);
            sp68.x = fVar12 * (1.0f + (fVar14 * cM_ssin(i_this->field_0x109c - (i * 600))));
            sp68.y = JREG_F(7) * (fVar11 * (fVar14 * cM_ssin(i_this->field_0x109c - (i * 650))));
            sp68.z = fVar13 * (1.0f + (fVar14 * cM_ssin(i_this->field_0x109c - (i * 630))));
        } else {
            if (iVar1 != 0) {
                sp68.x = iVar1 * (0.5f * speed.x);
                sp68.z = iVar1 * (0.5f * speed.z);
            } else {
                sp68.x = sp68.z = 0.0f;
            }
            sp68.y = 0.0f;
        }

        fVar1 = sp68.x + (sp74.x + (temp_r28[0].x - temp_r28[-1].x));
        fVar3 = sp68.z + (sp74.z + (temp_r28[0].z - temp_r28[-1].z));

        gnd_chk_pos.x = temp_r28[0].x;
        gnd_chk_pos.y = 50.0f + temp_r28[0].y;
        gnd_chk_pos.z = temp_r28[0].z;
        gnd_chk.SetPos(&gnd_chk_pos);

        fVar5 = KREG_F(7) + (2.0f + dComIfG_Bgsp().GroundCross(&gnd_chk));
        if (i >= 50 && i <= 90) {
            f32 var_f31 = (0.07f + KREG_F(9)) * (20.0f - fabsf(i - 70.0f));
            if (var_f31 > 1.0f) {
                var_f31 = 1.0f;
            }
            var_f31 *= var_f31;
            fVar8 = AREG_F(14) + (5.0f + (i_this->field_0x590 + (i_this->field_0xf74 * var_f31)));
        } else {
            fVar8 = 5.0f + i_this->field_0x590 + AREG_F(14);
        }

        fVar6 = ((0.5f + TREG_F(5)) * (fVar8 - temp_r28[0].y)) - 0.25f;
        if (fVar6 < fVar9) {
            fVar6 = fVar9;
        } else if (fVar6 > 2.0f) {
            fVar6 = 2.0f;
        }

        if (i >= (TREG_S(9) + BREG_S(5) + 71) && temp_r28[0].y < fVar8) {
            f32 temp_f31 = (0.02f + VREG_F(3)) * (AREG_S(7) + (i - (TREG_S(9) + BREG_S(5) + 61)));
            fVar6 = fVar10 * (temp_f31 * temp_f31);
        }

        fVar4 = sp68.y + (sp74.y + (temp_r28[0].y + fVar6));

        if ((fVar4 <= fVar8 || fVar4 <= fVar5) && (i == (TREG_S(9) + BREG_S(6) + 72))) {
            cLib_addCalc0(&i_this->field_0x6e0, 1.0f, 0.08f + BREG_F(19));
        }

        if (fVar4 <= fVar5) {
            fVar4 = fVar5;
            if (i == 99 && fVar4 > fVar8) {
                i_this->field_0x102e = 1;
            }
        }

        fVar2 = (fVar4 - temp_r28[-1].y);

        s16 x_rot, y_rot;
        y_rot = (s16)cM_atan2s(fVar1, fVar3);
        x_rot = -cM_atan2s(fVar2, JMAFastSqrt(SQUARE(fVar1) + SQUARE(fVar3)));

        cMtx_YrotS(*calc_mtx, y_rot);
        cMtx_XrotM(*calc_mtx, x_rot);
        MtxPosition(&work, &offset);
        temp_r28[0].x = temp_r28[-1].x + offset.x;
        temp_r28[0].y = temp_r28[-1].y + offset.y;
        temp_r28[0].z = temp_r28[-1].z + offset.z;

        if (i == 99) {
            i_this->field_0x764 = *temp_r28;
        }
    }
}

static void line_control2_u(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    int i;
    cXyz work;
    cXyz offset;
    Vec pos;
    s16 x_rot, y_rot;
    dBgS_GndChk gnd_chk;

    i_this->mg_line.pos[99] = actor->current.pos;
    cXyz* linePosP = &i_this->mg_line.pos[98];

    work.x = 0.0f;
    work.y = 0.0f;
    work.z = i_this->field_0xf5c;

    for (i = 98; i >= 1; i--, linePosP--) {
        f32 x_delta = linePosP[0].x - linePosP[1].x;
        f32 y_delta;
        f32 z_delta = linePosP[0].z - linePosP[1].z;

        if (i_this->action == ACTION_UKI_STANDBY) {
            pos.x = linePosP->x;
            pos.y = 50.0f + linePosP->y;
            pos.z = linePosP->z;
            gnd_chk.SetPos(&pos);

            f32 gnd_cross_offset = KREG_F(7) + (2.0f + dComIfG_Bgsp().GroundCross(&gnd_chk));
            f32 y = linePosP->y;
            if (y < gnd_cross_offset) {
                y = gnd_cross_offset;
            }
            y_delta = y - linePosP[1].y;
        } else {
            y_delta = linePosP[0].y - linePosP[1].y;
        }

        y_rot = (s16)cM_atan2s(x_delta, z_delta);
        x_rot = -cM_atan2s(y_delta, JMAFastSqrt(SQUARE(x_delta) + SQUARE(z_delta)));

        cMtx_YrotS(*calc_mtx, y_rot);
        cMtx_XrotM(*calc_mtx, x_rot);
        MtxPosition(&work, &offset);
        linePosP[0].x = linePosP[1].x + offset.x;
        linePosP[0].y = linePosP[1].y + offset.y;
        linePosP[0].z = linePosP[1].z + offset.z;
    }
}

static void line_main(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;

    if (
        i_this->kind == MG_ROD_KIND_LURE && i_this->action != ACTION_LURE_CATCH && i_this->action == ACTION_LURE_HIT &&
        (i_this->reel_btn_flags != 0 || mDoCPd_c::getHoldX(PAD_1))
    ) {
        cLib_addCalc2(&i_this->field_0xf5c, 0.5f, 1.0f, 0.1f + YREG_F(11));
    }

    if (i_this->kind == MG_ROD_KIND_LURE) {
        line_control1(i_this);
        i_this->field_0x760 = 2.0f * cM_ssin(i_this->field_0x75e);
    } else {
        line_control1_u(i_this);
    }
}

static f32 depth_check(dmg_rod_class* i_this, cXyz* param_1) {
    dBgS_GndChk gnd_chk;
    Vec pos;

    pos.x = param_1->x;
    pos.y = 50.0f + i_this->field_0x590;
    pos.z = param_1->z;
    gnd_chk.SetPos(&pos);

    return i_this->field_0x590 - dComIfG_Bgsp().GroundCross(&gnd_chk);
}

static void sibuki_set(dmg_rod_class* i_this, f32 i_size, cXyz* i_pos, BOOL param_3) {
    fopAc_ac_c* actor = &i_this->actor;

    if (i_this->field_0x1165 == 0 && (param_3 || i_this->field_0x590 - i_pos->y < 20.0f) && depth_check(i_this, i_pos) > 0.0f) {
        cXyz pos(*i_pos);
        pos.y = i_this->field_0x590;

        static cXyz sc(i_size, i_size, i_size);

        for (int i = 0; i < 4; i++) {
            static u16 w_eff_id[] = {ID_ZI_J_DOWNWTRA_A, ID_ZI_J_DOWNWTRA_B, ID_ZI_J_DOWNWTRA_C, ID_ZI_J_DOWNWTRA_D};
            i_this->sibuki_eff[i] = dComIfGp_particle_set(i_this->sibuki_eff[i], w_eff_id[i], &pos, &actor->tevStr, NULL, &sc, 0xFF, NULL, -1, NULL, NULL, NULL);
        }
    }
}

static void* s_boat_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_CANOE) {
        cXyz spC = ((fopAc_ac_c*)i_actor)->current.pos - ((fopAc_ac_c*)i_data)->current.pos;
        return i_actor;
    }

    return NULL;
}

static void lure_onboat(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz pos_delta;
    cXyz sp14;

    i_this->field_0x100d = 0;

    fopAc_ac_c* actor_p = fopAcM_SearchByID(i_this->boat_actor_id);
    if (actor_p == NULL) {
        fopAcM_delete(actor);
        return;
    }

    if (mDoCPd_c::getTrigB(PAD_1)) {
        i_this->timers[1] = 30;
        i_this->field_0x146c = 30;
    }

    if (i_this->field_0x146c == 0 && !dComIfGp_checkPlayerStatus0(0, 0x2000)) {
        dComIfGp_setAStatusForce(79, 2);
    }

    i_this->field_0xf64 = -30.0f + KREG_F(8);
    i_this->field_0xf5c = 0.125f + BREG_F(2);
    i_this->field_0x6f8 = 15.0f + BREG_F(4);
    actor->current.pos = i_this->field_0x764;

    pos_delta = i_this->field_0x6ac - actor->current.pos;
    actor->current.angle.x = -cM_atan2s(pos_delta.y, pos_delta.z);
    actor->current.angle.y = (s16)cM_atan2s(pos_delta.x, JMAFastSqrt(SQUARE(pos_delta.y) + SQUARE(pos_delta.z)));

    if (daAlink_getAlinkActorClass()->checkFishingRodGrab(actor)) {
        i_this->action = ACTION_LURE_STANDBY;
        i_this->timers[4] = WREG_S(2) + 8;
        i_this->field_0xf5c = 0.5f;
        i_this->play_cam_mode = 1;
        i_this->timers[0] = 0;
        i_this->timers[1] = 10;
        i_this->field_0x14f8 = 0;

        camera_class* camera = dComIfGp_getCamera(0);
        f32 x_delta = camera->lookat.center.x - camera->lookat.eye.x;
        f32 z_delta = camera->lookat.center.z - camera->lookat.eye.z;
        i_this->field_0x1418 = cM_atan2s(x_delta, z_delta);

        daAlink_getAlinkActorClass()->setCanoeFishingWaitAngle(i_this->field_0x1418);
        daAlink_getAlinkActorClass()->seStartOnlyReverb(Z2SE_AL_ROD_TAKEOUT);
    }
}

static int lure_standby(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    cXyz work;
    cXyz offset;

    int sp14 = 0;
    int sp10 = 0;
    i_this->field_0x10a6 = 0;
    i_this->field_0x100d = 0;

    if (daAlink_getAlinkActorClass()->checkCanoeFishingRodGrabOrPut()) {
        i_this->field_0x146c = 5;
    }

    f32 var_f30 = 0.3f + BREG_F(5);
    if (i_this->timers[4] != 0) {
        var_f30 = 0.275f;
    }
    cLib_addCalc2(&i_this->field_0xf5c, var_f30, 1.0f, 1.0f + BREG_F(3));

    i_this->field_0xf64 = -30.0f + KREG_F(8);
    actor->current.pos = i_this->field_0x764;

    work = i_this->field_0x6ac - actor->current.pos;
    actor->current.angle.x = -cM_atan2s(work.y, work.z);
    actor->current.angle.y = (s16)cM_atan2s(work.x, JMAFastSqrt(SQUARE(work.y) + SQUARE(work.z)));

    if (i_this->timers[1] != 0) {
        i_this->field_0x14f8 = -7000;
        daAlink_getAlinkActorClass()->setFishingArnmAngle(i_this->field_0x14f8);
        return sp14;
    }

    if (data_804BBBD4 != 0) {
        dComIfGp_setDoStatusForce(4, 0);
    } else {
        dComIfGp_setDoStatusForce(42, 0);
    }

    i_this->rod_stick_x = mDoCPd_c::getStickX3D(PAD_1) * mDoCPd_c::getStickX3D(PAD_1);
    if (mDoCPd_c::getStickX3D(PAD_1) < 0.0f) {
        i_this->rod_stick_x *= -1.0f;
    }

    if (i_this->rod_stick_x > 1.0f) {
        i_this->rod_stick_x = 1.0f;
    } else if (i_this->rod_stick_x < -1.0f) {
        i_this->rod_stick_x = -1.0f;
    }

    s16 sp8 = (i_this->rod_substick_y * (-14000.0f + JREG_F(18))) - 7000.0f;
    if (sp8 > 7000) {
        sp8 = 7000;
    } else if (sp8 < -7000) {
        sp8 = -7000;
    }

    cLib_addCalcAngleS2(&i_this->field_0x14f8, sp8, 2, (JREG_S(9) + 4000));

    if (i_this->prev_rod_substick_y < 0.0f) {
        if ((i_this->rod_substick_y - i_this->prev_rod_substick_y) >= 0.5f) {
            sp10 = 1;
            i_this->cast_power = fabsf(i_this->prev_rod_substick_y);
            i_this->cast_power *= i_this->cast_power;
            if (i_this->cast_power < 0.6f) {
                i_this->cast_power = 0.6f;
            }
        }
    }

    daAlink_getAlinkActorClass()->setFishingArnmAngle(i_this->field_0x14f8);
    work = actor->current.pos - actor->old.pos;
    
    f32 temp_f29 = (0.005f + NREG_F(9)) * work.abs();
    if (temp_f29 > (0.4f + NREG_F(8))) {
        i_this->field_0x6e0 = temp_f29;
        if (i_this->field_0x6e0 > 1.0f) {
            i_this->field_0x6e0 = 1.0f;
        }
    }

    if (i_this->timers[0] != 0) {
        i_this->field_0x6e0 = 1.0f + XREG_F(11);

        if (i_this->timers[0] == 3) {
            dComIfGp_getVibration().StartShock(3, 1, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (i_this->field_0x6e8 != 0) {
            cMtx_YrotS(*calc_mtx, i_this->field_0x6c4);

            if (i_this->field_0x6e8 == 1) {
                work.x = -100.0f + hREG_F(1);
            } else {
                work.x = 100.0f + hREG_F(6);
            }

            work.y = hREG_F(2);
            work.z = hREG_F(3);
            MtxPosition(&work, &offset);
            actor->current.pos = offset + player->eyePos;
            sp14 = 1;
        }

        if (i_this->timers[0] == 1) {
            daAlink_getAlinkActorClass()->setCanoeCast();
            i_this->action = ACTION_LURE_CAST;
            i_this->field_0x10a8 = 0;
            i_this->play_cam_timer = 0;

            actor->current.angle.y = i_this->field_0x6c4 + NREG_S(6) + 0x7E0C;
            OS_REPORT(" CAST POWER %f\n", i_this->cast_power);

            f32 var_f31 = i_this->cast_power;
            if (var_f31 > 1.0f) {
                var_f31 = 1.0f;
            }
            actor->speedF = (-40.0f + JREG_F(13)) * var_f31;

            cMtx_YrotS(*calc_mtx, actor->current.angle.y);

            if (i_this->field_0x6e8 != 0) {
                actor->speed.y = 25.0f + JREG_F(14);
                work.x = 0.0f;
                work.y = WREG_F(15);
                work.z = WREG_F(16);

                if (i_this->field_0x6e8 == 1) {
                    i_this->field_0x14fa = -5000;
                } else {
                    i_this->field_0x14fa = 5000;
                }
            } else {
                actor->speed.y = 35.0f + JREG_F(14);
                work.x = 0.0f;
                work.y = 50.0f + WREG_F(5);
                work.z = -50.0f + WREG_F(6);
                i_this->field_0x14fa = 0;
            }

            MtxPosition(&work, &offset);
            actor->current.pos += offset;

            i_this->field_0x1410 = 0.05f + ZREG_F(11);
            i_this->field_0xf64 = KREG_F(8);
            i_this->field_0x14f8 = XREG_S(8) - 5000;
            i_this->field_0x6e0 = i_this->field_0x594 = 0.0f;
        }
    } else if (sp10 != 0) {
        if (sp10 == 1) {
            i_this->timers[0] = XREG_S(7) + 5;
        } else {
            i_this->timers[0] = XREG_S(7) + 4;
        }

        daAlink_getAlinkActorClass()->seStartOnlyReverb(Z2SE_AL_ROD_SWING_LURE);
        daAlink_getAlinkActorClass()->seStartOnlyReverb(Z2SE_AL_REEL_ROLL_THROW);
        i_this->field_0x1514 = 30;
    } 
    #if VERSION != VERSION_SHIELD_DEBUG
    else if (i_this->field_0x14f8 < 5000) {
        dComIfGp_setCStickStatusForce(80, 2, 0);
    }
    #endif

    i_this->field_0x6f8 = (500.0f + NREG_F(19)) * i_this->field_0x6e0;

    if (!daAlink_getAlinkActorClass()->checkFishingRodGrab(actor)) {
        i_this->action = ACTION_LURE_ONBOAT;
        i_this->play_cam_mode = 0;
        
        camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
        camera->mCamera.Reset(i_this->play_cam_center, i_this->play_cam_eye, i_this->play_cam_fovy, 0);
        camera->mCamera.Start();
        camera->mCamera.SetTrimSize(0);

        dMw_onMenuRing();
        daAlink_getAlinkActorClass()->seStartOnlyReverb(Z2SE_AL_ROD_TAKEOUT);
        i_this->timers[1] = 30;
        i_this->field_0x146c = 5;
    }

    return sp14;
}

static void lure_bound_se_set(dmg_rod_class* i_this) {
    if (i_this->lure_type != MG_LURE_FR) {
        if (i_this->lure_type == MG_LURE_SP) {
            i_this->sound.startCreatureSound(Z2SE_AL_LURE_BOUND, 0, -1);
        } else {
            i_this->sound.startCreatureSound(Z2SE_AL_LURE_BOUND_WOOD, 0, -1);
        }
    }
}

static void lure_cast(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    cXyz sp4C;
    cXyz sp40;

    if (!daAlink_getAlinkActorClass()->checkFishingRodGrab(actor)) {
        i_this->action = ACTION_LURE_ONBOAT;
        i_this->play_cam_mode = 0;
        
        camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
        camera->mCamera.Reset(i_this->play_cam_center, i_this->play_cam_eye, i_this->play_cam_fovy, 0);
        camera->mCamera.Start();
        camera->mCamera.SetTrimSize(0);

        dMw_onMenuRing();
        return;
    }

    cLib_addCalcAngleS2(&i_this->field_0x14f8, (VREG_S(5) - 7000), 8, (VREG_S(6) + 200));
    daAlink_getAlinkActorClass()->setFishingArnmAngle(i_this->field_0x14f8);

    sp4C = actor->current.pos - i_this->field_0x6ac;

    if (i_this->reel_btn_flags != 0) {
        actor->speedF *= 0.95f + VREG_F(11);
        sp40.x = 50.0f + VREG_F(12);
        sp40.y = (0.0105f + TREG_F(10)) * sp4C.abs();
        i_this->field_0x75c += (s16)0x1100;
        i_this->field_0x75e += (s16)0x880;
    } else {
        sp40.x = 0.0f;
        sp40.y = (0.011f + TREG_F(11)) * sp4C.abs();
        i_this->field_0x75c += (s16)0x2200;
        i_this->field_0x75e += (s16)0x1100;
    }

    cLib_addCalc2(&i_this->field_0x6f8, sp40.x, 0.1f, 5.0f);
    cLib_addCalc2(&i_this->field_0xf5c, sp40.y, 1.0f, 1.5f + YREG_F(13));

    cMtx_YrotS(*calc_mtx, actor->current.angle.y);
    sp4C.x = 0.0f;
    sp4C.y = 0.0f;
    sp4C.z = actor->speedF;
    MtxPosition(&sp4C, &sp40);
    actor->speed.x = sp40.x;
    actor->speed.z = sp40.z;
    actor->speed.y -= 1.5f + NREG_F(12);
    actor->current.pos += actor->speed;

    int sp10 = 0;

    if (i_this->timers[1] == 0) {
        int spC = 0;

        dBgS_LinChk sp58;
        sp58.SetRope();
        sp4C = actor->old.pos;
        sp40 = actor->current.pos;

        sp40.x += 2.0f * actor->speed.x;
        sp40.z += 2.0f * actor->speed.z;

        sp58.Set(&sp4C, &sp40, actor);
        if (dComIfG_Bgsp().LineCross(&sp58)) {
            spC = 1;
        }

        sp4C = player->eyePos;

        sp58.Set(&sp4C, &sp40, actor);
        if (dComIfG_Bgsp().LineCross(&sp58)) {
            spC = 1;
            OS_REPORT(" LURE WALL 2 HIT \n");
        }

        sp4C.y = 1.0f + i_this->field_0x590 + XREG_F(7);
        sp4C.x -= actor->speed.x * (3.0f + XREG_F(6));
        sp4C.z -= actor->speed.z * (3.0f + XREG_F(6));

        sp40 = actor->current.pos;
        sp40.x -= actor->speed.x * (3.0f + XREG_F(6));
        sp40.z -= actor->speed.z * (3.0f + XREG_F(6));
        sp40.y = 1.0f + i_this->field_0x590 + XREG_F(7);

        sp58.Set(&sp4C, &sp40, actor);
        if (dComIfG_Bgsp().LineCross(&sp58) || spC != 0) {
            actor->old.pos.x -= actor->speed.x;
            actor->old.pos.z -= actor->speed.z;
            actor->current.pos.x = actor->old.pos.x;
            actor->current.pos.z = actor->old.pos.z;

            if (actor->speedF < 0.0f) {
                actor->speedF = 0.1f;
                if (spC != 0) {
                    i_this->field_0x114e = cM_rndFX(5000.0f);
                    i_this->field_0x1150 = cM_rndFX(5000.0f);
                    actor->speed.y = -2.0f;
                    lure_bound_se_set(i_this);
                }
            }
            i_this->timers[1] = 10;
        }
    }

    if (i_this->acch.ChkGroundHit()) {
        actor->speed.y = 20.0f;
        actor->speedF *= 0.5f;
        i_this->field_0xffc = cM_rndFX(3000.0f);
        sp10 = 2;
    }

    if (i_this->field_0x100d != 0) {
        if (actor->speedF < -5.0f + VREG_F(7)) {
            actor->speedF = -5.0f + VREG_F(7);
        }
        actor->speed.y = -10.0f + VREG_F(0);
        actor->current.pos.y = i_this->field_0x590;

        i_this->field_0x1009 = 30;
        i_this->field_0x10a4 = 30;

        if (i_this->lure_type == MG_LURE_WS) {
            i_this->field_0x1020 = -(12000.0f + cM_rndF(4000.0f));
            i_this->field_0x1022 = 12000.0f + cM_rndF(4000.0f);
        }

        i_this->field_0x10a8 = 2;
        sp10 = 1;
    }

    if (sp10 != 0) {
        i_this->action = ACTION_LURE_ACTION;
        i_this->timers[3] = 30;
        i_this->timers[4] = 5;
        i_this->timers[6] = WREG_S(6) + 25;
        i_this->play_cam_mode = 5;
        i_this->field_0x1410 = 0.0f;

        if (i_this->lure_type == MG_LURE_SP) {
            if ((s16)(player->shape_angle.y - daAlink_getAlinkActorClass()->getFishingRodAngleY()) < 0) {
                i_this->field_0x141a = 0x1000;
            } else {
                i_this->field_0x141a = -0x1000;
            }
            i_this->field_0x140c = 35.0f;
        } else {
            i_this->field_0x140c = 5.0f;
        }

        i_this->field_0x14d0 = i_this->field_0x14d4 = 0.0f;
        i_this->field_0x114e = i_this->field_0x1150 = i_this->field_0x114a = i_this->field_0x114c = 0;
        i_this->field_0x1008 = 0;
        i_this->field_0x100c = 0;
        i_this->field_0x100a = 0;
        i_this->field_0x100b = 0;

        Z2GetAudioMgr()->changeFishingBgm(1);
    }

    cLib_addCalcAngleS2(&actor->current.angle.x, 0, 2, 0x1000);
    i_this->field_0x114a += i_this->field_0x114e;
    i_this->field_0x114c += i_this->field_0x1150;
    i_this->sound.startCreatureSoundLevel(Z2SE_AL_ROD_CASTING_LOOP, 0, -1);
}

static int simple_bg_check(dmg_rod_class* i_this, f32 param_1) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    cXyz sp24;
    cXyz sp18;
    cXyz spC;

    sp24 = actor->current.pos;
    spC.y = 0.0f;

    cMtx_YrotS(*calc_mtx, actor->current.angle.y);
    
    dBgS_LinChk linchk;
    linchk.SetRope();

    for (int i = 0; i < 3; i++) {
        static f32 c_x[] = {0.0f, 15.0f, -15.0f};
        static f32 c_z[] = {15.0f, 0.0f, 0.0f};

        spC.x = param_1 * c_x[i];
        spC.z = param_1 * c_z[i];
        MtxPosition(&spC, &sp18);
        sp18 += sp24;
        
        linchk.Set(&sp24, &sp18, actor);
        if (dComIfG_Bgsp().LineCross(&linchk)) {
            return i + 1;
        }
    }

    return 0;
}

static void* s_wd_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_OBJ_LP) {
        return i_actor;
    }

    return NULL;
}

static wd_ss* wd_check(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    obj_lp_class* obj_lp = (obj_lp_class*)fpcM_Search(s_wd_sub, i_this);

    if (obj_lp != NULL) {
        wd_ss* wd_p = obj_lp->mWdSs;
        cXyz sp14;

        for (int i = 0; i < obj_lp->field_0xad98; i++, wd_p++) {
            sp14 = wd_p->field_0x10 - actor->current.pos;
            if (sp14.abs() < wd_p->field_0x3c * (22.0f + KREG_F(11))) {
                return wd_p;
            }
        }
    }

    return NULL;
}

static void fr_action(dmg_rod_class* i_this, f32 param_1) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz sp14;
    cXyz sp8;

    cLib_addCalc0(&actor->speedF, 0.1f, 1.0f + BREG_F(12));

    if (i_this->reel_btn_flags != 0) {
        actor->speedF = i_this->reel_speed;

        if (i_this->field_0x578 & 48) {
            i_this->field_0xffe = (1000.0f + ZREG_F(8)) * cM_ssin(i_this->field_0x578 * 5000);
        } else {
            i_this->field_0xffe = 0;
        }

        if (actor->speedF > 12.0f) {
            i_this->field_0x1188 = 2.0f;
        }
    }

    if (param_1 >= 0.2f && i_this->field_0x100e == 0) {
        actor->speedF = (6.0f + BREG_F(14)) * param_1;

        if (i_this->field_0xffe > 0) {
            i_this->field_0xffe = -(BREG_S(2) + 8000);
        } else {
            i_this->field_0xffe = BREG_S(2) + 8000;
        }

        i_this->field_0x1000 = 0;
        i_this->field_0xf5c -= 2.0f + BREG_F(10);
        i_this->field_0x1009 = 10;
        i_this->field_0x100e = 3;
    }

    cLib_addCalcAngleS2(&i_this->field_0xffc, i_this->field_0xffe, 4, (BREG_S(3) + 12000));
    i_this->field_0xff0 = 0;
}

static void pe_action(dmg_rod_class* i_this, f32 param_1) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz sp18;
    cXyz spC;

    if (i_this->reel_btn_flags != 0) {
        actor->speedF = i_this->reel_speed;

        if (i_this->field_0x578 & 48) {
            i_this->field_0xffe = (1000.0f + ZREG_F(8)) * cM_ssin(i_this->field_0x578 * 5000);
        } else {
            i_this->field_0xffe = 0;
        }

        if (actor->speedF > 12.0f) {
            i_this->field_0x1188 = 2.0f;
        }
    }

    if (param_1 >= 0.2f && i_this->field_0x100e == 0) {
        actor->speedF = (8.0f + BREG_F(14)) * param_1;

        s16 temp_r26 = 13000.0f * param_1;
        if (i_this->field_0xffe > 0) {
            i_this->field_0xffe = -temp_r26;
        } else {
            i_this->field_0xffe = temp_r26;
        }

        i_this->field_0x1000 = 5000.0f * param_1;
        i_this->field_0xf5c -= 2.0f + BREG_F(10);
        i_this->field_0x1009 = 30;
        i_this->field_0x100e = 3;
    }

    cLib_addCalcAngleS2(&i_this->field_0xffc, i_this->field_0xffe, 4, (BREG_S(3) + 12000));

    s16 var_r25 = 4;
    s16 var_r27 = 500;
    if (i_this->field_0x10a4 != 0) {
        var_r27 = 5000;
    }

    if (actor->speedF > 4.0f) {
        i_this->sound.startCreatureSoundLevel(Z2SE_AL_ROD_ROLLBACK_SLOW, 0, -1);
        i_this->field_0x1002 = YREG_S(0) - 500;
        var_r25 = 4;
        var_r27 = 15000;
    } else {
        i_this->field_0x1002 = (s16)(YREG_S(1) + ((400.0f + ZREG_F(18)) * cM_ssin(i_this->field_0x578 * 1500))) - 2500;
    }

    cLib_addCalc0(&actor->speedF, 0.1f, 0.3f + BREG_F(12));
    cLib_addCalcAngleS2(&i_this->field_0x1000, i_this->field_0x1002, var_r25, var_r27);

    if (i_this->field_0x1000 > 0) {
        i_this->field_0x594 = (-5.0f + YREG_F(11)) * cM_ssin(i_this->field_0x1000);
    } else {
        i_this->field_0x594 = 0.0f;
    }
}

static void po_action(dmg_rod_class* i_this, f32 param_1) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz sp18;
    cXyz spC;
    
    s16 var_r26 = 1500;
    if (i_this->reel_btn_flags != 0) {
        actor->speedF = i_this->reel_speed;
        if (i_this->field_0x578 & 48) {
            i_this->field_0xffe = (2000.0f + ZREG_F(8)) * cM_ssin(i_this->field_0x578 * 4000);
        } else {
            i_this->field_0xffe = 0;
        }

        var_r26 = 4000;
        i_this->timers[0] = ZREG_S(5) + 15;

        if (actor->speedF > 3.0f) {
            i_this->field_0x1188 = 2.0f;
        }
    }

    if (param_1 >= 0.2f && i_this->field_0x100e == 0) {
        actor->speedF = (10.0f + BREG_F(14)) * param_1;

        if ((i_this->field_0x590 - actor->current.pos.y) <= 15.0f) {
            u32 var_r27 = (127.0f * param_1);
            if (var_r27 > 127) {
                var_r27 = 127;
            }
            i_this->sound.startCreatureSound(Z2SE_AL_UKI_POKOPOKO, var_r27, -1);
            i_this->field_0x100c++;
        }

        i_this->field_0x1009 = VREG_S(9) + 10;
        i_this->field_0xffe = 0;
        i_this->field_0x1000 = VREG_S(0) + 5000;
        actor->speed.y = -5.0f + VREG_F(5);
        i_this->field_0xf5c -= 2.0f + BREG_F(10);
        i_this->timers[0] = ZREG_S(5) + 15;
        i_this->field_0x100e = 3;
        i_this->field_0x118c = actor->speedF;
    }

    f32 temp_f31 = actor->speedF;
    cLib_addCalc0(&actor->speedF, 0.5f, 1.5f + BREG_F(12));

    if (actor->speedF < 2.0f) {
        if (temp_f31 >= 2.0f) {
            if (cM_rndF(1.0f) < 0.5f) {
                i_this->field_0xffe = -(BREG_S(2) + 10000);
            } else {
                i_this->field_0xffe = BREG_S(2) + 10000;
            }
            i_this->field_0x598 = 1.0f;
        }
    }

    if (i_this->timers[0] == 1) {
        i_this->field_0xffe += (s16)cM_rndFX(5000.0f);
    }

    cLib_addCalcAngleS2(&i_this->field_0xffc, i_this->field_0xffe, 4, var_r26);

    if (actor->speedF > 4.0f) {
        i_this->field_0x1002 = YREG_S(0) - 500;
    } else {
        i_this->field_0x1002 = YREG_S(1);
    }

    cLib_addCalcAngleS2(&i_this->field_0x1000, i_this->field_0x1002, 8, (YREG_S(2) + 1000));
}

static void ws_action(dmg_rod_class* i_this, f32 param_1) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz sp1C;
    cXyz sp10;
    s16 sp8 = 6000;

    if (i_this->reel_btn_flags != 0) {
        actor->speedF = i_this->reel_speed;

        if (i_this->field_0x578 & 48) {
            i_this->field_0xffe = (1000.0f + ZREG_F(8)) * cM_ssin(i_this->field_0x578 * 3000);
        } else {
            i_this->field_0xffe = 0;
        }

        i_this->field_0x1009 = 4;
        i_this->field_0x1020 = 15000.0f + cM_rndF(5000.0f);
        i_this->field_0x1022 = -(10000.0f + cM_rndF(3000.0f));
    }

    if (param_1 >= 0.2f && i_this->field_0x100e == 0) {
        actor->speedF = (15.0f + BREG_F(14)) * param_1;
        i_this->field_0x1020 = (1300.0f * actor->speedF) + cM_rndF(5000.0f);
        i_this->field_0x1022 = -((1000.0f * actor->speedF) + cM_rndF(3000.0f));
        i_this->field_0xffe = 0;
        i_this->field_0xf5c -= 2.0f + BREG_F(10);
        i_this->field_0x100e = 3;
    }

    if ((i_this->field_0x590 - actor->current.pos.y) <= 15.0f && actor->speedF > 2.0f) {
        u32 var_r27 = 9.0f * actor->speedF;
        if (var_r27 > 127) {
            var_r27 = 127;
        }
        i_this->sound.startCreatureSoundLevel(Z2SE_AL_UKI_KURUKURU_LOOP, var_r27, -1);
    }

    f32 temp_f31 = actor->speedF;
    cLib_addCalc0(&actor->speedF, 0.3f, 0.5f + BREG_F(12));

    if (actor->speedF < 4.0f) {
        if (temp_f31 >= 4.0f) {
            if (cM_rndF(1.0f) < 0.5f) {
                i_this->field_0xffe = -(BREG_S(2) + 10000);
            } else {
                i_this->field_0xffe = BREG_S(2) + 10000;
            }
            i_this->field_0x1009 = VREG_S(9) + 10;
        }
    }

    cLib_addCalcAngleS2(&i_this->field_0xffc, i_this->field_0xffe, 4, sp8);
    cLib_addCalcAngleS2(&i_this->field_0x1000, 0, 8, (YREG_S(2) + 1000));

    if (actor->speedF > 3.0f) {
        i_this->field_0x1188 = actor->speedF;
    }
}

static void sp_action(dmg_rod_class* i_this, f32 param_1) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz sp18;
    cXyz spC;

    i_this->field_0xf6e = 2;

    f32 var_f31 = fopAcM_searchPlayerDistanceXZ(actor);
    var_f31 *= -0.2f + VREG_F(18);
    
    if (var_f31 < -500.0f) {
        var_f31 = -500.0f;
    }

    f32 var_f30;
    if (var_f31 > -200.0f) {
        var_f30 = 0.0f;
    } else {
        var_f30 = -50.0f;
    }

    if (i_this->reel_btn_flags != 0 || i_this->timers[0] != 0) {
        cLib_addCalc2(&actor->speedF, i_this->reel_speed, 1.0f, 3.0f + BREG_F(14));
        cLib_addCalc2(&i_this->field_0x594, var_f30, 0.1f, 0.2f * i_this->reel_speed);
        actor->current.angle.z = 28000.0f * cM_ssin(i_this->field_0x578 * 3000);
        i_this->field_0x1004 = 4000.0f * cM_ssin(i_this->field_0x578 * 4000);
        i_this->field_0x1009 = 5;
    } else {
        cLib_addCalc2(&i_this->field_0x594, var_f31, 0.1f, 1.5f);
        if (i_this->field_0x594 > 20.0f + var_f31) {
            actor->current.angle.z = 28000.0f * cM_ssin(i_this->field_0x578 * 2000);
            i_this->field_0x1004 = 1000.0f * cM_ssin(i_this->field_0x578 * 4000);
        }
    }

    cLib_addCalc0(&actor->speedF, 0.1f, 1.0f);
    i_this->field_0x1009 = 10;
}

static void ground_action(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz sp3C;
    cXyz sp30;
    Vec sp24;
    f32 var_f31 = 0.0f;

    if (actor->speedF > 15.0f && i_this->acch.ChkWallHit()) {
        actor->speedF = 0.0f;
        i_this->field_0xffc = cM_rndFX(3000.0f);
    }

    if (i_this->reel_btn_flags != 0) {
        var_f31 = i_this->reel_speed;
        cLib_addCalcAngleS2(&i_this->field_0xffc, i_this->field_0xffe, 2, var_f31 * (1000.0f + JREG_F(7)));
    }

    if (actor->speedF >= 5.0f && (i_this->field_0x578 & 1) == 0 && cM_rndF(1.0f) < 0.5f) {
        i_this->field_0xffe = cM_rndFX(3000.0f + VREG_F(12));
        i_this->field_0x1000 = cM_rndFX(3000.0f + VREG_F(13));

        if (i_this->lure_type == MG_LURE_FR) {
            i_this->field_0xff2 = 2000;
        } else if (i_this->lure_type == MG_LURE_WS) {
            i_this->field_0x1020 = 2000.0f + cM_rndF(2000.0f);
            i_this->field_0x1022 = -(2000.0f + cM_rndF(2000.0f));
        }
    }

    if (i_this->acch.ChkGroundHit()) {
        cLib_addCalcAngleS2(&i_this->field_0x1000, 0, 1, (YREG_S(2) + 0x2000));

        if (i_this->field_0x10a8 == 0) {
            i_this->field_0xffc = cM_rndFX(32768.0f);
            i_this->field_0x1000 = cM_rndFX(32768.0f);

            if (cM_rndF(1.0f) < 0.5f) {
                actor->current.angle.z = (f32)0xB000;
            } else {
                actor->current.angle.z = (f32)-0xB000;
            }

            i_this->field_0x10a8++;
            if (actor->current.pos.y < 70.0f + i_this->field_0x590) {
                actor->speed.y = 20.0f + AREG_F(11);
                lure_bound_se_set(i_this);
                fopAcM_effSmokeSet1(&i_this->field_0x1178, &i_this->field_0x117c, &actor->current.pos, NULL, 0.2f, &actor->tevStr, 1);
            }
        } else {
            if (i_this->field_0x10a8 == 1) {
                lure_bound_se_set(i_this);
            }
            i_this->field_0x10a8 = 2;
            actor->speed.y = -10.0f;
        }

        dBgS_GndChk sp48;
        cMtx_YrotS(*calc_mtx, actor->shape_angle.y);
        sp3C.x = 0.0f;
        sp3C.y = 0.0f;
        sp3C.z = -10.0f;
        MtxPosition(&sp3C, &sp30);
        sp30 += actor->current.pos;

        sp24.x = sp30.x;
        sp24.y = 50.0f + sp30.y;
        sp24.z = sp30.z;
        sp48.SetPos(&sp24);

        sp30.y = 3.0f + dComIfG_Bgsp().GroundCross(&sp48);
        sp3C = sp30 - actor->current.pos;

        if (fabsf(sp3C.y) < 30.0f) {
            actor->current.angle.x = cM_atan2s(sp3C.y, JMAFastSqrt(SQUARE(sp3C.x) + SQUARE(sp3C.z)));

            sp3C.x = 10.0f;
            sp3C.y = 0.0f;
            sp3C.z = 0.0f;
            MtxPosition(&sp3C, &sp30);
            sp30 += actor->current.pos;

            sp24.x = sp30.x;
            sp24.y = 50.0f + sp30.y;
            sp24.z = sp30.z;
            sp48.SetPos(&sp24);

            sp30.y = 3.0f + dComIfG_Bgsp().GroundCross(&sp48);
            sp3C = sp30 - actor->current.pos;

            s16 sp8 = (s16)cM_atan2s(sp3C.y, JMAFastSqrt(SQUARE(sp3C.x) + SQUARE(sp3C.z)));
            if (actor->current.angle.x >= 0x1800 || sp8 >= 0x1800 || sp8 <= -0x1800) {
                actor->speedF = 20.0f;
                if (cM_rndF(1.0f) < 0.5f) {
                    actor->current.angle.z = 32768.0f + cM_rndFX(16000.0f);
                    i_this->field_0xffc = cM_rndFX(3000.0f + VREG_F(12));
                    i_this->field_0x1000 = cM_rndFX(3000.0f + VREG_F(13));
                }
            }
        }
    } else if (i_this->field_0x10a8 == 1) {
        i_this->field_0xffc += 3200;
        i_this->field_0x1000 += 4000;
    }

    cLib_addCalc2(&actor->speedF, var_f31, 1.0f, 5.0f + BREG_F(12));
}

static void wd_action(dmg_rod_class* i_this, f32 param_1, wd_ss* i_wd_s) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz sp28;
    cXyz sp1C;
    cLib_addCalc0(&actor->speedF, 1.0f, 5.0f + BREG_F(12));

    if (i_this->lure_type == MG_LURE_FR) {
        if (i_this->reel_btn_flags != 0) {
            actor->speedF = i_this->reel_speed;
            i_this->field_0xffe = 0;
        }

        if (param_1 >= 0.2f && i_this->field_0x100e == 0) {
            actor->speedF = (10.0f + BREG_F(14)) * param_1;
            s16 spC = 2000.0f * param_1;

            if (i_this->field_0xffe > 0) {
                i_this->field_0xffe = -spC;
            } else {
                i_this->field_0xffe = spC;
            }

            i_this->field_0x1000 = 0;
            i_this->field_0x100e = 3;
        }

        if (i_this->field_0xff0 == 0) {
            if (cM_rndF(1.0f) < 0.5f) {
                i_this->field_0xff0 = 0x7000;
            } else {
                i_this->field_0xff0 = -0x7000;
            }
        }

        if (actor->speedF > 5.0f && (i_this->field_0x578 & 3) == 0 && cM_rndF(1.0f) < 0.75f) {
            if (cM_rndF(1.0f) < 0.5f) {
                i_this->field_0xff0 = 0x7000;
            } else {
                i_this->field_0xff0 = -0x7000;
            }
        }

        actor->current.angle.z = i_this->field_0xff0;
    } else {
        i_this->field_0x1008 = 1;
        if (i_this->reel_btn_flags != 0) {
            actor->speedF = 3.0f + BREG_F(17);
            i_this->field_0xf6e = 1;
            i_this->field_0xffe = 0;
        }

        if (param_1 >= 0.2f && i_this->field_0x100e == 0) {
            actor->speedF = (7.0f + BREG_F(14)) * param_1;
            s16 spA = 4000.0f * param_1;

            if (i_this->field_0xffe > 0) {
                i_this->field_0xffe = -spA;
            } else {
                i_this->field_0xffe = spA;
            }

            i_this->field_0x1000 = 0;
            i_this->field_0x100e = 3;
        }

        if (fabsf(actor->speed.x) > 1.0f || fabsf(actor->speed.z) > 1.0f) {
            i_wd_s->field_0x10.x += 0.65f * actor->speed.x;
            i_wd_s->field_0x10.z += 0.65f * actor->speed.z;
            sp28 = actor->current.pos - i_wd_s->field_0x10;
            
            s16 sp8 = (s16)cM_atan2s(sp28.x, sp28.z);
            sp8 -= actor->shape_angle.y;

            if (sp8 < 0) {
                i_wd_s->field_0x38 += 200;
            } else {
                i_wd_s->field_0x38 -= 200;
            }
        }
    }

    cLib_addCalcAngleS2(&i_this->field_0xffc, i_this->field_0xffe, 2, (BREG_S(3) + 12000));
    cLib_addCalcAngleS2(&i_this->field_0x1000, 0, 1, (YREG_S(2) + 1000));
    cLib_addCalc2(&i_this->field_0x59c, 4.0f + KREG_F(15), 1.0f, 1.0f);
}

static void heart_action(dmg_rod_class* i_this, f32 param_1) {
    fopAc_ac_c* actor = &i_this->actor;
    cLib_addCalc0(&actor->speedF, 1.0f, 5.0f + BREG_F(12));

    if (i_this->reel_btn_flags != 0) {
        actor->speedF = i_this->reel_speed;

        if ((i_this->field_0x578 & 7) == 0) {
            dComIfGp_getVibration().StartShock(VIBMODE_S_POWER1, 1, cXyz(0.0f, 1.0f, 0.0f));
        }

        if (i_this->lure_type == MG_LURE_WS) {
            i_this->field_0x1020 = 7000.0f + cM_rndF(2000.0f);
            i_this->field_0x1022 = -(4000.0f + cM_rndF(1000.0f));
            i_this->sound.startCreatureSound(Z2SE_AL_UKI_OUT_WATER, 0, -1);
        }

        if (actor->speedF > 3.0f) {
            i_this->field_0x1188 = 2.0f;
        }
    }

    if (param_1 >= 0.2f && i_this->field_0x100e == 0) {
        actor->speedF = (10.0f + BREG_F(14)) * param_1;
        i_this->field_0x100e = 3;
    }
}

static void action_eff_set(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    if (!((i_this->field_0x590 - actor->current.pos.y) > 15.0f)) {
        cXyz sp3C;
        cXyz sp30;
        s16 temp_r0 = actor->shape_angle.y - fopAcM_searchPlayerAngleY(actor);
        temp_r0 /= 2;

        cMtx_YrotS(*calc_mtx, actor->shape_angle.y - temp_r0);
        sp3C.set(0.0f, 0.0f, 20.0f + hREG_F(6));
        MtxPosition(&sp3C, &sp30);
        sp30 += actor->current.pos;

        if (i_this->field_0x1188 > 0.1f) {
            f32 var_f31 = (0.2f + NREG_F(7)) * i_this->field_0x1188;
            if (var_f31 > 1.0f) {
                var_f31 = 1.0f;
            }

            cXyz sp24(var_f31, var_f31, var_f31);
            for (int i = 0; i < 2; i++) {
                static u16 w_eff_id[] = {0x897D, 0x897E};
                i_this->field_0x1180[i] = dComIfGp_particle_set(i_this->field_0x1180[i], w_eff_id[i], &sp30, &actor->tevStr, &actor->shape_angle, &sp24, 0xFF, NULL, -1, NULL, NULL, NULL);
            }
        }

        i_this->field_0x1188 = 0.0f;
        if (i_this->field_0x118c > 0.1f) {
            f32 var_f30 = (0.2f + NREG_F(7)) * i_this->field_0x118c;
            if (var_f30 > 1.0f) {
                var_f30 = 1.0f;
            }

            cXyz sp18(var_f30, var_f30, var_f30);
            for (int i = 0; i < 2; i++) {
                static u16 w_eff_id[] = {0x897F, 0x8980};
                dComIfGp_particle_set(w_eff_id[i], &sp30, &actor->tevStr, &actor->shape_angle, &sp18);
            }
        }

        i_this->field_0x118c = 0.0f;
    }
}

static void lure_action(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz sp54;
    cXyz sp48;

    if (i_this->field_0x10a6 == 0) {
        dMeter2Info_onUseButton(2);
        dComIfGp_setAStatusForce(86, 0);
        dComIfGp_set3DStatusForce(114, 5, 0);
    }

    if (i_this->timers[4] == 1) {
        daAlink_getAlinkActorClass()->onFishingRodCastingEnd();
    }

    i_this->field_0x75c = TREG_F(18) + (16000.0f + (2259.862f * (2.0f * daAlink_getAlinkActorClass()->getFishingReelFrame())));
    i_this->field_0x75e = 2259.862f * daAlink_getAlinkActorClass()->getFishingReelFrame();

    cLib_addCalc2(&i_this->field_0xf64, -10.0f + KREG_F(9), 1.0f, 0.5f);
    cLib_addCalc2(&i_this->field_0x6f8, i_this->field_0xf60 * (0.7f + VREG_F(14)), 0.1f, 10.0f);

    f32 var_f30 = i_this->rod_stick_x;
    if (fabsf(var_f30) < 0.1f) {
        var_f30 = 0.0f;
    }

    f32 var_f29 = i_this->rod_stick_y;
    if (var_f29 > -0.1f) {
        var_f29 = 0.0f;
    }

    f32 temp_f1 = JMAFastSqrt(SQUARE(var_f30) + SQUARE(var_f29));
    f32 var_f31 = temp_f1 - i_this->field_0x14ec;
    if (var_f31 < 0.0f) {
        var_f31 = 0.0f;
    }

    var_f31 *= 1.0f + ZREG_F(11);
    if (var_f31 > 1.0f) {
        var_f31 = 1.0f;
    }

    i_this->field_0x14ec = temp_f1;
    if (i_this->reel_btn_flags != 0) {
        if (i_this->reel_speed > 11.0f) {
            daAlink_getAlinkActorClass()->seStartOnlyReverbLevel(Z2SE_AL_REEL_FAST_LOOP);
            i_this->sound.startCreatureSoundLevel(Z2SE_AL_ROD_ROLLBACK_FAST, 0, -1);
            i_this->field_0x100b += 2;
        } else {
            daAlink_getAlinkActorClass()->seStartOnlyReverbLevel(Z2SE_AL_REEL_SLOW_LOOP);
            i_this->sound.startCreatureSoundLevel(Z2SE_AL_ROD_ROLLBACK_SLOW, 0, -1);
            i_this->field_0x100b++;
        }

        if (i_this->lure_type != MG_LURE_SP && i_this->lure_type != MG_LURE_WS && i_this->field_0x100b > 60) {
            i_this->field_0x100b = 60;
            i_this->field_0x100a = 1;
        }
    } else if (i_this->field_0x100b != 0) {
        i_this->field_0x100b--;
    } else {
        i_this->field_0x100a = 0;
    }

    if (fabsf(actor->speedF) > 20.0f) {
        i_this->field_0x100a = 1;
    }

    if (i_this->field_0x10a4 != 0) {
        i_this->field_0x10a4--;
    }

    if (i_this->field_0x100e != 0) {
        i_this->field_0x100e--;
    }

    if (i_this->field_0x100d != 0) {
        if (i_this->field_0x10a9 != 0) {
            heart_action(i_this, var_f31);
            actor->current.angle.z = 0;
            actor->current.angle.x = 0;
            i_this->field_0x114c = 0;
            i_this->field_0x114a = 0;
            i_this->field_0xffc = 0;
            i_this->field_0x1000 = 0;
            cMtx_YrotS(*calc_mtx, actor->current.angle.y);
        } else {
            wd_ss* wd_p = wd_check(i_this);
            if (wd_p != NULL) {
                wd_action(i_this, var_f31, wd_p);
                cMtx_YrotS(*calc_mtx, actor->current.angle.y);
            } else {
                cLib_addCalc0(&i_this->field_0x59c, 1.0f, 1.0f);

                switch (i_this->lure_type) {
                case MG_LURE_FR:
                    fr_action(i_this, var_f31);
                    break;
                case MG_LURE_PE:
                    pe_action(i_this, var_f31);
                    break;
                case MG_LURE_PO:
                    po_action(i_this, var_f31);
                    break;
                case MG_LURE_WS:
                    ws_action(i_this, var_f31);
                    break;
                case MG_LURE_SP:
                    sp_action(i_this, var_f31);
                    break;
                }

                if (actor->speedF > 10.0f) {
                    cLib_addCalc2(&actor->speedF, 10.0f, 1.0f, 2.0f);
                }

                action_eff_set(i_this);
            }

            if (i_this->timers[3] == 0 && actor->speedF > 1.0f) {
                int sp1C = simple_bg_check(i_this, 1.0f);
                if (sp1C != 0) {
                    i_this->timers[3] = 2;

                    if (sp1C == 2) {
                        i_this->field_0xffe = -0x1000;
                    } else if (sp1C == 3) {
                        i_this->field_0xffe = 0x1000;
                    } else {
                        i_this->field_0xffe = 0;
                    }

                    actor->speedF *= 0.8f;
                    actor->current.pos = actor->old.pos;
                }
            }

            cLib_addCalcAngleS2(&actor->current.angle.x, 0, 4, 0x2000);

            if (i_this->lure_type != MG_LURE_SP) {
                cLib_addCalcAngleS2(&actor->current.angle.z, 0, 2, 0x1000);
                cMtx_YrotS(*calc_mtx, (actor->current.angle.y + i_this->field_0xffc));
            } else {
                cMtx_YrotS(*calc_mtx, actor->current.angle.y);
            }
        }
    } else {
        ground_action(i_this);
        cMtx_YrotS(*calc_mtx, actor->current.angle.y);
    }

    sp54.x = 0.0f;
    sp54.y = 0.0f;
    sp54.z = actor->speedF;
    MtxPosition(&sp54, &sp48);
    actor->speed.x = sp48.x;
    actor->speed.z = sp48.z;
    actor->current.pos += actor->speed;

    if (i_this->field_0x100d != 0) {
        cLib_addCalc2(&actor->current.pos.y, i_this->field_0x590 + i_this->field_0x594, 0.5f, 2.0f + VREG_F(1));
        cLib_addCalc0(&actor->speed.y, 1.0f, 2.0f + VREG_F(2));
    } else {
        actor->speed.y -= 5.0f;
        i_this->field_0x594 = 0.0f;
    }

    sp54 = i_this->field_0x6ac - actor->current.pos;

    f32 var_f28;
    if (i_this->field_0xf6e == 1) {
        var_f28 = (0.007999999f + BREG_F(12)) * sp54.abs();
    } else if (i_this->field_0xf6e == 2) {
        var_f28 = (0.0095f + BREG_F(13)) * sp54.abs();
    } else {
        var_f28 = (0.01f + BREG_F(11)) * sp54.abs();
    }

    cLib_addCalc2(&i_this->field_0xf5c, var_f28, 1.0f, 1.5f + YREG_F(13));

    sp54 = i_this->mg_line.pos[90] - actor->current.pos;
    cLib_addCalcAngleS2(&actor->current.angle.y, cM_atan2s(sp54.x, sp54.z), 1, 0x1000);

    if (i_this->field_0xf5c < 2.0f) {
        i_this->field_0xf5c = 2.0f;
    }

    if (i_this->timers[6] == 0 && (i_this->field_0xf5c <= 2.5f + BREG_F(17) || (fopAcM_searchPlayerDistanceXZ(actor) < 450.0f + AREG_F(15))) && i_this->field_0x10a6 == 0) {
        if (i_this->field_0x10a9 != 0) {
            i_this->field_0x10a9 = 2;
            i_this->action = ACTION_LURE_HEART;
            Z2GetAudioMgr()->changeBgmStatus(4);
            i_this->play_cam_mode = 30;
            i_this->play_cam_timer = 0;
            i_this->play_cam_fovy = 90.0f + DREG_F(6);
            i_this->field_0x1410 = -40.0f;
            i_this->field_0x146d = 0;
            i_this->field_0x10b0 = 0;
            daPy_getLinkPlayerActorClass()->onFishingRodGetFish();
        } else {
            daPy_getLinkPlayerActorClass()->onFishingRodReelEnd();
            actor->speed.y = 6.0f;
            i_this->action = ACTION_LURE_STANDBY;

            Z2GetAudioMgr()->changeFishingBgm(0);

            if (i_this->field_0x10a7 == 3) {
                daAlink_getAlinkActorClass()->setCanoeFishingWaitAngle(dComIfGp_getPlayer(0)->shape_angle.y);
            } else {
                daAlink_getAlinkActorClass()->setCanoeFishingWaitAngle(daAlink_getAlinkActorClass()->getFishingRodAngleY());
            }

            i_this->field_0x10a7 = 0;
            sp54 = i_this->field_0x6ac - actor->current.pos;

            actor->current.angle.x = -cM_atan2s(sp54.y, sp54.z);
            actor->current.angle.y = cM_atan2s(sp54.x, JMAFastSqrt(SQUARE(sp54.y) + SQUARE(sp54.z)));

            OS_REPORT(" MG ROD noo 1!!! \n");

            i_this->play_cam_mode = 2;
            i_this->play_cam_timer = 0;
            i_this->field_0x1410 = 10.0f;
            i_this->field_0x1000 = 0;
            i_this->field_0xffc = 0;
            i_this->timers[0] = 0;
            i_this->timers[1] = 20;
            i_this->field_0x14f8 = 0;
            i_this->field_0x6f8 = 0.0f;
            i_this->field_0x6e0 = 0.0f;

            actor->old.pos = actor->current.pos = i_this->field_0x764;

            for (int i = 1; i < 99; i++) {
                i_this->field_0xdb0[i] = 0.0f;
                i_this->field_0xc20[i] = 0.0f;
            }
        }
    }

    if (i_this->field_0x10a6 != 0) {
        i_this->field_0x10a6--;
        if (i_this->field_0x10a6 == 0) {
            i_this->play_cam_mode = 20;
            i_this->play_cam_timer = 0;
            
            camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
            camera->mCamera.SetTrimSize(1);

            i_this->action = ACTION_LURE_BARE;
            i_this->field_0x146d = 0;
        }
    }

    if (i_this->field_0x13a8 > 50) {
        i_this->field_0x100d = 0;
        actor->speed.y = 0.0f;
        actor->current.pos.y += 3.0f + VREG_F(16);
    }
}

static void lure_hit(dmg_rod_class* i_this, mg_fish_class* i_mg_fish) {
    fopAc_ac_c* sp1C = &i_this->actor;
    fopAc_ac_c* sp18 = dComIfGp_getPlayer(0);
    cXyz sp44;
    cXyz sp38;

    i_this->field_0x1515 = 0;

    if (henna != NULL) {
        henna->field_0x7cc |= 1;
    }

    if (i_this->timers[7] == 1) {
        Z2GetAudioMgr()->seStart(Z2SE_AL_FISHING_HIT, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
        dMeter2Info_setMeterString(0x4C7);  // "FISH ON!"
        Z2GetAudioMgr()->changeFishingBgm(2);
    }

    if (i_this->reel_btn_flags != 0 && i_mg_fish->mRemainingHookTime == 0 && i_this->timers[7] == 0) {
        daAlink_getAlinkActorClass()->seStartOnlyReverbLevel(Z2SE_AL_REEL_SLOW_LOOP);
    }

    i_this->field_0x75c = TREG_F(18) + (16000.0f + (2259.862f * (2.0f * daAlink_getAlinkActorClass()->getFishingReelFrame())));
    i_this->field_0x75e = 2259.862f * daAlink_getAlinkActorClass()->getFishingReelFrame();
    sp1C->speedF = 0.0f;
    i_this->field_0xf64 = -3.5f + JREG_F(9);

    i_this->field_0x6f8 = i_this->field_0xf60 * (2.0f + JREG_F(11));
    if (i_this->field_0x6f8 > 500.0f + JREG_F(12)) {
        i_this->field_0x6f8 = 500.0f + JREG_F(12);
    }

    if (i_mg_fish != NULL) {
        i_this->field_0x6f8 += (i_this->field_0x6f8 * (0.05f + XREG_F(9))) + ((0.05f + XREG_F(9)) * (i_this->field_0x6f8 * cM_ssin(i_mg_fish->jointYaws2[1])));
    }

    if (i_mg_fish != NULL) {
        sp44 = sp1C->current.pos - sp18->current.pos;

        f32 temp_f28 = 0.5f + ((0.004f + XREG_F(13)) * sp44.abs());
        if (i_this->field_0xf5c < temp_f28) {
            i_this->field_0xf5c = temp_f28;
        }

        if (i_this->field_0xf60 > 200.0f + JREG_F(14)) {
            daAlink_getAlinkActorClass()->seStartOnlyReverbLevel(Z2SE_AL_ROD_BEND);
        }

        if (i_this->field_0xf60 > 100.0f + JREG_F(16)) {
            f32 temp_f31 = i_this->field_0xf60 - (100.0f + JREG_F(16));
            
            f32 var_f30 = i_this->rod_stick_x;
            if (fabsf(var_f30) < 0.1f) {
                var_f30 = 0.0f;
            }

            temp_f31 *= 0.1f + JREG_F(17);
            if (i_this->rod_stick_y < -0.5f) {
                i_this->field_0x1515 = 1;
            }

            f32 var_f29;
            if (i_this->field_0x1515 != 0) {
                var_f29 = 6.0f;
            } else {
                var_f29 = 3.0f;
            }

            if (temp_f31 > var_f29) {
                temp_f31 = var_f29;
            }

            cMtx_YrotS(*calc_mtx, i_this->field_0x6c4);
            sp44.x = var_f30 * -(200.0f + VREG_F(10));
            sp44.y = 0.0f;
            sp44.z = 0.0f;
            MtxPosition(&sp44, &sp38);
            sp38 += sp18->current.pos;
            sp44 = sp38 - sp1C->current.pos;

            s16 spA = cM_atan2s(sp44.x, sp44.z);
            cMtx_YrotS(*calc_mtx, spA);
            sp44.x = 0.0f;
            sp44.y = 0.0f;
            sp44.z = temp_f31;
            MtxPosition(&sp44, &sp38);
            i_mg_fish->actor.current.pos += sp38;

            for (int i = 0; i <= i_mg_fish->mNumJoints; i++) {
                i_mg_fish->mJointTranslations[i] += sp38;
            }

            if (i_mg_fish->mCurAction == 52) {
                cLib_addCalc2(&i_mg_fish->actor.current.pos.y, i_this->field_0x590 - ((25.0f + JREG_F(5)) * i_mg_fish->mJointScale), 0.05f, temp_f31 * (1.0f + JREG_F(6)));
                if (i_mg_fish->mDistToPlayer < 200.0f) {
                    cLib_addCalc2(&i_mg_fish->actor.current.pos.y, i_this->field_0x590 - ((20.0f + JREG_F(5)) * i_mg_fish->mJointScale), 0.2f, 0.8f + fabsf(i_mg_fish->actor.speed.y));
                }
            }
        }

        s16 sp8 = 0;
        if (i_this->lure_type == MG_LURE_FR) {
            if (i_mg_fish->mRemainingHookTime != 0) {
                if (i_mg_fish->mRemainingHookTime < 15) {
                    sp8 = 1;
                } else {
                    sp8 = 2;
                }
            }
        } else if (i_mg_fish->mRemainingHookTime != 0) {
            sp8 = 1;
        }

        if (sp8 != 0) {
            dMeter2Info_onUseButton(2);
            dComIfGp_setAStatusForce(82, 0);
            dComIfGp_set3DStatusForce(82, 2, 0);

            if (i_this->reel_btn_flags != 0) {
                cLib_addCalc2(&i_this->field_0xf5c, 0.5f, 1.0f, 0.2f + DREG_F(14));
            }

            int sp10 = 0;
            if (i_this->rod_stick_y < -0.5f) {
                sp10 = 1;
            }

            if (sp10 != 0) {
                OS_REPORT("///  HOOKSET TIM !!%d\n", i_mg_fish->mRemainingHookTime);
                i_mg_fish->mRemainingHookTime = 0;

                if (sp8 == 2) {
                    i_this->field_0x10a7 = 4;
                } else {
                    i_mg_fish->mCurAction = 52;
                    i_mg_fish->mActionPhase = 0;
                    i_mg_fish->field_0x65a = 0;
                    i_this->field_0x10a7 = 0;

                    static u8 learn_d[] = {0x01, 0x02, 0x04, 0x08, 0x10};
                    i_mg_fish->field_0x750 |= learn_d[i_this->lure_type];

                    if (i_this->field_0xf60 >= 200.0f) {
                        i_this->field_0xf5c -= 1.0f + YREG_F(14);
                    } else if (i_this->field_0xf60 >= 100.0f) {
                        i_this->field_0xf5c -= 2.0f + YREG_F(14);
                    } else {
                        i_this->field_0xf5c -= 3.0f + YREG_F(15);
                    }

                    if (i_mg_fish->mGedouKind == 3) {
                        i_this->vibmode = VIBMODE_S_POWER5;
                        i_this->vib_timer = 20;
                    } else if (i_mg_fish->mJointScale >= 0.7f) {
                        i_this->vibmode = VIBMODE_S_POWER8;
                        i_this->vib_timer = 40;
                    } else if (i_mg_fish->mJointScale >= 0.65f) {
                        i_this->vibmode = VIBMODE_S_POWER7;
                        i_this->vib_timer = 35;
                    } else if (i_mg_fish->mJointScale >= 0.6f) {
                        i_this->vibmode = VIBMODE_S_POWER6;
                        i_this->vib_timer = 30;
                    } else {
                        i_this->vibmode = VIBMODE_S_POWER5;
                        i_this->vib_timer = 25;
                    }

                    daAlink_getAlinkActorClass()->onFishingHit();

                    i_this->timers[7] = WREG_S(9) + 10;
                    i_this->field_0x1420 = 400.0f;
                    i_this->field_0x141c = 400.0f;

                    if (i_this->field_0x140c < 20.0f + AREG_F(19)) {
                        if ((s16)(sp18->shape_angle.y - daAlink_getAlinkActorClass()->getFishingRodAngleY()) < 0) {
                            i_this->field_0x141a = 0x1000;
                        } else {
                            i_this->field_0x141a = -0x1000;
                        }
                    } else if (i_this->field_0x141a > 0) {
                        i_this->field_0x141a = 0x1000;
                    } else {
                        i_this->field_0x141a = -0x1000;
                    }

                    i_this->field_0x140c = 35.0f + AREG_F(7);
                    i_this->field_0x1407 = 20;
                }
            }
        } else if (i_this->field_0x10a6 == 0 && i_this->field_0x100f == 0) {
            dMeter2Info_onUseButton(2);
            dComIfGp_setAStatusForce(0x55, 0);
            dComIfGp_set3DStatusForce(0x55, 2, 0);
        }
    }
}

static void lure_catch(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)i_this;
    fopAc_ac_c* mgfish_a = fopAcM_SearchByID(i_this->mg_fish_id);
    mg_fish_class* mgfish = (mg_fish_class*)mgfish_a;

    i_this->field_0xf64 = -5.0f + JREG_F(9);
    i_this->field_0x6f8 = 0.0f;
    i_this->field_0xf5c = 2.3f + AREG_F(5);
    i_this->field_0x100d = 0;
    csXyz angle;

    if (mgfish->mActionPhase < 2 && i_this->play_cam_timer > 25) {
        s16 target = DREG_S(6) + 2000;
        if (mgfish->mJointScale > 0.5f) {
            target += (s16)((mgfish->mJointScale - 0.5f) * (20000.0f + DREG_F(19)));
        }

        if (target > 6000) {
            target = 6000;
        }

        cLib_addCalcAngleS2(&i_this->field_0x10b0, target, 8, 300);
    } else {
        i_this->field_0x10b0 = 0;
    }

    angle.x = i_this->field_0x10b0;
    angle.y = 0;
    angle.z = i_this->field_0x10b0;
    daAlink_getAlinkActorClass()->setFishingArm1Angle(angle);

    if (i_this->field_0x14c2 != 0) {
        i_this->field_0x14c2++;
        if (i_this->field_0x14c2 >= 2) {
            if (i_this->field_0x14c2 == 2) {
                i_this->msgflow.init(actor, 0x2D6, 0, NULL);
            }

            if (i_this->field_0x14c2 > 100) {
                i_this->field_0x14c2 = 100;
            }

            if (i_this->msgflow.doFlow(actor, NULL, 0)) {
                i_this->field_0x14c2 = 0;
                mgfish_a->health = 1;
            }
        }
        return;
    }

    s16 var_r26;
    if (data_804BBBD4 == 1) {
        var_r26 = 39.37008f * mgfish->mJointScale;
    } else {
        var_r26 = 100.0f * mgfish->mJointScale;
    }

    if (var_r26 > 99) {
        var_r26 = 99;
    }

    u8 fish_kind = mgfish->mKind2;
    if (l_HIO.force_fish_msg_output != 0) {
        fish_kind = l_HIO.force_fish_msg_output - 1;
    }

    if (i_this->play_cam_timer == 1) {
        daAlink_getAlinkActorClass()->changeFishGetFace(1);
        if (data_804BBBD4 == 2) {
            f32 var_f31 = var_r26 / 2.54f;
            f32 var_f30 = dComIfGs_getEventReg(check_kind[fish_kind]) / 2.54f;

            if ((u8)var_f31 > (u8)var_f30) {
                i_this->field_0x14c0 = 1;
            } else {
                i_this->field_0x14c0 = 0;
            }
        } else if ((u8)var_r26 > dComIfGs_getEventReg(check_kind[fish_kind])) {
            i_this->field_0x14c0 = 1;
        } else {
            i_this->field_0x14c0 = 0;
        }
    }

    if (i_this->play_cam_timer == 40) {
        if (!dComIfGs_isEventBit((u16)dSv_event_flag_c::saveBitLabels[467])) {
            Z2GetAudioMgr()->subBgmStart(Z2BGM_FISHING_GET1);
        } else if (fish_kind == 3) {
            Z2GetAudioMgr()->subBgmStart(Z2BGM_FISHING_GET3);
        } else if (i_this->field_0x14c0 != 0) {
            Z2GetAudioMgr()->subBgmStart(Z2BGM_FISHING_GET1);
        } else {
            Z2GetAudioMgr()->subBgmStart(Z2BGM_FISHING_GET2);
        }
    } else if (i_this->play_cam_timer == 80) {
        u8 sp8 = 1;
        if (i_this->field_0x14c0 != 0) {
            if (!dComIfGs_isEventBit((u16)dSv_event_flag_c::saveBitLabels[467])) {
                sp8 = 2;
            } else if (fish_kind != 3) {
                sp8 = 2;
            }
        } else if (fish_kind == 3) {
            sp8 = 3;
        }

        if (fish_kind == 1) {
            sp8 = 2;
        }

        if (henna != NULL && sp8 == 2) {
            henna->move_mode = 10;
        }

        daAlink_getAlinkActorClass()->changeFishGetFace(sp8);
    }

    if (i_this->play_cam_timer >= 60) {
        if (i_this->field_0x146d == 0) {
            data_80450C9A++;
            if (data_80450C9A > 200) {
                data_80450C9A = 200;
            }

            dComIfGs_onEventBit((u16)dSv_event_flag_c::saveBitLabels[467]);

            static u16 catch_msgn_20[] = {0x02CE, 0x02D1, 0x02CF, 0x02D0};
            static u16 catch_msgn_21[] = {0x02D2, 0x02D5, 0x02D3, 0x02D4};
            static u16 catch_msgn_22[] = {0x02D7, 0x02DA, 0x02D8, 0x02D9};
            static u16 catch_msgn_10[] = {0x02DC, 0x02DF, 0x02DD, 0x02DE};
            static u16 catch_msgn_11[] = {0x02E0, 0x02E3, 0x02E1, 0x02E2};
            static u16 catch_msgn_12[] = {0x02E4, 0x02E7, 0x02E5, 0x02E6};

            if (dComIfGs_getEventReg(check_kind[fish_kind]) == 0) {
                dComIfGs_setEventReg(check_kind[fish_kind], 1);
                if (henna != NULL) {
                    i_this->catch_flow_id = catch_msgn_20[fish_kind];
                } else {
                    i_this->catch_flow_id = catch_msgn_10[fish_kind];
                }
            } else if (i_this->field_0x14c0 != 0) {
                if (henna != NULL) {
                    i_this->catch_flow_id = catch_msgn_21[fish_kind];
                } else {
                    i_this->catch_flow_id = catch_msgn_11[fish_kind];
                }
            } else if (henna != NULL) {
                i_this->catch_flow_id = catch_msgn_22[fish_kind];
            } else {
                i_this->catch_flow_id = catch_msgn_12[fish_kind];
            }

            if (data_804BBBD4 == 2) {
                dComIfGp_setMessageCountNumber((u8)(var_r26 / 2.54f));
            } else {
                dComIfGp_setMessageCountNumber(var_r26);
            }

            i_this->msgflow.init(actor, i_this->catch_flow_id, 0, NULL);
            i_this->field_0x146d = 1;
        }

        if (i_this->field_0x146d == 1 && i_this->msgflow.doFlow(actor, NULL, 0)) {
            i_this->field_0x146d = 2;
            int sp10 = 0;

            if (i_this->field_0x14c0 != 0) {
                data_80450C9B = 1;
                if (dMsgObject_getSelectCursorPos() == 0) {
                    dComIfGs_setEventReg(check_kind[fish_kind], var_r26);
                    sp10 = 1;
                    actor->health = 1;
                    i_this->mg_fish_id = -1;
                    i_this->play_cam_mode = 2;
                    i_this->play_cam_timer = 20;
                    i_this->field_0x1418 = daAlink_getAlinkActorClass()->shape_angle.y;
                    i_this->field_0x1410 = 0.0f;

                    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
                    camera->mCamera.SetTrimSize(1);
                    daAlink_getAlinkActorClass()->onFishingKeep();
                    data_80450C9B = 2;

                    if (fish_kind == 1) {
                        if (var_r26 >= 72) {
                            data_80450C9B = 4;
                        } else {
                            data_80450C9B = 3;
                        }
                    }
                }
            }

            if (sp10 == 0) {
                if (i_this->catch_flow_id == 0x2D1 || i_this->catch_flow_id == 0x2D5) {
                    i_this->field_0x14c2 = 1;
                } else {
                    mgfish_a->health = 1;
                }
            } else {
                mgfish_a->health = 2;
            }
        }
    }

    if (actor->health != 0) {
        i_this->action = ACTION_LURE_STANDBY;
        i_this->field_0xf5c = 0.5f;
        i_this->field_0x10a5 = 0;
        i_this->timers[2] = 20;
        actor->current.angle.set(0, 0, 0);
        i_this->field_0xffc = i_this->field_0x1000 = 0;
        daAlink_getAlinkActorClass()->changeFishGetFace(0);
        daAlink_getAlinkActorClass()->setCanoeFishingWaitAngle(daAlink_getAlinkActorClass()->shape_angle.y);
    }
}

static void lure_bare(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* mgfish = fopAcM_SearchByID(i_this->mg_fish_id);
    i_this->rod_stick_x = 0.0f;
    i_this->rod_stick_y = 0.0f;

    if (i_this->play_cam_timer >= 20) {
        if (i_this->field_0x146d == 0) {
            if (i_this->field_0x10a7 == 0) {
                if (henna != NULL) {
                    i_this->msgflow.init(actor, 0x3E6, 0, NULL);
                } else {
                    i_this->msgflow.init(actor, 0x2EC, 0, NULL);
                }
            } else if (i_this->field_0x10a7 == 1) {
                if (henna != NULL) {
                    i_this->msgflow.init(actor, 0x3E4, 0, NULL);
                } else {
                    i_this->msgflow.init(actor, 0x2EA, 0, NULL);
                }
            } else if (i_this->field_0x10a7 == 2) {
                if (henna != NULL) {
                    i_this->msgflow.init(actor, 0x3E5, 0, NULL);
                } else {
                    i_this->msgflow.init(actor, 0x2EB, 0, NULL);
                }
            } else if (i_this->field_0x10a7 == 3) {
                if (henna != NULL) {
                    i_this->msgflow.init(actor, 0x3E7, 0, NULL);
                } else {
                    i_this->msgflow.init(actor, 0x2ED, 0, NULL);
                }
            } else {
                if (henna != NULL) {
                    i_this->msgflow.init(actor, 0x2E9, 0, NULL);
                } else {
                    i_this->msgflow.init(actor, 0x2EE, 0, NULL);
                }
            }

            i_this->field_0x146d = 1;
        }

        if (i_this->field_0x146d == 1 && i_this->msgflow.doFlow(actor, NULL, 0)) {
            i_this->field_0x146d = 2;
            i_this->play_cam_mode = 5;
            i_this->field_0x1410 = 1.0f;
            i_this->field_0x1407 = 110;
            dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0))->mCamera.SetTrimSize(1);
            i_this->action = ACTION_LURE_ACTION;
            i_this->field_0x1006 = 0;
            daAlink_getAlinkActorClass()->changeFishGetFace(0);
            Z2GetAudioMgr()->changeFishingBgm(1);
            return;
        }

        if (i_this->play_cam_timer == 25) {
            Z2GetAudioMgr()->subBgmStart(Z2BGM_FISHING_BARE);
            if (i_this->field_0x10a7 == 1) {
                daAlink_getAlinkActorClass()->changeFishGetFace(4);
            } else if (i_this->field_0x10a7 == 2) {
                daAlink_getAlinkActorClass()->changeFishGetFace(3);
            } else {
                daAlink_getAlinkActorClass()->changeFishGetFace(4);
            }
        }
    }

    if (i_this->lure_type != MG_LURE_SP) {
        i_this->field_0x594 = 0.0f;
        actor->current.pos.y = i_this->field_0x590;
    }

    if (i_this->field_0x10a7 == 3 && i_this->timers[2] != 0) {
        fopAc_ac_c* player = dComIfGp_getPlayer(0);
        cXyz sp18;
        cXyz spC;
        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        sp18.x = 200.0f * cM_ssin(i_this->field_0x578 * 1800);
        sp18.y = -300.0f;
        sp18.z = 100.0f;
        MtxPosition(&sp18, &actor->current.pos);
        actor->current.pos += player->current.pos;
    }

    actor->speed.y = 0.0f;
}

static void lure_heart(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;

    i_this->field_0xf64 = -5.0f + JREG_F(9);
    i_this->field_0x6f8 = 0.0f;
    i_this->field_0xf5c = 0.29999995f + AREG_F(5);
    i_this->field_0x100d = 0;
    csXyz sp8;

    i_this->field_0x10b0 = 0;
    sp8.x = i_this->field_0x10b0;
    sp8.y = 0;
    sp8.z = i_this->field_0x10b0;

    fopAc_ac_c* obj_life = (fopAc_ac_c*)fopAcM_SearchByName(PROC_Obj_LifeContainer);
    if (obj_life != NULL) {
        fopAc_ac_c* temp_r25 = (fopAc_ac_c*)dComIfGp_getPlayer(0);
        cXyz sp10;

        cMtx_YrotS(*calc_mtx, temp_r25->shape_angle.y);
        sp10.x = 0.0f;
        sp10.y = 50.0f + hREG_F(5);
        sp10.z = 150.0f + hREG_F(6);
        MtxPosition(&sp10, &actor->current.pos);
        actor->current.pos += obj_life->current.pos;
    }

    daAlink_getAlinkActorClass()->setFishingArm1Angle(sp8);
    if (i_this->play_cam_timer == 1) {
        daAlink_getAlinkActorClass()->changeFishGetFace(2);
    }

    if (i_this->play_cam_timer == 65) {
        if ((dComIfGs_getMaxLife() % 5) == 4) {
            mDoAud_subBgmStart(Z2BGM_HEART_GET);
        } else {
            mDoAud_subBgmStart(Z2BGM_ITEM_GET_MINI);
        }

        dComIfGp_setMesgBgmOn();
        if (henna != NULL) {
            henna->move_mode = 10;
            henna->field_0x688 = 1;
        }
    }

    if (i_this->play_cam_timer >= 60) {
        if (i_this->play_cam_timer == 60) {
            if (henna != NULL) {
                i_this->msgflow.init(actor, 0x2DB, 0, NULL);
            } else {
                i_this->msgflow.init(actor, 0x2E8, 0, NULL);
            }
        }

        if (i_this->play_cam_timer == 90 && henna != NULL) {
            henna->field_0x688 = 0;
        }

        if (i_this->msgflow.doFlow(actor, NULL, 0)) {
            i_this->play_cam_mode = 2;
            i_this->play_cam_timer = 20;
            i_this->field_0x1418 = daAlink_getAlinkActorClass()->shape_angle.y;
            i_this->field_0x1410 = 0.0f;

            dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0))->mCamera.SetTrimSize(1);
            daAlink_getAlinkActorClass()->onFishingKeep();

            i_this->action = ACTION_LURE_STANDBY;
            i_this->field_0xf5c = 0.5f;
            i_this->field_0x10a5 = 0;
            i_this->timers[2] = 20;

            daAlink_getAlinkActorClass()->changeFishGetFace(0);
            daAlink_getAlinkActorClass()->setCanoeFishingWaitAngle(daAlink_getAlinkActorClass()->shape_angle.y);
            actor->current.angle.set(0, 0, 0);

            if (obj_life != NULL) {
                fopAcM_delete(obj_life);
                fopAcM_onItem(obj_life, 0x80);
                execItemGet(fpcNm_ITEM_KAKERA_HEART);
                dComIfGs_setEventReg(0xECFF, dComIfGs_getEventReg(0xECFF) | 0x40);
            }

            i_this->field_0x10a9 = 0;
        }
    }
}

static void hook_set(dmg_rod_class* i_this, cXyz* param_1, int param_2) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    cXyz sp38;
    cXyz sp2C;
    cXyz sp20;

    if (i_this->field_0x100d != 0) {
        if (i_this->lure_type == MG_LURE_SP) {
            sp20.set(0.0f, -2.0f, 0.0f);
        } else {
            mDoMtx_stack_c::YrotS(actor->shape_angle.y);
            mDoMtx_stack_c::ZrotM(actor->shape_angle.z);
            sp38.set(0.0f, -2.0f + NREG_F(14), 0.0f);
            mDoMtx_stack_c::multVec(&sp38, &sp20);
        }
    } else if (i_this->action == ACTION_LURE_ACTION) {
        mDoMtx_stack_c::YrotS(actor->shape_angle.y);
        mDoMtx_stack_c::XrotM(actor->shape_angle.x);

        if (param_2 == 0) {
            s16 sp10 = actor->shape_angle.z - 0x8000;
            mDoMtx_stack_c::ZrotM((sp10 * 2) + 0x8000);
            sp38.x = 0.0f;
            sp38.y = -5.0f;
            sp38.z = -5.0f;
        } else {
            sp38.x = 0.0f;
            sp38.y = 0.0f;
            sp38.z = -5.0f;
        }

        mDoMtx_stack_c::multVec(&sp38, &sp20);
    } else if (i_this->lure_type == MG_LURE_SP && i_this->action == ACTION_LURE_CATCH) {
        sp20.set(0.0f, 50.0f, 0.0f);
    } else {
        sp20.set(0.0f, -10.0f + NREG_F(15), 0.0f);
    }

    mg_hook_s* temp_r30 = &i_this->mg_hook[param_2];
    temp_r30->field_0x0[0] = *param_1;

    f32 temp_f27 = sp20.x + (temp_r30->field_0x0[1].x - temp_r30->field_0x0[0].x);
    f32 temp_f30 = sp20.y + (temp_r30->field_0x0[1].y - temp_r30->field_0x0[0].y);
    f32 temp_f29 = sp20.z + (temp_r30->field_0x0[1].z - temp_r30->field_0x0[0].z);

    temp_r30->field_0x18.x = -cM_atan2s(temp_f30, temp_f29);
    temp_r30->field_0x18.y = cM_atan2s(temp_f27, JMAFastSqrt(SQUARE(temp_f30) + SQUARE(temp_f29)));

    cMtx_XrotS(*calc_mtx, temp_r30->field_0x18.x);
    cMtx_YrotM(*calc_mtx, temp_r30->field_0x18.y);
    sp38.set(0.0f, 0.0f, 1.0f + BREG_F(17));
    MtxPosition(&sp38, &sp2C);
    temp_r30->field_0x0[1] = temp_r30->field_0x0[0] + sp2C;
    
    mDoMtx_stack_c::transS(temp_r30->field_0x0[0].x, temp_r30->field_0x0[0].y, temp_r30->field_0x0[0].z);

    if (param_2 == 0 && (i_this->action == ACTION_LURE_HIT || i_this->action == ACTION_LURE_CATCH)) {
        mDoMtx_stack_c::YrotM(actor->shape_angle.y);
        mDoMtx_stack_c::XrotM(actor->shape_angle.x);
        mDoMtx_stack_c::ZrotM(actor->shape_angle.z);
    } else {
        mDoMtx_stack_c::XrotM(temp_r30->field_0x18.x);
        mDoMtx_stack_c::YrotM(temp_r30->field_0x18.y);
        mDoMtx_stack_c::XrotM(-0x4000);

        if (param_2 == 1 && (i_this->action <= ACTION_LURE_STANDBY || (i_this->lure_type != MG_LURE_SP && i_this->action == ACTION_LURE_CATCH))) {
            sp38.x = fabsf(temp_r30->field_0x18.y);
            sp38.z = fabsf(temp_r30->field_0x18.x - 0x4000);

            f32 var_f31 = JMAFastSqrt(SQUARE(sp38.x) + SQUARE(sp38.z));
            if (var_f31 > 7000.0f) {
                var_f31 = 7000.0f;
            }

            if (var_f31 > 2000.0f && i_this->field_0xfec < var_f31) {
                i_this->field_0xfec = var_f31;
            }

            s16 spE, spC;
            spE = i_this->field_0xfec * cM_ssin(i_this->field_0x578 * 4000);
            spC = i_this->field_0xfec * cM_ssin(i_this->field_0x578 * 3800);

            mDoMtx_stack_c::XrotM(spE);
            mDoMtx_stack_c::ZrotM(spC);
            cLib_addCalc0(&i_this->field_0xfec, 1.0f, 150.0f);
        }
    }

    f32 temp_f28 = 1.0f + ZREG_F(6);
    mDoMtx_stack_c::scaleM(temp_f28, temp_f28, temp_f28);
    mDoMtx_stack_c::YrotM(actor->current.angle.z);
    i_this->hook_model[param_2]->setBaseTRMtx(mDoMtx_stack_c::get());
}

static f32 lure_scale[] = {1.0f, 0.75f, 0.75f, 0.6f, 0.375f};

static f32 lure_eye_off[] = {4.0f, 17.0f, 11.0f, 16.5f, 4.0f};

static f32 hook_1_offy[] = {-3.5f, -3.5f, -3.5f, -5.0f, -3.5f};

static f32 hook_2_offz[] = {-16.0f, -19.0f, -16.0f, -23.0f, -43.5f};

static f32 hook_2_offy[] = {0.0f, -1.0f, 0.0f, 0.0f, 0.0f};

static f32 lure_nose_off[] = {-4.6f, -16.6f, -11.6f, -17.6f, -2.6f};

static f32 hiton_off[] = {4.3f, 16.3f, 11.3f, 17.3f, 2.3000002f};

static void lure_disp(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    cXyz sp28;
    cXyz sp1C;

    if (i_this->action <= ACTION_LURE_STANDBY) {
        mDoMtx_stack_c::XrotM((s16)actor->shape_angle.x);
        mDoMtx_stack_c::YrotM((s16)actor->shape_angle.y);
        mDoMtx_stack_c::ZrotM(actor->current.angle.z);
        actor->current.angle.z += i_this->field_0x114e;

        if (i_this->field_0x578 & 0x80) {
            cLib_addCalcAngleS2(&i_this->field_0x114e, 700, 8, 13);
        } else {
            cLib_addCalcAngleS2(&i_this->field_0x114e, -700, 8, 13);
        }

        if (i_this->play_cam_mode >= 950 && i_this->play_cam_mode < 1000) {
            i_this->field_0x1154 = lure_nose_off[i_this->lure_type];
        } else {
            cLib_addCalc2(&i_this->field_0x1154, lure_nose_off[i_this->lure_type], 1.0f, 2.0f);
        }
    } else {
        mDoMtx_stack_c::YrotM(actor->shape_angle.y + i_this->field_0x114c);
        mDoMtx_stack_c::XrotM(actor->shape_angle.x + i_this->field_0x114a);

        if (i_this->lure_type == MG_LURE_SP) {
            mDoMtx_stack_c::transM(0.0f, 0.0f, 10.0f + nREG_F(3));
            mDoMtx_stack_c::YrotM(i_this->field_0x1004);
            mDoMtx_stack_c::transM(0.0f, 0.0f, -(10.0f + nREG_F(3)));
        }

        mDoMtx_stack_c::ZrotM(actor->shape_angle.z);
        cLib_addCalc0(&i_this->field_0x1154, 1.0f, 2.0f);
    }

    f32 var_f31 = l_HIO.field_0x8 * lure_scale[i_this->lure_type];
    if (i_this->action == ACTION_LURE_CATCH) {
        var_f31 *= 0.8f;
    }

    mDoMtx_stack_c::scaleM(var_f31, var_f31, var_f31);
    mDoMtx_stack_c::transM(0.0f, 0.0f, i_this->field_0x1154 + WREG_F(14));
    i_this->lure_model[i_this->lure_type]->setBaseTRMtx(mDoMtx_stack_c::get());
    
    sp28.set(0.0f, 0.0f, WREG_F(6) + lure_eye_off[i_this->lure_type]);
    mDoMtx_stack_c::multVec(&sp28, &i_this->field_0x1010);

    f32 var_f30 = 0.2f + WREG_F(5);
    if (i_this->lure_type == MG_LURE_SP) {
        var_f30 += 0.5f;
    }

    mDoMtx_stack_c::push();

    mDoMtx_stack_c::transM(0.0f, 0.0f, WREG_F(15) + hiton_off[i_this->lure_type]);
    if (i_this->lure_type != 0) {
        mDoMtx_stack_c::ZrotM(0x4000);
    }
    mDoMtx_stack_c::scaleM(var_f30, var_f30, var_f30);
    i_this->ring_model->setBaseTRMtx(mDoMtx_stack_c::get());

    mDoMtx_stack_c::pop();

    if (i_this->lure_type != MG_LURE_FR) {
        if (i_this->lure_type != MG_LURE_SP) {
            mDoMtx_stack_c::push();
            mDoMtx_stack_c::transM(ZREG_F(0), ZREG_F(1) + hook_1_offy[i_this->lure_type], 1.0f + ZREG_F(2));
            mDoMtx_stack_c::multVecZero(&sp1C);
            hook_set(i_this, &sp1C, 0);
            mDoMtx_stack_c::pop();
        }

        mDoMtx_stack_c::transM(ZREG_F(3), ZREG_F(4) + hook_2_offy[i_this->lure_type], ZREG_F(5) + hook_2_offz[i_this->lure_type]);
        mDoMtx_stack_c::multVecZero(&sp1C);
        hook_set(i_this, &sp1C, 1);
        return;
    }

    if (i_this->action <= ACTION_LURE_STANDBY) {
        i_this->field_0xff2 = 7000.0f + (i_this->field_0xff8 * cM_ssin(i_this->field_0x578 * 4000));

        f32 var_f29 = (300.0f + KREG_F(12)) * (actor->current.pos - actor->old.pos).abs();
        if (var_f29 > 6000.0f) {
            var_f29 = 6000.0f;
        }

        if (var_f29 > i_this->field_0xff8) {
            i_this->field_0xff8 = var_f29;
        }

        cLib_addCalc2(&i_this->field_0xff8, 500.0f, 1.0f, 50.0f + KREG_F(13));
        return;
    }

    if (i_this->action == ACTION_LURE_CAST) {
        i_this->field_0xff2 = 7000.0f * cM_ssin(i_this->field_0x578 * 6000);
        return;
    }

    s16 spC = 2;
    s16 spA = 0x400;
    s16 sp8;
    if (i_this->action == ACTION_LURE_ACTION && (actor->current.angle.z > 0x1000 || actor->current.angle.z < -0x1000)) {
        i_this->field_0xff4 = 6000;
        spC = 1;
        spA = 0x800;
    } else if (actor->speedF >= 0.5f) {
        i_this->field_0xff4 = -4000.0f + (2000.0f * actor->speedF);
        if (i_this->field_0xff4 > 7000) {
            i_this->field_0xff4 = 7000;
        }
        i_this->field_0xff8 = 2500.0f;
        sp8 = 0x800;
    } else {
        i_this->field_0xff4 = -5000;
    }

    cLib_addCalcAngleS2(&i_this->field_0xff2, i_this->field_0xff4 + (s16)(i_this->field_0xff8 * cM_ssin(i_this->field_0x578 * 2000)), spC, spA);
    cLib_addCalc2(&i_this->field_0xff8, 500.0f, 1.0f, 200.0f);
}

static void lure_main(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    cXyz sp68;
    cXyz sp5C;

    fopAc_ac_c* mgfish_a = fopAcM_SearchByID(i_this->mg_fish_id);
    mg_fish_class* mgfish = (mg_fish_class*)mgfish_a;

    int sp20 = 0;
    int sp1C = 1;
    int sp18 = 0;
    int sp14 = 1;

    switch (i_this->action) {
    case ACTION_LURE_ONBOAT:
        lure_onboat(i_this);
        sp1C = 0;
        sp14 = 0;
        break;
    case ACTION_LURE_STANDBY:
        sp14 = lure_standby(i_this);
        sp1C = 0;
        dMeter2Info_onGameStatus(1);
        break;
    case ACTION_LURE_CAST:
        lure_cast(i_this);
        sp20 = 1;
        dMeter2Info_onGameStatus(1);
        break;
    case ACTION_LURE_ACTION:
        lure_action(i_this);
        sp20 = 1;
        dComIfGp_setCStickStatusForce(91, 2, 0);
        dMeter2Info_onGameStatus(1);
        sp18 = 1;
        break;
    case ACTION_LURE_HIT:
        lure_hit(i_this, mgfish);
        sp20 = 1;
        sp1C = 0;
        dMeter2Info_onGameStatus(1);
        sp18 = 1;
        break;
    case ACTION_LURE_CATCH:
        lure_catch(i_this);
        sp20 = 1;
        sp1C = 0;
        dMeter2Info_onGameStatus(1);
        break;
    case ACTION_LURE_BARE:
        lure_bare(i_this);
        sp1C = 0;
        dMeter2Info_onGameStatus(1);
        break;
    case ACTION_LURE_HEART:
        lure_heart(i_this);
        sp20 = 1;
        sp1C = 0;
        dMeter2Info_onGameStatus(1);
        break;
    }

    if (sp18 != 0 && i_this->rod_stick_x > 0.0f) {
        i_this->rod_stick_x *= 0.7f;
    }

    if (i_this->lure_type == MG_LURE_WS) {
        i_this->field_0x101c[0] += i_this->field_0x1020;
        i_this->field_0x101e += i_this->field_0x1022;

        if (i_this->action == ACTION_LURE_CAST) {
            i_this->field_0x1020 = 8000;
            i_this->field_0x1022 = -8000;
        } else if (i_this->action == ACTION_LURE_HIT) {
            if ((i_this->field_0x578 & 7) == 0 && cM_rndF(1.0f) < 0.35f) {
                i_this->field_0x1020 = -(3000.0f + cM_rndF(3000.0f));
            }

            if ((i_this->field_0x578 & 7) == 7 && cM_rndF(1.0f) < 0.35f) {
                i_this->field_0x1022 = 3000.0f + cM_rndF(3000.0f);
            }
        }

        cLib_addCalcAngleS2(&i_this->field_0x1020, 0, 1, 500);
        cLib_addCalcAngleS2(&i_this->field_0x1022, 0, 1, 500);
    }

    if (sp20 != 0) {
        dComIfGp_att_LookRequest(actor, 400.0f, 300.0f, -300.0f, 0x6000, 1);
    }

    cXyz sp50;
    if (i_this->field_0x10a5 != 0) {
        i_this->field_0x10a5--;
        if (i_this->field_0x10a5 == 0) {
            i_this->action = ACTION_LURE_ACTION;
            i_this->field_0x1006 = 0;
            OS_REPORT(" MG ROD noo 3!!! \n");
        }
    }

    if (i_this->action == ACTION_LURE_ACTION || i_this->action == ACTION_LURE_CAST) {
        f32 temp_f31 = 3.0f + VREG_F(11);
        actor->current.pos.y -= temp_f31;
        actor->old.pos.y -= temp_f31;
        i_this->acch.CrrPos(dComIfG_Bgsp());
        actor->current.pos.y += temp_f31;
        actor->old.pos.y += temp_f31;

        if (i_this->acch.ChkWallHit()) {
            i_this->field_0x13a8++;

            dBgS_LinChk sp78;
            sp78.SetRope();

            for (int i = 0; i < 4; i++) {
                static f32 ax[] = {0.0f, 0.0f, 20.0f, -20.0f};
                static f32 az[] = {20.0f, -20.0f, 0.0f, -0.0f};

                sp5C.x = actor->current.pos.x + ax[i];
                sp5C.y = actor->current.pos.y;
                sp5C.z = actor->current.pos.z + az[i];
                sp78.Set(&actor->current.pos, &sp5C, actor);

                if (dComIfG_Bgsp().LineCross(&sp78)) {
                    actor->current.pos.x -= 0.05f * ax[i];
                    actor->current.pos.z -= 0.05f * az[i];
                    break;
                }
            }
        } else {
            i_this->field_0x13a8 = 0;
        }

        if (actor->current.pos.y <= i_this->field_0x590) {
            if (i_this->field_0x100d == 0) {
                i_this->field_0x100d = 1;
                i_this->field_0x1009 = 30;
                i_this->field_0x10a4 = 50;

                if (sp1C != 0) {
                    if (i_this->field_0x10a9 != 0) {
                        i_this->sound.startCreatureSound(Z2SE_AL_FISH_JUMP_DOWN, 0, -1);
                        sp68 = actor->current.pos;
                        sp68.y = i_this->field_0x590;
                        fopKyM_createWpillar(&sp68, 1.5f, 0);
                    } else {
                        sibuki_set(i_this, 0.25f, &actor->current.pos, 1);
                        i_this->sound.startCreatureSound(Z2SE_AL_FISH_JUMP_UP_S, 0, -1);
                    }

                    if (actor->speed.y <= -20.0f + VREG_F(0)) {
                        actor->speed.y = -10.0f + VREG_F(0);
                    } else {
                        actor->speed.y = 0.0f;
                    }

                    dKy_Sound_set(actor->current.pos, 5, fopAcM_GetID(i_this), 5);
                    if (actor->speedF > 5.0f) {
                        actor->speedF = 5.0f;
                    }
                }
            }
        } else if (i_this->field_0x100d != 0) {
            i_this->field_0x100d = 0;

            if (cM_rndF(1.0f) < 0.5f) {
                actor->current.angle.z = (f32)0xB000;
            } else {
                actor->current.angle.z = (f32)-0xB000;
            }
        }

        if (i_this->field_0x100d != 0) {
            sp50 = actor->current.pos;
            sp50.y = i_this->field_0x590;

            f32 var_f30;
            if (fabsf(actor->speedF) >= 2.0f) {
                var_f30 = 0.2f + KREG_F(1);
            } else {
                var_f30 = 0.05f + KREG_F(0);
            }

            f32 var_f29;
            if (i_this->field_0x10a9 != 0) {
                var_f29 = 1.2f;
            } else {
                var_f29 = 0.3f;
            }

            fopAcM_effHamonSet(&i_this->field_0x1178, &sp50, var_f29, var_f30);
        }
    } else if (i_this->action == ACTION_LURE_HIT) {
        i_this->field_0x100d = 1;
        sp68 = i_this->field_0x6ac - actor->current.pos;

        sp50.x = actor->current.pos.x + ((i_this->field_0x590 - actor->current.pos.y) / std::tan(cM_atan2f(sp68.y, sp68.x)));
        sp50.z = actor->current.pos.z + ((i_this->field_0x590 - actor->current.pos.y) / std::tan(cM_atan2f(sp68.y, sp68.z)));
        sp50.y = i_this->field_0x590;
        fopAcM_effHamonSet(&i_this->field_0x1178, &sp50, 0.3f + KREG_F(12), 0.3f + KREG_F(13));
    }

    cLib_addCalcAngleS2(&actor->shape_angle.y, actor->current.angle.y + i_this->field_0xffc, 2, 0x2000);
    cLib_addCalcAngleS2(&actor->shape_angle.x, actor->current.angle.x + i_this->field_0x1000, 2, 0x2000);
    cLib_addCalcAngleS2(&actor->shape_angle.z, actor->current.angle.z, 2, 0x1000);

    mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y + i_this->field_0x59c + (i_this->field_0x598 * cM_ssin(i_this->field_0x578 * 5000)), actor->current.pos.z);
    cLib_addCalc0(&i_this->field_0x598, 1.0f, 0.05f);

    lure_disp(i_this);

    if (sp14 != 0) {
        line_control2(i_this);
        cXyz sp44 = i_this->mg_line.pos[0] - i_this->mg_line.pos[1];
        i_this->field_0xf60 = sp44.abs();
    }
}

static int uki_calc(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp68;
    cXyz sp5C;

    int var_r28 = 0;

    if (dComIfGp_checkPlayerStatus0(0, 0x100000)) {
        i_this->field_0x1165 = 30;
        i_this->field_0x100d = 0;
    }

    if (i_this->hook_pos.y < 10.0f + i_this->field_0x590 + BREG_F(12) && depth_check(i_this, &i_this->hook_pos) > 0.0f) {
        if (i_this->field_0x100d == 0) {
            i_this->field_0x100d = 1;
            if (i_this->field_0x1165 == 0) {
                sp68.x = i_this->hook_pos.x;
                sp68.y = i_this->field_0x590;
                sp68.z = i_this->hook_pos.z;
                fopKyM_createWpillar(&sp68, 0.2f, 0);
                i_this->field_0x1165 = 20;
                i_this->sound.startCreatureSound(Z2SE_AL_UKI_IN_WATER, 0, -1);
            }

            i_this->field_0x108a = 2500.0f + cM_rndF(2500.0f);
            i_this->field_0x108c = 800.0f + cM_rndF(400.0f);
            if (cM_rndF(1.0f) < 0.5f) {
                i_this->field_0x108c *= -1;
            }
        }

        if (i_this->field_0x10a5 != 0 || abs(i_this->field_0x1084.x) < 0x3000 || i_this->field_0x10a2 > 2000) {
            cXyz sp50(i_this->hook_pos);
            sp50.y = i_this->field_0x590;
            fopAcM_effHamonSet(&i_this->field_0x1178, &sp50, 0.2f + KREG_F(2), 0.05f + KREG_F(0));
        }

        if (i_this->field_0x590 - i_this->mg_line.pos[85].y > 45.0f + BREG_F(11)) {
            cLib_addCalcAngleS2(&i_this->field_0x108a, 0, 8, WREG_S(0) + 50);
            if (i_this->field_0x108a > WREG_S(0) + 10) {
                i_this->field_0x108e += i_this->field_0x108c;
            }
            cLib_addCalc2(&i_this->field_0x1090, 0.5f + AREG_F(10), 1.0f, 0.01f + AREG_F(11));
        } else {
            i_this->field_0x1090 = 0.0f;
        }
    } else {
        if (i_this->field_0x100d != 0 && i_this->field_0x1165 == 0) {
            sp68.x = i_this->hook_pos.x;
            sp68.y = i_this->field_0x590;
            sp68.z = i_this->hook_pos.z;
            fopKyM_createWpillar(&sp68, 0.2f, 0);
            i_this->field_0x1165 = 20;
            i_this->sound.startCreatureSound(Z2SE_AL_UKI_OUT_WATER, 0, -1);
        }

        i_this->field_0x1090 = -10.0f;
        i_this->field_0x100d = 0;
        cLib_addCalcAngleS2(&i_this->field_0x108a, 0, 2, 100);
    }

    cXyz sp44;
    if (i_this->action == ACTION_UKI_CATCH) {
        sp44 = i_this->mg_line.pos[TREG_S(9) + BREG_S(9) + 51];
    } else {
        sp44 = i_this->mg_line.pos[TREG_S(9) + BREG_S(6) + 68];
    }

    cXyz sp38;
    cMtx_YrotS(*calc_mtx, player->shape_angle.y);
    sp68.x = 0.0f;
    sp68.y = 0.0f;
    sp68.z = 0.2f + YREG_F(8);
    MtxPosition(&sp68, &sp38);
    
    f32 temp_f30 = sp38.x + (i_this->hook_pos.x - sp44.x);
    dBgS_GndChk spE8;
    Vec sp2C;
    sp2C.x = i_this->hook_pos.x;
    sp2C.y = 50.0f + i_this->hook_pos.y;
    sp2C.z = i_this->hook_pos.z;
    spE8.SetPos(&sp2C);

    f32 var_f31 = KREG_F(17) + (17.0f + dComIfG_Bgsp().GroundCross(&spE8));
    if (var_f31 < (5.0f + i_this->field_0x590 + AREG_F(18))) {
        if (i_this->action < ACTION_UKI_HIT) {
            var_f31 = 5.0f + i_this->field_0x590 + AREG_F(18);
        }
    } else {
        var_r28 = 1;
    }

    f32 var_f28 = i_this->hook_pos.y + i_this->field_0x1090;
    if (var_f28 < var_f31) {
        var_f28 = var_f31;
    } else {
        var_r28 = 0;
    }

    f32 temp_f27 = var_f28 - sp44.y;
    f32 temp_f29 = sp38.z + (i_this->hook_pos.z - sp44.z);

    s16 temp_r0 = -cM_atan2s(temp_f27, JMAFastSqrt(SQUARE(temp_f30) + SQUARE(temp_f29)));
    s16 sp8;
    if (temp_r0 > 0 || i_this->field_0x1090 > 0.0f || i_this->action >= ACTION_UKI_HIT) {
        if (temp_r0 > 0 || i_this->action >= ACTION_UKI_HIT) {
            sp8 = 0x800;
        } else {
            sp8 = 4096.0f * i_this->field_0x1090;
        }
        cLib_addCalcAngleS2(&i_this->field_0x1084.x, temp_r0, 2, sp8);
    }

    cLib_addCalcAngleS2(&i_this->field_0x1084.y, (s16)cM_atan2s(temp_f30, temp_f29), 2, 0x800);
    cMtx_YrotS(*calc_mtx, i_this->field_0x1084.y);
    cMtx_XrotM(*calc_mtx, i_this->field_0x1084.x);
    sp68.set(0.0f, 0.0f, 6.5f + XREG_F(8));
    MtxPosition(&sp68, &sp5C);
    i_this->hook_pos = sp44 + sp5C;

    if (i_this->field_0x6e0 > 0.1f) {
        dBgS_LinChk sp78;
        sp78.SetRope();
        sp78.Set(&i_this->field_0x106c, &i_this->hook_pos, actor);
        if (dComIfG_Bgsp().LineCross(&sp78)) {
            OS_REPORT(" UKI WALL HIT \n");
            i_this->field_0x6e0 = -0.5f;
            i_this->hook_pos = i_this->field_0x106c;
        }
    }

    i_this->field_0x106c = i_this->hook_pos;
    return var_r28;
}

static void uki_ready(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    cXyz sp24;
    cXyz sp18;

    u32 sp8 = daAlink_getAlinkActorClass()->checkFishingRodUseStart();

    i_this->field_0x1508 = 1.0f;
    sp24 = i_this->field_0x6c8 - i_this->field_0x6d4;
    sp18.x = 0.0666667f * (i_this->field_0x6a0 * sp24.abs());
    if (sp18.x > (10.0f + ZREG_F(2))) {
        sp18.z = sp18.x * (5.0f + ZREG_F(3));
        if (sp18.z > 100.0f) {
            sp18.z = 100.0f;
        }
        sp18.y = sp18.z;
    } else {
        sp24.x = player->speedF * (2.0f + ZREG_F(4));
        sp18.z = 30.0f + ZREG_F(5) + sp24.x + (sp24.x * cM_ssin(i_this->field_0x578 * (ZREG_S(2) + 4000)));
        if (sp18.z > 100.0f) {
            sp18.z = 100.0f;
        }
        sp18.y = 10.0f + ZREG_F(6);
    }

    cLib_addCalc2(&i_this->field_0x6f8, sp18.z, 1.0f, sp18.y);

    MTXCopy(daAlink_getAlinkActorClass()->getLeftItemMatrix(), *calc_mtx);
    sp24.set(0.0f, 0.0f, 0.0f);
    MtxPosition(&sp24, &sp18);

    if (i_this->timers[3] != 0) {
        actor->current.pos = sp18;
    } else {
        cLib_addCalc2(&actor->current.pos.x, sp18.x, 1.0f, i_this->field_0x1094);
        cLib_addCalc2(&actor->current.pos.y, sp18.y, 1.0f, i_this->field_0x1094);
        cLib_addCalc2(&actor->current.pos.z, sp18.z, 1.0f, i_this->field_0x1094);
        cLib_addCalc2(&i_this->field_0x1094, 200.0f, 1.0f, 10.0f);
    }

    line_control2_u(i_this);
    uki_calc(i_this);

    cLib_addCalc2(&i_this->field_0xf64, -30.0f + ZREG_F(1), 1.0f, 0.5f + BREG_F(14));
    cLib_addCalc2(&i_this->field_0xf68, -(20.0f + AREG_F(10)), 1.0f, 0.02f * (20.0f + AREG_F(10)));

    if (i_this->timers[0] != 0 || dComIfGp_event_runCheck() != 0 || i_this->play_cam_mode >= 1000 || (henna != NULL && henna->cam_mode != 0) || dComIfGp_checkPlayerStatus0(0, 0x100000)) {
        return;
    }

    if (daAlink_getAlinkActorClass()->checkFishingRodUseAccept() && player->speedF < 1.0f && i_this->rod_substick_y < -0.9f) {
        i_this->field_0x14e4++;
        if (i_this->field_0x14e4 > 6) {
            i_this->play_cam_mode = 1;
        }
    } else {
        i_this->field_0x14e4 = 0;
    }

    if (sp8 != 0) {
        i_this->play_cam_mode = 1;
    }

    if (i_this->play_cam_mode != 0) {
        daAlink_getAlinkActorClass()->startFishingCastWait();
        i_this->action = ACTION_UKI_STANDBY;
        i_this->field_0x6e0 = 0.0f;
        i_this->field_0x1504 = 0.0f;
        i_this->field_0x1418 = player->shape_angle.y - dCam_getControledAngleY(dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0)));
        i_this->timers[0] = 20;
        i_this->field_0x102e = 0;
        daAlink_getAlinkActorClass()->seStartOnlyReverb(Z2SE_AL_ROD_SWING_UKI);
        i_this->timers[6] = 20;
        i_this->cast_power = 1.0f;
        i_this->field_0x6e0 = 1.0f;
        i_this->field_0xf68 = 0.0f;
        i_this->field_0xf64 = 0.0f;
    }
}

static BOOL uki_rod_bg_check(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    
    dBgS_LinChk sp20;
    sp20.SetRope();

    cXyz sp14;
    cXyz sp8;
    sp14 = player->current.pos;
    sp14.y += 100.0f;
    sp8 = i_this->field_0x6ac;
    sp8.y += 30.0f + YREG_F(1);

    sp20.Set(&sp14, &sp8, actor);
    if (dComIfG_Bgsp().LineCross(&sp20)) {
        return TRUE;
    } else {
        return FALSE;
    }
}

static void uki_pl_arm_calc(dmg_rod_class* i_this) {
    fopAc_ac_c* temp_r28 = dComIfGp_getPlayer(0);
    csXyz sp10;
    csXyz sp8;

    if (i_this->action == ACTION_UKI_STANDBY) {
        f32 var_f30 = i_this->hook_pos.y;
        if (var_f30 < i_this->field_0x590) {
            var_f30 = i_this->field_0x590;
        }
        
        f32 var_f31 = temp_r28->current.pos.y - var_f30;
        if (var_f31 > 100.0f + hREG_F(16)) {
            var_f31 = temp_r28->current.pos.y - i_this->field_0x590;
            var_f31 -= 100.0f + hREG_F(16);
            var_f31 *= 5.0f + hREG_F(17);
            if (var_f31 > 1000.0f) {
                var_f31 = 1000.0f;
            }
        } else {
            var_f31 = 0.0f;
        }
        cLib_addCalc2(&i_this->field_0x1504, var_f31, 0.1f, 20.0f);
    } else {
        i_this->field_0x1504 = 0.0f;
    }

    sp8.x = 0;
    sp8.y = ((1000.0f + (6000.0f + WREG_S(1))) - 300.0f) + (5.0f * i_this->field_0x1504);
    sp8.z = (300.0f + ((-3500.0f + WREG_S(2)) - 1000.0f)) - (3.0f * i_this->field_0x1504);

    sp10.x = 0;
    sp10.y = WREG_S(4) - 5000;
    sp10.z = WREG_S(5) + 2000;

    sp8.y += (s16)(-15000.0f * i_this->field_0x1508);
    sp8.z += (s16)(3500.0f * i_this->field_0x1508);

    sp10.y += (s16)(-4000.0f * i_this->field_0x1508);
    sp10.z += (s16)((3500.0f * i_this->field_0x1508) + ((-11000.0f + WREG_F(8)) * (i_this->field_0x150c * i_this->field_0x1508)));

    if (i_this->action == ACTION_UKI_HIT && i_this->field_0xf60 > 140.0f + JREG_F(14)) {
        sp8.y += (s16)((50.0f + nREG_F(0)) * cM_ssin(i_this->field_0x578 * 0x6200));
        sp8.z += (s16)((50.0f + nREG_F(0)) * cM_ssin(i_this->field_0x578 * 0x6500));
        daAlink_getAlinkActorClass()->seStartOnlyReverbLevel(Z2SE_AL_ROD_BEND);
    }

    daAlink_getAlinkActorClass()->setFishingArm1Angle(sp8);
    daAlink_getAlinkActorClass()->setFishingArm2Angle(sp10);
}

static void uki_standby(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp30;
    cXyz sp24;

    dComIfGp_setDoStatusForce(42, 0);

    f32 var_f31 = -(i_this->rod_substick_y * (1.05f + AREG_F(13)));
    f32 temp_f30 = i_this->rod_substick_x;
    if (var_f31 < 0.0f) {
        var_f31 = 0.0f;
    }

    cLib_addCalc2(&i_this->field_0x1508, var_f31, 0.5f, 0.2f);
    cLib_addCalc2(&i_this->field_0x150c, temp_f30, 0.5f, 0.2f);

    if (i_this->field_0x1508 > 0.3f && i_this->play_cam_mode < 5) {
        i_this->field_0x1418 += (s16)((-500.0f + VREG_F(3)) * mDoCPd_c::getStickX3D(PAD_1));
    }

    cMtx_YrotS(*calc_mtx, i_this->field_0x1418);
    sp30.x = 0.0f;
    sp30.y = 0.0f;
    sp30.z = 1.0f;
    MtxPosition(&sp30, &sp24);
    i_this->rod_stick_x = -sp24.x;
    i_this->rod_stick_y = sp24.z;

    uki_pl_arm_calc(i_this);

    sp30 = i_this->field_0x6c8 - i_this->field_0x6d4;
    sp24.x = 0.0666667f * (i_this->field_0x6a0 * sp30.abs());
    if (sp24.x > 10.0f + ZREG_F(2)) {
        sp24.z = sp24.x * (5.0f + ZREG_F(3));
        if (sp24.z > 100.0f) {
            sp24.z = 100.0f;
        }
        sp24.y = sp24.z;
    } else {
        sp30.x = player->speedF * (2.0f + ZREG_F(4));
        sp24.z = 30.0f + ZREG_F(5) + sp30.x + (sp30.x * cM_ssin(i_this->field_0x578 * (ZREG_S(2) + 4000)));
        if (sp24.z > 100.0f) {
            sp24.z = 100.0f;
        }
        sp24.y = 10.0f + ZREG_F(6);
    }

    cLib_addCalc2(&i_this->field_0x6f8, sp24.z, 1.0f, sp24.y);

    if (i_this->field_0x10a5 == 0) {
        actor->current.pos = i_this->field_0x764;
        if (actor->current.pos.y <= (10.0f + i_this->field_0x590) && actor->old.pos.y > (10.0f + i_this->field_0x590)) {
            sp30.x = actor->current.pos.x;
            sp30.y = i_this->field_0x590;
            sp30.z = actor->current.pos.z;
            fopKyM_createWpillar(&sp30, 0.1f, 0);
        }
    }

    actor->eyePos = i_this->hook_pos;

    BOOL sp8 = 0;
    if (i_this->prev_rod_substick_y < 0.0f) {
        if (i_this->rod_substick_y - i_this->prev_rod_substick_y >= 0.2f) {
            sp8 = 1;
            i_this->cast_power = fabsf(i_this->prev_rod_substick_y);
            i_this->cast_power *= i_this->cast_power;
            if (i_this->cast_power < 0.2f) {
                i_this->cast_power = 0.2f;
            }

            if (i_this->field_0x1508 > 0.7f) {
                daAlink_getAlinkActorClass()->seStartOnlyReverb(Z2SE_AL_ROD_SWING_UKI);
            }
        }
    }

    if (sp8 != 0) {
        i_this->field_0x6e0 = i_this->cast_power;
        i_this->field_0xf68 = 0.0f;
        i_this->field_0xf64 = 0.0f;
    }

    uki_calc(i_this);

    cLib_addCalc0(&i_this->field_0x6e0, 1.0f, 0.02f + BREG_F(18));
    cLib_addCalc2(&i_this->field_0xf64, -30.0f + ZREG_F(1), 1.0f, 0.5f + BREG_F(14));
    cLib_addCalc2(&i_this->field_0xf68, -(10.0f + AREG_F(10)), 1.0f, 0.01f * (10.0f + AREG_F(10)));

    f32 temp_f29 = (player->current.pos - i_this->hook_pos).abs();
    if (temp_f29 >= YREG_F(18) && i_this->field_0x1508 < 0.5f && i_this->field_0x100d != 0) {
        if (i_this->timers[1] == 0 && i_this->play_cam_mode < 5) {
            i_this->play_cam_mode = 5;
            i_this->timers[1] = 15;
            i_this->field_0xf68 = 0.0f;
            Z2GetAudioMgr()->changeFishingBgm(1);
        }
    } else if (i_this->timers[1] == 0 && i_this->play_cam_mode >= 5) {
        i_this->play_cam_mode = 3;
        i_this->timers[1] = 15;
        Z2GetAudioMgr()->changeFishingBgm(0);
    }

    if (mDoCPd_c::getTrigA(PAD_1)) {
        i_this->play_cam_mode = 90;
        Z2GetAudioMgr()->changeFishingBgm(0);
    }

    if (i_this->timers[0] == 0 && (dComIfGp_checkPlayerStatus0(0, 0x100000) || i_this->ccSph.ChkCoHit() || dComIfGp_event_runCheck() != 0 || uki_rod_bg_check(i_this) || (i_this->field_0x100d == 0 && player->current.pos.y - actor->current.pos.y > (350.0f + nREG_F(11))) || i_this->field_0x102e != 0)) {
        i_this->play_cam_mode = 90;
    }

    if (i_this->play_cam_mode == 90) {
        i_this->action = ACTION_UKI_READY;
        daAlink_getAlinkActorClass()->endFishingCastWait();
        i_this->field_0x1094 = 0.0f;
        i_this->field_0x6e0 = 0.0f;
        i_this->field_0x10a5 = 0;
    }
}

static void uki_hit(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp54;
    cXyz sp48;
    fopAc_ac_c* mgfish_a = fopAcM_SearchByID(i_this->mg_fish_id);
    mg_fish_class* mgfish = (mg_fish_class*)mgfish_a;

    actor->eyePos.x = i_this->hook_pos.x;
    actor->eyePos.z = i_this->hook_pos.z;
    actor->eyePos.y = i_this->field_0x590;

    int sp18 = 0;
    int sp14 = 0;

    f32 var_f29 = -i_this->rod_substick_y;
    f32 temp_f27 = i_this->rod_substick_x;
    if (var_f29 < 0.0f) {
        var_f29 = 0.0f;
    }

    cLib_addCalc2(&i_this->field_0x1508, var_f29, 0.5f, 0.2f);
    cLib_addCalc2(&i_this->field_0x150c, temp_f27, 0.5f, 0.2f);

    i_this->rod_stick_x = 0.0f;
    i_this->rod_stick_y = 0.0f;
    uki_pl_arm_calc(i_this);

    if (i_this->field_0x1508 > 0.5f) {
        f32 var_f31 = (i_this->field_0x1508 - 0.5f);
        var_f31 *= (20.0f + JREG_F(17));
        if (var_f31 > 6.0f + JREG_F(19)) {
            var_f31 = 6.0f + JREG_F(19);
        }

        s16 spA = (-15000.0f + TREG_F(14)) * (i_this->field_0x150c * i_this->field_0x1508);
        cMtx_YrotS(*calc_mtx, player->shape_angle.y + spA);
        sp54.x = 0.0f;
        sp54.y = 0.0f;
        sp54.z = 150.0f + TREG_F(13);
        MtxPosition(&sp54, &sp48);
        sp48 += player->current.pos;

        sp54.x = (sp48.x - actor->current.pos.x);
        sp54.z = (sp48.z - actor->current.pos.z);

        s16 sp8 = cM_atan2s(sp54.x, sp54.z);
        f32 var_f30 = 0.01f * JMAFastSqrt(SQUARE(sp54.x) + SQUARE(sp54.z));
        if (var_f30 > 1.0f) {
            var_f30 = 1.0f;
        }

        if (mgfish->mJointScale > 0.55f) {
            var_f30 *= mgfish->mJointScale * (1.1f + (6.0f * (mgfish->mJointScale - 0.55f)));
        }

        cMtx_YrotS(*calc_mtx, sp8);
        sp54.x = 0.0f;
        sp54.y = 0.0f;
        sp54.z = var_f31 * var_f30;
        MtxPosition(&sp54, &sp48);
        mgfish->actor.current.pos += sp48;

        for (int i = 0; i <= mgfish->mNumJoints; i++) {
            mgfish->mJointTranslations[i] += sp48;
        }

        if (i_this->timers[5] == 0 || i_this->field_0x590 - mgfish_a->current.pos.y > 300.0f) {
            cLib_addCalc2(&mgfish_a->current.pos.y, 100.0f + i_this->field_0x590, 0.5f, var_f31 * (0.7f + JREG_F(6)));
            if (mgfish_a->current.pos.y > (i_this->field_0x590 - (KREG_F(11) * mgfish->mJointScale))) {
                sp18 = 1;
            }
        }

        i_this->timers[3] = 20;
    } else if (i_this->timers[3] == 0 && i_this->field_0x1508 < 0.3f) {
        sp14 = 1;
    }

    if (sp14 != 0 || i_this->timers[2] == 0) {
        i_this->action = ACTION_UKI_STANDBY;
        i_this->field_0x6e0 = 0.0f;
        i_this->timers[0] = 20;
        i_this->play_cam_mode = 2;
        i_this->field_0x10a5 = 0;
        i_this->field_0x1090 = 0.0f;

        mgfish->mCurAction = 0;
        mgfish->mActionPhase = 0;
        if (mgfish->mCaughtType == MG_CATCH_SG) {
            e_sg_class* sg = (e_sg_class*)fopAcM_SearchByID(mgfish->mSkullfishId);
            sg->mInactive = 0;
        }
    
        mgfish->mCaughtType = MG_CATCH_FISH;
        mgfish->field_0x62e = 300.0f + cM_rndF(200.0f);
        Z2GetAudioMgr()->changeFishingBgm(1);
        return;
    }

    f32 var_f28 = i_this->field_0xf60 * (1.3f + JREG_F(11));
    if (var_f28 > 280.0f + TREG_F(2)) {
        var_f28 = 280.0f + TREG_F(2);
    }

    cLib_addCalc2(&i_this->field_0x6f8, var_f28, 0.2f, 50.0f);

    uki_calc(i_this);

    cLib_addCalc0(&i_this->field_0x6e0, 1.0f, 0.02f + BREG_F(18));
    cLib_addCalc2(&i_this->field_0xf64, -30.0f + ZREG_F(1), 1.0f, 0.5f + BREG_F(14));
    cLib_addCalc2(&i_this->field_0xf68, -(10.0f + AREG_F(10)), 1.0f, 0.01f * (10.0f + AREG_F(10)));

    cXyz sp3C;
    sp54 = i_this->field_0x6ac - actor->current.pos;
    sp3C.x = actor->current.pos.x + ((i_this->field_0x590 - actor->current.pos.y) / std::tan(cM_atan2f(sp54.y, sp54.x)));
    sp3C.z = actor->current.pos.z + ((i_this->field_0x590 - actor->current.pos.y) / std::tan(cM_atan2f(sp54.y, sp54.z)));
    sp3C.y = i_this->field_0x590;
    fopAcM_effHamonSet(&i_this->field_0x1178, &sp3C, 0.3f + KREG_F(12), 0.3f + KREG_F(13));

    if (sp18 != 0) {
        dComIfGp_getVibration().StartShock(4, 1, cXyz(0.0f, 1.0f, 0.0f));
        mgfish->mCurAction = 72;
        mgfish->mActionPhase = 0;
        mgfish->field_0x624[0] = 6;

        i_this->action = ACTION_UKI_CATCH;
        i_this->play_cam_mode = 20;
        i_this->play_cam_timer = 0;
        i_this->field_0x1410 = 0.0f;
        i_this->field_0x146d = 0;
        i_this->field_0x14fc = 0;
    }
}

static int bb_get(dmg_rod_class* i_this) {
    for (int i = 0; i < 3; i++) {
        u8 itemno =  dComIfGs_getItem(i + 15, false);
        if (itemno != fpcNm_ITEM_NONE) {
            if (itemno == fpcNm_ITEM_BOMB_BAG_LV1) {
                dComIfGs_setEmptyBombBagItemIn(fpcNm_ITEM_WATER_BOMB, 1, true);
                return 1;
            } else if (itemno == fpcNm_ITEM_WATER_BOMB) {
                if (dComIfGs_getBombNum(i) < dComIfGs_getBombMax(itemno)) {
                    dComIfGp_setItemBombNumCount(i, 1);
                    return 1;
                }
            }
        }
    }

    return 0;
}

static void uki_catch(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    cXyz sp20;
    cXyz sp14;

    fopAc_ac_c* mgfish_a = fopAcM_SearchByID(i_this->mg_fish_id);
    mg_fish_class* mgfish = (mg_fish_class*)mgfish_a;
    u8 fish_kind = mgfish->mKind2;

    s16 fish_size;
    if (data_804BBBD4 == 1) {
        fish_size = 39.37008f * mgfish->mJointScale;
    } else {
        fish_size = 100.0f * mgfish->mJointScale;
    }

    if (data_804BBBD4 == 2) {
        f32 temp_f31 = fish_size / 2.54f;
        f32 var_f30 = dComIfGs_getFishSize(fish_kind) / 2.54f;
        if ((u8)temp_f31 > (u8)var_f30) {
            i_this->field_0x14c0 = 1;
        } else {
            i_this->field_0x14c0 = 0;
        }
    } else if ((u8)fish_size > dComIfGs_getFishSize(fish_kind)) {
        i_this->field_0x14c0 = 1;
    } else {
        i_this->field_0x14c0 = 0;
    }

    i_this->rod_stick_x = 0.0f;
    i_this->rod_stick_y = 0.0f;

    if (i_this->play_cam_mode == 20 && i_this->play_cam_timer < 26) {
        cLib_addCalc2(&i_this->field_0x1508, 1.2f, 0.5f, 0.3f);
        cLib_addCalcAngleS2(&i_this->field_0x14fc, -700, 2, 200);
    } else {
        cLib_addCalc2(&i_this->field_0x1508, 0.8f, 0.3f, 0.15f);
        cLib_addCalcAngleS2(&i_this->field_0x14fc, 0, 1, 100);
    }

    cLib_addCalc0(&i_this->field_0x150c, 0.5f, 0.5f);
    uki_pl_arm_calc(i_this);

    i_this->field_0x6f8 = 200.0f + YREG_F(18);
    if (l_HIO.force_fish_msg_output != 0) {
        fish_kind = l_HIO.force_fish_msg_output - 1;
    }

    if (mgfish->mCaughtType == MG_CATCH_BT || mgfish->mCaughtType == MG_CATCH_KN || mgfish->mCaughtType == MG_CATCH_ED || mgfish->mCaughtType == MG_CATCH_SY) {
        if (i_this->play_cam_timer == 80) {
            Z2GetAudioMgr()->subBgmStart(Z2BGM_FISHING_GET3);
            if (cM_rndF(1.0f) < 0.5f) {
                daAlink_getAlinkActorClass()->changeFishGetFace(3);
            } else {
                daAlink_getAlinkActorClass()->changeFishGetFace(4);
            }
        }
    } else if (i_this->play_cam_timer == 40) {
        if (fish_kind == 4 || mgfish->mCaughtType == MG_CATCH_SP || mgfish->mCaughtType == MG_CATCH_BB || mgfish->mCaughtType == MG_CATCH_BIN || mgfish->mCaughtType == MG_CATCH_LH) {
            Z2GetAudioMgr()->subBgmStart(Z2BGM_FISHING_GET1);
            daAlink_getAlinkActorClass()->changeFishGetFace(2);
        } else if (i_this->field_0x14c0 != 0 || dComIfGs_getFishSize(fish_kind) == 0) {
            Z2GetAudioMgr()->subBgmStart(Z2BGM_FISHING_GET1);
            daAlink_getAlinkActorClass()->changeFishGetFace(2);
        } else {
            Z2GetAudioMgr()->subBgmStart(Z2BGM_FISHING_GET2);
            daAlink_getAlinkActorClass()->changeFishGetFace(1);
        }
    }

    if (i_this->play_cam_timer >= 50) {
        if (i_this->field_0x146d == 0) {
            if (mgfish->mCaughtType == MG_CATCH_BT) {
                if (data_80450C9E != 0) {
                    i_this->msgflow.init(actor, 0x1395, 0, NULL);
                } else {
                    i_this->msgflow.init(actor, 0x1394, 0, NULL);
                    data_80450C9E = 1;
                }

                if (henna != NULL) {
                    henna->move_mode = 30;
                }
            } else if (mgfish->mCaughtType == MG_CATCH_SP) {
                i_this->msgflow.init(actor, 0x1397, 0, NULL);
                dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[466]);
            } else if (mgfish->mCaughtType == MG_CATCH_LH) {
                i_this->msgflow.init(actor, 0x1396, 0, NULL);
            } else if (mgfish->mCaughtType == MG_CATCH_SG) {
                i_this->msgflow.init(actor, 0x1398, 0, NULL);
            } else if (mgfish->mCaughtType == MG_CATCH_BB) {
                if (bb_get(i_this)) {
                    i_this->msgflow.init(actor, 0x1399, 0, NULL);
                } else {
                    i_this->msgflow.init(actor, 0x139E, 0, NULL);
                }
            } else if (mgfish->mCaughtType == MG_CATCH_BIN) {
                i_this->msgflow.init(actor, 0x139A, 0, NULL);
                dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[468]);
                dComIfGs_setEmptyBottle();
            } else if (mgfish->mCaughtType == MG_CATCH_KN) {
                i_this->msgflow.init(actor, 0x139C, 0, NULL);
            } else if (mgfish->mCaughtType == MG_CATCH_ED) {
                i_this->msgflow.init(actor, 0x139D, 0, NULL);
            } else if (mgfish->mCaughtType == MG_CATCH_SY) {
                i_this->msgflow.init(actor, 0x139B, 0, NULL);
            } else {
                OS_REPORT(" GET FISH SIZE %d\n", fish_size);
                OS_REPORT(" GET FISH KIND %d\n", fish_kind);
                OS_REPORT(" GET FISH REC  %d\n", dComIfGs_getFishSize(fish_kind));

                switch (fish_kind) {
                case 5:
                    if (i_this->field_0x14c0 == 0) {
                        i_this->msgflow.init(actor, 0x138A, 0, NULL);
                    } else {
                        i_this->msgflow.init(actor, 0x138B, 0, NULL);
                    }
                    break;
                case 0:
                    if (i_this->field_0x14c0 == 0) {
                        i_this->msgflow.init(actor, 0x138C, 0, NULL);
                    } else {
                        i_this->msgflow.init(actor, 0x138D, 0, NULL);
                    }
                    break;
                case 1:
                    if (i_this->field_0x14c0 == 0) {
                        i_this->msgflow.init(actor, 0x1390, 0, NULL);
                    } else {
                        i_this->msgflow.init(actor, 0x1391, 0, NULL);
                    }
                    break;
                case 2:
                    if (i_this->field_0x14c0 == 0) {
                        i_this->msgflow.init(actor, 0x138E, 0, NULL);
                    } else {
                        i_this->msgflow.init(actor, 0x138F, 0, NULL);
                    }
                    break;
                case 3:
                    if (i_this->field_0x14c0 == 0) {
                        i_this->msgflow.init(actor, 0x1392, 0, NULL);
                    } else {
                        i_this->msgflow.init(actor, 0x1393, 0, NULL);
                    }
                    break;
                case 4:
                    if (i_this->field_0x14c0 == 0) {
                        i_this->msgflow.init(actor, 0x1389, 0, NULL);
                    } else {
                        i_this->msgflow.init(actor, 0x139F, 0, NULL);
                    }
                    dComIfGs_onEventBit(dSv_event_flag_c::saveBitLabels[620]);
                    break;
                }

                if (data_804BBBD4 == 2) {
                    dComIfGp_setMessageCountNumber((u8)(fish_size / 2.54f));
                } else {
                    dComIfGp_setMessageCountNumber(fish_size);
                }
            }

            i_this->field_0x146d = 1;
        }

        if (i_this->field_0x146d == 1 && i_this->msgflow.doFlow(actor, NULL, 0)) {
            i_this->field_0x146d = 2;
            i_this->play_cam_mode = 21;
            i_this->play_cam_timer = 0;

            if (mgfish->mCaughtType != MG_CATCH_FISH) {
                if (mgfish->mCaughtType == MG_CATCH_LH) {
                    dComIfGp_setItemRupeeCount(10.0f + cM_rndF(40.9f));
                }
            } else {
                dComIfGs_addFishNum(fish_kind);
                if (i_this->field_0x14c0 != 0) {
                    dComIfGs_setFishSize(fish_kind, fish_size);
                }
            }

            i_this->timers[2] = 30;

            if (henna != NULL) {
                henna->field_0x7cc |= 2;
            }
        }
    }
}

static void* s_sg_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_SG && ((fopAc_ac_c*)i_actor)->current.pos.y < ((dmg_rod_class*)i_data)->field_0x590 - 50.0f) {
        return i_actor;
    }

    return NULL;
}

static void* s_bb_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_E_BG && ((fopAc_ac_c*)i_actor)->current.pos.y < ((dmg_rod_class*)i_data)->field_0x590 - 50.0f) {
        return i_actor;
    }

    return NULL;
}

static void* s_lh_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_MG_FISH && ((mg_fish_class*)i_actor)->mGedouKind == 21) {
        if ((((fopAc_ac_c*)i_actor)->current.pos - ((fopAc_ac_c*)i_data)->current.pos).abs() < ((mg_fish_class*)i_actor)->mJointScale) {
            return i_actor;
        }
    }

    return NULL;
}

static void* s_bt_sub(void* i_actor, void* i_data) {
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_MG_FISH && ((mg_fish_class*)i_actor)->mGedouKind == 20) {
        if ((((fopAc_ac_c*)i_actor)->current.pos - ((fopAc_ac_c*)i_data)->current.pos).abs() < ((mg_fish_class*)i_actor)->mJointScale) {
            return i_actor;
        }
    }

    return NULL;
}

static void uki_main(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    fopAc_ac_c* mgfish_a = fopAcM_SearchByID(i_this->mg_fish_id);
    mg_fish_class* mgfish = (mg_fish_class*)mgfish_a;

    cXyz sp90;
    cXyz sp84;

    f32 var_f30 = i_this->rod_stick_x;
    if (fabsf(var_f30) < 0.1f) {
        var_f30 = 0.0f;
    }

    i_this->field_0xf5c = 4.25f + ZREG_F(0);
    int sp38 = 0;

    if (i_this->field_0x6a8 != 0) {
        i_this->field_0x6a8--;
        cLib_addCalc2(&i_this->field_0x6a4, 1.0f, 1.0f, 0.077f + hREG_F(11));
    } else if (daAlink_getAlinkActorClass()->checkEquipAnime()) {
        cLib_addCalc0(&i_this->field_0x6a4, 1.0f, 0.2f + hREG_F(10));
    } else {
        i_this->field_0x6a4 = 1.0f;
    }

    if (i_this->action == ACTION_UKI_READY) {
        if (daAlink_getAlinkActorClass()->checkFisingRodJewl()) {
            i_this->hook_kind = 1;
        } else {
            i_this->hook_kind = 0;
        }
    }

    if (daAlink_getAlinkActorClass()->checkFisingRodWorm()) {
        i_this->esa_kind = 2;
    } else if (daAlink_getAlinkActorClass()->checkFisingRodBee()) {
        i_this->esa_kind = 1;
    } else {
        i_this->esa_kind = 0;
    }

    switch (i_this->action) {
    case ACTION_UKI_READY:
        uki_ready(i_this);
        break;
    case ACTION_UKI_STANDBY:
        uki_standby(i_this);
        dMeter2Info_onGameStatus(1);
        break;
    case ACTION_UKI_HIT:
        uki_hit(i_this);
        dMeter2Info_onGameStatus(1);
        break;
    case ACTION_UKI_CATCH:
        uki_catch(i_this);
        dMeter2Info_onGameStatus(1);
        break;
    }

    if (i_this->rod_substick_y <= -0.5f && (i_this->rod_substick_y - i_this->prev_rod_substick_y) <= -0.5f && i_this->action == ACTION_UKI_STANDBY) {
        if (i_this->field_0x100d != 0) {
            daAlink_getAlinkActorClass()->seStartOnlyReverb(Z2SE_AL_ROD_SWING_LURE);
        }
    }

    if (i_this->action == ACTION_UKI_STANDBY && i_this->rod_substick_y < -0.5f) {
        sp38 = 1;
    }

    if (i_this->field_0x10a5 != 0) {
        i_this->field_0x10a5--;

        line_control2_u(i_this);
        if (i_this->action == ACTION_UKI_CATCH) {
            uki_calc(i_this);
        }
    
        cXyz sp78 = i_this->mg_line.pos[0] - i_this->mg_line.pos[1];
        i_this->field_0xf60 = sp78.abs();

        if (i_this->timers[2] == 0 && i_this->action < ACTION_UKI_HIT && sp38 != 0) {
            daAlink_getAlinkActorClass()->onFishingHit();
            i_this->action = ACTION_UKI_HIT;
            i_this->play_cam_mode = 10;
            mgfish->mCurAction = 71;
            mgfish->mActionPhase = 0;

            e_sg_class* sg = (e_sg_class*)fpcM_Search(s_sg_sub, i_this);
            if (sg != NULL) {
                mgfish->mSkullfishId = fopAcM_GetID(sg);
                mgfish->mCaughtType = MG_CATCH_SG;
                sg->mInactive = 1;
            } else {
                fopAc_ac_c* bb = (fopAc_ac_c*)fpcM_Search(s_bb_sub, i_this);
                if (bb != NULL) {
                    mgfish->mCaughtType = MG_CATCH_BB;
                }
            }

            int get_no = dComIfGs_getFishNum(0) +
                         dComIfGs_getFishNum(1) +
                         dComIfGs_getFishNum(5) +
                         dComIfGs_getFishNum(4) +
                         dComIfGs_getFishNum(1) +
                         dComIfGs_getFishNum(2);
            OS_REPORT(" GET NO %d\n", get_no);

            if (strcmp(dComIfGp_getStartStageName(), "F_SP127") == 0) {
                if (get_no >= 5 && cM_rndF(1.0f) < 0.15f) {
                    int rnd_catch = cM_rndF(4.99f);
                    if (rnd_catch == 0) {
                        mgfish->mCaughtType = MG_CATCH_BT;
                    } else if (rnd_catch == 1) {
                        mgfish->mCaughtType = MG_CATCH_LH;
                    } else if (rnd_catch == 2) {
                        mgfish->mCaughtType = MG_CATCH_KN;
                    } else if (rnd_catch == 3) {
                        mgfish->mCaughtType = MG_CATCH_ED;
                    } else if (rnd_catch == 4) {
                        mgfish->mCaughtType = MG_CATCH_SY;
                    }
                }

                if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[468])) {
                    if (cM_rndF(1.0f) <= 0.5f) {
                        cXyz bin_pos(6800.0f, 30.0f, -270.0f);
                        bin_pos -= player->current.pos;

                        if (bin_pos.abs() < 2500.0f) {
                            s16 spE = (player->shape_angle.y - cM_atan2s(bin_pos.x, bin_pos.z));
                            if (spE < 0x4000 && spE > -0x4000) {
                                mgfish->mCaughtType = MG_CATCH_BIN;
                            }
                        }
                    }
                }

                if (!dComIfGs_isEventBit(dSv_event_flag_c::saveBitLabels[466]) && i_this->hook_kind == 1) {
                    static cXyz sp_pos[] = {
                        cXyz(-4600.0f, 0.0f, 3100.0f),
                        cXyz(3616.0f, 0.0f, -4739.0f),
                    };

                    for (int i = 0; i < 2; i++) {
                        sp90 = actor->current.pos - sp_pos[i];
                        sp90.y = 0.0f;
                        if (sp90.abs() < 300.0f) {
                            int sp20 = 0;
                            for (int j = 0; j <= 3; j++) {
                                if ((j == 0 || j == 2 || j == 3) && dComIfGs_getEventReg(check_kind[j]) >= 10) {
                                    sp20++;
                                }
                            }

                            if (sp20 >= 3) {
                                mgfish->mCaughtType = MG_CATCH_SP;
                            }
                            break;
                        }
                    }
                }
            } else if (get_no >= 4 && cM_rndF(1.0f) < 0.2f) {
                if (cM_rndF(1.0f) < 0.5f) {
                    if (fpcM_Search(s_lh_sub, i_this) != NULL) {
                        mgfish->mCaughtType = MG_CATCH_LH;
                    }
                } else if (fpcM_Search(s_bt_sub, i_this) != NULL) {
                    mgfish->mCaughtType = MG_CATCH_BT;
                }
            }

            Z2GetAudioMgr()->seStart(Z2SE_AL_FISHING_HIT, NULL, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            dMeter2Info_setMeterString(0x4C7);
            Z2GetAudioMgr()->changeFishingBgm(2);

            i_this->timers[3] = 20;
            i_this->field_0x1508 = 0.7f;
            i_this->timers[5] = 80;
            i_this->timers[2] = 400;
        }
    }

    mDoMtx_stack_c::transS(i_this->hook_pos.x, i_this->hook_pos.y, i_this->hook_pos.z);

    cXyz sp60;
    f32 sp18;
    dKyw_get_AllWind_vec(&i_this->hook_pos, &sp60, &sp18);

    if (sp18 >= 0.4f && i_this->hook_pos.y < (i_this->field_0x590 - 5.0f) + ZREG_F(11)) {
        f32 temp_f29 = 7000.0f + (900.0f * cM_ssin(i_this->field_0x578 * 2000));

        cLib_addCalcAngleS2(&i_this->field_0x10a0, cM_atan2s(sp60.x, sp60.z), 4, 100);
        cLib_addCalcAngleS2(&i_this->field_0x10a2, (sp18 - 0.4f) * temp_f29, 4, 100);
    } else {
        cLib_addCalcAngleS2(&i_this->field_0x10a2, 0, 2, 200);
    }

    mDoMtx_stack_c::YrotM(i_this->field_0x10a0);
    mDoMtx_stack_c::XrotM(i_this->field_0x10a2);
    mDoMtx_stack_c::YrotM(i_this->field_0x1084.y);
    mDoMtx_stack_c::XrotM(i_this->field_0x1084.x);
    mDoMtx_stack_c::ZrotM(i_this->field_0x108e);

    s16 spC = i_this->field_0x108a * cM_ssin(i_this->field_0x1084.x);
    mDoMtx_stack_c::XrotM(spC);
    mDoMtx_stack_c::scaleM(l_HIO.field_0xc, l_HIO.field_0xc, l_HIO.field_0xc);
    mDoMtx_stack_c::transM(0.0f, 0.0f, l_HIO.field_0x10);
    mDoMtx_stack_c::XrotM(XREG_S(8));
    i_this->uki_model->setBaseTRMtx(mDoMtx_stack_c::get());

    mDoMtx_stack_c::transM(0.0f, 0.0f, l_HIO.field_0x14);
    mDoMtx_stack_c::scaleM(l_HIO.field_0x18, l_HIO.field_0x18, l_HIO.field_0x1c);
    i_this->uki_saki_model->setBaseTRMtx(mDoMtx_stack_c::get());

    sp90 = i_this->mg_line.pos[95] - actor->current.pos;
    s16 spA = -cM_atan2s(sp90.y, sp90.z);
    s16 sp8 = (s16)cM_atan2s(sp90.x, JMAFastSqrt(SQUARE(sp90.y) + SQUARE(sp90.z)));

    actor->current.angle.z += i_this->field_0x114e;

    if (i_this->action == ACTION_UKI_CATCH && mgfish->mCaughtType == MG_CATCH_SP) {
        if (g_Counter.mTimer & 128) {
            cLib_addCalcAngleS2(&i_this->field_0x114e, 1000, 4, 40);
        } else {
            cLib_addCalcAngleS2(&i_this->field_0x114e, -1000, 4, 40);
        }
    } else if (i_this->action == ACTION_UKI_CATCH && mgfish->mCaughtType >= MG_CATCH_KN) {
        if (g_Counter.mTimer & 256) {
            cLib_addCalcAngleS2(&i_this->field_0x114e, 200, 4, 8);
        } else {
            cLib_addCalcAngleS2(&i_this->field_0x114e, -200, 4, 8);
        }
    } else if (i_this->field_0x578 & 256) {
        cLib_addCalcAngleS2(&i_this->field_0x114e, 500, 8, 10);
    } else {
        cLib_addCalcAngleS2(&i_this->field_0x114e, -500, 8, 10);
    }

    mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y, actor->current.pos.z);
    mDoMtx_stack_c::XrotM(spA);
    mDoMtx_stack_c::YrotM(sp8);
    mDoMtx_stack_c::ZrotM(actor->current.angle.z);
    mDoMtx_stack_c::push();

    mDoMtx_stack_c::XrotM(0x4000);

    static f32 hook_size[] = {1.0f, 0.5f};
    f32 temp_f31 = hook_size[i_this->hook_kind];
    mDoMtx_stack_c::scaleM(temp_f31, temp_f31, temp_f31);
    i_this->hook_model[i_this->hook_kind]->setBaseTRMtx(mDoMtx_stack_c::get());

    mDoMtx_stack_c::pop();

    if (i_this->esa_kind != 0) {
        f32 var_f31;
        if (i_this->esa_kind == 1) {
            mDoMtx_stack_c::XrotM(-0x8000);
            if (i_this->hook_kind == 1) {
                mDoMtx_stack_c::transM(JREG_F(8), -0.5f + JREG_F(9), 16.5f + JREG_F(10));
            } else {
                mDoMtx_stack_c::transM(JREG_F(8), -0.5f + JREG_F(9), 9.0f + JREG_F(10));
            }
            var_f31 = 0.1f;
        } else {
            if (i_this->hook_kind == 1) {
                mDoMtx_stack_c::transM(JREG_F(13), -1.0f + JREG_F(14), -9.0f + JREG_F(15));
            } else {
                mDoMtx_stack_c::transM(JREG_F(13), -1.0f + JREG_F(14), -3.0f + JREG_F(15));
            }
            var_f31 = 0.065f + JREG_F(12);
        }

        mDoMtx_stack_c::scaleM(var_f31, var_f31, var_f31);
        i_this->esa_model[i_this->esa_kind - 1]->setBaseTRMtx(mDoMtx_stack_c::get());

        if (i_this->esa_kind == 2) {
            for (int i = XREG_S(2) + 1; i < 9; i++) {
                i_this->field_0x103c[i] = 4000.0f * cM_ssin((i_this->field_0x578 * 2000) - (i * 4000));
                i_this->field_0x104e[i] = 2000.0f * cM_ssin((i_this->field_0x578 * 2000) - (i * 5000));
            }
        }
    }

    i_this->ccSph.SetC(i_this->mg_line.pos[(i_this->field_0x578 * 4) & 60]);
    dComIfG_Ccsp()->Set(&i_this->ccSph);
}

static void cam_3d_morf(dmg_rod_class* i_this, f32 i_scale) {
    cLib_addCalc2(&i_this->play_cam_center.x, i_this->play_cam_center_target.x, i_scale, i_this->play_cam_center_speed.x * i_this->field_0x1410);
    cLib_addCalc2(&i_this->play_cam_center.y, i_this->play_cam_center_target.y, i_scale, i_this->play_cam_center_speed.y * i_this->field_0x1410);
    cLib_addCalc2(&i_this->play_cam_center.z, i_this->play_cam_center_target.z, i_scale, i_this->play_cam_center_speed.z * i_this->field_0x1410);

    cLib_addCalc2(&i_this->play_cam_eye.x, i_this->play_cam_eye_target.x, i_scale, i_this->play_cam_eye_speed.x * i_this->field_0x1410);
    cLib_addCalc2(&i_this->play_cam_eye.y, i_this->play_cam_eye_target.y, i_scale, i_this->play_cam_eye_speed.y * i_this->field_0x1410);
    cLib_addCalc2(&i_this->play_cam_eye.z, i_this->play_cam_eye_target.z, i_scale, i_this->play_cam_eye_speed.z * i_this->field_0x1410);
}

// NONMATCHING - JMAFastSqrt needs to not be inlined
// tricks like FORCE_DONT_INLINE can stop inlining for most of the inlines in this function, but it's unclear how to stop just one inline
static void play_camera(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    fopAc_ac_c* mgfish_a = fopAcM_SearchByID(i_this->mg_fish_id);
    mg_fish_class* mgfish = (mg_fish_class*)mgfish_a;
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
    camera_class* camera0 = dComIfGp_getCamera(0);

    dBgS_GndChk sp244;
    dBgS_ObjGndChk_Spl sp1F0;
    int sp70 = 0;
    i_this->field_0x13b0 = i_this->rod_substick_y;
    i_this->field_0x13ac = i_this->rod_substick_x;

    cXyz sp174;
    cXyz sp168;
    cXyz sp15C;
    cXyz sp150;
    cXyz sp144(0.0f, 0.0f, 0.0f);

    switch (i_this->play_cam_mode) {
    case 0:
        if (dComIfGp_checkPlayerStatus0(0, 0x2000) || dComIfGp_event_runCheck() != 0) {
            i_this->field_0x146c = 20;
        }

        if (daAlink_getAlinkActorClass()->checkCanoeRide()) {
            dComIfGp_setZStatusForce(94, 0);
            dMeter2Info_onUseButton(0x800);

            if (data_804BBBD4 != 0) {
                dComIfGp_setDoStatusForce(124, 0);
            } else {
                dComIfGp_setDoStatusForce(15, 0);
            }

            if (i_this->field_0x146c == 0) {
                if (mDoCPd_c::getTrigA(PAD_1)) {
                    i_this->play_cam_mode = 1050;
                }

                if (mDoCPd_c::getTrigZ(PAD_1)) {
                    if (henna != NULL) {
                        i_this->play_cam_mode = 900;
                    } else {
                        i_this->play_cam_mode = 1000;
                    }
                }
            }
        }
        break;
    case 1: {
        i_this->play_cam_mode = 2;
        camera->mCamera.Stop();
        i_this->play_cam_timer = 0;
        i_this->field_0x141c = i_this->field_0x1420 = 500.0f;
        i_this->field_0x1424 = 180.0f + WREG_F(0);
        i_this->field_0x1428 = 100.0f + WREG_F(1);

        camera_class* sp58 = (camera_class*)dComIfGp_getCamera(0);
        i_this->field_0x144c = sp58->lookat.eye;
        i_this->field_0x1458 = sp58->lookat.center;
        i_this->play_cam_eye = i_this->field_0x144c;
        i_this->play_cam_center = i_this->field_0x1458;

        dMw_offMenuRing();
        camera->mCamera.SetTrimSize(1);
        i_this->play_cam_fovy = 55.0f;
        /* fallthrough */
    }
    case 2: {
        sp70 = 1;
        f32 sp6C = 0.3f + NREG_F(3);
        f32 sp68 = i_this->field_0x1420;
        f32 sp64 = 180.0f + WREG_F(0);
        f32 sp60 = 100.0f + WREG_F(1);
        f32 sp5C = 30.0f;

        if (i_this->play_cam_timer > (s16)(8 + YREG_S(6))) {
            cLib_addCalcAngleS2(&i_this->field_0x1418, daAlink_getAlinkActorClass()->getFishingRodAngleY(), 6, 2000);
        }

        sp6C = 0.5f + NREG_F(5);
        sp68 = 350.0f + WREG_F(2);
        sp5C = 100.0f;
        sp64 = 230.0f + WREG_F(3);
        sp60 = (150.0f + WREG_F(4)) - 30.0f;

        i_this->field_0x1420 = 500.0f;
        if (i_this->action == ACTION_LURE_CAST) {
            sp150 = actor->current.pos;
            if (sp150.y > (150.0f + (300.0f + i_this->field_0x590) + ZREG_F(19))) {
                sp150.y = (150.0f + (300.0f + i_this->field_0x590) + ZREG_F(19));
            }

            f32 fovy;
            if (i_this->lure_type == MG_LURE_SP) {
                fovy = 35.0f;
            } else {
                fovy = 5.0f;
            }
            cLib_addCalc2(&i_this->play_cam_fovy, fovy, 0.1f, 1.0f + BREG_F(16));
        } else {
            cLib_addCalc2(&i_this->play_cam_fovy, 55.0f, 0.2f, 10.0f);
            cLib_addCalc2(&i_this->field_0x141c, sp68, 0.5f, sp5C);
            cLib_addCalc2(&i_this->field_0x1424, sp64, 0.5f, 5.0f);
            cLib_addCalc2(&i_this->field_0x1428, sp60, 0.5f, 5.0f);

            cMtx_YrotS(*calc_mtx, i_this->field_0x1418);
            sp174.x = 0.0f;
            sp174.y = 0.0f;
            sp174.z = -i_this->field_0x141c;
            MtxPosition(&sp174, &sp15C);
            sp15C += player->current.pos;

            cLib_addCalc2(&i_this->play_cam_eye.x, sp15C.x, sp6C, 1000.0f);
            cLib_addCalc2(&i_this->play_cam_eye.z, sp15C.z, sp6C, 1000.0f);
            sp15C.y += ((250.0f + NREG_F(18)) - 100.0f) - 30.0f;
            cLib_addCalc2(&i_this->play_cam_eye.y, sp15C.y, sp6C, 1000.0f);

            sp174.y = i_this->field_0x1428;
            sp174.z = (100.0f + nREG_F(10));
            MtxPosition(&sp174, &sp150);
            sp150 += player->current.pos;
        }

        cLib_addCalc2(&i_this->field_0x1410, 1.0f, 1.0f, 0.01f + NREG_F(8));
        cLib_addCalc2(&i_this->play_cam_center.x, sp150.x, 0.3f + NREG_F(4), 200.0f * i_this->field_0x1410);
        cLib_addCalc2(&i_this->play_cam_center.y, sp150.y, 0.2f + NREG_F(5), 200.0f * i_this->field_0x1410);
        cLib_addCalc2(&i_this->play_cam_center.z, sp150.z, 0.3f + NREG_F(4), 200.0f * i_this->field_0x1410);

        i_this->field_0x141a = i_this->field_0x1418 - daAlink_getAlinkActorClass()->getFishingRodAngleY();
        i_this->field_0x140c = i_this->play_cam_fovy;
        break;
    }
    case 5: {
        sp70 = 1;
        cLib_addCalc2(&i_this->field_0x141c, 400.0f + BREG_F(7), 0.1f, (20.0f + YREG_F(8)) * i_this->field_0x1410);

        f32 sp50 = i_this->field_0x13b0;
        sp50 *= -(3.0f + BREG_F(2));

        sp174 = actor->current.pos - player->current.pos;
        f32 sp4C = sp174.abs();
        i_this->field_0x140c += sp50;

        f32 sp48 = (15.0f - (0.015f * (sp4C - 500.0f)));
        if (sp48 < 0.0f) {
            sp48 = 0.0f;
        }

        f32 sp44 = 55.0f;
        if (i_this->field_0x140c > sp44) {
            i_this->field_0x140c = sp44;
        } else if (i_this->field_0x140c < (5.0f + sp48)) {
            i_this->field_0x140c = 5.0f + sp48;
        }

        if (i_this->field_0x1407 != 0) {
            i_this->field_0x1407--;
            cLib_addCalc2(&i_this->play_cam_fovy, i_this->field_0x140c, 0.3f, 10.0f + BREG_F(2));
        } else {
            cLib_addCalc2(&i_this->play_cam_fovy, i_this->field_0x140c, 0.1f, 3.0f + BREG_F(2));
        }

        f32 sp40 = i_this->field_0x13ac;
        sp40 *= 1000.0f + BREG_F(3);

        i_this->field_0x141a += (s16)sp40;
        if (i_this->field_0x141a > 0x1000) {
            i_this->field_0x141a = 0x1000;
        } else if (i_this->field_0x141a < -0x1000) {
            i_this->field_0x141a = -0x1000;
        }

        sp174.x = actor->current.pos.x - player->current.pos.x;
        sp174.z = actor->current.pos.z - player->current.pos.z;

        cLib_addCalcAngleS2(&i_this->field_0x1418, (i_this->field_0x141a + cM_atan2s(sp174.x, sp174.z)), 2, 0x1000);

        cMtx_YrotS(*calc_mtx, i_this->field_0x1418);
        sp174.x = 0.0f;
        if (i_this->action == ACTION_LURE_HIT && mgfish != NULL && mgfish->mCurAction >= 52) {
            sp174.y = 50.0f + (80.0f + AREG_F(10));
        } else {
            sp174.y = 80.0f + AREG_F(10);
        }
        sp174.z = -i_this->field_0x141c;
        MtxPosition(&sp174, &sp15C);
        sp15C += player->current.pos;
        sp174 = actor->current.pos - sp15C;

        s16 spC = (s16)cM_atan2s(sp174.x, sp174.z);
        f32 sp3C;
        if (i_this->field_0x140c < (20.0f + AREG_F(19))) {
            sp3C = 700.0f;
        } else {
            sp3C = 0.0f;
        }

        f32 sp38 = 0.1f;
        if (i_this->field_0x1407 != 0) {
            cLib_addCalc2(&i_this->field_0x142c, sp3C, 0.3f, 200.0f);
            sp38 = 0.3f;
        } else {
            cLib_addCalc2(&i_this->field_0x142c, sp3C, 0.2f, 200.0f);
        }

        cMtx_YrotS(*calc_mtx, spC);
        sp174.x = 0.0f;
        sp174.y = 0.0f;
        sp174.z = i_this->field_0x142c;
        MtxPosition(&sp174, &sp168);
        sp15C += sp168;

        sp174 = actor->current.pos - player->current.pos;
        cMtx_YrotS(*calc_mtx, cM_atan2s(sp174.x, sp174.z));
        sp174.x = 0.0f;
        sp174.y = (100.0f * i_this->field_0x1410);
        sp174.z = (200.0f * i_this->field_0x1410);
        MtxPosition(&sp174, &sp168);

        cLib_addCalc2(&i_this->play_cam_eye.x, sp15C.x, sp38, fabsf(sp168.x));
        cLib_addCalc2(&i_this->play_cam_eye.y, sp15C.y, sp38, fabsf(sp168.y));
        cLib_addCalc2(&i_this->play_cam_eye.z, sp15C.z, sp38, fabsf(sp168.z));

        sp150 = actor->current.pos;
        if (i_this->lure_type == MG_LURE_SP && sp150.y < (i_this->field_0x590 - 100.0f)) {
            sp150.y = (i_this->field_0x590 - 100.0f);
        }

        sp174.x = 0.0f;
        sp174.y = 100.0f * i_this->field_0x1410;
        sp174.z = 200.0f * i_this->field_0x1410;
        MtxPosition(&sp174, &sp168);
        sp150 += i_this->field_0x1434;

        if (i_this->field_0x1407 > 100) {
            i_this->play_cam_center = sp150;
            i_this->field_0x1407 = 20;
        } else {
            cLib_addCalc2(&i_this->play_cam_center.x, sp150.x, 0.15f + NREG_F(4), fabsf(sp168.x));
            cLib_addCalc2(&i_this->play_cam_center.y, sp150.y, 0.25f + NREG_F(5), fabsf(sp168.y));
            cLib_addCalc2(&i_this->play_cam_center.z, sp150.z, 0.15f + NREG_F(4), fabsf(sp168.z));
        }

        cLib_addCalc2(&i_this->field_0x1410, 1.0f, 1.0f, 0.05f + NREG_F(8));
        cLib_addCalc2(&i_this->field_0x1434.x, i_this->field_0x1440.x, 1.0f, (5.0f + YREG_F(17)) * i_this->field_0x1414);
        cLib_addCalc2(&i_this->field_0x1434.y, i_this->field_0x1440.y, 1.0f, (5.0f + YREG_F(17)) * i_this->field_0x1414);
        cLib_addCalc2(&i_this->field_0x1434.z, i_this->field_0x1440.z, 1.0f, (5.0f + YREG_F(17)) * i_this->field_0x1414);
        cLib_addCalc2(&i_this->field_0x1414, 1.0f, 1.0f, 0.02f + NREG_F(12));

        if (i_this->timers[2] == 0) {
            i_this->timers[2] = 5.0f + cM_rndF(10.0f);

            f32 sp34 = (55.0f - i_this->play_cam_fovy) * (0.4f + YREG_F(19));
            if (sp34 > 0.1f) {
                i_this->field_0x1440.x = cM_rndFX(sp34);
                i_this->field_0x1440.y = cM_rndFX(sp34);
                i_this->field_0x1440.z = cM_rndFX(sp34);
                i_this->field_0x1414 = 0.0f;
            }
        }
        break;
    }
    case 10: {
        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        sp174.x = (25.0f + (100.0f + DREG_F(0))) - 50.0f;
        sp174.y = (10.0f + DREG_F(1)) - 10.0f;
        sp174.z = 40.0f + (50.0f + (30.0f + (100.0f + DREG_F(2))));
        MtxPosition(&sp174, &i_this->play_cam_eye);
        i_this->play_cam_eye += player->current.pos;

        f32 sp30 = 0.0f;
        if (mgfish->mJointScale > 0.5f) {
            sp30 = mgfish->mJointScale - 0.5f;
        }

        sp174.x = 10.0f + (-30.0f + DREG_F(3));
        sp174.z = DREG_F(5);
        sp174.y = (5.0f + (-35.0f + DREG_F(11))) - ((60.0f + DREG_F(12)) * sp30);
        if (i_this->lure_type == MG_LURE_SP) {
            sp174.y += 13.0f;
        }
        MtxPosition(&sp174, &sp150);
        sp150.x += player->current.pos.x;
        sp150.z += player->current.pos.z;
        sp150.y += actor->current.pos.y;
        if (sp150.y < player->current.pos.y + DREG_F(16)) {
            sp150.y = player->current.pos.y + DREG_F(16);
        }

        if (i_this->play_cam_timer == 0) {
            i_this->play_cam_center = sp150;
            camera->mCamera.SetTrimSize(1);
            i_this->play_cam_fovy = 20.0f;
        } else {
            cLib_addCalc2(&i_this->play_cam_center.x, sp150.x, 0.1f, 10.0f);
            cLib_addCalc2(&i_this->play_cam_center.y, sp150.y, 0.1f, 10.0f);
            cLib_addCalc2(&i_this->play_cam_center.z, sp150.z, 0.1f, 10.0f);
        }

        cLib_addCalc2(&i_this->play_cam_fovy, 40.0f + DREG_F(6), 0.05f, 1.0f);

        f32 sp2C = -20.0f + (20.0f * cM_ssin(i_this->field_0x578 * 700));
        actor->eyePos.y = actor->current.pos.y + sp2C;
        if (TREG_S(7) != 0) {
            camera_class* sp28 = dComIfGp_getCamera(0);
            actor->eyePos = sp28->lookat.eye;
        }
        i_this->field_0xf78 = 0.05f;
        break;
    }
    case 11:
        cLib_addCalc2(&i_this->play_cam_fovy, 55.0f + DREG_F(6), 0.05f, 1.0f);

        cMtx_YrotS(*calc_mtx, daAlink_getAlinkActorClass()->getFishingRodAngleY());
        sp174.x = -150.0f + DREG_F(0);
        sp174.y = 50.0f + DREG_F(1);
        sp174.z = 150.0f + DREG_F(2);
        MtxPosition(&sp174, &sp15C);
        sp15C += player->current.pos;

        sp174.x = -50.0f + DREG_F(3);
        sp174.y = DREG_F(4);
        sp174.z = 50.0f + DREG_F(5);
        MtxPosition(&sp174, &sp150);
        sp150 += player->current.pos;

        if (i_this->play_cam_timer == 0) {
            i_this->play_cam_eye = sp15C;
            i_this->play_cam_center = sp150;
        } else {
            cLib_addCalc2(&i_this->play_cam_center.x, sp150.x, 0.1f, 10.0f);
            cLib_addCalc2(&i_this->play_cam_center.y, sp150.y, 0.1f, 10.0f);
            cLib_addCalc2(&i_this->play_cam_center.z, sp150.z, 0.1f, 10.0f);
        }
        if (i_this->play_cam_timer >= (s16)(XREG_S(4) + 68)) {
            i_this->play_cam_mode = 2;
            i_this->play_cam_timer = 20;
            i_this->field_0x1418 = daAlink_getAlinkActorClass()->shape_angle.y;
            i_this->field_0x1410 = 0.0f;
            camera->mCamera.SetTrimSize(1);
        }

        i_this->field_0xf78 = 0.05f;
        i_this->timers[1] = 20;
        break;
    case 20:
        i_this->play_cam_fovy = 40.0f + DREG_F(18);

        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        sp174.x = 60.0f + DREG_F(10);
        sp174.y = 50.0f + DREG_F(11);
        sp174.z = 110.0f + DREG_F(12);
        MtxPosition(&sp174, &sp15C);
        sp15C += player->current.pos;

        if (henna != NULL) {
            sp174.x = 20.0f + DREG_F(13);
        } else {
            sp174.x = 0.0f;
        }
        sp174.y = 45.0f + DREG_F(14);
        sp174.z = DREG_F(15);
        MtxPosition(&sp174, &sp150);
        sp150 += player->current.pos;

        if (i_this->play_cam_timer == 0) {
            i_this->play_cam_eye = sp15C;
            i_this->play_cam_center = sp150;
        } else {
            cLib_addCalc2(&i_this->play_cam_center.x, sp150.x, 0.1f, 10.0f);
            cLib_addCalc2(&i_this->play_cam_center.y, sp150.y, 0.1f, 10.0f);
            cLib_addCalc2(&i_this->play_cam_center.z, sp150.z, 0.1f, 10.0f);
        }
    
        if (i_this->timers[2] != 0) {
            i_this->play_cam_timer = 15;
        }
        break;
    case 30:
        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        sp174.x = (25.0f + (100.0f + DREG_F(0))) - 50.0f;
        sp174.y = (10.0f + DREG_F(1)) - 10.0f;
        sp174.z = 40.0f + (50.0f + (30.0f + (100.0f + DREG_F(2))));
        MtxPosition(&sp174, &i_this->play_cam_eye);
        i_this->play_cam_eye += player->current.pos;

        sp174.x = 10.0f + (-30.0f + DREG_F(3));
        sp174.z = DREG_F(5);
        sp174.y = 5.0f + (-35.0f + DREG_F(11));
        MtxPosition(&sp174, &sp150);
        sp150.x += player->current.pos.x;
        sp150.z += player->current.pos.z;
        sp150.y += actor->current.pos.y;
        if (sp150.y < (player->current.pos.y + DREG_F(16))) {
            sp150.y = (player->current.pos.y + DREG_F(16));
        }

        if (i_this->play_cam_timer <= 2) {
            i_this->play_cam_center = sp150;
            camera->mCamera.SetTrimSize(1);
            i_this->play_cam_fovy = 20.0f;
        } else {
            cLib_addCalc2(&i_this->play_cam_center.x, sp150.x, 0.1f, 10.0f);
            cLib_addCalc2(&i_this->play_cam_center.y, sp150.y, 0.1f, 10.0f);
            cLib_addCalc2(&i_this->play_cam_center.z, sp150.z, 0.1f, 10.0f);
        }

        cLib_addCalc2(&i_this->play_cam_fovy, 40.0f + DREG_F(6), 0.05f, 1.0f);

        if (i_this->play_cam_timer >= BREG_S(7) + 24) {
            cLib_addCalc0(&i_this->field_0x1410, 1.0f, 20.0f);
            if (i_this->play_cam_timer == 35) {
                mDoAud_seStart(Z2SE_AL_FISH_CATCH, NULL, 0, 0);
            }
        }

        i_this->field_0xf78 = 0.05f;
        break;
    case 900:
        if (!actor->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
            actor->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }

        camera->mCamera.Stop();
        i_this->play_cam_mode = 901;
        i_this->play_cam_timer = 0;
        i_this->field_0x146d = 0;
        i_this->play_cam_fovy = dComIfGd_getView()->fovy;
        camera->mCamera.SetTrimSize(1);

        player->changeOriginalDemo();
        player->changeDemoMode(1, 1, 0, 0);

        sp174.x = camera0->lookat.eye.x - camera0->lookat.center.x;
        sp174.y = camera0->lookat.eye.y - camera0->lookat.center.y;
        sp174.z = camera0->lookat.eye.z - camera0->lookat.center.z;

        i_this->field_0x1400 = cM_atan2s(sp174.x, sp174.z) - player->shape_angle.y;
        if (i_this->field_0x1400 >= 0) {
            i_this->field_0x1402 = 6000;
        } else {
            i_this->field_0x1402 = -6000;
        }

        i_this->field_0x1404 = -cM_atan2s(sp174.y, JMAFastSqrt(SQUARE(sp174.x) + SQUARE(sp174.z)));
        i_this->field_0x1464 = sp174.abs();
        i_this->play_cam_center_target = camera0->lookat.center;
    case 901:
        if (daAlink_getAlinkActorClass()->checkCanoeRide()) {
            fopAc_ac_c* sp24 = fopAcM_SearchByID(i_this->boat_actor_id);
            cLib_addCalc0(&sp24->speedF, 1.0f, 1.0f + TREG_F(11));
        }

        cLib_addCalcAngleS2(&i_this->field_0x1400, i_this->field_0x1402, 4, 3000);
        cLib_addCalcAngleS2(&i_this->field_0x1404, -1000, 4, 1000);
        cLib_addCalc2(&i_this->field_0x1464, 400.0f + VREG_F(19), 0.25f, 200.0f);
        cLib_addCalc2(&i_this->play_cam_fovy, 30.0f + TREG_F(12), 0.25f, 10.0f);

        cMtx_YrotS(*calc_mtx, (player->shape_angle.y + i_this->field_0x1400));
        cMtx_XrotM(*calc_mtx, i_this->field_0x1404);
        sp174.x = 0.0f;
        sp174.y = 0.0f;
        sp174.z = i_this->field_0x1464;
        MtxPosition(&sp174, &i_this->play_cam_eye);
        i_this->play_cam_eye += i_this->play_cam_center_target;
        i_this->play_cam_center = i_this->play_cam_center_target;
        i_this->play_cam_center.y += (2.0f + TREG_F(17)) * cM_ssin(i_this->play_cam_timer * 1200);

        fopAcM_OnStatus(actor, 0);
        cLib_onBit<u32>(actor->attention_info.flags, fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);

        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        if (i_this->field_0x1402 < 0) {
            sp174.x = -200.0f;
        } else {
            sp174.x = 200.0f;
        }
        sp174.y = (-50.0f + JREG_F(11));
        sp174.z = 0.0f;
        MtxPosition(&sp174, &actor->eyePos);
        actor->eyePos += player->eyePos;
        actor->attention_info.position = actor->eyePos;

        if (i_this->play_cam_timer >= 10) {
            if (i_this->field_0x146d == 0) {
                i_this->field_0x146d = 1;
                if (henna->field_0x7d5 != 0 && (henna->field_0x7d6 == 0 || (i_this->field_0x1472 & 3) == 0)) {
                    i_this->msgflow.init(actor, 0x3C3, 0, NULL);
                    henna->field_0x7d5 = 0;
                    henna->field_0x7d6 = 1;
                } else if (henna->field_0x7b8 != 0) {
                    i_this->msgflow.init(actor, 0x2C9, 0, NULL);
                    henna->field_0x6ba = 50;
                    henna->field_0x7b8 = 0;
                } else if (!dComIfGs_isEventBit((u16)dSv_event_flag_c::saveBitLabels[467])) {
                    if (i_this->field_0x1470 == 0) {
                        sp174.x = -2800.0f;
                        sp174.z = 4608.0f;
                        sp174.x -= player->current.pos.x;
                        sp174.z -= player->current.pos.z;
                        if (JMAFastSqrt(SQUARE(sp174.x) + SQUARE(sp174.z)) < 1500.0f) {
                            i_this->msgflow.init(actor, 0x3C2, 0, NULL);
                            i_this->field_0x1470++;
                        } else {
                            i_this->msgflow.init(actor, 0x3CF, 0, NULL);
                        }
                    } else {
                        i_this->msgflow.init(actor, 0x3CF, 0, NULL);
                    }
                } else {
                    i_this->field_0x1472++;
                    if (i_this->field_0x1472 == 1) {
                        i_this->msgflow.init(actor, 0x3D0, 0, NULL);
                    } else if (i_this->field_0x1472 == 2) {
                        i_this->msgflow.init(actor, 0x3D1, 0, NULL);
                    } else if (i_this->field_0x1472 == 3) {
                        i_this->msgflow.init(actor, 0x3D2, 0, NULL);
                        i_this->field_0x1472 = 0;
                    }
                }
            }

            if (i_this->msgflow.doFlow(actor, NULL, 0)) {
                camera->mCamera.Reset(i_this->play_cam_center, i_this->play_cam_eye, i_this->play_cam_fovy, 0);
                camera->mCamera.Start();
                camera->mCamera.SetTrimSize(0);
                dComIfGp_event_reset();
                daPy_getPlayerActorClass()->cancelOriginalDemo();
                i_this->play_cam_mode = 0;
                i_this->field_0x146c = 30;
                i_this->field_0x146d = 0;
            }
        }
        break;

    case 950:
        i_this->play_cam_mode = 951;
        i_this->play_cam_timer = 0;
        i_this->play_cam_fovy = 60.0f;
        i_this->play_cam_center = actor->current.pos;
        i_this->play_cam_center.y += AREG_F(7) - 25.0f;
        // fallthrough
    case 951:
        dComIfGp_setDoStatusForce(34, 0);
        i_this->field_0xf78 = 0.1f + TREG_F(19);

        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        sp174.x = AREG_F(4);
        sp174.y = (50.0f + AREG_F(5)) - 20.0f;
        sp174.z = 50.0f + AREG_F(6);
        MtxPosition(&sp174, &i_this->play_cam_eye);
        i_this->play_cam_eye += player->current.pos;

        sp150 = actor->current.pos;
        sp150.y += AREG_F(7) - 25.0f;
        cLib_addCalc2(&i_this->play_cam_center.x, sp150.x, 0.1f, 20.0f);
        cLib_addCalc2(&i_this->play_cam_center.y, sp150.y, 0.1f, 20.0f);
        cLib_addCalc2(&i_this->play_cam_center.z, sp150.z, 0.1f, 20.0f);

        if (i_this->play_cam_mode == 951) {
            static f32 old_stick_x = 0.0f;
            static f32 old_stick_sx = 0.0f;

            if (
                (mDoCPd_c::getStickX3D(PAD_1) >= 0.8f && old_stick_x < 0.8f) || (mDoCPd_c::getStickX3D(PAD_1) <= -0.8f && old_stick_x > -0.8f) 
                #if VERSION != VERSION_SHIELD_DEBUG
                || (mDoCPd_c::getSubStickX3D(PAD_1) >= 0.8f && old_stick_sx < 0.8f) || (mDoCPd_c::getSubStickX3D(PAD_1) <= -0.8f && old_stick_sx > -0.8f)
                #endif
            ) {
                int sp20 = 0;
                int sp1C = 1;
                if (dComIfGs_isEventBit((u16)dSv_event_flag_c::saveBitLabels[466])) {
                    sp20 = 1;
                }

                if (dComIfGs_isEventBit((u16)dSv_event_flag_c::saveBitLabels[469])) {
                    sp1C = 0;
                }

                if (i_this->play_cam_timer >= 15) {
                    if (mDoCPd_c::getStickX3D(PAD_1) >= 0.5f
                        #if VERSION != VERSION_SHIELD_DEBUG
                        || mDoCPd_c::getSubStickX3D(PAD_1) >= 0.5f
                        #endif
                    ) {
                        i_this->field_0xf81++;
                        if (i_this->field_0xf81 > (s8)(sp20 + 3)) {
                            i_this->field_0xf81 = sp1C;
                        }
                    } else {
                        i_this->field_0xf81--;
                        if (i_this->field_0xf81 < sp1C) {
                            i_this->field_0xf81 = (s8)(sp20 + 3);
                        }
                    }

                    mDoAud_seStart(Z2SE_SY_CURSOR_FLOOR, NULL, 0, 0);
                    i_this->msgflow.remove();
                    i_this->play_cam_timer = 0;
                }
            }

            old_stick_x = mDoCPd_c::getStickX3D(PAD_1);
            old_stick_sx = mDoCPd_c::getSubStickX(PAD_1);

            if (i_this->play_cam_timer == 1) {
                if (i_this->field_0xf81 == 0) {
                    i_this->msgflow.init(actor, 0x2C3, 0, NULL);
                } else if (i_this->field_0xf81 == 1) {
                    i_this->msgflow.init(actor, 0x2C4, 0, NULL);
                } else if (i_this->lure_type == MG_LURE_PO) {
                    i_this->msgflow.init(actor, 0x2C5, 0, NULL);
                } else if (i_this->field_0xf81 == 3) {
                    i_this->msgflow.init(actor, 0x2C6, 0, NULL);
                } else {
                    i_this->msgflow.init(actor, 0x2C7, 0, NULL);
                }
            }

            if (i_this->play_cam_timer >= 15 && mDoCPd_c::getTrigA(PAD_1)) {
                mDoAud_seStart(Z2SE_SY_ITEM_SET_X, NULL, 0, 0);
                i_this->play_cam_timer = 1;
            }

            if (i_this->play_cam_timer >= 1 && i_this->msgflow.doFlow(actor, NULL, 0) && i_this->play_cam_timer >= 15) {
                if (henna != NULL && i_this->field_0xf81 == 4) {
                    i_this->play_cam_mode = 1058;
                    i_this->play_cam_timer = 0;
                } else {
                    i_this->play_cam_mode = 1053;
                }
            }
        }
        break;
    case 1000:
        if (!actor->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
            actor->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }

        i_this->play_cam_mode = 1001;
        i_this->play_cam_timer = 0;
        i_this->msgflow.init(actor, 0x2CA, 0, NULL);
        camera->mCamera.Stop();
        camera->mCamera.SetTrimSize(1);
    case 1001:
        if (daAlink_getAlinkActorClass()->checkCanoeRide()) {
            fopAc_ac_c* boat = fopAcM_SearchByID(i_this->boat_actor_id);
            cLib_addCalc0(&boat->speedF, 1.0f, 1.0f + TREG_F(11));
        }

        if (i_this->msgflow.doFlow(actor, NULL, 0)) {
            i_this->play_cam_timer = 0;
            i_this->play_cam_mode = 1002;
        }
        break;
    case 1002:
        if (i_this->play_cam_timer >= 1) {
            u8 sp8 = 0;
            if (i_this->play_cam_timer == 1) {
                if (dMsgObject_getSelectCursorPos() == 0) {
                    i_this->msgflow.init(actor, 0x2CB, 0, NULL);
                } else if (dMsgObject_getSelectCursorPos() == 1) {
                    i_this->msgflow.init(actor, 0x2CC, 0, NULL);
                } else if (dMsgObject_getSelectCursorPos() == 2) {
                    i_this->msgflow.init(actor, 0x2CD, 0, NULL);
                } else {
                    sp8 = 1;
                }
            }

            if (sp8 || i_this->msgflow.doFlow(actor, NULL, 0)) {
                dComIfGp_event_reset();
                camera->mCamera.Start();
                camera->mCamera.SetTrimSize(0);
                i_this->play_cam_mode = 0;
                i_this->field_0x146c = 30;
            }
        }
        break;
    case 1050:
        if (!actor->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
            actor->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }

        i_this->play_cam_mode = 1051;
        i_this->play_cam_timer = 0;

        if (henna != NULL) {
            i_this->msgflow.init(actor, 0x2BF, 0, NULL);
        } else {
            i_this->msgflow.init(actor, 0x2C1, 0, NULL);
        }

        camera->mCamera.Stop();
        camera->mCamera.SetTrimSize(1);
        // fallthrough
    case 1051:
        if (daAlink_getAlinkActorClass()->checkCanoeRide()) {
            fopAc_ac_c* boat = fopAcM_SearchByID(i_this->boat_actor_id);
            cLib_addCalc0(&boat->speedF, 1.0f, 1.0f + TREG_F(11));
        }

        if (i_this->msgflow.doFlow(actor, NULL, 0)) {
            i_this->play_cam_timer = 0;
            i_this->play_cam_mode = 1052;
        }
        break;
    case 1052:
        if (i_this->play_cam_timer >= 1) {
            if (i_this->play_cam_timer == 1) {
                if (dMsgObject_getSelectCursorPos() == 0) {
                    i_this->play_cam_mode = 1053;
                } else if (dMsgObject_getSelectCursorPos() == 1) {
                    i_this->play_cam_mode = 950;
                } else {
                    if (henna != NULL) {
                        i_this->msgflow.init(actor, 0x2C0, 0, NULL);
                    } else {
                        i_this->msgflow.init(actor, 0x2C2, 0, NULL);
                    }

                    i_this->play_cam_mode = 1055;
                }
            }

            i_this->msgflow.doFlow(actor, NULL, 0);
        }
        break;
    case 1053:
        if (i_this->msgflow.doFlow(actor, NULL, 0)) {
            dComIfGp_event_reset();
            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);
            i_this->play_cam_mode = 0;
            i_this->field_0x146c = 30;
        }
        break;
    case 1055:
        if (i_this->msgflow.doFlow(actor, NULL, 0)) {
            if (dMsgObject_getSelectCursorPos() != 0) {
                dComIfGp_event_reset();
                camera->mCamera.Start();
                camera->mCamera.SetTrimSize(0);
                i_this->field_0x146c = 30;
                i_this->play_cam_mode = 0;
            } else {
                dStage_changeScene(2, 0.0f, 0, 0, 0, -1);
                data_80450C99 = 1;
            }
        }
        break;
    case 1058:
        if (i_this->play_cam_timer >= 5) {
            if (i_this->play_cam_timer == 5) {
                i_this->msgflow.init(actor, 0x2C8, 0, NULL);
                daAlink_getAlinkActorClass()->changeFishGetFace(3);
            }

            if (i_this->msgflow.doFlow(actor, NULL, 0)) {
                dComIfGp_event_reset();
                camera->mCamera.Start();
                camera->mCamera.SetTrimSize(0);
                i_this->play_cam_mode = 0;
                i_this->field_0x146c = 30;
                dComIfGs_offEventBit((u16)dSv_event_flag_c::saveBitLabels[466]);
                i_this->field_0xf81 = 1;
            }
        }
        break;
    }

    if (i_this->play_cam_mode != 0) {
        i_this->play_cam_timer++;
        if (i_this->play_cam_timer > 10000) {
            i_this->play_cam_timer = 10000;
        }

        if (i_this->play_cam_mode < 1000) {
            cXyz sp138(i_this->play_cam_eye);
            cXyz sp12C(i_this->play_cam_center);
            if (sp70 != 0) {
                dBgS_LinChk sp180;
                sp180.SetRope();

                cXyz sp120(i_this->play_cam_eye);
                cXyz sp114;

                sp174 = sp120 - i_this->play_cam_center;
                s16 spA = (s16)cM_atan2s(sp174.x, sp174.z);
                cMtx_YrotS(*calc_mtx, spA);
                sp174.x = 0.0f;
                sp174.y = 0.0f;
                sp174.z = 100.0f + hREG_F(10);
                MtxPosition(&sp174, &sp168);
                sp120 += sp168;

                sp114 = i_this->play_cam_eye - (sp168 * (5.0f + nREG_F(5)));

                sp180.Set(&sp114, &sp120, actor);
                if (dComIfG_Bgsp().LineCross(&sp180)) {
                    sp138 = sp180.GetCross() - sp168;
                    sp174 = sp138 - player->eyePos;

                    f32 sp10 = sp174.abs();
                    if (sp10 < 100.0f + nREG_F(17)) {
                        sp10 = (100.0f + nREG_F(17)) - sp10;
                        sp138.y += sp10;
                    }
                }
            }

            camera->mCamera.Set(sp12C, sp138, i_this->play_cam_fovy, 0);
        }
    }
}

static void play_camera_u(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
    fopAc_ac_c* mgfish_a = fopAcM_SearchByID(i_this->mg_fish_id);
    camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));

    i_this->field_0x13b0 = i_this->rod_substick_y;
    i_this->field_0x13ac = i_this->rod_substick_x;

    cXyz spCC;
    cXyz spC0;
    cXyz spB4;
    cXyz spA8;

    int sp18 = 0;
    int sp14 = 0;

    switch (i_this->play_cam_mode) {
    case 0:
        break;
    case 1: {
        i_this->play_cam_mode = 2;
        camera->mCamera.Stop();
        i_this->play_cam_timer = 0;
        dMw_offMenuRing();
        i_this->play_cam_fovy = 55.0f;
        
        camera_class* sp10 = (camera_class*)dComIfGp_getCamera(0);
        camera->mCamera.SetTrimSize(1);

        i_this->play_cam_eye = sp10->lookat.eye;
        i_this->play_cam_center = sp10->lookat.center;
        i_this->field_0x1410 = 1000.0f;
    }
        /* fallthrough */
    case 2:
        sp14 = 1;
        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        spCC.x = 0.0f;
        spCC.y = 250.0f + WREG_F(0);
        spCC.z = -170.0f + WREG_F(1);
        MtxPosition(&spCC, &spB4);
        spB4 += player->current.pos;

        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        spCC.x = (-50.0f + WREG_F(10)) * (i_this->field_0x150c * i_this->field_0x1508);
        spCC.y = 200.0f + (i_this->field_0x1508 * (50.0f + WREG_F(11)));
        spCC.z = 0.0f;
        MtxPosition(&spCC, &spA8);
        spA8 += player->current.pos;

        spCC.x = 0.0f;
        spCC.y = i_this->field_0x1508 * (-100.0f + WREG_F(13));
        spCC.z = i_this->field_0x1508 * (-100.0f + WREG_F(14));
        MtxPosition(&spCC, &spC0);
        spB4 += spC0;

        f32 var_f31;
        if (i_this->play_cam_timer < 10) {
            var_f31 = 200.0f;
        } else {
            var_f31 = 10.0f;
        }

        cLib_addCalc2(&i_this->play_cam_eye.x, spB4.x, 0.2f, var_f31);
        cLib_addCalc2(&i_this->play_cam_eye.y, spB4.y, 0.2f, var_f31);
        cLib_addCalc2(&i_this->play_cam_eye.z, spB4.z, 0.2f, var_f31);
        cLib_addCalc2(&i_this->play_cam_center.x, spA8.x, 0.2f, var_f31);
        cLib_addCalc2(&i_this->play_cam_center.y, spA8.y, 0.2f, var_f31);
        cLib_addCalc2(&i_this->play_cam_center.z, spA8.z, 0.2f, var_f31);
        cLib_addCalc2(&i_this->play_cam_fovy, 55.0f, 0.2f, 2.0f);
        break;
    case 3:
        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        spCC.x = 0.0f;
        spCC.y = 250.0f + WREG_F(0);
        spCC.z = -170.0f + WREG_F(1);
        MtxPosition(&spCC, &i_this->play_cam_eye_target);
        i_this->play_cam_eye_target += player->current.pos;
    
        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        spCC.x = 0.0f;
        spCC.y = 200.0f + (50.0f + WREG_F(11));
        spCC.z = 0.0f;
        MtxPosition(&spCC, &i_this->play_cam_center_target);
        i_this->play_cam_center_target += player->current.pos;

        i_this->play_cam_eye_speed.x = fabsf(i_this->play_cam_eye_target.x - i_this->play_cam_eye.x);
        i_this->play_cam_eye_speed.y = fabsf(i_this->play_cam_eye_target.y - i_this->play_cam_eye.y);
        i_this->play_cam_eye_speed.z = fabsf(i_this->play_cam_eye_target.z - i_this->play_cam_eye.z);
        i_this->play_cam_center_speed.x = fabsf(i_this->play_cam_center_target.x - i_this->play_cam_center.x);
        i_this->play_cam_center_speed.y = fabsf(i_this->play_cam_center_target.y - i_this->play_cam_center.y);
        i_this->play_cam_center_speed.z = fabsf(i_this->play_cam_center_target.z - i_this->play_cam_center.z);

        i_this->field_0x1410 = 0.0f;
        i_this->play_cam_mode = 4;
        i_this->play_cam_timer = 0;
        daAlink_getAlinkActorClass()->fishingCastWaitAnimeStart();
        /* fallthrough */
    case 4:
        cam_3d_morf(i_this, 0.1f);
        cLib_addCalc2(&i_this->field_0x1410, 0.1f, 1.0f, 0.02f);

        if (i_this->play_cam_timer > 10) {
            i_this->play_cam_mode = 2;
        }

        cLib_addCalc2(&i_this->play_cam_fovy, 55.0f, 0.2f, 2.0f);
        break;
    case 5:
        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        spCC.x = 0.0f;
        spCC.y = 150.0f + ZREG_F(2);
        spCC.z = 150.0f + ZREG_F(3);
        MtxPosition(&spCC, &i_this->play_cam_eye_target);
        i_this->play_cam_eye_target += player->current.pos;
        i_this->play_cam_center_target = i_this->hook_pos;

        if (i_this->play_cam_center_target.y < i_this->field_0x590) {
            i_this->play_cam_center_target.y = i_this->field_0x590;
        }

        i_this->play_cam_eye_speed.x = fabsf(i_this->play_cam_eye_target.x - i_this->play_cam_eye.x);
        i_this->play_cam_eye_speed.y = fabsf(i_this->play_cam_eye_target.y - i_this->play_cam_eye.y);
        i_this->play_cam_eye_speed.z = fabsf(i_this->play_cam_eye_target.z - i_this->play_cam_eye.z);
        i_this->play_cam_center_speed.x = fabsf(i_this->play_cam_center_target.x - i_this->play_cam_center.x);
        i_this->play_cam_center_speed.y = fabsf(i_this->play_cam_center_target.y - i_this->play_cam_center.y);
        i_this->play_cam_center_speed.z = fabsf(i_this->play_cam_center_target.z - i_this->play_cam_center.z);

        i_this->field_0x1410 = 0.0f;
        i_this->play_cam_mode = 6;
        i_this->play_cam_timer = 0;
        /* fallthrough */
    case 6:
        cam_3d_morf(i_this, 0.1f);
        cLib_addCalc2(&i_this->field_0x1410, 0.1f, 1.0f, 0.01f);

        if (i_this->play_cam_timer > AREG_S(3) + 20) {
            i_this->play_cam_mode = 7;
        }
        goto block_41;  // fakematch? whats going on here
    case 7: {
        spA8 = i_this->hook_pos;
        spA8.y += 20.0f + AREG_F(11);
        if (spA8.y < i_this->field_0x590) {
            spA8.y = i_this->field_0x590;
        }
        spA8.y = (spA8.y + ((0.1f + hREG_F(13)) * (player->current.pos.y - i_this->hook_pos.y)));

        f32 temp_f31 = 5.0f;
        cLib_addCalc2(&i_this->play_cam_center.x, spA8.x, 0.1f, temp_f31);
        cLib_addCalc2(&i_this->play_cam_center.y, spA8.y, 0.1f, temp_f31);
        cLib_addCalc2(&i_this->play_cam_center.z, spA8.z, 0.1f, temp_f31);
        // fallthrough
    }
    block_41: {
        if (nREG_S(6) == (int)daAlink_getAlinkActorClass()->getFishingReelFrame()) {
            daAlink_getAlinkActorClass()->fishingCastWaitAnimeStop();
        }

        i_this->field_0x140c = 20.0f + (40.0f + BREG_F(15));
        spCC.z = (i_this->hook_pos - i_this->play_cam_eye).abs();

        i_this->field_0x140c -= spCC.z * (0.1f + BREG_F(16));
        if (i_this->field_0x140c > 40.0f) {
            i_this->field_0x140c = 40.0f;
        } else if (i_this->field_0x140c < 15.0f) {
            i_this->field_0x140c = 15.0f;
        }

        cLib_addCalc2(&i_this->play_cam_fovy, i_this->field_0x140c, 0.05f, 2.0f);
        break;
    }
    case 10:
        daAlink_getAlinkActorClass()->fishingCastWaitAnimeStop();
        i_this->play_cam_mode = 11;
        i_this->play_cam_timer = 0;
    case 11: {
        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        spCC.x = ZREG_F(16) - 100.0f;
        spCC.y = 50.0f + (150.0f + ZREG_F(17));
        spCC.z = (50.0f + ZREG_F(18)) - 200.0f;
        MtxPosition(&spCC, &spB4);
        spB4 += player->current.pos;
        spA8 = actor->current.pos;

        f32 temp_f31_2 = 50.0f + ZREG_F(19);
        cLib_addCalc2(&i_this->play_cam_eye.x, spB4.x, 0.2f, temp_f31_2);
        cLib_addCalc2(&i_this->play_cam_eye.y, spB4.y, 0.2f, temp_f31_2);
        cLib_addCalc2(&i_this->play_cam_eye.z, spB4.z, 0.2f, temp_f31_2);
        cLib_addCalc2(&i_this->play_cam_center.x, spA8.x, 0.05f, temp_f31_2);
        cLib_addCalc2(&i_this->play_cam_center.y, 200.0f + (i_this->field_0x590 + ZREG_F(14)), 0.05f, temp_f31_2);
        cLib_addCalc2(&i_this->play_cam_center.z, spA8.z, 0.05f, temp_f31_2);
        cLib_addCalc2(&i_this->play_cam_fovy, 55.0f, 0.1f, 10.0f);
        break;
    }
    case 20:
    case 21: {
        if (!actor->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
            actor->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }

        csXyz pl_angle(i_this->field_0x14fc, player->shape_angle.y, 0);
        daPy_getPlayerActorClass()->setPlayerPosAndAngle(&player->current.pos, &pl_angle);
        i_this->field_0xf78 = 0.1f;

        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        spCC.x = -80.0f + DREG_F(0);
        spCC.y = 130.0f + DREG_F(1);
        spCC.z = 130.0f + DREG_F(2);
        MtxPosition(&spCC, &spB4);
        spB4 += player->current.pos;

        cLib_addCalc2(&i_this->play_cam_eye.x, spB4.x, 0.2f, 100.0f * i_this->field_0x1410);
        cLib_addCalc2(&i_this->play_cam_eye.y, spB4.y, 0.2f, 100.0f * i_this->field_0x1410);
        cLib_addCalc2(&i_this->play_cam_eye.z, spB4.z, 0.2f, 100.0f * i_this->field_0x1410);

        spCC.x = 0.0f;
        spCC.y = 0.0f;
        spCC.z = -25.0f + KREG_F(10);
        MtxPosition(&spCC, &spA8);
        spA8 += mgfish_a->current.pos;

        if (i_this->play_cam_mode == 20 && i_this->play_cam_timer <= 1) {
            camera->mCamera.SetTrimSize(1);
        }

        cLib_addCalc2(&i_this->play_cam_center.x, spA8.x, 0.2f, 100.0f * i_this->field_0x1410);
        cLib_addCalc2(&i_this->play_cam_center.y, spA8.y, 0.2f, 100.0f * i_this->field_0x1410);
        cLib_addCalc2(&i_this->play_cam_center.z, spA8.z, 0.2f, 100.0f * i_this->field_0x1410);

        if (i_this->play_cam_timer > 5) {
            cLib_addCalc2(&i_this->field_0x1410, 1.0f, 1.0f, 0.02f + NREG_F(8));
            cLib_addCalc2(&i_this->play_cam_fovy, 55.0f, 0.05f, 1.0f);
        }

        if (henna != NULL && i_this->play_cam_mode == 20 && i_this->play_cam_timer <= 5) {
            dBgS_GndChk sp19C;
            dBgS_ObjGndChk_Spl sp148;

            spCC.x = 200.0f + DREG_F(15);
            spCC.y = 300.0f + DREG_F(16);
            spCC.z = -40.0f + DREG_F(17);
            MtxPosition(&spCC, &spC0);
            spC0 += player->current.pos;

            sp19C.SetPos(&spC0);
            sp148.SetPos(&spC0);

            spC0.y = dComIfG_Bgsp().GroundCross(&sp19C);
            if (spC0.y > dComIfG_Bgsp().GroundCross(&sp148) && fabsf(spC0.y - player->current.pos.y) < 50.0f) {
                henna->actor.current.pos = spC0;
            }
        }

        actor->eyePos.y = actor->current.pos.y - 20.0f;

        mg_fish_class* mgfish = (mg_fish_class*)mgfish_a;
        if (i_this->play_cam_mode == 21 && i_this->play_cam_timer == 3 && mgfish->mGedouKind == 10) {
            spC0.set(3277.0f, player->current.pos.y, 13034.0f);
            daPy_getPlayerActorClass()->setPlayerPosAndAngle(&spC0, -0x5E4D, 0);
        }

        if (i_this->play_cam_mode == 21 && i_this->play_cam_timer == 4) {
            cMtx_YrotS(*calc_mtx, player->shape_angle.y);
            if (mgfish->mGedouKind == 10) {
                spCC.x = 300.0f;
                spCC.y = 250.0f;
                spCC.z = -200.0f;
                MtxPosition(&spCC, &i_this->play_cam_eye);
                i_this->play_cam_eye += player->current.pos;

                spCC.x = 0.0f;
                spCC.y = 200.0f;
                spCC.z = 0.0f;
                MtxPosition(&spCC, &i_this->play_cam_center);
                i_this->play_cam_center += player->current.pos;

                spCC.x = 0.0f;
                spCC.y = 0.0f;
                spCC.z = 200.0f;
                MtxPosition(&spCC, &spC0);
                spC0 += player->current.pos;
            } else {
                spCC.x = 0.0f;
                spCC.y = 250.0f;
                spCC.z = -400.0f;
                MtxPosition(&spCC, &i_this->play_cam_eye);
                i_this->play_cam_eye += player->current.pos;

                spCC.x = 0.0f;
                spCC.y = 200.0f;
                spCC.z = 0.0f;
                MtxPosition(&spCC, &i_this->play_cam_center);
                i_this->play_cam_center += player->current.pos;
            }

            dComIfGp_event_reset();
            sp18 = 1;

            if (mgfish->mCaughtType != MG_CATCH_FISH) {
                if (mgfish->mCaughtType == MG_CATCH_SG) {
                    e_sg_class* spC = (e_sg_class*)fopAcM_SearchByID(mgfish->mSkullfishId);
                    spC->mInactive = 0;
                    spC->current.pos = mgfish_a->current.pos;
                    spC->old.pos = spC->current.pos;
                    spC->mAction = 3;
                    spC->mMode = 0;
                    spC->field_0x567 = 1;
                } else if (mgfish->mCaughtType == MG_CATCH_BB) {
                    mgfish->mSound.startCreatureSound(Z2SE_AL_FISH_JUMP_DOWN, 0, -1);
                }

                mgfish->mCaughtType = MG_CATCH_FISH;
                mgfish_a->current.pos = mgfish_a->home.pos;
                mgfish->mCurAction = 0;
                mgfish->mActionPhase = 0;
                mgfish->field_0x659 = 0;
            } else {
                cMtx_YrotS(*calc_mtx, player->shape_angle.y);
                spCC.x = 0.0f;
                spCC.y = 100.0f;
                spCC.z = 100.0f;
                MtxPosition(&spCC, &mgfish_a->current.pos);
                mgfish_a->current.pos += player->current.pos;

                mgfish->mCurAction = 0x35;
                mgfish->mActionPhase = 10;
                mgfish->field_0x659 = 0;
                mgfish->field_0x624[1] = 0;

                if (mgfish->mGedouKind == 10) {
                    mgfish->field_0x624[2] = 300;
                } else {
                    mgfish->field_0x624[2] = 0;
                }

                mgfish->field_0x5f6 = player->shape_angle.y;
            }

            i_this->action = ACTION_UKI_READY;
            i_this->field_0x10a5 = 0;
            daAlink_getAlinkActorClass()->endFishingCastWait();
            i_this->field_0x1094 = 0.0f;
            i_this->field_0x6e0 = 0.0f;
            i_this->field_0xf64 = -30.0f;
            daAlink_getAlinkActorClass()->changeFishGetFace(0);
        }
        break;
    }
    case 90:
        sp18 = 1;
        break;
    case 1000:
        if (!actor->eventInfo.checkCommandDemoAccrpt()) {
            fopAcM_orderPotentialEvent(actor, 2, 0xFFFF, 0);
            actor->eventInfo.onCondition(dEvtCnd_CANDEMO_e);
            return;
        }

        i_this->play_cam_mode = 1001;
        i_this->play_cam_timer = 0;
        i_this->msgflow.init(actor, 0x3DA, 0, NULL);

        camera->mCamera.Stop();
        camera->mCamera.SetTrimSize(1);
    case 1001:
        if (i_this->msgflow.doFlow(actor, NULL, 0)) {
            dComIfGp_event_reset();
            i_this->play_cam_mode = 0;

            camera->mCamera.Start();
            camera->mCamera.SetTrimSize(0);

            if (dMsgObject_getSelectCursorPos() != 0) {
                henna->cam_mode = 300;
            }
        }
        break;
    }
        
    if (sp18 != 0) {
        cMtx_YrotS(*calc_mtx, player->shape_angle.y);
        spCC.x = 0.0f;
        spCC.y = 100.0f + JREG_F(1);
        spCC.z = -250.0f + JREG_F(2);
        MtxPosition(&spCC, &i_this->play_cam_eye);
        i_this->play_cam_eye += player->current.pos;
        i_this->play_cam_center = player->current.pos;
        i_this->play_cam_center.y += 120.0f;

        i_this->play_cam_mode = 0;
        camera->mCamera.Reset(i_this->play_cam_center, i_this->play_cam_eye, i_this->play_cam_fovy, 0);
        camera->mCamera.Start();
        camera->mCamera.SetTrimSize(0);

        dMw_onMenuRing();
    }

    if (i_this->play_cam_mode != 0) {
        i_this->play_cam_timer++;
        if (i_this->play_cam_timer > 10000) {
            i_this->play_cam_timer = 10000;
        }

        if (i_this->play_cam_mode < 1000) {
            cXyz sp9C;
            cXyz sp90;
            sp9C = i_this->play_cam_center;
            sp90 = i_this->play_cam_eye;

            if (sp14 != 0) {
                dBgS_LinChk spD8;
                spD8.SetRope();

                spD8.Set(&i_this->play_cam_center, &i_this->play_cam_eye, actor);
                if (dComIfG_Bgsp().LineCross(&spD8)) {
                    sp90 = spD8.GetCross() + ((sp9C - sp90) * 0.15f);
                }
            }

            camera->mCamera.Set(sp9C, sp90, i_this->play_cam_fovy, 0);
        }
    }
}

static int dmg_rod_Execute(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;

    #if VERSION == VERSION_SHIELD_DEBUG
    if (dComIfGs_getPalLanguage() == 1) {
        data_804BBBD4 = 2;
    } else {
        data_804BBBD4 = 0;
    }
    #elif VERSION == VERSION_SHIELD
    data_804BBBD4 = 0;
    #elif REGION_PAL
    if (dComIfGs_getPalLanguage() == 0) {
        data_804BBBD4 = 2;
    } else {
        data_804BBBD4 = 0;
    }
    #elif REGION_JPN
    data_804BBBD4 = 0;
    #else
    data_804BBBD4 = 1;
    #endif

    if (i_this->vib_timer != 0) {
        i_this->vib_timer--;
        dComIfGp_getVibration().StartShock(i_this->vibmode, 1, cXyz(0.0f, 1.0f, 0.0f));
    }

    if (strcmp(dComIfGp_getStartStageName(), "F_SP127") == 0) {}

    henna = (npc_henna_class*)fopAcM_SearchByName(PROC_NPC_HENNA);
    if (henna != NULL && henna->field_0x734 != 0) {
        henna = NULL;
    }

    actor->eyePos = actor->current.pos;

    i_this->rod_stick_x = mDoCPd_c::getStickX3D(PAD_1);
    i_this->rod_stick_y = mDoCPd_c::getStickY(PAD_1);
    i_this->rod_substick_x = mDoCPd_c::getSubStickX(PAD_1);
    i_this->prev_rod_substick_y = i_this->rod_substick_y;
    i_this->rod_substick_y = mDoCPd_c::getSubStickY(PAD_1);

    i_this->reel_speed = 5.0f;
    i_this->reel_btn_flags = mDoCPd_c::getHoldB(PAD_1) | mDoCPd_c::getHoldDown(PAD_1);
    if (mDoCPd_c::getHoldDown(PAD_1)) {
        i_this->reel_speed = 15.0f;
    }

    if (i_this->rod_stick_x > 1.0f) {
        i_this->rod_stick_x = 1.0f;
    } else if (i_this->rod_stick_x < -1.0f) {
        i_this->rod_stick_x = -1.0f;
    }

    if (i_this->rod_stick_y > 1.0f) {
        i_this->rod_stick_y = 1.0f;
    } else if (i_this->rod_stick_y < -1.0f) {
        i_this->rod_stick_y = -1.0f;
    }

    i_this->field_0xf6e = 0;

    if (0.0f != i_this->field_0x590 && (g_Counter.mTimer & 15) == 0) {
        dBgS_ObjGndChk_Spl gnd_chk_spl;
        Vec pos;

        pos.x = actor->current.pos.x;
        pos.y = 500.0f + actor->current.pos.y;
        pos.z = actor->current.pos.z;
        gnd_chk_spl.SetPos(&pos);
        i_this->field_0x590 = dComIfG_Bgsp().GroundCross(&gnd_chk_spl);
    }

    i_this->field_0x578++;

    for (int i = 0; i < 10; i++) {
        if (i_this->timers[i] != 0) {
            i_this->timers[i]--;
        }
    }

    if (i_this->field_0x1009 != 0) {
        i_this->field_0x1009--;
    }

    if (i_this->field_0x1165 != 0) {
        i_this->field_0x1165--;
    }

    if (i_this->field_0x146c != 0) {
        i_this->field_0x146c--;
    }

    if (i_this->field_0x1514 != 0) {
        i_this->field_0x1514--;
    }

    rod_main(i_this);
    line_main(i_this);

    cXyz* line_posp = i_this->linemat.getPos(0);
    if (i_this->kind == MG_ROD_KIND_LURE) {
        i_this->lure_type = i_this->field_0xf81;
        lure_main(i_this);

        cXyz sp118[6];
        cXyz spFC(0.0f, 5.0f, 0.0f);
        cXyz spF0;
        cXyz spE4;

        for (int i = 0; i < 6; i++) {
            static int rodline_p[] = {4, 6, 8, 10, 12, 15};

            if (i == 5) {
                sp118[i] = i_this->mg_rod.field_0x0[rodline_p[i]];
            } else {
                sp118[i] = i_this->mg_rod.field_0x0[rodline_p[i]] + ((i_this->mg_rod.field_0x0[rodline_p[i] + 1] - i_this->mg_rod.field_0x0[rodline_p[i]]) * (0.5f + AREG_F(11)));
            }
        }

        s16 spA = 0;
        if (i_this->action <= ACTION_LURE_STANDBY) {
            spA = -0x4000;
        }

        if (i_this->play_cam_mode >= 950 && i_this->play_cam_mode < 1000) {
            spA = 0;
        }

        static f32 rodline_off[] = {0.5f, 0.4f, 0.35f, 0.3f, 0.25f, 0.2f};

        for (int i = 0; i < 6; i++, line_posp++) {
            if (i == 0) {
                *line_posp = i_this->field_0x744;
            } else {
                spF0 = sp118[i] - sp118[i - 1];

                s16 spC = -cM_atan2s(spF0.y, JMAFastSqrt(SQUARE(spF0.x) + SQUARE(spF0.z)));
                if (abs(spC) < 0x3800) {
                    i_this->field_0x71c[i].y = cM_atan2s(spF0.x, spF0.z);
                    i_this->field_0x71c[i].x = spC;

                    spC = i_this->field_0x71c[i].y - i_this->field_0x6c4;
                    if (spC < 0x4000 && spC > -0x4000) {
                        spC = 0;
                    } else {
                        spC = -0x8000;
                    }
                    i_this->field_0x71c[i].z = (spA + spC);
                }

                cMtx_YrotS(*calc_mtx, i_this->field_0x71c[i].y);
                cMtx_XrotM(*calc_mtx, i_this->field_0x71c[i].x);
                cMtx_ZrotM(*calc_mtx, i_this->field_0x71c[i].z);
                spFC.x = i_this->field_0x760 * rodline_off[i - 1];
                MtxPosition(&spFC, &spE4);
                *line_posp = sp118[i - 1] + (spE4 * rodline_off[i - 1]) * (1.0f + YREG_F(2));
            }
        }

        for (int i = 0; i < MG_ROD_UKI_LINE_LEN; i++, line_posp++) {
            *line_posp = i_this->mg_line.pos[i];
            if (i == 0) {
                *line_posp += spE4 * (0.2f + YREG_F(7));
            }
        }

        for (int i = 0; i < 6; i++) {
            MtxTrans(sp118[i].x, sp118[i].y, sp118[i].z, 0);

            f32 temp_f30 = (0.9f + XREG_F(2)) * rodline_off[i];
            MtxScale(temp_f30, temp_f30, temp_f30, 1);

            if (i == 0) {
                cMtx_YrotM(*calc_mtx, i_this->field_0x71c[1].y);
                cMtx_XrotM(*calc_mtx, i_this->field_0x71c[1].x);
                cMtx_ZrotM(*calc_mtx, i_this->field_0x71c[1].z);
            } else {
                cMtx_YrotM(*calc_mtx, i_this->field_0x71c[i].y);
                cMtx_XrotM(*calc_mtx, i_this->field_0x71c[i].x);
                cMtx_ZrotM(*calc_mtx, i_this->field_0x71c[i].z);
            }

            MtxTrans(0.0f, 6.0f + AREG_F(15), 0.0f, 1);
            cMtx_XrotM(*calc_mtx, 0x4000);
            i_this->unk_ring_model[i]->setBaseTRMtx(*calc_mtx);
        }

        play_camera(i_this);
    } else if (i_this->kind == MG_ROD_KIND_UKI) {
        f32 var_f31;
        if (fabsf(i_this->hook_pos.y - i_this->field_0x590) < 30.0f) {
            var_f31 = (25.0f + (6.0f + BREG_F(17))) * cM_ssin(i_this->field_0x1084.x);
            if (var_f31 > 0.0f) {
                var_f31 = 0.0f;
            }
        } else {
            var_f31 = 0.0f;
        }

        cLib_addCalc2(&i_this->field_0xf74, var_f31, 0.1f, 1.0f + AREG_F(13));
        uki_main(i_this);

        for (int i = 0; i < MG_ROD_UKI_LINE_LEN; i++, line_posp++) {
            *line_posp = i_this->mg_line.pos[i];
        }

        play_camera_u(i_this);
    }

    actor->health = 0;
    i_this->sound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(actor)));

    if (i_this->play_cam_mode != 0) {
        if (i_this->field_0x168c == 0) {
            i_this->field_0x168c = 1;
            dMeter2Info_offWindowAccept(2);
            dMeter2Info_offWindowAccept(3);
        }
    } else if (i_this->field_0x168c != 0) {
        i_this->field_0x168c = 0;
        dMeter2Info_onWindowAccept(2);
        dMeter2Info_onWindowAccept(3);
    }

    if (i_this->field_0x1406 != (u8)i_this->play_cam_fovy || i_this->field_0xf78 > 0.001f) {
        f32* sp30 = i_this->linemat.getSize(0);

        f32 var_f29;
        if (i_this->field_0xf78 > 0.001f) {
            var_f29 = i_this->field_0xf78;
        } else if (i_this->kind == MG_ROD_KIND_LURE) {
            var_f29 = 0.05f + YREG_F(8) + ((i_this->play_cam_fovy - 5.0f) * (0.003f + YREG_F(9)));
        } else {
            var_f29 = 0.05f + YREG_F(8) + ((i_this->play_cam_fovy - 15.0f) * (0.003f + YREG_F(9)));
        }

        int sp2C;
        if (i_this->kind == MG_ROD_KIND_LURE) {
            sp2C = MG_ROD_LURE_LINE_LEN;
        } else {
            sp2C = MG_ROD_UKI_LINE_LEN;
        }

        for (int i = 0; i < sp2C; i++, sp30++) {
            *sp30 = var_f29;
        }

        i_this->field_0xf78 = 0.0f;
    }

    i_this->field_0x1406 = i_this->play_cam_fovy;

    if (i_this->kind == MG_ROD_KIND_LURE) {
        daObjLife_c* obj_life = (daObjLife_c*)fopAcM_SearchByName(PROC_Obj_LifeContainer);
        if (obj_life != NULL) {
            if (i_this->field_0x10a9 != 0) {
                if (i_this->field_0x10a9 == 2) {
                    cXyz spD8;
                    MTXCopy(daPy_getLinkPlayerActorClass()->getRightItemMatrix(), *calc_mtx);
                    spD8.x = 15.0f + XREG_F(0);
                    spD8.y = XREG_F(1) - 27.0f;
                    spD8.z = XREG_F(2) - 3.0f;
                    MtxPosition(&spD8, &obj_life->current.pos);
                    obj_life->current.pos.y += i_this->field_0x1410;

                    actor->eyePos = obj_life->current.pos;
                    actor->eyePos.y += -20.0f + (20.0f * cM_ssin(i_this->field_0x578 * 700));

                    obj_life->shape_angle.x = 0;
                    obj_life->shape_angle.z = 0;
                } else {
                    obj_life->current.pos = actor->current.pos;
                    obj_life->current.pos.y -= 15.0f + JREG_F(7);
                    
                    fopAc_ac_c* player = dComIfGp_getPlayer(0);
                    cLib_addCalcAngleS2(&obj_life->shape_angle.y, daAlink_getAlinkActorClass()->getFishingRodAngleY(), 8, 0x200);
                    obj_life->shape_angle.z = 500.0f * cM_ssin(i_this->field_0x578 * 1300);

                    s16 sp8 = 500.0f * cM_ssin(i_this->field_0x578 * 1100);
                    if (i_this->reel_btn_flags != 0) {
                        sp8 += (s16)0x2000;
                    }

                    cLib_addCalcAngleS2(&obj_life->shape_angle.x, sp8, 15, 0x200);
                    if (actor->current.pos.y > 200.0f) {
                        actor->speedF = 20.0f;
                    }
                }

                obj_life->speed.zero();
            } else if (i_this->action == ACTION_LURE_CAST || i_this->action == ACTION_LURE_ACTION) {
                cXyz spCC = obj_life->current.pos - actor->current.pos;
                if (spCC.abs() < 50.0f) {
                    i_this->field_0x10a9 = 1;
                    obj_life->startCtrl();
                    dComIfGp_getVibration().StartShock(VIBMODE_S_POWER3, 1, cXyz(0.0f, 1.0f, 0.0f));
                }
            }
        }
    }

    if (daAlink_getAlinkActorClass()->checkFishingRodGrab(actor) && i_this->field_0x100f != 0) {
        if (i_this->field_0x100f == 1) {
            dComIfGp_setDoStatusForce(81, 0);
        } else if (i_this->field_0x100f == 2) {
            dComIfGp_set3DStatusForce(116, 8, 0);
        }
    }

    i_this->field_0x100f = 0;
    return 1;
}

static int dmg_rod_IsDelete(dmg_rod_class* i_this) {
    if (i_this->play_cam_mode != 0) {
        camera_class* camera = dComIfGp_getCamera(dComIfGp_getPlayerCameraID(0));
        camera->mCamera.Reset(i_this->play_cam_center, i_this->play_cam_eye, i_this->play_cam_fovy, 0);
        camera->mCamera.Start();
        camera->mCamera.SetTrimSize(0);
        dMw_onMenuRing();
    }

    return 1;
}

static int dmg_rod_Delete(dmg_rod_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fpc_ProcID id = fopAcM_GetID(i_this);
    dComIfG_resDelete(&i_this->phase, i_this->arcname);

    if (i_this->HIOInit) {
        l_HIOInit = FALSE;
        mDoHIO_DELETE_CHILD(l_HIO.id);
    }

    if (actor->heap != NULL) {
        i_this->sound.deleteObject();
    }

    dMeter2Info_onWindowAccept(2);
    dMeter2Info_onWindowAccept(3);
    Z2GetAudioMgr()->changeFishingBgm(0);
    return 1;
}

static int useHeapInit(fopAc_ac_c* i_this) {
    dmg_rod_class* a_this = (dmg_rod_class*)i_this;
    int bmd_idx;
    u32 mdlflg;
    J3DModel* model;
    J3DModelData* modelData;
    f32* line_sizep;

    if (a_this->kind == MG_ROD_KIND_LURE) {
        if (!a_this->linemat.init(1, MG_ROD_LURE_LINE_LEN, 1)) {
            return 0;
        }

        line_sizep = a_this->linemat.getSize(0);

        for (int i = 0; i < MG_ROD_LURE_LINE_LEN; i++, line_sizep++) {
            *line_sizep = 0.1f + (0.1f * XREG_S(0));
        }

        for (int i = 0; i < 5; i++) {
            static int lure_bmd[] = {6, 7, 8, 10, 13};
            modelData = (J3DModelData*)dComIfG_getObjectRes(a_this->arcname, lure_bmd[i]);
            JUT_ASSERT(11397, modelData != NULL);

            mdlflg = i == 4 ? 0 : J3DMdlFlag_DifferedDLBuffer;
            a_this->lure_model[i] = mDoExt_J3DModel__create(modelData, mdlflg, 0x11000084);
            if (a_this->lure_model[i] == NULL) {
                return 0;
            }

            if (i == 0) {
                a_this->lure_model[i]->setUserArea((uintptr_t)a_this);
                for (u16 j = 0; j < a_this->lure_model[i]->getModelData()->getJointNum(); j++) {
                    if (j == 1) {
                        a_this->lure_model[i]->getModelData()->getJointNodePointer(j)->setCallBack(frog_nodeCallBack);
                    }
                }
            } else {
                if (i == 3) {
                    a_this->lure_model[i]->setUserArea((uintptr_t)a_this);
                    for (u16 j = 0; j < a_this->lure_model[i]->getModelData()->getJointNum(); j++) {
                        if (j == 1 || j == 2) {
                            a_this->lure_model[i]->getModelData()->getJointNodePointer(j)->setCallBack(ws_nodeCallBack);
                        }
                    }

                    a_this->field_0x101c[0] = cM_rndF(65536.0f);
                    a_this->field_0x101e = cM_rndF(65536.0f);
                }

                modelData = static_cast<J3DModelData*>(dComIfG_getObjectRes(a_this->arcname, 5));
                JUT_ASSERT(11440, modelData != NULL);

                for (int j = 0; j < 2; j++) {
                    a_this->hook_model[j] = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
                    if (a_this->hook_model[j] == NULL) {
                        return 0;
                    }
                }

                modelData = (J3DModelData*)dComIfG_getObjectRes(a_this->arcname, 9);
                JUT_ASSERT(11453, modelData != NULL);

                a_this->ring_model = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
                if (a_this->ring_model == NULL) {
                    return 0;
                }
            }
        }

        a_this->rod_modelMorf = new mDoExt_McaMorf((J3DModelData*)dComIfG_getObjectRes(a_this->arcname, 4), NULL, NULL, NULL, 0, 1.0f, 0, -1, 1, 0, 0x80000, 0x11000084);
        if (a_this->rod_modelMorf == NULL || a_this->rod_modelMorf->getModel() == NULL) {
            return 0;
        }
    
        model = a_this->rod_modelMorf->getModel();
        model->setUserArea((uintptr_t)a_this);
        for (u16 i = 0; i < model->getModelData()->getJointNum(); i++) {
            if (i == 1 || i == 3) {
                model->getModelData()->getJointNodePointer(i)->setCallBack(Reel_CallBack);
            }
        }

        modelData = (J3DModelData*)dComIfG_getObjectRes(a_this->arcname, 9);
        JUT_ASSERT(11499, modelData != NULL);

        for (int i = 0; i < 6; i++) {
            a_this->unk_ring_model[i] = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
            if (a_this->unk_ring_model[i] == NULL) {
                return 0;
            }
        }

        for (int i = 0; i < 15; i++) {
            if ((i & 1) || i == 2) {
                bmd_idx = 41;
            } else {
                bmd_idx = 42;
            }

            modelData = (J3DModelData*)dComIfG_getObjectRes("Alink", bmd_idx);
            JUT_ASSERT(11523, modelData != NULL);

            a_this->rod_uki_model[i] = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
            if (a_this->rod_uki_model[i] == NULL) {
                return 0;
            }
        }
    } else {
        if (!a_this->linemat.init(1, MG_ROD_UKI_LINE_LEN, 1)) {
            return 0;
        }

        line_sizep = a_this->linemat.getSize(0);
        for (int i = 0; i < MG_ROD_UKI_LINE_LEN; i++, line_sizep++) {
            *line_sizep = 0.1f + (0.1f * XREG_S(0));
        }

        modelData = (J3DModelData*)dComIfG_getObjectRes(a_this->arcname, 0x2D);
        JUT_ASSERT(11547, modelData != NULL);

        a_this->uki_model = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
        if (a_this->uki_model == NULL) {
            return 0;
        }

        modelData = (J3DModelData*)dComIfG_getObjectRes(a_this->arcname, 0x2E);
        JUT_ASSERT(11556, modelData != NULL);

        a_this->uki_saki_model = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
        if (a_this->uki_saki_model == NULL) {
            return 0;
        }

        JUT_ASSERT(11564, modelData != NULL);

        for (int i = 0; i < 2; i++) {
            static int hook_bmd[] = {44, 43,};
            modelData = (J3DModelData*)dComIfG_getObjectRes(a_this->arcname, hook_bmd[i]);
            a_this->hook_model[i] = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
            if (a_this->hook_model[i] == NULL) {
                return 0;
            }

            static int esa_bmd[] = {36, 48};
            modelData = (J3DModelData*)dComIfG_getObjectRes(a_this->arcname, esa_bmd[i]);
            a_this->esa_model[i] = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
            if (a_this->esa_model[i] == NULL) {
                return 0;
            }
        }

        int bmd_idx = 40;
        for (int i = 0; i < 15; i++) {
            if (i > 0) {
                if (i == 3 || i == 6 || i == 9 || i >= 12) {
                    bmd_idx = 42;
                } else {
                    bmd_idx = 41;
                }
            }

            modelData = (J3DModelData*)dComIfG_getObjectRes(a_this->arcname, bmd_idx);
            JUT_ASSERT(11624, modelData != NULL);

            a_this->rod_uki_model[i] = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
            if (a_this->rod_uki_model[i] == NULL) {
                return 0;
            }
        }
    }

    return 1;
}

static int dmg_rod_Create(fopAc_ac_c* i_this) {
    dmg_rod_class* a_this = (dmg_rod_class*)i_this;
    fopAcM_ct(i_this, dmg_rod_class);

    a_this->arg0 = fopAcM_GetParam(i_this);
    a_this->arg1 = (fopAcM_GetParam(i_this) & 0xFF00) >> 8;
    a_this->arg2 = (fopAcM_GetParam(i_this) & 0xFF0000) >> 0x10;

    fopAc_ac_c* player = dComIfGp_getPlayer(0);
    
    if (a_this->arg0 == 0xF) {
        a_this->arg0 = 13;
    }

    if (daAlink_getAlinkActorClass()->checkCanoeRide()) {
        a_this->arg0 = 13;
        a_this->arg1 = 0;
    }

    if (a_this->arg0 == 13) {
        if (strcmp(dComIfGp_getStartStageName(), "T_MUKAO") == 0 && fopAcM_GetRoomNo(player) == 4) {
            a_this->arg1 = 1;
            a_this->arg0 = KREG_S(8) + 29;
        }
    }

    a_this->field_0x6a0 = a_this->arg0;

    u32 heap_size;
    if (a_this->arg1 == 0) {
        a_this->kind = MG_ROD_KIND_LURE;
        a_this->arcname = "Mg_rod";
        heap_size = 0x15FE0;
    } else {
        a_this->kind = MG_ROD_KIND_UKI;
        a_this->arcname = "Alink";
        heap_size = 0xC9A0;
    }

    #if VERSION == VERSION_SHIELD_DEBUG
    if (dComIfGs_getPalLanguage() == 1) {
        data_804BBBD4 = 2;
    } else {
        data_804BBBD4 = 0;
    }
    #elif VERSION == VERSION_SHIELD
    data_804BBBD4 = 0;
    #elif REGION_PAL
    if (dComIfGs_getPalLanguage() == 0) {
        data_804BBBD4 = 2;
    } else {
        data_804BBBD4 = 0;
    }
    #elif REGION_JPN
    data_804BBBD4 = 0;
    #else
    data_804BBBD4 = 1;
    #endif

    int phase_state = dComIfG_resLoad(&a_this->phase, a_this->arcname);
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("MG_ROD PARAM %x\n", fopAcM_GetParam(i_this));
        OS_REPORT("MG_ROD//////////////MG_ROD SET 1 !!\n");

        if (!fopAcM_entrySolidHeap(i_this, useHeapInit, heap_size)) {
            OS_REPORT("//////////////MG_ROD SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////MG_ROD SET 2 !!\n");
        if (!l_HIOInit) {
            a_this->HIOInit = TRUE;
            l_HIOInit = TRUE;
            l_HIO.id = mDoHIO_CREATE_CHILD("釣りシステム", &l_HIO);
        }

        a_this->acch.Set(fopAcM_GetPosition_p(i_this), fopAcM_GetOldPosition_p(i_this), i_this, 1, &a_this->acchcir, fopAcM_GetSpeed_p(i_this), NULL, NULL);
        a_this->acchcir.SetWall(10.0f, 10.0f);

        if (strcmp(dComIfGp_getStartStageName(), "F_SP127") != 0) {
            a_this->field_0x590 = -50000.0f;
        }

        a_this->sound.init(&i_this->current.pos, &i_this->current.pos, 3, 1);
        a_this->field_0x1165 = 20;

        fopAc_ac_c* boat_actor = (fopAc_ac_c*)fpcM_Search(s_boat_sub, i_this);
        a_this->boat_actor_id = fopAcM_GetID(boat_actor);

        if (a_this->kind == MG_ROD_KIND_UKI) {
            fopAcM_setStageLayer(i_this);
            fopAc_ac_c* sp18 = dComIfGp_getPlayer(0);

            a_this->action = ACTION_UKI_READY;
            a_this->hook_pos = sp18->current.pos;
            a_this->hook_pos.y += 200.0f;
            i_this->current.pos = a_this->hook_pos;
            a_this->field_0x1094 = 200.0f;
            a_this->timers[0] = 5;
            a_this->timers[3] = 5;

            static dCcD_SrcSph cc_sph_src = {
                {
                    {0x0, {{0x0, 0x0, 0x0}, {0x0, 0x0}, 0x49}}, // mObj
                    {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
                    {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
                    {0x0}, // mGObjCo
                }, // mObjInf
                {
                    {{0.0f, 0.0f, 0.0f}, 5.0f} // mSph
                } // mSphAttr
            };

            a_this->ccStts.Init(10, 0, i_this);
            a_this->ccSph.Set(cc_sph_src);
            a_this->ccSph.SetStts(&a_this->ccStts);

            a_this->field_0x6a8 = 15;
        } else {
            if (boat_actor != NULL) {
                a_this->action = ACTION_LURE_ONBOAT;
            } else {
                a_this->action = ACTION_LURE_STANDBY;
            }

            a_this->timers[1] = 20;
            a_this->field_0xf81 = a_this->lure_type = MG_LURE_PE;

            u8 lure_ct = dComIfGs_getEventReg(0xF11F) & 0xFF;
            OS_REPORT(" SP LURE CT %d\n", lure_ct);
            if (lure_ct != 0) {
                lure_ct--;
                dComIfGs_setEventReg(0xF11F, lure_ct);
            }

            a_this->field_0x6a4 = 1.0f;
        }

        OS_REPORT("//////////////MG_ROD SET 3 !!\n");
        dmg_rod_Execute(a_this);
    }

    return phase_state;
}

static actor_method_class l_dmg_rod_Method = {
    (process_method_func)dmg_rod_Create,
    (process_method_func)dmg_rod_Delete,
    (process_method_func)dmg_rod_Execute,
    (process_method_func)dmg_rod_IsDelete,
    (process_method_func)dmg_rod_Draw,
};

extern actor_process_profile_definition g_profile_MG_ROD = {
  fpcLy_CURRENT_e,            // mLayerID
  8,                          // mListID
  fpcPi_CURRENT_e,            // mListPrio
  PROC_MG_ROD,                // mProcName
  &g_fpcLf_Method.base,      // sub_method
  sizeof(dmg_rod_class),      // mSize
  0,                          // mSizeOther
  0,                          // mParameters
  &g_fopAc_Method.base,       // sub_method
  438,                        // mPriority
  &l_dmg_rod_Method,          // sub_method
  0x00060000,                 // mStatus
  fopAc_ACTOR_e,              // mActorType
  fopAc_CULLBOX_0_e,          // cullType
};

AUDIO_INSTANCES
#include "JSystem/JAudio2/JAUSectionHeap.h"
template<>
JAUSectionHeap* JASGlobalInstance<JAUSectionHeap>::sInstance;
