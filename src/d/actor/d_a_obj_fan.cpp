//
// Fan
//

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_fan.h"
#include "d/actor/d_a_obj_tornado2.h"
#include "d/d_com_inf_game.h"
#include "d/d_bg_w.h"

static int const l_bmd[3] = {
    4,4,4,
};

static int const l_dzb2[3] = {
    7,8,8,
};

static int const l_dzb3[3] = {
    7,9,9,
};

static int const l_se[3] = {
    Z2SE_OBJ_FAN_ROLL_S, Z2SE_OBJ_FAN_ROLL_M, Z2SE_OBJ_FAN_ROLL_L,
};

static int const l_wind_se[3] = {
    Z2SE_OBJ_FAN_WIND_S, Z2SE_OBJ_FAN_WIND_M, Z2SE_OBJ_FAN_WIND_L,
};

static s16 const l_max_rotspeed[3] = {
    0x2000,
    0x2000,
    0x0BB8,
};

static u32 const l_heap_size[3] = {
    0x1680, 0x9AA0, 0x7E80,
};

const static dCcD_SrcSph l_sph_src = {
    {
        {0x0, {{AT_TYPE_CSTATUE_SWING, 0x1, 0x1f}, {0x0, 0x11}, 0x0}}, // mObj
        {dCcD_SE_STONE, 0x0, 0x2, 0x0, 0x8}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x2}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 150.0f} // mSph
    } // mSphAttr
};

static void* searchTornado2(void* param_1, void* param_2) {
    fopAc_ac_c* actor = (fopAc_ac_c*)param_1;
    if (actor != NULL && fopAcM_IsActor(actor) && fopAcM_GetProfName(actor) == 0x15c &&
        ((fopAc_ac_c*)param_2)->current.pos.abs(actor->current.pos) < 100.0f)
    {
        return actor;
    }
    return NULL;
}

fopAc_ac_c* daObjFan_c::search_tornado() {
    fopAc_ac_c* rv = NULL;
    field_0xad0 = parentActorID;
    if (parentActorID == -1) {
        rv = (fopAc_ac_c*)fpcM_Search(searchTornado2, this);
        if (rv != NULL) {
            parentActorID = fopAcM_GetID(rv);
        }
    }
    return rv;
}

