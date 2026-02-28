/**
 * @file d_a_mg_fshop.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_mg_fshop.h"
#include "d/actor/d_a_npc_henna.h"
#include "d/actor/d_a_mg_fish.h"
#include "d/actor/d_a_player.h"
#include "f_op/f_op_camera_mng.h"
#include "d/d_timer.h"
#include "d/d_s_play.h"
#include "Z2AudioLib/Z2Instances.h"

enum koro2_parts {
    KORO2_PART_BOX = 1,
    KORO2_PART_CURVE_A_U_L,
    KORO2_PART_CURVE_A_U_R,
    KORO2_PART_CURVE_A_R_U,
    KORO2_PART_CURVE_A_L_U,
    KORO2_PART_START,
    KORO2_PART_GOAL,
    KORO2_PART_CURVE_B_U_L,
    KORO2_PART_CURVE_B_U_R,
    KORO2_PART_CURVE_B_R_U,
    KORO2_PART_CURVE_B_L_U,
    KORO2_PART_SLOPE_D,
    KORO2_PART_SLOPE_U,
    KORO2_PART_SLOPE_L,
    KORO2_PART_SLOPE_R,
};

/**
 * Koro2 (Rollgoal) map data works as a 15x15 grid
 * Each grid value represents a piece to be placed there (koro2_parts enum)
 * (0) represents an empty space
 *
 * Up to 100 parts are supported in a course, though each piece has it's own max limit:
 * Start: 1
 * Goal: 1
 * Box: 31
 * CurveA: 16
 * CurveB: 4
 * Slope: 8
 */

static s8 koro2_map_LV1[135] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x05, 0x01, 0x01, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x04, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x05, 0x01, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static s8 koro2_map_LV2[135] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x05, 0x01, 0x01, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x03, 0x01, 0x02, 0x00, 0x03, 0x01, 0x04, 0x00,
    0x00, 0x01, 0x00, 0x05, 0x01, 0x04, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x05, 0x01, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static s8 koro2_map_LV3[135] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x05, 0x01, 0x01, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x03, 0x01, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x01, 0x00, 0x05, 0x01, 0x01, 0x01, 0x04, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x03, 0x01, 0x01, 0x02, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x05, 0x01, 0x01, 0x04, 0x00, 0x00, 0x06, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static s8 koro2_map_LV4[135] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x03, 0x01, 0x02, 0x00, 0x00, 0x00,
    0x00, 0x05, 0x01, 0x04, 0x00, 0x05, 0x01, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x04, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x05, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x04, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x03, 0x01, 0x01, 0x02, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x05, 0x01, 0x01, 0x04, 0x00, 0x00, 0x06, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static s8 koro2_map_LV5[135] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x09, 0x00, 0x00, 0x08, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x0C, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0B, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static s8 koro2_map_LV7[135] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x05, 0x0E, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static s8 koro2_map_LV6[135] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x03, 0x01, 0x04, 0x00, 0x03, 0x01, 0x02, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00,
    0x00, 0x01, 0x00, 0x03, 0x01, 0x04, 0x00, 0x0C, 0x00,
    0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x05, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static s8 koro2_map_LV8[135] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x05, 0x0E, 0x00, 0x00, 0x0F, 0x02, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x03, 0x01, 0x01, 0x02, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x0C, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x0C, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x0D, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x0D, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x06, 0x00, 0x00, 0x05, 0x01, 0x01, 0x04, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static s8* koro2_map_d[8] = {
    koro2_map_LV1, koro2_map_LV2, koro2_map_LV3, koro2_map_LV4,
    koro2_map_LV5, koro2_map_LV6, koro2_map_LV7, koro2_map_LV8,
};

static void ride_call_back(dBgW*, fopAc_ac_c*, fopAc_ac_c* i_rideActor) {
    if (i_rideActor->field_0x567 == 0) {
        i_rideActor->field_0x567 = 1;
        ((fshop_class*)i_rideActor)->field_0x428c = 0;
    }
}

static int Reel_CallBack(J3DJoint* i_joint, int param_1) {
    J3DJoint* var_r27;

    if (param_1 == 0) {
        var_r27 = i_joint;
        int jnt_no = var_r27->getJntNo();
        J3DModel* model = j3dSys.getModel();
        fs_rod_s* rod = (fs_rod_s*)model->getUserArea();
        MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);
        cMtx_XrotM(*calc_mtx, rod->rot_x);
        model->setAnmMtx(jnt_no, *calc_mtx);
        MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
    }

    return 1;
}

static int frog_CallBack(J3DJoint* i_joint, int param_1) {
    fs_lure_s* lure;
    J3DJoint* var_r27;

    if (param_1 == 0) {
        var_r27 = i_joint;
        int jnt_no = var_r27->getJntNo();
        J3DModel* model = j3dSys.getModel();
        lure = (fs_lure_s*)model->getUserArea();

        if (jnt_no == 1) {
            MTXCopy(model->getAnmMtx(jnt_no), *calc_mtx);
            cMtx_YrotM(*calc_mtx, JREG_S(7) + -8800);
            MtxScale(JREG_F(10) + 2.0f, JREG_F(11) + 1.0f, JREG_F(12) + 1.5f, 1);
            model->setAnmMtx(jnt_no, *calc_mtx);
            MTXCopy(*calc_mtx, J3DSys::mCurrentMtx);
        }
    }

    return 1;
}

static cXyz BallStartPos;

static cXyz BallEndPos;

#if !PLATFORM_SHIELD
static J3DModel* ArcIX_A_crwaku_model;

static J3DModel* ArcIX_A_crstart_model[1];

static J3DModel* ArcIX_A_crgoal_model[1];

static J3DModel* ArcIX_A_crbox_model[31];

static J3DModel* ArcIX_A_crcurve_a_model[16];

static J3DModel* ArcIX_A_crcurve_b_model[4];

static J3DModel* ArcIX_A_crsaka_model[8];

static J3DModel** koro2_union_model[6] = {
    ArcIX_A_crstart_model,   ArcIX_A_crgoal_model,    ArcIX_A_crbox_model,
    ArcIX_A_crcurve_a_model, ArcIX_A_crcurve_b_model, ArcIX_A_crsaka_model,
};
#endif

#if PLATFORM_SHIELD
    #define GET_CRWAKU_MODEL(ptr) (ptr)->mpA_crwaku_model
#else
    #define GET_CRWAKU_MODEL(ptr) ArcIX_A_crwaku_model
#endif

static void koro2_draw(fshop_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;

    if (actor->scale.x > 5.0f) {
        dComIfGd_setListBG();
    }

    g_env_light.setLightTevColorType_MAJI(GET_CRWAKU_MODEL(i_this), &actor->tevStr);
    mDoExt_modelUpdateDL(GET_CRWAKU_MODEL(i_this));

    for (int i = 0; i < ARRAY_SIZE(i_this->mKoro2); i++) {
        if (i_this->mKoro2[i].model != NULL) {
            g_env_light.setLightTevColorType_MAJI(i_this->mKoro2[i].model, &actor->tevStr);
            mDoExt_modelUpdateDL(i_this->mKoro2[i].model);
        }
    }

    dComIfGd_setList();
}

