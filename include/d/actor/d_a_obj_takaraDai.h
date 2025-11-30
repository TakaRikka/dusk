#ifndef D_A_OBJ_TAKARADAI_H
#define D_A_OBJ_TAKARADAI_H

#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daTkraDai_c
 * @brief Flight-by-Fowl Platform
 *
 * @details
 *
 */
class daTkraDai_c : public dBgS_MoveBgActor {
public:
    void setBaseMtx();
    int CreateHeap();
    int create();
    int Execute(Mtx**);
    void procMain();
    void init_modeWait();
    void modeWait();
    int Draw();
    int Delete();

    u32 getSwBit() { return fopAcM_GetParamBit(this, 0, 8); }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpBaseModel;
    /* 0x5AC */ J3DModel* mpTopModel;
    /* 0x5B0 */ u8 mMode;
    /* 0x5B1 */ u8 field_0x5b1[0x5d0 - 0x5b1];
    /* 0x5D0 */ s32 field_0x5d0;
    /* 0x5D4 */ csXyz field_0x5d4;
    /* 0x5DA */ u8 field_0x5da[0x5dc - 0x5da];
    /* 0x5DC */ f32 field_0x5dc;
    /* 0x5E0 */ f32 field_0x5e0;
    /* 0x5E4 */ f32 field_0x5e4;
    /* 0x5E8 */ f32 field_0x5e8;
    /* 0x5EC */ f32 field_0x5ec;
    /* 0x5F0 */ f32 field_0x5f0;
    /* 0x5F4 */ f32 field_0x5f4;
    /* 0x5F8 */ f32 field_0x5f8;
    /* 0x5FC */ f32 field_0x5fc;
    /* 0x600 */ f32 field_0x600;
    /* 0x604 */ f32 field_0x604;
    /* 0x608 */ f32 field_0x608;
    /* 0x60C */ f32 field_0x60c;
    /* 0x610 */ f32 field_0x610;
    /* 0x614 */ f32 field_0x614;
    /* 0x618 */ u8 field_0x618;
    /* 0x619 */ u8 field_0x619[0x61a - 0x619];
    /* 0x61A */ s16 field_0x61a;
    /* 0x61C */ cXyz field_0x61c;
    /* 0x628 */ dBgW* mpBgW;
    /* 0x62C */ Mtx field_0x62c;
};

STATIC_ASSERT(sizeof(daTkraDai_c) == 0x65c);

class daTkraDai_HIO_c : public mDoHIO_entry_c {
public:
    daTkraDai_HIO_c();
    virtual ~daTkraDai_HIO_c() {}
    void genMessage(JORMContext *);

    /* 0x04 */ f32 field_0x04;
    /* 0x08 */ f32 field_0x08;
    /* 0x0C */ f32 field_0x0c;
    /* 0x10 */ f32 field_0x10;
    /* 0x14 */ f32 field_0x14;
    /* 0x18 */ f32 field_0x18;
    /* 0x1C */ f32 field_0x1c;
    /* 0x20 */ f32 field_0x20;
    /* 0x24 */ f32 field_0x24;
    /* 0x28 */ f32 field_0x28;
    /* 0x2C */ f32 field_0x2c;
    /* 0x30 */ f32 field_0x30;
    /* 0x34 */ f32 field_0x34;
    /* 0x38 */ u8 field_0x38;
    /* 0x39 */ u8 field_0x39[0x3c - 0x39];
};

STATIC_ASSERT(sizeof(daTkraDai_HIO_c) == 0x3c);

#endif /* D_A_OBJ_TAKARADAI_H */
