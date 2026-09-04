#ifndef D_A_NPC_HENNA0_H
#define D_A_NPC_HENNA0_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class d_a_npc_henna0
 * @brief Henna 0 (unused?)
 *
 * @details
 *
 */
class d_a_npc_henna0 : public fopAc_ac_c {
    public:
        /* 0x568 */ u8 field_0x568[0x828 - 0x568];
};

STATIC_ASSERT(sizeof(d_a_npc_henna0) == 0x828);

#endif /* D_A_NPC_HENNA0_H */