static int daFshop_Draw(fshop_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    g_env_light.settingTevStruct(0x10, &actor->current.pos, &actor->tevStr);

    if (i_this->ballModel != NULL) {
        g_env_light.setLightTevColorType_MAJI(i_this->ballModel, &actor->tevStr);

        J3DMaterial* sp1C = i_this->ballModel->getModelData()->getMaterialNodePointer(0);
        sp1C->getTevKColor(1)->r = 0;

        mDoExt_modelUpdateDL(i_this->ballModel);
        dComIfGd_setSimpleShadow(
            &actor->current.pos, i_this->ballAcch.GetGroundH(), (TREG_F(8) + 66.6f) * actor->scale.x,
            i_this->ballAcch.m_gnd, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
       return 1;
    }
    
    if (i_this->field_0x400e != 0) {
        if (i_this->field_0x400d == 0) {
            koro2_draw(i_this);
            i_this->field_0x400d = 1;
        }
        return 1;
    }
    
    int roomNo = fopAcM_GetRoomNo(actor);
    camera_class* camera = (camera_class*)dComIfGp_getCamera(0);
    
    for (int i = 0; i < 48; i++) {
        f32 fVar4 = i_this->mLure[i].field_0x00.x - camera->lookat.eye.x;
        f32 fVar5 = i_this->mLure[i].field_0x00.z - camera->lookat.eye.z;
        if (SQUARE(fVar4) + SQUARE(fVar5) > KREG_F(11) + 1200.0f) {
            g_env_light.setLightTevColorType_MAJI(i_this->mLure[i].model, &actor->tevStr);
            mDoExt_modelUpdateDL(i_this->mLure[i].model);

            if (i_this->mLure[i].field_0x24 != 3) {
                for (int j = 0; j < 2; j++) {
                    dComIfGp_entrySimpleModel(i_this->mLure[i].hookModel[j], roomNo);
                }
            }
        }
    }

    for (int i = 0; i < 3; i++) {
        g_env_light.setLightTevColorType_MAJI(i_this->mRod[i].model, &actor->tevStr);
        mDoExt_modelUpdateDL(i_this->mRod[i].model);

        static GXColor rod_color[3] = {
            {0x96, 0x50, 0x14, 0xFF},
            {0x64, 0x96, 0x14, 0xFF},
            {0x96, 0x14, 0x14, 0xFF},
        };
        i_this->mRod[i].line_mat.update(8, rod_color[i], &i_this->actor.tevStr);
        dComIfGd_set3DlineMat(&i_this->mRod[i].line_mat);

        for (int j = 0; j < 6; j++) {
            dComIfGp_entrySimpleModel(i_this->mRod[i].ringModel[j], roomNo);
        }
    }

    for (int i = 0; i < 2; i++) {
        g_env_light.setLightTevColorType_MAJI(i_this->mTsubo[i].model, &actor->tevStr);
        mDoExt_modelUpdateDL(i_this->mTsubo[i].model);
    }

    static GXColor l_color = {0x32, 0x64, 0x1E, 0xFF};
    i_this->field_0x3f88.update(15, TREG_F(11) + 1.2f, l_color, 2, &i_this->actor.tevStr);
    dComIfGd_set3DlineMat(&i_this->field_0x3f88);

    g_env_light.setLightTevColorType_MAJI(i_this->canoeModel, &actor->tevStr);
    mDoExt_modelUpdateDL(i_this->canoeModel);

    g_env_light.setLightTevColorType_MAJI(i_this->hatModel, &actor->tevStr);
    mDoExt_modelUpdateDL(i_this->hatModel);

    if (i_this->photoModel != NULL) {
        mDoMtx_stack_c::transS(0.0f, 0.0f, 0.0f);
        i_this->photoModel->setBaseTRMtx(mDoMtx_stack_c::get());
        g_env_light.setLightTevColorType_MAJI(i_this->photoModel, &actor->tevStr);
        mDoExt_modelUpdateDL(i_this->photoModel);
    }

    return 1;
}

static f32 lure_scale[3] = {
    0.75f, 0.75f, 0.6f,
};

static f32 lure_eye_off[3] = {
    17.0f, 11.0f, 16.5f,
};

static f32 hook_1_offy[3] = {
    -3.5f, -3.5f, -5.0f,
};

static f32 hook_2_offz[3] = {
    -19.0f, -16.0f, -21.0f,
};

static f32 hook_2_offy[3] = {
    -1.0f, 0.0f, 0.0f,
};

static void lure_set(fshop_class* i_this) {
    static f32 lure_ofy[3] = {-12.0f, -10.0f, -14.0f};
    fs_lure_s* pLure = i_this->mLure;

    for (int i = 0; i < 48; i++, pLure++) {
        if (daPy_getPlayerActorClass()->checkFrontRollCrash()) {
            pLure->field_0x34 = cM_rndF(65536.0f);
            pLure->field_0x36 = cM_rndF(65536.0f);
            pLure->field_0x38 = cM_rndF(1000.0f) + 500.0f;
            pLure->field_0x3c = cM_rndF(1000.0f) + 500.0f;
        }

        ANGLE_ADD(pLure->field_0x34, 4000);
        ANGLE_ADD(pLure->field_0x36, 4000);
        pLure->field_0x32 = pLure->field_0x3c * cM_ssin(pLure->field_0x36);
        pLure->field_0x30 = pLure->field_0x38 * cM_ssin(pLure->field_0x34);

        cLib_addCalc0(&pLure->field_0x3c, 1.0f, 40.0f);
        cLib_addCalc0(&pLure->field_0x38, 1.0f, 40.0f);

        f32 fVar1 = 0.0f;
        f32 fVar2;
        if (pLure->field_0x24 != 3) {
            fVar2 = 25.0f;
            fVar1 = 13.0f / pLure->field_0x28 + lure_ofy[pLure->field_0x24];
        } else {
            fVar2 = 12.0f;
        }

        mDoMtx_stack_c::transS(
            pLure->field_0x00.x,
            pLure->field_0x00.y + fVar1,
            pLure->field_0x00.z);
        mDoMtx_stack_c::transM(0.0f, fVar2, 0.0f);
        mDoMtx_stack_c::ZrotM(pLure->field_0x32);
        mDoMtx_stack_c::XrotM(pLure->field_0x30);
        mDoMtx_stack_c::transM(0.0f, -fVar2, 0.0f);
        mDoMtx_stack_c::YrotM(pLure->field_0x0c.y);
        mDoMtx_stack_c::XrotM(pLure->field_0x0c.x);
        mDoMtx_stack_c::ZrotM(pLure->field_0x0c.z);
        mDoMtx_stack_c::scaleM(pLure->field_0x2c, pLure->field_0x2c, pLure->field_0x28);
        mDoMtx_stack_c::transM(0.0f, 0.0f, AREG_F(18));
        pLure->model->setBaseTRMtx(mDoMtx_stack_c::get());

        if (pLure->field_0x24 != 3) {
            mDoMtx_stack_c::push();
            mDoMtx_stack_c::transM(ZREG_F(0), hook_1_offy[pLure->field_0x24] + ZREG_F(1), 1.0f + ZREG_F(2));
            mDoMtx_stack_c::XrotM(-10000);
            pLure->hookModel[0]->setBaseTRMtx(mDoMtx_stack_c::get());
            mDoMtx_stack_c::pop();
            mDoMtx_stack_c::transM(ZREG_F(3), hook_2_offy[pLure->field_0x24] + ZREG_F(4),
                                   hook_2_offz[pLure->field_0x24] + ZREG_F(5));
            mDoMtx_stack_c::XrotM(0x4000);
            pLure->hookModel[1]->setBaseTRMtx(mDoMtx_stack_c::get());
        }
    }
}

static void rod_set(fshop_class* i_this) {
    fs_rod_s* pRod = i_this->mRod;

    for (int i = 0; i < 3; i++, pRod++) {
        mDoMtx_stack_c::transS(pRod->field_0x00.x, pRod->field_0x00.y, pRod->field_0x00.z);
        if (i == 0) {
            mDoMtx_stack_c::YrotM(KREG_S(0) - 12000);
        } else if (i == 1) {
            mDoMtx_stack_c::YrotM(KREG_S(0) - 9000);
        } else {
            mDoMtx_stack_c::YrotM(KREG_S(0) - 16000);
        }
        mDoMtx_stack_c::XrotM(KREG_S(1) + 1300);
        mDoMtx_stack_c::YrotM(s16(KREG_S(2)));
        pRod->model->setBaseTRMtx(mDoMtx_stack_c::get());

        cXyz* local_64 = pRod->line_mat.getPos(0);
        if (pRod->field_0x4c == 0) {
            mDoMtx_stack_c::transM(0.0f, 15.0f + BREG_F(1), 0.0f);
        } else {
            mDoMtx_stack_c::transM(0.0f, 15.0f + BREG_F(1), -0.8f);
        }

        cXyz cStack_1c;
        cXyz cStack_28;
        cXyz vec_0x4c;
        for (int j = 0; j < 8; j++, local_64++) {
            mDoMtx_stack_c::multVecZero(local_64);
            if (j == 0) {
                cStack_1c = *local_64;
            } else if (j == 7) {
                cStack_28 = *local_64;
            }
            mDoMtx_stack_c::transM(0.0f, pRod->field_0x54, 0.0f);
        }

        static f32 guide_p[6] = {0.0f, 0.1f, 0.22f, 0.36f, 0.52f, 0.7f};
        static f32 guide_s[6] = {0.5f, 0.5f, 0.55f, 0.6f, 0.65f, 0.8f};
        for (int j = 0; j < 6; j++) {
            vec_0x4c = cStack_28 + (cStack_1c - cStack_28) * guide_p[j];
            mDoMtx_stack_c::transS(vec_0x4c.x, vec_0x4c.y, vec_0x4c.z);
            mDoMtx_stack_c::scaleM(guide_s[j], guide_s[j], guide_s[j]);
            mDoMtx_stack_c::transM(VREG_F(0), 4.0f + VREG_F(1), VREG_F(2));
            mDoMtx_stack_c::YrotM(0x4000);
            pRod->ringModel[j]->setBaseTRMtx(mDoMtx_stack_c::get());
        }
    }
}

static void* s_fish_sub(void* i_actor, void* i_data) {
    (void) i_data;
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_MG_FISH) {
        if (((mg_fish_class*)i_actor)->actor.speedF > 0.1f) {
            fs_weed_s* weed = (fs_weed_s*)i_data;
            f32 x_dist = ((mg_fish_class*)i_actor)->actor.current.pos.x - weed->field_0x00[0].x;
            f32 y_dist = ((mg_fish_class*)i_actor)->actor.current.pos.y - weed->field_0x00[0].y;
            f32 z_dist = ((mg_fish_class*)i_actor)->actor.current.pos.z - weed->field_0x00[0].z;

            f32 dVar3 = ((mg_fish_class*)i_actor)->mJointScale;
            if (dVar3 > 0.7f) {
                dVar3 = 0.7f;
            }

            if (fabsf(y_dist) < (60.0f + AREG_F(14)) * weed->field_0xb4 && JMAFastSqrt(SQUARE(x_dist) + SQUARE(z_dist)) < dVar3 * 35.0f) {
                cLib_addCalc2(&weed->field_0xb8, dVar3, 0.1f, 0.05f);
                return i_actor;
            }
        }
    }

    return NULL;
}

