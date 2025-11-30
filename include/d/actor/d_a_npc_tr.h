#ifndef D_A_NPC_TR_H
#define D_A_NPC_TR_H

#include "SSystem/SComponent/c_phase.h"
#include "f_op/f_op_actor.h"

/**
 * @ingroup actors-npcs
 * @class npc_tr_class
 * @brief Trout
 *
 * @details
 *
 */
class npc_tr_class : public fopAc_ac_c {
public:
    /* 0x568 */ u8 field_0x568[0x5AC - 0x568];
    /* 0x5AC */ request_of_phase_process_class mPhaseReq;
    /* 0x5B4 */ u8 field_0x5b4;
    /* 0x5B8 */ J3DModel* field_0x5b8;
    /* 0x5BC */ s16 field_0x5bc;
    /* 0x5BE */ s16 field_0x5be;
    /* 0x5C0 */ s16 field_0x5c0;
    /* 0x5C4 */ cXyz field_0x5c4;
    /* 0x5D0 */ u8 field_0x5d0[0x8];
    /* 0x5D8 */ f32 field_0x5d8;
    /* 0x5DC */ s16 field_0x5dc[4];
    /* 0x5E4 */ f32 field_0x5e4;
    /* 0x5E8 */ f32 field_0x5e8;
    /* 0x5EC */ s16 field_0x5ec;
    /* 0x5EE */ s16 field_0x5ee;
    /* 0x5F0 */ s16 field_0x5f0;
    /* 0x5F2 */ s16 field_0x5f2[3];
    /* 0x5F8 */ f32 field_0x5f8;
    /* 0x5FC */ f32 field_0x5fc;
    /* 0x600 */ u8 field_0x600;
};

class daNPC_TR_HIO_c {
public:
    daNPC_TR_HIO_c();
    virtual ~daNPC_TR_HIO_c();

    /* 0x04 */ s8 field_0x4;
    /* 0x08 */ f32 field_0x8;
    /* 0x0C */ f32 field_0xc;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
};

#endif /* D_A_NPC_TR_H */
