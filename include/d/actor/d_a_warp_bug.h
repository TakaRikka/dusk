#ifndef D_A_WARP_BUG_H
#define D_A_WARP_BUG_H

#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-unsorted
 * @class daWarpBug_c
 * @brief Warp Bug (unused)
 *
 * @details
 *
 */
class daWarpBug_c : public fopAc_ac_c {
public:
    inline ~daWarpBug_c();

    void create_init();
    int draw();

    inline int createHeap();
    inline int execute();
    inline int create();

    /* 0x568 */ J3DModel* mpModel;
    /* 0x56C */ mDoExt_brkAnm* mpBrk;
    /* 0x570 */ mDoExt_btkAnm* mpBtk;
    /* 0x574 */ request_of_phase_process_class mPhase;
    /* 0x57C */ s16 field_0x57c;
    /* 0x57E */ u8 field_0x57e;
};

STATIC_ASSERT(sizeof(daWarpBug_c) == 0x580);

#endif /* D_A_WARP_BUG_H */
