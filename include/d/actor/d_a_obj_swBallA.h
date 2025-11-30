#ifndef D_A_OBJ_SWBALLA_H
#define D_A_OBJ_SWBALLA_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_event_lib.h"

// dEvLib_callback_c is not generated at the correct location.
/**
 * @ingroup actors-objects
 * @class daObjSwBallA_c
 * @brief Ball Switch A
 *
 * @details
 *
 */
class daObjSwBallA_c : public fopAc_ac_c , public dEvLib_callback_c {
public:
    daObjSwBallA_c() : dEvLib_callback_c(this) {}

    int checkArea_sub(fopAc_ac_c*);
    void search_ball();
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create();
    int execute();
    void actionRun();
    void actionStop();
    void PutCrrPos();
    int draw();
    int _delete();
    virtual ~daObjSwBallA_c() {}

    u32 getArg0() { return fopAcM_GetParamBit(this, 0, 8); }
    u32 getSwbit2() { return fopAcM_GetParamBit(this, 8, 8); }
    u32 getSwbit() { return fopAcM_GetParamBit(this, 16, 8); }
    u32 getType() { return fopAcM_GetParamBit(this, 24, 4); }
    u8 getEvent() { return field_0x5c6; }
    void clrFlag(u8 idx) { field_0x5af[idx] = 0; }
    void onFlag(u8 idx, u8 val) { field_0x5af[idx] |= val; }
    u8 checkFlag(u8 idx, u8 val) { return field_0x5af[idx] & val; }

private:
    /* 0x57C */ request_of_phase_process_class field_0x57c;
    /* 0x584 */ J3DModel* mModel;
    /* 0x588 */ mDoExt_btkAnm* field_0x588;
    /* 0x58C */ u32 field_0x58c[8];
    /* 0x5AC */ u8 field_0x5ac;
    /* 0x5AD */ u8 field_0x5ad;
    /* 0x5AE */ u8 field_0x5ae;
    /* 0x5AF */ u8 field_0x5af[8];
    /* 0x5B7 */ u8 field_0x5b7[8];
    /* 0x5C0 */ u32 field_0x5c0;
    /* 0x5C4 */ u8 field_0x5c4;
    /* 0x5C5 */ u8 field_0x5c5;
    /* 0x5C6 */ u16 field_0x5c6;
    /* 0x5C8 */ u16 field_0x5c8;
};

// STATIC_ASSERT(sizeof(daObjSwBallA_c) == 0x5CC);

#endif /* D_A_OBJ_SWBALLA_H */
