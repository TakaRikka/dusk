#ifndef D_A_TAG_HJUMP_H
#define D_A_TAG_HJUMP_H

#include "d/d_bg_s_movebg_actor.h"
#include "f_op/f_op_actor_mng.h"

class daTagHjump_c : public dBgS_MoveBgActor {
public:
    enum Type_e {
        TYPE_TRIGGER_e = 0,
        TYPE_OBJECT_e = 3,
    };

    int create();
    ~daTagHjump_c();
    int execute();

    virtual int CreateHeap();
    virtual int Draw();

    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mpModel;
    /* 0x5AC */ u8 mType;
    /* 0x5AD */ u8 field_0x5ad;
    /* 0x5AE */ s16 field_0x5ae;
    /* 0x5B0 */ u16 mOnFlagID;
    /* 0x5B2 */ u16 mOffFlagID;
    /* 0x5B4 */ f32 field_0x5b4;
    /* 0x5B8 */ f32 field_0x5b8;
};

#endif /* D_A_TAG_HJUMP_H */
