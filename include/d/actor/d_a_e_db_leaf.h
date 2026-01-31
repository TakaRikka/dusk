#ifndef D_A_E_DB_LEAF_H
#define D_A_E_DB_LEAF_H

#include "d/d_com_inf_game.h"
#include <dolphin/types.h>

/**
 * @ingroup actors-enemies
 * @class e_db_leaf_class
 * @brief Deku Baba - Leaf
 * 
 * @details 
 * 
 */
class e_db_leaf_class : public fopEn_enemy_c {
public:
    /* 0x5AC */ request_of_phase_process_class mPhase;
    /* 0x5B4 */ mDoExt_McaMorf* mpMorf;
}; // Size: 0x5B8

#endif /* D_A_E_DB_LEAF_H */