static void tsubo_set(fshop_class* i_this) {
    fopAc_ac_c* pfVar5 = dComIfGp_getPlayer(0);
    fs_tsubo_s* pTsubo = i_this->mTsubo;

    cXyz local_40;
    f32 reg_f31 = 1.0f;
    s16 xrot;
    s16 zrot;
    for (int i = 0; i < 2; i++, pTsubo++) {
        local_40.x = pTsubo->field_0x00.x - pfVar5->current.pos.x;
        local_40.z = pTsubo->field_0x00.z - pfVar5->current.pos.z;
        if (daPy_getPlayerActorClass()->checkFrontRollCrash() && JMAFastSqrt(SQUARE(local_40.x) + SQUARE(local_40.z)) < 150.0f) {
            if (i == 0) {
                pTsubo->field_0x1c = 1200.0f;
                pTsubo->field_0x24 = 1800;
            } else {
                pTsubo->field_0x1c = 1800.0f;
                pTsubo->field_0x24 = 2800;
            }
            pTsubo->field_0x20 = cM_rndF(65536.0f);
            pTsubo->field_0x22 = cM_rndF(65536.0f);

            npc_henna_class* henna = (npc_henna_class*)fopAcM_SearchByName(PROC_NPC_HENNA);
            if (henna != NULL) {
                henna->field_0x7b5 = 60;
            }
        }

        xrot = cM_ssin(pTsubo->field_0x20) * pTsubo->field_0x1c;
        zrot = cM_ssin(pTsubo->field_0x22) * pTsubo->field_0x1c;
        pTsubo->field_0x20 += pTsubo->field_0x24;
        ANGLE_ADD_2(pTsubo->field_0x22, pTsubo->field_0x24 + 700);
        cLib_addCalcAngleS2(&pTsubo->field_0x24, 9000 + TREG_S(8), 1, 200);

        mDoMtx_stack_c::transS(pTsubo->field_0x00.x, pTsubo->field_0x00.y, pTsubo->field_0x00.z);
        mDoMtx_stack_c::XrotM(xrot);
        mDoMtx_stack_c::ZrotM(zrot);
        mDoMtx_stack_c::scaleM(reg_f31, reg_f31, reg_f31);
        pTsubo->model->setBaseTRMtx(mDoMtx_stack_c::get());

        cLib_addCalc0(&pTsubo->field_0x1c, 1.0f, 50.0f + TREG_F(18));
    }
}

static void weed_control(fshop_class* i_this, fs_weed_s* i_weed) {
    UNUSED(i_this);
    int i;
    cXyz local_84;
    cXyz local_90;
    cXyz* pfVar7 = i_weed->field_0x00 + 1;

    local_84.x = 0.0f;
    local_84.y = 0.0f;
    local_84.z = i_weed->field_0xb4 * 5.0f;

    f32 reg_f29 = i_weed->field_0xb8 + 0.5f;
    f32 reg_f28;
    f32 reg_f27;
    f32 reg_f26;
    f32 reg_f31;
    f32 reg_f30;
    ANGLE_ADD(i_weed->field_0xbc, i_weed->field_0xb8 * 600.0f + 200.0f);
    cLib_addCalc0(&i_weed->field_0xb8, 0.05f, 0.02f);

    for (i = 1; i < 15; i++, pfVar7++) {
        reg_f27 = reg_f29 * cM_ssin(i_weed->field_0xbc + i * (AREG_S(7)-5000));
        reg_f26 = reg_f29 * cM_ssin(i_weed->field_0xbc + i * (AREG_S(8)-5500));
        reg_f28 = reg_f27 + (pfVar7[0].x - pfVar7[-1].x);
        reg_f31 = (pfVar7[0].y - pfVar7[-1].y) + 3.0f + KREG_F(7);
        reg_f30 = reg_f26 + (pfVar7[0].z - pfVar7[-1].z);
        cMtx_XrotS(*calc_mtx, -cM_atan2s(reg_f31, reg_f30));
        cMtx_YrotM(*calc_mtx, (s16)cM_atan2s(reg_f28, JMAFastSqrt(SQUARE(reg_f31) + SQUARE(reg_f30))));
        MtxPosition(&local_84, &local_90);

        pfVar7[0].x = pfVar7[-1].x + local_90.x;
        pfVar7[0].y = pfVar7[-1].y + local_90.y;
        pfVar7[0].z = pfVar7[-1].z + local_90.z;
    }
}

static void stage_copy(fshop_class* i_dst, fshop_class* i_src) {
    i_dst->field_0x4014 = i_src->field_0x4014;
    i_dst->field_0x4020 = i_src->field_0x4020;
    i_dst->actor.scale = i_src->actor.scale;
}

static void koro2_mtx_set(fshop_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;

    cXyz cStack_44;
    cXyz cStack_50;
    if (i_this->field_0x400d) {
        mDoMtx_stack_c::transS(0.0f, -10000.0f, 0.0f);
        MTXCopy(mDoMtx_stack_c::get(), i_this->field_0x4030);
        i_this->koro2WakuBgw->Move();

        for (int i = 0; i < 100; i++) {
            if (i_this->mKoro2[i].model != NULL) {
                MTXCopy(mDoMtx_stack_c::get(), i_this->mKoro2[i].bgMtx);
                i_this->mKoro2[i].bgw->Move();
            }
        }
    } else {
        mDoMtx_stack_c::transS(i_this->field_0x4014.x, i_this->field_0x4014.y, i_this->field_0x4014.z);
        mDoMtx_stack_c::YrotM(i_this->field_0x4020.y);
        mDoMtx_stack_c::XrotM(i_this->field_0x4020.x);
        mDoMtx_stack_c::ZrotM(i_this->field_0x4020.z);
        mDoMtx_stack_c::scaleM(actor->scale.x, actor->scale.x, actor->scale.x);
        mDoMtx_stack_c::push();
        mDoMtx_stack_c::scaleM(0.01f, 0.01f, 0.01f);
        i_this->mpA_crwaku_model->setBaseTRMtx(mDoMtx_stack_c::get());
        MTXCopy(mDoMtx_stack_c::get(), i_this->field_0x4030);

        i_this->koro2WakuBgw->Move();
        mDoMtx_stack_c::pop();

        f32 var_f31 = VREG_F(2) + -10.0f;
        f32 var_f30 = VREG_F(4) + -18.0f;

        for (int i = 0; i < 100; i++) {
            if (i_this->mKoro2[i].model != NULL) {
                mDoMtx_stack_c::push();
                mDoMtx_stack_c::transM(i_this->mKoro2[i].pos.x + var_f31,
                                       i_this->mKoro2[i].pos.y,
                                       i_this->mKoro2[i].pos.z + var_f30);
                mDoMtx_stack_c::YrotM(i_this->mKoro2[i].rot_y);
                mDoMtx_stack_c::scaleM(i_this->mKoro2[i].size.x,
                                       i_this->mKoro2[i].size.y,
                                       i_this->mKoro2[i].size.z);
                mDoMtx_stack_c::push();
                mDoMtx_stack_c::scaleM(0.01f, 0.01f, 0.01f);
                i_this->mKoro2[i].model->setBaseTRMtx(mDoMtx_stack_c::get());
                MTXCopy(mDoMtx_stack_c::get(), i_this->mKoro2[i].bgMtx);

                i_this->mKoro2[i].bgw->Move();
                mDoMtx_stack_c::pop();
                mDoMtx_stack_c::multVecZero(&i_this->mKoro2[i].world_pos);

                if (i_this->mKoro2[i].part_id == KORO2_PART_START) {
                    cStack_44.set(0.0f, 0.0f, 2.5f);
                    mDoMtx_stack_c::multVec(&cStack_44, &BallStartPos);
                } else if (i_this->mKoro2[i].part_id == KORO2_PART_GOAL) {
                    cStack_44.set(0.0f, 0.0f, -2.0f);
                    mDoMtx_stack_c::multVec(&cStack_44, &BallEndPos);
                }

                mDoMtx_stack_c::pop();
            }
        }
    }
}

static void* s_sel_sub(void* i_actor, void* i_data) {
    (void) i_data;
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_FSHOP) {
        if (((fshop_class*)i_actor)->field_0x400e == (u8)((((fshop_class*)i_data)->field_0x400c & 7) + 1)) {
            return i_actor;
        }
    }

    return NULL;
}

static void* s_ball_sub(void* i_actor, void* i_data) {
    UNUSED(i_data);
    if (fopAcM_IsActor(i_actor) && fopAcM_GetName(i_actor) == PROC_FSHOP && (fopAcM_GetParam(i_actor) & 0xFF) == 0x23) {
        return i_actor;
    }

    return NULL;
}

