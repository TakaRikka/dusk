/**
 * @file d_a_obj_automata.cpp
 * 
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_automata.h"
#include "SSystem/SComponent/c_counter.h"
#include "d/d_cc_d.h"
#include "d/d_com_inf_game.h"

static int l_bmdData[1][2] = {
    9, 1,
};

static char* l_resNameList[2] = {
    "",
    "AutoMata",
};

f32 const daObj_AutoMata_Param_c::m[3] = {
    220.0f, 80.0f, 40.0f,
};

static dCcD_SrcGObjInf const l_ccDObjData = 
{
        {0x0, {{0x0, 0x0, 0}, {0xD8FBFDFF, 0x1F}, 0x79}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_STONE, 0x0, 0x0, 0x0, 0x0}, // mGObjTg
        {0x0}, // mGObjCo
};

static dCcD_SrcCyl l_ccDCyl = {
    l_ccDObjData, // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f}, // mCenter
            0.0f, // mRadius
            0.0f // mHeight
        } // mCyl
    }
};

static dCcD_SrcSph l_ccDSph = {
    l_ccDObjData, // mObjInf
    {
        {{0.0f, 0.0f, 0.0f}, 0.0f} // mSph
    } // mSphAttr
};

daObj_AutoMata_c::~daObj_AutoMata_c() {
    OS_REPORT("|%06d:%x|daObj_AutoMata_c -> デストラクト\n", g_Counter.mCounter0, this);
    if (mpMorf != NULL) {
        mpMorf->stopZelAnime();
    }
    dComIfG_resDelete(&mPhase, l_resNameList[l_bmdData[field_0xb30][1]]);
}

int daObj_AutoMata_c::create() {
    fopAcM_ct(this, daObj_AutoMata_c);
    field_0xb30 = 0;
    int rv = dComIfG_resLoad(&mPhase, l_resNameList[l_bmdData[field_0xb30][1]]);
    if (rv == cPhs_COMPLEATE_e) {
        OS_REPORT("\t(%s:%d) <%08x>\n", fopAcM_getProcNameString(this), field_0xb30, fopAcM_GetParam(this));
        if (fopAcM_entrySolidHeap(this, createHeapCallBack, 0x1370) == 0) {
            return cPhs_ERROR_e;
        } 
        mpMorf->getModel()->getModelData();
        fopAcM_SetMtx(this, mpMorf->getModel()->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -200.0f, -100.0f, -200.0f, 200.0f, 300.0f, 200.0f);
        mCreature.init(&current.pos, &eyePos, 3, 1);
        mAcch.Set(fopAcM_GetPosition_p(this), fopAcM_GetOldPosition_p(this), this, 1, &field_0x834,
                  fopAcM_GetSpeed_p(this), fopAcM_GetAngle_p(this), fopAcM_GetShapeAngle_p(this));
        mStts.Init(0xff, 0, this);
        mCyl.Set(l_ccDCyl);
        mCyl.SetStts(&mStts);
        mSph.Set(l_ccDSph);
        mSph.SetStts(&mStts);
        mAcch.SetWallNone();
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        if (mGroundH != -G_CM3D_F_INF) {
            setEnvTevColor();
            setRoomNo();
        }
        Execute();
    }
    return rv;
}

int daObj_AutoMata_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(
        l_resNameList[l_bmdData[field_0xb30][1]],
        l_bmdData[field_0xb30][0]);
    if (modelData == NULL) {
        return 0;
    }
    mpMorf = new mDoExt_McaMorfSO(modelData, NULL, NULL, NULL, -1, 1.0f, 0, -1, &mCreature, 0x80000,
                                  0x11000284);
    if (mpMorf == NULL || mpMorf->getModel() == NULL) {
        return 0;
    } 
    return setAnm(0, false) ? 1 : 0;
}

int daObj_AutoMata_c::Delete() {
    OS_REPORT("|%06d:%x|daObj_AutoMata_c -> Delete\n", g_Counter.mCounter0, this);
    this->~daObj_AutoMata_c();
    return 1;
}

int daObj_AutoMata_c::Execute() {
    if (home.roomNo == dComIfGp_roomControl_getStayNo()) {
        cXyz sphCenter;
        cXyz cStack_28;
        mAcch.SetWallNone();
        gravity = 0.0f;
        if (dComIfGp_event_runCheck()) {
            gravity = -4.0f;
        }
        fopAcM_posMoveF(this, mStts.GetCCMoveP());
        mAcch.CrrPos(dComIfG_Bgsp());
        mGndChk = mAcch.m_gnd;
        mGroundH = mAcch.GetGroundH();
        if (mGroundH != -G_CM3D_F_INF) {
            if (fopAcIt_Judge((fopAcIt_JudgeFunc)srchTHouse, &current.pos) != NULL) {
                current.pos.y = mGroundH;
            }
            setEnvTevColor();
            setRoomNo();
        }
        mpMorf->setPlaySpeed(mAnmPlaySpeed);
        mpMorf->play(0, 0);
        mBtk.setPlaySpeed(mAnmPlaySpeed);
        mBtk.play();
        mAnmPlaySpeed = 0.0f;
        setSe();
        setMtx();
        if (mCoNone == 0) {
            cStack_28.set(0.0f, 40.0f, 0.0f);
            mDoMtx_stack_c::copy(mpMorf->getModel()->getAnmMtx(3));
            mDoMtx_stack_c::multVec(&cStack_28, &sphCenter);
            mSph.SetC(sphCenter);
            mSph.SetR(daObj_AutoMata_Param_c::m[2]);
            dComIfG_Ccsp()->Set(&mSph);
            mCyl.SetH(daObj_AutoMata_Param_c::m[0]);
            mCyl.SetR(daObj_AutoMata_Param_c::m[1]);
            mCyl.SetC(current.pos);
            dComIfG_Ccsp()->Set(&mCyl);
        }
        mSph.ClrCoHit();
        mCyl.ClrCoHit();
        attention_info.flags = 0;
        eyePos = attention_info.position = current.pos;
        mCoNone = 0;
        return 1;
    } 
    return 0;
}

int daObj_AutoMata_c::Draw() {
    J3DModel* morfModel = mpMorf->getModel();
    J3DModelData* modelData = morfModel->getModelData();
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(morfModel, &tevStr);
    mBtk.entry(modelData);
    mpMorf->entryDL();
    mBtk.remove(modelData);
    if (mGroundH != -G_CM3D_F_INF) {
        mShadowId = dComIfGd_setShadow(
            mShadowId, 1, morfModel, &current.pos, 600.0f, 20.0f,
            current.pos.y, mGroundH, mGndChk,
            &tevStr, 0, 1.0f, dDlst_shadowControl_c::getSimpleTex());
    }
    return 1;
}

int daObj_AutoMata_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daObj_AutoMata_c*>(i_this)->CreateHeap();
}

void* daObj_AutoMata_c::srchTHouse(fopAc_ac_c* actor, void* param_1) {
    if (fopAcM_GetName(actor) == PROC_Obj_TobyHouse) {
        if (fopAcM_IsExecuting(fopAcM_GetID(actor))) {
            return actor;
        }
    }
    return NULL;
}

static int const bckArcIx[2] = {
    5, 6,
};

static int const btkArcIx[2] = {
    12, 13,
};

static int const playMode[2] = {
    0, 2,
};

int daObj_AutoMata_c::setAnm(int param_1, bool param_2) {
    J3DModelData* modelData = mpMorf->getModel()->getModelData();
    const char* resName = l_resNameList[l_bmdData[field_0xb30][1]];
    if (param_1 >= 0 && param_1 < 2) {
        J3DAnmTransform* anmTransform = (J3DAnmTransform*)dComIfG_getObjectRes(
            resName,
            bckArcIx[param_1]);
        if (anmTransform == NULL) {
            return 0;
        }
        mpMorf->setAnm(anmTransform, playMode[param_1], 0.0f, 0.0f, 0.0f, -1.0f);
        J3DAnmTextureSRTKey* srtKey =
            (J3DAnmTextureSRTKey*)dComIfG_getObjectRes(resName, btkArcIx[param_1]);
        if (srtKey == NULL) {
            return 0;
        }
        if (mBtk.init(modelData, srtKey, 1, playMode[param_1], 0.0f, 0, -1) == 1) {
            return 1;
        }
        return 0;
    }
    return 0;
}

void daObj_AutoMata_c::setEnvTevColor() {
    tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mGndChk);
    tevStr.room_no = dComIfG_Bgsp().GetRoomId(mGndChk);
}

void daObj_AutoMata_c::setRoomNo() {
    s32 roomId = dComIfG_Bgsp().GetRoomId(mGndChk);
    fopAcM_SetRoomNo(this, roomId);
    mStts.SetRoomId(roomId);
}

void daObj_AutoMata_c::setMtx() {
    J3DModel* morfModel = mpMorf->getModel();
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(scale);
    morfModel->setBaseTRMtx(mDoMtx_stack_c::get());
    mpMorf->modelCalc();
}

void daObj_AutoMata_c::setSe() {
    if (cM3d_IsZero(mpMorf->getPlaySpeed()) == false) {
        const char* resName = l_resNameList[l_bmdData[field_0xb30][1]];
        J3DAnmTransform* anmTransform = (J3DAnmTransform*)dComIfG_getObjectRes(resName, 5);
        J3DAnmTransform* currentAnm = mpMorf->getAnm();
        if (anmTransform == currentAnm) {
            if (mpMorf->checkFrame(17.0f)) {
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_AMATA_ACTION, &current.pos,
                                         mpMorf->getPlaySpeed() * 100.0f, 0, 1.0f, 1.0f, -1.0f,
                                         -1.0f, 0);
            }
            Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_AMATA_CRK, &current.pos,
                                          mpMorf->getPlaySpeed() * 100.0f, 0, 1.0f, 1.0f, -1.0f,
                                          -1.0f, 0);
        }
        anmTransform = (J3DAnmTransform*)dComIfG_getObjectRes(resName, 6);
        currentAnm = mpMorf->getAnm();
        if (anmTransform == currentAnm) {
            if (mpMorf->checkFrame(7.0f)) {
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_AMATA_ACTION, &current.pos,
                                         mpMorf->getPlaySpeed() * 100.0f, 0, 1.0f, 1.0f, -1.0f,
                                         -1.0, 0);
            }
            if (mpMorf->checkFrame(67.0f)) {
                Z2GetAudioMgr()->seStart(Z2SE_OBJ_AMATA_ACTION, &current.pos,
                                         mpMorf->getPlaySpeed() * 100.0f, 0, 1.0f, 1.0f, -1.0f,
                                         -1.0f, 0);
            }
            Z2GetAudioMgr()->seStartLevel(Z2SE_OBJ_AMATA_CRK, &current.pos,
                                          mpMorf->getPlaySpeed() * 100.0f, 0, 1.0f, 1.0f, -1.0f,
                                          -1.0f, 0);
        }
    }
}

static int daObj_AutoMata_Create(void* i_this) {
    return static_cast<daObj_AutoMata_c*>(i_this)->create();
}

static int daObj_AutoMata_Delete(void* i_this) {
    return static_cast<daObj_AutoMata_c*>(i_this)->Delete();
}

static int daObj_AutoMata_Execute(void* i_this) {
    return static_cast<daObj_AutoMata_c*>(i_this)->Execute();
}

static int daObj_AutoMata_Draw(void* i_this) {
    return static_cast<daObj_AutoMata_c*>(i_this)->Draw();
}

static int daObj_AutoMata_IsDelete(void* i_this) {
    return 1;
}

static daObj_AutoMata_Param_c l_HIO;

static actor_method_class daObj_AutoMata_MethodTable = {
    (process_method_func)daObj_AutoMata_Create,
    (process_method_func)daObj_AutoMata_Delete,
    (process_method_func)daObj_AutoMata_Execute,
    (process_method_func)daObj_AutoMata_IsDelete,
    (process_method_func)daObj_AutoMata_Draw,
};

actor_process_profile_definition g_profile_OBJ_AUTOMATA = {
  fpcLy_CURRENT_e,             // mLayerID
  8,                           // mListID
  fpcPi_CURRENT_e,             // mListPrio
  PROC_OBJ_AUTOMATA,           // mProcName
  &g_fpcLf_Method.base,       // sub_method
  sizeof(daObj_AutoMata_c),    // mSize
  0,                           // mSizeOther
  0,                           // mParameters
  &g_fopAc_Method.base,        // sub_method
  81,                          // mPriority
  &daObj_AutoMata_MethodTable, // sub_method
  0x00044100,                  // mStatus
  fopAc_ACTOR_e,               // mActorType
  fopAc_CULLBOX_CUSTOM_e,      // cullType
};
