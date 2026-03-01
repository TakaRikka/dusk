/**
 * @file d_a_obj_waterfall.cpp
*/

#include "d/dolzel_rel.h" // IWYU pragma: keep

#if DEBUG
#include "d/d_debug_viewer.h"
#endif

#include "d/actor/d_a_obj_waterfall.h"
#include "d/actor/d_a_arrow.h"
#include "d/d_com_inf_game.h"
#include "d/d_bomb.h"
#include "SSystem/SComponent/c_math.h"
#include "f_op/f_op_actor.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_mtx.h"

#define MAX_TARGET_INFO_COUNT 10

static int daObjWaterFall_Draw(daObjWaterFall_c* i_this);
static int daObjWaterFall_Execute(daObjWaterFall_c* i_this);
static int daObjWaterFall_Delete(daObjWaterFall_c* i_this);
static int daObjWaterFall_Create(fopAc_ac_c* i_this);

static fopAc_ac_c* target_info[MAX_TARGET_INFO_COUNT]; // Mutually exclusive list of bomb/arrow actors to be potentially deleted
static int target_info_count;

const char* l_arcName = "sample";

#if DEBUG
static cull_box l_cull_box = {
    {-200.0f, -100.0f, -200.0f},
    {200.0f, 100.0f, 200.0f}
};

static dCcD_SrcTri l_tri_src = {
    {
        {0, {{0, 0, 0}, {0x6002, 0x11}, 0}},
        {dCcD_SE_NONE, 0, 0, 0, {0}},
        {dCcD_SE_NONE, 0, 0, 0, {0x84}},
        {0},
    },
    {
        {
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
        },
    },
};
#endif

static dCcD_SrcCyl l_cyl_src = {
    {
        {0x0, {{0x0, 0x0, 0x0}, {0x2, 0x10}, 0x119}}, // mObj
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x0}, // mGObjAt
        {dCcD_SE_NONE, 0x0, 0x0, 0x0, 0x84}, // mGObjTg
        {0x2}, // mGObjCo
    }, // mObjInf
    {
        {
            {0.0f, 0.0f, 0.0f}, // mCenter
            0.0f, // mRadius
            0.0f // mHeight
        } // mCyl
    }
};

#if DEBUG
class daObjWaterFall_HIO_c : public mDoHIO_entry_c {
public:
    daObjWaterFall_HIO_c();
    ~daObjWaterFall_HIO_c() {}

    void genMessage(JORMContext*);

    /* 0x08 */ f32 mPushStrengthHuman;
    /* 0x0C */ f32 mPushStrengthWolf;
};

static daObjWaterFall_HIO_c l_HIO;

daObjWaterFall_HIO_c::daObjWaterFall_HIO_c() {
    mPushStrengthHuman = 10.0f;
    mPushStrengthWolf = 25.0f;
}

void daObjWaterFall_HIO_c::genMessage(JORMContext* mctx) {
    /* Waterfall */
    mctx->genLabel("滝", 0);
    // Pushing force ・ human
    mctx->genSlider("押す力・人間", &mPushStrengthHuman, 0.0f, 100.0f);
    // Pushing force ・ wolf
    mctx->genSlider("押す力・狼", &mPushStrengthWolf, 0.0f, 100.0f);
}

#endif

static void* s_b_sub(void* param_0, void* unused) {
    #define BOMB_ACTOR static_cast<dBomb_c*>(param_0)
    UNUSED(unused);

    if(fopAcM_IsActor(param_0) && dBomb_c::checkBombActor(BOMB_ACTOR)) {
        if(!dBomb_c::checkWaterBomb(BOMB_ACTOR) && !BOMB_ACTOR->checkStateExplode()) {
            if(target_info_count < MAX_TARGET_INFO_COUNT) {
                target_info[target_info_count] = BOMB_ACTOR;
                target_info_count++;
            }
        }
    }
    return NULL;
}

static void* s_a_sub(void* param_0, void* unused) {
    UNUSED(unused);

    if(fopAcM_IsActor(param_0) && fopAcM_GetName(param_0) == PROC_ARROW) {
        if(target_info_count < MAX_TARGET_INFO_COUNT) {
            target_info[target_info_count] = static_cast<daArrow_c*>(param_0);
            target_info_count++;
        }
    }
    return NULL;
}

