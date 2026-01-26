
#ifndef F_PC_DRAW_PRIORITY_H_
#define F_PC_DRAW_PRIORITY_H_

#include <dolphin/types.h>

typedef struct draw_priority_class {
    s16 priority;
} draw_priority_class;

s16 fpcDwPi_Get(const draw_priority_class* i_drawpriority);
void fpcDwPi_Set(draw_priority_class* i_drawpriority, s16 i_priority);
void fpcDwPi_Init(draw_priority_class* i_drawpriority, s16 i_priority);

#endif
