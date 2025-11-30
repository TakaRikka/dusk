#ifndef D_A_OBJ_IKADA_H
#define D_A_OBJ_IKADA_H

#include "d/d_bg_s_acch.h"
#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"

/**
 * @ingroup actors-objects
 * @class daObjIkada_c
 * @brief Raft
 *
 * @details
 *
*/

class daObjIkada_c : public dBgS_MoveBgActor {
public:
    void RideOn_Angle(s16&, f32, s16, f32);
    BOOL Check_RideOn();
    void initBaseMtx();
    void setBaseMtx();
    inline int CreateHeap();
    inline int Create();
    inline int Execute(Mtx**);
    inline int Draw();
    inline int Delete();

    inline cPhs__Step create();

    /* 0x5A0 */ f32 field_0x5a0;
    /* 0x5A4 */ f32 field_0x5a4;
    /* 0x5A8 */ u8 field_0x5a8[0x5ac - 0x5a8];
    /* 0x5AC */ s16 field_0x5ac;
    /* 0x5AE */ u8 field_0x5ae[0x5bc - 0x5ae];
    /* 0x5BC */ csXyz field_0x5bc;
    /* 0x5C2 */ u8 field_0x5c2[0x5c4 - 0x5c2];
    /* 0x5C4 */ s16 field_0x5c4;
    /* 0x5C6 */ s16 field_0x5c6;
    /* 0x5C8 */ csXyz field_0x5c8;
    /* 0x5D0 */ f32 field_0x5d0;
    /* 0x5D4 */ s16 field_0x5d4;
    /* 0x5D6 */ u8 field_0x5d6;
    /* 0x5D8 */ cXyz field_0x5d8;
    /* 0x5E4 */ dCcD_Stts mStts;
    /* 0x620 */ J3DModel* mpModel;
    /* 0x624 */ request_of_phase_process_class mPhase;
    /* 0x62C */ dBgS_AcchCir mAcchCir;
    /* 0x66C */ dBgS_ObjAcch mBgc;
};

STATIC_ASSERT(sizeof(daObjIkada_c) == 0x844);


#endif /* D_A_OBJ_IKADA_H */