void daObjWaterFall_c::search_bomb() {
    if(!checkFallOut()) {
        //  Get rid of previously found arrow or bomb processes populating target_info
        target_info_count = 0;
        for(int i = 0; i < MAX_TARGET_INFO_COUNT; i++) {
            target_info[i] = NULL;
        }

        //  Find first 10 bomb processes to populate target_info
        fpcM_Search(s_b_sub, this);

        //  Check whether the bomb processes are within bounds (and should therefore be deleted)
        if(target_info_count) {
            for(int i = 0; i < target_info_count; i++) {
                cXyz vectorToOldBombPos;
                cXyz vectorToCurrentBombPos;
                dBomb_c* const bomb = static_cast<dBomb_c*>(target_info[i]);

                vectorToOldBombPos = bomb->old.pos - current.pos;
                vectorToCurrentBombPos =  bomb->current.pos - current.pos;

                mDoMtx_stack_c::YrotS(-current.angle.y);

                mDoMtx_stack_c::multVec(&vectorToOldBombPos, &vectorToOldBombPos);
                mDoMtx_stack_c::multVec(&vectorToCurrentBombPos, &vectorToCurrentBombPos);

                // Bomb's Z position passed that of waterfall, and was within X,Y bounds both in the previous frame and current frame
                bool multiFrameDeleteCondition = (
                    (vectorToOldBombPos.z * vectorToCurrentBombPos.z) < 0.0f &&
                    (vectorToOldBombPos.x > -scale.x * 50.0f && vectorToOldBombPos.x < scale.x * 50.0f) &&
                    (vectorToCurrentBombPos.x > -scale.x * 50.0f && vectorToCurrentBombPos.x < scale.x * 50.0f) &&
                    (vectorToOldBombPos.y > 0.0f && vectorToOldBombPos.y < scale.y * 100.0f) &&
                    (vectorToCurrentBombPos.y > 0.0f && vectorToCurrentBombPos.y < scale.y * 100.0f)
                );

                // Bomb is within X,Y,Z bounds in the current frame
                bool currentFrameDeleteCondition = (
                    (vectorToCurrentBombPos.x > -scale.x * 50.0f && vectorToCurrentBombPos.x < scale.x * 50.0f) &&
                    (vectorToCurrentBombPos.y > 0.0f && vectorToCurrentBombPos.y < scale.y * 100.0f) &&
                    (vectorToCurrentBombPos.z > -scale.z * 50.0f && vectorToCurrentBombPos.z < scale.z * 50.0f)
                );

                if(multiFrameDeleteCondition || currentFrameDeleteCondition)
                    fopAcM_delete(bomb);
            }
        }
    }
}

void daObjWaterFall_c::search_arrow() {
    //  Some waterfalls (e.g. in the Lakebed Temple) allow arrows to fly through them
    if(getType() == ALLOW_ARROWS_e || checkFallOut())
        return;

    //  Get rid of previously found arrow or bomb processes populating target_info
    target_info_count = 0;
    for(int i = 0; i < MAX_TARGET_INFO_COUNT; i++) {
        target_info[i] = NULL;
    }

    //  Find first 10 arrow processes to populate target_info
    fpcM_Search(s_a_sub, this);

    //  Check whether the arrow processes are within bounds (and should therefore be deleted)
    if(target_info_count) {
        for(int i = 0; i < target_info_count; i++) {
            cXyz vectorToOldArrowPos;
            cXyz vectorToCurrentArrowPos;
            daArrow_c* const arrow = static_cast<daArrow_c*>(target_info[i]);

            vectorToOldArrowPos = arrow->old.pos - current.pos;
            vectorToCurrentArrowPos =  arrow->current.pos - current.pos;

            mDoMtx_stack_c::YrotS(-current.angle.y);

            mDoMtx_stack_c::multVec(&vectorToOldArrowPos, &vectorToOldArrowPos);
            mDoMtx_stack_c::multVec(&vectorToCurrentArrowPos, &vectorToCurrentArrowPos);

            // Arrow's Z position passed that of waterfall, and was within X,Y bounds both in the previous frame and current frame
            bool multiFrameDeleteCondition = (
                (vectorToOldArrowPos.z * vectorToCurrentArrowPos.z) < 0.0f &&
                (vectorToOldArrowPos.x > -scale.x * 50.0f && vectorToOldArrowPos.x < scale.x * 50.0f) &&
                (vectorToCurrentArrowPos.x > -scale.x * 50.0f && vectorToCurrentArrowPos.x < scale.x * 50.0f) &&
                (vectorToOldArrowPos.y > 0.0f && vectorToOldArrowPos.y < scale.y * 100.0f) &&
                (vectorToCurrentArrowPos.y > 0.0f && vectorToCurrentArrowPos.y < scale.y * 100.0f)
            );

            // Arrow is within X,Y,Z bounds in the current frame
            bool currentFrameDeleteCondition = (
                (vectorToCurrentArrowPos.x > -scale.x * 50.0f && vectorToCurrentArrowPos.x < scale.x * 50.0f) &&
                (vectorToCurrentArrowPos.y > 0.0f && vectorToCurrentArrowPos.y < scale.y * 100.0f) &&
                (vectorToCurrentArrowPos.z > -scale.z * 50.0f && vectorToCurrentArrowPos.z < scale.z * 50.0f)
            );

            if(multiFrameDeleteCondition || currentFrameDeleteCondition)
                fopAcM_delete(arrow);
        }
    }
}