static void koro2_game(fshop_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;

    cXyz sp5C;
    cXyz sp68;
    actor->scale.x = 1.0f;

    switch (i_this->field_0x4010) {
    case 0:
        i_this->field_0x4014.x = 284.0f;
        i_this->field_0x4014.y = 87.8f + VREG_F(8);
        i_this->field_0x4014.z = 432.0;
        i_this->field_0x4020.x = i_this->field_0x4020.z = 0;
        break;
    case 1:
        i_this->field_0x4014.x = 204.0f + YREG_F(8);
        i_this->field_0x4014.y = 147.8f + YREG_F(9) - 1500.0f;
        i_this->field_0x4014.z = 430.0f + YREG_F(10);
        i_this->field_0x4020.y = YREG_S(0) - 0x4000;

        cLib_addCalcAngleS2(&i_this->field_0x4020.x, 0, 2, 0x200);
        cLib_addCalcAngleS2(&i_this->field_0x4020.z, 0, 2, 0x200);
    case 2:
        actor->scale.x = 10.0f;
        if (i_this->field_0x4010 == 2) {
            static f32 old_stick_x = 0.0f;

            if ((mDoCPd_c::getSubStickX(PAD_1) >= 0.8f && old_stick_x < 0.8f) ||
                (mDoCPd_c::getSubStickX(PAD_1) <= -0.8f && old_stick_x > -0.8f))
            {
                if (mDoCPd_c::getSubStickX(PAD_1) > 0.0f) {
                    ANGLE_ADD(i_this->field_0x4062, 0x4000);
                } else {
                    ANGLE_ADD(i_this->field_0x4062, -0x4000);
                }
            }

            old_stick_x = mDoCPd_c::getSubStickX(PAD_1);
            cLib_addCalcAngleS2(&i_this->field_0x4060, i_this->field_0x4062, 4, 0x1000);
            cMtx_YrotS(*calc_mtx, -i_this->field_0x4060);

            sp5C.x = mDoCPd_c::getStickX3D(PAD_1);
            sp5C.y = 0.0f;
            sp5C.z = mDoCPd_c::getStickY(PAD_1);
            MtxPosition(&sp5C, &sp68);

            f32 reg_f31 = sp68.x;
            f32 reg_f30 = sp68.z;

            if (reg_f31 > 0.15f) {
                reg_f31 -= 0.15f;
            } else if (reg_f31 < -0.15f) {
                reg_f31 += 0.15f;
            } else {
                reg_f31 = 0.0f;
            }

            if (reg_f30 > 0.15f) {
                reg_f30 -= 0.15f;
            } else if (reg_f30 < -0.15f) {
                reg_f30 += 0.15f;
            } else {
                reg_f30 = 0.0f;
            }

            cLib_addCalcAngleS2(&i_this->field_0x4020.x, reg_f30 * (-6000.0f + JREG_F(7)), 4, 0x200);
            cLib_addCalcAngleS2(&i_this->field_0x4020.z, reg_f31 * (-6000.0f + JREG_F(8)), 4, 0x200);
        }
        break;
    }

    i_this->field_0x4008 = (fshop_class*)fpcM_Search(s_sel_sub, actor);
    if (i_this->field_0x4008 != NULL) {
        stage_copy(i_this->field_0x4008, i_this);
        i_this->field_0x4008->field_0x400d = 0;

        fshop_class* ball_p = (fshop_class*)fpcM_Search(s_ball_sub, actor);
        if (ball_p != NULL) {
            ball_p->field_0x4008 = i_this->field_0x4008;
        }
    }
}

static int ball_wall_check(fshop_class* i_this) {
    if (!i_this->ballAcch.ChkWallHit()) {
        return 0;
    }

    fopAc_ac_c* actor = (fopAc_ac_c*)&i_this->actor;
    dBgS_LinChk adStack_88;
    cXyz cStack_94;
    cXyz cStack_a0;
    cXyz local_ac;
    local_ac.y = 0.0f;

    cStack_94 = actor->current.pos;
    cStack_94.y += nREG_F(7) + 5.0f;

    for (int i = 0; i < 4; i++) {
        static f32 c_x[4] = {14.0f, -14.0f, 0.0f, 0.0f};
        static f32 c_z[4] = {0.0f, 0.0f, 14.0f, -14.0f};
        local_ac.x = c_x[i];
        local_ac.z = c_z[i];
        cStack_a0 = cStack_94 + local_ac;

        adStack_88.Set(&cStack_94, &cStack_a0, actor);
        if (dComIfG_Bgsp().LineCross(&adStack_88)) {
            if ((i == 0 && actor->speed.x > 0.0f) ||
                (i == 1 && actor->speed.x < 0.0f))
            {
                actor->speed.x = 0.0f;
            }

            if ((i == 2 && actor->speed.z > 0.0f) ||
                (i == 3 && actor->speed.z < 0.0f))
            {
                actor->speed.z = 0.0f;
            }
        }
    }

    return 0;
}

#if DEBUG
    #define DEBUG_CHECK_PAD_TRIG_Z 1
#else
    #define DEBUG_CHECK_PAD_TRIG_Z 0
#endif

