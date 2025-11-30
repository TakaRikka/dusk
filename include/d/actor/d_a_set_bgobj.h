#ifndef D_A_SET_BGOBJ_H
#define D_A_SET_BGOBJ_H

#include "d/d_com_inf_game.h"

/**
 * @ingroup actors-unsorted
 * @class daSetBgObj_c
 * @brief Set Background Object
 *
 * @details
 *
 */
class daSetBgObj_c : public fopAc_ac_c {
public:
    static const char* getArcName(fopAc_ac_c*);
    int CreateInit();
    int create();
    int Delete();

    /* 0x568 */ request_of_phase_process_class mPhase;
    /* 0x570 */ char mArcName[16];
};

#endif /* D_A_SET_BGOBJ_H */
