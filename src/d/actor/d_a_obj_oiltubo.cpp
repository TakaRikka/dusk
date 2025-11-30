/**
 * d_a_obj_oiltubo.cpp
 * Lantern Oil Pot
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_oiltubo.h"

static char* l_resFileName = "Obj_otubo";

static char* l_bmdFileName = "x_oiltubo_00.bmd";

static char* l_bbmdFileName = "x_oiltubo_00b.bmd";

static char* l_bbtkFileName = "x_oiltubo_00b.btk";

dCcD_SrcCyl daObj_Oiltubo_c::mCcDCyl = {
    daObj_Oiltubo_c::mCcDObjInfo,
    {
        {0.0f, 0.0f, 0.0f},  // mCenter
        0.0f,                // mRadius
        0.0f                 // mHeight
    }                        // mCyl
};

int daObj_Oiltubo_c::create() {
    fopAcM_ct(this, daObj_Oiltubo_c);

    int phase_state = dComIfG_resLoad(&mPhase, getResName());
    if (phase_state == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x2000)) {
            return cPhs_ERROR_e;
        }

        initialize();
    }

    return phase_state;
}

const dCcD_SrcGObjInf daObj_Oiltubo_c::mCcDObjInfo = {
    {0x0, {{0x0, 0x0, 0x0}, {0xd8fbfdff, 0x1F}, 0x79}},  // mObj
    {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0},                  // mGObjAt
    {dCcD_SE_NONE, 0x2, 0x0, 0x0, 0x1},                  // mGObjTg
    {0x0},                                               // mGObjCo
};

int daObj_Oiltubo_c::CreateHeap() {
    J3DModelData* mdlData_p = (J3DModelData*)dComIfG_getObjectRes(getResName(), l_bmdFileName);
    JUT_ASSERT(0, mdlData_p != NULL);

    mpModel = mDoExt_J3DModel__create(mdlData_p, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    J3DModelData* mdlBData_p = (J3DModelData*)dComIfG_getObjectRes(getResName(), l_bbmdFileName);
    JUT_ASSERT(0, mdlBData_p != NULL);

    mpBModel = mDoExt_J3DModel__create(mdlBData_p, 0x80000, 0x19000284);
    if (mpBModel == NULL) {
        return 0;
    }

    mBtk.init(mdlBData_p, (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(getResName(), l_bbtkFileName),
              TRUE, J3DFrameCtrl::EMode_LOOP, 1.0f, 0, -1);
    mSound.init(&current.pos, 1);
    return 1;
}

int daObj_Oiltubo_c::Delete() {
    dComIfG_resDelete(&mPhase, getResName());
    mSound.deleteObject();
    return 1;
}

int daObj_Oiltubo_c::Execute() {
    BOOL event_chk = chkEvent();

    mBtk.play();
    setParam();

    mAcch.CrrPos(dComIfG_Bgsp());
    mGroundChk = mAcch.m_gnd;
    mGroundHeight = mAcch.GetGroundH();

    if (mGroundHeight != -G_CM3D_F_INF) {
        setEnvTevColor();
        setRoomNo();
    }

    if (event_chk && mProcess != NULL) {
        (this->*mProcess)(NULL);
    }

    setMtx();

    mCcCoCyl.SetCoSPrm(0);
    mCcCoCyl.SetTgType(0xD8FBFDFF);
    mCcCoCyl.SetTgSPrm(0x1F);
    mCcCoCyl.SetR(50.0f);
    mCcCoCyl.SetH(70.0f);
    mCcCoCyl.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mCcCoCyl);

    mCcAtCyl.SetAtType(0);
    mCcAtCyl.SetAtSPrm(0);
    mCcAtCyl.SetR(50.0f);
    mCcAtCyl.SetH(70.0f);
    mCcAtCyl.SetC(current.pos);
    dComIfG_Ccsp()->Set(&mCcAtCyl);

    setAttnPos();

    if (mCcAtCyl.ChkTgHit() && mCcAtCyl.GetTgHitObj() != NULL) {
        mSound.startCollisionSE(mCcAtCyl.GetTgHitObjHitSeID(1), 2, NULL);
    }

    mSound.framework(0, dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
    return 1;
}

int daObj_Oiltubo_c::Draw() {
    g_env_light.settingTevStruct(8, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpBModel, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    mDoExt_modelUpdateDL(mpModel);
    mBtk.entry(mpBModel->getModelData());
    mDoExt_modelUpdateDL(mpBModel);

    mShadowKey = dComIfGd_setShadow(mShadowKey, 1, mpModel, &current.pos, 800.0f, 120.0f,
                                    current.pos.y, mGroundHeight, mGroundChk, &tevStr, 0, 1.0f,
                                    dDlst_shadowControl_c::getSimpleTex());
    return 1;
}

int daObj_Oiltubo_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daObj_Oiltubo_c*>(i_this)->CreateHeap();
}

const char* daObj_Oiltubo_c::getResName() {
    return l_resFileName;
}

void daObj_Oiltubo_c::restart() {
    current.angle.set(0, home.angle.y, 0);
    shape_angle = current.angle;

    setProcess(&daObj_Oiltubo_c::wait);
}

void daObj_Oiltubo_c::initialize() {
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_SetMtx(this, mpBModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());
    fopAcM_setCullSizeBox2(this, mpBModel->getModelData());

    eventInfo.setArchiveName((char*)getResName());
    attention_info.flags = 0;

    mAcchCir.SetWall(50.0f, 70.0f);
    mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &mAcchCir,
              fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
    mCcStts.Init(0xFF, 0, this);

    mCcAtCyl.Set(mCcDCyl);
    mCcAtCyl.SetStts(&mCcStts);

    mCcCoCyl.Set(mCcDCyl);
    mCcCoCyl.SetStts(&mCcStts);

    mAcch.CrrPos(dComIfG_Bgsp());
    mGroundChk = mAcch.m_gnd;
    mGroundHeight = mAcch.GetGroundH();

    setEnvTevColor();
    setRoomNo();
    restart();
    Execute();
}

int daObj_Oiltubo_c::setProcess(daObj_Oiltubo_c::processFn i_process) {
    mMode = 2;

    if (mProcess != NULL) {
        (this->*mProcess)(NULL);
    }

    mMode = 0;
    mProcess = i_process;

    if (mProcess != NULL) {
        (this->*mProcess)(NULL);
    }

    mMode = 1;
    return 1;
}

void daObj_Oiltubo_c::setParam() {
    scale.set(1.0f, 1.0f, 1.0f);
    mAcchCir.SetWallR(50.0f);
    mAcchCir.SetWallH(70.0f);
}

void daObj_Oiltubo_c::setEnvTevColor() {
    tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mGroundChk);
    tevStr.room_no = dComIfG_Bgsp().GetRoomId(mGroundChk);
}

void daObj_Oiltubo_c::setRoomNo() {
    int roomId = dComIfG_Bgsp().GetRoomId(mGroundChk);

    fopAcM_SetRoomNo(this, roomId);
    mCcStts.SetRoomId(roomId);
}

void daObj_Oiltubo_c::setMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(scale);

    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mpBModel->setBaseTRMtx(mDoMtx_stack_c::get());
}

void daObj_Oiltubo_c::setAttnPos() {
    attention_info.position = current.pos;
    attention_info.position.y += 150.0f;
    eyePos = current.pos;
}

BOOL daObj_Oiltubo_c::chkEvent() {
    BOOL var_r5 = 1;

    if (!dComIfGp_getEvent().isOrderOK()) {
        var_r5 = 0;

        if (eventInfo.checkCommandCatch()) {
            return var_r5;
        }
    }

    return var_r5;
}

int daObj_Oiltubo_c::wait(void* unused) {
    switch (mMode) {
    case 1:
        if (!eventInfo.checkCommandCatch()) {
            dComIfGp_att_CatchRequest(this, fpcNm_ITEM_OIL_BOTTLE_2, 100.0f, 50.0f, -50.0f, 0x2000, 1);
            eventInfo.onCondition(dEvtCnd_40_e);
        }
        break;
    case 2:
    case 0:
        break;
    }

    return 1;
}

static int daObj_Oiltubo_Create(void* i_this) {
    return static_cast<daObj_Oiltubo_c*>(i_this)->create();
}

static int daObj_Oiltubo_Delete(void* i_this) {
    return static_cast<daObj_Oiltubo_c*>(i_this)->Delete();
}

static int daObj_Oiltubo_Execute(void* i_this) {
    return static_cast<daObj_Oiltubo_c*>(i_this)->Execute();
}

static int daObj_Oiltubo_Draw(void* i_this) {
    return static_cast<daObj_Oiltubo_c*>(i_this)->Draw();
}

static int daObj_Oiltubo_IsDelete(void* i_this) {
    return 1;
}

static actor_method_class daObj_Oiltubo_MethodTable = {
    (process_method_func)daObj_Oiltubo_Create,  (process_method_func)daObj_Oiltubo_Delete,
    (process_method_func)daObj_Oiltubo_Execute, (process_method_func)daObj_Oiltubo_IsDelete,
    (process_method_func)daObj_Oiltubo_Draw,
};

extern actor_process_profile_definition g_profile_OBJ_OILTUBO = {
    fpcLy_CURRENT_e,
    7,
    fpcPi_CURRENT_e,
    PROC_OBJ_OILTUBO,
    &g_fpcLf_Method.base,
    sizeof(daObj_Oiltubo_c),
    0,
    0,
    &g_fopAc_Method.base,
    66,
    &daObj_Oiltubo_MethodTable,
    0x44100,
    fopAc_ACTOR_e,
    fopAc_CULLBOX_CUSTOM_e,
};