static int daFshop_Execute(fshop_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAc_ac_c* pPlayer = dComIfGp_getPlayer(0);
    cXyz local_c0;

    i_this->field_0x0570++;

    if (i_this->field_0x400e != 0) {
        if (i_this->field_0x428d != 0) {
            i_this->field_0x428d--;
        }

        koro2_mtx_set(i_this);
        return 1;
    }

    if (i_this->ballModel != NULL) {
        actor->scale.x = 0.024f;
        cXyz local_cc;
        cXyz local_d8;
        BOOL bVar5 = FALSE;

        if (i_this->field_0x428d != 0) {
            i_this->field_0x428d--;
        }

        switch (i_this->field_0x0572) {
        case 0:
            actor->current.pos = BallStartPos;
            actor->current.pos.y += 4.0f + XREG_F(7);
            actor->old.pos = actor->current.pos;
            i_this->field_0x428d = 10;
            break;
        case 1:
            actor->current.pos = BallStartPos;
            actor->current.pos.y += 4.0f + XREG_F(7);
            actor->old.pos = actor->current.pos;
            actor->speed.zero();

            i_this->field_0x0572++;
            i_this->field_0x428c = 3;
            actor->field_0x567 = 0;
            i_this->field_0x428d = 10;
        case 2:
            bVar5 = TRUE;
            actor->scale.x = 0.24000001f;
            break;
        }

        if (bVar5) {
            cMtx_YrotS(*calc_mtx, i_this->field_0x4020.y);
            cMtx_XrotM(*calc_mtx, i_this->field_0x4020.x);
            cMtx_ZrotM(*calc_mtx, i_this->field_0x4020.z);
            MtxPosition(&actor->speed, &i_this->field_0x4064);
            i_this->field_0x4064 = actor->speed;
            actor->current.pos += i_this->field_0x4064;

            if (actor->speed.x || actor->speed.z) {
                actor->current.angle.y = cM_atan2s(actor->speed.x, actor->speed.z);
            }

            actor->speed.y -= 3.5f;
            if (actor->speed.y <= -15.0f) {
                actor->speed.y = -15.0f;
            }

            i_this->ballAcch.m_gnd.OffWall();
            i_this->ballAcch.CrrPos(dComIfG_Bgsp());
            ball_wall_check(i_this);

            fshop_class* iVar6 = i_this->field_0x4008;
            for (int i = 0; i < 100; i++) {
                if (iVar6->mKoro2[i].model != NULL) {
                    local_cc.x = iVar6->mKoro2[i].world_pos.x - actor->current.pos.x;
                    local_cc.z = iVar6->mKoro2[i].world_pos.z - actor->current.pos.z;
                    if (JMAFastSqrt(local_cc.x * local_cc.x + local_cc.z * local_cc.z) < 40.0f) {
                        iVar6->mKoro2[i].field_0x60 = 0;
                    }
                }
            }

            if (actor->current.pos.y < -2500.0f) {
                actor->field_0x567 = 1;
            }

            npc_henna_class* henna = (npc_henna_class*)fopAcM_SearchByName(PROC_NPC_HENNA);
            if (henna != NULL && henna->field_0x7b9 != 0 && (actor->field_0x567 == 1 || dTimer_getRestTimeMs() == 0)) {
                BOOL bVar5 = FALSE;
                for (int i = 0; i < 100; i++) {
                    if (iVar6->mKoro2[i].model != NULL && iVar6->mKoro2[i].field_0x60) {
                        bVar5 = TRUE;
                        break;
                    }
                }

                if (!bVar5) {
                    henna->actor.health = 1;
                } else if (actor->field_0x567 == 1) {
                    henna->actor.health = 0;
                } else {
                    henna->actor.health = 2;
                }

                henna->cam_mode = 73;
                henna->field_0x754 = 0;
                henna->field_0x7b9 = 0;
                actor->field_0x567 = 2;
            } else {
                if (actor->field_0x567 == 0) {
                    local_cc = BallEndPos - actor->current.pos;
                    if (local_cc.abs() < 65.0f + hREG_F(16)
#if DEBUG_CHECK_PAD_TRIG_Z
                    || mDoCPd_c::getTrigZ(PAD_1)
#endif
                    ) {
                        npc_henna_class* henna = (npc_henna_class*)fopAcM_SearchByName(PROC_NPC_HENNA);
                        if (henna != NULL) {
                            BOOL bVar5 = FALSE;
                            for (int i = 0; i < 100; i++) {
                                if (iVar6->mKoro2[i].model != NULL && iVar6->mKoro2[i].field_0x60) {
                                    bVar5 = TRUE;
                                    break;
                                }
                            }

#if DEBUG_CHECK_PAD_TRIG_Z
                            if (mDoCPd_c::getTrigZ(PAD_1)) {
                                bVar5 = FALSE;
                            }
#endif

                            if (bVar5) {
                                henna->actor.health = 3;
                                henna->cam_mode = 73;
                            } else {
                                henna->cam_mode = 76;
                            }

                            henna->field_0x754 = 0;
                            henna->field_0x7b9 = 0;
                            actor->field_0x567 = 2;
                        }
                    }
                }
            }

            if (i_this->ballAcch.ChkGroundHit()) {
                if (i_this->field_0x428c == 0) {
                    actor->speed.y = 7.0f;
                } else if (i_this->field_0x428c == 1) {
                    actor->speed.y = 2.5f;
                } else if (i_this->field_0x428c == 2) {
                    actor->speed.y = 1.0f;
                } else {
                    actor->speed.y = -5.0f;
                    i_this->field_0x428c = 2;
                }

                i_this->field_0x428c++;

                dBgS_GndChk adStack_b4;
                cXyz local_e4;
                local_e4 = actor->current.pos;
                local_e4.y += 10.0f;
                local_e4.z += 1.0f;
                adStack_b4.SetPos(&local_e4);
                local_e4.y = dComIfG_Bgsp().GroundCross(&adStack_b4);
                f32 reg_f29;
                f32 reg_f31 = local_e4.y - actor->current.pos.y;
                f32 reg_f28 = local_e4.z - actor->current.pos.z;
                s16 local_160 = -cM_atan2s(reg_f31, reg_f28);
                if (local_160 > 0x3000) {
                    local_160 = 0x3000;
                } else if (local_160 < -0x3000) {
                    local_160 = -0x3000;
                }
                local_e4 = actor->current.pos;
                local_e4.y += 10.0f;
                local_e4.x += 1.0f;
                adStack_b4.SetPos(&local_e4);
                local_e4.y = dComIfG_Bgsp().GroundCross(&adStack_b4);
                reg_f31 = local_e4.y - actor->current.pos.y;
                reg_f29 = local_e4.x - actor->current.pos.x;
                s16 local_162 = (s16)cM_atan2s(reg_f31, reg_f29);
                if (local_162 > 0x3000) {
                    local_162 = 0x3000;
                } else if (local_162 < -0x3000) {
                    local_162 = -0x3000;
                }

                cMtx_XrotS(*calc_mtx, local_160);
                cMtx_ZrotM(*calc_mtx, local_162);
                local_cc.x = 0.0f;
                local_cc.y = 30.0f + AREG_F(13);
                local_cc.z = 0.0f;
                MtxPosition(&local_cc, &local_d8);

                cLib_addCalc2(&actor->speed.x, local_d8.x, 0.05f, fabsf(local_d8.x) * 0.1f + 0.2f);
                cLib_addCalc2(&actor->speed.z, local_d8.z, 0.05f, fabsf(local_d8.z) * 0.1f + 0.2f);
            }
        }
        camera_class* pmVar11 = (camera_class*)dComIfGp_getCamera(0);
        mDoMtx_stack_c::transS(actor->current.pos.x, actor->current.pos.y,
                               actor->current.pos.z);
        mDoMtx_stack_c::scaleM(actor->scale.x, actor->scale.x,
                               actor->scale.x);
        mDoMtx_stack_c::transM(0.0f, (190.0f + hREG_F(11)) * actor->scale.x, 0.0f);
        local_cc = pmVar11->lookat.eye - actor->current.pos;
        mDoMtx_stack_c::YrotM(cM_atan2s(local_cc.x, local_cc.z));
        mDoMtx_stack_c::XrotM(-cM_atan2s(local_cc.y, JMAFastSqrt((local_cc.x * local_cc.x + local_cc.z * local_cc.z))));
        i_this->ballModel->setBaseTRMtx(mDoMtx_stack_c::get());
        return 1;
    } else {
        lure_set(i_this);
        rod_set(i_this);
        tsubo_set(i_this);

        fs_weed_s* pWeed = i_this->mWeed;
        cXyz* local_148;
        for (int i = 0; i < 60; i++, pWeed++) {
            weed_control(i_this, pWeed);
            local_148 = i_this->field_0x3f88.getPos(i);
            for (int local_150 = 0; local_150 < 15; local_150++, local_148++) {
                *local_148 = pWeed->field_0x00[local_150];
            }
            fpcM_Search(s_fish_sub, pWeed);
        }

        cXyz cStack_f0(-720.0f, 30.0f + KREG_F(14), 70.0f);
        f32 reg_f30 = 0.5f + KREG_F(15);
        cXyz cStack_fc(reg_f30, reg_f30, reg_f30);
        for (int i = 0; i < 2; i++) {
            if (i == 0) {
                cStack_f0.z = -30.0f;
            } else {
                cStack_f0.z = 220.0f;
            }

            i_this->field_0x6b6c[i] = dComIfGp_particle_set(i_this->field_0x6b6c[i], ID_ZI_J_LK_ABUKU_A, &cStack_f0, 0, &cStack_fc);
        }

        if (daPy_getPlayerActorClass()->checkFrontRollCrash()) {
            i_this->field_0x6b34 = 30 + JREG_S(3);
        }

        s16 iVar6 = i_this->field_0x6b34 * cM_ssin(i_this->field_0x6b34 * 5000) * (30.0f + JREG_F(17));
        if (i_this->field_0x6b34 != 0) {
            i_this->field_0x6b34--;
        }

        mDoMtx_stack_c::transS(-450.0f + NREG_F(7), 25.0f + NREG_F(8), -250.0f + NREG_F(9));
        mDoMtx_stack_c::YrotM(0x4000);
        mDoMtx_stack_c::ZrotM(2000 + NREG_S(8) + iVar6);
        i_this->canoeModel->setBaseTRMtx(mDoMtx_stack_c::get());
        mDoMtx_stack_c::transS(-450.0f + NREG_F(13), 0.0f, -250.0f + NREG_F(14));
        mDoMtx_stack_c::scaleM(4.0f + NREG_F(15), 1.5f + NREG_F(16), 1.0f + NREG_F(17));
        MTXCopy(mDoMtx_stack_c::get(), i_this->field_0x6b38);

        i_this->tableBgw->Move();

        local_c0.x = 325.0f - pPlayer->current.pos.x;
        local_c0.z = 237.0f - pPlayer->current.pos.z;
        if (daPy_getPlayerActorClass()->checkFrontRollCrash() && JMAFastSqrt(SQUARE(local_c0.x) + SQUARE(local_c0.z)) < 150.0f) {
            i_this->field_0x3ff8 = cM_rndF(65536.0f);
            i_this->field_0x3ffa = cM_rndF(65536.0f);
            i_this->field_0x3ffc = cM_rndF(600.0f) + 1300.0f;
            i_this->field_0x4000 = cM_rndF(600.0f) + 1300.0f;
        }

        ANGLE_ADD(i_this->field_0x3ff8, 4000);
        ANGLE_ADD(i_this->field_0x3ffa, 4000);
        s16 iVar10 = i_this->field_0x4000 * cM_ssin(i_this->field_0x3ffa);
        s16 iVar11 = i_this->field_0x3ffc * cM_ssin(i_this->field_0x3ff8);
        cLib_addCalc0(&i_this->field_0x3ffc, 1.0f, 40.0f);
        cLib_addCalc0(&i_this->field_0x4000, 1.0f, 40.0f);

        mDoMtx_stack_c::transS(325.0f + AREG_F(17), 140.0f + AREG_F(18), 237.0f + AREG_F(19));
        mDoMtx_stack_c::XrotM(iVar10 + YREG_S(7));
        mDoMtx_stack_c::ZrotM(iVar11 + 15000 + YREG_S(9));
        mDoMtx_stack_c::transM(-10.0f + AREG_F(14), -7.0f + AREG_F(15), 0.0f + AREG_F(16));
        i_this->hatModel->setBaseTRMtx(mDoMtx_stack_c::get());

        koro2_game(i_this);
    }

    return 1;
}

static int daFshop_IsDelete(fshop_class*) {
    return 1;
}

static int daFshop_Delete(fshop_class* i_this) {
    fopAc_ac_c* actor = &i_this->actor;
    fopAcM_RegisterDeleteID(i_this, "Fshop");

    if (i_this->field_0x6b7c != 0) {
        J3DModelData* pModelData = (J3DModelData*) dComIfG_getObjectRes("Fshop", 5);
        dComIfGp_removeSimpleModel(pModelData, fopAcM_GetRoomNo(actor));
        pModelData = (J3DModelData*) dComIfG_getObjectRes("Fshop", 6);
        dComIfGp_removeSimpleModel(pModelData, fopAcM_GetRoomNo(actor));
    }

    dComIfG_resDelete(&i_this->mPhase, "Fshop");
    dComIfG_Bgsp().Release(i_this->tableBgw);

    if (i_this->field_0x400e != 0) {
        dComIfG_Bgsp().Release(i_this->koro2WakuBgw);
        for (int i = 0; i < 100; i++) {
            if (i_this->mKoro2[i].bgw != NULL) {
                dComIfG_Bgsp().Release(i_this->mKoro2[i].bgw);
            }
        }
    }

    return 1;
}

