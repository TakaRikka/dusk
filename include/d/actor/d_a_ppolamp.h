#ifndef D_A_PPOLAMP_H
#define D_A_PPOLAMP_H

#include "dolphin/types.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-unsorted
 * @class daPPolamp_c
 * @brief P Poe Lamp
 *
 * @details
 *
 */
class daPPolamp_c : public fopAc_ac_c, public request_of_phase_process_class {
public:
    virtual ~daPPolamp_c();
    int create();
    int execute();
    int draw();
    int Delete();
    void setModelMtx();
    void setPclModelMtx();
    int createHeap();
    void moveSwing();
    void initParam();

private:
    /* 0x574 */ J3DModel* mModel1;
    /* 0x578 */ J3DModel* mModel2;
    /* 0x57C */ mDoExt_bckAnm mBck;
    /* 0x598 */ int field_0x598;
    /* 0x59C */ s8 field_0x59c;
    /* 0x59E */ s16 field_0x59e;
    /* 0x5A0 */ u8 mSwingTimer;
    /* 0x5A4 */ f32 field_0x5a4;
    /* 0x5A8 */ f32 field_0x5a8;
};

STATIC_ASSERT(sizeof(daPPolamp_c) == 0x5ac);


#endif /* D_A_PPOLAMP_H */
