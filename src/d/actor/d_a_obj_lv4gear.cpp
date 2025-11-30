//
// d_a_obj_lv4gear.cpp
// Spinner gear (Arbiter's Grounds)
//

#include "d/dolzel_rel.h" // IWYU pragma: keep

#include "d/actor/d_a_obj_lv4gear.h"
#include "d/d_procname.h"

static int CheckCreateHeap(fopAc_ac_c* i_this) {
    return static_cast<daObjLv4Gear_c*>(i_this)->CreateHeap();
}

static void* searchSwSpinSub(void* i_actor, void* i_data) {
    daObjSwSpinner_c* sw = (daObjSwSpinner_c*)i_actor;
    daObjLv4Gear_c* gear = (daObjLv4Gear_c*)i_data;

    if (sw != NULL && fopAc_IsActor(sw) && fpcM_GetProfName(sw) == PROC_Obj_SwSpinner) {
        u8 sw_bit = sw->getSwbit();
        if (sw_bit != 0xFF && sw_bit == gear->getSwBit()) {
            return sw;
        }
    }

    return NULL;
}

void daObjLv4Gear_c::initBaseMtx() {
    mpModel->setBaseScale(scale);
    setBaseMtx();
}

void daObjLv4Gear_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos);
    mDoMtx_stack_c::YrotM(shape_angle.y + mRotation);
    MTXCopy(mDoMtx_stack_c::get(), mpModel->mBaseTransformMtx);
}

int daObjLv4Gear_c::Create() {
    initBaseMtx();
    fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
    fopAcM_setCullSizeBox2(this, mpModel->getModelData());
    mSwActorID = fpcM_ERROR_PROCESS_ID_e;
    return 1;
}

static int const l_bmd[] = {4, 3};

static char* l_arcName = "P_Gear";

int daObjLv4Gear_c::CreateHeap() {
    J3DModelData* modelData = (J3DModelData*)dComIfG_getObjectRes(l_arcName, l_bmd[mType]);
    JUT_ASSERT(213, modelData != NULL);
    mpModel = mDoExt_J3DModel__create(modelData, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    return 1;
}

static int const l_size[] = {0x42C80000, 0x43960000};

static s16 const l_rot_start_time[] = {15, 100};

static int const l_heap_size[] = {0xF00, 0xF00};

int daObjLv4Gear_c::create() {
    fopAcM_ct(this, daObjLv4Gear_c);

    mType = getType();

    int phase = dComIfG_resLoad(&mPhase, l_arcName);
    if (phase == cPhs_COMPLEATE_e) {
        if (!fopAcM_entrySolidHeap(this, (heapCallbackFunc)CheckCreateHeap, l_heap_size[mType])) {
            return cPhs_ERROR_e;
        }

        if (!Create()) {
            return cPhs_ERROR_e;
        }
    }

    return phase;
}

int daObjLv4Gear_c::execute() {
    mTarget = 0;

    if (mSwActorID == fpcM_ERROR_PROCESS_ID_e) {
        fopAc_ac_c* sw_actor = (fopAc_ac_c*)fpcM_Search(searchSwSpinSub, this);
        if (sw_actor != NULL) {
            mSwActorID = fopAcM_GetID(sw_actor);
        }
    } else {
        daObjSwSpinner_c* sw = (daObjSwSpinner_c*)fopAcM_SearchByID(mSwActorID);
        if (sw != NULL) {
            mTarget = sw->GetRotSpeedY();
        }

        if (mTarget == 0) {
            mCount = 0;
        } else {
            mCount++;
        }

        if (mCount > l_rot_start_time[mType]) {
            cLib_chaseS(&mSpeed, mTarget, 10);
        } else {
            cLib_chaseS(&mSpeed, 0, 50);
        }

        if (checkSE() == 0 && mSpeed != 0) {
            if (mType == GEAR_S_e) {
                fopAcM_seStartLevel(this, Z2SE_OBJ_SPNR_GEAR_S, mSpeed);
            } else if (mType == GEAR_L_e) {
                fopAcM_seStartLevel(this, Z2SE_OBJ_SPNR_GEAR_L, mSpeed);
            }
        }
    }

    mRotation += mSpeed;
    setBaseMtx();
    return 1;
}

int daObjLv4Gear_c::draw() {
    g_env_light.settingTevStruct(0, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    mDoExt_modelUpdateDL(mpModel);
    return 1;
}

int daObjLv4Gear_c::_delete() {
    dComIfG_resDelete(&mPhase, l_arcName);
    return 1;
}

static int daObjLv4Gear_Draw(daObjLv4Gear_c* i_this) {
    return i_this->draw();
}

static int daObjLv4Gear_Execute(daObjLv4Gear_c* i_this) {
    return i_this->execute();
}

static int daObjLv4Gear_Delete(daObjLv4Gear_c* i_this) {
    return i_this->_delete();
}

static int daObjLv4Gear_Create(daObjLv4Gear_c* i_this) {
    return i_this->create();
}

static actor_method_class l_daObjLv4Gear_Method = {
    (process_method_func)daObjLv4Gear_Create,  (process_method_func)daObjLv4Gear_Delete,
    (process_method_func)daObjLv4Gear_Execute, (process_method_func)NULL,
    (process_method_func)daObjLv4Gear_Draw,
};

extern actor_process_profile_definition g_profile_Obj_Lv4Gear = {
    fpcLy_CURRENT_e,         // mLayerID
    7,                       // mListID
    fpcPi_CURRENT_e,         // mListPrio
    PROC_Obj_Lv4Gear,        // mProcName
    &g_fpcLf_Method.base,   // sub_method
    sizeof(daObjLv4Gear_c),  // mSize
    0,                       // mSizeOther
    0,                       // mParameters
    &g_fopAc_Method.base,    // sub_method
    561,                     // mPriority
    &l_daObjLv4Gear_Method,  // sub_method
    0x40100,                 // mStatus
    fopAc_ACTOR_e,           // mActorType
    fopAc_CULLBOX_CUSTOM_e,  // cullType
};
