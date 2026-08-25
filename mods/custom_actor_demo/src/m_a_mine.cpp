#include "m_a_mine.hpp"
#include "d/d_com_inf_game.h"
#include "res/Object/O_mD_jira.h"

// The name of the archive in /res/Object/
static const char* l_resName = "O_mD_jira";

// The actor's heap (for resources) should be enough to hold the data for the model and collision
static constexpr u32 heap_size = ALIGN_NEXT(16832, 0x20);

ma_Mine_c::~ma_Mine_c() {
    // Called every time the actor is deleted

    // Delete the sound object
    mSound.deleteObject();

    // Request to unload the archive (the data acts as a shared pointer, and only gets deleted when
    // the reference counter goes to zero)
    dComIfG_resDelete(&mPhase, l_resName);
}

cPhs_Step ma_Mine_c::create() {
    // Because of how the actor system works, an actor's constructor doesn't get called when an
    // actor is created. We need to manually do it here with the following ma:
    fopAcM_ct(this, ma_Mine_c);

    // The create function gets called until we return cPhs_COMPLEATE_e while an actor is loading.
    // We request to load the archive here, and wait until the dvd completes loading it
    cPhs_Step step = dComIfG_resLoad(&mPhase, l_resName);
    if (step == cPhs_COMPLEATE_e) {
        // Initialize the solid heap for the actor's resources, if needed
        if (!fopAcM_entrySolidHeap(this, createHeapCallBack, heap_size)) {
            return cPhs_ERROR_e;
        }

        // Setup our collision sphere and register it to the world
        // Set a circle "wall" of 30 units around the actor
        mAcchCir.SetWall(30.0f, 30.0f);
        mAcch.Set(this, 1, &mAcchCir);
        mAcch.ClrWaterNone();
        mAcch.SetRoofCrrHeight(60.0f);
        mAcch.SetWaterCheckOffset(10000.0f);
        mAcch.SetWtrChkMode(2);
        mAcch.OnLineCheck();

        mCcStts.Init(30, 0xFF, this);

        // Collision Sphere for the actor (copied from Bomb Actor)
        static const dCcD_SrcSph
            l_sphSrc = {.mObjInf =
                            {
                                .mObj = {.mFlags = 0x0,
                                    .mSrcObjHitInf = {.mObjAt = {.mType = AT_TYPE_BOMB,
                                                          .mAtp = 0x4,
                                                          .mBase = {.mSPrm = 0x1e}},
                                        .mObjTg = {.mType = 0xd8fbffef, .mBase = {.mSPrm = 0x11}},
                                        .mObjCo = {.mBase = {.mSPrm = 0x79}}}},
                                .mGObjAt{.mSe = dCcD_SE_NONE,
                                    .mHitMark = 0x0,
                                    .mSpl = 0x1,
                                    .mMtrl = 0x0,
                                    .mBase = {.mGFlag = 0x0}},
                                .mGObjTg{.mSe = dCcD_SE_NONE,
                                    .mHitMark = 0x0,
                                    .mSpl = 0x0,
                                    .mMtrl = 0x0,
                                    .mBase = {.mGFlag = 0x4}},
                                .mGObjCo{.mBase = {.mGFlag = 0x0}},
                            },
                .mSphAttr = {.mSph = {.mCenter = {0.0f, 0.0f, 0.0f}, .mRadius = 80.0f}}};

        mCollisionSphere.Set(l_sphSrc);
        mCollisionSphere.SetStts(&mCcStts);

        // We register a callback anytime the actor is hit (both attacks and is hit)
        mCollisionSphere.SetAtHitCallback(atHitCallback);
        mCollisionSphere.SetTgHitCallback(atHitCallback);
        mCollisionSphere.OffTgSetBit();
        mCollisionSphere.OffCoSetBit();
        mCollisionSphere.OnAtSetBit();  // Enable the attack sphere

        // Set the initial matrix and cull box for the actor
        fopAcM_SetMtx(this, mpModel->getBaseTRMtx());
        fopAcM_SetMin(this, -36.0f, 0.0f, -36.0f);
        fopAcM_SetMax(this, 36.0f, 66.0f, 36.0f);

        // Call execute so the actor's information in the world can be updated
        Execute();
    }
    return step;
}

// Initializes the heap that all instances of this actor will use for resources
// Gets called from the callback in fopAcM_entrySolidHeap
int ma_Mine_c::CreateHeap() {
    // Get the bmd data from the archive and initialize it
    J3DModelData* model_data =
        (J3DModelData*)dComIfG_getObjectRes(l_resName, dRes_INDEX_O_MD_JIRA_BMD_O_MD_JIRAI_e);
    if (model_data == NULL) {
        return 0;
    }
    mpModel = mDoExt_J3DModel__create(model_data, 0x80000, 0x11000084);
    if (mpModel == NULL) {
        return 0;
    }

    // Create the sound object the actor will use
    mSound.init(&current.pos, 1);

    return 1;
}

