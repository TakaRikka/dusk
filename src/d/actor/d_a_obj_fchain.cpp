/**
 * @file d_a_obj_fchain.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_fchain.h"
#include "d/actor/d_a_player.h"
#include "JSystem/J3DGraphBase/J3DDrawBuffer.h"
#include "SSystem/SComponent/c_math.h"
#include "d/d_com_inf_game.h"

static char const l_arcName[] = "Fchain";

int daObjFchain_c::createHeap() {
    mModelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, 3);
    return mModelData != NULL ? 1 : 0;
}

static int daObjFchain_createHeap(fopAc_ac_c* i_this) {
    return static_cast<daObjFchain_c*>(i_this)->createHeap();
}

static Vec const localVec = {
    0.0f, 0.0f, -9.0f,
};

int daObjFchain_c::create() {
    fopAcM_ct(this, daObjFchain_c);
    int rv = dComIfG_resLoad(&mPhase, l_arcName);
    switch (rv) {
    case cPhs_COMPLEATE_e:
        if (fopAcM_entrySolidHeap(this, daObjFchain_createHeap,
                                                      0x10) == 0) {
            return cPhs_ERROR_e;
        }
        tevStr.room_no = dComIfGp_roomControl_getStayNo();
        mShape.setUserArea((uintptr_t)this);
        current.pos.y += 3.0f;
        s16 local_58 = shape_angle.y;
        cXyz* local_44 = &current.pos;
        cXyz* local_48 = field_0x694;
        csXyz* pdVar5 = field_0x8a4;
        for (int i = 0; i < 22; i++) {
            if ((i & 1) != 0) {
                pdVar5->z = cM_rndFX(2048.0f) + 4096.0f;
            } else {
                pdVar5->z = cM_rndFX(2048.0f) + -4096.0f;
            }
            f32 fVar1;
            if (cM_rnd() < 0.5f) {
                fVar1 = 1.0f;
            } else {
                fVar1 = -1.0f;
            }
            pdVar5->y = local_58 + (fVar1 * (cM_rndF(3072.0f) + 1024.0f));
            mDoMtx_stack_c::transS(*local_44);
            mDoMtx_stack_c::YrotM(pdVar5->y);
            mDoMtx_stack_c::multVec(&localVec, local_48);
            local_58 = pdVar5->y;
            local_44 = local_48;
            pdVar5++;
            local_48++;
        }
        rv = cPhs_COMPLEATE_e;
        break;
    }
    return rv;
}

static int daObjFchain_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjFchain_c*>(i_this)->create();
}

daObjFchain_c::~daObjFchain_c() {
    dComIfG_resDelete(&mPhase,l_arcName);
}

static int daObjFchain_Delete(daObjFchain_c* i_this) {
    i_this->~daObjFchain_c();
    return 1;
}

void daObjFchain_c::checkPlayerFoot(cXyz const* param_1, cXyz const* param_2, cXyz* param_3) {
    cXyz local_24 = *param_1 - *param_2;
    f32 dVar4 = local_24.absXZ();
    if (fabsf(local_24.y) < 5.0f && dVar4 < 20.0f) {
        local_24.y = 0.0f;
        local_24.normalizeZP();
        f32 fVar1 = 20.0f - dVar4;
        param_3->x += local_24.x * fVar1;
        param_3->z += local_24.z * fVar1;
    }
}

void daObjFchain_c::setGroundVec(cXyz* param_1, f32 param_2) {
    f32 dVar6 = param_1->absXZ();
    param_1->y += param_2;
    if (dVar6 < 0.1f) {
        param_1->z += param_2;
    } else {
        f32 fVar1 = (1.0f / dVar6);
        param_1->x += fVar1 * (param_2 * param_1->x);
        param_1->z += fVar1 * (param_2 * param_1->z);
    }
    param_1->normalizeZP();
}

static Vec const wolfChainBaseOffset = {
    22.0f, 0.0f, -12.0f,
};

int daObjFchain_c::execute() {
    if (dComIfGp_event_runCheck()) {
        if (dComIfGp_getEventManager().getRunEventName() != NULL) {
            if (strcmp("demo06_01", dComIfGp_getEventManager().getRunEventName()) == 0 &&
                field_0x588 >= 0)
            {
                field_0x588++;
                if (field_0x588 > 310) {
                    field_0x588 = -1;
                }
            }
        }
    } else {
        field_0x588 = 0;
    }
    cXyz* sp_38 = &current.pos;
    cXyz sp_12c;
    cXyz sp_120;
    if (field_0x586 != 0) {
        field_0x586--;
    }
    int i;
    cXyz* var_r28 = field_0x694;
    cXyz* sp_30 = field_0x79c;
    u32 isWolf = daPy_py_c::checkNowWolf();
    daPy_py_c* player = daPy_getLinkPlayerActorClass();
    for (i = 0; i < 22; i++, var_r28++, sp_30++) {
        sp_12c = (*var_r28 - *sp_38) + *sp_30;
        sp_12c.y += -1.5f;
            /* dSv_event_flag_c::M_011 - Inside Hyrule Castle - Midna removes wolf's chains in prison */
        if (dComIfGs_isEventBit(0x510)) {
            if (isWolf) {
                checkPlayerFoot(var_r28, player->getLeftHandPosP(), &sp_12c);
                checkPlayerFoot(var_r28, player->getRightHandPosP(), &sp_12c);
            }
            checkPlayerFoot(var_r28, player->getLeftFootPosP(), &sp_12c);
            checkPlayerFoot(var_r28, player->getRightFootPosP(), &sp_12c);
        }
        sp_120 = sp_12c;
        sp_12c.normalizeZP();
        *var_r28 = *sp_38 + (sp_12c * 9.0f);
        if (var_r28->y < current.pos.y) {
            setGroundVec(&sp_120, current.pos.y - var_r28->y);
            *var_r28 = *sp_38 + (sp_120 * 9.0f);
        }
        sp_38 = var_r28;
    }
                   /* dSv_event_flag_c::M_011 - Inside Hyrule Castle - Midna removes wolf's chains in prison */
    if (isWolf && !dComIfGs_isEventBit(0x510)) {
        mDoMtx_multVec(
            player->getModelJointMtx(17), &wolfChainBaseOffset,
            &field_0x694[21]);
        sp_12c = (field_0x694[21] - current.pos);
        f32 dVar13 = sp_12c.abs();
        if (dVar13 > 198.0f) {
            player->setOutPower(dVar13 - 198.0f, cM_atan2s(-sp_12c.x, -sp_12c.z), 0);
            player->onWolfFchainPull();
            var_r28 = field_0x694;
            sp_38 = &current.pos;
            sp_12c *= 9.0f / dVar13;
            for (i = 0; i < 22; i++, var_r28++) {
                *var_r28 = *sp_38 + sp_12c;
                sp_38 = var_r28;
            }
            field_0x584 = 0;
        } else {
            var_r28 = &field_0x694[20];
            sp_30 = &field_0x79c[20];
            sp_38 = var_r28 + 1;
            for (i = 20; i >= 0; i--, var_r28--, sp_30--) {
                sp_12c = (*var_r28 - *sp_38) + *sp_30;
                sp_12c.y += -1.5f;
                sp_120 = sp_12c;
                sp_12c.normalizeZP();
                *var_r28 = *sp_38 + (sp_12c * 9.0f);
                if (var_r28->y < current.pos.y) {
                    setGroundVec(&sp_120, current.pos.y - var_r28->y);
                    *var_r28 = *sp_38 + (sp_120 * 9.0f);
                }
                sp_38 = var_r28;
            }
        }
    }
    sp_38 = &current.pos;
    var_r28 = field_0x694;
    sp_30 = field_0x79c;
    csXyz* sp_24 = field_0x8a4;
    cXyz* sp_20 = field_0x58c;
    s16 sp_08 = 0;
    for (i = 0; i < 22; i++, var_r28++, sp_30++, sp_24++, sp_20++) {
        *sp_30 = (*var_r28 - *sp_20) * 0.3f;
        *sp_20 = *var_r28;
        sp_12c = *sp_38 - *var_r28;
        sp_24->x = sp_12c.atan2sY_XZ();
        if (!(sp_12c.absXZ() < 3.5f)) {
            sp_24->y = sp_12c.atan2sX_Z();
        }
        if (sp_30->abs2() > 3.0f && field_0x586 == 0) {
            f32 fVar1;
            if (cM_rnd() < 0.5f) {
                fVar1 = 1.0f;
            } else {
                fVar1 = -1.0f;
            }
            sp_24->z = sp_08 + 0x4000 +
                          (fVar1 * (cM_rndF(4096.0f) + 1536.0f));
            if (var_r28->y <= current.pos.y + 2.0f) {
                if ((sp_24->z >= 0 && sp_24->z < 0x4000) ||
                    (sp_24->z > -0x7fff && sp_24->z < -0x4000))
                {
                    sp_24->z = cM_rndFX(2048.0f) + 4096.0f;
                } else {
                    sp_24->z = cM_rndFX(2048.0f) + -4096.0f;
                }
            }
        }
        sp_08 = sp_24->z;
        sp_38 = var_r28;
    }
    if (field_0x584 == 0) {
        field_0x584 = 1;
        sp_30 = field_0x79c;
        for (i = 0; i < 22; i++, sp_30++) {
            *sp_30 = cXyz::Zero;
        }
        field_0x586 = 5;
    }
    return 1;
}

