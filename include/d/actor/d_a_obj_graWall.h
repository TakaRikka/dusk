#ifndef D_A_OBJ_GRAWALL_H
#define D_A_OBJ_GRAWALL_H

#include "d/d_com_inf_game.h"
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-objects
 * @class daObjGraWall_c
 * @brief Goron Wall?
 *
 * @details
 *
 */
class daObjGraWall_c : public fopAc_ac_c {
public:
    int Create();
    int Execute();
    int Delete();
    void col_init();
    void col_set();

private:
    /* 0x568 */ dCcD_Stts mStts;
    /* 0x5A4 */ dCcD_Cyl mCyl;
    /* 0x6E0 */ u8 field_0x6e0;
};

STATIC_ASSERT(sizeof(daObjGraWall_c) == 0x6E4);

#endif /* D_A_OBJ_GRAWALL_H */
