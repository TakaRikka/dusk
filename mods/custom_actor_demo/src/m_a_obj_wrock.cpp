/*
 * m_a_obj_wrock.cpp
 * An example actor for a rock that can be placed in the world.
 */

#include "m_a_obj_wrock.hpp"
#include "d/d_com_inf_game.h"
#include "res/Object/WRock.h"

// The name of the archive in /res/Object/
static const char* l_resName = "Wrock";

// The actor's heap (for resources) should be enough to hold the data for the model and collision
static constexpr u32 heap_size = ALIGN_NEXT(13952, 0x20) + ALIGN_NEXT(1920, 0x20);

maObj_Wrock_c::~maObj_Wrock_c() {
    // Called every time the actor is deleted

    // Remove the collider from the world's collision
    if (mpCollider != NULL) {
        dComIfG_Bgsp().Release(mpCollider);
    }

    // Request to unload the archive (the data acts as a shared pointer, and only gets deleted when
    // the reference counter goes to zero)
    dComIfG_resDelete(&mPhase, l_resName);
}

cPhs_Step maObj_Wrock_c::create() {
    // Because of how the actor system works, an actor's constructor doesn't get called when an
    // actor is created. We need to manually do it here with the following macro:
    fopAcM_ct(this, maObj_Wrock_c);

    // The create function gets called until we return cPhs_COMPLEATE_e while an actor is loading.
    // We request to load the archive here, and wait until the dvd completes loading it
    cPhs_Step step = dComIfG_resLoad(&mPhase, l_resName);
    if (step == cPhs_COMPLEATE_e) {
        // Initialize the solid heap for the actor's resources, if needed
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, heap_size)) {
            return cPhs_ERROR_e;
        }

        // Register the actor's collider to the current world's collision
        if (mpCollider != NULL) {
            if (dComIfG_Bgsp().Regist(mpCollider, this) == true) {
                return cPhs_ERROR_e;
            }
        }

        // Set the initial matrix and cull box for the actor
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_setCullSizeBox(this, -400.0f, -400.0f, -400.0f, 400.0f, 400.0f, 400.0f);

        // Setup collision info (will be used in execute)
        mAcch.Set(&current.pos, &old.pos, this, 1, &mAcchCir, &speed, &current.angle, &shape_angle);

        // Call execute so the actor's information in the world can be updated
        Execute();
    }
    return step;
}

// Initializes the heap that all instances of this actor will use for resources
// Gets called from the callback in fopAcM_entrySolidHeap
int maObj_Wrock_c::CreateHeap() {
    // Get the bmd data from the archive and initialize it
    J3DModelData* model_data =
        (J3DModelData*)dComIfG_getObjectRes(l_resName, dRes_INDEX_WROCK_BMD_WROCK_e);
    if (model_data == NULL) {
        return 0;
    }
    mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    // Get the dzb collision data from the archive and initialize it
    mpCollider = JKR_NEW dBgW();
    if (mpCollider == NULL) {
        return 0;
    }
    cBgD_t* dzb = (cBgD_t*)dComIfG_getObjectRes(l_resName, dRes_INDEX_WROCK_DZB_WROCK_e);
    if (mpCollider->Set(dzb, 1, &mColliderMtx) == true) {
        return 0;
    }
    mpCollider->SetCrrFunc(dBgS_MoveBGProc_Typical);
    return 1;
}

int maObj_Wrock_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<maObj_Wrock_c*>(i_this)->CreateHeap();
}

int maObj_Wrock_c::Delete() {
    // Call the destructor anytime we delete the actor
    this->~maObj_Wrock_c();
    return 1;
}

int maObj_Wrock_c::Execute() {
    // Update collision with the world
    mAcch.CrrPos(dComIfG_Bgsp());

    // Get the collision below the actor
    mGndChk = mAcch.m_gnd;
    mGroundH = mAcch.GetGroundH();
    if (mGroundH != -G_CM3D_F_INF) {
        // Set the actor's environment colors to match the room's current ones
        tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(mGndChk);
        tevStr.room_no = dComIfG_Bgsp().GetRoomId(mGndChk);

        // Set the actor's room to be where it is sitting
        fopAcM_SetRoomNo(this, dComIfG_Bgsp().GetRoomId(mGndChk));
    }

    // Update the model's transformation matrix to match the actor's transform
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());

    // Copy the model's transformation matrix to the collider's transformation matrix and update the
    // collider
    if (mpCollider != NULL) {
        PSMTXCopy(mpModel->getBaseTRMtx(), mColliderMtx);
        mpCollider->Move();
    }

    // Set any attention flags (if needed)
    eyePos = attention_info.position = current.pos;
    attention_info.flags = 0;
    return 1;
}

int maObj_Wrock_c::Draw() {
    // Update the model's lighting with the scene
    g_env_light.settingTevStruct(0x20, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    // Set the current dlist to BG Which means that shadows can be cast on it
    // Because of the messy collider, the shadows don't look great, but this is here as an example
    dComIfGd_setListBG();

    // Set the bmd model to be drawn when the display list is executed
    mDoExt_modelUpdateDL(mpModel);

    // Cast a shadow for the actor onto the ground.
    // We can only do this if we are drawing to the normal dlist, not the BG dlist
    // if (mGroundH != -G_CM3D_F_INF) {
    //     mShadow = dComIfGd_setShadow(mShadow, 1, mpModel, &current.pos,
    //                                  2000.0f, 0.0f,
    //                                  current.pos.y, mGroundH, mGndChk, &tevStr, 0,
    //                                  1.0f, &dDlst_shadowControl_c::mSimpleTexObj);
    // }

    // Reset the active dlist
    dComIfGd_setList();

    return 1;
}

static cPhs_Step maObj_Wrock_Create(void* i_this) {
    return static_cast<maObj_Wrock_c*>(i_this)->create();
}

static int maObj_Wrock_Delete(void* i_this) {
    return static_cast<maObj_Wrock_c*>(i_this)->Delete();
}

static int maObj_Wrock_Execute(void* i_this) {
    return static_cast<maObj_Wrock_c*>(i_this)->Execute();
}

static int maObj_Wrock_Draw(void* i_this) {
    return static_cast<maObj_Wrock_c*>(i_this)->Draw();
}

static int maObj_Wrock_IsDelete(void*) {
    return 1;
}

s16 maObj_Wrock_c::sProcName = -1;
ActorHandle maObj_Wrock_c::sActorHandle = -1;
const ActorProfileDesc maObj_Wrock_c::sProfile = {.name = MAOBJ_WROCK_NAME,
    .priority_group = 7,
    .process_size = sizeof(maObj_Wrock_c),
    .draw_priority = fpcDwPi_OBJ_LBOX_e,  // An unused draw priority
    .status = fopAcStts_UNK_0x40000_e | fopAcStts_UNK_0x4000_e | fopAcStts_CULL_e,
    .group = fopAc_ACTOR_e,
    .cull_type = fopAc_CULLBOX_CUSTOM_e,
    .create_function = maObj_Wrock_Create,
    .delete_function = maObj_Wrock_Delete,
    .execute_function = maObj_Wrock_Execute,
    .is_delete_function = maObj_Wrock_IsDelete,
    .draw_function = maObj_Wrock_Draw};