void daObjFan_c::initBaseMtx() {
    mModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjFan_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::ZrotM(field_0xace);
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
    if (field_0xad4 == 0 || (mpBgW != NULL && mpBgW->ChkUsed())) {
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(home.angle);
    } else {
        mDoMtx_stack_c::transS(current.pos);
        mDoMtx_stack_c::ZXYrotM(shape_angle);
        mDoMtx_stack_c::ZrotM(field_0xace);
    }
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

int daObjFan_c::Create() {
    field_0xace = 0x2000;
    home.angle.x -= 0x4000;
    current.angle.x = home.angle.x;
    shape_angle.x = home.angle.x;
    initBaseMtx();
    fopAcM_SetMtx(this, mModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mModel->getModelData());
    field_0x5b0.Init(0xff, 0xff, this);
    for (int i = 0; i < 4; i++) {
        field_0x5ec[i].Set(l_sph_src);
        field_0x5ec[i].SetStts(&field_0x5b0);
    }
    MoveBGExecute();
    return 1;
}

static char* l_arcName[3] = {
    "Obj_prop1",
    "Obj_prop0",
    "Obj_prop2",
};

int daObjFan_c::CreateHeap() {
    void* modelData = dComIfG_getObjectRes(l_arcName[field_0xad4], l_bmd[field_0xad4]);
    JUT_ASSERT(352, modelData != NULL);
    mModel = mDoExt_J3DModel__create((J3DModelData*)modelData, 0x80000, 0x11000084);
    if (mModel == NULL) {
        return 0;
    }
    field_0x5ac = new dBgW();
    if (field_0x5ac == NULL ||
        field_0x5ac->Set((cBgD_t*)dComIfG_getObjectRes(l_arcName[field_0xad4], l_dzb3[field_0xad4]),
                         1, &mBgMtx))
    {
        field_0x5ac = NULL;
        return 0;
    }
    return 1;
}

int daObjFan_c::create1st() {
    field_0xad4 = getType();
    int rv = dComIfG_resLoad(&mPhase, l_arcName[field_0xad4]);
    switch (rv) {
    case 4:
        parentActorID = 0xffffffff;
        field_0xad0 = parentActorID;
        rv = MoveBGCreate(l_arcName[field_0xad4], l_dzb2[field_0xad4], dBgS_MoveBGProc_TypicalRotY,
                          l_heap_size[field_0xad4], NULL);
        switch (rv) {
        case 5:
            break;
        }
        break;
    }
    return rv;
}

int daObjFan_c::Execute(f32 (**param_1)[3][4]) {
    search_tornado();
    if (field_0xad4 != 0) {
        if (field_0xacc > l_max_rotspeed[field_0xad4] * 0.5f) {
            if (mpBgW != 0 && !mpBgW->ChkUsed()) {
                dComIfG_Bgsp().Regist(mpBgW, this);
            }
            if (field_0x5ac != NULL && field_0x5ac->ChkUsed())
            {
                dComIfG_Bgsp().Release(field_0x5ac);
            }
        } else {
            if (field_0x5ac != NULL && !field_0x5ac->ChkUsed())
            {
                dComIfG_Bgsp().Regist(field_0x5ac, this);
            }
            if (mpBgW != NULL && mpBgW->ChkUsed())
            {
                dComIfG_Bgsp().Release(mpBgW);
            }
        }
    }
    if (field_0x5ac != NULL) {
        field_0x5ac->Move();
    }
    if (field_0xacc > l_max_rotspeed[field_0xad4] * 0.5f)
    {
        setCollision();
    }
    action();
    *param_1 = &mBgMtx;
    setBaseMtx();
    return 1;
}

void daObjFan_c::action() {
    f32 dVar11 = 0.0f;
    f32 dVar10 = 0.0f;
    fopAc_ac_c* local_58 = NULL;
    if (parentActorID != -1 && fopAcM_SearchByID(parentActorID, &local_58) != 0 && local_58 != NULL)
    {
        dVar11 = ((daObjTrnd2_c*)local_58)->getTargetLength();
        dVar10 = ((daObjTrnd2_c*)local_58)->getNowLength();
    }

    s16 sVar8;
    if (dVar11 == 0.0f) {
        sVar8 = 0;
    } else {
        sVar8 = l_max_rotspeed[field_0xad4];
    }

    if (field_0xad0 == -1 && parentActorID != -1) {
        field_0xacc = sVar8;
    } else if (sVar8 != 0) {
        cLib_addCalcAngleS(&field_0xacc, sVar8, 50, l_max_rotspeed[field_0xad4] * 0.02f, 10);
    } else {
        s16 unaff_r27;
        cLib_addCalcAngleS(&field_0xacc, sVar8, 50, l_max_rotspeed[field_0xad4] * 0.02f, 10);
        if (field_0xace > -0x2000 && 0x2000 >= field_0xace) {
            unaff_r27 = 0x2000;
        } else {
            if (field_0xace > 0x2000 && 0x6000 >= field_0xace) {
                unaff_r27 = 0x6000;
            } else {
                if ((field_0xace > 0x6000 && field_0xace <= 0x7fff) ||
                    (-0x6000 > field_0xace && field_0xace > -0x7fff))
                {
                    unaff_r27 = -0x6000;
                } else if (-0x6000 < field_0xace && field_0xace <= -0x2000) {
                    unaff_r27 = -0x2000;
                }
            }
        }
        cLib_addCalcAngleS(&field_0xace, unaff_r27, 50, l_max_rotspeed[field_0xad4] * 0.02f, 10);
        
    }
    field_0xace += field_0xacc;
    if (field_0xacc != 0) {
        u32 uVar5 = (field_0xacc * 127.0f) / l_max_rotspeed[field_0xad4];
        if (fopAcM_GetRoomNo(this) == 4) {
            fopAcM_seStartLevel(this, Z2SE_OBJ_FAN_ROLL_M, uVar5);
        } else {
            fopAcM_seStartLevel(this, l_se[field_0xad4], uVar5);
        }
    }
    if (dVar10 != 0.0f) {
        if (fopAcM_GetRoomNo(this) == 4) {
            fopAcM_seStartLevel(this, Z2SE_OBJ_FAN_WIND_M, 0);
        } else if (l_wind_se[field_0xad4] == Z2SE_OBJ_FAN_WIND_S) {
            if (local_58 != NULL) {
                field_0xad8 = ((daObjTrnd2_c*)local_58)->getWindEndPos();
            } else {
                field_0xad8 = eyePos;
            }
            mDoAud_seStartLevel(l_wind_se[field_0xad4], &field_0xad8, 0, 0);
        } else {
            fopAcM_seStartLevel(this, l_wind_se[field_0xad4], 0);
        }
    }
}

static Vec l_offset_posM[4] = {
    {-150.0f, 150.0f, 0.0f},
    {-150.0f, -150.0f, 0.0f},
    {150.0f, 150.0f, 0.0f},
    {150.0f, -150.0f, 0.0f},
};

static Vec l_offset_posL[4] = {
    {-300.0f, 300.0f, 0.0f},
    {-300.0f, -300.0f, 0.0f},
    {300.0f, 300.0f, 0.0f},
    {300.0f, -300.0f, 0.0f},
};

void daObjFan_c::setCollision() {
    if (field_0xad4 == 0) {
        return;
    }
    mDoMtx_stack_c::ZXYrotS(shape_angle);
    mDoMtx_stack_c::ZrotM(field_0xace);
    for (int i = 0; i < 4; i++) {
        cXyz acStack_28;
        if (field_0xad4 == 1) {
            mDoMtx_stack_c::multVec(
                &l_offset_posM[i],
                &acStack_28);
            field_0x5ec[i].SetR(150.0f);
        } else if (field_0xad4 == 2) {
            mDoMtx_stack_c::multVec(&l_offset_posL[i],
                &acStack_28);
            field_0x5ec[i].SetR(300.0f);
        }
        acStack_28 += current.pos;
        field_0x5ec[i].SetC(acStack_28);
        dComIfG_Ccsp()->Set(&field_0x5ec[i]);
    }
}

int daObjFan_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mModel);
    dComIfGd_setList();
    return 1;
}

