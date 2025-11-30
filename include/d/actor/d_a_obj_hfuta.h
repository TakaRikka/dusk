#ifndef D_A_OBJ_HFUTA_H
#define D_A_OBJ_HFUTA_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_bg_s_movebg_actor.h"

/**
 * @ingroup actors-objects
 * @class daObjFuta_c
 * @brief Crawling?
 *
 * @details
 *
 */
class daObjFuta_c : public dBgS_MoveBgActor {
public:
    enum Mode {
        MODE_0,
        MODE_1,
        MODE_2,
    };

    typedef void (daObjFuta_c::*modeFunc)();

    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create1st();
    int Execute(Mtx**);
    void mode_proc_call();
    void mode_wait();
    void mode_move();
    void mode_end();
    int Draw();
    int Delete();

    void setMode(u8 mode) { mMode = mode; }

private:
    /* 0x5A0 */ request_of_phase_process_class mPhase;
    /* 0x5A8 */ J3DModel* mModel;
    /* 0x5AC */ cXyz mTransM;
    /* 0x5B8 */ csXyz mZXYRotationM;
    /* 0x5BE */ u8 mMode;
    /* 0x5BF */ u8 field_0x5bf;
    /* 0x5C0 */ int field_0x5c0;
    /* 0x5C4 */ f32 field_0x5c4;
    /* 0x5C8 */ u8 field_0x5c8;
    /* 0x5CA */ u16 mRotationIndex;
};

//STATIC_ASSERT(sizeof(daObjFuta_c) == 0x5cc);


#endif /* D_A_OBJ_HFUTA_H */
