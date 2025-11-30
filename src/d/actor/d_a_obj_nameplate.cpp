/**
* @file d_a_obj_nameplate.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_nameplate.h"
#include "d/actor/d_a_alink.h"
#include "d/d_a_obj.h"
#include "dolphin/mtx.h"
#include "f_op/f_op_actor.h"
#include "include/SSystem/SComponent/c_math.h"

// clang-format off
daObjNameplate_c::M_attrs const daObjNameplate_c::M_attr = {
    50.0f,
    2500.0f,
    400.0f,
    6.0f,
    -0.02f,
    0.95f,
    170,
    2000,
    10,
    0x00,
};
// clang-format on

void daObjNameplate_c::create_init() {
    fopAcM_setCullSizeBox(this, -75.0f, -180.0f, -25.0f, 75.0f, 20.0f, 25.0f);

    cXyz local_18(cM_ssin(shape_angle.y) * 20.0f, -105.0, cM_scos(shape_angle.y) * 20.0f);
    VECAdd(&attention_info.position, &local_18, &attention_info.position);

    eyePos.y = eyePos.y - 120.0f;

    mMessageFlowId = fopAcM_GetParam(this);

    initBaseMtx();
    initCcSphere();
}

void daObjNameplate_c::initBaseMtx() {
    setBaseMtx();
}

void daObjNameplate_c::setBaseMtx() {
    mDoMtx_stack_c::transS(home.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y);

    switch (attr().field_0x1E) {
    case 0:
        mDoMtx_stack_c::ZrotM(field_0x724);
        mDoMtx_stack_c::YrotM(field_0x722);
        break;
    case 1:
        mDoMtx_stack_c::ZrotM(field_0x724);
        mDoMtx_stack_c::YrotM(field_0x722);
        break;
    default:
        break;
    }

    model->setBaseTRMtx(mDoMtx_stack_c::get());
    PSMTXCopy(mDoMtx_stack_c::now, mMtx);
}

void daObjNameplate_c::initCcSphere() {
    /* 80594EB4-80594EF4 000040 0040+00 1/1 0/0 0/0 .rodata          ccSphSrc$3700 */
    const static dCcD_SrcSph ccSphSrc = {
        {
            {0x0, {{0x0, 0x0, 0x0}, {0x1607a, 0x11}, 0x99}},  // mObj
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},               // mGObjAt
            {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x1},               // mGObjTg
            {0x0},                                            // mGObjCo
        },                                                    // mObjInf
        {
            {{0.0f, -115.0f, 0.0f}, 50.0f}  // mSph
        }                                   // mSphAttr
    };

    mCcDStts.Init(0xff, 0xff, this);
    mCcDSph.Set(ccSphSrc);
    mCcDSph.SetStts(&mCcDStts);
    mCcDSph.SetC(home.pos);
    mCcDSph.SetR(attr().radius1);
}

void daObjNameplate_c::setCcSphere() {
    cXyz local_lc1;
    cXyz local_lc(0.0f, -115.0f, 0.0f);  // fails unless this unused cXyz is present

    local_lc1.x = 0.0f;
    local_lc1.y = -115.0f;
    local_lc1.z = 0.0f;

    MTXCopy(mMtx, mDoMtx_stack_c::get());
    mDoMtx_stack_c::multVec(&local_lc1, &local_lc1);
    mCcDSph.SetC(local_lc1);
    dComIfG_Ccsp()->Set(&mCcDSph);
}

void daObjNameplate_c::calcAngle() {
    if (field_0x77E > 0) {
        field_0x77E += -1;
    }

    if (field_0x77E <= 0 && (mCcDSph.ChkTgHit() != 0)) {
        int hitAngle = checkHitAngle();

        // can't be switch
        if (hitAngle == 0 || hitAngle == 2) {
            field_0x77A = (s16)attr().field_0x04;
        } else if (hitAngle == 1 || hitAngle == 3) {
            field_0x77A = (s16)-attr().field_0x04;
        }

        field_0x71C = 0.0f;
        field_0x718 = attr().field_0x08;
        field_0x77E = attr().field_0x1C;

        daObj::HitSeStart(&this->current.pos, fopAcM_GetRoomNo(this), &this->mCcDSph, 14);
    } else if (mCcDSph.ChkCoHit() != 0) {
        int hitAngle = checkHitAngle();

        if (hitAngle == 0 || hitAngle == 2) {
            field_0x77A = (s16)attr().field_0x18;
        } else if (hitAngle == 1 || hitAngle == 3) {
            field_0x77A = -(s16)attr().field_0x18;
        }

        field_0x71C = 0.0f;
        field_0x77E = attr().field_0x1C;
    } else if (daPy_getPlayerActorClass()->checkFrontRollCrash() &&
               fopAcM_searchPlayerDistanceXZ(this) < 200.0f)
    {
        int hitAngle = checkHitAngle();
        if (hitAngle == 0 || hitAngle == 2) {
            field_0x77A = (s16)attr().field_0x04;
        } else if (hitAngle == 1 || hitAngle == 3) {
            field_0x77A = (s16)-attr().field_0x04;
        }

        field_0x77E = attr().field_0x1C;
    }

    field_0x720 = 0;
    calcSpring();
    field_0x724 = (field_0x718 * cM_scos(field_0x77C));

    if (cLib_chaseF(&field_0x718, 0.0f, attr().field_0x0C) != 0) {
        field_0x77C = 0;
    }

    field_0x77C += attr().field_0x1A;
}

