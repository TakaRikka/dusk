#ifndef D_A_TBOXSW_H
#define D_A_TBOXSW_H

#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-unsorted
 * @class daTboxSw_c
 * @brief Treasure Box Switch
 *
 * @details
 *
 */
class daTboxSw_c : public fopAc_ac_c {
public:
    int Create();
    int create();
    int execute();
    int draw();
    int _delete();
};

STATIC_ASSERT(sizeof(daTboxSw_c) == 0x568);

namespace daTboxSw_prm {
    inline u8 getTboxNo(daTboxSw_c* p_tbox) { return fopAcM_GetParam(p_tbox) & 0x3F; }
    inline u8 getSwNo(daTboxSw_c* p_tbox) { return fopAcM_GetParam(p_tbox) >> 8; }
};

#endif /* D_A_TBOXSW_H */
