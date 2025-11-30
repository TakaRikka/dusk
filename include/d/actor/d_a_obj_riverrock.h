#ifndef D_A_OBJ_RIVERROCK_H
#define D_A_OBJ_RIVERROCK_H

#include "d/d_bg_s_movebg_actor.h"
#include "d/d_cc_d.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjRIVERROCK_c
 * @brief (Zora?) River Rock
 *
 * @details
 *
 */
class daObjRIVERROCK_c : public dBgS_MoveBgActor {
public:
    typedef enum {
        ACTION_WAIT,
        ACTION_BREAK,
    } ActionEnum;

    typedef enum {
        BREAK_WAIT,
        BREAK_MOVE,
        BREAK_BROKEN,
        BREAK_3,
    } BreakSubAction;

    void initCcCylinder();
    void SetCcCyl();
    void BreakWaitAction();
    void CheckBG();
    void BreakMoveAction();
    void BrokenAction();
    void BreakAction();
    void Action();
    void BreakSet();
    void WaitAction();
    void setBaseMtx();
    int CreateHeap();
    int create();
    int Create();
    int Execute(Mtx**);
    int Draw();
    int Delete();

    /* 0x5A0 */ ActionEnum mAction;
    /* 0x5A4 */ BreakSubAction mBreakSubAction;
    /* 0x5A8 */ u8 field_0x5a8;
    /* 0x5A9 */ u8 field_0x5a9;
    /* 0x5AA */ s16 field_0x5aa;
    /* 0x5AC */ int field_0x5ac;
    /* 0x5B0 */ s16 field_0x5b0;
    /* 0x5B2 */ s16 field_0x5b2;
    /* 0x5B4 */ s16 field_0x5b4;
    /* 0x5B6 */ s16 field_0x5b6;
    /* 0x5B8 */ s16 field_0x5b8;
    /* 0x5BA */ u8 field_0x5ba[0x5c4 - 0x5ba];
    /* 0x5C4 */ cXyz field_0x5c4;
    /* 0x5D0 */ cXyz field_0x5d0;
    /* 0x5DC */ cXyz field_0x5dc;
    /* 0x5E8 */ csXyz field_0x5e8;
    /* 0x5EE */ bool field_0x5ee;
    /* 0x5F0 */ Z2SoundObjSimple mSoundObj;
    /* 0x610 */ int field_0x610;
    /* 0x614 */ J3DModel* mModel;
    /* 0x618 */ request_of_phase_process_class mPhase;
    /* 0x620 */ dCcD_Stts mStts;
    /* 0x65C */ dCcD_Cyl mCyl;
    /* 0x798 */ bool mHIONeedDelete;
};

STATIC_ASSERT(sizeof(daObjRIVERROCK_c) == 0x79c);

class daObjRIVERROCK_HIO_c {
public:
    daObjRIVERROCK_HIO_c();
    virtual ~daObjRIVERROCK_HIO_c() {}

    /* 0x4 */ s8 field_0x4;
    /* 0x8 */ f32 field_0x8;
    /* 0xC */ f32 field_0xc;
};


#endif /* D_A_OBJ_RIVERROCK_H */
