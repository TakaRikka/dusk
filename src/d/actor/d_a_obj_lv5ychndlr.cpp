/**
 * @file d_a_obj_lv5ychndlr.cpp
 * Snowpeak Ruins Chandelier (Swinging Platform)
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv5ychndlr.h"
#include "SSystem/SComponent/c_math.h"
#include "d/actor/d_a_player.h"
#include "d/d_bg_w.h"
#include "d/d_com_inf_game.h"

static char* l_arcName = "Ychandelr";

int daObjYchndlr_c::create1st() {
    int phase = dComIfG_resLoad(this, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        phase = MoveBGCreate(l_arcName, 8, dBgS_MoveBGProc_TypicalRotY, 0x4DB0, &field_0x5a8);
        if (phase == cPhs_ERROR_e) {
            return phase;
        }
    }

    return phase;
}

static const int l_bmdidx[] = {4, 5};

static const cull_box l_cull_box = {
    {-1500.0f, -2500.0f, -500.0f},
    {1500.0f, 200.0f, 500.0f},
};

static Vec l_shaft_vec[] = {
    {-140.0f, 0.0f, 200.0f},
    {140.0f, 0.0f, 200.0f},
    {-140.0f, 0.0f, -200.0f},
    {140.0f, 0.0f, -200.0f},
};

static s16 l_shaft_roty[] = {0x7FFF, 0x7FFF, 0x0000, 0x0000};

void daObjYchndlr_c::setMtx() {
    for (int i = 0; i < 4; i++) {
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
        mDoMtx_stack_c::YrotM(current.angle.y);
        mDoMtx_stack_c::transM(l_shaft_vec[i].x, l_shaft_vec[i].y, l_shaft_vec[i].z);
        mDoMtx_stack_c::ZrotM(mShaftRotZ);
        mDoMtx_stack_c::YrotM(l_shaft_roty[i]);
        mpShaftModels[i]->setBaseTRMtx(mDoMtx_stack_c::get());
    }

    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(current.angle.y);
    MTXCopy(mDoMtx_stack_c::get(), field_0x5d8);

    mDoMtx_stack_c::ZrotM(mShaftRotZ);
    mDoMtx_stack_c::transM(0.0f, -840.0f, 0.0f);
    MTXMultVec(mDoMtx_stack_c::get(), &cXyz::Zero, &mChandelierCenter);

    mDoMtx_stack_c::transS(mChandelierCenter.x, mChandelierCenter.y, mChandelierCenter.z);
    mDoMtx_stack_c::YrotM(current.angle.y);
    mpChandlierModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), field_0x5a8);
}

static void rideCallBack(dBgW* i_bgw, fopAc_ac_c* i_this, fopAc_ac_c* i_rideActor) {
    ((daObjYchndlr_c*)i_this)->rideActor(i_rideActor);
}

void daObjYchndlr_c::rideActor(fopAc_ac_c* i_actor) {
    field_0xc8c = 1;

    daPy_py_c* player = (daPy_py_c*)dComIfGp_getPlayer(0);
    if (i_actor == player && player->getIronBallBgHit() && !mCcCyl.ChkTgHit()) {
        field_0xc90 = 1.0f;

        if (player->current.pos.x - current.pos.x > 0.0f) {
            field_0x620 = 0;
        } else {
            field_0x620 = 1;
        }
    }
}

int daObjYchndlr_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, l_bmdidx[0]);
    JUT_ASSERT(0, model_data != NULL);

    mpChandlierModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpChandlierModel == NULL) {
        return 0;
    }

    model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, l_bmdidx[1]);
    JUT_ASSERT(0, model_data != NULL);

    for (int i = 0; i < 4; i++) {
        mpShaftModels[i] = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
        if (mpShaftModels[i] == NULL) {
            return 0;
        }
    }

    return 1;
}

static dCcD_SrcCyl l_cc_src_shaft_cyl = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x11}, 0x79}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                  // mGObjAt
        {dCcD_SE_NONE, 0x5, 0x0, 0x0, 0x2},                  // mGObjTg
        {0x0},                                               // mGObjCo
    },                                                       // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f},  // mCenter
            50.0f,               // mRadius
            100.0f               // mHeight
        }  // mCyl
    }  // mCylAttr
};

static dCcD_SrcCyl l_cc_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x408000, 0x11}, 0x0}},  // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},               // mGObjAt
        {dCcD_SE_NONE, 0x5, 0x0, 0x0, 0x2},               // mGObjTg
        {0x0},                                            // mGObjCo
    },                                                    // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f},  // mCenter
            350.0f,              // mRadius
            150.0f               // mHeight
        }  // mCyl
    }  // mCylAttr
};

int daObjYchndlr_c::Create() {
    fopAcM_SetMtx(this, field_0x5d8);
    setMtx();
    fopAcM_setCullSizeBox(this, l_cull_box.min.x, l_cull_box.min.y, l_cull_box.min.z,
                          l_cull_box.max.x, l_cull_box.max.y, l_cull_box.max.z);

    mCcStts.Init(0xFF, 0, this);

    for (int i = 0; i < 4; i++) {
        mShaftCcCyl[i].Set(l_cc_src_shaft_cyl);
        mShaftCcCyl[i].SetStts(&mCcStts);
    }

    mCcCyl.Set(l_cc_cyl_src);
    mCcCyl.SetStts(&mCcStts);
    mpBgW->SetRideCallback(rideCallBack);

    mShaftRotZ = 0;
    field_0x61e = 50;
    field_0x620 = 1;
    field_0xc8c = 0;
    field_0xc90 = 0.0f;
    attention_info.distances[fopAc_attn_BATTLE_e] = 91;
    return 1;
}

int daObjYchndlr_c::Execute(Mtx** param_0) {
    s16 var_r27 = mShaftRotZ;
    if (var_r27 < 0) {
        var_r27 *= -1;
    }

    if (!cM3d_IsZero(field_0xc90)) {
        if (field_0xc90 < 0.1f) {
            field_0xc90 = 0.0f;
        }

        if (field_0x620) {
            field_0x61e = field_0xc90 * ((field_0xc90 * 4750.0f - (f32)var_r27) / 4750.0f) * 350.0f;
            if (field_0x61e < 35) {
                field_0x61e = 35;
            }

            if (mShaftRotZ + field_0x61e > 0 && mShaftRotZ < 0) {
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_CHANDLV5_SWING, &mChandelierCenter, field_0x61e,
                                         0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            }

            mShaftRotZ += field_0x61e;

            if (mShaftRotZ >= field_0xc90 * 4750.0f) {
                field_0x620 = 0;
            }
        } else {
            field_0x61e =
                field_0xc90 * ((field_0xc90 * 4750.0f - (f32)var_r27) / 4750.0f) * -350.0f;
            if (field_0x61e > -35) {
                field_0x61e = -35;
            }

            if (mShaftRotZ + field_0x61e < 0 && mShaftRotZ > 0) {
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_CHANDLV5_SWING, &mChandelierCenter, -field_0x61e,
                                         0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
            }

            mShaftRotZ += field_0x61e;

            if (mShaftRotZ <= field_0xc90 * -4750.0f) {
                field_0x620 = 1;
            }
        }

        field_0xc90 *= 0.9985f;
    }

    setMtx();
    *param_0 = &field_0x5a8;

    for (int i = 0; i < 4; i++) {
        cXyz shaft_center;
        MTXMultVec(field_0x5a8, &l_shaft_vec[i], &shaft_center);
        mShaftCcCyl[i].SetC(shaft_center);
        dComIfG_Ccsp()->Set(&mShaftCcCyl[i]);
    }

    if (mCcCyl.ChkTgHit() && field_0xc8c == 0) {
        cXyz dist_to_player = dComIfGp_getPlayer(0)->current.pos - mChandelierCenter;

        if (!cM3d_IsZero(dist_to_player.getSquareMag())) {
            dist_to_player.normalize();

            s16 var_r26 = current.angle.y - cM_atan2s(dist_to_player.x, dist_to_player.z);
            if (var_r26 > 0x2FFF && var_r26 < 0x4FFF) {
                u32 spC0;
                if (field_0x61e < 0) {
                    spC0 = -field_0x61e;
                } else {
                    spC0 = field_0x61e;
                }

                Z2GetAudioMgr()->seStart(Z2SE_OBJ_CHANDLV5_HIT, &mChandelierCenter, spC0, 0, 1.0f,
                                         1.0f, -1.0f, -1.0f, 0);
                field_0x620 = 1;
                field_0xc90 = 1.0f;
            } else if (var_r26 < -0x2FFF && var_r26 > -0x4FFF) {
                u32 spC4;
                if (field_0x61e < 0) {
                    spC4 = -field_0x61e;
                } else {
                    spC4 = field_0x61e;
                }

                Z2GetAudioMgr()->seStart(Z2SE_OBJ_CHANDLV5_HIT, &mChandelierCenter, spC4, 0, 1.0f,
                                         1.0f, -1.0f, -1.0f, 0);
                field_0x620 = 0;
                field_0xc90 = 1.0f;
            } else if (field_0xc90 < 0.12f) {
                field_0x620 = 1;
                field_0xc90 = 0.12f;
            }
        }
    }

    cXyz cyl_center(mChandelierCenter);
    cyl_center.y -= 120.0f;
    mCcCyl.SetC(cyl_center);
    dComIfG_Ccsp()->Set(&mCcCyl);
    mCcStts.Move();

    if (field_0xc8c) {
        fopAcM_OffStatus(this, 0);
        attention_info.flags &= ~fopAc_AttnFlag_BATTLE_e;
    } else {
        fopAcM_OnStatus(this, 0);
        attention_info.flags = fopAc_AttnFlag_BATTLE_e;

        Vec attn_pos, spA0, offset;
        f32 var_f31, var_f30;

        offset.x = 210.0f;
        offset.y = -30.0f;
        offset.z = 0.0f;
        MTXMultVec(field_0x5a8, &offset, &attn_pos);
        fopAc_ac_c* player = (fopAc_ac_c*)dComIfGp_getPlayer(0);
        var_f30 = VECSquareDistance(&player->current.pos, &attn_pos);

        offset.x = -210.0f;
        MTXMultVec(field_0x5a8, &offset, &spA0);
        var_f31 = VECSquareDistance(&player->current.pos, &spA0);
        if (var_f30 > var_f31) {
            var_f30 = var_f31;
            attn_pos = spA0;
        }

        offset.x = 0.0f;
        offset.z = 210.0f;
        MTXMultVec(field_0x5a8, &offset, &spA0);
        var_f31 = VECSquareDistance(&player->current.pos, &spA0);
        if (var_f30 > var_f31) {
            var_f30 = var_f31;
            attn_pos = spA0;
        }

        offset.z = -210.0f;
        MTXMultVec(field_0x5a8, &offset, &spA0);
        var_f31 = VECSquareDistance(&player->current.pos, &spA0);
        if (var_f30 > var_f31) {
            var_f30 = var_f31;
            attn_pos = spA0;
        }

        attention_info.position = attn_pos;
        eyePos = attn_pos;
    }

    field_0xc8c = 0;
    return 1;
}

int daObjYchndlr_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpChandlierModel, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpChandlierModel);

    for (int i = 0; i < 4; i++) {
        mDoExt_modelUpdateDL(mpShaftModels[i]);
    }

    dComIfGd_setList();
    return 1;
}

int daObjYchndlr_c::Delete() {
    dComIfG_resDelete(this, l_arcName);
    return 1;
}

static int daObjYchndlr_create1st(daObjYchndlr_c* i_this) {
    fopAcM_ct(i_this, daObjYchndlr_c);
    return i_this->create1st();
}

static int daObjYchndlr_MoveBGDelete(daObjYchndlr_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjYchndlr_MoveBGExecute(daObjYchndlr_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjYchndlr_MoveBGDraw(daObjYchndlr_c* i_this) {
    return i_this->MoveBGDraw();
}

static actor_method_class daObjYchndlr_METHODS = {
    (process_method_func)daObjYchndlr_create1st,     (process_method_func)daObjYchndlr_MoveBGDelete,
    (process_method_func)daObjYchndlr_MoveBGExecute, (process_method_func)NULL,
    (process_method_func)daObjYchndlr_MoveBGDraw,
};

actor_process_profile_definition g_profile_Obj_Ychndlr = {
    fpcLy_CURRENT_e,         // mLayerID
    3,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_Ychndlr,        // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daObjYchndlr_c),  // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    664,                     // mPriority
    &daObjYchndlr_METHODS,   // sub_method
    0x00040100,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