int ma_Mine_c::createHeapCallBack(fopAc_ac_c* i_this) {
    return static_cast<ma_Mine_c*>(i_this)->CreateHeap();
}

int ma_Mine_c::Delete() {
    // Call the destructor anytime we delete the actor
    this->~ma_Mine_c();
    return 1;
}

int ma_Mine_c::Execute() {
    // Update collision with the world
    mAcch.CrrPos(dComIfG_Bgsp());
    mCollisionSphere.SetC(attention_info.position);
    dComIfG_Ccsp()->Set(&mCollisionSphere);

    // Get the collision below the actor
    cBgS_GndChk groundChunk = mAcch.m_gnd;
    f32 groundH = mAcch.GetGroundH();
    if (groundH != -G_CM3D_F_INF) {
        // Set the actor's environment colors to match the room's current ones
        int roomNo = dComIfG_Bgsp().GetRoomId(groundChunk);
        tevStr.YukaCol = dComIfG_Bgsp().GetPolyColor(groundChunk);
        tevStr.room_no = roomNo;

        // Set the actor's room to be where it is sitting
        mCcStts.SetRoomId(roomNo);
        fopAcM_SetRoomNo(this, roomNo);

        // Get the reverb info here that the actor can use when exploding
        mReverb = dComIfGp_getReverb(roomNo);
    }

    // Update the model's transformation matrix to match the actor's transform
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle);
    mDoMtx_stack_c::scaleM(scale);
    mpModel->setBaseTRMtx(mDoMtx_stack_c::get());

    // Set any attention flags (if needed)
    eyePos = attention_info.position = current.pos;
    attention_info.flags = 0;
    return 1;
}

int ma_Mine_c::Draw() {
    // Update the model's lighting with the scene
    g_env_light.settingTevStruct(0x20, &current.pos, &tevStr);
    g_env_light.setLightTevColorType_MAJI(mpModel, &tevStr);

    // Set the bmd model to be drawn when the display list is executed
    mDoExt_modelUpdateDL(mpModel);

    return 1;
}

void ma_Mine_c::atHit(dCcD_GObjInf* i_atObjInf) {
    // Create particles with these IDs at the actor's position
    static const u16 normalNameID[] = {
        0x161, 0x162, 0x163, 0x164, 0x165, 0x166, 0x167, 0x168, 0x1EC};
    for (int i = 0; i < ARRAY_SIZE(normalNameID); i++) {
        dComIfGp_particle_setColor(normalNameID[i], &current.pos, &tevStr, NULL, NULL, 0.0f, 0xFF,
            &shape_angle, &scale, NULL, -1, NULL);
    }

    // Create an explosion sound
    mSound.startSound(Z2SE_OBJ_BOMB_EXPLODE, 0, mReverb);

    // Vibrate the controller
    dComIfGp_getVibration().StartShock(4, 31, cXyz(0.0f, 1.0f, 0.0f));

    // Request to delete the actor so it disappears
    fopAcM_delete(this);
}

void ma_Mine_c::atHitCallback(fopAc_ac_c* i_tgActor, dCcD_GObjInf* i_tgObjInf,
    fopAc_ac_c* i_atActor, dCcD_GObjInf* i_atObjInf) {
    // This callback gets triggered anytime an intersection happens with the object's collision sphere
    ((ma_Mine_c*)i_tgActor)->atHit(i_atObjInf);
}

static cPhs_Step ma_Mine_create(void* i_this) {
    return static_cast<ma_Mine_c*>(i_this)->create();
}

static int maMine_Delete(void* i_this) {
    return static_cast<ma_Mine_c*>(i_this)->Delete();
}

static int maMine_Execute(void* i_this) {
    return static_cast<ma_Mine_c*>(i_this)->Execute();
}

static int maMine_Draw(void* i_this) {
    return static_cast<ma_Mine_c*>(i_this)->Draw();
}

static int maMine_IsDelete(void*) {
    return 1;
}

s16 ma_Mine_c::sProcName = -1;
ActorHandle ma_Mine_c::sActorHandle = -1;
const ActorProfileDesc ma_Mine_c::sProfile = {.name = MA_MINE_NAME,
    .priority_group = 7,
    .process_size = sizeof(ma_Mine_c),
    .draw_priority = fpcDwPi_OBJ_LBOX_e,  // An unused draw priority
    .status = fopAcStts_UNK_0x40000_e | fopAcStts_UNK_0x4000_e | fopAcStts_CULL_e,
    .group = fopAc_ACTOR_e,
    .cull_type = fopAc_CULLBOX_CUSTOM_e,
    .create_function = ma_Mine_create,
    .delete_function = maMine_Delete,
    .execute_function = maMine_Execute,
    .is_delete_function = maMine_IsDelete,
    .draw_function = maMine_Draw};