static int daObjFchain_Execute(daObjFchain_c* i_this) {
    return static_cast<daObjFchain_c*>(i_this)->execute();
}

void daObjFchain_shape_c::draw() {
    daObjFchain_c* i_this = (daObjFchain_c*)getUserArea();
    cXyz* pPos = i_this->getPos();
    csXyz* pAngle = i_this->getAngle();
    J3DModelData* modelData = i_this->getModelData();
    J3DMaterial* material = modelData->getMaterialNodePointer(0);
    dKy_tevstr_c* tevStr = &i_this->tevStr;
    j3dSys.setVtxPos(modelData->getVtxPosArray());
    j3dSys.setVtxNrm(modelData->getVtxNrmArray());
    j3dSys.setVtxCol(modelData->getVtxColorArray(0));
    J3DShape::resetVcdVatCache();
    material->loadSharedDL();
    material->getShape()->loadPreDrawSetting();
    GXColor ambColor;
    ambColor.r = tevStr->AmbCol.r;
    ambColor.g = tevStr->AmbCol.g;
    ambColor.b = tevStr->AmbCol.b;
    ambColor.a = tevStr->AmbCol.a;
    GXSetChanAmbColor(GX_COLOR0A0, ambColor);
    GXSetChanMatColor(GX_COLOR0A0, g_whiteColor);
    dKy_setLight_again();
    dKy_GxFog_tevstr_set(tevStr);
    GXLoadLightObjImm(&tevStr->mLightObj.mLightObj, GX_LIGHT0);
    int local_30;
        /* dSv_event_flag_c::M_011 - Inside Hyrule Castle - Midna removes wolf's chains in prison */
    if (dComIfGs_isEventBit(0x510)) {
        local_30 = 17;
    } else {
        local_30 = 22;
    }
    for (int i = 0; i < local_30; i++, pPos++, pAngle++) {
        mDoMtx_stack_c::copy(j3dSys.getViewMtx());
        mDoMtx_stack_c::transM(*pPos);
        mDoMtx_stack_c::ZXYrotM(*pAngle);
        GXLoadPosMtxImm(mDoMtx_stack_c::get(), 0);
        GXLoadNrmMtxImm(mDoMtx_stack_c::get(), 0);
        material->getShape()->simpleDrawCache();
    }
}

int daObjFchain_c::draw() {
    if (field_0x584 != 0) {
        g_env_light.settingTevStruct(0, &current.pos, &tevStr);
        g_env_light.setLightTevColorType_MAJI(mModelData, &tevStr);
        if (field_0x588 > 0) {
            return 1;
        }
        dComIfGd_getOpaListDark()->entryImm(&mShape, 0);
    }
    return 1;
}

static int daObjFchain_Draw(daObjFchain_c* i_this) {
    return static_cast<daObjFchain_c*>(i_this)->draw();
}

static actor_method_class l_daObjFchain_Method = {
    (process_method_func)daObjFchain_Create,
    (process_method_func)daObjFchain_Delete,
    (process_method_func)daObjFchain_Execute,
    NULL,
    (process_method_func)daObjFchain_Draw,
};

actor_process_profile_definition g_profile_Obj_Fchain = {
  fpcLy_CURRENT_e,        // mLayerID
  7,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_Fchain,        // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjFchain_c),  // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  246,                    // mPriority
  &l_daObjFchain_Method,  // sub_method
  0x00060000,             // mStatus
  fopAc_ENV_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
