#ifndef D_A_OBJ_CRYSTAL_H
#define D_A_OBJ_CRYSTAL_H

#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjCrystal_c
 * @brief Crystal Switch
 *
 * @details
 *
 */
class daObjCrystal_c : public fopAc_ac_c {
public:
    void initBaseMtx();
    void setBaseMtx();
    int Create();
    int CreateHeap();
    int create();
    int execute();
    int draw();
    int _delete();

private:
    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ J3DModel* mpModel[2];
    /* 0x578 */ JPABaseEmitter* mpEmitter;
    /* 0x57C */ u16 mJointID;
};

STATIC_ASSERT(sizeof(daObjCrystal_c) == 0x580);

#endif /* D_A_OBJ_CRYSTAL_H */
