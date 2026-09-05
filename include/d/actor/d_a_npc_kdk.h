#ifndef D_A_NPC_KDK_H
#define D_A_NPC_KDK_H

#include "d/actor/d_a_npc.h"

/**
 * @ingroup actors-npcs
 * @class daNpcKdk_c
 * @brief Temporary Cutscene Guy?
 *
 * @details
 *
 */
class daNpcKdk_c : public fopAc_ac_c {
public:
    /* 0x568 */ u8 field_0x568[0xFA0 - 0x568];
};

STATIC_ASSERT(sizeof(daNpcKdk_c) == 0xFA0);

#endif /* D_A_NPC_KDK_H */