int daObjFan_c::Delete() {
    if (field_0x5ac != NULL && field_0x5ac->ChkUsed()) {
        dComIfG_Bgsp().Release(field_0x5ac);
    }
    dComIfG_resDelete(&mPhase, l_arcName[field_0xad4]);
    return 1;
}

static void daObjFan_create1st(daObjFan_c* param_0) {
    fopAcM_ct(param_0, daObjFan_c);
    param_0->create1st();
}

static int daObjFan_MoveBGDelete(daObjFan_c* param_0) {
    return param_0->MoveBGDelete();
}

static int daObjFan_MoveBGExecute(daObjFan_c* param_0) {
    return param_0->MoveBGExecute();
}

static int daObjFan_MoveBGDraw(daObjFan_c* param_0) {
    return param_0->MoveBGDraw();
}

static actor_method_class daObjFan_METHODS = {
    (process_method_func)daObjFan_create1st,
    (process_method_func)daObjFan_MoveBGDelete,
    (process_method_func)daObjFan_MoveBGExecute,
    (process_method_func)NULL,
    (process_method_func)daObjFan_MoveBGDraw,
};

extern actor_process_profile_definition g_profile_Obj_Fan = {
    fpcLy_CURRENT_e,        // mLayerID
    7,                      // mListID
    fpcPi_CURRENT_e,        // mListPrio
    PROC_Obj_Fan,           // mProcName
    &g_fpcLf_Method.base,  // sub_method
    sizeof(daObjFan_c),     // mSize
    0,                      // mSizeOther
    0,                      // mParameters
    &g_fopAc_Method.base,   // sub_method
    0x235,                  // mPriority
    &daObjFan_METHODS,      // sub_method
    0x40100,                // mStatus
    fopAc_ACTOR_e,          // mActorType
    fopAc_CULLBOX_CUSTOM_e, // cullType
};
