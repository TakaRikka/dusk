#ifndef D_A_OBJ_ITA_H
#define D_A_OBJ_ITA_H

#include "d/d_bg_s_acch.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daObjIta_c
 * @brief Plank
 *
 * @details
 *
*/

class daObjIta_c : public dBgS_MoveBgActor {
public:
    void Search_Ymb();
    BOOL Check_RideOn();
    void initBaseMtx();
    void setBaseMtx();
    inline cPhs__Step create();
    inline int CreateHeap();
    inline int Create();
    inline int Execute(Mtx**);
    inline int Draw();
    inline int Delete();
    
    f32 dummy2();

    /* 0x5A0 */ f32 field_0x5a0;
    /* 0x5A4 */ f32 field_0x5a4;
    /* 0x5A8 */ f32 field_0x5a8;
    /* 0x5AC */ f32 field_0x5ac;
    /* 0x5B0 */ s16 field_0x5b0;
    /* 0x5B2 */ u8 field_0x5b2[0x5c0 - 0x5b2];
    /* 0x5C0 */ csXyz field_0x5c0;
    /* 0x5C6 */ u8 field_0x5c6[0x5c8 - 0x5c6];
    /* 0x5C8 */ s16 field_0x5c8;
    /* 0x5CA */ s16 field_0x5ca;
    /* 0x5CC */ s16 field_0x5cc;
    /* 0x5CE */ csXyz field_0x5ce;
    /* 0x5D4 */ f32 field_0x5d4;
    /* 0x5D8 */ cXyz field_0x5d8;
    /* 0x5E4 */ u8 field_0x5e4;
    /* 0x5E5 */ u8 field_0x5e5;
    /* 0x5E6 */ s16 field_0x5e6;
    /* 0x5E8 */ dCcD_Stts mStts;
    /* 0x624 */ J3DModel* mpModel;
    /* 0x628 */ Z2Creature mSound;
    /* 0x6B8 */ request_of_phase_process_class mPhase;
    /* 0x6C0 */ u8 field_0x6c0;
    /* 0x6C4 */ dBgS_AcchCir mAcchCir;
    /* 0x704 */ dBgS_ObjAcch mBgc;
};

STATIC_ASSERT(sizeof(daObjIta_c) == 0x8dc);

#endif /* D_A_OBJ_ITA_H */
