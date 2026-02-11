#ifndef D_A_KYTAG13_H
#define D_A_KYTAG13_H

#include <dolphin/types.h>
#include "f_op/f_op_actor_mng.h"

/**
 * @ingroup actors-tags
 * @class kytag13_class
 * @brief Blowing Snow Tag
 *
 * @details
 *
*/

class kytag13_class {
public:
    /* 0x000 */ fopAc_ac_c actor;
    /* 0x568 */ u8 field_0x568[0x574 - 0x568];
    /* 0x574 */ f32 mSpeed;
    /* 0x578 */ u16 mMax;
    /* 0x57A */ u8 field_0x57a;
    /* 0x57C */ u16 field_0x57c;
    /* 0x57E */ u16 field_0x57e;
};

STATIC_ASSERT(sizeof(kytag13_class) == 0x580);


#endif /* D_A_KYTAG13_H */
