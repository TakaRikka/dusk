#ifndef D_A_D_A_ITEMBASE_STATIC_H
#define D_A_D_A_ITEMBASE_STATIC_H

#include "dolphin/types.h"

class fopAc_ac_c;

int CheckFieldItemCreateHeap(fopAc_ac_c* actor);
int CheckItemCreateHeap(fopAc_ac_c* i_this);

#if TARGET_PC
const char* dItem_fieldModelArc(u8 itemNo);
#endif

#endif /* D_A_D_A_ITEMBASE_STATIC_H */
