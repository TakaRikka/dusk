/**
 * @file d_a_obj_stairBlock.cpp
 *
 */

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_stairBlock.h"

daStairBlock_HIO_c::daStairBlock_HIO_c() {
    for (int i = 0; i < 10; i++) {
        collision_radius[i] = 0.0f;
    }
}

void daStairBlock_c::setBaseMtx() {
    for (int i = 0; i < 10; i++) {
        mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
        mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
        mpModel[i]->setBaseScale(scale);
        mpModel[i]->setBaseTRMtx(mDoMtx_stack_c::get());
    }
}

const dCcD_SrcGObjInf daStairBlock_c::mCcDObjInfo = {
    {0, {0, 0, 0, {0x0, 0x0}, {0x19}}},
    {dCcD_SE_NONE, 0, 0, 0, 0x0},
    {dCcD_SE_NONE, 0, 0, 0, 0x0},
    {0},
};

static const s32 l_bmdIdx[10] = {
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
};

static const s32 l_bckIdx[10] = {
    0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D,
};

static const Vec centerPos[10] = {
    {28214.64f, -2576.334f, 7412.408f},   {28217.744f, -2681.6169f, 7392.684f},
    {28336.742f, -2641.343f, 7329.605f},  {27901.223f, -2526.949f, 7028.0098f},
    {27893.008f, -2604.662f, 7142.8018f}, {28036.146f, -2555.01f, 7141.08f},
    {27985.621f, -2644.115f, 6984.075f},  {27985.621f, -2644.115f, 6984.075f},
    {27985.621f, -2644.115f, 6984.075f},  {27985.621f, -2644.115f, 6984.075f},
};

int daStairBlock_c::CreateHeap() {
    for (int i = 0; i < 10; i++) {
        J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes("StaBlock", l_bmdIdx[i]);
        JUT_ASSERT(276, modelData != NULL);

        mpModel[i] = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
        if (mpModel[i] == NULL) {
            return 0;
        }

        s32 res = mBckAnm[i].init((J3DAnmTransform*)dComIfG_getObjectRes("StaBlock", l_bckIdx[i]), 1, 0, 1.0f, 0, -1, false);
        JUT_ASSERT(290, res == 1);

        mBckAnm[i].setEndFrame(mBckAnm[i].getEndFrame() - 1.0f);
    }
    return 1;
}

int daStairBlock_c::create() {
    fopAcM_ct(this, daStairBlock_c);

    int phase = dComIfG_resLoad(&mPhaseReq, "StaBlock");
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, 0x9000)) {
            return cPhs_ERROR_e;
        }

        fopAcM_SetMtx(this, mpModel[0]->getBaseTRMtx());
        fopAcM_setCullSizeBox2(this, mpModel[0]->getModelData());
        mStts.Init(0, 0xFF, this);

        for (int i = 0; i < 10; i++) {
            mSpheres[i].Set(mCcDSph);
            mSpheres[i].SetStts(&mStts);
            mHit[i] = 0;
        }

        for (int i = 1; i < 10; i++) {
            mDoExt_setupShareTexture(mpModel[i]->getModelData(), mpModel[0]->getModelData());
        }

        setBaseMtx();
    }

    return phase;
}

int daStairBlock_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<daStairBlock_c*>(i_this)->CreateHeap();
}

static daStairBlock_HIO_c l_HIO;

int daStairBlock_c::Execute() {
    for (int i = 0; i < 10; i++) {
        if (mSpheres[i].ChkCoHit()) {
            if (mHit[i] == 0) {
                mSpheres[i].OffCoSetBit();
                mHit[i] = 1;
                cXyz soundPos(centerPos[i]);
                mDoAud_seStart(Z2SE_OBJ_BREAK_STEPS, &soundPos, 0,
                               dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
            }
            mSpheres[i].ClrCoHit();
        }
        mSpheres[i].SetR(l_HIO.collision_radius[i] + 200.0f);
        cXyz posC(centerPos[i]);
        mSpheres[i].SetC(posC);
        dComIfG_Ccsp()->Set(&mSpheres[i]);

        if (mHit[i] != 0 && mBckAnm[i].play()) {
            mHit[i] = 0;
            cXyz soundPos(centerPos[i]);
            mDoAud_seStart(Z2SE_OBJ_BREAK_STONE, &soundPos, 0,
                           dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
            mDoAud_seStart(Z2SE_OBJ_FALL_WATER_S, &soundPos, 0,
                           dComIfGp_getReverb(fopAcM_GetRoomNo(this)));
        }
    }
    return 1;
}

int daStairBlock_c::Draw() {
    g_env_light.settingTevStruct(0x40, &current.pos, &tevStr);
    dComIfGd_setListBG();

    for (int i = 0; i < 10; i++) {
        g_env_light.setLightTevColorType_MAJI(mpModel[i], &tevStr);
        mBckAnm[i].entry(mpModel[i]->getModelData());
        mDoExt_modelUpdateDL(mpModel[i]);
    }
    dComIfGd_setList();
    return 1;
}

int daStairBlock_c::Delete() {
    dComIfG_resDelete(&mPhaseReq, "StaBlock");

#if DEBUG
    l_HIO.removeHIO();
#endif

    return 1;
}

static int daStairBlock_Draw(daStairBlock_c* i_this) {
    return i_this->Draw();
}

static int daStairBlock_Execute(daStairBlock_c* i_this) {
    return i_this->Execute();
}

static int daStairBlock_Delete(daStairBlock_c* i_this) {
    return i_this->Delete();
}

static int daStairBlock_Create(fopAc_ac_c* i_this) {
    return static_cast<daStairBlock_c*>(i_this)->create();
}

dCcD_SrcSph daStairBlock_c::mCcDSph = {daStairBlock_c::mCcDObjInfo, {{{0.0f, 0.0f, 0.0f}, 4.0f}}};

static actor_method_class l_daStairBlock_Method = {
    (process_method_func)daStairBlock_Create,  (process_method_func)daStairBlock_Delete,
    (process_method_func)daStairBlock_Execute, 0,
    (process_method_func)daStairBlock_Draw,
};

actor_process_profile_definition g_profile_Obj_StairBlock = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_StairBlock,     // mProcName
    &g_fpcLf_Method.base,    // sub_method
    sizeof(daStairBlock_c),  // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    500,                     // mPriority
    &l_daStairBlock_Method,  // sub_method
    0x00044000,              // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
