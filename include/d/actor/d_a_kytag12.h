#ifndef D_A_KYTAG12_H
#define D_A_KYTAG12_H

#include <dolphin/types.h>
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-tags
 * @class kytag12_class
 * @brief Palace of Twilight - Dark Fog Tag
 *
 * @details
 *
 */
class kytag12_class : public fopAc_ac_c {
public:
    /* 0x568 */ u8 field_0x568[0x574 - 0x568];
    /* 0x574 */ u8 field_0x574;
    /* 0x575 */ u8 field_0x575;
    /* 0x576 */ u16 field_0x576;
    /* 0x578 */ s16 field_0x578;
    /* 0x57C */ cXyz field_0x57c[5];
};

STATIC_ASSERT(sizeof(kytag12_class) == 0x5B8);


#endif /* D_A_KYTAG12_H */