void daObjWaterFall_c::initBaseMtx() {
    setBaseMtx();
}

void daObjWaterFall_c::setBaseMtx() {
    mDoMtx_stack_c::transS(current.pos.x, current.pos.y, current.pos.z);
    mDoMtx_stack_c::ZXYrotM(shape_angle.x, shape_angle.y, shape_angle.z);
}

cPhs_Step daObjWaterFall_c::Create() {
    initBaseMtx();

    mCylColliderStts.Init(0, 0xFF, this);
    mCylCollider.Set(l_cyl_src);
    mCylCollider.SetStts(&mCylColliderStts);
    mCylCollider.SetH(scale.y * 100.0f);

    if(scale.x < scale.z)
        mCylCollider.SetR(scale.x * 50.0f);
    else
        mCylCollider.SetR(scale.z * 50.0f);

    mCylCollider.OnCoNoCamHit();

    //  If a waterfall process is not a circular cylinder (i.e. scale.x != scale.z), the circular cylindrical collider
    //      is oscillated in order to approximate an elliptical shape
    //
    //  speedF is used to control the speed of oscillation
    //
    //  This effect can be observed at the largest waterfall in Zora's Domain
    if(scale.x > scale.z) {
        mColOscPosTargets[0].set((scale.x * -50.0f) + (scale.z * 50.0f), 0.0f,0.0f);
        mColOscPosTargets[1].set((scale.x * 50.0f) - (scale.z * 50.0f), 0.0f,0.0f);
        speedF = ((scale.x * 100.0f) - (scale.z * 100.0f)) / 10.0f;
    }
    else {
        // Oscillation position targets are <0, 0, 0> if scale.x == scale.z
        mColOscPosTargets[0].set(0.0f, 0.0f,(scale.z * -50.0f) + (scale.x * 50.0f));
        mColOscPosTargets[1].set(0.0f, 0.0f,(scale.z * 50.0f) - (scale.x * 50.0f));
        speedF = ((scale.z * 100.0f) - (scale.x * 100.0f)) / 10.0f;
    }

    mDoMtx_stack_c::transS(home.pos);
    mDoMtx_stack_c::YrotM(home.angle.y);

    mDoMtx_stack_c::multVec(&mColOscPosTargets[0], &mColOscPosTargets[0]);
    mDoMtx_stack_c::multVec(&mColOscPosTargets[1], &mColOscPosTargets[1]);

    mColCenter = mColOscPosTargets[0];
    mColOscDir = 1; // Move collider toward mColOscPosTargets[1]

    return cPhs_LOADING_e;
}

cPhs_Step daObjWaterFall_c::create() {
    fopAcM_ct(this, daObjWaterFall_c);

    if(Create() == cPhs_INIT_e) {
        return cPhs_ERROR_e;
    }

    #if DEBUG
    l_HIO.entryHIO("滝"); // "Waterfall"
    #endif

    return cPhs_COMPLEATE_e;
}