void daObjNameplate_c::calcSpring() {
    field_0x778 += field_0x77A;
    float fVar1 = static_cast<float>(field_0x778) * attr().field_0x10;

    if ((field_0x77A > 0 && fVar1 < 0.0f) || (field_0x77A < 0 && fVar1 > 0.0f)) {
        fVar1 = 0.0f;
    }

    field_0x71C = (field_0x71C + fVar1) * attr().field_0x14;
    field_0x778 = (float)field_0x778 + field_0x71C;
    f32 iVar2 = (float)abs((int)field_0x778);

    if (iVar2 > attr().field_0x04) {
        if (field_0x778 > 0) {
            field_0x778 = (s16)attr().field_0x04;
        } else if (field_0x778 < 0) {
            field_0x778 = (s16)-attr().field_0x04;
        }
    }

    field_0x77A = 0;
    field_0x722 = field_0x778;
}

int daObjNameplate_c::checkHitAngle() {
    s16 sourceAngle = fopAcM_searchPlayerAngleY(this) - shape_angle.y;

    if (sourceAngle >= 0 && sourceAngle < 0x4000) {
        return 0;
    } else if (sourceAngle >= 0x4000 && sourceAngle < 0x7fff) {
        return 1;
    } else if (sourceAngle < -0x4000 && sourceAngle >= -0x8000) {
        return 2;
    } else if (sourceAngle < 0 && sourceAngle >= -0x4000) {
        return 3;
    }

    return -1;
}

void daObjNameplate_c::messageProc() {
    switch (mShowMessage) {
    case 0:
        if (mMessageFlowId != -1 &&
            cLib_distanceAngleS(shape_angle.GetY(), fopAcM_searchPlayerAngleY(this)) < 0x3000)
        {
            fopAcM_OnStatus(this, 0);
            cLib_onBit<u32>(attention_info.flags, fopAc_AttnFlag_TALKREAD_e | fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
            attention_info.distances[fopAc_attn_TALK_e] = 0x15;
            attention_info.distances[fopAc_attn_SPEAK_e] = 0x15;
            eventInfo.onCondition(1);
        } else {
            fopAcM_OffStatus(this, 0);
            cLib_offBit<u32>(attention_info.flags, fopAc_AttnFlag_TALKREAD_e | fopAc_AttnFlag_SPEAK_e | fopAc_AttnFlag_TALK_e);
        }

        if (dComIfGp_event_runCheck() && eventInfo.checkCommandTalk()) {
            mMsgFlow.init(this, mMessageFlowId, 0, 0);
            mShowMessage = 1;
        }
        break;

    case 1:
        dComIfG_play_c& play = g_dComIfG_gameInfo.play;
        if (play.getEvent().runCheck() && eventInfo.checkCommandTalk() != 0 &&
            mMsgFlow.doFlow(this, NULL, 0) != 0)
        {
            play.getEvent().reset();
            mShowMessage = 0;
        }
        break;
    }
}

char* daObjNameplate_c::l_arcName = "J_Hyosatu";

int daObjNameplate_c::createHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "J_Hyousatu.bmd");
    model = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);

    // return model == NULL;
    if (model == NULL) {
        return 0;
    }

    return 1;
}

static int createSolidHeap(fopAc_ac_c* i_this) {
    return static_cast<daObjNameplate_c*>(i_this)->createHeap();
}

int daObjNameplate_c::draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(model, &tevStr);

    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(model);
    dComIfGd_setList();

    return 1;
}

static int daObjNameplate_Draw(daObjNameplate_c* i_this) {
    return i_this->draw();
}

/* 80594A6C-80594AB4 000A4C 0048+00 1/0 0/0 0/0 .text
 * daObjNameplate_Execute__FP16daObjNameplate_c
 */
static int daObjNameplate_Execute(daObjNameplate_c* i_this) {
    i_this->calcAngle();
    i_this->setBaseMtx();
    i_this->setCcSphere();
    i_this->messageProc();

    return 1;
}

static int daObjNameplate_IsDelete(daObjNameplate_c* i_this) {
    return 1;
}

/* 80594ABC-80594BF8 000A9C 013C+00 1/0 0/0 0/0 .text
 * daObjNameplate_Delete__FP16daObjNameplate_c
 */
static int daObjNameplate_Delete(daObjNameplate_c* i_this) {
    i_this->~daObjNameplate_c();
    return 1;
}

int daObjNameplate_c::create() {
    fopAcM_ct(this, daObjNameplate_c);

    int phase_state = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase_state == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createSolidHeap, 0x800)) {
            return cPhs_ERROR_e;
        }
        create_init();
        fopAcM_SetMtx(this, mMtx);
    }

    return phase_state;
}

static int daObjNameplate_Create(fopAc_ac_c* i_this) {
    return static_cast<daObjNameplate_c*>(i_this)->create();
}

/* ##############################################################################################
 */
static actor_method_class l_daObjNameplate_Method = {
    (process_method_func)daObjNameplate_Create,  (process_method_func)daObjNameplate_Delete,
    (process_method_func)daObjNameplate_Execute, (process_method_func)daObjNameplate_IsDelete,
    (process_method_func)daObjNameplate_Draw,
};

extern actor_process_profile_definition g_profile_Obj_NamePlate = {
    fpcLy_CURRENT_e,           // mLayerID
    7,                         // mListID
    fpcPi_CURRENT_e,           // mListPrio
    PROC_Obj_NamePlate,        // mProcName
    &g_fpcLf_Method.base,      // sub_method
    sizeof(daObjNameplate_c),  // mSize
    0,                         // mSizeOther
    0,                         // mParameters
    &g_fopAc_Method.base,      // sub_method
    32,                        // mPriority
    &l_daObjNameplate_Method,  // sub_method
    0x00040180,                // mStatus
    fopAc_ACTOR_e,             // mActorType
    fopAc_CULLBOX_CUSTOM_e,    // cullType
};