static int koro2_heapinit(fopAc_ac_c* actor) {
#if PLATFORM_SHIELD
    static int sg_bmd[2] = {
        0x0F, 0x0D,
    };
#endif

    static int koro2_bmd[15] = {
        0x0A, 0x0B, 0x0B, 0x0B, 0x0B, 0x0F, 0x0D, 0x0C,
        0x0C, 0x0C, 0x0C, 0x0E, 0x0E, 0x0E, 0x0E,
    };

    static u32 koro2_dzb[15] = {
        0x1F, 0x20, 0x20, 0x20, 0x20, 0x24, 0x22, 0x21,
        0x21, 0x21, 0x21, 0x23, 0x23, 0x23, 0x23,
    };

    static s8 pande_d[16] = {
        0x00, 0x01, 0x03, 0x02, 0x05, 0x04, 0x06, 0x07,
        0x09, 0x08, 0x0B, 0x0A, 0x0C, 0x0D, 0x0F, 0x0E,
    };

    fshop_class* i_this = (fshop_class*)actor;

    i_this->koro2WakuBgw = new dBgW();
    if (i_this->koro2WakuBgw == NULL) {
        return 0;
    }

    if (i_this->koro2WakuBgw->Set((cBgD_t*)dComIfG_getObjectRes("Fshop", 37), 1, &i_this->field_0x4030) == 1) {
        return 0;
    }

    i_this->koro2WakuBgw->SetCrrFunc(dBgS_MoveBGProc_Typical);
    i_this->koro2WakuBgw->SetRideCallback(ride_call_back);
#if PLATFORM_SHIELD
    i_this->field_0x4020.y = -16384.0f + cM_rndFX(2000.0f);
    J3DModelData* modelData = (J3DModelData*) dComIfG_getObjectRes("Fshop", 16);
    JUT_ASSERT(2554, modelData != NULL);
    i_this->mpA_crwaku_model = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
    if (i_this->mpA_crwaku_model == NULL) {
        return FALSE;
    }

    for (int sp_0x44 = 0; sp_0x44 < 2; ++sp_0x44) {
        modelData = (J3DModelData*) dComIfG_getObjectRes("Fshop", sg_bmd[sp_0x44]);
        JUT_ASSERT(2571, modelData != NULL);
        i_this->dbg_models[sp_0x44] = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
        if (i_this->dbg_models[sp_0x44] == NULL) {
            return FALSE;
        }
    }
#else
    i_this->mpA_crwaku_model = ArcIX_A_crwaku_model;
#endif

    int part_no = 0;
    int index;
    int sp_0x38 = actor->field_0x567 & 0x7;
    int sp_0x34 = actor->field_0x567 & 0x8;
    int crstart_model_idx = 0;
    int crgoal_model_idx = 0;
    int crbox_model_idx = 0;
    int crcurve_a_model_idx = 0;
    int crcurve_b_model_idx = 0;
    int crcurve_c_model_idx = 0;

    s8* mapData = koro2_map_d[sp_0x38];

    for (int column = 0; column < 9; column++) {
        for (int row = 0; row < 15; row++) {
            index = (row * 9) + column;
            if (mapData[index] != 0) {
                if (sp_0x34) {
                    i_this->mKoro2[part_no].part_id = pande_d[mapData[index]];
                    i_this->mKoro2[part_no].pos.x = 2.5f * (8 - column);
                } else {
                    i_this->mKoro2[part_no].part_id = mapData[index];
                    i_this->mKoro2[part_no].pos.x = 2.5f * column;
                }

                i_this->mKoro2[part_no].pos.y = 2.0f + VREG_F(1);
                i_this->mKoro2[part_no].pos.z = 2.5f * row;

                if (i_this->mKoro2[part_no].part_id == KORO2_PART_START) {
                    i_this->mKoro2[part_no].pos.z -= 2.5f;
                } else if (i_this->mKoro2[part_no].part_id == KORO2_PART_CURVE_B_U_L) {
                    i_this->mKoro2[part_no].pos.z -= 2.5f;
                    i_this->mKoro2[part_no].pos.x -= 2.5f;
                } else if (i_this->mKoro2[part_no].part_id == KORO2_PART_CURVE_B_L_U) {
                    i_this->mKoro2[part_no].pos.z += 2.5f;
                    i_this->mKoro2[part_no].pos.x += 2.5f;
                }

                i_this->mKoro2[part_no].size.set(2.5f, 2.5f, 2.5f);

                if (i_this->mKoro2[part_no].part_id == KORO2_PART_CURVE_A_U_L ||
                    i_this->mKoro2[part_no].part_id == KORO2_PART_CURVE_B_U_L)
                {
                    i_this->mKoro2[part_no].rot_y = -0x4000;
                } else if (i_this->mKoro2[part_no].part_id == KORO2_PART_CURVE_A_R_U ||
                           i_this->mKoro2[part_no].part_id == KORO2_PART_CURVE_B_R_U)
                {
                    i_this->mKoro2[part_no].rot_y = -0x8000;
                } else if (i_this->mKoro2[part_no].part_id == KORO2_PART_CURVE_A_L_U ||
                           i_this->mKoro2[part_no].part_id == KORO2_PART_CURVE_B_L_U)
                {
                    i_this->mKoro2[part_no].rot_y = 0x4000;
                } else if (i_this->mKoro2[part_no].part_id == KORO2_PART_SLOPE_D) {
                    i_this->mKoro2[part_no].rot_y = -0x8000;
                } else if (i_this->mKoro2[part_no].part_id == KORO2_PART_SLOPE_L) {
                    i_this->mKoro2[part_no].rot_y = -0x4000;
                } else if (i_this->mKoro2[part_no].part_id == KORO2_PART_SLOPE_R) {
                    i_this->mKoro2[part_no].rot_y = 0x4000;
                }

#if PLATFORM_SHIELD
                modelData = (J3DModelData*) dComIfG_getObjectRes("Fshop", koro2_bmd[i_this->mKoro2[part_no].part_id - 1]);
                JUT_ASSERT(2704, modelData != NULL);
                i_this->mKoro2[part_no].model = mDoExt_J3DModel__create(modelData, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
#else
                if (koro2_bmd[i_this->mKoro2[part_no].part_id - 1] == 10) {
                    i_this->mKoro2[part_no].model = ArcIX_A_crbox_model[crbox_model_idx];
                    crbox_model_idx++;
                } else if (koro2_bmd[i_this->mKoro2[part_no].part_id - 1] == 15) {
                    i_this->mKoro2[part_no].model = ArcIX_A_crstart_model[crstart_model_idx];
                    crstart_model_idx++;
                } else if (koro2_bmd[i_this->mKoro2[part_no].part_id - 1] == 13) {
                    i_this->mKoro2[part_no].model = ArcIX_A_crgoal_model[crgoal_model_idx];
                    crgoal_model_idx++;
                } else if (koro2_bmd[i_this->mKoro2[part_no].part_id - 1] == 11) {
                    i_this->mKoro2[part_no].model = ArcIX_A_crcurve_a_model[crcurve_a_model_idx];
                    crcurve_a_model_idx++;
                } else if (koro2_bmd[i_this->mKoro2[part_no].part_id - 1] == 12) {
                    i_this->mKoro2[part_no].model = ArcIX_A_crcurve_b_model[crcurve_b_model_idx];
                    crcurve_b_model_idx++;
                } else if (koro2_bmd[i_this->mKoro2[part_no].part_id - 1] == 14) {
                    i_this->mKoro2[part_no].model = ArcIX_A_crsaka_model[crcurve_c_model_idx];
                    crcurve_c_model_idx++;
                }
#endif

                if (i_this->mKoro2[part_no].model == NULL) {
                    return 0;
                }

                i_this->mKoro2[part_no].bgw = new dBgW();
                if (i_this->mKoro2[part_no].bgw == NULL) {
                    return 0;
                }

                if (i_this->mKoro2[part_no].bgw->Set((cBgD_t*)dComIfG_getObjectRes("Fshop",koro2_dzb[i_this->mKoro2[part_no].part_id - 1]),
                    1, &i_this->mKoro2[part_no].bgMtx) == 1) {
                    return 0;
                }

                i_this->mKoro2[part_no].bgw->SetCrrFunc(dBgS_MoveBGProc_Typical);

                part_no++;
                if (part_no >= 100) {
                    OS_REPORT("//////////////KORO2 PART MAX OVER !!\n");
                    return 0;
                }
            }
        }
    }

    return 1;
}

static int useHeapInit(fopAc_ac_c* actor) {
    fshop_class* i_this = (fshop_class*)actor;
    cM_initRnd2(JREG_S(8) + 37, 123, 70);

    void* modelData;
    for (int i = 0; i < 48; i++) {
        i_this->mLure[i].field_0x00.x = -370.0f + VREG_F(6) + (-(i & 15) * (16.0f + VREG_F(8))) + cM_rndFX(2.5f);
        i_this->mLure[i].field_0x00.y = (215.0f + VREG_F(7)) - (((i >> 4) & 3) * (43.0f + VREG_F(9)));
        i_this->mLure[i].field_0x00.z = 593.0f + JREG_F(1);

        if (i >= 32 && i <= 33) {
            i_this->mLure[i].field_0x24 = 3;
            i_this->mLure[i].field_0x0c.x = JREG_S(4) - 0x4000;
            i_this->mLure[i].field_0x0c.y = cM_rndFX2(32768.0f);
            i_this->mLure[i].field_0x28 = 1.2f + JREG_F(0);
            i_this->mLure[i].field_0x2c = 1.2f + JREG_F(0);
            i_this->mLure[i].field_0x00.y += 13.0f;
        } else {
            if (cM_rndF2(1.0f) < 0.3f) {
                i_this->mLure[i].field_0x24 = cM_rndF2(2.99f);
            } else {
                i_this->mLure[i].field_0x24 = i + (i >> 4);
                i_this->mLure[i].field_0x24 &= 3;
                if (i_this->mLure[i].field_0x24 == 3) {
                    i_this->mLure[i].field_0x24 = cM_rndF2(2.99f);
                }
            }

            i_this->mLure[i].field_0x0c.x = JREG_S(3) + 0x4000;
            i_this->mLure[i].field_0x0c.y = cM_rndFX2(65536.0f);
            i_this->mLure[i].field_0x28 = 0.8f + JREG_F(0) + cM_rndFX2(0.1f);

            if (i_this->mLure[i].field_0x24 == 2) {
                i_this->mLure[i].field_0x2c = (0.71999997f + JREG_F(0)) - cM_rndF2(0.15f);
            } else {
                i_this->mLure[i].field_0x2c = 0.8f + JREG_F(0) + cM_rndFX2(0.1f);
            }
        }

        static int lure_d[] = {0x16, 0x17, 0x1C, 0x14};
        modelData = dComIfG_getObjectRes("Fshop", lure_d[i_this->mLure[i].field_0x24]);
        JUT_ASSERT(2832, modelData != NULL);

        i_this->mLure[i].model = mDoExt_J3DModel__create((J3DModelData*)modelData, 0x80000, 0x11000084);
        if (i_this->mLure[i].model == NULL) {
            return 0;
        }

        if (i_this->mLure[i].field_0x24 == 3) {
            i_this->mLure[i].model->setUserArea((uintptr_t)&i_this->mLure[i]);

            for (u16 j = 0; j < ((J3DModelData*)modelData)->getJointNum(); j++) {
                if (j == 1) {
                    ((J3DModelData*)modelData)->getJointNodePointer(j)->setCallBack(frog_CallBack);
                }
            }
        } else {
            modelData = dComIfG_getObjectRes("Fshop", 5);
            JUT_ASSERT(2852, modelData != NULL);

            for (int j = 0; j < 2; j++) {
                i_this->mLure[i].hookModel[j] = mDoExt_J3DModel__create((J3DModelData*)modelData, 0x20000, 0x11000084);
                if (i_this->mLure[i].hookModel[j] == NULL) {
                    return 0;
                }
            }
        }
    }

    cM_initRnd2(JREG_S(9) + 79, JREG_S(8) + 133, 70);

    for (int i = 0; i < 3; i++) {
        i_this->mRod[i].field_0x4c = cM_rndF2(1.99f);
        i_this->mRod[i].field_0x00.x = -685.0f + YREG_F(2);
        i_this->mRod[i].field_0x00.y = 18.0f + YREG_F(3);
        i_this->mRod[i].field_0x00.z = 565.0f + YREG_F(4);

        if (i == 1) {
            i_this->mRod[i].field_0x00.x -= 15.0f;
        } else if (i == 2) {
            i_this->mRod[i].field_0x00.z += 15.0f;
        }

        if (i_this->mRod[i].field_0x4c == 0) {
            i_this->mRod[i].field_0x54 = 30.0f * (1.2f + cM_rndF2(0.15f));
        } else {
            i_this->mRod[i].field_0x54 = 30.0f * (1.0f + cM_rndF2(0.15f));
        }

        static int rod_d[] = {0x19, 0x18};
        modelData = dComIfG_getObjectRes("Fshop", rod_d[i_this->mRod[i].field_0x4c]);
        JUT_ASSERT(2903, modelData != NULL);
        i_this->mRod[i].model = mDoExt_J3DModel__create((J3DModelData*)modelData, 0x80000, 0x11000084);
        if (i_this->mRod[i].model == NULL) {
            return 0;
        }

        i_this->mRod[i].model->setUserArea((uintptr_t)&i_this->mRod[i]);
        i_this->mRod[i].rot_x = cM_rndF2(65536.0f);

        for (u16 j = 0; j < ((J3DModelData*)modelData)->getJointNum(); j++) {
            if (j == 1) {
                ((J3DModelData*)modelData)->getJointNodePointer(j)->setCallBack(Reel_CallBack);
            }
        }

        modelData = dComIfG_getObjectRes("Fshop", 6);
        JUT_ASSERT(2923, modelData != NULL);

        for (int j = 0; j < 6; j++) {
            i_this->mRod[i].ringModel[j] = mDoExt_J3DModel__create((J3DModelData*)modelData, 0x20000, 0x11000084);
            if (i_this->mRod[i].ringModel[j] == NULL) {
                return 0;
            }
        }

        if (!i_this->mRod[i].line_mat.init(1, 8, 1)) {
            return 0;
        }

        f32* sp1C = i_this->mRod[i].line_mat.getSize(0);
        for (int j = 0; j < 8; j++, sp1C++) {
            static f32 rod_w[8] = {
                1.0f,
                0.89999998f,
                0.8f,
                0.69999999f,
                0.6f,
                0.5f,
                0.4f,
                0.3f,
            };

            *sp1C = rod_w[j];
        }
    }

    for (int i = 0; i < 2; i++) {
        if (i == 0) {
            i_this->mTsubo[i].field_0x00.set(177.0f, 290.0f, 550.0f);
            modelData = dComIfG_getObjectRes("Fshop", 0x1A);
        } else {
            i_this->mTsubo[i].field_0x00.set(40.0f, 290.0f, 560.0f);
            modelData = dComIfG_getObjectRes("Fshop", 0x1B);
        }

        JUT_ASSERT(2969, modelData != NULL);

        i_this->mTsubo[i].model = mDoExt_J3DModel__create((J3DModelData*)modelData, 0x80000, 0x11000084);
        if (i_this->mTsubo[i].model == NULL) {
            return 0;
        }
    }

    if (!i_this->field_0x3f88.init(60, 15, 0)) {
        return 0;
    }

    modelData = dComIfG_getObjectRes("Fshop", 0x12);
    JUT_ASSERT(3035, modelData != NULL);
    i_this->canoeModel = mDoExt_J3DModel__create((J3DModelData*)modelData, 0x80000, 0x11000084);
    if (i_this->canoeModel == NULL) {
        return 0;
    }

    i_this->tableBgw = new dBgW();
    if (i_this->tableBgw == NULL) {
        return 0;
    }

    if (i_this->tableBgw->Set((cBgD_t*)dComIfG_getObjectRes("Fshop", 0x26), 1, &i_this->field_0x6b38) == 1) {
        return 0;
    }

    i_this->tableBgw->SetCrrFunc(dBgS_MoveBGProc_Typical);

    modelData = dComIfG_getObjectRes("Fshop", 0x15);
    JUT_ASSERT(3069, modelData != NULL);
    i_this->hatModel = mDoExt_J3DModel__create((J3DModelData*)modelData, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
    if (i_this->hatModel == NULL) {
        return 0;
    }

#if !PLATFORM_SHIELD
    i_this->field_0x4020.y = cM_rndFX(2000.0f) + -16384.0f;

    modelData = dComIfG_getObjectRes("Fshop", 16);
    JUT_ASSERT(3069, modelData != NULL);
    ArcIX_A_crwaku_model = mDoExt_J3DModel__create((J3DModelData*)modelData, J3DMdlFlag_DifferedDLBuffer, 0x11000084);
    if (ArcIX_A_crwaku_model == NULL) {
        return 0;
    }

    J3DModel** var_r24;
    for (int i = 0; i < 6; i++) {
        static int koro2_union_bmd[] = {
            0xF,
            0xD,
            0xA,
            0xB,
            0xC,
            0xE,
        };

        modelData = dComIfG_getObjectRes("Fshop", koro2_union_bmd[i]);

        static int koro2_union_no[] = {
            1,
            1,
            31,
            16,
            4,
            8,
        };

        var_r24 = koro2_union_model[i];
        for (int j = 0; j < koro2_union_no[i]; j++, var_r24++) {
            *var_r24 = mDoExt_J3DModel__create((J3DModelData*)modelData, 0x80000, 0x11000084);
            if (*var_r24 == NULL) {
                return 0;
            }
        }
    }
#else
    if (!koro2_heapinit(actor)) {
        return 0;
    }
#endif

    if ((int)dComIfGs_getEventReg(0xF47F) >= 10) {
        modelData = dComIfG_getObjectRes("Fshop", 0x11);
        JUT_ASSERT(3137, modelData != NULL);
        i_this->photoModel = mDoExt_J3DModel__create((J3DModelData*)modelData, 0x80000, 0x11000084);
        if (i_this->photoModel == NULL) {
            return 0;
        }
    }

    return 1;
}

static int BalluseHeapInit(fopAc_ac_c* actor) {
    fshop_class* i_this = (fshop_class*)actor;

    void* modelData = dComIfG_getObjectRes("Fshop", 9);
    JUT_ASSERT(3157, modelData != NULL);
    i_this->ballModel = mDoExt_J3DModel__create((J3DModelData*)modelData, 0x80000, 0x11000084);
    if (i_this->ballModel == NULL) {
        return 0;
    }

    return 1;
}

static int daFshop_Create(fopAc_ac_c* actor) {
    fshop_class* i_this = (fshop_class*)actor;
    fopAcM_ct(&i_this->actor, fshop_class);

    int phase_state = dComIfG_resLoad(&i_this->mPhase, "Fshop");
    if (phase_state == cPhs_COMPLEATE_e) {
        OS_REPORT("FSHOP PARAM %x\n", fopAcM_GetParam(actor));
        if ((fopAcM_GetParam(actor) & 0xFF) == 0x23) {
            if (!fopAcM_entrySolidHeap(actor, BalluseHeapInit, 0x800)) {
                OS_REPORT("//////////////FSHOP BALL SET NON !!\n");
                return cPhs_ERROR_e;
            }

            i_this->ballAcch.Set(fopAcM_GetPosition_p(actor), fopAcM_GetOldPosition_p(actor), actor, 1, &i_this->ballAcchcir, fopAcM_GetSpeed_p(actor), NULL, NULL);
            i_this->ballAcchcir.SetWall(9.0f, 11.0f);
            i_this->ballAcch.OnWallSort();
            return phase_state;
        }

        if ((s8)(fopAcM_GetParam(actor) & 0xFF) >= 100) {
            i_this->field_0x400e = (fopAcM_GetParam(actor) & 0xFF) - 100;
            actor->field_0x567 = ((i_this->field_0x400e - 1) | (dComIfGs_getEventReg(0xF63F) & 8));

            u32 heapsizes[] =
#if PLATFORM_SHIELD
            {
                0x171C0,
                0x18EF0,
                0x20970,
                0x28E40,
                0x14F50,
                0x19FA0,
                0xF250,
                0x1BF50,
            };
#else
            {
                0x84A0,
                0x9AC0,
                0xB440,
                0xE100,
                0x8220,
                0x9A40,
                0x64E0,
                0xA9E0,
            };
#endif

            if (!fopAcM_entrySolidHeap(actor, koro2_heapinit, heapsizes[i_this->field_0x400e - 1])) {
                OS_REPORT("//////////////FSHOP KORO222 SET NON !!\n");
                return cPhs_ERROR_e;
            }

            if (i_this->koro2WakuBgw != NULL && dComIfG_Bgsp().Regist(i_this->koro2WakuBgw, &i_this->actor)) {
                return cPhs_ERROR_e;
            }

            for (int i = 0; i < 100; i++) {
                if (i_this->mKoro2[i].bgw != NULL && dComIfG_Bgsp().Regist(i_this->mKoro2[i].bgw, &i_this->actor)) {
                    return cPhs_ERROR_e;
                }
            }

            i_this->field_0x428d = 30;
#if !PLATFORM_SHIELD
            i_this->field_0x400d = 1;
#endif
            return phase_state;
        }

        fopAcM_SetParam(actor, 0);
        i_this->field_0x400c = dComIfGs_getEventReg(0xF63F) & 0xFF;

        OS_REPORT("FSHOP//////////////FSHOP SET 1 !!\n");

#if PLATFORM_SHIELD
    #define HEAP_SIZE 0x522E0
#else
    #define HEAP_SIZE 0x5B000
#endif

        if (!fopAcM_entrySolidHeap(actor, useHeapInit, HEAP_SIZE)) {
            OS_REPORT("//////////////FSHOP SET NON !!\n");
            return cPhs_ERROR_e;
        }

        OS_REPORT("//////////////FSHOP SET 2 !!\n");

        if (i_this->tableBgw != NULL && dComIfG_Bgsp().Regist(i_this->tableBgw, &i_this->actor)) {
            return cPhs_ERROR_e;
        }

        for (int i = 0; i < 60; i++) {
            if (i < 30) {
                i_this->mWeed[i].field_0x00[0].z = -30.0f + cM_rndFX2(20.0f);
                i_this->mWeed[i].field_0x00[0].y = 67.0f;
                i_this->mWeed[i].field_0xb4 = 0.9f + cM_rndF2(0.6f);
            } else {
                i_this->mWeed[i].field_0x00[0].z = 220.0f + cM_rndFX2(20.0f);
                i_this->mWeed[i].field_0x00[0].y = 67.0f;
                i_this->mWeed[i].field_0xb4 = 0.9f + cM_rndF2(0.6f);
            }

            i_this->mWeed[i].field_0x00[0].x = -720.0f + cM_rndFX2(20.0f);
            i_this->mWeed[i].field_0xbc = cM_rndF2(65536.0f);
        }

        daFshop_Execute(i_this);

        void* modelData = dComIfG_getObjectRes("Fshop", 5);
        JUT_ASSERT(3354, modelData != NULL);
        if (dComIfGp_addSimpleModel((J3DModelData*)modelData, fopAcM_GetRoomNo(actor), 0) == -1) {
            OS_REPORT("\x1B[43;30m針のシンプルモデル登録失敗しました。\n\x1B[m");
        }

        modelData = dComIfG_getObjectRes("Fshop", 6);
        JUT_ASSERT(3362, modelData != NULL);
        if (dComIfGp_addSimpleModel((J3DModelData*)modelData, fopAcM_GetRoomNo(actor), 0) == -1) {
            OS_REPORT("\x1B[43;30mリングのシンプルモデル登録失敗しました。\n\x1B[m");
        }
    
        i_this->field_0x6b7c = 1;

        for (int i = 0; i < 8; i++) {
            fopAcM_createChild(PROC_FSHOP, fopAcM_GetID(actor), i - 155, &actor->current.pos, fopAcM_GetRoomNo(actor), NULL, NULL, -1, NULL);
        }
    
        fopAcM_createChild(PROC_FSHOP, fopAcM_GetID(actor), 0xFFFFFF23, &actor->current.pos, fopAcM_GetRoomNo(actor), NULL, NULL, -1, NULL);

        u8 sp10;
#if VERSION == VERSION_GCN_PAL || VERSION == VERSION_WII_PAL
        if (dComIfGs_getPalLanguage() == dSv_player_config_c::LANGUAGE_ENGLISH) {
            sp10 = 2;
        } else {
            sp10 = 0;
        }
#elif PLATFORM_SHIELD
        if (dComIfGs_getPalLanguage() == dSv_player_config_c::LANGUAGE_GERMAN) {
            sp10 = 2;
        } else {
            sp10 = 0;
        }
#else
        sp10 = 1;
#endif

        u32 parameters;
        int sp24;
        for (int i = 0; i <= 3; i++) {
            static u16 check_kind[] = {
                0xF57F,
                0xF47F,
                0xF37F,
                0xF27F,
            };

            if (sp10 == 1) {
#if VERSION == VERSION_GCN_JPN
                sp24 = dComIfGs_getEventReg(check_kind[i]);
#else
                sp24 = 2.54f * dComIfGs_getEventReg(check_kind[i]);
#endif
            } else {
                sp24 = dComIfGs_getEventReg(check_kind[i]);
            }

            if (sp24 >= 10) {
                parameters = (sp24 << 8) | 0xFFFF0000 | i;
                fopAcM_create(PROC_MG_FISH, parameters, &actor->current.pos, fopAcM_GetRoomNo(actor), NULL, NULL, -1);
            }
        }

        cXyz work_pos(-648.0f + YREG_F(7), 215.0f + YREG_F(8), 380.0f + YREG_F(9));
        csXyz work_angle(0, 0, 0);
        fopAcM_create(PROC_OBJ_KAGE, 0xFFFFFF01, &work_pos, fopAcM_GetRoomNo(actor), &work_angle, NULL, -1);

        for (int i = 0; i < 8; i++) {
            fopAcM_create(PROC_MG_FISH, 0xFFFF2005, &actor->current.pos, fopAcM_GetRoomNo(actor), NULL, NULL, -1);
        }

        work_pos.set(-131.0f, 3000.0f + nREG_F(7), -4500.0f);
        fopAcM_create(PROC_NPC_TK, -1, &work_pos, fopAcM_GetRoomNo(actor), NULL, NULL, -1);

        for (int i = 0; i < 5; i++) {
            work_pos.set(-450.0f, 0.0f, -878.0f);
            fopAcM_create(PROC_BD, 0xFFFFFFFF, &work_pos, fopAcM_GetRoomNo(actor), NULL, NULL, -1);
        }
    }

    return phase_state;
}

static actor_method_class l_daFshop_Method = {
    (process_method_func)daFshop_Create,
    (process_method_func)daFshop_Delete,
    (process_method_func)daFshop_Execute,
    (process_method_func)daFshop_IsDelete,
    (process_method_func)daFshop_Draw,
};

actor_process_profile_definition g_profile_FSHOP = {
  fpcLy_CURRENT_e,       // mLayerID
  7,                     // mListID
  fpcPi_CURRENT_e,       // mListPrio
  PROC_FSHOP,            // mProcName
  &g_fpcLf_Method.base, // sub_method
  0x00006B80,            // mSize
  0,                     // mSizeOther
  0,                     // mParameters
  &g_fopAc_Method.base,  // sub_method
  721,                   // mPriority
  &l_daFshop_Method,     // sub_method
  0x00044000,            // mStatus
  fopAc_ACTOR_e,         // mActorType
  fopAc_CULLBOX_0_e,     // cullType
};

AUDIO_INSTANCES;