int daObjWaterFall_c::execute() {
    if(getSwbit() != 0xFF && !fopAcM_isSwitch(this, getSwbit()))
        return 1;

    push_player();
    search_bomb();
    search_arrow();

    if(mColOscDir > 0) {
        if(cLib_chasePosXZ(&mColCenter, mColOscPosTargets[1], speedF))
            mColOscDir = -1; // Move collider toward mColOscPosTargets[0]
    }
    else if(cLib_chasePosXZ(&mColCenter, mColOscPosTargets[0], speedF)) {
        mColOscDir = 1; // Move collider toward mColOscPosTargets[1]
    }

    mCylCollider.SetC(mColCenter);
    dComIfG_Ccsp()->Set(&mCylCollider);

    return 1;
}

void daObjWaterFall_c::push_player() {
    daPy_py_c* const player = daPy_getPlayerActorClass();

    #if DEBUG
    f32 outMagnitude = l_HIO.mPushStrengthHuman;
    #else
    f32 outMagnitude = 10.0f;
    #endif

    if(player->checkNowWolf()) {
        #if DEBUG
        outMagnitude = l_HIO.mPushStrengthWolf;
        #else
        outMagnitude = 25.0f;
        #endif
    }

    if(scale.x == scale.z) {
        // Waterfall is a circular cylinder
        if(fopAcM_searchPlayerDistanceXZ(this) < scale.x * 50.0f) {
            cXyz vectorToPlayer = player->current.pos - current.pos;
            player->setOutPower(outMagnitude, cM_atan2s(vectorToPlayer.x, vectorToPlayer.z) , FALSE);
        }
    }
    else {
        // Waterfall is an elliptic cylinder
        cXyz vectorToPlayer = player->current.pos - current.pos;

        mDoMtx_stack_c::YrotS(-current.angle.y);
        mDoMtx_stack_c::multVec(&vectorToPlayer, &vectorToPlayer);

        if((vectorToPlayer.x > -scale.x * 50.0f && vectorToPlayer.x < scale.x * 50.0f) &&
        vectorToPlayer.y > -10.0f && vectorToPlayer.y < scale.y * 100.0f) {
            if(vectorToPlayer.z >= scale.z * 10.0f && vectorToPlayer.z < scale.z * 60.0f) {
                player->setOutPower(outMagnitude, current.angle.y, FALSE);
            }
            else if(vectorToPlayer.z < scale.z * -10.0f && vectorToPlayer.z > scale.z * -60.0f) {
                 player->setOutPower(outMagnitude, (current.angle.y + (1 << 16)) - 0x8000, FALSE);
            }
        }
    }
}

int daObjWaterFall_c::draw() {
    return 1;
}

int daObjWaterFall_c::_delete() {
    dComIfG_resDelete(&mPhase, l_arcName);

    #if DEBUG
    l_HIO.removeHIO();
    #endif

    return 1;
}

static int daObjWaterFall_Draw(daObjWaterFall_c* i_this) {
    return i_this->draw();
}

static int daObjWaterFall_Execute(daObjWaterFall_c* i_this) {
    return i_this->execute();
}

static int daObjWaterFall_Delete(daObjWaterFall_c* i_this) {
    fopAcM_RegisterDeleteID(i_this, "WaterFall");
    return i_this->_delete();
}

static int daObjWaterFall_Create(fopAc_ac_c* i_this) {
    daObjWaterFall_c* const actor = static_cast<daObjWaterFall_c*>(i_this);
    fopAcM_RegisterCreateID(i_this, "WaterFall");
    return actor->create();
}

static actor_method_class l_daObjWaterFall_Method = {
    (process_method_func)daObjWaterFall_Create,
    (process_method_func)daObjWaterFall_Delete,
    (process_method_func)daObjWaterFall_Execute,
    NULL,
    (process_method_func)daObjWaterFall_Draw,
};

actor_process_profile_definition g_profile_Obj_WaterFall = {
  fpcLy_CURRENT_e,          // mLayerID
  7,                        // mListID
  fpcPi_CURRENT_e,          // mListPrio
  PROC_Obj_WaterFall,       // mProcName
  &g_fpcLf_Method.base,    // sub_method
  sizeof(daObjWaterFall_c), // mSize
  0,                        // mSizeOther
  0,                        // mParameters
  &g_fopAc_Method.base,     // sub_method
  543,                      // mPriority
  &l_daObjWaterFall_Method, // sub_method
  0x00040100,               // mStatus
  fopAc_ACTOR_e,            // mActorType
  fopAc_CULLBOX_CUSTOM_e,   // cullType
};
