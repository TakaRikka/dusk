#ifndef D_A_OBJ_MATO_H
#define D_A_OBJ_MATO_H

#include "f_op/f_op_actor_mng.h"
#include "d/d_cc_d.h"
#include "d/d_path.h"

class daObjMATO_c;

struct MatoData_s {
    /* 0x000 */ J3DModel* model;
    /* 0x004 */ dCcD_Stts ccStts;
    /* 0x040 */ dCcD_Cyl ccCyl;
    /* 0x17C */ cXyz pos;
    /* 0x188 */ cXyz scale;
    /* 0x194 */ csXyz rot;
    /* 0x19C */ void (daObjMATO_c::*action)(int);
    /* 0x1A8 */ void (daObjMATO_c::*prev_action)(int);
    /* 0x1B4 */ int mode;
    /* 0x1B8 */ s16 rot_speed;
    /* 0x1BA */ u8 type;
    /* 0x1BB */ u8 is_hit;
    /* 0x1BC */ u8 timer;
};

/**
 * @ingroup actors-objects
 * @class daObjMATO_c
 * @brief Flight by Fowl Rupee Target
 *
 * @details
 *
 */
class daObjMATO_c : public fopAc_ac_c {
public:
    void setAction(void (daObjMATO_c::*)(int), int);
    void action();
    void hit_check();
    void start_wait(int);
    void wait(int);
    void disappear(int);
    void getRupee(int);
    int execute();
    int _delete();
    void setBaseMtx();
    int mato_init();
    int create();

    inline int CreateHeap();
    inline int draw();

    /* 0x0568 */ request_of_phase_process_class mPhase;
    /* 0x0570 */ dPath* mpPath;
    /* 0x0574 */ MatoData_s mData[30];
    /* 0x39F4 */ u16 field_0x39f4;
    /* 0x39F6 */ u8 field_0x39f6;
    /* 0x39F7 */ u8 mIsAddSimpleModel;
};

STATIC_ASSERT(sizeof(daObjMATO_c) == 0x39f8);

#endif /* D_A_OBJ_MATO_H */
