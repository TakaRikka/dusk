/**
 * d_a_obj_grave_stone.cpp
 * Object - Zora Gravestone
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_grave_stone.h"
#include "d/d_procname.h"
#include "d/d_com_inf_game.h"
#include "d/d_bg_w.h"

static char const* l_arcName = "H_Haka";

dCcD_SrcGObjInf const daGraveStone_c::mCcDObjInfo = {
    {0, {{0, 0, 0}, {0, 0}, {0x79}}},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0},
};

dCcD_SrcCyl const daGraveStone_c::mCcDCyl = {
    {
        {0, {{0, 0, 0}, {0, 0}, {0x79}}},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0},
    },
    {{0.0f, 0.0f, 0.0f}, 0.0f, 0.0f},
};

void daGraveStone_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::YrotM(shape_angle.y);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    PSMTXCopy(mDoMtx_stack_c::get(), mBgMtx);
}

void daGraveStone_c::moveCalc() {
    if (cLib_calcTimer(&mTimer) != 0) {
        fopAcM_posMoveF(this, mColStatus.GetCCMoveP());
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        if (-mAcch.GetGroundH()) {
            setEnvTevColor();
            setRoomNo();
        }
        attention_info.position = current.pos;
        PSMTXCopy(mpModel->getBaseTRMtx(), mDoMtx_stack_c::get());
        
        cXyz vec(0.0f, 0.0f, 0.0f);
        if (speedF > 1.0f) {
            vec.z = -30.0f;
        } else {
            vec.z = 30.0f;
        }

        mDoMtx_stack_c::push();
        vec.x = 90.0f;
        mDoMtx_stack_c::multVec(&vec, &mPrtclMngr[0].mPos);
        mPrtclMngr[0].mAngle = current.angle;
        mPrtclMngr[0].field_0x00 = true;
        mDoMtx_stack_c::pop();

        vec.x = -90.0f;
        mDoMtx_stack_c::push();
        mDoMtx_stack_c::multVec(&vec, &mPrtclMngr[1].mPos);
        mPrtclMngr[1].mAngle = current.angle;
        mPrtclMngr[1].field_0x00 = true;
        mDoMtx_stack_c::pop();

        vec.x = 30.0f;
        mDoMtx_stack_c::push();
        mDoMtx_stack_c::multVec(&vec, &mPrtclMngr[2].mPos);
        mPrtclMngr[2].mAngle = current.angle;
        mPrtclMngr[2].field_0x00 = true;
        mDoMtx_stack_c::pop();

        vec.x = -30.0f;
        mDoMtx_stack_c::push();
        mDoMtx_stack_c::multVec(&vec, &mPrtclMngr[3].mPos);
        mPrtclMngr[3].mAngle = current.angle;
        mPrtclMngr[3].field_0x00 = true;
        mDoMtx_stack_c::pop();

        Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_ZORA_BOSEKI_MV, &current.pos,
                                      0, 0, 1.0f, 1.0f, -1.0f, -1.0f, 0);
    }
}

int daGraveStone_c::setPrtcl() {
    for (int i = 0; i < 4; i++) {
        mPrtclMngr[i].field_0x24.setEffectCenter(
            &tevStr, &mPrtclMngr[i].mPos, 0, 0,
            mPrtclMngr[i].field_0x00 == true ? &mPrtclMngr[i].mPos : NULL,
            mPrtclMngr[i].field_0x00 == true ? &mPrtclMngr[i].mAngle : NULL,
            NULL, fopAcM_GetRoomNo(this), 0.0f, speedF
        );
        if (mPrtclMngr[i].field_0x00) {
            JPABaseEmitter* emitter = mPrtclMngr[i].field_0x24.getCenterEmitter(0, 0);
            if (emitter != NULL) {
                mPrtclMngr[i].scale.set(2.5f, 2.5f, 2.5f);
                // JGeometry::TVec3<f32> scale(mPrtclMngr[i].scale);
                JGeometry::TVec3<f32> scale;
                JGeometry::setTVec3f(&mPrtclMngr[i].scale.x, &scale.x);
                emitter->setGlobalParticleScale(scale);
            }
        }
        mPrtclMngr[i].field_0x00 = 0;
    }
    return 1;
}

void daGraveStone_c::setEnvTevColor() {
    tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mGndChk);
    tevStr.room_no = dComIfG_Bgsp().GetRoomId(mGndChk);
}

void daGraveStone_c::setRoomNo() {
    s8 room_no = dComIfG_Bgsp().GetRoomId(mGndChk);
    fopAcM_SetRoomNo(this, room_no);
    mColStatus.SetRoomId(room_no);
}

int daGraveStone_c::Create() {
    setBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    mAcchCir.SetWall(0.0f, 0.0f);
    mAcch.Set(&current.pos, &old.pos, this, 1, &mAcchCir, &speed, &current.angle, &shape_angle);
    mColStatus.Init(0xff, 0, this);
    mColCyl.Set(mCcDCyl);
    mColCyl.SetStts(&mColStatus);
    mAcch.CrrPos(dComIfG_Bgsp());
    mGndChk = mAcch.m_gnd;
    setEnvTevColor();
    setRoomNo();
    attention_info.position = current.pos;
    for (int i = 0; i < 4; i++) {
        mPrtclMngr[i].field_0x24.init(&mAcch, 0.0f, 0.0f);
    }
    J3DJoint* joint = mpModel->getModelData()->getJointNodePointer(0);
    fopAcM_setCullSizeBox(this, joint->getMin()->x, joint->getMin()->y, joint->getMin()->z,
                          joint->getMax()->x, joint->getMax()->y, joint->getMax()->z);
    mTimer = 0;
    return 1;
}

int daGraveStone_c::CreateHeap() {
    J3DModelData* model_data = (J3DModelData*)dComIfG_getObjectRes(l_arcName, "H_Haka.bmd");
    mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }
    return 1;
}

cPhs__Step daGraveStone_c::create() {
    fopAcM_ct(this, daGraveStone_c);
    cPhs__Step step = (cPhs__Step)dComIfG_resLoad(&mPhase, l_arcName);
    if (step == cPhs_COMPLEATE_e) {
        int dzb_id = dComIfG_getObjctResName2Index(l_arcName, "H_Haka.dzb");
        step = (cPhs__Step)MoveBGCreate(l_arcName, dzb_id,
                                        dBgS_MoveBGProc_TypicalRotY, 0x29A0, NULL);
        if (step == cPhs_ERROR_e) {
            return step;
        }
    }
    return step;
}

int daGraveStone_c::Execute(Mtx** i_mtxP) {
    *i_mtxP = &mBgMtx;
    moveCalc();
    setBaseMtx();
    setPrtcl();
    return 1;
}

int daGraveStone_c::Draw() {
    g_env_light.settingTevStruct(0x10, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);
    dComIfGd_setListBG();
    mDoExt_modelUpdateDL(mpModel);
    dComIfGd_setList();
    return 1;
}

int daGraveStone_c::Delete() {
    dComIfG_resDelete(&mPhase, l_arcName);
    if (mpBgW != NULL && mpBgW->ChkUsed()) {
        dComIfG_Bgsp().Release(mpBgW);
    }
    return 1;
}

static int daGraveStone_Draw(daGraveStone_c* i_this) {
    return i_this->MoveBGDraw();
}

static int daGraveStone_Execute(daGraveStone_c* i_this) {
    return i_this->MoveBGExecute();
}

static int daGraveStone_IsDelete(daGraveStone_c* i_this) {
    return 1;
}

static int daGraveStone_Delete(daGraveStone_c* i_this) {
    return i_this->MoveBGDelete();
}

static cPhs__Step daGraveStone_create(fopAc_ac_c* i_this) {
    return static_cast<daGraveStone_c*>(i_this)->create();
}

static actor_method_class l_daGraveStone_Method = {
    (process_method_func)daGraveStone_create,
    (process_method_func)daGraveStone_Delete,
    (process_method_func)daGraveStone_Execute,
    (process_method_func)daGraveStone_IsDelete,
    (process_method_func)daGraveStone_Draw,
};

extern actor_process_profile_definition g_profile_Obj_GraveStone = {
    fpcLy_CURRENT_e,
    3,
    fpcPi_CURRENT_e,
    PROC_Obj_GraveStone,
    &g_fpcLf_Method.base,
    sizeof(daGraveStone_c),
    0,
    0,
    &g_fopAc_Method.base,
    0x25F,
    &l_daGraveStone_Method,
    0x44100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
