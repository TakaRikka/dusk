/**
 * @file d_a_obj_gra_rock.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_gra_rock.h"
#include "d/actor/d_a_npc.h"
#include "d/actor/d_a_npc4.h"
#include "d/d_bg_w.h"

static void dummy() {
    // fakematch to fix weak function order
    cLib_calcTimer<u8>(0);
    cLib_calcTimer<s16>(0);
}

dCcD_SrcCyl const daObjGraRock_c::mCcDCyl = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x2020, 0x11}, 0x0}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjTg
        {0x0}, // mGObjCo
    }, // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f}, // mCenter
            0.0f, // mRadius
            0.0f // mHeight
        } // mCyl
    },
};

void daObjGraRock_c::setAttnPos() {
    cXyz cStack_50(0.0f, 680.0f, 145.0f);
    mDoMtx_stack_c::ZXYrotS(current.angle.x, current.angle.y, current.angle.z);
    mDoMtx_stack_c::multVec(&cStack_50, &attention_info.position);
    attention_info.position += current.pos;
    attention_info.distances[0] = daNpcF_getDistTableIdx(8, 5);
    attention_info.flags = fopAc_AttnFlag_LOCK_e;
    dAttention_c* attention = dComIfGp_getAttention();
    if (attention->LockonTruth()) {
        if (attention->LockonTarget(0) == this && field_0x9bc == 0 && field_0x9b6 == 0) {
            field_0x9b6 = 90;
            field_0x9bd = 3;
            s16 sVar4 = cLib_getRndValue(0, 0x10000);
            s16 sVar5 = cLib_getRndValue(0x400, 0x200);
            field_0x9b2 = sVar5 * cM_scos(sVar4);
            field_0x9b4 = sVar5 * cM_ssin(sVar4);
            field_0x9b8 = cLib_getRndValue(2, 2);
            field_0x9ba = cLib_getRndValue(2, 2);
            Z2GetAudioMgr()->seStart(Z2SE_OBJ_ZR_GRN_ROCK_GRGR, &current.pos, 0, 0, 1.0f, 1.0f,
                                     -1.0f, -1.0f, 0);
            Z2GetAudioMgr()->seStart(Z2SE_GRA_V_YURAYURA, &attention_info.position, 0, 0, 1.0f,
                                     1.0f, -1.0f, -1.0f, 0);
        }
        field_0x9bc = 1;
    } else {
        field_0x9bc = 0;
    }
    if (cLib_calcTimer(&field_0x9b6) != 0) {
        current.angle.x = ((f32)field_0x9b2 * (f32)field_0x9b6 / 90.0f) * cM_ssin((field_0x9b8 * (field_0x9b6 * 0x10000)) / 90);
        current.angle.z = ((f32)field_0x9b4 * (f32)field_0x9b6 / 90.0f) * cM_ssin((field_0x9ba * (field_0x9b6 * 0x10000)) / 90);
        setPrtcl();
    }
    shape_angle = current.angle;
}

void daObjGraRock_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
    mModel->setBaseTRMtx(mDoMtx_stack_c::get());
    MTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

static u16 const l_prticles_id[2] = {
    0x875F,
    0x8760,
};

void daObjGraRock_c::setPrtcl() {
    for (int i = 0; i < 2; i++) {
        mParticleIds[i] = dComIfGp_particle_set(mParticleIds[i], l_prticles_id[i], &current.pos,
                                                &current.angle, &scale);
    }
}

void daObjGraRock_c::bombParticleSet() {
    dComIfGp_particle_set(0x875b, &current.pos, &tevStr, &current.angle, 0);
    dComIfGp_particle_set(0x8767, &current.pos, &tevStr, &current.angle, 0);
    dComIfGp_particle_set(0x875c, &current.pos, &tevStr, &current.angle, 0);
    dComIfGp_particle_set(0x875d, &current.pos, &tevStr, &current.angle, 0);
    dComIfGp_particle_set(0x875e, &current.pos, &tevStr, &current.angle, 0);
}

void daObjGraRock_c::setEnvTevColor() {
    tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mGndChk);
    tevStr.room_no = dComIfG_Bgsp().GetRoomId(mGndChk);
}

void daObjGraRock_c::setRoomNo() {
    s32 roomId = dComIfG_Bgsp().GetRoomId(mGndChk);
    fopAcM_SetRoomNo(this, roomId);
    mStts.SetRoomId(roomId);
}

void daObjGraRock_c::col_set() {
    if (mCyl.ChkTgHit()) {
        if (checkHitAt(mCyl.GetTgHitObj())) {
            cXyz acStack_2c(0.0f, 680.0f, 145.0f);
            mDoMtx_stack_c::ZXYrotS(current.angle.x, current.angle.y, current.angle.z);
            mDoMtx_stack_c::multVec(&acStack_2c, &acStack_2c);
            acStack_2c += current.pos;
            bombParticleSet();
            daNpcF_onEvtBit(0x150);
            dComIfGs_onSwitch(field_0x9c8, fopAcM_GetRoomNo(this));
            csXyz local_34(current.angle);
            local_34.x = 2;
            fopAcM_create(PROC_NPC_GRA, 0x5fffff00, &acStack_2c, fopAcM_GetRoomNo(this), &local_34, 0,
                                          -1);
            Z2GetAudioMgr()->seStart(Z2SE_OBJ_ZR_GRN_ROCK_BRK, &current.pos, 0, 0, 1.0f, 1.0f, -1.0f, -1.0f,
                             0);
            fopAcM_delete(this);
            return;
        }
        mCyl.ClrTgHit();
    }
    dComIfG_Ccsp()->Set(&mCyl);
}

int daObjGraRock_c::checkHitAt(cCcD_Obj* hitObj) {
    return hitObj->ChkAtType(AT_TYPE_BOMB) ? 1 : 0;
}

int daObjGraRock_c::Create() {
    setBaseMtx();
    fopAcM_SetMtx(this, mModel->getBaseTRMtx());
    fopAcM_setCullSizeBox(this, -328.444f, -150.0f,
                                          -384.168f, 322.307f,
                                          1055.554931640625f, 401.247f);
    mAcchCir.SetWall(0.0f, 0.0f);
    mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1,
                     &mAcchCir, fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
    mStts.Init(0xff, 0, this);
    mCyl.Set(mCcDCyl);
    mCyl.SetStts(&mStts);
    fopAcM_getCullSizeBoxMin(this);
    const Vec* cullMax = fopAcM_getCullSizeBoxMax(this);
    f32 fVar1;
    if (cullMax->x > cullMax->z) {
        fVar1 = cullMax->x;
    } else {
        fVar1 = cullMax->z;
    }
    mCyl.SetC(current.pos);
    mCyl.SetH(cullMax->y);
    mCyl.SetR(fVar1);
    mAcch.CrrPos(dComIfG_Bgsp());
    mGndChk = mAcch.m_gnd;
    setEnvTevColor();
    setRoomNo();
    for (int i = 0; i < 2; i++) {
        mParticleIds[i] = 0xffffffff;
    }
    field_0x9b0 = 0;
    field_0x9b6 = 0;
    field_0x9bc = 0;
    field_0x9bd = 0;
    field_0x9be = cLib_getRndValue(90, 90);
    return 1;
}

static char* l_arcName[5] = {
    "M_VBom",
    "grA_base",
    "grA_mdl",
    "grA_Rock",
    "grA_RockD",
};

int daObjGraRock_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName[0], "M_VBom_Zora.bmd");
    JUT_ASSERT(488, modelData != NULL);
    mModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11020084);
    if (mModel == NULL) {
        return 0;
    }
    J3DAnmTexPattern* btp_p =
        (J3DAnmTexPattern*)dComIfG_getObjectRes(l_arcName[0], "M_VBom_Zora.btp");
    JUT_ASSERT(501, btp_p != NULL);
    if (mBtp.init(modelData, btp_p, 1, 2, 1.0f, 0, -1) == 0) {
        return 0;
    }
    mBtp.setPlaySpeed(1.0f);
    return 1;
}

int daObjGraRock_c::create() {
    fopAcM_ct(this, daObjGraRock_c);
    field_0x9c8 = fopAcM_GetParam(this);
    int i;
    int completedNum;
    int rv;
    completedNum = 0;
    for (i = 0; i < 5; i++) {
        rv = dComIfG_resLoad(&mPhases[i], l_arcName[i]);
        if (rv == cPhs_ERROR_e || rv == cPhs_UNK3_e) {
            return cPhs_ERROR_e;
        }
        if (rv == cPhs_COMPLEATE_e) {
            completedNum++;
        }
    }
    if (i == completedNum) {
        int dzb_id = dComIfG_getObjctResName2Index(l_arcName[0], "M_VBom_Zora.dzb");
        JUT_ASSERT(550, dzb_id != -1);
        rv = MoveBGCreate(l_arcName[0], dzb_id,
            dBgS_MoveBGProc_TypicalRotY, 0x2d50, NULL);
        if (daNpcF_chkEvtBit(0x150)) {
            rv = cPhs_ERROR_e;
        }
        if (rv == cPhs_ERROR_e) {
            return rv;
        }
    }
    return rv;
}

int daObjGraRock_c::Execute(Mtx** pMtx) {
    mBtp.play();
    if (field_0x9bd != 0) {
        if (mBtp.isLoop()) {
            if (cLib_calcTimer(&field_0x9bd) == 0) {
                mBtp.setPlaySpeed(0.0f);
                field_0x9be = cLib_getRndValue(90, 90);
            }
        } else {
            mBtp.setPlaySpeed(1.0f);
        }
    } else {
        if (cLib_calcTimer(&field_0x9be) == 0) {
            mBtp.setPlaySpeed(1.0);
            if (mBtp.isLoop()) {
                field_0x9be = cLib_getRndValue(90, 90);
                mBtp.setPlaySpeed(0.0f);
            }
        } else {
            mBtp.setPlaySpeed(0.0f);
        }
    }
    col_set();
    *pMtx = &mBgMtx;
    setAttnPos();
    setBaseMtx();
    return 1;
}

int daObjGraRock_c::Draw() {
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mModel, &tevStr);
    dComIfGd_setListBG();
    mBtp.entry(mModel->getModelData());
    mDoExt_modelUpdateDL(mModel);
    dComIfGd_setList();
    return 1;
}

int daObjGraRock_c::Delete() {
    for (int i = 0; i < 5; i = i + 1) {
        dComIfG_resDelete(&mPhases[i], l_arcName[i]);
    }
    if (mpBgW != NULL && mpBgW->ChkUsed()) {
        if (dComIfG_Bgsp().Release(mpBgW) != 0) {
           OS_REPORT("Release Error\n");
        }
    }
    return 1;
}

static int daObjGraRock_Draw(daObjGraRock_c* i_this) {
    return i_this->Draw();
}

static int daObjGraRock_Execute(daObjGraRock_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daObjGraRock_IsDelete(daObjGraRock_c* i_this) {
    return 1;
}

static int daObjGraRock_Delete(daObjGraRock_c* i_this) {
    return i_this->MoveBGDelete();
}

static int daObjGraRock_create(fopAc_ac_c* i_this) {
    return static_cast<daObjGraRock_c*>(i_this)->create();
}

static actor_method_class l_daObjGraRock_Method = {
    (process_method_func)daObjGraRock_create,
    (process_method_func)daObjGraRock_Delete,
    (process_method_func)daObjGraRock_Execute,
    (process_method_func)daObjGraRock_IsDelete,
    (process_method_func)daObjGraRock_Draw,
};

actor_process_profile_definition g_profile_Obj_GraRock = {
  fpcLy_CURRENT_e,        // mLayerID
  3,                      // mListID
  fpcPi_CURRENT_e,        // mListPrio
  PROC_Obj_GraRock,       // mProcName
  &g_fpcLf_Method.base,  // sub_method
  sizeof(daObjGraRock_c), // mSize
  0,                      // mSizeOther
  0,                      // mParameters
  &g_fopAc_Method.base,   // sub_method
  609,                    // mPriority
  &l_daObjGraRock_Method, // sub_method
  0x00044100,             // mStatus
  fopAc_ACTOR_e,          // mActorType
  fopAc_CULLBOX_CUSTOM_e, // cullType
};
