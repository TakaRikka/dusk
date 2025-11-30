#ifndef D_A_OBJ_CRVHAHEN_H
#define D_A_OBJ_CRVHAHEN_H

#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"
#include "m_Do/m_Do_ext.h"
#include "m_Do/m_Do_lib.h"

/**
 * @ingroup actors-objects
 * @class daObjCRVHAHEN_c
 * @brief Bulblin Camp Caraven Wooden Fence Fragments
 *
 * @details Hahen (はへん) a fragment, shard, or splinter.
 * physics-enabled shards or fragments that simulate debris from destruction.
 *
 */
class daObjCRVHAHEN_c : public fopAc_ac_c {
public:
    int HahenSet(cXyz, cXyz, cXyz, cXyz, f32);
    bool Wall_Check(cXyz, cXyz);
    void Hahen_Hakai(int, int);
    void CheckCull();
    bool checkViewArea(cXyz*);
    int Execute();
    int Delete();
    void setBaseMtx();
    int create();

    int CreateHeap();
    int Draw();

    /* 0x568 */ cXyz mPos[10];
    /* 0x5e0 */ cXyz mVelocity[10];
    /* 0x658 */ cXyz mInitialPos[10];
    /* 0x6d0 */ csXyz mRotation[10];
    /* 0x70c */ csXyz mRotSpeed[10];
    /* 0x748 */ u32 mFlags;
    /* 0x74c */ f32 mGroundHeight;
    /* 0x750 */ bool mDrawHahen;
    /* 0x751 */ u8 mStatus[10];
    /* 0x75b */ u8 field_0x75b;
    /* 0x75c */ f32 mTimer;
    /* 0x760 */ J3DModel* mpModel[10];
    /* 0x788 */ request_of_phase_process_class mPhase;
    /* 0x790 */ u8 mEffectState;
    /* 0x791 */ bool mInitialized;
    /* 0x792 */ u8 mCounter;
    /* 0x793 */ u8 mDrawState;
};

STATIC_ASSERT(sizeof(daObjCRVHAHEN_c) == 0x794);

#endif /* D_A_OBJ_CRVHAHEN_H */
